
PREFIX=/usr
CFLAGS+=-DAVDATADIR=\"/usr/share/ffmpeg/\"
CFLAGS+=
LDFLAGS+=-lavcodec -lvpx -lm -lvpx -lm -lvpx -lm -lvpx -lm -pthread -lm -lopencore-amrwb -lz -lfdk-aac -lgsm -lmp3lame -lm -lopencore-amrnb -lopus -lspeex -ltheoraenc -ltheoradec -logg -lvorbis -lvorbisenc -lwavpack -lx264 -lx265 -lxvidcore -ldl -lswresample -lm -lavutil -pthread -lm -ldl
CFLAGS+=
LDFLAGS+=-lavutil -pthread -lm -ldl
CFLAGS+=
LDFLAGS+=-lavformat -lm -lmodplug -lbluray -lz -lgnutls -lrtmp -lz -lgmp -lavcodec -lvpx -lm -lvpx -lm -lvpx -lm -lvpx -lm -pthread -lm -lopencore-amrwb -lz -lfdk-aac -lgsm -lmp3lame -lm -lopencore-amrnb -lopus -lspeex -ltheoraenc -ltheoradec -logg -lvorbis -lvorbisenc -lwavpack -lx264 -lx265 -lxvidcore -ldl -lswresample -lm -lavutil -pthread -lm -ldl
CFLAGS+=
LDFLAGS+=-lswscale -lm -lavutil -pthread -lm -ldl
CFLAGS+=
LDFLAGS+=-lavdevice -lXv -lX11 -lXext -lm -lopenal -lxcb -lxcb-shape -lxcb -lxcb-xfixes -lxcb-render -lxcb-shape -lxcb -lasound -lcaca -lSDL2 -lsndio -lavfilter -pthread -lm -lass -lfontconfig -lfreetype -ldl -lavresample -lm -lswscale -lm -lavformat -lm -lmodplug -lbluray -lz -lgnutls -lrtmp -lz -lgmp -lavcodec -lvpx -lm -lvpx -lm -lvpx -lm -lvpx -lm -pthread -lm -lopencore-amrwb -lz -lfdk-aac -lgsm -lmp3lame -lm -lopencore-amrnb -lopus -lspeex -ltheoraenc -ltheoradec -logg -lvorbis -lvorbisenc -lwavpack -lx264 -lx265 -lxvidcore -ldl -lswresample -lm -lavutil -pthread -lm -ldl
CFLAGS+=
LDFLAGS+=-lavfilter -pthread -lm -lass -lfontconfig -lfreetype -ldl -lavresample -lm -lswscale -lm -lavformat -lm -lmodplug -lbluray -lz -lgnutls -lrtmp -lz -lgmp -lavcodec -lvpx -lm -lvpx -lm -lvpx -lm -lvpx -lm -pthread -lm -lopencore-amrwb -lz -lfdk-aac -lgsm -lmp3lame -lm -lopencore-amrnb -lopus -lspeex -ltheoraenc -ltheoradec -logg -lvorbis -lvorbisenc -lwavpack -lx264 -lx265 -lxvidcore -ldl -lswresample -lm -lavutil -pthread -lm -ldl
CFLAGS+=
LDFLAGS+=-lswresample -lm -lavutil -pthread -lm -ldl
EXTRA_LIBS=
CODECS=1
FILTERS=1
DEVICES=1
AVFILTER=1
SWRESAMPLE=1
