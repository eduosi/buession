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

/* $Id: validate/validate.c 310447 2013-09-12 11:20:30Z Yong.Teng $ */

/* include php file */
#include "Zend/zend.h"
#include "Zend/zend_API.h"
#include "main/php.h"
#include "main/php_main.h"
#include "ext/standard/url.h"
#include "ext/date/php_date.h"
#include "ext/date/lib/timelib.h"
#include "ext/pcre/php_pcre.h"

/* include buession file */
#include "buession.h"
#include "validate.h"
#include "dict/domain.h"
#include "dict/creditcard/creditcard.h"
#include "calendar/calendar.h"

zend_class_entry *validate_ce;

/* {{{ arginfo */
ZEND_BEGIN_ARG_INFO_EX(validate_isNull_arguments, FALSE, FALSE, 1)
	ZEND_ARG_INFO(FALSE, data)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(validate_isBlank_arguments, FALSE, FALSE, 1)
	ZEND_ARG_INFO(FALSE, str)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(validate_length_arguments, FALSE, FALSE, 2)
	ZEND_ARG_INFO(FALSE, data)
	ZEND_ARG_INFO(FALSE, length)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(validate_isBetween_arguments, FALSE, FALSE, 3)
	ZEND_ARG_INFO(FALSE, data)
	ZEND_ARG_INFO(FALSE, min)
	ZEND_ARG_INFO(FALSE, max)
	ZEND_ARG_INFO(FALSE, iscontain)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(validate_lessEqual_arguments, FALSE, FALSE, 1)
	ZEND_ARG_INFO(FALSE, data)
	ZEND_ARG_INFO(FALSE, length)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(validate_lessThan_arguments, FALSE, FALSE, 1)
	ZEND_ARG_INFO(FALSE, data)
	ZEND_ARG_INFO(FALSE, length)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(validate_greatEqual_arguments, FALSE, FALSE, 1)
	ZEND_ARG_INFO(FALSE, data)
	ZEND_ARG_INFO(FALSE, length)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(validate_greatThan_arguments, FALSE, FALSE, 1)
	ZEND_ARG_INFO(FALSE, data)
	ZEND_ARG_INFO(FALSE, length)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(validate_isEqual_arguments, FALSE, FALSE, 1)
	ZEND_ARG_INFO(FALSE, data1)
	ZEND_ARG_INFO(FALSE, data2)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(validate_isAscii_arguments, FALSE, FALSE, 1)
	ZEND_ARG_INFO(FALSE, data)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(validate_isChinese_arguments, FALSE, FALSE, 1)
	ZEND_ARG_INFO(FALSE, str)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(validate_isEnglish_arguments, FALSE, FALSE, 1)
	ZEND_ARG_INFO(FALSE, str)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(validate_isAlpha_arguments, FALSE, FALSE, 1)
	ZEND_ARG_INFO(FALSE, str)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(validate_isLetter_arguments, FALSE, FALSE, 1)
	ZEND_ARG_INFO(FALSE, str)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(validate_isLower_arguments, FALSE, FALSE, 1)
	ZEND_ARG_INFO(FALSE, str)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(validate_isUpper_arguments, FALSE, FALSE, 1)
	ZEND_ARG_INFO(FALSE, str)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(validate_isNumeric_arguments, FALSE, FALSE, 1)
	ZEND_ARG_INFO(FALSE, data)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(validate_isDigital_arguments, FALSE, FALSE, 1)
	ZEND_ARG_INFO(FALSE, data)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(validate_isUnsignedNumeric_arguments, FALSE, FALSE, 1)
	ZEND_ARG_INFO(FALSE, data)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(validate_isSignedNumeric_arguments, FALSE, FALSE, 1)
	ZEND_ARG_INFO(FALSE, data)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(validate_isInteger_arguments, FALSE, FALSE, 1)
	ZEND_ARG_INFO(FALSE, data)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(validate_isUnsignedInteger_arguments, FALSE, FALSE, 1)
	ZEND_ARG_INFO(FALSE, data)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(validate_isSignedInteger_arguments, FALSE, FALSE, 1)
	ZEND_ARG_INFO(FALSE, data)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(validate_isFloat_arguments, FALSE, FALSE, 1)
	ZEND_ARG_INFO(FALSE, data)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(validate_isUnsignedFloat_arguments, FALSE, FALSE, 1)
	ZEND_ARG_INFO(FALSE, data)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(validate_isSignedFloat_arguments, FALSE, FALSE, 1)
	ZEND_ARG_INFO(FALSE, data)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(validate_isAlnum_arguments, FALSE, FALSE, 1)
	ZEND_ARG_INFO(FALSE, data)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(validate_isXdigit_arguments, FALSE, FALSE, 1)
	ZEND_ARG_INFO(FALSE, data)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(validate_isCntrl_arguments, FALSE, FALSE, 1)
	ZEND_ARG_INFO(FALSE, str)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(validate_isPrint_arguments, FALSE, FALSE, 1)
	ZEND_ARG_INFO(FALSE, str)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(validate_isCurrency_arguments, FALSE, FALSE, 1)
	ZEND_ARG_INFO(FALSE, str)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(validate_isTel_arguments, FALSE, FALSE, 1)
	ZEND_ARG_INFO(FALSE, str)
	ZEND_ARG_INFO(FALSE, flags)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(validate_isMobile_arguments, FALSE, FALSE, 1)
	ZEND_ARG_INFO(FALSE, str)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(validate_isPostCode_arguments, FALSE, FALSE, 1)
	ZEND_ARG_INFO(FALSE, str)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(validate_isIP_arguments, FALSE, FALSE, 1)
	ZEND_ARG_INFO(FALSE, str)
	ZEND_ARG_INFO(FALSE, type)
	ZEND_ARG_INFO(FALSE, flags)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(validate_isPort_arguments, FALSE, FALSE, 1)
	ZEND_ARG_INFO(FALSE, port)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(validate_isDomain_arguments, FALSE, FALSE, 1)
	ZEND_ARG_INFO(FALSE, str)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(validate_isUrl_arguments, FALSE, FALSE, 1)
	ZEND_ARG_INFO(FALSE, str)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(validate_isMail_arguments, FALSE, FALSE, 1)
	ZEND_ARG_INFO(FALSE, str)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(validate_isQQ_arguments, FALSE, FALSE, 1)
	ZEND_ARG_INFO(FALSE, str)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(validate_isMsn_arguments, FALSE, FALSE, 1)
	ZEND_ARG_INFO(FALSE, str)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(validate_isAlipay_arguments, FALSE, FALSE, 1)
	ZEND_ARG_INFO(FALSE, str)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(validate_isIDCard_arguments, FALSE, FALSE, 1)
	ZEND_ARG_INFO(FALSE, str)
	ZEND_ARG_INFO(FALSE, strict)
	ZEND_ARG_INFO(FALSE, birthday)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(validate_isCreditCard_arguments, FALSE, FALSE, 1)
	ZEND_ARG_INFO(FALSE, str)
	ZEND_ARG_INFO(FALSE, type)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(validate_isIsbn_arguments, FALSE, FALSE, 1)
	ZEND_ARG_INFO(FALSE, str)
	ZEND_ARG_INFO(FALSE, type)
	ZEND_ARG_INFO(FALSE, separator)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(validate_Regex_arguments, FALSE, FALSE, 1)
	ZEND_ARG_INFO(FALSE, str)
	ZEND_ARG_INFO(FALSE, pattern)
ZEND_END_ARG_INFO()
/* }}} */

const static uint IDCard_divisors[18] = {7, 9, 10, 5, 8, 4, 2, 1, 6, 3, 7, 9, 10, 5, 8, 4, 2};
const static char IDCard_check_codes[11] = {'1', '0', 'X', '9', '8', '7', '6', '5', '4', '3', '2'};

