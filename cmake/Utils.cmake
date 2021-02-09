include_guard(GLOBAL)

function(install_executable_system_dependencies EXEC_FILE DEP_FOLDERS)
  CMAKE_POLICY(VERSION 3.16)
  INSTALL(CODE "SET(EXEC_FILE \"${EXEC_FILE}\")")
  INSTALL(CODE "SET(DEP_FOLDERS \"${DEP_FOLDERS}\")")

  INSTALL(CODE [[

    FILE(GET_RUNTIME_DEPENDENCIES
      EXECUTABLES ${EXEC_FILE}
      RESOLVED_DEPENDENCIES_VAR _r_deps
      UNRESOLVED_DEPENDENCIES_VAR _u_deps
      CONFLICTING_DEPENDENCIES_PREFIX _c_deps
      DIRECTORIES ${DEP_FOLDERS}
      PRE_INCLUDE_REGEXES "api-ms-*"
      POST_EXCLUDE_REGEXES ".*system32/.*\\.dll" ".*SysWOW64/.*\\.dll"
      )

    IF (_u_deps)
      MESSAGE(WARNING "There were unresolved dependencies for executable ${EXEC_FILE}: \"${_u_deps}\"!")
    ENDIF()
    IF (_c_deps_FILENAMES)
      MESSAGE(WARNING "There were conflicting dependencies for executable ${EXEC_FILE}: \"${_c_deps_FILENAMES}\"!")
    ENDIF()

    FOREACH(_file ${_r_deps})
      FILE(INSTALL
        DESTINATION "${CMAKE_INSTALL_PREFIX}/bin"
        TYPE SHARED_LIBRARY
        FOLLOW_SYMLINK_CHAIN
        FILES "${_file}"
        )
    ENDFOREACH()
    ]])

endfunction()
