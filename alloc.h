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

/* $Id: alloc.h 310447 2013-08-02 16:19:32Z Yong.Teng $ */
#ifndef BUESSION_ALLOC_H
#define BUESSION_ALLOC_H

#ifndef TRUE
	#define TRUE 1
#endif
#ifndef FALSE
	#define FALSE 0
#endif

#define buession_estrdup(str) ((str) ? estrdup(str) : str)
#define buession_estrndup(str, length) ((str) ? estrndup(str, length) : str)
#define buession_estrdup_rel(str) ((str) ? estrdup_rel(ptr) : str)
#define buession_estrndup_rel(str, length) ((str) ? estrndup_rel(ptr, length) : str)
#define buession_pestrdup(str, is_persistent) ((str) ? pestrdup(str, is_persistent) : str)
#define buession_pestrndup(str, length, is_persistent) ((str) ? pestrndup(str, length, is_persistent) : str)

#define BUESSION_COPY_ZVAL(target, source){	\
	zval *z = &(source);	\
	ALLOC_ZVAL(target);	\
	MAKE_COPY_ZVAL(&z, target);	\
}
#define BUESSION_COPY_ZVAL_P(target, source){	\
	ALLOC_ZVAL(target);	\
	MAKE_COPY_ZVAL(&source, target);	\
}
#define BUESSION_COPY_ZVAL_PP(target, source){	\
	ALLOC_ZVAL(target);	\
	MAKE_COPY_ZVAL(source, target);	\
}

#define BUESSION_ARRAY_INIT(array)	\
	MAKE_STD_ZVAL(array);	\
	array_init(array);
#define BUESSION_ARRAY_SIZE_INIT(array, size)	\
	MAKE_STD_ZVAL(array);	\
	array_init_size(array, size);

#define buession_free(v){	\
	char *a = (v);	\
	if(a){	\
		efree(a);	\
		a = NULL;	\
	}	\
}
#define buession_pfree(v, is_persistent){	\
	if(v){	\
		pefree(v, is_persistent);	\
		v = NULL;	\
	}	\
}
#define buession_zval_dtor(z){	\
	if(z){	\
		zval_dtor(z);	\
		efree(z);	\
		z = NULL;	\
	}	\
}
#define buession_zval_ptr_dtor(z){	\
	if(z){	\
		zval_ptr_dtor(&z);	\
		efree(z);	\
		z = NULL;	\
	}	\
}
#define buession_zval_pptr_dtor(z){	\
	if(z){	\
		zval_ptr_dtor(z);	\
		efree(z);	\
	}	\
}
#define buession_hash_free(ht){	\
	if(ht){	\
		zend_hash_destroy(ht);	\
		efree(ht);	\
		ht = NULL;	\
	}	\
}

BUESSION_API zval *buession_zval_persistent_value(zval *value TSRMLS_DC);

BUESSION_API void buession_zval_persistent_dtor(zval **value);

#endif /* BUESSION_ALLOC_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */