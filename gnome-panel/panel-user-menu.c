/*
 * panel-user-menu.c: user status menu
 *
 * Copyright (C) 2003 Sun Microsystems, Inc.
 * Copyright (C) 2004 Vincent Untz
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
 * 02111-1307, USA.
 *
 * Authors:
 *	Mark McLoughlin <mark@skynet.ie>
 *	Vincent Untz <vincent@vuntz.net>
 */

#include <config.h>

#include "applet.h"
#include "panel-layout.h"
#include "panel-menu-bar-object.h"
#include "panel-menu-items.h"
#include "panel-typebuiltins.h"
#include "panel-schemas.h"

#include "panel-user-menu.h"

G_DEFINE_TYPE (PanelUserMenu, panel_user_menu, PANEL_TYPE_MENU_BAR_OBJECT)

#define PANEL_USER_MENU_GET_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE ((o), PANEL_TYPE_USER_MENU, PanelUserMenuPrivate))

enum {
	PROP_0,
	PROP_TITLE
};

struct _PanelUserMenuPrivate {
	AppletInfo  *info;
	PanelWidget *panel;
	GSettings   *settings_instance;

	GtkWidget   *desktop_item;

	PanelUserMenuTitle title;
};

static void
panel_user_menu_init (PanelUserMenu *usermenu)
{
	usermenu->priv = PANEL_USER_MENU_GET_PRIVATE (usermenu);

	usermenu->priv->info = NULL;
	usermenu->priv->settings_instance = NULL;

	usermenu->priv->desktop_item = NULL;

	usermenu->priv->title = PANEL_USER_MENU_TITLE_REAL_NAME;
}

static void
panel_user_menu_finalize (GObject *object)
{
	PanelUserMenu *usermenu = PANEL_USER_MENU (object);

	if (usermenu->priv->settings_instance)
		g_object_unref (usermenu->priv->settings_instance);
	usermenu->priv->settings_instance = NULL;

	G_OBJECT_CLASS (panel_user_menu_parent_class)->finalize (object);
}

static void
panel_user_menu_get_property (GObject    *object,
			      guint       prop_id,
			      GValue     *value,
			      GParamSpec *pspec)
{
	PanelUserMenu *usermenu = PANEL_USER_MENU (object);

	switch (prop_id) {
	case PROP_TITLE:
		g_value_set_enum (value, usermenu->priv->title);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
		break;
	}
}

static void
panel_user_menu_set_property (GObject      *object,
			      guint         prop_id,
			      const GValue *value,
			      GParamSpec   *pspec)
{
	PanelUserMenu *usermenu = PANEL_USER_MENU (object);

	switch (prop_id) {
	case PROP_TITLE:
                panel_user_menu_set_title (usermenu, g_value_get_enum (value));
                break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
		break;
	}
}

static void
panel_user_menu_parent_set (GtkWidget *widget,
			   GtkWidget *previous_parent)
{
	PanelUserMenu *usermenu = PANEL_USER_MENU (widget);
	GtkWidget    *parent;

	parent = gtk_widget_get_parent (widget);
	g_assert (!parent || PANEL_IS_WIDGET (parent));

	usermenu->priv->panel = (PanelWidget *) parent;

	if (usermenu->priv->desktop_item)
		panel_desktop_menu_item_set_panel (usermenu->priv->desktop_item,
						   usermenu->priv->panel);
}

static void
panel_user_menu_class_init (PanelUserMenuClass *klass)
{
	GObjectClass   *gobject_class = (GObjectClass   *) klass;
	GtkWidgetClass *widget_class  = (GtkWidgetClass *) klass;

	gobject_class->finalize     = panel_user_menu_finalize;
	gobject_class->get_property = panel_user_menu_get_property;
	gobject_class->set_property = panel_user_menu_set_property;

	widget_class->parent_set = panel_user_menu_parent_set;

	g_type_class_add_private (klass, sizeof (PanelUserMenuPrivate));

	g_object_class_install_property (
			gobject_class,
			PROP_TITLE,
			g_param_spec_enum ("title",
					   "Title",
					   "Title for the menu",
					   PANEL_TYPE_USER_MENU_TITLE,
					   PANEL_USER_MENU_TITLE_REAL_NAME,
					   G_PARAM_READWRITE));
}

static void
panel_user_menu_settings_changed (GSettings       *settings,
				  char            *key,
				  PanelUserMenu   *usermenu)
{
	if (g_strcmp0 (key, PANEL_USER_MENU_TITLE_KEY) == 0) {
		PanelUserMenuTitle value;
		value = g_settings_get_enum (settings, key);
		panel_user_menu_set_title (usermenu, value);
	}
}

void
panel_user_menu_set_title (PanelUserMenu      *usermenu,
			   PanelUserMenuTitle  title)
{
	g_return_if_fail (PANEL_IS_USER_MENU (usermenu));

	if (usermenu->priv->title == title)
		return;

	usermenu->priv->title = title;

	if (usermenu->priv->desktop_item != NULL)
		panel_desktop_menu_item_set_title(usermenu->priv->desktop_item,
						  title);
}

void
panel_user_menu_load (PanelWidget *panel,
		     const char  *id,
		     GSettings   *settings)
{
	PanelUserMenu      *usermenu;
	GSettings          *settings_instance;
	PanelUserMenuTitle  title;

	g_return_if_fail (panel != NULL);

	settings_instance = panel_layout_get_instance_settings (settings,
								PANEL_USER_MENU_SCHEMA);

	title = g_settings_get_enum (settings_instance,
				     PANEL_USER_MENU_TITLE_KEY);

	usermenu = g_object_new (PANEL_TYPE_USER_MENU,
				 "title", title, NULL);

	usermenu->priv->desktop_item = panel_desktop_menu_item_new (TRUE, TRUE, TRUE, usermenu->priv->title);
	gtk_menu_shell_append (GTK_MENU_SHELL (usermenu),
			       usermenu->priv->desktop_item);
	gtk_widget_show (usermenu->priv->desktop_item);

	usermenu->priv->info = panel_applet_register (
					GTK_WIDGET (usermenu), panel,
					PANEL_OBJECT_USER_MENU, id,
					settings,
					NULL, NULL);
	if (!usermenu->priv->info) {
		gtk_widget_destroy (GTK_WIDGET (usermenu));
		g_object_unref (settings_instance);

		return;
	}

	panel_menu_bar_object_object_load_finish (PANEL_MENU_BAR_OBJECT (usermenu),
						  panel);

	usermenu->priv->settings_instance = settings_instance;

	g_signal_connect (usermenu->priv->settings_instance, "changed",
			  G_CALLBACK (panel_user_menu_settings_changed),
			  usermenu);
}

void
panel_user_menu_create (PanelToplevel       *toplevel,
		       PanelObjectPackType  pack_type,
		       int                  pack_index)
{
	panel_layout_object_create (PANEL_OBJECT_USER_MENU, NULL,
				    panel_toplevel_get_id (toplevel),
				    pack_type, pack_index);
}
