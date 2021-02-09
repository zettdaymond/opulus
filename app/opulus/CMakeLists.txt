if(WIN32)
	set(SUBSYSTEM WIN32)
endif()

add_executable(${PROJECT_NAME} ${SUBSYSTEM} main.cpp)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        opulus::gui
        ${PLUGINS_TO_LINK}
)

set_target_properties(${PROJECT_NAME}
    PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin
)


install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
)

if(DEPLOY_QT_AND_SYSTEM_LIBRARIES)
    get_target_property(_qmake_executable Qt5::qmake IMPORTED_LOCATION)
    get_filename_component(_qt_bin_dir "${_qmake_executable}" DIRECTORY)
    find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS "${_qt_bin_dir}")
    find_program(MACDEPLOYQT_EXECUTABLE macdeployqt HINTS "${_qt_bin_dir}")

    add_custom_command(TARGET ${PROJECT_NAME}
        POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E remove_directory "${CMAKE_BINARY_DIR}/windeployqt"
            COMMAND "${CMAKE_COMMAND}" -E
                env PATH="${_qt_bin_dir}" "${WINDEPLOYQT_EXECUTABLE}" --dir "${CMAKE_BINARY_DIR}/windeployqt" "$<TARGET_FILE:${PROJECT_NAME}>"
            COMMENT "Running windeployqt..."
    )

    install(
        DIRECTORY "${CMAKE_BINARY_DIR}/windeployqt/"
        DESTINATION bin
    )

    install(
        PROGRAMS ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS}
        DESTINATION bin
    )

    install_executable_system_dependencies($<TARGET_FILE:${PROJECT_NAME}> "${QT_QMAKE_EXECUTABLE}/..")
endif()