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

/* $Id: validate/validate.h 310447 2013-09-12 11:20:03Z Yong.Teng $ */
#ifndef BUESSION_VALIDATE_H
#define BUESSION_VALIDATE_H

/* include buession file */
#include "dict/creditcard/creditcard.h"

extern zend_class_entry *validate_ce;

#define BUESSION_VALIDATE_ME(method, arguments) ZEND_ME(buession_validate, method, arguments, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
#define BUESSION_VALIDATE_MALIAS(alias, method, arguments) ZEND_MALIAS(buession_validate, alias, method, arguments, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
#define BUESSION_VALIDATE_MAPPING(method, function_name, arguments) ZEND_ME_MAPPING(method, function_name, arguments, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)

BUESSION_API zend_bool validate_isNull(zval *data TSRMLS_DC);

BUESSION_API zend_bool validate_isBlank(const char *str TSRMLS_DC);
BUESSION_API zend_bool validate_isBlank_ex(const char *str, uint str_length TSRMLS_DC);

BUESSION_API zend_bool validate_isAscii(zval *data TSRMLS_DC);

BUESSION_API zend_bool validate_isVarName(const char *str, zend_bool vd TSRMLS_DC);
BUESSION_API zend_bool validate_isVarName_ex(const char *str, uint str_length, zend_bool vd TSRMLS_DC);

BUESSION_API zend_bool validate_isChinese(const char *str TSRMLS_DC);
BUESSION_API zend_bool validate_isChinese_ex(const char *str, uint str_length TSRMLS_DC);

BUESSION_API zend_bool validate_isAlpha(const char *str TSRMLS_DC);	//  check letter
BUESSION_API zend_bool validate_isAlpha_ex(const char *str, uint str_length TSRMLS_DC);	//  check letter
#define validate_isLetter alidate_isAlpha
#define validate_isLetter_ex validate_isAlpha_ex
BUESSION_API zend_bool validate_isLowerLetter(const char *str TSRMLS_DC);	//  check lower letter
BUESSION_API zend_bool validate_isLowerLetter_ex(const char *str, uint str_length TSRMLS_DC);	//  check lower letter
BUESSION_API zend_bool validate_isUpperLetter(const char *str TSRMLS_DC);	//  check upper letter
BUESSION_API zend_bool validate_isUpperLetter_ex(const char *str, uint str_length TSRMLS_DC);	//  check upper letter

BUESSION_API zend_bool validate_isNumeric(zval *data TSRMLS_DC);	//  check numeric
#define validate_isDigital validate_isNumeric
BUESSION_API zend_bool validate_isUnsignedNumeric(zval *data TSRMLS_DC);	//  check unsigned numeric
#define validate_isUnsignedDigital validate_isUnsignedNumeric
BUESSION_API zend_bool validate_isSignedNumeric(zval *data TSRMLS_DC);	//  check signed numeric
#define validate_isSignedDigital validate_isSignedNumeric
BUESSION_API zend_bool validate_isInteger(zval *data TSRMLS_DC);	//  check integer
BUESSION_API zend_bool validate_isUnsignedInteger(zval *data TSRMLS_DC);	//  check unsigned integer
BUESSION_API zend_bool validate_isSignedInteger(zval *data TSRMLS_DC);	//  check signed integer
BUESSION_API zend_bool validate_isFloat(zval *data TSRMLS_DC);	//  check float
BUESSION_API zend_bool validate_isUnsignedFloat(zval *data TSRMLS_DC);	//  check unsigned float
BUESSION_API zend_bool validate_isSignedFloat(zval *data TSRMLS_DC);	//  check signed float

BUESSION_API zend_bool validate_isAlnum(zval *data TSRMLS_DC);	//  check letter or numeric

BUESSION_API zend_bool validate_isXdigit(zval *data TSRMLS_DC);	//  check hexadecimal digits

BUESSION_API zend_bool validate_isCntrl(const char *str TSRMLS_DC);	//  check control string
BUESSION_API zend_bool validate_isCntrl_ex(const char *str, uint str_length TSRMLS_DC);	//  check control string
BUESSION_API zend_bool validate_isPrint(const char *str TSRMLS_DC);	//  check print string
BUESSION_API zend_bool validate_isPrint_ex(const char *str, uint str_length TSRMLS_DC);	//  check print string

BUESSION_API zend_bool validate_isCurrency(const char *str TSRMLS_DC);	//  check currency
BUESSION_API zend_bool validate_isCurrency_ex(const char *str, uint str_length TSRMLS_DC);	//  check currency

BUESSION_API zend_bool validate_isTel(const char *str, int flags TSRMLS_DC);	//  check tel
BUESSION_API zend_bool validate_isTel_ex(const char *str, uint str_length, int flags TSRMLS_DC);	//  check tel
BUESSION_API zend_bool validate_isMobile(const char *str TSRMLS_DC);	//  check mobile
BUESSION_API zend_bool validate_isMobile_ex(const char *str, uint str_length TSRMLS_DC);	//  check mobile

BUESSION_API zend_bool validate_isPostCode(const char *str TSRMLS_DC);	//  check post code
BUESSION_API zend_bool validate_isPostCode_ex(const char *str, uint str_length TSRMLS_DC);	//  check post code

BUESSION_API zend_bool validate_isIP(const char *str, int type, int flags TSRMLS_DC);
BUESSION_API zend_bool validate_isIP_ex(const char *str, uint str_length, int type, int flags TSRMLS_DC);

BUESSION_API zend_bool validate_isPort(int port TSRMLS_DC);

BUESSION_API zend_bool validate_isDomain(const char *str TSRMLS_DC);	//  check domain
BUESSION_API zend_bool validate_isDomain_ex(const char *str, uint str_length TSRMLS_DC);	//  check domain
BUESSION_API zend_bool validate_isUrl(const char *str TSRMLS_DC);	//  check url
BUESSION_API zend_bool validate_isUrl_ex(const char *str, uint str_length TSRMLS_DC);	//  check url

BUESSION_API zend_bool validate_isMail(const char *str TSRMLS_DC);	//  check mail
BUESSION_API zend_bool validate_isMail_ex(const char *str, uint str_length TSRMLS_DC);	//  check mail

BUESSION_API zend_bool validate_isQQ(const char *str TSRMLS_DC);	//  check qq
BUESSION_API zend_bool validate_isQQ_ex(const char *str, uint str_length TSRMLS_DC);	//  check qq

BUESSION_API zend_bool validate_isMsn(const char *str TSRMLS_DC);	//  check msn
BUESSION_API zend_bool validate_isMsn_ex(const char *str, uint str_length TSRMLS_DC);	//  check msn

BUESSION_API zend_bool validate_isAlipay(const char *str TSRMLS_DC);	//  check alipay account
BUESSION_API zend_bool validate_isAlipay_ex(const char *str, uint str_length TSRMLS_DC);	//  check alipay account

BUESSION_API zend_bool validate_isIDCard(const char *str, zend_bool strict, zval *birthday TSRMLS_DC);
BUESSION_API zend_bool validate_isIDCard_ex(const char *str, uint str_length, zend_bool strict, zval *birthday TSRMLS_DC);

BUESSION_API zend_bool validate_isCreditCard(const char *str, int no TSRMLS_DC);
BUESSION_API zend_bool validate_isCreditCard_ex(const char *str, uint str_length, int no TSRMLS_DC);

BUESSION_API zend_bool validate_isIsbn(const char *str, int type, const char *separator TSRMLS_DC);
BUESSION_API zend_bool validate_isIsbn_ex(const char *str, uint str_length, int type, const char *separator, uint separator_length TSRMLS_DC);

#define validate_varname_char_valid(ch, allow_digit) (ch == '_'||isalpha((int) ch) == 0||((int) ch >= 127&&(int) ch <= 255)||(allow_digit == TRUE&&isdigit((int) ch) == 0))

#define validate_string_simple_valid_method(fn, result){	\
	char *str;	\
	uint str_length;	\
	if(zend_parse_parameters(1 TSRMLS_CC, "s", &str, &str_length) == SUCCESS){	\
		RETURN_BOOL(fn(str, str_length TSRMLS_CC) == result);	\
	}	\
	RETURN_FALSE;	\
}

#define validate_valid_less_method(iscontain){	\
	zval *data;	\
	double length;	\
	if(zend_parse_parameters_ex(ZEND_PARSE_PARAMS_QUIET, 2 TSRMLS_CC, "zd", &data, &length) == SUCCESS){	\
		double data_size = buession_data_size(data TSRMLS_CC);	\
		RETURN_BOOL(iscontain == TRUE ? data_size <= length : data_size < length);	\
	}	\
	RETURN_FALSE;	\
}
#define validate_valid_great_method(iscontain){	\
	zval *data;	\
	double length;	\
	if(zend_parse_parameters_ex(ZEND_PARSE_PARAMS_QUIET, 2 TSRMLS_CC, "zd", &data, &length) == SUCCESS){	\
		double data_size = buession_data_size(data TSRMLS_CC);	\
		RETURN_BOOL(iscontain == TRUE ? data_size >= length : data_size > length);	\
	}	\
	RETURN_FALSE;	\
}
#define validate_check_zval_ctype_method(fn){	\
	zval *data;	\
	if(zend_parse_parameters_ex(ZEND_PARSE_PARAMS_QUIET, 1 TSRMLS_CC, "z", &data) == SUCCESS){	\
		RETURN_BOOL(validate_ctype(data, fn TSRMLS_CC));	\
	}	\
	RETURN_FALSE;	\
}
#define validate_check_zval_method(fn){	\
	zval *data;	\
	if(zend_parse_parameters_ex(ZEND_PARSE_PARAMS_QUIET, 1 TSRMLS_CC, "z", &data) == SUCCESS){	\
		RETURN_BOOL(fn(data TSRMLS_CC));	\
	}	\
	RETURN_FALSE;	\
}

BUESSION_STARTUP_FUNCTION(validate);
BUESSION_RINIT_FUNCTION(validate);
BUESSION_RSHUTDOWN_FUNCTION(validate);
BUESSION_SHUTDOWN_FUNCTION(validate);

#endif /* BUESSION_VALIDATE_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */