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

/* $Id: Buession_API.c 310447 2013-09-10 13:49:45Z Yong.Teng $ */

/* include php file */
#include "Zend/zend.h"
#include "Zend/zend_API.h"
#include "Zend/zend_exceptions.h"
#include "main/php.h"
#include "main/php_main.h"
#include "TSRM/tsrm_virtual_cwd.h"
#include "ext/standard/basic_functions.h"
#include "ext/standard/flock_compat.h"
#include "ext/standard/file.h"
#include "ext/json/php_json.h"
#include "ext/hash/php_hash.h"

/* include buession file */
#include "buession.h"
#include "Buession_API.h"

BUESSION_API int loader(char *path, uint path_length, zend_bool use_path TSRMLS_DC){
	char real_path_buffer[MAXPATHLEN];
	char *real_path;
	int retval = FAILURE;

	if(VCWD_REALPATH(path, real_path_buffer)){
		zend_file_handle file_handle;
		zend_op_array *op_array;

		if(PG(safe_mode) == TRUE&&(!php_checkuid(real_path_buffer, NULL, CHECKUID_CHECK_FILE_AND_DIR))){
			return FAILURE;
		}

		if(php_check_open_basedir(real_path_buffer TSRMLS_CC)){
			return FAILURE;
		}

		#ifdef ZTS
			if(VCWD_ACCESS(real_path_buffer, F_OK)){
				return FAILURE;
			}
		#endif

		real_path = (unsigned char *) real_path_buffer;
		if(zend_hash_exists(&EG(included_files), real_path, strlen(real_path) + 1) == TRUE){
			return SUCCESS;
		}

		file_handle.filename = real_path;
		file_handle.free_filename = 0;
		file_handle.type = ZEND_HANDLE_FILENAME;
		file_handle.opened_path = NULL;
		file_handle.handle.fp = NULL;

		op_array = zend_compile_file(&file_handle, ZEND_INCLUDE TSRMLS_CC);
		if(op_array&&file_handle.handle.stream.handle){
			uint dummy = 1;

			if(!file_handle.opened_path){
				file_handle.opened_path = real_path;
			}

			zend_hash_add(&EG(included_files), file_handle.opened_path, strlen(file_handle.opened_path) + 1, (void *) &dummy, sizeof(uint), NULL);
		}
		zend_destroy_file_handle(&file_handle TSRMLS_CC);

		if(op_array){
			zval *retval_ptr = NULL;

			BUESSION_STORE_EG_ENVIRON();

			EG(return_value_ptr_ptr) = &retval_ptr;
			EG(active_op_array) = op_array;

			if(!EG(active_symbol_table)){
				zend_rebuild_symbol_table(TSRMLS_C);
			}

			zend_execute(op_array TSRMLS_CC);
			destroy_op_array(op_array TSRMLS_CC);
			buession_free(op_array);

			if(!EG(exception)){
				if(EG(return_value_ptr_ptr)&&*EG(return_value_ptr_ptr)){
					zval_ptr_dtor(EG(return_value_ptr_ptr));
				}
			}

			buession_zval_ptr_dtor(retval_ptr);

			BUESSION_RESTORE_EG_ENVIRON();

			return SUCCESS;
		}
	}

	return FAILURE;
}

BUESSION_API int buession_call_function(char *name, uint name_length, uint argument_count, zval **arguments[], zval **retval TSRMLS_DC){
	char *fn = zend_str_tolower_dup(name, name_length);
	zval zfn;
	zval *retval_ptr = NULL;
	int result;

	ZVAL_STRINGL(&zfn, fn, name_length, FALSE);
	if((result = call_user_function_ex(EG(function_table), NULL, &zfn, &retval_ptr, argument_count, arguments, TRUE, NULL TSRMLS_CC)) == SUCCESS){
		if(retval != NULL){
			*retval = retval_ptr;
		}else{
			zval_ptr_dtor(&retval_ptr);
		}
	}

	//buession_free(arguments);

	return result;
}

/* env function start */
const port_entity port_entities[] = {
	{"http",	4,	80},
	{"https", 	5,	334},
	{"ftp", 	3,	21},
	{"ftps", 	4,	990},
	{"ssh", 	3,	22},
	{"telnet", 	6,	23},
	{"pop2", 	4,	109},
	{"pop3", 	4,	110},
	{"smtp", 	4,	25},
	{"imap", 	4,	143},
	{"snmp", 	4,	161},
	{NULL,		0,	-1}
};

BUESSION_API int buession_put_env(char *name, uint name_length, char *value, uint value_length TSRMLS_DC){
	#ifdef HAVE_PUTENV
		char **env;
		putenv_entry pe;
		#ifdef PHP_WIN32
			int error_code;
		#endif
		int retval;

		if(PG(safe_mode)){
			if(zend_hash_exists(&BG(sm_protected_env_vars), name, name_length) == TRUE){
				zval *exception = buession_throw_exception_error_ex(runtimeexception_ce, E_WARNING, "Safe Mode warning: Cannot override protected environment variable '%s'", 0, name);

				zval_ptr_dtor(&exception);

				return FAILURE;
			}
		}

		if(value){
			spprintf(&(pe.putenv_string), 0, "%s=%s", name, value);
		}

		zend_hash_del(&BG(putenv_ht), name, name_length + 1);
		if(value == NULL){
			return SUCCESS;
		}

		pe.key = estrndup(name, name_length);
		pe.key_len = name_length;

		/* find previous value */
		pe.previous_value = NULL;
		for(env = environ; env != NULL&&*env != NULL; env++){
			if(strncmp(*env, pe.key, pe.key_len) == 0&&(*env)[pe.key_len] == '='){	/* found it */
				#if defined(PHP_WIN32)
					/* must copy previous value because MSVCRT's putenv can free the string without notice */
					  pe.previous_value = estrdup(*env);
				#else
					  pe.previous_value = *env;
				#endif
				break;
			}
		}

		#if HAVE_UNSETENV
			if(!value){ /* no '=' means we want to unset it */
				unsetenv(pe.putenv_string);
			}
			if(!value||putenv(pe.putenv_string) == 0){ /* success */
		#else
			#ifndef PHP_WIN32
				if(putenv(pe.putenv_string) == 0){ /* success */
			#else
				error_code = SetEnvironmentVariable(pe.key, value);
				#if _MSC_VER < 1500
					/* Yet another VC6 bug, unset may return env not found */
					if(error_code != 0||(error_code == 0&&GetLastError() == ERROR_ENVVAR_NOT_FOUND)){
				#else
					if(error_code != 0){ /* success */
				#endif
			#endif
		#endif

			retval = zend_hash_add(&BG(putenv_ht), pe.key, pe.key_len + 1, (void **) &pe, sizeof(putenv_entry), NULL);

			#ifdef HAVE_TZSET
				if(!strncmp(pe.key, "TZ", pe.key_len)){
					tzset();
				}
			#endif

			return retval;
		}else{
			buession_free(pe.key);
			buession_free(pe.putenv_string);
		}
	#endif

	return SUCCESS;
}
BUESSION_API char *buession_get_env(char *name, uint name_length, uint *value_length TSRMLS_DC){
	char *value = sapi_getenv(name, name_length TSRMLS_CC);

	if(value != NULL){
		common:
		*value_length = strlen(value);
		return value;
	}

	if((value = zend_getenv(name, name_length TSRMLS_CC)) != NULL){
		goto common;
	}

	#ifdef PHP_WIN32
	{
		char dummybuf;

		SetLastError(0);
		*value_length = GetEnvironmentVariableA(name, &dummybuf, 0);
		if(GetLastError() == ERROR_ENVVAR_NOT_FOUND){
			return NULL;
		}

		if(*value_length == 0){
			return STR_EMPTY_ALLOC();
		}

		value = emalloc(*value_length);
		*value_length = GetEnvironmentVariableA(name, value, *value_length);

		return value;
	}
	#else
	{
		char *val = getenv(name);

		if(val){
			value_length = strlen(val);
			return estrndup(val, *value_length);
		}
	}
	#endif

	return NULL;
}

BUESSION_API char *buession_get_scheme(uint *scheme_length TSRMLS_DC){
	if(buession_is_http(TSRMLS_C) == TRUE){
		zval *scheme = buession_get_server_value("SCHEME", 6 TSRMLS_CC);

		if(scheme&&Z_TYPE_P(scheme) == IS_STRING){
			*scheme_length = Z_STRLEN_P(scheme);
			return zend_str_tolower_dup(Z_STRVAL_P(scheme), *scheme_length);
		}else{
			zval *portocol = buession_get_server_value(ZEND_STRL("SERVER_PROTOCOL") TSRMLS_CC);

			if(portocol&&Z_TYPE_P(portocol) == IS_STRING){
				char *p = (char *) memchr(Z_STRVAL_P(portocol), '/', Z_STRLEN_P(portocol));

				*scheme_length = p ? Z_STRLEN_P(portocol) - (p - Z_STRVAL_P(portocol)) : Z_STRLEN_P(portocol);
				return zend_str_tolower_dup(Z_STRVAL_P(portocol), *scheme_length);
			}
		}
	}

	*scheme_length = 4;
	return estrndup("file", 4);
}
BUESSION_API zend_bool buession_is_ssl(char *scheme, uint scheme_length TSRMLS_DC){
	if(!scheme||scheme_length == 0){
		return FALSE;
	}

	return (scheme_length == 5&&strncasecmp(scheme, "https", 5) == 0)||(scheme_length == 4&&strncasecmp(scheme, "ftps", 4) == 0);
}

BUESSION_API zend_bool buession_is_http(TSRMLS_D){
	uint name_length = buession_get_sapi_module_name_length();

	switch(name_length){
		case 3:
			return memcmp(sapi_module.name, "cgi", 3) == 0||memcmp(sapi_module.name, "cli", 3) == 0 ? FALSE : TRUE;
			break;
		case 8:
			return memcmp(sapi_module.name, "cgi-fcgi", 8) == 0 ? FALSE : TRUE;
			break;
		default:
			break;
	}

	return TRUE;
}
BUESSION_API zend_bool buession_is_cli(TSRMLS_D){
	uint name_length = buession_get_sapi_module_name_length();
	return name_length == 3&&memcmp(sapi_module.name, "cli", 3) == 0;
}
BUESSION_API zend_bool buession_is_cgi(TSRMLS_D){
	uint name_length = buession_get_sapi_module_name_length();
	return (name_length == 3&&memcmp(sapi_module.name, "cgi", 3) == 0)||(name_length == 8&&memcmp(sapi_module.name, "cgi-fcgi", 8) == 0);
}

BUESSION_API zend_bool buession_is_default_port(const char *scheme, uint scheme_length, uint port TSRMLS_DC){
	return buession_get_default_port(scheme, scheme_length TSRMLS_CC) == port;
}
BUESSION_API zend_ushort buession_get_default_port(const char *scheme, uint scheme_length TSRMLS_DC){
	const port_entity *e;
	uint i = 0;

	if(!scheme||scheme_length == 0){
		return 0;
	}

	for(; (e = &port_entities[i])->scheme != NULL; i++){
		if(zend_binary_strcasecmp(scheme, scheme_length, e->scheme, e->scheme_length) == 0){
			return e->value;
		}
	}

	return 0;
}

BUESSION_API const char *buession_get_request_method(uint *method_length TSRMLS_DC){
	if(SG(request_info).request_method){
		*method_length = strlen(SG(request_info).request_method);
		return SG(request_info).request_method;
	}else{
		if(buession_is_cli(TSRMLS_C) == TRUE){
			*method_length = 3;
			return "cli";
		}else if(buession_is_cgi(TSRMLS_C) == TRUE){
			*method_length = 3;
			return "cgi";
		}
	}

	return NULL;
}

BUESSION_API char *buession_get_client_ip(uint *ip_length TSRMLS_DC){
	char *params[] = {"HTTP_CLIENT_IP", "HTTP_X_FORWARDED_FOR", "REMOTE_ADDR"};
	uint i = 0;
	char *ip = NULL;

	for(; i < 3; i++){
		if((ip = buession_get_env(ZEND_STRL(params[i]), ip_length TSRMLS_CC)) != NULL){
			break;
		}
	}

	*ip_length = 9;

	return estrndup("127.0.0.1", 9);
}
/* env function end */

BUESSION_API zend_bool buession_extension_loaded(const char *name TSRMLS_DC){
	return buession_extension_loaded_ex(name, strlen(name) TSRMLS_CC);
}
BUESSION_API zend_bool buession_extension_loaded_ex(const char *name, uint length TSRMLS_DC){
	char *lcname = zend_str_tolower_dup(name, length);
	zend_bool result = zend_hash_exists(&module_registry, lcname, length + 1);

	buession_free(lcname);

	return result;
}

BUESSION_API void buession_json_encode(zval *data, smart_str *result TSRMLS_DC){
	php_json_encode(result, data, 0 TSRMLS_CC);
	smart_str_0(result);
}

BUESSION_API int buession_hashcode(char *str, char *algo, char **result, uint *result_length TSRMLS_DC){
	return buession_hashcode_ex(str, strlen(str), algo, strlen(algo), result, result_length TSRMLS_CC);
}
BUESSION_API int buession_hashcode_ex(char *str, uint str_length, char *algo, uint algo_length, char **result, uint *result_length TSRMLS_DC){
	char *digest;
	const php_hash_ops *ops = php_hash_fetch_ops(algo, algo_length);
	void *context;
	char *ret;

	if(!ops){
		zval *exception = buession_throw_exception_error_ex(invalidexception_ce, E_WARNING, "Unknown hashing algorithm: %s", 0, algo);
		zval_ptr_dtor(&exception);
		return FAILURE;
	}

	context = emalloc(ops->context_size);
	ops->hash_init(context);
	ops->hash_update(context, (unsigned char *) str, str_length);
	digest = emalloc(ops->digest_size + 1);
	ops->hash_final((unsigned char *) digest, context);
	buession_free(context);

	ret = safe_emalloc(ops->digest_size, 2, 1);
	php_hash_bin2hex(ret, (unsigned char *) digest, ops->digest_size);
	ret[2 * ops->digest_size] = 0;
	*result = ret;
	*result_length = strlen(ret);
	buession_free(digest);

	return SUCCESS;

}

/* file system object start */
BUESSION_API zend_bool buession_check_folder_exists(char *path, uint path_length TSRMLS_DC){
	zval stat;

	php_stat(path, path_length, FS_EXISTS, &stat TSRMLS_CC);
	if(Z_TYPE(stat) == IS_BOOL&&Z_BVAL(stat) == TRUE){
		php_stat(path, path_length, FS_IS_DIR, &stat TSRMLS_CC);
		return Z_TYPE(stat) == IS_BOOL ? Z_BVAL(stat) : FALSE;
	}

	return FALSE;
}
BUESSION_API zend_bool buession_check_file_exists(char *path, uint path_length TSRMLS_DC){
	zval stat;

	php_stat(path, path_length, FS_EXISTS, &stat TSRMLS_CC);
	if(Z_TYPE(stat) == IS_BOOL&&Z_BVAL(stat) == TRUE){
		php_stat(path, path_length, FS_IS_FILE, &stat TSRMLS_CC);
		return Z_TYPE(stat) == IS_BOOL ? Z_BVAL(stat) : FALSE;
	}

	return FALSE;
}

BUESSION_API int buession_get_fso_real_path(char *path, uint path_length, char **real_path, uint *real_path_length TSRMLS_DC){
	char real_path_buffer[MAXPATHLEN];

	if(VCWD_REALPATH(path, real_path_buffer)){
		if(PG(safe_mode) == TRUE&&(!php_checkuid(real_path_buffer, NULL, CHECKUID_CHECK_FILE_AND_DIR))){
			return FAILURE;
		}

		if(php_check_open_basedir(real_path_buffer TSRMLS_CC)){
			return FAILURE;
		}

		#ifdef ZTS
			if(VCWD_ACCESS(real_path_buffer, F_OK)){
				return FAILURE;
			}
		#endif

		*real_path = (unsigned char *) real_path_buffer;
		*real_path_length = strlen(*real_path);

		return SUCCESS;
	}

	return FAILURE;
}
BUESSION_API int buession_get_fso_parent_path(char *path, uint path_length, char **parent_path, uint *parent_path_length TSRMLS_DC){
	char *_path = estrndup(path, path_length);

	zend_dirname(_path, path_length);
	*parent_path_length = spprintf(parent_path, 0, "%s%c", _path, DEFAULT_SLASH);
	buession_free(_path);

	return SUCCESS;
}

BUESSION_API int buession_folder_create(char *path, uint path_length, int mode, zend_bool overwrite TSRMLS_DC){
	zval *zcontext = NULL;
	php_stream_context *context = php_stream_context_from_zval(zcontext, 0);

	return buession_folder_create_ex(path, path_length, mode, overwrite, context TSRMLS_CC);
}
BUESSION_API buession_folder_create_ex(char *path, uint path_length, int mode, zend_bool overwrite, php_stream_context *context TSRMLS_DC){
	if(buession_check_folder_exists(path, path_length TSRMLS_CC) == TRUE){
		if(overwrite == TRUE){
			return SUCCESS;
		}else{
			buession_throw_exception_ex(ioexception_ce, E_WARNING, "Directory '%s' already exists", 0, path);
			return FAILURE;
		}
	}else{
		return php_stream_mkdir(path, mode, PHP_STREAM_MKDIR_RECURSIVE|REPORT_ERRORS, context) == TRUE ? SUCCESS : FAILURE;
	}
}

BUESSION_API zend_bool buession_fso_isReadable(char *path, uint path_length TSRMLS_DC){
	fso_check_permission_fn(path, path_length, FS_IS_R);
	return FALSE;
}
BUESSION_API zend_bool buession_fso_isWritable(char *path, uint path_length TSRMLS_DC){
	fso_check_permission_fn(path, path_length, FS_IS_W);
	return FALSE;
}
BUESSION_API zend_bool buession_fso_isExecutable(char *path, uint path_length TSRMLS_DC){
	fso_check_permission_fn(path, path_length, FS_IS_X);
	return FALSE;
}

BUESSION_API int buession_file_read(char *path, uint path_length, size_t offset, size_t max_size, char **content, size_t *content_length TSRMLS_DC){
	zval *zcontext = NULL;
	php_stream_context *context = php_stream_context_from_zval(zcontext, 0);

	return buession_file_read_ex(path, path_length, offset, max_size, context, content, content_length TSRMLS_CC);
}
BUESSION_API int buession_file_read_ex(char *path, uint path_length, size_t offset, size_t max_size, php_stream_context *context, char **content, size_t *content_length TSRMLS_DC){
	if(buession_check_file_exists(path, path_length TSRMLS_CC) == TRUE){
		php_stream *stream = php_stream_open_wrapper_ex(path, "rb", ENFORCE_SAFE_MODE|REPORT_ERRORS, NULL, context);

		if(stream){
			if(offset > 0&&php_stream_seek(stream, offset, SEEK_SET) < 0){
				zval *exception = buession_throw_exception_error_ex(illegalexception_ce, E_WARNING, "Failed to seek to position %ld in the stream", 0, offset);

				zval_ptr_dtor(&exception);
				buession_stream_free(stream);

				return FAILURE;
			}

			if((*content_length = php_stream_copy_to_mem(stream, content, max_size, FALSE)) <= 0){
				*content = STR_EMPTY_ALLOC();
				*content_length = 0;
			}
			buession_stream_free(stream);

			return SUCCESS;
		}
	}else{
		buession_file_not_exists_exception(path, E_WARNING);
	}

	return FAILURE;
}

BUESSION_API int buession_file_write(char *path, uint path_length, char *str, size_t str_length, long flag, size_t *result_size TSRMLS_DC){
	zval *zcontext = NULL;
	php_stream_context *context = php_stream_context_from_zval(zcontext, flag&PHP_FILE_NO_DEFAULT_CONTEXT);

	return buession_file_write_ex(path, path_length, str, str_length, flag, context, result_size TSRMLS_CC);
}
BUESSION_API int buession_file_write_ex(char *path, uint path_length, char *str, size_t str_length, long flag, php_stream_context *context, size_t *result_size TSRMLS_DC){
	php_stream *stream;
	char *parent_path = NULL;
	uint parent_path_length = 0;
	char mode[3] = "wb";
	zval *exception = NULL;

	*result_size = -1;

	if(flag&PHP_FILE_APPEND){
		mode[0] = 'a';
	}else if(flag&PHP_LOCK_EX){
		/* check to make sure we are dealing with a regular file */
		if(zend_memnstr(path, "://", 3, path + path_length)){
			if(strncasecmp(path, "file://", 7)){
				exception = buession_throw_exception(illegalexception_ce, E_WARNING, ZEND_STRL("Exclusive locks may only be set for regular files"), 0);
				goto failure;
			}
		}

		mode[0] = 'c';
	}
	mode[2] = '\0';

	if(buession_get_fso_parent_path(path, path_length, &parent_path, &parent_path_length TSRMLS_CC) == FAILURE){
		buession_free(parent_path);
		exception = buession_throw_exception_error(ioexception_ce, E_WARNING, ZEND_STRL("Get parent directory failure"), 0);
		goto failure;
	}

	if(buession_check_folder_exists(parent_path, parent_path_length TSRMLS_CC) == FALSE){
		if(buession_folder_create(parent_path, parent_path_length, 0644, TRUE TSRMLS_CC) == FAILURE){
			exception = buession_throw_exception_error_ex(ioexception_ce, E_WARNING, "Create parent directory '%s' failure", 0, parent_path);
			buession_free(parent_path);
			goto failure;
		}
	}
	buession_free(parent_path);

	if((stream = php_stream_open_wrapper_ex(path, mode, ((flag&PHP_FILE_USE_INCLUDE_PATH) ? USE_PATH : 0)|ENFORCE_SAFE_MODE|REPORT_ERRORS, NULL, context)) == NULL){
		php_stream_context_free(context);
		return FAILURE;
	}

	if((flag&PHP_LOCK_EX)&&(!php_stream_supports_lock(stream)||php_stream_lock(stream, PHP_LOCK_EX))){
		php_stream_close(stream);
		exception = buession_throw_exception_error(ioexception_ce, E_WARNING, ZEND_STRL("Exclusive locks are not supported for this stream"), 0);
		goto failure;
	}

	if(mode[0] == 'c'){
		php_stream_truncate_set_size(stream, 0);
	}

	*result_size = str != NULL ? php_stream_write(stream, str, str_length) : 0;
	buession_stream_free(stream);

	if(*result_size == str_length){
		return SUCCESS;
	}else{
		exception = buession_throw_exception_error_ex(ioexception_ce, E_WARNING, "Only %d of %d bytes written, possibly out of free disk space", 0, *result_size, str_length);
	}

	failure:
		if(exception != NULL){
			zval_ptr_dtor(&exception);
		}
		return FAILURE;
}

BUESSION_API int buession_file_delete(char *path, uint path_length TSRMLS_DC){
	zval *zcontext = NULL;
	php_stream_context *context = php_stream_context_from_zval(zcontext, 0);

	return buession_file_delete_ex(path, path_length, context TSRMLS_CC);
}
BUESSION_API int buession_file_delete_ex(char *path, uint path_length, php_stream_context *context TSRMLS_DC){
	php_stream_wrapper *wrapper = php_stream_locate_url_wrapper(path, NULL, 0 TSRMLS_CC);

	if(!wrapper||!wrapper->wops){
		buession_fso_illegal_stream_wrapper_exception(NULL);
		return FAILURE;
	}

	if(!wrapper->wops->unlink){
		zval *exception = buession_throw_exception_error_ex(ioexception_ce, E_WARNING, "%s does not allow unlinking", 0, wrapper->wops->label ? wrapper->wops->label : "Wrapper");

		zval_ptr_dtor(&exception);

		return FAILURE;
	}

	return wrapper->wops->unlink(wrapper, path, ENFORCE_SAFE_MODE|REPORT_ERRORS, context TSRMLS_CC) == TRUE ? SUCCESS : FAILURE;
}

BUESSION_API int buession_folder_clear(char *path, uint path_length TSRMLS_DC){
	zval *zcontext = NULL;
	php_stream_context *context = php_stream_context_from_zval(zcontext, 0);

	return buession_folder_clear_ex(path, path_length, context TSRMLS_CC);
}
BUESSION_API int buession_folder_clear_ex(char *path, uint path_length, php_stream_context *context TSRMLS_DC){
	php_stream *stream = php_stream_opendir(path, ENFORCE_SAFE_MODE|REPORT_ERRORS, context);
	php_stream_dirent dirent;
	int result = FAILURE;

	if(stream == NULL){
		goto finish;
	}

	stream->flags |= PHP_STREAM_FLAG_NO_FCLOSE;
	if(!(stream->flags&PHP_STREAM_FLAG_IS_DIR)){
		goto finish;
	}

	while(php_stream_readdir(stream, &dirent) != NULL){
		char *child_path = NULL;
		uint child_path_length = 0;
		zval stat;

		if(strcmp(dirent.d_name, ".") == 0||strcmp(dirent.d_name, "..") == 0){
			continue;
		}

		child_path_length = spprintf(&child_path, 0, "%s%c%s", path, DEFAULT_SLASH, dirent.d_name);
		php_stat(child_path, child_path_length, FS_IS_DIR, &stat TSRMLS_CC);
		if(Z_TYPE(stat) == IS_BOOL&&Z_BVAL(stat) == TRUE){
			result = buession_folder_delete_ex(child_path, child_path_length, context TSRMLS_CC);
		}else{
			result = buession_file_delete_ex(child_path, child_path_length, context TSRMLS_CC);
		}
		buession_free(path);
	}

	finish:
		buession_stream_free(stream);
		//php_stream_context_free(context);

		return result;
}
BUESSION_API int buession_folder_delete(char *path, uint path_length TSRMLS_DC){
	zval *zcontext = NULL;
	php_stream_context *context = php_stream_context_from_zval(zcontext, 0);

	return buession_folder_delete_ex(path, path_length, context TSRMLS_CC);
}
BUESSION_API int buession_folder_delete_ex(char *path, uint path_length, php_stream_context *context TSRMLS_DC){
	if(buession_folder_clear_ex(path, path_length, context TSRMLS_CC) == SUCCESS){
		return php_stream_rmdir(path, REPORT_ERRORS, context) == TRUE ? SUCCESS : FAILURE;
	}

	return FAILURE;
}
/* file system object end */

/* ini alert start */
BUESSION_API int buession_alert_ini_entry(char *name, uint name_length, zval *value, int modify_type, int stage TSRMLS_DC){
	return buession_alert_ini_entry_ex(name, name_length, value, modify_type, stage, FALSE TSRMLS_CC);
}
BUESSION_API int buession_alert_ini_entry_ex(char *name, uint name_length, zval *value, int modify_type, int stage, zend_bool force TSRMLS_DC){
	switch(Z_TYPE_P(value)){
		case IS_STRING:
			return zend_alter_ini_entry_ex(name, name_length, Z_STRVAL_P(value), Z_STRLEN_P(value), modify_type, stage, force TSRMLS_CC);
			break;
		case IS_LONG:
			return zend_alter_ini_long_entry_ex(name, name_length, Z_LVAL_P(value), modify_type, stage, force TSRMLS_CC);
			break;
		case IS_DOUBLE:
			return zend_alter_ini_double_entry_ex(name, name_length, Z_BVAL_P(value), modify_type, stage, force TSRMLS_CC);
			break;
		case IS_BOOL:
			return zend_alter_ini_bool_entry_ex(name, name_length, Z_BVAL_P(value), modify_type, stage, force TSRMLS_CC);
			break;
		case IS_NULL:
			return zend_alter_ini_null_entry_ex(name, name_length, modify_type, stage, force TSRMLS_CC);
			break;
		default:
			break;
	}

	return FAILURE;
}

BUESSION_API int zend_alter_ini_long_entry(char *name, uint name_length, long value, int modify_type, int stage TSRMLS_DC){
	return zend_alter_ini_long_entry_ex(name, name_length, value, modify_type, stage, FALSE TSRMLS_CC);
}
BUESSION_API int zend_alter_ini_long_entry_ex(char *name, uint name_length, long value, int modify_type, int stage, zend_bool force TSRMLS_DC){
	char *str_value;
	uint str_value_length = spprintf(&str_value, 0, "%ld", value);
	int result = zend_alter_ini_entry_ex(name, name_length, str_value, str_value_length, modify_type, stage, force TSRMLS_CC);

	buession_free(str_value);
	return result;
}

BUESSION_API int zend_alter_ini_double_entry(char *name, uint name_length, double value, int modify_type, int stage TSRMLS_DC){
	return zend_alter_ini_double_entry_ex(name, name_length, value, modify_type, stage, FALSE TSRMLS_CC);
}
BUESSION_API int zend_alter_ini_double_entry_ex(char *name, uint name_length, double value, int modify_type, int stage, zend_bool force TSRMLS_DC){
	char *str_value;
	uint str_value_length = spprintf(&str_value, 0, "%.*G", (int) EG(precision), value);
	int result = zend_alter_ini_entry_ex(name, name_length, str_value, str_value_length, modify_type, stage, force TSRMLS_CC);

	buession_free(str_value);
	return result;
}

BUESSION_API int zend_alter_ini_bool_entry(char *name, uint name_length, zend_bool value, int modify_type, int stage TSRMLS_DC){
	return zend_alter_ini_bool_entry_ex(name, name_length, value, modify_type, stage, FALSE TSRMLS_CC);
}
BUESSION_API int zend_alter_ini_bool_entry_ex(char *name, uint name_length, zend_bool value, int modify_type, int stage, zend_bool force TSRMLS_DC){
	return zend_alter_ini_entry_ex(name, name_length, BUESSION_BOOL_TO_INT(value), 1, modify_type, stage, force TSRMLS_CC);
}

BUESSION_API int zend_alter_ini_null_entry(char *name, uint name_length, int modify_type, int stage TSRMLS_DC){
	return zend_alter_ini_null_entry_ex(name, name_length, modify_type, stage, FALSE TSRMLS_CC);
}
BUESSION_API int zend_alter_ini_null_entry_ex(char *name, uint name_length, int modify_type, int stage, zend_bool force TSRMLS_DC){
	return zend_alter_ini_entry_ex(name, name_length, "", 0, modify_type, stage, force TSRMLS_CC);
}

BUESSION_API zend_bool zend_ini_bool(char *name, uint name_length, int orig TSRMLS_DC){
	return (zend_bool) zend_ini_long(name, name_length, orig);
}
/* ini alert end */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */