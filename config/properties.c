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

/* $Id: config/properties.c 310447 2011-12-22 19:55:26Z Teng Yong $ */

/* include php file */
#include "Zend/zend.h"
#include "Zend/zend_API.h"
#include "Zend/zend_ini_scanner.h"
#include "main/php.h"
#include "main/php_main.h"
#include "ext/standard/flock_compat.h"

/* include buession file */
#include "buession.h"
#include "properties.h"
#include "config.h"
#include "cache.h"
//#include "logger/logger.h"

zend_class_entry *config_properties_ce;

/* {{{ arginfo */
ZEND_BEGIN_ARG_INFO_EX(config_properties___construct_arguments, FALSE, FALSE, 0)
	ZEND_ARG_INFO(FALSE, path)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(config_properties_load_arguments, FALSE, FALSE, 0)
	ZEND_ARG_INFO(FALSE, path)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(config_properties_save_arguments, FALSE, FALSE, 1)
	ZEND_ARG_INFO(FALSE, path)
	ZEND_ARG_INFO(FALSE, data)
ZEND_END_ARG_INFO()
/* }}} */

static void config_properties_add_value(zval *dest, char *key, uint key_length, zval *value TSRMLS_DC){
	if(value){
		zval *val;

		config_properties_parse_value(value, val);
		add_assoc_zval_ex(dest, key, key_length, val);
	}
}

static void config_properties_add_next_index_value(zval *dest, zval *value TSRMLS_DC){
	if(value){
		zval *val;

		config_properties_parse_value(value, val);
		add_next_index_zval(dest, val);
	}
}

static void config_properties_parse(zval *arg1, zval *arg2, zval *arg3, int callback_type, zval *result TSRMLS_DC){
	zval *exception = NULL;

	switch(callback_type){
		case ZEND_INI_PARSER_ENTRY:
			if(!arg2){
				break;
			}

			config_properties_add_value(result, Z_STRVAL_P(arg1), Z_STRLEN_P(arg1) + 1, arg2 TSRMLS_CC);
			break;
		case ZEND_INI_PARSER_POP_ENTRY: {
				zval *hash;
				zval **find_hash;
				long l;

				if(!arg2){
					break;
				}

				if(!(Z_STRLEN_P(arg1) > 1&&Z_STRVAL_P(arg1)[0] == '0')
						&&is_numeric_string(Z_STRVAL_P(arg1), Z_STRLEN_P(arg1), &l, NULL, FALSE) == IS_LONG){
					if(zend_hash_index_find(Z_ARRVAL_P(result), l, (void **) &find_hash) == FAILURE){
						BUESSION_ARRAY_INIT(hash);
						add_index_zval(result, l, hash);
					}else{
						hash = *find_hash;
					}
				}else{
					if(zend_hash_find(Z_ARRVAL_P(result), Z_STRVAL_P(arg1), Z_STRLEN_P(arg1) + 1, (void **) &find_hash) == FAILURE){
						BUESSION_ARRAY_INIT(hash);
						add_assoc_zval_ex(result, Z_STRVAL_P(arg1), Z_STRLEN_P(arg1) + 1, hash);
					}else{
						hash = *find_hash;
					}
				}

				if(Z_TYPE_P(hash) != IS_ARRAY){
					BUESSION_ARRAY_INIT(hash);
				}

				if(arg3&&Z_STRLEN_P(arg3) > 0){
					config_properties_add_value(hash, Z_STRVAL_P(arg1), Z_STRLEN_P(arg1) + 1, arg2 TSRMLS_CC);
				}else{
					config_properties_add_next_index_value(hash, arg2 TSRMLS_CC);
				}
			}
			break;
		case ZEND_INI_PARSER_SECTION:
			break;
		default:
			break;
	}
}

static int config_properties_render_apply(zval **value TSRMLS_DC, int num_args, va_list args, zend_hash_key *hash_key){
	smart_str *buffer = va_arg(args, smart_str *);
	char *parent = va_arg(args, char *);
	uint parent_length = va_arg(args, uint);

	if(Z_TYPE_PP(value) == IS_ARRAY||Z_TYPE_PP(value) == IS_OBJECT){
		HashTable *ht = buession_zval_convert_to_hash(*value TSRMLS_CC);
		char *parent_key = NULL;
		uint parent_key_length = 0;

		if(hash_key->arKey&&hash_key->nKeyLength > 0){
			if(parent){
				parent_key_length = spprintf(&parent_key, 0, "%s.%s", parent, hash_key->arKey);
			}else{
				parent_key_length = hash_key->nKeyLength - 1;
				parent_key = estrndup(hash_key->arKey, hash_key->nKeyLength);
			}
		}else{
			if(parent){
				parent_key_length = spprintf(&parent_key, 0, "%s.%ld", parent, hash_key->h);
			}else{
				parent_key_length = spprintf(&parent_key, 0, "%ld", hash_key->h);
			}
		}

		if(ht){
			zend_hash_apply_with_arguments(ht TSRMLS_CC, (apply_func_args_t) config_properties_render_apply, 3, buffer, parent_key, parent_key_length);
		}

		buession_free(parent_key);
		parent_key_length = 0;
	}else{
		if(parent){
			smart_str_appendl(buffer, parent, parent_length);
			smart_str_appendl(buffer, ".", 1);
		}
		if(hash_key->arKey&&hash_key->nKeyLength > 0){
			smart_str_appendl(buffer, hash_key->arKey, hash_key->nKeyLength - 1);
		}else{
			smart_str_append_long(buffer, (long) hash_key->h);
		}
		smart_str_appendl(buffer, "=", 1);

		switch(Z_TYPE_PP(value)){
			case IS_LONG:
				smart_str_append_long(buffer, Z_LVAL_PP(value));
				break;
			case IS_DOUBLE:
				smart_str_append_double(buffer, Z_DVAL_PP(value));
				break;
			case IS_STRING:
				smart_str_appendl(buffer, "\"", 1);
				smart_str_appendl(buffer, Z_STRVAL_PP(value), Z_STRLEN_PP(value));
				smart_str_appendl(buffer, "\"", 1);
				break;
			case IS_BOOL:
				config_smart_str_append_bool(buffer, Z_BVAL_PP(value));
				break;
			case IS_NULL:
				smart_str_appendl(buffer, "NULL", 4);
				break;
			default:
				break;
		}

		smart_str_appendl(buffer, PHP_EOL, sizeof(PHP_EOL) - 1);
	}

	return ZEND_HASH_APPLY_KEEP;
}

