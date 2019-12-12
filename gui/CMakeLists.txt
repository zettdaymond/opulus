set(OpulusGui_SRCs

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
    netmatrixmodel.cpp
    pluginhandler.cpp
    petrinetviewzoom.cpp

    commands/cmdcreateplace.cpp
    commands/cmdcreatetransition.cpp
    commands/cmdcreatearc.cpp
    commands/cmdcreatearcwithweight.cpp
    commands/cmdcreateinhibitorarc.cpp
    commands/cmdchangeproperty.hpp
    commands/cmdremoveitem.cpp
    commands/cmdremoveitemgroup.cpp
    commands/cmdmovenode.cpp
    commands/cmdmovenodegroup.cpp
    commands/cmdpack.cpp
    matrixwidget.cpp
    opulusmath.h

    gui.qrc

    mainwindow.ui
)

if(BUILD_STATIC_CORE_LIBS)
    add_library(opulusgui STATIC ${OpulusGui_SRCs})
else()
    add_library(opulusgui SHARED ${OpulusGui_SRCs})
endif()

add_library(opulus::gui ALIAS opulusgui)

target_include_directories(opulusgui PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
)

target_link_libraries( opulusgui
    opulus::core

    Qt5::Gui
    Qt5::Svg
    Qt5::Xml
    Qt5::PrintSupport
)

set_target_properties(opulusgui PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin
        RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin
)

install(TARGETS opulusgui
    ARCHIVE DESTINATION lib 
    LIBRARY DESTINATION lib 
    RUNTIME DESTINATION bin)
