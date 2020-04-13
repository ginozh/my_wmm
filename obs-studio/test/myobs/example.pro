TARGET = myobs
TEMPLATE = app
QT += core gui widgets svg 
QT += opengl
CONFIG -= app_bundle
CONFIG += c++11

INCLUDEPATH += .
INCLUDEPATH += ./obs-frontend-api
INCLUDEPATH += ../../libobs 
INCLUDEPATH += ../../deps/libff
INCLUDEPATH += ../../deps/json11
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

LIBS += ../../build/rundir/DEBUG/bin/libobs.0.dylib -L/usr//local/lib/ -lavformat -lavutil -lswscale -lswresample -lavcodec

win32 {
#	include_directories(${OBS_JANSSON_INCLUDE_DIRS})
#	include_directories(${BLAKE2_INCLUDE_DIR})
SOURCES +=  \
    platform-windows.cpp \
    win-update/update-window.cpp \
    win-update/win-update.cpp \
    win-update/win-update-helpers.cpp \

HEADERS  += \
    win-update/update-window.hpp \
    win-update/win-update.hpp \
    win-update/win-update-helpers.hpp \

LIBS += -lcrypt32 -lblake2
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
	obs-app.cpp \
	window-dock.cpp \
	api-interface.cpp \
	window-basic-main.cpp \
	window-basic-stats.cpp \
	window-basic-filters.cpp \
	window-basic-settings.cpp \
	window-basic-interaction.cpp \
	window-basic-properties.cpp \
	window-basic-auto-config.cpp \
	window-basic-main-outputs.cpp \
	window-basic-source-select.cpp \
	window-basic-settings-stream.cpp \
	window-basic-auto-config-test.cpp \
	window-basic-main-scene-collections.cpp \
	window-basic-main-transitions.cpp \
	window-basic-main-dropfiles.cpp \
	window-basic-main-profiles.cpp \
	window-basic-main-browser.cpp \
	window-basic-main-icons.cpp \
	window-basic-status-bar.cpp \
	window-basic-adv-audio.cpp \
	window-basic-transform.cpp \
	window-basic-preview.cpp \
	window-basic-about.cpp \
	window-namedialog.cpp \
	window-log-reply.cpp \
	window-projector.cpp \
	window-remux.cpp \
	auth-base.cpp \
	source-tree.cpp \
	scene-tree.cpp \
	properties-view.cpp \
	focus-list.cpp \
	menu-button.cpp \
	double-slider.cpp \
	slider-ignorewheel.cpp \
	combobox-ignorewheel.cpp \
	spinbox-ignorewheel.cpp \
	record-button.cpp \
	ui-validation.cpp \
	url-push-button.cpp \
	volume-control.cpp \
	adv-audio-control.cpp \
	item-widget-helpers.cpp \
	horizontal-scroll-area.cpp \
	vertical-scroll-area.cpp \
	visibility-item-widget.cpp \
	slider-absoluteset-style.cpp \
	qt-display.cpp \
	crash-report.cpp \
	hotkey-edit.cpp \
	source-label.cpp \
	remote-text.cpp \
	audio-encoders.cpp \
	qt-wrappers.cpp \
	obs-frontend-api/obs-frontend-api.cpp \
	../../deps/libff/libff/ff-util.c \
	../../deps/json11/json11.cpp \


HEADERS  += \
	obs-app.hpp \
	platform.hpp \
	window-dock.hpp \
	window-main.hpp \
	window-basic-main.hpp \
	window-basic-stats.hpp \
	window-basic-filters.hpp \
	window-basic-settings.hpp \
	window-basic-interaction.hpp \
	window-basic-properties.hpp \
	window-basic-auto-config.hpp \
	window-basic-main-outputs.hpp \
	window-basic-source-select.hpp \
	window-basic-about.hpp \
	window-basic-status-bar.hpp \
	window-basic-adv-audio.hpp \
	window-basic-transform.hpp \
	window-basic-preview.hpp \
	window-namedialog.hpp \
	window-log-reply.hpp \
	window-projector.hpp \
	window-remux.hpp \
	auth-base.hpp \
	source-tree.hpp \
	scene-tree.hpp \
	properties-view.hpp \
	properties-view.moc.hpp \
	display-helpers.hpp \
	balance-slider.hpp \
	double-slider.hpp \
	slider-ignorewheel.hpp \
	combobox-ignorewheel.hpp \
	spinbox-ignorewheel.hpp \
	focus-list.hpp \
	menu-button.hpp \
	mute-checkbox.hpp \
	record-button.hpp \
	ui-validation.hpp \
	url-push-button.hpp \
	volume-control.hpp \
	adv-audio-control.hpp \
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
	hotkey-edit.hpp \
	source-label.hpp \
	remote-text.hpp \
	audio-encoders.hpp \
	qt-wrappers.hpp \
	clickable-label.hpp \
	obs-frontend-api/obs-frontend-api.h \
	obs-frontend-api/obs-frontend-internal.hpp \
	../../deps/libff/libff/ff-util.h \
	../../deps/json11/json11.hpp \


