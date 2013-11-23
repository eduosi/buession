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

/* $Id: Buession_API.h 310447 2013-09-10 13:25:35Z Yong.Teng $ */
#ifndef BUESSION_BUESSION_API_H
#define BUESSION_BUESSION_API_H

/* include php file */
#include "ext/standard/php_smart_str_public.h"
#include "ext/standard/php_smart_str.h"
#include "ext/standard/php_string.h"

/* include buession common header file */
#include "alloc.h"
#include "types.h"

#ifndef E_INFO
	#define E_INFO (9<<1L)
#endif

#ifndef E_DEBUG
	#define E_DEBUG (9<<2L)
#endif

#define BUESSION_EXTERN_MODULE_GLOBALS(module) 	ZEND_EXTERN_MODULE_GLOBALS(buession_##module)
#define BUESSION_DECLARE_MODULE_GLOBALS(module)	ZEND_DECLARE_MODULE_GLOBALS(buession_##module)
#define BUESSION_BEGIN_MODULE_GLOBALS(module) 	ZEND_BEGIN_MODULE_GLOBALS(buession_##module)
#define BUESSION_END_MODULE_GLOBALS(module) 	ZEND_END_MODULE_GLOBALS(buession_##module)
#define BUESSION_MODULE_GLOBALS_CTOR(module)	void module##_globals_ctor(zend_buession_##module##_globals *buession_##module##_globals TSRMLS_DC)
#define BUESSION_MODULE_GLOBALS_DTOR(module)	void module##_globals_dtor(zend_buession_##module##_globals *buession_##module##_globals TSRMLS_DC)

#define BUESSION_STARTUP(module)	 		ZEND_MODULE_STARTUP_N(buession_##module)(INIT_FUNC_ARGS_PASSTHRU)
#define BUESSION_RSTARTUP(module)	 		ZEND_MODULE_ACTIVATE_N(buession_##module)(INIT_FUNC_ARGS_PASSTHRU)
#define BUESSION_RSHUTDOWN(module)	 		ZEND_MODULE_DEACTIVATE_N(buession_##module)(SHUTDOWN_FUNC_ARGS_PASSTHRU)
#define BUESSION_SHUTDOWN(module)	 	    ZEND_MODULE_SHUTDOWN_N(buession_##module)(SHUTDOWN_FUNC_ARGS_PASSTHRU)
#define BUESSION_STARTUP_FUNCTION(module)   ZEND_MODULE_STARTUP_D(buession_##module)
#define BUESSION_RINIT_FUNCTION(module)		ZEND_MODULE_ACTIVATE_D(buession_##module)
#define BUESSION_RSHUTDOWN_FUNCTION(module)	ZEND_MODULE_DEACTIVATE_D(buession_##module)
#define BUESSION_SHUTDOWN_FUNCTION(module)  	ZEND_MODULE_SHUTDOWN_D(buession_##module)

#define BUESSION_STORE_EG_ENVIRON(){ \
	zval **old_return_value_ptr_ptr = EG(return_value_ptr_ptr); \
	zend_op **old_opline_ptr = EG(opline_ptr); \
	zend_op_array *old_active_op_array = EG(active_op_array);

#define BUESSION_RESTORE_EG_ENVIRON() \
		EG(return_value_ptr_ptr) = old_return_value_ptr_ptr;\
		EG(opline_ptr) = old_opline_ptr; \
		EG(active_op_array) = old_active_op_array; \
	}

BUESSION_API int loader(char *path, uint path_length, zend_bool use_path TSRMLS_DC);

#define BUESSION_CLASS_NA(classname) 	Buession\\classname
#define BUESSION_CLASS_NAME(classname) 	ZEND_NS_NAME(BUESSION, classname)
#define BUESSION_INIT_CLASS_ENTRY(ce, classname, methods) 	INIT_NS_CLASS_ENTRY(ce, BUESSION, classname, methods)
#define BUESSION_REGISTER_CLASS_ALIAS(ce, classname)		zend_register_ns_class_alias(BUESSION, classname, ce)

#define BUESSION_METHOD(classname, name) 								ZEND_METHOD(buession_##classname, name)
#define BUESSION_ARG_OBJ_INFO(pass_by_ref, name, classname, allow_null) ZEND_ARG_OBJ_INFO(pass_by_ref, name, classname, allow_null)

#ifndef ZEND_ABSTRACT_MALIAS
	#define ZEND_ABSTRACT_MALIAS(classname, name, alias, arguments)	ZEND_MALIAS(classname, name, alias, arguments, ZEND_ACC_PUBLIC|ZEND_ACC_ABSTRACT)
#endif

BUESSION_API void buession_fcall_info_init(zval *method, zend_fcall_info *fci, zend_fcall_info_cache *fcic TSRMLS_DC);

BUESSION_API int buession_call_function(char *function, uint function_length, uint argument_count, zval **arguments[], zval **retval TSRMLS_DC);
#define buession_call_function_0(name, name_length, retval, result){	\
	result = buession_call_function((name), (name_length), 0, NULL, (retval) TSRMLS_CC);	\
}
#define buession_call_function_1(name, name_length, arg, retval, result){	\
	zval **arguments[1] = {&arg};	\
	result = buession_call_function((name), (name_length), 1, arguments, (retval) TSRMLS_CC);	\
}
#define buession_call_function_2(name, name_length, arg_1, arg_2, retval, result){	\
	zval **arguments[2] = {&arg_1, &arg_2};	\
	result = buession_call_function((name), (name_length), 2, arguments, (retval) TSRMLS_CC);	\
}
#define buession_call_function_3(name, name_length, arg_1, arg_2, arg_3, retval, result){	\
	zval **arguments[3] = {&arg_1, &arg_2, &arg_3};	\
	result = buession_call_function((name), (name_length), 3, arguments, (retval) TSRMLS_CC);	\
}
#define buession_call_function_4(name, name_length, arg_1, arg_2, arg_3, arg_4, retval, result){	\
	zval **arguments[4] = {&arg_1, &arg_2, &arg_3, &arg_4};	\
	result = buession_call_function((name), (name_length), 4, arguments, (retval) TSRMLS_CC);	\
}
#define buession_call_function_5(name, name_length, arg_1, arg_2, arg_3, arg_4, arg_5, retval, result){	\
	zval **arguments[5] = {&arg_1, &arg_2, &arg_3, &arg_4, &arg_5};	\
	result = buession_call_function((name), (name_length), 5, arguments, (retval) TSRMLS_CC);	\
}

#define BUESSION_RETVAL_STRING(s, duplicate)	\
	if(return_value_used){	\
		if(s == NULL){	\
			RETVAL_NULL();	\
		}else{	\
			RETVAL_STRING(s, duplicate);	\
		}	\
	}
#define BUESSION_RETVAL_STRINGL(s, l, duplicate)	\
	if(return_value_used){	\
		if(s == NULL){	\
			RETVAL_NULL();	\
		}else{	\
			RETVAL_STRINGL(s, l, duplicate);	\
		}	\
	}
#define BUESSION_RETVAL_HASHTABLE(ht){	\
	HashTable *_ht = (ht);	\
	if(return_value_used){	\
		if(ht == NULL){	\
			RETVAL_NULL();	\
		}else{	\
			array_init_size(return_value, zend_hash_num_elements(_ht));	\
			zend_hash_copy(Z_ARRVAL_P(return_value), _ht, (copy_ctor_func_t) zval_add_ref, NULL, sizeof(zval *));	\
		}	\
	}	\
}
#define BUESSION_RETVAL_ZVAL(v, copy, dtor)	\
	if(return_value_used){	\
		if(z == NULL){	\
			RETVAL_NULL();	\
		}else{	\
			RETVAL_ZVAL(v, copy, dtor);	\
		}	\
	}

