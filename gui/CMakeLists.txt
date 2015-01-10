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
	commands/cmdcreateinhibitorarc.cpp
	commands/cmdremoveitem.cpp
	commands/cmdmovenode.cpp
)

set(opulusgui_uis
	mainwindow.ui
	analyserstatus.ui
)

set(opulusgui_rcs
	gui.qrc
)

qt4_add_resources(opulusgui_rc_srcs ${opulusgui_rcs})
qt4_automoc(opulusgui ${opulusgui_srcs})
qt4_wrap_ui(opulusgui_ui_srcs ${opulusgui_uis})

include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/../core)
add_library(opulusgui SHARED ${opulusgui_srcs} ${opulusgui_ui_srcs} ${opulusgui_rc_srcs})
target_link_libraries(opulusgui ${QT_QTGUI_LIBRARY} ${QT_QTSVG_LIBRARY} opuluscore)

install(TARGETS opulusgui DESTINATION lib)
