find_package(Qt${QT_VERSION_MAJOR}LinguistTools REQUIRED)

# hopefully prevent `make clean` from removing source translation files
set_directory_properties(PROPERTIES CLEAN_NO_CUSTOM 1)

file(GLOB_RECURSE translations
    ${PROJECT_SOURCE_DIR}/i18n/*.ts
)

if(UPDATE_TRANSLATIONS)
	message(AUTHOR_WARNING "-DUPDATE_TRANSLATIONS: This will modify in-source translation files. Make sure you have a backup or version control system and avoid using `make clean`.")
        qt5_create_translation(qm_files ${PROJECT_SOURCE_DIR} ${translations})
else()
	qt5_add_translation(qm_files ${translations})
endif()

add_custom_target(opulus_compile_qm DEPENDS ${qm_files})
add_dependencies(opulus opulus_compile_qm)

file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/share/opulus/i18n)
file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/bin/share/opulus/i18n)

foreach(qm_file ${qm_files})
        message( STATUS "Copy ${qm_file}")
        add_custom_command(TARGET opulus_compile_qm POST_BUILD COMMAND cmake -E copy ${qm_file} ${PROJECT_BINARY_DIR}/share/opulus/i18n/)
        add_custom_command(TARGET opulus_compile_qm POST_BUILD COMMAND cmake -E copy ${qm_file} ${PROJECT_BINARY_DIR}/bin/share/opulus/i18n/)
endforeach()

# install translation files.
install(FILES ${qm_files} DESTINATION "share/opulus/i18n")
