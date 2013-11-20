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

/* $Id: variable.c 310447 2013-08-02 16:00:37Z Yong.Teng $ */

/* include php file */
#include "Zend/zend.h"
#include "Zend/zend_API.h"
#include "Zend/zend_interfaces.h"
#include "main/php.h"
#include "main/php_main.h"
#include "ext/session/php_session.h"
#include "ext/spl/spl_iterators.h"

#if HAVE_SETLOCALE
	#include <locale.h>
#endif

/* include buession file */
#include "buession.h"
#include "variable.h"

BUESSION_API zval *buession_get_global_variables(uint type TSRMLS_DC){
	zval **result;

	#if(PHP_MAJOR_VERSION == 5)&&(PHP_MINOR_VERSION < 4)
		zend_bool jit_initialization = (PG(auto_globals_jit)&&!PG(register_globals)&&!PG(register_long_arrays));
	#else
		zend_bool jit_initialization = PG(auto_globals_jit);
	#endif

	#if HAVE_BUESSION_DEBUG
		switch(type){
			case TRACK_VARS_POST:
				zend_hash_find(&EG(symbol_table), "_POST", 6, (void **) &result);
				break;
			case TRACK_VARS_GET:
				zend_hash_find(&EG(symbol_table), "_GET", 5, (void **) &result);
				break;
			case TRACK_VARS_SESSION:
				result = &PS(http_session_vars);
				break;
			case TRACK_VARS_COOKIE:
				zend_hash_find(&EG(symbol_table), "_COOKIE", 8, (void **) &result);
				break;
			case TRACK_VARS_SERVER:
				if(jit_initialization){
					zend_is_auto_global("_SERVER", 7 TSRMLS_CC);
				}
				zend_hash_find(&EG(symbol_table), "_SERVER", 8, (void **) &result);
				break;
			case TRACK_VARS_ENV:
				if(jit_initialization){
					zend_is_auto_global("_ENV", 4 TSRMLS_CC);
				}
				result = &PG(http_globals)[TRACK_VARS_ENV];
				break;
			case TRACK_VARS_FILES:
				result = &PG(http_globals)[TRACK_VARS_FILES];
				break;
			case TRACK_VARS_REQUEST:
				if(jit_initialization){
					zend_is_auto_global("_REQUEST", 9 TSRMLS_CC);
				}
				zend_hash_find(&EG(symbol_table), "_REQUEST", 9, (void **) &result);
				break;
			case TRACK_VARS_GLOBALS:
				zend_hash_find(&EG(symbol_table), "GLOBALS", 8, (void **) &result);
				break;
			default:
				break;
		}
	#else
		switch(type){
			case TRACK_VARS_POST: case TRACK_VARS_GET: case TRACK_VARS_FILES: case TRACK_VARS_COOKIE:
				result = &PG(http_globals)[type];
				break;
			case TRACK_VARS_SESSION:
				result = &PS(http_session_vars);
				break;
			case TRACK_VARS_ENV:
				if(jit_initialization){
					zend_is_auto_global("_ENV", 4 TSRMLS_CC);
				}
				result = &PG(http_globals)[type];
				break;
			case TRACK_VARS_SERVER:
				if(jit_initialization){
					zend_is_auto_global("_SERVER", 7 TSRMLS_CC);
				}
				result = &PG(http_globals)[type];
				break;
			case TRACK_VARS_REQUEST:
				if(jit_initialization){
					zend_is_auto_global("_REQUEST", 8 TSRMLS_CC);
				}
				result = &PG(http_globals)[type];
				break;
			case TRACK_VARS_GLOBALS:
				zend_hash_find(&EG(symbol_table), "GLOBALS", 8, (void **) &result);
				break;
			default:
				break;
		}
	#endif

	if(!result||!(*result)){
		BUESSION_ARRAY_INIT(*result);
		return *result;
	}

	Z_ADDREF_PP(result);
	return *result;
}
BUESSION_API zval *buession_get_global_variable_value(uint type, char *name, uint length TSRMLS_DC){
	zval *carrier = buession_get_global_variables(type TSRMLS_CC);

	if(carrier&&Z_TYPE_P(carrier) == IS_ARRAY){
		zval **result;

		if(zend_hash_find(Z_ARRVAL_P(carrier), name, length + 1, (void **) &result) == SUCCESS){
			Z_ADDREF_PP(result);
			return *result;
		}
	}

	return NULL;
}
BUESSION_API zval *buession_get_server_variables(TSRMLS_D){
	return buession_get_global_variables(TRACK_VARS_SERVER TSRMLS_CC);
}
BUESSION_API zval *buession_get_server_value(char *name, uint length TSRMLS_DC){
	return buession_get_global_variable_value(TRACK_VARS_SERVER, name, length TSRMLS_CC);
}
BUESSION_API zval *buession_get_env_variables(TSRMLS_D){
	return buession_get_global_variables(TRACK_VARS_ENV TSRMLS_CC);
}
BUESSION_API zval *buession_get_env_value(char *name, uint length TSRMLS_DC){
	return buession_get_global_variable_value(TRACK_VARS_ENV, name, length TSRMLS_CC);
}
BUESSION_API zval *buession_get_post_variables(TSRMLS_D){
	return buession_get_global_variables(TRACK_VARS_POST TSRMLS_CC);
}
BUESSION_API zval *buession_get_post_value(char *name, uint length TSRMLS_DC){
	return buession_get_global_variable_value(TRACK_VARS_POST, name, length TSRMLS_CC);
}
BUESSION_API zval *buession_get_get_variables(TSRMLS_D){
	return buession_get_global_variables(TRACK_VARS_GET TSRMLS_CC);
}
BUESSION_API zval *buession_get_get_value(char *name, uint length TSRMLS_DC){
	return buession_get_global_variable_value(TRACK_VARS_GET, name, length TSRMLS_CC);
}
BUESSION_API zval *buession_get_request_variables(TSRMLS_D){
	return buession_get_global_variables(TRACK_VARS_REQUEST TSRMLS_CC);
}
BUESSION_API zval *buession_get_request_value(char *name, uint length TSRMLS_DC){
	return buession_get_global_variable_value(TRACK_VARS_REQUEST, name, length TSRMLS_CC);
}
BUESSION_API zval *buession_get_files_variables(TSRMLS_D){
	return buession_get_global_variables(TRACK_VARS_FILES TSRMLS_CC);
}
BUESSION_API zval *buession_get_files_value(char *name, uint length TSRMLS_DC){
	return buession_get_global_variable_value(TRACK_VARS_FILES, name, length TSRMLS_CC);
}
BUESSION_API zval *buession_get_session_variables(TSRMLS_D){
	zval *sessions = buession_get_global_variables(TRACK_VARS_SESSION TSRMLS_CC);

	if(!sessions||Z_TYPE_P(sessions) != IS_ARRAY){
		BUESSION_ARRAY_INIT(sessions);
	}

	return sessions;
}
BUESSION_API zval *buession_get_session_value(char *name, uint length TSRMLS_DC){
	zval **result;

	if(php_get_session_var(name, length, &result TSRMLS_CC) == SUCCESS){
		return *result;
	}else{
		return NULL;
	}
}
BUESSION_API zval *buession_get_cookie_variables(TSRMLS_D){
	return buession_get_global_variables(TRACK_VARS_COOKIE TSRMLS_CC);
}
BUESSION_API zval *buession_get_cookie_value(char *name, uint length TSRMLS_DC){
	return buession_get_global_variable_value(TRACK_VARS_COOKIE, name, length TSRMLS_CC);
}
BUESSION_API zval *buession_get_globals_variables(TSRMLS_D){
	return buession_get_global_variables(TRACK_VARS_GLOBALS TSRMLS_CC);
}
BUESSION_API zval *buession_get_globals_value(char *name, uint length TSRMLS_DC){
	return buession_get_global_variable_value(TRACK_VARS_GLOBALS, name, length TSRMLS_CC);
}

BUESSION_API double buession_data_size(zval *data TSRMLS_DC){
	if(data == NULL||Z_TYPE_P(data) == IS_NULL){
		return 0.0;
	}else if(Z_TYPE_P(data) == IS_ARRAY){
		return (double) zend_hash_num_elements(Z_ARRVAL_P(data));
	}else if(Z_TYPE_P(data) == IS_OBJECT){
		return (double) buession_get_object_size(data TSRMLS_CC);
	}else if(Z_TYPE_P(data) == IS_STRING){
		return (double) Z_STRLEN_P(data);
	}else if(Z_TYPE_P(data) == IS_LONG){
		return (double) Z_LVAL_P(data);
	}else if(Z_TYPE_P(data) == IS_DOUBLE){
		return Z_DVAL_P(data);
	}else{
		return data ? 1.0 : 0.0;
	}
}

BUESSION_API int buession_convert_object_to_string(zval *object TSRMLS_DC){
	zend_class_entry *ce = Z_OBJCE_P(object);

	if(ce->__tostring&&Z_OBJ_HT_P(object)->cast_object){
		zval dst;

		if(Z_OBJ_HT_P(object)->cast_object(object, &dst, IS_STRING TSRMLS_CC) == FAILURE){
			buession_object_unsupported_cast_exception(Z_OBJCE_P(object), "string");
		}else{
			zval_dtor(object);
			Z_TYPE_P(object) = IS_STRING;
			object->value = dst.value;

			return SUCCESS;
		}
	}else{
		if(Z_OBJ_HT_P(object)->get){
			zval *newobject = Z_OBJ_HT_P(object)->get(object TSRMLS_CC);

			if(Z_TYPE_P(newobject) != IS_OBJECT){
				/* for safety - avoid loop */
				zval_dtor(object);
				*object = *newobject;
				FREE_ZVAL(object);
				convert_to_string(object);

				return SUCCESS;
			}
		}
	}

	return FAILURE;
}
BUESSION_API char *buession_object_valueof(zval *data, size_t *result_length TSRMLS_DC){
	switch(Z_TYPE_P(data)){
		case IS_STRING:
			*result_length = Z_STRLEN_P(data);
			return estrndup(Z_STRVAL_P(data), Z_STRLEN_P(data));
			break;
		case IS_NULL:
			*result_length = 4;
			return estrndup("NULL", 4);
			break;
		case IS_BOOL:
			if(Z_BVAL_P(data) == TRUE){
				*result_length = 4;
				return estrndup("true", 4);
			}else{
				*result_length = 5;
				return estrndup("false", 5);
			}
			break;
		case IS_OBJECT:
			if(buession_convert_object_to_string(data TSRMLS_CC) == SUCCESS){
				if(Z_TYPE_P(data) == IS_STRING){
					*result_length = Z_STRLEN_P(data);
					return Z_STRVAL_P(data);
				}
			}

			*result_length = Z_OBJCE_P(data)->name_length;
			return estrndup(Z_OBJCE_P(data)->name, Z_OBJCE_P(data)->name_length);
			break;
		case IS_ARRAY:
			*result_length = 5;
			buession_unsupported_convert_exception("Array", "string");

			return estrndup("Array", 5);
			break;
		default: {
				zval str = *data;

				zval_copy_ctor(&str);
				convert_to_string(&str);

				*result_length = Z_STRLEN(str);

				return Z_STRVAL(str);
			}
			break;
	}

	*result_length = 0;
	return estrndup("", 0);
}

BUESSION_API zval *buession_string_convert_original(char *value TSRMLS_DC){
	return buession_string_convert_original_ex(value, strlen(value) TSRMLS_CC);
}
BUESSION_API zval *buession_string_convert_original_ex(char *value, uint value_length TSRMLS_DC){
	zval *result;

	MAKE_STD_ZVAL(result);
	if(value == NULL||(value_length == 4&&strncasecmp(value, "null", 4) == 0)){
		ZVAL_NULL(result);
	}else if(BUESSION_STR_IS_TRUE(value)){
		ZVAL_BOOL(result, TRUE);
	}else if(BUESSION_STR_IS_FALSE(value)){
		ZVAL_BOOL(result, FALSE);
	}else{
		long l;
		double d;

		switch(is_numeric_string(value, value_length, &l, &d, FALSE)){
			case IS_LONG:
				ZVAL_LONG(result, l);
				break;
			case IS_DOUBLE:
				ZVAL_DOUBLE(result, d);
				break;
			default:
				ZVAL_STRINGL(result, value, value_length, TRUE);
				break;
		}
	}

	return result;
}

static inline int buession_object_convert_iterator_to_array_hash(zend_object_iterator *iterator, HashTable *ht TSRMLS_DC){
	zval **data;
	char *key;
	uint key_length;
	ulong index;
	int key_type;

	iterator->funcs->get_current_data(iterator, &data TSRMLS_CC);
	if(EG(exception)||data == NULL||*data == NULL){
		return ZEND_HASH_APPLY_STOP;
	}

	if(iterator->funcs->get_current_key){
		key_type = iterator->funcs->get_current_key(iterator, &key, &key_length, &index TSRMLS_CC);
		if(EG(exception)){
			return ZEND_HASH_APPLY_STOP;
		}

		switch(key_type){
			case HASH_KEY_IS_STRING:
				Z_ADDREF_PP(data);
				buession_hash_add_zval_ex(ht, key, key_length, *data TSRMLS_CC);
				buession_free(key);
				break;
			case HASH_KEY_IS_LONG:
				Z_ADDREF_PP(data);
				buession_hash_index_add_zval(ht, index, *data TSRMLS_CC);
				break;
			default:
				break;
		}
	}else{
		Z_ADDREF_PP(data);
		buession_hash_next_index_add_zval(ht, *data TSRMLS_CC);
	}

	return ZEND_HASH_APPLY_KEEP;
}
static inline int buession_object_convert_to_hashtable_apply(zval **value TSRMLS_DC, int num_args, va_list args, zend_hash_key *hash_key){
	zend_object *zobj = va_arg(args, zend_object *);
	HashTable *properies = va_arg(args, HashTable *);

	if(hash_key->nKeyLength > 0){
		char *class_name;
		char *property_name;

		if(zend_check_property_access(zobj, hash_key->arKey, hash_key->nKeyLength - 1 TSRMLS_CC) == SUCCESS){
			Z_ADDREF_PP(value);
			zend_unmangle_property_name(hash_key->arKey, hash_key->nKeyLength - 1, &class_name, &property_name);
			buession_hash_update_zval(properies, property_name, *value TSRMLS_CC);
		}
	}

	return ZEND_HASH_APPLY_KEEP;
}
BUESSION_API HashTable *buession_zval_convert_to_hash(zval *var TSRMLS_DC){
	if(var){
		switch(Z_TYPE_P(var)){
			case IS_ARRAY:
				return Z_ARRVAL_P(var);
				break;
			case IS_OBJECT: {
					zend_class_entry *var_ce = Z_OBJCE_P(var);

					if(instanceof_function(var_ce, zend_ce_traversable TSRMLS_CC) == TRUE){
						HashTable *ht;

						ALLOC_HASHTABLE(ht);
						zend_hash_init(ht, 0, NULL, ZVAL_PTR_DTOR, FALSE);
						if(spl_iterator_apply(var, buession_object_convert_iterator_to_array_hash, (void *) ht TSRMLS_CC) == SUCCESS){
							return ht;
						}

						buession_hash_free(ht);
					}else{
						zend_object_handlers *handlers = Z_OBJ_HT_P(var);

						if(handlers->get_properties){
							HashTable *properties = handlers->get_properties(var TSRMLS_CC);

							if(properties){
								zend_object *zobj = zend_object_store_get_object(var TSRMLS_CC);
								HashTable *ht = NULL;

								ALLOC_HASHTABLE(ht);
								zend_hash_init(ht, zend_hash_num_elements(properties), NULL, ZVAL_PTR_DTOR, FALSE);
								zend_hash_apply_with_arguments(properties TSRMLS_CC, (apply_func_args_t) buession_object_convert_to_hashtable_apply, 2, zobj, ht);

								return ht;
							}
						}else{
							zval *exception = buession_throw_exception_error_ex(invalidpropertyexception_ce, E_ERROR, "Invalid get class [%s] properties, could not support get properties.", 0, Z_OBJCE_P(var)->name);
							zval_ptr_dtor(&exception);
						}
					}
				}
				break;
			default:
				break;
		}
	}

	return NULL;
}

