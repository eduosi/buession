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

/* $Id: config/ini.c 310447 2011-12-22 19:55:26Z Teng Yong $ */

/* include php file */
#include "Zend/zend.h"
#include "Zend/zend_API.h"
#include "Zend/zend_ini_scanner.h"
#include "main/php.h"
#include "main/php_main.h"
#include "ext/standard/flock_compat.h"

/* include buession file */
#include "buession.h"
#include "ini.h"
#include "config.h"
#include "cache.h"
//#include "logger/logger.h"

zend_class_entry *config_ini_ce;

/* {{{ arginfo */
ZEND_BEGIN_ARG_INFO_EX(config_ini___construct_arguments, FALSE, FALSE, 0)
	ZEND_ARG_INFO(FALSE, path)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(config_ini_load_arguments, FALSE, FALSE, 0)
	ZEND_ARG_INFO(FALSE, path)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(config_ini_save_arguments, FALSE, FALSE, 1)
	ZEND_ARG_INFO(FALSE, path)
	ZEND_ARG_INFO(FALSE, data)
ZEND_END_ARG_INFO()
/* }}} */

static void config_ini_add_value(zval *dest, char *key, uint key_length, zval *value TSRMLS_DC){
	zval *val;

	config_ini_parse_value(value, val);
	add_assoc_zval_ex(dest, key, key_length, val);
}

static void config_ini_add_next_index_value(zval *dest, zval *value TSRMLS_DC){
	zval *val;

	config_ini_parse_value(value, val);
	add_next_index_zval(dest, val);
}

static void config_ini_simple_parse(zval *arg1, zval *arg2, zval *arg3, int callback_type, zval *result TSRMLS_DC){
	zval *exception = NULL;

	switch(callback_type){
		case ZEND_INI_PARSER_ENTRY:
			if(!arg2){
				break;
			}

			if(strstr(Z_STRVAL_P(arg1), ".") != NULL){
				char *sKey = estrndup(Z_STRVAL_P(arg1), Z_STRLEN_P(arg1));
				char *seg;
				char *ptr;
				zval *dest = result;
				zval **val;

				if((seg = php_strtok_r(sKey, ".", &ptr))){
					do{
						char *real_key = seg;
						uint key_length = strlen(real_key) + 1;

						seg = php_strtok_r(NULL, ".", &ptr);
						if(zend_symtable_find(Z_ARRVAL_P(dest), real_key, key_length, (void **) &val) == FAILURE){
							if(seg){
								config_ini_add_empty_array(dest, real_key, key_length, (void **) &val);
							}else{
								goto update;
							}
						}else{
							if(Z_TYPE_PP(val) != IS_ARRAY){
								if(seg){
									config_ini_add_empty_array(dest, real_key, key_length, (void **) &val);
								}else{
									update:
									config_ini_add_value(dest, real_key, key_length, arg2 TSRMLS_CC);
								}
							}
						}

						dest = *val;
					}while(seg);
				}
				buession_free(sKey);
			}else{
				config_ini_add_value(result, Z_STRVAL_P(arg1), Z_STRLEN_P(arg1) + 1, arg2 TSRMLS_CC);
			}
			break;
		case ZEND_INI_PARSER_POP_ENTRY: {
				zval *hash;
				zval **find_hash;
				long l;

				if(!arg2){
					break;
				}

				if(!(Z_STRLEN_P(arg1) > 1&&*(Z_STRVAL_P(arg1)) == '0')
					&&is_numeric_string(Z_STRVAL_P(arg1), Z_STRLEN_P(arg1), &l, NULL, FALSE) == IS_LONG){
					if(zend_hash_index_find(Z_ARRVAL_P(result), l, (void **) &find_hash) == FAILURE){
						BUESSION_ARRAY_INIT(hash);
						add_index_zval(result, l, hash);
					}else{
						hash = *find_hash;
					}
				}else{
					if(strstr(Z_STRVAL_P(arg1), ".") != NULL){
						char *sKey = estrndup(Z_STRVAL_P(arg1), Z_STRLEN_P(arg1));
						char *seg;
						uint seg_length;
						char *ptr;
						zval *dst;

						dst = result;
						if((seg = php_strtok_r(sKey, ".", &ptr))){
							while(seg){
								seg_length = strlen(seg);

								if(zend_symtable_find(Z_ARRVAL_P(dst), seg, seg_length + 1, (void **) &find_hash) == FAILURE){
									BUESSION_ARRAY_INIT(hash);
									add_assoc_zval_ex(dst, seg, seg_length + 1, hash);
								}

								dst = *find_hash;
								seg = php_strtok_r(NULL, ".", &ptr);
							}
							hash = dst;
						}
						buession_free(sKey);
					}else{
						if(zend_hash_find(Z_ARRVAL_P(result), Z_STRVAL_P(arg1), Z_STRLEN_P(arg1) + 1, (void **) &find_hash) == FAILURE){
							BUESSION_ARRAY_INIT(hash);
							add_assoc_zval_ex(result, Z_STRVAL_P(arg1), Z_STRLEN_P(arg1) + 1, hash);
						}else{
							hash = *find_hash;
						}
					}
				}

				if(Z_TYPE_P(hash) != IS_ARRAY){
					BUESSION_ARRAY_INIT(hash);
				}

				if(arg3&&Z_STRLEN_P(arg3) > 0){
					config_ini_add_value(hash, Z_STRVAL_P(arg1), Z_STRLEN_P(arg1) + 1, arg2 TSRMLS_CC);
				}else{
					config_ini_add_next_index_value(hash, arg2 TSRMLS_CC);
				}
			}
			break;
		case ZEND_INI_PARSER_SECTION:
			break;
		default:
			break;
	}
}

static void config_ini_parse_with_section(zval *arg1, zval *arg2, zval *arg3, int callback_type, zval *result TSRMLS_DC){
	if(callback_type == ZEND_INI_PARSER_SECTION){
		BUESSION_ARRAY_INIT(BUESSION_CONFIG_G(sections));
		zend_symtable_update(Z_ARRVAL_P(result), Z_STRVAL_P(arg1), Z_STRLEN_P(arg1) + 1, &BUESSION_CONFIG_G(sections), sizeof(zval *), NULL);
	}else if(arg2){
		config_ini_simple_parse(arg1, arg2, arg3, callback_type, BUESSION_CONFIG_G(sections) ? BUESSION_CONFIG_G(sections) : result TSRMLS_CC);
	}
}

static int config_ini_render_apply(zval **value TSRMLS_DC, int num_args, va_list args, zend_hash_key *hash_key){
	smart_str *buffer = va_arg(args, smart_str *);
	char *parent = va_arg(args, char *);
	uint parent_length = va_arg(args, uint);
	int isBranch = va_arg(args, int);

	if(Z_TYPE_PP(value) == IS_ARRAY||Z_TYPE_PP(value) == IS_OBJECT){
		HashTable *ht = buession_zval_convert_to_hash(*value TSRMLS_CC);
		char *parent_key = NULL;
		uint parent_key_length = 0;

		if(isBranch == TRUE){
			if(hash_key->arKey&&hash_key->nKeyLength > 0){
				if(parent){
					parent_key_length = spprintf(&parent_key, 0, "%s.%s", parent, hash_key->arKey);
				}else{
					parent_key_length = hash_key->nKeyLength - 1;
					parent_key = estrndup(hash_key->arKey, parent_key_length);
				}
			}else{
				if(parent){
					parent_key_length = spprintf(&parent_key, 0, "%s.%ld", parent, hash_key->h);
				}else{
					parent_key_length = spprintf(&parent_key, 0, "%ld", hash_key->h);
				}
			}
		}else{
			smart_str_appendl(buffer, "[", 1);
			if(hash_key->arKey&&hash_key->nKeyLength > 0){
				smart_str_appendl(buffer, hash_key->arKey, hash_key->nKeyLength - 1);
			}else{
				smart_str_append_long(buffer, (long) hash_key->h);
			}
			smart_str_appendl(buffer, "]", 1);
			smart_str_appendl(buffer, PHP_EOL, sizeof(PHP_EOL) - 1);
		}

		if(ht){
			zend_hash_apply_with_arguments(ht TSRMLS_CC, (apply_func_args_t) config_ini_render_apply, 4, buffer, parent_key, parent_key_length, TRUE);
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

/* {{{ public void Ini::__construct([string $path = NULL]) */
static BUESSION_METHOD(config_ini, __construct){
	config___construct();
}
/* }}} */

/* {{{ public mixed Config::load(string $path)
 * 	   public mixed Config::load()
 */
static BUESSION_METHOD(config_ini, load){
	config_object *intern = (config_object *) zend_object_store_get_object(getThis() TSRMLS_CC);
	zend_file_handle file_handle;

	config_load_init(intern);
	CONFIG_RETURN_CACHE(intern);

	memset(&file_handle, 0, sizeof(zend_file_handle));
	file_handle.filename = intern->path;
	file_handle.type = ZEND_HANDLE_FILENAME;

	BUESSION_CONFIG_G(sections) = NULL;

	array_init(return_value);
	if(zend_parse_ini_file(&file_handle, FALSE, ZEND_INI_SCANNER_RAW, (zend_ini_parser_cb_t) config_ini_parse_with_section, return_value TSRMLS_CC) == SUCCESS){
		zend_hash_merge(intern->data, Z_ARRVAL_P(return_value), (copy_ctor_func_t) zval_add_ref, NULL, sizeof(zval *), TRUE);
		CONFIG_SAVE_CACHE(intern);
		CONFIG_LOAD_FILE_DEBUG_SUCCESS("ini", intern->path);
		return;
	}else{
		CONFIG_LOAD_FILE_DEBUG_FAILURE("ini", intern->path);
		zval_ptr_dtor(&return_value);
	}

	RETURN_FALSE;
}
/* }}} */

/* {{{ public boolean Ini::save(string $path, miexd $data)
 *     public boolean Ini::save(miexd $data)
 **/
static BUESSION_METHOD(config_ini, save){
	config_object *intern = (config_object *) zend_object_store_get_object(getThis() TSRMLS_CC);
	char *path;
	size_t path_length;
	zval *data;
	HashTable *data_ht = NULL;
	smart_str content = {0};

	config_save_init(intern, path, path_length, data);

	if((data_ht = buession_zval_convert_to_hash(data TSRMLS_CC))){
		zend_hash_apply_with_arguments(data_ht TSRMLS_CC, (apply_func_args_t) config_ini_render_apply, 4, &content, NULL, 0, FALSE);
	}

	content.len = content.len - (sizeof(PHP_EOL) - 1);
	smart_str_0(&content);
	config_save_write(path, path_length, content);
}
/* }}} */

/* {{{ public void Ini::__destruct() */
static BUESSION_METHOD(config_ini, __destruct){
}
/* }}} */

static zend_function_entry config_ini_methods[] = {
	BUESSION_CONFIG_INI_ME(__construct, config_ini___construct_arguments, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	BUESSION_CONFIG_INI_ME(load, config_ini_load_arguments, ZEND_ACC_PUBLIC)
	BUESSION_CONFIG_INI_ME(save, config_ini_save_arguments, ZEND_ACC_PUBLIC)
	BUESSION_CONFIG_INI_ME(__destruct, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_DTOR)
	ZEND_FE_END
};

/** {{{ BUESSION_STARTUP_FUNCTION */
BUESSION_STARTUP_FUNCTION(config_ini){
	zend_class_entry ce;

	BUESSION_INIT_CLASS_ENTRY(ce, "Config\\Ini", config_ini_methods);
	config_ini_ce = zend_register_internal_class_ex(&ce, config_ce, NULL TSRMLS_CC);
	config_ini_ce->ce_flags |= ZEND_ACC_FINAL_CLASS;

	return SUCCESS;
}
/* }}} */

/** {{{ BUESSION_RINIT_FUNCTION */
BUESSION_RINIT_FUNCTION(config_ini){
	return SUCCESS;
}
/* }}} */

/** {{{ BUESSION_RSHUTDOWN_FUNCTION */
BUESSION_RSHUTDOWN_FUNCTION(config_ini){
	return SUCCESS;
}
/* }}} */

/** {{{ BUESSION_SHUTDOWN_FUNCTION */
BUESSION_SHUTDOWN_FUNCTION(config_ini){
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
