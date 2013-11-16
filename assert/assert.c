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

/* $Id: assert/assert.c 310447 2013-09-12 09:31:47Z Yong.Teng $ */

/* include php file */
#include "Zend/zend.h"
#include "Zend/zend_API.h"
#include "Zend/zend_interfaces.h"
#include "main/php.h"
#include "main/php_main.h"
#include "ext/spl/spl_iterators.h"

/* include buession file */
#include "buession.h"
#include "assert.h"
#include "validate/validate.h"

zend_class_entry *assert_ce;

/* {{{ arginfo */
ZEND_BEGIN_ARG_INFO_EX(assert_isTrue_arguments, FALSE, FALSE, 1)
	ZEND_ARG_INFO(FALSE, expression)
	ZEND_ARG_INFO(FALSE, message)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(assert_isFalse_arguments, FALSE, FALSE, 1)
	ZEND_ARG_INFO(FALSE, expression)
	ZEND_ARG_INFO(FALSE, message)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(assert_isNull_arguments, FALSE, FALSE, 1)
	ZEND_ARG_INFO(FALSE, expression)
	ZEND_ARG_INFO(FALSE, message)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(assert_notNull_arguments, FALSE, FALSE, 1)
	ZEND_ARG_INFO(FALSE, expression)
	ZEND_ARG_INFO(FALSE, message)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(assert_hasLength_arguments, FALSE, FALSE, 1)
	ZEND_ARG_INFO(FALSE, expression)
	ZEND_ARG_INFO(FALSE, message)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(assert_isEmpty_arguments, FALSE, FALSE, 1)
	ZEND_ARG_INFO(FALSE, expression)
	ZEND_ARG_INFO(FALSE, message)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(assert_notEmpty_arguments, FALSE, FALSE, 1)
	ZEND_ARG_INFO(FALSE, expression)
	ZEND_ARG_INFO(FALSE, message)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(assert_hasText_arguments, FALSE, FALSE, 1)
	ZEND_ARG_INFO(FALSE, str)
	ZEND_ARG_INFO(FALSE, message)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(assert_isContain_arguments, FALSE, FALSE, 2)
	ZEND_ARG_INFO(FALSE, expression)
	ZEND_ARG_INFO(FALSE, search)
	ZEND_ARG_INFO(FALSE, message)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(assert_notContain_arguments, FALSE, FALSE, 2)
	ZEND_ARG_INFO(FALSE, expression)
	ZEND_ARG_INFO(FALSE, search)
	ZEND_ARG_INFO(FALSE, message)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(assert_isEqual_arguments, FALSE, FALSE, 2)
	ZEND_ARG_INFO(FALSE, expression_1)
	ZEND_ARG_INFO(FALSE, expression_2)
	ZEND_ARG_INFO(FALSE, message)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(assert_notEqual_arguments, FALSE, FALSE, 2)
	ZEND_ARG_INFO(FALSE, expression_1)
	ZEND_ARG_INFO(FALSE, expression_2)
	ZEND_ARG_INFO(FALSE, message)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(assert_isInstanceOf_arguments, FALSE, FALSE, 2)
	ZEND_ARG_INFO(FALSE, class_1)
	ZEND_ARG_INFO(FALSE, class_2)
	ZEND_ARG_INFO(FALSE, message)
ZEND_END_ARG_INFO()
/* }}} */

static inline zend_bool assert_object_isEmpty(zval *object, zend_bool *hasToString, zend_bool *instanceOfTraversable TSRMLS_DC){
	zend_class_entry *ce = NULL;
	zend_object_handlers *handlers = NULL;
	zval *temp = NULL;
	zend_bool result = TRUE;

	*hasToString = FALSE;
	*instanceOfTraversable = FALSE;

	BUESSION_COPY_ZVAL_P(temp, object);
	if(buession_convert_object_to_string(temp TSRMLS_CC) == SUCCESS){
		*hasToString = TRUE;
		if((result = Z_STRLEN_P(temp) == 0) == TRUE){
			goto property;
		}
	}else{
		property:
		ce = Z_OBJCE_P(object);
		if(instanceof_function(ce, zend_ce_traversable TSRMLS_CC) == TRUE){
			zend_object_iterator *iterator = ce->get_iterator(ce, object, 0 TSRMLS_CC);

			*instanceOfTraversable = TRUE;
			if(EG(exception)){
				goto iterator_finish;
			}

			ASSERT_OBJECT_ITERATOR_REWIND(iterator);
			if(iterator->funcs->valid(iterator TSRMLS_CC) == SUCCESS){
				result = FALSE;
				ASSERT_OBJECT_ITERATOR_DTOR(iterator);
				goto finish;
			}

			iterator_finish:
			ASSERT_OBJECT_ITERATOR_DTOR(iterator);
		}

		handlers = Z_OBJ_HT_P(object);
		if(handlers->get_properties){
			HashTable *properties = handlers->get_properties(object TSRMLS_CC);
			result = properties == NULL||zend_hash_num_elements(properties) == 0;
		}
	}

	finish:
		buession_zval_dtor(temp);
		return result;
}

