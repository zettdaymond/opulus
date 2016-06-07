cmake_minimum_required(VERSION 2.8.12)

project(opulus)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

set(QT_MIN_VERSION    "5.0")
find_package(Qt5 COMPONENTS Core Widgets Xml Gui PrintSupport Svg Test REQUIRED)

find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
endif(CCACHE_FOUND)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(translation.cmake)
#SET(CMAKE_POSITION_INDEPENDENT_CODE ON)

if(NOT CMAKE_BUILD_TYPE)
	message(STATUS "Build type not especified, building as release.")
        message(STATUS "Use   -DCMAKE_BUILD_TYPE=debug|release to set the build type.")
	set(CMAKE_BUILD_TYPE release)
endif(NOT CMAKE_BUILD_TYPE)

# adtional compiler options for release and debug modes
add_definitions(-std=c++14 -Wall -Wextra)
if (${CMAKE_BUILD_TYPE} STREQUAL release)
        add_definitions(-DQT_NO_DEBUG -O3)
else (${CMAKE_BUILD_TYPE} STREQUAL release)
        add_definitions(-ggdb)
endif (${CMAKE_BUILD_TYPE} STREQUAL release)

#compiller flags
add_definitions(-fdata-sections -ffunction-sections -Wl,--gc-sections)
#linker flags
#set(GCC_COVERAGE_LINKER_FLAGS "-static -static-libgcc -static-libstdc++ -Xlinker --allow-multiple-definition ")
#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINKER_FLAGS}")
#set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${GCC_COVERAGE_LINKER_FLAGS}")
#set(CMAKE_STATIC_LINKER_FLAGS "${CMAKE_STATIC_LINKER_FLAGS} ${GCC_COVERAGE_LINKER_FLAGS}")

#TODO: Detect in future
set(QT_STATIC 1)

if(STATIC_QT_BUILD)
    if(WIN32)
        if(QT_STATIC)
            add_definitions(-DSTATIC_QT_BUILD)
            add_definitions(-static -static-libstdc++ -static-libgcc)
            set(STATIC_PLUGINS 1)
            set(STATIC_LIBS 1)
        else()
            message( FATAL_ERROR "Your vesion of Qt is not STATIC. Could not statically link to non static Qt." )
        endif()
    else()
        message( FATAL_ERROR "Static link to Qt implemented only for Windows. Cmake will exit." )
    endif(WIN32)
endif(STATIC_QT_BUILD)

#if variable not set, set default 0 value to nice printing build status.
if(NOT STATIC_QT_BUILD)
    set (STATIC_QT_BUILD 0)
endif(NOT STATIC_QT_BUILD)

if(NOT STATIC_LIBS)
    set (STATIC_LIBS 0)
endif(NOT STATIC_LIBS)

if(NOT STATIC_PLUGINS)
    set (STATIC_PLUGINS 0)
endif(NOT STATIC_PLUGINS)

#additional features
if(STATIC_PLUGINS)
    add_definitions(-DSTATIC_PLUGINS)
    message("Plugins link : STATIC")
else(STATIC_PLUGINS)
    message("Plugins link : SHARED")
endif(STATIC_PLUGINS)


#print options status
message("BUILD STATUS:")
message("# BUILD_TYPE: " ${CMAKE_BUILD_TYPE})
message("# STATIC_QT_BIULD: " ${STATIC_QT_BUILD})
message("# STATIC_LIBS: " ${STATIC_LIBS})
message("# STATIC_PLUGINS: " ${STATIC_PLUGINS})


add_subdirectory(core)
add_subdirectory(gui)
add_subdirectory(main)
add_subdirectory(plugins)
add_subdirectory(tests)
add_subdirectory(windowsinstaller)

# i18n stuff
install(FILES ${Opulus_QMS} DESTINATION share/opulus/i18n)

# .desktop file
install(FILES opulus.desktop DESTINATION share/applications)
# icons
install(FILES opulus16.png DESTINATION share/icons/hicolor/16x16/apps RENAME opulus.png)
install(FILES opulus22.png DESTINATION share/icons/hicolor/22x22/apps RENAME opulus.png)
install(FILES opulus32.png DESTINATION share/icons/hicolor/32x32/apps RENAME opulus.png)
install(FILES opulus48.png DESTINATION share/icons/hicolor/48x48/apps RENAME opulus.png)
