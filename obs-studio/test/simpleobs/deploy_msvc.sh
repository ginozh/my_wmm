#!/bin/bash

<<COMMENT
@author: storm
@date: 2020-04-21
@description: 
COMMENT

cp ../../build/x64/Debug/obs.dll debug/
cp ../../build/x64/Debug/w32-pthreads.dll debug/
cp ../../build/x64/Debug/zlibd1.dll debug/

echo "run the follow cmd in 'Qt 5.10.0 64-bit for Desktop(MSVC 2017)'";
#echo "windeployqt --qmldir .. effectadvancequick.exe";
echo "cd c:\\qtproject\\obs-studio\\test\\simpleobs\\debug\\"
echo "windeployqt myobs.exe";
