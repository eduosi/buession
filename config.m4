dnl $Id$
dnl config.m4 for extension buession

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

dnl PHP_ARG_WITH(buession, for buession support,
dnl Make sure that the comment is aligned:
dnl [  --with-buession             enable buession support])

dnl Otherwise use enable:

dnl PHP_ARG_ENABLE(buession, whether to enable buession support,
dnl Make sure that the comment is aligned:
dnl [  --enable-buession           enable buession support])

dnl PHP_ARG_ENABLE(buession-debug, 
dnl [  --enable-buession-debug           enable buession debug support, default no], 
dnl [  AC_DEFINE(HAVE_BUESSION_DEBUG, 1, [Enable Buession Debug Support])  ], 
dnl [  AC_DEFINE(HAVE_BUESSION_DEBUG, 0, [Disable Buession Debug Support])  ]
dnl )

PHP_ARG_ENABLE(buession, whether to enable buession support,
	[  --enable-buession           enable buession support])

PHP_ARG_ENABLE(buession-debug, enable buession debug support, default no,
	[  --enable-buession-debug          Enable Buession Debug Support])

if test "$PHP_BUESSION" != "no"; then
	AC_MSG_CHECKING([PHP version])

	tmp_version=$PHP_VERSION
	if test -z "$tmp_version"; then
		if test -z "$PHP_CONFIG"; then
			AC_MSG_ERROR([php-config not found])
		fi

		php_version=`$PHP_CONFIG --version 2>/dev/null|head -n 1|sed -e 's#\(\d\.\d*\.\d*\)\(.*\)#\1#'`
	else
		php_version=`echo "$tmp_version"|sed -e 's#\(\d\.\d*\.\d*\)\(.*\)#\1#'`
	fi

	if test -z "$php_version"; then
		AC_MSG_ERROR([Failed to detect PHP version, please report])
	fi

	ac_IFS=$IFS
  	IFS="."
  	set $php_version
  	IFS=$ac_IFS
  	buession_php_version=`expr [$]1 \* 1000000 + [$]2 \* 1000 + [$]3`
  	AC_MSG_ERROR([$buession_php_version])

  	if test "$buession_php_version" -le "5004000"; then
    	AC_MSG_ERROR([You need at least PHP 5.3.0 to be able to use this version of Buession. PHP $php_version found])
  	else
    	AC_MSG_RESULT([$php_version, ok])
  	fi

	creditcard_source = "dict/creditcard/creditcard.c \
			dict/creditcard/amerucan_express.c \
			dict/creditcard/diners_club_us.c dict/creditcard/diners_club.c dict/creditcard/discover.c \
			dict/creditcard/jcb.c \
			dict/creditcard/laser.c \
			dict/creditcard/maestro.c dict/creditcard/mastercard.c \
			dict/creditcard/solo.c \
			dict/creditcard/uniopay.c \
			dict/creditcard/visa.c"

	PHP_BUESSION_FLAGS="-DHAVE_CONFIG_H -I@ext_srcdir@/dict"
	PHP_NEW_EXTENSION(buession, [buession.c alloc.c types.c Buession_API.c variable.c regex.c hash.c objects_API.c constant.c exception.c \
		$creditcard_source \
		assert/assert.c calendar/calendar.c registry/registry.c \
		validate/validate.c], $ext_shared, ,$PHP_BUESSION_FLAGS)

	PHP_ADD_BUILD_DIR($ext_builddir/dict)
	PHP_ADD_BUILD_DIR($ext_builddir/dict/creditcard)

	AC_DEFINE(HAVE_BUESSION, 1, [Buession Support])

	if test "$PHP_BUESSION_DEBUG" = "yes"; then
		AC_DEFINE(HAVE_BUESSION_DEBUG, 1, [Enable Buession Debug Support])
	else
		AC_DEFINE(HAVE_BUESSION_DEBUG, 0, [Disable Buession Debug Support])
	fi

	PHP_SUBST(BUESSION_SHARED_LIBADD)
fi