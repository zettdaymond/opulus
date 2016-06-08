cmake_minimum_required(VERSION 3.4)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

if(WIN32)
	set(SUBSYSTEM WIN32)
endif()

if(BUILD_STATIC_PLUGINS)
    set(PLUGINS_TO_LINK coverage reachability)
else()
    set(PLUGINS_TO_LINK)
endif()

add_executable(opulus ${SUBSYSTEM} main.cpp ${UI_RESOURCES_RCC})

target_link_libraries(opulus opulus::gui ${PLUGINS_TO_LINK})

#copy
#add_dependencies( opulus copyfiles )

set_target_properties(opulus PROPERTIES	RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)


install(TARGETS opulus DESTINATION bin)
