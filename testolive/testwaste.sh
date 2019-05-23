#!/bin/bash

<<COMMENT
@author: storm
@date: 2019-03-20
@description: 
COMMENT

egrep 'video_position' $1 | 
    awk 'BEGIN{t=0;wf=0;wi=0;ws=0;wa=0;ww=0;}
        {t++;wf+=$5;ws+=$7;wa+=$9;ww+=$11;}
    END{print t"\n完全显示在gpu上: "wf/t" (ms)\n从gpu中取图片: "wi/t"\n图片缩放格式转换: "ws/t"\n获取音频帧: "wa/t" \n写入文件: "ww/t;}'
