AM_CPPFLAGS =							\
	$(LIBPANEL_APPLET_CFLAGS)				\
	$(FISH_CFLAGS)						\
	-I$(srcdir)						\
	-I$(srcdir)/../../libpanel-applet			\
	-I$(top_builddir)/libpanel-applet			\
	-DGNOMELOCALEDIR=\""$(prefix)/$(DATADIRNAME)/locale"\"	\
	-DFISH_ICONDIR=\""$(fishbitmapsdir)"\"			\
	-DFISH_BUILDERDIR=\""$(uidir)"\"			\
	-DFISH_MENU_UI_DIR=\""$(xmluidir)"\" 			\
	$(DISABLE_DEPRECATED_CFLAGS)

AM_CFLAGS = $(WARN_CFLAGS)

FISH_SOURCES = fish.c

FISH_LDADD =						\
	../../libpanel-applet/libpanel-applet-4.la	\
	$(FISH_LIBS)					\
	$(LIBPANEL_APPLET_LIBS)

if FISH_INPROCESS
APPLET_IN_PROCESS = true
APPLET_LOCATION   = $(pkglibdir)/$(LIBPANEL_APPLET_API_VERSION)/libfish-applet.so

fish_applet_libdir = $(pkglibdir)/$(LIBPANEL_APPLET_API_VERSION)
fish_applet_lib_LTLIBRARIES = libfish-applet.la
libfish_applet_la_SOURCES = $(FISH_SOURCES)
libfish_applet_la_LIBADD = $(FISH_LDADD)
libfish_applet_la_LDFLAGS = -module -avoid-version
libfish_applet_la_CFLAGS = $(AM_CFLAGS)
else
APPLET_IN_PROCESS = false
APPLET_LOCATION = $(libexecdir)/fish-applet

libexec_PROGRAMS = fish-applet
fish_applet_SOURCES = $(FISH_SOURCES)
fish_applet_LDADD = $(FISH_LDADD)
fish_applet_CFLAGS = $(AM_CFLAGS)
endif

appletdir       = $(appletsdir)
applet_in_files = org.gnome.panel.FishApplet.panel-applet.in
applet_DATA     = $(applet_in_files:.panel-applet.in=.panel-applet)

$(applet_in_files): $(applet_in_files).in Makefile
	$(AM_V_GEN)sed \
		-e "s|\@LOCATION\@|$(APPLET_LOCATION)|" \
		-e "s|\@IN_PROCESS\@|$(APPLET_IN_PROCESS)|" \
		-e "s|\@VERSION\@|$(PACKAGE_VERSION)|" \
		$< > $@

@PANEL_INTLTOOL_PANEL_APPLET_RULE@

if !FISH_INPROCESS
servicedir       = $(datadir)/dbus-1/services
service_in_files = org.gnome.panel.applet.FishAppletFactory.service.in
service_DATA     = $(service_in_files:.service.in=.service)

org.gnome.panel.applet.FishAppletFactory.service: $(service_in_files)
	$(AM_V_GEN)sed \
		-e "s|\@LOCATION\@|$(APPLET_LOCATION)|" \
		$< > $@
endif

fishbitmapsdir = $(datadir)/gnome-panel/pixmaps
fishbitmaps_DATA = 	\
	wanda.fish	\
	wanda.png	\
	fishanim.fish	\
	fishanim.png	\
	oldwanda.fish	\
	oldwanda.png	\
	footguy.fish	\
	footguy.png	\
	monkey.fish	\
	monkey.png

uidir		= $(datadir)/gnome-panel/ui
ui_DATA		= fish.ui

xmluidir 	= $(datadir)/gnome-panel/ui
xmlui_DATA	= fish-menu.xml

EXTRA_DIST =						\
	wanda.svg					\
	$(fishbitmaps_DATA)				\
	org.gnome.panel.FishApplet.panel-applet.in.in	\
	$(service_in_files)				\
	$(ui_DATA)					\
	$(xmlui_DATA)

CLEANFILES = $(applet_DATA) $(applet_DATA).in $(service_DATA)

-include $(top_srcdir)/git.mk
