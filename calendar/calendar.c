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

/* $Id: calendar/calendar.c 310447 2013-09-16 22:39:27Z Yong.Teng $ */

/* include php file */
#include "Zend/zend.h"
#include "Zend/zend_API.h"
#include "main/php.h"
#include "main/php_main.h"
#include "ext/date/php_date.h"

/* include buession file */
#include "buession.h"
#include "calendar.h"

zend_class_entry *calendar_ce;

/* {{{ arginfo */
ZEND_BEGIN_ARG_INFO_EX(calendar___construct_arguments, FALSE, FALSE, 0)
	ZEND_ARG_INFO(FALSE, time)
	ZEND_ARG_OBJ_INFO(FALSE, timezone, DateTimeZone, TRUE)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(calendar_getYear_arguments, FALSE, FALSE, 1)
	ZEND_ARG_INFO(FALSE, format)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(calendar_getSeason_arguments, FALSE, FALSE, 1)
	ZEND_ARG_INFO(FALSE, format)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(calendar_getMonth_arguments, FALSE, FALSE, 1)
	ZEND_ARG_INFO(FALSE, format)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(calendar_getWeekday_arguments, FALSE, FALSE, 1)
	ZEND_ARG_INFO(FALSE, format)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(calendar_getDay_arguments, FALSE, FALSE, 1)
	ZEND_ARG_INFO(FALSE, format)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(calendar_getHour_arguments, FALSE, FALSE, 1)
	ZEND_ARG_INFO(FALSE, format)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(calendar_getMinutes_arguments, FALSE, FALSE, 1)
	ZEND_ARG_INFO(FALSE, format)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(calendar_getSeconds_arguments, FALSE, FALSE, 1)
	ZEND_ARG_INFO(FALSE, format)
ZEND_END_ARG_INFO()
/* }}} */

static char *calendar_season_names[4] = {
	"Spring", "Summer", "Autumn", "Winter"
};

static char *calendar_zh_season_names[4] = {
	"\u6625", "\u590f", "\u79cb", "\u51ac"
};

static char *calendar_zh_month_names[13] = {
	"\u4e00", "\u4e8c", "\u4e09", "\u56db", "\u4e94", "\u516d",
	"\u4e03", "\u516b", "\u4e5d", "\u5341", "\u5341\u4e00", "\u5341\u4e8c"
};

static char *calendar_zh_weekday_names[13] = {
	"\u65e5", "\u4e00", "\u4e8c", "\u4e09", "\u56db", "\u4e94", "\u516d"
};

BUESSION_API int calendar_string_to_timestamp(char *str, uint str_length, time_t *timestamp TSRMLS_DC){
	int retval = FAILURE;

	if(str&&str_length > 0){
		php_date_obj date_obj;

		if(php_date_initialize(&date_obj, str, str_length, NULL, NULL, TRUE TSRMLS_CC) == TRUE){
			*timestamp = (time_t) date_obj.time->sse;
			retval = SUCCESS;
		}

		if(date_obj.time){
			/*if(date_obj.time->tz_abbr){
				free(date_obj.time->tz_abbr);
				date_obj.time->tz_abbr = NULL;
			}
			free(date_obj.time);
			date_obj.time = NULL;*/
		}
	}

	return retval;
}

static inline zend_bool calendar_isLeapYear(timelib_time *time TSRMLS_DC){
	char *string = php_format_date("L", 1, (time_t) time->sse, TRUE TSRMLS_CC);
	zend_bool result = memcmp(string, "1", 1);

	buession_free(string);
	return result;
}

/* {{{ public void Calendar::__construct(integer $timestamp[, DateTimeZone $timezone = NULL])
 *	   public void Calendar::__construct(string $time[, DateTimeZone $timezone = NULL])
 *	   public void Calendar::__construct(DateTime $datetime[, DateTimeZone $timezone = NULL])
*/
static BUESSION_METHOD(calendar, __construct){
	php_date_obj *intern = (php_date_obj *) zend_object_store_get_object(getThis() TSRMLS_CC);
	char *time_str = NULL;
	uint time_str_length = 0;
	long timestamp = 0;
	zval *timezone = NULL;
	zend_class_entry *timezone_ce = php_date_get_timezone_ce();
	//zend_error_handling error_handling;

	//zend_replace_error_handling(EH_THROW, parseexception_ce, &error_handling TSRMLS_CC);
	if(zend_parse_parameters_ex(ZEND_PARSE_PARAMS_QUIET, ZEND_NUM_ARGS() TSRMLS_CC, "l|O", &timestamp, &timezone, timezone_ce) == SUCCESS){
		time_str = calendar_format_date("Y-m-d H:i:s", sizeof("Y-m-d H:i:s") - 1);
		time_str_length = strlen(time_str);
	}else if(zend_parse_parameters_ex(ZEND_PARSE_PARAMS_QUIET, ZEND_NUM_ARGS() TSRMLS_CC, "s|O", &time_str, &time_str_length, &timezone, timezone_ce) == SUCCESS){
		time_str = estrndup(time_str, time_str_length);
	}

	php_date_initialize(intern, time_str, time_str_length, NULL, timezone, TRUE TSRMLS_CC);
	buession_free(time_str);
	//zend_restore_error_handling(&error_handling TSRMLS_CC);
}
/* }}} */

/* {{{ public integer Calendar::getYear([string $format = "YYYY"]) */
static BUESSION_METHOD(calendar, getYear){
	char *format = CALENDAR_FORMAT_YEAR;
	uint format_length = sizeof(CALENDAR_FORMAT_YEAR) - 1;
	long year = -1;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|s", &format, &format_length) == SUCCESS){
		php_date_obj *intern = (php_date_obj *) zend_object_store_get_object(getThis() TSRMLS_CC);
		char *_format = NULL;
		char *string = NULL;

		if(CALENDAR_IS_FORMAT(format, format_length, YEAR)){	//	1999
			calendar_return_long(calendar_format_date("Y", 1));
		}else if(CALENDAR_IS_FORMAT(format, format_length, SHORT_YEAR)){	//	99
			calendar_return_long(calendar_format_date("y", 1));
		}else{
			calendar_invalid_format_exception("year", format);
			RETURN_LONG(year);
		}
	}

	RETURN_LONG(year);
}
/* }}} */

/* {{{ public boolean Calendar::isLeapYear() */
static BUESSION_METHOD(calendar, isLeapYear){
	php_date_obj *intern = (php_date_obj *) zend_object_store_get_object(getThis() TSRMLS_CC);
	RETURN_BOOL(calendar_isLeapYear(intern->time TSRMLS_CC));
}
/* }}} */

/* {{{ public integer Calendar::getYearDays() */
static BUESSION_METHOD(calendar, getYearDays){
	php_date_obj *intern = (php_date_obj *) zend_object_store_get_object(getThis() TSRMLS_CC);
	RETURN_LONG(calendar_isLeapYear(intern->time TSRMLS_CC) == TRUE ? 366 : 365);
}
/* }}} */

/* {{{ public integer Calendar::getDayOfYear() */
static BUESSION_METHOD(calendar, getDayOfYear){
	php_date_obj *intern = (php_date_obj *) zend_object_store_get_object(getThis() TSRMLS_CC);
	calendar_return_long(calendar_format_date("z", 1));
}
/* }}} */

