project(coverageGraph)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/plugins/coveragegraph/cmake" ${CMAKE_MODULE_PATH})

if(GRAPHVIZ_BACKEND)
    find_package(GraphViz REQUIRED)
else(GRAPHVIZ_BACKEND)
    if(GRAPHVIZ_STATIC_BACKEND)
        find_package(GraphVizStatic REQUIRED)
    endif(GRAPHVIZ_STATIC_BACKEND)
endif(GRAPHVIZ_BACKEND)

if(GRAPHVIZ_FOUND)
    add_definitions(-DGRAPHVIZ_BACKEND)
    message("Coverage graph backend : Graphviz SHARED")
endif(GRAPHVIZ_FOUND)

if(GRAPHVIZ_STATIC_FOUND)
    add_definitions(-DGRAPHVIZ_STATIC_BACKEND)
    message("Coverage graph backend : Graphviz STATIC")
endif(GRAPHVIZ_STATIC_FOUND)

if(NOT GRAPHVIZ_FOUND)
    if(NOT GRAPHVIZ_STATIC_FOUND)
        message("Coverage graph backend : NONE")
    endif(NOT GRAPHVIZ_STATIC_FOUND)
endif(NOT GRAPHVIZ_FOUND)

set(coverageGraph_SRCS
    coveragegraph.cpp
    utils.cpp
    graphicsviewzoom.cpp
    #extended
    extended/analysis_utils.cpp
    extended/preservingmatrixanalyser.cpp
    extended/propertyanalyser.cpp
    extended/transitionanaliser.cpp
    extended/markingnode.cpp
    extended/invertibilityanalyser.cpp
)
set(coverageGraph_UIS
	coveragegraph.ui
)

#qt5_add_translations(coverageGraph .)

include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR}/../../core ${CMAKE_CURRENT_BINARY_DIR})
qt5_wrap_ui(coverageGraph_ui_srcs ${coverageGraph_UIS})


#add_definitions(${QT_DEFINITIONS})
#add_definitions(-DQT_PLUGIN)
#add_definitions(-DQT_SHARED)

if(_STATIC_PLUGINS)
    add_library(coverageGraph STATIC ${coverageGraph_SRCS} ${coverageGraph_ui_srcs} ${coverageGraph_moc_srcs})
else(_STATIC_PLUGINS)
    add_library(coverageGraph SHARED ${coverageGraph_SRCS} ${coverageGraph_ui_srcs} ${coverageGraph_moc_srcs})
endif(_STATIC_PLUGINS)

target_link_libraries(coverageGraph opuluscore)
target_link_libraries(
    coverageGraph
    Qt5::Core
    Qt5::Widgets
    Qt5::Gui
    Qt5::Svg
)

if(GRAPHVIZ_FOUND)
    target_link_libraries(coverageGraph cgraph gvc)
endif(GRAPHVIZ_FOUND)

#DO NOT TOUCH this lines. I don't know how it works and why it works in that way, but it is.
if(GRAPHVIZ_STATIC_FOUND)
    target_link_libraries(coverageGraph
        libcgraph.a
        libgvc.a
        libgvplugin_dot_layout.a
    )

    target_link_libraries( coverageGraph
        libcdt.a
        libgvc.a
        libcommon.a
        libdotgen.a
        libgvplugin_core.a
        libgvplugin_dot_layout.a
        liblabel.a
        libortho.a
        libpack.a
        libpathplan.a
        libxdot.a
    )

endif(GRAPHVIZ_STATIC_FOUND)

set_target_properties(coverageGraph PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAPROJECT_BINARY_DIR}../../share/opulus/plugins
    RUNTIME_OUTPUT_DIRECTORY ${CMAPROJECT_BINARY_DIR}../../share/opulus/plugins
)

install(FILES ${coverageGraph_QMS} DESTINATION share/opulus/i18n)

install(TARGETS coverageGraph DESTINATION share/opulus/plugins)

