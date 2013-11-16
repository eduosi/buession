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

/* $Id: assert/assert.h 310447 2013-09-12 09:31:35Z Yong.Teng $ */
#ifndef BUESSION_ASSERT_H
#define BUESSION_ASSERT_H

extern zend_class_entry *assert_ce;

#define BUESSION_ASSERT_ME(method, arguments, flags) ZEND_ME(buession_assert, method, arguments, flags)

typedef struct _assert_handlers assert_handlers;

extern BUESSION_API assert_handlers assert;

struct _assert_handlers {
	void (*isTrue)(zval *expression, char *message, uint message_length TSRMLS_DC);
	void (*isFalse)(zval *expression, char *message, uint message_length TSRMLS_DC);
	void (*isNull)(zval *expression, char *message, uint message_length TSRMLS_DC);
	void (*notNull)(zval *expression, char *message, uint message_length TSRMLS_DC);
	void (*hasLength)(zval *expression, char *message, uint message_length TSRMLS_DC);
	void (*isEmpty)(zval *expression, char *message, uint message_length TSRMLS_DC);
	void (*notEmpty)(zval *expression, char *message, uint message_length TSRMLS_DC);
	void (*hasText)(const char *str, uint str_length, char *message, uint message_length TSRMLS_DC);
	void (*isContain)(zval *expression, char *search, uint search_length, char *message, uint message_length TSRMLS_DC);
	void (*notContain)(zval *expression, char *search, uint search_length, char *message, uint message_length TSRMLS_DC);
	void (*isEqual)(zval *expression_1, zval *expression_2, char *message, uint message_length TSRMLS_DC);
	void (*notEqual)(zval *expression_1, zval *expression_2, char *message, uint message_length TSRMLS_DC);
	void (*isInstanceOf)(zval *object, zend_class_entry *ce, char *message, uint message_length TSRMLS_DC);
	void (*isInstanceOf_ce)(zend_class_entry *ce_1, zend_class_entry *ce_2, char *message, uint message_length TSRMLS_DC);
};

#define ASSERT_ASSERTION_FAILED "[Assertion failed]"
#define ASSERT_ARRAY_MUST_HAVE_ELEMENT " - this Array must not be empty: it must contain at least 1 element"
#define ASSERT_OBJECT_MUST_HAVE_PROPERTY 		ASSERT_ASSERTION_FAILED" - this Object must not be empty: it must contain at least 1 property"
#define ASSERT_OBJECT_MUST_NOT_HAVE_PROPERTY 				ASSERT_ASSERTION_FAILED" - this Object must be empty: it must be contain 0 property"
#define ASSERT_OBJECT_MUST_HAVE_MEMBER	ASSERT_ASSERTION_FAILED" - this Object must not be empty: it must contain at least 1 element"
#define ASSERT_OBJECT_MUST_NOT_HAVE_MEMBER 				ASSERT_OBJECT_MUST_NOT_HAVE_PROPERTY" and could not contain the member: it must be contain 0 element"
#define ASSERT_STRING_MUST_CONTAIN_SUBSTRING ASSERT_ASSERTION_FAILED" - this String argument must contain the substring [%s]"
#define ASSERT_STRING_MUST_NOT_CONTAIN_SUBSTRING ASSERT_ASSERTION_FAILED" - this String argument could not contain the substring [%s]"
#define ASSERT_OBJECT_MUST_CONTAIN_PROPERTY 		ASSERT_ASSERTION_FAILED" - this Object argument must contain the property [%s]"
#define ASSERT_OBJECT_MUST_CONTAIN_MEMBER ASSERT_OBJECT_MUST_CONTAIN_PROPERTY" or must contain the member [%s]"
#define ASSERT_OBJECT_MUST_NOT_CONTAIN_PROPERTY 		ASSERT_ASSERTION_FAILED" - this Object argument could not contain the property [%s]"
#define ASSERT_OBJECT_MUST_NOT_CONTAIN_MEMBER ASSERT_OBJECT_MUST_CONTAIN_PROPERTY" or could not contain the member [%s]"

#define ASSERT_OBJECT_ITERATOR_REWIND(iterator)	\
	if((iterator)->funcs->rewind){	\
		(iterator)->funcs->rewind((iterator) TSRMLS_CC);	\
		if(EG(exception)){	\
			goto finish;	\
		}	\
	}

#define ASSERT_OBJECT_ITERATOR_FORWARD(iterator)	\
	(iterator)->index++;	\
	(iterator)->funcs->move_forward((iterator) TSRMLS_CC);	\
	if(EG(exception)){	\
		goto finish;	\
	}

#define ASSERT_OBJECT_ITERATOR_DTOR(iterator)	\
	if(iterator){	\
		(iterator)->funcs->dtor((iterator) TSRMLS_CC);	\
	}

#define assert_throw_exception(message, default_message){	\
	zval *exception = NULL;	\
	if(message == NULL){	\
		exception = buession_throw_exception_error(illegalargumentexception_ce, E_ERROR, ZEND_STRL(ASSERT_ASSERTION_FAILED""default_message), 0);	\
	}else{	\
		exception = buession_throw_exception_error(illegalargumentexception_ce, E_ERROR, (message), (message_length), 0);	\
	}	\
	zval_ptr_dtor(&exception);	\
}

#define assert_isInstanceOf_ce_exception(class_name_1, class_name_2, message){	\
	zval *exception = buession_throw_exception_error_ex(illegalargumentexception_ce, E_ERROR, "%sObject of class [%s] must be an instance of %s", 0, (message == NULL ? "" : message), (class_name_1), (class_name_2));	\
	zval_ptr_dtor(&exception);	\
}

BUESSION_STARTUP_FUNCTION(assert);
BUESSION_RINIT_FUNCTION(assert);
BUESSION_RSHUTDOWN_FUNCTION(assert);
BUESSION_SHUTDOWN_FUNCTION(assert);

#endif /* BUESSION_ASSERT_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */