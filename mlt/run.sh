#!/bin/bash

<<COMMENT
@author: storm
@date: 2019-02-26
@description: 
COMMENT

#build
#sudo apt-get install libsdl1.2-dev frei0r-plugins-dev libgtk2.0-dev libexif-dev
#./configure --enable-debug --prefix=/usr

date;

#melt upan/jpg/WeChatSight2266.mp4 -consumer avformat:output.mp4 acodec=aac ab=256k vcodec=libx264 s=1280x720 r=20 
#   > logs/WeChatSight2266_2.log 2>&1
#melt /media/psf/Home/Desktop/upan/qqrtx/QQ/Thousand.Planets.mp4 -consumer avformat:output.mp4 vcodec=libx264 r=30

#melt 'Wildlife_0.mp4' -consumer avformat:'output.mp4' properties=x264-medium f=mp4 vcodec=libx264 acodec=aac g=120 crf=30 ab=96k preset=faster threads=4 real_time=-1 s=1280x720 r=30
#melt 'rgb 50-50-50 - Wildlife_0.mp4' -consumer 'avformat:rgb 50-50-50 - melt.bmp' vcodec=bmp

#in=0 out=49 和-filter greyscale 不能同时叠加
#melt Wildlife_0.mp4 -track /z/Desktop/upan/jpg/WeChatSight2266.mp4 in=0 out=49
#melt Wildlife_0.mp4 -track /z/Desktop/upan/jpg/WeChatSight2266.mp4 -filter greyscale
#melt /z/Desktop/upan/jpg/Wildlife.wmv -attach watermark:+hello.txt -attach invert -debug > log.log 2>&1
#melt /z/Desktop/upan/jpg/WeChatSight2266.mp4 -attach watermark:+hello.txt -attach invert -consumer avformat:output.avi acodec=libmp3lame vcodec=libx264 > log.log 2>&1
#melt /z/Desktop/upan/jpg/WeChatSight2266.mp4 -attach movit.blur -debug > log.log 2>&1  #wrong
#melt Wildlife_0.mp4 -track /z/Desktop/upan/jpg/WeChatSight2266.mp4 -attach glsl.manager -attach movit.overlay -debug > log.log 2>&1

#console run
#melt /z/Desktop/upan/jpg/WeChatSight2266.mp4 -consumer avformat:output.avi acodec=libmp3lame vcodec=libx264 -debug > log.log 2>&1
#remove audio: acodec=libmp3lame an=1
#melt /c/shareproject/FFMPEG/ffmpeg-mf/1.MPG -consumer avformat:test.mp4 vcodec=libx264 r=30 an=1 
#.\melt C:\shareproject\FFMPEG\ffmpeg-mf\1.MPG -consumer avformat:test.mp4 vcodec=h264_mf r=30 an=1 
#melt /c/shareproject/FFMPEG/ffmpeg-mf/1.MPG -consumer avformat:output.mp4 vcodec=libx264 r=30 an=1 -debug > log.log 2>&1
./src/melt/melt -debug /media/psf/Home/Desktop/upan/qqrtx/QQ/Thousand.Planets.mp4
date;
