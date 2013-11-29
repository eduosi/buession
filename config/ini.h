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

/* $Id: config/ini.h 310447 2011-12-22 19:54:34Z Teng Yong $ */

#ifndef BUESSION_CONFIG_INI_H
#define BUESSION_CONFIG_INI_H

extern zend_class_entry *config_ini_ce;

#define BUESSION_CONFIG_INI_ME(method, arguments, flags) ZEND_ME(buession_config_ini, method, arguments, flags)

#define config_ini_add_empty_array(array, key, key_length, pDest)	\
	do{	\
		zval *value;	\
		BUESSION_ARRAY_INIT(value);	\
		zend_symtable_update(Z_ARRVAL_P(array), (key), (key_length), (void **) &(value), sizeof(zval *), (pDest));	\
	}while(0);

#define config_ini_parse_value(value, result)	\
	do{	\
		if(Z_TYPE_P(value) == IS_STRING){	\
			zval *exception = NULL;	\
			char ch = *(Z_STRVAL_P(value));	\
			char last_ch = Z_STRVAL_P(value)[Z_STRLEN_P(value) - 1];	\
			if(ch == '"'){	\
				if(last_ch == '"'){	\
					goto remove_value_quote;	\
				}else{	\
					goto parse_error;	\
				}	\
			}else if(ch == '\''){	\
				if(last_ch == '\''){	\
					remove_value_quote:	\
					MAKE_STD_ZVAL(result);	\
					ZVAL_STRINGL(result, Z_STRVAL_P(value) + 1, Z_STRLEN_P(value) - 2, TRUE);	\
				}else{	\
					parse_error:	\
					exception = buession_throw_exception_error_ex(parseexception_ce, E_ERROR, "parse error", 0);	\
					zval_ptr_dtor(&exception);	\
				}	\
			}else{	\
				result = buession_string_convert_original_ex(Z_STRVAL_P(value), Z_STRLEN_P(value) TSRMLS_CC);	\
			}	\
		}else{	\
			BUESSION_COPY_ZVAL_P(result, value);	\
		}	\
	}while(0);

BUESSION_STARTUP_FUNCTION(config_ini);
BUESSION_RINIT_FUNCTION(config_ini);
BUESSION_RSHUTDOWN_FUNCTION(config_ini);
BUESSION_SHUTDOWN_FUNCTION(config_ini);

#endif	/* BUESSION_CONFIG_INI_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
