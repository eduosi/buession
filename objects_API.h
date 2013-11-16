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

/* $Id: objects_API.h 310447 2013-10-09 11:52:28Z Yong.Teng $ */
#ifndef BUESSION_OBJECTS_API_H
#define BUESSION_OBJECTS_API_H

BUESSION_API char *buession_get_package_classname(char *package, uint package_length, uint *classname_length TSRMLS_DC);

#define buession_getClassFilePath(package, package_length) php_str_to_str(package, package_length, ".", 1, "/", 1, &package_length)
BUESSION_API int buession_loadClass(char *package, uint package_length TSRMLS_DC);

BUESSION_API zend_bool buession_classname_instanceof(const char *classname, uint classname_length, const zend_class_entry *ce TSRMLS_DC);
BUESSION_API zend_bool buession_classname_instanceof_ex(const char *classname, uint classname_length, const zend_class_entry *ce, zend_bool interfaces_only TSRMLS_DC);

BUESSION_API zval *buession_getInstance(zend_class_entry *ce, HashTable *properties, uint argument_count, zval **arguments[] TSRMLS_DC);
BUESSION_API zval *buession_getInstance_ex(zend_class_entry *ce, HashTable *properties TSRMLS_DC);

BUESSION_API int zend_update_static_property_hashtable(zend_class_entry *ce, char *name, int name_length, HashTable *value TSRMLS_DC);
BUESSION_API void zend_update_property_hashtable(zend_class_entry *ce, zval *object, char *name, int name_length, HashTable *value TSRMLS_DC);
BUESSION_API int zend_update_static_property_array(zend_class_entry *ce, char *name, int name_length, uint size TSRMLS_DC);
BUESSION_API void zend_update_property_array(zend_class_entry *ce, zval *object, char *name, int name_length, uint size TSRMLS_DC);

BUESSION_API int buession_std_read_property_na(void *intern, zval *instance, zval *property, zval **result TSRMLS_DC);
BUESSION_API int buession_std_write_property_na(void *intern, zval *instance, zval *property, zval *value TSRMLS_DC);
BUESSION_API int buession_std_unset_property_na(void *intern, zval *instance, zval *property TSRMLS_DC);

#define BUESSION_COPY_OBJECT_HANDLERS(handlers)	memcpy((handlers), zend_get_std_object_handlers(), sizeof(zend_object_handlers))

#define BUESSION_OBJECT_NEW(module) zend_object_value module##_object_new(zend_class_entry *ce TSRMLS_DC)
#define BUESSION_OBJECT_NEW_NAMED(module) module##_object_new
#define BUESSION_OBJECT_CLONE(module) zend_object_value module##_clone_object(zval *instance TSRMLS_DC)
#define BUESSION_OBJECT_CLONE_NAMED(module) module##_clone_object
#define BUESSION_OBJECT_CAST(module) int module##_cast_object(zval *instance, zval *result, int type TSRMLS_DC)
#define BUESSION_OBJECT_CAST_NAMED(module) module##_cast_object
#define BUESSION_OBJECT_FREE(module) void module##_object_free_storage(void *instance TSRMLS_DC)
#define BUESSION_OBJECT_FREE_NAMED(module) module##_object_free_storage

#define BUESSION_OBJECT_STRING_FIELDS_DUP(object, fields, value, value_length){	\
	if(zend_binary_strcmp((value), (value_length), (object)->fields, (object)->fields##_length) != 0){	\
		buession_free((object)->fields);	\
		(object)->fields = buession_estrndup(value, value_length);	\
		(object)->fields##_length = value_length;	\
	}	\
}
#define BUESSION_OBJECT_STRING_FIELDS_CLONE(new_object, old_object, fields) BUESSION_OBJECT_STRING_FIELDS_DUP(new_object, fields, (old_object)->fields, (old_object)->fields##_length)
#define BUESSION_OBJECT_STRING_FIELDS_FREE(object, fields){ \
	buession_free((object)->fields);	\
	(object)->fields##_length = 0;	\
}

#define BUESSION_OBJECT_HASHTABLE_FIELDS_DUP(object, fields, value){	\
	if((object)->fields != NULL){	\
		zend_hash_clean((object)->fields);	\
	}	\
	if(value != NULL){	\
		zend_hash_copy((object)->fields, value, (copy_ctor_func_t) zval_add_ref, NULL, sizeof(zval *));	\
	}	\
}
#define BUESSION_OBJECT_HASHTABLE_FIELDS_CLONE(new_object, old_object, fields) BUESSION_OBJECT_HASHTABLE_FIELDS_DUP(new_object, fields, (old_object)->fields)

