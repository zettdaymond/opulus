
find_package(Qt5LinguistTools REQUIRED)

#set(REFRESH_TRANSLATIONS 1)

IF(REFRESH_TRANSLATIONS)
    IF(NOT QT_LUPDATE_EXECUTABLE AND NOT Qt5_LUPDATE_EXECUTABLE)
        MESSAGE(WARNING "Opulus could not find lupdate. You won't be able to update translations.")
    ENDIF(NOT QT_LUPDATE_EXECUTABLE AND NOT Qt5_LUPDATE_EXECUTABLE)
ENDIF(REFRESH_TRANSLATIONS)

IF(NOT QT_LRELEASE_EXECUTABLE AND NOT Qt5_LRELEASE_EXECUTABLE)
    MESSAGE(WARNING "Opulus could not find lrelease. Your build won't contain translations.")
ENDIF(NOT QT_LRELEASE_EXECUTABLE AND NOT Qt5_LRELEASE_EXECUTABLE)

IF(QT_LRELEASE_EXECUTABLE)
  MESSAGE(STATUS "Qt4 LRealease find")
ENDIF(QT_LRELEASE_EXECUTABLE)

IF(Qt5_LRELEASE_EXECUTABLE)
  MESSAGE(STATUS "Qt5 LRealease find")
ENDIF(Qt5_LRELEASE_EXECUTABLE)

## Collect all translations files
FILE(GLOB Opulus_TS ${CMAKE_SOURCE_DIR}/i18n/*.ts)

IF(QT_LRELEASE_EXECUTABLE OR Qt5_LRELEASE_EXECUTABLE)
  IF(REFRESH_TRANSLATIONS)
      FILE(GLOB_RECURSE translate_main_SRCS ${CMAKE_SOURCE_DIR}/main/*.cpp)
      FILE(GLOB_RECURSE translate_core_SRCS ${CMAKE_SOURCE_DIR}/core/*.cpp)
      FILE(GLOB_RECURSE translate_gui_SRCS ${CMAKE_SOURCE_DIR}/gui/*.cpp)

      FILE(GLOB_RECURSE translate_plugin_cg_SRCS ${CMAKE_SOURCE_DIR}/plugins/coveragegraph/*.cpp)
      FILE(GLOB_RECURSE translate_plugin_reach_SRCS ${CMAKE_SOURCE_DIR}/plugins/reachability/*.cpp)

      FILE(GLOB_RECURSE translate_gui_UIS ${CMAKE_SOURCE_DIR}/gui/*.ui)
      FILE(GLOB_RECURSE translate_plugin_cg_UIS ${CMAKE_SOURCE_DIR}/plugins/coveragegraph/*.ui)
      FILE(GLOB_RECURSE translate_plugin_reach_UIS ${CMAKE_SOURCE_DIR}/plugins/reachability/*.ui)

      SET(translate_SRCS ${translate_main_SRCS} ${translate_core_SRCS} ${translate_gui_SRCS} ${translate_plugin_cg_SRCS} ${translate_plugin_reach_SRCS}
        ${translate_gui_UIS} ${translate_plugin_cg_UIS} ${translate_plugin_reach_UIS} )
  ENDIF(REFRESH_TRANSLATIONS)
      IF(Qt5_FOUND)
          IF(REFRESH_TRANSLATIONS)
              QT5_CREATE_TRANSLATION(Opulus_QMS ${translate_SRCS} ${Opulus_TS})
              MESSAGE(STATUS "Qt5 translation refreshed and compiled")
          ELSE(REFRESH_TRANSLATIONS)
              QT5_ADD_TRANSLATION(Opulus_QMS ${Opulus_TS})
              MESSAGE(STATUS "Qt5 translation compiled")
          ENDIF(REFRESH_TRANSLATIONS)
      ELSE(Qt5_FOUND )
          IF(REFRESH_TRANSLATIONS)
              QT4_CREATE_TRANSLATION(Opulus_QMS ${translate_SRCS} ${Opulus_TS})
              MESSAGE(STATUS "Qt4 translation refreshed and compiled")
          ELSE(REFRESH_TRANSLATIONS)
              QT4_ADD_TRANSLATION(Opulus_QMS ${Opulus_TS})
              MESSAGE(STATUS "Qt4 translation compiled")
          ENDIF(REFRESH_TRANSLATIONS)
      ENDIF(Qt5_FOUND)

      ADD_CUSTOM_TARGET(translations ALL DEPENDS ${Opulus_QMS})
      IF(REFRESH_TRANSLATIONS)
          ADD_CUSTOM_TARGET(translations_src ALL DEPENDS ${Opulus_QMS_SRC})
      ENDIF(REFRESH_TRANSLATIONS)
ENDIF(QT_LRELEASE_EXECUTABLE OR Qt5_LRELEASE_EXECUTABLE)


