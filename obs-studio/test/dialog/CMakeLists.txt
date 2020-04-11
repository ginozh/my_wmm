if(DISABLE_UI)
	message(STATUS "UI disabled")
	return()
elseif(ENABLE_UI)
	set(FIND_MODE REQUIRED)
else()
	set(FIND_MODE QUIET)
endif()

project(myobs)

set(CMAKE_INCLUDE_CURRENT_DIR TRUE)
set(CMAKE_AUTOMOC TRUE)

find_package(Qt5Svg ${FIND_MODE})
find_package(Qt5Xml ${FIND_MODE})
find_package(FFmpeg REQUIRED COMPONENTS avcodec avutil avformat)

if(APPLE)
	find_package(Qt5MacExtras REQUIRED)
endif(APPLE)

if(NOT Qt5Widgets_FOUND)
	if (ENABLE_UI)
		message(FATAL_ERROR "Failed to find Qt5")
	else()
		message(STATUS "Qt5 not found - UI disabled")
		return()
	endif()
endif()

include_directories(${FFMPEG_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/libobs")

if(WIN32)

	set(obs_PLATFORM_SOURCES
		platform-windows.cpp
		)
	set(obs_PLATFORM_HEADERS
		win-update/update-window.hpp
		)
	if(CMAKE_SIZEOF_VOID_P EQUAL 4)
		set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /LARGEADDRESSAWARE")
	endif()

elseif(APPLE)
	set(obs_PLATFORM_SOURCES
		platform-osx.mm)

	find_package(AppKit REQUIRED)
	set(obs_PLATFORM_LIBRARIES ${APPKIT_LIBRARIES})

	add_definitions(-fobjc-arc)

endif()

if(MSVC)
	set_source_files_properties(
		../deps/libff/libff/ff-util.c
		PROPERTIES COMPILE_FLAGS -Dinline=__inline
		)
	set(obs_PLATFORM_LIBRARIES
		${obs_PLATFORM_LIBRARIES}
		w32-pthreads)
endif()

set(obs_SOURCES
	${obs_PLATFORM_SOURCES}
	${obs_libffutil_SOURCES}
    main.cpp
    mainwindow.cpp
    )
set(obs_HEADERS
	${obs_PLATFORM_HEADERS}
    mainwindow.h
    )

add_executable(myobs WIN32
    #myobs.manifest
	${obs_SOURCES}
	${obs_HEADERS}
    )

if(WIN32)
	if(CMAKE_SIZEOF_VOID_P EQUAL 8)
		set(_output_suffix "64")
	else()
		set(_output_suffix "32")
	endif()

	set_target_properties(myobs
		PROPERTIES
			OUTPUT_NAME "myobs${_output_suffix}")
endif()

target_link_libraries(myobs
	libobs
	Qt5::Widgets
	Qt5::Svg
	Qt5::Xml
	${FFMPEG_LIBRARIES}
	${obs_PLATFORM_LIBRARIES})

if (APPLE)
	target_link_libraries(myobs
			Qt5::MacExtras)
	set_target_properties(myobs PROPERTIES LINK_FLAGS "-pagezero_size 10000 -image_base 100000000")
endif()

define_graphic_modules(myobs)
