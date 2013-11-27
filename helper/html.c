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

/* $Id: helper/html.c 310447 2013-10-11 12:16:32Z Yong.Teng $ */

/* include php file */
#include "Zend/zend.h"
#include "Zend/zend_API.h"
#include "main/php.h"
#include "main/php_main.h"
#include "ext/standard/html.h"

/* include buession file */
#include "buession.h"
#include "html.h"
#include "helper.h"

zend_class_entry *helper_html_ce;

/* {{{ arginfo */
ZEND_BEGIN_ARG_INFO_EX(helper_html_DOCTYPE_arguments, FALSE, FALSE, 0)
	ZEND_ARG_INFO(FALSE, doctype)
	ZEND_ARG_INFO(FALSE, dtd)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(helper_html_html_arguments, FALSE, FALSE, 0)
	ZEND_ARG_INFO(FALSE, doctype)
	ZEND_ARG_INFO(FALSE, lang)
	ZEND_ARG_ARRAY_INFO(FALSE, attributes, TRUE)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(helper_html_head_arguments, FALSE, FALSE, 0)
	ZEND_ARG_INFO(FALSE, content)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(helper_html_meta_arguments, FALSE, FALSE, 2)
	ZEND_ARG_INFO(FALSE, equiv)
	ZEND_ARG_INFO(FALSE, value)
	ZEND_ARG_INFO(FALSE, mode)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(helper_html_title_arguments, FALSE, FALSE, 0)
	ZEND_ARG_INFO(FALSE, title)
	ZEND_ARG_INFO(FALSE, encode)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(helper_html_body_arguments, FALSE, FALSE, 0)
	ZEND_ARG_INFO(FALSE, content)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(helper_html_img_arguments, FALSE, FALSE, 1)
	ZEND_ARG_INFO(FALSE, src)
	ZEND_ARG_ARRAY_INFO(FALSE, attributes, TRUE)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(helper_html_javascript_arguments, FALSE, FALSE, 0)
	ZEND_ARG_INFO(FALSE, src)
	ZEND_ARG_INFO(FALSE, code)
	ZEND_ARG_INFO(FALSE, isSelected)
	ZEND_ARG_ARRAY_INFO(FALSE, attributes, TRUE)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(helper_html___callStatic_arguments, FALSE, FALSE, 1)
	ZEND_ARG_INFO(FALSE, method)
	ZEND_ARG_ARRAY_INFO(FALSE, arguments, TRUE)
ZEND_END_ARG_INFO()
/* }}} */

static inline int helper_html___callStatic_attributes_apply(zval **value, HashTable *attributes TSRMLS_DC){
	if(Z_TYPE_PP(value) == IS_ARRAY){
		zend_hash_copy(attributes, Z_ARRVAL_PP(value), (copy_ctor_func_t) zval_add_ref, NULL, sizeof(zval *));
	}else if(Z_TYPE_PP(value) == IS_OBJECT){
		HashTable *attrs = buession_zval_convert_to_hash(*value TSRMLS_CC);

		if(attrs != NULL){
			zend_hash_copy(attributes, attrs, (copy_ctor_func_t) zval_add_ref, NULL, sizeof(zval *));
		}
	}

	return ZEND_HASH_APPLY_KEEP;
}

static inline int helper_html_add_attribute(zval **value TSRMLS_DC, int num_args, va_list args, zend_hash_key *hashkey){
	smart_str *str = va_arg(args, smart_str *);
	zend_bool isEvent = FALSE;

	if(hashkey->nKeyLength > 0){
		if(hashkey->nKeyLength == sizeof("content")&&memcmp(hashkey->arKey, "content", 7) == 0){
			return ZEND_HASH_APPLY_KEEP;
		}else{
			char *key = NULL;
			uint key_length = hashkey->nKeyLength - 1;

			key = zend_str_tolower_dup(hashkey->arKey, key_length);

			smart_str_appendl(str, " ", 1);
			smart_str_appendl(str, hashkey->arKey, key_length);

			isEvent = (hashkey->nKeyLength == sizeof("onabort")&&memcmp(key, "onabort", key_length) == 0)
						||(hashkey->nKeyLength == sizeof("onblur")&&memcmp(key, "onblur", key_length) == 0)
						||(hashkey->nKeyLength == sizeof("onchange")&&memcmp(key, "onchange", key_length) == 0)
						||(hashkey->nKeyLength == sizeof("onclick")&&memcmp(key, "onclick", key_length) == 0)
						||(hashkey->nKeyLength == sizeof("ondblclick")&&memcmp(key, "ondblclick", key_length) == 0)
						||(hashkey->nKeyLength == sizeof("onerror")&&memcmp(key, "onerror", key_length) == 0)
						||(hashkey->nKeyLength == sizeof("onfocus")&&memcmp(key, "onfocus", key_length) == 0)
						||(hashkey->nKeyLength == sizeof("onkeydown")&&memcmp(key, "onkeydown", key_length) == 0)
						||(hashkey->nKeyLength == sizeof("onkeypress")&&memcmp(key, "onkeypress", key_length) == 0)
						||(hashkey->nKeyLength == sizeof("onkeyup")&&memcmp(key, "onkeyup", key_length) == 0)
						||(hashkey->nKeyLength == sizeof("onload")&&memcmp(key, "onload", key_length) == 0)
						||(hashkey->nKeyLength == sizeof("onmousedown")&&memcmp(key, "onmousedown", key_length) == 0)
						||(hashkey->nKeyLength == sizeof("onmousemove")&&memcmp(key, "onmousemove", key_length) == 0)
						||(hashkey->nKeyLength == sizeof("onmouseout")&&memcmp(key, "onmouseout", key_length) == 0)
						||(hashkey->nKeyLength == sizeof("onmouseover")&&memcmp(key, "onmouseover", key_length) == 0)
						||(hashkey->nKeyLength == sizeof("onmouseup")&&memcmp(key, "onmouseup", key_length) == 0)
						||(hashkey->nKeyLength == sizeof("onreset")&&memcmp(key, "onreset", key_length) == 0)
						||(hashkey->nKeyLength == sizeof("onresize")&&memcmp(key, "onresize", key_length) == 0)
						||(hashkey->nKeyLength == sizeof("onselect")&&memcmp(key, "onselect", key_length) == 0)
						||(hashkey->nKeyLength == sizeof("onsubmit")&&memcmp(key, "onsubmit", key_length) == 0)
						||(hashkey->nKeyLength == sizeof("onunload")&&memcmp(key, "onunload", key_length) == 0);
			buession_free(key);
		}
	}else{
		smart_str_appendl(str, " ", 1);
		smart_str_append_long(str, (long) hashkey->h);
	}

	smart_str_appendl(str, "=\"", 2);

	switch(Z_TYPE_PP(value)){
		case IS_STRING:
			if(Z_STRVAL_PP(value) > 0){
				if(isEvent){
					smart_str_appendl(str, Z_STRVAL_PP(value), Z_STRLEN_PP(value));
				}else{
					helper_html_append_encode_value(str, Z_STRVAL_PP(value), Z_STRLEN_PP(value));
				}
			}
			break;
		case IS_LONG:
			smart_str_append_long(str, Z_LVAL_PP(value));
			break;
		case IS_DOUBLE:
			smart_str_append_double(str, Z_DVAL_PP(value));
			break;
		case IS_BOOL:
			if(Z_BVAL_PP(value) == TRUE){
				smart_str_appendl(str, "true", 4);
			}else{
				smart_str_appendl(str, "false", 5);
			}
			break;
		case IS_ARRAY:
			smart_str_appendl(str, "Array", 5);
			break;
		case IS_OBJECT: {
				zval temp;

				BUESSION_CONVERT_VARIABLE_TO_STRING(*value, temp);
				if(Z_STRVAL_PP(value) > 0){
					if(isEvent){
						smart_str_appendl(str, Z_STRVAL_PP(value), Z_STRLEN_PP(value));
					}else{
						helper_html_append_encode_value(str, Z_STRVAL_PP(value), Z_STRLEN_PP(value));
					}
				}
				BUESSION_CONVERT_VARIABLE_PTR_DTOR(*value, temp);
			}
			break;
		case IS_NULL:
			if(hashkey->nKeyLength > 0){
				smart_str_appendl(str, hashkey->arKey, hashkey->nKeyLength - 1);
			}else{
				smart_str_append_long(str, (long) hashkey->h);
			}
			break;
		default:
			break;
	}

	smart_str_appendl(str, "\"", 1);

	return ZEND_HASH_APPLY_KEEP;
}

BUESSION_API void helper_html_add_attributes(smart_str *str, HashTable *attributes TSRMLS_DC){
	if(attributes&&zend_hash_num_elements(attributes) > 0){
		zend_hash_apply_with_arguments(attributes TSRMLS_CC, (apply_func_args_t) helper_html_add_attribute, 1, str);
	}
}

