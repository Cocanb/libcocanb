AUTOMAKE_OPTIONS = foreign subdir-objects

lib_LIBRARIES = libcocanb.a
libcocanb_a_SOURCES = $(top_builddir)/src/cocanb.c $(top_builddir)/include/cocanb.h
libcocanb_a_CFLAGS = -I$(top_builddir)/include -Wall

TESTS = cocanb_test
check_PROGRAMS = cocanb_test
cocanb_test_SOURCES = $(top_builddir)/tests/cocanb-basic.c $(top_builddir)/include/cocanb.h
cocanb_test_CFLAGS = @CHECK_CFLAGS@ -I$(top_builddir)/include
cocanb_test_LDADD = libcocanb.a @CHECK_LIBS@
