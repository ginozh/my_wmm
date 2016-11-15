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

<<COMMENT
#right: videos mp3 text
./ffmpeg_r.exe -y -f concat -i videolist.txt -f lavfi -i anullsrc=channel_layout=5.1:sample_rate=48000 -i jpg/lovechina1.mp3 -filter_complex \
    "[1:a]atrim=0:1,asetpts=PTS-STARTPTS[aud1]; \
    [2:a]atrim=10:11,asetpts=PTS-STARTPTS[aud2]; \
    [aud1][aud2]concat=n=2:v=0:a=1[aout]" \
    -map 0:v -map "[aout]" -vf ass=jpg/subtitle.ass jpg/mp3.512.5.avi
COMMENT

#right: add animation
#time ./ffmpeg_r.exe -y  -framerate 24 -i jpg/512img002.jpg -vf "zoompan=z='zoom+0.001':d=125:s=512x384"  -t 2 jpg/zoom.512.2.avi;
time ./ffmpeg_r.exe -y  -framerate 24 -i jpg/512img002.jpg -vf "zoompan=z='zoom+0.001':s=512x384" -t 2 jpg/zoom.512.2.avi;
time ./ffmpeg_r.exe -y  -framerate 24 -i jpg/512img003.jpg -vf "zoompan=z='zoom+0.001':s=512x384" -t 2 jpg/zoom.512.3.avi;
time ./ffmpeg_r.exe -y -i jpg/zoom.512.3.avi -i jpg/zoom.512.2.avi -filter_complex "blend=all_expr='if(crossfade,1.5,2)'"  jpg/annimation_zoom.avi

#right: rotate and scale
#0 = 90CounterCLockwise and Vertical Flip (default)
#1 = 90Clockwise 顺时针
#2 = 90CounterClockwise 逆时针
#3 = 90Clockwise and Vertical Flip
#-vf "transpose=2,transpose=2" for 180 degrees. http://stackoverflow.com/questions/3937387/rotating-videos-with-ffmpeg
#setsar=1:1,setdar=4:3 像素点长宽比不一样时生成动画时会出错 http://video.stackexchange.com/questions/9947/how-do-i-change-frame-size-preserving-width-using-ffmpeg
time ./ffmpeg_r.exe -y -i jpg/img007.jpg -vf "transpose=1,scale=512:384,setsar=1:1,setdar=4:3" jpg/512img007_1.jpg