/* {{{ public void Properties::__construct([string $path = NULL]) */
static BUESSION_METHOD(config_properties, __construct){
	config___construct();
}
/* }}} */

/* {{{ public mixed Config::load(string $path)
 * 	   public mixed Config::load()
 */
static BUESSION_METHOD(config_properties, load){
	config_object *intern = (config_object *) zend_object_store_get_object(getThis() TSRMLS_CC);
	zend_file_handle file_handle;

	config_load_init(intern);
	CONFIG_RETURN_CACHE(intern);

	memset(&file_handle, 0, sizeof(zend_file_handle));
	file_handle.filename = intern->path;
	file_handle.type = ZEND_HANDLE_FILENAME;

	BUESSION_CONFIG_G(sections) = NULL;

	array_init(return_value);
	if(zend_parse_ini_file(&file_handle, FALSE, ZEND_INI_SCANNER_RAW, (zend_ini_parser_cb_t) config_properties_parse, return_value TSRMLS_CC) == SUCCESS){
		zend_hash_merge(intern->data, Z_ARRVAL_P(return_value), (copy_ctor_func_t) zval_add_ref, NULL, sizeof(zval *), TRUE);
		CONFIG_SAVE_CACHE(intern);
		CONFIG_LOAD_FILE_DEBUG_SUCCESS("properties", intern->path);
		return;
	}else{
		CONFIG_LOAD_FILE_DEBUG_FAILURE("properties", intern->path);
		zval_ptr_dtor(&return_value);
	}

	RETURN_FALSE;
}
/* }}} */

/* {{{ public boolean Properties::save(string $path, miexd $data)
 *	   public boolean Properties::save(miexd $data)
 * */
static BUESSION_METHOD(config_properties, save){
	config_object *intern = (config_object *) zend_object_store_get_object(getThis() TSRMLS_CC);
	char *path;
	size_t path_length;
	zval *data;
	HashTable *data_ht = NULL;
	smart_str content = {0};

	config_save_init(intern, path, path_length, data);
	if((data_ht = buession_zval_convert_to_hash(data TSRMLS_CC))){
		zend_hash_apply_with_arguments(data_ht TSRMLS_CC, (apply_func_args_t) config_properties_render_apply, 3, &content, NULL, 0);
	}
	smart_str_0(&content);
	config_save_write(path, path_length, content);
}
/* }}} */

/* {{{ public void Properties::__destruct() */
static BUESSION_METHOD(config_properties, __destruct){
}
/* }}} */

static zend_function_entry config_properties_methods[] = {
	BUESSION_CONFIG_PROPERTIES_ME(__construct, config_properties___construct_arguments, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	BUESSION_CONFIG_PROPERTIES_ME(load, config_properties_load_arguments, ZEND_ACC_PUBLIC)
	BUESSION_CONFIG_PROPERTIES_ME(save, config_properties_save_arguments, ZEND_ACC_PUBLIC)
	BUESSION_CONFIG_PROPERTIES_ME(__destruct, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_DTOR)
	ZEND_FE_END
};

/** {{{ BUESSION_STARTUP_FUNCTION
*/
BUESSION_STARTUP_FUNCTION(config_properties){
	zend_class_entry ce;

	BUESSION_INIT_CLASS_ENTRY(ce, "Config\\Properties", config_properties_methods);
	config_properties_ce = zend_register_internal_class_ex(&ce, config_ce, NULL TSRMLS_CC);
	config_properties_ce->ce_flags |= ZEND_ACC_FINAL_CLASS;

	return SUCCESS;
}
/* }}} */

/** {{{ BUESSION_RPROPERTIEST_FUNCTION */
BUESSION_RINIT_FUNCTION(config_properties){
	return SUCCESS;
}
/* }}} */

/** {{{ BUESSION_RSHUTDOWN_FUNCTION */
BUESSION_RSHUTDOWN_FUNCTION(config_properties){
	return SUCCESS;
}
/* }}} */

/** {{{ BUESSION_SHUTDOWN_FUNCTION */
BUESSION_SHUTDOWN_FUNCTION(config_properties){
	return SUCCESS;
}

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
