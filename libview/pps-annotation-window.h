// SPDX-License-Identifier: GPL-2.0-or-later
/* pps-annotation-window.h
 *  this file is part of papers, a gnome document viewer
 *
 * Copyright (C) 2009 Carlos Garcia Campos <carlosgc@gnome.org>
 * Copyright (C) 2007 Iñigo Martinez <inigomartinez@gmail.com>
 */

#pragma once

#if !defined(PAPERS_COMPILATION)
#error "This is a private header."
#endif

#include <gtk/gtk.h>

#include <pps-document.h>

#include "pps-annotation.h"

G_BEGIN_DECLS

typedef struct _PpsAnnotationWindow PpsAnnotationWindow;
typedef struct _PpsAnnotationWindowClass PpsAnnotationWindowClass;

#define PPS_TYPE_ANNOTATION_WINDOW (pps_annotation_window_get_type ())
#define PPS_ANNOTATION_WINDOW(object) (G_TYPE_CHECK_INSTANCE_CAST ((object), PPS_TYPE_ANNOTATION_WINDOW, PpsAnnotationWindow))
#define PPS_ANNOTATION_WINDOW_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), PPS_TYPE_ANNOTATION_WINDOW, PpsAnnotationWindowClass))
#define PPS_IS_ANNOTATION_WINDOW(object) (G_TYPE_CHECK_INSTANCE_TYPE ((object), PPS_TYPE_ANNOTATION_WINDOW))
#define PPS_IS_ANNOTATION_WINDOW_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), PPS_TYPE_ANNOTATION_WINDOW))
#define PPS_ANNOTATION_WINDOW_GET_CLASS(object) (G_TYPE_INSTANCE_GET_CLASS ((object), PPS_TYPE_ANNOTATION_WINDOW, PpsAnnotationWindowClass))

GType pps_annotation_window_get_type (void);
GtkWidget *pps_annotation_window_new (PpsAnnotationMarkup *annot,
                                      GtkWindow *parent);
PpsAnnotation *pps_annotation_window_get_annotation (PpsAnnotationWindow *window);
gboolean pps_annotation_window_is_open (PpsAnnotationWindow *window);
void pps_annotation_window_show (PpsAnnotationWindow *window);
void pps_annotation_window_set_enable_spellchecking (PpsAnnotationWindow *window,
                                                     gboolean spellcheck);

G_END_DECLS
