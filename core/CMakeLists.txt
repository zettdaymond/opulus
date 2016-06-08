project (opuluscore)
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


find_package(Qt5 REQUIRED COMPONENTS Core Xml)
find_package(PkgConfig)
find_package(Git)
pkg_search_module(EIGEN3 REQUIRED eigen3)


if(BUILD_STATIC_CORE_LIBS)
    add_library(opuluscore STATIC ${opuluscore_srcs})
else()
    add_library(opuluscore SHARED ${opuluscore_srcs})
endif()
add_library(opulus::core ALIAS opuluscore)


target_include_directories(opuluscore INTERFACE ${EIGEN3_INCLUDE_DIRS})

target_include_directories(opuluscore PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

target_link_libraries(opuluscore Qt5::Core Qt5::Xml)

if(CMAKE_COMPILER_IS_GNUCXX)
	target_compile_options(opuluscore PUBLIC -Wall -Wextra)
	if(GIT_FOUND AND USE_REVISION)
		execute_process(COMMAND ${GIT_EXECUTABLE} rev-list --count HEAD  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR} OUTPUT_VARIABLE git_revcount OUTPUT_STRIP_TRAILING_WHITESPACE)
		execute_process(COMMAND ${GIT_EXECUTABLE} rev-parse --short HEAD WORKING_DIRECTORY ${PROJECT_SOURCE_DIR} OUTPUT_VARIABLE git_revhash  OUTPUT_STRIP_TRAILING_WHITESPACE)
		target_compile_definitions(opuluscore PUBLIC "-DOPULUS_REVISION=\"r${git_revcount}.${git_revhash}\"")
	endif()
endif()

set_target_properties(opuluscore PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

install(TARGETS opuluscore 
	ARCHIVE DESTINATION lib 
	LIBRARY DESTINATION lib 
	RUNTIME DESTINATION bin)
