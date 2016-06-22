#!/bin/bash

<<COMMENT
@author: ginozhang
@date: 2016-06-21
@description: 
COMMENT

DS=1.0 # display start
DE=5.0 # display end
FID=1.5 # fade in duration
FOD=5 # fade out duration

dir="/c/QtProjects";
#ffmpeg -i $dir/qtmovie/lovechina1.mp3 -pattern_type glob -framerate 1 -i "$dir/qtmovie/jpg/*.jpg" $dir/qtmovie/output.mkv
#ffmpeg -vf subtitles=$dir/Blackway.srt -i $dir/qtmovie/lovechina1.mp3 -framerate 1 -i "$dir/qtmovie/jpg/img%3d.jpg" $dir/qtmovie/output.avi
#dir="/c/QtProjects";ffmpeg -i $dir/qtmovie/output.avi -vf drawtext="fontfile=/c/Windows/fonts/simsun.ttc: text='Stack Overflow': fontcolor=white: fontsize=24: box=1: boxcolor=black@0.5: boxborderw=5: x=(w-text_w)/2: y=(h-text_h)/2:fontcolor_expr=ff0000%{eif\\\\: clip(255*(1*between(t\\, $DS + $FID\\, $DE - $FOD) + ((t - $DS)/$FID)*between(t\\, $DS\\, $DS + $FID) + (-(t - $DE)/$FOD)*between(t\\, $DE - $FOD\\, $DE) )\\, 0\\, 255) \\\\: x\\\\: 2 }" -codec:a copy out.avi
#dir="/c/QtProjects";ffmpeg -i $dir/qtmovie/output.avi -vf drawtext="fontfile=/c/Windows/fonts/simsun.ttc: fontcolor=white:x=100:y=x/dar:enable=lt(mod(t\,3)\,1):text='blink'" -codec:a copy out.avi

#dir="/c/QtProjects";ffmpeg -i $dir/qtmovie/lovechina1.mp3 -framerate 1 -i "$dir/qtmovie/jpg/img%3d.jpg" -vf drawtext="fontfile=/c/Windows/fonts/simsun.ttc: fontcolor=black:x=200:y=x/dar:enable=lt(mod(t\,3)\,1):text='bb'" $dir/qtmovie/output.avi
#dir="/c/QtProjects";ffmpeg -i $dir/qtmovie/lovechina1.mp3 -framerate 1 -i "$dir/qtmovie/jpg/img%3d.jpg" -vf drawtext="expansion=none: x=100:y=x/dar: fontsize=60: enable=lt(mod(t\,3)\,1):text='blink'"  $dir/qtmovie/output.avi
#dir="/c/QtProjects/qtmovie";ffmpeg -i $dir/lovechina1.mp3 -framerate 1 -i "$dir/jpg/img%3d.jpg" -vf drawtext="enable='gte(t,3)':fontfile=/c/Windows/fonts/simsun.ttc: fontcolor=white: fontsize=60: textfile=text.txt:reload=1:y=h-line_h-10:x=(W/tw)*n"  $dir/output.avi
# ok
#dir="/c/QtProjects/qtmovie";ffmpeg -i $dir/lovechina1.mp3 -framerate 1 -i "$dir/jpg/img%3d.jpg" -vf subtitles=subtitle.srt  $dir/output.avi
dir="/c/QtProjects/qtmovie";ffmpeg -i $dir/lovechina1.mp3 -framerate 1 -i "$dir/jpg/img%3d.jpg" -vf ass=subtitle.ass  $dir/output.avi

#dir="/c/QtProjects/qtmovie";ffmpeg -i output.avi -i $dir/sub.ass -vf ass=sub.ass   $dir/out.avi

#ffplay -f lavfi "color,drawtext=text=TEST:fontsize=50:fontfile=FreeSerif.ttf:fontcolor_expr=ff0000%{eif\\\\: clip(255*(1*between(t\\, $DS + $FID\\, $DE - $FOD) + ((t - $DS)/$FID)*between(t\\, $DS\\, $DS + $FID) + (-(t - $DE)/$FOD)*between(t\\, $DE - $FOD\\, $DE) )\\, 0\\, 255) \\\\: x\\\\: 2 }"
