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
  | Copyright: Copyright © 2011-2013 Buession.com.cn Inc.                |
  +----------------------------------------------------------------------+
*/

/* $Id: math/math.h 310447 2013-08-09 15:12:27Z Teng Yong $ */
#ifndef BUESSION_MATH_H
#define BUESSION_MATH_H

extern zend_class_entry *math_ce;

#define BUESSION_MATH_ME(method, arguments) ZEND_ME(buession_math, method, arguments, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)

#define math_register_long_constant(name, value) zend_declare_class_constant_long(math_ce, ZEND_STRL(name), value TSRMLS_CC)
#define math_register_double_constant(name, value) zend_declare_class_constant_double(math_ce, ZEND_STRL(name), value TSRMLS_CC)

#define math_exchange_value(value1, value2){	\
	value1 ^= value2;	\
	value2 ^= value1;	\
	value1 ^= value2;	\
}

#define math_lcm_gcd_equal_0_exception(index, arg){	\
	zval *exception = buession_throw_exception_error_ex(illegalexception_ce, E_WARNING, "Invalid argument %ld, could not equal 0", 0, index);	\
	zval_ptr_dtor(&exception);	\
}

BUESSION_API long math_random(long min, long max TSRMLS_DC);

BUESSION_STARTUP_FUNCTION(math);
BUESSION_RINIT_FUNCTION(math);
BUESSION_RSHUTDOWN_FUNCTION(math);
BUESSION_SHUTDOWN_FUNCTION(math);

#endif /* BUESSION_MATH_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
