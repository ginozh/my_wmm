#!/bin/bash

<<COMMENT
@author: storm
@date: 2017-02-09
@description: 
COMMENT

cd /c/qtproject/ffmpeg-3.0.2
make install
find . -name '*.dll' | while read file; do cp $file /c/msys32/mingw32/bin/;done
