#!/bin/bash

<<COMMENT
@author: ginozhang
@date: 2016-10-01
@description: 
COMMENT

#./ffmpeg_r.exe -y -i jpg/mp3.512.5.avi -vf ass=jpg/subtitle.ass jpg/subt.mp3.512.5.avi

#./ffmpeg_r.exe -y -i jpg/subt.mp3.512.5.avi -t 2 -vf "color=red@0.5:50x50 [c]; [in][c] overlay=x='t*80-w':y =0" jpg/drawbox.avi
#./ffmpeg_r.exe -y -i jpg/lovechina1.mp3 -ss 00:00:00 -t 00:00:00.800 /c/QtProjects/QtExamples/multimedia/spectrum/debug/longlovechina1.wav

#add music
#./ffmpeg_r.exe -y -ss 00:00:10 -t 00:00:02 -i jpg/lovechina1.mp3 -i jpg/loop.512.5.avi jpg/mp3.512.5.avi
#精确时长-无用: http://stackoverflow.com/questions/21420296/how-to-extract-time-accurate-video-segments-with-ffmpeg
#无用：./ffmpeg_r.exe -y -ss 00:00:10 -t 00:00:03 -i jpg/lovechina1.mp3 -i jpg/loop.512.5.avi -an -shortest jpg/mp3.512.5.avi
#wrong: http://stackoverflow.com/questions/26899871/cannot-join-mp3-file-and-avi-file-with-ffmpeg
#./ffmpeg_r.exe -y -i /c/Users/user/Videos/11.avi -ss 0 -t 5 -i /c/Users/user/Music/shortlovechina2.wav   -map 0 -map 1:a -c copy  /c/Users/user/Videos/11music.avi

#wrong add music, subtitle
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
./ffmpeg_r.exe -y -f lavfi -i anullsrc=channel_layout=5.1:sample_rate=48000 -i jpg/lovechina1.wav -filter_complex \
    "[0:a]atrim=0:1,asetpts=PTS-STARTPTS[aud1]; \
    [1:a]atrim=0:2,asetpts=PTS-STARTPTS[aud2]; \
    [aud1][aud2]concat=n=2:v=0:a=1[aout]" \
    -map "[aout]"  jpg/nulllovechina2.mp3
COMMENT

<<COMMENT
#right: videos mp3 text
./ffmpeg_r.exe -y -f concat -i videolist.txt -f lavfi -i anullsrc=channel_layout=5.1:sample_rate=48000 -i jpg/lovechina1.mp3 -filter_complex \
    "[1:a]atrim=0:1,asetpts=PTS-STARTPTS[aud1]; \
    [2:a]atrim=10:11,asetpts=PTS-STARTPTS[aud2]; \
    [aud1][aud2]concat=n=2:v=0:a=1[aout]" \
    -map 0:v -map "[aout]" -vf ass=jpg/subtitle.ass jpg/mp3.512.5.avi
COMMENT

<<COMMENT
#right: add animation
#time ./ffmpeg_r.exe -y  -framerate 24 -i jpg/512img002.jpg -vf "zoompan=z='zoom+0.001':d=125:s=512x384"  -t 2 jpg/zoom.512.2.avi;
time ./ffmpeg_r.exe -y  -framerate 24 -i jpg/512img002.jpg -vf "zoompan=z='zoom+0.001':s=512x384" -t 2 jpg/zoom.512.2.avi;
time ./ffmpeg_r.exe -y  -framerate 24 -i jpg/512img003.jpg -vf "zoompan=z='zoom+0.001':s=512x384" -t 2 jpg/zoom.512.3.avi;
time ./ffmpeg_r.exe -y -i jpg/zoom.512.3.avi -i jpg/zoom.512.2.avi -filter_complex "blend=all_expr='if(crossfade,1.5,2)'"  jpg/annimation_zoom.avi
COMMENT

<<COMMENT
#right: rotate and scale
#0 = 90CounterCLockwise and Vertical Flip (default)
#1 = 90Clockwise 顺时针
#2 = 90CounterClockwise 逆时针
#3 = 90Clockwise and Vertical Flip
#-vf "transpose=2,transpose=2" for 180 degrees. http://stackoverflow.com/questions/3937387/rotating-videos-with-ffmpeg
#setsar=1:1,setdar=4:3 像素点长宽比不一样时生成动画时会出错 http://video.stackexchange.com/questions/9947/how-do-i-change-frame-size-preserving-width-using-ffmpeg
time ./ffmpeg_r.exe -y -i jpg/img007.jpg -vf "transpose=1,scale=512:384,setsar=1:1,setdar=4:3" jpg/512img007_1.jpg
COMMENT

#right: zoompan + posterize
#time ./ffmpeg_r.exe -y  -framerate 24 -i jpg/512img003.jpg -vf "zoompan=z='zoom+0.001':s=512x384,boxblur=2:1:cr=0:ar=0" -t 2 jpg/zoom.512.3.avi;
#right
#time ./ffmpeg_r.exe -y  -i jpg/img001.jpg.mp4 -i jpg/img002.jpg.mp4 -i jpg/img003.jpg.mp4 -filter_complex "[0:v]scale=(iw/3-10):(ih/3-10),pad=iw:ih/3:10:10:black[mid0];[1:v]scale=(iw/3-10):(ih/3-10),pad=iw/3:ih/3:10:10:black[mid1];[mid0][mid1]overlay=W/3:0[ref7];[2:v]scale=(iw/3-10):(ih/3-10),pad=iw/3:ih/3:10:10:black[mid2];[ref7][mid2]overlay=2*W/3:0" jpg/bottom_tmp.mp4
#1008*756
#time ./ffmpeg_r.exe -y  -i jpg/img001.jpg.mp4 -i jpg/img002.jpg.mp4 -i jpg/img003.jpg.mp4 -filter_complex "[0:v]scale=(1008/3-10):(756/3-10),pad=1008:756/3:10:10:black[mid0];[1:v]scale=(1008/3-10):(756/3-10),pad=1008/3:756/3:10:10:black[mid1];[mid0][mid1]overlay=1008/3:0[ref7];[2:v]scale=(1008/3-10):(756/3-10),pad=1008/3:756/3:10:10:black[mid2];[ref7][mid2]overlay=2*1008/3:0" jpg/bottom_tmp.mp4
#right: zoom + fade
#time ./ffmpeg_r.exe -y  -i jpg/img001.jpg.mp4 -filter_complex "zoompan=z='if(lte(zoom,1.0),1.2,max(1.001,zoom-0.01))':mwhx='1':x='(iw-mw)/2':mhhy='1':y='(ih-mh)/2',fade=t=out:st=3:d=1:color=black" -t 4 jpg/simple2.mp4


#right: nullsrc clip + ass + overlay
#./ffmpeg_r.exe -y -filter_complex "color=c=black:d=4:s=320*240,ass=jpg/subtitle.ass[back];color=c=red[top];[back][top]overlay=x='t*80-w/2':y =0" -t 4 jpg/startclip.mp4

#right: scale video
#./ffmpeg_r.exe -y -i /c/Users/user/Videos/1.mp4 -vf "scale=512*384,setsar=1:1,setdar=4:3" jpg/startclip.mp4
