AUTOMAKE_OPTIONS = foreign subdir-objects

lib_LIBRARIES = libcocanb.a
libcocanb_a_SOURCES = $(top_builddir)/src/core/cocanb.c $(top_builddir)/include/cocanb.h
libcocanb_a_CFLAGS = -I$(top_builddir)/include -Wall

TESTS = cocanb_test_encode_basic cocanb_test_encode_quote
check_PROGRAMS = cocanb_test_encode_basic cocanb_test_encode_quote

cocanb_test_encode_basic_SOURCES = $(top_builddir)/tests/encode-basic.c $(top_builddir)/include/cocanb.h
cocanb_test_encode_basic_CFLAGS = @CHECK_CFLAGS@ -I$(top_builddir)/include
cocanb_test_encode_basic_LDADD = libcocanb.a @CHECK_LIBS@

cocanb_test_encode_quote_SOURCES = $(top_builddir)/tests/encode-quote.c $(top_builddir)/include/cocanb.h
cocanb_test_encode_quote_CFLAGS = @CHECK_CFLAGS@ -I$(top_builddir)/include
cocanb_test_encode_quote_LDADD = libcocanb.a @CHECK_LIBS@
