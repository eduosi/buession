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

/* $Id: config/config.c 310447 2013-08-11 16:21:22Z Teng Yong $ */

/* include php file */
#include "Zend/zend.h"
#include "Zend/zend_API.h"
#include "Zend/zend_interfaces.h"
#include "main/php.h"
#include "main/php_main.h"
#include "ext/standard/file.h"
#include "ext/standard/php_filestat.h"
#include "ext/standard/flock_compat.h"
#include "ext/spl/spl_iterators.h"

/* include buession file */
#include "buession.h"
#include "config.h"
#include "cache.h"
#include "simple.h"
#include "json.h"
#include "ini.h"
#include "properties.h"
//#include "logger/logger.h"

zend_class_entry *config_ce;

ZEND_DECLARE_MODULE_GLOBALS(buession_config)

static zend_object_handlers config_object_handlers;

/* {{{ arginfo */
ZEND_BEGIN_ARG_INFO_EX(config_load_arguments, FALSE, FALSE, 1)
	ZEND_ARG_INFO(FALSE, path)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(config_save_arguments, FALSE, FALSE, 0)
	ZEND_ARG_INFO(FALSE, path)
	ZEND_ARG_INFO(FALSE, data)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(config_isSet_arguments, FALSE, FALSE, 1)
	ZEND_ARG_INFO(FALSE, key)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(config_set_arguments, FALSE, FALSE, 2)
	ZEND_ARG_INFO(FALSE, key)
	ZEND_ARG_INFO(FALSE, value)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(config_get_arguments, FALSE, FALSE, 1)
	ZEND_ARG_INFO(FALSE, key)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(config_delete_arguments, FALSE, FALSE, 1)
	ZEND_ARG_INFO(FALSE, key)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(config_remove_arguments, FALSE, FALSE, 1)
	ZEND_ARG_INFO(FALSE, key)
ZEND_END_ARG_INFO()
/* }}} */

static BUESSION_MODULE_GLOBALS_CTOR(config){
	memset(buession_config_globals, 0, sizeof(zend_buession_config_globals));

	BUESSION_CONFIG_G(sections) = NULL;
}

static BUESSION_MODULE_GLOBALS_DTOR(config){
	BUESSION_CONFIG_G(sections) = NULL;
}

static BUESSION_OBJECT_FREE(config){
	config_object *intern = (config_object *) instance;

	if(--intern->refcount){
		return;
	}

	BUESSION_OBJECT_STRING_FIELDS_FREE(intern, path);
	BUESSION_OBJECT_STRING_FIELDS_FREE(intern, cacheid);
	buession_hash_free(intern->data);

	zend_object_std_dtor(&intern->std TSRMLS_CC);
	buession_free(intern);
}

static zend_object_value config_object_new_ex(zend_class_entry *ce, config_object **object TSRMLS_DC){
	config_object *intern = (config_object *) emalloc(sizeof(config_object));
	zend_object_value result;

	memset(intern, 0, sizeof(config_object));
	if(object){
		*object = intern;
	}

	intern->path = NULL;
	intern->path_length = 0;
	intern->cacheid = NULL;
	intern->cacheid_length = 0;
	ALLOC_HASHTABLE(intern->data);
	zend_hash_init(intern->data, 0, NULL, ZVAL_PTR_DTOR, FALSE);

	intern->refcount = 1;

	zend_object_std_init(&intern->std, ce TSRMLS_CC);
	zend_hash_copy(intern->std.properties, &ce->default_properties, zval_copy_property_ctor(ce), NULL, sizeof(zval *));

	result.handle = zend_objects_store_put(intern, NULL, (zend_objects_free_object_storage_t) BUESSION_OBJECT_FREE_NAMED(config), NULL TSRMLS_CC);
	result.handlers = &config_object_handlers;

	return result;
}

static BUESSION_OBJECT_NEW(config){
	return config_object_new_ex(ce, NULL TSRMLS_CC);
}

static zend_bool config_has_member(zval *instance, zval *offset TSRMLS_DC){
	config_object *intern = (config_object *) zend_object_store_get_object(instance TSRMLS_CC);
	return buession_has_dimension(intern->data, offset, NULL, NULL, NULL TSRMLS_CC);
}

