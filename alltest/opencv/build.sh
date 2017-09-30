#!/bin/bash

<<COMMENT
@author: ginozhang
@date: 2017-08-22
@description: 
COMMENT

#g++ -LC:/msys32/mingw32/bin `pkg-config --cflags --libs opencv` -o hello main.cpp
g++ -IC:/msys32/mingw32/include/opencv -IC:/msys32/mingw32/include -LC:/msys32/mingw32/bin \
    -lopencv_imgcodecs320 -lopencv_highgui320 -lopencv_core320 -o hello main.cpp
