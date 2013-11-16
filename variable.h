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
  | Author: Teng Yong	<webmaster@buession.com>                         |
  +----------------------------------------------------------------------+
*/

/* $Id: variable.h 310447 2013-08-02 16:00:26Z Teng Yong $ */
#ifndef BUESSION_VARIABLE_H
#define BUESSION_VARIABLE_H

#include "ext/standard/php_var.h"

#ifndef TRACK_VARS_SESSION
	#define TRACK_VARS_SESSION 7
#endif
#ifndef TRACK_VARS_GLOBALS
	#define TRACK_VARS_GLOBALS 8
#endif

#define IS_SCALAR(data) (Z_TYPE(data) == IS_STRING||Z_TYPE(data) == IS_LONG||Z_TYPE(data) == IS_DOUBLE||Z_TYPE(data) == IS_BOOL||Z_TYPE(data) == IS_NULL)
#define IS_SCALAR_P(data) IS_SCALAR(*data)
#define IS_SCALAR_PP(data) IS_SCALAR(**data)

#define ZVAL_IS_TRUE_P(zv) ((zv)&&(Z_TYPE_P(zv) == IS_BOOL||Z_TYPE_P(zv) == IS_LONG)&&Z_LVAL_P(zv) == 1)
#define ZVAL_IS_FALSE_P(zv) ((zv)(Z_TYPE_P(zv) == IS_BOOL||Z_TYPE_P(zv) == IS_LONG)&&Z_LVAL_P(zv) == 0)
#define ZVAL_IS_TRUE_PP(zv) ((zv)&&(Z_TYPE_PP(zv) == IS_BOOL||Z_TYPE_PP(zv) == IS_LONG)&&Z_LVAL_PP(zv) == 1)
#define ZVAL_IS_FALSE_PP(zv) ((zv)(Z_TYPE_PP(zv) == IS_BOOL||Z_TYPE_PP(zv) == IS_LONG)&&Z_LVAL_PP(zv) == 0))

#define BUESSION_STR_IS_TRUE(str) ((str) != NULL&&strcasecmp((str), "true") == 0)
#define BUESSION_STR_IS_FALSE(str) ((str) != NULL&&strcasecmp((str), "false") == 0)

#define BUESSION_BOOL_TO_INT(b) ((b) == TRUE ? "1" : "0")
#define BUESSION_BOOL_TO_STR(b) ((b) == TRUE ? "true" : "false")

#define BUESSION_CONVERT_VARIABLE_TO_STRING(variable, temp_variable)	\
	if(Z_TYPE_P(variable) != IS_STRING){	\
		temp_variable = *variable;	\
		zval_copy_ctor(&temp_variable);	\
		_convert_to_string(&temp_variable ZEND_FILE_LINE_CC);	\
		variable = &temp_variable;	\
	}
#define BUESSION_CONVERT_VARIABLE_PTR_DTOR(variable, temp_variable)	\
	if(variable == &temp_variable){	\
		zval_dtor(variable);	\
	}

BUESSION_API zval *buession_get_global_variables(uint type TSRMLS_DC);
BUESSION_API zval *buession_get_global_variable_value(uint type, char *name, uint length TSRMLS_DC);
BUESSION_API zval *buession_get_server_variables(TSRMLS_D);
BUESSION_API zval *buession_get_server_value(char *name, uint length TSRMLS_DC);
BUESSION_API zval *buession_get_env_variables(TSRMLS_D);
BUESSION_API zval *buession_get_env_value(char *name, uint length TSRMLS_DC);
BUESSION_API zval *buession_get_post_variables(TSRMLS_D);
BUESSION_API zval *buession_get_post_value(char *name, uint length TSRMLS_DC);
BUESSION_API zval *buession_get_get_variables(TSRMLS_D);
BUESSION_API zval *buession_get_get_value(char *name, uint length TSRMLS_DC);
BUESSION_API zval *buession_get_request_variables(TSRMLS_D);
BUESSION_API zval *buession_get_request_value(char *name, uint length TSRMLS_DC);
BUESSION_API zval *buession_get_files_variables(TSRMLS_D);
BUESSION_API zval *buession_get_files_value(char *name, uint length TSRMLS_DC);
BUESSION_API zval *buession_get_session_variables(TSRMLS_D);
BUESSION_API zval *buession_get_session_value(char *name, uint length TSRMLS_DC);
BUESSION_API zval *buession_get_cookie_variables(TSRMLS_D);
BUESSION_API zval *buession_get_cookie_value(char *name, uint length TSRMLS_DC);
BUESSION_API zval *buession_get_globals_variables(TSRMLS_D);
BUESSION_API zval *buession_get_globals_value(char *name, uint length TSRMLS_DC);

#define buession_unsupported_convert_exception(source_type_name, target_type_name){	\
	zval *exception = buession_throw_exception_error_ex(unsupportedoperationexception_ce, E_CORE_WARNING, "%s could not be converted to %s", 0, source_type_name, (target_type_name));	\
	zval_ptr_dtor(&exception);	\
}

BUESSION_API double buession_data_size(zval *data TSRMLS_DC);

BUESSION_API int buession_convert_object_to_string(zval *object TSRMLS_DC);
BUESSION_API char *buession_object_valueof(zval *data, size_t *result_length TSRMLS_DC);

BUESSION_API zval *buession_string_convert_original(char *value TSRMLS_DC);
BUESSION_API zval *buession_string_convert_original_ex(char *value, uint value_length TSRMLS_DC);

BUESSION_API HashTable *buession_zval_convert_to_hash(zval *var TSRMLS_DC);

BUESSION_API zend_bool buession_has_dimension(HashTable *ht, zval *offset, char **key, uint *key_length, ulong *index TSRMLS_DC);
BUESSION_API int buession_read_dimension(HashTable *ht, zval *offset, zval ***result, char **key, uint *key_length, ulong *index TSRMLS_DC);
BUESSION_API int buession_write_dimension(HashTable *ht, zval *offset, zval *value, zend_bool overwrite, char **key, uint *key_length, ulong *index TSRMLS_DC);
BUESSION_API int buession_unset_dimension(HashTable *ht, zval *offset, char **key, uint *key_length, ulong *index TSRMLS_DC);

BUESSION_API zend_bool buession_search_array(HashTable *ht, zval *value, zend_bool strict, uint *key_type, char **key, uint *key_length, ulong *index TSRMLS_DC);
BUESSION_API int buession_array_slice(HashTable *ht, uint start, int length, HashTable **result TSRMLS_DC);

BUESSION_API int buession_serialize(zval *var, php_serialize_data_t *var_hash, smart_str *str TSRMLS_DC);
BUESSION_API int buession_unserialize(char *str, uint str_length, php_unserialize_data_t *var_hash, zval **result TSRMLS_DC);

#endif /* BUESSION_VARIABLE_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */