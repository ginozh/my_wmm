#!/bin/bash

<<COMMENT
@author: ginozhang
@date: 2017-06-14
@description: 
COMMENT

find . -name '*.c' -o -name '*.h' | xargs.exe egrep "\bstorm\b" | awk -F':' '{print $1}' | sort -u |while read file;do 
    echo "file: $file";
    #diff $file /c/shareproject/ffmpeg-3.0.2/$file;
    diff $file /c/qtproject/ffmpeg-3.3/$file;
done
