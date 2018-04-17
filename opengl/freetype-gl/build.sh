#!/bin/bash

<<COMMENT
@author: ginozhang
@date: 2017-10-02
@description: 
COMMENT

mkdir build;cd build;
#cmake -G "MinGW Makefiles" -DCMAKE_SH="CMAKE_SH-NOTFOUND" ../
cmake -G "MinGW Makefiles" -DCMAKE_SH="CMAKE_SH-NOTFOUND"  -Dfreetype-gl_BUILD_HARFBUZZ=ON -Dfreetype-gl_BUILD_DEMOS=ON ..
#ANT_TWEAK_BAR_INCLUDE_PATH=/c/qtproject/opengl/AntTweakBar/include/ ANT_TWEAK_BAR_LIBRARY=/c/qtproject/opengl/AntTweakBar/lib cmake -G "MinGW Makefiles" -DCMAKE_SH="CMAKE_SH-NOTFOUND"  -Dfreetype-gl_BUILD_HARFBUZZ=ON ..
mingw32-make;
#mingw32-make install;
