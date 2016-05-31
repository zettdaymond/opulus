cmake_minimum_required(VERSION 3.4)

project(opulus)

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
	message(STATUS "Source Directory: " ${CMAKE_SOURCE_DIR})
	message(STATUS "Build Directory:  " ${CMAKE_BINARY_DIR})
	message(FATAL_ERROR "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there. You may need to remove CMakeCache.txt. ")
endif()

if (NOT CMAKE_BUILD_TYPE)
	message(STATUS "Build type not set, using \"Release\"")
	set(CMAKE_BUILD_TYPE "Release")
endif()

option(WITH_PLUGINS "Build and install plugins." ON)
option(WITH_TRANSLATIONS "Build and install translations." ON)
option(WITH_TESTS "Build tests." OFF)
option(BUILD_SHARED_LIBS "Build shared core libraries." ON)
option(USE_REVISION "Use revision number and commit hash instead of numeric version. Disable if building final release or without git." ON)


add_subdirectory(core)
add_subdirectory(gui)
add_subdirectory(main)

if(WITH_TRANSLATIONS)
	message(STATUS "Translations enabled")
	add_subdirectory(i18n)
	add_dependencies(opulus opulus_compile_qm)
endif()

if(WITH_PLUGINS)
	message(STATUS "Plugins enabled")
	add_subdirectory(plugins)
endif()

if(WITH_TESTS)
	message(STATUS "Tests enabled")
	enable_testing()
	add_subdirectory(tests)
endif()

# .desktop file
install(FILES opulus.desktop DESTINATION share/applications)
# icons
install(FILES opulus16.png DESTINATION share/icons/hicolor/16x16/apps RENAME opulus.png)
install(FILES opulus22.png DESTINATION share/icons/hicolor/22x22/apps RENAME opulus.png)
install(FILES opulus32.png DESTINATION share/icons/hicolor/32x32/apps RENAME opulus.png)
install(FILES opulus48.png DESTINATION share/icons/hicolor/48x48/apps RENAME opulus.png)
