#<<<<<<< HEAD
project(coverage)
cmake_minimum_required(VERSION 3.4)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/plugins/coveragegraph/cmake" ${CMAKE_MODULE_PATH})

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5 REQUIRED COMPONENTS Gui Svg)

if(GRAPHVIZ_BACKEND STREQUAL "SHARED")
    find_package(GraphViz REQUIRED)
else()
    if(GRAPHVIZ_BACKEND STREQUAL "STATIC")
        find_package(GraphVizStatic REQUIRED)
    else()
        message("GRAPHVIZ_BACKEND is not set. Using \"NONE\"")
    endif()
endif()

if(GRAPHVIZ_FOUND)
    add_definitions(-DGRAPHVIZ_BACKEND)
    set(GRAPHVIZ_BACKEND_LIBS cgraph gvc)
endif()

if(GRAPHVIZ_STATIC_FOUND)
    add_definitions(-DGRAPHVIZ_STATIC_BACKEND)
    #This libraries MUST be sent to linker in proper order.
    set(GRAPHVIZ_BACKEND_LIBS
        libgvplugin_core.a
        libgvplugin_dot_layout.a
        libdotgen.a
        libcommon.a
        libxdot.a
        libgvc.a
        libcgraph.a
        libcdt.a
        libpathplan.a
        libortho.a
        libpack.a
        liblabel.a)
endif()

set(coverage_srcs
    coveragegraph.cpp
    utils.cpp
    graphicsviewzoom.cpp

	extended/analysis_utils.cpp
	extended/preservingmatrixanalyser.cpp
	extended/propertyanalyser.cpp
	extended/transitionanaliser.cpp
	extended/markingnode.cpp
	extended/invertibilityanalyser.cpp
)

if(BUILD_STATIC_PLUGINS)
    add_library(coverage STATIC ${coverage_srcs})
else()
    add_library(coverage SHARED ${coverage_srcs})
endif()

target_link_libraries(
    coverage
	opulus::core
	Qt5::Widgets
	Qt5::Gui
	Qt5::Svg
    ${GRAPHVIZ_BACKEND_LIBS}
)

target_include_directories(coverage PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

#set_target_properties(coverageGraph PROPERTIES
#    LIBRARY_OUTPUT_DIRECTORY ${CMAPROJECT_BINARY_DIR}../../share/opulus/plugins
#    RUNTIME_OUTPUT_DIRECTORY ${CMAPROJECT_BINARY_DIR}../../share/opulus/plugins
#)

#install(FILES ${coverageGraph_QMS} DESTINATION share/opulus/i18n)

#install(TARGETS coverageGraph DESTINATION share/opulus/plugins)
#=======
set_target_properties(coverage PROPERTIES
	LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/share/opulus/plugins
	RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/share/opulus/plugins
)

install(TARGETS coverage DESTINATION share/opulus/plugins)
