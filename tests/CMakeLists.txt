cmake_minimum_required(VERSION 3.4)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5 REQUIRED COMPONENTS Test)

add_executable(petrinet petrinettest.cpp)
add_executable(place placetest.cpp)
add_executable(undoredo undoredotest.cpp)

target_link_libraries(petrinet opulus::core Qt5::Test)
target_link_libraries(place opulus::core Qt5::Test)
target_link_libraries(undoredo opulus::gui Qt5::Test)

set(TESTS_WD ${PROJECT_BINARY_DIR}/bin)

add_test(NAME petrinet COMMAND petrinet WORKING_DIRECTORY ${TESTS_WD})
add_test(NAME place COMMAND place WORKING_DIRECTORY ${TESTS_WD})
add_test(NAME undoredo COMMAND undoredo WORKING_DIRECTORY ${TESTS_WD})