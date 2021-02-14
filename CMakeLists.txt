cmake_minimum_required(VERSION 3.15)

project(opulus LANGUAGES CXX)

include(cmake/Utils.cmake)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Gui Widgets Svg Xml PrintSupport REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Gui Widgets Svg Xml PrintSupport REQUIRED)
find_package(Eigen3 3.3 REQUIRED NO_MODULE)

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(STATUS "Source Directory: " ${CMAKE_SOURCE_DIR})
    message(STATUS "Build Directory:  " ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there. You may need to remove CMakeCache.txt. ")
endif()

find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
    message(STATUS "[#] Ccache found")
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ${CCACHE_FOUND})
    set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ${CCACHE_FOUND})
endif()

if (NOT CMAKE_BUILD_TYPE)
    message(STATUS "[#] Build type not set, using \"Release\"")
    set(CMAKE_BUILD_TYPE "Release")
endif()

option(WITH_PLUGINS "Build and install plugins." ON)
option(WITH_TRANSLATIONS "Build and install translations." ON)
option(DEPLOY_QT_AND_SYSTEM_LIBRARIES "Ship Qt in bin folder during install" OFF)
option(WITH_TESTS "Build tests." OFF)
option(BUILD_STATIC_CORE_LIBS "Build static core libraries." OFF)
option(BUILD_STATIC_PLUGINS "Build static core libraries." OFF)
option(USE_REVISION "Use revision number and commit hash instead of numeric version. Disable if building final release or without git." ON)

if (MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -pedantic)
endif()

# Build configuration

if(BUILD_STATIC_CORE_LIBS)
    set(CORE_LIBS_TYPE STATIC)
else()
    set(CORE_LIBS_TYPE SHARED)
endif()

if(BUILD_STATIC_PLUGINS)
    set(PLUGIN_LIBS_TYPE STATIC)
else()
    set(PLUGIN_LIBS_TYPE SHARED)
endif()

add_subdirectory(libs/core)
add_subdirectory(libs/gui)

if(WITH_PLUGINS)
    message(STATUS "[#] Plugins enabled")
    add_subdirectory(plugins)
endif()

if(WITH_TESTS)
    message(STATUS "[#] Tests enabled")
    enable_testing()
    add_subdirectory(tests)
endif()

add_subdirectory(app/opulus)

if(WITH_TRANSLATIONS)
    message(STATUS "[#] Translations enabled")
    add_subdirectory(i18n)
endif()

# Print options status
message("[#] BUILD STATUS:")
message("[#] WITH_PLUGINS: " ${WITH_PLUGINS})
message("[#] WITH_TRANSLATIONS: " ${WITH_TRANSLATIONS})
message("[#] WITH_TESTS: " ${WITH_TESTS})
message("[#] BUILD_STATIC_CORE_LIBS: " ${BUILD_STATIC_CORE_LIBS})
message("[#] BUILD_STATIC_PLUGINS: " ${BUILD_STATIC_PLUGINS})
message("[#] USE_REVISION: " ${USE_REVISION})

if(UNIX AND NOT APPLE)
    # .desktop file
    install(FILES opulus.desktop DESTINATION share/applications)
    # icons
    install(FILES opulus16.png DESTINATION share/icons/hicolor/16x16/apps RENAME opulus.png)
    install(FILES opulus22.png DESTINATION share/icons/hicolor/22x22/apps RENAME opulus.png)
    install(FILES opulus32.png DESTINATION share/icons/hicolor/32x32/apps RENAME opulus.png)
    install(FILES opulus48.png DESTINATION share/icons/hicolor/48x48/apps RENAME opulus.png)
endif()

install(DIRECTORY example_nets DESTINATION share/opulus/)

if(APPLE AND NOT CPACK_GENERATOR)
#    set(CPACK_GENERATOR "Bundle")
elseif(WIN32 AND NOT CPACK_GENERATOR)
    set(CPACK_GENERATOR "ZIP")
elseif(UNIX AND NOT APPLE AND NOT CPACK_GENERATOR)
    set(CPACK_GENERATOR "TGZ")
endif()

set(CPACK_PACKAGE_NAME "${PROJECT_NAME}")
set(CPACK_PACKAGE_VENDOR "zett_n_cat")
set(CPACK_PACKAGE_INSTALL_REGISTRY_KEY "${PROJECT_NAME}")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Opulus is a simple Petri Net simulator/editor written in C++/Qt5")

include(CPack)
