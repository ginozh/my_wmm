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
#./ffmpeg_r.exe -y -i jpg/shortlovechina2.wav -i jpg/shortlovechina2.wav -i jpg/simple.mp4 jpg/mp3.512.5.avi
#精确时长-无用: http://stackoverflow.com/questions/21420296/how-to-extract-time-accurate-video-segments-with-ffmpeg
#无用：./ffmpeg_r.exe -y -ss 00:00:10 -t 00:00:03 -i jpg/lovechina1.mp3 -i jpg/loop.512.5.avi -an -shortest jpg/mp3.512.5.avi
#wrong: http://stackoverflow.com/questions/26899871/cannot-join-mp3-file-and-avi-file-with-ffmpeg
#./ffmpeg_r.exe -y -i /c/Users/user/Videos/11.avi -ss 0 -t 5 -i /c/Users/user/Music/shortlovechina2.wav   -map 0 -map 1:a -c copy  /c/Users/user/Videos/11music.avi

#wrong add music, subtitle
#./ffmpeg_r.exe -y -ss 00:00:10 -t 00:00:02 -i jpg/lovechina1.mp3 -i jpg/loop.512.5.avi -vf ass=jpg/subtitle.ass jpg/mp3.512.5.avi

<<COMMENT
#1, trim 分割
./ffmpeg_r.exe -y -i jpg/loop.512.5.avi -i jpg/lovechina1.mp3 -i jpg/lovechina1.mp3 -filter_complex \
    "[1:a]atrim=1:2,asetpts=PTS-STARTPTS[aud1]; \
    [2:a]atrim=0:1,asetpts=PTS-STARTPTS[aud2]; \
    [aud1][aud2]concat=n=2:v=0:a=1[aout]" \
    -map 0:v -map "[aout]" -c:v copy jpg/mp3.512.5.avi
COMMENT

<<COMMENT
#2, right
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
#3, right: videos mp3 text
./ffmpeg_r.exe -y -f concat -i videolist.txt -f lavfi -i anullsrc=channel_layout=5.1:sample_rate=48000 -i jpg/lovechina1.mp3 -filter_complex \
    "[1:a]atrim=0:1,asetpts=PTS-STARTPTS[aud1]; \
    [2:a]atrim=10:11,asetpts=PTS-STARTPTS[aud2]; \
    [aud1][aud2]concat=n=2:v=0:a=1[aout]" \
    -map 0:v -map "[aout]" -vf ass=jpg/subtitle.ass jpg/mp3.512.5.avi
./ffmpeg_r.exe -y -f concat -i videolist.txt -i jpg/shortlovechina2.wav -filter_complex \
    "[1:a]asetpts=PTS-STARTPTS[aud1]; \
    [1:a]asetpts=PTS-STARTPTS[aud2]; \
    [1:a]atrim=0:1,asetpts=PTS-STARTPTS[aud3]; \
    [aud1][aud2][aud3]concat=n=3:v=0:a=1[aout]" \
    -map 0:v -map "[aout]" -vf ass=jpg/subtitle.ass jpg/mp3.512.5.avi
COMMENT

<<COMMENT
#4, right: add animation
#time ./ffmpeg_r.exe -y  -framerate 24 -i jpg/512img002.jpg -vf "zoompan=z='zoom+0.001':d=125:s=512x384"  -t 2 jpg/zoom.512.2.avi;
time ./ffmpeg_r.exe -y  -framerate 24 -i jpg/512img002.jpg -vf "zoompan=z='zoom+0.001':s=512x384" -t 2 jpg/zoom.512.2.avi;
time ./ffmpeg_r.exe -y  -framerate 24 -i jpg/512img003.jpg -vf "zoompan=z='zoom+0.001':s=512x384" -t 2 jpg/zoom.512.3.avi;
time ./ffmpeg_r.exe -y -i jpg/zoom.512.3.avi -i jpg/zoom.512.2.avi -filter_complex "blend=all_expr='if(crossfade,1.5,2)'"  jpg/annimation_zoom.avi
COMMENT

<<COMMENT
#5, right: rotate and scale
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
#time ./ffmpeg_r.exe -y  -i jpg/img001.jpg.mp4 -i jpg/img002.jpg.mp4 -i jpg/img003.jpg.mp4 -filter_complex "[0:v]scale=(iw/3-10):(ih/3-10),pad=iw:ih/3:10:10:black[mid0];[1:v]scale=(iw/3-10):(ih/3-10),pad=iw/3:ih/3:10:10:black[mid1];[mid0][mid1]overlay=W/3:0[ref7];[2:v]scale=(iw/3-10):(ih/3-10),pad=iw/3:ih/3:10:10:black[mid2];[ref7][mid2]overlay=2*W/3:0" -t 4 jpg/bottom_tmp.mp4
#1008*756
#time ./ffmpeg_r.exe -y  -i jpg/img001.jpg.mp4 -i jpg/concat.avi -i jpg/img003.jpg.mp4 -filter_complex "[0:v]scale=(1008/3-10):(756/3-10),pad=1008:756/3:10:10:black[mid0];[1:v]scale=(1008/3-10):(756/3-10),pad=1008/3:756/3:10:10:black[mid1];[mid0][mid1]overlay=1008/3:0[ref7];[2:v]scale=(1008/3-10):(756/3-10),pad=1008/3:756/3:10:10:black[mid2];[ref7][mid2]overlay=2*1008/3:0" -t 4 jpg/bottom_tmp.mp4
#right: zoom + fade
#time ./ffmpeg_r.exe -y  -i jpg/img001.jpg.mp4 -filter_complex "zoompan=z='if(lte(zoom,1.0),1.2,max(1.001,zoom-0.01))':mwhx='1':x='(iw-mw)/2':mhhy='1':y='(ih-mh)/2',fade=t=out:st=3:d=1:color=black" -t 4 jpg/simple2.mp4


#right: nullsrc clip + ass + overlay
#./ffmpeg_r.exe -y -filter_complex "color=c=black:d=4:s=512*384,ass=jpg/subtitle.ass,zoompan=z='zoom+0.001'[back];color=c=red:s=512*384[top];[back][top]overlay=x='t*80+w/2':y =0" -t 4 jpg/startclip.mp4
#./ffmpeg_r.exe -y -filter_complex "color=c=black:d=4:s=512*384"  -f avi -vcodec mpeg4 jpg/startclip.avi
#right: scale video
#./ffmpeg_r.exe -y -i /c/Users/user/Videos/1.mp4 -vf "scale=512*384,setsar=1:1,setdar=4:3" jpg/startclip.mp4

<<COMMENT
#合并
#concat right: dst
#./ffmpeg_r.exe -y -framerate 25 -loop 1 -pix_fmt yuv420p -filter_complex 'color=c=black:d=2:s=504*378' -vcodec mpeg4 -t 2 -f avi jpg/black.avi
##./ffmpeg_r.exe -y -i /c/Users/user/Documents/My\ Videos/IMG_1924.MP4 -vf 'scale=504*378,setsar=1:1,setdar=4:3' -t 6.81 -f avi jpg/IMG_1924.avi
#./ffmpeg_r.exe -y -i /c/shareproject/effect_video/example/debug/love_start.flv -vf 'fps=fps=25,scale=504*378,setsar=1:1,setdar=4:3' -vcodec mpeg4 -t 6.81 -f avi jpg/love_start.avi
#./ffmpeg_r.exe -y -f concat -i videolist.txt -c:v copy -f avi jpg/concat.avi
##./ffmpeg_r.exe -y -f concat -i videolist.txt -c:v libvpx-vp9 -c:a libopus -f avi jpg/concat.avi
##./ffmpeg_r.exe -y -f concat -i videolist.txt -f avi jpg/mm.avi
##./ffmpeg_r.exe -y -f concat -i imagelist.txt -vsync vfr -vf "fps=25" -pix_fmt yuv420p -vcodec h264 -preset ultrafast -t 2 -f avi jpg/concat.avi
## ./ffmpeg_r.exe -y -i /c/shareproject/effect_video/example/tmp/tmp_12.mp4 -i /c/shareproject/effect_video/example/tmp/tmp_14.mp4 -filter_complex "blend=all_expr='if(wheel,2,10)'" -pix_fmt yuv420p -vcodec h264 -preset ultrafast -b:v 3000k /c/shareproject/effect_video/example/tmp/tmp_0.mp4
##./ffmpeg_r.exe -y -t 1.800 -i jpg/music6sec.avi -t 1.800 -i jpg/music6sec.avi jpg/concat.avi
./ffmpeg_r.exe -y -v trace -f avi -i jpg/loop.512.1.avi -f avi -i jpg/loop.512.2.avi -f avi -i jpg/loop.512.1.avi -f lavfi -i anullsrc=channel_layout=5.1:sample_rate=48000 -i jpg/lovechina1.mp3 -filter_complex \
    "[0:v]trim=0:1[v0]; \
    [1:v]trim=0:1[v1]; \
    [2:v]trim=0:1[v2]; \
    [v0][v1][v2]concat=n=3:v=1[last]; \
    [last]ass=jpg/subtitle.ass[vout]; \
    [3:a]atrim=0:1,asetpts=PTS-STARTPTS[a3]; \
    [4:a]atrim=1:2,asetpts=PTS-STARTPTS[a4]; \
    [a3][a4]concat=n=2:v=0:a=1[aout]" \
    -map "[aout]" -map "[vout]" jpg/mm.avi
COMMENT
<<COMMENT
#ok
./ffmpeg_r.exe -y -v trace -f avi -i jpg/loop.512.1.avi -f avi -i jpg/loop.512.2.avi -f avi -i jpg/loop.512.1.avi -t 3 -i jpg/lovechina1.mp3 -filter_complex \
    "[0:v]trim=0:1[v0]; \
    [1:v]trim=0:1[v1]; \
    [2:v]trim=0:1[v2]; \
    [v0][v1][v2]concat=n=3:v=1[last]; \
    [last]ass=jpg/subtitle.ass[vout]; \
    [3:a]atrim=0:1,asetpts=PTS-STARTPTS[aout]" \
    -map "[aout]" -map "[vout]" jpg/mm.avi
COMMENT

iw=512;
ih=384;
w=$iw;
h=$ih;
#scale right
#./ffmpeg.exe -y -i jpg/img001.jpg.mp4 -filter_complex "[0:v]scale=w=$iw*1.25:h=$ih*1.25,crop=$w:$h:0:0,zoompan=z='1.2':x='min(x+(zoom-1)*iw/2/25,(zoom-1)*iw/2)':s=$iw*$ih,setsar=1:1,setdar=4:3" -t 2 jpg/simple1.mp4

    #"zoompan=z='1.2':oy='0.2*ih':y='max(y-(zoom-1)*ih/2/25,0)'" \
    #"zoompan=z='1.2':oy='0.2*ih':y='max(0.2*ih-on*ih/20/25,0)'" \
    #zoompan=z='1.2':x='(zoom-1)*iw/2':oy='0.2*ih':y='max(y-(zoom-1)*ih/2/25,0)'

    #zoompan=z='1+(0.001)*on':mwhx='1':x='(iw-mw)/2':mhhy='1':y='(ih-mh)/2'
#./ffmpeg_r.exe -y -i /c/shareproject/effect_video/example/tmp_1.avi -filter_complex \
#    "zoompan=z='if(lte(on,1),1.400,max(1.0001,1.400-0.0001*on))':mwhx='1':x='(iw-mw)/2':mhhy='1':y='(ih-mh)/2':s=512*384,setsar=1:1,setdar=4:3,fade=t=out:st=3.000:d=1.000:color=black" \
#-vcodec libx264 -preset ultrafast -t 4 /c/shareproject/effect_video/example/tmp_2.avi
    #"fade=t=out:st=3.000:d=1.000:color=black,zoompan=z='if(lte(on,1),1.400,max(1.001,1.400-0.001*on))':mwhx='1':x='(iw-mw)/2':mhhy='1':y='(ih-mh)/2':s=512*384,setsar=1:1,setdar=4:3" \


#./ffmpeg_r.exe -y -framerate 25 -loop 1 -t 6 -i /c/QtProjects/qtmovie/jpg/1.jpg -pix_fmt yuv420p -vf "scale=1280*720,setsar=1:1,setdar=16:9" -vcodec mpeg4 -f avi /c/shareproject/effect_video/example/tmp_0.avi
#./ffmpeg_r.exe -y -framerate 25 -loop 1 -i /c/QtProjects/qtmovie/jpg/1.jpg \
#    -pix_fmt yuv420p -vf "setsar=1:1,setdar=16:9" -vcodec h264 -t 6 /c/shareproject/effect_video/example/tmp_0.mp4

#lizi: 不能做scale操作
#./ffmpeg_r.exe -y -i /c/shareproject/effect_video/example/debug/love_lizi.flv -vf "scale=1280*720,setsar=1:1,setdar=16:9" /c/shareproject/effect_video/example/love_lizi.flv
##./ffmpeg_r.exe -y -i /c/shareproject/effect_video/example/tmp_1.mp4 -i jpg/Sport21_Crop.mov -filter_complex "[0:v][1:v]overlay=0:0" -vcodec h264 -preset ultrafast -t 9 -f avi /c/shareproject/effect_video/example/tmp_4.mp4
##./ffmpeg_r.exe -y -i /c/shareproject/effect_video/example/tmp/tmp_1.mp4 -i jpg/1.flv -filter_complex "[0:v][1:v]overlay=0:0" -vcodec h264 -preset ultrafast -t 9 /c/shareproject/effect_video/example/tmp/tmp_0.mp4

<<COMMENT
# 变形 + crop
w=1024;
h=768;
arWidth=1280;
arHeight=720;
perl -e "
    my \$wr=$arWidth/$w;
    my \$hr=$arHeight/$h;
    if(\$wr>1 && \$hr>1)
    {
        print 'crop=$w:$h:($arWidth-$w)/2:($arHeight/$h)/2';
    }
    elsif (\$wr>1)
    {
        my \$w=$arWidth*$h/$arHeight;
        my \$h=$h;
        print 'scale=\$w:\$h,crop=$w:$h:(\$w-$w)/2:0';
    } 
    elsif (\$hr>1)
    {
        my \$h=$arHeight*$w/$arWidth;
        my \$w=$w;
        print 'scale=\$w:\$h,crop=$w:$h:0:(\$h-$h)/2';
    }
    else
    {
    }
    #1, 1280/720 > 1024/768
    #高度偏小：同一个高度
    #1280*768/720=1365.333
    #-> 1365.333:768

    #2, 1024/768 > 1280/720
    #宽度偏小
    #1280*768 > 1024*720
    #1280*768/1024 > 720
    #960 > 720
    "
