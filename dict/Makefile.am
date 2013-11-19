## Process this file with automake to produce Makefile.in

LANG=C
EXTRA_DIST=AUTHORS DISCLAIMER LICENSE buildconf
SUBDIRS = creditcard doc
RUNTESTDEFAULTFLAGS=--tool $$tool --srcdir "$$srcdir"/tests
MAINTAINERCLEANFILES = aclocal.m4 config.h.in configure Makefile.in *~

dist-hook:
	cp $(srcdir)/doc/buession.1 $(distdir)/doc
	cp $(srcdir)/creditcard/creditcard.h $(distdir)/creditcard
	cp $(srcdir)/creditcard/amerucan_express.h $(distdir)/creditcard
	cp $(srcdir)/creditcard/diners_club_us.h $(distdir)/creditcard
	cp $(srcdir)/creditcard/diners_club.h $(distdir)/creditcard
	cp $(srcdir)/creditcard/discover.h $(distdir)/creditcard
	cp $(srcdir)/creditcard/jcb.h $(distdir)/creditcard
	cp $(srcdir)/creditcard/laser.h $(distdir)/creditcard
	cp $(srcdir)/creditcard/maestro.h $(distdir)/creditcard
	cp $(srcdir)/creditcard/mastercard.h $(distdir)/creditcard
	cp $(srcdir)/creditcard/solo.h $(distdir)/creditcard
	cp $(srcdir)/creditcard/uniopay.h $(distdir)/creditcard
	cp $(srcdir)/creditcard/visa.h $(distdir)/creditcard