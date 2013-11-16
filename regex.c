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

/* $Id: regex.c 310447 2012-03-18 22:58:13Z Yong.Teng $ */

/* include php file */
#include "Zend/zend.h"
#include "Zend/zend_API.h"
#include "main/php.h"
#include "main/php_main.h"
#include "ext/pcre/php_pcre.h"

/* include buession file */
#include "buession.h"
#include "regex.h"

BUESSION_API zend_bool buession_regex_match(char *str, char *pattern, zend_bool isglobal, long flags, zval **subparts TSRMLS_DC){
	return buession_regex_match_ex(str, strlen(str), pattern, strlen(pattern), isglobal, flags, subparts TSRMLS_CC);
}
BUESSION_API zend_bool buession_regex_match_ex(char *str, uint str_length, char *pattern, uint pattern_length, zend_bool isglobal, long flags, zval **subparts TSRMLS_DC){
	if(str == NULL||str_length == 0){
		return FALSE;
	}else{
		pcre_cache_entry *entry = pcre_get_compiled_regex_cache(pattern, pattern_length TSRMLS_CC);
		zval result;

		if(entry == NULL){
			return FALSE;
		}

		if(subparts != NULL){
			MAKE_STD_ZVAL(*subparts);
		}
		php_pcre_match_impl(entry, str, str_length, &result, subparts == NULL ? NULL : *subparts, isglobal, flags > 0, flags, 0 TSRMLS_CC);

		return Z_LVAL(result) > 0;
	}
}

BUESSION_API int buession_regex_split(char *str, char *pattern, int limit, long flags, zval **result TSRMLS_DC){
	return buession_regex_split_ex(str, strlen(str), pattern, strlen(pattern), limit, flags, result TSRMLS_CC);
}
BUESSION_API int buession_regex_split_ex(char *str, uint str_length, char *pattern, uint pattern_length, int limit, long flags, zval **result TSRMLS_DC){
	if(str == NULL||str_length == 0){
		return FAILURE;
	}else{
		pcre_cache_entry *entry = pcre_get_compiled_regex_cache(pattern, pattern_length TSRMLS_CC);

		if(entry == NULL){
			return FAILURE;
		}

		if(result != NULL){
			MAKE_STD_ZVAL(*result);
		}
		php_pcre_split_impl(entry, str, str_length, *result, limit, flags TSRMLS_CC);

		return SUCCESS;
	}
}

BUESSION_API int buession_regex_replace(char *str, char *pattern, char *replace, int limit, long flags, char **result, uint *result_length TSRMLS_DC){
	return buession_regex_replace_ex(str, strlen(str), pattern, strlen(pattern), replace, strlen(replace), limit, flags, result, result_length TSRMLS_CC);
}
BUESSION_API int buession_regex_replace_ex(char *str, uint str_length, char *pattern, uint pattern_length, char *replace, uint replace_length, int limit, long flags, char **result, uint *result_length TSRMLS_DC){
	if(str == NULL||str_length == 0){
		return FAILURE;
	}else{
		pcre_cache_entry *entry = pcre_get_compiled_regex_cache(pattern, pattern_length TSRMLS_CC);
		zval *zreplace;

		if(entry == NULL){
			return FAILURE;
		}

		MAKE_STD_ZVAL(zreplace);
		ZVAL_STRINGL(zreplace, replace, replace_length, TRUE);
		*result = php_pcre_replace_impl(entry, str, str_length, zreplace, FALSE, result_length, limit, NULL TSRMLS_CC);
		buession_zval_ptr_dtor(zreplace);

		return SUCCESS;
	}
}

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */