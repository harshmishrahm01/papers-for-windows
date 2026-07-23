// SPDX-License-Identifier: GPL-2.0-or-later
/* pps-mapping.h
 *  this file is part of papers, a gnome document viewer
 *
 * Copyright (C) 2009 Carlos Garcia Campos <carlosgc@gnome.org>
 */

#pragma once

#if !defined(__PPS_PAPERS_DOCUMENT_H_INSIDE__) && !defined(PAPERS_COMPILATION)
#error "Only <papers-document.h> can be included directly."
#endif

#include "pps-document.h"
#include "pps-macros.h"

G_BEGIN_DECLS

typedef struct _PpsMappingList PpsMappingList;

#define PPS_TYPE_MAPPING_LIST (pps_mapping_list_get_type ())
PPS_PUBLIC
GType pps_mapping_list_get_type (void);

PPS_PUBLIC
PpsMappingList *pps_mapping_list_new (guint page,
                                      GList *list,
                                      GDestroyNotify data_destroy_func);
PPS_PUBLIC
PpsMappingList *pps_mapping_list_ref (PpsMappingList *mapping_list);
PPS_PUBLIC
void pps_mapping_list_unref (PpsMappingList *mapping_list);

PPS_PUBLIC
guint pps_mapping_list_get_page (PpsMappingList *mapping_list);
PPS_PUBLIC
GList *pps_mapping_list_get_list (PpsMappingList *mapping_list);
PPS_PUBLIC
void pps_mapping_list_remove (PpsMappingList *mapping_list,
                              PpsMapping *mapping);
PPS_PUBLIC
PpsMapping *pps_mapping_list_find (PpsMappingList *mapping_list,
                                   gconstpointer data);
PPS_PUBLIC
PpsMapping *pps_mapping_list_get (PpsMappingList *mapping_list,
                                  const PpsPoint *point);
PPS_PUBLIC
gpointer pps_mapping_list_get_data (PpsMappingList *mapping_list,
                                    const PpsPoint *point);
PPS_PUBLIC
PpsMapping *pps_mapping_list_nth (PpsMappingList *mapping_list,
                                  guint n);
PPS_PUBLIC
guint pps_mapping_list_length (PpsMappingList *mapping_list);

G_END_DECLS
