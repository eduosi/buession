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

/* $Id: hash.c 310447 2012-12-30 02:48:44Z Yong.Teng $ */

/* include php file */
#include "Zend/zend.h"
#include "Zend/zend_API.h"
#include "main/php.h"
#include "main/php_main.h"

/* include buession file */
#include "buession.h"
#include "hash.h"

/* add start */
BUESSION_API int buession_hash_index_add_long(HashTable *ht, ulong index, long l TSRMLS_DC){
	zval *value;

	MAKE_STD_ZVAL(value);
	ZVAL_LONG(value, l);

	return zend_hash_index_update(ht, index, (void *) &value, sizeof(zval *), NULL);
}
BUESSION_API int buession_hash_next_index_add_long(HashTable *ht, long l TSRMLS_DC){
	zval *value;

	MAKE_STD_ZVAL(value);
	ZVAL_LONG(value, l);

	return zend_hash_next_index_insert(ht, (void *) &value, sizeof(zval *), NULL);
}
BUESSION_API int buession_hash_add_long(HashTable *ht, const char *key, long l TSRMLS_DC){
	return buession_hash_add_long_ex(ht, key, strlen(key) + 1, l TSRMLS_CC);
}
BUESSION_API int buession_hash_add_long_ex(HashTable *ht, const char *key, uint key_length, long l TSRMLS_DC){
	zval *value;

	MAKE_STD_ZVAL(value);
	ZVAL_LONG(value, l);

	return zend_hash_add(ht, key, key_length, (void *) &value, sizeof(zval *), NULL);
}

BUESSION_API int buession_hash_index_add_double(HashTable *ht, ulong index, double d TSRMLS_DC){
	zval *value;

	MAKE_STD_ZVAL(value);
	ZVAL_DOUBLE(value, d);

	return zend_hash_index_update(ht, index, (void *) &value, sizeof(zval *), NULL);
}
BUESSION_API int buession_hash_next_index_add_double(HashTable *ht, double d TSRMLS_DC){
	zval *value;

	MAKE_STD_ZVAL(value);
	ZVAL_DOUBLE(value, d);

	return zend_hash_next_index_insert(ht, (void *) &value, sizeof(zval *), NULL);
}
BUESSION_API int buession_hash_add_double(HashTable *ht, const char *key, double d TSRMLS_DC){
	return buession_hash_add_double_ex(ht, key, strlen(key) + 1, d TSRMLS_CC);
}
BUESSION_API int buession_hash_add_double_ex(HashTable *ht, const char *key, uint key_length, double d TSRMLS_DC){
	zval *value;

	MAKE_STD_ZVAL(value);
	ZVAL_DOUBLE(value, d);

	return zend_hash_add(ht, key, key_length, (void *) &value, sizeof(zval *), NULL);
}

BUESSION_API int buession_hash_index_add_null(HashTable *ht, ulong index TSRMLS_DC){
	zval *value;

	MAKE_STD_ZVAL(value);
	ZVAL_NULL(value);

	return zend_hash_index_update(ht, index, (void *) &value, sizeof(zval *), NULL);
}
BUESSION_API int buession_hash_next_index_add_null(HashTable *ht TSRMLS_DC){
	zval *value;

	MAKE_STD_ZVAL(value);
	ZVAL_NULL(value);

	return zend_hash_next_index_insert(ht, (void *) &value, sizeof(zval *), NULL);
}
BUESSION_API int buession_hash_add_null(HashTable *ht, const char *key TSRMLS_DC){
	return buession_hash_add_null_ex(ht, key, strlen(key) + 1 TSRMLS_CC);
}
BUESSION_API int buession_hash_add_null_ex(HashTable *ht, const char *key, uint key_length TSRMLS_DC){
	zval *value;

	MAKE_STD_ZVAL(value);
	ZVAL_NULL(value);

	return zend_hash_add(ht, key, key_length, (void *) &value, sizeof(zval *), NULL);
}