static inline zend_bool validate_ctype(zval *data, int (*fn)(int) TSRMLS_DC){
	if(!data){
		return FALSE;
	}

	if(Z_TYPE_P(data) == IS_LONG){
		if(Z_LVAL_P(data) >= 0&&Z_LVAL_P(data) <= 255){
			return fn(Z_LVAL_P(data));
		}else if(Z_LVAL_P(data) >= -128&&Z_LVAL_P(data) < 0){
			return fn(Z_LVAL_P(data) + 256);
		}

		convert_to_string(data);
	}else if(Z_TYPE_P(data) == IS_OBJECT){
		convert_to_string(data);
	}

	if(Z_TYPE_P(data) == IS_STRING){
		char *s = Z_STRVAL_P(data);
		char *p = Z_STRVAL_P(data) + Z_STRLEN_P(data);

		if(s == p){
			return FALSE;
		}

		while(s < p){
			if(!fn((int) *(unsigned char *)(s++))){
				return FALSE;
			}
		}

		return TRUE;
	}

	return FALSE;
}
static inline zend_bool validate_ctype_ex(const char *data, int data_length, int (*fn)(int) TSRMLS_DC){
	if(data == NULL||data_length < 0){
		return FALSE;
	}else{
		const char *s = data;
		const char *p = data + data_length;

		if(s == p){
			return FALSE;
		}

		while(s < p){
			if(!fn((int) *(unsigned char *)(s++))){
				return FALSE;
			}
		}

		return TRUE;
	}
}

static inline zend_bool validate_isInteger_valid(const char *str, uint str_length TSRMLS_DC){
	const char *end = str + str_length;
	const char *p = str;

	if(p == end){
		return FALSE;
	}

	while(p < end){
		if(*p < '0'||*p > '9'){
			return FALSE;
		}

		++p;
	}

	return TRUE;
}

BUESSION_API zend_bool validate_isNull(zval *data TSRMLS_DC){
	return data ? buession_data_size(data TSRMLS_CC) == 0.0 : TRUE;
}

BUESSION_API zend_bool validate_isBlank(const char *str TSRMLS_DC){
	return validate_isBlank_ex(str, strlen(str) TSRMLS_CC);
}
BUESSION_API zend_bool validate_isBlank_ex(const char *str, uint str_length TSRMLS_DC){
	if(str != NULL&&str_length != 0){
		uint i = 0;
		char *character = " \t\r\n\013\014";
		uint character_length = sizeof(" \t\r\n\013\014") - 1;

		for(; i < str_length; i++){
			if(memchr(character, str[i], character_length) == NULL){
				return FALSE;
			}
		}
	}

	return TRUE;
}

BUESSION_API zend_bool validate_isLessEqual(zval *data, double size, zend_bool iscontain TSRMLS_DC){
	double data_size = buession_data_size(data TSRMLS_CC);
	return iscontain == TRUE ? data_size <= size : data_size < size;
}
BUESSION_API zend_bool validate_isGreatEqual(zval *data, double size, zend_bool iscontain TSRMLS_DC){
	double data_size = buession_data_size(data TSRMLS_CC);
	return iscontain == TRUE ? data_size >= size : data_size > size;
}

BUESSION_API zend_bool validate_isAscii(zval *data TSRMLS_DC){
	return validate_ctype(data, isascii TSRMLS_CC);
}

BUESSION_API zend_bool validate_isVarName(const char *str, zend_bool vd TSRMLS_DC){
	return validate_isVarName_ex(str, strlen(str), vd TSRMLS_CC);
}
BUESSION_API zend_bool validate_isVarName_ex(const char *str, uint str_length, zend_bool vd TSRMLS_DC){
	if(str&&str_length > 0){
		/* validate first char is '$'*/
		if(vd == TRUE){
			if(*str != '$'){
				return FALSE;
			}

			/* These are allowed as first char: [a-zA-Z_\x7f-\xff] */
			if(validate_varname_char_valid(*(++str), FALSE) == FALSE){
				return FALSE;
			}
		}else{
			/* These are allowed as first char: [a-zA-Z_\x7f-\xff] */
			if(validate_varname_char_valid(*str, FALSE) == FALSE){
				return FALSE;
			}
		}

		/* And these as the rest: [a-zA-Z0-9_\x7f-\xff] */
		if(str_length > 1){
			uint i = vd == TRUE ? 2 : 1;

			for(; i < str_length; i++){
				if(validate_varname_char_valid(str[i], FALSE) == FALSE){
					return FALSE;
				}
			}
		}

		return TRUE;
	}else{
		return FALSE;
	}
}

BUESSION_API zend_bool validate_isChinese(const char *str TSRMLS_DC){
	return validate_isChinese_ex(str, strlen(str) TSRMLS_CC);
}
BUESSION_API zend_bool validate_isChinese_ex(const char *str, uint str_length TSRMLS_DC){
	pcre *re = NULL;
	pcre_extra *extra = NULL;
	int options = 0;
	int ovector[3];

	re = pcre_get_compiled_regex("/^[\x7f-\xff]+$/", &extra, &options TSRMLS_CC);
	return re == NULL||pcre_exec(re, NULL, str, str_length, 0, 0, ovector, 3) < 0 ? FALSE : TRUE;
}

BUESSION_API zend_bool validate_isAlpha(const char *str TSRMLS_DC){
	return validate_isAlpha_ex(str, strlen(str) TSRMLS_CC);
}
BUESSION_API zend_bool validate_isAlpha_ex(const char *str, uint str_length TSRMLS_DC){
	return validate_ctype_ex(str, str_length, isalpha TSRMLS_CC);
}
BUESSION_API zend_bool validate_isLowerLetter(const char *str TSRMLS_DC){
	return validate_isLowerLetter_ex(str, strlen(str) TSRMLS_CC);
}
BUESSION_API zend_bool validate_isLowerLetter_ex(const char *str, uint str_length TSRMLS_DC){
	return validate_ctype_ex(str, str_length, islower TSRMLS_CC);
}
BUESSION_API zend_bool validate_isUpperLetter(const char *str TSRMLS_DC){
	return validate_isUpperLetter_ex(str, strlen(str) TSRMLS_CC);
}
BUESSION_API zend_bool validate_isUpperLetter_ex(const char *str, uint str_length TSRMLS_DC){
	return validate_ctype_ex(str, str_length, isupper TSRMLS_CC);
}

static inline int _validate_numeric_valid(zval *data, int type TSRMLS_DC){
	if(!data){
	}else if(Z_TYPE_P(data) == IS_LONG){
		return type&IS_LONG;
	}else if(Z_TYPE_P(data) == IS_DOUBLE){
		return type&IS_DOUBLE;
	}else if(Z_TYPE_P(data) == IS_STRING){
		switch(is_numeric_string(Z_STRVAL_P(data), Z_STRLEN_P(data), NULL, NULL, 0)){
			case IS_LONG:
				return type&IS_LONG;
				break;
			case IS_DOUBLE:
				return type&IS_DOUBLE;
				break;
			default:
				break;
		}
	}else if(Z_TYPE_P(data) == IS_OBJECT){
		if(buession_convert_object_to_string(data TSRMLS_CC) == SUCCESS){
			zend_bool result = FALSE;

			switch(is_numeric_string(Z_STRVAL_P(data), Z_STRLEN_P(data), NULL, NULL, 0)){
				case IS_LONG:
					result = type&IS_LONG;
					break;
				case IS_DOUBLE:
					result = type&IS_DOUBLE;
					break;
				default:
					break;
			}
			buession_zval_ptr_dtor(data);

			return result;
		}

		buession_zval_ptr_dtor(data);
	}

	return FALSE;
}

static inline int _validate_isUnsigned_or_isSigned_numeric_valid(zval *data, int type, unsigned short symbol_type TSRMLS_DC){
	if(!data){
	}else if(Z_TYPE_P(data) == IS_LONG){
		return (type&IS_LONG)&&(symbol_type == 1 ? Z_LVAL_P(data) >= 0 : Z_LVAL_P(data) < 0);
	}else if(Z_TYPE_P(data) == IS_DOUBLE){
		return (type&IS_DOUBLE)&&(symbol_type == 1 ? Z_DVAL_P(data) >= 0.0 : Z_DVAL_P(data) < 0.0);
	}else if(Z_TYPE_P(data) == IS_STRING){
		long l = 0;
		double d = 0.0;

		switch(is_numeric_string(Z_STRVAL_P(data), Z_STRLEN_P(data), &l, &d, 0)){
			case IS_LONG:
				return (type&IS_LONG)&&(symbol_type == 1 ? l >= 0 : l < 0);
				break;
			case IS_DOUBLE:
				return (type&IS_DOUBLE)&&(symbol_type == 1 ? d >= 0.0 : d < 0.0);
				break;
			default:
				break;
		}
	}else if(Z_TYPE_P(data) == IS_OBJECT){
		if(buession_convert_object_to_string(data TSRMLS_CC) == SUCCESS){
			long l = 0;
			double d = 0.0;
			zend_bool result = FALSE;

			switch(is_numeric_string(Z_STRVAL_P(data), Z_STRLEN_P(data), &l, &d, 0)){
				case IS_LONG:
					result = (type&IS_LONG)&&(symbol_type == 1 ? l >= 0 : l < 0);
					break;
				case IS_DOUBLE:
					result = (type&IS_DOUBLE)&&(symbol_type == 1 ? d >= 0.0 : d < 0.0);
					break;
				default:
					break;
			}
			buession_zval_ptr_dtor(data);

			return result;
		}

		buession_zval_ptr_dtor(data);
	}

	return FALSE;
}

BUESSION_API zend_bool validate_isNumeric(zval *data TSRMLS_DC){
	return _validate_numeric_valid(data, IS_LONG|IS_DOUBLE TSRMLS_CC);
}
BUESSION_API zend_bool validate_isUnsignedNumeric(zval *data TSRMLS_DC){
	return _validate_isUnsigned_or_isSigned_numeric_valid(data, IS_LONG|IS_DOUBLE, 1 TSRMLS_CC);
}
BUESSION_API zend_bool validate_isSignedNumeric(zval *data TSRMLS_DC){
	return _validate_isUnsigned_or_isSigned_numeric_valid(data, IS_LONG|IS_DOUBLE, 0 TSRMLS_CC);
}
BUESSION_API zend_bool validate_isInteger(zval *data TSRMLS_DC){
	return _validate_numeric_valid(data, IS_LONG TSRMLS_CC);
}
BUESSION_API zend_bool validate_isUnsignedInteger(zval *data TSRMLS_DC){
	return _validate_isUnsigned_or_isSigned_numeric_valid(data, IS_LONG, 1 TSRMLS_CC);
}
BUESSION_API zend_bool validate_isSignedInteger(zval *data TSRMLS_DC){
	return _validate_isUnsigned_or_isSigned_numeric_valid(data, IS_LONG, 0 TSRMLS_CC);
}
BUESSION_API zend_bool validate_isFloat(zval *data TSRMLS_DC){
	return _validate_numeric_valid(data, IS_DOUBLE TSRMLS_CC);
}
BUESSION_API zend_bool validate_isUnsignedFloat(zval *data TSRMLS_DC){
	return _validate_isUnsigned_or_isSigned_numeric_valid(data, IS_DOUBLE, 1 TSRMLS_CC);
}
BUESSION_API zend_bool validate_isSignedFloat(zval *data TSRMLS_DC){
	return _validate_isUnsigned_or_isSigned_numeric_valid(data, IS_DOUBLE, 0 TSRMLS_CC);
}

BUESSION_API zend_bool validate_isAlnum(zval *data TSRMLS_DC){
	return validate_ctype(data, isalnum TSRMLS_CC);
}

BUESSION_API zend_bool validate_isXdigit(zval *data TSRMLS_DC){
	return validate_ctype(data, isxdigit TSRMLS_CC);
}

BUESSION_API zend_bool validate_isCntrl(const char *str TSRMLS_DC){
	return validate_isCntrl_ex(str, strlen(str) TSRMLS_CC);
}
BUESSION_API zend_bool validate_isCntrl_ex(const char *str, uint str_length TSRMLS_DC){
	return validate_ctype_ex(str, str_length, iscntrl TSRMLS_CC);
}
BUESSION_API zend_bool validate_isPrint(const char *str TSRMLS_DC){
	return validate_isPrint_ex(str, strlen(str) TSRMLS_CC);
}
BUESSION_API zend_bool validate_isPrint_ex(const char *str, uint str_length TSRMLS_DC){
	return validate_ctype_ex(str, str_length, isprint TSRMLS_CC);
}

BUESSION_API zend_bool validate_isCurrency(const char *str TSRMLS_DC){
	return validate_isCurrency_ex(str, strlen(str) TSRMLS_CC);
}
BUESSION_API zend_bool validate_isCurrency_ex(const char *str, uint str_length TSRMLS_DC){
	return FALSE;//buession_regex_match_ex(str, str_length, ZEND_STRL(VALIDATE_CURRENCY), FALSE, 0, NULL TSRMLS_CC);
}

static inline zend_bool validate_tel_tail_valid(const char *str, uint str_length TSRMLS_DC){
	if(str != NULL&&str_length > 0){
		const char *p = str;
		const char *end = str + str_length;

		if(*p == '0'){
			if(str_length < 3||str_length > 4){
				return FALSE;
			}

			++p;
			if(*p < '1'||*p > '9'){
				return FALSE;
			}

			++p;
			if(validate_isInteger_valid(p, str_length - (p - str) TSRMLS_CC) == TRUE){
				return TRUE;
			}
		}else if(*p == '8'){
			if(str_length < 4||str_length > 5){
				return FALSE;
			}

			++p;
			if(*p != '6'){
				return FALSE;
			}

			++p;
			if(validate_isInteger_valid(p, str_length - (p - str) TSRMLS_CC) == TRUE){
				return TRUE;
			}
		}
	}

	return FALSE;
}
BUESSION_API zend_bool validate_isTel(const char *str, int flags TSRMLS_DC){
	return validate_isTel_ex(str, strlen(str), flags TSRMLS_CC);
}
BUESSION_API zend_bool validate_isTel_ex(const char *str, uint str_length, int flags TSRMLS_DC){
	if(str != NULL&&str_length > 2){
		const char *p = str;
		const char *end = str + str_length;

		/*if(flags&TEL_TAIL){
			char *a = NULL;

			if(*p == '('){
				if((a = (char *) memchr(p, ')', str_length)) == NULL){
					return FALSE;
				}

				++p;
				if(validate_tel_tail_valid(p, a - p TSRMLS_CC) == FALSE){
					return FALSE;
				}
			}else if(*p >= '0'&&*p <= '9'){
				if((a = (char *) memchr(p, '-', str_length)) == NULL){
					return FALSE;
				}

				if(validate_tel_tail_valid(p, a - str TSRMLS_CC) == FALSE){
					return FALSE;
				}
			}else{
				return FALSE;
			}

			++a;
			p = a;
		}

		if(p < end){
			if(*p < '1'||*p > '9'){
				return FALSE;
			}

			++p;
			if(p < end){
				char *line = (char *) memchr(p, '-', str_length - (p - str));
				uint l = line == NULL ? str_length - (p - str) : line - p;

				if(l < 6|| l > 7||(line == NULL&&(flags&TEL_EXTENSION))){
					return FALSE;
				}

				if(validate_isInteger_valid(p, l TSRMLS_CC) == FALSE){
					return FALSE;
				}

				if(flags&TEL_EXTENSION){
					if(++line < end){
						l = str_length - (line - str);
						if(l >= 1&&l <= 5){
							return validate_isInteger_valid(line, l TSRMLS_CC);
						}
					}
				}else{
					return TRUE;
				}
			}
		}*/
	}

	return FALSE;
}
BUESSION_API zend_bool validate_isMobile(const char *str TSRMLS_DC){
	return validate_isMobile_ex(str, strlen(str) TSRMLS_CC);
}
BUESSION_API zend_bool validate_isMobile_ex(const char *str, uint str_length TSRMLS_DC){
	if(str != NULL&&str_length == 11){
		const char *p = str;

		if(*p != '1'){
			return FALSE;
		}

		++p;
		if(*p == '3'||*p == '8'){
			return validate_isInteger_valid(++p, str_length - (p - str) TSRMLS_CC);
		}else if(*p == '4'){
			++p;
			if(*p != '4'&&*p != '7'){
				return FALSE;
			}

			return validate_isInteger_valid(++p, str_length - (p - str) TSRMLS_CC);
		}else if(*p == '5'){
			if(*(++p) == '4'){
				return FALSE;
			}

			return validate_isInteger_valid(++p, str_length - (p - str) TSRMLS_CC);
		}else{
			return FALSE;
		}

		return TRUE;
	}

	return FALSE;
}

BUESSION_API zend_bool validate_isPostCode(const char *str TSRMLS_DC){
	return validate_isPostCode_ex(str, strlen(str) TSRMLS_CC);
}
BUESSION_API zend_bool validate_isPostCode_ex(const char *str, uint str_length TSRMLS_DC){
	if(str != NULL&&str_length == 6){
		if(*str >= '1'&&*str <= '9'){
			return validate_isInteger_valid(++str, str_length - 1 TSRMLS_CC);
		}
	}

	return FALSE;
}