/* {{{ public string/integer Calendar::getSeason([string $format = "s"]) */
static BUESSION_METHOD(calendar, getSeason){
	char *format = CALENDAR_FORMAT_SEASON;
	uint format_length = sizeof(CALENDAR_FORMAT_SEASON) - 1;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|s", &format, &format_length) == SUCCESS){
		zend_ushort format_type = 0;

		if(CALENDAR_IS_FORMAT(format, format_length, SEASON)){
			format_type = 1;
		}else if(format_length == 1&&memcmp(format, "S", 1) == 0){
			format_type = 2;
		}else if(CALENDAR_IS_ZH_FORMAT(format, format_length)){
			format_type = 3;
		}else{
			calendar_invalid_format_exception("season", format);
		}

		if(format_type == 1||format_type == 2||format_type == 3){
			php_date_obj *intern = (php_date_obj *) zend_object_store_get_object(getThis() TSRMLS_CC);
			char *string = calendar_format_date("n", 1);

			if(return_value_used){
				long month = strtol(string, NULL, 10);
				zend_ushort season = 0;

				buession_free(string);
				if(month >= 3&&month <= 5){
					season = 1;
				}else if(month >= 6&&month <= 8){
					season = 2;
				}else if(month >= 9&&month <= 11){
					season = 3;
				}else if(month >= 12||month <= 2){
					season = 4;
				}else{
					RETURN_NULL();
				}

				switch(format_type){
					case 1:
						RETURN_LONG(season);
						break;
					case 2:
						BUESSION_RETURN_STRING(calendar_season_names[season - 1], TRUE);
						break;
					case 3:
						BUESSION_RETURN_STRING(calendar_zh_season_names[season - 1], TRUE);
						break;
					default:
						break;
				}
			}
			buession_free(string);
		}
	}

	RETURN_NULL();
}
/* }}} */

/* {{{ public string/integer Calendar::getMonth([string $format = "m"]) */
static BUESSION_METHOD(calendar, getMonth){
	char *format = CALENDAR_FORMAT_SHORT_MONTH;
	uint format_length = sizeof(CALENDAR_FORMAT_SHORT_MONTH) - 1;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|s", &format, &format_length) == SUCCESS){
		php_date_obj *intern = (php_date_obj *) zend_object_store_get_object(getThis() TSRMLS_CC);

		if(CALENDAR_IS_FORMAT(format, format_length, SHORT_MONTH)){	// 1 ~ 12
			calendar_return_long(calendar_format_date("n", 1));
		}else if(CALENDAR_IS_FORMAT(format, format_length, MONTH)	// 01 ~ 12
			||CALENDAR_IS_FORMAT(format, format_length, FULL_MONTH)	//	January ~ December
			||CALENDAR_IS_FORMAT(format, format_length, ABB_MONTH)){	//	Jan ~ Dec
			BUESSION_RETURN_STRING(calendar_format_date(format, format_length), FALSE);
		}else if(CALENDAR_IS_ZH_FORMAT(format, format_length)){	// zh
			long month = 0;
			char *string = calendar_format_date(format, format_length);

			month = strtol(string, NULL, 10);
			buession_free(string);
			if(month >= 1&&month <= 12){
				BUESSION_RETURN_STRING(calendar_zh_month_names[month - 1], TRUE);
			}
		}else{
			calendar_invalid_format_exception("month", format);
		}
	}

	RETURN_NULL();
}
/* }}} */

/* {{{ public integer Calendar::getMonthDays() */
static BUESSION_METHOD(calendar, getMonthDays){
	php_date_obj *intern = (php_date_obj *) zend_object_store_get_object(getThis() TSRMLS_CC);
	calendar_return_long(calendar_format_date("t", 1));
}
/* }}} */

/* {{{ public integer Calendar::getDayOfMonth() */
static BUESSION_METHOD(calendar, getDayOfMonth){
	php_date_obj *intern = (php_date_obj *) zend_object_store_get_object(getThis() TSRMLS_CC);
	calendar_return_long(calendar_format_date("j", 1));
}
/* }}} */

/* {{{ public string/integer Calendar::getWeekday([string $format = "w"]) */
static BUESSION_METHOD(calendar, getWeekday){
	char *format = "w";
	uint format_length = 1;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|s", &format, &format_length) == SUCCESS){
		php_date_obj *intern = (php_date_obj *) zend_object_store_get_object(getThis() TSRMLS_CC);

		if(CALENDAR_IS_FORMAT(format, format_length, WEEKDAY)){	// 1 ~ 7
			calendar_return_long(calendar_format_date("N", 1));
		}else if(CALENDAR_IS_FORMAT(format, format_length, FULL_WEEKDAY)){	// Sunday ~ Saturday
			RETURN_STRING(calendar_format_date("l", 1), FALSE);
		}else if(CALENDAR_IS_FORMAT(format, format_length, ABB_WEEKDAY)){	// Mon ~ Sun
			RETURN_STRING(calendar_format_date("D", 1), FALSE);
		}else if(CALENDAR_IS_ZH_FORMAT(format, format_length)){	// zh
			long weekday = 0;
			char *string = calendar_format_date("w", 1);

			weekday = strtol(string, NULL, 10);
			buession_free(string);
			if(weekday >= 0&&weekday <= 6){
				BUESSION_RETURN_STRING(calendar_zh_weekday_names[weekday], TRUE);
			}
		}else{
			calendar_invalid_format_exception("weekday", format);
		}
	}

	RETURN_NULL();
}
/* }}} */

/* {{{ public integer Calendar::getDayOfWeek() */
static BUESSION_METHOD(calendar, getDayOfWeek){
	php_date_obj *intern = (php_date_obj *) zend_object_store_get_object(getThis() TSRMLS_CC);
	calendar_return_long(calendar_format_date("w", 1));
}
/* }}} */

/* {{{ public string/integer Calendar::getDay([string $format = "dd"]) */
static BUESSION_METHOD(calendar, getDay){
	char *format = CALENDAR_FORMAT_DAY;
	uint format_length = sizeof(CALENDAR_FORMAT_DAY) - 1;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|s", &format, &format_length) == SUCCESS){
		php_date_obj *intern = (php_date_obj *) zend_object_store_get_object(getThis() TSRMLS_CC);

		if(CALENDAR_IS_FORMAT(format, format_length, SHORT_DAY)){	//	1~31
			calendar_return_long(calendar_format_date("j", 1));
		}else if(CALENDAR_IS_FORMAT(format, format_length, DAY)){	// 01~31
			RETURN_STRING(calendar_format_date("d", 1), FALSE);
		}else if(CALENDAR_IS_FORMAT(format, format_length, DAY_S)){	// 1 st
			RETURN_STRING(calendar_format_date("j S", 3), FALSE);
		}else{
			calendar_invalid_format_exception("day", format);
		}
	}

	RETURN_NULL();
}
/* }}} */

/* {{{ public string/integer Calendar::getHour([string $format = "HH"]) */
static BUESSION_METHOD(calendar, getHour){
	char *format = CALENDAR_FORMAT_HOUR;
	uint format_length = sizeof(CALENDAR_FORMAT_HOUR) - 1;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|s", &format, &format_length) == SUCCESS){
		php_date_obj *intern = (php_date_obj *) zend_object_store_get_object(getThis() TSRMLS_CC);

		if(CALENDAR_IS_FORMAT(format, format_length, HOUR)){ // 00~23
			RETURN_STRING(calendar_format_date("H", 1), FALSE);
		}else if(CALENDAR_IS_FORMAT(format, format_length, SHORT_HOUR)){ // 0~23
			calendar_return_long(calendar_format_date("G", 1));
		}else if(CALENDAR_IS_FORMAT(format, format_length, 12_HOUR)){ // 00~12
			RETURN_STRING(calendar_format_date("h", 1), FALSE);
		}else if(CALENDAR_IS_FORMAT(format, format_length, SHORT_12_HOUR)){ // 0~12
			calendar_return_long(calendar_format_date("g", 1));
		}else if(CALENDAR_IS_AP_HOUR_FORMAT(format, format_length, HOUR_AP, 12_HOUR)){	// AM 09
			RETURN_STRING(calendar_format_date("A h", 2), FALSE);
		}else if(CALENDAR_IS_AP_HOUR_FORMAT(format, format_length, L_HOUR_AP, 12_HOUR)){	// am 09
			RETURN_STRING(calendar_format_date("a h", 2), FALSE);
		}else if(CALENDAR_IS_AP_HOUR_FORMAT(format, format_length, HOUR_AP, SHORT_12_HOUR)){	// AM 9
			RETURN_STRING(calendar_format_date("A g", 2), FALSE);
		}else if(CALENDAR_IS_AP_HOUR_FORMAT(format, format_length, L_HOUR_AP, SHORT_12_HOUR)){	// am 9
			RETURN_STRING(calendar_format_date("a g", 2), FALSE);
		}else{
			calendar_invalid_format_exception("hour", format);
		}
	}

	RETURN_NULL();
}
/* }}} */

/* {{{ public string/integer Calendar::getMinutes([string $format = "mm"]) */
static BUESSION_METHOD(calendar, getMinutes){
	char *format = CALENDAR_FORMAT_MINUTES;
	uint format_length = sizeof(CALENDAR_FORMAT_MINUTES) - 1;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|s", &format, &format_length) == SUCCESS){
		php_date_obj *intern = (php_date_obj *) zend_object_store_get_object(getThis() TSRMLS_CC);

		if(CALENDAR_IS_FORMAT(format, format_length, MINUTES)){ // 00~59
			RETURN_STRING(calendar_format_date("i", 1), FALSE);
		}else if(CALENDAR_IS_FORMAT(format, format_length, SHORT_MINUTES)){ // 0~59
			calendar_return_long(calendar_format_date("i", 1));
		}else{
			calendar_invalid_format_exception("minutes", format);
		}
	}

	RETURN_NULL();
}
/* }}} */

/* {{{ public string/integer Calendar::getSeconds([string $format = "ss"]) */
static BUESSION_METHOD(calendar, getSeconds){
	char *format = CALENDAR_FORMAT_SECONDS;
	uint format_length = sizeof(CALENDAR_FORMAT_SECONDS) - 1;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|s", &format, &format_length) == SUCCESS){
		php_date_obj *intern = (php_date_obj *) zend_object_store_get_object(getThis() TSRMLS_CC);

		if(CALENDAR_IS_FORMAT(format, format_length, SECONDS)){ // 00~59
			RETURN_STRING(calendar_format_date("s", 1), FALSE);
		}else if(CALENDAR_IS_FORMAT(format, format_length, SHORT_SECONDS)){ // 0~59
			calendar_return_long(calendar_format_date("s", 1));
		}else{
			calendar_invalid_format_exception("seconds", format);
		}
	}

	RETURN_NULL();
}
/* }}} */

/* {{{ public string Calendar::toGMTString() */
static BUESSION_METHOD(calendar, toGMTString){
	calendar_tostring("D, d-M-Y H:i:s e", FALSE);
}
/* }}} */

/* {{{ public string Calendar::toString() */
static BUESSION_METHOD(calendar, toString){
	calendar_tostring("D, d-M-Y H:i:s T", TRUE);
}
/* }}} */

/* {{{ public string Calendar::__toString() */
static BUESSION_METHOD(calendar, __toString){
	calendar_tostring("D, d-M-Y H:i:s T", TRUE);
}
/* }}} */

/* {{{ public void Calendar::__destruct() */
static BUESSION_METHOD(calendar, __destruct){
}
/* }}} */

