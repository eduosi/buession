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
  +----------------------------------------------------------------------+
*/

/* $Id: config/cache.c 310447 2013-08-11 16:31:51Z Teng Yong $ */

/* include php file */
#include "Zend/zend.h"
#include "Zend/zend_API.h"
#include "main/php.h"
#include "main/php_main.h"
#include "ext/standard/php_filestat.h"

/* include buession file */
#include "buession.h"
#include "cache.h"
#include "config.h"
//#include "logger/logger.h"

HashTable config_caches;

static inline time_t config_file_modified(char *path, uint path_length, time_t mtime TSRMLS_DC){
	zval stat;

	php_stat(path, path_length, FS_MTIME, &stat TSRMLS_CC);
	if(Z_TYPE(stat) != IS_BOOL&&mtime != Z_LVAL(stat)){
		return Z_LVAL(stat);
	}

	return 0;
}

BUESSION_API void config_cache_startup(TSRMLS_D){
	zend_hash_init(&config_caches, 128, NULL, NULL, TRUE);
}

BUESSION_API void config_cache_initialize(config_cache *cache TSRMLS_DC){
	cache->cacheid = NULL;
	cache->cacheid_length = 0;
	cache->modifiedat = 0;
	cache->data = NULL;
}

BUESSION_API int config_cache_save(const char *cacheid, size_t cacheid_length, char *path, uint path_length, HashTable *data TSRMLS_DC){
	config_cache cache;

	if(!data){
		goto failure;
	}

	if((cache.data = (HashTable *) pemalloc(sizeof(HashTable), TRUE)) == NULL){
		buession_outofmemory_exception();
		return FAILURE;
	}

	cache.cacheid = buession_pestrndup(cacheid, cacheid_length, TRUE);
	cache.cacheid_length = cacheid_length;
	cache.modifiedat = config_file_modified(path, path_length, 0 TSRMLS_CC);
	zend_hash_init(cache.data, zend_hash_num_elements(data), NULL, (dtor_func_t) buession_zval_persistent_dtor, TRUE);
	buession_hash_persistent_copy(cache.data, data TSRMLS_CC);

	if(zend_hash_update(&config_caches, cacheid, cacheid_length + 1, (void *) &cache, sizeof(config_cache), NULL) == SUCCESS){
		//logger.error("save file '%s' data to cache '%s' success.", path, cacheid);
		return SUCCESS;
	}

	config_cache_free(&cache TSRMLS_CC);

	failure:
		//logger.error("save file '%s' data to cache failure.", path, cacheid);

		return FAILURE;
}

BUESSION_API int config_cache_load(const char *cacheid, size_t cacheid_length, char *path, uint path_length, config_cache **cache TSRMLS_DC){
	if(zend_hash_find(&config_caches, cacheid, cacheid_length + 1, (void **) cache) == SUCCESS){
		if(config_file_modified(path, path_length, (*cache)->modifiedat TSRMLS_CC) == 0){
			//logger.debug("load config file '%s' from cache '%s' success.", path, cacheid);
			return SUCCESS;
		}else{
			zend_hash_del(&config_caches, cacheid, cacheid_length + 1);
		}
	}

	return FAILURE;
}

BUESSION_API void config_cache_free(config_cache *cache TSRMLS_DC){
	buession_free((char *) cache->cacheid);
	cache->cacheid_length = 0;
	cache->modifiedat = 0 ;
	buession_hash_free(cache->data);
}

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
