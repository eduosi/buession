## Process this file with automake to produce Makefile.in

SUBDIRS = src doc

MAINTAINERCLEANFILES = aclocal.m4 config.h.in configure Makefile.in \
			stamp-h.in *~

dist-hook:
	cp $(srcdir)/doc/buession.1 $(distdir)/doc
	cp $(srcdir)/src/creditcard.h $(distdir)/src
	cp $(srcdir)/src/amerucan_express.h $(distdir)/src
	cp $(srcdir)/src/diners_club_us.h $(distdir)/src
	cp $(srcdir)/src/diners_club.h $(distdir)/src
	cp $(srcdir)/src/discover.h $(distdir)/src
	cp $(srcdir)/src/jcb.h $(distdir)/src
	cp $(srcdir)/src/laser.h $(distdir)/src
	cp $(srcdir)/src/maestro.h $(distdir)/src
	cp $(srcdir)/src/mastercard.h $(distdir)/src
	cp $(srcdir)/src/solo.h $(distdir)/src
	cp $(srcdir)/src/uniopay.h $(distdir)/src
	cp $(srcdir)/src/visa.h $(distdir)/src
	cp $(srcdir)/FAQ $(distdir)