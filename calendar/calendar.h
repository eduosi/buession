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

/* $Id: calendar/calendar.h 310447 2013-09-16 22:39:13Z Yong.Teng $ */
#ifndef BUESSION_CALENDAR_H
#define BUESSION_CALENDAR_H

extern zend_class_entry *calendar_ce;

#define BUESSION_CALENDAR_ME(method, arguments, flags) ZEND_ME(buession_calendar, method, arguments, flags)

#define calendar_register_format_constant(name)	zend_declare_class_constant_stringl(calendar_ce, ZEND_STRL("FORMAT_"#name), ZEND_STRL(CALENDAR_FORMAT_##name) TSRMLS_CC)

/* format start */
#define CALENDAR_FORMAT_YEAR	"YYYY"
#define CALENDAR_FORMAT_SHORT_YEAR	"YY"

#define CALENDAR_FORMAT_SEASON	"s"

#define CALENDAR_FORMAT_MONTH	"mm"
#define CALENDAR_FORMAT_SHORT_MONTH	"m"
#define CALENDAR_FORMAT_FULL_MONTH	"F"
#define CALENDAR_FORMAT_ABB_MONTH	"M"

#define CALENDAR_FORMAT_WEEKDAY	"w"
#define CALENDAR_FORMAT_FULL_WEEKDAY	"WW"
#define CALENDAR_FORMAT_ABB_WEEKDAY	"W"
#define CALENDAR_FORMAT_DAY_OF_WEEK	"w"

#define CALENDAR_FORMAT_DAY	"dd"
#define CALENDAR_FORMAT_SHORT_DAY	"d"
#define CALENDAR_FORMAT_DAY_S	"ds"

#define CALENDAR_FORMAT_SWATCH	"B"

#define CALENDAR_FORMAT_HOUR	"HH"
#define CALENDAR_FORMAT_SHORT_HOUR	"H"
#define CALENDAR_FORMAT_12_HOUR	"hh"
#define CALENDAR_FORMAT_SHORT_12_HOUR	"h"
#define CALENDAR_FORMAT_HOUR_AP	"A"
#define CALENDAR_FORMAT_L_HOUR_AP	"a"

#define CALENDAR_FORMAT_MINUTES	"mm"
#define CALENDAR_FORMAT_SHORT_MINUTES	"m"

#define CALENDAR_FORMAT_SECONDS	"ss"
#define CALENDAR_FORMAT_SHORT_SECONDS	"s"

#define CALENDAR_FORMAT_CN	"cn"
#define CALENDAR_FORMAT_ZH	"zh"
#define CALENDAR_FORMAT_ZH_CN	"zh_cn"

#define CALENDAR_FORMAT_ATOM	"OOO"
#define CALENDAR_FORMAT_ISO_8601	"c"
#define CALENDAR_FORMAT_RFC_822	"R"
#define CALENDAR_FORMAT_W3C	"WWW"

/*		const DAY_SUFFIX        = "SS";
		const WEEKDAY_NARROW    = "E";
		const WEEKDAY_NAME      = "EE";
		const WEEKDAY_8601      = "eee";
		const WEEK              = "ww";
		const MONTH_DAYS        = "ddd";
		const MONTH_NAME        = "MMMM";
		const MONTH_NAME_SHORT  = "MMM";
		const MONTH_NAME_NARROW = "MMMMM";

		const MILLISECOND       = "S";
		const TIMEZONE_NAME     = "zzzz";
		const DAYLIGHT          = "I";
		const GMT_DIFF          = "Z";
		const GMT_DIFF_SEP      = "ZZZZ";
		const TIMEZONE          = "z";
		const TIMEZONE_SECS     = "X";
		const RFC_2822          = "r";
		const TIMESTAMP         = "U";
		const ERA               = "G";
		const ERA_NAME          = "GGGG";
		const ERA_NARROW        = "GGGGG";
		const DATES             = "F";
		const DATE_FULL         = "FFFFF";
		const DATE_LONG         = "FFFF";
		const DATE_MEDIUM       = "FFF";
		const DATE_SHORT        = "FF";
		const TIMES             = "WW";
		const TIME_FULL         = "TTTTT";
		const TIME_LONG         = "TTTT";
		const TIME_MEDIUM       = "TTT";
		const TIME_SHORT        = "TT";
		const DATETIME          = "K";
		const DATETIME_FULL     = "KKKKK";
		const DATETIME_LONG     = "KKKK";
		const DATETIME_MEDIUM   = "KKK";
		const DATETIME_SHORT    = "KK";
		const COOKIE            = "CCC";
		const RFC_850           = "RR";
		const RFC_1036          = "RRR";
		const RFC_1123          = "RRRR";
		const RFC_3339          = "RRRRR";
		const RSS               = "SSS";
		*/
/* format end */

#define calendar_format_date(format, format_length) php_format_date((format), (format_length), (time_t) intern->time->sse, TRUE TSRMLS_CC)

#define CALENDAR_IS_FORMAT(format, format_length, name)	\
		((format_length) == sizeof(CALENDAR_FORMAT_##name) - 1&&memcmp((format), CALENDAR_FORMAT_##name, (format_length)) == 0)
#define CALENDAR_IS_AP_HOUR_FORMAT(format, format_length, ap, name)	\
	((format_length) == sizeof(CALENDAR_FORMAT_##ap""CALENDAR_FORMAT_##name) - 1&&memcmp((format), CALENDAR_FORMAT_##ap""CALENDAR_FORMAT_##name, (format_length)) == 0)
#define CALENDAR_IS_ZH_FORMAT(format, format_length)	\
	(CALENDAR_IS_FORMAT(format, format_length, CN)||CALENDAR_IS_FORMAT(format, format_length, ZH)||CALENDAR_IS_FORMAT(format, format_length, ZH_CN))

#define calendar_return_long(string){	\
	char *str = (string);	\
	Z_TYPE_P(return_value) = IS_LONG;	\
	Z_LVAL_P(return_value) = strtol(str, NULL, 10);	\
	buession_free(str);	\
	return;	\
}

#define calendar_tostring(format, localtime){	\
	php_date_obj *intern = (php_date_obj *) zend_object_store_get_object(getThis() TSRMLS_CC);	\
	char *string = php_format_date(ZEND_STRL(format), (time_t) intern->time->sse, localtime TSRMLS_CC);	\
	BUESSION_RETURN_STRING(string, FALSE);	\
}

#define calendar_invalid_format_exception(part, format){	\
	zval *exception = buession_throw_exception_error_ex(parseexception_ce, E_WARNING, "Invalid '"part"' format '%s'.", 0, format);	\
	zval_ptr_dtor(&exception);	\
}

BUESSION_API int calendar_string_to_timestamp(char *str, uint str_length, time_t *timestamp TSRMLS_DC);

BUESSION_STARTUP_FUNCTION(calendar);
BUESSION_RINIT_FUNCTION(calendar);
BUESSION_RSHUTDOWN_FUNCTION(calendar);
BUESSION_SHUTDOWN_FUNCTION(calendar);

#endif /* BUESSION_CALENDAR_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