BUESSION_API zend_bool buession_has_dimension(HashTable *ht, zval *offset, char **key, uint *key_length, ulong *index TSRMLS_DC){
	ulong h;

	if(Z_TYPE_P(offset) == IS_LONG){
		h = Z_LVAL_P(offset);
		long_key:
		if(index != NULL){
			*index = h;
		}
		return zend_hash_index_exists(ht, h);
	}else if(Z_TYPE_P(offset) == IS_DOUBLE){
		h = (ulong) Z_DVAL_P(offset);
		goto long_key;
	}else{
		zval temp_offset;
		zend_bool result = FALSE;

		BUESSION_CONVERT_VARIABLE_TO_STRING(offset, temp_offset);
		result = zend_hash_exists(ht, Z_STRVAL_P(offset), Z_STRLEN_P(offset) + 1);
		if(key != NULL){
			*key = estrndup(Z_STRVAL_P(offset), Z_STRLEN_P(offset));
		}
		if(key_length != NULL){
			*key_length = Z_STRLEN_P(offset);
		}
		BUESSION_CONVERT_VARIABLE_PTR_DTOR(offset, temp_offset);

		return result;
	}
}
BUESSION_API int buession_read_dimension(HashTable *ht, zval *offset, zval ***result, char **key, uint *key_length, ulong *index TSRMLS_DC){
	ulong h;

	if(Z_TYPE_P(offset) == IS_LONG){
		h = Z_LVAL_P(offset);
		long_key:
		if(index != NULL){
			*index = h;
		}
		return zend_hash_index_find(ht, h, (void **) result) == SUCCESS ? SUCCESS : HASH_KEY_IS_LONG;
	}else if(Z_TYPE_P(offset) == IS_DOUBLE){
		h = (ulong) Z_DVAL_P(offset);
		goto long_key;
	}else{
		zval temp_offset;
		int retval = HASH_KEY_IS_STRING;

		BUESSION_CONVERT_VARIABLE_TO_STRING(offset, temp_offset);
		retval = zend_hash_find(ht, Z_STRVAL_P(offset), Z_STRLEN_P(offset) + 1, (void **) result) == SUCCESS ? SUCCESS : HASH_KEY_IS_STRING;
		if(key != NULL){
			*key = estrndup(Z_STRVAL_P(offset), Z_STRLEN_P(offset));
		}
		if(key_length != NULL){
			*key_length = Z_STRLEN_P(offset);
		}
		BUESSION_CONVERT_VARIABLE_PTR_DTOR(offset, temp_offset);

		return retval;
	}
}
BUESSION_API int buession_write_dimension(HashTable *ht, zval *offset, zval *value, zend_bool overwrite, char **key, uint *key_length, ulong *index TSRMLS_DC){
	ulong h;

	Z_ADDREF_P(value);
	if(Z_TYPE_P(offset) == IS_LONG){
		h = Z_LVAL_P(offset);
		long_key:
		if(index != NULL){
			*index = h;
		}
		if(overwrite == TRUE){
			return buession_hash_index_update_zval(ht, h, value TSRMLS_CC);
		}else{
			if(zend_hash_index_exists(ht, h) == FALSE){
				return buession_hash_index_update_zval(ht, h, value TSRMLS_CC);
			}
		}
	}else if(Z_TYPE_P(offset) == IS_DOUBLE){
		h = (ulong) Z_DVAL_P(offset);
		goto long_key;
	}else{
		zval temp_offset;
		int (*fn)(HashTable *, char *, uint, zval * TSRMLS_DC) = overwrite == TRUE ? buession_hash_update_zval_ex : buession_hash_add_zval_ex;
		int retval;

		BUESSION_CONVERT_VARIABLE_TO_STRING(offset, temp_offset);
		retval = fn(ht, Z_STRVAL_P(offset), Z_STRLEN_P(offset) + 1, value TSRMLS_CC);
		if(key != NULL){
			*key = estrndup(Z_STRVAL_P(offset), Z_STRLEN_P(offset));
		}
		if(key_length != NULL){
			*key_length = Z_STRLEN_P(offset);
		}
		BUESSION_CONVERT_VARIABLE_PTR_DTOR(offset, temp_offset);

		return retval;
	}

	return FAILURE;
}
BUESSION_API int buession_unset_dimension(HashTable *ht, zval *offset, char **key, uint *key_length, ulong *index TSRMLS_DC){
	ulong h;

	if(Z_TYPE_P(offset) == IS_LONG){
		h = Z_LVAL_P(offset);
		long_key:
		if(index != NULL){
			*index = h;
		}
		return zend_hash_index_del(ht, h);
	}else if(Z_TYPE_P(offset) == IS_DOUBLE){
		h = (ulong) Z_DVAL_P(offset);
		goto long_key;
	}else{
		zval temp_offset;
		int retval;

		BUESSION_CONVERT_VARIABLE_TO_STRING(offset, temp_offset);
		retval = zend_hash_del(ht, Z_STRVAL_P(offset), Z_STRLEN_P(offset) + 1);
		if(key != NULL){
			*key = estrndup(Z_STRVAL_P(offset), Z_STRLEN_P(offset));
		}
		if(key_length != NULL){
			*key_length = Z_STRLEN_P(offset);
		}
		BUESSION_CONVERT_VARIABLE_PTR_DTOR(offset, temp_offset);

		return retval;
	}
}

