#!/bin/bash

<<COMMENT
@author: storm
@date: 2016-11-11
@description: 
COMMENT

#make -f Makefile.Debug clean;
make -f Makefile.Debug;
cd debug/;
rm -fr deploy/;mkdir deploy/;
cp img2mov.exe deploy/;

cd deploy/;
#ldd img2mov.exe | egrep "Windows|mingw32|Program" | awk -F'=>' '{print $2}' |awk -F'(' '{print $1}' | while read file;do cp "$file" ./;done
ldd img2mov.exe | awk -F'=>' '{print $2}' |awk -F'(' '{print $1}' | while read file;do cp "$file" ./;done
windeployqt.exe img2mov.exe
cp -r ../../debug/images ./
cd ..

tar -zcf deploy.tgz deploy/
