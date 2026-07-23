// SPDX-FileCopyrightText: 2003, 2004 Marco Pesenti Gritti
// SPDX-FileCopyrightText: 2003, 2004 Christian Persch
//
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include <papers-view.h>

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define PPS_TYPE_PAGE_SELECTOR (pps_page_selector_get_type ())
#define PPS_PAGE_SELECTOR(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), PPS_TYPE_PAGE_SELECTOR, PpsPageSelector))

typedef struct _PpsPageSelector PpsPageSelector;
typedef struct _PpsPageSelectorClass PpsPageSelectorClass;

struct _PpsPageSelectorClass {
	GtkBoxClass parent_class;

	void (*activate_link) (PpsPageSelector *page_action,
	                       PpsLink *link);
};

GType pps_page_selector_get_type (void);

void pps_page_selector_set_model (PpsPageSelector *page_selector,
                                  PpsDocumentModel *doc_model);

G_END_DECLS
