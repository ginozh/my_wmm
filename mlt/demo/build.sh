#!/bin/bash

<<COMMENT
@author: storm
@date: 2019-02-26
@description: 
COMMENT

cc -DARCH_X86_64 -Wall -DPIC    -DUSE_MMX -DUSE_SSE -DUSE_SSE2 -g -D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE -fPIC -pthread -I.. -rdynamic -DVERSION=\"6.13.0\" -DHAVE_SDL2 -D_REENTRANT -I/usr/include/SDL2 -I/usr/include/mlt  -c -o hello_world.o hello_world.c
cc -o hello_world hello_world.o -Wl,--no-undefined -Wl,--as-needed -L../framework -lmlt -lpthread -lSDL2
