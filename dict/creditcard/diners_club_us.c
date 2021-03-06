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

/* $Id: dict/creditcard/diners_club_us.c 310447 2013-09-12 13:39:14Z Yong.Teng $ */

#include <stddef.h>

/* include buession file */
#include "diners_club_us.h"
#include "creditcard.h"

const creditcard_prefix buession_creditcard_diners_club_us_prefixs[] = {
	{"54", 	2},
	{"55", 	2},
	{NULL,	0}
};

static inline BOOL buession_creditcard_diners_club_us_16_valid(const char *str, unsigned int str_length, unsigned int length){
	if(str[4] == ' '){
		if(str_length == length + 3){
			unsigned int i = 0;

			for(; i < str_length; i++){
				if(i == 4||(i == 9&&str[9] == ' ')||(i == 14&&str[14] == ' ')){
					continue;
				}else if(str[i] < '0'||str[i] > '9'){
					return FALSE;
				}
			}

			return TRUE;
		}
	}else if(str_length == length){
		creditcard_all_digit_valid(str, str_length);
	}

	return FALSE;
}

CREDITCARD_VALID_FN(diners_club_us){
	creditcard_prefix prefix;
	unsigned int i = 0;

	while((prefix = (*entry->prefixs)[i++]).prefix != NULL){
		if(creditcard_prefix_valid(prefix, str, str_length)){
			if(creditcard_format_valid(diners_club_us, str, str_length, 16)){
				return TRUE;
			}
		}
	}

	return FALSE;
}

const creditcard_entry buession_creditcard_diners_club_us_entry = {
	CREDITCARD_NO_DINERS_CLUB_US,
	"DINERS CLUB US",
	14,
	{16},	/* 5512 3456 7890 1234 */
	1,
	(const creditcard_prefix (*)[]) &buession_creditcard_diners_club_us_prefixs,
	CREDITCARD_VALID_FN_NAMED(diners_club_us)
};

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */