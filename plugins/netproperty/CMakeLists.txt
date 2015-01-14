
project(netProperty)

set(netProperty_SRCS
    netproperty.cpp
)
set(netProperty_UIS
        netproperty.ui
)

#qt5_add_translations(netProperty .)

include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR}/../../core ${CMAKE_CURRENT_BINARY_DIR})

qt5_wrap_ui(netProperty_ui_srcs ${netProperty_UIS} ${netProperty_ui_srcs} ${netProperty_moc_srcs})


add_definitions(${QT_DEFINITIONS})

add_library(netProperty SHARED ${netProperty_SRCS})
target_link_libraries(netProperty opuluscore)

target_link_libraries(
    netProperty
    Qt5::Core
    Qt5::Widgets
    Qt5::Gui
    Qt5::Svg
)

set_target_properties(netProperty PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAPROJECT_BINARY_DIR}../../share/opulus/plugins
    RUNTIME_OUTPUT_DIRECTORY ${CMAPROJECT_BINARY_DIR}../../share/opulus/plugins
)

install(FILES ${netProperty_QMS} DESTINATION share/opulus/i18n)

#install(TARGETS netProperty DESTINATION share/opulus/plugins)

