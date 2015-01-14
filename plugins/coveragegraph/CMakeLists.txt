project(coverageGraph)

set(coverageGraph_SRCS
    coveragegraph.cpp
)
set(coverageGraph_UIS
	coveragegraph.ui
)

#qt5_add_translations(coverageGraph .)

include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR}/../../core ${CMAKE_CURRENT_BINARY_DIR})
qt5_wrap_ui(coverageGraph_ui_srcs ${coverageGraph_UIS})


#add_definitions(${QT_DEFINITIONS})
#add_definitions(-DQT_PLUGIN)
#add_definitions(-DQT_SHARED)

#add_library(coverageGraph SHARED ${coverageGraph_SRCS} ${coverageGraph_ui_srcs} ${coverageGraph_moc_srcs})
add_library(coverageGraph SHARED ${coverageGraph_SRCS} ${coverageGraph_ui_srcs} ${coverageGraph_moc_srcs})

target_link_libraries(coverageGraph opuluscore)
target_link_libraries(
    coverageGraph
    Qt5::Core
    Qt5::Widgets
    Qt5::Gui
    Qt5::Svg
)

set_target_properties(coverageGraph PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAPROJECT_BINARY_DIR}../../share/opulus/plugins
    RUNTIME_OUTPUT_DIRECTORY ${CMAPROJECT_BINARY_DIR}../../share/opulus/plugins
)


install(FILES ${coverageGraph_QMS} DESTINATION share/opulus/i18n)

install(TARGETS coverageGraph DESTINATION share/opulus/plugins)

