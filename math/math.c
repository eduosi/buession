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
  | Copyright: Copyright © 2011-2012 Buession.com.cn Inc.                |
  +----------------------------------------------------------------------+
*/

/* $Id: math/math.c 310447 2013-08-09 15:13:08Z Teng Yong $ */

/* include php file */
#include "Zend/zend.h"
#include "Zend/zend_API.h"
#include "main/php.h"
#include "main/php_main.h"
#include "ext/standard/php_math.h"
#include "ext/standard/php_rand.h"

/* include buession file */
#include "buession.h"
#include "math.h"

zend_class_entry *math_ce;

/* {{{ arginfo */
ZEND_BEGIN_ARG_INFO_EX(math_cplus_arguments, FALSE, FALSE, 1)
	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(math_rand_arguments, FALSE, FALSE, 2)
	ZEND_ARG_INFO(0, min)
	ZEND_ARG_INFO(0, max)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(math_lcm_arguments, FALSE, FALSE, 2)
	ZEND_ARG_INFO(0, value1)
	ZEND_ARG_INFO(0, value2)
	ZEND_ARG_INFO(0, ...)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(math_gcd_arguments, FALSE, FALSE, 2)
	ZEND_ARG_INFO(0, value1)
	ZEND_ARG_INFO(0, value2)
	ZEND_ARG_INFO(0, ...)
ZEND_END_ARG_INFO()
/* }}} */

static inline long math_gcd(long value1, long value2 TSRMLS_DC){
	while(value2){
	    int temp = value1%value2;

	    value1 = value2;
	    value2 = temp;
	}

	return value1;
}

static inline long math_lcm(long value1, long value2 TSRMLS_DC){
	long temp = math_gcd(value1, value2 TSRMLS_CC);
	return temp == 0 ? 0 : value1 * value2/temp;
}

static void math_lcm_gcd_method(INTERNAL_FUNCTION_PARAMETERS, long (*fn)(long, long TSRMLS_DC)){
	zval ***args = NULL;
	uint argc = 0;
	uint i = 0;
	long result = 0;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "+",  &args, &argc) == FAILURE){
		RETURN_LONG(0);
	}

	if(Z_TYPE_PP(args[0]) == IS_LONG||Z_TYPE_PP(args[0]) == IS_BOOL){
		if(Z_LVAL_PP(args[0]) == 0){
			buession_free(args);
			math_lcm_gcd_equal_0_exception(1, args[0]);
			RETURN_LONG(0);
		}else{
			result = Z_LVAL_PP(args[i]);
		}
	}else if(Z_TYPE_PP(args[0]) == IS_STRING){
		if(is_numeric_string(Z_STRVAL_PP(args[0]), Z_STRLEN_PP(args[0]), &result, NULL, 0) != IS_LONG){
			goto first_arg_type_error;
		}
	}else{
		first_arg_type_error:
		buession_method_argument_invalid_exception(1, E_WARNING, "integer", *(args[i]));
		buession_free(args);
		RETURN_LONG(0);
	}

	for(i = 1; i < argc; i++){
		if(Z_TYPE_PP(args[i]) == IS_LONG||Z_TYPE_PP(args[i]) == IS_BOOL){
			if(Z_LVAL_PP(args[i]) == 0){
				math_lcm_gcd_equal_0_exception(i + 1, args[i]);
			}else{
				result = fn(result, Z_LVAL_PP(args[i]) TSRMLS_CC);
			}
		}else if(Z_TYPE_PP(args[i]) == IS_STRING){
			long l;

			if(is_numeric_string(Z_STRVAL_PP(args[0]), Z_STRLEN_PP(args[0]), &l, NULL, 0) == IS_LONG){
				result = fn(result, l TSRMLS_CC);
			}else{
				goto arg_type_error;
			}
		}else{
			arg_type_error:
			buession_method_argument_invalid_exception(i + 1, E_WARNING, "integer", *(args[i]));
		}
	}

	buession_free(args);
	RETURN_LONG(result);
}

BUESSION_API long math_random(long min, long max TSRMLS_DC){
	long result = php_rand(TSRMLS_C);

	if(min > max){
		math_exchange_value(min, max);
	}

	return RAND_RANGE(result, min, max, PHP_RAND_MAX);
}

/* {{{ private void Math::__construct() */
static BUESSION_METHOD(math, __construct){
}
/* }}} */

