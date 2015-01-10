cmake_minimum_required(VERSION 2.4.6)
set(QT_MIN_VERSION    "4.3.2")
find_package(Qt4 REQUIRED)
include(opulus.cmake)

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

add_subdirectory(core)
add_subdirectory(gui)
add_subdirectory(main)
add_subdirectory(plugins)
add_subdirectory(tests)
add_subdirectory(windowsinstaller)

# i18n stuff
qt4_add_translations(opulus core gui)
install(FILES ${opulus_QMS} DESTINATION share/opulus/i18n)



# .desktop file
install(FILES opulus.desktop DESTINATION share/applications)
# icons
install(FILES opulus16.png DESTINATION share/icons/hicolor/16x16/apps RENAME opulus.png)
install(FILES opulus22.png DESTINATION share/icons/hicolor/22x22/apps RENAME opulus.png)
install(FILES opulus32.png DESTINATION share/icons/hicolor/32x32/apps RENAME opulus.png)
install(FILES opulus48.png DESTINATION share/icons/hicolor/48x48/apps RENAME opulus.png)