static inline zend_bool validate_isIPV4(const char *str, uint str_length, int flags TSRMLS_DC){
	if(str_length >= 7&&str_length <= 15){
		const char *end = str + str_length;
		const char *p = str;
		int ips[4];
		int num;
		int m;
		int n = 0;

		while(p < end){
			zend_bool leading_zero;

			if(n > 4||*str < '0'||*str > '9'){
				return FALSE;
			}

			leading_zero = (*p == '0');
			m = 1;
			num = ((*(p++)) - '0');
			while(str < end&&(*p >= '0'&&*p <= '9')){
				num = num * 10 + ((*(p++)) - '0');
				if(num > 255||++m > 3){
					return FALSE;
				}
			}

			/* don't allow a leading 0; that introduces octal numbers, which we don't support */
			if(leading_zero == TRUE&&(num != 0||m > 1)){
				return FALSE;
			}

			ips[n++] = num;

			if(n == 4){
				return str == end;
			}else if(p >= end||*(str++) != '.'){
				return FALSE;
			}
		}

		if(flags&IP_PRIV_RANGE){
			if((ips[0] == 10)
				||(ips[0] == 172&&(ips[1] >= 16&&ips[1] <= 31))
				||(ips[0] == 192&&ips[1] == 168)){
				return FALSE;
			}
		}

		if(flags&IP_RES_RANGE){
			if((ips[0] == 0)
				||(ips[0] == 127&&ips[1] == 0&&ips[2] == 0&&ips[3] == 1)
				||(ips[0] == 128&&ips[1] == 0)
				||(ips[0] == 169&&ips[1] == 254)
				||(ips[0] == 191&&ips[1] == 255)
				||(ips[0] == 192&&ips[1] == 0&&ips[2] == 2)
				||(ips[0] >= 224&&ips[0]<= 255)){
				return FALSE;
			}
		}

		return TRUE;
	}

	return FALSE;
}
static inline zend_bool _validate_IPV6_group_valid(const char *str, uint str_length TSRMLS_DC){
	if(str != NULL&&str_length >= 1&&str_length <= 4){
		const char *end = str + str_length;
		const char *p = str;

		while(p < end){
			if(!isxdigit((int) *(unsigned char *)(p++))){
				return FALSE;
			}
		}

		return TRUE;
	}

	return FALSE;
}
static inline zend_bool validate_isIPV6(const char *str, uint str_length, int flags TSRMLS_DC){
	PUTS("hhh");
	if(str != NULL&&str_length >= 2){
		if(str_length == 2){
			return str[0] == ':'&&str[1] == ':';
		}else{
			const char *p = (const char *) memchr(str, ':', str_length);

			PUTS("GGG");
			if(p != NULL){
				const char *s = str;
				const char *end = str + str_length;
				uint l = 0;
				uint n = 1;
				uint i = 1;

				do{
					if(i > 8||n > 2){
						return FALSE;
					}

					l = p - s;
					if(l == 0){
						++n;
					}else if(_validate_IPV6_group_valid(s, l TSRMLS_CC) == FALSE){
						return FALSE;
					}

					p = p + 1;
					s = p;
					++i;
				}while((p = (const char *) memchr(p, ':', str_length - (p - str))));

				if(i <= 8&&p < end){	/* last group */
					zend_bool result = FALSE;

					l = str_length - (s - str);
					if(i <= 7){
						if(_validate_IPV6_group_valid(s, l TSRMLS_CC) == TRUE||validate_isIPV4(s, l, flags TSRMLS_CC) == TRUE){
							result = TRUE;
						}
					}else if(_validate_IPV6_group_valid(s, l TSRMLS_CC) == TRUE){
						result = TRUE;
					}

					if(result == TRUE){
						if(flags&IP_PRIV_RANGE){
							PUTS("GG\r\n");
							if(str_length >=2&&(strncasecmp("FC", str, 2) == 0||strncasecmp("FD", str, 2) == 0)){
								PUTS("HH\r\n");
								return FALSE;
							}
						}

						if(flags&IP_RES_RANGE){
							PUTS("FF\r\n");
							if(str_length == 3&&(memcmp("::1", str, 3) == 0||memcmp("5f:", str, 3) == 0)){
								return FALSE;
							}

							if(str_length >= 5&&(strncasecmp("fe8", str, 3) == 0||strncasecmp("fe9", str, 3) == 0||strncasecmp("fea", str, 3) == 0||strncasecmp("feb", str, 3) == 0)){
								return FALSE;
							}

							if((str_length >= 9&&strncasecmp("2001:0db8", str, 9) == 0)
								||(str_length >= 2&&strncasecmp("5f", str, 2) == 0)
								||(str_length >= 4&&strncasecmp("3ff3", str, 4) == 0)
								||(str_length >= 8&&memcmp("2001:001", str, 8) == 0)){
								PUTS("MM\r\n");
								return FALSE;
							}
						}
					}

					return result;
				}
			}
		}
	}

	return FALSE;
}
BUESSION_API zend_bool validate_isIP(const char *str, int type, int flags TSRMLS_DC){
	return validate_isIP_ex(str, strlen(str), type, flags TSRMLS_CC);
}
BUESSION_API zend_bool validate_isIP_ex(const char *str, uint str_length, int type, int flags TSRMLS_DC){
	php_printf("%ld: %ld=>%ld\r\n", type, type&IPV4, type&IPV6);
	if((type&IPV4)&&validate_isIPV4(str, str_length, flags TSRMLS_CC) == TRUE){
		return TRUE;
	}

	if((type&IPV6)&&validate_isIPV6(str, str_length, flags TSRMLS_CC) == TRUE){
		return TRUE;
	}

	return FALSE;
}

BUESSION_API zend_bool validate_isPort(int port TSRMLS_DC){
	return port > 0&&port <= 65535;
}

BUESSION_API zend_bool validate_isDomain(const char *str TSRMLS_DC){
	return validate_isDomain_ex(str, strlen(str) TSRMLS_CC);
}
BUESSION_API zend_bool validate_isDomain_ex(const char *str, uint str_length TSRMLS_DC){
	if(str != NULL&&str_length > 0){
		const char *p = (const char *) memchr(str, '.', str_length);

		if(p != NULL){
			const char *s = str;
			const char *end = str + str_length;
			int c = 0;
			uint l = 0;
			uint j = 0;
			uint i = 1;

			do{
				l = p - s;

				if(l == 0){
					return FALSE;
				}

				c = (int) *(unsigned char *) s;
				if(isalnum(c) == FALSE){
					return FALSE;
				}

				for(j = 1; j < l; j++){
					c = (int) (s[j]);
					if(isalnum(c) == FALSE&&s[j] != '-'&&s[j] != '_'){
						return FALSE;
					}
				}

				p = p + 1;
				s = p;
				++i;
			}while((p = (const char *) memchr(p, '.', str_length - (p - str))));

			if(i >= 2&&p < end){	/* suffix valid */
				const domain_suffix_entry *e;

				l = str_length - (s - str);

				for(; (e = &domain_suffixs[i])->value != NULL; i++){
					if(e->length == l&&strncasecmp(e->value, s, l) == 0){
						return TRUE;
					}
				}
			}
		}
	}

	return FALSE;
}
BUESSION_API zend_bool validate_isUrl(const char *str TSRMLS_DC){
	return validate_isUrl_ex(str, strlen(str) TSRMLS_CC);
}
BUESSION_API zend_bool validate_isUrl_ex(const char *str, uint str_length TSRMLS_DC){
	php_url *url = php_url_parse_ex(str, str_length);

	if(url == NULL){
		return FALSE;
	}else{
		if(url->scheme != NULL&&(strcasecmp(url->scheme, "http") == 0||strcasecmp(url->scheme, "https") == 0)){
			char *end;
			char *p = url->host;

			if(url->host == NULL) {
				goto failure;
			}

			if(isalnum((int) *(unsigned char *) p) == FALSE){
				goto failure;
			}

			end = url->host + strlen(url->host);
			while(p < end){
				if(!isalnum((int) *(unsigned char *) p)&&*p != '-'&&*p != '.'){
					goto failure;
				}

				p++;
			}

			if (*(end - 1) == '.'){
				goto failure;
			}
		}

		if(url->scheme == NULL
			/* some schemas allow the host to be empty */
			||(url->host == NULL&&(memcmp(url->scheme, ZEND_STRL("mailto")) == 0&&memcmp(url->scheme, ZEND_STRL("news")) == 0&&memcmp(url->scheme, ZEND_STRL("file")) == 0))){
			failure:
			php_url_free(url);
			return FALSE;
		}

		php_url_free(url);
		return TRUE;
	}
}

