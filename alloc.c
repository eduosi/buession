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

/* $Id: alloc.c 310447 2013-08-13 15:28:34Z Yong.Teng $ */

/* include php file */
#include "Zend/zend.h"
#include "Zend/zend_API.h"
#include "main/php.h"
#include "main/php_main.h"

/* include buession file */
#include "buession.h"
#include "alloc.h"

BUESSION_API zval *buession_zval_persistent_value(zval *value TSRMLS_DC){
	zval *ret = (zval *) pemalloc(sizeof(zval), TRUE);

	INIT_PZVAL(ret);
	Z_TYPE_P(ret) = Z_TYPE_P(value);

	switch(Z_TYPE_P(value)){
		case IS_LONG: case IS_BOOL: {
				long *l = (long *) pemalloc(sizeof(long), TRUE);

				memcpy(l, &Z_LVAL_P(value), sizeof(long));
				Z_LVAL_P(ret) = *l;
			}
			break;
		case IS_DOUBLE:	{
				double *d = (double *) pemalloc(sizeof(double), TRUE);

				memcpy(d, &Z_DVAL_P(value), sizeof(double));
				Z_DVAL_P(ret) = *d;
			}
			break;
		case IS_CONSTANT: case IS_STRING:
			CHECK_ZVAL_STRING(zvalue);
			Z_STRVAL_P(ret) = pestrndup(Z_STRVAL_P(value), Z_STRLEN_P(value), TRUE);
			Z_STRLEN_P(ret) = Z_STRLEN_P(value);
			break;
		case IS_RESOURCE:
			break;
		case IS_ARRAY: case IS_CONSTANT_ARRAY: {
				HashTable *temp_ht = (HashTable *) pemalloc(sizeof(HashTable), TRUE);

				if(!temp_ht){
					zval_ptr_dtor(&ret);
					buession_outofmemory_exception();

					return NULL;
				}

				zend_hash_init(temp_ht, zend_hash_num_elements(Z_ARRVAL_P(value)), NULL, (dtor_func_t) buession_zval_persistent_dtor, TRUE);
				buession_hash_persistent_copy(temp_ht, Z_ARRVAL_P(value) TSRMLS_CC);
				Z_ARRVAL_P(ret) = temp_ht;
			}
			break;
		case IS_OBJECT: {
				/*zend_object_value *zobject = (zend_object_value *) pemalloc(sizeof(zend_object_value), TRUE);
				HashTable *properties = zend_std_get_properties(value TSRMLS_CC);
				zend_object *new_object;

				zobject->handle = Z_OBJ_HANDLE_P(value);
				zobject->handlers = (zend_object_handlers *) pemalloc(sizeof(zend_object_handlers), TRUE);
				//memcpy(zobject, &Z_OBJVAL_P(value), sizeof(zend_object_value));
				memcpy(zobject->handlers, &Z_OBJ_HT_P(value), sizeof(zend_object_handlers));
				Z_OBJVAL_P(ret) = *zobject;
				new_object = zend_object_store_get_object(ret TSRMLS_CC);
				buession_hash_persistent_copy(new_object->properties, properties TSRMLS_CC);*/
			}
			break;
		case IS_NULL:
			break;
		default:
			break;
	}

	return ret;
}

BUESSION_API void buession_zval_persistent_dtor(zval **value){
	if(*value){
		switch(Z_TYPE_PP(value)){
			case IS_STRING: case IS_CONSTANT:
				CHECK_ZVAL_STRING(*value);
				pefree(Z_STRVAL_PP(value), TRUE);
				pefree(*value, TRUE);
				break;
			case IS_ARRAY: case IS_CONSTANT_ARRAY:
				zend_hash_destroy(Z_ARRVAL_PP(value));
				pefree(Z_ARRVAL_PP(value), TRUE);
				pefree(*value, TRUE);
				break;
			default:
				break;
		}
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