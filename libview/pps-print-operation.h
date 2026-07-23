// SPDX-License-Identifier: GPL-2.0-or-later
/* this file is part of papers, a gnome document viewer
 *
 *  Copyright (C) 2008 Carlos Garcia Campos <carlosgc@gnome.org>
 */

#pragma once

#if !defined(__PPS_PAPERS_VIEW_H_INSIDE__) && !defined(PAPERS_COMPILATION)
#error "Only <papers-view.h> can be included directly."
#endif

#include <glib-object.h>
#include <gtk/gtk.h>

#include <papers-document.h>

G_BEGIN_DECLS

typedef struct _PpsPrintOperation PpsPrintOperation;
typedef struct _PpsPrintOperationClass PpsPrintOperationClass;

#define PPS_TYPE_PRINT_OPERATION (pps_print_operation_get_type ())
#define PPS_PRINT_OPERATION(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), PPS_TYPE_PRINT_OPERATION, PpsPrintOperation))
#define PPS_IS_PRINT_OPERATION(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), PPS_TYPE_PRINT_OPERATION))
#define PPS_PRINT_OPERATION_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), PPS_TYPE_PRINT_OPERATION, PpsPrintOperationClass))
#define PPS_IS_PRINT_OPERATION_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), PPS_TYPE_PRINT_OPERATION))
#define PPS_PRINT_OPERATION_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), PPS_TYPE_PRINT_OPERATION, PpsPrintOperationClass))

PPS_PUBLIC
GType pps_print_operation_get_type (void);

PPS_PUBLIC
gboolean pps_print_operation_exists_for_document (PpsDocument *document);
PPS_PUBLIC
PpsPrintOperation *pps_print_operation_new (PpsDocument *document);
PPS_PUBLIC
void pps_print_operation_set_current_page (PpsPrintOperation *op,
                                           gint current_page);
PPS_PUBLIC
void pps_print_operation_set_print_settings (PpsPrintOperation *op,
                                             GtkPrintSettings *print_settings);
PPS_PUBLIC
GtkPrintSettings *pps_print_operation_get_print_settings (PpsPrintOperation *op);
PPS_PUBLIC
void pps_print_operation_set_default_page_setup (PpsPrintOperation *op,
                                                 GtkPageSetup *page_setup);
PPS_PUBLIC
GtkPageSetup *pps_print_operation_get_default_page_setup (PpsPrintOperation *op);
PPS_PUBLIC
void pps_print_operation_set_job_name (PpsPrintOperation *op,
                                       const gchar *job_name);
PPS_PUBLIC
const gchar *pps_print_operation_get_job_name (PpsPrintOperation *op);
PPS_PUBLIC
void pps_print_operation_run (PpsPrintOperation *op,
                              GtkWindow *parent);
PPS_PUBLIC
void pps_print_operation_cancel (PpsPrintOperation *op);
PPS_PUBLIC
void pps_print_operation_get_error (PpsPrintOperation *op,
                                    GError **error);
PPS_PUBLIC
void pps_print_operation_set_embed_page_setup (PpsPrintOperation *op,
                                               gboolean embed);
PPS_PUBLIC
gboolean pps_print_operation_get_embed_page_setup (PpsPrintOperation *op);
PPS_PUBLIC
const gchar *pps_print_operation_get_status (PpsPrintOperation *op);
PPS_PUBLIC
gdouble pps_print_operation_get_progress (PpsPrintOperation *op);

G_END_DECLS
