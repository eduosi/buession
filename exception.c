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
  | Author: Teng Yong	<webmaster@buession.com>                         |
  +----------------------------------------------------------------------+
*/

/* $Id: exception.c 310447 2012-04-10 21:32:51Z Teng Yong $ */

/* include php file */
#include "Zend/zend.h"
#include "Zend/zend_API.h"
#include "Zend/zend_builtin_functions.h"
#include "Zend/zend_exceptions.h"
#include "main/php.h"
#include "main/php_main.h"
#include "ext/spl/spl_exceptions.h"

/* include buession file */
#include "buession.h"
#include "exception.h"

zend_class_entry *exception_ce;
zend_class_entry *parseexception_ce;
zend_class_entry *invalidexception_ce;
zend_class_entry *illegalexception_ce;
zend_class_entry *indexoutofboundsexception_ce;
zend_class_entry *nullpointerexception_ce;
zend_class_entry *runtimeexception_ce;
zend_class_entry *outofmemoryexception_ce;
zend_class_entry *ioexception_ce;
zend_class_entry *unsupportedoperationexception_ce;
zend_class_entry *classnotfoundexception_ce;
zend_class_entry *clonenotsupportedexception_ce;
zend_class_entry *invalidpropertyexception_ce;
zend_class_entry *illegalargumentexception_ce;
zend_class_entry *nosuchmethodexception_ce;

/* {{{ arginfo */
ZEND_BEGIN_ARG_INFO_EX(exception___construct_arguments, FALSE, FALSE, 0)
	ZEND_ARG_INFO(FALSE, message)
	ZEND_ARG_INFO(FALSE, code)
	ZEND_ARG_OBJ_INFO(FALSE, previous, Exception, TRUE)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(parseexception___construct_arguments, FALSE, FALSE, 0)
	ZEND_ARG_INFO(FALSE, message)
	ZEND_ARG_INFO(FALSE, code)
	ZEND_ARG_OBJ_INFO(FALSE, previous, Exception, TRUE)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(invalidexception___construct_arguments, FALSE, FALSE, 0)
	ZEND_ARG_INFO(FALSE, message)
	ZEND_ARG_INFO(FALSE, code)
	ZEND_ARG_OBJ_INFO(FALSE, previous, Exception, TRUE)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(illegalexception___construct_arguments, FALSE, FALSE, 0)
	ZEND_ARG_INFO(FALSE, message)
	ZEND_ARG_INFO(FALSE, code)
	ZEND_ARG_OBJ_INFO(FALSE, previous, Exception, TRUE)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(indexoutofboundsexception__construct_arguments, FALSE, FALSE, 0)
	ZEND_ARG_INFO(FALSE, message)
	ZEND_ARG_INFO(FALSE, code)
	ZEND_ARG_OBJ_INFO(FALSE, previous, Exception, TRUE)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(nullpointerexception___construct_arguments, FALSE, FALSE, 0)
	ZEND_ARG_INFO(FALSE, message)
	ZEND_ARG_INFO(FALSE, code)
	ZEND_ARG_OBJ_INFO(FALSE, previous, Exception, TRUE)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(runtimeexception___construct_arguments,  FALSE, FALSE, 0)
	ZEND_ARG_INFO(FALSE, message)
	ZEND_ARG_INFO(FALSE, code)
	ZEND_ARG_OBJ_INFO(0, previous, Exception, TRUE)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(outofmemoryexception___construct_arguments, FALSE, FALSE, 0)
	ZEND_ARG_INFO(FALSE, message)
	ZEND_ARG_INFO(FALSE, code)
	ZEND_ARG_OBJ_INFO(0, previous, Exception, TRUE)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ioexception___construct_arguments, FALSE, FALSE, 0)
	ZEND_ARG_INFO(FALSE, message)
	ZEND_ARG_INFO(FALSE, code)
	ZEND_ARG_OBJ_INFO(FALSE, previous, Exception, TRUE)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(unsupportedoperationexception___construct_arguments, FALSE, FALSE, 0)
	ZEND_ARG_INFO(FALSE, message)
	ZEND_ARG_INFO(FALSE, code)
	ZEND_ARG_OBJ_INFO(FALSE, previous, Exception, TRUE)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(classnotfoundexception___construct_arguments, FALSE, FALSE, 0)
	ZEND_ARG_INFO(FALSE, message)
	ZEND_ARG_INFO(FALSE, code)
	ZEND_ARG_OBJ_INFO(FALSE, previous, Exception, TRUE)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(clonenotsupportedexception___construct_arguments, FALSE, FALSE, 0)
	ZEND_ARG_INFO(FALSE, message)
	ZEND_ARG_INFO(FALSE, code)
	ZEND_ARG_OBJ_INFO(FALSE, previous, Exception, TRUE)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(invalidpropertyexception___construct_arguments, FALSE, FALSE, 0)
	ZEND_ARG_INFO(FALSE, message)
	ZEND_ARG_INFO(FALSE, code)
	ZEND_ARG_OBJ_INFO(FALSE, previous, Exception, TRUE)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(illegalargumentexception___construct_arguments, FALSE, FALSE, 0)
	ZEND_ARG_INFO(FALSE, message)
	ZEND_ARG_INFO(FALSE, code)
	ZEND_ARG_OBJ_INFO(FALSE, previous, Exception, TRUE)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(nosuchmethodexception___construct_arguments, FALSE, FALSE, 0)
	ZEND_ARG_INFO(FALSE, message)
	ZEND_ARG_INFO(FALSE, code)
	ZEND_ARG_OBJ_INFO(FALSE, previous, Exception, TRUE)
ZEND_END_ARG_INFO()
/* }}} */

BUESSION_API zval *buession_throw_exception(zend_class_entry *exception_ce, int severity, char *message, uint message_length, long code){
	zval *exception = NULL;

	TSRMLS_FETCH();

	if(instanceof_function(exception_ce, zend_exception_get_default(TSRMLS_C) TSRMLS_CC) == FALSE){
		zval *pexception = buession_throw_exception_error(illegalexception_ce, E_ERROR, ZEND_STRL("Exceptions must be derived from the Exception base class."), 0);
		zval_ptr_dtor(&pexception);
	}

	exception_throw_object_init(exception_ce, instance, severit, code, message, message_length);
	zend_throw_exception_object(exception TSRMLS_CC);

	return exception;
}
BUESSION_API zval *buession_throw_exception_ex(zend_class_entry *exception_ce, int severity, char *format, long code, ...){
	va_list arg;
	smart_str message = {0};
	zval *exception = NULL;

	TSRMLS_FETCH();

	if(format != NULL){
		char *_message = NULL;
		uint _message_length = 0;

		va_start(arg, code);
		_message_length = vspprintf(&_message, 0, format, arg);
		smart_str_appendl(&message, _message, _message_length);
		buession_free(_message);
		va_end(arg);
	}

	smart_str_0(&message);
	exception = buession_throw_exception(exception_ce, severity, message.c, message.len, code);
	smart_str_free(&message);

	return exception;
}

BUESSION_API zval *buession_throw_exception_error(zend_class_entry *exception_ce, int severity, char *message, uint message_length, long code){
	zval *exception = NULL;

	TSRMLS_FETCH();

	if(instanceof_function(exception_ce, zend_exception_get_default(TSRMLS_C) TSRMLS_CC) == FALSE){
		zval *pexception = buession_throw_exception_error(illegalexception_ce, E_ERROR, ZEND_STRL("Exceptions must be derived from the Exception base class."), 0);
		zval_ptr_dtor(&pexception);
	}

	exception_throw_object_init(exception_ce, instance, severit, code, message, message_length);
	zend_exception_error(exception, severity TSRMLS_CC);

	return exception;
}
BUESSION_API zval *buession_throw_exception_error_ex(zend_class_entry *exception_ce, int severity, char *format, long code, ...){
	va_list arg;
	smart_str message = {0};
	zval *exception = NULL;

	TSRMLS_FETCH();

	if(format != NULL){
		char *_message = NULL;
		uint _message_length = 0;

		va_start(arg, code);
		_message_length = vspprintf(&_message, 0, format, arg);
		smart_str_appendl(&message, _message, _message_length);
		buession_free(_message);
		va_end(arg);
	}

	smart_str_0(&message);
	exception = buession_throw_exception_error(exception_ce, severity, message.c, message.len, code);
	smart_str_free(&message);

	return exception;
}

/* {{{ public void Exception::__construct([string $message = ""[, int $code = 0[, Exception $previous = NULL]]]) */
static BUESSION_METHOD(exception, __construct){
	exception__construct(exception_ce, NULL, 0, 0);
}
/* }}} */

/* {{{ public void Exception::__destruct() */
static BUESSION_METHOD(exception, __destruct){
}
/* }}} */

/* {{{ public void ParseException::__construct([string $message = ""[, int $code = 0[, Exception $previous = NULL]]]) */
static BUESSION_METHOD(parseexception, __construct){
	exception__construct(parseexception_ce, NULL, 0, 0);
}
/* }}} */

/* {{{ public void ParseException::__destruct() */
static BUESSION_METHOD(parseexception, __destruct){
}
/* }}} */

/* {{{ public void InvalidException::__construct([string $message = ""[, int $code = 0[, Exception $previous = NULL]]]) */
static BUESSION_METHOD(invalidexception, __construct){
	exception__construct(invalidexception_ce, NULL, 0, 0);
}
/* }}} */

/* {{{ public void InvalidException::__destruct() */
static BUESSION_METHOD(invalidexception, __destruct){
}
/* }}} */

/* {{{ public void IllegalException::__construct([string $message = ""[, int $code = 0[, Exception $previous = NULL]]]) */
static BUESSION_METHOD(illegalexception, __construct){
	exception__construct(illegalexception_ce, NULL, 0, 0);
}
/* }}} */

/* {{{ public void IllegalException::__destruct() */
static BUESSION_METHOD(illegalexception, __destruct){
}
/* }}} */

/* {{{ public void IndexOutOfBoundsException::__construct([string $message = ""[, int $code = 0[, Exception $previous = NULL]]]) */
static BUESSION_METHOD(indexoutofboundsexception, __construct){
	exception__construct(indexoutofboundsexception_ce, NULL, 0, 0);
}
/* }}} */

/* {{{ public void IndexOutOfBoundsException::__destruct() */
static BUESSION_METHOD(indexoutofboundsexception, __destruct){
}
/* }}} */

/* {{{ public void NullPointerException::__construct([string $message = ""[, int $code = 0[, Exception $previous = NULL]]]) */
static BUESSION_METHOD(nullpointerexception, __construct){
	exception__construct(nullpointerexception_ce, NULL, 0, 0);
}
/* }}} */

/* {{{ public void NullPointerException::__destruct() */
static BUESSION_METHOD(nullpointerexception, __destruct){
}
/* }}} */

/* {{{ public void RuntimeException::__construct([string $message = ""[, int $code = 0[, Exception $previous = NULL]]]) */
static BUESSION_METHOD(runtimeexception, __construct){
	exception__construct(runtimeexception_ce, NULL, 0, 0);
}
/* }}} */

/* {{{ public void RuntimeException::__destruct() */
static BUESSION_METHOD(runtimeexception, __destruct){
}
/* }}} */

/* {{{ public void OutOfMemoryException::__construct([string $message = ""[, int $code = 0[, Exception $previous = NULL]]]) */
static BUESSION_METHOD(outofmemoryexception, __construct){
	exception__construct(outofmemoryexception_ce, "out of memory", 13, 500);
}
/* }}} */

/* {{{ public void OutOfMemoryException::__destruct() */
static BUESSION_METHOD(outofmemoryexception, __destruct){
}
/* }}} */

/* {{{ public void IOException::__construct([string $message = ""[, int $code = 0[, Exception $previous = NULL]]]) */
static BUESSION_METHOD(ioexception, __construct){
	exception__construct(ioexception_ce, NULL, 0, 500);
}
/* }}} */

/* {{{ public void IOException::__destruct() */
static BUESSION_METHOD(ioexception, __destruct){
}
/* }}} */

/* {{{ public void UnsupportedOperationException::__construct([string $message = ""[, int $code = 0[, Exception $previous = NULL]]]) */
static BUESSION_METHOD(unsupportedoperationexception, __construct){
	exception__construct(unsupportedoperationexception_ce, NULL, 0, 500);
}
/* }}} */

/* {{{ public void UnsupportedOperationException::__destruct() */
static BUESSION_METHOD(unsupportedoperationexception, __destruct){
}
/* }}} */

/* {{{ public void ClassNotFoundException::__construct([string $message = ""[, int $code = 0[, Exception $previous = NULL]]]) */
static BUESSION_METHOD(classnotfoundexception, __construct){
	char *message = NULL;
	uint message_length = 0;
	int code = 0;
	zval *previous = NULL;
	zend_class_entry *exception_ce = zend_exception_get_default(TSRMLS_C);

	if(zend_parse_parameters_ex(ZEND_PARSE_PARAMS_QUIET, ZEND_NUM_ARGS() TSRMLS_CC, "|slO!", &message, &message_length, &code, &previous, exception_ce) == FAILURE){
		exception__construct_argument_exception(classnotfoundexception_ce);
	}

	if(message == NULL){
		zval traces;

		zend_fetch_debug_backtrace(&traces, 1, 0 TSRMLS_CC);
		if(Z_TYPE(traces) == IS_ARRAY){
			zval **last_trace;
			zval **classname = NULL;

			if(zend_hash_index_find(Z_ARRVAL(traces), zend_hash_num_elements(Z_ARRVAL(traces)) - 1, (void **) &last_trace) == SUCCESS){
				zend_hash_find(Z_ARRVAL_PP(last_trace), "class", 6, (void **) &classname);
			}

			if(classname == NULL){
				goto default_message;
			}else{
				char *_message = NULL;
				uint _message_length = spprintf(&_message, 0, "class '%s' no found", classname == NULL ? "" : Z_STRVAL_PP(classname));

				if(_message){
					exception_update_memsage_property(classnotfoundexception_ce, _message, _message_length);
					buession_free(_message);
				}
			}
		}else{
			default_message:
			exception_update_memsage_property(classnotfoundexception_ce, "class no found", sizeof("class no found") - 1);
		}
	}else{
		if(message){
			exception_update_memsage_property(classnotfoundexception_ce, message, message_length);
		}
	}

	if(code){
		zend_update_property_long(classnotfoundexception_ce, getThis(), ZEND_STRL(EXCEPTION_PROPERTY_CODE), code TSRMLS_CC);
	}

	if(previous){
		zend_update_property(classnotfoundexception_ce, getThis(), ZEND_STRL(EXCEPTION_PROPERTY_PREVIOUS), previous TSRMLS_CC);
	}
}
/* }}} */

/* {{{ public void ClassNotFoundException::__destruct() */
static BUESSION_METHOD(classnotfoundexception, __destruct){
}
/* }}} */

/* {{{ public void CloneNotSupportedException::__construct([string $message = ""[, int $code = 0[, Exception $previous = NULL]]]) */
static BUESSION_METHOD(clonenotsupportedexception, __construct){
	exception__construct(clonenotsupportedexception_ce, NULL, 0, 0);
}
/* }}} */

/* {{{ public void CloneNotSupportedException::__destruct() */
static BUESSION_METHOD(clonenotsupportedexception, __destruct){
}
/* }}} */

/* {{{ public void InvalidPropertyException::__construct([string $message = ""[, int $code = 0[, Exception $previous = NULL]]]) */
static BUESSION_METHOD(invalidpropertyexception, __construct){
	exception__construct(invalidpropertyexception_ce, NULL, 0, 0);
}
/* }}} */

/* {{{ public void InvalidPropertyException::__destruct() */
static BUESSION_METHOD(invalidpropertyexception, __destruct){
}
/* }}} */