#define BUESSION_OBJECT_ZVAL_FIELDS_DUP(object, fields, value){	\
	buession_zval_ptr_dtor((object)->fields);	\
	if(value != NULL){	\
		BUESSION_COPY_ZVAL_P((object)->fields, value);	\
	}	\
}
#define BUESSION_OBJECT_ZVAL_FIELDS_CLONE(new_object, old_object, fields) BUESSION_OBJECT_ZVAL_FIELDS_DUP(new_object, fields, (old_object)->fields)

#define BUESSION_OBJECT_FIELDS_DTOR(object, fields){	\
	if((object)->fields){	\
		zval_ptr_dtor(&(object)->fields);	\
		(object)->fields = NULL;	\
	}	\
}

#define BUESSION_REGISTER_PROPERTY(module) 		void module##_register_property(char *name, size_t name_length, module##_read_fn read_fn, module##_write_fn write_fn TSRMLS_DC)
#define BUESSION_REGISTER_PROPERTY_NAMED(module) module##_register_property

#define BUESSION_OBJECT_HAS_PROPERTY(module) 		int module##_has_property(zval *instance, zval *property, int has_set_exists TSRMLS_DC)
#define BUESSION_OBJECT_HAS_PROPERTY_NAMED(module) 	module##_has_property

#define BUESSION_OBJECT_READ_PROPERTY(module) 				zval *module##_read_property(zval *instance, zval *property, int type TSRMLS_DC)
#define BUESSION_OBJECT_READ_PROPERTY_NAMED(module) 		module##_read_property
#define BUESSION_OBJECT_READ_PROPERTY_FN(module, fn)		int module##_##fn##_read(module##_object *intern, zval *instance, zval *property, zval **result TSRMLS_DC)
#define BUESSION_OBJECT_READ_PROPERTY_FN_NAMED(module, fn)	module##_##fn##_read

#define BUESSION_OBJECT_GET_PROPERTY_PTR_PTR(module) 			zval **module##_get_property_ptr_ptr(zval *instance, zval *property TSRMLS_DC)
#define BUESSION_OBJECT_GET_PROPERTY_PTR_PTR_NAMED(module) 		module##_get_property_ptr_ptr

#define BUESSION_OBJECT_GET_PROPERTYS(module) 		HashTable *module##_get_propertys(zval *instance TSRMLS_DC)
#define BUESSION_OBJECT_GET_PROPERTYS_NAMED(module) module##_get_propertys

#define BUESSION_OBJECT_WRITE_PROPERTY(module) 				void module##_write_property(zval *instance, zval *property, zval *value TSRMLS_DC)
#define BUESSION_OBJECT_WRITE_PROPERTY_NAMED(module) 		module##_write_property
#define BUESSION_OBJECT_WRITE_PROPERTY_FN(module, fn)		int module##_##fn##_write(module##_object *intern, zval *instance, zval *property, zval *value TSRMLS_DC)
#define BUESSION_OBJECT_WRITE_PROPERTY_FN_NAMED(module, fn)	module##_##fn##_write

#define BUESSION_OBJECT_UNSET_PROPERTY(module) void module##_unset_property(zval *instance, zval *property TSRMLS_DC)
#define BUESSION_OBJECT_UNSET_PROPERTY_NAMED(module) module##_unset_property

#define BUESSION_OBJECT_HAS_DIMENSION(module) int module##_has_dimension(zval *instance, zval *offset, int check_empty TSRMLS_DC)
#define BUESSION_OBJECT_HAS_DIMENSION_NAMED(module) module##_has_dimension

#define BUESSION_OBJECT_READ_DIMENSION(module) zval *module##_read_dimension(zval *instance, zval *offset, int type TSRMLS_DC)
#define BUESSION_OBJECT_READ_DIMENSION_NAMED(module) module##_read_dimension

#define BUESSION_OBJECT_WRITE_DIMENSION(module) void module##_write_dimension(zval *instance, zval *offset, zval *value TSRMLS_DC)
#define BUESSION_OBJECT_WRITE_DIMENSION_NAMED(module) module##_write_dimension

#define BUESSION_OBJECT_UNSET_DIMENSION(module) void module##_unset_dimension(zval *instance, zval *offset TSRMLS_DC)
#define BUESSION_OBJECT_UNSET_DIMENSION_NAMED(module) module##_unset_dimension

#define BUESSION_OBJECT_COUNT_ELEMENTS(module) int module##_count_elements(zval *instance, long *count TSRMLS_DC)
#define BUESSION_OBJECT_COUNT_ELEMENTS_NAMED(module) module##_count_elements

#define BUESSION_OBJECT_SERIALIZE(module) int module##_serialize(zval *instance, unsigned char **result, uint *result_length, zend_serialize_data *var_hash TSRMLS_DC)
#define BUESSION_OBJECT_SERIALIZE_NAMED(module) module##_serialize

#define BUESSION_OBJECT_UNSERIALIZE(module) int module##_unserialize(zval **instance, zend_class_entry *ce, const unsigned char *str, uint str_length, zend_unserialize_data *var_hash TSRMLS_DC)
#define BUESSION_OBJECT_UNSERIALIZE_NAMED(module) module##_unserialize

#define buession_object_serialize_method_common_serialize(var)	\
	do{	\
		php_serialize_data_t var_hash;	\
		smart_str buffer = {0};	\
		if(buession_serialize((var), &var_hash, &buffer TSRMLS_CC) == SUCCESS){	\
			buession_zval_ptr_dtor(var);	\
			BUESSION_RETURN_STRINGL(buffer.c, buffer.len, FALSE);	\
		}	\
		buession_zval_ptr_dtor(var);	\
	}while(0);

BUESSION_API int buession_call_method(zend_class_entry *ce, zval *instance, char *method, uint method_length, uint argument_count, zval **arguments[], zval **retval TSRMLS_DC);
BUESSION_API int buession_call_method_ex(zend_class_entry *ce, zval *instance, char *method, uint method_length, uint argument_count, zval **arguments[], zend_bool call___call, zval **retval TSRMLS_DC);
#define buession_call_method_0(ce, instance, method, method_length, retval, result){	\
	result = buession_call_method((ce), (instance), (method), (method_length), 0, NULL, (retval) TSRMLS_CC);	\
}
#define buession_call_method_1(ce, instance, method, method_length, arg, retval, result){	\
	zval **arguments[1] = {&arg};	\
	result = buession_call_method((ce), (instance), (method), (method_length), 1, arguments, (retval) TSRMLS_CC);	\
}
#define buession_call_method_2(ce, instance, method, method_length, arg_1, arg_2, retval, result){	\
	zval **arguments[2] = {&arg_1, &arg_2};	\
	result = buession_call_method((ce), (instance), (method), (method_length), 2, arguments, (retval) TSRMLS_CC);	\
}
#define buession_call_method_3(ce, instance, method, method_length, arg_1, arg_2, arg_3, retval, result){	\
	zval **arguments[3] = {&arg_1, &arg_2, &arg_3};	\
	result = buession_call_method((ce), (instance), (method), (method_length), 3, arguments, (retval) TSRMLS_CC);	\
}
#define buession_call_method_4(ce, instance, method, method_length, arg_1, arg_2, arg_3, arg_4, retval, result){	\
	zval **arguments[4] = {&arg_1, &arg_2, &arg_3, &arg_4};	\
	result = buession_call_method((ce), (instance), (method), (method_length), 4, arguments, (retval) TSRMLS_CC);	\
}
#define buession_call_method_5(ce, instance, method, method_length, arg_1, arg_2, arg_3, arg_4, arg_5, retval, result){	\
	zval **arguments[5] = {&arg_1, &arg_2, &arg_3, &arg_4, &arg_5};	\
	result = buession_call_method((ce), (instance), (method), (method_length), 5, arguments, (retval) TSRMLS_CC);	\
}

#define ZEND_METHOD_EMPTY_CALL_ENTRY(){	\
	char *method;	\
	uint *method_length;	\
	HashTable *arguments = NULL;	\
	zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|H", &method, &method_length, &arguments);	\
}
#define ZEND_METHOD_EMPTY_CALLSTATIC_ENTRY(){	\
	char *method;	\
	uint *method_length;	\
	HashTable *arguments = NULL;	\
	zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|H", &method, &method_length, &arguments);	\
}

BUESSION_API uint buession_get_object_size(zval *object TSRMLS_DC);
BUESSION_API uint buession_get_object_size_ex(zval *object, zend_bool check_toString TSRMLS_DC);

#endif /* BUESSION_OBJECTS_API_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */