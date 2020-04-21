TARGET = myobs
TEMPLATE = app
QT += core gui widgets svg 
QT += opengl
CONFIG -= app_bundle
CONFIG += c++11

INCLUDEPATH += .
INCLUDEPATH += ./obs-frontend-api
INCLUDEPATH += ../../libobs 
#INCLUDEPATH += ../../deps/libff
#INCLUDEPATH += ../../deps/json11
INCLUDEPATH += /usr/local/include


DEFINES += __STDC_FORMAT_MACROS

########Release 模式下可以调试#############
#QMAKE_CXXFLAGS_RELEASE = $$QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO
#QMAKE_LFLAGS_RELEASE = $$QMAKE_LFLAGS_RELEASE_WITH_DEBUGINFO
CONFIG+=debug_and_release 
CONFIG(debug, debug|release) {
} else {
    QMAKE_CFLAGS += -DQT_NO_DEBUG_OUTPUT
    QMAKE_CXXFLAGS += -DQT_NO_DEBUG_OUTPUT
}

macx:{
LIBS += ../../build/rundir/DEBUG/bin/libobs.0.dylib
}
win32 {
LIBS += ../../build/libobs/Debug/obs.lib
}
#LIBS += -L/usr//local/lib/ -lavformat -lavutil -lswscale -lswresample -lavcodec

win32 {
#	include_directories(${OBS_JANSSON_INCLUDE_DIRS})
#	include_directories(${BLAKE2_INCLUDE_DIR})
SOURCES +=  \
    platform-windows.cpp \

HEADERS  += \


#LIBS += -lcrypt32 -lblake2
# OBS_JANSSON_IMPORT
#if(CMAKE_SIZEOF_VOID_P EQUAL 4)
# set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /LARGEADDRESSAWARE")


}
macx:{
QT += macextras
LIBS += -framework AppKit
QMAKE_CFLAGS += -fobjc-arc
QMAKE_CXXFLAGS += -fobjc-arc
SOURCES +=  \
    platform-osx.mm

HEADERS  += \

}

SOURCES += \
	main.cpp \
    record-global.cpp \
	api-interface.cpp \
	window-basic-main.cpp \
	window-basic-auto-config.cpp \
	window-basic-auto-config-test.cpp \
	window-basic-main-outputs.cpp \
	window-basic-main-scene-collections.cpp \
	window-basic-main-transitions.cpp \
	window-basic-main-dropfiles.cpp \
	window-basic-main-profiles.cpp \
	window-basic-main-icons.cpp \
	window-basic-preview.cpp \
	window-projector.cpp \
	source-tree.cpp \
	scene-tree.cpp \
	properties-view.cpp \
	focus-list.cpp \
	slider-ignorewheel.cpp \
	combobox-ignorewheel.cpp \
	spinbox-ignorewheel.cpp \
	record-button.cpp \
	ui-validation.cpp \
	volume-control.cpp \
	item-widget-helpers.cpp \
	horizontal-scroll-area.cpp \
	vertical-scroll-area.cpp \
	visibility-item-widget.cpp \
	slider-absoluteset-style.cpp \
	qt-display.cpp \
	crash-report.cpp \
	source-label.cpp \
	audio-encoders.cpp \
	qt-wrappers.cpp \
	obs-frontend-api/obs-frontend-api.cpp \
    mainwindow.cpp \


HEADERS  += \
    record-global.hpp \
	platform.hpp \
	window-main.hpp \
	window-basic-main.hpp \
	window-basic-auto-config.hpp \
	window-basic-main-outputs.hpp \
	window-basic-preview.hpp \
	window-projector.hpp \
	source-tree.hpp \
	scene-tree.hpp \
	properties-view.hpp \
	properties-view.moc.hpp \
	display-helpers.hpp \
	slider-ignorewheel.hpp \
	combobox-ignorewheel.hpp \
	spinbox-ignorewheel.hpp \
	focus-list.hpp \
	mute-checkbox.hpp \
	record-button.hpp \
	ui-validation.hpp \
	volume-control.hpp \
	item-widget-helpers.hpp \
	visibility-checkbox.hpp \
	locked-checkbox.hpp \
	horizontal-scroll-area.hpp \
	expand-checkbox.hpp \
	vertical-scroll-area.hpp \
	visibility-item-widget.hpp \
	slider-absoluteset-style.hpp \
	qt-display.hpp \
	crash-report.hpp \
	source-label.hpp \
	audio-encoders.hpp \
	qt-wrappers.hpp \
	obs-frontend-api/obs-frontend-api.h \
	obs-frontend-api/obs-frontend-internal.hpp \
    mainwindow.h \


