#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([nasos], [0.1], [hadi@moshayedi.net])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])

AM_PROG_AR
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC

# Checks for libraries.
AC_CHECK_LIB([SDL2], [SDL_Init], [], [AC_MSG_ERROR([SDL2 library not found])])
AC_CHECK_LIB([SDL2_gfx], [rotozoomSurface], [], [AC_MSG_ERROR([SDL2_gfx library not found])])
AC_CHECK_LIB([m], [cos], [], [AC_MSG_ERROR([math library not found])])

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h string.h unistd.h], [],
		 [AC_MSG_ERROR([some required header files were not found])])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([clock_gettime], [], [AC_MSG_ERROR([clock_gettime not found])])

AC_CONFIG_FILES([Makefile src/Makefile data/Makefile])
AC_OUTPUT
