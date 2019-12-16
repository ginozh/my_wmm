#!/bin/bash

<<COMMENT
@author: storm
@date: 2017-08-22
@description: 
COMMENT

#g++ -LC:/msys32/mingw32/bin `pkg-config --cflags --libs opencv` -o hello main.cpp
g++ -IC:/msys64/mingw64/include/opencv -IC:/msys64/mingw64/include -LC:/msys64/mingw64/bin \
    -lopencv_imgcodecs340 -lopencv_highgui340 -lopencv_core340 -o hello main.cpp
