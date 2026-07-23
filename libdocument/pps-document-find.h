// SPDX-License-Identifier: GPL-2.0-or-later
/*
 *  Copyright (C) 2004 Red Hat, Inc.
 */

#pragma once

#if !defined(__PPS_PAPERS_DOCUMENT_H_INSIDE__) && !defined(PAPERS_COMPILATION)
#error "Only <papers-document.h> can be included directly."
#endif

#include <glib-object.h>
#include <glib.h>

#include "pps-document.h"
#include "pps-macros.h"

G_BEGIN_DECLS

#define PPS_TYPE_DOCUMENT_FIND (pps_document_find_get_type ())

PPS_PUBLIC
G_DECLARE_INTERFACE (PpsDocumentFind, pps_document_find, PPS, DOCUMENT_FIND, GObject)

typedef struct _PpsFindRectangle PpsFindRectangle;

#define PPS_TYPE_FIND_RECTANGLE (pps_find_rectangle_get_type ())
struct _PpsFindRectangle {
	gdouble x1;
	gdouble y1;
	gdouble x2;
	gdouble y2;
	gboolean next_line;    /* the boolean from poppler_rectangle_find_get_match_continued() */
	gboolean after_hyphen; /* the boolean from poppler_rectangle_find_get_ignored_hyphen() */
	void (*_pps_reserved1) (void);
	void (*_pps_reserved2) (void);
};

PPS_PUBLIC
GType pps_find_rectangle_get_type (void);
PPS_PUBLIC
PpsFindRectangle *pps_find_rectangle_new (void);
PPS_PUBLIC
PpsFindRectangle *pps_find_rectangle_copy (PpsFindRectangle *pps_find_rect);
PPS_PUBLIC
void pps_find_rectangle_free (PpsFindRectangle *pps_find_rect);

typedef enum {
	PPS_FIND_DEFAULT = 0,
	PPS_FIND_CASE_SENSITIVE = 1 << 0,
	PPS_FIND_WHOLE_WORDS_ONLY = 1 << 1
} PpsFindOptions;

struct _PpsDocumentFindInterface {
	GTypeInterface base_iface;

	/* Methods */
	PpsFindOptions (*get_supported_options) (PpsDocumentFind *document_find);
	GList *(*find_text) (PpsDocumentFind *document_find,
	                     PpsPage *page,
	                     const gchar *text,
	                     PpsFindOptions options);
};

PPS_PUBLIC
PpsFindOptions pps_document_find_get_supported_options (PpsDocumentFind *document_find);
PPS_PUBLIC
GList *pps_document_find_find_text (PpsDocumentFind *document_find,
                                    PpsPage *page,
                                    const gchar *text,
                                    PpsFindOptions options);

G_END_DECLS
