// SPDX-License-Identifier: LGPL-2.1-or-later
/* this file is part of papers, a gnome document viewer
 *
 * Copyright © 2026 Porting Team
 */

#include "pps-platform.h"
#include <gtk/gtk.h>

/* Windows GTK Implementation of the Platform Abstraction Layer */

int pps_platform_get_widget_width (void *widget) {
    return gtk_widget_get_width (GTK_WIDGET (widget));
}

int pps_platform_get_widget_height (void *widget) {
    return gtk_widget_get_height (GTK_WIDGET (widget));
}

void pps_platform_snapshot_save (PpsPlatformContext *ctx) {
    gtk_snapshot_save (GTK_SNAPSHOT (ctx));
}

void pps_platform_snapshot_restore (PpsPlatformContext *ctx) {
    gtk_snapshot_restore (GTK_SNAPSHOT (ctx));
}

void pps_platform_snapshot_translate (PpsPlatformContext *ctx, double x, double y) {
    graphene_point_t point = GRAPHENE_POINT_INIT(x, y);
    gtk_snapshot_translate (GTK_SNAPSHOT (ctx), &point);
}

void pps_platform_snapshot_append_color (PpsPlatformContext *ctx, double r, double g, double b, double a, PpsPlatformRect *area) {
    GdkRGBA color = { r, g, b, a };
    graphene_rect_t rect = GRAPHENE_RECT_INIT(area->x, area->y, area->width, area->height);
    gtk_snapshot_append_color (GTK_SNAPSHOT (ctx), &color, &rect);
}

void pps_platform_snapshot_append_texture (PpsPlatformContext *ctx, void *texture, PpsPlatformRect *area) {
    graphene_rect_t rect = GRAPHENE_RECT_INIT(area->x, area->y, area->width, area->height);
    gtk_snapshot_append_texture (GTK_SNAPSHOT (ctx), GDK_TEXTURE (texture), &rect);
}

void pps_platform_snapshot_push_blend (PpsPlatformContext *ctx, int blend_mode) {
    gtk_snapshot_push_blend (GTK_SNAPSHOT (ctx), blend_mode);
}

void pps_platform_snapshot_pop (PpsPlatformContext *ctx) {
    gtk_snapshot_pop (GTK_SNAPSHOT (ctx));
}

void *pps_platform_get_first_child (void *widget) {
    return gtk_widget_get_first_child (GTK_WIDGET (widget));
}

void *pps_platform_get_next_sibling (void *child) {
    return gtk_widget_get_next_sibling (GTK_WIDGET (child));
}

void pps_platform_unparent (void *widget) {
    gtk_widget_unparent (GTK_WIDGET (widget));
}

gboolean pps_platform_grab_focus (void *widget) {
    return gtk_widget_grab_focus (GTK_WIDGET (widget));
}

void pps_platform_error_bell (void *widget) {
    gtk_widget_error_bell (GTK_WIDGET (widget));
}

void pps_platform_add_css_class (void *widget, const gchar *class_name) {
    gtk_widget_add_css_class (GTK_WIDGET (widget), class_name);
}

void pps_platform_remove_css_class (void *widget, const gchar *class_name) {
    gtk_widget_remove_css_class (GTK_WIDGET (widget), class_name);
}

void pps_platform_gesture_set_state (void *gesture, int state) {
    gtk_gesture_set_state (GTK_GESTURE (gesture), state);
}

guint pps_platform_get_event_state (void *controller) {
    return gtk_event_controller_get_current_event_state (GTK_EVENT_CONTROLLER (controller));
}

double pps_platform_get_adjustment_value (void *adjustment) {
    return gtk_adjustment_get_value (GTK_ADJUSTMENT (adjustment));
}

double pps_platform_get_adjustment_upper (void *adjustment) {
    return gtk_adjustment_get_upper (GTK_ADJUSTMENT (adjustment));
}

double pps_platform_get_adjustment_lower (void *adjustment) {
    return gtk_adjustment_get_lower (GTK_ADJUSTMENT (adjustment));
}

double pps_platform_get_adjustment_page_size (void *adjustment) {
    return gtk_adjustment_get_page_size (GTK_ADJUSTMENT (adjustment));
}

void pps_platform_adjustment_set_value (void *adjustment, double value) {
    gtk_adjustment_set_value (GTK_ADJUSTMENT (adjustment), value);
}

guint32 pps_platform_get_event_time (void *controller) {
    return gtk_event_controller_get_current_event_time (GTK_EVENT_CONTROLLER (controller));
}

guint pps_platform_get_current_button (void *gesture) {
    return gtk_gesture_single_get_current_button (GTK_GESTURE_SINGLE (gesture));
}

double pps_platform_get_widget_dpi (void *widget) {
    // Windows fallback display DPI is 96, or standard system display scaling.
    // Under GTK4 Win32 backend, monitor geometries are fully supported.
    // So we can still rely on the common helper.
    extern gdouble pps_document_misc_get_widget_dpi (GtkWidget *widget);
    return pps_document_misc_get_widget_dpi (GTK_WIDGET (widget));
}

gint pps_platform_get_text_direction (void *widget) {
    return gtk_widget_get_direction (GTK_WIDGET (widget)) || gtk_widget_get_default_direction ();
}

void pps_platform_queue_draw (void *widget) {
    gtk_widget_queue_draw (GTK_WIDGET (widget));
}
