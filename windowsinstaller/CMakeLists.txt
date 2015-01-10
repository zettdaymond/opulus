if (CMAKE_BUILD_TYPE STREQUAL release)
if (Q_WS_WIN)
		
project(metainstaller)
add_executable(metainstaller metainstaller.cpp)
include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR}/../core)
target_link_libraries(metainstaller ${QT_QTCORE_LIBRARY} opuluscore)

add_custom_target(windowsinstaller ./metainstaller ${metainstaller_SOURCE_DIR} ${CMAKE_INSTALL_PREFIX} ${QT_LIBRARY_DIR}/../ DEPENDS metainstaller WORKING_DIRECTORY ${CMAKE_CURRENT_BUILD_DIR} COMMENT "Creating innosetup script")


endif (Q_WS_WIN)
endif (CMAKE_BUILD_TYPE STREQUAL release)

