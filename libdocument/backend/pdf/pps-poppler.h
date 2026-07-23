// SPDX-License-Identifier: GPL-2.0-or-later
/* pdfdocument.h: Implementation of PpsDocument for PDF
 * Copyright (C) 2004, Red Hat, Inc.
 */

#pragma once

#include "pps-document.h"
#include "pps-macros.h"

G_BEGIN_DECLS

#define PDF_TYPE_DOCUMENT (pdf_document_get_type ())
#define PDF_DOCUMENT(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), PDF_TYPE_DOCUMENT, PdfDocument))
#define PDF_IS_DOCUMENT(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), PDF_TYPE_DOCUMENT))

typedef struct _PdfDocument PdfDocument;
typedef struct _PdfDocumentClass PdfDocumentClass;

GType pdf_document_get_type (void);

G_END_DECLS
