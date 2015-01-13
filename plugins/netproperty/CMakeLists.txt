
project(netProperty)

set(netProperty_SRCS
    netproperty.cpp
)
set(netProperty_UIS
        netproperty.ui
)

#qt5_add_translations(netProperty .)

include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR}/../../core ${CMAKE_CURRENT_BINARY_DIR})
qt5_wrap_ui(netProperty_ui_srcs ${netProperty_UIS})


add_definitions(${QT_DEFINITIONS})
add_definitions(-DQT_PLUGIN)
add_definitions(-DQT_SHARED)

add_library(netProperty SHARED ${netProperty_SRCS} ${netProperty_ui_srcs} ${netProperty_moc_srcs})
target_link_libraries(netProperty opuluscore)

target_link_libraries(
    netProperty
    Qt5::Core
    Qt5::Gui
    Qt5::Svg
    Qt5::Plugins
)

install(FILES ${netProperty_QMS} DESTINATION share/opulus/i18n)

#install(TARGETS netProperty DESTINATION share/opulus/plugins)

