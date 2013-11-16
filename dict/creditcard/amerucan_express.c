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

/* $Id: dict/creditcard/amerucan_express.c 310447 2013-09-12 13:38:09Z Yong.Teng $ */

#include <stddef.h>

/* include buession file */
#include "amerucan_express.h"
#include "creditcard.h"

const creditcard_prefix buession_creditcard_amerucan_express_prefixs[] = {
	{"34", 	2},
	{"37", 	2},
	{NULL,	0}
};

static BOOL buession_creditcard_amerucan_express_15_valid(const char *str, unsigned int str_length, unsigned int length){
	if(str[4] == ' '){
		if(str_length == length + 2){
			unsigned int i = 0;

			for(; i < str_length; i++){
				if(i == 4||(i == 11&&str[11] == ' ')){
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

CREDITCARD_VALID_FN(amerucan_express){
	creditcard_prefix prefix;
	unsigned int i = 0;

	while((prefix = (*entry->prefixs)[i++]).prefix != NULL){
		if(creditcard_prefix_valid(prefix, str, str_length)){
			if(creditcard_format_valid(amerucan_express, str, str_length, 15)){
				return TRUE;
			}
		}
	}

	return FALSE;
}

const creditcard_entry buession_creditcard_amerucan_express_entry = {
	CREDITCARD_NO_AMERICAN_EXPRESS,
	"AMERICAN EXPRESS",
	16,
	{15},	/* 3412 345678 9012 */
	1,
	(const creditcard_prefix (*)[]) &buession_creditcard_amerucan_express_prefixs,
	CREDITCARD_VALID_FN_NAMED(amerucan_express)
};

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */