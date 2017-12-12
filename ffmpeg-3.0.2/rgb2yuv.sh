#!/bin/bash

<<COMMENT
@author: storm
@date: 2016-12-21
@description: 
COMMENT

dir_tools='/usr/local/avs/public_tools'
r="";
b="";
g="";
function usage()
{
    echo
    echo "Usage: $0 [OPTION...] argv"
    echo "功能: "
    echo
    echo "OPTION"
    echo "  -h|--help                 help"
    echo "  -r|--red                 "
    echo "  -g|--green                 "
    echo "  -b|--blue                 "
    echo
}
while [ $# -gt 0 ]; do
    case "$1" in
        "--help" | "-h")
            usage; exit 0 ;;
        "--red" | "-r")
            shift; if [ $# -gt 0 ]; then r="$1"; fi ;;
        "--green" | "-g")
            shift; if [ $# -gt 0 ]; then g="$1"; fi ;;
        "--blue" | "-b")
            shift; if [ $# -gt 0 ]; then b="$1"; fi ;;
        *)
            break;;
    esac
    shift;
done

if [ "$r" == "" ] || [ "$g" == "" ] || [ "$b" == "" ];then
    usage
    exit 0;
fi

#y=$(echo "(0.257 * $r) + (0.504 * $g) + (0.098 * $b) + 16" | bc)
#v=$(echo "(0.439 * $r) - (0.368 * $g) - (0.071 * $b) + 128" | bc)
#u=$(echo "-(0.148 * $r) - (0.291 * $g) + (0.439 * $b) + 128" | bc)
y=$(perl -e "printf((0.257 * $r) + (0.504 * $g) + (0.098 * $b) + 16);");
v=$(perl -e "printf((0.439 * $r) - (0.368 * $g) - (0.071 * $b) + 128);");
u=$(perl -e "printf(-(0.148 * $r) - (0.291 * $g) + (0.439 * $b) + 128);");
echo "y: $y u: $u v: $v"