BUESSION_API zend_bool buession_search_array(HashTable *ht, zval *value, zend_bool strict, uint *key_type, char **key, uint *key_length, ulong *index TSRMLS_DC){
	if(ht&&zend_hash_num_elements(ht) > 0){
		HashPosition position;
		zval **element;
		int (*is_equal_fn)(zval *, zval *, zval * TSRMLS_DC) = strict == TRUE ? is_identical_function : is_equal_function;
		zval retval;

		zend_hash_internal_pointer_reset_ex(ht, &position);
		while(zend_hash_get_current_data_ex(ht, (void **) &element, &position) == SUCCESS){
			if(is_equal_fn(&retval, value, *element TSRMLS_CC) == SUCCESS&&Z_BVAL(retval) == TRUE){
				char *_key;
				uint _key_length;
				ulong _index;
				int _key_type = zend_hash_get_current_key_ex(ht, &_key, &_key_length, &_index, key != NULL, &position);

				switch(_key_type){
					case HASH_KEY_IS_LONG:
						if(index != NULL){
							*index = _index;
						}
						break;
					case HASH_KEY_IS_STRING:
						if(key != NULL){
							*key = _key;
						}
						if(key_length != NULL){
							*key_length = _key_length;
						}
						break;
					default:
						break;
				}
				if(key_type != NULL){
					*key_type = _key_type;
				}

				return TRUE;
			}

			zend_hash_move_forward_ex(ht, &position);
		}
	}

	return FALSE;
}
BUESSION_API int buession_array_slice(HashTable *ht, uint start, int length, HashTable **result TSRMLS_DC){
	if(ht){
		uint size = zend_hash_num_elements(ht);

		if(length == -1){
			length = size;
		}

		if((int) start <= length){
			if(start + length > size){
				length = size - start;
			}

			ALLOC_HASHTABLE(*result);
			zend_hash_init(*result, length, NULL, ZVAL_PTR_DTOR, FALSE);

			if(length > 0){
				HashPosition position;
				char *key;
				uint key_length;
				ulong index;
				zval **value;
				uint pos = 0;

				zend_hash_internal_pointer_reset_ex(ht, &position);
				while(pos < start&&zend_hash_get_current_data_ex(ht, (void **) &value, &position) == SUCCESS){
					pos++;
					zend_hash_move_forward_ex(ht, &position);
				}

				while(pos < start + length&&zend_hash_get_current_data_ex(ht, (void **) &value, &position) == SUCCESS){
					Z_ADDREF_PP(value);

					switch(zend_hash_get_current_key_ex(ht, &key, &key_length, &index, FALSE, &position)){
						case HASH_KEY_IS_LONG:
							php_printf("[index: %d]\r\n", index);
							if(zend_hash_index_update(*result, index, value, sizeof(zval *), NULL) == FAILURE){
								return FAILURE;
							}
							break;
						case HASH_KEY_IS_STRING:
							if(zend_hash_update(*result, key, key_length, value, sizeof(zval *), NULL) == FAILURE){
								return FAILURE;
							}
							break;
						default:
							break;
					}

					pos++;
					zend_hash_move_forward_ex(ht, &position);
				}
			}

			return SUCCESS;
		}
	}

	return FAILURE;
}

BUESSION_API int buession_serialize(zval *var, php_serialize_data_t *var_hash, smart_str *result TSRMLS_DC){
	PHP_VAR_SERIALIZE_INIT(*var_hash);
	php_var_serialize(result, &var, var_hash TSRMLS_CC);
	PHP_VAR_SERIALIZE_DESTROY(*var_hash);

	return result->c ? SUCCESS : FAILURE;
}
BUESSION_API int buession_unserialize(char *str, uint str_length, php_unserialize_data_t *var_hash, zval **result TSRMLS_DC){
	if(!str||str_length == 0){
		return FAILURE;
	}else{
		const unsigned char *p = (const unsigned char *) str;
		zval *retval;

		//PHP_VAR_UNSERIALIZE_INIT(*var_hash);
		ALLOC_INIT_ZVAL(retval);
		if(!php_var_unserialize(&retval, &p, p + str_length, var_hash TSRMLS_CC)){
			zval *exception = NULL;

			PHP_VAR_UNSERIALIZE_DESTROY(*var_hash);
			zval_ptr_dtor(&retval);

			exception = buession_throw_exception_error_ex(illegalexception_ce, E_NOTICE, "Error at offset %ld of %d bytes", 0, ((char *) p - str), str_length);
			zval_ptr_dtor(&exception);

			return FAILURE;
		}

		if(result){
			BUESSION_COPY_ZVAL_P(*result, retval);
		}
		//zval_ptr_dtor(&retval);

		//PHP_VAR_UNSERIALIZE_DESTROY(*var_hash);

		return SUCCESS;
	}
}

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */