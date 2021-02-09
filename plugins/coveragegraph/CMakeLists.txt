set(GRAPHVIZ_BACKEND "NONE" CACHE STRING "Use graphviz or not" )

if(GRAPHVIZ_BACKEND STREQUAL "SHARED")
    find_package(PkgConfig)
    pkg_check_modules(cgraph REQUIRED IMPORTED_TARGET libcgraph)
    pkg_check_modules(gvc REQUIRED IMPORTED_TARGET libgvc)
else()
    if(GRAPHVIZ_BACKEND STREQUAL "STATIC")
        find_package(GraphVizStatic REQUIRED)
    endif()
endif()


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

if(${cgraph_FOUND} AND ${gvc_FOUND})
    target_compile_definitions(coverage
        PRIVATE
            -DGRAPHVIZ_BACKEND
    )

    target_link_libraries(coverage
        PRIVATE
            PkgConfig::cgraph
            PkgConfig::gvc
    )
endif()

if(GRAPHVIZ_STATIC_FOUND)
    target_compile_definitions(coverage
        PRIVATE
            -DGRAPHVIZ_STATIC_BACKEND
    )

    #This libraries MUST be sent to linker in proper order.
    target_link_libraries(coverage
        PRIVATE
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

target_include_directories(coverage PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

message("PLUGIN Coverage status:")
message("# GRAPHVIZ_BACKEND: ${GRAPHVIZ_BACKEND}")

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

if(GRAPHVIZ_BACKEND STREQUAL "SHARED")
    install_executable_system_dependencies($<TARGET_FILE:coverage> "${QT_QMAKE_EXECUTABLE}/..")
endif()
