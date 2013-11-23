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
  | Author: Teng Yong	<webmaster@buession.com>                         |
  +----------------------------------------------------------------------+
*/

/* $Id: registry/registry.h 310447 2011-11-12 01:43:47Z Teng Yong $ */

#ifndef REGISTRY_H
#define REGISTRY_H

extern zend_class_entry *registry_ce;

#define BUESSION_REGISTRY_ME(method, arguments) ZEND_ME(buession_registry, method, arguments, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
#define BUESSION_REGISTRY_MALIAS(alias, method, arguments) ZEND_MALIAS(buession_registry, alias, method, arguments, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)

#define registry_exists(key, key_length)		zend_hash_exists(BUESSION_G(registries), (key), (key_length) + 1)
#define registry_add(key, key_length, value)	buession_hash_add_zval_ex(BUESSION_G(registries), (key), (key_length) + 1, value TSRMLS_CC)
#define registry_set(key, key_length, value)	buession_hash_update_zval_ex(BUESSION_G(registries), (key), (key_length) + 1, value TSRMLS_CC)
#define registry_find(key, key_length, value)	zend_hash_find(BUESSION_G(registries), (key), (key_length) + 1, (void **) &value)
#define registry_delete(key, key_length)		zend_hash_del(BUESSION_G(registries), (key), (key_length) + 1)

BUESSION_STARTUP_FUNCTION(registry);
BUESSION_RINIT_FUNCTION(registry);
BUESSION_RSHUTDOWN_FUNCTION(registry);
BUESSION_SHUTDOWN_FUNCTION(registry);

#endif	/* REGISTRY_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */