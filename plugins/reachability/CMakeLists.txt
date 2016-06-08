project (reachability)
cmake_minimum_required(VERSION 3.4)

set(reachability_srcs
    reachability.cpp
    delegate.cpp
)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5 REQUIRED COMPONENTS Gui Svg)

if(BUILD_STATIC_PLUGINS)
    add_library(reachability STATIC ${reachability_srcs})
else()
    add_library(reachability SHARED ${reachability_srcs})
endif()

target_link_libraries(reachability opuluscore)

target_link_libraries(
    reachability
    opulus::core
    Qt5::Widgets
    Qt5::Gui
    Qt5::Svg
)

set_target_properties(reachability PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/share/opulus/plugins
    RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/share/opulus/plugins
)

#install(FILES ${reachability_QMS} DESTINATION share/opulus/i18n)

install(TARGETS reachability DESTINATION share/opulus/plugins)

