SUBDIRS = pixmaps

noinst_LTLIBRARIES = libsystem-timezone.la
noinst_PROGRAMS = test-system-timezone

AM_CPPFLAGS =				\
	$(TZ_CFLAGS)			\
	$(DISABLE_DEPRECATED_CFLAGS)

AM_CFLAGS = $(WARN_CFLAGS)

libsystem_timezone_la_SOURCES = \
	system-timezone.c	\
	system-timezone.h
libsystem_timezone_la_LIBADD = $(TZ_LIBS)

if HAVE_LIBECAL
CALENDAR_SOURCES =		\
	calendar-client.c	\
	calendar-client.h	\
	calendar-sources.c	\
	calendar-sources.h	\
	calendar-debug.h
endif

CLOCK_SOURCES = 		\
	calendar-window.c	\
	calendar-window.h	\
	clock.c			\
	clock.h			\
	clock-face.c		\
	clock-face.h		\
	clock-location.c	\
	clock-location.h	\
	clock-location-tile.c	\
	clock-location-tile.h	\
	clock-map.c		\
	clock-map.h		\
	clock-sunpos.c		\
	clock-sunpos.h		\
	clock-utils.c		\
	clock-utils.h		\
	set-timezone.c		\
	set-timezone.h		\
	$(BUILT_SOURCES)	\
	$(CALENDAR_SOURCES)

CLOCK_CPPFLAGS =						\
	$(AM_CPPFLAGS)						\
	$(LIBPANEL_APPLET_CFLAGS)				\
	$(CLOCK_CFLAGS)						\
	-I$(srcdir)/../../libpanel-applet			\
	-I$(top_builddir)/libpanel-applet			\
	-DGNOMELOCALEDIR=\""$(prefix)/$(DATADIRNAME)/locale"\"	\
	-DBUILDERDIR=\""$(uidir)"\"				\
	-DCLOCK_MENU_UI_DIR=\""$(xmluidir)"\"			\
	-DICONDIR=\""$(datadir)/gnome-panel/pixmaps"\"		\
	-DCLOCK_EDS_ICONDIR="\"$(CLOCK_EDS_ICONDIR)\""		\
	-DGWEATHER_I_KNOW_THIS_IS_UNSTABLE

CLOCK_LDADD =						\
	../../libpanel-applet/libpanel-applet-4.la	\
	$(CLOCK_LIBS)					\
	$(LIBPANEL_APPLET_LIBS)				\
	libsystem-timezone.la				\
	-lm

test_system_timezone_SOURCES = 	\
	test-system-timezone.c
test_system_timezone_LDADD = libsystem-timezone.la

if CLOCK_INPROCESS
APPLET_IN_PROCESS = true
APPLET_LOCATION   = $(pkglibdir)/$(LIBPANEL_APPLET_API_VERSION)/libclock-applet.so

clock_appletlibdir = $(pkglibdir)/$(LIBPANEL_APPLET_API_VERSION)
clock_appletlib_LTLIBRARIES = libclock-applet.la
libclock_applet_la_SOURCES = $(CLOCK_SOURCES)
libclock_applet_la_CPPFLAGS = $(CLOCK_CPPFLAGS)
libclock_applet_la_LIBADD = $(CLOCK_LDADD)
libclock_applet_la_LDFLAGS = -module -avoid-version
libclock_applet_la_CFLAGS = $(AM_CFLAGS)
$(libclock_applet_la_OBJECTS): $(BUILT_SOURCES)
else
APPLET_IN_PROCESS = false
APPLET_LOCATION = $(libexecdir)/clock-applet

libexec_PROGRAMS = clock-applet
clock_applet_SOURCES = $(CLOCK_SOURCES)
clock_applet_CPPFLAGS = $(CLOCK_CPPFLAGS)
clock_applet_LDADD = $(CLOCK_LDADD)
clock_applet_CFLAGS = $(AM_CFLAGS)
$(clock_applet_OBJECTS): $(BUILT_SOURCES)
endif

clock-marshallers.c: clock-marshallers.list
	$(AM_V_GEN)glib-genmarshal --prefix _clock_marshal --header --body --internal $< > $@

clock-marshallers.h: clock-marshallers.list
	$(AM_V_GEN)glib-genmarshal --prefix _clock_marshal --header --internal $< > $@

clock_enum_headers =					\
	$(top_srcdir)/applets/clock/clock-utils.h

clock-typebuiltins.c: $(clock_enum_headers)
	$(AM_V_GEN)glib-mkenums \
			--fhead "#include <glib-object.h>\n" \
			--fhead "#include \"clock-typebuiltins.h\"\n\n" \
			--fprod "\n/* enumerations from \"@filename@\" */" \
			--fprod "\n#include \"@filename@\"\n" \
			--vhead "static const GEnumValue _@enum_name@_values[] = {" \
			--vprod "  { @VALUENAME@, \"@VALUENAME@\", \"@valuenick@\" }," \
			--vtail "  { 0, NULL, NULL }\n};\n\n" \
			--vtail "GType\n@enum_name@_get_type (void)\n{\n" \
			--vtail "  static GType type = 0;\n\n" \
			--vtail "  if (!type)\n" \
			--vtail "    type = g_enum_register_static (\"@EnumName@\", _@enum_name@_values);\n\n" \
			--vtail "  return type;\n}\n\n" \
		$(clock_enum_headers) > $@

clock-typebuiltins.h: $(clock_enum_headers)
	$(AM_V_GEN)glib-mkenums \
			--fhead "#ifndef __CLOCK_TYPEBUILTINS_H__\n" \
			--fhead "#define __CLOCK_TYPEBUILTINS_H__ 1\n\n" \
			--fhead "G_BEGIN_DECLS\n\n" \
			--ftail "G_END_DECLS\n\n" \
			--ftail "#endif /* __CLOCK_TYPEBUILTINS_H__ */\n" \
			--fprod "\n/* --- @filename@ --- */" \
			--eprod "#define CLOCK_TYPE_@ENUMSHORT@ @enum_name@_get_type()\n" \
			--eprod "GType @enum_name@_get_type (void);\n" \
		$(clock_enum_headers) > $@

BUILT_SOURCES =					\
	clock-marshallers.c			\
	clock-marshallers.h			\
	clock-typebuiltins.c			\
	clock-typebuiltins.h

appletdir       = $(appletsdir)
applet_in_files = org.gnome.panel.ClockApplet.panel-applet.in
applet_DATA     = $(applet_in_files:.panel-applet.in=.panel-applet)

$(applet_in_files): $(applet_in_files).in Makefile
	$(AM_V_GEN)sed \
		-e "s|\@LOCATION\@|$(APPLET_LOCATION)|" \
		-e "s|\@IN_PROCESS\@|$(APPLET_IN_PROCESS)|" \
		-e "s|\@VERSION\@|$(PACKAGE_VERSION)|" \
		$< > $@

@PANEL_INTLTOOL_PANEL_APPLET_RULE@

if !CLOCK_INPROCESS
servicedir       = $(datadir)/dbus-1/services
service_in_files = org.gnome.panel.applet.ClockAppletFactory.service.in
service_DATA     = $(service_in_files:.service.in=.service)

org.gnome.panel.applet.ClockAppletFactory.service: $(service_in_files)
	$(AM_V_GEN)sed \
		-e "s|\@LOCATION\@|$(APPLET_LOCATION)|" \
		$< > $@
endif

uidir		= $(datadir)/gnome-panel/ui
ui_DATA		= clock.ui

xmluidir 	= $(datadir)/gnome-panel/ui
xmlui_DATA	= clock-menu.xml

schemasdir       = $(GCONF_SCHEMA_FILE_DIR)
schemas_in_files = clock.schemas.in
schemas_DATA     = $(schemas_in_files:.schemas.in=.schemas)

@INTLTOOL_SCHEMAS_RULE@

if GCONF_SCHEMAS_INSTALL
install-data-local:
	if test -z "$(DESTDIR)" ; then \
		for p in $(schemas_DATA) ; do \
			GCONF_CONFIG_SOURCE=$(GCONF_SCHEMA_CONFIG_SOURCE) $(GCONFTOOL) --makefile-install-rule $(top_builddir)/applets/clock/$$p ; \
		done \
	fi
uninstall-local:
	for p in $(schema_DATA) ; do \
		GCONF_CONFIG_SOURCE=$(GCONF_SCHEMA_CONFIG_SOURCE) $(GCONFTOOL) --makefile-uninstall-rule $(top_builddir)/applets/clock/$$p ; \
	done
endif

EXTRA_DIST =						\
	org.gnome.panel.ClockApplet.panel-applet.in.in	\
	$(schemas_in_files)				\
	$(xmlui_DATA)					\
	$(ui_DATA)					\
	clock-marshallers.list				\
	$(service_in_files)

CLEANFILES =			\
	$(BUILT_SOURCES)	\
	$(applet_DATA) 		\
	$(applet_DATA).in	\
	$(service_DATA)		\
	$(schemas_DATA)		\
	$(polkit_DATA)		\
	$(dbus_services_DATA)

-include $(top_srcdir)/git.mk
