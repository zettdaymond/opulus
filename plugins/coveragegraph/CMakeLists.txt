cmake_minimum_required(VERSION 3.4)

set(coverage_srcs
	coveragegraph.cpp
	extended/analysis_utils.cpp
	extended/preservingmatrixanalyser.cpp
	extended/propertyanalyser.cpp
	extended/transitionanaliser.cpp
	extended/markingnode.cpp
	extended/invertibilityanalyser.cpp
)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5 REQUIRED COMPONENTS Gui Svg Xml PrintSupport)

add_library(coverage SHARED ${coverage_srcs})

target_link_libraries(
	coverage
	PUBLIC
	opulus::core
	Qt5::Widgets
	Qt5::Gui
	Qt5::Svg
)

set_target_properties(coverage PROPERTIES
	LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/share/opulus/plugins
	RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/share/opulus/plugins
)

install(TARGETS coverage DESTINATION share/opulus/plugins)

