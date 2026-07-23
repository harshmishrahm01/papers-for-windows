
// SPDX-License-Identifier: GPL-2.0-or-later
/* pdfdocument.h: Implementation of PpsDocument for tiffs
 * Copyright (C) 2005, Jonathan Blandford <jrb@gnome.org>
 */

#pragma once

#include "pps-document.h"
#include "pps-macros.h"

G_BEGIN_DECLS

#define TIFF_TYPE_DOCUMENT (tiff_document_get_type ())
#define TIFF_DOCUMENT(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), TIFF_TYPE_DOCUMENT, TiffDocument))
#define TIFF_IS_DOCUMENT(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TIFF_TYPE_DOCUMENT))

typedef struct _TiffDocument TiffDocument;

GType tiff_document_get_type (void);

G_END_DECLS