static void assert_std_isTrue(zval *expression, char *message, uint message_length TSRMLS_DC){
	if(zval_is_true(expression) == FALSE){
		assert_throw_exception(message, " - this expression must be true");
	}
}

static void assert_std_isFalse(zval *expression, char *message, uint message_length TSRMLS_DC){
	if(zval_is_true(expression) == TRUE){
		assert_throw_exception(message, " - this expression must be false");
	}
}

static void assert_std_isNull(zval *expression, char *message, uint message_length TSRMLS_DC){
	if(expression&&Z_TYPE_P(expression) != IS_NULL){
		assert_throw_exception(message, " - this expression must be null");
	}
}

static void assert_std_notNull(zval *expression, char *message, uint message_length TSRMLS_DC){
	if(expression == NULL||Z_TYPE_P(expression) == IS_NULL){
		assert_throw_exception(message, " - this argument is required; it could not be null");
	}
}

static void assert_std_hasLength(zval *expression, char *message, uint message_length TSRMLS_DC){
	if(expression == NULL||Z_TYPE_P(expression) == IS_NULL){
		assert_throw_exception(message, " - this expression must have length; it could not be null");
	}else if(Z_TYPE_P(expression) == IS_STRING){
		if(Z_STRLEN_P(expression) == 0){
			assert_throw_exception(message, " - this String argument must have length; it could not be null or empty");
		}
	}else if(Z_TYPE_P(expression) == IS_ARRAY){
		if(zend_hash_num_elements(Z_ARRVAL_P(expression)) == 0){
			assert_throw_exception(message, ASSERT_ARRAY_MUST_HAVE_ELEMENT);
		}
	}else if(Z_TYPE_P(expression) == IS_OBJECT){
		zend_class_entry *ce = NULL;
		zval *temp = NULL;
		zend_bool hasToString = FALSE;
		zval *exception = NULL;

		BUESSION_COPY_ZVAL_P(temp, expression);
		if(buession_convert_object_to_string(temp TSRMLS_CC) == SUCCESS){
			hasToString = TRUE;
			if(Z_STRLEN_P(temp) == 0){
				buession_zval_dtor(temp);
				goto property;
			}

			buession_zval_dtor(temp);
			return;
		}else{
			property:
			ce = Z_OBJCE_P(expression);
			buession_zval_dtor(temp);
			if(instanceof_function(ce, zend_ce_traversable TSRMLS_CC) == TRUE){
				zend_object_iterator *iterator = ce->get_iterator(ce, expression, 0 TSRMLS_CC);

				if(EG(exception)){
					goto finish;
				}

				ASSERT_OBJECT_ITERATOR_REWIND(iterator);
				if(iterator->funcs->valid(iterator TSRMLS_CC) == SUCCESS){
					ASSERT_OBJECT_ITERATOR_DTOR(iterator);
					return;
				}

				finish:
				ASSERT_OBJECT_ITERATOR_DTOR(iterator);
			}else{
				if(hasToString == FALSE){
					exception = buession_throw_exception_error_ex(illegalargumentexception_ce, E_ERROR, ASSERT_ASSERTION_FAILED" - this Object must instanceof %s or define the \"__toString\" method", 0, zend_ce_traversable->name);
					goto end;
				}
			}
		}

		if(message == NULL&&hasToString == TRUE){
			exception = buession_throw_exception_error_ex(illegalargumentexception_ce, E_ERROR, ASSERT_OBJECT_MUST_HAVE_MEMBER" or %s::__toString() return value could not be null or empty", 0, Z_OBJCE_P(expression)->name);
		}else{
			exception = buession_throw_exception_ex(illegalargumentexception_ce, E_ERROR, ZEND_STRL(ASSERT_OBJECT_MUST_HAVE_MEMBER), 0);
		}

		end:
			if(exception){
				zval_ptr_dtor(&exception);
			}
	}else{
		buession_method_argument_invalid_exception(1, E_ERROR, "string, array, object or null", expression);
	}
}

static void assert_std_isEmpty(zval *expression, char *message, uint message_length TSRMLS_DC){
	if(expression == NULL){
	}else if(Z_TYPE_P(expression) == IS_STRING){
		if(Z_STRLEN_P(expression) != 0){
			assert_throw_exception(message, " - this String argument must be empty");
		}
	}else if(Z_TYPE_P(expression) == IS_ARRAY){
		if(zend_hash_num_elements(Z_ARRVAL_P(expression)) != 0){
			assert_throw_exception(message, " - this Array must be empty: it must be contain 0 element");
		}
	}else if(Z_TYPE_P(expression) == IS_OBJECT){
		zend_bool hasToString = FALSE;
		zend_bool instanceOfTraversable = FALSE;

		if(assert_object_isEmpty(expression, &hasToString, &instanceOfTraversable TSRMLS_CC) == FALSE){
			zval *exception = NULL;

			if(message == NULL){
				if(hasToString == TRUE){
					if(instanceOfTraversable == TRUE){
						exception = buession_throw_exception_error_ex(illegalargumentexception_ce, E_ERROR, ASSERT_OBJECT_MUST_NOT_HAVE_MEMBER" and %s::__toString() return value must be emty", 0, Z_OBJCE_P(expression)->name);
					}else{
						exception = buession_throw_exception_error_ex(illegalargumentexception_ce, E_ERROR, ASSERT_OBJECT_MUST_NOT_HAVE_PROPERTY" and %s::__toString() return value must be emty", 0, Z_OBJCE_P(expression)->name);
					}
				}else{
					if(instanceOfTraversable == TRUE){
						exception = buession_throw_exception_error(illegalargumentexception_ce, E_ERROR, ZEND_STRL(ASSERT_OBJECT_MUST_NOT_HAVE_MEMBER), 0);
					}else{
						exception = buession_throw_exception_error(illegalargumentexception_ce, E_ERROR, ZEND_STRL(ASSERT_OBJECT_MUST_NOT_HAVE_PROPERTY), 0);
					}
				}
			}else{
				exception = buession_throw_exception_error(illegalargumentexception_ce, E_ERROR, message, message_length, 0);
			}

			if(exception){
				zval_ptr_dtor(&exception);
			}
		}
	}
}

static void assert_std_notEmpty(zval *expression, char *message, uint message_length TSRMLS_DC){
	if(expression == NULL){
		assert_throw_exception(message, " - this expression must have content");
	}else if(Z_TYPE_P(expression) == IS_STRING){
		if(Z_STRLEN_P(expression) == 0){
			assert_throw_exception(message, " - this String argument could not be empty");
		}
	}else if(Z_TYPE_P(expression) == IS_ARRAY){
		if(zend_hash_num_elements(Z_ARRVAL_P(expression)) == 0){
			assert_throw_exception(message, ASSERT_ARRAY_MUST_HAVE_ELEMENT);
		}
	}else if(Z_TYPE_P(expression) == IS_OBJECT){
		zend_bool hasToString = FALSE;
		zend_bool instanceOfTraversable = FALSE;

		if(assert_object_isEmpty(expression, &hasToString, &instanceOfTraversable TSRMLS_CC) == TRUE){
			zval *exception = NULL;

			if(message == NULL){
				if(hasToString == TRUE){
					if(instanceOfTraversable == TRUE){
						exception = buession_throw_exception_error_ex(illegalargumentexception_ce, E_ERROR, ASSERT_OBJECT_MUST_HAVE_MEMBER" or %s::__toString() return value could not be empty", 0, Z_OBJCE_P(expression)->name);
					}else{
						exception = buession_throw_exception_error_ex(illegalargumentexception_ce, E_ERROR, ASSERT_OBJECT_MUST_HAVE_PROPERTY" or %s::__toString() return value could not be empty", 0, Z_OBJCE_P(expression)->name);
					}
				}else{
					if(instanceOfTraversable == TRUE){
						exception = buession_throw_exception_error(illegalargumentexception_ce, E_ERROR, ZEND_STRL(ASSERT_OBJECT_MUST_HAVE_MEMBER), 0);
					}else{
						exception = buession_throw_exception_error(illegalargumentexception_ce, E_ERROR, ZEND_STRL(ASSERT_OBJECT_MUST_HAVE_PROPERTY), 0);
					}
				}
			}else{
				exception = buession_throw_exception_error(illegalargumentexception_ce, E_ERROR, message, message_length, 0);
			}

			if(exception){
				zval_ptr_dtor(&exception);
			}
		}
	}
}

