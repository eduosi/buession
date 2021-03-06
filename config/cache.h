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

/* $Id: config/cache.h 310447 2013-08-11 16:32:02Z Teng Yong $ */
#ifndef BUESSION_CONFIG_CACHE_H
#define BUESSION_CONFIG_CACHE_H

typedef struct _config_cache {
	const char *cacheid;
	size_t cacheid_length;
	time_t modifiedat;
	HashTable *data;
} config_cache;

BUESSION_API void config_cache_startup(TSRMLS_D);

BUESSION_API void config_cache_initialize(config_cache *cache TSRMLS_DC);

BUESSION_API int config_cache_save(const char *cacheid, size_t cacheid_length, char *path, uint path_length, HashTable *data TSRMLS_DC);

BUESSION_API int config_cache_load(const char *cacheid, size_t cacheid_length, char *path, uint path_length, config_cache **cache TSRMLS_DC);

BUESSION_API void config_cache_free(config_cache *cache TSRMLS_DC);

#endif /* BUESSION_CONFIG_CACHE_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
