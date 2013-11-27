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

/* $Id: helper/helper.c 310447 2013-10-11 12:12:12Z Yong.Teng $ */

/* include php file */
#include "Zend/zend.h"
#include "Zend/zend_API.h"
#include "main/php.h"
#include "main/php_main.h"

/* include buession file */
#include "buession.h"
#include "helper.h"
#include "html.h"
#include "form.h"

zend_class_entry *helper_ce;

/* {{{ arginfo */
ZEND_BEGIN_ARG_INFO_EX(helper___call_arguments, FALSE, FALSE, 1)
	ZEND_ARG_INFO(FALSE, method)
	ZEND_ARG_ARRAY_INFO(FALSE, arguments, TRUE)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(helper___callStatic_arguments, FALSE, FALSE, 1)
	ZEND_ARG_INFO(FALSE, method)
	ZEND_ARG_ARRAY_INFO(FALSE, arguments, TRUE)
ZEND_END_ARG_INFO()
/* }}} */

/* {{{ public void Helper::__construct() */
static BUESSION_METHOD(helper, __construct){
}
/* }}} */

/* {{{ public mixed Helper::__call($method, array $arguments) */
static BUESSION_METHOD(helper, __call){
	ZEND_METHOD_EMPTY_CALL_ENTRY();
}
/* }}} */

/* {{{ public mixed Helper::__callStatic($method, array $arguments) */
static BUESSION_METHOD(helper, __callStatic){
	ZEND_METHOD_EMPTY_CALLSTATIC_ENTRY();
}
/* }}} */

/* {{{ public void Helper::__destruct() */
static BUESSION_METHOD(helper, __destruct){
}
/* }}} */

static zend_function_entry helper_methods[] = {
	BUESSION_HELPER_ME(__construct, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	BUESSION_HELPER_ME(__call, helper___call_arguments, ZEND_ACC_PUBLIC)
	BUESSION_HELPER_ME(__callStatic, helper___callStatic_arguments, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	BUESSION_HELPER_ME(__destruct, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_DTOR)
	ZEND_FE_END
};

/** {{{ BUESSION_STARTUP_FUNCTION */
BUESSION_STARTUP_FUNCTION(helper){
	zend_class_entry ce;

	BUESSION_INIT_CLASS_ENTRY(ce, "Helper", helper_methods);
	helper_ce = zend_register_internal_class(&ce TSRMLS_CC);
	helper_ce->ce_flags |= ZEND_ACC_EXPLICIT_ABSTRACT_CLASS;

	BUESSION_STARTUP(helper_html);
	BUESSION_STARTUP(helper_form);

	return SUCCESS;
}
/* }}} */

/** {{{ BUESSION_RINIT_FUNCTION */
BUESSION_RINIT_FUNCTION(helper){
	BUESSION_RSTARTUP(helper_html);
	BUESSION_RSTARTUP(helper_form);

	return SUCCESS;
}
/* }}} */

/** {{{ BUESSION_RSHUTDOWN_FUNCTION */
BUESSION_RSHUTDOWN_FUNCTION(helper){
	BUESSION_RSHUTDOWN(helper_html);
	BUESSION_RSHUTDOWN(helper_form);

	return SUCCESS;
}
/* }}} */

/** {{{ BUESSION_SHUTDOWN_FUNCTION */
BUESSION_SHUTDOWN_FUNCTION(helper){
	BUESSION_SHUTDOWN(helper_html);
	BUESSION_SHUTDOWN(helper_form);

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
