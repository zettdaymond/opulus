add_library(reachability ${PLUGIN_LIBS_TYPE}
    reachability.cpp
    delegate.cpp
)

target_link_libraries(reachability
    PRIVATE
        opulus::core
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::Gui
        Qt${QT_VERSION_MAJOR}::Svg
)

set_target_properties(reachability PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/share/opulus/plugins
    RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/share/opulus/plugins
)

install(TARGETS reachability
    ARCHIVE DESTINATION share/opulus/plugins
    LIBRARY DESTINATION share/opulus/plugins
    RUNTIME DESTINATION share/opulus/plugins
    COMPONENT plugins
)

