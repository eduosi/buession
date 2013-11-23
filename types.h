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

/* $Id: types.h 310447 2013-09-12 13:08:39Z Yong.Teng $ */
#ifndef BUESSION_TYPES_H
#define BUESSION_TYPES_H

enum ip_type {
	IPV4 = 1,
	IPV6 = 2
};

enum isbn_type {
	ISBN_10 = 1,
	ISBN_13 = 2
};

enum tel_flags {
	TEL_TAIL 			= 1,
	TEL_EXTENSION 		= 2,
	TEL_SPECIAL 		= 4,
	TEL_FORCE_SEGMENT	= 8
};

#endif /* BUESSION_TYPES_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */