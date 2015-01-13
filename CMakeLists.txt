cmake_minimum_required(VERSION 2.8.12)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

set(QT_MIN_VERSION    "5.0")
find_package(Qt5 COMPONENTS Core Widgets Xml Gui PrintSupport Svg Test REQUIRED)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
#include(opulus.cmake)

#TODO

#SET(CMAKE_POSITION_INDEPENDENT_CODE ON)

#include_directories(${Qt5Widgets_INCLUDE_DIRS})
#include_directories(${Qt5Core_INCLUDE_DIRS})
#include_directories(${Qt5Gui_INCLUDE_DIRS})
#include_directories(${Qt5Xml_INCLUDE_DIRS})
#include_directories(${Qt5Svg_INCLUDE_DIRS})
#include_directories(${Qt5PrintSupport_INCLUDE_DIRS})
#include_directories(${Qt5Test_INCLUDE_DIRS})


# translations
set(I18N pt_BR es)



if(NOT CMAKE_BUILD_TYPE)
	message(STATUS "Build type not especified, building as release.")
	message(STATUS "Use -DCMAKE_BUILD_TYPE=debug|release to set the build type.")
	set(CMAKE_BUILD_TYPE release)
endif(NOT CMAKE_BUILD_TYPE)

# adtional compiler options for release and debug modes
add_definitions(-Wall)
if (${CMAKE_BUILD_TYPE} STREQUAL release)
        add_definitions(-DQT_NO_DEBUG -O2)
else (${CMAKE_BUILD_TYPE} STREQUAL release)
        add_definitions(-ggdb)
endif (${CMAKE_BUILD_TYPE} STREQUAL release)

   set(CMAKE_EXE_LINKER_FLAGS "-static -static-libgcc -static-libstdc++")
#add_definitions(${GCC_COVERAGE_COMPILE_FLAGS})
#SET( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}" )

#SET(GCC_COVERAGE_COMPILE_FLAGS "")
#SET(GCC_COVERAGE_LINK_FLAGS    "-static")

add_subdirectory(core)
add_subdirectory(gui)
add_subdirectory(main)
add_subdirectory(plugins)
add_subdirectory(tests)
add_subdirectory(windowsinstaller)

# i18n stuff
#qt5_add_translations(opulus core gui)
install(FILES ${opulus_QMS} DESTINATION share/opulus/i18n)



# .desktop file
install(FILES opulus.desktop DESTINATION share/applications)
# icons
install(FILES opulus16.png DESTINATION share/icons/hicolor/16x16/apps RENAME opulus.png)
install(FILES opulus22.png DESTINATION share/icons/hicolor/22x22/apps RENAME opulus.png)
install(FILES opulus32.png DESTINATION share/icons/hicolor/32x32/apps RENAME opulus.png)
install(FILES opulus48.png DESTINATION share/icons/hicolor/48x48/apps RENAME opulus.png)
