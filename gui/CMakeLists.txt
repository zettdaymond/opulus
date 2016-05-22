project (opulusgui)

set(opulusgui_srcs
    mainwindow.cpp
    controller.cpp
    petrinetscene.cpp
    messagewidget.cpp
    graphicutil.cpp
    graphicitem.cpp
    graphicnode.cpp
    graphicplace.cpp
    graphictransition.cpp
    graphicabstractarc.cpp
    graphicinhibitorarc.cpp
    graphicarc.cpp
    miniview.cpp
    propertyeditormodel.cpp
    propertyeditordelegate.cpp
    property.cpp
    intproperty.cpp
    propertyhandler.cpp
    placepropertyhandler.cpp
    transitionpropertyhandler.cpp
    arcpropertyhandler.cpp
    analyserstatusdlg.cpp

    commands/cmdcreateplace.cpp
    commands/cmdcreatetransition.cpp
    commands/cmdcreatearc.cpp
    commands/cmdcreatearcwithweight.cpp
    commands/cmdcreateinhibitorarc.cpp
    commands/cmdchangeproperty.hpp
    commands/cmdremoveitem.cpp
    commands/cmdmovenode.cpp
    commands/cmdmovenodegroup.cpp
    matrixwidget.cpp
    opulusmath.h
)

set(opulusgui_uis
    mainwindow.ui
    analyserstatus.ui
    matrixwidget.ui
)

set(opulusgui_rcs
    gui.qrc
)

qt5_add_resources(opulusgui_rc_srcs ${opulusgui_rcs})
#qt5_generate_moc(opulusgui ${opulusgui_srcs})
qt5_wrap_ui(opulusgui_ui_srcs ${opulusgui_uis})

include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/../core)
add_library(opulusgui SHARED ${opulusgui_srcs} ${opulusgui_ui_srcs} ${opulusgui_rc_srcs})
#target_link_libraries(opulusgui ${QT_QTGUI_LIBRARY} ${QT_QTSVG_LIBRARY} opuluscore)
target_link_libraries(opulusgui opuluscore)
target_link_libraries(
    opulusgui
    Qt5::Core
    Qt5::Gui
    Qt5::Svg
    Qt5::Xml
    Qt5::PrintSupport
)

install(TARGETS opulusgui DESTINATION lib)
