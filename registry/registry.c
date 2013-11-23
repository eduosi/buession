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

/* $Id: registry/registry.c 310447 2011-11-12 01:44:23Z Yong.Teng $ */

/* include php file */
#include "Zend/zend.h"
#include "Zend/zend_API.h"
#include "main/php.h"
#include "main/php_main.h"
#include "ext/spl/spl_exceptions.h"

/* include buession file */
#include "buession.h"
#include "registry.h"

zend_class_entry *registry_ce;

/* {{{ arginfo */
ZEND_BEGIN_ARG_INFO_EX(registry_isSet_arguments, FALSE, FALSE, 1)
	ZEND_ARG_INFO(FALSE, key)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(registry_add_arguments, FALSE, FALSE, 2)
	ZEND_ARG_INFO(FALSE, key)
	ZEND_ARG_INFO(FALSE, value)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(registry_registry_arguments, FALSE, FALSE, 2)
	ZEND_ARG_INFO(FALSE, key)
	ZEND_ARG_INFO(FALSE, value)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(registry_set_arguments, FALSE, FALSE, 2)
	ZEND_ARG_INFO(FALSE, key)
	ZEND_ARG_INFO(FALSE, value)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(registry_get_arguments, FALSE, FALSE, 1)
	ZEND_ARG_INFO(FALSE, key)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(registry_delete_arguments, FALSE, FALSE, 1)
	ZEND_ARG_INFO(FALSE, key)
ZEND_END_ARG_INFO()
/* }}} */

/* {{{ private void Registry::__construct() */
static BUESSION_METHOD(registry, __construct){
}
/* }}} */

/* {{{ public boolean Registry::isSet(string $key) */
static BUESSION_METHOD(registry, isSet){
	char *key;
	uint key_length;

	if(zend_parse_parameters(1 TSRMLS_CC, "s", &key, &key_length) == SUCCESS){
		RETURN_BOOL(registry_exists(key, key_length));
	}

	RETURN_FALSE;
}
/* }}} */

/* {{{ public boolean Registry::add(string $key, mixed $value) */
static BUESSION_METHOD(registry, add){
	char *key;
	uint key_length;
	zval *value;

	if(zend_parse_parameters(2 TSRMLS_CC, "sz", &key, &key_length, &value) == SUCCESS){
		Z_ADDREF_P(value);
		RETURN_BOOL(registry_add(key, key_length, value) == SUCCESS);
	}

	RETURN_FALSE;
}
/* }}} */

/* {{{ public boolean Registry::set(string $key, mixed $value) */
static BUESSION_METHOD(registry, set){
	char *key;
	uint key_length;
	zval *value;

	if(zend_parse_parameters(2 TSRMLS_CC, "sz", &key, &key_length, &value) == SUCCESS){
		Z_ADDREF_P(value);
		RETURN_BOOL(registry_set(key, key_length, value) == SUCCESS);
	}

	RETURN_FALSE;
}
/* }}} */

/* {{{ public mixed Registry::get(string $key) */
static BUESSION_METHOD(registry, get){
	char *key;
	uint key_length;

	if(zend_parse_parameters(1 TSRMLS_CC, "s", &key, &key_length) == SUCCESS){
		zval **result;

		if(registry_find(key, key_length, result) == SUCCESS){
			BUESSION_RETURN_ZVAL(*result, TRUE, FALSE);
		}else{
			buession_throw_exception_error_ex(spl_ce_OutOfBoundsException, E_NOTICE, "Could not found \"%s\" in registry", 0, key);
		}
	}

	RETURN_NULL();
}
/* }}} */

/* {{{ public array Registry::getAll() */
static BUESSION_METHOD(registry, getAll){
	BUESSION_RETURN_HASHTABLE(BUESSION_G(registries));
}
/* }}} */

/* {{{ public boolean Registry::delete(string $key) */
static BUESSION_METHOD(registry, delete){
	char *key;
	uint key_length;

	if(zend_parse_parameters(1 TSRMLS_CC, "s", &key, &key_length) == SUCCESS){
		RETURN_BOOL(registry_delete(key, key_length) == SUCCESS);
	}

	RETURN_FALSE;
}
/* }}} */

/* {{{ public void Registry::clean() */
static BUESSION_METHOD(registry, clean){
	zend_hash_clean(BUESSION_G(registries));
}
/* }}} */

/* {{{ private void Registry::__destruct() */
static BUESSION_METHOD(registry, __destruct){
}
/* }}} */

zend_function_entry registry_methods[] = {
	ZEND_ME(buession_registry, __construct, NULL, ZEND_ACC_PRIVATE|ZEND_ACC_CTOR)
	BUESSION_REGISTRY_ME(isSet, registry_isSet_arguments)
	BUESSION_REGISTRY_MALIAS(isRegistered, isSet, registry_isSet_arguments)
	BUESSION_REGISTRY_MALIAS(has, isSet, registry_isSet_arguments)
	BUESSION_REGISTRY_ME(add, registry_add_arguments)
	BUESSION_REGISTRY_MALIAS(register, add, registry_registry_arguments)
	BUESSION_REGISTRY_ME(set, registry_set_arguments)
	BUESSION_REGISTRY_ME(get, registry_get_arguments)
	BUESSION_REGISTRY_MALIAS(getRegistered, get, registry_get_arguments)
	BUESSION_REGISTRY_ME(getAll, NULL)
	BUESSION_REGISTRY_ME(delete, registry_delete_arguments)
	BUESSION_REGISTRY_MALIAS(remove, delete, registry_delete_arguments)
	BUESSION_REGISTRY_ME(clean, NULL)
	ZEND_ME(buession_registry, __destruct, NULL, ZEND_ACC_PRIVATE|ZEND_ACC_DTOR)
	ZEND_FE_END
};

/** {{{ BUESSION_STARTUP_FUNCTION */
BUESSION_STARTUP_FUNCTION(registry){
	zend_class_entry ce;

	BUESSION_INIT_CLASS_ENTRY(ce, "Registry", registry_methods);
	registry_ce = zend_register_internal_class(&ce TSRMLS_CC);
	registry_ce->ce_flags |= ZEND_ACC_FINAL_CLASS;

	return SUCCESS;
}
/* }}} */

/** {{{ BUESSION_RINIT_FUNCTION */
BUESSION_RINIT_FUNCTION(registry){
	return SUCCESS;
}
/* }}} */

/** {{{ BUESSION_RSHUTDOWN_FUNCTION */
BUESSION_RSHUTDOWN_FUNCTION(registry){
	return SUCCESS;
}
/* }}} */

/** {{{ BUESSION_SHUTDOWN_FUNCTION */
BUESSION_SHUTDOWN_FUNCTION(registry){
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