/* {{{ public integer/null Math::cplus(mixed $value) */
static BUESSION_METHOD(math, cplus){
	zval **value;

	if(zend_parse_parameters(1 TSRMLS_CC, "Z", &value) == SUCCESS){
		convert_scalar_to_number_ex(value);
		if(Z_TYPE_PP(value) == IS_DOUBLE){
			RETURN_LONG((long) fabs(Z_DVAL_PP(value)));
		}else if(Z_TYPE_PP(value) == IS_LONG){
			RETURN_LONG((long) fabs((double) Z_LVAL_PP(value)));
		}
	}

	RETURN_NULL();
}
/* }}} */

/* {{{ public integer Math::rand(integer $min, integer $max) */
static BUESSION_METHOD(math, rand){
	long min;
	long max;

	if(zend_parse_parameters(2 TSRMLS_CC, "ll", &min, &max) == SUCCESS){
		RETURN_LONG(math_random(min, max TSRMLS_CC));
	}

	RETURN_LONG(0);
}
/* }}} */

/* {{{
 * least common multiple
 * public integer Math::lcm([integer ...])
 */
static BUESSION_METHOD(math, lcm){
	math_lcm_gcd_method(INTERNAL_FUNCTION_PARAM_PASSTHRU, math_lcm);
}
/* }}} */

/* {{{
 * greatest common divisor
 * public integer Math::gcd([integer ...])
 **/
static BUESSION_METHOD(math, gcd){
	math_lcm_gcd_method(INTERNAL_FUNCTION_PARAM_PASSTHRU, math_gcd);
}
/* }}} */

/* {{{ private void Math::__destruct() */
static BUESSION_METHOD(math, __destruct){
}
/* }}} */

static zend_function_entry math_methods[] = {
	BUESSION_MATH_ME(__construct, NULL, ZEND_ACC_PRIVATE|ZEND_ACC_CTOR)
	BUESSION_MATH_STATIC_ME(cplus, math_cplus_arguments)
	BUESSION_MATH_STATIC_ME(rand, math_rand_arguments)
	BUESSION_MATH_STATIC_ME(lcm, math_lcm_arguments)
	BUESSION_MATH_STATIC_ME(gcd, math_gcd_arguments)
	BUESSION_MATH_ME(__destruct, NULL, ZEND_ACC_PRIVATE|ZEND_ACC_DTOR)
	ZEND_FE_END
};

/** {{{ BUESSION_STARTUP_FUNCTION */
BUESSION_STARTUP_FUNCTION(math){
	zend_class_entry ce;

	BUESSION_INIT_CLASS_ENTRY(ce, "Math", math_methods);
	math_ce = zend_register_internal_class(&ce TSRMLS_CC);

	math_register_double_constant("PI", M_PI);
	math_register_double_constant("E", M_E);
	math_register_double_constant("LOG2E", M_LOG2E);
	math_register_double_constant("LOG10E", M_LOG10E);
	math_register_double_constant("LN2", M_LN2);
	math_register_double_constant("LN10", M_LN10);
	math_register_double_constant("EULER", M_EULER);
	#ifdef INT_MIN
		math_register_long_constant("INT_MIN", INT_MIN);
	#endif
	#ifdef INT_MAX
		math_register_long_constant("INT_MAX", INT_MAX);
	#endif
	#ifdef UINT_MAX
		math_register_long_constant("UINT_MAX", UINT_MAX);
	#endif
	#ifdef LONG_MIN
		math_register_long_constant("LONG_MIN", LONG_MIN);
	#endif
	#ifdef LONG_MAX
		math_register_long_constant("LONG_MAX", LONG_MAX);
	#endif
	#ifdef ULONG_MAX
		math_register_long_constant("ULONG_MAX", ULONG_MAX);
	#endif

	return SUCCESS;
}
/* }}} */

/** {{{ BUESSION_RINIT_FUNCTION */
BUESSION_RINIT_FUNCTION(math){
	return SUCCESS;
}
/* }}} */

/** {{{ BUESSION_RSHUTDOWN_FUNCTION */
BUESSION_RSHUTDOWN_FUNCTION(math){
	return SUCCESS;
}
/* }}} */

/** {{{ BUESSION_SHUTDOWN_FUNCTION */
BUESSION_SHUTDOWN_FUNCTION(math){
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
