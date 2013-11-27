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

/* $Id: helper/form.h 310447 2013-10-11 19:02:10Z Yong.Teng $ */
#ifndef BUESSION_HELPER_FORM_H
#define BUESSION_HELPER_FORM_H

extern zend_class_entry *helper_form_ce;

#define BUESSION_HELPER_FORM_ME(method, arguments, flags) ZEND_ME(buession_helper_form, method, arguments, flags)
#define BUESSION_HELPER_FORM_STATIC_ME(method, arguments) ZEND_ME(buession_helper_form, method, arguments, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)

#define helper_form_create_attributes_from_source(target, source)	\
	do{	\
		if(source != NULL){	\
			ALLOC_HASHTABLE(target);	\
			zend_hash_init(target, zend_hash_num_elements(source), NULL, ZVAL_PTR_DTOR, FALSE);	\
			zend_hash_copy(target, source, (copy_ctor_func_t) zval_add_ref, NULL, sizeof(zval *));	\
		}	\
	}while(0);

#define helper_form_create_input(str, type, type_length, id, id_length, name, name_length, value, value_length, attributes)	\
	do{	\
		smart_str_appendl(str, "<input", sizeof("<input") - 1);	\
		if(id&&id_length > 0){	\
			smart_str_appendl(str, " id=\"", sizeof(" id=\"") - 1);	\
			helper_html_append_encode_value(str, id, id_length);	\
			smart_str_appendl(str, "\"", 1);	\
		}	\
		if(name&&name_length > 0){	\
			smart_str_appendl(str, " name=\"", sizeof(" name=\"") - 1);	\
			helper_html_append_encode_value(str, name, name_length);	\
			smart_str_appendl(str, "\"", 1);	\
		}	\
		smart_str_appendl(str, " type=\"", sizeof(" type=\"") - 1);	\
		smart_str_appendl(str, type, type_length);	\
		smart_str_appendl(str, "\"", 1);	\
		if(attributes){	\
			zend_hash_del(attributes, "id", 3);	\
			zend_hash_del(attributes, "name", 5);	\
			zend_hash_del(attributes, "type", 5);	\
			zend_hash_del(attributes, "value", 6);	\
			helper_html_add_attributes(str, attributes TSRMLS_CC);	\
		}	\
		if(value){	\
			smart_str_appendl(str, " value=\"", sizeof(" value=\"") - 1);	\
			helper_html_append_encode_value(str, value, value_length);	\
			smart_str_appendl(str, "\"", 1);	\
		}	\
		smart_str_appendl(str, " />", 3);	\
	}while(0);

#define helper_form_create_option(str, value, value_length, text, text_length, attributes)	\
	do{	\
		smart_str_appendl(str, "<option", sizeof("<option") - 1);	\
		if(value){	\
			buession_hash_update_stringl_ex((attributes), "value", 6, (value), (value_length), TRUE TSRMLS_CC);	\
		}	\
		helper_html_add_attributes((str), (attributes) TSRMLS_CC);	\
		smart_str_appendl(str, ">", 1);	\
		if(text&&text_length > 0){	\
			helper_html_append_encode_value(str, text, text_length);	\
		}	\
		smart_str_appendl(str, "</option>", sizeof("</option>") - 1);	\
	}while(0);

BUESSION_STARTUP_FUNCTION(helper_form);
BUESSION_RINIT_FUNCTION(helper_form);
BUESSION_RSHUTDOWN_FUNCTION(helper_form);
BUESSION_SHUTDOWN_FUNCTION(helper_form);

#endif /* BUESSION_HELPER_FORM_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
