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
  | Copyright: Copyright © 2011-2012 Buession.com.cn Inc.                |
  +----------------------------------------------------------------------+
*/

/* $Id: regex.h 310447 2012-03-18 22:58:01Z Yong.Teng $ */

#ifndef BUESSION_REGEX_H
#define BUESSION_REGEX_H

BUESSION_API zend_bool buession_regex_match(char *str, char *pattern, zend_bool isglobal, long flags, zval **subparts TSRMLS_DC);
BUESSION_API zend_bool buession_regex_match_ex(char *str, uint str_length, char *pattern, uint pattern_length, zend_bool isglobal, long flags, zval **subparts TSRMLS_DC);

BUESSION_API zend_bool buession_regex_test(char *str, char *pattern, zend_bool isglobal, long flags, zval **subparts TSRMLS_DC);
BUESSION_API zend_bool buession_regex_test_ex(char *str, uint str_length, char *pattern, uint pattern_length, zend_bool isglobal, long flags, zval **subparts TSRMLS_DC);

BUESSION_API int buession_regex_split(char *str, char *pattern, int limit, long flags, zval **result TSRMLS_DC);
BUESSION_API int buession_regex_split_ex(char *str, uint str_length, char *pattern, uint pattern_length, int limit, long flags, zval **result TSRMLS_DC);

BUESSION_API int buession_regex_replace(char *str, char *pattern, char *replace, int limit, long flags, char **result, uint *result_length TSRMLS_DC);
BUESSION_API int buession_regex_replace_ex(char *str, uint str_length, char *pattern, uint pattern_length, char *replace, uint replace_length, int limit, long flags, char **result, uint *result_length TSRMLS_DC);

#endif	/* BUESSION_REGEX_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */