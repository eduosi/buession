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

/* $Id: objects_API.c 310447 2013-10-09 11:52:40Z Yong.Teng $ */

/* include php file */
#include "Zend/zend.h"
#include "Zend/zend_API.h"
#include "Zend/zend_interfaces.h"
#include "Zend/zend_exceptions.h"
#include "main/php.h"
#include "main/php_main.h"
#include "ext/standard/basic_functions.h"
#include "ext/standard/flock_compat.h"
#include "ext/standard/file.h"
#include "ext/spl/spl_iterators.h"

/* include buession file */
#include "buession.h"
#include "objects_API.h"

BUESSION_API char *buession_get_package_classname(char *package, uint package_length, uint *classname_length TSRMLS_DC){
	if(memchr(package, '.', package_length) != NULL){
		return php_str_to_str(package, package_length, ".", 1, "\\", 1, classname_length);
	}else{
		*classname_length = package_length;
		return estrndup(package, package_length);
	}
}

BUESSION_API int buession_loadClass(char *package, uint package_length TSRMLS_DC){
	smart_str directory = {0};
	zval *zcontext = NULL;
	php_stream_context *context = NULL;
	php_stream *stream = NULL;
	php_stream_dirent dirent;
	char *path = NULL;
	uint path_length = 0;
	zval isfile;
	zval *exception = NULL;
	int result = FAILURE;

	if(package_length == 0){
		exception = buession_throw_exception_error(illegalexception_ce, E_WARNING, ZEND_STRL("Invalid class file path, could not empty"), 0);
		zval_ptr_dtor(&exception);

		return FAILURE;
	}

	if(BUESSION_G(classpath) == NULL||BUESSION_G(classpath) == 0){
		smart_str_appendl(&directory, ".", 1);
	}else{
		smart_str_appendl(&directory, BUESSION_G(classpath), BUESSION_G(classpath_length));
	}
	smart_str_appendc(&directory, DEFAULT_SLASH);

	if(package_length == 1&&package[package_length - 1] == '*'){
		goto dir_load;
	}else{
		char *filename = buession_getClassFilePath(package, package_length);

		if((package[package_length - 2] == '.')&&(package[package_length - 1] == '*')){
			smart_str_appendl(&directory, package, package_length - 2);

			dir_load:
			smart_str_0(&directory);
			context = php_stream_context_from_zval(zcontext, 0);
			if((stream = php_stream_opendir(directory.c, ENFORCE_SAFE_MODE|REPORT_ERRORS, context)) == NULL){
				goto end;
			}

			stream->flags |= PHP_STREAM_FLAG_NO_FCLOSE;
			if(!(stream->flags&PHP_STREAM_FLAG_IS_DIR)){
				goto end;
			}

			result = SUCCESS;
			while(php_stream_readdir(stream, &dirent) != NULL){
				if(memcmp(dirent.d_name, ".", 1) == 0||memcmp(dirent.d_name, "..", 2) == 0){
					continue;
				}

				path_length = spprintf(&path, 0, "%s%c%s", directory.c, DEFAULT_SLASH, dirent.d_name);
				php_stat(path, path_length, FS_IS_FILE, &isfile TSRMLS_CC);
				if(Z_TYPE(isfile) == IS_BOOL&&Z_BVAL(isfile) == TRUE){
					if(loader(path, path_length, FALSE TSRMLS_CC) == FAILURE){
						buession_free(path);
						result = FAILURE;
						goto end;
					}
				}
				buession_free(path);
			}

			end:
				buession_stream_free(stream);
				//php_stream_context_free(context);
		}else{
			smart_str_0(&directory);
			path_length = spprintf(&path, 0, "%s%c%s.php", directory.c, DEFAULT_SLASH, filename);

			php_printf("path: %s\r\n", path);
			result = loader(path, path_length, FALSE TSRMLS_CC);
			buession_free(path);
		}

		buession_free(filename);
	}
	smart_str_free(&directory);

	return result;
}

BUESSION_API zend_bool buession_classname_instanceof(const char *classname, uint classname_length, const zend_class_entry *ce TSRMLS_DC){
	return buession_classname_instanceof_ex(classname, classname_length, ce, FALSE TSRMLS_CC);
}
BUESSION_API zend_bool buession_classname_instanceof_ex(const char *classname, uint classname_length, const zend_class_entry *ce, zend_bool interfaces_only TSRMLS_DC){
	zend_class_entry **instance_ce;

	if(zend_lookup_class_ex(classname, classname_length, FALSE, &instance_ce TSRMLS_CC) == SUCCESS){
		return instanceof_function_ex(*instance_ce, ce, interfaces_only TSRMLS_CC);
	}

	return FAILURE;
}

BUESSION_API zval *buession_getInstance(zend_class_entry *ce, HashTable *properties, uint argument_count, zval **arguments[] TSRMLS_DC){
	zval *instance;

	if(ce->ce_flags&ZEND_ACC_INTERFACE){
		buession_throw_exception_ex(runtimeexception_ce, E_ERROR, "Cannot instantiate interface %s.", 0, ce->name);
		return NULL;
	}

	MAKE_STD_ZVAL(instance);
	if(ce->constructor){
		zend_fcall_info fci;
		zend_fcall_info_cache fcic;
		zval *retval_ptr = NULL;
		uint i = 0;

		if(!(ce->constructor->common.fn_flags&ZEND_ACC_PUBLIC)){
			buession_throw_exception_ex(runtimeexception_ce, E_ERROR, "Access to non-public constructor of class %s.", 0, ce->name);
			return NULL;
		}

		if(properties == NULL){
			object_init_ex(instance, ce);
		}else{
			object_and_properties_init(instance, ce, properties);
		}

		buession_fcall_info_init(NULL, &fci, &fcic TSRMLS_CC);

		fci.retval_ptr_ptr = &retval_ptr;
		fci.param_count = argument_count;
		fci.params = arguments;

		fcic.function_handler = ce->constructor;
		fcic.calling_scope = EG(scope);
		fcic.called_scope = ce;
		fcic.object_ptr = instance;

		if(zend_call_function(&fci, &fcic TSRMLS_CC) == FAILURE){
			//buession_free(arguments);
			buession_zval_ptr_dtor(retval_ptr);
			buession_zval_ptr_dtor(instance);
			//zend_restore_error_handling(&error_handling TSRMLS_CC);
			buession_throw_exception_ex(exception_ce, E_ERROR, "Invocation of %s's constructor failed", 0, ce->name);

			return NULL;
		}

		//buession_free(arguments);
		zval_ptr_dtor(&retval_ptr);
		retval_ptr = NULL;
	}else if(properties == NULL){
		object_init_ex(instance, ce);
	}else{
		object_and_properties_init(instance, ce, properties);
	}

	return instance;
}
BUESSION_API zval *buession_getInstance_ex(zend_class_entry *ce, HashTable *properties TSRMLS_DC){
	zval *instance;

	MAKE_STD_ZVAL(instance);
	if(properties == NULL){
		object_init_ex(instance, ce);
	}else{
		object_and_properties_init(instance, ce, properties);
	}

	return instance;
}

BUESSION_API int zend_update_static_property_hashtable(zend_class_entry *ce, char *name, int name_length, HashTable *value TSRMLS_DC){
	zval *temp;

	MAKE_STD_ZVAL(temp);
	Z_ARRVAL_P(temp) = value;

	return zend_update_static_property(ce, name, name_length, temp TSRMLS_CC);
}
BUESSION_API void zend_update_property_hashtable(zend_class_entry *ce, zval *object, char *name, int name_length, HashTable *value TSRMLS_DC){
	zval *temp;

	MAKE_STD_ZVAL(temp);
	Z_ARRVAL_P(temp) = value;

	zend_update_property(ce, object, name, name_length, temp TSRMLS_CC);
}
BUESSION_API int zend_update_static_property_array(zend_class_entry *ce, char *name, int name_length, uint size TSRMLS_DC){
	zval *value;

	BUESSION_ARRAY_SIZE_INIT(value, size);
	return zend_update_static_property(ce, name, name_length, value TSRMLS_CC);
}
BUESSION_API void zend_update_property_array(zend_class_entry *ce, zval *object, char *name, int name_length, uint size TSRMLS_DC){
	zval *value;

	BUESSION_ARRAY_SIZE_INIT(value, size);
	zend_update_property(ce, object, name, name_length, value TSRMLS_CC);
}

