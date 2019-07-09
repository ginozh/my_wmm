#!/bin/bash

<<COMMENT
@author: storm
@date: 2019-07-09
@description: 
COMMENT

dir_tools='/usr/local/avs/public_tools'
#path=$(readlink -f $0)
#dir=$(dirname $path); #当前脚本目录

#source $dir_tool/oneprocess.sh
#fun_runone $0; #保证只有一个进程

#ip_local=$($dir_tools/getprivateip.pl); #本地ip

function usage()
{
    echo
    echo "Usage: $0 [OPTION...] file"
    echo "功能: "
    echo
    echo "OPTION"
    echo "  -h|--help                 help"
    echo
}
while [ $# -gt 0 ]; do
    case "$1" in
        "--help" | "-h")
            usage; exit 0 ;;
        *)
            break;;
    esac
    shift;
done

if [ $# -ne 1 ]; then
    usage
    exit 0;
fi
file=$1; #upan/jpg/WeChatSight2266.mp4
name=$(echo "$file" |awk -F'/' '{print $NF}');
dstfile="logs/${name}.log";
ffprobe $file > $dstfile 2>&1;
melt $file -consumer avformat:output.mp4 acodec=aac ab=256k vcodec=libx264 s=1280x720 r=20 >> $dstfile 2>&1
cp output.mp4 upan/tmp/
