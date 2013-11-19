# Makefile for creditcard library

lib_LIBRARIES = creditcard.a

include_HEADERS = creditcard.h \
	amerucan_express.h \
	diners_club_us.h diners_club.c discover.h \
	jcb.h \
	laser.h \
	maestro.h mastercard.h \
	solo.h \
	uniopay.h \
	visa.h

creditcard_a_SOURCES = creditcard.c \
	amerucan_express.c \
	diners_club_us.c diners_club.c discover.c \
	jcb.c \
	laser.c \
	maestro.c mastercard.c \
	solo.c \
	uniopay.c \
	visa.c

INCLUDES = -I$(srcdir) -I..

CFLAGS = @CFLAGS@
CPPFLAGS = $(INCLUDES) -Wall

MAINTAINERCLEANFILES= Makefile.in $(libbcmath_a_SOURCES) creditcard.h amerucan_express.h diners_club_us.h diners_club.c discover.h jcb.h laser.h maestro.h mastercard.h solo.h uniopay.h visa.h

all: $(bin_LIBRARIES)

clean:
	rm -f $(OBJS) $(LIB) *~