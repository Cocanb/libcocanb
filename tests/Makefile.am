TESTS = cocanb_test_encode_basic cocanb_test_encode_quote cocanb_test_encode_number cocanb_test_encode_paren
check_PROGRAMS = cocanb_test_encode_basic cocanb_test_encode_quote cocanb_test_encode_number cocanb_test_encode_paren

cocanb_test_encode_basic_SOURCES = encode-basic.c
cocanb_test_encode_basic_CFLAGS = @CHECK_CFLAGS@ -I$(top_srcdir)/include
cocanb_test_encode_basic_LDADD = $(top_builddir)/src/libcocanb.a @CHECK_LIBS@

cocanb_test_encode_quote_SOURCES = encode-quote.c
cocanb_test_encode_quote_CFLAGS = @CHECK_CFLAGS@ -I$(top_srcdir)/include
cocanb_test_encode_quote_LDADD = $(top_builddir)/src/libcocanb.a @CHECK_LIBS@

cocanb_test_encode_number_SOURCES = encode-number.c
cocanb_test_encode_number_CFLAGS = @CHECK_CFLAGS@ -I$(top_srcdir)/include
cocanb_test_encode_number_LDADD = $(top_builddir)/src/libcocanb.a @CHECK_LIBS@

cocanb_test_encode_paren_SOURCES = encode-paren.c
cocanb_test_encode_paren_CFLAGS = @CHECK_CFLAGS@ -I$(top_srcdir)/include
cocanb_test_encode_paren_LDADD = $(top_builddir)/src/libcocanb.a @CHECK_LIBS@
