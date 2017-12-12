#!/bin/bash

<<COMMENT
@author: storm
@date: 2016-09-11
@description: 
COMMENT

#--prefix=/mingw32 --target-os=mingw32 --arch=i686 --enable-debug --disable-static --enable-avresample --enable-dxva2 --enable-d3d11va --enable-fontconfig --enable-gnutls --enable-gpl --enable-libass --enable-libbluray --enable-libcaca --enable-libcelt --enable-libdcadec --enable-libfreetype --enable-libgsm --enable-libmodplug --enable-libmp3lame --enable-libopencore_amrnb --enable-libopencore_amrwb --enable-libopus --enable-librtmp --enable-libschroedinger --enable-libspeex --enable-libtheora --enable-libvorbis --enable-libx264 --enable-libx265 --enable-libxvid --enable-libvpx --enable-openal --enable-libwavpack --enable-pic --enable-postproc --enable-runtime-cpudetect --enable-shared --enable-static --enable-swresample --enable-version3 --enable-zlib --disable-doc --disable-pthreads

#need --enable-libfaac --enable-nonfree
#build libfaac: https://pcloadletter.co.uk/2011/05/07/compiling-64bit-ffmpeg-for-windows-with-libfaac/
#   CFLAGS="-m32" CXXFLAGS="-m32" ./configure --prefix=/mingw32 --build=x86_64-w64-mingw32 --enable-static --disable-shared --with-mp4v2=no
config=$(./ffmpeg.exe -h 2>&1| egrep "configuration:" | awk -F':' '{print $2}')
echo "$config";
echo "$config" > tmp.log;
##./configure $config --disable-pthreads --disable-w32threads --disable-os2threads --extra-ldflags=-g
#1, 可能出现"ERROR: caca not found using pkg-config" 忽略
#2, debug版本修改config.mak
# rm O3
#HOSTCFLAGS=   -O3
#CFLAGS= -O3


#以下无用
#修改config.mak
#1, rm O3
#2, #define HAVE_W32THREADS 0
#3, #define HAVE_THREADS 0
#4, #define CONFIG_FRAME_THREAD_ENCODER 0


##make -j10
