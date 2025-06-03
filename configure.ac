
AC_INIT([MODE7],[25.6])
AC_PREFIX_DEFAULT([/opt/MODE7])

## Prevent autoconf from supplying undesirable defaults if not already set
: ${CFLAGS=""}
: ${CXXFLAGS=""}

## M4 macros are in a subdirectory named "M4"
AC_CONFIG_MACRO_DIR([BuildTools/M4])
## Build scripts are in a subdirectory named "BuildScripts"
AC_CONFIG_AUX_DIR([BuildTools/Scripts])

AM_INIT_AUTOMAKE([foreign subdir-objects])
AM_MAINTAINER_MODE

AC_PROG_CC([clang gcc cc])
AC_PROG_CXX([clang++ g++ c++])

AS_VAR_APPEND([AM_CPPFLAGS],[" -I\${top_srcdir}/PrivateHeaders"])
AS_VAR_APPEND([AM_CPPFLAGS],[" -I\${top_srcdir}/Headers"])

AC_SUBST([AM_CPPFLAGS])
AC_SUBST([AM_CXXFLAGS])

AC_MSG_CHECKING([build type])
AC_ARG_ENABLE([debug],
	[AS_HELP_STRING([--disable-debug],
		[disable debugging code (default is no)])],
	[case $withval in
		no) build_type=debug ;;
		*) build_type=release
	esac],
	[build_type=debug])
AC_MSG_RESULT([$build_type])

if test $build_type = release ; then
	AC_DEFINE_UNQUOTED([NDEBUG], [1], [Define to disable additional code to aid debugging])
fi

if test "$GCC" = "yes" ; then
	AS_VAR_APPEND([AM_CPPFLAGS],[" -W -Wall"])
	if test $build_type = debug ; then
		AS_VAR_APPEND([AM_CPPFLAGS],[" -O0 -g -Werror"])
	else
		AS_VAR_APPEND([AM_CPPFLAGS],[" -O2"])
	fi
fi

LT_INIT

AC_CONFIG_FILES([
Makefile
Examples/simple-carousel.sh
Sources/Makefile
Sources/Tools/Makefile
Sources/Tools/Common/Makefile
Sources/Tools/TeletextDemo/Makefile
Sources/Tools/TeletextServer/Makefile
Sources/Tools/MODE7/Makefile
BuildTools/Scripts/TeletextDemo.sh
BuildTools/Scripts/TeletextServer.sh
BuildTools/Scripts/MODE7.sh
])

AC_OUTPUT