static zval *config_read_member(zval *instance, zval *offset, zend_ushort type TSRMLS_DC){
	config_object *intern = (config_object *) zend_object_store_get_object(instance TSRMLS_CC);
	zval **value = &EG(uninitialized_zval_ptr);
	char *key;
	uint key_length;
	ulong index;

	switch(buession_read_dimension(intern->data, offset, &value, &key, &key_length, &index TSRMLS_CC)){
		case HASH_KEY_IS_LONG:
			if(type == 0){
				buession_hash_index_undefined_exception(index);
			}else{
				char *property = NULL;

				spprintf(&property, 0, "%ld", index);
				buession_property_undefined_exception(config_ce, property);
				buession_free(property);
			}
			break;
		case HASH_KEY_IS_STRING:
			if(type == 0){
				buession_hash_key_undefined_exception(key);
			}else{
				buession_property_undefined_exception(config_ce, key);
				buession_free(key);
			}
			break;
		default:
			break;
	}

	return *value;
}

static int config_write_member(zval *instance, zval *offset, zval *value TSRMLS_DC){
	config_object *intern = (config_object *) zend_object_store_get_object(instance TSRMLS_CC);
	return buession_write_dimension(intern->data, offset, value, TRUE, NULL, NULL, NULL TSRMLS_CC);
}

static int config_unset_member(zval *instance, zval *offset TSRMLS_DC){
	config_object *intern = (config_object *) zend_object_store_get_object(instance TSRMLS_CC);
	return buession_unset_dimension(intern->data, offset, NULL, NULL, NULL TSRMLS_CC);
}

static BUESSION_OBJECT_HAS_PROPERTY(config){
	return config_has_member(instance, property TSRMLS_CC);
}

static BUESSION_OBJECT_READ_PROPERTY(config){
	return config_read_member(instance, property, 1 TSRMLS_CC);
}

static BUESSION_OBJECT_GET_PROPERTYS(config){
	config_object *intern = (config_object *) zend_object_store_get_object(instance TSRMLS_CC);
	uint size = zend_hash_num_elements(intern->data);

	if(size > 0){
		zend_hash_copy(intern->std.properties, intern->data, (copy_ctor_func_t) zval_add_ref, NULL, sizeof(zval *));
	}

	return intern->std.properties;
}

static BUESSION_OBJECT_WRITE_PROPERTY(config){
	config_write_member(instance, property, value TSRMLS_CC);
}

static BUESSION_OBJECT_UNSET_PROPERTY(config){
	config_unset_member(instance, property TSRMLS_CC);
}

static BUESSION_OBJECT_HAS_DIMENSION(config){
	return config_has_member(instance, offset TSRMLS_CC);
}

static BUESSION_OBJECT_READ_DIMENSION(config){
	return config_read_member(instance, offset, 0 TSRMLS_CC);
}

static BUESSION_OBJECT_WRITE_DIMENSION(config){
	config_write_member(instance, offset, value TSRMLS_CC);
}

static BUESSION_OBJECT_UNSET_DIMENSION(config){
	config_unset_member(instance, offset TSRMLS_CC);
}

static BUESSION_OBJECT_COUNT_ELEMENTS(config){
	config_object *intern = (config_object *) zend_object_store_get_object(instance TSRMLS_CC);

	*count = zend_hash_num_elements(intern->data);
	return SUCCESS;
}

static BUESSION_OBJECT_SERIALIZE(config){
	config_object *intern = (config_object *) zend_object_store_get_object(instance TSRMLS_CC);
	zval var;
	zval *pvar;
	smart_str str = {0};

	INIT_PZVAL(&var);
	Z_ARRVAL(var) = intern->data;
	Z_TYPE(var) = IS_ARRAY;
	pvar = &var;
	php_var_serialize(&str, &pvar, (php_serialize_data_t *) var_hash TSRMLS_CC);

	if(str.c){
		*result = (unsigned char *) estrndup(str.c, str.len);
		*result_length = str.len;
		buession_free(str.c);

		return SUCCESS;
	}else{
		return FAILURE;
	}
}

static BUESSION_OBJECT_UNSERIALIZE(config){
	config_object *intern;
	const unsigned char *s = (const unsigned char *) str;
	zval *pvar;

	object_init_ex(*instance, ce);
	intern = (config_object *) zend_object_store_get_object(*instance TSRMLS_CC);

	ALLOC_INIT_ZVAL(pvar);
	if(!php_var_unserialize(&pvar, &s, s + str_length, (php_unserialize_data_t *) var_hash TSRMLS_CC)){
		zval_ptr_dtor(&pvar);
		buession_throw_exception_ex(illegalexception_ce, E_NOTICE, "Error at offset %ld of %d bytes", 0, (long) ((char *) s - str), str_length);

		return FAILURE;
	}

	zend_hash_copy(intern->data, Z_ARRVAL_P(pvar), (copy_ctor_func_t) zval_add_ref, NULL, sizeof(zval *));
	buession_zval_ptr_dtor(pvar);

	return SUCCESS;
}

static BUESSION_OBJECT_CLONE(config){
	config_object *old_object = (config_object *) zend_object_store_get_object(instance TSRMLS_CC);
	config_object *new_object;
	zend_object_value new_object_value = config_object_new_ex(old_object->std.ce, &new_object TSRMLS_CC);

	new_object->path = buession_estrndup(old_object->path, old_object->path_length);
	new_object->path_length = old_object->path_length;
	new_object->cacheid = buession_estrndup(old_object->cacheid, old_object->cacheid_length);
	new_object->cacheid_length = old_object->cacheid_length;
	zend_hash_copy(new_object->data, old_object->data, (copy_ctor_func_t) zval_add_ref, NULL, sizeof(zval *));

	return new_object_value;
}

static BUESSION_OBJECT_CAST(config){
	config_object *intern = (config_object *) zend_object_store_get_object(instance TSRMLS_CC);

	if(type == IS_ARRAY){
		INIT_PZVAL(result);
		array_init(result);
		zend_hash_copy(Z_ARRVAL_P(result), intern->data, (copy_ctor_func_t) zval_add_ref, NULL, sizeof(zval *));

		return SUCCESS;
	}

	buession_object_unsupported_cast_exception(config_ce, zend_get_type_by_const(type));

	return FAILURE;
}

/* {{{ public void Config::__construct() */
BUESSION_METHOD(config, __construct){
}
/* }}} */

/* {{{ public mixed Config::getPath() */
BUESSION_METHOD(config, getPath){
	if(return_value_used){
		config_object *intern = (config_object *) zend_object_store_get_object(getThis() TSRMLS_CC);
		BUESSION_RETURN_STRINGL(intern->path, intern->path_length, TRUE);
	}
}
/* }}} */

/* {{{ public mixed Config::load(string $path)
 * 	   public mixed Config::load()
 */
BUESSION_METHOD(config, load){
	buession_not_an_implementation_method_exception(config_ce, "load");
}
/* }}} */

/* {{{ public boolean Config::isSet(mixed $key) */
static BUESSION_METHOD(config, isSet){
	zval *key;

	if(zend_parse_parameters(1 TSRMLS_CC, "z", &key) == SUCCESS){
		RETURN_BOOL(config_has_member(getThis(), key TSRMLS_CC));
	}

	RETURN_FALSE;
}
/* }}} */

/* {{{ public boolean Config::set(mixed $key, mixed $value) */
static BUESSION_METHOD(config, set){
	zval *key;
	zval *value;

	if(zend_parse_parameters(2 TSRMLS_CC, "zz", &key, &value) == SUCCESS){
		RETURN_BOOL(config_write_member(getThis(), key, value TSRMLS_CC) == SUCCESS);
	}

	RETURN_FALSE;
}
/* }}} */

/* {{{ public mixed Config::get(mixed $key) */
BUESSION_METHOD(config, get){
	zval *key;

	if(zend_parse_parameters(1 TSRMLS_CC, "z", &key) == SUCCESS){
		zval *value = config_read_member(getThis(), key, 0 TSRMLS_CC);
		BUESSION_RETURN_ZVAL(value, TRUE, FALSE);
	}

	RETURN_NULL();
}
/* }}} */

/* {{{ public mixed Config::getAll() */
BUESSION_METHOD(config, getAll){
	if(return_value_used){
		config_object *intern = (config_object *) zend_object_store_get_object(getThis() TSRMLS_CC);

		array_init_size(return_value, zend_hash_num_elements(intern->data));
		zend_hash_copy(Z_ARRVAL_P(return_value), intern->data, (copy_ctor_func_t) zval_add_ref, NULL, sizeof(zval *));
	}
}
/* }}} */

/* {{{ public int/string/null Config::key() */
static BUESSION_METHOD(config, key){
	if(return_value_used){
		config_object *intern = (config_object *) zend_object_store_get_object(getThis() TSRMLS_CC);
		char *key;
		uint key_length;
		ulong index;

		switch(zend_hash_get_current_key_ex(intern->data, &key, &key_length, &index, TRUE, NULL)){
			case HASH_KEY_IS_LONG:
				RETURN_LONG(index);
				break;
			case HASH_KEY_IS_STRING:
				BUESSION_RETURN_STRINGL(key, key_length - 1, FALSE);
				break;
			default:
				break;
		}

		RETURN_NULL();
	}
}
/* }}} */

