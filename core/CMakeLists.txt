cmake_minimum_required(VERSION 3.4)

set(opuluscore_srcs
	petrinet.cpp
	item.cpp
	node.cpp
	place.cpp
	transition.cpp
	abstractarc.cpp
	arc.cpp
	inhibitorarc.cpp
	simulation.cpp
	marking.cpp
	analysisrunner.cpp
	analysisreporter.cpp
	matrix_util.cpp
)
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_library(opuluscore ${opuluscore_srcs})
add_library(opulus::core ALIAS opuluscore)

find_package(Qt5 REQUIRED COMPONENTS Core Xml)
find_package(PkgConfig)
pkg_search_module(EIGEN3 REQUIRED eigen3)

target_include_directories(opuluscore INTERFACE ${EIGEN3_INCLUDE_DIRS})

target_include_directories(opuluscore PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

target_link_libraries(opuluscore PUBLIC Qt5::Core Qt5::Xml)
target_compile_features(opuluscore PUBLIC cxx_auto_type cxx_nullptr cxx_override cxx_noexcept)

if(CMAKE_COMPILER_IS_GNUCXX)
	target_compile_options(opuluscore PUBLIC -Wall -Wextra)
endif()

set_target_properties(opuluscore PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

install(TARGETS opuluscore 
	ARCHIVE DESTINATION lib 
	LIBRARY DESTINATION lib 
	RUNTIME DESTINATION bin)
