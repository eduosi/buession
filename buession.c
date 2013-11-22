/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2013 The PHP Group                                |
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

/* $Id: buession.c 310447 2011-04-23 21:14:10Z Yong.Teng $ */

#ifdef HAVE_CONFIG_H
	#include "config.h"
#endif

/* include php file */
#include "Zend/zend.h"
#include "Zend/zend_API.h"
#include "Zend/zend_modules.h"
#include "main/php.h"
#include "main/php_main.h"
#include "main/SAPI.h"
#include "ext/standard/info.h"

/* include buession file */
#include "buession.h"
#include "assert/assert.h"
#include "calendar/calendar.h"
#include "registry/registry.h"
#include "validate/validate.h"

ZEND_DECLARE_MODULE_GLOBALS(buession)

/* PHP versions lower than 5.3.0 are not compatible or are causing tests to FAIL */
#if ((PHP_MAJOR_VERSION < 5)||(PHP_MAJOR_VERSION == 5&&PHP_MINOR_VERSION < 3))
	#error PHP 5.3.0 or newer is required. Please upgrade your php version.
#endif

zend_class_entry *buession_ce;

/* {{{ arginfo */
ZEND_BEGIN_ARG_INFO_EX(buession_getInstance_arguments, FALSE, FALSE, 1)
	ZEND_ARG_INFO(FALSE, package)
	ZEND_ARG_INFO(FALSE, ...)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(buession_loadClass_arguments, FALSE, FALSE, 1)
	ZEND_ARG_INFO(FALSE, package)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(buession_getAddress_arguments, FALSE, FALSE, 1)
	ZEND_ARG_INFO(TRUE, var)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(buession___callStatic_arguments, FALSE, FALSE, 1)
	ZEND_ARG_INFO(FALSE, method)
	ZEND_ARG_ARRAY_INFO(FALSE, arguments, FALSE)
ZEND_END_ARG_INFO()
/* }}} */

static ZEND_INI_MH(OnUpdateEnvironment){
	BUESSION_G(environment_length) = new_value_length;
	OnUpdateString(entry, new_value, new_value_length, mh_arg1, mh_arg2, mh_arg3, stage TSRMLS_CC);

	return SUCCESS;
}

static ZEND_INI_MH(OnUpdateClasspath){
	BUESSION_G(classpath_length) = new_value_length;
	OnUpdateString(entry, new_value, new_value_length, mh_arg1, mh_arg2, mh_arg3, stage TSRMLS_CC);

	return SUCCESS;
}

static void buession_destroy_globals(zend_buession_globals *buession_globals TSRMLS_DC){
}

/* {{{ ZEND_INI */
ZEND_INI_BEGIN()
	STD_ZEND_INI_ENTRY("buession.environment", NULL, ZEND_INI_ALL, OnUpdateEnvironment, environment, zend_buession_globals, buession_globals)
	STD_ZEND_INI_ENTRY("buession.classpath", NULL, ZEND_INI_ALL, OnUpdateClasspath, classpath, zend_buession_globals, buession_globals)
ZEND_INI_END()
/* }}} */

BUESSION_API zend_bool buession_instance_exists(zend_class_entry *ce TSRMLS_DC){
	char *lname = zend_str_tolower_dup(ce->name, ce->name_length);
	zend_bool result = zend_hash_exists(BUESSION_G(registry).instances, lname, ce->name_length + 1);

	buession_free(lname);

	return result;
}
BUESSION_API int buession_instance_add(zend_class_entry *ce, zval *instance TSRMLS_DC){
	int result = FAILURE;

	if(instance&&Z_TYPE_P(instance) == IS_OBJECT){
		char *lname = zend_str_tolower_dup(ce->name, ce->name_length);

		result = zend_hash_add(BUESSION_G(registry).instances, lname, ce->name_length + 1, (void *) &instance, sizeof(zval *), NULL);
		buession_free(lname);
	}

	return result;
}
BUESSION_API zval *buession_instance_find(zend_class_entry *ce TSRMLS_DC){
	return buession_instance_find_ex(ce->name, ce->name_length TSRMLS_CC);
}
BUESSION_API zval *buession_instance_find_ex(const char *classname, uint classname_length TSRMLS_DC){
	char *lclassname = zend_str_tolower_dup(classname, classname_length);
	zval **instance;

	if(zend_hash_find(BUESSION_G(registry).instances, lclassname, classname_length + 1, (void **) &instance) == SUCCESS&&Z_TYPE_PP(instance) == IS_OBJECT){
		Z_ADDREF_PP(instance);
		buession_free(lclassname);

		return *instance;
	}
	buession_free(lclassname);

	return NULL;
}

/* {{{ private void Buession::__construct() */
static ZEND_METHOD(buession, __construct){
}
/* }}} */

/* {{{ public object/null Buession::getInstance(string $package[, mixed ...]) */
static ZEND_METHOD(buession, getInstance){
	char *package;
	uint package_length;
	zval ***args = NULL;
	uint argc = 0;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|+", &package, &package_length, &args, &argc) == SUCCESS){
		char *classname = NULL;
		uint classname_length = 0;
		zend_class_entry **ce;
		zval *instance = NULL;
		zend_bool is_copy = TRUE;

		if(package_length == 0){
			zval *exception = buession_throw_exception_error(illegalargumentexception_ce, E_WARNING, ZEND_STRL("Invalid package, could not empty"), 0);

			zval_ptr_dtor(&exception);
			RETURN_FALSE;
		}

		classname = buession_get_package_classname(package, package_length, &classname_length TSRMLS_CC);
		if(argc == 0){
			if((instance = buession_instance_find_ex(classname, classname_length TSRMLS_CC)) != NULL){
				//LOGGER_INFO("get '%s' instance from cache", package);
				buession_free(classname);
				goto return_value;
			}
		}

		if(zend_lookup_class_ex(classname, classname_length, FALSE, &ce TSRMLS_CC) == SUCCESS){
			buession_free(classname);
			instance = buession_getInstance(*ce, NULL, argc, args TSRMLS_CC);
			//LOGGER_INFO("get '%s' instance from internal", package);
		}else{
			if(buession_loadClass(package, package_length TSRMLS_CC) == SUCCESS){
				if(zend_lookup_class_ex(classname, classname_length, FALSE, &ce TSRMLS_CC) == SUCCESS){
					buession_free(classname);
					instance = buession_getInstance(*ce, NULL, argc, args TSRMLS_CC);
					//LOGGER_INFO("get '%s' instance from file", package);
				}
			}
		}

		buession_free(classname);
		if(instance&&Z_TYPE_P(instance) == IS_OBJECT){
			if(argc == 0){
				buession_instance_add(*ce, instance TSRMLS_CC);
			}else{
				is_copy = FALSE;
			}

			return_value:
			if(return_value_used){
				RETURN_ZVAL(instance, is_copy, FALSE);
			}

			return;
		}
	}

	buession_class_undefined_exception(package);

	RETURN_NULL();
}
/* }}} */

/* {{{ public boolean Buession::loadClass(string $package) */
static ZEND_METHOD(buession, loadClass){
	char *package;
	uint package_length;

	if(zend_parse_parameters(1 TSRMLS_CC, "s", &package, &package_length) == SUCCESS){
		RETURN_BOOL(buession_loadClass(package, package_length TSRMLS_CC) == SUCCESS);
	}

	RETURN_FALSE;
}
/* }}} */

/* {{{ public string/null Buession::getAddress(mixed $var) */
static ZEND_METHOD(buession, getAddress){
	zval *var;

	if(zend_parse_parameters(1 TSRMLS_CC, "z", &var) == SUCCESS){
		if(return_value_used){
			Z_TYPE_P(return_value) = IS_STRING;
			Z_STRLEN_P(return_value) = spprintf(&Z_STRVAL_P(return_value), 0, "%p", var);
			return;
		}
	}

	RETURN_NULL();
}
/* }}} */

/* {{{ public string Buession::getVersion() */
static ZEND_METHOD(buession, getVersion){
	BUESSION_RETURN_STRINGL(BUESSION_VERSION, sizeof(BUESSION_VERSION) - 1, TRUE);
}
/* }}} */

/* {{{ public integer Buession::getMajorVersion() */
static ZEND_METHOD(buession, getMajorVersion){
	RETURN_LONG(BUESSION_MAJOR_VERSION);
}
/* }}} */

/* {{{ public integer Buession::getMinorVersion() */
static ZEND_METHOD(buession, getMinorVersion){
	RETURN_LONG(BUESSION_MINOR_VERSION);
}
/* }}} */

/* {{{ public integer Buession::getReleaseVersion() */
static ZEND_METHOD(buession, getReleaseVersion){
	RETURN_LONG(BUESSION_RELEASE_VERSION);
}
/* }}} */

/* {{{ public string Buession::getExtraVersion() */
static ZEND_METHOD(buession, getExtraVersion){
	BUESSION_RETURN_STRINGL(BUESSION_EXTRA_VERSION, sizeof(BUESSION_EXTRA_VERSION) - 1, TRUE);
}
/* }}} */

/* {{{ public string Buession::getBuild() */
static ZEND_METHOD(buession, getBuild){
	BUESSION_RETURN_STRINGL(BUESSION_BUILD, sizeof(BUESSION_BUILD) - 1, TRUE);
}
/* }}} */

/* {{{ public string Buession::getApiVersion() */
static ZEND_METHOD(buession, getApiVersion){
	BUESSION_RETURN_STRINGL(BUESSION_API_VERSION, sizeof(BUESSION_API_VERSION) - 1, TRUE);
}
/* }}} */

/* {{{ public string Buession::getRevisionID() */
static ZEND_METHOD(buession, getRevisionID){
	BUESSION_RETURN_STRINGL(BUESSION_REVISION_ID, sizeof(BUESSION_REVISION_ID) - 1, TRUE);
}
/* }}} */

/* {{{ public mixed Buession::__callStatic(string $method[, array $arguments = array()]) */
static ZEND_METHOD(buession, __callStatic){
	ZEND_METHOD_EMPTY_CALLSTATIC_ENTRY();
}
/* }}} */

/* {{{ private void Buession::__destruct() */
static ZEND_METHOD(buession, __destruct){
}
/* }}} */

static zend_function_entry buession_methods[] = {
	BUESSION_ME(__construct, NULL, ZEND_ACC_PRIVATE|ZEND_ACC_CTOR)
	BUESSION_STATIC_ME(getInstance, buession_getInstance_arguments)
	BUESSION_STATIC_ME(loadClass, buession_loadClass_arguments)
	BUESSION_STATIC_ME(getAddress, buession_getAddress_arguments)
	BUESSION_STATIC_ME(getVersion, NULL)
	BUESSION_STATIC_ME(getMajorVersion, NULL)
	BUESSION_STATIC_ME(getMinorVersion, NULL)
	BUESSION_STATIC_ME(getReleaseVersion, NULL)
	BUESSION_STATIC_ME(getExtraVersion, NULL)
	BUESSION_STATIC_ME(getBuild, NULL)
	BUESSION_STATIC_ME(getApiVersion, NULL)
	BUESSION_STATIC_ME(getRevisionID, NULL)
	BUESSION_STATIC_ME(__callStatic, buession___callStatic_arguments)
	BUESSION_ME(__destruct, NULL, ZEND_ACC_PRIVATE|ZEND_ACC_DTOR)
	ZEND_FE_END
};

/* {{{ buession_functions */
const zend_function_entry buession_functions[] = {
	ZEND_FE_END
};
/* }}} */

/** {{{ ZEND_MINIT_FUNCTION */
ZEND_MINIT_FUNCTION(buession){
	zend_class_entry ce;

	INIT_CLASS_ENTRY(ce, "Buession", buession_methods);
	buession_ce = zend_register_internal_class(&ce TSRMLS_CC);
	buession_ce->ce_flags |= ZEND_ACC_FINAL_CLASS;

	zend_declare_class_constant_stringl(buession_ce, "NAME", 4, ZEND_STRL(BUESSION) TSRMLS_CC);
	zend_declare_class_constant_stringl(buession_ce, "VERSION", 7, ZEND_STRL(BUESSION_VERSION) TSRMLS_CC);
	zend_declare_class_constant_long(buession_ce, "MAJOR_VERSION", 13, BUESSION_MAJOR_VERSION TSRMLS_CC);
	zend_declare_class_constant_long(buession_ce, "MINOR_VERSION", 13, BUESSION_MINOR_VERSION TSRMLS_CC);
	zend_declare_class_constant_long(buession_ce, "RELEASE_VERSION", 15, BUESSION_RELEASE_VERSION TSRMLS_CC);
	zend_declare_class_constant_stringl(buession_ce, "EXTRA_VERSION", 13, ZEND_STRL(BUESSION_EXTRA_VERSION) TSRMLS_CC);
	zend_declare_class_constant_stringl(buession_ce, "BUILD", 5, ZEND_STRL(BUESSION_BUILD) TSRMLS_CC);
	zend_declare_class_constant_stringl(buession_ce, "API_VERSION", 11, ZEND_STRL(BUESSION_API_VERSION) TSRMLS_CC);
	zend_declare_class_constant_stringl(buession_ce, "REVISION_ID", 11, ZEND_STRL(BUESSION_REVISION_ID) TSRMLS_CC);

	REGISTER_INI_ENTRIES();

	zend_register_auto_global("_GLOBALS", 8, NULL TSRMLS_CC);

	BUESSION_STARTUP(constant);
	BUESSION_STARTUP(exception);

	BUESSION_STARTUP(assert);
	BUESSION_STARTUP(calendar);
	BUESSION_STARTUP(registry);
	BUESSION_STARTUP(validate);

	return SUCCESS;
}
/* }}} */

/** {{{ ZEND_RINIT_FUNCTION */
ZEND_RINIT_FUNCTION(buession){
	zval *globals;
	smart_str *root = {0};

	BUESSION_G(timestamp) = SG(global_request_time);
	BUESSION_G(clientip) = buession_get_client_ip(&BUESSION_G(clientip_length) TSRMLS_CC);
	BUESSION_G(scheme) = buession_get_scheme(&BUESSION_G(scheme_length) TSRMLS_CC);

	if(BUESSION_G(environment) == NULL&&(BUESSION_G(environment) = INI_ORIG_STR("runtime.environment")) != NULL){
		BUESSION_G(environment_length) = strlen(BUESSION_G(environment));
	}

	BUESSION_G(isHttp) = (BUESSION_G(scheme_length) == 4&&memcmp(BUESSION_G(scheme), "http", 4) == 0)
			||(BUESSION_G(scheme_length) == 5&&memcmp(BUESSION_G(scheme), "https", 5) == 0);
	if(BUESSION_G(isHttp) == TRUE){
		sapi_header_line ctr = {0};

		if((BUESSION_G(webroot) = sapi_getenv(ZEND_STRL("DOCUMENT_ROOT") TSRMLS_CC)) != NULL){
			char *root;

			BUESSION_G(webroot_length) = strlen(BUESSION_G(webroot));
			root = estrndup(BUESSION_G(webroot), BUESSION_G(webroot_length));
			zend_dirname(root, BUESSION_G(webroot_length));
			BUESSION_G(root_length) = spprintf(&BUESSION_G(root), 0, "%s%c", root, DEFAULT_SLASH);
			buession_free(root);
		}

		ctr.line_len = spprintf(&(ctr.line), 0, "X-Powered-By: %s", BUESSION);
		sapi_header_op(SAPI_HEADER_REPLACE, &ctr TSRMLS_CC);
		buession_free(ctr.line);

		if(BUESSION_G(root)){
			BUESSION_G(classpath_length) = spprintf(&BUESSION_G(classpath), 0, "%s%cFramework%cclasses%c", BUESSION_G(root), DEFAULT_SLASH, DEFAULT_SLASH, DEFAULT_SLASH);
		}else{
			goto default_classpath;
		}

		BUESSION_G(secure) = buession_is_ssl((char *) BUESSION_G(scheme), BUESSION_G(scheme_length) TSRMLS_CC);
	}else{
		default_classpath:
		BUESSION_G(classpath_length) = spprintf(&BUESSION_G(classpath), 0, ".%c", DEFAULT_SLASH);
	}

	if(!BUESSION_G(charset)||BUESSION_G(charset_length) == 0){
		if(SG(default_charset)&&(BUESSION_G(charset_length) = strlen(SG(default_charset))) > 0){
			BUESSION_G(charset) = estrndup(SG(default_charset), BUESSION_G(charset_length));
		}else{
			BUESSION_G(charset) = estrndup("UTF-8", 5);
			BUESSION_G(charset_length) = 5;
		}
	}

	BUESSION_ARRAY_SIZE_INIT(globals, 0);
	ZEND_SET_GLOBAL_VAR_WITH_LENGTH("_GLOBALS", 9, globals, 1, TRUE);

	BUESSION_RSTARTUP(constant);
	BUESSION_RSTARTUP(exception);

	BUESSION_RSTARTUP(assert);
	BUESSION_RSTARTUP(calendar);
	BUESSION_RSTARTUP(registry);
	BUESSION_RSTARTUP(validate);

	return SUCCESS;
}
/* }}} */

/** {{{ ZEND_RSHUTDOWN_FUNCTION */
ZEND_RSHUTDOWN_FUNCTION(buession){
	BUESSION_RSHUTDOWN(constant);
	BUESSION_RSHUTDOWN(exception);

	BUESSION_RSHUTDOWN(assert);
	BUESSION_RSHUTDOWN(calendar);
	BUESSION_RSHUTDOWN(registry);
	BUESSION_RSHUTDOWN(validate);

	BUESSION_CLEAN_STRING_G(environment);
	BUESSION_CLEAN_STRING_G(classpath);
	BUESSION_CLEAN_STRING_G(clientip);
	BUESSION_CLEAN_STRING_G(scheme);
	BUESSION_CLEAN_STRING_G(root);
	BUESSION_CLEAN_STRING_G(webroot);
	BUESSION_CLEAN_STRING_G(charset);

	zend_hash_clean(BUESSION_G(registry).registry);
	zend_hash_clean(BUESSION_G(registry).instances);

	return SUCCESS;
}
/* }}} */

/** {{{ ZEND_MSHUTDOWN_FUNCTION */
ZEND_MSHUTDOWN_FUNCTION(buession){
	UNREGISTER_INI_ENTRIES();

	BUESSION_SHUTDOWN(constant);
	BUESSION_SHUTDOWN(exception);

	BUESSION_SHUTDOWN(assert);
	BUESSION_SHUTDOWN(calendar);
	BUESSION_SHUTDOWN(registry);
	BUESSION_SHUTDOWN(validate);

	zend_hash_destroy(BUESSION_G(registry).registry);
	zend_hash_destroy(BUESSION_G(registry).instances);

	#ifdef ZTS
		ts_free_id(buession_globals_id);
	#else
		buession_destroy_globals(&buession_globals TSRMLS_CC);
	#endif

	return SUCCESS;
}
/* }}} */

/** {{{ ZEND_MINFO_FUNCTION */
ZEND_MINFO_FUNCTION(buession){
	php_info_print_table_start();
	php_info_print_table_header(2, "buession support", "enabled");
	php_info_print_table_row(2, "Name", BUESSION);
	php_info_print_table_row(2, "Version", BUESSION_VERSION);
	if(BUESSION_BUILD[0] != '\0'){
		php_info_print_table_row(2, "Build", BUESSION_BUILD);
	}
	php_info_print_table_row(2, "API Version", BUESSION_API_VERSION);
	php_info_print_table_row(2, "Revision Id", BUESSION_REVISION_ID);
	php_info_print_table_row(2, "Support", BUESSION_SUPPORT_URL);
	php_info_print_table_row(2, "Support e-mail", BUESSION_SUPPORT_EMAIL);
	php_info_print_table_end();

	DISPLAY_INI_ENTRIES();
}
/* }}} */

/** {{{ ZEND_GINIT_FUNCTION */
ZEND_GINIT_FUNCTION(buession){
	buession_globals->timestamp = 0;
	buession_globals->environment = NULL;
	buession_globals->environment_length = 0;
	buession_globals->classpath = NULL;
	buession_globals->classpath_length = 0;
	buession_globals->clientip = NULL;
	buession_globals->clientip_length = 0;
	buession_globals->scheme = NULL;
	buession_globals->scheme_length = 0;
	buession_globals->isHttp = FALSE;
	buession_globals->root = NULL;
	buession_globals->root_length = 0;
	buession_globals->webroot = NULL;
	buession_globals->webroot_length = 0;
	buession_globals->charset = NULL;
	buession_globals->charset_length = 0;
	buession_globals->secure = FALSE;

	ALLOC_HASHTABLE(buession_globals->registry.registry);
	ALLOC_HASHTABLE(buession_globals->registry.instances);
	zend_hash_init(buession_globals->registry.registry, 0, NULL, ZVAL_PTR_DTOR, FALSE);
	zend_hash_init(buession_globals->registry.instances, 0, NULL, ZVAL_PTR_DTOR, FALSE);
}
/* }}} */

/* {{{ module depends */
zend_module_dep buession_deps[] = {
	ZEND_MOD_REQUIRED("spl")
	ZEND_MOD_REQUIRED("pcre")
	ZEND_MOD_OPTIONAL("math")
	ZEND_MOD_OPTIONAL("session")
	{NULL, NULL, NULL}
};
/* }}} */

#ifdef COMPILE_DL_BUESSION
	ZEND_GET_MODULE(buession)
#endif

/* {{{ buession_module_entry */
zend_module_entry buession_module_entry = {
	STANDARD_MODULE_HEADER_EX,
	NULL,
	buession_deps,
	BUESSION,
	buession_functions,
	ZEND_MODULE_STARTUP_N(buession),
	ZEND_MODULE_SHUTDOWN_N(buession),
	ZEND_MODULE_ACTIVATE_N(buession),
	ZEND_MODULE_DEACTIVATE_N(buession),
	ZEND_MINFO(buession),
	BUESSION_VERSION,
	ZEND_MODULE_GLOBALS(buession),
	ZEND_GINIT(buession),
	NULL,
	NULL,
	STANDARD_MODULE_PROPERTIES_EX
};
/* }}} */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */