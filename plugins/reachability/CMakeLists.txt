project(reachability)

set(reachability_SRCS
    reachability.cpp
)
set(reachability_UIS
        reachability.ui
)

#qt5_add_translations(reachability .)

include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR}/../../core ${CMAKE_CURRENT_BINARY_DIR})

qt5_wrap_ui(reachability_ui_srcs ${reachability_UIS})


add_definitions(${QT_DEFINITIONS})

add_library(reachability SHARED ${reachability_SRCS} ${reachability_ui_srcs} ${reachability_moc_srcs})
target_link_libraries(reachability opuluscore)

target_link_libraries(
    reachability
    Qt5::Core
    Qt5::Widgets
    Qt5::Gui
    Qt5::Svg
)

set_target_properties(reachability PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAPROJECT_BINARY_DIR}../../share/opulus/plugins
    RUNTIME_OUTPUT_DIRECTORY ${CMAPROJECT_BINARY_DIR}../../share/opulus/plugins
)

install(FILES ${reachability_QMS} DESTINATION share/opulus/i18n)

#install(TARGETS reachability DESTINATION share/opulus/plugins)

