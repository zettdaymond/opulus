add_subdirectory(coveragegraph)
add_subdirectory(reachability)

if(BUILD_STATIC_PLUGINS)
    set(PLUGINS_TO_LINK coverage reachability)
else()
    set(PLUGINS_TO_LINK)
endif()