/* {{{ public void Config::current() */
static BUESSION_METHOD(config, current){
	if(return_value_used){
		config_object *intern = (config_object *) zend_object_store_get_object(getThis() TSRMLS_CC);
		zval **value;

		if(zend_hash_get_current_data(intern->data, (void **) &value) == SUCCESS){
			BUESSION_RETURN_ZVAL(*value, TRUE, FALSE);
		}

		RETURN_NULL();
	}
}
/* }}} */

/* {{{ public void Config::previous() */
static BUESSION_METHOD(config, previous){
	config_object *intern = (config_object *) zend_object_store_get_object(getThis() TSRMLS_CC);
	zend_hash_move_backwards(intern->data);
}
/* }}} */

/* {{{ public void Config::next() */
static BUESSION_METHOD(config, next){
	config_object *intern = (config_object *) zend_object_store_get_object(getThis() TSRMLS_CC);
	zend_hash_move_forward(intern->data);
}
/* }}} */

/* {{{ public void Config::rewind() */
static BUESSION_METHOD(config, rewind){
	config_object *intern = (config_object *) zend_object_store_get_object(getThis() TSRMLS_CC);
	zend_hash_internal_pointer_reset(intern->data);
}
/* }}} */

/* {{{ public boolean Config::valid() */
static BUESSION_METHOD(config, valid){
	config_object *intern = (config_object *) zend_object_store_get_object(getThis() TSRMLS_CC);
	RETURN_BOOL(zend_hash_has_more_elements(intern->data) == SUCCESS);
}
/* }}} */

/* {{{ public integer Config::count() */
static BUESSION_METHOD(config, count){
	if(return_value_used){
		config_object *intern = (config_object *) zend_object_store_get_object(getThis() TSRMLS_CC);
		uint count = zend_hash_num_elements(intern->data);

		RETURN_LONG(count);
	}
}
/* }}} */

/* {{{ public boolean Config::delete(zval $key) */
static BUESSION_METHOD(config, delete){
	zval *key;

	if(zend_parse_parameters(1 TSRMLS_CC, "z", &key) == SUCCESS){
		RETURN_BOOL(config_unset_member(getThis(), key TSRMLS_CC) == SUCCESS);
	}

	RETURN_FALSE;
}
/* }}} */

/* {{{ public void Config::clean() */
static BUESSION_METHOD(config, clean){
	config_object *intern = (config_object *) zend_object_store_get_object(getThis() TSRMLS_CC);
	zend_hash_clean(intern->data);
}
/* }}} */

/* {{{ public boolean Config::save(string $path, mixed $data)
 * 	   public boolean Config::save(mixed $data)
 * 	   public boolean Config::save()
 */
static BUESSION_METHOD(config, save){
	buession_not_an_implementation_method_exception(config_ce, "save");
}
/* }}} */

/* {{{ public void Config::__destruct() */
BUESSION_METHOD(config, __destruct){
}
/* }}} */