BUESSION_API int buession_std_read_property_na(void *intern, zval *instance, zval *property, zval **result TSRMLS_DC){
	zval temp_property;

	BUESSION_CONVERT_VARIABLE_TO_STRING(property, temp_property)
	buession_property_could_not_read_exception(Z_OBJCE_P(instance), Z_STRVAL_P(property));
	BUESSION_CONVERT_VARIABLE_PTR_DTOR(property, temp_property);

	return FAILURE;
}
BUESSION_API int buession_std_write_property_na(void *intern, zval *instance, zval *property, zval *value TSRMLS_DC){
	zval temp_property;

	BUESSION_CONVERT_VARIABLE_TO_STRING(property, temp_property);
	buession_property_could_not_write_exception(Z_OBJCE_P(instance), Z_STRVAL_P(property));
	BUESSION_CONVERT_VARIABLE_PTR_DTOR(property, temp_property);

	return FAILURE;
}
BUESSION_API int buession_std_unset_property_na(void *intern, zval *instance, zval *property TSRMLS_DC){
	zval temp_property;

	BUESSION_CONVERT_VARIABLE_TO_STRING(property, temp_property)
	buession_property_could_not_unset_exception(Z_OBJCE_P(instance), Z_STRVAL_P(property));
	BUESSION_CONVERT_VARIABLE_PTR_DTOR(property, temp_property);

	return FAILURE;
}

BUESSION_API void buession_fcall_info_init(zval *method, zend_fcall_info *fci, zend_fcall_info_cache *fcic TSRMLS_DC){
	fci->size = sizeof(zend_fcall_info);
	fci->function_table = fcic->calling_scope ? &fcic->calling_scope->function_table : EG(function_table);
	fci->object_ptr = fcic->object_ptr;
	fci->function_name = method;
	fci->retval_ptr_ptr = NULL;
	fci->param_count = 0;
	fci->params = NULL;
	fci->no_separation = TRUE;
	fci->symbol_table = NULL;
	fcic->initialized = TRUE;
}

BUESSION_API int buession_call_method(zend_class_entry *ce, zval *instance, char *method, uint method_length, uint argument_count, zval **arguments[], zval **retval TSRMLS_DC){
	return buession_call_method_ex(ce, instance, method, method_length, argument_count, arguments, TRUE, retval TSRMLS_CC);
}
BUESSION_API int buession_call_method_ex(zend_class_entry *ce, zval *instance, char *method, uint method_length, uint argument_count, zval **arguments[], zend_bool call___call, zval **retval TSRMLS_DC){
	zend_fcall_info fci;
	zend_fcall_info_cache fcic;
	char *method_name = zend_str_tolower_dup(method, method_length);
	zval zmethod;
	char *error = NULL;
	zval *retval_ptr;

	ZVAL_STRINGL(&zmethod, method, method_length, FALSE);
	buession_fcall_info_init(&zmethod, &fci, &fcic TSRMLS_CC);

	fcic.object_ptr = instance;

	if(!ce){
		if(instance){
			ce = Z_OBJCE_P(instance);
		}else{
			buession_free(method_name);
			buession_free(arguments);
			buession_throw_exception(classnotfoundexception_ce, E_ERROR, NULL, 0, 0);

			return FAILURE;
		}
	}

	if(zend_hash_find(&(ce->function_table), method_name, method_length + 1, (void **) &fcic.function_handler) == FAILURE){
		if(call___call == TRUE){
			zend_function *call = instance ? ce->__call : ce->__callstatic;

			if(call != NULL){
				zval *_zmethod;
				zval zcall;

				ZVAL_STRING(&zcall, call->common.function_name, FALSE);

				fci.function_name = &zcall;
				fcic.function_handler = call;

				MAKE_STD_ZVAL(_zmethod);
				ZVAL_STRINGL(_zmethod, method_name, method_length, TRUE);

				if(argument_count > 0){
					zval *args = NULL;
					zval **params[2] = {&_zmethod, &args};
					uint i = 0;

					BUESSION_ARRAY_SIZE_INIT(args, argument_count);
					for(; i < argument_count; i++){
						add_index_zval(args, i, *(arguments[i]));
					}

					fci.param_count = 2;
					fci.params = params;
				}else{
					zval **params[2] = {&_zmethod};

					fci.param_count = 1;
					fci.params = params;
				}

				goto call;
			}
		}

		buession_free(method_name);
		buession_free(arguments);
		buession_method_not_found_exception(ce, method);

		return FAILURE;
	}else{
		fci.param_count = argument_count;
		fci.params = arguments;
	}

	call:
	buession_free(method_name);

	fci.retval_ptr_ptr = retval ? retval : &retval_ptr;
	fcic.calling_scope = ce;
	if(instance){
		fcic.called_scope = Z_OBJCE_P(instance);
	}else if(ce&&!(EG(called_scope)&&instanceof_function(EG(called_scope), ce TSRMLS_CC))){
		fcic.called_scope = ce;
	}else{
		fcic.called_scope = EG(called_scope);
	}

	if(zend_call_function(&fci, &fcic TSRMLS_CC) == FAILURE){
		if(retval == NULL){
			zval_ptr_dtor(&retval_ptr);
		}else{
			zval_ptr_dtor(retval);
		}

		if(!EG(exception)){
			buession_method_not_found_exception(ce, method);
		}else{
			zend_exception_error(EG(exception), E_CORE_ERROR TSRMLS_CC);
		}

		return FAILURE;
	}

	if(retval == NULL){
		zval_ptr_dtor(&retval_ptr);
	}

	return SUCCESS;
}

BUESSION_API uint buession_get_object_size(zval *object TSRMLS_DC){
	return buession_get_object_size_ex(object, TRUE TSRMLS_CC);
}
BUESSION_API uint buession_get_object_size_ex(zval *object, zend_bool check_toString TSRMLS_DC){
	zend_class_entry *ce = Z_OBJCE_P(object);
	zend_object_handlers *handlers = NULL;

	if(instanceof_function(ce, zend_ce_traversable TSRMLS_CC) == TRUE){
		zend_object_iterator *iterator = ce->get_iterator(ce, object, 0 TSRMLS_CC);

		if(EG(exception)){
			goto finish;
		}

		iterator->index = 0;
		if(iterator->funcs->rewind){
			iterator->funcs->rewind((iterator) TSRMLS_CC);
			if(EG(exception)){
				goto finish;
			}
		}

		while(iterator->funcs->valid(iterator TSRMLS_CC) == SUCCESS){
			if(EG(exception)){
				goto finish;
			}

			iterator->index++;
			iterator->funcs->move_forward(iterator TSRMLS_CC);
			if(EG(exception)){
				goto finish;
			}
		}

		finish:
		if(iterator){
			iterator->funcs->dtor(iterator TSRMLS_CC);
		}

		if(iterator->index > 0){
			return iterator->index;
		}

		goto property;
	}else{
		property:
		handlers = Z_OBJ_HT_P(object);
		if(handlers->get_properties){
			HashTable *properties = handlers->get_properties(object TSRMLS_CC);
			uint count = properties == NULL ? 0 : zend_hash_num_elements(properties);

			if(count > 0){
				return count;
			}
		}
	}

	if(check_toString == TRUE){
		zval *temp = NULL;

		BUESSION_COPY_ZVAL_P(temp, object);
		if(buession_convert_object_to_string(temp TSRMLS_CC) == SUCCESS){
			uint length = Z_STRLEN_P(temp);

			buession_zval_dtor(temp);
			return length;
		}
	}

	return 0;
}

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */