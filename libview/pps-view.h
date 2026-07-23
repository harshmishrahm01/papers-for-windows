// SPDX-License-Identifier: GPL-2.0-or-later
/* this file is part of papers, a gnome document viewer
 *
 *  Copyright (C) 2004 Red Hat, Inc
 */

#pragma once

#if !defined(__PPS_PAPERS_VIEW_H_INSIDE__) && !defined(PAPERS_COMPILATION)
#error "Only <papers-view.h> can be included directly."
#endif

#include <gtk/gtk.h>

#include <papers-document.h>

#include "context/pps-annotations-context.h"
#include "context/pps-document-model.h"
#include "context/pps-search-context.h"
#include "pps-jobs.h"

G_BEGIN_DECLS

#define PPS_TYPE_VIEW (pps_view_get_type ())
PPS_PUBLIC
G_DECLARE_DERIVABLE_TYPE (PpsView, pps_view, PPS, VIEW, GtkWidget)

PPS_PUBLIC
PpsView *pps_view_new (void);
PPS_PUBLIC
void pps_view_set_model (PpsView *view,
                         PpsDocumentModel *model);
PPS_PUBLIC
void pps_view_reload (PpsView *view);
PPS_PUBLIC
void pps_view_set_page_cache_size (PpsView *view,
                                   gsize cache_size);

PPS_PUBLIC
void pps_view_set_allow_links_change_zoom (PpsView *view,
                                           gboolean allowed);
PPS_PUBLIC
gboolean pps_view_get_allow_links_change_zoom (PpsView *view);

/* Selections */
#define PPS_TYPE_VIEW_SELECTION (pps_view_selection_get_type ())
typedef struct {
	int page;
	/* The coordinates here are at scale == 1.0, so that we can ignore
	 * resizings.  There is one per page, maximum.
	 */
	PpsRectangle rect;
	cairo_region_t *covered_region;
	PpsSelectionStyle style;
} PpsViewSelection;

PPS_PUBLIC
GType pps_view_selection_get_type (void);
PPS_PUBLIC
PpsViewSelection *pps_view_selection_copy (PpsViewSelection *selection);
PPS_PUBLIC
void pps_view_selection_free (PpsViewSelection *selection);

PPS_PUBLIC
void pps_view_copy (PpsView *view);
PPS_PUBLIC
void pps_view_copy_link_address (PpsView *view,
                                 PpsLinkAction *action);
PPS_PUBLIC
void pps_view_select_all (PpsView *view);
PPS_PUBLIC
gboolean pps_view_has_selection (PpsView *view);
PPS_PUBLIC
char *pps_view_get_selected_text (PpsView *view);
PPS_PUBLIC
GList *pps_view_get_selections (PpsView *view);

/* Page size */
PPS_PUBLIC
gboolean pps_view_can_zoom_in (PpsView *view);
PPS_PUBLIC
void pps_view_zoom_in (PpsView *view);
PPS_PUBLIC
gboolean pps_view_can_zoom_out (PpsView *view);
PPS_PUBLIC
void pps_view_zoom_out (PpsView *view);

/* Find */
PPS_PUBLIC
void pps_view_set_search_context (PpsView *view,
                                  PpsSearchContext *context);

/* Navigation */
PPS_PUBLIC
void pps_view_handle_link (PpsView *view,
                           PpsLink *link);
PPS_PUBLIC
gboolean pps_view_next_page (PpsView *view);
PPS_PUBLIC
gboolean pps_view_previous_page (PpsView *view);

PPS_PUBLIC
PpsDocumentPoint *pps_view_get_document_point_for_view_point (PpsView *view,
                                                              gdouble view_point_x,
                                                              gdouble view_point_y);

/* Annotations */
PPS_PUBLIC
void pps_view_set_annotations_context (PpsView *view,
                                       PpsAnnotationsContext *context);
PPS_PUBLIC
void pps_view_focus_annotation (PpsView *view,
                                PpsAnnotation *annot);
PPS_PUBLIC
void pps_view_open_annotation_editor (PpsView *view,
                                      PpsAnnotation *annot);
PPS_PUBLIC
void pps_view_set_enable_spellchecking (PpsView *view,
                                        gboolean spellcheck);
PPS_PUBLIC
gboolean pps_view_get_enable_spellchecking (PpsView *view);

/* Caret navigation */
PPS_PUBLIC
gboolean pps_view_supports_caret_navigation (PpsView *view);
PPS_PUBLIC
gboolean pps_view_is_caret_navigation_enabled (PpsView *view);
PPS_PUBLIC
void pps_view_set_caret_navigation_enabled (PpsView *view,
                                            gboolean enabled);
PPS_PUBLIC
void pps_view_set_caret_cursor_position (PpsView *view,
                                         guint page,
                                         guint offset);
PPS_PUBLIC
gboolean pps_view_current_event_is_type (PpsView *view,
                                         GdkEventType type);

/* Signatures */
PPS_PUBLIC
void pps_view_start_signature_rect (PpsView *view);
PPS_PUBLIC
void pps_view_cancel_signature_rect (PpsView *view);

G_END_DECLS
