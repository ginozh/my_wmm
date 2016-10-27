#!/bin/bash

<<COMMENT
@author: ginozhang
@date: 2016-10-01
@description: 
COMMENT

#./ffmpeg_r.exe -y -i jpg/mp3.512.5.avi -vf ass=jpg/subtitle.ass jpg/subt.mp3.512.5.avi
#
#./ffmpeg_r.exe -y -i jpg/subt.mp3.512.5.avi -t 2 -vf "color=red@0.5:50x50 [c]; [in][c] overlay=x='t*80-w':y =0" jpg/drawbox.avi
#./ffmpeg_r.exe -y -i jpg/lovechina1.mp3 -ss 00:00:00 -t 00:00:00.800 /c/QtProjects/QtExamples/multimedia/spectrum/debug/longlovechina1.wav

#add music
#./ffmpeg_r.exe -y -ss 00:00:10 -t 00:00:02 -i jpg/lovechina1.mp3 -i jpg/loop.512.5.avi jpg/mp3.512.5.avi

#add music, subtitle
./ffmpeg_r.exe -y -ss 00:00:10 -t 00:00:02 -i jpg/lovechina1.mp3 -i jpg/loop.512.5.avi -vf ass=jpg/subtitle.ass jpg/mp3.512.5.avi

