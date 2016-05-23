cmake_minimum_required(VERSION 3.4)

project(opulus)

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
	message(FATAL_ERROR "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there. You may need to remove CMakeCache.txt. ")
endif()

if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release")
endif()

set(BUILD_SHARED_LIBS ON) # strange bugs with QRC when building static libs

add_subdirectory(core)
add_subdirectory(gui)
add_subdirectory(main)
add_subdirectory(plugins)

enable_testing()
add_subdirectory(tests)
#add_subdirectory(windowsinstaller)

# i18n stuff
# install(FILES ${Opulus_QMS} DESTINATION share/opulus/i18n)

# .desktop file
install(FILES opulus.desktop DESTINATION share/applications)
# icons
install(FILES opulus16.png DESTINATION share/icons/hicolor/16x16/apps RENAME opulus.png)
install(FILES opulus22.png DESTINATION share/icons/hicolor/22x22/apps RENAME opulus.png)
install(FILES opulus32.png DESTINATION share/icons/hicolor/32x32/apps RENAME opulus.png)
install(FILES opulus48.png DESTINATION share/icons/hicolor/48x48/apps RENAME opulus.png)