BUESSION_API zend_bool validate_isMail(const char *str TSRMLS_DC){
	return validate_isMail_ex(str, strlen(str) TSRMLS_CC);
}
BUESSION_API zend_bool validate_isMail_ex(const char *str, uint str_length TSRMLS_DC){
	/* The maximum length of an e-mail address is 320 octets, per RFC 2821. */
	if(str_length <= 320){
		pcre *re = NULL;
		pcre_extra *extra = NULL;
		int options = 0;
		int ovector[150];

		re = pcre_get_compiled_regex("/^(?!(?:(?:\\x22?\\x5C[\\x00-\\x7E]\\x22?)|(?:\\x22?[^\\x5C\\x22]\\x22?)){255,})(?!(?:(?:\\x22?\\x5C[\\x00-\\x7E]\\x22?)|(?:\\x22?[^\\x5C\\x22]\\x22?)){65,}@)(?:(?:[\\x21\\x23-\\x27\\x2A\\x2B\\x2D\\x2F-\\x39\\x3D\\x3F\\x5E-\\x7E]+)|(?:\\x22(?:[\\x01-\\x08\\x0B\\x0C\\x0E-\\x1F\\x21\\x23-\\x5B\\x5D-\\x7F]|(?:\\x5C[\\x00-\\x7F]))*\\x22))(?:\\.(?:(?:[\\x21\\x23-\\x27\\x2A\\x2B\\x2D\\x2F-\\x39\\x3D\\x3F\\x5E-\\x7E]+)|(?:\\x22(?:[\\x01-\\x08\\x0B\\x0C\\x0E-\\x1F\\x21\\x23-\\x5B\\x5D-\\x7F]|(?:\\x5C[\\x00-\\x7F]))*\\x22)))*@(?:(?:(?!.*[^.]{64,})(?:(?:(?:xn--)?[a-z\\d]+(?:-+[a-z\\d]+)*\\.){1,126}){1,}(?:(?:[a-z][a-z\\d]*)|(?:(?:xn--)[a-z\\d]+))(?:-+[a-z\\d]+)*)|(?:\\[(?:(?:IPv6:(?:(?:[a-f\\d]{1,4}(?::[a-f\\d]{1,4}){7})|(?:(?!(?:.*[a-f\\d][:\\]]){7,})(?:[a-f\\d]{1,4}(?::[a-f\\d]{1,4}){0,5})?::(?:[a-f\\d]{1,4}(?::[a-f\\d]{1,4}){0,5})?)))|(?:(?:IPv6:(?:(?:[a-f\\d]{1,4}(?::[a-f\\d]{1,4}){5}:)|(?:(?!(?:.*[a-f\\d]:){5,})(?:[a-f\\d]{1,4}(?::[a-f\\d]{1,4}){0,3})?::(?:[a-f\\d]{1,4}(?::[a-f\\d]{1,4}){0,3}:)?)))?(?:(?:25[0-5])|(?:2[0-4]\\d)|(?:1\\d{2})|(?:[1-9]?\\d))(?:\\.(?:(?:25[0-5])|(?:2[0-4]\\d)|(?:1\\d{2})|(?:[1-9]?\\d))){3}))\\]))$/iD", &extra, &options TSRMLS_CC);
		return re == NULL||pcre_exec(re, NULL, str, str_length, 0, 0, ovector, 3) < 0 ? FALSE : TRUE;
	}

	return FALSE;
}

BUESSION_API zend_bool validate_isQQ(const char *str TSRMLS_DC){
	return validate_isQQ_ex(str, strlen(str) TSRMLS_CC);
}
BUESSION_API zend_bool validate_isQQ_ex(const char *str, uint str_length TSRMLS_DC){
	if(str != NULL&&str_length >= 5&&str_length <= 10){
		return (*str >= '1'&&*str <= '9')&&validate_isInteger_valid(++str, str_length - 1 TSRMLS_CC) == TRUE;
	}

	return FALSE;
}

BUESSION_API zend_bool validate_isMsn(const char *str TSRMLS_DC){
	return validate_isMsn_ex(str, strlen(str) TSRMLS_CC);
}
BUESSION_API zend_bool validate_isMsn_ex(const char *str, uint str_length TSRMLS_DC){
	return validate_isMail_ex(str, str_length TSRMLS_CC);
}

BUESSION_API zend_bool validate_isAlipay(const char *str TSRMLS_DC){
	return validate_isAlipay_ex(str, strlen(str) TSRMLS_CC);
}
BUESSION_API zend_bool validate_isAlipay_ex(const char *str, uint str_length TSRMLS_DC){
	return validate_isMobile_ex(str, str_length TSRMLS_CC) == TRUE||validate_isMail_ex(str, str_length TSRMLS_CC);
}

BUESSION_API zend_bool validate_isIDCard(const char *str, zend_bool strict, zval *birthday TSRMLS_DC){
	return validate_isIDCard_ex(str, strlen(str), strict, birthday TSRMLS_CC);
}
BUESSION_API zend_bool validate_isIDCard_ex(const char *str, uint str_length, zend_bool strict, zval *birthday TSRMLS_DC){
	if(str != NULL&&str_length == 18){
		const char *end = str + str_length;
		uint l = str_length - 1;
		uint sum = 0;
		uint i = 0;
		char check_code;

		if(strict == TRUE&&birthday == NULL){
			return FALSE;
		}

		for(; i < l; i++){
			if(str[i] < '0'||str[i] > '9'){
				return FALSE;
			}

			sum += (str[i] - '0') * IDCard_divisors[i];
		}

		check_code = IDCard_check_codes[sum % 11];
		if(check_code == str[l]){
			if(strict == TRUE){
				time_t timestamp;
				time_t s_timestamp;

				if(calendar_string_to_timestamp((char *) (str + 6), 8, &s_timestamp TSRMLS_CC) == FAILURE){
					return FALSE;
				}

				switch(Z_TYPE_P(birthday)){
					case IS_STRING:
						if(calendar_string_to_timestamp(Z_STRVAL_P(birthday), Z_STRLEN_P(birthday), &timestamp TSRMLS_CC) == SUCCESS){
							return s_timestamp == timestamp;
						}
						break;
					case IS_LONG:
						return s_timestamp == (time_t) Z_LVAL_P(birthday);
						break;
					case IS_OBJECT: {
							zend_class_entry *object_ce = Z_OBJCE_P(birthday);

							if(instanceof_function(object_ce, php_date_get_date_ce() TSRMLS_CC) == FALSE){
								if(buession_convert_object_to_string(birthday TSRMLS_CC) == SUCCESS){
									if(calendar_string_to_timestamp(Z_STRVAL_P(birthday), Z_STRLEN_P(birthday), &timestamp TSRMLS_CC) == SUCCESS){
										return s_timestamp == timestamp;
									}
								}else{
									char *fmethod = get_active_function_name(TSRMLS_C);
									buession_throw_exception_ex(parseexception_ce, E_PARSE, "Argument 1 passed to %s::%() must be an instance of DateTime, instance of %s given", 0, php_date_get_date_ce()->name, fmethod, object_ce->name);
								}

								return FALSE;
							}else{
								zval *ztimestamp;

								if(buession_call_method(object_ce, birthday, "getTimestamp", 12, 0, NULL, &ztimestamp TSRMLS_CC) == SUCCESS){
									timestamp = (time_t) Z_LVAL_P(ztimestamp);
									buession_zval_ptr_dtor(ztimestamp);
									return s_timestamp == timestamp;
								}
							}
						}
						break;
					default:
						break;
				}
			}else{
				return TRUE;
			}
		}
	}

	return FALSE;
}

BUESSION_API zend_bool validate_isCreditCard(const char *str, int no TSRMLS_DC){
	return validate_isCreditCard_ex(str, strlen(str), no TSRMLS_CC);
}
BUESSION_API zend_bool validate_isCreditCard_ex(const char *str, uint str_length, int no TSRMLS_DC){
	switch(buession_creditcard_valid(str, str_length, no)){
		case CREDITCARD_VALID_STSTE_INVALID:
			return FALSE;
			break;
		case CREDITCARD_VALID_STSTE_SUCCESS:
			return TRUE;
			break;
		default:
			break;
	}

	buession_throw_exception_ex(invalidexception_ce, E_NOTICE, "Invalid type '%ld', must between %d to %d", 0, no, CREDITCARD_NO_AMERICAN_EXPRESS, CREDITCARD_NO_VISA);
	return FALSE;
}

