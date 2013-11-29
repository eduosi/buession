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

/* $Id: config/json.c 310447 2011-12-22 19:55:26Z Teng Yong $ */

/* include php file */
#include "Zend/zend.h"
#include "Zend/zend_API.h"
#include "main/php.h"
#include "main/php_main.h"
#include "ext/json/php_json.h"
#include "ext/standard/file.h"
#include "ext/standard/flock_compat.h"

/* include buession file */
#include "buession.h"
#include "json.h"
#include "config.h"
#include "cache.h"
//#include "logger/logger.h"

zend_class_entry *config_json_ce;

/* {{{ arginfo */
ZEND_BEGIN_ARG_INFO_EX(config_json___construct_arguments, FALSE, FALSE, 0)
	ZEND_ARG_INFO(FALSE, path)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(config_json_load_arguments, FALSE, FALSE, 0)
	ZEND_ARG_INFO(FALSE, path)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(config_json_save_arguments, FALSE, FALSE, 1)
	ZEND_ARG_INFO(FALSE, path)
	ZEND_ARG_INFO(FALSE, data)
ZEND_END_ARG_INFO()
/* }}} */

/* {{{ public void Json::__construct([string $path = NULL]) */
static BUESSION_METHOD(config_json, __construct){
	config___construct();
}
/* }}} */

/* {{{ public mixed Config::load(string $path)
 * 	   public mixed Config::load()
 */
static BUESSION_METHOD(config_json, load){
	config_object *intern = (config_object *) zend_object_store_get_object(getThis() TSRMLS_CC);
	zval *zcontext = NULL;
	php_stream_context *context = NULL;
	php_stream *stream = NULL;

	config_load_init(intern);
	CONFIG_RETURN_CACHE(intern);

	zend_hash_clean(intern->data);
	context = php_stream_context_from_zval(zcontext, 0);
	if(!(stream = php_stream_open_wrapper_ex(intern->path, "rb", ENFORCE_SAFE_MODE|REPORT_ERRORS, NULL, context))){
		buession_stream_free(stream);
		RETVAL_FALSE;
		config_file_read_exception(intern->path);
	}else{
		char *content = NULL;
		size_t content_length = php_stream_copy_to_mem(stream, &content, PHP_STREAM_COPY_ALL, FALSE);

		php_json_decode(return_value, content, content_length, TRUE, JSON_PARSER_DEFAULT_DEPTH TSRMLS_CC);
		buession_free(content);
		buession_stream_free(stream);

		if(return_value&&Z_TYPE_P(return_value) == IS_ARRAY){
			zend_hash_merge(intern->data, Z_ARRVAL_P(return_value), (copy_ctor_func_t) zval_add_ref, NULL, sizeof(zval *), TRUE);
			CONFIG_SAVE_CACHE(intern);
			CONFIG_LOAD_FILE_DEBUG_SUCCESS("json", intern->path);
			return;
		}

		CONFIG_LOAD_FILE_DEBUG_FAILURE("json", intern->path);
		if(return_value){
			zval_ptr_dtor(&return_value);
		}
	}

	RETURN_FALSE;
}
/* }}} */

/* {{{ public boolean Json::save(string $path, miexd $data)
 *	   public boolean Json::save(miexd $data)
 * */
static BUESSION_METHOD(config_json, save){
	config_object *intern = (config_object *) zend_object_store_get_object(getThis() TSRMLS_CC);
	char *path = NULL;
	size_t path_length = 0;
	zval *data;
	smart_str content = {0};

	config_save_init(intern, path, path_length, data);
	buession_json_encode(data, &content TSRMLS_CC);
	config_save_write(path, path_length, content);
}
/* }}} */

/* {{{ public void Json::__destruct() */
static BUESSION_METHOD(config_json, __destruct){
}
/* }}} */

static zend_function_entry config_json_methods[] = {
	BUESSION_CONFIG_JSON_ME(__construct, config_json___construct_arguments, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	BUESSION_CONFIG_JSON_ME(load, config_json_load_arguments, ZEND_ACC_PUBLIC)
	BUESSION_CONFIG_JSON_ME(save, config_json_save_arguments, ZEND_ACC_PUBLIC)
	BUESSION_CONFIG_JSON_ME(__destruct, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_DTOR)
	ZEND_FE_END
};

/** {{{ BUESSION_STARTUP_FUNCTION
*/
BUESSION_STARTUP_FUNCTION(config_json){
	zend_class_entry ce;

	BUESSION_INIT_CLASS_ENTRY(ce, "Config\\Json", config_json_methods);
	config_json_ce = zend_register_internal_class_ex(&ce, config_ce, NULL TSRMLS_CC);
	config_json_ce->ce_flags |= ZEND_ACC_FINAL_CLASS;

	return SUCCESS;
}
/* }}} */

/** {{{ BUESSION_RINIT_FUNCTION */
BUESSION_RINIT_FUNCTION(config_json){
	return SUCCESS;
}
/* }}} */

/** {{{ BUESSION_RSHUTDOWN_FUNCTION */
BUESSION_RSHUTDOWN_FUNCTION(config_json){
	return SUCCESS;
}
/* }}} */

/** {{{ BUESSION_SHUTDOWN_FUNCTION */
BUESSION_SHUTDOWN_FUNCTION(config_json){
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
