#!/bin/bash

<<COMMENT
@author: storm
@date: 2017-08-22
@description: 
COMMENT

#g++ -LC:/msys32/mingw32/bin `pkg-config --cflags --libs opencv` -o hello main.cpp
g++ -IC:/msys32/mingw32/include/opencv -IC:/msys32/mingw32/include -LC:/msys32/mingw32/bin \
    -lopencv_imgcodecs330 -lopencv_highgui330 -lopencv_core330 -o hello main.cpp
