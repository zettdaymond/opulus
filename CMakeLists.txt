cmake_minimum_required(VERSION 2.8.12)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

set(QT_MIN_VERSION    "5.0")
find_package(Qt5 COMPONENTS Core Widgets Xml Gui PrintSupport Svg Test REQUIRED)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(translation.cmake)
#SET(CMAKE_POSITION_INDEPENDENT_CODE ON)

if(NOT CMAKE_BUILD_TYPE)
	message(STATUS "Build type not especified, building as release.")
        message(STATUS "Use     |release to set the build type.")
	set(CMAKE_BUILD_TYPE release)
endif(NOT CMAKE_BUILD_TYPE)

# adtional compiler options for release and debug modes
add_definitions(-Wall)
if (${CMAKE_BUILD_TYPE} STREQUAL release)
        add_definitions(-DQT_NO_DEBUG -O2)
else (${CMAKE_BUILD_TYPE} STREQUAL release)
        add_definitions(-ggdb)
endif (${CMAKE_BUILD_TYPE} STREQUAL release)

#compiller flags
set(GCC_COVERAGE_COMPILE_FLAGS "")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")

#linker flags
#set(GCC_COVERAGE_LINKER_FLAGS "-static -static-libgcc -static-libstdc++ -Xlinker --allow-multiple-definition ")
set(GCC_COVERAGE_LINKER_FLAGS "")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINKER_FLAGS}")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${GCC_COVERAGE_LINKER_FLAGS}")

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
