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

/* $Id: helper/form.c 310447 2013-10-11 19:02:16Z Yong.Teng $ */

/* include php file */
#include "Zend/zend.h"
#include "Zend/zend_API.h"
#include "main/php.h"
#include "main/php_main.h"
#include "ext/standard/html.h"

/* include buession file */
#include "buession.h"
#include "form.h"
#include "helper.h"
#include "html.h"

zend_class_entry *helper_form_ce;

/* {{{ arginfo */
ZEND_BEGIN_ARG_INFO_EX(helper_form_text_arguments, FALSE, FALSE, 1)
	ZEND_ARG_INFO(FALSE, name)
	ZEND_ARG_INFO(FALSE, id)
	ZEND_ARG_INFO(FALSE, value)
	ZEND_ARG_ARRAY_INFO(FALSE, arguments, TRUE)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(helper_form_password_arguments, FALSE, FALSE, 1)
	ZEND_ARG_INFO(FALSE, name)
	ZEND_ARG_INFO(FALSE, id)
	ZEND_ARG_INFO(FALSE, value)
	ZEND_ARG_ARRAY_INFO(FALSE, arguments, TRUE)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(helper_form_email_arguments, FALSE, FALSE, 1)
	ZEND_ARG_INFO(FALSE, name)
	ZEND_ARG_INFO(FALSE, id)
	ZEND_ARG_INFO(FALSE, value)
	ZEND_ARG_ARRAY_INFO(FALSE, arguments, TRUE)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(helper_form_url_arguments, FALSE, FALSE, 1)
	ZEND_ARG_INFO(FALSE, name)
	ZEND_ARG_INFO(FALSE, id)
	ZEND_ARG_INFO(FALSE, value)
	ZEND_ARG_ARRAY_INFO(FALSE, arguments, TRUE)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(helper_form_number_arguments, FALSE, FALSE, 1)
	ZEND_ARG_INFO(FALSE, name)
	ZEND_ARG_INFO(FALSE, id)
	ZEND_ARG_INFO(FALSE, value)
	ZEND_ARG_INFO(FALSE, min)
	ZEND_ARG_INFO(FALSE, max)
	ZEND_ARG_INFO(FALSE, step)
	ZEND_ARG_ARRAY_INFO(FALSE, arguments, TRUE)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(helper_form_range_arguments, FALSE, FALSE, 1)
	ZEND_ARG_INFO(FALSE, name)
	ZEND_ARG_INFO(FALSE, id)
	ZEND_ARG_INFO(FALSE, value)
	ZEND_ARG_INFO(FALSE, min)
	ZEND_ARG_INFO(FALSE, max)
	ZEND_ARG_INFO(FALSE, step)
	ZEND_ARG_ARRAY_INFO(FALSE, arguments, TRUE)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(helper_form_date_arguments, FALSE, FALSE, 1)
	ZEND_ARG_INFO(FALSE, name)
	ZEND_ARG_INFO(FALSE, id)
	ZEND_ARG_INFO(FALSE, value)
	ZEND_ARG_ARRAY_INFO(FALSE, arguments, TRUE)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(helper_form_month_arguments, FALSE, FALSE, 1)
	ZEND_ARG_INFO(FALSE, name)
	ZEND_ARG_INFO(FALSE, id)
	ZEND_ARG_INFO(FALSE, value)
	ZEND_ARG_ARRAY_INFO(FALSE, arguments, TRUE)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(helper_form_week_arguments, FALSE, FALSE, 1)
	ZEND_ARG_INFO(FALSE, name)
	ZEND_ARG_INFO(FALSE, id)
	ZEND_ARG_INFO(FALSE, value)
	ZEND_ARG_ARRAY_INFO(FALSE, arguments, TRUE)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(helper_form_time_arguments, FALSE, FALSE, 1)
	ZEND_ARG_INFO(FALSE, name)
	ZEND_ARG_INFO(FALSE, id)
	ZEND_ARG_INFO(FALSE, value)
	ZEND_ARG_ARRAY_INFO(FALSE, arguments, TRUE)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(helper_form_datetime_arguments, FALSE, FALSE, 1)
	ZEND_ARG_INFO(FALSE, name)
	ZEND_ARG_INFO(FALSE, id)
	ZEND_ARG_INFO(FALSE, value)
	ZEND_ARG_ARRAY_INFO(FALSE, arguments, TRUE)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(helper_form_datetimeLocal_arguments, FALSE, FALSE, 1)
	ZEND_ARG_INFO(FALSE, name)
	ZEND_ARG_INFO(FALSE, id)
	ZEND_ARG_INFO(FALSE, value)
	ZEND_ARG_ARRAY_INFO(FALSE, arguments, TRUE)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(helper_form_search_arguments, FALSE, FALSE, 1)
	ZEND_ARG_INFO(FALSE, name)
	ZEND_ARG_INFO(FALSE, id)
	ZEND_ARG_INFO(FALSE, value)
	ZEND_ARG_ARRAY_INFO(FALSE, arguments, TRUE)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(helper_form_color_arguments, FALSE, FALSE, 1)
	ZEND_ARG_INFO(FALSE, name)
	ZEND_ARG_INFO(FALSE, id)
	ZEND_ARG_INFO(FALSE, value)
	ZEND_ARG_ARRAY_INFO(FALSE, arguments, TRUE)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(helper_form_hidden_arguments, FALSE, FALSE, 1)
	ZEND_ARG_INFO(FALSE, name)
	ZEND_ARG_INFO(FALSE, id)
	ZEND_ARG_INFO(FALSE, value)
	ZEND_ARG_ARRAY_INFO(FALSE, arguments, TRUE)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(helper_form_radio_arguments, FALSE, FALSE, 3)
	ZEND_ARG_INFO(FALSE, name)
	ZEND_ARG_INFO(FALSE, value)
	ZEND_ARG_INFO(FALSE, text)
	ZEND_ARG_INFO(FALSE, id)
	ZEND_ARG_ARRAY_INFO(FALSE, arguments, TRUE)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(helper_form_checkbox_arguments, FALSE, FALSE, 3)
	ZEND_ARG_INFO(FALSE, name)
	ZEND_ARG_INFO(FALSE, value)
	ZEND_ARG_INFO(FALSE, text)
	ZEND_ARG_INFO(FALSE, id)
	ZEND_ARG_ARRAY_INFO(FALSE, arguments, TRUE)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(helper_form_file_arguments, FALSE, FALSE, 1)
	ZEND_ARG_INFO(FALSE, name)
	ZEND_ARG_ARRAY_INFO(FALSE, arguments, TRUE)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(helper_form_output_arguments, FALSE, FALSE, 0)
	ZEND_ARG_INFO(FALSE, name)
	ZEND_ARG_ARRAY_INFO(FALSE, arguments, TRUE)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(helper_form_keygen_arguments, FALSE, FALSE, 2)
	ZEND_ARG_INFO(FALSE, name)
	ZEND_ARG_INFO(FALSE, keytype)
	ZEND_ARG_ARRAY_INFO(FALSE, arguments, TRUE)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(helper_form_datalist_arguments, FALSE, FALSE, 2)
	ZEND_ARG_INFO(FALSE, id)
	ZEND_ARG_ARRAY_INFO(FALSE, data, TRUE)
	ZEND_ARG_ARRAY_INFO(FALSE, arguments, TRUE)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(helper_form_select_arguments, FALSE, FALSE, 2)
	ZEND_ARG_INFO(FALSE, name)
	ZEND_ARG_ARRAY_INFO(FALSE, data, TRUE)
	ZEND_ARG_INFO(FALSE, multiple)
	ZEND_ARG_INFO(FALSE, size)
	ZEND_ARG_ARRAY_INFO(FALSE, arguments, TRUE)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(helper_form_option_arguments, FALSE, FALSE, 2)
	ZEND_ARG_INFO(FALSE, value)
	ZEND_ARG_INFO(FALSE, text)
	ZEND_ARG_INFO(FALSE, isSelected)
	ZEND_ARG_ARRAY_INFO(FALSE, attributes, TRUE)
ZEND_END_ARG_INFO()
/* }}} */

static inline int helper_html_create_option_apply(zval **element TSRMLS_DC, int num_args, va_list args, zend_hash_key *hash_key){
	smart_str *str = va_arg(args, smart_str *);

	if(Z_TYPE_PP(element) == IS_ARRAY||Z_TYPE_PP(element) == IS_OBJECT){
		HashTable *attributes = buession_zval_convert_to_hash(*element TSRMLS_CC);
		zval **ztext = NULL;

		zend_hash_find(attributes, "text", 5, (void **) &ztext);
		if(ztext){
			char *text = NULL;
			uint text_length = 0;
			zval temp;

			BUESSION_CONVERT_VARIABLE_TO_STRING(*ztext, temp);
			text = estrndup(Z_STRVAL_PP(ztext), Z_STRLEN_PP(ztext));
			text_length = Z_STRLEN_PP(ztext);
			zend_hash_del(Z_ARRVAL_PP(element), "text", 5);

			helper_form_create_option(str, NULL, 0, text, text_length, attributes);

			BUESSION_CONVERT_VARIABLE_PTR_DTOR(*ztext, temp);
			buession_free(text);
		}else{
			helper_form_create_option(str, NULL, 0, NULL, 0, attributes);
		}
	}

	return ZEND_HASH_APPLY_KEEP;
}

static inline void helper_form_normal_input_fn(INTERNAL_FUNCTION_PARAMETERS, char *type, uint type_length){
	char *name;
	uint name_length;
	char *id = NULL;
	uint id_length = 0;
	char *value = "";
	uint value_length = 0;
	HashTable *attributes = NULL;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|ssH", &name, &name_length, &id, &id_length, &value, &value_length, &attributes) == SUCCESS){
		smart_str str = {0};
		HashTable *attrs = NULL;

		helper_form_create_attributes_from_source(attrs, attributes);
		helper_form_create_input(&str, type, type_length, id, id_length, name, name_length, value, value_length, attrs);
		buession_hash_free(attrs);

		HELPER_HTML_RETURN(str);
	}
}

static inline void helper_form_number_input_fn(INTERNAL_FUNCTION_PARAMETERS, char *type, uint type_length){
	char *name;
	uint name_length;
	char *id = NULL;
	uint id_length = 0;
	char *value = "";
	uint value_length = 0;
	double min = 0;
	double max = 0;
	double step = 0;
	HashTable *attributes = NULL;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|ssdddH", &name, &name_length, &id, &id_length, &value, &value_length, &min, &max, &step, &attributes) == SUCCESS){
		smart_str str = {0};
		HashTable *attrs = NULL;

		helper_form_create_attributes_from_source(attrs, attributes);

		if(ZEND_NUM_ARGS() >= 4){
			buession_hash_symtable_update_double_ex(attrs, "min", 4, min TSRMLS_CC);

			if(ZEND_NUM_ARGS() >= 5){
				buession_hash_symtable_update_double_ex(attrs, "max", 4, max TSRMLS_CC);

				if(ZEND_NUM_ARGS() >= 6){
					buession_hash_symtable_update_double_ex(attrs, "step", 5, step TSRMLS_CC);
				}
			}
		}

		helper_form_create_input(&str, type, type_length, id, id_length, name, name_length, value, value_length, attrs);
		buession_hash_free(attrs);

		HELPER_HTML_RETURN(str);
	}
}

static inline void helper_form_radio_checkbox_fn(INTERNAL_FUNCTION_PARAMETERS, char *type, uint type_length){
	char *name;
	uint name_length;
	char *value;
	uint value_length;
	char *text;
	uint text_length;
	char *id = NULL;
	uint id_length = 0;
	HashTable *attributes = NULL;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sss|sH", &name, &name_length, &value, &value_length, &text, &text_length, &id, &id_length, &attributes) == SUCCESS){
		smart_str str = {0};
		HashTable *attrs = NULL;

		helper_form_create_attributes_from_source(attrs, attributes);
		helper_form_create_input(&str, type, type_length, id, id_length, name, name_length, value, value_length, attrs);
		buession_hash_free(attrs);

		smart_str_appendl(&str, "<lable for=\"", sizeof("<lable for=\"") - 1);
		if(id&&id_length > 0){
			smart_str_appendl(&str, id, id_length);
		}
		smart_str_appendl(&str, "\">", 2);
		if(text&&text_length > 0){
			smart_str_appendl(&str, text, text_length);
		}
		smart_str_appendl(&str, "</lable>", sizeof("</lable>") - 1);

		HELPER_HTML_RETURN(str);
	}
}

/* {{{ private void Form::__construct() */
static BUESSION_METHOD(helper_form, __construct){
}
/* }}} */

