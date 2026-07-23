// SPDX-License-Identifier: GPL-2.0-or-later
/* pps-colors.c
 *
 * Copyright (C) 2025 Markus Göllnitz <camelcasenick@bewares.it>
 */

#include "pps-colors.h"

#include <adwaita.h>

void
get_accent_color (GdkRGBA *background_color, GdkRGBA *foreground_color)
{
	if (background_color) {
		AdwStyleManager *style_manager = adw_style_manager_get_default ();
		AdwAccentColor accent = adw_style_manager_get_accent_color (style_manager);
		adw_accent_color_to_rgba (accent, background_color);
	}

	if (foreground_color) {
		foreground_color->red = 1.0;
		foreground_color->green = 1.0;
		foreground_color->blue = 1.0;
	}
}

#ifdef HAVE_TRANSPARENT_SELECTION
void
get_selection_color (GtkWidget *widget, GdkRGBA *color)
{
	if (gtk_widget_get_state_flags (widget) & GTK_STATE_FLAG_FOCUS_WITHIN) {
		AdwStyleManager *style_manager = adw_style_manager_get_default ();
		AdwAccentColor accent = adw_style_manager_get_accent_color (style_manager);
		adw_accent_color_to_rgba (accent, color);
		color->alpha *= 0.3;
	} else {
		gtk_widget_get_color (widget, color);
		color->alpha *= 0.1;
	}
}
#endif
