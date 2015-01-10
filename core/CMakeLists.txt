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
	analysisrunner.cpp
	analysisreporter.cpp
)


qt4_automoc(opuluscore ${opuluscore_srcs})
include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})

add_library(opuluscore SHARED ${opuluscore_srcs})
target_link_libraries(opuluscore ${QT_QTCORE_LIBRARY} ${QT_QTXML_LIBRARY})

install(TARGETS opuluscore DESTINATION lib)
