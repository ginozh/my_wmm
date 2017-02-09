#!/bin/bash

<<COMMENT
@author: ginozhang
@date: 2017-02-09
@description: 
COMMENT

cd /c/qtproject/ffmpeg-3.0.2
find . -name '*.dll' | while read file; do cp $file /c/shareproject/moviemaker/lib_ffmpeg/lib/;done
find . -name "*.h" | while read file;do dir="/c/shareproject/moviemaker/lib_ffmpeg/header/$file";mkdir -p $dir;echo "$dir";rm -fr $dir;cp $file $dir;done
