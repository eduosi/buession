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

/* $Id: config/json.h 310447 2011-12-22 19:54:34Z Teng Yong $ */

#ifndef BUESSION_CONFIG_JSON_H
#define BUESSION_CONFIG_JSON_H

extern zend_class_entry *buession_config_json_ce;

#define BUESSION_CONFIG_JSON_ME(method, arguments, flags) ZEND_ME(buession_config_json, method, arguments, flags)

#ifndef JSON_PARSER_DEFAULT_DEPTH
	#define JSON_PARSER_DEFAULT_DEPTH 512
#endif

BUESSION_STARTUP_FUNCTION(config_json);
BUESSION_RINIT_FUNCTION(config_json);
BUESSION_RSHUTDOWN_FUNCTION(config_json);
BUESSION_SHUTDOWN_FUNCTION(config_json);

#endif	/* BUESSION_CONFIG_JSON_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
