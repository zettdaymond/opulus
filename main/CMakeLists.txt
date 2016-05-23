cmake_minimum_required(VERSION 3.4)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_executable(opulus main.cpp ${UI_RESOURCES_RCC})

target_link_libraries(opulus opulus::gui)

install(TARGETS opulus DESTINATION bin)
