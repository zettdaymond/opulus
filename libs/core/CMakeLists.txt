find_package(Eigen3 3.3 REQUIRED NO_MODULE)

add_library(opuluscore
    ${CORE_LIBS_TYPE}
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

add_library(opulus::core ALIAS opuluscore)

target_include_directories(opuluscore PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

target_link_libraries(opuluscore
    PUBLIC
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Xml
        Eigen3::Eigen
)

if(CMAKE_COMPILER_IS_GNUCXX)
	target_compile_options(opuluscore PUBLIC -Wall -Wextra)
        if(USE_REVISION)
            find_package(Git)
            if(GIT_FOUND)
		execute_process(COMMAND ${GIT_EXECUTABLE} rev-list --count HEAD  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR} OUTPUT_VARIABLE git_revcount OUTPUT_STRIP_TRAILING_WHITESPACE)
		execute_process(COMMAND ${GIT_EXECUTABLE} rev-parse --short HEAD WORKING_DIRECTORY ${PROJECT_SOURCE_DIR} OUTPUT_VARIABLE git_revhash  OUTPUT_STRIP_TRAILING_WHITESPACE)
		target_compile_definitions(opuluscore PUBLIC "-DOPULUS_REVISION=\"r${git_revcount}.${git_revhash}\"")
            else()
                message(ERROR "Could not find git in PATH")
            endif()
	endif()
endif()

set_target_properties(opuluscore PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin
    RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin
)

install(TARGETS opuluscore
        LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
)
