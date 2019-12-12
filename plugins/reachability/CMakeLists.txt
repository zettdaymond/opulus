set(reachability_srcs
    reachability.cpp
    delegate.cpp
)

if(BUILD_STATIC_PLUGINS)
    add_library(reachability STATIC ${reachability_srcs})
else()
    add_library(reachability SHARED ${reachability_srcs})
endif()

target_link_libraries(reachability
    PUBLIC
        opulus::core
        Qt5::Widgets
        Qt5::Gui
        Qt5::Svg
)

set_target_properties(reachability PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/share/opulus/plugins
    RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/share/opulus/plugins
)

install(TARGETS reachability DESTINATION share/opulus/plugins)

