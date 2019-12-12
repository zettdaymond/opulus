if(WIN32)
	set(SUBSYSTEM WIN32)
endif()

if(BUILD_STATIC_PLUGINS)
    set(PLUGINS_TO_LINK coverage reachability)
else()
    set(PLUGINS_TO_LINK)
endif()

add_executable(${PROJECT_NAME} ${SUBSYSTEM} main.cpp ${UI_RESOURCES_RCC})

target_link_libraries(${PROJECT_NAME}
    opulus::gui
    ${PLUGINS_TO_LINK}
)

set_target_properties(${PROJECT_NAME}
    PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin
)


install(TARGETS ${PROJECT_NAME} DESTINATION bin)