static void assert_std_hasText(const char *str, uint str_length, char *message, uint message_length TSRMLS_DC){
	if(str != NULL&&str_length != 0&&validate_isBlank_ex(str, str_length TSRMLS_CC) == FALSE){
		assert_throw_exception(message, " - this String argument must have text; it must not be null, empty, or blank, or '\t\r\n\013\014'");
	}
}

static inline zend_bool assert_array_key_exists(zval *array, char *member, uint member_length, int *key_type TSRMLS_DC){
	HashTable *ht = Z_ARRVAL_P(array);

	*key_type = HASH_KEY_IS_STRING;
	if(zend_hash_exists(ht, member, member_length + 1) == TRUE){
		return TRUE;
	}else{
		long l;
		double d;

		switch(is_numeric_string(member, member_length, &l, &d, FALSE)){
			case IS_LONG:
				index_check:
				*key_type = HASH_KEY_IS_LONG;
				return zend_hash_index_exists(ht, l);
				break;
			case IS_DOUBLE:
				l = (long) d;
				goto index_check;
				break;
			default:
				break;
		}
	}

	return FALSE;
}

static inline zend_bool assert_object_member_exists(zval *object, char *member, uint member_length, zend_bool *hasToString, zend_bool *instanceOfTraversable TSRMLS_DC){
	zend_class_entry *ce = NULL;
	zval *temp = NULL;
	zend_bool result = FALSE;

	*hasToString = FALSE;
	*instanceOfTraversable = FALSE;

	BUESSION_COPY_ZVAL_P(temp, object);
	if(buession_convert_object_to_string(temp TSRMLS_CC) == SUCCESS){
		*hasToString = TRUE;
		if(strstr(Z_STRVAL_P(temp), member) == NULL){
			buession_zval_dtor(temp);
			return TRUE;
		}
	}

	ce = Z_OBJCE_P(object);
	if(instanceof_function(ce, zend_ce_traversable TSRMLS_CC) == TRUE){
		zend_object_iterator *iterator = ce->get_iterator(ce, object, 0 TSRMLS_CC);
		char *str_key;
		uint str_key_length;
		uint index;

		*instanceOfTraversable = TRUE;
		if(EG(exception)){
			goto finish;
		}

		iterator->index = 0;
		ASSERT_OBJECT_ITERATOR_REWIND(iterator);

		if(iterator->funcs->get_current_key){
			long l = 0;
			double d;
			zend_bool compare = TRUE;

			switch(is_numeric_string(member, member_length, &l, &d, FALSE)){
				case IS_LONG:
					break;
				case IS_DOUBLE:
					l = (double) d;
					break;
				default:
					compare = FALSE;
					break;
			}

			while(iterator->funcs->valid(iterator TSRMLS_CC) == SUCCESS){
				if(EG(exception)){
					goto finish;
				}

				switch(iterator->funcs->get_current_key(iterator, &str_key, &str_key_length, &index TSRMLS_CC)){
					case HASH_KEY_IS_LONG:
						if(compare&&(result = index == l) == TRUE){
							goto finish;
						}
						break;
					case HASH_KEY_IS_STRING:
						result = str_key_length == member_length + 1&&memcmp(member, str_key, member_length) == 0;
						buession_free(str_key);
						if(result == TRUE){
							goto finish;
						}
						break;
					default:
						break;
				}

				ASSERT_OBJECT_ITERATOR_FORWARD(iterator);
			}
		}

		finish:
		ASSERT_OBJECT_ITERATOR_DTOR(iterator);
	}else{
		HashTable *properties = buession_zval_convert_to_hash(temp TSRMLS_CC);

		result = properties != NULL&&zend_hash_exists(properties, member, member_length + 1);
	}

	return result;
}

static void assert_std_isContain(zval *expression, char *search, uint search_length, char *message, uint message_length TSRMLS_DC){
	zval *exception = NULL;

	if(expression == NULL){
	}else if(Z_TYPE_P(expression) == IS_STRING){
		if(strstr(Z_STRVAL_P(expression), search) != NULL){
			if(message == NULL){
				exception = buession_throw_exception_error_ex(illegalargumentexception_ce, E_ERROR, ASSERT_STRING_MUST_NOT_CONTAIN_SUBSTRING, 0, search);
			}else{
				exception = buession_throw_exception_error(illegalargumentexception_ce, E_ERROR, message, message_length, 0);
			}
		}
	}else if(Z_TYPE_P(expression) == IS_ARRAY){
		int key_type;

		if(assert_array_key_exists(expression, search, search_length, &key_type TSRMLS_CC) == FALSE){
			if(message == NULL){
				exception = buession_throw_exception_error_ex(illegalargumentexception_ce, E_ERROR, ASSERT_ASSERTION_FAILED" - this Array argument could not contain the %s [%s]", 0, key_type == IS_LONG ? "index" : "key", search);
			}else{
				exception = buession_throw_exception_error(illegalargumentexception_ce, E_ERROR, message, message_length, 0);
			}
		}
	}else if(Z_TYPE_P(expression) == IS_OBJECT){
		zend_bool hasToString = FALSE;
		zend_bool instanceOfTraversable = FALSE;

		if(assert_object_member_exists(expression, search, search_length, &hasToString, &instanceOfTraversable TSRMLS_CC) == TRUE){
			if(message == NULL){
				if(hasToString == TRUE){
					if(instanceOfTraversable == TRUE){
						exception = buession_throw_exception_error_ex(illegalargumentexception_ce, E_ERROR, ASSERT_OBJECT_MUST_CONTAIN_MEMBER" or %s::__toString return value could not contain the substring [%s]", 0, search, search, Z_OBJCE_P(expression)->name, search);
					}else{
						exception = buession_throw_exception_error_ex(illegalargumentexception_ce, E_ERROR, ASSERT_OBJECT_MUST_CONTAIN_PROPERTY" or %s::__toString return value could not contain the substring [%s]", 0, search, Z_OBJCE_P(expression)->name, search);
					}
				}else{
					if(instanceOfTraversable == TRUE){
						exception = buession_throw_exception_error_ex(illegalargumentexception_ce, E_ERROR, ASSERT_OBJECT_MUST_CONTAIN_MEMBER, 0, search, search);
					}else{
						exception = buession_throw_exception_error_ex(illegalargumentexception_ce, E_ERROR, ASSERT_OBJECT_MUST_CONTAIN_PROPERTY, 0, search);
					}
				}
			}else{
				exception = buession_throw_exception_error(illegalargumentexception_ce, E_ERROR, message, message_length, 0);
			}
		}
	}

	if(exception){
		zval_ptr_dtor(&exception);
	}
}

static void assert_std_notContain(zval *expression, char *search, uint search_length, char *message, uint message_length TSRMLS_DC){
	zval *exception = NULL;

	if(expression == NULL){
	}else if(Z_TYPE_P(expression) == IS_STRING){
		if(strstr(Z_STRVAL_P(expression), search) == NULL){
			if(message == NULL){
				exception = buession_throw_exception_error_ex(illegalargumentexception_ce, E_ERROR, ASSERT_STRING_MUST_CONTAIN_SUBSTRING, 0, search);
			}else{
				exception = buession_throw_exception_error(illegalargumentexception_ce, E_ERROR, message, message_length, 0);
			}
		}
	}else if(Z_TYPE_P(expression) == IS_ARRAY){
		int key_type;

		if(assert_array_key_exists(expression, search, search_length, &key_type TSRMLS_CC) == TRUE){
			if(message == NULL){
				exception = buession_throw_exception_error_ex(illegalargumentexception_ce, E_ERROR, ASSERT_ASSERTION_FAILED" - this Array argument must contain the %s [%s]", 0, key_type == IS_LONG ? "index" : "key", search);
			}else{
				exception = buession_throw_exception_error(illegalargumentexception_ce, E_ERROR, message, message_length, 0);
			}
		}
	}else if(Z_TYPE_P(expression) == IS_OBJECT){
		zend_bool hasToString = FALSE;
		zend_bool instanceOfTraversable = FALSE;

		if(assert_object_member_exists(expression, search, search_length, &hasToString, &instanceOfTraversable TSRMLS_CC) == FALSE){
			if(message == NULL){
				if(hasToString == TRUE){
					if(instanceOfTraversable == TRUE){
						exception = buession_throw_exception_error_ex(illegalargumentexception_ce, E_ERROR, ASSERT_OBJECT_MUST_CONTAIN_MEMBER" or %s::__toString() return value must contain the substring [%s]", 0, search, search, Z_OBJCE_P(expression)->name, search);
					}else{
						exception = buession_throw_exception_error_ex(illegalargumentexception_ce, E_ERROR, ASSERT_OBJECT_MUST_CONTAIN_PROPERTY" or %s::__toString() return value must contain the substring [%s]", 0, search, Z_OBJCE_P(expression)->name, search);
					}
				}else{
					if(instanceOfTraversable == TRUE){
						exception = buession_throw_exception_error_ex(illegalargumentexception_ce, E_ERROR, ASSERT_OBJECT_MUST_CONTAIN_MEMBER, 0, search, search);
					}else{
						exception = buession_throw_exception_error_ex(illegalargumentexception_ce, E_ERROR, ASSERT_OBJECT_MUST_CONTAIN_PROPERTY, 0, search);
					}
				}
			}else{
				exception = buession_throw_exception_error(illegalargumentexception_ce, E_ERROR, message, message_length, 0);
			}
		}
	}

	if(exception){
		zval_ptr_dtor(&exception);
	}
}

static void assert_std_isEqual(zval *expression_1, zval *expression_2, char *message, uint message_length TSRMLS_DC){
	if(expression_1 == NULL&&expression_2 == NULL){
	}else if((expression_1 == NULL&&expression_2 != NULL)||(expression_1 != NULL&&expression_2 == NULL)){
		goto exception;
	}else{
		zval retval;

		if(compare_function(&retval, expression_1, expression_2 TSRMLS_CC) == FAILURE||Z_LVAL(retval) != 0){
			exception:
			assert_throw_exception(message, " - this argument 2 must be equal argument 1");
		}
	}
}

static void assert_std_notEqual(zval *expression_1, zval *expression_2, char *message, uint message_length TSRMLS_DC){
	if(expression_1 == NULL&&expression_2 == NULL){
		goto exception;
	}else if((expression_1 == NULL&&expression_2 != NULL)||(expression_1 != NULL&&expression_2 == NULL)){
		goto exception;
	}else{
		zval retval;

		if(compare_function(&retval, expression_1, expression_2 TSRMLS_CC) == SUCCESS&&Z_LVAL(retval) == 0){
			exception:
			assert_throw_exception(message, " - this argument 2 could not equal argument 1");
		}
	}
}

static void assert_std_isInstanceOf(zval *object, zend_class_entry *ce, char *message, uint message_length TSRMLS_DC){
	zend_class_entry *target_ce = NULL;

	assert.notNull(object, ZEND_STRL("Argument 1 to check against must not be null") TSRMLS_CC);

	switch(Z_TYPE_P(object)){
		case IS_STRING: {
				zend_class_entry **temp_target_ce;

				if(zend_lookup_class(Z_STRVAL_P(object), Z_STRLEN_P(object), &temp_target_ce TSRMLS_CC) == SUCCESS){
					target_ce = *temp_target_ce;
				}else{
					assert_isInstanceOf_ce_exception(Z_STRVAL_P(object), ce->name, message);
					return;
				}
			}
			break;
		case IS_OBJECT:
			target_ce = Z_OBJCE_P(object);
			break;
		default:
			buession_method_argument_invalid_exception(1, E_ERROR, "string or object", object);
			return;
			break;
	}

	if(ce == NULL){
		buession_method_argument_invalid_exception(1, E_ERROR, "string or object", EG(uninitialized_zval_ptr));
		return;
	}

	assert.isInstanceOf_ce(target_ce, ce, message, message_length TSRMLS_CC);
}

static void assert_std_isInstanceOf_ce(zend_class_entry *ce_1, zend_class_entry *ce_2, char *message, uint message_length TSRMLS_DC){
	if(ce_1 == NULL||instanceof_function(ce_1, ce_2 TSRMLS_CC) == FALSE){
		assert_isInstanceOf_ce_exception(ce_1 == NULL ? "null" : ce_1->name, ce_2->name, message);
	}
}

BUESSION_API assert_handlers assert = {
	assert_std_isTrue,
	assert_std_isFalse,
	assert_std_isNull,
	assert_std_notNull,
	assert_std_hasLength,
	assert_std_isEmpty,
	assert_std_notEmpty,
	assert_std_hasText,
	assert_std_isContain,
	assert_std_notContain,
	assert_std_isEqual,
	assert_std_notEqual,
	assert_std_isInstanceOf,
	assert_std_isInstanceOf_ce
};

/* {{{ private void Assert::__construct() */
static BUESSION_METHOD(assert, __construct){
}
/* }}} */

/* {{{ public void Assert::isTrue(mixed $expression[, string $message = NULL]) */
static BUESSION_METHOD(assert, isTrue){
	zval *expression;
	char *message = NULL;
	uint message_length = 0;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z|s", &expression, &message, &message_length) == SUCCESS){
		assert.isTrue(expression, message, message_length TSRMLS_CC);
	}
}
/* }}} */

/* {{{ public void Assert::isFalse(mixed $expression[, string $message = NULL]) */
static BUESSION_METHOD(assert, isFalse){
	zval *expression;
	char *message = NULL;
	uint message_length = 0;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z|s", &expression, &message, &message_length) == SUCCESS){
		assert.isFalse(expression, message, message_length TSRMLS_CC);
	}
}
/* }}} */

/* {{{ public void Assert::isNull(mixed $expression[, string $message = NULL]) */
static BUESSION_METHOD(assert, isNull){
	zval *expression;
	char *message = NULL;
	uint message_length = 0;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z|s", &expression, &message, &message_length) == SUCCESS){
		assert.isNull(expression, message, message_length TSRMLS_CC);
	}
}
/* }}} */

/* {{{ public void Assert::notNull(mixed $expression[, string $message = NULL]) */
static BUESSION_METHOD(assert, notNull){
	zval *expression;
	char *message = NULL;
	uint message_length = 0;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z|s", &expression, &message, &message_length) == SUCCESS){
		assert.notNull(expression, message, message_length TSRMLS_CC);
	}
}
/* }}} */

/* {{{ public void Assert::hasLength(string/array $expression[, string $message = NULL]) */
static BUESSION_METHOD(assert, hasLength){
	zval *expression;
	char *message = NULL;
	uint message_length = 0;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z|s", &expression, &message, &message_length) == SUCCESS){
		assert.hasLength(expression, message, message_length TSRMLS_CC);
	}
}
/* }}} */

/* {{{ public void Assert::isEmpty(mixed $expression[, string $message = NULL]) */
static BUESSION_METHOD(assert, isEmpty){
	zval *expression;
	char *message = NULL;
	uint message_length = 0;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z|s", &expression, &message, &message_length) == SUCCESS){
		assert.isEmpty(expression, message, message_length TSRMLS_CC);
	}
}
/* }}} */

/* {{{ public void Assert::notEmpty(mixed $expression[, string $message = NULL]) */
static BUESSION_METHOD(assert, notEmpty){
	zval *expression;
	char *message = NULL;
	uint message_length = 0;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z|s", &expression, &message, &message_length) == SUCCESS){
		assert.notEmpty(expression, message, message_length TSRMLS_CC);
	}
}
/* }}} */

/* {{{ public void Assert::hasText(sring $str[, string $message = NULL]) */
static BUESSION_METHOD(assert, hasText){
	char *str;
	uint str_length;
	char *message = NULL;
	uint message_length = 0;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|s", &str, &str_length, &message, &message_length) == SUCCESS){
		assert.hasText(str, str_length, message, message_length TSRMLS_CC);
	}
}
/* }}} */

/* {{{ public void Assert::isContain(mixed $expression, char *search[, string $message = NULL]) */
static BUESSION_METHOD(assert, isContain){
	zval *expression;
	char *search = NULL;
	uint search_length = 0;
	char *message = NULL;
	uint message_length = 0;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "zs|s", &expression, &search, &search_length, &message, &message_length) == SUCCESS){
		assert.isContain(expression, search, search_length, message, message_length TSRMLS_CC);
	}
}
/* }}} */

/* {{{ public void Assert::notContain(mixed $expression, char *search[, string $message = NULL]) */
static BUESSION_METHOD(assert, notContain){
	zval *expression;
	char *search = NULL;
	uint search_length = 0;
	char *message = NULL;
	uint message_length = 0;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "zs|s", &expression, &search, &search_length, &message, &message_length) == SUCCESS){
		assert.notContain(expression, search, search_length, message, message_length TSRMLS_CC);
	}
}
/* }}} */

/* {{{ public void Assert::isEqual(mixed $expression_1, mixed $expression_2[, string $message = NULL]) */
static BUESSION_METHOD(assert, isEqual){
	zval *expression_1;
	zval *expression_2;
	char *message = NULL;
	uint message_length = 0;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "zz|s", &expression_1, &expression_2, &message, &message_length) == SUCCESS){
		assert.isEqual(expression_1, expression_2, message, message_length TSRMLS_CC);
	}
}
/* }}} */

/* {{{ public void Assert::notEqual(mixed $expression_1, mixed $expression_2[, string $message = NULL]) */
static BUESSION_METHOD(assert, notEqual){
	zval *expression_1;
	zval *expression_2;
	char *message = NULL;
	uint message_length = 0;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "zz|s", &expression_1, &expression_2, &message, &message_length) == SUCCESS){
		assert.notEqual(expression_1, expression_2, message, message_length TSRMLS_CC);
	}
}
/* }}} */

/* {{{ public void Assert::isInstanceOf(Object/string $class_1, Object/string $class_2[, string $message = NULL]) */
static BUESSION_METHOD(assert, isInstanceOf){
	zval *class_1;
	zval *class_2;
	char *message = NULL;
	uint message_length = 0;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "zz|s", &class_1, &class_2, &message, &message_length) == SUCCESS){
		zend_class_entry *class_2_ce = NULL;

		switch(Z_TYPE_P(class_2)){
			case IS_STRING: {
					zend_class_entry **temp_class_2_ce;

					if(zend_lookup_class(Z_STRVAL_P(class_2), Z_STRLEN_P(class_2), &temp_class_2_ce TSRMLS_CC) == SUCCESS){
						class_2_ce = *temp_class_2_ce;
					}else{
						assert_isInstanceOf_ce_exception(Z_STRVAL_P(class_1), Z_STRVAL_P(class_2), message);
						return;
					}
				}
				break;
			case IS_OBJECT:
				class_2_ce = Z_OBJCE_P(class_2);
				break;
			default:
				buession_method_argument_invalid_exception(1, E_ERROR, "string or object", class_2);
				return;
				break;
		}

		assert.isInstanceOf(class_1, class_2_ce, message, message_length TSRMLS_CC);
	}
}
/* }}} */

/* {{{ private void Assert::__destruct() */
static BUESSION_METHOD(assert, __destruct){
}
/* }}} */

static zend_function_entry assert_methods[] = {
	BUESSION_ASSERT_ME(__construct, NULL, ZEND_ACC_PRIVATE|ZEND_ACC_CTOR)
	BUESSION_ASSERT_ME(isTrue, assert_isTrue_arguments, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	BUESSION_ASSERT_ME(isFalse, assert_isFalse_arguments, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	BUESSION_ASSERT_ME(isNull, assert_isNull_arguments, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	BUESSION_ASSERT_ME(notNull, assert_notNull_arguments, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	BUESSION_ASSERT_ME(hasLength, assert_hasLength_arguments, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	BUESSION_ASSERT_ME(isEmpty, assert_isEmpty_arguments, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	BUESSION_ASSERT_ME(notEmpty, assert_notEmpty_arguments, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	BUESSION_ASSERT_ME(hasText, assert_hasText_arguments, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	BUESSION_ASSERT_ME(isContain, assert_isContain_arguments, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	BUESSION_ASSERT_ME(notContain, assert_notContain_arguments, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	BUESSION_ASSERT_ME(isEqual, assert_isEqual_arguments, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	BUESSION_ASSERT_ME(notEqual, assert_notEqual_arguments, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	BUESSION_ASSERT_ME(isInstanceOf, assert_isInstanceOf_arguments, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	BUESSION_ASSERT_ME(__destruct, NULL, ZEND_ACC_PRIVATE|ZEND_ACC_DTOR)
	ZEND_FE_END
};

/** {{{ BUESSION_STARTUP_FUNCTION */
BUESSION_STARTUP_FUNCTION(assert){
	zend_class_entry ce;

	BUESSION_INIT_CLASS_ENTRY(ce, "Assert", assert_methods);
	assert_ce = zend_register_internal_class(&ce TSRMLS_CC);

	return SUCCESS;
}
/* }}} */

/** {{{ BUESSION_RINIT_FUNCTION */
BUESSION_RINIT_FUNCTION(assert){
	return SUCCESS;
}
/* }}} */

/** {{{ BUESSION_RSHUTDOWN_FUNCTION */
BUESSION_RSHUTDOWN_FUNCTION(assert){
	return SUCCESS;
}
/* }}} */

/** {{{ BUESSION_SHUTDOWN_FUNCTION */
BUESSION_SHUTDOWN_FUNCTION(assert){
	return SUCCESS;
}
/* }}} */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */