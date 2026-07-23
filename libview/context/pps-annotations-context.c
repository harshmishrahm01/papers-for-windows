/* pps-annotations-context.c
 *  this file is part of papers, a gnome document viewer
 *
 * Copyright (C) 2024 Pablo Correa Gomez <ablocorrea@hotmail.com>
 *
 * Papers is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * Papers is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <papers-view.h>
#include <time.h>

#define ANNOT_POPUP_WINDOW_DEFAULT_WIDTH 200
#define ANNOT_POPUP_WINDOW_DEFAULT_HEIGHT 150
#define ANNOTATION_ICON_SIZE 24

enum {
	SIGNAL_ANNOT_ADDED,
	SIGNAL_ANNOT_REMOVED,
	SIGNAL_ANNOTS_LOADED,
	N_SIGNALS
};

static guint signals[N_SIGNALS];

enum {
	PROP_0,
	PROP_DOCUMENT_MODEL,
	PROP_UNDO_CONTEXT,
	NUM_PROPERTIES
};

typedef enum {
	PPS_ANNOTATIONS_MODIFIED,
	PPS_ANNOTATIONS_ADDED,
	PPS_ANNOTATIONS_REMOVED
} PpsAnnotationsContextUndoableAction;

typedef struct {
	PpsAnnotationsContextUndoableAction action;
	/* a union whose value depends on @type. */
	union {
		struct {
			/* list of changed properties, one for every item of @annots */
			GList *changes;
		} modified;
		struct {
			/* list of initial annots areas (as they may be modified by
			poppler when added to the doc for instance), one for every
			item of @annots */
			GList *areas;
		} added;
	};
	/* list of modified annotations */
	GList *annots;

	GDateTime *time;
} PpsAnnotationsContextUndoable;

typedef struct {
	const gchar *property;
	GValue value;
} PpsAnnotationChangedProperty;

typedef struct
{
	PpsDocumentModel *model;
	PpsJobAnnots *job;
	PpsUndoContext *undo_context;

	GListStore *annots_model;
	gboolean next_squash_forbidden;
} PpsAnnotationsContextPrivate;

static void
connect_notify_signals (PpsAnnotationsContext *self, PpsAnnotation *annot);

static void pps_annotations_context_undo_handler_init (PpsUndoHandlerInterface *iface);
G_DEFINE_TYPE_WITH_CODE (PpsAnnotationsContext, pps_annotations_context, G_TYPE_OBJECT, G_ADD_PRIVATE (PpsAnnotationsContext) G_IMPLEMENT_INTERFACE (PPS_TYPE_UNDO_HANDLER, pps_annotations_context_undo_handler_init))

#define GET_PRIVATE(o) pps_annotations_context_get_instance_private (o)

static GParamSpec *props[NUM_PROPERTIES] = {
	NULL,
};

static PpsInkTransformation ink_transformation = NULL;
static gpointer ink_transformation_data;

/**
 * pps_annotations_context_register_ink_transformation:
 * @transformation: (scope forever) (closure user_data): a #GCallback to call everytime a new ink annotation is created

 * This is a hack to be able to call rust code from C.
 */
void
pps_annotations_context_register_ink_transformation (PpsInkTransformation transformation, gpointer user_data)
{
	ink_transformation = transformation;
	ink_transformation_data = user_data;
}

static void
pps_annotations_context_clear_job (PpsAnnotationsContext *self)
{
	PpsAnnotationsContextPrivate *priv = GET_PRIVATE (self);

	if (!priv->job)
		return;

	/* this function may be a callback of the cancelled signal of priv->job,
	so we have to make sure it is disconnected before continuing before continuing */
	g_signal_handlers_disconnect_matched (priv->job, G_SIGNAL_MATCH_DATA, 0, 0, NULL, NULL, self);

	if (!pps_job_is_finished (PPS_JOB (priv->job)))
		pps_job_cancel (PPS_JOB (priv->job));

	g_clear_object (&priv->job);
}

static void
annotations_job_finished_cb (PpsJobAnnots *job,
                             PpsAnnotationsContext *self)
{
	PpsAnnotationsContextPrivate *priv = GET_PRIVATE (self);
	g_autoptr (GPtrArray) annotations_array = g_ptr_array_new ();
	gpointer *annotations;
	GList *annotations_list = pps_job_annots_get_annots (job);
	gsize n_annotations;

	for (GList *l = annotations_list; l && l->data; l = g_list_next (l)) {
		g_ptr_array_add (annotations_array, l->data);
		connect_notify_signals (self, PPS_ANNOTATION (l->data));
	}

	annotations = g_ptr_array_steal (annotations_array, &n_annotations);
	if (n_annotations > 0)
		g_list_store_splice (priv->annots_model, 0, 0, annotations, (guint) n_annotations);

	pps_annotations_context_clear_job (self);

	g_signal_emit (self, signals[SIGNAL_ANNOTS_LOADED], 0);
}

static void
pps_annotations_context_setup_document (PpsAnnotationsContext *self,
                                        PpsDocument *document)
{
	PpsAnnotationsContextPrivate *priv = GET_PRIVATE (self);

	g_list_store_remove_all (priv->annots_model);

	if (!PPS_IS_DOCUMENT_ANNOTATIONS (document))
		return;

	pps_annotations_context_clear_job (self);

	priv->job = PPS_JOB_ANNOTS (pps_job_annots_new (document));
	g_signal_connect (priv->job, "finished",
	                  G_CALLBACK (annotations_job_finished_cb),
	                  self);
	g_signal_connect_swapped (priv->job, "cancelled",
	                          G_CALLBACK (pps_annotations_context_clear_job),
	                          self);

	pps_job_scheduler_push_job (PPS_JOB (priv->job), PPS_JOB_PRIORITY_NONE);
}

static void
document_changed_cb (PpsDocumentModel *model,
                     GParamSpec *pspec,
                     PpsAnnotationsContext *self)
{
	pps_annotations_context_setup_document (self, pps_document_model_get_document (model));
}

static void
pps_annotations_context_init (PpsAnnotationsContext *self)
{
	PpsAnnotationsContextPrivate *priv = GET_PRIVATE (self);

	priv->annots_model = g_list_store_new (PPS_TYPE_ANNOTATION);
}

static void
pps_annotations_context_dispose (GObject *object)
{
	PpsAnnotationsContext *self = PPS_ANNOTATIONS_CONTEXT (object);
	PpsAnnotationsContextPrivate *priv = GET_PRIVATE (self);

	pps_annotations_context_clear_job (self);
	g_clear_object (&priv->annots_model);

	G_OBJECT_CLASS (pps_annotations_context_parent_class)->dispose (object);
}

static void
pps_annotations_context_set_property (GObject *object,
                                      guint prop_id,
                                      const GValue *value,
                                      GParamSpec *pspec)
{
	PpsAnnotationsContext *self = PPS_ANNOTATIONS_CONTEXT (object);
	PpsAnnotationsContextPrivate *priv = GET_PRIVATE (self);

	switch (prop_id) {
	case PROP_DOCUMENT_MODEL:
		priv->model = g_value_get_object (value);
		break;
	case PROP_UNDO_CONTEXT:
		priv->undo_context = g_value_get_object (value);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
	}
}

static void
pps_annotations_context_constructed (GObject *object)
{
	PpsAnnotationsContext *self = PPS_ANNOTATIONS_CONTEXT (object);
	PpsAnnotationsContextPrivate *priv = GET_PRIVATE (self);

	G_OBJECT_CLASS (pps_annotations_context_parent_class)->constructed (object);

	g_object_add_weak_pointer (G_OBJECT (priv->model),
	                           (gpointer) &priv->model);

	pps_annotations_context_setup_document (self, pps_document_model_get_document (priv->model));
	g_signal_connect_object (priv->model, "notify::document",
	                         G_CALLBACK (document_changed_cb),
	                         self, G_CONNECT_DEFAULT);
}

static void
pps_annotations_context_class_init (PpsAnnotationsContextClass *klass)
{
	GObjectClass *gobject_class = G_OBJECT_CLASS (klass);

	gobject_class->set_property = pps_annotations_context_set_property;
	gobject_class->dispose = pps_annotations_context_dispose;
	gobject_class->constructed = pps_annotations_context_constructed;

	props[PROP_DOCUMENT_MODEL] =
	    g_param_spec_object ("document-model",
	                         "DocumentModel",
	                         "The document model",
	                         PPS_TYPE_DOCUMENT_MODEL,
	                         G_PARAM_WRITABLE | G_PARAM_CONSTRUCT_ONLY | G_PARAM_STATIC_STRINGS);
	props[PROP_UNDO_CONTEXT] =
	    g_param_spec_object ("undo-context",
	                         "UndoContext",
	                         "The undo context",
	                         PPS_TYPE_UNDO_CONTEXT,
	                         G_PARAM_WRITABLE | G_PARAM_CONSTRUCT_ONLY | G_PARAM_STATIC_STRINGS);

	g_object_class_install_properties (gobject_class, NUM_PROPERTIES, props);

	signals[SIGNAL_ANNOT_ADDED] =
	    g_signal_new ("annot-added",
	                  G_TYPE_FROM_CLASS (gobject_class),
	                  G_SIGNAL_RUN_LAST,
	                  0, NULL, NULL,
	                  g_cclosure_marshal_generic,
	                  G_TYPE_NONE, 1,
	                  PPS_TYPE_ANNOTATION);

	signals[SIGNAL_ANNOT_REMOVED] =
	    g_signal_new ("annot-removed",
	                  G_TYPE_FROM_CLASS (gobject_class),
	                  G_SIGNAL_RUN_LAST,
	                  0, NULL, NULL,
	                  g_cclosure_marshal_generic,
	                  G_TYPE_NONE, 1,
	                  PPS_TYPE_ANNOTATION);

	signals[SIGNAL_ANNOTS_LOADED] =
	    g_signal_new ("annots-loaded",
	                  G_TYPE_FROM_CLASS (gobject_class),
	                  G_SIGNAL_RUN_LAST,
	                  0, NULL, NULL,
	                  g_cclosure_marshal_generic,
	                  G_TYPE_NONE, 0);
}

PpsAnnotationsContext *
pps_annotations_context_new (PpsDocumentModel *model, PpsUndoContext *undo_context)
{
	return PPS_ANNOTATIONS_CONTEXT (g_object_new (PPS_TYPE_ANNOTATIONS_CONTEXT,
	                                              "document-model", model,
	                                              "undo-context", undo_context,
	                                              NULL));
}

/**
 * pps_annotations_context_get_annots_model:
 * @self: a #PpsAnnotationsContext
 *
 * Returns: (not nullable) (transfer none): the returned #GListModel. The model
 * is owned but the `PpsAnnotationsContext` and shall not be modified outside
 * of it.
 */
GListModel *
pps_annotations_context_get_annots_model (PpsAnnotationsContext *self)
{
	g_return_val_if_fail (PPS_IS_ANNOTATIONS_CONTEXT (self), NULL);

	PpsAnnotationsContextPrivate *priv = GET_PRIVATE (self);

	return G_LIST_MODEL (priv->annots_model);
}

/**
 * pps_annotations_context_first_index_for_page:
 * @self: a #PpsAnnotationsContext
 * @page: The page number to search for.
 *
 * Finds the first index of an annotation for the specified page with a dichotomic
 * search.
 *
 * Returns: The index of the first annotation for the specified page in the model,
 * or the number of items if there is none.
 */
int
pps_annotations_context_first_index_for_page (PpsAnnotationsContext *self, gint page)
{
	g_return_val_if_fail (PPS_IS_ANNOTATIONS_CONTEXT (self), 0);

	PpsAnnotationsContextPrivate *priv = GET_PRIVATE (self);
	GListModel *annots_model = G_LIST_MODEL (priv->annots_model);

	int n = g_list_model_get_n_items (annots_model);
	int l = 0;
	int u = n - 1;
	PpsAnnotation *annot;

	if (page < 0 || l > u) {
		return n;
	}

	annot = PPS_ANNOTATION (g_list_model_get_item (annots_model, l));
	if (pps_annotation_get_page_index (annot) > page) {
		return n;
	}

	if (pps_annotation_get_page_index (annot) == page) {
		return l;
	}

	annot = PPS_ANNOTATION (g_list_model_get_item (annots_model, u));
	if (pps_annotation_get_page_index (annot) < page) {
		return n;
	}

	while (l < u) {
		int m = (l + u) / 2;
		annot = PPS_ANNOTATION (g_list_model_get_item (annots_model, m));
		int page_annot = pps_annotation_get_page_index (annot);
		if (page_annot > page) {
			u = m;
		} else if (page_annot == page) {
			if (m == 0 || pps_annotation_get_page_index (PPS_ANNOTATION (g_list_model_get_item (annots_model, m - 1))) < page) {
				return m;
			} else {
				u = m;
			}
		} else if (l == m) { /* u = l + 1 */
			PpsAnnotation *annot_u = PPS_ANNOTATION (g_list_model_get_item (annots_model, u));
			if (pps_annotation_get_page_index (annot_u) == page) {
				return u;
			}
			return n;
		} else {
			l = m;
		}
	}
	return l;
}

static int
compare_annot (const PpsAnnotation *a,
               const PpsAnnotation *b,
               gpointer user_data)
{
	guint index_a = pps_annotation_get_page_index ((PpsAnnotation *) a);
	guint index_b = pps_annotation_get_page_index ((PpsAnnotation *) b);

	if (index_a == index_b)
		return 0;

	return index_a > index_b ? 1 : -1;
}

static void
pps_annotation_changed_property_free (PpsAnnotationChangedProperty *changed)
{
	/* `property` is not owned by the changed property struct, so no need to free it */
	g_value_unset (&changed->value);
	g_free (changed);
}

static void
pps_annotations_context_free_action (gpointer data)
{
	PpsAnnotationsContextUndoable *undoable = data;
	if (undoable->action == PPS_ANNOTATIONS_MODIFIED) {
		g_list_free_full (undoable->modified.changes, (GDestroyNotify) pps_annotation_changed_property_free);
	} else if (undoable->action == PPS_ANNOTATIONS_ADDED) {
		g_list_free_full (undoable->added.areas, (GDestroyNotify) g_free);
	}
	g_list_free_full (undoable->annots, g_object_unref);
	g_date_time_unref (undoable->time);
	g_free (undoable);
}

static gboolean
pps_annotations_context_try_squash (PpsAnnotationsContext *context, PpsAnnotationsContextUndoable *undoable1, PpsAnnotationsContextUndoable *undoable2)
{
	if (g_date_time_difference (undoable2->time, undoable1->time) > 1000000) {
		return FALSE;
	}

	if (undoable1->action == PPS_ANNOTATIONS_MODIFIED && undoable2->action == PPS_ANNOTATIONS_MODIFIED) {
		/* to squash undoable2 into undoable1, we iterate over all changes
		   of undoable2 and add them to undoable1 if and only if there is no
		   other related change in undoable1 */
		GList *annots2 = undoable2->annots;
		for (GList *c2 = undoable2->modified.changes; c2; c2 = g_list_next (c2)) {
			PpsAnnotationChangedProperty *data2 = c2->data;
			gboolean in_undoable1 = FALSE;
			g_debug ("Squashing %s", data2->property);
			GList *annots1 = undoable1->annots;
			for (GList *c1 = undoable1->modified.changes; c1; c1 = g_list_next (c1)) {
				PpsAnnotationChangedProperty *data1 = c1->data;
				if (annots2->data == annots1->data && !g_strcmp0 (data1->property, data2->property)) {
					in_undoable1 = TRUE;
					break;
				}
				annots1 = g_list_next (annots1);
			}
			if (!in_undoable1) {
				undoable1->modified.changes = g_list_prepend (undoable1->modified.changes, data2);
				undoable1->annots = g_list_prepend (undoable1->annots, annots2->data);
			} else {
				pps_annotation_changed_property_free (data2);
				g_object_unref (G_OBJECT (annots2->data));
			}
			annots2 = g_list_next (annots2);
		}
		/* we can't use pps_annotations_context_free_action here since
		   contents of changes of undoable2 must not be freed since
		   they were copied over to undoable1 */
		g_list_free (undoable2->modified.changes);
		g_list_free (undoable2->annots);
		g_free (undoable2);
		return TRUE;
	}
	return FALSE;
}

static void
save_annotation (PpsAnnotationsContext *self,
                 PpsAnnotation *annot,
                 GList *changes)
{
	PpsAnnotationsContextPrivate *priv = GET_PRIVATE (self);
	PpsAnnotationsContextUndoable *undoable;
	gboolean squashed = FALSE;

	/* no change, we can stop here */
	if (!changes) {
		g_debug ("No change in annotation");
		return;
	}

	undoable = g_new0 (PpsAnnotationsContextUndoable, 1);
	undoable->time = g_date_time_new_now_local ();

	for (GList *c = changes; c; c = g_list_next (c)) {
		PpsAnnotationChangedProperty *data = (PpsAnnotationChangedProperty *) c->data;
		g_debug ("Change in %s", data->property);
		undoable->annots = g_list_prepend (undoable->annots, g_object_ref (annot));
	}
	undoable->action = PPS_ANNOTATIONS_MODIFIED;
	undoable->modified.changes = changes;

	if (!priv->next_squash_forbidden) {
		if (pps_undo_context_get_last_handler (priv->undo_context) == PPS_UNDO_HANDLER (self)) {
			PpsAnnotationsContextUndoable *last_undo = (PpsAnnotationsContextUndoable *) pps_undo_context_get_last_action (priv->undo_context);
			if (pps_annotations_context_try_squash (self, last_undo, undoable)) {
				squashed = TRUE;
			}
		}
	}
	priv->next_squash_forbidden = FALSE;
	if (!squashed) {
		pps_undo_context_add_action (priv->undo_context, PPS_UNDO_HANDLER (self), undoable);
	}
}

static void
annot_prop_changed_cb (PpsAnnotation *annot, GParamSpec *pspec, PpsAnnotationsContext *self)
{
	PpsAnnotationChangedProperty *changed = g_new0 (PpsAnnotationChangedProperty, 1);
	changed->property = pspec->name;
	g_value_init (&changed->value, pspec->value_type);
	pps_annotation_get_value_last_property (annot, &changed->value);

	save_annotation (self, annot, g_list_append (NULL, changed));
}

static void
connect_notify_signals (PpsAnnotationsContext *self, PpsAnnotation *annot)
{
	g_signal_connect_data (annot,
	                       "notify::area",
	                       G_CALLBACK (annot_prop_changed_cb),
	                       self, (GClosureNotify) NULL,
	                       G_CONNECT_DEFAULT);
	g_signal_connect_data (annot,
	                       "notify::contents",
	                       G_CALLBACK (annot_prop_changed_cb),
	                       self, (GClosureNotify) NULL,
	                       G_CONNECT_DEFAULT);
	g_signal_connect_data (annot,
	                       "notify::rgba",
	                       G_CALLBACK (annot_prop_changed_cb),
	                       self, (GClosureNotify) NULL,
	                       G_CONNECT_DEFAULT);
	g_signal_connect_data (annot,
	                       "notify::hidden",
	                       G_CALLBACK (annot_prop_changed_cb),
	                       self, (GClosureNotify) NULL,
	                       G_CONNECT_DEFAULT);

	if (PPS_IS_ANNOTATION_MARKUP (annot)) {
		g_signal_connect_data (annot,
		                       "notify::label",
		                       G_CALLBACK (annot_prop_changed_cb),
		                       self, (GClosureNotify) NULL,
		                       G_CONNECT_DEFAULT);
		g_signal_connect_data (annot,
		                       "notify::opacity",
		                       G_CALLBACK (annot_prop_changed_cb),
		                       self, (GClosureNotify) NULL,
		                       G_CONNECT_DEFAULT);
		g_signal_connect_data (annot,
		                       "notify::rectangle",
		                       G_CALLBACK (annot_prop_changed_cb),
		                       self, (GClosureNotify) NULL,
		                       G_CONNECT_DEFAULT);
		g_signal_connect_data (annot,
		                       "notify::popup-is-open",
		                       G_CALLBACK (annot_prop_changed_cb),
		                       self, (GClosureNotify) NULL,
		                       G_CONNECT_DEFAULT);
	}

	if (PPS_IS_ANNOTATION_TEXT (annot)) {
		g_signal_connect_data (annot,
		                       "notify::icon",
		                       G_CALLBACK (annot_prop_changed_cb),
		                       self, (GClosureNotify) NULL,
		                       G_CONNECT_DEFAULT);
		g_signal_connect_data (annot,
		                       "notify::is-open",
		                       G_CALLBACK (annot_prop_changed_cb),
		                       self, (GClosureNotify) NULL,
		                       G_CONNECT_DEFAULT);
	}

	if (PPS_IS_ANNOTATION_TEXT_MARKUP (annot)) {
		g_signal_connect_data (annot,
		                       "notify::type",
		                       G_CALLBACK (annot_prop_changed_cb),
		                       self, (GClosureNotify) NULL,
		                       G_CONNECT_DEFAULT);
	}

	if (PPS_IS_ANNOTATION_FREE_TEXT (annot)) {
		g_signal_connect_data (annot,
		                       "notify::font-rgba",
		                       G_CALLBACK (annot_prop_changed_cb),
		                       self, (GClosureNotify) NULL,
		                       G_CONNECT_DEFAULT);
		g_signal_connect_data (annot,
		                       "notify::font-desc",
		                       G_CALLBACK (annot_prop_changed_cb),
		                       self, (GClosureNotify) NULL,
		                       G_CONNECT_DEFAULT);
	}
	if (PPS_IS_ANNOTATION_INK (annot)) {
		g_signal_connect_data (annot,
		                       "notify::ink-list",
		                       G_CALLBACK (annot_prop_changed_cb),
		                       self, (GClosureNotify) NULL,
		                       G_CONNECT_DEFAULT);
	}
}

static void
add_annotation (PpsAnnotationsContext *self, PpsAnnotation *annot)
{
	PpsAnnotationsContextPrivate *priv = GET_PRIVATE (self);
	PpsDocument *document = pps_document_model_get_document (priv->model);
	PpsAnnotationsContextUndoable *undoable = g_new0 (PpsAnnotationsContextUndoable, 1);

	undoable->action = PPS_ANNOTATIONS_ADDED;
	undoable->time = g_date_time_new_now_local ();
	undoable->annots = g_list_append (NULL, g_object_ref (annot));
	PpsRectangle rect;
	pps_annotation_get_area (annot, &rect);
	undoable->added.areas = g_list_append (NULL, pps_rectangle_copy (&rect));
	pps_undo_context_add_action (priv->undo_context, PPS_UNDO_HANDLER (self), undoable);

	pps_document_annotations_add_annotation (PPS_DOCUMENT_ANNOTATIONS (document), annot);

	g_signal_emit (self, signals[SIGNAL_ANNOT_ADDED], 0, annot);

	g_list_store_insert_sorted (priv->annots_model, annot,
	                            (GCompareDataFunc) compare_annot, NULL);

	connect_notify_signals (self, annot);
}

/**
 * pps_annotations_context_add_annotation_sync:
 * @self: a #PpsAnnotationsContext
 * @page_index: the index of the page where the annotation will be added
 * @type: the type of annotation to add
 * @start: point where to start creating an annotation (ignored for ink annotations)
 * @end: point where to end creating the annotation. It is ignored for TEXT
 * annotations and ink annotations.
 * @color: the color to give to the annotation
 * @user_data: a pointer with auxiliary data that is annotation-dependent.
 * For text markup, this points to a #PpsAnnotationTextMarkupType.
 * For stamps, this points to a #cairo_surface_t.
 * For free text, this points to a #PangoFontDescription.
 * For ink, this points to a #PpsAnnotationInkAddData.
 *
 * Add an annotation based on the provided information.
 *
 * Returns: (transfer none): the newly created annotation
 *
 * Since: 48.0
 *
 */
PpsAnnotation *
pps_annotations_context_add_annotation_sync (PpsAnnotationsContext *self,
                                             gint page_index,
                                             PpsAnnotationType type,
                                             const PpsPoint *start,
                                             const PpsPoint *end,
                                             const GdkRGBA *color,
                                             const gpointer user_data)
{
	PpsAnnotationsContextPrivate *priv = GET_PRIVATE (self);
	PpsDocument *document = pps_document_model_get_document (priv->model);
	PpsAnnotation *annot;
	PpsRectangle doc_rect;
	PpsPage *page;

	g_return_val_if_fail (PPS_IS_ANNOTATIONS_CONTEXT (self), NULL);

	page = pps_document_get_page (document, page_index);

	switch (type) {
	case PPS_ANNOTATION_TYPE_TEXT:
		doc_rect.x1 = start->x - ANNOTATION_ICON_SIZE / 2;
		doc_rect.y1 = start->y - ANNOTATION_ICON_SIZE / 2;
		doc_rect.x2 = start->x + ANNOTATION_ICON_SIZE / 2;
		doc_rect.y2 = start->y + ANNOTATION_ICON_SIZE / 2;
		annot = pps_annotation_text_new (page);
		break;
	case PPS_ANNOTATION_TYPE_TEXT_MARKUP:
		doc_rect.x1 = start->x;
		doc_rect.y1 = start->y;
		doc_rect.x2 = end->x;
		doc_rect.y2 = end->y;
		annot = pps_annotation_text_markup_new (page, *(PpsAnnotationTextMarkupType *) user_data);
		break;
	case PPS_ANNOTATION_TYPE_FREE_TEXT: {
		GdkRGBA transparent = { 0, 0, 0, 0. };
		g_autoptr (PangoFontDescription) font;
		gdouble font_size = 12.;

		if (!user_data) {
			font = pango_font_description_new ();
			pango_font_description_set_family (font, "Liberation Sans");
			pango_font_description_set_size (font, font_size * PANGO_SCALE);
		} else {
			font = pango_font_description_copy ((PangoFontDescription *) user_data);
			font_size = (double) pango_font_description_get_size (font) / PANGO_SCALE;
		}

		doc_rect.x1 = start->x;
		doc_rect.x2 = start->x + 2;
		/* Ideally we should call pps_annotation_free_text_autoresize but
		we don't have a PangoContext here */
		doc_rect.y1 = start->y - font_size / 2 * 1.4;
		doc_rect.y2 = start->y + font_size / 2 * 1.4;

		annot = pps_annotation_free_text_new (page);

		pps_annotation_free_text_set_font_description (PPS_ANNOTATION_FREE_TEXT (annot), font);

		pps_annotation_free_text_set_font_rgba (PPS_ANNOTATION_FREE_TEXT (annot), color);
		pps_annotation_set_rgba (annot, &transparent);

		pps_annotation_set_contents (annot, "");
		break;
	}
	case PPS_ANNOTATION_TYPE_STAMP: {
		annot = pps_annotation_stamp_new (page);
		pps_annotation_stamp_set_surface (PPS_ANNOTATION_STAMP (annot), (cairo_surface_t *) user_data);
		doc_rect.x1 = start->x;
		doc_rect.y1 = start->y;
		doc_rect.x2 = end->x;
		doc_rect.y2 = end->y;
		break;
	}
	case PPS_ANNOTATION_TYPE_INK: {
		PpsAnnotationInkAddData *add_data = (PpsAnnotationInkAddData *) user_data;

		if (add_data->highlight) {
			annot = pps_annotation_ink_new_highlight (page);
		} else {
			annot = pps_annotation_ink_new (page);
		}

		pps_annotation_ink_set_ink_list (PPS_ANNOTATION_INK (annot), pps_ink_list_copy (add_data->ink_list));
		pps_annotation_ink_set_time_list (PPS_ANNOTATION_INK (annot), g_slist_copy_deep (add_data->times, (GCopyFunc) pps_ink_time_copy, NULL));
		pps_annotation_set_border_width (annot, add_data->line_width);
		if (ink_transformation)
			ink_transformation (annot, ink_transformation_data);
		break;
	}
	default:
		g_assert_not_reached ();
		return NULL;
	}

	/* By default, free text annotations have a transparent background, so color should not be set. */
	if (type != PPS_ANNOTATION_TYPE_FREE_TEXT) {
		pps_annotation_set_rgba (annot, color);
	}

	if (type != PPS_ANNOTATION_TYPE_INK) {
		/* Coordinates of an ink annotation are in its ink_list, setting the area results in a rescaling */
		pps_annotation_set_area (annot, &doc_rect);
	}

	if (PPS_IS_ANNOTATION_MARKUP (annot)) {
		pps_annotation_markup_set_label (PPS_ANNOTATION_MARKUP (annot), g_get_real_name ());
		pps_annotation_markup_set_popup_is_open (PPS_ANNOTATION_MARKUP (annot), FALSE);
		pps_annotation_markup_set_opacity (PPS_ANNOTATION_MARKUP (annot), 1.0);
	}

	pps_annotation_set_modified_from_time_t (annot, time (NULL));

	add_annotation (self, annot);

	return annot;
}

void
pps_annotations_context_remove_annotation (PpsAnnotationsContext *self,
                                           PpsAnnotation *annot)
{
	PpsAnnotationsContextPrivate *priv = GET_PRIVATE (self);
	PpsDocument *document = pps_document_model_get_document (priv->model);
	guint position;
	PpsAnnotationsContextUndoable *undoable;

	g_return_if_fail (PPS_IS_ANNOTATIONS_CONTEXT (self));
	g_return_if_fail (PPS_IS_ANNOTATION (annot));

	undoable = g_new0 (PpsAnnotationsContextUndoable, 1);
	undoable->time = g_date_time_new_now_local ();
	undoable->action = PPS_ANNOTATIONS_REMOVED;
	undoable->annots = g_list_append (NULL, g_object_ref (annot));

	pps_document_annotations_remove_annotation (PPS_DOCUMENT_ANNOTATIONS (document),
	                                            annot);

	if (!g_list_store_find (priv->annots_model, annot, &position))
		g_assert_not_reached ();

	g_list_store_remove (priv->annots_model, position);
	g_signal_emit (self, signals[SIGNAL_ANNOT_REMOVED], 0, annot);

	g_signal_handlers_disconnect_by_func (annot, annot_prop_changed_cb, self);

	pps_undo_context_add_action (priv->undo_context, PPS_UNDO_HANDLER (self), undoable);
}

static void
pps_annotations_context_undo (PpsUndoHandler *self, gpointer data)
{
	PpsAnnotationsContextUndoable *undoable = data;
	PpsAnnotationsContext *context = PPS_ANNOTATIONS_CONTEXT (self);
	PpsAnnotationsContextPrivate *priv = GET_PRIVATE (context);
	GList *annots, *areas;

	priv->next_squash_forbidden = TRUE;

	switch (undoable->action) {
	case PPS_ANNOTATIONS_MODIFIED:
		annots = undoable->annots;
		for (GList *c = undoable->modified.changes; c; c = g_list_next (c)) {
			PpsAnnotationChangedProperty *data = (PpsAnnotationChangedProperty *) c->data;
			g_debug ("Undoing %s", data->property);
			g_object_set_property (G_OBJECT (annots->data), data->property, &data->value);
			annots = g_list_next (annots);
		}
		break;
	case PPS_ANNOTATIONS_ADDED:
		areas = undoable->added.areas;
		for (GList *a = undoable->annots; a; a = g_list_next (a)) {
			pps_annotations_context_remove_annotation (context, g_object_ref (a->data));
			/* the area may be changed by poppler when the annotation
			   is added to the document, so we restore it now to its initial value */
			pps_annotation_set_area (PPS_ANNOTATION (a->data), areas->data);
			areas = g_list_next (areas);
		}
		break;
	case PPS_ANNOTATIONS_REMOVED:
		for (GList *a = undoable->annots; a; a = g_list_next (a)) {
			add_annotation (context, g_object_ref (a->data));
		}
		break;
	}
	priv->next_squash_forbidden = FALSE;
}

static void
pps_annotations_context_undo_handler_init (PpsUndoHandlerInterface *iface)
{
	iface->undo = pps_annotations_context_undo;
	iface->free_action = pps_annotations_context_free_action;
}

static int
cmp_rectangle_area_size (PpsRectangle *a,
                         PpsRectangle *b)
{
	gdouble wa, ha, wb, hb;

	wa = a->x2 - a->x1;
	ha = a->y2 - a->y1;
	wb = b->x2 - b->x1;
	hb = b->y2 - b->y1;

	if (wa == wb) {
		if (ha == hb)
			return 0;
		return (ha < hb) ? -1 : 1;
	}

	if (ha == hb) {
		return (wa < wb) ? -1 : 1;
	}

	return (wa * ha < wb * hb) ? -1 : 1;
}

/**
 * pps_annotations_context_get_annot_at_doc_point:
 * @self: a #PpsAnnotationsContext
 * @doc_point: the document point where to search for annotations
 *
 * Returns: (nullable) (transfer none): the #PpsAnnotation, if to be found
 *
 * Since: 49.0
 */
PpsAnnotation *
pps_annotations_context_get_annot_at_doc_point (PpsAnnotationsContext *self,
                                                const PpsDocumentPoint *doc_point)
{
	g_return_val_if_fail (PPS_IS_ANNOTATIONS_CONTEXT (self), NULL);
	g_return_val_if_fail (doc_point, NULL);

	PpsAnnotationsContextPrivate *priv = GET_PRIVATE (self);
	PpsDocument *document_obj = pps_document_model_get_document (priv->model);
	PpsDocumentAnnotations *document;
	GListModel *model = G_LIST_MODEL (priv->annots_model);
	PpsPoint point_on_page = doc_point->point_on_page;
	PpsAnnotation *best;

	if (!PPS_IS_DOCUMENT_ANNOTATIONS (document_obj))
		return NULL;

	document = PPS_DOCUMENT_ANNOTATIONS (document_obj);

	best = NULL;
	for (gint i = 0; i < g_list_model_get_n_items (model); i++) {
		PpsRectangle area;
		g_autoptr (PpsAnnotation) annot = g_list_model_get_item (model, i);

		if (pps_annotation_get_page_index (annot) != doc_point->page_index)
			continue;

		pps_annotation_get_area (annot, &area);

		if ((point_on_page.x >= area.x1) &&
		    (point_on_page.y >= area.y1) &&
		    (point_on_page.x <= area.x2) &&
		    (point_on_page.y <= area.y2)) {
			PpsRectangle best_area;

			if (pps_annotation_get_annotation_type (annot) == PPS_ANNOTATION_TYPE_TEXT_MARKUP &&
			    pps_document_annotations_over_markup (document, annot, point_on_page.x, point_on_page.y) == PPS_ANNOTATION_OVER_MARKUP_NOT)
				continue; /* ignore markup annots clicked outside the markup text */

			if (best == NULL) {
				best = annot;
				continue;
			}

			/* In case of only one match choose that. Otherwise
			 * compare the area of the bounding boxes and return the
			 * smallest element */
			pps_annotation_get_area (best, &best_area);
			if (cmp_rectangle_area_size (&area, &best_area) < 0)
				best = annot;
		}
	}
	return best;
}
