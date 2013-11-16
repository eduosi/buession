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

/* $Id: constant.c 310447 2013-11-04 12:58:37 Yong.Teng $ */

/* include php file */
#include "Zend/zend.h"
#include "Zend/zend_API.h"
#include "main/php.h"
#include "main/php_main.h"

/* include buession file */
#include "buession.h"
#include "constant.h"
#include "dict/creditcard/creditcard.h"

zend_class_entry *constant_ce;

/* {{{ arginfo */
/* }}} */

/* {{{ private void Constant::__construct() */
static BUESSION_METHOD(constant, __construct){
}
/* }}} */

/* {{{ private void Constant::__destruct() */
static BUESSION_METHOD(constant, __destruct){
}
/* }}} */

static zend_function_entry constant_methods[] = {
	BUESSION_CONSTANT_ME(__construct, NULL, ZEND_ACC_PRIVATE|ZEND_ACC_CTOR)
	BUESSION_CONSTANT_ME(__destruct, NULL, ZEND_ACC_PRIVATE|ZEND_ACC_DTOR)
	ZEND_FE_END
};

/** {{{ BUESSION_STARTUP_FUNCTION */
BUESSION_STARTUP_FUNCTION(constant){
	zend_class_entry ce;

	BUESSION_INIT_CLASS_ENTRY(ce, "Constant", constant_methods);
	constant_ce = zend_register_internal_class(&ce TSRMLS_CC);

	constant_register_ip_type_constant(4);
	constant_register_ip_type_constant(6);

	constant_register_isbn_type_constant(10);
	constant_register_isbn_type_constant(13);

	constant_register_creditcard_types_constant();

	return SUCCESS;
}
/* }}} */

/** {{{ BUESSION_RINIT_FUNCTION */
BUESSION_RINIT_FUNCTION(constant){
	return SUCCESS;
}
/* }}} */

/** {{{ BUESSION_RSHUTDOWN_FUNCTION */
BUESSION_RSHUTDOWN_FUNCTION(constant){
	return SUCCESS;
}
/* }}} */

/** {{{ BUESSION_SHUTDOWN_FUNCTION */
BUESSION_SHUTDOWN_FUNCTION(constant){
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