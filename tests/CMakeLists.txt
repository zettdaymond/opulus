cmake_minimum_required(VERSION 3.4)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5 REQUIRED COMPONENTS Test)

add_executable(petrinet_test petrinettest.cpp)
add_executable(place_test placetest.cpp)
add_executable(undoredo_test undoredotest.cpp)

target_link_libraries(petrinet_test opulus::core Qt5::Test)
target_link_libraries(place_test opulus::core Qt5::Test)
target_link_libraries(undoredo_test opulus::gui Qt5::Test)

set(TESTS_WD ${PROJECT_BINARY_DIR}/bin)

add_test(NAME petrinet_test COMMAND petrinet WORKING_DIRECTORY ${TESTS_WD})
add_test(NAME place_test COMMAND place WORKING_DIRECTORY ${TESTS_WD})
add_test(NAME undoredo_test COMMAND undoredo WORKING_DIRECTORY ${TESTS_WD})
