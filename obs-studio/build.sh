#!/bin/bash

<<COMMENT
@author: storm
@date: 2020-03-28
@description: 
COMMENT
rm -fr build
mkdir -p build
cd build
cmake -DBUILD_TESTS=TRUE -DCMAKE_BUILD_TYPE=DEBUG -DCMAKE_C_FLAGS="-g -o0" -DCMAKE_CXX_FLAGS="-g -o0" ..
make
