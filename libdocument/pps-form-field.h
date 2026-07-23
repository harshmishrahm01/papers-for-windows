// SPDX-License-Identifier: GPL-2.0-or-later
/* this file is part of papers, a gnome document viewer
 *
 *  Copyright (C) 2006 Julien Rebetez
 */

#pragma once

#if !defined(__PPS_PAPERS_DOCUMENT_H_INSIDE__) && !defined(PAPERS_COMPILATION)
#error "Only <papers-document.h> can be included directly."
#endif

#include <glib-object.h>

#include "pps-document.h"
#include "pps-link.h"
#include "pps-macros.h"

G_BEGIN_DECLS

#define PPS_TYPE_FORM_FIELD (pps_form_field_get_type ())
#define PPS_FORM_FIELD(object) (G_TYPE_CHECK_INSTANCE_CAST ((object), PPS_TYPE_FORM_FIELD, PpsFormField))
#define PPS_FORM_FIELD_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), PPS_TYPE_FORM_FIELD, PpsFormFieldClass))
#define PPS_IS_FORM_FIELD(object) (G_TYPE_CHECK_INSTANCE_TYPE ((object), PPS_TYPE_FORM_FIELD))
#define PPS_IS_FORM_FIELD_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), PPS_TYPE_FORM_FIELD))
#define PPS_FORM_FIELD_GET_CLASS(object) (G_TYPE_INSTANCE_GET_CLASS ((object), PPS_TYPE_FORM_FIELD, PpsFormFieldClass))

#define PPS_TYPE_FORM_FIELD_TEXT (pps_form_field_text_get_type ())
#define PPS_FORM_FIELD_TEXT(object) (G_TYPE_CHECK_INSTANCE_CAST ((object), PPS_TYPE_FORM_FIELD_TEXT, PpsFormFieldText))
#define PPS_FORM_FIELD_TEXT_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), PPS_TYPE_FORM_FIELD_TEXT, PpsFormFieldTextClass))
#define PPS_IS_FORM_FIELD_TEXT(object) (G_TYPE_CHECK_INSTANCE_TYPE ((object), PPS_TYPE_FORM_FIELD_TEXT))
#define PPS_IS_FORM_FIELD_TEXT_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), PPS_TYPE_FORM_FIELD_TEXT))
#define PPS_FORM_FIELD_TEXT_GET_CLASS(object) (G_TYPE_INSTANCE_GET_CLASS ((object), PPS_TYPE_FORM_FIELD_TEXT, PpsFormFieldTextClass))

#define PPS_TYPE_FORM_FIELD_BUTTON (pps_form_field_button_get_type ())
#define PPS_FORM_FIELD_BUTTON(object) (G_TYPE_CHECK_INSTANCE_CAST ((object), PPS_TYPE_FORM_FIELD_BUTTON, PpsFormFieldButton))
#define PPS_FORM_FIELD_BUTTON_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), PPS_TYPE_FORM_FIELD_BUTTON, PpsFormFieldButtonClass))
#define PPS_IS_FORM_FIELD_BUTTON(object) (G_TYPE_CHECK_INSTANCE_TYPE ((object), PPS_TYPE_FORM_FIELD_BUTTON))
#define PPS_IS_FORM_FIELD_BUTTON_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), PPS_TYPE_FORM_FIELD_BUTTON))
#define PPS_FORM_FIELD_BUTTON_GET_CLASS(object) (G_TYPE_INSTANCE_GET_CLASS ((object), PPS_TYPE_FORM_FIELD_BUTTON, PpsFormFieldButtonClass))

#define PPS_TYPE_FORM_FIELD_CHOICE (pps_form_field_choice_get_type ())
#define PPS_FORM_FIELD_CHOICE(object) (G_TYPE_CHECK_INSTANCE_CAST ((object), PPS_TYPE_FORM_FIELD_CHOICE, PpsFormFieldChoice))
#define PPS_FORM_FIELD_CHOICE_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), PPS_TYPE_FORM_FIELD_CHOICE, PpsFormFieldChoiceClass))
#define PPS_IS_FORM_FIELD_CHOICE(object) (G_TYPE_CHECK_INSTANCE_TYPE ((object), PPS_TYPE_FORM_FIELD_CHOICE))
#define PPS_IS_FORM_FIELD_CHOICE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), PPS_TYPE_FORM_FIELD_CHOICE))
#define PPS_FORM_FIELD_CHOICE_GET_CLASS(object) (G_TYPE_INSTANCE_GET_CLASS ((object), PPS_TYPE_FORM_FIELD_CHOICE, PpsFormFieldChoiceClass))

#define PPS_TYPE_FORM_FIELD_SIGNATURE (pps_form_field_signature_get_type ())
#define PPS_FORM_FIELD_SIGNATURE(object) (G_TYPE_CHECK_INSTANCE_CAST ((object), PPS_TYPE_FORM_FIELD_SIGNATURE, PpsFormFieldSignature))
#define PPS_FORM_FIELD_SIGNATURE_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), PPS_TYPE_FORM_FIELD_SIGNATURE, PpsFormFieldSignatureClass))
#define PPS_IS_FORM_FIELD_SIGNATURE(object) (G_TYPE_CHECK_INSTANCE_TYPE ((object), PPS_TYPE_FORM_FIELD_SIGNATURE))
#define PPS_IS_FORM_FIELD_SIGNATURE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), PPS_TYPE_FORM_FIELD_SIGNATURE))
#define PPS_FORM_FIELD_SIGNATURE_GET_CLASS(object) (G_TYPE_INSTANCE_GET_CLASS ((object), PPS_TYPE_FORM_FIELD_SIGNATURE, PpsFormFieldSignatureClass))

typedef struct _PpsFormField PpsFormField;
typedef struct _PpsFormFieldClass PpsFormFieldClass;

typedef struct _PpsFormFieldText PpsFormFieldText;
typedef struct _PpsFormFieldTextClass PpsFormFieldTextClass;

typedef struct _PpsFormFieldButton PpsFormFieldButton;
typedef struct _PpsFormFieldButtonClass PpsFormFieldButtonClass;

typedef struct _PpsFormFieldChoice PpsFormFieldChoice;
typedef struct _PpsFormFieldChoiceClass PpsFormFieldChoiceClass;

typedef struct _PpsFormFieldSignature PpsFormFieldSignature;
typedef struct _PpsFormFieldSignatureClass PpsFormFieldSignatureClass;

typedef enum {
	PPS_FORM_FIELD_TEXT_NORMAL,
	PPS_FORM_FIELD_TEXT_MULTILINE,
	PPS_FORM_FIELD_TEXT_FILE_SELECT
} PpsFormFieldTextType;

typedef enum {
	PPS_FORM_FIELD_BUTTON_PUSH,
	PPS_FORM_FIELD_BUTTON_CHECK,
	PPS_FORM_FIELD_BUTTON_RADIO
} PpsFormFieldButtonType;

typedef enum {
	PPS_FORM_FIELD_CHOICE_COMBO,
	PPS_FORM_FIELD_CHOICE_LIST
} PpsFormFieldChoiceType;

struct _PpsFormField {
	GObject parent;

	gint id;
	gboolean is_read_only;
	gdouble font_size;
	PpsLink *activation_link;

	PpsPage *page;
	gboolean changed;
};

struct _PpsFormFieldClass {
	GObjectClass parent_class;
};

struct _PpsFormFieldText {
	PpsFormField parent;

	PpsFormFieldTextType type;

	gboolean do_spell_check : 1;
	gboolean do_scroll : 1;
	gboolean comb : 1;
	gboolean is_rich_text : 1;
	gboolean is_password;

	gint max_len;
	gchar *text;
};

struct _PpsFormFieldTextClass {
	PpsFormFieldClass parent_class;
};

struct _PpsFormFieldButton {
	PpsFormField parent;

	PpsFormFieldButtonType type;

	gboolean state;
};

struct _PpsFormFieldButtonClass {
	PpsFormFieldClass parent_class;
};

struct _PpsFormFieldChoice {
	PpsFormField parent;

	PpsFormFieldChoiceType type;

	gboolean multi_select : 1;
	gboolean is_editable : 1;
	gboolean do_spell_check : 1;
	gboolean commit_on_sel_change : 1;

	GList *selected_items;
	gchar *text;
};

struct _PpsFormFieldChoiceClass {
	PpsFormFieldClass parent_class;
};

struct _PpsFormFieldSignature {
	PpsFormField parent;

	/* TODO */
};

struct _PpsFormFieldSignatureClass {
	PpsFormFieldClass parent_class;
};

/* PpsFormField base class */
PPS_PUBLIC
GType pps_form_field_get_type (void);

/* PpsFormFieldText */
PPS_PUBLIC
GType pps_form_field_text_get_type (void);
PPS_PUBLIC
PpsFormField *pps_form_field_text_new (gint id,
                                       PpsFormFieldTextType type);

/* PpsFormFieldButton */
PPS_PUBLIC
GType pps_form_field_button_get_type (void);
PPS_PUBLIC
PpsFormField *pps_form_field_button_new (gint id,
                                         PpsFormFieldButtonType type);

/* PpsFormFieldChoice */
PPS_PUBLIC
GType pps_form_field_choice_get_type (void);
PPS_PUBLIC
PpsFormField *pps_form_field_choice_new (gint id,
                                         PpsFormFieldChoiceType type);

/* PpsFormFieldSignature */
PPS_PUBLIC
GType pps_form_field_signature_get_type (void);
PPS_PUBLIC
PpsFormField *pps_form_field_signature_new (gint id);

G_END_DECLS
