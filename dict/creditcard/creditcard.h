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

/* $Id: dict/creditcard/creditcard.h 310447 2013-09-12 13:14:40Z Yong.Teng $ */
#ifndef BUESSION_DICT_CREDITCARD_CREDITCARD_H
#define BUESSION_DICT_CREDITCARD_CREDITCARD_H

#if defined(WIN32) && !defined(__cplusplus)
	#undef inline
	#define inline __inline
#endif

#ifdef FALSE
	#undef FALSE
#endif

#ifdef TRUE
	#undef TRUE
#endif

typedef enum {
	FALSE,
	TRUE
} BOOL;

typedef enum {
	CREDITCARD_NO_AMERICAN_EXPRESS = (1<<0L),
	CREDITCARD_NO_DINERS_CLUB = (1<<1L),
	CREDITCARD_NO_DINERS_CLUB_US = (1<<2L),
	CREDITCARD_NO_DISCOVER = (1<<3L),
	CREDITCARD_NO_JCB = (1<<4L),
	CREDITCARD_NO_LASER = (1<<5L),
	CREDITCARD_NO_MAESTRO = (1<<6L),
	CREDITCARD_NO_MASTERCARD = (1<<7L),
	CREDITCARD_NO_SOLO = (1<<8L),
	CREDITCARD_NO_UNIONPAY = (1<<9L),
	CREDITCARD_NO_VISA = (1<<10L)
} creditcard_no;

#define CREDITCARD_NO_ALL (CREDITCARD_NO_AMERICAN_EXPRESS|CREDITCARD_NO_DINERS_CLUB|CREDITCARD_NO_DINERS_CLUB_US|CREDITCARD_NO_DISCOVER|CREDITCARD_NO_JCB|CREDITCARD_NO_LASER|CREDITCARD_NO_MAESTRO|CREDITCARD_NO_MASTERCARD|CREDITCARD_NO_SOLO|CREDITCARD_NO_UNIONPAY|CREDITCARD_NO_VISA)

typedef struct _creditcard_prefix {
	const char *prefix;
	unsigned int length;
} creditcard_prefix;

typedef struct _creditcard_entry {
	const creditcard_no no;
	const char *name;
	int name_length;
	unsigned int lengths[8];
	unsigned int lengths_size;
	const creditcard_prefix (*prefixs)[];
	BOOL (*valid)(const struct _creditcard_entry *entry, const char *str, unsigned int str_length);
} creditcard_entry;

typedef enum {
	CREDITCARD_VALID_STSTE_TYPE_ERROR	= -1,
	CREDITCARD_VALID_STSTE_INVALID 		= 0,
	CREDITCARD_VALID_STSTE_SUCCESS 		= 1
} creditcard_valid_state;

#define CREDITCARD_VALID_FN(name) 		static BOOL buession_creditcard_##name##_valid(const creditcard_entry *entry, const char *str, unsigned int str_length)
#define CREDITCARD_VALID_FN_NAMED(name) buession_creditcard_##name##_valid

#define creditcard_prefix_valid(pre, str, str_length) \
	((pre).prefix != NULL&&(str_length) > (pre).length&&memcmp((str), (pre).prefix, (pre).length) == 0)
#define creditcard_format_valid(card_type, str, str_length, length) \
	((str_length) >= length&&buession_creditcard_##card_type##_##length##_valid((str), (str_length), (length)) == TRUE)
#define creditcard_all_digit_valid(str, str_length)	\
	do{	\
		unsigned int i = 0;	\
		for(; i < str_length; i++){	\
			if(str[i] < '0'||str[i] > '9'){	\
				return FALSE;	\
			}	\
		}	\
		return TRUE;	\
	}while(0);

const creditcard_entry **buession_creditcard_getAll();

const creditcard_entry *buession_creditcard_no2creditcard(const creditcard_no no);

const creditcard_entry *buession_creditcard_name2creditcard(const char *name, unsigned int name_length);

creditcard_valid_state buession_creditcard_valid(const char *str, unsigned int str_length, int no);
creditcard_valid_state buession_creditcard_valid_ex(const char *str, unsigned int str_length, const creditcard_entry *entry);

#endif /* BUESSION_DICT_CREDITCARD_CREDITCARD_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */