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

if(USE_REVISION)
    target_compile_definitions(opuluscore PUBLIC "-DOPULUS_REVISION=\"r${git_revcount}.${git_revhash}\"")
else()
    target_compile_definitions(opuluscore PUBLIC "-DOPULUS_VERSION=\"${OPULUS_VERSION}\"")
endif()

set_target_properties(opuluscore PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin
    RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin
)

if(NOT BUILD_STATIC_CORE_LIBS)
    install(TARGETS opuluscore
            ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
            LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
            RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
    )
endif()
