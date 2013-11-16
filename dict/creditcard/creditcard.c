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

/* $Id: dict/creditcard/creditcard.c 310447 2013-09-12 13:14:45Z Yong.Teng $ */

#include <stddef.h>
#include <string.h>

/* include buession file */
#include "creditcard.h"
#include "amerucan_express.h"
#include "diners_club_us.h"
#include "diners_club.h"
#include "discover.h"
#include "jcb.h"
#include "laser.h"
#include "maestro.h"
#include "mastercard.h"
#include "solo.h"
#include "uniopay.h"
#include "visa.h"

static const creditcard_entry *creditcard_entries[] = {
	&buession_creditcard_amerucan_express_entry,
	&buession_creditcard_diners_club_us_entry,
	&buession_creditcard_diners_club_entry,
	&buession_creditcard_discover_entry,
	&buession_creditcard_jcb_entry,
	&buession_creditcard_laser_entry,
	&buession_creditcard_maestro_entry,
	&buession_creditcard_mastercard_entry,
	&buession_creditcard_solo_entry,
	&buession_creditcard_uniopay_entry,
	&buession_creditcard_visa_entry,
	NULL
};

const creditcard_entry **buession_creditcard_getAll(){
	return creditcard_entries;
}

const creditcard_entry *buession_creditcard_no2creditcard(const creditcard_no no){
	const creditcard_entry *entry;
	unsigned int i = 0;

	if(no < CREDITCARD_NO_AMERICAN_EXPRESS||no > CREDITCARD_NO_VISA){
		return NULL;
	}

	while((entry = creditcard_entries[i++]) != NULL){
		if(entry->no == no){
			return entry;
		}
	}

	return NULL;
}

const creditcard_entry *buession_creditcard_name2creditcard(const char *name, unsigned int name_length){
	const creditcard_entry *entry;
	unsigned int i = 0;

	if(name == NULL||name_length == 0){
		return NULL;
	}

	while((entry = creditcard_entries[i++]) != NULL){
		if(entry->name_length == name_length&&memcmp(entry->name, name, name_length) == 0){
			return entry;
		}
	}

	return NULL;
}

static inline BOOL _buession_creditcard_valid(const char *str, unsigned int str_length){
	unsigned int i = 0;
	unsigned int j = 1;
	unsigned int n = str_length - 1;
	unsigned int k = 0;
	unsigned int temp = 0;
	unsigned int sum = 0;

	for(; i < str_length; i++){
		if(str[i] == ' '){
			--n;
			continue;
		}

		temp = str[n] - '0';

		if(j % 2 == 1){
			sum += temp;
		}

		if(j % 2 == 0){
			k = temp * 2;
			sum += k > 9 ? k - 9 : k;
		}

		++j;
		--n;
	}

	return sum % 10 == 0;
}

creditcard_valid_state buession_creditcard_valid(const char *str, unsigned int str_length, int no){
	const creditcard_entry *entry;
	unsigned int i = 0;

	if(no != 0&&!(no&CREDITCARD_NO_ALL)){
		return CREDITCARD_VALID_STSTE_TYPE_ERROR;
	}

	while((entry = creditcard_entries[i++]) != NULL){
		if(no == 0||(no&entry->no)){
			if(entry->valid&&entry->valid(entry, str, str_length) == TRUE){
				if(_buession_creditcard_valid(str, str_length) == TRUE){
					return CREDITCARD_VALID_STSTE_SUCCESS;
				}
			}
		}
	}

	return CREDITCARD_VALID_STSTE_INVALID;
}
creditcard_valid_state buession_creditcard_valid_ex(const char *str, unsigned int str_length, const creditcard_entry *entry){
	return entry->valid&&entry->valid(entry, str, str_length) == TRUE ? CREDITCARD_VALID_STSTE_SUCCESS : CREDITCARD_VALID_STSTE_INVALID;
}

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */