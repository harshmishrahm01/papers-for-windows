// SPDX-License-Identifier: GPL-2.0-or-later
/* this file is part of papers, a gnome document viewer
 *
 *  Copyright (C) 2008 Carlos Garcia Campos <carlosgc@gnome.org>
 *  Copyright (C) 2005 Red Hat, Inc
 */

#pragma once

#include <libdocument/pps-macros.h>
#if !defined(__PPS_PAPERS_VIEW_H_INSIDE__) && !defined(PAPERS_COMPILATION)
#error "Only <papers-view.h> can be included directly."
#endif

#include <cairo.h>
#include <gio/gio.h>
#include <gtk/gtk.h>

#include <papers-document.h>

#include "pps-job.h"

#define PPS_GET_TYPE_NAME(instance) g_type_name_from_instance ((gpointer) instance)

G_BEGIN_DECLS

typedef struct _PpsJobRenderTexture PpsJobRenderTexture;
typedef struct _PpsJobRenderTextureClass PpsJobRenderTextureClass;

typedef struct _PpsJobPageData PpsJobPageData;
typedef struct _PpsJobPageDataClass PpsJobPageDataClass;

typedef struct _PpsJobFind PpsJobFind;
typedef struct _PpsJobFindClass PpsJobFindClass;

#define PPS_TYPE_JOB_RENDER_TEXTURE (pps_job_render_texture_get_type ())
#define PPS_JOB_RENDER_TEXTURE(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), PPS_TYPE_JOB_RENDER_TEXTURE, PpsJobRenderTexture))
#define PPS_IS_JOB_RENDER_TEXTURE(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), PPS_TYPE_JOB_RENDER_TEXTURE))
#define PPS_JOB_RENDER_TEXTURE_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), PPS_TYPE_JOB_RENDER_TEXTURE, PpsJobRenderTextureClass))
#define PPS_IS_JOB_RENDER_TEXTURE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), PPS_TYPE_JOB_RENDER_TEXTURE))
#define PPS_JOB_RENDER_TEXTURE_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), PPS_TYPE_JOB_RENDER_TEXTURE, PpsJobRenderTextureClass))

#define PPS_TYPE_JOB_PAGE_DATA (pps_job_page_data_get_type ())
#define PPS_JOB_PAGE_DATA(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), PPS_TYPE_JOB_PAGE_DATA, PpsJobPageData))
#define PPS_IS_JOB_PAGE_DATA(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), PPS_TYPE_JOB_PAGE_DATA))
#define PPS_JOB_PAGE_DATA_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), PPS_TYPE_JOB_PAGE_DATA, PpsJobPageDataClass))
#define PPS_IS_JOB_PAGE_DATA_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), PPS_TYPE_JOB_PAGE_DATA))
#define PPS_JOB_PAGE_DATA_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), PPS_TYPE_JOB_PAGE_DATA, PpsJobPageDataClass))

#define PPS_TYPE_JOB_FIND (pps_job_find_get_type ())
#define PPS_JOB_FIND(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), PPS_TYPE_JOB_FIND, PpsJobFind))
#define PPS_IS_JOB_FIND(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), PPS_TYPE_JOB_FIND))
#define PPS_JOB_FIND_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), PPS_TYPE_JOB_FIND, PpsJobFindClass))
#define PPS_IS_JOB_FIND_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), PPS_TYPE_JOB_FIND))
#define PPS_JOB_FIND_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), PPS_TYPE_JOB_FIND, PpsJobFindClass))

struct _PpsJobLinks {
	PpsJob parent;
};

struct _PpsJobAttachments {
	PpsJob parent;
};

struct _PpsJobAnnots {
	PpsJob parent;
};

struct _PpsJobRenderTexture {
	PpsJob parent;

	gint page;
	gint rotation;
	gdouble scale;

	gboolean page_ready;
	gint target_width;
	gint target_height;
	GdkTexture *texture;

	gboolean include_selection;
	GdkTexture *selection;
	cairo_region_t *selection_region;
	PpsRectangle selection_points;
	PpsSelectionStyle selection_style;
	GdkRGBA base;
	GdkRGBA text;

	PpsRenderAnnotsFlags annot_flags;
};

struct _PpsJobRenderTextureClass {
	PpsJobClass parent_class;
};

typedef enum {
	PPS_PAGE_DATA_INCLUDE_NONE = 0,
	PPS_PAGE_DATA_INCLUDE_LINKS = 1 << 0,
	PPS_PAGE_DATA_INCLUDE_TEXT = 1 << 1,
	PPS_PAGE_DATA_INCLUDE_TEXT_MAPPING = 1 << 2,
	PPS_PAGE_DATA_INCLUDE_TEXT_LAYOUT = 1 << 3,
	PPS_PAGE_DATA_INCLUDE_TEXT_ATTRS = 1 << 4,
	PPS_PAGE_DATA_INCLUDE_TEXT_LOG_ATTRS = 1 << 5,
	PPS_PAGE_DATA_INCLUDE_IMAGES = 1 << 6,
	PPS_PAGE_DATA_INCLUDE_FORMS = 1 << 7,
	PPS_PAGE_DATA_INCLUDE_MEDIA = 1 << 8,
	PPS_PAGE_DATA_INCLUDE_ALL = (1 << 9) - 1
} PpsJobPageDataFlags;

struct _PpsJobPageData {
	PpsJob parent;

	gint page;
	PpsJobPageDataFlags flags;

	PpsMappingList *link_mapping;
	PpsMappingList *image_mapping;
	PpsMappingList *form_field_mapping;
	PpsMappingList *media_mapping;
	cairo_region_t *text_mapping;
	gchar *text;
	PpsRectangle *text_layout;
	guint text_layout_length;
	PangoAttrList *text_attrs;
	PangoLogAttr *text_log_attrs;
	gulong text_log_attrs_length;
};

struct _PpsJobPageDataClass {
	PpsJobClass parent_class;
};

struct _PpsJobThumbnailTexture {
	PpsJob parent;
};

struct _PpsJobFonts {
	PpsJob parent;
};

struct _PpsJobLoad {
	PpsJob parent;
};

struct _PpsJobSave {
	PpsJob parent;
};

struct _PpsJobFind {
	PpsJob parent;

	gint start_page;
	gint n_pages;
	GList **pages;
	gchar *text;
	gboolean has_results;
	PpsFindOptions options;
};

struct _PpsJobFindClass {
	PpsJobClass parent_class;

	/* Signals */
	void (*updated) (PpsJobFind *job,
	                 gint page);
};

struct _PpsJobLayers {
	PpsJob parent;
};

struct _PpsJobExport {
	PpsJob parent;
};

struct _PpsJobPrint {
	PpsJob parent;
};

struct _PpsJobSignatures {
	PpsJob parent;
};

/* PpsJobLinks */
PPS_PUBLIC
G_DECLARE_FINAL_TYPE (PpsJobLinks, pps_job_links, PPS, JOB_LINKS, PpsJob)
#define PPS_TYPE_JOB_LINKS (pps_job_links_get_type ())

PPS_PUBLIC
PpsJob *pps_job_links_new (PpsDocument *document);
PPS_PUBLIC
GListModel *pps_job_links_get_model (PpsJobLinks *job);

/* PpsJobAttachments */
PPS_PUBLIC
G_DECLARE_FINAL_TYPE (PpsJobAttachments, pps_job_attachments, PPS, JOB_ATTACHMENTS, PpsJob)
#define PPS_TYPE_JOB_ATTACHMENTS (pps_job_attachments_get_type ())

PPS_PUBLIC
PpsJob *pps_job_attachments_new (PpsDocument *document);
PPS_PUBLIC
GList *pps_job_attachments_get_attachments (PpsJobAttachments *job_attachments);

/* PpsJobAnnots */
PPS_PUBLIC
G_DECLARE_FINAL_TYPE (PpsJobAnnots, pps_job_annots, PPS, JOB_ANNOTS, PpsJob)
#define PPS_TYPE_JOB_ANNOTS (pps_job_annots_get_type ())

PPS_PUBLIC
PpsJob *pps_job_annots_new (PpsDocument *document);
PPS_PUBLIC
GList *pps_job_annots_get_annots (PpsJobAnnots *job);

/* PpsJobRenderTexture */
PPS_PUBLIC
GType pps_job_render_texture_get_type (void);
PPS_PUBLIC
PpsJob *pps_job_render_texture_new (PpsDocument *document,
                                    gint page,
                                    gint rotation,
                                    gdouble scale,
                                    gint width,
                                    gint height,
                                    PpsRenderAnnotsFlags annot_flags);
PPS_PUBLIC
void pps_job_render_texture_set_selection_info (PpsJobRenderTexture *job,
                                                PpsRectangle *selection_points,
                                                PpsSelectionStyle selection_style,
                                                GdkRGBA *text,
                                                GdkRGBA *base);

/* PpsJobPageData */
PPS_PUBLIC
GType pps_job_page_data_get_type (void);
PPS_PUBLIC
PpsJob *pps_job_page_data_new (PpsDocument *document,
                               gint page,
                               PpsJobPageDataFlags flags);

/* PpsJobThumbnailTexture */
PPS_PUBLIC
G_DECLARE_FINAL_TYPE (PpsJobThumbnailTexture, pps_job_thumbnail_texture, PPS, JOB_THUMBNAIL_TEXTURE, PpsJob)
#define PPS_TYPE_JOB_THUMBNAIL_TEXTURE (pps_job_thumbnail_texture_get_type ())

PPS_PUBLIC
PpsJob *pps_job_thumbnail_texture_new (PpsDocument *document,
                                       gint page,
                                       gint rotation,
                                       gdouble scale);
PPS_PUBLIC
PpsJob *pps_job_thumbnail_texture_new_with_target_size (PpsDocument *document,
                                                        gint page,
                                                        gint rotation,
                                                        gint target_width,
                                                        gint target_height);
PPS_PUBLIC
GdkTexture *pps_job_thumbnail_texture_get_texture (PpsJobThumbnailTexture *job);

