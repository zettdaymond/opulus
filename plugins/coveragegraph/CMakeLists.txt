option(COVERAGE_GRAPH_DOT "Enables coverage graph graphviz' dot backend" ON)
option(COVERAGE_GRAPH_DOT_STATIC "Enables coverage graph graphviz' dot STATIC backend" OFF)

add_library(coverage ${PLUGIN_LIBS_TYPE}
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

target_link_libraries(coverage
    PRIVATE
        opulus::core
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::Gui
        Qt${QT_VERSION_MAJOR}::Svg
)

if(COVERAGE_GRAPH_DOT AND NOT COVERAGE_GRAPH_DOT_STATIC)
    find_package(PkgConfig)
    pkg_check_modules(cgraph REQUIRED IMPORTED_TARGET libcgraph)
    pkg_check_modules(gvc REQUIRED IMPORTED_TARGET libgvc)

    if(${cgraph_FOUND} AND ${gvc_FOUND})
        target_compile_definitions(coverage
            PRIVATE
                GRAPHVIZ_BACKEND
        )

        target_link_libraries(coverage
            PRIVATE
                PkgConfig::cgraph
                PkgConfig::gvc
        )
    else()
        message(FATAL_ERROR "[#] Could not find cgraph and gvc library. Generation terminated")
    endif()
endif()

if(COVERAGE_GRAPH_DOT_STATIC AND NOT COVERAGE_GRAPH_DOT)
        add_subdirectory(${PROJECT_SOURCE_DIR}/thirdparty/graphviz-cmake graphviz )
        target_compile_definitions(coverage
            PRIVATE
                GRAPHVIZ_STATIC_BACKEND
        )
        target_link_libraries(coverage
            PRIVATE
                graphviz::plugin_core
                graphviz::plugin_dot_layout
                graphviz::common
                graphviz::gvc
                graphviz::graph
                graphviz::cdt
                graphviz::pathplan
        )
endif()

if((COVERAGE_GRAPH_DOT_STATIC AND COVERAGE_GRAPH_DOT) OR (NOT COVERAGE_GRAPH_DOT_STATIC AND NOT COVERAGE_GRAPH_DOT))
    message(FATAL_ERROR "[#] Coverage Graph plugin Backend is not determined!")
endif()

target_include_directories(coverage
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

message("[#] -----------------------------")
message("[#] PLUGIN Coverage graph status:")
message("[#] -----------------------------")
message("[#] -- COVERAGE_GRAPH_DOT: ${COVERAGE_GRAPH_DOT}")
message("[#] -- COVERAGE_GRAPH_DOT_STATIC: ${COVERAGE_GRAPH_DOT_STATIC}")
message("[#] -----------------------------")

set_target_properties(coverage PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/share/opulus/plugins
        RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/share/opulus/plugins
)

install(TARGETS coverage
    ARCHIVE DESTINATION share/opulus/plugins
    LIBRARY DESTINATION share/opulus/plugins
    RUNTIME DESTINATION share/opulus/plugins
    COMPONENT plugins
)

if(COVERAGE_GRAPH_DOT)
    install_executable_system_dependencies($<TARGET_FILE:coverage> "${QT_QMAKE_EXECUTABLE}/..")
endif()
