// SPDX-License-Identifier: GPL-2.0-or-later
/* this file is part of papers, a gnome document viewer
 *
 *  Copyright (C) 2006 Carlos Garcia Campos <carlosgc@gnome.org>
 */

#pragma once

#if !defined(__PPS_PAPERS_DOCUMENT_H_INSIDE__) && !defined(PAPERS_COMPILATION)
#error "Only <papers-document.h> can be included directly."
#endif

#include <gio/gio.h>
#include <glib-object.h>

#include "pps-macros.h"

G_BEGIN_DECLS

#define PPS_TYPE_ATTACHMENT (pps_attachment_get_type ())

#define PPS_ATTACHMENT_ERROR (pps_attachment_error_quark ())

PPS_PUBLIC
G_DECLARE_DERIVABLE_TYPE (PpsAttachment, pps_attachment, PPS, ATTACHMENT, GObject);

struct _PpsAttachmentClass {
	GObjectClass base_class;
};

PPS_PUBLIC
GQuark pps_attachment_error_quark (void);

PPS_PUBLIC
PpsAttachment *pps_attachment_new (const gchar *name,
                                   const gchar *description,
                                   GDateTime *mtime,
                                   GDateTime *ctime,
                                   gsize size,
                                   gpointer data);

PPS_PUBLIC
const gchar *pps_attachment_get_name (PpsAttachment *attachment);
PPS_PUBLIC
const gchar *pps_attachment_get_description (PpsAttachment *attachment);

PPS_PUBLIC
GDateTime *pps_attachment_get_modification_datetime (PpsAttachment *attachment);
PPS_PUBLIC
GDateTime *pps_attachment_get_creation_datetime (PpsAttachment *attachment);

PPS_PUBLIC
const gchar *pps_attachment_get_mime_type (PpsAttachment *attachment);
PPS_PUBLIC
gboolean pps_attachment_save (PpsAttachment *attachment,
                              GFile *file,
                              GError **error);
PPS_PUBLIC
gboolean pps_attachment_open (PpsAttachment *attachment,
                              GAppLaunchContext *context,
                              GError **error);

G_END_DECLS
