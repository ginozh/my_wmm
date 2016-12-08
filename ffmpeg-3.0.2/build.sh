#!/bin/bash

<<COMMENT
@author: ginozhang
@date: 2016-09-11
@description: 
COMMENT

#--prefix=/mingw32 --target-os=mingw32 --arch=i686 --enable-debug --disable-static --enable-avresample --enable-dxva2 --enable-d3d11va --enable-fontconfig --enable-gnutls --enable-gpl --enable-libass --enable-libbluray --enable-libcaca --enable-libcelt --enable-libdcadec --enable-libfreetype --enable-libgsm --enable-libmodplug --enable-libmp3lame --enable-libopencore_amrnb --enable-libopencore_amrwb --enable-libopus --enable-librtmp --enable-libschroedinger --enable-libspeex --enable-libtheora --enable-libvorbis --enable-libx264 --enable-libx265 --enable-libxvid --enable-libvpx --enable-openal --enable-libwavpack --enable-pic --enable-postproc --enable-runtime-cpudetect --enable-shared --enable-static --enable-swresample --enable-version3 --enable-zlib --disable-doc --disable-pthreads
config=$(ffmpeg.exe -h 2>&1| egrep "configuration:" | awk -F':' '{print $2}')
./configure $config --disable-pthreads --disable-w32threads --disable-os2threads --extra-ldflags=-g
#修改config.mak
# rm O3
#HOSTCFLAGS=   -O3
#CFLAGS= -O3


#以下无用
#修改config.mak
#1, rm O3
#2, #define HAVE_W32THREADS 0
#3, #define HAVE_THREADS 0
#4, #define CONFIG_FRAME_THREAD_ENCODER 0


make -j10
