TARGET = qttext
TEMPLATE = app
QT += widgets multimedia multimediawidgets
QT += core opengl
CONFIG -= app_bundle
CONFIG += c++11
CONFIG +=debug_and_release 

QMAKE_CFLAGS += -D__STDC_CONSTANT_MACROS -Wno-deprecated -Wno-unused-function -Wno-sign-compare -Wno-unused-variable -Wno-unused-but-set-variable -Wno-reorder -Wno-switch -fopenmp
QMAKE_CXXFLAGS += -D__STDC_CONSTANT_MACROS -Wno-deprecated -Wno-unused-function -Wno-sign-compare -Wno-unused-variable -Wno-unused-but-set-variable -Wno-reorder -Wno-switch -fopenmp

#INCLUDEPATH += ../SimpleOpenGLImageLibrary/src
INCLUDEPATH += c:/msys64/mingw64/include/freetype2/ 
INCLUDEPATH += c:/msys64/mingw64/include/SDL2/
INCLUDEPATH += c:/shareproject/MovieMakeAdvance/libffmpeg_64/header/

QMAKE_CFLAGS += -D__STDC_CONSTANT_MACROS -Wno-deprecated -Wno-unused-function -Wno-sign-compare -Wno-unused-variable -Wno-unused-but-set-variable -Wno-reorder -Wno-switch -fopenmp -Wno-deprecated-declarations
QMAKE_CXXFLAGS += -D__STDC_CONSTANT_MACROS -Wno-deprecated -Wno-unused-function -Wno-sign-compare -Wno-unused-variable -Wno-unused-but-set-variable -Wno-reorder -Wno-switch -fopenmp -Wno-deprecated-declarations

RESOURCES  = effects.qrc

#LIBS += -lfreetype 
LIBS += c:/shareproject/MovieMakeAdvance/libffmpeg_64/lib/libavformat.a \
        c:/shareproject/MovieMakeAdvance/libffmpeg_64/lib/libavfilter.a \
        c:/shareproject/MovieMakeAdvance/libffmpeg_64/lib/libavcodec.a \
        c:/shareproject/MovieMakeAdvance/libffmpeg_64/lib/libavresample.a \
        c:/shareproject/MovieMakeAdvance/libffmpeg_64/lib/libavutil.a \
        c:/shareproject/MovieMakeAdvance/libffmpeg_64/lib/libswscale.a \
        c:/shareproject/MovieMakeAdvance/libffmpeg_64/lib/libpostproc.a \
        c:/shareproject/MovieMakeAdvance/libffmpeg_64/lib/libswresample.a \
        -L c:/msys64/mingw64/bin/ \
        -lcelt0-2 -lgsm -liconv -llzma-5 -lmp3lame-0 -lopencore-amrnb-0 -lopencore-amrwb-0 \
        -lopus-0 -lschroedinger-1.0-0 -lorc-0.4-0 -lspeex-1 \
        -ltheoradec-1 -ltheoraenc-1 -logg-0 -lvorbis-0 -lvorbisenc-2 \
        -lvpx-1 -lwavpack-1 -lx264-148 -lx265 -lcaca-0 \
        -lass-9 -lfontconfig-1 -lexpat-1 -lfreetype-6 -lbz2-1 -lharfbuzz-0 \
        -lintl-8 -lpcre-1 -lgraphite2 -lpng16-16 -lfribidi-0 \
        -lbluray-2 \
        -lxml2-2 -lmodplug-1 -lrtmp-1 -lgmp-10 -lgnutls-30 -lhogweed-4 -lnettle-6 \
        -lidn2-0 -lp11-kit-0 -lffi-6 -ltasn1-6 -lwsock32 -lWs2_32 -lz \
        -lSecur32 -lfdk-aac -lxvidcore \


#        x86/rasterizer.o x86/blend_bitmaps.o x86/blur.o x86/cpuid.o
LIBS += -lSDL2

SOURCES +=  \
    main.cpp \
    vector.c \
    utf8-utils.c \
    edtaa3func.c \
    distance-field.c \
    texture-atlas.c \
    platform.c \
    texture-font.c \
    font-manager.c \
    mat4.c \
    GLDisplayWidget.cpp \
    GLHiddenWidget.cpp \
    MMComm.cpp \
    mainwindow.cpp \
    PlayerPrivate.cpp \
    ass_string.c \
    ass_utils.c \
    ass_strtod.c \
    ass_directwrite.c \
    ass_fontselect.c \
    ass_library.c \
    ass_outline.c \
    ass_cache.c \
    ass_font.c \
    ass_bitmap.c \
    ass.c \
    ass_blur.c \
    GraphicsScene.cpp \
    OpenGLView.cpp \



HEADERS  +=  \
    vector.h \
    utf8-utils.h \
    edtaa3func.h \
    distance-field.h \
    vec234.h \
    texture-atlas.h \
    platform.h \
    texture-font.h \
    markup.h \
    font-manager.h \
    mat4.h \
    markup.h \
    GLDisplayWidget.h \
    GLHiddenWidget.h \
    MMComm.h \
    FFmpegUtilities.h \
    mainwindow.h \
    PlayerPrivate.h \
    dwrite_c.h \
    ass_compat.h \
    ass_config.h \
    ass_types.h \
    ass_string.h \
    ass_utils.h \
    x86/cpuid.h \
    ass_directwrite.h \
    ass_fontselect.h \
    ass_library.h \
    ass_outline.h \
    ass_cache.h \
    ass_font.h \
    ass_bitmap.h \
    ass.h \
    ass_cache_template.h \
    ass_func_template.h \
    GraphicsScene.h \
    OpenGLView.h \

QMAKE_EXTRA_COMPILERS += nasm
NASMEXTRAFLAGS = -DARCH_X86_64=1 -DPIC -f win64 -DHAVE_ALIGNED_STACK=1 -DHAVE_CPUNOP=0 -Dprivate_prefix=ass
OTHER_FILES += $$NASM_SOURCES
nasm.output = x86/${QMAKE_FILE_BASE}.o
nasm.commands = nasm $$NASMEXTRAFLAGS -o x86/${QMAKE_FILE_BASE}.o ${QMAKE_FILE_NAME}
nasm.input = NASM_SOURCES
NASM_SOURCES += \
    x86/be_blur.asm \
    x86/blend_bitmaps.asm \
    x86/blur.asm \
    x86/cpuid.asm \
    x86/gaussian.asm \
    x86/rasterizer.asm \
    x86/utils.asm \
    x86/x86inc.asm
