# -*- autoconf -*-
AC_INIT([dateadd],
        [0.1.0],
        [alan.falloon@gmail.com])

AC_CONFIG_SRCDIR([src/dateadd.c])

AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_HEADERS([include/config.h])

AM_INIT_AUTOMAKE([-Wall -Werror foreign])

AC_PROG_CC_STDC
gl_EARLY
AM_PROG_CC_C_O
AC_PROG_CPP
AC_PROG_GCC_TRADITIONAL
AC_PROG_RANLIB
AC_PROG_LN_S
gl_INIT
AC_CONFIG_FILES(
  Makefile
  lib/Makefile
  src/Makefile
  )
AC_OUTPUT