static zend_function_entry calendar_methods[] = {
	BUESSION_CALENDAR_ME(__construct, calendar___construct_arguments, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	BUESSION_CALENDAR_ME(getYear, calendar_getYear_arguments, ZEND_ACC_PUBLIC)
	BUESSION_CALENDAR_ME(isLeapYear, NULL, ZEND_ACC_PUBLIC)
	BUESSION_CALENDAR_ME(getYearDays, NULL, ZEND_ACC_PUBLIC)
	BUESSION_CALENDAR_ME(getDayOfYear, NULL, ZEND_ACC_PUBLIC)
	BUESSION_CALENDAR_ME(getSeason, calendar_getSeason_arguments, ZEND_ACC_PUBLIC)
	BUESSION_CALENDAR_ME(getMonth, calendar_getMonth_arguments, ZEND_ACC_PUBLIC)
	BUESSION_CALENDAR_ME(getMonthDays, NULL, ZEND_ACC_PUBLIC)
	BUESSION_CALENDAR_ME(getDayOfMonth, calendar_getMonth_arguments, ZEND_ACC_PUBLIC)
	BUESSION_CALENDAR_ME(getWeekday, calendar_getWeekday_arguments, ZEND_ACC_PUBLIC)
	BUESSION_CALENDAR_ME(getDayOfWeek, NULL, ZEND_ACC_PUBLIC)
	BUESSION_CALENDAR_ME(getDay, calendar_getDay_arguments, ZEND_ACC_PUBLIC)
	BUESSION_CALENDAR_ME(getHour, calendar_getHour_arguments, ZEND_ACC_PUBLIC)
	BUESSION_CALENDAR_ME(getMinutes, calendar_getMinutes_arguments, ZEND_ACC_PUBLIC)
	BUESSION_CALENDAR_ME(getSeconds, calendar_getSeconds_arguments, ZEND_ACC_PUBLIC)
	BUESSION_CALENDAR_ME(toGMTString, NULL, ZEND_ACC_PUBLIC)
	BUESSION_CALENDAR_ME(toString, NULL, ZEND_ACC_PUBLIC)
	BUESSION_CALENDAR_ME(__toString, NULL, ZEND_ACC_PUBLIC)
	BUESSION_CALENDAR_ME(__destruct, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_DTOR)
	ZEND_FE_END
};

/** {{{ BUESSION_STARTUP_FUNCTION */
BUESSION_STARTUP_FUNCTION(calendar){
	/*zend_class_entry ce;

	BUESSION_INIT_CLASS_ENTRY(ce, "Calendar", calendar_methods);
	calendar_ce = zend_register_internal_class_ex(&ce, php_date_get_date_ce(), NULL TSRMLS_CC);

	calendar_register_format_constant(YEAR);
	calendar_register_format_constant(SHORT_YEAR);

	calendar_register_format_constant(SEASON);

	calendar_register_format_constant(MONTH);
	calendar_register_format_constant(SHORT_MONTH);
	calendar_register_format_constant(FULL_MONTH);
	calendar_register_format_constant(ABB_MONTH);

	calendar_register_format_constant(WEEKDAY);
	calendar_register_format_constant(FULL_WEEKDAY);
	calendar_register_format_constant(ABB_WEEKDAY);
	calendar_register_format_constant(DAY_OF_WEEK);

	calendar_register_format_constant(DAY);
	calendar_register_format_constant(SHORT_DAY);
	calendar_register_format_constant(DAY_S);

	calendar_register_format_constant(SWATCH);

	calendar_register_format_constant(HOUR);
	calendar_register_format_constant(SHORT_HOUR);
	calendar_register_format_constant(12_HOUR);
	calendar_register_format_constant(SHORT_12_HOUR);
	calendar_register_format_constant(HOUR_AP);
	calendar_register_format_constant(L_HOUR_AP);

	calendar_register_format_constant(MINUTES);
	calendar_register_format_constant(SHORT_MINUTES);

	calendar_register_format_constant(SECONDS);
	calendar_register_format_constant(SHORT_SECONDS);

	calendar_register_format_constant(CN);
	calendar_register_format_constant(ZH);
	calendar_register_format_constant(ZH_CN);

	calendar_register_format_constant(ATOM);
	calendar_register_format_constant(ISO_8601);
	calendar_register_format_constant(RFC_822);
	calendar_register_format_constant(W3C);*/

	return SUCCESS;
}
/* }}} */

/** {{{ BUESSION_RINIT_FUNCTION */
BUESSION_RINIT_FUNCTION(calendar){
	return SUCCESS;
}
/* }}} */

/** {{{ BUESSION_RSHUTDOWN_FUNCTION */
BUESSION_RSHUTDOWN_FUNCTION(calendar){
	return SUCCESS;
}
/* }}} */

/** {{{ BUESSION_SHUTDOWN_FUNCTION */
BUESSION_SHUTDOWN_FUNCTION(calendar){
	return SUCCESS;
}
/* }}} */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
