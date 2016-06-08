project(opulus)
cmake_minimum_required(VERSION 3.4)


if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(STATUS "Source Directory: " ${CMAKE_SOURCE_DIR})
    message(STATUS "Build Directory:  " ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there. You may need to remove CMakeCache.txt. ")
endif()

find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
endif(CCACHE_FOUND)

if (NOT CMAKE_BUILD_TYPE)
    message(STATUS "Build type not set, using \"Release\"")
    set(CMAKE_BUILD_TYPE "Release")
endif()

option(WITH_PLUGINS "Build and install plugins." ON)
option(WITH_TRANSLATIONS "Build and install translations." ON)
option(WITH_TESTS "Build tests." OFF)
option(BUILD_STATIC_CORE_LIBS "Build static core libraries." ON)
option(BUILD_STATIC_PLUGINS "Build static core libraries." ON)
option(BUILD_WITH_QT5_STATIC "Link to static Qt5 libraries." OFF)
option(USE_REVISION "Use revision number and commit hash instead of numeric version. Disable if building final release or without git." ON)


# adtional compiler options for release and debug modes
add_definitions(-std=c++14 -Wall -Wextra)
if (${CMAKE_BUILD_TYPE} STREQUAL Release)
    add_definitions(-DQT_NO_DEBUG -O3)
else ()
    add_definitions(-ggdb)
endif()

#----------Build configuration---------------------

#TODO: Detect in future
set(QT_STATIC 1)

if(BUILD_WITH_QT5_STATIC)
    if(WIN32)
        if(QT_STATIC)
            set(BUILD_STATIC_CORE_LIBS ON)
            set(BUILD_STATIC_PLUGINS ON)
            message("Warning: BUILD_STATIC_CORE_LIBS and BUILD_STATIC_PLUGINS does not allowed. Force set to OFF.")
            add_definitions(-DSTATIC_QT_BUILD)
            add_definitions(-static -static-libstdc++ -static-libgcc)
        else()
            message( FATAL_ERROR "Your vesion of Qt is not STATIC. Could not statically link to non static Qt. Please use -DBUILD_WITH_QT5_STATIC=OFF" )
        endif()
    else()
        message( FATAL_ERROR "Static link to Qt implemented only for Windows. Cmake will exit. Please use -DBUILD_WITH_QT5_STATIC=OFF to build shared version" )
    endif()
endif()

if(BUILD_STATIC_CORE_LIBS AND NOT BUILD_STATIC_PLUGINS )
    message("Warning: BUILD_STATIC_CORE_LIBS with static plugins doesn't make sense . Thus BUILD_STATIC_PLUGINS sets to OFF")
    set(BUILD_STATIC_PLUGINS OFF)
endif()

if(BUILD_STATIC_PLUGINS)
    add_definitions(-DSTATIC_PLUGINS)
endif()

add_subdirectory(core)
add_subdirectory(gui)

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

add_subdirectory(main)

#print options status
message("BUILD STATUS:")
message("# WITH_PLUGINS: " ${WITH_PLUGINS})
message("# WITH_TRANSLATIONS: " ${WITH_TRANSLATIONS})
message("# WITH_TESTS: " ${WITH_TESTS})
message("# BUILD_WITH_QT5_STATIC: " ${BUILD_WITH_QT5_STATIC})
message("# BUILD_STATIC_CORE_LIBS: " ${BUILD_STATIC_CORE_LIBS})
message("# BUILD_STATIC_PLUGINS: " ${BUILD_STATIC_PLUGINS})
message("# USE_REVISION: " ${USE_REVISION})

# .desktop file
install(FILES opulus.desktop DESTINATION share/applications)
# icons
install(FILES opulus16.png DESTINATION share/icons/hicolor/16x16/apps RENAME opulus.png)
install(FILES opulus22.png DESTINATION share/icons/hicolor/22x22/apps RENAME opulus.png)
install(FILES opulus32.png DESTINATION share/icons/hicolor/32x32/apps RENAME opulus.png)
install(FILES opulus48.png DESTINATION share/icons/hicolor/48x48/apps RENAME opulus.png)
