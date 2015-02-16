project(opuluscore)

set(opuluscore_srcs
    petrinet.cpp
    item.cpp
    node.cpp
    place.cpp
    transition.cpp
    abstractarc.cpp
    arc.cpp
    inhibitorarc.cpp
    simulation.cpp
    marking.cpp
    analyser.h
    analysisrunner.cpp
    analysisreporter.cpp
    matrix_util.cpp
    exceptions.h
    version.h
    itemid.h
)


#qt5_generate_moc(opuluscore ${opuluscore_srcs})
include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})

add_library(opuluscore SHARED ${opuluscore_srcs})

#qt5_use_modules(opuluscore Core Widgets Xml Gui PrintSupport Svg Test)


#target_link_libraries(opuluscore ${QT_QTCORE_LIBRARY} ${QT_QTXML_LIBRARY})
#${Qt5Core_QTMAIN_LIBRARIES}target_link_libraries(opuluscore ${QT_QTCORE_LIBRARY} ${QT_QTXML_LIBRARY})
target_link_libraries(
    opuluscore
    Qt5::Core
    Qt5::Xml
)
install(TARGETS opuluscore DESTINATION lib)
