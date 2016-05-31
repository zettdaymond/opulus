cmake_minimum_required(VERSION 3.4)

find_package(Qt5LinguistTools REQUIRED)

# hopefully prevent `make clean` from removing source translation files
SET_DIRECTORY_PROPERTIES(PROPERTIES CLEAN_NO_CUSTOM 1)

file(GLOB translations *.ts)
file(GLOB translations_srcs
	${PROJECT_SOURCE_DIR}/core/*.cpp
	${PROJECT_SOURCE_DIR}/core/*.h
	${PROJECT_SOURCE_DIR}/gui/*.cpp
	${PROJECT_SOURCE_DIR}/gui/*.h
	${PROJECT_SOURCE_DIR}/gui/*.ui
	${PROJECT_SOURCE_DIR}/gui/commands/*.cpp
	${PROJECT_SOURCE_DIR}/gui/commands/*.h
	${PROJECT_SOURCE_DIR}/plugins/coveragegraph/*.cpp
	${PROJECT_SOURCE_DIR}/plugins/coveragegraph/*.h
	${PROJECT_SOURCE_DIR}/plugins/coveragegraph/extended/*.cpp
	${PROJECT_SOURCE_DIR}/plugins/coveragegraph/extended/*.h
	${PROJECT_SOURCE_DIR}/plugins/coveragegraph/*.ui
	${PROJECT_SOURCE_DIR}/plugins/reachability/*.cpp
	${PROJECT_SOURCE_DIR}/plugins/reachability/*.h
	${PROJECT_SOURCE_DIR}/plugins/reachability/*.ui)

if(UPDATE_TRANSLATIONS)
	message(AUTHOR_WARNING "-DUPDATE_TRANSLATIONS: This will modify in-source translation files. Make sure you have a backup or version control system and avoid using `make clean`.")
	qt5_create_translation(qm_files ${translations_srcs} ${translations})
else()
	qt5_add_translation(qm_files ${translations})
endif()

add_custom_target(opulus_compile_qm ALL DEPENDS ${qm_files})

# When starting application from build directory it should load translation files from ../share/opulus/i18n
add_custom_command(TARGET opulus_compile_qm PRE_BUILD COMMAND cmake -E make_directory ${PROJECT_BINARY_DIR}/share/opulus/i18n)
foreach(qm_file ${qm_files})
	add_custom_command(TARGET opulus_compile_qm POST_BUILD COMMAND cmake -E copy ${qm_file} ${PROJECT_BINARY_DIR}/share/opulus/i18n/)
endforeach()

# install translation files.
install(FILES ${qm_files} DESTINATION "share/opulus/i18n")