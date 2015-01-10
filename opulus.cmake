#####################################################################
#
#  Copyright (C) 2006-2007,  Matt Edman, Justin Hipple
#
#  This program is free software; you can redistribute it and/or
#  modify it under the terms of the GNU General Public License
#  as published by the Free Software Foundation; either version 2
#  of the License, or (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program; if not, write to the Free Software
#  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  
#  02110-1301, USA.
#####################################################################


## Search for lrelease
find_program(QT_LRELEASE_EXECUTABLE NAMES lrelease-qt4 lrelease
  PATHS ${QT_BINARY_DIR} NO_DEFAULT_PATH
)
if (NOT QT_LRELEASE_EXECUTABLE)
  message(FATAL_ERROR
    "Could not find lrelease. Please make sure Qt4 is installed."
  )
endif(NOT QT_LRELEASE_EXECUTABLE)


## Search for lupdate
find_program(QT_LUPDATE_EXECUTABLE NAMES lupdate-qt4 lupdate
  PATHS ${QT_BINARY_DIR} NO_DEFAULT_PATH
)
if (NOT QT_LUPDATE_EXECUTABLE)
  message(FATAL_ERROR
    "Could not find lupdate. Please make sure Qt4 is installed."
  )
endif(NOT QT_LUPDATE_EXECUTABLE)


## We need windres.exe when building on MinGW to compile the .rc file
if (MINGW)
  find_program(MINGW_WINDRES_EXECUTABLE  NAMES windres.exe ${QT_BINARY_DIR})
  if (NOT MINGW_WINDRES_EXECUTABLE)
    message(FATAL_ERR
      "Could not find windres. Please make sure MinGW is installed and its bin directory is in your PATH environment variable."
    )
  endif(NOT MINGW_WINDRES_EXECUTABLE)
endif(MINGW)


## Wraps the supplied .ts files in lrelease commands
macro(QT4_ADD_TRANSLATIONS name)
	set(var_TSS ${name}_TSS)
	set(var_QMS ${name}_QMS)
	set(var_lupdate_target_name ${name}_lupdate)

	# create tss variable
	foreach(it ${I18N})
		list(APPEND ${var_TSS} ${CMAKE_CURRENT_SOURCE_DIR}/i18n/${it}.ts)
	endforeach(it)
 

	foreach (it ${${var_TSS}})
		get_filename_component(it ${it} ABSOLUTE)
		get_filename_component(outfile ${it} NAME_WE)
		
		set(outfile ${CMAKE_CURRENT_BINARY_DIR}/${name}_${outfile}.qm)
		add_custom_command(OUTPUT ${outfile}
			COMMAND ${QT_LRELEASE_EXECUTABLE}
			ARGS -compress -silent -nounfinished ${it} -qm ${outfile}
			DEPENDS ${it}
		)
		LIST(APPEND ${var_QMS} ${outfile})
	endforeach(it)
	
	set(SRC_dirs )
	foreach(it ${ARGN})
		list(APPEND SRC_dirs ${CMAKE_CURRENT_SOURCE_DIR}/${it})
	endforeach(it)
	add_custom_target(${var_lupdate_target_name}
		COMMAND ${QT_LUPDATE_EXECUTABLE} ${SRC_dirs} -ts ${${var_TSS}}
	)

	add_custom_target(${name}_i18n ALL DEPENDS ${${var_QMS}})
endmacro(QT4_ADD_TRANSLATIONS)


if (MINGW)
  ## Wraps the supplied .rc files in windres commands
  macro(MINGW_WRAP_RC outfiles)
    foreach(it ${ARGN})
      get_filename_component(it      ${it} ABSOLUTE)
      get_filename_component(outfile ${it} NAME_WE)
      get_filename_component(rc_path ${it} PATH)
      
      set(outfile ${CMAKE_CURRENT_BINARY_DIR}/${outfile}_res.o)
      add_custom_command(OUTPUT ${outfile}
        COMMAND ${MINGW_WINDRES_EXECUTABLE}
        ARGS -i ${it} -o ${outfile} --include-dir=${rc_path}
        MAIN_DEPENDENCY ${it}
      )
      set(${outfiles} ${${outfiles}} ${outfile})
    endforeach(it)
  endmacro(MINGW_WRAP_RC)
endif(MINGW)

