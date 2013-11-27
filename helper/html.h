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

/* $Id: helper/html.h 310447 2013-10-11 12:16:26Z Yong.Teng $ */
#ifndef BUESSION_HELPER_HTML_H
#define BUESSION_HELPER_HTML_H

extern zend_class_entry *helper_html_ce;

#define BUESSION_HELPER_HTML_ME(method, arguments, flags) ZEND_ME(buession_helper_html, method, arguments, flags)
#define BUESSION_HELPER_HTML_STATIC_ME(method, arguments) ZEND_ME(buession_helper_html, method, arguments, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)

#define HELPER_HTML_W3C_DOMAIN 		"www.w3.org"
#define HELPER_HTML_WAPFORM_DOMAIN 	"www.wapform.org"

BUESSION_API void helper_html_add_attributes(smart_str *str, HashTable *attributes TSRMLS_DC);

#define helper_html_append_encode_value(str, value, value_length)	\
	do{	\
		if(value&&value_length > 0){	\
			char *val = NULL;	\
			uint val_length = 0;	\
			val = php_escape_html_entities_ex((value), (value_length), &val_length, 0, ENT_COMPAT, NULL, TRUE TSRMLS_CC);	\
			smart_str_appendl(str, val, val_length);	\
			buession_free(val);	\
		}	\
	}while(0);

#define HELPER_HTML_RETURN(str)	\
	smart_str_0(&str);	\
	BUESSION_RETURN_STRINGL(str.c, str.len, FALSE);


BUESSION_STARTUP_FUNCTION(helper_html);
BUESSION_RINIT_FUNCTION(helper_html);
BUESSION_RSHUTDOWN_FUNCTION(helper_html);
BUESSION_SHUTDOWN_FUNCTION(helper_html);

#endif /* BUESSION_HELPER_HTML_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
