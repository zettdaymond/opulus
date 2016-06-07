#project(opulus)

#if(STATIC_PLUGINS)
#    set(MAIN_LINKED_LIBS )
#elseif(STATIC_PLUGINS)
#    set(MAIN_LINKED_LIBS opulusgui)
#endif(STATIC_PLUGINS)

if( MINGW )
	# resource compilation for MinGW
	add_custom_command(
		OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/opulusico.obj
		COMMAND windres.exe
				-I ${CMAKE_CURRENT_SOURCE_DIR}
				-i ${CMAKE_CURRENT_SOURCE_DIR}/opulus.rc
				-o ${CMAKE_CURRENT_BINARY_DIR}/opulusico.obj
		MAIN_DEPENDENCY ${CMAKE_CURRENT_SOURCE_DIR}/opulus.rc )
	set(opulus_SRCS main.cpp ${CMAKE_CURRENT_BINARY_DIR}/opulusico.obj)
else( MINGW )
	set(opulus_SRCS main.cpp)
endif( MINGW )


include_directories(
	${QT_INCLUDES}
	${CMAKE_CURRENT_BINARY_DIR}/../gui
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/../gui
	${CMAKE_CURRENT_SOURCE_DIR}/../core)

add_executable(opulus ${opulus_SRCS})

target_link_libraries(opulus opulusgui)

# Disable console window on windows.
if (CMAKE_BUILD_TYPE STREQUAL release)
	if (Q_WS_WIN)
		set_target_properties(opulus PROPERTIES LINK_FLAGS "-mwindows")
	endif (Q_WS_WIN)
endif (CMAKE_BUILD_TYPE STREQUAL release)

#copy
add_dependencies( opulus copyfiles )

install(TARGETS opulus DESTINATION bin)
