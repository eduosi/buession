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

/* $Id: hash.h 310447 2012-12-30 02:48:30Z Yong.Teng $ */

#ifndef BUESSION_HASH_H
#define BUESSION_HASH_H

/* add start */
BUESSION_API int buession_hash_index_add_long(HashTable *ht, ulong index, long l TSRMLS_DC);
BUESSION_API int buession_hash_next_index_add_long(HashTable *ht, long l TSRMLS_DC);
BUESSION_API int buession_hash_add_long(HashTable *ht, const char *key, long l TSRMLS_DC);
BUESSION_API int buession_hash_add_long_ex(HashTable *ht, const char *key, uint key_length, long l TSRMLS_DC);

BUESSION_API int buession_hash_index_add_double(HashTable *ht, ulong index, double d TSRMLS_DC);
BUESSION_API int buession_hash_next_index_add_double(HashTable *ht, double d TSRMLS_DC);
BUESSION_API int buession_hash_add_double(HashTable *ht, const char *key, double d TSRMLS_DC);
BUESSION_API int buession_hash_add_double_ex(HashTable *ht, const char *key, uint key_length, double d TSRMLS_DC);

BUESSION_API int buession_hash_index_add_null(HashTable *ht, ulong index TSRMLS_DC);
BUESSION_API int buession_hash_next_index_add_null(HashTable *ht TSRMLS_DC);
BUESSION_API int buession_hash_add_null(HashTable *ht, const char *key TSRMLS_DC);
BUESSION_API int buession_hash_add_null_ex(HashTable *ht, const char *key, uint key_length TSRMLS_DC);

BUESSION_API int buession_hash_index_add_bool(HashTable *ht, ulong index, zend_bool b TSRMLS_DC);
BUESSION_API int buession_hash_next_index_add_bool(HashTable *ht, zend_bool b TSRMLS_DC);
BUESSION_API int buession_hash_add_bool(HashTable *ht, const char *key, zend_bool b TSRMLS_DC);
BUESSION_API int buession_hash_add_bool_ex(HashTable *ht, const char *key, uint key_length, zend_bool b TSRMLS_DC);

BUESSION_API int buession_hash_index_add_resource(HashTable *ht, ulong index, int r TSRMLS_DC);
BUESSION_API int buession_hash_next_index_add_resource(HashTable *ht, int r TSRMLS_DC);
BUESSION_API int buession_hash_add_resource(HashTable *ht, const char *key, int r TSRMLS_DC);
BUESSION_API int buession_hash_add_resource_ex(HashTable *ht, const char *key, uint key_length, int r TSRMLS_DC);

BUESSION_API int buession_hash_index_add_string(HashTable *ht, ulong index, char *str, zend_bool duplicate TSRMLS_DC);
BUESSION_API int buession_hash_next_index_add_string(HashTable *ht, char *str, zend_bool duplicate TSRMLS_DC);
BUESSION_API int buession_hash_add_string(HashTable *ht, const char *key, char *str, zend_bool duplicate TSRMLS_DC);
BUESSION_API int buession_hash_add_string_ex(HashTable *ht, const char *key, uint key_length, char *str, zend_bool duplicate TSRMLS_DC);

BUESSION_API int buession_hash_index_add_stringl(HashTable *ht, ulong index, char *str, uint str_length, zend_bool duplicate TSRMLS_DC);
BUESSION_API int buession_hash_next_index_add_stringl(HashTable *ht, char *str, uint str_length, zend_bool duplicate TSRMLS_DC);
BUESSION_API int buession_hash_add_stringl(HashTable *ht, const char *key, char *str, uint str_length, zend_bool duplicate TSRMLS_DC);
BUESSION_API int buession_hash_add_stringl_ex(HashTable *ht, const char *key, uint key_length, char *str, uint str_length, zend_bool duplicate TSRMLS_DC);

BUESSION_API int buession_hash_index_add_hash(HashTable *ht, ulong index, HashTable *value TSRMLS_DC);
BUESSION_API int buession_hash_next_index_add_hash(HashTable *ht, HashTable *value TSRMLS_DC);
BUESSION_API int buession_hash_add_hash(HashTable *ht, const char *key, HashTable *value TSRMLS_DC);
BUESSION_API int buession_hash_add_hash_ex(HashTable *ht, const char *key, uint key_length, HashTable *value TSRMLS_DC);

BUESSION_API int buession_hash_index_add_zval(HashTable *ht, ulong index, zval *value TSRMLS_DC);
BUESSION_API int buession_hash_next_index_add_zval(HashTable *ht, zval *value TSRMLS_DC);
BUESSION_API int buession_hash_add_zval(HashTable *ht, const char *key, zval *value TSRMLS_DC);
BUESSION_API int buession_hash_add_zval_ex(HashTable *ht, const char *key, uint key_length, zval *value TSRMLS_DC);
/* add end */

/* update start */
BUESSION_API int buession_hash_index_update_long(HashTable *ht, ulong index, long l TSRMLS_DC);
BUESSION_API int buession_hash_update_long(HashTable *ht, const char *key, long l TSRMLS_DC);
BUESSION_API int buession_hash_update_long_ex(HashTable *ht, const char *key, uint key_length, long l TSRMLS_DC);
BUESSION_API int buession_hash_symtable_update_long(HashTable *ht, const char *key, long l TSRMLS_DC);
BUESSION_API int buession_hash_symtable_update_long_ex(HashTable *ht, const char *key, uint key_length, long l TSRMLS_DC);

BUESSION_API int buession_hash_index_update_double(HashTable *ht, ulong index, double d TSRMLS_DC);
BUESSION_API int buession_hash_update_double(HashTable *ht, const char *key, double d TSRMLS_DC);
BUESSION_API int buession_hash_update_double_ex(HashTable *ht, const char *key, uint key_length, double d TSRMLS_DC);
BUESSION_API int buession_hash_symtable_update_double(HashTable *ht, const char *key, double d TSRMLS_DC);
BUESSION_API int buession_hash_symtable_update_double_ex(HashTable *ht, const char *key, uint key_length, double d TSRMLS_DC);

BUESSION_API int buession_hash_index_update_null(HashTable *ht, ulong index TSRMLS_DC);
BUESSION_API int buession_hash_update_null(HashTable *ht, const char *key TSRMLS_DC);
BUESSION_API int buession_hash_update_null_ex(HashTable *ht, const char *key, uint key_length TSRMLS_DC);
BUESSION_API int buession_hash_symtable_update_null(HashTable *ht, const char *key TSRMLS_DC);
BUESSION_API int buession_hash_symtable_update_null_ex(HashTable *ht, const char *key, uint key_length TSRMLS_DC);

BUESSION_API int buession_hash_index_update_bool(HashTable *ht, ulong index, zend_bool b TSRMLS_DC);
BUESSION_API int buession_hash_update_bool(HashTable *ht, const char *key, zend_bool b TSRMLS_DC);
BUESSION_API int buession_hash_update_bool_ex(HashTable *ht, const char *key, uint key_length, zend_bool b TSRMLS_DC);
BUESSION_API int buession_hash_symtable_update_bool(HashTable *ht, const char *key, zend_bool b TSRMLS_DC);
BUESSION_API int buession_hash_symtable_update_bool_ex(HashTable *ht, const char *key, uint key_length, zend_bool b TSRMLS_DC);

BUESSION_API int buession_hash_index_update_string(HashTable *ht, ulong index, char *str, zend_bool duplicate TSRMLS_DC);
BUESSION_API int buession_hash_update_string(HashTable *ht, const char *key, char *str, zend_bool duplicate TSRMLS_DC);
BUESSION_API int buession_hash_update_string_ex(HashTable *ht, const char *key, uint key_length, char *str, zend_bool duplicate TSRMLS_DC);
BUESSION_API int buession_hash_symtable_update_string(HashTable *ht, const char *key, char *str, zend_bool duplicate TSRMLS_DC);
BUESSION_API int buession_hash_symtable_update_string_ex(HashTable *ht, const char *key, uint key_length, char *str, zend_bool duplicate TSRMLS_DC);

BUESSION_API int buession_hash_index_update_stringl(HashTable *ht, ulong index, char *str, uint str_length, zend_bool duplicate TSRMLS_DC);
BUESSION_API int buession_hash_update_stringl(HashTable *ht, const char *key, char *str, uint str_length, zend_bool duplicate TSRMLS_DC);
BUESSION_API int buession_hash_update_stringl_ex(HashTable *ht, const char *key, uint key_length, char *str, uint str_length, zend_bool duplicate TSRMLS_DC);
BUESSION_API int buession_hash_symtable_update_stringl(HashTable *ht, const char *key, char *str, uint str_length, zend_bool duplicate TSRMLS_DC);
BUESSION_API int buession_hash_symtable_update_stringl_ex(HashTable *ht, const char *key, uint key_length, char *str, uint str_length, zend_bool duplicate TSRMLS_DC);

BUESSION_API int buession_hash_index_update_zval(HashTable *ht, ulong index, zval *value TSRMLS_DC);
BUESSION_API int buession_hash_update_zval(HashTable *ht, const char *key, zval *value TSRMLS_DC);
BUESSION_API int buession_hash_update_zval_ex(HashTable *ht, const char *key, uint key_length, zval *value TSRMLS_DC);
BUESSION_API int buession_hash_symtable_update_zval(HashTable *ht, const char *key, zval *value TSRMLS_DC);
BUESSION_API int buession_hash_symtable_update_zval_ex(HashTable *ht, const char *key, uint key_length, zval *value TSRMLS_DC);
/* update end */

#define BUESSION_HASH_LONG_UPDATE(fn, ht, key, key_length, l) \
	do{	\
		zval *value;	\
		MAKE_STD_ZVAL(value);	\
		ZVAL_LONG(value, l);	\
		return fn((ht), (key), (key_length), (void *) &value, sizeof(zval *), NULL);	\
	}while(0);

#define BUESSION_HASH_DOUBLE_UPDATE(fn, ht, key, key_length, d) \
	do{	\
		zval *value;	\
		MAKE_STD_ZVAL(value);	\
		ZVAL_DOUBLE(value, d);	\
		return fn((ht), (key), (key_length), (void *) &value, sizeof(zval *), NULL);	\
	}while(0);

#define BUESSION_HASH_BOOL_UPDATE(fn, ht, key, key_length, b) \
	do{	\
		zval *value;	\
		MAKE_STD_ZVAL(value);	\
		ZVAL_BOOL(value, b);	\
		return fn((ht), (key), (key_length), (void *) &value, sizeof(zval *), NULL);	\
	}while(0);

#define BUESSION_HASH_NULL_UPDATE(fn, ht, key, key_length) \
	do{	\
		zval *value;	\
		MAKE_STD_ZVAL(value);	\
		ZVAL_NULL(value);	\
		return fn((ht), (key), (key_length), (void *) &value, sizeof(zval *), NULL);	\
	}while(0);

#define BUESSION_HASH_STRING_UPDATE(fn, ht, key, key_length, str, str_length, duplicate) \
	do{	\
		zval *value;	\
		MAKE_STD_ZVAL(value);	\
		ZVAL_STRINGL(value, str, str_length, duplicate);	\
		return fn((ht), (key), (key_length), (void *) &value, sizeof(zval *), NULL);	\
	}while(0);

BUESSION_API void buession_hash_persistent_copy(HashTable *target, HashTable *source TSRMLS_DC);

#endif	/* BUESSION_HASH_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */