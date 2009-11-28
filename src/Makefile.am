AM_CPPFLAGS = -I$(top_srcdir)/lib
bin_PROGRAMS = dateadd datediff
dateadd_SOURCES = dateadd.c
datediff_SOURCES = datediff.c
datediff_CFLAGS = -Wmissing-field-initializers -Wextra
LDADD = ../lib/libdateutils.a $(LIB_CLOCK_GETTIME)
