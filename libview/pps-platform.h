// SPDX-License-Identifier: LGPL-2.1-or-later
/* this file is part of papers, a gnome document viewer
 *
 * Copyright © 2026 Porting Team
 */

#pragma once

#include <glib.h>
#include <stdbool.h>

G_BEGIN_DECLS

/**
 * PpsPlatformContext:
 * Abstract handle to the platform-specific graphics context.
 */
typedef struct _PpsPlatformContext PpsPlatformContext;

/**
 * PpsPlatformRect:
 * Platform-agnostic rectangle definition.
 */
typedef struct {
    double x, y, width, height;
} PpsPlatformRect;

/**
 * PpsPlatformEvent:
 * Abstract event type to decouple from GdkEvent.
 */
typedef enum {
    PPS_PLATFORM_EVENT_MOUSE_MOVE,
    PPS_PLATFORM_EVENT_MOUSE_BUTTON,
    PPS_PLATFORM_EVENT_KEY_PRESS,
    PPS_PLATFORM_EVENT_SCROLL,
} PpsPlatformEventType;

typedef struct {
    PpsPlatformEventType type;
    double x, y;
    guint button;
    guint state;
} PpsPlatformEvent;

/* Graphics Abstractions */
void pps_platform_snapshot_save (PpsPlatformContext *ctx);
void pps_platform_snapshot_restore (PpsPlatformContext *ctx);
void pps_platform_snapshot_translate (PpsPlatformContext *ctx, double x, double y);
void pps_platform_snapshot_append_color (PpsPlatformContext *ctx, double r, double g, double b, double a, PpsPlatformRect *area);
void pps_platform_snapshot_append_texture (PpsPlatformContext *ctx, void *texture, PpsPlatformRect *area);
void pps_platform_snapshot_push_blend (PpsPlatformContext *ctx, int blend_mode);
void pps_platform_snapshot_pop (PpsPlatformContext *ctx);

/* Window/Widget Abstractions */
void pps_platform_gesture_set_state (void *gesture, int state);
guint pps_platform_get_event_state (void *controller);
double pps_platform_get_adjustment_value (void *adjustment);
double pps_platform_get_adjustment_upper (void *adjustment);
double pps_platform_get_adjustment_lower (void *adjustment);
double pps_platform_get_adjustment_page_size (void *adjustment);
void pps_platform_adjustment_set_value (void *adjustment, double value);
guint32 pps_platform_get_event_time (void *controller);
guint pps_platform_get_current_button (void *gesture);
double pps_platform_get_widget_dpi (void *widget);
gint pps_platform_get_text_direction (void *widget);
int pps_platform_get_widget_width (void *widget);
int pps_platform_get_widget_height (void *widget);
void pps_platform_queue_draw (void *widget);

/* Widget Hierarchy and Focus */
void *pps_platform_get_first_child (void *widget);
void *pps_platform_get_next_sibling (void *child);
void pps_platform_unparent (void *widget);
gboolean pps_platform_grab_focus (void *widget);
void pps_platform_error_bell (void *widget);

/* Styling */
void pps_platform_add_css_class (void *widget, const gchar *class_name);
void pps_platform_remove_css_class (void *widget, const gchar *class_name);

G_END_DECLS