/* {{{ public string/null Form::text(string $name[, string $id = NULL[, string $value = ""[, array $attributes = NULL]]]) */
static BUESSION_METHOD(helper_form, text){
	helper_form_normal_input_fn(INTERNAL_FUNCTION_PARAM_PASSTHRU, "text", 4);
}
/* }}} */

/* {{{ public string/null Form::password(string $name[, string $id = NULL[, string $value = ""[, array $attributes = NULL]]]) */
static BUESSION_METHOD(helper_form, password){
	helper_form_normal_input_fn(INTERNAL_FUNCTION_PARAM_PASSTHRU, "password", 8);
}
/* }}} */

/* {{{ public string/null Form::email(string $name[, string $id = NULL[, string $value = ""[, array $attributes = NULL]]]) */
static BUESSION_METHOD(helper_form, email){
	helper_form_normal_input_fn(INTERNAL_FUNCTION_PARAM_PASSTHRU, "email", 5);
}
/* }}} */

/* {{{ public string/null Form::url(string $name[, string $id = NULL[, string $value = ""[, array $attributes = NULL]]]) */
static BUESSION_METHOD(helper_form, url){
	helper_form_normal_input_fn(INTERNAL_FUNCTION_PARAM_PASSTHRU, "url", 3);
}
/* }}} */

/* {{{ public string/null Form::number(string $name[, string $id = NULL[, string $value = ""[, double $min = 0[, double $max = 0[, double $step = 0[, array $attributes = NULL]]]]]]) */
static BUESSION_METHOD(helper_form, number){
	helper_form_number_input_fn(INTERNAL_FUNCTION_PARAM_PASSTHRU, "number", 6);
}
/* }}} */

/* {{{ public string/null Form::range(string $name[, string $id = NULL[, string $value = ""[, double $min = 0[, double $max = 0[, double $step = 0[, array $attributes = NULL]]]]]]) */
static BUESSION_METHOD(helper_form, range){
	helper_form_number_input_fn(INTERNAL_FUNCTION_PARAM_PASSTHRU, "range", 5);
}
/* }}} */

/* {{{ public string/null Form::date(string $name[, string $id = NULL[, string $value = ""[, array $attributes = NULL]]]) */
static BUESSION_METHOD(helper_form, date){
	helper_form_normal_input_fn(INTERNAL_FUNCTION_PARAM_PASSTHRU, "date", 4);
}
/* }}} */

/* {{{ public string/null Form::month(string $name[, string $id = NULL[, string $value = ""[, array $attributes = NULL]]]) */
static BUESSION_METHOD(helper_form, month){
	helper_form_normal_input_fn(INTERNAL_FUNCTION_PARAM_PASSTHRU, "month", 5);
}
/* }}} */

/* {{{ public string/null Form::week(string $name[, string $id = NULL[, string $value = ""[, array $attributes = NULL]]]) */
static BUESSION_METHOD(helper_form, week){
	helper_form_normal_input_fn(INTERNAL_FUNCTION_PARAM_PASSTHRU, "week", 4);
}
/* }}} */

/* {{{ public string/null Form::time(string $name[, string $id = NULL[, string $value = ""[, array $attributes = NULL]]]) */
static BUESSION_METHOD(helper_form, time){
	helper_form_normal_input_fn(INTERNAL_FUNCTION_PARAM_PASSTHRU, "time", 4);
}
/* }}} */

/* {{{ public string/null Form::datetime(string $name[, string $id = NULL[, string $value = ""[, array $attributes = NULL]]]) */
static BUESSION_METHOD(helper_form, datetime){
	helper_form_normal_input_fn(INTERNAL_FUNCTION_PARAM_PASSTHRU, "datetime", 8);
}
/* }}} */

/* {{{ public string/null Form::datetimeLocal(string $name[, string $id = NULL[, string $value = ""[, array $attributes = NULL]]]) */
static BUESSION_METHOD(helper_form, datetimeLocal){
	helper_form_normal_input_fn(INTERNAL_FUNCTION_PARAM_PASSTHRU, "datetime-local", 14);
}
/* }}} */

/* {{{ public string/null Form::search(string $name[, string $id = NULL[, string $value = ""[, array $attributes = NULL]]]) */
static BUESSION_METHOD(helper_form, search){
	helper_form_normal_input_fn(INTERNAL_FUNCTION_PARAM_PASSTHRU, "search", 6);
}
/* }}} */

/* {{{ public string/null Form::color(string $name[, string $id = NULL[, string $value = ""[, array $attributes = NULL]]]) */
static BUESSION_METHOD(helper_form, color){
	helper_form_normal_input_fn(INTERNAL_FUNCTION_PARAM_PASSTHRU, "color", 5);
}
/* }}} */

/* {{{ public string/null Form::hidden(string $name[, string $id = NULL[, string $value = ""[, array $attributes = NULL]]]) */
static BUESSION_METHOD(helper_form, hidden){
	helper_form_normal_input_fn(INTERNAL_FUNCTION_PARAM_PASSTHRU, "hidden", 6);
}
/* }}} */

/* {{{ public string/null Form::radio(string $name, string $value, string $text[, string $id = NULL[, array $attributes = NULL]]) */
static BUESSION_METHOD(helper_form, radio){
	helper_form_radio_checkbox_fn(INTERNAL_FUNCTION_PARAM_PASSTHRU, "radio", 5);
}
/* }}} */

/* {{{ public string/null Form::checkbox(string $name, string $value, string $text[, string $id = NULL[, array $attributes = NULL]]) */
static BUESSION_METHOD(helper_form, checkbox){
	helper_form_radio_checkbox_fn(INTERNAL_FUNCTION_PARAM_PASSTHRU, "checkbox", 8);
}
/* }}} */

/* {{{ public string/null Form::file(string $name[, array $attributes = NULL]) */
static BUESSION_METHOD(helper_form, file){
	char *name;
	uint name_length;
	HashTable *attributes = NULL;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|H", &name, &name_length, &attributes) == SUCCESS){
		smart_str str = {0};
		HashTable *attrs = NULL;

		helper_form_create_attributes_from_source(attrs, attributes);
		buession_hash_update_stringl_ex(attrs, "enctype", 8, ZEND_STRL("multipart/form-data"), TRUE TSRMLS_CC);

		helper_form_create_input(&str, "file", 4, NULL, 0, name, name_length, NULL, 0, attrs);
		buession_hash_free(attrs);

		HELPER_HTML_RETURN(str);
	}
}
/* }}} */

/* {{{ public string/null Form::output([string $name = NULL[, array $attributes = NULL]]) */
static BUESSION_METHOD(helper_form, output){
	char *name = NULL;
	uint name_length = 0;
	HashTable *attributes = NULL;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|sH", &name, &name_length, &attributes) == SUCCESS){
		smart_str str = {0};
		HashTable *attrs = NULL;

		if(attributes != NULL){
			helper_form_create_attributes_from_source(attrs, attributes);
			zend_hash_del(attrs, "name", 5);
		}

		smart_str_appendl(&str, "<output", sizeof("<output") - 1);
		if(name){
			smart_str_appendl(&str, " name=\"", sizeof(" name=\"") - 1);
			helper_html_append_encode_value(&str, name, name_length);
			smart_str_appendl(&str, "\"", 1);
		}

		if(attrs){
			helper_html_add_attributes(&str, attrs TSRMLS_CC);
		}

		smart_str_appendl(&str, "></output>", sizeof("></output>") - 1);
		buession_hash_free(attrs);

		HELPER_HTML_RETURN(str);
	}
}
/* }}} */

/* {{{ public string/null Form::keygen(string $name, string $keytype[, array $attributes = NULL]) */
static BUESSION_METHOD(helper_form, keygen){
	char *name;
	uint name_length;
	char *keytype;
	uint keytype_length;
	HashTable *attributes = NULL;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ss|H", &name, &name_length, &keytype, &keytype_length, &attributes) == SUCCESS){
		smart_str str = {0};
		HashTable *attrs = NULL;

		if(attributes != NULL){
			helper_form_create_attributes_from_source(attrs, attributes);
			zend_hash_del(attrs, "name", 5);
			zend_hash_del(attrs, "keytype", 8);
		}

		smart_str_appendl(&str, "<keygen name=\"", sizeof("<keygen name=\"") - 1);
		helper_html_append_encode_value(&str, name, name_length);
		smart_str_appendl(&str, "\" keytype=\"", sizeof("\" keytype=\"") - 1);
		helper_html_append_encode_value(&str, keytype, keytype_length);
		smart_str_appendl(&str, "\"", 1);

		if(attrs){
			helper_html_add_attributes(&str, attrs TSRMLS_CC);
		}

		smart_str_appendl(&str, " />", 3);
		buession_hash_free(attrs);

		HELPER_HTML_RETURN(str);
	}
}
/* }}} */

/* {{{ public string/null Form::datalist(string $id, array $data[, array $attributes = NULL]) */
static BUESSION_METHOD(helper_form, datalist){
	char *id;
	uint id_length;
	HashTable *data;
	HashTable *attributes = NULL;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sH|H", &id, &id_length, &data, &attributes) == SUCCESS){
		smart_str str = {0};
		HashTable *attrs = NULL;

		if(attributes != NULL){
			helper_form_create_attributes_from_source(attrs, attributes);
			zend_hash_del(attrs, "id", 3);
		}

		smart_str_appendl(&str, "<datalist id=\"", sizeof("<datalist id=\"") - 1);
		helper_html_append_encode_value(&str, id, id_length);
		smart_str_appendl(&str, "\"", 1);

		if(attrs){
			helper_html_add_attributes(&str, attrs TSRMLS_CC);
		}

		smart_str_appendl(&str, ">", 1);

		if(data){
			zend_hash_apply_with_arguments(data TSRMLS_CC, (apply_func_args_t) helper_html_create_option_apply, 1, &str);
		}

		smart_str_appendl(&str, "</datalist>", sizeof("</datalist>") - 1);

		buession_hash_free(attrs);

		HELPER_HTML_RETURN(str);
	}
}
/* }}} */

/* {{{ public string/null Form::select(string $id, array $data[boolean $multiple = FALSE[, integer $size = -1[, array $attributes = NULL]]]) */
static BUESSION_METHOD(helper_form, select){
	char *id;
	uint id_length;
	HashTable *data;
	zend_bool multiple = FALSE;
	int size = -1;
	HashTable *attributes = NULL;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sH|blH", &id, &id_length, &data, &multiple, &size, &attributes) == SUCCESS){
		smart_str str = {0};
		HashTable *attrs = NULL;
		ulong attrs_size = attributes ? zend_hash_num_elements(attributes) : 0;

		ALLOC_HASHTABLE(attrs);
		zend_hash_init(attrs, attrs_size, NULL, ZVAL_PTR_DTOR, FALSE);

		if(attributes != NULL){
			zend_hash_copy(attrs, attributes, (copy_ctor_func_t) zval_add_ref, NULL, sizeof(zval *));
			zend_hash_del(attrs, "id", 3);
			zend_hash_del(attrs, "multiple", 9);
			zend_hash_del(attrs, "size", 5);
		}

		smart_str_appendl(&str, "<select id=\"", sizeof("<select id=\"") - 1);
		helper_html_append_encode_value(&str, id, id_length);
		smart_str_appendl(&str, "\"", 1);

		if(multiple == TRUE){
			buession_hash_update_stringl_ex(attrs, "multiple", 9, "multiple", 8, TRUE TSRMLS_CC);
		}
		if(size >= 0){
			buession_hash_update_long_ex(attrs, "size", 5, size TSRMLS_CC);
		}

		helper_html_add_attributes(&str, attrs TSRMLS_CC);

		smart_str_appendl(&str, ">", 1);

		if(data){
			zend_hash_apply_with_arguments(data TSRMLS_CC, (apply_func_args_t) helper_html_create_option_apply, 1, &str);
		}

		smart_str_appendl(&str, "</select>", sizeof("</select>") - 1);

		buession_hash_free(attrs);

		HELPER_HTML_RETURN(str);
	}
}
/* }}} */

/* {{{ public string/null Form::option(string $value, string $text[, boolean $isSelected = FALSE[, array $attributes = NULL]]) */
static BUESSION_METHOD(helper_form, option){
	char *value;
	uint value_length;
	char *text;
	uint text_length;
	zend_bool isSelected = FALSE;
	HashTable *attributes = NULL;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ss|bH", &value, &value_length, &text, &text_length, &isSelected, &attributes) == SUCCESS){
		smart_str str = {0};
		HashTable *attrs = NULL;
		ulong attrs_size = attributes ? zend_hash_num_elements(attributes) : 0;

		ALLOC_HASHTABLE(attrs);
		zend_hash_init(attrs, attrs_size, NULL, ZVAL_PTR_DTOR, FALSE);

		if(attributes != NULL){
			zend_hash_copy(attrs, attributes, (copy_ctor_func_t) zval_add_ref, NULL, sizeof(zval *));
			zend_hash_del(attrs, "selected", 9);
		}

		if(isSelected == TRUE){
			buession_hash_update_stringl_ex(attrs, "selected", 9, "selected", 8, TRUE TSRMLS_CC);
		}

		helper_form_create_option(&str, value, value_length, text, text_length, attrs);
		buession_hash_free(attrs);

		HELPER_HTML_RETURN(str);
	}
}
/* }}} */

/* {{{ private void Form::__destruct() */
static BUESSION_METHOD(helper_form, __destruct){
}
/* }}} */

static zend_function_entry helper_form_methods[] = {
	BUESSION_HELPER_FORM_ME(__construct, NULL, ZEND_ACC_PRIVATE|ZEND_ACC_CTOR)
	BUESSION_HELPER_FORM_STATIC_ME(text, helper_form_text_arguments)
	BUESSION_HELPER_FORM_STATIC_ME(password, helper_form_password_arguments)
	BUESSION_HELPER_FORM_STATIC_ME(email, helper_form_email_arguments)
	BUESSION_HELPER_FORM_STATIC_ME(url, helper_form_url_arguments)
	BUESSION_HELPER_FORM_STATIC_ME(number, helper_form_number_arguments)
	BUESSION_HELPER_FORM_STATIC_ME(range, helper_form_range_arguments)
	BUESSION_HELPER_FORM_STATIC_ME(date, helper_form_date_arguments)
	BUESSION_HELPER_FORM_STATIC_ME(month, helper_form_month_arguments)
	BUESSION_HELPER_FORM_STATIC_ME(week, helper_form_week_arguments)
	BUESSION_HELPER_FORM_STATIC_ME(time, helper_form_time_arguments)
	BUESSION_HELPER_FORM_STATIC_ME(datetime, helper_form_datetime_arguments)
	BUESSION_HELPER_FORM_STATIC_ME(datetimeLocal, helper_form_datetimeLocal_arguments)
	BUESSION_HELPER_FORM_STATIC_ME(search, helper_form_search_arguments)
	BUESSION_HELPER_FORM_STATIC_ME(color, helper_form_color_arguments)
	BUESSION_HELPER_FORM_STATIC_ME(hidden, helper_form_hidden_arguments)
	BUESSION_HELPER_FORM_STATIC_ME(radio, helper_form_radio_arguments)
	BUESSION_HELPER_FORM_STATIC_ME(checkbox, helper_form_checkbox_arguments)
	BUESSION_HELPER_FORM_STATIC_ME(file, helper_form_file_arguments)
	BUESSION_HELPER_FORM_STATIC_ME(output, helper_form_output_arguments)
	BUESSION_HELPER_FORM_STATIC_ME(keygen, helper_form_keygen_arguments)
	BUESSION_HELPER_FORM_STATIC_ME(datalist, helper_form_datalist_arguments)
	BUESSION_HELPER_FORM_STATIC_ME(select, helper_form_select_arguments)
	BUESSION_HELPER_FORM_STATIC_ME(option, helper_form_option_arguments)
	BUESSION_HELPER_FORM_ME(__destruct, NULL, ZEND_ACC_PRIVATE|ZEND_ACC_DTOR)
	ZEND_FE_END
};

/** {{{ BUESSION_STARTUP_FUNCTION */
BUESSION_STARTUP_FUNCTION(helper_form){
	zend_class_entry ce;

	BUESSION_INIT_CLASS_ENTRY(ce, "Helper\\Form", helper_form_methods);
	helper_form_ce = zend_register_internal_class(&ce TSRMLS_CC);

	return SUCCESS;
}
/* }}} */

/** {{{ BUESSION_RINIT_FUNCTION */
BUESSION_RINIT_FUNCTION(helper_form){
	return SUCCESS;
}
/* }}} */

/** {{{ BUESSION_RSHUTDOWN_FUNCTION */
BUESSION_RSHUTDOWN_FUNCTION(helper_form){
	return SUCCESS;
}
/* }}} */

/** {{{ BUESSION_SHUTDOWN_FUNCTION */
BUESSION_SHUTDOWN_FUNCTION(helper_form){
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
