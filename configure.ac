AC_PREREQ([2.69])
AC_INIT([game-of-decay], [0.0.0], [b.heden@gmail.com])

AC_PROG_CC

AC_CHECK_HEADERS([stdlib.h])
AC_CHECK_HEADERS([string.h])
AC_CHECK_HEADERS([getopt.h])
AC_CHECK_HEADERS([time.h])
AC_CHECK_HEADERS([unistd.h])

AC_TYPE_SIZE_T

AC_FUNC_MALLOC

AC_CHECK_FUNCS([strdup])
AC_CHECK_FUNCS([snprintf])

AC_ARG_ENABLE(
    debugging,
    AC_HELP_STRING(
        [--enable-debugging],
        [Pass debugging flags to the compiler]
    ),
    [CFLAGS="${CFLAGS} -O0 -g3"]
)

AC_ARG_ENABLE(
    coverage,
    AC_HELP_STRING(
        [--enable-coverage],
        [Pass coverage flags to the compiler]
    ),
    [CFLAGS="${CFLAGS} -O0 -g3 -fprofile-arcs -ftest-coverage"]
)

AC_CONFIG_FILES([Makefile])
AC_CONFIG_FILES([src/Makefile])
AC_CONFIG_FILES([test/Makefile])

AC_OUTPUT