/* {{{ private void Html::__construct() */
static BUESSION_METHOD(helper_html, __construct){
}
/* }}} */

/* {{{ public string/null Html::DOCTYPE([string $doctype = "xhtml"[, string $dtd = "transitional"]]) */
static BUESSION_METHOD(helper_html, DOCTYPE){
	char *doctype = "xhtml";
	uint doctype_length = 5;
	char *dtd = "transitional";
	uint dtd_length = 12;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|ss", &doctype, &doctype_length, &dtd, &dtd_length) == SUCCESS){
		smart_str str = {0};

		smart_str_appendl(&str, "<!DOCTYPE ", sizeof("<!DOCTYPE ") - 1);
		if(doctype_length == 5&&strncasecmp(doctype, "xhtml", 5) == 0){
			smart_str_appendl(&str, "html PUBLIC \"", sizeof("html PUBLIC \"") - 1);
			if((dtd_length == 12&&strncasecmp(dtd, "transitional", 12) == 0)||(dtd_length == 6&&strncasecmp(dtd, "strict", 6) == 0)){
				char *ldtd = zend_str_tolower_dup(dtd, dtd_length);

				*ldtd = toupper((int) *ldtd);
				smart_str_appendl(&str, "-//W3C//DTD XHTML 1.0 ", sizeof("-//W3C//DTD XHTML 1.0 ") - 1);
				smart_str_appendl(&str, ldtd, dtd_length);
				smart_str_appendl(&str, "//EN\" \"http://", sizeof("//EN\" \"http://") - 1);
				smart_str_appendl(&str, HELPER_HTML_W3C_DOMAIN, sizeof(HELPER_HTML_W3C_DOMAIN) - 1);
				smart_str_appendl(&str, "/TR/xhtml1/DTD/xhtml1-", sizeof("/TR/xhtml1/DTD/xhtml1-") - 1);
				smart_str_appendl(&str, dtd, dtd_length);
				buession_free(ldtd);
			}else if(dtd_length == 6&&strncasecmp(dtd, "mobile", 6) == 0){
				smart_str_appendl(&str, "-//WAPFORUM//DTD XHTML Mobile 1.0//EN\" \"http://", sizeof("-//WAPFORUM//DTD XHTML Mobile 1.0//EN\" \"http://") - 1);
				smart_str_appendl(&str, HELPER_HTML_WAPFORM_DOMAIN, sizeof(HELPER_HTML_WAPFORM_DOMAIN) - 1);
				smart_str_appendl(&str, "/DTD/xhtml-mobile10", sizeof("/DTD/xhtml-mobile10") - 1);
			}else{
				smart_str_appendl(&str, "-//W3C//DTD XHTML 1.1//EN\" \"http://", sizeof("-//W3C//DTD XHTML 1.1//EN\" \"http://") - 1);
				smart_str_appendl(&str, HELPER_HTML_W3C_DOMAIN, sizeof(HELPER_HTML_W3C_DOMAIN) - 1);
				smart_str_appendl(&str, "/TR/xhtml11/DTD/xhtml11", sizeof("/TR/xhtml11/DTD/xhtml11") - 1);
			}
			smart_str_appendl(&str, ".dtd\"", sizeof(".dtd\"") - 1);
		}else if(doctype_length == 4&&strncasecmp(doctype, "html", 4) == 0){
			if(dtd_length == 12&&strncasecmp(dtd, "transitional", 12) == 0){
				smart_str_appendl(&str, "html PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\" \"http://", sizeof("html PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\" \"http://") - 1);
				smart_str_appendl(&str, HELPER_HTML_W3C_DOMAIN, sizeof(HELPER_HTML_W3C_DOMAIN) - 1);
				smart_str_appendl(&str, "/TR/html4/loose.dtd\"", sizeof("/TR/html4/loose.dtd\"") - 1);
			}else if(dtd_length == 6&&strncasecmp(dtd, "strict", 6) == 0){
				smart_str_appendl(&str, "html PUBLIC \"-//W3C//DTD HTML 4.01//EN\" \"http://", sizeof("html PUBLIC \"-//W3C//DTD HTML 4.01//EN\" \"http://") - 1);
				smart_str_appendl(&str, HELPER_HTML_W3C_DOMAIN, sizeof(HELPER_HTML_W3C_DOMAIN) - 1);
				smart_str_appendl(&str, "/TR/html4/strict.dtd\"", sizeof("/TR/html4/strict.dtd\"") - 1);
			}
		}else if(doctype_length == 5&&strncasecmp(doctype, "html5", 5) == 0){
			smart_str_appendl(&str, "HTML", 4);
		}else if(doctype_length == 3&&strncasecmp(doctype, "wml", 3) == 0){
			smart_str_appendl(&str, "wml \"-//WAPFORUM//DTD WML 1.1//EN\" \"http://", sizeof("wml \"-//WAPFORUM//DTD WML 1.1//EN\" \"http://") - 1);
			smart_str_appendl(&str, HELPER_HTML_WAPFORM_DOMAIN, sizeof(HELPER_HTML_WAPFORM_DOMAIN) - 1);
			smart_str_appendl(&str, "/DTD/wml_1.1.xml\"", sizeof("/DTD/wml_1.1.xml\"") - 1);
		}
		smart_str_appendl(&str, ">", 1);

		HELPER_HTML_RETURN(str);
	}
}
/* }}} */

/* {{{ public string/null Html::html([string $doctype = "xhtml"[, string $lang = NULL[, array $attributes = NULL]]]) */
static BUESSION_METHOD(helper_html, html){
	char *doctype = "xhtml";
	uint doctype_length = 5;
	char *lang = NULL;
	uint lang_length = 0;
	HashTable *attributes = NULL;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|ssH", &doctype, &doctype_length, &lang, &lang_length, &attributes) == SUCCESS){
		smart_str str = {0};

		smart_str_appendl(&str, "<html", 5);
		if((doctype_length == 5&&strncasecmp(doctype, "xhtml", 5) == 0)||(doctype_length == 3&&strncasecmp(doctype, "wml", 3) == 0)){
			smart_str_appendl(&str, " xmlns=\"http://", sizeof(" http://") - 1);
			smart_str_appendl(&str, HELPER_HTML_W3C_DOMAIN, sizeof(HELPER_HTML_W3C_DOMAIN) - 1);
			smart_str_appendl(&str, "/1999/xhtml\"", sizeof("/1999/xhtml\"") - 1);
		}

		if(attributes != NULL){
			helper_html_add_attributes(&str, attributes TSRMLS_CC);
		}

		if(lang&&lang_length){
			smart_str_appendl(&str, " xml:lang=\"", sizeof(" xml:lang=\"") - 1);
			smart_str_appendl(&str, lang, lang_length);
			smart_str_appendl(&str, "\"", 1);
			smart_str_appendl(&str, " lang=\"", sizeof(" lang=\"") - 1);
			smart_str_appendl(&str, lang, lang_length);
			smart_str_appendl(&str, "\"", 1);
		}

		smart_str_appendl(&str, ">", 1);

		HELPER_HTML_RETURN(str);
	}
}
/* }}} */

/* {{{ public string/null Html::head([string $content = NULL]) */
static BUESSION_METHOD(helper_html, head){
	char *content = NULL;
	uint content_length = 0;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|s", &content, &content_length) == SUCCESS){
		smart_str str = {0};

		smart_str_appendl(&str, "<head>", sizeof("<head>") - 1);
		if(content&&content_length > 0){
			smart_str_appendl(&str, content, content_length);
		}
		smart_str_appendl(&str, "</head>", sizeof("</head>") - 1);

		HELPER_HTML_RETURN(str);
	}
}
/* }}} */

/* {{{ public string/null Html::meta(string $equiv, string $value[, string $mode = "equiv"]) */
static BUESSION_METHOD(helper_html, meta){
	char *equiv;
	uint equiv_length;
	char *value;
	uint value_length;
	char *mode = "equiv";
	uint mode_length = 5;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ss|s", &equiv, &equiv_length, &value, &value_length, &mode, &mode_length) == SUCCESS){
		smart_str str = {0};

		smart_str_appendl(&str, "<meta", 5);
		if(mode_length == 5&&strncasecmp(mode, "equiv", 5) == 0){
			smart_str_appendl(&str, "http-equiv=\"", sizeof("http-equiv=\"") - 1);
		}else{
			smart_str_appendl(&str, mode, mode_length);
			smart_str_appendl(&str, "=\"", 2);
		}
		smart_str_appendl(&str, equiv, equiv_length);
		smart_str_appendl(&str, "\" content=\"", sizeof("\" content=\"") - 1);
		smart_str_appendl(&str, value, value_length);
		smart_str_appendl(&str, "\">", 2);

		HELPER_HTML_RETURN(str);
	}
}
/* }}} */

/* {{{ public string/null Html::title([string $title = NULL[, boolean $encode = TRUE]]) */
static BUESSION_METHOD(helper_html, title){
	char *title = NULL;
	uint title_length = 0;
	zend_bool encode = TRUE;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|sb", &title, &title_length, &encode) == SUCCESS){
		smart_str str = {0};

		smart_str_appendl(&str, "<title>", sizeof("<title>") - 1);
		if(title&&title_length > 0){
			if(encode == TRUE){
				helper_html_append_encode_value(&str, title, title_length);
			}else{
				smart_str_appendl(&str, title, title_length);
			}
		}
		smart_str_appendl(&str, "</title>", sizeof("</title>") - 1);

		HELPER_HTML_RETURN(str);
	}
}
/* }}} */

/* {{{ public string/null Html::body([string $content = NULL]) */
static BUESSION_METHOD(helper_html, body){
	char *content = NULL;
	uint content_length = 0;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|s", &content, &content_length) == SUCCESS){
		smart_str str = {0};

		smart_str_appendl(&str, "<body>", sizeof("<body>") - 1);
		if(content&&content_length > 0){
			smart_str_appendl(&str, content, content_length);
		}
		smart_str_appendl(&str, "</body>", sizeof("</body>") - 1);

		HELPER_HTML_RETURN(str);
	}
}
/* }}} */

/* {{{ public string/null Html::img(string $src[, array $attributes = NULL]) */
static BUESSION_METHOD(helper_html, img){
	char *src = NULL;
	uint src_length = 0;
	HashTable *attributes = NULL;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|H", &src, &src_length, &attributes) == SUCCESS){
		smart_str str = {0};

		smart_str_appendl(&str, "<img src=\"", sizeof("<img src=\"") - 1);
		if(src&&src_length > 0){
			smart_str_appendl(&str, src, src_length);
		}
		smart_str_appendl(&str, "\"", 1);
		helper_html_add_attributes(&str, attributes TSRMLS_CC);
		smart_str_appendl(&str, " />", 3);

		HELPER_HTML_RETURN(str);
	}
}
/* }}} */

/* {{{ public string/null Html::javascript([string $src = NULL[, string $code = NULL[, array $attributes = NULL]]]) */
static BUESSION_METHOD(helper_html, javascript){
	char *src = NULL;
	uint src_length = 0;
	char *code = NULL;
	uint code_length = 0;
	HashTable *attributes = NULL;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|ssH", &src, &src_length, &code, &code_length, &attributes) == SUCCESS){
		smart_str str = {0};
		HashTable *attrs = NULL;

		ALLOC_HASHTABLE(attrs);
		zend_hash_init(attrs, attributes ? zend_hash_num_elements(attributes) : 0, NULL, ZVAL_PTR_DTOR, FALSE);

		if(attributes != NULL){
			zend_hash_copy(attrs, attributes, (copy_ctor_func_t) zval_add_ref, NULL, sizeof(zval *));
			zend_hash_del(attrs, "src", 4);
			zend_hash_del(attrs, "type", 5);
		}

		smart_str_appendl(&str, "<script", sizeof("<script") - 1);

		if(src&&src_length > 0){
			buession_hash_update_stringl_ex(attrs, "src", 4, src, src_length, TRUE TSRMLS_CC);
		}

		helper_html_add_attributes(&str, attrs TSRMLS_CC);
		smart_str_appendl(&str, " type=\"text/javascript\">", sizeof(" type=\"text/javascript\">") - 1);
		buession_hash_free(attrs);

		if((!src||src_length == 0)&&code&&code_length > 0){
			smart_str_appendl(&str, code, code_length);
		}

		smart_str_appendl(&str, "</script>", sizeof("</script>") - 1);

		HELPER_HTML_RETURN(str);
	}
}
/* }}} */

/* {{{ public string/null Html::__callStatic(string $method[, array $attributes = array()]) */
static BUESSION_METHOD(helper_html, __callStatic){
	char *method;
	uint method_length;
	HashTable *attributes = NULL;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|H", &method, &method_length, &attributes) == SUCCESS){
		smart_str str = {0};
		char *lmethod = zend_str_tolower_dup(method, method_length);
		HashTable *attrs = NULL;

		smart_str_appendl(&str, "<", 1);
		smart_str_appendl(&str, lmethod, method_length);

		if(attributes != NULL){
			ALLOC_HASHTABLE(attrs);
			zend_hash_init(attrs, zend_hash_num_elements(attributes), NULL, ZVAL_PTR_DTOR, FALSE);
			zend_hash_apply_with_argument(attributes, (apply_func_arg_t) helper_html___callStatic_attributes_apply, attrs TSRMLS_CC);
			helper_html_add_attributes(&str, attrs TSRMLS_CC);
		}

		if((method_length == 2&&(memcmp(lmethod, "br", 2) == 0||memcmp(lmethod, "hr", 2) == 0))
			||(method_length == 3&&(memcmp(lmethod, "col", 3) == 0))
			||(method_length == 4&&(memcmp(lmethod, "area", 4) == 0||memcmp(lmethod, "base", 4) == 0))
			||(method_length == 5&&(memcmp(lmethod, "embed", 5) == 0||memcmp(lmethod, "track", 5) == 0))
			||(method_length == 6&&memcmp(lmethod, "source", 6) == 0)
			||(method_length == 8&&memcmp(lmethod, "colgroup", 8) == 0)){
			smart_str_appendl(&str, " />", 3);
		}else{
			smart_str_appendl(&str, ">", 1);

			if(attrs != NULL){
				zval **content;

				if(zend_hash_find(attrs, ZEND_STRS("content"), (void **) &content) == SUCCESS){
					zval temp;

					BUESSION_CONVERT_VARIABLE_TO_STRING(*content, temp);
					if(Z_STRVAL_PP(content) > 0){
						smart_str_appendl(&str, Z_STRVAL_PP(content), Z_STRLEN_PP(content));
					}
					BUESSION_CONVERT_VARIABLE_PTR_DTOR(*content, temp);
				}
			}

			smart_str_appendl(&str, "<", 1);
			smart_str_appendl(&str, lmethod, method_length);
			smart_str_appendl(&str, ">", 1);
		}
		buession_free(lmethod);
		buession_hash_free(attrs);

		HELPER_HTML_RETURN(str);
	}
}
/* }}} */

/* {{{ private void Html::__destruct() */
static BUESSION_METHOD(helper_html, __destruct){
}
/* }}} */

static zend_function_entry helper_html_methods[] = {
	BUESSION_HELPER_HTML_ME(__construct, NULL, ZEND_ACC_PRIVATE|ZEND_ACC_CTOR)
	BUESSION_HELPER_HTML_STATIC_ME(DOCTYPE, helper_html_DOCTYPE_arguments)
	BUESSION_HELPER_HTML_STATIC_ME(html, helper_html_html_arguments)
	BUESSION_HELPER_HTML_STATIC_ME(head, helper_html_head_arguments)
	BUESSION_HELPER_HTML_STATIC_ME(meta, helper_html_meta_arguments)
	BUESSION_HELPER_HTML_STATIC_ME(title, helper_html_title_arguments)
	BUESSION_HELPER_HTML_STATIC_ME(body, helper_html_body_arguments)
	BUESSION_HELPER_HTML_STATIC_ME(img, helper_html_img_arguments)
	BUESSION_HELPER_HTML_STATIC_ME(javascript, helper_html_javascript_arguments)
	BUESSION_HELPER_HTML_STATIC_ME(__callStatic, helper_html___callStatic_arguments)
	BUESSION_HELPER_HTML_ME(__destruct, NULL, ZEND_ACC_PRIVATE|ZEND_ACC_DTOR)
	ZEND_FE_END
};

/** {{{ BUESSION_STARTUP_FUNCTION */
BUESSION_STARTUP_FUNCTION(helper_html){
	zend_class_entry ce;

	BUESSION_INIT_CLASS_ENTRY(ce, "Helper\\Html", helper_html_methods);
	helper_html_ce = zend_register_internal_class(&ce TSRMLS_CC);

	return SUCCESS;
}
/* }}} */

/** {{{ BUESSION_RINIT_FUNCTION */
BUESSION_RINIT_FUNCTION(helper_html){
	return SUCCESS;
}
/* }}} */

/** {{{ BUESSION_RSHUTDOWN_FUNCTION */
BUESSION_RSHUTDOWN_FUNCTION(helper_html){
	return SUCCESS;
}
/* }}} */

/** {{{ BUESSION_SHUTDOWN_FUNCTION */
BUESSION_SHUTDOWN_FUNCTION(helper_html){
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