/* PpsJobFonts */
PPS_PUBLIC
G_DECLARE_FINAL_TYPE (PpsJobFonts, pps_job_fonts, PPS, JOB_FONTS, PpsJob)
#define PPS_TYPE_JOB_FONTS (pps_job_fonts_get_type ())

PPS_PUBLIC
PpsJob *pps_job_fonts_new (PpsDocument *document);

/* PpsJobLoad */
PPS_PUBLIC
G_DECLARE_FINAL_TYPE (PpsJobLoad, pps_job_load, PPS, JOB_LOAD, PpsJob)
#define PPS_TYPE_JOB_LOAD (pps_job_load_get_type ())

PPS_PUBLIC
PpsJob *pps_job_load_new (void);
PPS_PUBLIC
void pps_job_load_set_uri (PpsJobLoad *job,
                           const gchar *uri);
PPS_PUBLIC
gboolean pps_job_load_set_fd (PpsJobLoad *job,
                              int fd,
                              const char *mime_type,
                              GError **error);
PPS_PUBLIC
void pps_job_load_take_fd (PpsJobLoad *job,
                           int fd,
                           const char *mime_type);
PPS_PUBLIC
void pps_job_load_set_password (PpsJobLoad *job,
                                const gchar *password);
PPS_PUBLIC
const gchar *pps_job_load_get_password (PpsJobLoad *job);
PPS_PUBLIC
void pps_job_load_set_password_save (PpsJobLoad *job,
                                     GPasswordSave save);
PPS_PUBLIC
GPasswordSave pps_job_load_get_password_save (PpsJobLoad *job);
PPS_PUBLIC
PpsDocument *pps_job_load_get_loaded_document (PpsJobLoad *job);

/* PpsJobSave */
PPS_PUBLIC
G_DECLARE_FINAL_TYPE (PpsJobSave, pps_job_save, PPS, JOB_SAVE, PpsJob)
#define PPS_TYPE_JOB_SAVE (pps_job_save_get_type ())

PPS_PUBLIC
PpsJob *pps_job_save_new (PpsDocument *document,
                          const gchar *uri,
                          const gchar *document_uri);
PPS_PUBLIC
const gchar *pps_job_save_get_uri (PpsJobSave *job_save);

/* PpsJobFind */
PPS_PUBLIC
GType pps_job_find_get_type (void);
PPS_PUBLIC
PpsJob *pps_job_find_new (PpsDocument *document,
                          gint start_page,
                          gint n_pages,
                          const gchar *text,
                          PpsFindOptions options);
PPS_PUBLIC
PpsFindOptions pps_job_find_get_options (PpsJobFind *job);
PPS_PUBLIC
gint pps_job_find_get_n_main_results (PpsJobFind *job,
                                      gint page);
PPS_PUBLIC
gboolean pps_job_find_has_results (PpsJobFind *job);
PPS_PUBLIC
GList **pps_job_find_get_results (PpsJobFind *job);

/* PpsJobLayers */
PPS_PUBLIC
G_DECLARE_FINAL_TYPE (PpsJobLayers, pps_job_layers, PPS, JOB_LAYERS, PpsJob)
#define PPS_TYPE_JOB_LAYERS (pps_job_layers_get_type ())

PPS_PUBLIC
PpsJob *pps_job_layers_new (PpsDocument *document);
PPS_PUBLIC
GListModel *pps_job_layers_get_model (PpsJobLayers *job_layers);

/* PpsJobExport */
PPS_PUBLIC
G_DECLARE_FINAL_TYPE (PpsJobExport, pps_job_export, PPS, JOB_EXPORT, PpsJob)
#define PPS_TYPE_JOB_EXPORT (pps_job_export_get_type ())

PPS_PUBLIC
PpsJob *pps_job_export_new (PpsDocument *document);
PPS_PUBLIC
void pps_job_export_set_page (PpsJobExport *job,
                              gint page);

/* PpsJobPrint */
PPS_PUBLIC
G_DECLARE_FINAL_TYPE (PpsJobPrint, pps_job_print, PPS, JOB_PRINT, PpsJob)
#define PPS_TYPE_JOB_PRINT (pps_job_print_get_type ())

PPS_PUBLIC
PpsJob *pps_job_print_new (PpsDocument *document);
PPS_PUBLIC
void pps_job_print_set_page (PpsJobPrint *job,
                             gint page);
PPS_PUBLIC
void pps_job_print_set_cairo (PpsJobPrint *job,
                              cairo_t *cr);

/* PpsJobSignatures */
PPS_PUBLIC
G_DECLARE_FINAL_TYPE (PpsJobSignatures, pps_job_signatures, PPS, JOB_SIGNATURES, PpsJob)
#define PPS_TYPE_JOB_SIGNATURES (pps_job_signatures_get_type ())

PPS_PUBLIC
PpsJob *pps_job_signatures_new (PpsDocument *document);
PPS_PUBLIC
GList *
pps_job_signatures_get_signatures (PpsJobSignatures *self);

G_END_DECLS
