add_library(opulusgui
    ${CORE_LIBS_TYPE}
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

add_library(opulus::gui ALIAS opulusgui)

if(BUILD_STATIC_PLUGINS)
    target_compile_definitions(opulusgui
        PRIVATE
            STATIC_PLUGINS
    )
endif()

target_include_directories(opulusgui PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
)

target_link_libraries( opulusgui
    opulus::core

    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Svg
    Qt${QT_VERSION_MAJOR}::Xml
    Qt${QT_VERSION_MAJOR}::PrintSupport
)

set_target_properties(opulusgui PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin
        RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin
)

if(NOT BUILD_STATIC_CORE_LIBS)
    install(TARGETS opulusgui
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin
    )
endif()
