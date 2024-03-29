/* Gnome panel: Initialization routines
 * (C) 1997,1998,1999,2000 the Free Software Foundation
 * (C) 2000 Eazel, Inc.
 *
 * Authors: Federico Mena
 *          Miguel de Icaza
 *          George Lebl
 */

#include <config.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>

#include <glib/gi18n.h>

#include <libegg/eggdesktopfile.h>
#include <libegg/eggsmclient.h>

#include <libpanel-util/panel-cleanup.h>
#include <libpanel-util/panel-glib.h>

#include "panel-shell.h"
#include "panel-multiscreen.h"
#include "panel-session.h"
#include "panel-stock-icons.h"
#include "panel-action-protocol.h"
#include "panel-icon-names.h"
#include "panel-layout.h"
#include "xstuff.h"

#include "nothing.cP"

/* globals */
GSList *panels = NULL;
GSList *panel_list = NULL;

static gboolean  replace = FALSE;
static gint      screen = 0;

static const GOptionEntry options[] = {
  { "replace", 0, 0, G_OPTION_ARG_NONE, &replace, N_("Replace a currently running panel"), NULL },
  { "screen", 0, G_OPTION_FLAG_HIDDEN, G_OPTION_ARG_INT, &screen, "Ignored - for compatibility", "num"},
  { NULL }
};

int
main (int argc, char **argv)
{
	char           *desktopfile;
	GOptionContext *context;
	GError         *error;

	bindtextdomain (GETTEXT_PACKAGE, GNOMELOCALEDIR);
	bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
	textdomain (GETTEXT_PACKAGE);

	/* We will register explicitly when we're ready -- see panel-session.c */
	egg_sm_client_set_mode (EGG_SM_CLIENT_MODE_DISABLED);

	g_set_prgname ("gnome-panel");

	desktopfile = panel_g_lookup_in_applications_dirs ("gnome-panel.desktop");
	if (desktopfile) {
		egg_set_desktop_file (desktopfile);
		g_free (desktopfile);
	}

	context = g_option_context_new ("");
	g_option_context_add_group (context,
				    egg_sm_client_get_option_group ());
	g_option_context_add_group (context, gtk_get_option_group (TRUE));
	g_option_context_add_main_entries (context, options, GETTEXT_PACKAGE);

	gtk_init (&argc, &argv);

	error = NULL;
	if (!g_option_context_parse (context, &argc, &argv, &error)) {
		g_printerr ("%s\n", error->message);
		g_error_free (error);
		g_option_context_free (context);

		return 1;
	}

	g_option_context_free (context);

	if (!egg_get_desktop_file ()) {
		g_set_application_name (_("Panel"));
		gtk_window_set_default_icon_name (PANEL_ICON_PANEL);
	}

	if (!panel_shell_register (replace)) {
		panel_cleanup_do ();
		return 1;
	}

	panel_action_protocol_init ();
	panel_multiscreen_init ();
	panel_init_stock_icons_and_items ();

	if (!panel_layout_load ()) {
		panel_cleanup_do ();
		return 1;
	}

	xstuff_init ();

	/* Flush to make sure our struts are seen by everyone starting
	 * immediate after (eg, the nautilus desktop). */
	gdk_flush ();

	/* Do this at the end, to be sure that we're really ready when
	 * connecting to the session manager */
	panel_session_init ();

	gtk_main ();

	panel_cleanup_do ();

	return 0;
}
