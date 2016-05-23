cmake_minimum_required(VERSION 3.4)

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
    commands/cmdremoveitemgroup.cpp
    commands/cmdmovenode.cpp
    commands/cmdmovenodegroup.cpp
    matrixwidget.cpp
    opulusmath.h
    gui.qrc
)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)


add_library(opulusgui ${opulusgui_srcs})
add_library(opulus::gui ALIAS opulusgui)

find_package(Qt5 REQUIRED COMPONENTS Gui Svg Xml PrintSupport)

target_include_directories(opulusgui PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
)

target_link_libraries(
    opulusgui
    PUBLIC
    opulus::core
    Qt5::Gui
    Qt5::Svg
    Qt5::Xml
    Qt5::PrintSupport
)

install(TARGETS opulusgui
    ARCHIVE DESTINATION lib 
    LIBRARY DESTINATION lib 
    RUNTIME DESTINATION bin)
