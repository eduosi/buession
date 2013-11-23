/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2011 The PHP Group                                |
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

/* $Id: constant.h 310447 2013-11-04 12:58:30 Yong.Teng $ */
#ifndef BUESSION_CONSTANT_H
#define BUESSION_CONSTANT_H

extern zend_class_entry *constant_ce;

#define BUESSION_CONSTANT_ME(method, arguments, flags) ZEND_ME(buession_constant, method, arguments, flags)

#define constant_register_ip_type_constant(type) zend_declare_class_constant_long(constant_ce, ZEND_STRL("IPV"#type), IPV##type TSRMLS_CC)
#define constant_register_ip_flags_constant(flags) zend_declare_class_constant_long(constant_ce, ZEND_STRL("IP_"#flags), IP_##flags TSRMLS_CC)

#define constant_register_isbn_type_constant(type) zend_declare_class_constant_long(constant_ce, ZEND_STRL("ISBN_"#type), ISBN_##type TSRMLS_CC)
#define constant_register_creditcard_type_constant(no, name, name_length){	\
	char *constant_name;	\
	uint constant_name_length = spprintf(&constant_name, 0, "CREDITCARD_%s", (name));	\
	zend_declare_class_constant_long(constant_ce, constant_name, constant_name_length, (no) TSRMLS_CC);	\
	buession_free(constant_name);	\
	constant_name_length = spprintf(&constant_name, 0, "CREDITCARD_%s_NAME", (name));	\
	zend_declare_class_constant_stringl(constant_ce, constant_name, constant_name_length, (name), (name_length) TSRMLS_CC);	\
	buession_free(constant_name);	\
}
#define constant_register_creditcard_types_constant(){	\
	const creditcard_entry **creditcard_entries = buession_creditcard_getAll(TSRMLS_C);	\
	const creditcard_entry *entry;	\
	char *constant_name;	\
	uint constant_name_length;	\
	uint i = 0;	\
	uint j = 0;	\
	while((entry = creditcard_entries[i++]) != NULL){	\
		constant_register_creditcard_type_constant(entry->no, entry->name, entry->name_length);	\
		for(j = 0; j < entry->lengths_size; j++){	\
			constant_name_length = spprintf(&constant_name, 0, "CREDITCARD_%s_LENGTH_%d", entry->name, entry->lengths[j]);	\
			zend_declare_class_constant_long(constant_ce, constant_name, constant_name_length, entry->lengths[i] TSRMLS_CC);	\
			buession_free(constant_name);	\
		}	\
	}	\
}

BUESSION_STARTUP_FUNCTION(constant);
BUESSION_RINIT_FUNCTION(constant);
BUESSION_RSHUTDOWN_FUNCTION(constant);
BUESSION_SHUTDOWN_FUNCTION(constant);

#endif /* BUESSION_CONSTANT_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */