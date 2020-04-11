#!/bin/bash

<<COMMENT
@author: storm
@date: 2020-04-11
@description: 
COMMENT

install_name_tool -change @rpath/libobs.0.dylib ../../build/rundir/DEBUG/bin/libobs.0.dylib myobs
./myobs
