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

/* $Id: config/simple.c 310447 2012-04-14 18:45:46Z Teng Yong $ */

/* include php file */
#include "Zend/zend.h"
#include "Zend/zend_API.h"
#include "main/php.h"
#include "main/php_main.h"
#include "ext/standard/php_var.h"
#include "ext/standard/flock_compat.h"

/* include buession file */
#include "buession.h"
#include "simple.h"
#include "config.h"
//#include "logger/logger.h"

zend_class_entry *config_simple_ce;

/* {{{ arginfo */
ZEND_BEGIN_ARG_INFO_EX(config_simple___construct_arguments, FALSE, FALSE, 0)
	ZEND_ARG_INFO(FALSE, path)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(config_simple_load_arguments, FALSE, FALSE, 0)
	ZEND_ARG_INFO(FALSE, path)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(config_simple_save_arguments, FALSE, FALSE, 0)
	ZEND_ARG_INFO(FALSE, path)
	ZEND_ARG_INFO(FALSE, data)
ZEND_END_ARG_INFO()
/* }}} */

/* {{{ public void Simple::__construct([string $path = NULL]) */
static BUESSION_METHOD(config_simple, __construct){
	config___construct();
}
/* }}} */

/* {{{ public mixed Config::load(string $path)
 * 	   public mixed Config::load()
 */
static BUESSION_METHOD(config_simple, load){
	config_object *intern = (config_object *) zend_object_store_get_object(getThis() TSRMLS_CC);
	zend_file_handle file_handle;
	zend_op_array *op_array = NULL;
	zval *result = NULL;

	config_load_init(intern);

	memset(&file_handle, 0, sizeof(zend_file_handle));
	file_handle.filename = intern->path;
	file_handle.free_filename = 0;
	file_handle.type = ZEND_HANDLE_FILENAME;
	file_handle.opened_path = NULL;
	file_handle.handle.fp = NULL;

	op_array = zend_compile_file(&file_handle, ZEND_INCLUDE TSRMLS_CC);
	if(op_array&&file_handle.handle.stream.handle){
		uint dummy = 1;

		if(!file_handle.opened_path){
			file_handle.opened_path = intern->path;
		}

		zend_hash_add(&EG(included_files), file_handle.opened_path, strlen(file_handle.opened_path) + 1, (void *) &dummy, sizeof(uint), NULL);
	}
	zend_destroy_file_handle(&file_handle TSRMLS_CC);

	if(op_array){
		BUESSION_STORE_EG_ENVIRON();

		EG(return_value_ptr_ptr) = &result;
		EG(active_op_array) = op_array;

		if(!EG(active_symbol_table)){
			zend_rebuild_symbol_table(TSRMLS_C);
		}

		zend_execute(op_array TSRMLS_CC);

		destroy_op_array(op_array TSRMLS_CC);
		buession_free(op_array);

		BUESSION_RESTORE_EG_ENVIRON();

		if(result&&Z_TYPE_P(result) == IS_ARRAY){
			buession_hash_free(intern->data);
			intern->data = Z_ARRVAL_P(result);
			CONFIG_LOAD_FILE_DEBUG_SUCCESS("php file", intern->path);
			BUESSION_RETURN_ZVAL(result, TRUE, FALSE);
		}

		if(result){
			zval_ptr_dtor(&result);
		}
	}

	zend_hash_clean(intern->data);
	CONFIG_LOAD_FILE_DEBUG_FAILURE("php file", intern->path);

	RETURN_FALSE;
}
/* }}} */

/* {{{ public boolean Simple::save(string $path, miexd $data)
	   public boolean Simple::save(miexd $data)
*/
static BUESSION_METHOD(config_simple, save){
	config_object *intern = (config_object *) zend_object_store_get_object(getThis() TSRMLS_CC);
	char *path = NULL;
	size_t path_length = 0;
	zval *data;
	smart_str content = {0};

	config_save_init(intern, path, path_length, data);

	smart_str_appendl(&content, "<?php\r\n\treturn ", sizeof("<?php\r\n\treturn ") - 1);
	if(Z_TYPE_P(data) == IS_OBJECT){
		HashTable *properties = Z_OBJ_HT_P(data)->get_properties(data TSRMLS_CC);
		zval *zproperties;

		if(properties){
			uint size = zend_hash_num_elements(properties);

			BUESSION_ARRAY_SIZE_INIT(zproperties, size);
			if(size > 0){
				zend_hash_merge(Z_ARRVAL_P(zproperties), properties, (copy_ctor_func_t) zval_add_ref, NULL, sizeof(zval *), TRUE);
			}
		}else{
			BUESSION_ARRAY_INIT(zproperties)
		}

		php_var_export_ex(&zproperties, 1, &content TSRMLS_CC);
		buession_zval_dtor(zproperties);
	}else{
		php_var_export_ex(&data, 1, &content TSRMLS_CC);
	}
	smart_str_appendl(&content, ";\r\n?>", sizeof(";\r\n?>") - 1);

	smart_str_0(&content);
	config_save_write(path, path_length, content);
}
/* }}} */

/* {{{ public void Simple::__destruct() */
static BUESSION_METHOD(config_simple, __destruct){
}
/* }}} */

static zend_function_entry config_simple_methods[] = {
	BUESSION_CONFIG_SIMLATE_ME(__construct, config_simple___construct_arguments, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	BUESSION_CONFIG_SIMLATE_ME(load, config_simple_load_arguments, ZEND_ACC_PUBLIC)
	BUESSION_CONFIG_SIMLATE_ME(save, config_simple_save_arguments, ZEND_ACC_PUBLIC)
	BUESSION_CONFIG_SIMLATE_ME(__destruct, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_DTOR)
	ZEND_FE_END
};

/** {{{ BUESSION_STARTUP_FUNCTION */
BUESSION_STARTUP_FUNCTION(config_simple){
	zend_class_entry ce;

	BUESSION_INIT_CLASS_ENTRY(ce, "Config\\Simple", config_simple_methods);
	config_simple_ce = zend_register_internal_class_ex(&ce, config_ce, NULL TSRMLS_CC);
	config_simple_ce->ce_flags |= ZEND_ACC_FINAL_CLASS;

	return SUCCESS;
}
/* }}} */

/** {{{ BUESSION_RINIT_FUNCTION */
BUESSION_RINIT_FUNCTION(config_simple){
	return SUCCESS;
}
/* }}} */

/** {{{ BUESSION_RSHUTDOWN_FUNCTION */
BUESSION_RSHUTDOWN_FUNCTION(config_simple){
	return SUCCESS;
}
/* }}} */

/** {{{ BUESSION_SHUTDOWN_FUNCTION */
BUESSION_SHUTDOWN_FUNCTION(config_simple){
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
