AC_PREREQ([2.65])
AC_INIT([purple-events], [0.99.2])

AC_CONFIG_HEADERS([include/config.h])
AC_CONFIG_SRCDIR([src/])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_AUX_DIR([.aux])

AM_INIT_AUTOMAKE([1.11 subdir-objects dist-xz no-dist-gzip tar-ustar foreign])

AM_SILENT_RULES([yes])

AC_PROG_CC
AC_PROG_CC_C99
AM_PROG_CC_C_O
AC_DISABLE_STATIC
LT_INIT
PKG_PROG_PKG_CONFIG

GETTEXT_PACKAGE=$PACKAGE_NAME
AC_DEFINE_UNQUOTED([GETTEXT_PACKAGE], ["$GETTEXT_PACKAGE"], [Gettext Package])
AC_SUBST(GETTEXT_PACKAGE)
IT_PROG_INTLTOOL([0.40.0])

AC_PROG_INSTALL


PACKAGE_DEVELOPER="sardemff7"
AC_DEFINE_UNQUOTED([PURPLE_EVENTS_PLUGIN_ID], ["core-$PACKAGE_DEVELOPER-$PACKAGE_NAME"], [libpurple plugin id])

AM_CFLAGS="-Wall -Wextra -Wno-unused-parameter"
AM_LDFLAGS=""

#
# Check for glib
#
PKG_CHECK_MODULES(GLIB, glib-2.0)

#
# Check for libpurple
#
AC_ARG_WITH(purple-plugindir, AS_HELP_STRING([--with-purple-plugindir=DIR], [Directory for libpurple plugin]), [], with_purple_plugindir=yes)
case "$with_purple_plugindir" in
    no|"") AC_MSG_ERROR([*** You must define purple-plugindir]) ;;
    yes) plugindir="`$PKG_CONFIG --variable=plugindir purple`" ;;
    *) plugindir="$with_purple_plugindir" ;;
esac
PKG_CHECK_MODULES(PURPLE, purple >= 2.6.0)
AC_DEFINE_UNQUOTED([PURPLE_DATADIR], ["`$PKG_CONFIG --variable=datadir purple`"], [libpurple data directory])
AC_SUBST(plugindir)


AC_SUBST(AM_CFLAGS)
AC_SUBST(AM_LDFLAGS)

AC_SUBST(shrext_cmds)

AC_CONFIG_FILES([
    data/purple-events.pc
    Makefile
    po/Makefile.in
])
AC_OUTPUT
