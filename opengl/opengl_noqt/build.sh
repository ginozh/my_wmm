#!/bin/bash

<<COMMENT
@author: storm
@date: 2017-10-05
@description: 
COMMENT

g++ -c -DBUILDING_EXAMPLE_DLL opengl.cpp
#g++ -shared -Wl,-subsystem,windows -mthreads -Wl,--out-implib,../lib_effectvideo/lib//libeffectvideo.dll.a
g++ -shared -o myopengl.dll opengl.o -Wl,-subsystem,windows -mthreads -Wl,--out-implib,libmyopengl.dll.a -lopengl32 /mingw32/lib/libglew32.a /mingw32/lib/libglfw3.a
#g++ -shared -o opengl.dll opengl.o -Wl,-subsystem,windows -mthreads -Wl,--out-implib,libopengl.dll.a /mingw32/i686-w64-mingw32/lib/libopengl32.a /mingw32/lib/libglew32.a  /mingw32/lib/libglfw3.a