#FFREPORT=file=jpg/log.txt:level=16 ./ffmpeg -y -v warning -i /c/QtProjects/qtmovie/jpg/1.jpg -filter_complex "scale=1024:575.5,pad=1024:768:0:(768-575.5)/2:black,scale=1024:768" /c/QtProjects/qtmovie/jpg/1_scale.jpg
time ./ffmpeg -y -report -v warning -i /c/QtProjects/qtmovie/jpg/1.jpg -filter_complex "scale=1024:575.5,pad=1024:768:0:(768-575.5)/2:black,scale=1024:768" /c/QtProjects/qtmovie/jpg/1_scale.jpg
COMMENT
<<COMMENT
# 变形 + pad黑色
w=1024;
h=768;
arWidth=1280;
arHeight=720;
perl -e "
    #1280/720 > 1024/768
    #1024*720/1280 <768
    #576 < 768
    if($arWidth/$arHeight > $w/$h)
    {
        #高度部分需要填充
        $w*$arHeight/$arWidth < $h
        #576 < 768
        #1024:576 == 1280/720
    }
    "
#pad black 55764
##time ./doc/examples/simple_filter.exe
echo
echo
##for((idx=1;$idx<=4;idx++));do
##time ./ffmpeg_r.exe -v error -y  -framerate 24 -loop 1 -i /c/QtProjects/qtmovie/jpg/1.jpg -filter_complex "scale=1024:576,pad=1024:768:0:(768-576)/2:black,scale=1024:768" -t 10 jpg/tmp_$idx.avi &
##done
time ./ffmpeg_r.exe -v error -y  -framerate 24 -loop 1 -i /c/QtProjects/qtmovie/jpg/1.jpg -filter_complex "scale=1024:576,pad=1024:768:0:(768-576)/2:black,scale=1024:768" -t 10 jpg/tmp_$idx.avi 

echo
echo
##time ./ffmpeg_r -y -i /c/QtProjects/qtmovie/jpg/1.jpg -filter_complex "scale=1024:576,pad=1024:768:0:(768-576)/2:black,scale=1024:768" -qscale:v 1 /c/QtProjects/qtmovie/jpg/1_ori_scale.jpg
echo
echo
#time ./ffmpeg_r -y -i /c/QtProjects/qtmovie/jpg/1.jpg -filter_complex "scale=1024:576,pad=1024:768:512:(768-576)/2:black,scale=1024:768" /c/QtProjects/qtmovie/jpg/1_ori_scale.jpg
#time ./doc/examples/filtering_video.exe /c/QtProjects/qtmovie/jpg/1.jpg
##./ffmpeg -y -i /c/QtProjects/qtmovie/jpg/1_1280_719.jpg -filter_complex "scale=1024:575,pad=1024:768:0:(768-575)/2:black,scale=1024:768" /c/QtProjects/qtmovie/jpg/1_scale.jpg
COMMENT

#add music argv && zoom
#./ffmpeg.exe -y -i /c/shareproject/effect_video/example/tmp_1.avi -i jpg/lovechina1.mp3 \
#    -vf "zoompan=z='1+(0.0004)*on':mwhx='1':x='(iw-mw)/2':mhhy='1':y='(ih-mh)/2':s=1280x720" \
#    -vol 150 -b:a 256k -ac 2 -ar 44100 -c:a aac -vcodec libx264 -b:v 1200k -preset ultrafast -t 7 \
#    /c/shareproject/effect_video/example/tmp_5.avi

#一张图片重叠三张
w=1280; #512;
h=720; #384;
h_single=$h/4;
h_double=$(echo "$h_single * 2" | bc);
#./ffmpeg.exe -y -i jpg/loop.512.3.avi -filter_complex "[0:v]crop=$w:$h_single:0:2*$h_single,pad=$w:$h:0:0[top];[0:v]crop=$w:$h_double:0:$h_single[middle];[0:v]crop=$w:$h_single:0:$h_single[bottom];[top][middle]overlay=0:$h_single[top1];[top1][bottom]overlay=0:3*$h_single" jpg/0.avi
#./ffmpeg.exe -y -i jpg/loop.512.3.avi -i jpg/0.avi -filter_complex "blend=all_expr='if(slidedownt,1.5,2)'" jpg/1.avi
#./ffmpeg.exe -y -i jpg/loop.512.3.avi -filter_complex "[0:v]crop=$w:(n/50)*$h_single:0:2*$h_single[top];[0:v]crop=$w:(n/50)*$h_double:0:$h_single[middle];[0:v]crop=$w:(n/50)*$h_single:0:$h_single[bottom];[0:v][top]overlay=0:0" jpg/0.avi
#./ffmpeg.exe -y -i jpg/loop.512.3.avi -i jpg/loop.512.3.avi -filter_complex "[0:v]crop=$w:((n+1)/50)*$h_single:0:2*$h_single[top];[1:v][top]overlay=0:0" jpg/0.avi
#./ffmpeg.exe -y -i jpg/loop.512.3.avi -filter_complex "[0:v]crop=$w:$h_single:0:2*$h_single[top];[0:v][top]overlay=x=0:y='(n+1)*$h_single/51'" jpg/0.avi

#./ffmpeg.exe -y -i jpg/loop.512.3.avi -filter_complex "[0:v]zoompan=z='1.4':mwhx='1':x='(iw-mw)/2':mhhy='1':y='(ih-mh)/2'[backg];[0:v]crop=$w:$h_single:0:2*$h_single[top];[0:v][top]overlay=0:0[top1];[0:v]crop=$w:$h_double:0:$h_single[middle];[top1][middle]overlay=x=0:y='if(lte(n+1,25),n+1,25)*$h_single/25':shortest=1[top2];[0:v]crop=$w:$h_double:0:$h_single[bottom];[top2][bottom]overlay=x=0:y='if(lte(n+1,25),n+1,25)*3*$h_single/25:shortest=1'" jpg/0.avi

#./ffmpeg.exe -y -i jpg/loop.512.3.avi -filter_complex "[0:v]zoompan=z='1.4':mwhx='1':x='(iw-mw)/2':mhhy='1':y='(ih-mh)/2':s=512*384" -t 2 jpg/0.avi
#./ffmpeg.exe -y -i jpg/0.avi -filter_complex "[0:v]crop=$w:$h_single:0:2*$h_single[top];[0:v][top]overlay=0:0[top1];[0:v]crop=$w:$h_double:0:$h_single[middle];[top1][middle]overlay=x=0:y='if(lte(n+1,25),n+1,25)*$h_single/25':shortest=1[top2];[0:v]crop=$w:$h_double:0:$h_single[bottom];[top2][bottom]overlay=x=0:y='if(lte(n+1,25),n+1,25)*3*$h_single/25:shortest=1'" -t 2 jpg/1.avi
#./ffmpeg.exe -y -framerate 25 -loop 1 -i /c/QtProjects/qtmovie/jpg/2.jpg -filter_complex "[0:v]crop=$w:$h_single:0:2*$h_single[top];[0:v][top]overlay=0:0[top1];[0:v]crop=$w:$h_double:0:$h_single[middle];[top1][middle]overlay=x=0:y='if(lte(n+1,25),n+1,25)*$h_single/25':shortest=1[top2];[0:v]crop=$w:$h_double:0:$h_single[bottom];[top2][bottom]overlay=x=0:y='if(lte(n+1,25),n+1,25)*3*$h_single/25:shortest=1'" -t 2 jpg/0.avi

#./ffmpeg.exe -y -framerate 25 -loop 1 -i /c/QtProjects/qtmovie/jpg/2.jpg -filter_complex "[0:v]crop=$w:$h_single:0:2*$h_single,pad=$w:$h:0:0[top];[0:v]crop=$w:$h_double:0:$h_single[middle];[0:v]crop=$w:$h_single:0:$h_single[bottom];[top][middle]overlay=0:$h_single[top1];[top1][bottom]overlay=0:3*$h_single" -t 2 jpg/0.avi
#./ffmpeg.exe -y -i jpg/0.avi -filter_complex "zoompan=z='1.2':x='min(on*(zoom-1)*iw/10/25,(zoom-1)*iw/2)':y='(zoom-1)*ih':s=$w*$h" -t 2 jpg/1.avi
<<COMMENT
#一张图片垂直重叠三张
zoom=1.6;
w=1280; #512;
h=720; #384;
h_single=$h/4;
h_double=$(echo "$h_single * 2" | bc);
#r1: 放大zoom倍之前第一张所占的高度r1表达式(-最上部分多出来的高度=放大后截取的高度为截取整体高度的1/4): $zoom*r1-($zoom-1)*(1/2)*$h=(1/4)*$h
#r2: 放大zoom倍之前第二张所占的高度r2表达式(放大后截取的高度为截取整体高度的1/2): $zoom*r2=(1/2)*$h
#r3: 同r1
r1=$(perl -e "print $h*(1/2-1/(4*$zoom))")
r2=$(perl -e "print $h*(1/(2*$zoom))"  )
r3=$r1;
#off1: 放大zoom之前截图图片的开始高度位置
off1=$(echo "$r2" | bc)
off2=$r1;
off3=$r1;
echo "r1: $r1 r2: $r2 r3: $r3 off1: $off1 off2: $off2 off3: $off3"
#1, 缩放之前叠加三张图片
./ffmpeg.exe -y -framerate 25 -loop 1 -i /c/QtProjects/qtmovie/jpg/2.jpg -filter_complex  "[0:v]crop=$w:$r1:0:$off1[top];[0:v][top]overlay=0:0[top1];[0:v]crop=$w:$r2:0:$off2[middle];[top1][middle]overlay=0:$r1[top2];[0:v]crop=$w:$r3:0:$off3[bottom];[top2][bottom]overlay=0:($r1+$r2)" -t 2 -vcodec libx264 -preset ultrafast jpg/1.mp4
#./ffmpeg.exe -y -i jpg/loop.512.3.mp4 -filter_complex "zoompan=z='$zoom':x='min(on*(zoom-1)*iw/10/25,(zoom-1)*iw/2)':y='(zoom-1)*ih':s=$w*$h" -t 2 -vcodec libx264 -preset ultrafast jpg/2.mp4
#2, 放大后再图片向右移动: panright
./ffmpeg.exe -v info -y -i jpg/1.mp4 -filter_complex "zoompan=z='$zoom':mhhy='1':y='(ih-mh)/2':x='min(on*(zoom-1)*iw/4/25,(zoom-1)*iw/2)'" -t 2 -vcodec libx264 -preset ultrafast jpg/2.mp4
#3, 放大后截取最右边中间位置的$w*$h图片
./ffmpeg.exe -y -framerate 25 -loop 1 -i /c/QtProjects/qtmovie/jpg/2.jpg -filter_complex  "[0:v]scale=w=$zoom*$w:h=$zoom*$h,crop=$w:$h:0:$h*($zoom-1)/2" -pix_fmt yuv420p -vcodec libx264 -preset ultrafast -t 2 jpg/3.mp4
#4, 向下移动叠加的三张图片
./ffmpeg.exe -y -i jpg/3.mp4 -filter_complex  "[0:v]crop=$w:$h_single:0:2*$h_single[top];[0:v][top]overlay=0:0[top1];[0:v]crop=$w:$h_double:0:$h_single[middle];[top1][middle]overlay=x=0:y='if(lte(n,25),n,25)*$h_single/25':shortest=1[top2];[0:v]crop=$w:$h_double:0:$h_single[bottom];[top2][bottom]overlay=x=0:y='if(lte(n,25),n,25)*3*$h_single/25:shortest=1'" -t 2 -vcodec libx264 -preset ultrafast jpg/4.mp4
#5, 合成第4、2个视频
./ffmpeg.exe -y -i jpg/4.mp4 -i jpg/2.mp4 -filter_complex "[0:v:0] [1:v:0] concat=n=2:v=1 [v] " -map "[v]" -vcodec libx264 -preset ultrafast jpg/5.mp4

#6，过渡到原始放大后的图片
./ffmpeg.exe -y -ss 1 -i jpg/5.mp4 -i jpg/3.mp4 -filter_complex "blend=all_expr='if(slidedownt,1,2)'" jpg/6.mp4

COMMENT

#一张图片水平重叠三张
zoom=1.6;
w=1280; #512;
h=720; #384;
w_single=$w/4;
w_double=$(echo "$w_single * 2" | bc);
#r1: 放大zoom倍之前第一张所占的宽度r1表达式(-最左边部分多出来的宽度=放大后截取的宽度为截取整体宽度的1/4): $zoom*r1-($zoom-1)*(1/2)*$w=(1/4)*$w
#r2: 放大zoom倍之前第二张所占的宽度r2表达式(放大后截取的宽度为截取整体宽度的1/2): $zoom*r2=(1/2)*$w
#r3: 同r1
r1=$(perl -e "print $w*(1/2-1/(4*$zoom))")
r2=$(perl -e "print $w*(1/(2*$zoom))"  )
r3=$r1;
#off1: 放大zoom之前截图图片的开始宽度位置
off1=$(echo "$r2" | bc)
off2=$r1;
off3=$r1;
<<COMMENT
#1, 放大后截取最上边中间位置的$w*$h图片
./ffmpeg.exe -y -framerate 25 -loop 1 -i /c/QtProjects/qtmovie/jpg/2.jpg -pix_fmt yuv420p -vcodec libx264 -preset ultrafast -t 2 jpg/0.mp4
./ffmpeg.exe -y -i jpg/0.mp4 -filter_complex  "[0:v]scale=w=$zoom*$w:h=$zoom*$h,crop=$w:$h:$w*($zoom-1)/2:0,scale=$w*$h" -vcodec libx264 -preset ultrafast -t 2 jpg/1.mp4
#2, 向右移动叠加的三张图片
./ffmpeg.exe -y -i jpg/1.mp4 -filter_complex  "[0:v]crop=$w_single:$h:2*$w_single:0[left];[0:v][left]overlay=0:0[left1];[0:v]crop=$w_double:$h:$w_single:0[middle];[left1][middle]overlay=x='if(lte(n,25),n,25)*$w_single/25':y=0:shortest=1[left2];[0:v]crop=$w_double:$h:$w_single:0[right];[left2][right]overlay=x='if(lte(n,25),n,25)*3*$w_single/25:y=0:shortest=1'" -t 1 -vcodec libx264 -preset ultrafast jpg/2.mp4
#3, 缩放之前叠加三张图片
./ffmpeg.exe -y -i jpg/0.mp4 -filter_complex  "[0:v]crop=$r1:$h:$off1:0[left];[0:v][left]overlay=0:0[left1];[0:v]crop=$r2:$h:$off2:0[middle];[left1][middle]overlay=$r1:0[left2];[0:v]crop=$r3:$h:$off3:0[right];[left2][right]overlay=($r1+$r2):0" -t 2 -vcodec libx264 -preset ultrafast jpg/3.mp4
#4, 放大后再图片向下移动(看起来向上): pandown
./ffmpeg.exe -v info -y -i jpg/3.mp4 -filter_complex "zoompan=z='$zoom':mwhx='1':x='(iw-mw)/2':y='min(on*(zoom-1)*ih/2/25,(zoom-1)*ih)':s=$w*$h" -t 2 -vcodec libx264 -preset ultrafast -pix_fmt yuv420p jpg/4.mp4
#5, 合成第2,4个视频
./ffmpeg.exe -y -i jpg/2.mp4 -i jpg/4.mp4 -filter_complex "[0:v:0] [1:v:0] concat=n=2:v=1 [v] " -map "[v]" -vcodec libx264 -preset ultrafast jpg/5.mp4

#6，过渡到原始放大后的图片
#./ffmpeg.exe -y -ss 1 -i jpg/5.mp4 -i jpg/3.mp4 -filter_complex "blend=all_expr='if(slideleftt,1,2)'" jpg/6.mp4
./ffmpeg.exe -y -i jpg/0.mp4 -filter_complex "scale=w=$zoom*$w:h=$zoom*$h,crop=$w:$h:$w*($zoom-1)/2:($zoom-1)*$h" -vcodec libx264 -preset ultrafast -t 2 jpg/6.mp4
#7 
./ffmpeg.exe -y -ss 2 -i jpg/5.mp4 -i jpg/6.mp4 -filter_complex "blend=all_expr='if(slideleftt,1,2)'" -t 2 jpg/7.mp4

#./ffmpeg.exe -y -ss 3 -i jpg/5.mp4 -i jpg/3.mp4 -filter_complex "blend=all_expr='if(slideleftt,1,2)'" jpg/6.mp4
#8 合并5 7
./ffmpeg.exe -y -t 2 -i jpg/5.mp4 -i jpg/7.mp4 -filter_complex "[0:v:0] [1:v:0] concat=n=2:v=1 [v] " -map "[v]" -vcodec libx264 -preset ultrafast jpg/8.mp4
COMMENT

<<COMMENT
#循环叠加特效视频+ music: video, effect, music, ass
#问题：使用这种方式, 如果没有anullsrc，那么最后的视频长度将是视频和音频的最小值!uncomplete tmp
./ffmpeg_r.exe -y -i /c/Users/user/Videos/output.avi \
    -itsoffset 0 \
    -i /c/shareproject/effect_video/example/templates/Happy/videos/1.flv \
    -itsoffset 5.5 \
    -i /c/shareproject/effect_video/example/templates/Happy/videos/1.flv \
    -i jpg/longlovechina2.wav \
    -f lavfi -i anullsrc=channel_layout=5.1:sample_rate=48000 \
    -filter_complex \
    " \
    [0:v][1:v]overlay=x=0:y=0,setpts=PTS-STARTPTS[video1]; \
    [video1][2:v]overlay=x=0:y=0,setpts=PTS-STARTPTS[last]; \
    [last]ass=jpg/subtitle.ass[vout]; \
    [3:a]atrim=0:5,asetpts=PTS-STARTPTS[aud1]; \
    [4:a]atrim=0:5,asetpts=PTS-STARTPTS[aud2]; \
    [aud1][aud2]concat=n=2:v=0:a=1[aout] \
    " \
    -t 10 -map "[vout]" -map "[aout]" \
    jpg/drawbox1.avi 
#ffmpeg -i INPUT1 -i INPUT2 -i INPUT3 -filter_complex amix=inputs=3:duration=first:dropout_transition=3 OUTPUT

    #"movie=filename=/c/shareproject/effect_video/example/happy/1.flv:loop=200,hue=s=0[lizhi1] 

COMMENT
<<COMMENT
#叠加所有: video, effect, music, ass
#[video1]ass=jpg/subtitle.ass[vout]; 
./ffmpeg_r.exe -y -i /c/Users/user/Videos/output.avi \
    -itsoffset 1 \
    -i /c/shareproject/effect_video/example/templates/Happy/videos/1.flv \
    -i jpg/longlovechina2.wav \
    -filter_complex \
    " \
    [0:v][1:v]overlay=x=0:y=0,setpts=PTS-STARTPTS,ass=jpg/subtitle.ass[vout]; \
    [2:a]asetpts=PTS-STARTPTS[aout] \
    " \
    -map "[vout]" -map "[aout]" \
    -vcodec h264 -preset ultrafast -t 2 -b:v 3000k \
    jpg/drawbox1.avi 
COMMENT


#增加大量粒子视频
<<COMMENT
./ffmpeg -y -i /c/shareproject/effect_video/example/concat.avi -itsoffset  0 -i  /c/shareproject/effect_video/example/happy/1.flv \
-itsoffset  6 -i  /c/shareproject/effect_video/example/happy/2.flv -itsoffset  15.6 -i  /c/shareproject/effect_video/example/happy/3.flv -itsoffset  24.97 \
-i  /c/shareproject/effect_video/example/happy/4.flv -itsoffset  26.47 -i  /c/shareproject/effect_video/example/happy/5.flv -itsoffset  27.64 -i  /c/shareproject/effect_video/example/happy/6.flv \
-itsoffset  29.04 -i  /c/shareproject/effect_video/example/happy/7.flv -itsoffset  30.21 -i  /c/shareproject/effect_video/example/happy/8.flv -itsoffset 31.14 \
-i /c/shareproject/effect_video/example/happy/9.flv -itsoffset 32.91 -i /c/shareproject/effect_video/example/happy/10.flv -itsoffset 38.91 -i /c/shareproject/effect_video/example/happy/11.flv \
-itsoffset 44.91 -i /c/shareproject/effect_video/example/happy/12.flv -itsoffset 50.91 -i /c/shareproject/effect_video/example/happy/13.flv -itsoffset 56.91 \
-i /c/shareproject/effect_video/example/happy/14.flv -itsoffset 67.03 -i /c/shareproject/effect_video/example/happy/15.flv \
-filter_complex "\
[0:v][1:v]overlay=x=0:y=0[video1];[video1][2:v]overlay=x=0:y=0[video2];\
[video2][3:v]overlay=x=0:y=0[video3];[video3][4:v]overlay=x=0:y=0[video4];\
[video4][5:v]overlay=x=0:y=0[video5];[video5][6:v]overlay=x=0:y=0[video6];\
[video6][7:v]overlay=x=0:y=0[video7];[video7][8:v]overlay=x=0:y=0[video8];\
[video8][9:v]overlay=x=0:y=0[video9];[video9][10:v]overlay=x=0:y=0[video10];\
[video10][11:v]overlay=x=0:y=0[video11];[video11][12:v]overlay=x=0:y=0[video12];\
[video12][13:v]overlay=x=0:y=0[video13];[video13][14:v]overlay=x=0:y=0[video14];\
[video14][15:v]overlay=x=0:y=0\
"  \
-vcodec h264 -preset ultrafast -t 70.78 -b:v 1200k \
/c/shareproject/effect_video/example/tmp_0.avi
COMMENT
<<COMMENT
./ffmpeg_r -y -f concat -i lizilist.txt -c:v copy  jpg/lizi.avi
#需要增加lte,否则有些粒子会一直覆盖到最后
#增加lizi、music
./ffmpeg -y -i /c/shareproject/effect_video/example/tmp/0.3.avi -itsoffset  1 -i jpg/lizi.avi -i jpg/lovechina1.mp3 -i jpg/lovechina1.mp3 \
-filter_complex "\
[0:v][1:v]overlay=x='if(lte(t,10),(main_w-overlay_w)/2-200,nan)':y=(main_h-overlay_h)/2-200,setpts=PTS-STARTPTS[vout];\
[1:a]atrim=1:2,asetpts=PTS-STARTPTS[aud1]; \
[2:a]atrim=0:1,asetpts=PTS-STARTPTS[aud2]; \
[aud1][aud2]concat=n=2:v=0:a=1[aout] \
"  \
 -map "[vout]" -map "[aout]" -vcodec h264 -preset ultrafast -t 10 -b:v 3000k \
/c/shareproject/effect_video/example/tmp/tmp_0.avi
COMMENT
#time ./ffmpeg_r.exe -y  -i jpg/img001.jpg.mp4 -filter_complex "zoompan=z='0.5',pad=iw:ih:10:10:black" -t 4 jpg/simple2.mp4

<<COMMENT
./ffmpeg -y -i /c/shareproject/effect_video/example/tmp_1.avi -i /c/shareproject/effect_video/example/tmp_3.avi -i /c/shareproject/effect_video/example/tmp_4.avi \
    -filter_complex "[0:v]scale=331:246,pad=1024:256:10:10:black[mid0];\
    [1:v]scale=331:246,pad=341:256:10:10:black[mid1];\
    [mid0][mid1]overlay=341:0[ref30000];\
    [2:v]scale=331:246,pad=341:256:10:10:black[mid2];\
    [ref30000][mid2]overlay=2*341:0" \
    -vcodec h264 -preset ultrafast -t 10 -b:v 1200k /c/shareproject/effect_video/example/tmp_5.avi
COMMENT

<<COMMENT
##虚幻2侧
./ffmpeg_r.exe -y -i /c/Users/user/Documents/My\ Videos/IMG_1924.MP4 -filter_complex \
    '[0:v]scale=ih*16/9:-1,boxblur=luma_radius=min(h\,w)/20:luma_power=1:chroma_radius=min(cw\,ch)/20:chroma_power=1[bg];[bg][0:v]overlay=(W-w)/2:(H-h)/2,crop=h=iw*9/16' -f avi jpg/IMG_1924.avi
#./ffmpeg_r.exe -y -i jpg/512.4.mp4 -filter_complex \
#    'scale=ih*16/9:-1,boxblur=luma_radius=min(h\,w)/20:luma_power=1:chroma_radius=min(cw\,ch)/20:chroma_power=1[bg];[bg][0:v]overlay=(W-w)/2:(H-h)/2,crop=h=iw*9/16,scale=512*384' /c/shareproject/effect_video/example/tmp_0.mp4
COMMENT

#<<COMMENT
ffmpeg -i  -filter_complex "[0:v]colorchannelmixer=0.98786:0.01538:0.00378:0.0:0.00698:0.99372:0.00336:0.0:0.00544:0.01068:0.98262:0.0[colorchannelmixed]; \
[colorchannelmixed]eq=0.85:0.0175:0.02:1.0:1.0:1.0:1.0:1.0[color_effect]" -map [color_effect] -c:v png output_image
#COMMENT
