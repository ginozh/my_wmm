#!/bin/bash

<<COMMENT
@author: storm
@date: 2018-10-16
@description: 
COMMENT

./configure --enable-test --disable-fontconfig CFLAGS="-O0 -g" CPPFLAGS="-O0 -g"
make V=1 -j10
