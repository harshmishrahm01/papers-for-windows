// SPDX-License-Identifier: GPL-2.0-or-later
/* comics-document.h: Implementation of PpsDocument for comic book archives
 * Copyright (C) 2005, Teemu Tervo <teemu.tervo@gmx.net>
 */

#pragma once

#include "pps-document.h"
#include "pps-macros.h"

G_BEGIN_DECLS

#define COMICS_TYPE_DOCUMENT (comics_document_get_type ())
#define COMICS_DOCUMENT(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), COMICS_TYPE_DOCUMENT, ComicsDocument))
#define COMICS_IS_DOCUMENT(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), COMICS_TYPE_DOCUMENT))

typedef struct _ComicsDocument ComicsDocument;

GType comics_document_get_type (void);

G_END_DECLS