BUESSION_API int buession_hash_index_add_bool(HashTable *ht, ulong index, zend_bool b TSRMLS_DC){
	zval *value;

	MAKE_STD_ZVAL(value);
	ZVAL_BOOL(value, b);

	return zend_hash_index_update(ht, index, (void *) &value, sizeof(zval *), NULL);
}
BUESSION_API int buession_hash_next_index_add_bool(HashTable *ht, zend_bool b TSRMLS_DC){
	zval *value;

	MAKE_STD_ZVAL(value);
	ZVAL_BOOL(value, b);

	return zend_hash_next_index_insert(ht, (void *) &value, sizeof(zval *), NULL);
}
BUESSION_API int buession_hash_add_bool(HashTable *ht, const char *key, zend_bool b TSRMLS_DC){
	return buession_hash_add_bool_ex(ht, key, strlen(key) + 1, b TSRMLS_CC);
}
BUESSION_API int buession_hash_add_bool_ex(HashTable *ht, const char *key, uint key_length, zend_bool b TSRMLS_DC){
	zval *value;

	MAKE_STD_ZVAL(value);
	ZVAL_BOOL(value, b);

	return zend_hash_add(ht, key, key_length, (void *) &value, sizeof(zval *), NULL);
}

BUESSION_API int buession_hash_index_add_resource(HashTable *ht, ulong index, int r TSRMLS_DC){
	zval *value;

	MAKE_STD_ZVAL(value);
	ZVAL_RESOURCE(value, r);

	return zend_hash_index_update(ht, index, (void *) &value, sizeof(zval *), NULL);
}
BUESSION_API int buession_hash_next_index_add_resource(HashTable *ht, int r TSRMLS_DC){
	zval *value;

	MAKE_STD_ZVAL(value);
	ZVAL_RESOURCE(value, r);

	return zend_hash_next_index_insert(ht, (void *) &value, sizeof(zval *), NULL);
}
BUESSION_API int buession_hash_add_resource(HashTable *ht, const char *key, int r TSRMLS_DC){
	return buession_hash_add_resource_ex(ht, key, strlen(key) + 1, r TSRMLS_CC);
}
BUESSION_API int buession_hash_add_resource_ex(HashTable *ht, const char *key, uint key_length, int r TSRMLS_DC){
	zval *value;

	MAKE_STD_ZVAL(value);
	ZVAL_RESOURCE(value, r);

	return zend_hash_add(ht, key, key_length, (void *) &value, sizeof(zval *), NULL);
}

BUESSION_API int buession_hash_index_add_string(HashTable *ht, ulong index, char *str, zend_bool duplicate TSRMLS_DC){
	return buession_hash_index_add_stringl(ht, index, str, strlen(str), duplicate TSRMLS_CC);
}
BUESSION_API int buession_hash_next_index_add_string(HashTable *ht, char *str, zend_bool duplicate TSRMLS_DC){
	return buession_hash_next_index_add_stringl(ht, str, strlen(str), duplicate TSRMLS_CC);
}
BUESSION_API int buession_hash_add_string(HashTable *ht, const char *key, char *str, zend_bool duplicate TSRMLS_DC){
	return buession_hash_add_stringl_ex(ht, key, strlen(key) + 1, str, strlen(str), duplicate TSRMLS_CC);
}
BUESSION_API int buession_hash_add_string_ex(HashTable *ht, const char *key, uint key_length, char *str, zend_bool duplicate TSRMLS_DC){
	return buession_hash_add_stringl_ex(ht, key, key_length, str, strlen(str), duplicate TSRMLS_CC);
}

BUESSION_API int buession_hash_index_add_stringl(HashTable *ht, ulong index, char *str, uint str_length, zend_bool duplicate TSRMLS_DC){
	zval *value;

	MAKE_STD_ZVAL(value);
	ZVAL_STRINGL(value, str, str_length, duplicate);

	return zend_hash_index_update(ht, index, (void *) &value, sizeof(value), NULL);
}
BUESSION_API int buession_hash_next_index_add_stringl(HashTable *ht, char *str, uint str_length, zend_bool duplicate TSRMLS_DC){
	zval *value;

	MAKE_STD_ZVAL(value);
	ZVAL_STRINGL(value, str, str_length, duplicate);

	return zend_hash_next_index_insert(ht, (void *) &value, sizeof(value), NULL);
}
BUESSION_API int buession_hash_add_stringl(HashTable *ht, const char *key, char *str, uint str_length, zend_bool duplicate TSRMLS_DC){
	return buession_hash_add_stringl_ex(ht, key, strlen(key) + 1, str, str_length, duplicate TSRMLS_CC);
}
BUESSION_API int buession_hash_add_stringl_ex(HashTable *ht, const char *key, uint key_length, char *str, uint str_length, zend_bool duplicate TSRMLS_DC){
	zval *value;

	MAKE_STD_ZVAL(value);
	ZVAL_STRINGL(value, str, str_length, duplicate);

	return zend_hash_add(ht, key, key_length, (void *) &value, sizeof(zval *), NULL);
}

