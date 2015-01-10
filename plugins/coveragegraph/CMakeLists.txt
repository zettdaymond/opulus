project(coverageGraph)

set(coverageGraph_SRCS
    coveragegraph.cpp
)
set(coverageGraph_UIS
	coveragegraph.ui
)

qt4_add_translations(coverageGraph .)

include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR}/../../core ${CMAKE_CURRENT_BINARY_DIR})
qt4_automoc(coverageGraph ${coverageGraph_SRCS})
qt4_wrap_ui(coverageGraph_ui_srcs ${coverageGraph_UIS})


add_definitions(${QT_DEFINITIONS})
add_definitions(-DQT_PLUGIN)
add_definitions(-DQT_SHARED)

add_library(coverageGraph SHARED ${coverageGraph_SRCS} ${coverageGraph_ui_srcs} ${coverageGraph_moc_srcs})
target_link_libraries(coverageGraph opuluscore ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} ${QT_QTSVG_LIBRARY})

install(FILES ${coverageGraph_QMS} DESTINATION share/opulus/i18n)

install(TARGETS coverageGraph DESTINATION share/opulus/plugins)