/* {{{ public void IllegalArgumentException::__construct([string $message = ""[, int $code = 0[, Exception $previous = NULL]]]) */
static BUESSION_METHOD(illegalargumentexception, __construct){
	exception__construct(illegalargumentexception_ce, NULL, 0, 0);
}
/* }}} */

/* {{{ public void IllegalArgumentException::__destruct() */
static BUESSION_METHOD(illegalargumentexception, __destruct){
}
/* }}} */

/* {{{ public void NoSuchMethodException::__construct([string $message = ""[, int $code = 0[, Exception $previous = NULL]]]) */
static BUESSION_METHOD(nosuchmethodexception, __construct){
	char *message = NULL;
	uint message_length = 0;
	int code = 0;
	zval *previous = NULL;
	zend_class_entry *exception_ce = zend_exception_get_default(TSRMLS_C);

	if(zend_parse_parameters_ex(ZEND_PARSE_PARAMS_QUIET, ZEND_NUM_ARGS() TSRMLS_CC, "|slO!", &message, &message_length, &code, &previous, exception_ce) == FAILURE){
		exception__construct_argument_exception(nosuchmethodexception_ce);
	}

	if(message == NULL){
		zval traces;

		zend_fetch_debug_backtrace(&traces, 1, 0 TSRMLS_CC);
		if(Z_TYPE(traces) == IS_ARRAY){
			zval **last_trace;
			zval **classname = NULL;
			zval **method = NULL;

			if(zend_hash_index_find(Z_ARRVAL(traces), zend_hash_num_elements(Z_ARRVAL(traces)) - 1, (void **) &last_trace) == SUCCESS){
				zend_hash_find(Z_ARRVAL_PP(last_trace), "class", 6, (void **) &classname);
				zend_hash_find(Z_ARRVAL_PP(last_trace), "function", 9, (void **) &method);
			}

			if(classname == NULL||method == NULL){
				goto default_message;
			}else{
				char *_message = NULL;
				uint _message_length = spprintf(&_message, 0, "class %s method %s no found", classname == NULL ? "" : Z_STRVAL_PP(classname), method == NULL ? "" : Z_STRVAL_PP(method));

				if(_message){
					exception_update_memsage_property(nosuchmethodexception_ce, _message, _message_length);
					buession_free(_message);
				}
			}
		}else{
			default_message:
			exception_update_memsage_property(nosuchmethodexception_ce, "class's method no found", sizeof("class's method no found") - 1);
		}
	}else{
		if(message){
			exception_update_memsage_property(nosuchmethodexception_ce, message, message_length);
		}
	}

	if(code){
		zend_update_property_long(nosuchmethodexception_ce, getThis(), ZEND_STRL(EXCEPTION_PROPERTY_CODE), code TSRMLS_CC);
	}

	if(previous){
		zend_update_property(nosuchmethodexception_ce, getThis(), ZEND_STRL(EXCEPTION_PROPERTY_PREVIOUS), previous TSRMLS_CC);
	}
}
/* }}} */

/* {{{ public void NoSuchMethodException::__destruct() */
static BUESSION_METHOD(nosuchmethodexception, __destruct){
}
/* }}} */

static zend_function_entry exception_methods[] = {
	BUESSION_EXCEPTION_ME(__construct, exception___construct_arguments, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	BUESSION_EXCEPTION_ME(__destruct, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_DTOR)
	ZEND_FE_END
};

static zend_function_entry parseexception_methods[] = {
	BUESSION_PARSREXCEPTION_ME(__construct, parseexception___construct_arguments, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	BUESSION_PARSREXCEPTION_ME(__destruct, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_DTOR)
	ZEND_FE_END
};

static zend_function_entry invalidexception_methods[] = {
	BUESSION_INVALIDEXCEPTION_ME(__construct, invalidexception___construct_arguments, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	BUESSION_INVALIDEXCEPTION_ME(__destruct, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_DTOR)
	ZEND_FE_END
};

static zend_function_entry illegalexception_methods[] = {
	BUESSION_ILLEGALEXCEPTION_ME(__construct, illegalexception___construct_arguments, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	BUESSION_ILLEGALEXCEPTION_ME(__destruct, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_DTOR)
	ZEND_FE_END
};

static zend_function_entry indexoutofboundsexception_methods[] = {
	BUESSION_INDEXOUTOFBOUNDSEXCEPTION_ME(__construct, indexoutofboundsexception__construct_arguments, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	BUESSION_INDEXOUTOFBOUNDSEXCEPTION_ME(__destruct, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_DTOR)
	ZEND_FE_END
};

static zend_function_entry nullpointerexception_methods[] = {
	BUESSION_NULLPOINTEREXCEPTION_ME(__construct, nullpointerexception___construct_arguments, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	BUESSION_NULLPOINTEREXCEPTION_ME(__destruct, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_DTOR)
	ZEND_FE_END
};

static zend_function_entry runtimeexception_methods[] = {
	BUESSION_RUNTIMEEXCEPTION_ME(__construct, runtimeexception___construct_arguments, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	BUESSION_RUNTIMEEXCEPTION_ME(__destruct, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_DTOR)
	ZEND_FE_END
};

static zend_function_entry outofmemoryexception_methods[] = {
	BUESSION_OUTOFMEMORYEXCEPTION_ME(__construct, outofmemoryexception___construct_arguments, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	BUESSION_OUTOFMEMORYEXCEPTION_ME(__destruct, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_DTOR)
	ZEND_FE_END
};

static zend_function_entry ioexception_methods[] = {
	BUESSION_IOEXCEPTION_ME(__construct, ioexception___construct_arguments, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	BUESSION_IOEXCEPTION_ME(__destruct, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_DTOR)
	ZEND_FE_END
};

static zend_function_entry unsupportedoperationexception_methods[] = {
	BUESSION_UNSUPPORTEDOPERATIONEXCEPTION_ME(__construct, unsupportedoperationexception___construct_arguments, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	BUESSION_UNSUPPORTEDOPERATIONEXCEPTION_ME(__destruct, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_DTOR)
	ZEND_FE_END
};

static zend_function_entry classnotfoundexception_methods[] = {
	BUESSION_CLASSFOUNDEXCEPTION_ME(__construct, classnotfoundexception___construct_arguments, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	BUESSION_CLASSFOUNDEXCEPTION_ME(__destruct, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_DTOR)
	ZEND_FE_END
};

static zend_function_entry clonenotsupportedexception_methods[] = {
	BUESSION_CLONENOTSUPPORTEDEXCEPTION_ME(__construct, clonenotsupportedexception___construct_arguments, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	BUESSION_CLONENOTSUPPORTEDEXCEPTION_ME(__destruct, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_DTOR)
	ZEND_FE_END
};

static zend_function_entry invalidpropertyexception_methods[] = {
	BUESSION_INVALIDPROPERTYEXCEPTION_ME(__construct, invalidpropertyexception___construct_arguments, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	BUESSION_INVALIDPROPERTYEXCEPTION_ME(__destruct, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_DTOR)
	ZEND_FE_END
};

static zend_function_entry illegalargumentexception_methods[] = {
	BUESSION_ILLEGALARGUMENTEXCEPTION_ME(__construct, illegalargumentexception___construct_arguments, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	BUESSION_ILLEGALARGUMENTEXCEPTION_ME(__destruct, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_DTOR)
	ZEND_FE_END
};

static zend_function_entry nosuchmethodexception_methods[] = {
	BUESSION_NOSUCHMETHODEXCEPTION_ME(__construct, nosuchmethodexception___construct_arguments, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	BUESSION_NOSUCHMETHODEXCEPTION_ME(__destruct, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_DTOR)
	ZEND_FE_END
};

/** {{{ BUESSION_STARTUP_FUNCTION */
BUESSION_STARTUP_FUNCTION(exception){
	zend_class_entry ce;

	BUESSION_INIT_CLASS_ENTRY(ce, "Exception", exception_methods);
	exception_ce = zend_register_internal_class_ex(&ce, zend_exception_get_default(TSRMLS_C), NULL TSRMLS_CC);

	BUESSION_INIT_CLASS_ENTRY(ce, "ParseException", parseexception_methods);
	parseexception_ce = zend_register_internal_class_ex(&ce, exception_ce, NULL TSRMLS_CC);

	BUESSION_INIT_CLASS_ENTRY(ce, "InvalidException", invalidexception_methods);
	invalidexception_ce = zend_register_internal_class_ex(&ce, exception_ce, NULL TSRMLS_CC);

	BUESSION_INIT_CLASS_ENTRY(ce, "IllegalException", illegalexception_methods);
	illegalexception_ce = zend_register_internal_class_ex(&ce, exception_ce, NULL TSRMLS_CC);

	BUESSION_INIT_CLASS_ENTRY(ce, "IndexOutOfBoundsException", indexoutofboundsexception_methods);
	indexoutofboundsexception_ce = zend_register_internal_class_ex(&ce, spl_ce_OutOfBoundsException, NULL TSRMLS_CC);

	BUESSION_INIT_CLASS_ENTRY(ce, "NullPointerException", nullpointerexception_methods);
	nullpointerexception_ce = zend_register_internal_class_ex(&ce, illegalexception_ce, NULL TSRMLS_CC);

	BUESSION_INIT_CLASS_ENTRY(ce, "RuntimeException", runtimeexception_methods);
	runtimeexception_ce = zend_register_internal_class_ex(&ce, spl_ce_RuntimeException, NULL TSRMLS_CC);

	BUESSION_INIT_CLASS_ENTRY(ce, "OutOfMemoryException", outofmemoryexception_methods);
	outofmemoryexception_ce = zend_register_internal_class_ex(&ce, exception_ce, NULL TSRMLS_CC);

	BUESSION_INIT_CLASS_ENTRY(ce, "IOException", ioexception_methods);
	ioexception_ce = zend_register_internal_class_ex(&ce, exception_ce, NULL TSRMLS_CC);

	BUESSION_INIT_CLASS_ENTRY(ce, "UnsupportedOperationException", unsupportedoperationexception_methods);
	unsupportedoperationexception_ce = zend_register_internal_class_ex(&ce, exception_ce, NULL TSRMLS_CC);

	BUESSION_INIT_CLASS_ENTRY(ce, "ClassNotFoundException", classnotfoundexception_methods);
	classnotfoundexception_ce = zend_register_internal_class_ex(&ce, exception_ce, NULL TSRMLS_CC);

	BUESSION_INIT_CLASS_ENTRY(ce, "CloneNotSupportedException", clonenotsupportedexception_methods);
	clonenotsupportedexception_ce = zend_register_internal_class_ex(&ce, illegalexception_ce, NULL TSRMLS_CC);

	BUESSION_INIT_CLASS_ENTRY(ce, "InvalidPropertyException", invalidpropertyexception_methods);
	invalidpropertyexception_ce = zend_register_internal_class_ex(&ce, spl_ce_UnexpectedValueException, NULL TSRMLS_CC);

	BUESSION_INIT_CLASS_ENTRY(ce, "IllegalArgumentException", illegalargumentexception_methods);
	illegalargumentexception_ce = zend_register_internal_class_ex(&ce, spl_ce_InvalidArgumentException, NULL TSRMLS_CC);

	BUESSION_INIT_CLASS_ENTRY(ce, "NoSuchMethodException", nosuchmethodexception_methods);
	nosuchmethodexception_ce = zend_register_internal_class_ex(&ce, spl_ce_BadMethodCallException, NULL TSRMLS_CC);

	return SUCCESS;
}
/* }}} */

/** {{{ BUESSION_RINIT_FUNCTION */
BUESSION_RINIT_FUNCTION(exception){
	return SUCCESS;
}
/* }}} */

/** {{{ BUESSION_RSHUTDOWN_FUNCTION */
BUESSION_RSHUTDOWN_FUNCTION(exception){
	return SUCCESS;
}
/* }}} */

/** {{{ BUESSION_SHUTDOWN_FUNCTION */
BUESSION_SHUTDOWN_FUNCTION(exception){
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