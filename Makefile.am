AM_DISTCHECK_CONFIGURE_FLAGS = \
	--disable-debug \
	--with-purple-plugindir="$$dc_install_base/$(plugindir)"

ACLOCAL_AMFLAGS = -I m4 ${ACLOCAL_FLAGS}
AM_MAKEFLAGS = --no-print-directory

SUBDIRS = po

LIBPURPLE_EVENTS_CURRENT=0
LIBPURPLE_EVENTS_REVISION=0
LIBPURPLE_EVENTS_AGE=0

pkgconfigdir = $(libdir)/pkgconfig
vapidir = $(datadir)/vala/vapi

AM_CFLAGS += \
	-DLOCALEDIR=\"$(localedir)\" \
	-I $(srcdir)/include

# Helper library
pkgconfig_DATA = \
	data/purple-events.pc

pkginclude_HEADERS = \
	include/purple-events.h

dist_vapi_DATA = \
	vapi/purple-events.deps \
	vapi/purple-events.vapi

lib_LTLIBRARIES = \
	libpurple-events.la

libpurple_events_la_SOURCES = \
	include/purple-events-handler.h \
	include/purple-events-context.h \
	include/purple-events.h \
	src/lib/utils.c \
	src/lib/handler.c \
	src/lib/context.c

libpurple_events_la_CFLAGS = \
	$(AM_CFLAGS) \
	$(PURPLE_CFLAGS) \
	$(GLIB_CFLAGS)

libpurple_events_la_LDFLAGS = \
	-no-undefined \
	-version-info $(LIBPURPLE_EVENTS_CURRENT):$(LIBPURPLE_EVENTS_REVISION):$(LIBPURPLE_EVENTS_AGE) \
	-export-dynamic

libpurple_events_la_LIBADD = \
	$(PURPLE_LIBS) \
	$(GLIB_LIBS)

# Plugin
plugin_LTLIBRARIES = \
	purple-events.la

purple_events_la_SOURCES = \
	include/config.h \
	include/purple-events-handler.h \
	include/purple-events-context.h \
	include/purple-events.h \
	src/plugin/utils.h \
	src/plugin/utils.c \
	src/plugin/ui.h \
	src/plugin/ui.c \
	src/plugin/callbacks.h \
	src/plugin/callbacks.c \
	src/plugin/events.h \
	src/plugin/events.c

purple_events_la_CFLAGS = \
	$(AM_CFLAGS) \
	$(PURPLE_CFLAGS) \
	$(GLIB_CFLAGS)

purple_events_la_LDFLAGS = \
	-no-undefined \
	-avoid-version -module \
	-export-symbols-regex 'purple_init_(purple_events_)?plugin'

purple_events_la_LIBADD = \
	$(PURPLE_LIBS) \
	$(GLIB_LIBS)

# Exec “sample” plugin
plugin_LTLIBRARIES += \
	purple-events-exec.la

purple_events_exec_la_SOURCES = \
	include/config.h \
	include/purple-events.h \
	src/plugin-exec/exec.c

purple_events_exec_la_CFLAGS = \
	$(AM_CFLAGS) \
	$(PURPLE_CFLAGS) \
	$(GLIB_CFLAGS)

purple_events_exec_la_LDFLAGS = \
	-no-undefined \
	-avoid-version -module \
	-export-symbols-regex 'purple_init_(purple_events_exec_)?plugin'

purple_events_exec_la_LIBADD = \
	libpurple-events.la \
	$(PURPLE_LIBS) \
	$(GLIB_LIBS)


#
# Hooks
#

install-data-hook: la-files-install-hook
uninstall-hook: la-files-uninstall-hook

# *.la files cleanup
la-files-install-hook:
	cd $(DESTDIR)$(plugindir) && \
		rm $(plugin_LTLIBRARIES)

# Remove shared object files at uninstall since
# we remove *.la files at install
la-files-uninstall-hook:
	cd $(DESTDIR)$(plugindir) && \
		rm $(plugin_LTLIBRARIES:.la=$(shrext_cmds))
	-rmdir $(DESTDIR)$(plugindir)
