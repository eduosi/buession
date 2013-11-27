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

/* $Id: helper/helper.h 310447 2013-10-11 12:12:06Z Yong.Teng $ */
#ifndef BUESSION_HELPER_H
#define BUESSION_HELPER_H

extern zend_class_entry *helper_ce;

#define BUESSION_HELPER_ME(method, arguments, flags) ZEND_ME(buession_helper, method, arguments, flags)

BUESSION_STARTUP_FUNCTION(helper);
BUESSION_RINIT_FUNCTION(helper);
BUESSION_RSHUTDOWN_FUNCTION(helper);
BUESSION_SHUTDOWN_FUNCTION(helper);

#endif /* BUESSION_HELPER_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
