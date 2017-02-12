#!/bin/bash

<<COMMENT
@author: ginozhang
@date: 2017-02-12
@description: 
COMMENT
arr_files=("jpg/img001.jpg" "jpg/img002.jpg" "jpg/img003.jpg")
w_r=4;h_r=3; #w:h 4:3
#w_r=16;h_r=9;
w=512;h=384;

#1, single original video
t=4; #s
t_s=12;
t_e=12;
f=24; #framerate
w_tmp=$(echo "$w/(3*$w_r)" | bc)
w=$(echo "$w_tmp*3*$w_r" | bc );h=$(echo "$w*$h_r/$w_r" | bc )
echo "w: $w h: $h"
#<<COMMENT
#time ./ffmpeg_r.exe -y  -framerate 24 -i jpg/512img002.jpg -vf "scale=512x384" -t 2 jpg/zoom.512.2.avi;
for file in ${arr_files[0]} ${arr_files[1]} ${arr_files[2]};do
    echo $file;
    time ./ffmpeg.exe -y  -framerate $f -loop 1 -i $file -pix_fmt yuv420p -vf "scale=$w*$h" -t $t $file.mp4
done
#COMMENT

#2, first frame
#2.1, bottom
w_interval=10;
w_single=$(echo "$w/3" | bc); h_single=$(echo "$h/3" | bc);
w_double=$(echo "$w_single * 2" | bc); h_double=$(echo "$h_single * 2" | bc)
h_cutfrom=$(echo "$h_single/2" | bc);
w_scale1=$(echo "$w_single - $w_interval" |bc); h_scale1=$(echo "$h_single - $w_interval" |bc); 
#./ffmpeg.exe -y -i ${arr_files[0]} -i ${arr_files[1]} -i ${arr_files[1]} -filter_complex "[0:v]scale=280:180,pad=900:200:10:10[left];[1:v]scale=280:180,pad=300:200:10:10[middle];[left][middle]overlay=300:0[bleft];[2:v]scale=280:180,pad=300:200:10:10[right];[bleft][right]overlay=600:0"  jpg/simple.avi
./ffmpeg.exe -y -i ${arr_files[0]}.mp4 -i ${arr_files[1]}.mp4 -i ${arr_files[2]}.mp4 -filter_complex "[0:v]scale=$w_scale1:$h_scale1,pad=$w:$h_single:$w_interval:$w_interval[left];[1:v]scale=$w_scale1:$h_scale1,pad=$w_single:$h_single:$w_interval:$w_interval[middle];[left][middle]overlay=$w_single:0[bleft];[2:v]scale=$w_scale1:$h_scale1,pad=$w_single:$h_single:$w_interval:$w_interval[right];[bleft][right]overlay=$w_double:0"  jpg/bottom_tmp.mp4

./ffmpeg.exe -y -i jpg/bottom_tmp.mp4 -i jpg/bottom_tmp.mp4 -filter_complex "blend=all_expr='if(slideleftt,$t,$t)'"  jpg/bottom.mp4

#2.2 top+bottom
#./ffmpeg.exe -y -i ${arr_files[2]}.mp4 -i jpg/bottom.mp4 -filter_complex "[0:v]scale=900*600,crop=900:400:0:100,pad=900:600:0:0[top];[top][1:v]overlay=0:400" jpg/simple.avi
./ffmpeg.exe -y -i ${arr_files[2]}.mp4 -i jpg/bottom.mp4 -filter_complex "[0:v]scale=$w*$h,crop=$w:$h_double:0:$h_cutfrom,pad=$w:$h:0:0[top];[top][1:v]overlay=0:$h_double" jpg/simple1.mp4

#3, second frame
#zoom out && fade out black
zoom_rate=1.2;
interval=$(echo "$zoom_rate - 1" | bc );
./ffmpeg.exe -y  -framerate $f -i ${arr_files[0]} -loop 1  -vf "scale=$w*$h,zoompan=z='if(lte(zoom,1.0),$zoom_rate,max(1.001,zoom-0.01))':mwhx='1':x='(iw-mw)/2':mhhy='1':y='(ih-mh)/2'" -t $t -pix_fmt yuv420p ${arr_files[0]}.zoom.mp4
./ffmpeg.exe -y -i ${arr_files[0]}.zoom.mp4 -vf "fade=t=out:st=1:d=1:color=black" jpg/simple2.mp4 #${arr_files[0]}.zoom.fade.mp4

#4, 3th frame
#pan left && fade in black
#./ffmpeg.exe -y  -framerate 24 -i ${arr_files[0]} -loop 1  -vf "scale=$w*$h,zoompan=z='$zoom_rate':y='(zoom-1)*ih/2':ox='0.2*iw':x='max(x-(zoom-1)*iw/2/$f,0)'" -pix_fmt yuv420p -t $t ${arr_files[0]}.zoom.mp4
./ffmpeg.exe -y  -framerate $f -i ${arr_files[1]} -loop 1  -vf "scale=$w*$h,zoompan=z='$zoom_rate':y='(zoom-1)*ih/2':x='min(x+(zoom-1)*iw/2/$f,$interval*iw)'" -pix_fmt yuv420p -t $t ${arr_files[1]}.zoom.mp4
./ffmpeg.exe -y -i ${arr_files[1]}.zoom.mp4 -vf "fade=t=in:d=1:color=black" jpg/simple3.mp4 #${arr_files[1]}.zoom.fade.mp4

#5, 4th frame
#pan right && reveal right
./ffmpeg.exe -y  -framerate $f -i ${arr_files[2]} -loop 1  -vf "zoompan=z='$zoom_rate':y='(zoom-1)*ih/2':ox='$interval*iw':x='max(x-(zoom-1)*iw/2/$f,0)'" -pix_fmt yuv420p -t $t ${arr_files[2]}.zoom.mp4
./ffmpeg.exe -y -i jpg/simple3.mp4 -i ${arr_files[2]}.zoom.mp4 -filter_complex "blend=all_expr='if(revealright,$t,$t)'" jpg/simple4.mp4

#6, together
file="simple.txt";
echo "file 'jpg/simple1.mp4'" > $file;
echo "file 'jpg/simple2.mp4'" >> $file;
echo "file 'jpg/simple3.mp4'" >> $file;
echo "file 'jpg/simple4.mp4'" >> $file;
echo "file 'jpg/simple1.mp4'" >> $file;
./ffmpeg.exe -y -f concat -i $file jpg/simple.mp4
