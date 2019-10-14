#!/bin/bash

<<COMMENT
@author: storm
@date: 2017-11-04
@description:  ldd在shell脚本环境下有时无效，只能在shell下运行
COMMENT

exe="effectadvancequick.exe";
#exe="mmaker.exe";
rm -fr deploy/;mkdir deploy/;
#cp debug/$exe deploy/;
#<<COMMENT
make -f Makefile.Release;
cp release/$exe deploy/;
#COMMENT
<<COMMENT
cd ../src;make -f Makefile.Debug; cd -
make -f Makefile.Debug;
cp debug/$exe deploy/;
COMMENT

cd deploy/;

# 人工在shell命令下输入
# ldd.exe release/effectadvancequick.exe |egrep -v 'Windows|\bProgram\b|shareproject' | awk -F'=>' '{print 
# $2}' |awk -F'(' '{print $1}'|while read file; do name=$(echo "$file" | awk -F'/' '{print $NF}');
# echo "cp $file ./"; done
cp /mingw64/bin/libstdc++-6.dll ./
cp /mingw64/bin/libgcc_s_seh-1.dll ./
cp /mingw64/bin/Qt5Core.dll ./
cp /mingw64/bin/Qt5Gui.dll ./
cp /mingw64/bin/Qt5Qml.dll ./
cp /mingw64/bin/Qt5Gui.dll ./
cp /mingw64/bin/Qt5Gui.dll ./
cp /mingw64/bin/zlib1.dll ./
cp /mingw64/bin/libicuin58.dll ./
cp /mingw64/bin/libwinpthread-1.dll ./
cp /mingw64/bin/libicuuc58.dll ./
cp /mingw64/bin/libpcre2-16-0.dll ./
cp /mingw64/bin/zlib1.dll ./
cp /mingw64/bin/Qt5Network.dll ./
cp /mingw64/bin/libicudt58.dll ./
cp /mingw64/bin/zlib1.dll ./
cp /mingw64/bin/Qt5Gui.dll ./
cp /mingw64/bin/Qt5Quick.dll ./
cp /mingw64/bin/Qt5Widgets.dll ./
cp /mingw64/bin/zlib1.dll ./
cp /mingw64/bin/libharfbuzz-0.dll ./
cp /mingw64/bin/libpng16-16.dll ./
cp /mingw64/bin/libfreetype-6.dll ./
cp /mingw64/bin/libglib-2.0-0.dll ./
cp /mingw64/bin/libgraphite2.dll ./
cp /mingw64/bin/libintl-8.dll ./
cp /mingw64/bin/libpcre-1.dll ./
cp /mingw64/bin/libiconv-2.dll ./
cp /mingw64/bin/libfreetype-6.dll ./
cp /mingw64/bin/libpcre-1.dll ./
cp /mingw64/bin/libbz2-1.dll ./
cp '/c/Program Files/Wondershare/Filmora9/opengl32sw.dll' ./

#--debug --release
windeployqt.exe $exe
windeployqt.exe --qmldir .. --angle $exe

cp /c/msys64/mingw64/share/qt5/plugins/platforms/qwindows.dll platforms/

cd ..
tar -zcf deploy.tgz deploy/
