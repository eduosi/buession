/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2013 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | License: http://www.buession.com.cn/license                          |
  | Author: Yong.Teng	<webmaster@buession.com>                         |
  +----------------------------------------------------------------------+
*/

/* $Id: buession.h 310447 2011-04-23 21:14:10Z Yong.Teng $ */

#ifndef BUESSION_H
#define BUESSION_H

extern zend_module_entry buession_module_entry;
#define phpext_buession_ptr &buession_module_entry

#ifdef PHP_WIN32
#	ifdef BUESSION_EXPORTS
#		define BUESSION_API __declspec(dllexport)
#	else
#		define BUESSION_API __declspec(dllimport)
#	endif
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define BUESSION_API __attribute__ ((visibility("default")))
#else
#	define BUESSION_API
#endif

#ifdef ZTS
	#include "TSRM.h"
#endif

#include "Buession_API.h"
#include "variable.h"
#include "regex.h"
#include "hash.h"
#include "objects_API.h"
#include "constant.h"
#include "exception.h"

extern zend_class_entry *buession_ce;

#define BUESSION_ME(method, arguments, flags)	ZEND_ME(buession, method, arguments, flags)
#define BUESSION_STATIC_ME(method, arguments)	ZEND_ME(buession, method, arguments, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)

#define BUESSION 					"Buession"
#define BUESSION_MAJOR_VERSION		0
#define BUESSION_MINOR_VERSION		0
#define BUESSION_RELEASE_VERSION	1
#define BUESSION_EXTRA_VERSION		"Release"
#define BUESSION_VERSION 			"0.0.1"
#define BUESSION_BUILD				""
#define BUESSION_API_VERSION 		ZEND_TOSTR(PHP_API_VERSION)
#define BUESSION_REVISION_ID		"$Revision: "BUESSION_API_VERSION" $"
#define BUESSION_SUPPORT_URL 		"http://www.buession.com/"
#define BUESSION_SUPPORT_EMAIL 		"webmaster@buession.com"

BUESSION_API zend_bool buession_instance_exists(zend_class_entry *ce TSRMLS_DC);
BUESSION_API int buession_instance_add(zend_class_entry *ce, zval *instance TSRMLS_DC);
BUESSION_API zval *buession_instance_find(zend_class_entry *ce TSRMLS_DC);
BUESSION_API zval *buession_instance_find_ex(const char *classname, uint classname_length TSRMLS_DC);

ZEND_BEGIN_MODULE_GLOBALS(buession)
	time_t timestamp;
	char *environment;
	size_t environment_length;
	char *classpath;
	size_t classpath_length;
	buession_registry registry;
	const char *clientip;
	uint clientip_length;
	const char *scheme;
	uint scheme_length;
	zend_bool isHttp;
	char *root;
	size_t root_length;
	char *webroot;
	size_t webroot_length;
	char *charset;
	size_t charset_length;
	zend_bool secure;
ZEND_END_MODULE_GLOBALS(buession)

ZEND_EXTERN_MODULE_GLOBALS(buession)

#define BUESSION_CLEAN_STRING_G(fields){	\
	char *v = (char *) BUESSION_G(fields);	\
	buession_free(v);	\
	BUESSION_G(fields##_length) = 0;	\
}

#ifdef ZTS
	#define BUESSION_G(v) TSRMG(buession_globals_id, zend_buession_globals *, v)
#else
	#define BUESSION_G(v) (buession_globals.v)
#endif

PHP_MINIT_FUNCTION(buession);
PHP_RINIT_FUNCTION(buession);
PHP_RSHUTDOWN_FUNCTION(buession);
PHP_MSHUTDOWN_FUNCTION(buession);
PHP_MINFO_FUNCTION(buession);

#endif	/* BUESSION_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */