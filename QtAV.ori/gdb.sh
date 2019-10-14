#!/bin/bash

<<COMMENT
@author: storm
@date: 2019-09-13
@description: 
COMMENT

gdb --batch --command=test.gdb --args ./bin/framereader.exe
