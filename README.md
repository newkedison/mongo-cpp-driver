mongo-cpp-driver [![Build Status](https://travis-ci.org/newkedison/mongo-cpp-driver.png?branch=master)](https://travis-ci.org/newkedison/mongo-cpp-driver)
================

Since [c++ driver for mongodb](http://dl.mongodb.org/dl/cxx-driver/) is develop with mongodb itself, it can be up to date and more quickly.

But it has some disadvantage in my own opinion:

* depend on [Scons](http://www.scons.org/)

  Scons is a good tool but I prefer the Makefile-style build system
  
* depend on [boost](http://www.boost.org/)

  Many useful classes in boost is available in C++11 standard, it's no need to include such a big library in the project

* cannot be compile with mingw on Windows

  In windows, it can only be compile with Visual Studio family, which is commercial software
  
So, I want to develope a new c++ driver base on [mongo-c-driver](https://github.com/mongodb/mongo-c-driver).
This driver will only depends on C++11 standard and the gcc toolchain




[![Bitdeli Badge](https://d2weczhvl823v0.cloudfront.net/newkedison/mongo-cpp-driver/trend.png)](https://bitdeli.com/free "Bitdeli Badge")

