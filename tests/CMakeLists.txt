include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/../core ${CMAKE_CURRENT_SOURCE_DIR}/../gui)

qt4_automoc(petrinettest petrinettest.cpp)
add_executable(petrinettest petrinettest.cpp)
target_link_libraries(petrinettest ${QT_QTTEST_LIBRARY} opuluscore)

qt4_automoc(placetest placetest.cpp)
add_executable(placetest placetest.cpp)
target_link_libraries(placetest ${QT_QTTEST_LIBRARY} opuluscore)

qt4_automoc(undoredotest undoredotest.cpp)
add_executable(undoredotest undoredotest.cpp)
target_link_libraries(undoredotest ${QT_QTTEST_LIBRARY} opuluscore opulusgui)