BUESSION_API int buession_hash_index_add_hash(HashTable *ht, ulong index, HashTable *value TSRMLS_DC){
	return zend_hash_index_update(ht, index, (void *) &value, sizeof(HashTable *), NULL);
}
BUESSION_API int buession_hash_next_index_add_hash(HashTable *ht, HashTable *value TSRMLS_DC){
	return zend_hash_next_index_insert(ht, (void *) &value, sizeof(HashTable *), NULL);
}
BUESSION_API int buession_hash_add_hash(HashTable *ht, const char *key, HashTable *value TSRMLS_DC){
	return buession_hash_add_hash_ex(ht, key, strlen(key) + 1, value TSRMLS_CC);
}
BUESSION_API int buession_hash_add_hash_ex(HashTable *ht, const char *key, uint key_length, HashTable *value TSRMLS_DC){
	return zend_hash_add(ht, key, key_length, (void *) &value, sizeof(HashTable *), NULL);
}

BUESSION_API int buession_hash_index_add_zval(HashTable *ht, ulong index, zval *value TSRMLS_DC){
	return zend_hash_index_update(ht, index, (void *) &value, sizeof(zval *), NULL);
}
BUESSION_API int buession_hash_next_index_add_zval(HashTable *ht, zval *value TSRMLS_DC){
	return zend_hash_next_index_insert(ht, (void *) &value, sizeof(zval *), NULL);
}
BUESSION_API int buession_hash_add_zval(HashTable *ht, const char *key, zval *value TSRMLS_DC){
	return buession_hash_add_zval_ex(ht, key, strlen(key) + 1, value TSRMLS_CC);
}
BUESSION_API int buession_hash_add_zval_ex(HashTable *ht, const char *key, uint key_length, zval *value TSRMLS_DC){
	return zend_hash_add(ht, key, key_length, (void *) &value, sizeof(zval *), NULL);
}
/* add end */

/* update start */
BUESSION_API int buession_hash_index_update_long(HashTable *ht, ulong index, long l TSRMLS_DC){
	zval *value;

	MAKE_STD_ZVAL(value);
	ZVAL_LONG(value, l);

	return zend_hash_index_update(ht, index, (void *) &value, sizeof(zval *), NULL);
}
BUESSION_API int buession_hash_update_long(HashTable *ht, const char *key, long l TSRMLS_DC){
	BUESSION_HASH_LONG_UPDATE(zend_hash_update, ht, key, strlen(key), l);
	return FAILURE;
}
BUESSION_API int buession_hash_update_long_ex(HashTable *ht, const char *key, uint key_length, long l TSRMLS_DC){
	BUESSION_HASH_LONG_UPDATE(zend_hash_update, ht, key, key_length, l);
	return FAILURE;
}
BUESSION_API int buession_hash_symtable_update_long(HashTable *ht, const char *key, long l TSRMLS_DC){
	BUESSION_HASH_LONG_UPDATE(zend_symtable_update, ht, key, strlen(key), l);
	return FAILURE;
}
BUESSION_API int buession_hash_symtable_update_long_ex(HashTable *ht, const char *key, uint key_length, long l TSRMLS_DC){
	BUESSION_HASH_LONG_UPDATE(zend_symtable_update, ht, key, key_length, l);
	return FAILURE;
}

BUESSION_API int buession_hash_index_update_double(HashTable *ht, ulong index, double d TSRMLS_DC){
	zval *value;

	MAKE_STD_ZVAL(value);
	ZVAL_DOUBLE(value, d);

	return zend_hash_index_update(ht, index, (void *) &value, sizeof(zval *), NULL);
}
BUESSION_API int buession_hash_update_double(HashTable *ht, const char *key, double d TSRMLS_DC){
	BUESSION_HASH_DOUBLE_UPDATE(zend_hash_update, ht, key, strlen(key), d);
	return FAILURE;
}
BUESSION_API int buession_hash_update_double_ex(HashTable *ht, const char *key, uint key_length, double d TSRMLS_DC){
	BUESSION_HASH_DOUBLE_UPDATE(zend_hash_update, ht, key, key_length, d);
	return FAILURE;
}
BUESSION_API int buession_hash_symtable_update_double(HashTable *ht, const char *key, double d TSRMLS_DC){
	BUESSION_HASH_DOUBLE_UPDATE(zend_symtable_update, ht, key, strlen(key), d);
	return FAILURE;
}
BUESSION_API int buession_hash_symtable_update_double_ex(HashTable *ht, const char *key, uint key_length, double d TSRMLS_DC){
	BUESSION_HASH_DOUBLE_UPDATE(zend_symtable_update, ht, key, key_length, d);
	return FAILURE;
}

BUESSION_API int buession_hash_index_update_null(HashTable *ht, ulong index TSRMLS_DC){
	zval *temp;

	MAKE_STD_ZVAL(temp);
	ZVAL_NULL(temp);

	return zend_hash_index_update(ht, index, (void *) &temp, sizeof(zval *), NULL);
}
BUESSION_API int buession_hash_update_null(HashTable *ht, const char *key TSRMLS_DC){
	BUESSION_HASH_NULL_UPDATE(zend_hash_update, ht, key, strlen(key));
	return FAILURE;
}
BUESSION_API int buession_hash_update_null_ex(HashTable *ht, const char *key, uint key_length TSRMLS_DC){
	BUESSION_HASH_NULL_UPDATE(zend_hash_update, ht, key, key_length);
	return FAILURE;
}
BUESSION_API int buession_hash_symtable_update_null(HashTable *ht, const char *key TSRMLS_DC){
	BUESSION_HASH_NULL_UPDATE(zend_symtable_update, ht, key, strlen(key));
	return FAILURE;
}
BUESSION_API int buession_hash_symtable_update_null_ex(HashTable *ht, const char *key, uint key_length TSRMLS_DC){
	BUESSION_HASH_NULL_UPDATE(zend_symtable_update, ht, key, key_length);
	return FAILURE;
}

BUESSION_API int buession_hash_index_update_bool(HashTable *ht, ulong index, zend_bool b TSRMLS_DC){
	zval *value;

	MAKE_STD_ZVAL(value);
	ZVAL_BOOL(value, b);

	return zend_hash_index_update(ht, index, (void *) &value, sizeof(zval *), NULL);
}
BUESSION_API int buession_hash_update_bool(HashTable *ht, const char *key, zend_bool b TSRMLS_DC){
	BUESSION_HASH_BOOL_UPDATE(zend_hash_update, ht, key, strlen(key), b);
	return FAILURE;
}
BUESSION_API int buession_hash_update_bool_ex(HashTable *ht, const char *key, uint key_length, zend_bool b TSRMLS_DC){
	BUESSION_HASH_BOOL_UPDATE(zend_hash_update, ht, key, key_length, b);
	return FAILURE;
}
BUESSION_API int buession_hash_symtable_update_bool(HashTable *ht, const char *key, zend_bool b TSRMLS_DC){
	BUESSION_HASH_BOOL_UPDATE(zend_symtable_update, ht, key, strlen(key), b);
	return FAILURE;
}
BUESSION_API int buession_hash_symtable_update_bool_ex(HashTable *ht, const char *key, uint key_length, zend_bool b TSRMLS_DC){
	BUESSION_HASH_BOOL_UPDATE(zend_symtable_update, ht, key, key_length, b);
	return FAILURE;
}

BUESSION_API int buession_hash_index_update_string(HashTable *ht, ulong index, char *str, zend_bool duplicate TSRMLS_DC){
	return buession_hash_index_update_stringl(ht, index, str, strlen(str), duplicate TSRMLS_CC);
}
BUESSION_API int buession_hash_update_string(HashTable *ht, const char *key, char *str, zend_bool duplicate TSRMLS_DC){
	BUESSION_HASH_STRING_UPDATE(zend_hash_update, ht, key, strlen(key), str, strlen(str), duplicate);
	return FAILURE;
}
BUESSION_API int buession_hash_update_string_ex(HashTable *ht, const char *key, uint key_length, char *str, zend_bool duplicate TSRMLS_DC){
	BUESSION_HASH_STRING_UPDATE(zend_hash_update, ht, key, key_length, str, strlen(str), duplicate);
	return FAILURE;
}
BUESSION_API int buession_hash_symtable_update_string(HashTable *ht, const char *key, char *str, zend_bool duplicate TSRMLS_DC){
	BUESSION_HASH_STRING_UPDATE(zend_symtable_update, ht, key, strlen(key), str, strlen(str), duplicate);
	return FAILURE;
}
BUESSION_API int buession_hash_symtable_update_string_ex(HashTable *ht, const char *key, uint key_length, char *str, zend_bool duplicate TSRMLS_DC){
	BUESSION_HASH_STRING_UPDATE(zend_symtable_update, ht, key, key_length, str, strlen(str), duplicate);
	return FAILURE;
}

BUESSION_API int buession_hash_index_update_stringl(HashTable *ht, ulong index, char *str, uint str_length, zend_bool duplicate TSRMLS_DC){
	zval *temp;

	MAKE_STD_ZVAL(temp);
	ZVAL_STRINGL(temp, str, str_length, duplicate);
	return zend_hash_index_update(ht, index, (void *) &temp, sizeof(zval *), NULL);
}
BUESSION_API int buession_hash_update_stringl(HashTable *ht, const char *key, char *str, uint str_length, zend_bool duplicate TSRMLS_DC){
	BUESSION_HASH_STRING_UPDATE(zend_hash_update, ht, key, strlen(key), str, str_length, duplicate);
	return FAILURE;
}
BUESSION_API int buession_hash_update_stringl_ex(HashTable *ht, const char *key, uint key_length, char *str, uint str_length, zend_bool duplicate TSRMLS_DC){
	BUESSION_HASH_STRING_UPDATE(zend_hash_update, ht, key, key_length, str, str_length, duplicate);
	return FAILURE;
}
BUESSION_API int buession_hash_symtable_update_stringl(HashTable *ht, const char *key, char *str, uint str_length, zend_bool duplicate TSRMLS_DC){
	BUESSION_HASH_STRING_UPDATE(zend_symtable_update, ht, key, strlen(key), str, str_length, duplicate);
	return FAILURE;
}
BUESSION_API int buession_hash_symtable_update_stringl_ex(HashTable *ht, const char *key, uint key_length, char *str, uint str_length, zend_bool duplicate TSRMLS_DC){
	BUESSION_HASH_STRING_UPDATE(zend_symtable_update, ht, key, key_length, str, str_length, duplicate);
	return FAILURE;
}

BUESSION_API int buession_hash_index_update_zval(HashTable *ht, ulong index, zval *value TSRMLS_DC){
	return zend_hash_index_update(ht, index, (void *) &value, sizeof(zval *), NULL);
}
BUESSION_API int buession_hash_update_zval(HashTable *ht, const char *key, zval *value TSRMLS_DC){
	return zend_hash_update(ht, key, strlen(key) + 1, (void *) &value, sizeof(zval *), NULL);
}
BUESSION_API int buession_hash_update_zval_ex(HashTable *ht, const char *key, uint key_length, zval *value TSRMLS_DC){
	return zend_hash_update(ht, key, key_length, (void *) &value, sizeof(zval *), NULL);
}
BUESSION_API int buession_hash_symtable_update_zval(HashTable *ht, const char *key, zval *value TSRMLS_DC){
	return zend_symtable_update(ht, key, strlen(key) + 1, (void *) &value, sizeof(zval *), NULL);
}
BUESSION_API int buession_hash_symtable_update_zval_ex(HashTable *ht, const char *key, uint key_length, zval *value TSRMLS_DC){
	return zend_symtable_update(ht, key, key_length, (void *) &value, sizeof(zval *), NULL);
}
/* update end */

static inline int buession_hash_persistent_copy_apply(zval **value TSRMLS_DC, int num_args, va_list args, zend_hash_key *hash_key){
	HashTable *target = va_arg(args, HashTable *);
	zval *val = buession_zval_persistent_value(*value TSRMLS_CC);

	if(val){
		if(hash_key->nKeyLength > 0){
			buession_hash_update_zval_ex(target, hash_key->arKey, hash_key->nKeyLength, val TSRMLS_CC);
		}else{
			buession_hash_index_update_zval(target, hash_key->h, val TSRMLS_CC);
		}
	}

	return ZEND_HASH_APPLY_KEEP;
}
BUESSION_API void buession_hash_persistent_copy(HashTable *target, HashTable *source TSRMLS_DC){
	zend_hash_apply_with_arguments(source TSRMLS_CC, (apply_func_args_t) buession_hash_persistent_copy_apply, 1, target);
}

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */