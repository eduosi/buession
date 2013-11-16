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

/* $Id: exception.h 310447 2012-04-10 21:32:18Z Teng Yong $ */

#ifndef BUESSION_EXCEPTION_H
#define BUESSION_EXCEPTION_H

extern zend_class_entry *exception_ce;
extern zend_class_entry *parseexception_ce;
extern zend_class_entry *invalidexception_ce;
extern zend_class_entry *illegalexception_ce;
extern zend_class_entry *indexoutofboundsexception_ce;
extern zend_class_entry *nullpointerexception_ce;
extern zend_class_entry *runtimeexception_ce;
extern zend_class_entry *outofmemoryexception_ce;
extern zend_class_entry *ioexception_ce;
extern zend_class_entry *unsupportedoperationexception_ce;
extern zend_class_entry *classnotfoundexception_ce;
extern zend_class_entry *clonenotsupportedexception_ce;
extern zend_class_entry *invalidpropertyexception_ce;
extern zend_class_entry *illegalargumentexception_ce;
extern zend_class_entry *nosuchmethodexception_ce;

#define EXCEPTION_PROPERTY_MESSAGE 	"message"
#define EXCEPTION_PROPERTY_CODE 	"code"
#define EXCEPTION_PROPERTY_PREVIOUS "previous"
#define EXCEPTION_PROPERTY_FILE 	"file"
#define EXCEPTION_PROPERTY_LINE 	"line"
#define EXCEPTION_PROPERTY_SEVERITY	"severity"
#define EXCEPTION_PROPERTY_TRACE	"trace"
#define EXCEPTION_PROPERTY_STRING	"string"

#define BUESSION_EXCEPTION_ME(method, arguments, flags) 					ZEND_ME(buession_exception, method, arguments, flags)
#define BUESSION_PARSREXCEPTION_ME(method, arguments, flags) 				ZEND_ME(buession_parseexception, method, arguments, flags)
#define BUESSION_INVALIDEXCEPTION_ME(method, arguments, flags) 				ZEND_ME(buession_invalidexception, method, arguments, flags)
#define BUESSION_ILLEGALEXCEPTION_ME(method, arguments, flags) 				ZEND_ME(buession_illegalexception, method, arguments, flags)
#define BUESSION_INDEXOUTOFBOUNDSEXCEPTION_ME(method, arguments, flags) 	ZEND_ME(buession_indexoutofboundsexception, method, arguments, flags)
#define BUESSION_NULLPOINTEREXCEPTION_ME(method, arguments, flags) 			ZEND_ME(buession_nullpointerexception, method, arguments, flags)
#define BUESSION_RUNTIMEEXCEPTION_ME(method, arguments, flags) 				ZEND_ME(buession_runtimeexception, method, arguments, flags)
#define BUESSION_OUTOFMEMORYEXCEPTION_ME(method, arguments, flags) 			ZEND_ME(buession_outofmemoryexception, method, arguments, flags)
#define BUESSION_IOEXCEPTION_ME(method, arguments, flags) 					ZEND_ME(buession_ioexception, method, arguments, flags)
#define BUESSION_UNSUPPORTEDOPERATIONEXCEPTION_ME(method, arguments, flags)	ZEND_ME(buession_unsupportedoperationexception, method, arguments, flags)
#define BUESSION_CLASSFOUNDEXCEPTION_ME(method, arguments, flags) 			ZEND_ME(buession_classnotfoundexception, method, arguments, flags)
#define BUESSION_CLONENOTSUPPORTEDEXCEPTION_ME(method, arguments, flags) 	ZEND_ME(buession_clonenotsupportedexception, method, arguments, flags)
#define BUESSION_INVALIDPROPERTYEXCEPTION_ME(method, arguments, flags) 		ZEND_ME(buession_invalidpropertyexception, method, arguments, flags)
#define BUESSION_ILLEGALARGUMENTEXCEPTION_ME(method, arguments, flags) 		ZEND_ME(buession_illegalargumentexception, method, arguments, flags)
#define BUESSION_NOSUCHMETHODEXCEPTION_ME(method, arguments, flags) 		ZEND_ME(buession_nosuchmethodexception, method, arguments, flags)

// catch exception by "try ... catch ...", could not use free memory by zval_dtor and zval_ptr_dtor
BUESSION_API zval *buession_throw_exception(zend_class_entry *exception_ce, int severity, char *message, uint message_length, long code);
BUESSION_API zval *buession_throw_exception_ex(zend_class_entry *exception_ce, int severity, char *format, long code, ...);

// direct print exception, could free memory by zval_dtor or zval_ptr_dtor
BUESSION_API zval *buession_throw_exception_error(zend_class_entry *exception_ce, int severity, char *message, uint message_length, long code);
BUESSION_API zval *buession_throw_exception_error_ex(zend_class_entry *exception_ce, int severity, char *format, long code, ...);

#define exception__construct_argument_exception(exception_ce){	\
	zval *exception = buession_throw_exception_error_ex(illegalargumentexception_ce, E_ERROR, "Wrong arguments for %s([string $message[, long $code[, Exception $previous = NULL]]])", 0, (exception_ce)->name);	\
	zval_ptr_dtor(&exception);	\
}

#define exception__construct(ce, default_message, default_message_length, default_code){	\
	char *message = default_message;	\
	uint message_length = default_message_length;	\
	int code = default_code;	\
	zval *previous = NULL;	\
	zend_class_entry *exception_ce = zend_exception_get_default(TSRMLS_C);	\
	if(zend_parse_parameters_ex(ZEND_PARSE_PARAMS_QUIET, ZEND_NUM_ARGS() TSRMLS_CC, "|slO!", &message, &message_length, &code, &previous, exception_ce) == FAILURE){	\
		exception__construct_argument_exception(ce);	\
	}	\
	if(message){	\
		exception_update_memsage_property(ce, message, message_length);	\
	}	\
	if(code){	\
		zend_update_property_long((ce), getThis(), ZEND_STRL(EXCEPTION_PROPERTY_CODE), code TSRMLS_CC);	\
	}	\
	if(previous){	\
		zend_update_property((ce), getThis(), ZEND_STRL(EXCEPTION_PROPERTY_PREVIOUS), previous TSRMLS_CC);	\
	}	\
}

#define exception_update_memsage_property(ce, message, message_length)	zend_update_property_stringl((ce), getThis(), ZEND_STRL(EXCEPTION_PROPERTY_MESSAGE), (message), (message_length) TSRMLS_CC)

#define exception_throw_object_init(exception_ce, instance, severit, code, message, message_length){	\
	exception = buession_getInstance_ex((exception_ce), NULL TSRMLS_CC);	\
	if(message){	\
		zend_update_property_stringl((exception_ce), exception, ZEND_STRL(EXCEPTION_PROPERTY_MESSAGE), (message), (message_length) TSRMLS_CC);	\
	}	\
	if(code){	\
		zend_update_property_long((exception_ce), exception, ZEND_STRL(EXCEPTION_PROPERTY_CODE), (code) TSRMLS_CC);	\
	}	\
	if(severity){	\
		zend_update_property_long((exception_ce), exception, ZEND_STRL(EXCEPTION_PROPERTY_SEVERITY), (severity) TSRMLS_CC);	\
	}	\
}

#define buession_outofmemory_exception(){	\
	zval *exception = buession_throw_exception_error(outofmemoryexception_ce, E_CORE_ERROR, NULL, 0, 0);	\
	zval_ptr_dtor(&exception);	\
}

#define buession_null_exception(code, message, message_length){	\
	zval *exception = buession_throw_exception_error(nullpointerexception_ce, E_CORE_ERROR, message, message_length, code);	\
	zval_ptr_dtor(&exception);	\
}

#define buession_unsupportedoperation_exception(code, message, message_length){	\
	zval *exception = buession_throw_exception_error(unsupportedoperationexception_ce, E_CORE_ERROR, message, message_length, code);	\
	zval_ptr_dtor(&exception);	\
}

#define buession_not_an_implementation_method_exception(ce, method){	\
	zval *exception = buession_throw_exception_error_ex(unsupportedoperationexception_ce, E_ERROR, "not an implementation method %s::%s().", 0, (ce)->name, method);	\
	zval_ptr_dtor(&exception);	\
}

#define buession_new_access_exception(ce, constructor_name, access_name){	\
	zval *exception = NULL;	\
	if(EG(scope)){	\
		exception = buession_throw_exception_error_ex(invalidexception_ce, E_ERROR, "Call to %s %s::%s() from context '%s'", 0, (access_name), (ce)->name, (constructor_name), EG(scope)->name);	\
	}else{	\
		exception = buession_throw_exception_error_ex(invalidexception_ce, E_ERROR, "Call to %s %s::%s() from invalid context", 0, (access_name), (ce)->name, (constructor_name));	\
	}	\
	if(exception != NULL){	\
		zval_ptr_dtor(&exception);	\
	}	\
}

#define buession_class_undefined_exception(name){ \
	zval *exception = buession_throw_exception_error_ex(classnotfoundexception_ce, E_ERROR, "class %s no found", 0, (name));	\
	zval_ptr_dtor(&exception);	\
}
#define buession_object_could_not_serialize_exception(ce){	\
	zval *exception = buession_throw_exception_error_ex(unsupportedoperationexception_ce, E_CORE_WARNING, "Serialize of '%s' objects is currently not supported", 0, (ce)->name);	\
	zval_ptr_dtor(&exception);	\
}
#define buession_object_could_not_unserialize_exception(ce){	\
	zval *exception = buession_throw_exception_error_ex(unsupportedoperationexception_ce, E_CORE_WARNING, "Unserialize of '%s' objects is currently not supported", 0, (ce)->name);	\
	zval_ptr_dtor(&exception);	\
}
#define buession_object_could_not_clone_exception(ce){	\
	zval *exception = buession_throw_exception_error_ex(clonenotsupportedexception_ce, E_CORE_WARNING, "clone of '%s' objects is currently not supported", 0, (ce)->name);	\
	zval_ptr_dtor(&exception);	\
}
#define buession_object_unsupported_cast_exception(ce, target_type){	\
	zval *exception = buession_throw_exception_error_ex(unsupportedoperationexception_ce, E_CORE_WARNING, "Object of class %s could not be converted to %s.", 0, (ce)->name, (target_type));	\
	zval_ptr_dtor(&exception);	\
}

#define buession_const_undefined_exception(ce, constant){	\
	zval *exception = buession_throw_exception_error_ex(illegalexception_ce, E_WARNING, "Undefined class %s constant '%s'", 0, (ce)->name, (constant));	\
	zval_ptr_dtor(&exception);	\
}

#define buession_property_invalid_exception(ce, property, value, correct_type){	\
	zval *exception = buession_throw_exception_error_ex(invalidpropertyexception_ce, E_ERROR, "Property %s::$%s to be %s, %s given", 0, (ce)->name, (property), (correct_type), zend_zval_type_name(value));	\
	zval_ptr_dtor(&exception);	\
}
#define buession_property_invalid_empty_exception(ce, property){	\
	zval *exception = buession_throw_exception_error_ex(invalidpropertyexception_ce, E_ERROR, "Property %s::$%s could not be empty", 0, (ce)->name, (property));	\
	zval_ptr_dtor(&exception);	\
}
#define buession_property_undefined_exception(ce, property){	\
	zval *exception = buession_throw_exception_error_ex(invalidpropertyexception_ce, E_NOTICE, "Undefined property: %s::$%s", 0, (ce)->name, (property));	\
	zval_ptr_dtor(&exception);	\
}
#define buession_property_could_not_read_exception(ce, property){	\
	zval *exception = buession_throw_exception_error_ex(invalidpropertyexception_ce, E_WARNING, "Property '%s' of class %s could not be read", 0, (property), (ce)->name);	\
	zval_ptr_dtor(&exception);	\
}
#define buession_property_could_not_write_exception(ce, property){	\
	zval *exception = buession_throw_exception_error_ex(invalidpropertyexception_ce, E_ERROR, "Property '%s' of class %s could not be write", 0, (property), (ce)->name);	\
	zval_ptr_dtor(&exception);	\
}
#define buession_property_could_not_unset_exception(ce, property){	\
	zval *exception = buession_throw_exception_error_ex(invalidpropertyexception_ce, E_ERROR, "Property '%s' of class %s could not be unset", 0, (property), (ce)->name);	\
	zval_ptr_dtor(&exception);	\
}

#define buession_method_not_found_exception(ce, method){ \
	zval *exception = buession_throw_exception_error_ex(nosuchmethodexception_ce, E_ERROR, "class %s method %s could not be found", 0, (ce)->name, (method));	\
	zval_ptr_dtor(&exception);	\
}
#define buession_method_argument_size_invalid_exception(size, correct_size){	\
	zval *exception = buession_throw_exception_error_ex(illegalargumentexception_ce, E_WARNING, "%s::%s() expects exactly %d arguments, %d given", 0, EG(called_scope)->name, get_active_function_name(TSRMLS_C), (correct_size), (size));	\
	zval_ptr_dtor(&exception);	\
}
#define buession_method_argument_invalid_exception(index, level, correct_type, argument){	\
	zval *exception = buession_throw_exception_error_ex(illegalargumentexception_ce, level, "expects argument %ld to be %s, %s given", 0, (index), (correct_type), zend_zval_type_name(argument));	\
	zval_ptr_dtor(&exception);	\
}
#define buession_method_object_argument_invalid_exception(index, ce, correct_ce, argument){	\
	zval *exception = buession_throw_exception_error_ex(illegalargumentexception_ce, E_RECOVERABLE_ERROR, "Argument %d passed to %s::%s() must be an instance of %s, instance of %s given", 0, (index), (ce)->name, get_active_function_name(TSRMLS_C), (correct_ce)->name, Z_OBJCE_P(argument)->name);	\
	zval_ptr_dtor(&exception);	\
}

#define buession_convet_type_exception(source_type, target_type, ce){	\
	char *message = NULL;	\
	uint message_length;	\
	if(source_type == IS_OBJECT){	\
		message_length = spprintf(&message, 0, "Object of class %s to %s convert error", (ce)->name, zend_get_type_by_const(target_type));	\
	}else{	\
		message_length = spprintf(&message, 0, "%s to %s convert error", zend_get_type_by_const(source_type), zend_get_type_by_const(target_type));	\
		*message = toupper(*message);	\
	}	\
	buession_throw_exception(unsupportedoperationexception_ce, E_NOTICE, message, message_length, 0);	\
	buession_free(message);	\
}

#define buession_key_type_exception(key, correct_type){	\
	zval *exception = buession_throw_exception_error_ex(illegalexception_ce, E_WARNING, "Illegal key type, must %s, %s given", 0, (correct_type), zend_zval_type_name(key));	\
	zval_ptr_dtor(&exception);	\
}

#define buession_hash_index_undefined_exception(index){	\
	zval *exception = buession_throw_exception_error_ex(indexoutofboundsexception_ce, E_NOTICE, "Undefined index: %ld", 0, (index));	\
	zval_ptr_dtor(&exception);	\
}
#define buession_hash_key_undefined_exception(key){	\
	zval *exception = buession_throw_exception_error_ex(indexoutofboundsexception_ce, E_NOTICE, "Undefined key: %s", 0, (key));	\
	zval_ptr_dtor(&exception);	\
}

#define buession_file_not_exists_exception(path, severity){	\
	zval *exception = buession_throw_exception_error_ex(ioexception_ce, severity, "File '%s' is not exists", 0, (path));	\
	zval_ptr_dtor(&exception);	\
}

#define buession_fso_illegal_stream_wrapper_exception(wrapper){	\
	zval *exception = buession_throw_exception_error_ex(ioexception_ce, E_WARNING, "Unable to locate stream wrapper%s%s.", 0, wrapper ? ": " : "", wrapper ? wrapper : "");	\
	zval_ptr_dtor(&exception);	\
}

BUESSION_STARTUP_FUNCTION(exception);
BUESSION_RINIT_FUNCTION(exception);
BUESSION_RSHUTDOWN_FUNCTION(exception);
BUESSION_SHUTDOWN_FUNCTION(exception);

#endif	/* EXCEPTION_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */