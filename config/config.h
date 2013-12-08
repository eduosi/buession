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

/* $Id: config/config.h 310447 2013-08-11 16:21:11Z Teng Yong $ */
#ifndef BUESSION_CONFIG_H
#define BUESSION_CONFIG_H

#include "cache.h"

extern zend_class_entry *config_ce;

#define BUESSION_CONFIG_ME(method, arguments, flags) ZEND_ME(buession_config, method, arguments, flags)
#define BUESSION_CONFIG_MALIAS(alias, method, arguments, flags) ZEND_MALIAS(buession_config, alias, method, arguments, flags)

typedef struct _config_object {
	zend_object	std;
	char *path;
	size_t path_length;
	char *cacheid;
	size_t cacheid_length;
	HashTable *data;
	int refcount;
	zend_object_handle handle;
} config_object; /* extends zend_object */

#define config_smart_str_append_bool(str, b)	\
	do{	\
		if(b == TRUE){	\
			smart_str_appendl(str, "true", 4);	\
		}else{	\
			smart_str_appendl(str, "false", 5);	\
		}	\
	}while(0);

#define config_path_init(config, _path, _path_length)	\
	do{	\
		if(_path == NULL||_path_length == 0){	\
			config_file_empty_exception();	\
		}else{	\
			char *real_path;	\
			uint real_path_length;	\
			buession_free((config)->path);	\
			buession_free((config)->cacheid);	\
			if(buession_get_fso_real_path((_path), (_path_length), &real_path, &real_path_length TSRMLS_CC) == SUCCESS){	\
				(config)->path = estrndup(real_path, real_path_length);	\
				(config)->path_length = real_path_length;	\
			}else{	\
				(config)->path = estrndup(_path, _path_length);	\
				(config)->path_length = _path_length;	\
			}	\
			if(buession_hashcode_ex((config)->path, (config)->path_length, "sha1", 4, (char **) &(config)->cacheid, &(config)->cacheid_length TSRMLS_CC) == FAILURE){	\
				(config)->cacheid = estrndup((config)->path, (config)->path_length);	\
				(config)->cacheid_length = (config)->path_length;	\
			}	\
		}	\
	}while(0)

#define config___construct(){	\
	char *path = NULL;	\
	size_t path_length = 0;	\
	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|s", &path, &path_length) == SUCCESS){	\
		if(ZEND_NUM_ARGS() > 0){	\
			config_object *intern = (config_object *) zend_object_store_get_object(getThis() TSRMLS_CC);	\
			config_path_init(intern, path, path_length);	\
		}	\
	}	\
}

#define config_load_init(config){	\
	char *path;	\
	size_t path_length;	\
	if((config)->path == NULL||(config)->path_length == 0){	\
		if(zend_parse_parameters(1 TSRMLS_CC, "s", &path, &path_length) == SUCCESS){	\
			config_path_init(intern, path, path_length);	\
		}else{	\
			RETURN_FALSE;	\
		}	\
	}else{	\
		if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|s", &path, &path_length) == SUCCESS){	\
			if(ZEND_NUM_ARGS() > 0){	\
				config_path_init(intern, path, path_length);	\
			}	\
		}else{	\
			RETURN_FALSE;	\
		}	\
	}	\
	if(buession_check_file_exists((config)->path, (config)->path_length TSRMLS_CC) == FALSE){	\
		config_file_not_exists_exception((config)->path);	\
		RETURN_FALSE;	\
	}	\
}

#define config_save_init(config, _path, _path_length, data){	\
	switch(ZEND_NUM_ARGS()){	\
		case 0:	\
			buession_method_argument_invalid_exception(1, E_ERROR, "string", EG(uninitialized_zval_ptr));	\
			RETURN_FALSE;	\
			break;	\
		case 1:	\
			if(zend_parse_parameters(1 TSRMLS_CC, "z", &data) == FAILURE){	\
				RETURN_FALSE;	\
			}	\
			if((config)->path == NULL||(config)->path_length == 0){	\
				buession_method_argument_invalid_exception(1, E_ERROR, "string", data);	\
				RETURN_FALSE;	\
			}	\
			_path = (config)->path;	\
			_path_length = (config)->path_length;	\
			break;	\
		default:	\
			if(zend_parse_parameters(2 TSRMLS_CC, "sz", &_path, &_path_length, &data) == FAILURE){	\
				RETURN_FALSE;	\
			}	\
			break;	\
	}	\
	if(_path == NULL||_path_length == 0){	\
		config_file_empty_exception();	\
		RETURN_FALSE;	\
	}	\
}

#define CONFIG_SAVE_CACHE(config)	config_cache_save((config)->cacheid, (config)->cacheid_length, (config)->path, (config)->path_length, (config)->data TSRMLS_CC)

#define CONFIG_RETURN_CACHE(config){	\
	config_cache *cache;	\
	if(config_cache_load((config)->cacheid, (config)->cacheid_length, (config)->path, (config)->path_length, &cache TSRMLS_CC) == SUCCESS){	\
		array_init_size(return_value, zend_hash_num_elements(cache->data));	\
		zend_hash_merge(Z_ARRVAL_P(return_value), cache->data, (copy_ctor_func_t) zval_add_ref, NULL, sizeof(zval *), TRUE);	\
		zend_hash_merge((config)->data, cache->data, (copy_ctor_func_t) zval_add_ref, NULL, sizeof(zval *), TRUE);	\
		CONFIG_LOAD_FILE_DEBUG_SUCCESS("cache", (config)->path);	\
		return;	\
	}	\
}

#define config_save_write(_path, _path_length, str){	\
	size_t result_size;	\
	int result = buession_file_write((_path), (_path_length), (str).c, (str).len, PHP_LOCK_EX, &result_size TSRMLS_CC);	\
	smart_str_free(&(str));	\
	RETURN_BOOL(result == SUCCESS);	\
}

//#define CONFIG_LOAD_FILE_DEBUG_SUCCESS(file_type, path)	//logger.debug("load config from "file_type" file '%s' success.", (path))
#define CONFIG_LOAD_FILE_DEBUG_SUCCESS(file_type, path)	php_printf("load config from %s file '%s' success.", file_type, (path))
//#define CONFIG_LOAD_FILE_DEBUG_FAILURE(file_type, path)	//logger.debug("load config from "file_type" file '%s' failure.", (path))
#define CONFIG_LOAD_FILE_DEBUG_FAILURE(file_type, path)	php_printf("load config from %s file '%s' failure.", file_type, (path))

#define config_file_empty_exception(){	\
	zval *exception = buession_throw_exception_error(illegalexception_ce, E_ERROR, ZEND_STRL("Invalid config file path, could not empty"), 0);	\
	zval_ptr_dtor(&exception);	\
}

#define config_file_not_exists_exception(path){	\
	zval *exception = buession_throw_exception_error_ex(ioexception_ce, E_ERROR, "config file '%s' is not exists", 0, (path));	\
	zval_ptr_dtor(&exception);	\
}

#define config_file_read_exception(path){	\
	zval *exception = buession_throw_exception_error_ex(ioexception_ce, E_ERROR, "config file '%s' read failure", 0, (path));	\
	zval_ptr_dtor(&exception);	\
}

ZEND_BEGIN_MODULE_GLOBALS(buession_config)
	zval *sections;
ZEND_END_MODULE_GLOBALS(buession_config)

BUESSION_API ZEND_EXTERN_MODULE_GLOBALS(buession_config)

#ifdef ZTS
	#define BUESSION_CONFIG_G(v) TSRMG(buession_config_globals_id, zend_buession_config_globals *, v)
#else
	#define BUESSION_CONFIG_G(v) (buession_config_globals.v)
#endif

BUESSION_STARTUP_FUNCTION(config);
BUESSION_RINIT_FUNCTION(config);
BUESSION_RSHUTDOWN_FUNCTION(config);
BUESSION_SHUTDOWN_FUNCTION(config);

#endif /* BUESSION_CONFIG_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
