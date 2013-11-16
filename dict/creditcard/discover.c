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

/* $Id: dict/creditcard/discover.c 310447 2013-09-12 13:39:42Z Yong.Teng $ */

#include <stddef.h>

/* include buession file */
#include "discover.h"
#include "creditcard.h"

const creditcard_prefix buession_creditcard_discover_prefixs[] = {
	{"6011", 	4},
	{"622126", 	6},
	{"6221 26", 7},
	{"622127", 	6},
	{"6221 27", 7},
	{"622128", 	6},
	{"6221 28", 7},
	{"622129", 	6},
	{"6221 29", 7},
	{"62213", 	5},
	{"6221 3", 	6},
	{"62214", 	5},
	{"6221 4", 	6},
	{"62215", 	5},
	{"6221 5",  6},
	{"62216", 	5},
	{"6221 6", 	6},
	{"62217", 	5},
	{"6221 7", 	6},
	{"62218", 	5},
	{"6221 8", 	6},
	{"62219", 	5},
	{"6221 9", 	6},
	{"6222", 	4},
	{"6223", 	4},
	{"6224", 	4},
	{"6225", 	4},
	{"6226", 	4},
	{"6227", 	4},
	{"6228", 	4},
	{"62290", 	5},
	{"6229 0", 	6},
	{"62291", 	5},
	{"6229 1", 	6},
	{"622920", 	6},
	{"6229 20", 7},
	{"622921", 	6},
	{"6229 21", 7},
	{"622922", 	6},
	{"6229 22", 7},
	{"622923", 	6},
	{"6229 23", 7},
	{"622924", 	6},
	{"6229 24", 7},
	{"622925", 	6},
	{"6229 25", 7},
	{"644", 	3},
	{"645", 	3},
	{"646", 	3},
	{"647", 	3},
	{"648", 	3},
	{"649", 	3},
	{"65", 		2},
	{NULL,		0}
};

static inline BOOL buession_creditcard_discover_16_valid(const char *str, unsigned int str_length, unsigned int length){
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

CREDITCARD_VALID_FN(discover){
	creditcard_prefix prefix;
	unsigned int i = 0;

	while((prefix = (*entry->prefixs)[i++]).prefix != NULL){
		if(creditcard_prefix_valid(prefix, str, str_length)){
			if(creditcard_format_valid(discover, str, str_length, 16)){
				return TRUE;
			}
		}
	}

	return FALSE;
}

const creditcard_entry buession_creditcard_discover_entry = {
	CREDITCARD_NO_DISCOVER,
	"DISCOVER",
	8,
	{16},	/* 6512 3456 7890 1234 */
	1,
	(const creditcard_prefix (*)[]) &buession_creditcard_discover_prefixs,
	CREDITCARD_VALID_FN_NAMED(discover)
};

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */