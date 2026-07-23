// SPDX-License-Identifier: GPL-2.0-or-later
/* pps-colors.h
 *
 * Copyright (C) 2025 Markus Göllnitz <camelcasenick@bewares.it>
 */

#include <gtk/gtk.h>

void get_accent_color (GdkRGBA *color, GdkRGBA *foreground_color);

#ifdef HAVE_TRANSPARENT_SELECTION
void get_selection_color (GtkWidget *widget, GdkRGBA *color);
#endif
