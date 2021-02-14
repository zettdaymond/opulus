add_subdirectory(coveragegraph)
add_subdirectory(reachability)

add_library(opulus_plugins INTERFACE)
add_library(opulus::plugins ALIAS opulus_plugins)


if(BUILD_STATIC_PLUGINS)
    target_link_libraries(opulus_plugins INTERFACE coverage reachability)
endif()


