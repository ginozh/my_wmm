#!/bin/bash

<<COMMENT
@author: ginozhang
@date: 2016-11-11
@description: 
COMMENT

#make -f Makefile.Release clean;
make -f Makefile.Release;
cd release/;
rm -fr deploy/;mkdir deploy/;
cp img2mov.exe deploy/;

cd deploy/;
ldd img2mov.exe | egrep "Windows|mingw32|Program" | awk -F'=>' '{print $2}' |awk -F'(' '{print $1}' | while read file;do cp "$file" ./;done
windeployqt.exe img2mov.exe
cp -r ../../debug/images ./
cd ..

tar -zcf deploy.tgz deploy/