static zend_function_entry config_methods[] = {
	BUESSION_CONFIG_ME(__construct, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	BUESSION_CONFIG_ME(getPath, NULL, ZEND_ACC_PUBLIC)
	BUESSION_CONFIG_ME(load, config_load_arguments, ZEND_ACC_PUBLIC)
	BUESSION_CONFIG_ME(isSet, config_isSet_arguments, ZEND_ACC_PUBLIC)
	BUESSION_CONFIG_ME(set, config_set_arguments, ZEND_ACC_PUBLIC)
	BUESSION_CONFIG_ME(get, config_get_arguments, ZEND_ACC_PUBLIC)
	BUESSION_CONFIG_ME(getAll, NULL, ZEND_ACC_PUBLIC)
	BUESSION_CONFIG_ME(key, NULL, ZEND_ACC_PUBLIC)
	BUESSION_CONFIG_ME(current, NULL, ZEND_ACC_PUBLIC)
	BUESSION_CONFIG_ME(previous, NULL, ZEND_ACC_PUBLIC)
	BUESSION_CONFIG_MALIAS(prev, previous, NULL, ZEND_ACC_PUBLIC)
	BUESSION_CONFIG_ME(next, NULL, ZEND_ACC_PUBLIC)
	BUESSION_CONFIG_ME(rewind, NULL, ZEND_ACC_PUBLIC)
	BUESSION_CONFIG_ME(valid, NULL, ZEND_ACC_PUBLIC)
	BUESSION_CONFIG_ME(count, NULL, ZEND_ACC_PUBLIC)
	BUESSION_CONFIG_ME(delete, config_delete_arguments, ZEND_ACC_PUBLIC)
	BUESSION_CONFIG_MALIAS(remove, delete, config_remove_arguments, ZEND_ACC_PUBLIC)
	BUESSION_CONFIG_ME(clean, NULL, ZEND_ACC_PUBLIC)
	BUESSION_CONFIG_ME(save, config_save_arguments, ZEND_ACC_PUBLIC)
	BUESSION_CONFIG_ME(__destruct, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_DTOR)
	ZEND_FE_END
};

/** {{{ BUESSION_STARTUP_FUNCTION */
BUESSION_STARTUP_FUNCTION(config){
	zend_class_entry ce;

	BUESSION_INIT_CLASS_ENTRY(ce, "Config", config_methods);
	ce.create_object = BUESSION_OBJECT_NEW_NAMED(config);
	ce.serialize = BUESSION_OBJECT_SERIALIZE_NAMED(config);
	ce.unserialize = BUESSION_OBJECT_UNSERIALIZE_NAMED(config);
	config_ce = zend_register_internal_class(&ce TSRMLS_CC);
	config_ce->ce_flags |= ZEND_ACC_EXPLICIT_ABSTRACT_CLASS;

	zend_class_implements(config_ce TSRMLS_CC, 2, zend_ce_iterator, spl_ce_Countable);

	BUESSION_COPY_OBJECT_HANDLERS(&config_object_handlers);
	config_object_handlers.has_property = BUESSION_OBJECT_HAS_PROPERTY_NAMED(config);
	config_object_handlers.read_property = BUESSION_OBJECT_READ_PROPERTY_NAMED(config);
	config_object_handlers.get_properties = BUESSION_OBJECT_GET_PROPERTYS_NAMED(config);
	config_object_handlers.write_property = BUESSION_OBJECT_WRITE_PROPERTY_NAMED(config);
	config_object_handlers.unset_property = BUESSION_OBJECT_UNSET_PROPERTY_NAMED(config);
	config_object_handlers.has_dimension = BUESSION_OBJECT_HAS_DIMENSION_NAMED(config);
	config_object_handlers.read_dimension = BUESSION_OBJECT_READ_DIMENSION_NAMED(config);
	config_object_handlers.write_dimension = BUESSION_OBJECT_WRITE_DIMENSION_NAMED(config);
	config_object_handlers.unset_dimension = BUESSION_OBJECT_UNSET_DIMENSION_NAMED(config);
	config_object_handlers.count_elements = BUESSION_OBJECT_COUNT_ELEMENTS_NAMED(config);
	config_object_handlers.clone_obj = BUESSION_OBJECT_CLONE_NAMED(config);
	config_object_handlers.cast_object = BUESSION_OBJECT_CAST_NAMED(config);

	ZEND_INIT_MODULE_GLOBALS(buession_config, config_globals_ctor, NULL);
	config_cache_startup(TSRMLS_C);

	BUESSION_STARTUP(config_simple);
	BUESSION_STARTUP(config_json);
	BUESSION_STARTUP(config_ini);
	BUESSION_STARTUP(config_properties);

	return SUCCESS;
}
/* }}} */

/** {{{ BUESSION_RINIT_FUNCTION */
BUESSION_RINIT_FUNCTION(config){
	BUESSION_RSTARTUP(config_simple);
	BUESSION_RSTARTUP(config_json);
	BUESSION_RSTARTUP(config_ini);
	BUESSION_RSTARTUP(config_properties);

	return SUCCESS;
}
/* }}} */

/** {{{ BUESSION_RSHUTDOWN_FUNCTION */
BUESSION_RSHUTDOWN_FUNCTION(config){
	BUESSION_RSHUTDOWN(config_simple);
	BUESSION_RSHUTDOWN(config_json);
	BUESSION_RSHUTDOWN(config_ini);
	BUESSION_RSHUTDOWN(config_properties);

	return SUCCESS;
}
/* }}} */

/** {{{ BUESSION_SHUTDOWN_FUNCTION */
BUESSION_SHUTDOWN_FUNCTION(config){
	#ifdef ZTS
		ts_free_id(buession_config_globals_id);
	#endif

	BUESSION_SHUTDOWN(config_simple);
	BUESSION_SHUTDOWN(config_json);
	BUESSION_SHUTDOWN(config_ini);
	BUESSION_SHUTDOWN(config_properties);

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
