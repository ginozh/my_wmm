#!/bin/bash

<<COMMENT
@author: storm
@date: 2017-10-02
@description: 
COMMENT
#qtcreator use cmake: http://doc.qt.io/qtcreator/creator-project-cmake.html
#qtcreator debug: https://stackoverflow.com/questions/32560862/cant-debug-in-qt-creator-with-cmake-project

mkdir build;cd build;
#cmake -G "MinGW Makefiles" -DCMAKE_SH="CMAKE_SH-NOTFOUND" ../
cmake -G "MinGW Makefiles" -DCMAKE_SH="CMAKE_SH-NOTFOUND"  -Dfreetype-gl_BUILD_HARFBUZZ=ON -Dfreetype-gl_BUILD_DEMOS=ON ..
#ANT_TWEAK_BAR_INCLUDE_PATH=/c/qtproject/opengl/AntTweakBar/include/ ANT_TWEAK_BAR_LIBRARY=/c/qtproject/opengl/AntTweakBar/lib cmake -G "MinGW Makefiles" -DCMAKE_SH="CMAKE_SH-NOTFOUND"  -Dfreetype-gl_BUILD_HARFBUZZ=ON ..
cmake --build .
#mingw32-make;
#mingw32-make install;
