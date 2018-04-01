#!/bin/bash

<<COMMENT
@author: ginozhang
@date: 2017-10-02
@description: 
COMMENT

mkdir build;cd build;
cmake -G "MinGW Makefiles" -DCMAKE_SH="CMAKE_SH-NOTFOUND" ../
mingw32-make;
mingw32-make install;
