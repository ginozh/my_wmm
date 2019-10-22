#!/bin/bash

<<COMMENT
@author: storm
@date: 2017-11-04
@description:  ldd在shell脚本环境下有时无效，只能在shell下运行
COMMENT

exe="qglwidget_thead.exe";
#exe="mmaker.exe";
#cp debug/$exe deploy/;
#<<COMMENT
make -f Makefile.Release;
rm -fr deploy/;mkdir deploy/;
cp release/$exe deploy/;
#COMMENT
<<COMMENT
cd ../src;make -f Makefile.Debug; cd -
make -f Makefile.Debug;
cp debug/$exe deploy/;
COMMENT

cd deploy/;

# 人工在shell命令下输入
# ldd.exe release/qglwidget_thead.exe |egrep -vi 'Windows|\bProgram\b|shareproject' | awk -F'=>' '{print 
# $2}' |awk -F'(' '{print $1}'|while read file; do name=$(echo "$file" | awk -F'/' '{print $NF}');
# echo "cp $file ./"; done
cp /mingw32/bin/libgcc_s_dw2-1.dll ./
cp /mingw32/bin/libwinpthread-1.dll ./
cp /mingw32/bin/libstdc++-6.dll ./
cp /mingw32/bin/Qt5Core.dll ./
cp /mingw32/bin/libicuin58.dll ./
cp /mingw32/bin/libicuuc58.dll ./
cp /mingw32/bin/libicudt58.dll ./
cp /mingw32/bin/zlib1.dll ./
cp /mingw32/bin/Qt5Gui.dll ./
cp /mingw32/bin/libharfbuzz-0.dll ./
cp /mingw32/bin/libfreetype-6.dll ./
cp /mingw32/bin/libbz2-1.dll ./
cp /mingw32/bin/libpng16-16.dll ./
cp /mingw32/bin/libglib-2.0-0.dll ./
cp /mingw32/bin/libintl-8.dll ./
cp /mingw32/bin/libiconv-2.dll ./
cp /mingw32/bin/libpcre-1.dll ./
cp /mingw32/bin/libgraphite2.dll ./
cp /mingw32/bin/Qt5OpenGL.dll ./
cp /mingw32/bin/Qt5Widgets.dll ./

#
cp /mingw32/bin/libjpeg-8.dll ./

cp /c/shareproject/jpg/1.jpg ./
cp /c/shareproject/jpg/2.jpg ./


windeployqt.exe $exe

cp /c/msys32/mingw32/share/qt5/plugins/platforms/qwindows.dll platforms/

cd ..
tar -zcf deploy.tgz deploy/
