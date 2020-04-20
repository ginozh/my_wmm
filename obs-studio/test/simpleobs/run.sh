#!/bin/bash

<<COMMENT
@author: storm
@date: 2020-04-11
@description: 
COMMENT

install_name_tool -change @rpath/libobs.0.dylib ../../build/rundir/DEBUG/bin/libobs.0.dylib myobs
<<COMMENT
mkdir -p ../../data/obs-studio;
cp -fr ../..//UI/data/* ../../data/obs-studio/
cp ../..//build/plugins/obs-ffmpeg/ffmpeg-mux/obs-ffmpeg-mux ./
cp ../..//CI/install/osx/Info.plist ./
COMMENT
./myobs
