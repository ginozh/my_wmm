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
#./ffmpeg_r.exe -y -ss 00:00:10 -t 00:00:02 -i jpg/lovechina1.mp3 -i jpg/loop.512.5.avi -vf ass=jpg/subtitle.ass jpg/mp3.512.5.avi

<<COMMENT
./ffmpeg_r.exe -y -i jpg/loop.512.5.avi -i jpg/lovechina1.mp3 -i jpg/lovechina1.mp3 -filter_complex \
    "[1:a]atrim=1:2,asetpts=PTS-STARTPTS[aud1]; \
    [2:a]atrim=0:1,asetpts=PTS-STARTPTS[aud2]; \
    [aud1][aud2]concat=n=2:v=0:a=1[aout]" \
    -map 0:v -map "[aout]" -c:v copy jpg/mp3.512.5.avi
COMMENT

<<COMMENT
#right
./ffmpeg_r.exe -y -i jpg/loop.512.5.avi -f lavfi -i anullsrc=channel_layout=5.1:sample_rate=48000 -i jpg/lovechina1.mp3 -filter_complex \
    "[1:a]atrim=0:1,asetpts=PTS-STARTPTS[aud1]; \
    [2:a]atrim=10:11,asetpts=PTS-STARTPTS[aud2]; \
    [aud1][aud2]concat=n=2:v=0:a=1[aout]" \
    -map 0:v -map "[aout]" -vf ass=jpg/subtitle.ass jpg/mp3.512.5.avi
COMMENT
./ffmpeg_r.exe -y -f concat -i videolist.txt -f lavfi -i anullsrc=channel_layout=5.1:sample_rate=48000 -i jpg/lovechina1.mp3 -filter_complex \
    "[1:a]atrim=0:1,asetpts=PTS-STARTPTS[aud1]; \
    [2:a]atrim=10:11,asetpts=PTS-STARTPTS[aud2]; \
    [aud1][aud2]concat=n=2:v=0:a=1[aout]" \
    -map 0:v -map "[aout]" -vf ass=jpg/subtitle.ass jpg/mp3.512.5.avi
