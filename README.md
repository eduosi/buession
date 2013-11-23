Introduction
------------
Buession Framework 是使用 C 语言编写的一款 PHP 扩展。


Features
--------
* ...


Requirement
------------
PHP Version >= 5.3


Installation
------------
Buession Framework can be downloaded at https://github.com/eduosi/buession/releases . 你也可以从 GitHub（https://github.com/eduosi/buession） checkout 最新代码。

Buession Framework install:
```
$ cd buession
$ php_installed_path/bin/phpize
$ ./configure --with-php-config=php_installed_path/bin/php-config --enable-buession-debug=1/0(可选，是否开启 DEBUG，默认未开启，正式环境不建议开启)
$ make
# make install
```

最后，在php.ini中加入extension=buession.so


Configuration
------------
```
buession.environment string default value is null。设置当前运行环境，可根据此值在开发、测试、正式环境时，加载不同的应用配置文件。
buession.classpath string default value is null。设置自定义类目录路径，默认为当前目录。
```

Documentation
-------------
编写中.