#define BUESSION_RETURN_STRING(s, duplicate){	\
	if(return_value_used){	\
		if(s == NULL){	\
			RETURN_NULL();	\
		}else{	\
			RETURN_STRING(s, duplicate);	\
		}	\
	}	\
}
#define BUESSION_RETURN_STRINGL(s, l, duplicate){	\
	if(return_value_used){	\
		if(s == NULL){	\
			RETURN_NULL();	\
		}else{	\
			RETURN_STRINGL(s, l, duplicate);	\
		}	\
	}	\
}
#define BUESSION_RETURN_HASHTABLE(ht){	\
	BUESSION_RETVAL_HASHTABLE(ht);	\
	return;	\
}
#define BUESSION_RETURN_ZVAL(z, copy, dtor){	\
	if(z == NULL){	\
		RETURN_NULL();	\
	}else{	\
		RETURN_ZVAL(z, copy, dtor);	\
	}	\
}

#ifndef smart_str_append_double
	#define smart_str_append_double(dest, d)	\
		do{	\
			char *temp;	\
			uint temp_length = spprintf(&temp, 0, "%g", d);	\
			smart_str_appendl(dest, temp, temp_length);	\
			buession_free(temp);	\
		}while(0);
#endif

/* env function start */
typedef struct _port_entity {
	const char *scheme;
	uint scheme_length;
	zend_ushort value;
} port_entity;

BUESSION_API int buession_put_env(char *name, uint name_length, char *value, uint value_length TSRMLS_DC);
BUESSION_API char *buession_get_env(char *name, uint name_length, uint *value_length TSRMLS_DC);

#define buession_get_sapi_module_name_length() sapi_module.name ? strlen(sapi_module.name) : 0

BUESSION_API char *buession_get_scheme(uint *scheme_length TSRMLS_DC);
BUESSION_API zend_bool buession_is_ssl(char *scheme, uint scheme_length TSRMLS_DC);

BUESSION_API zend_bool buession_is_http(TSRMLS_D);
BUESSION_API zend_bool buession_is_cli(TSRMLS_D);
BUESSION_API zend_bool buession_is_cgi(TSRMLS_D);

BUESSION_API zend_bool buession_is_default_port(const char *scheme, uint scheme_length, uint port TSRMLS_DC);
BUESSION_API zend_ushort buession_get_default_port(const char *scheme, uint scheme_length TSRMLS_DC);

BUESSION_API const char *buession_get_request_method(uint *method_length TSRMLS_DC);

BUESSION_API char *buession_get_client_ip(uint *ip_length TSRMLS_DC);
/* env function end */

BUESSION_API zend_bool buession_extension_loaded(const char *name TSRMLS_DC);
BUESSION_API zend_bool buession_extension_loaded_ex(const char *name, uint length TSRMLS_DC);

BUESSION_API void buession_json_encode(zval *data, smart_str *result TSRMLS_DC);

BUESSION_API int buession_hashcode(char *str, char *algo, char **result, uint *result_length TSRMLS_DC);
BUESSION_API int buession_hashcode_ex(char *str, uint str_length, char *algo, uint algo_length, char **result, uint *result_length TSRMLS_DC);

/* file system object start */
BUESSION_API zend_bool buession_check_folder_exists(char *path, uint path_length TSRMLS_DC);
BUESSION_API zend_bool buession_check_file_exists(char *path, uint path_length TSRMLS_DC);

BUESSION_API int buession_get_fso_real_path(char *path, uint path_length, char **real_path, uint *real_path_length TSRMLS_DC);
BUESSION_API int buession_get_fso_parent_path(char *path, uint path_length, char **parent_path, uint *parent_path_length TSRMLS_DC);

BUESSION_API int buession_folder_create(char *path, uint path_length, int mode, zend_bool overwrite TSRMLS_DC);
BUESSION_API int buession_folder_create_ex(char *path, uint path_length, int mode, zend_bool overwrite, php_stream_context *context TSRMLS_DC);

BUESSION_API zend_bool buession_fso_isReadable(char *path, uint path_length TSRMLS_DC);
BUESSION_API zend_bool buession_fso_isWritable(char *path, uint path_length TSRMLS_DC);
BUESSION_API zend_bool buession_fso_isExecutable(char *path, uint path_length TSRMLS_DC);
#define fso_check_permission_fn(path, path_length, mode){	\
	zval stat;	\
	php_stat((path), (path_length), (mode), &stat TSRMLS_CC);	\
	if(Z_TYPE(stat) == IS_BOOL&&Z_BVAL(stat) == TRUE){	\
		return TRUE;	\
	}	\
	return FALSE;	\
}

BUESSION_API int buession_file_read(char *path, uint path_length, size_t offset, size_t max_size, char **content, size_t *content_length TSRMLS_DC);
BUESSION_API int buession_file_read_ex(char *path, uint path_length, size_t offset, size_t max_size, php_stream_context *context, char **content, size_t *content_length TSRMLS_DC);

BUESSION_API int buession_file_write(char *path, uint path_length, char *str, size_t str_length, long flag, size_t *result_size TSRMLS_DC);
BUESSION_API int buession_file_write_ex(char *path, uint path_length, char *str, size_t str_length, long flag, php_stream_context *context, size_t *result_size TSRMLS_DC);

BUESSION_API int buession_file_delete(char *path, uint path_length TSRMLS_DC);
BUESSION_API int buession_file_delete_ex(char *path, uint path_length, php_stream_context *context TSRMLS_DC);

BUESSION_API int buession_folder_clear(char *path, uint path_length TSRMLS_DC);
BUESSION_API int buession_folder_clear_ex(char *path, uint path_length, php_stream_context *context TSRMLS_DC);
BUESSION_API int buession_folder_delete(char *path, uint path_length TSRMLS_DC);
BUESSION_API int buession_folder_delete_ex(char *path, uint path_length, php_stream_context *context TSRMLS_DC);

#define buession_stream_free(stream)	\
	if(stream){	\
		if(!(stream)->is_persistent){	\
			zend_list_delete((stream)->rsrc_id);	\
		}else{	\
			php_stream_pclose((stream));	\
		}	\
	}
/* file system object end */

/* ini alert start */
BUESSION_API int buession_alert_ini_entry(char *name, uint name_length, zval *value, int modify_type, int stage TSRMLS_DC);
BUESSION_API int buession_alert_ini_entry_ex(char *name, uint name_length, zval *value, int modify_type, int stage, zend_bool force TSRMLS_DC);

BUESSION_API int zend_alter_ini_long_entry(char *name, uint name_length, long value, int modify_type, int stage TSRMLS_DC);
BUESSION_API int zend_alter_ini_long_entry_ex(char *name, uint name_length, long value, int modify_type, int stage, zend_bool force TSRMLS_DC);

BUESSION_API int zend_alter_ini_double_entry(char *name, uint name_length, double value, int modify_type, int stage TSRMLS_DC);
BUESSION_API int zend_alter_ini_double_entry_ex(char *name, uint name_length, double value, int modify_type, int stage, zend_bool force TSRMLS_DC);

BUESSION_API int zend_alter_ini_bool_entry(char *name, uint name_length, zend_bool value, int modify_type, int stage TSRMLS_DC);
BUESSION_API int zend_alter_ini_bool_entry_ex(char *name, uint name_length, zend_bool value, int modify_type, int stage, zend_bool force TSRMLS_DC);

BUESSION_API int zend_alter_ini_null_entry(char *name, uint name_length, int modify_type, int stage TSRMLS_DC);
BUESSION_API int zend_alter_ini_null_entry_ex(char *name, uint name_length, int modify_type, int stage, zend_bool force TSRMLS_DC);

BUESSION_API zend_bool zend_ini_bool(char *name, uint name_length, int orig TSRMLS_DC);
/* ini alert end */

#endif /* BUESSION_BUESSION_API_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */