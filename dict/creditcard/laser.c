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

/* $Id: dict/creditcard/laser.c 310447 2013-09-12 13:40:18Z Yong.Teng $ */

#include <stddef.h>

/* include buession file */
#include "laser.h"
#include "creditcard.h"

const creditcard_prefix buession_creditcard_laser_prefixs[] = {
	{"6304", 	4},
	{"6706", 	4},
	{"6771", 	4},
	{"6709", 	4},
	{NULL,		0}
};

CREDITCARD_VALID_FN(laser){
	return FALSE;
}

const creditcard_entry buession_creditcard_laser_entry = {
	CREDITCARD_NO_LASER,
	"LASER",
	5,
	{16, 17, 18, 19},
	4,
	(const creditcard_prefix (*)[]) &buession_creditcard_laser_prefixs,
	CREDITCARD_VALID_FN_NAMED(laser)
};

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */