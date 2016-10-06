#!/bin/bash

<<COMMENT
@author: ginozhang
@date: 2016-10-01
@description: 
COMMENT

./ffmpeg_r.exe -y -i jpg/mp3.512.5.avi -vf ass=jpg/subtitle.ass jpg/subt.mp3.512.5.avi
#
#./ffmpeg_r.exe -y -i jpg/subt.mp3.512.5.avi -t 2 -vf "color=red@0.5:50x50 [c]; [in][c] overlay=x='t*80-w':y =0" jpg/drawbox.avi
