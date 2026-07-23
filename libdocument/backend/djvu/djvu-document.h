// SPDX-License-Identifier: GPL-2.0-or-later
/* djvu-document.h: Implementation of PpsDocument for djvu documents
 * Copyright (C) 2005, Nickolay V. Shmyrev <nshmyrev@yandex.ru>
 */

#pragma once

#include "pps-document.h"
#include "pps-macros.h"

G_BEGIN_DECLS

#define DJVU_TYPE_DOCUMENT (djvu_document_get_type ())
#define DJVU_DOCUMENT(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), DJVU_TYPE_DOCUMENT, DjvuDocument))
#define DJVU_IS_DOCUMENT(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DJVU_TYPE_DOCUMENT))

typedef struct _DjvuDocument DjvuDocument;

GType djvu_document_get_type (void);

G_END_DECLS