static inline zend_bool validate_isIsbn_10_valid(const char *str, uint str_length, const char *separator, uint separator_length TSRMLS_DC){
	uint i = 0;
	uint j = 0;
	char lash_ch = str[str_length - 1];
	uint sum = 0;
	uint checksum = 0;
	char ch;

	if(separator == NULL&&separator_length == 0){
		if(str_length != 10){
			return FALSE;
		}

		if((lash_ch < '0'||lash_ch > '9')&&lash_ch != 'X'){
			return FALSE;
		}

		for(; i < 9; i++){
			if(str[i] < '0'||str[i] > '9'){
				return FALSE;
			}
		}
	}else{
		uint sl = 0;
		uint gl = 0;

		if(str_length != 13){
			return FALSE;
		}

		if(*str < '0'||*str > '9'||((lash_ch < '0'||lash_ch > '9')&&lash_ch != 'X')){
			return FALSE;
		}

		for(i = 1; i < 12; i++){
			if(sl > 3||gl > 7){
				return FALSE;
			}

			if(str[i] == *separator){
				if(str[i - 1] == *separator){
					return FALSE;
				}

				++sl;
				gl = 0;
				continue;
			}else if(str[i] < '0'||str[i] > '9'){
				return FALSE;
			}else{
				++gl;
			}
		}

		if(sl != 3){
			return FALSE;
		}
	}

	for(i = 0; i < str_length; i++){
		if(j == 9){
			break;
		}

		if(str[i] == *separator){
			continue;
		}

		sum += (10 - j++) * (str[i] - '0');
	}

	checksum = 11 - (sum % 11);
	switch(checksum){
		case 11:
			ch = '0';
			break;
		case 10:
			ch = 'X';
			break;
		default:
			ch = checksum + '0';
			break;
	}

	return ch == lash_ch;
}
static inline zend_bool validate_isIsbn_13_valid(const char *str, uint str_length, const char *separator, uint separator_length TSRMLS_DC){
	uint i = 0;
	uint j = 0;
	uint sum = 0;
	uint checksum = 0;

	if(separator == NULL&&separator_length == 0){
		if(str_length != 13||validate_isInteger_valid(str, str_length TSRMLS_CC) == FALSE){
			return FALSE;
		}
	}else{
		uint sl = 0;
		uint gl = 0;

		if(str_length != 17){
			return FALSE;
		}

		if(*str < '0'||*str > '9'){
			return FALSE;
		}

		for(i = 1; i < str_length; i++){
			if(sl > 4||(sl == 1&&gl > 5)||gl > 9){
				return FALSE;
			}

			if(str[i] == *separator){
				if(str[i - 1] == *separator){
					return FALSE;
				}

				++sl;
				gl = 0;
				continue;
			}else if(str[i] < '0'||str[i] > '9'){
				return FALSE;
			}else{
				++gl;
			}
		}

		if(sl != 4){
			return FALSE;
		}
	}

	for(i = 0; i < str_length; i++){
		if(j == 12){
			break;
		}

		if(str[i] == *separator){
			continue;
		}

		sum += j++ % 2 == 0 ? (str[i] - '0') : 3 * (str[i] - '0');
	}

	checksum = 10 - (sum % 10);

	return checksum == 10 ? '0' : checksum + '0' == str[str_length - 1];
}
BUESSION_API zend_bool validate_isIsbn(const char *str, int type, const char *separator TSRMLS_DC){
	return validate_isIsbn_ex(str, strlen(str), type, separator, strlen(separator) TSRMLS_CC);
}
BUESSION_API zend_bool validate_isIsbn_ex(const char *str, uint str_length, int type, const char *separator, uint separator_length TSRMLS_DC){
	if(str&&str_length > 0){
		if((type&ISBN_10)&&validate_isIsbn_10_valid(str, str_length, separator, separator_length TSRMLS_CC) == TRUE){
			return TRUE;
		}

		if((type&ISBN_13)&&validate_isIsbn_13_valid(str, str_length, separator, separator_length TSRMLS_CC) == TRUE){
			return TRUE;
		}
	}

	return FALSE;
}

/* {{{ private void Validate::__construct() */
static BUESSION_METHOD(validate, __construct){
}
/* }}} */

/* {{{ public boolean Validate::isNull(mixed $data) */
static BUESSION_METHOD(validate, isNull){
	zval *data;

	if(zend_parse_parameters(1 TSRMLS_CC, "z", &data) == SUCCESS){
		RETURN_BOOL(validate_isNull(data TSRMLS_CC) == TRUE);
	}

	RETURN_TRUE;
}
/* }}} */

/* {{{ public boolean Validate::isBlank(string $str) */
static BUESSION_METHOD(validate, isBlank){
	validate_string_simple_valid_method(validate_isBlank_ex, TRUE);
}
/* }}} */

/* {{{ public boolean Validate::length(mixed $data, integer $length) */
static BUESSION_METHOD(validate, length){
	zval *data;
	int length;

	if(zend_parse_parameters(2 TSRMLS_CC, "zl", &data, &length) == SUCCESS){
		if(data == NULL||Z_TYPE_P(data) == IS_NULL){
			RETURN_BOOL(0 == length);
		}else if(Z_TYPE_P(data) == IS_ARRAY){
			RETURN_BOOL(zend_hash_num_elements(Z_ARRVAL_P(data)) == length);
		}else if(Z_TYPE_P(data) == IS_OBJECT){
			RETURN_BOOL(buession_get_object_size(data TSRMLS_CC) == length);
		}else if(Z_TYPE_P(data) == IS_STRING){
			RETURN_BOOL(Z_STRLEN_P(data) == length);
		}else if(Z_TYPE_P(data) == IS_LONG){
			RETURN_BOOL(Z_LVAL_P(data) == length);
		}else{
			buession_method_argument_invalid_exception(1, E_NOTICE, "string, array, object or null", data);
		}
	}

	RETURN_FALSE;
}
/* }}} */

/* {{{ public boolean Validate::isBetween(mixed $data, integer/float $min, integer/float $max[, boolean $iscontain = true]) */
static BUESSION_METHOD(validate, isBetween){
	zval *data;
	double min;
	double max;
	zend_bool iscontain = TRUE;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "zdd|b", &data, &min, &max, &iscontain) == SUCCESS){
		double data_size = buession_data_size(data TSRMLS_CC);
		RETURN_BOOL(iscontain == TRUE ? data_size >= min&&data_size <= max : data_size > min&&data_size < max);
	}

	RETURN_FALSE;
}
/* }}} */

/* {{{ public boolean Validate::lessEqual(mixed $data, integer/float $length) */
static BUESSION_METHOD(validate, lessEqual){
	validate_valid_less_method(TRUE);
}
/* }}} */

/* {{{ public boolean Validate::lessThan(mixed $data, integer/float $length) */
static BUESSION_METHOD(validate, lessThan){
	validate_valid_less_method(FALSE);
}
/* }}} */

/* {{{ public boolean Validate::greatEqual(mixed $data, integer/float $length) */
static BUESSION_METHOD(validate, greatEqual){
	validate_valid_great_method(TRUE);
}
/* }}} */

/* {{{ public boolean Validate::greatThan(mixed $data, integer/float $length) */
static BUESSION_METHOD(validate, greatThan){
	validate_valid_great_method(FALSE);
}
/* }}} */

/* {{{ public boolean Validate::isEqual(mixed $data1, mixed $data2) */
static BUESSION_METHOD(validate, isEqual){
	zval *data1 = NULL;
	zval *data2 = NULL;

	if(zend_parse_parameters(2 TSRMLS_CC, "zz", &data1, &data2) == SUCCESS){
		if(data1 == NULL&&data2 == NULL){
			RETURN_TRUE;
		}else if(compare_function(return_value, data1, data2 TSRMLS_CC) == SUCCESS){
			RETURN_BOOL(Z_LVAL_P(return_value) == 0);
		}
	}

	RETURN_FALSE;
}
/* }}} */

/* {{{ public boolean Validate::isAscii(mixed $data) */
static BUESSION_METHOD(validate, isAscii){
	validate_check_zval_ctype_method(isascii);
}
/* }}} */

/* {{{ public boolean Validate::isChinese(string $str) */
static BUESSION_METHOD(validate, isChinese){
	validate_string_simple_valid_method(validate_isChinese_ex, TRUE);
}
/* }}} */

/* {{{ public boolean Validate::isEnglish(string $str) */
static BUESSION_METHOD(validate, isEnglish){
	validate_string_simple_valid_method(validate_isLetter_ex, TRUE);
}
/* }}} */

/* {{{ public boolean Validate::isLower(mixed $str) */
static BUESSION_METHOD(validate, isLower){
	validate_string_simple_valid_method(validate_isLowerLetter_ex, TRUE);
}
/* }}} */

/* {{{ public boolean Validate::isUpper(mixed $str) */
static BUESSION_METHOD(validate, isUpper){
	validate_string_simple_valid_method(validate_isUpperLetter_ex, TRUE);
}
/* }}} */

/* {{{ public boolean Validate::isNumeric(mixed $data) */
static BUESSION_METHOD(validate, isNumeric){
	validate_check_zval_method(validate_isNumeric);
}
/* }}} */

/* {{{ public boolean Validate::isUnsignedNumeric(mixed $data) */
static BUESSION_METHOD(validate, isUnsignedNumeric){
	validate_check_zval_method(validate_isUnsignedNumeric);
}
/* }}} */

/* {{{ public boolean Validate::isSignedNumeric(mixed $data) */
static BUESSION_METHOD(validate, isSignedNumeric){
	validate_check_zval_method(validate_isSignedNumeric);
}
/* }}} */

/* {{{ public boolean Validate::isInteger(mixed $data) */
static BUESSION_METHOD(validate, isInteger){
	validate_check_zval_method(validate_isInteger);
}
/* }}} */

/* {{{ public boolean Validate::isUnsignedInteger(mixed $data) */
static BUESSION_METHOD(validate, isUnsignedInteger){
	validate_check_zval_method(validate_isUnsignedInteger);
}
/* }}} */

/* {{{ public boolean Validate::isSignedInteger(mixed $data) */
static BUESSION_METHOD(validate, isSignedInteger){
	validate_check_zval_method(validate_isSignedInteger);
}
/* }}} */

/* {{{ public boolean Validate::isFloat(mixed $data) */
static BUESSION_METHOD(validate, isFloat){
	validate_check_zval_method(validate_isFloat);
}
/* }}} */

/* {{{ public boolean Validate::isUnsignedFloat(mixed $data) */
static BUESSION_METHOD(validate, isUnsignedFloat){
	validate_check_zval_method(validate_isUnsignedFloat);
}
/* }}} */

/* {{{ public boolean Validate::isSignedFloat(mixed $data) */
static BUESSION_METHOD(validate, isSignedFloat){
	validate_check_zval_method(validate_isSignedFloat);
}
/* }}} */

/* {{{ public boolean Validate::isAlnum(mixed $data) */
static BUESSION_METHOD(validate, isAlnum){
	validate_check_zval_ctype_method(isalnum);
}
/* }}} */

/* {{{ public boolean Validate::isXdigit(mixed $data) */
static BUESSION_METHOD(validate, isXdigit){
	validate_check_zval_ctype_method(isxdigit);
}
/* }}} */

/* {{{ public boolean Validate::isCntrl(string $str) */
static BUESSION_METHOD(validate, isCntrl){
	validate_string_simple_valid_method(validate_isCntrl_ex, TRUE);
}
/* }}} */

/* {{{ public boolean Validate::isPrint(string $str) */
static BUESSION_METHOD(validate, isPrint){
	validate_string_simple_valid_method(validate_isPrint_ex, TRUE);
}
/* }}} */

/* {{{ public boolean Validate::isCurrency(string $str) */
static BUESSION_METHOD(validate, isCurrency){
	validate_string_simple_valid_method(validate_isCurrency_ex, TRUE);
}
/* }}} */

/* {{{ public boolean Validate::isTel(string $str[, int $flags = TEL_TAIL|TEL_EXTENSION|TEL_SPECIAL]) */
static BUESSION_METHOD(validate, isTel){
	char *str;
	uint str_length;
	int flags = TEL_TAIL|TEL_EXTENSION|TEL_SPECIAL;

	if(zend_parse_parameters_ex(ZEND_PARSE_PARAMS_QUIET, ZEND_NUM_ARGS() TSRMLS_CC, "s|l", &str, &str_length, &flags) == SUCCESS){
		RETURN_BOOL(validate_isTel_ex(str, str_length, flags TSRMLS_CC));
	}

	RETURN_FALSE;
}
/* }}} */

/* {{{ public boolean Validate::isMobile(string $str) */
static BUESSION_METHOD(validate, isMobile){
	validate_string_simple_valid_method(validate_isMobile_ex, TRUE);
}
/* }}} */

/* {{{ public boolean Validate::isPostCode(string $str) */
static BUESSION_METHOD(validate, isPostCode){
	validate_string_simple_valid_method(validate_isPostCode_ex, TRUE);
}
/* }}} */

/* {{{ public boolean Validate::isIP(string $str[, int $type = IPV4|IPV6, [int $flags = IP_PRIV_RANGE|IP_RES_RANGE]]) */
static BUESSION_METHOD(validate, isIP){
	char *str;
	uint str_length;
	int type = IPV4|IPV6;
	int flags = IP_PRIV_RANGE|IP_RES_RANGE;

	if(zend_parse_parameters_ex(ZEND_PARSE_PARAMS_QUIET, ZEND_NUM_ARGS() TSRMLS_CC, "s|ll", &str, &str_length, &type, &flags) == SUCCESS){
		php_printf("%ld=>%ld\r\n", type, flags);
		RETURN_BOOL(validate_isIP_ex(str, str_length, type, flags TSRMLS_CC));
	}

	RETURN_FALSE;
}
/* }}} */

/* {{{ public boolean Validate::isPort(integer $port) */
static BUESSION_METHOD(validate, isPort){
	int port;

	if(zend_parse_parameters_ex(ZEND_PARSE_PARAMS_QUIET, 1 TSRMLS_CC, "l", &port) == SUCCESS){
		RETURN_BOOL(validate_isPort(port TSRMLS_CC));
	}

	RETURN_FALSE;
}
/* }}} */

/* {{{ public boolean Validate::isDomain(string $str) */
static BUESSION_METHOD(validate, isDomain){
	validate_string_simple_valid_method(validate_isDomain_ex, TRUE);
}
/* }}} */

/* {{{ public boolean Validate::isUrl(string $str) */
static BUESSION_METHOD(validate, isUrl){
	validate_string_simple_valid_method(validate_isUrl_ex, TRUE);
}
/* }}} */

/* {{{ public boolean Validate::isMail(string $str) */
static BUESSION_METHOD(validate, isMail){
	validate_string_simple_valid_method(validate_isMail_ex, TRUE);
}
/* }}} */

/* {{{ public boolean Validate::isQQ(string $str) */
static BUESSION_METHOD(validate, isQQ){
	validate_string_simple_valid_method(validate_isQQ_ex, TRUE);
}
/* }}} */

/* {{{ public boolean Validate::isMsn(string $str) */
static BUESSION_METHOD(validate, isMsn){
	validate_string_simple_valid_method(validate_isMsn_ex, TRUE);
}
/* }}} */

/* {{{ public boolean Validate::isAlipay(string $str) */
static BUESSION_METHOD(validate, isAlipay){
	validate_string_simple_valid_method(validate_isAlipay_ex, TRUE);
}
/* }}} */

/* {{{ public boolean Validate::isIDCard(string $str[, boolean $strict = FALSE, int/string/object $birthday = NULL]) */
static BUESSION_METHOD(validate, isIDCard){
	char *str;
	uint str_length;

	if(ZEND_NUM_ARGS() == 1){
		if(zend_parse_parameters_ex(ZEND_PARSE_PARAMS_QUIET, 1 TSRMLS_CC, "s", &str, &str_length) == SUCCESS){
			RETURN_BOOL(validate_isIDCard_ex(str, str_length, FALSE, NULL TSRMLS_CC));
		}
	}else{
		zend_bool strict;
		zval *birthday;

		if(zend_parse_parameters_ex(ZEND_PARSE_PARAMS_QUIET, 3 TSRMLS_CC, "sbz", &str, &str_length, &strict, &birthday) == SUCCESS){
			RETURN_BOOL(validate_isIDCard_ex(str, str_length, strict, birthday TSRMLS_CC));
		}
	}


	RETURN_FALSE;
}

/* {{{ public boolean Validate::isCreditCard(string $str[, ingeger $type = 0]) */
static BUESSION_METHOD(validate, isCreditCard){
	char *str;
	uint str_length;
	int type = 0;

	if(zend_parse_parameters_ex(ZEND_PARSE_PARAMS_QUIET, ZEND_NUM_ARGS() TSRMLS_CC, "s|l", &str, &str_length, &type) == SUCCESS){
		RETURN_BOOL(validate_isCreditCard_ex(str, str_length, type TSRMLS_CC));
	}

	RETURN_FALSE;
}
/* }}} */

/* {{{ public boolean Validate::isIsbn(string $str[, integer $type = ISBN_10|ISBN_13[, string $separator = "-"]]) */
static BUESSION_METHOD(validate, isIsbn){
	char *str;
	uint str_length;
	int type = ISBN_10|ISBN_13;
	char *separator = "-";
	uint separator_length = 1;

	if(zend_parse_parameters_ex(ZEND_PARSE_PARAMS_QUIET, ZEND_NUM_ARGS() TSRMLS_CC, "s|ls", &str, &str_length, &type, &separator, &separator_length) == SUCCESS){
		RETURN_BOOL(validate_isIsbn_ex(str, str_length, type, separator, separator_length TSRMLS_CC));
	}

	RETURN_FALSE;
}
/* }}} */

/* {{{ public boolean Validate::Regex(string $str, string $pattern) */
static BUESSION_METHOD(validate, Regex){
	char *str;
	uint str_length;
	char *pattern;
	uint pattern_length;

	if(zend_parse_parameters_ex(ZEND_PARSE_PARAMS_QUIET, 2 TSRMLS_CC, "ss", &str, &str_length, &pattern, &pattern_length) == SUCCESS){
		pcre *re = NULL;
		pcre_extra *extra = NULL;
		int options = 0;
		int ovector[3];

		re = pcre_get_compiled_regex(pattern, &extra, &options TSRMLS_CC);
		RETURN_BOOL(re == NULL||pcre_exec(re, NULL, str, str_length, 0, 0, ovector, 3) < 0 ? FALSE : TRUE);
	}

	RETURN_FALSE;
}
/* }}} */

/* {{{ private void Validate::__destruct() */
static BUESSION_METHOD(validate, __destruct){
}
/* }}} */

static zend_function_entry validate_methods[] = {
	ZEND_ME(buession_validate, __construct, NULL, ZEND_ACC_PRIVATE|ZEND_ACC_CTOR)
	BUESSION_VALIDATE_ME(isNull, validate_isNull_arguments)
	BUESSION_VALIDATE_ME(isBlank, validate_isBlank_arguments)
	BUESSION_VALIDATE_ME(length, validate_length_arguments)
	BUESSION_VALIDATE_ME(isBetween, validate_isBetween_arguments)
	BUESSION_VALIDATE_ME(lessEqual, validate_lessEqual_arguments)
	BUESSION_VALIDATE_ME(lessThan, validate_lessThan_arguments)
	BUESSION_VALIDATE_ME(greatEqual, validate_greatEqual_arguments)
	BUESSION_VALIDATE_ME(greatThan, validate_greatThan_arguments)
	BUESSION_VALIDATE_ME(isEqual, validate_isEqual_arguments)
	BUESSION_VALIDATE_ME(isAscii, validate_isAscii_arguments)
	BUESSION_VALIDATE_ME(isChinese, validate_isChinese_arguments)
	BUESSION_VALIDATE_ME(isEnglish, validate_isEnglish_arguments)
	BUESSION_VALIDATE_MALIAS(isAlpha, isEnglish, validate_isAlpha_arguments)
	BUESSION_VALIDATE_MALIAS(isLetter, isEnglish, validate_isLetter_arguments)
	BUESSION_VALIDATE_ME(isLower, validate_isLower_arguments)
	BUESSION_VALIDATE_ME(isUpper, validate_isUpper_arguments)
	BUESSION_VALIDATE_ME(isNumeric, validate_isNumeric_arguments)
	BUESSION_VALIDATE_MALIAS(isDigital, isNumeric, validate_isDigital_arguments)
	BUESSION_VALIDATE_ME(isUnsignedNumeric, validate_isUnsignedNumeric_arguments)
	BUESSION_VALIDATE_MALIAS(isUnsignedDigital, isUnsignedNumeric, validate_isUnsignedNumeric_arguments)
	BUESSION_VALIDATE_ME(isSignedNumeric, validate_isSignedNumeric_arguments)
	BUESSION_VALIDATE_MALIAS(isSignedDigital, isSignedNumeric, validate_isSignedNumeric_arguments)
	BUESSION_VALIDATE_ME(isInteger, validate_isInteger_arguments)
	BUESSION_VALIDATE_MALIAS(isInt, isInteger, validate_isInteger_arguments)
	BUESSION_VALIDATE_ME(isUnsignedInteger, validate_isUnsignedInteger_arguments)
	BUESSION_VALIDATE_MALIAS(isUnsignedInt, isUnsignedInteger, validate_isUnsignedInteger_arguments)
	BUESSION_VALIDATE_ME(isSignedInteger, validate_isSignedInteger_arguments)
	BUESSION_VALIDATE_MALIAS(isSignedInt, isSignedInteger, validate_isSignedInteger_arguments)
	BUESSION_VALIDATE_ME(isFloat, validate_isFloat_arguments)
	BUESSION_VALIDATE_MALIAS(isDouble, isFloat, validate_isFloat_arguments)
	BUESSION_VALIDATE_ME(isUnsignedFloat, validate_isUnsignedFloat_arguments)
	BUESSION_VALIDATE_MALIAS(isUnsignedDouble, isUnsignedFloat, validate_isUnsignedFloat_arguments)
	BUESSION_VALIDATE_ME(isSignedFloat, validate_isSignedFloat_arguments)
	BUESSION_VALIDATE_MALIAS(isSignedDouble, isSignedFloat, validate_isSignedFloat_arguments)
	BUESSION_VALIDATE_ME(isAlnum, validate_isAlnum_arguments)
	BUESSION_VALIDATE_ME(isXdigit, validate_isXdigit_arguments)
	BUESSION_VALIDATE_MALIAS(isHex, isXdigit, validate_isXdigit_arguments)
	BUESSION_VALIDATE_ME(isCntrl, validate_isCntrl_arguments)
	BUESSION_VALIDATE_ME(isPrint, validate_isPrint_arguments)
	BUESSION_VALIDATE_MALIAS(isPrintable, isPrint, validate_isPrint_arguments)
	//BUESSION_VALIDATE_ME(isCurrency, validate_isCurrency_arguments)
	//BUESSION_VALIDATE_ME(isTel, validate_isTel_arguments)
	//BUESSION_VALIDATE_MALIAS(isFax, isTel, validate_isTel_arguments)
	BUESSION_VALIDATE_ME(isMobile, validate_isMobile_arguments)
	BUESSION_VALIDATE_ME(isPostCode, validate_isPostCode_arguments)
	BUESSION_VALIDATE_ME(isIP, validate_isIP_arguments)
	BUESSION_VALIDATE_ME(isPort, validate_isPort_arguments)
	BUESSION_VALIDATE_ME(isDomain, validate_isDomain_arguments)
	BUESSION_VALIDATE_ME(isUrl, validate_isUrl_arguments)
	BUESSION_VALIDATE_ME(isMail, validate_isMail_arguments)
	BUESSION_VALIDATE_MALIAS(isEmail, isMail, validate_isMail_arguments)
	BUESSION_VALIDATE_ME(isQQ, validate_isQQ_arguments)
	BUESSION_VALIDATE_ME(isMsn, validate_isMsn_arguments)
	BUESSION_VALIDATE_ME(isAlipay, validate_isAlipay_arguments)
	BUESSION_VALIDATE_ME(isIDCard, validate_isIDCard_arguments)
	BUESSION_VALIDATE_ME(isCreditCard, validate_isCreditCard_arguments)
	BUESSION_VALIDATE_ME(isIsbn, validate_isIsbn_arguments)
	BUESSION_VALIDATE_ME(Regex, validate_Regex_arguments)
	ZEND_ME(buession_validate, __destruct, NULL, ZEND_ACC_PRIVATE|ZEND_ACC_DTOR)
	ZEND_FE_END
};

/** {{{ BUESSION_STARTUP_FUNCTION */
BUESSION_STARTUP_FUNCTION(validate){
	zend_class_entry ce;

	BUESSION_INIT_CLASS_ENTRY(ce, "Validate", validate_methods);
	validate_ce = zend_register_internal_class(&ce TSRMLS_CC);

	return SUCCESS;
}
/* }}} */

/** {{{ BUESSION_RINIT_FUNCTION */
BUESSION_RINIT_FUNCTION(validate){
	return SUCCESS;
}
/* }}} */

/** {{{ BUESSION_RSHUTDOWN_FUNCTION */
BUESSION_RSHUTDOWN_FUNCTION(validate){
	return SUCCESS;
}
/* }}} */

/** {{{ BUESSION_SHUTDOWN_FUNCTION */
BUESSION_SHUTDOWN_FUNCTION(validate){
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