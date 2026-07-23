// SPDX-License-Identifier: GPL-2.0-or-later
/* this file is part of papers, a gnome document viewer
 *
 *  Copyright (C) 2005 Red Hat, Inc
 */

/* This File is basically an extention of PpsView, and is out here just to keep
 * pps-view.c from exploding.
 */

#pragma once

#if !defined(PAPERS_COMPILATION)
#error "This is a private header."
#endif

#include <gtk/gtk.h>

#include <papers-document.h>
#include <papers-view.h>

G_BEGIN_DECLS

#define PPS_TYPE_PIXBUF_CACHE (pps_pixbuf_cache_get_type ())
#define PPS_PIXBUF_CACHE(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), PPS_TYPE_PIXBUF_CACHE, PpsPixbufCache))
#define PPS_IS_PIXBUF_CACHE(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), PPS_TYPE_PIXBUF_CACHE))

typedef struct _PpsPixbufCache PpsPixbufCache;
typedef struct _PpsPixbufCacheClass PpsPixbufCacheClass;

GType pps_pixbuf_cache_get_type (void);
PpsPixbufCache *pps_pixbuf_cache_new (GtkWidget *view,
                                      PpsDocumentModel *model,
                                      gsize max_size);
void pps_pixbuf_cache_set_max_size (PpsPixbufCache *pixbuf_cache,
                                    gsize max_size);
void pps_pixbuf_cache_set_page_range (PpsPixbufCache *pixbuf_cache,
                                      gint start_page,
                                      gint end_page,
                                      GList *selection_list);
GdkTexture *pps_pixbuf_cache_get_texture (PpsPixbufCache *pixbuf_cache,
                                          gint page);
void pps_pixbuf_cache_clear (PpsPixbufCache *pixbuf_cache);
void pps_pixbuf_cache_style_changed (PpsPixbufCache *pixbuf_cache);
void pps_pixbuf_cache_reload_page (PpsPixbufCache *pixbuf_cache,
                                   cairo_region_t *region,
                                   gint page,
                                   gint rotation,
                                   gdouble scale);
/* Selection */
GdkTexture *pps_pixbuf_cache_get_selection_texture (PpsPixbufCache *pixbuf_cache,
                                                    gint page,
                                                    gfloat scale);
cairo_region_t *pps_pixbuf_cache_get_selection_region (PpsPixbufCache *pixbuf_cache,
                                                       gint page,
                                                       gfloat scale);
void pps_pixbuf_cache_set_selection_list (PpsPixbufCache *pixbuf_cache,
                                          GList *selection_list);
GList *pps_pixbuf_cache_get_selection_list (PpsPixbufCache *pixbuf_cache);

PpsRenderAnnotsFlags pps_pixbuf_cache_rendered_state (PpsPixbufCache *pixbuf_cache, gint page);

G_END_DECLS
