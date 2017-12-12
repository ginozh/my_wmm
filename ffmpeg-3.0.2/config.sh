#!/bin/bash

<<COMMENT
@author: storm
@date: 2017-05-12
@description: 
COMMENT
common="--prefix=/mingw32 --target-os=mingw32 --arch=i686 --disable-static --enable-avresample --enable-dxva2 --enable-d3d11va --enable-fontconfig --enable-gnutls --enable-gpl --enable-libass --enable-libbluray --enable-libcaca --enable-libcelt --enable-libdcadec --enable-libfreetype --enable-libgsm --enable-libmodplug --enable-libmp3lame --enable-libopencore_amrnb --enable-libopencore_amrwb --enable-libopus --enable-librtmp --enable-libschroedinger --enable-libspeex --enable-libtheora --enable-libvorbis --enable-libx264 --enable-libx265 --enable-libxvid --enable-libvpx --enable-openal --enable-libwavpack --enable-pic --enable-postproc --enable-runtime-cpudetect --enable-shared --enable-static --enable-swresample --enable-version3 --enable-zlib --disable-doc --disable-pthreads --disable-pthreads --disable-w32threads --disable-os2threads --enable-libfaac --enable-nonfree"

#debug
./configure $common --extra-ldflags=-g


#release
#./configure --disable-debug $common
