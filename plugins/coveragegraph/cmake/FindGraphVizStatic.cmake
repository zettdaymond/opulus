# Look for the header file.
FIND_PATH(GRAPHVIZ_STATIC_INCLUDE_DIR NAMES graphviz/cgraph.h PATH_SUFFIXES include/graphviz/)

# Look for the library.
FIND_LIBRARY(GRAPHVIZ_cdt_LIBRARY NAMES libccdt.a )
FIND_LIBRARY(GRAPHVIZ_cgraph_LIBRARY NAMES libcgraph.a )
FIND_LIBRARY(GRAPHVIZ_circogen_LIBRARY NAMES libcircogen.a )
FIND_LIBRARY(GRAPHVIZ_common_LIBRARY NAMES libcommon.a )

FIND_LIBRARY(GRAPHVIZ_dotgen_LIBRARY NAMES libdotgen.a )
FIND_LIBRARY(GRAPHVIZ_dotgen2_LIBRARY NAMES libdotgen2.a )
FIND_LIBRARY(GRAPHVIZ_fdpgen_LIBRARY NAMES libfdpgen.a )
FIND_LIBRARY(GRAPHVIZ_gvc_LIBRARY NAMES libgvc.a )

FIND_LIBRARY(GRAPHVIZ_gvplugin_core_LIBRARY NAMES libgvplugin_core.a)
FIND_LIBRARY(GRAPHVIZ_gvplugin_dot_layout_LIBRARY NAMES libgvplugin_dot_layout.a )
FIND_LIBRARY(GRAPHVIZ_gvplugin_neato_layout_LIBRARY NAMES libgvplugin_neato_layout.a )
FIND_LIBRARY(GRAPHVIZ_ingraphs_LIBRARY NAMES libingraphs.a )

FIND_LIBRARY(GRAPHVIZ_label_LIBRARY NAMES liblabel.a )
FIND_LIBRARY(GRAPHVIZ_neatogen_LIBRARY NAMES libneatogen.a )
FIND_LIBRARY(GRAPHVIZ_bortho_LIBRARY NAMES libortho.a )
FIND_LIBRARY(GRAPHVIZ_osage_LIBRARY NAMES libosage.a )

FIND_LIBRARY(GRAPHVIZ_pack_LIBRARY NAMES libpack.a )
FIND_LIBRARY(GRAPHVIZ_patchwork_LIBRARY NAMES libpatchwork.a )
FIND_LIBRARY(GRAPHVIZ_pathplan_LIBRARY NAMES libpathplan.a)
FIND_LIBRARY(GRAPHVIZ_sfdpgen_LIBRARY NAMES libsfdpgen.a )

FIND_LIBRARY(GRAPHVIZ_sparse_LIBRARY NAMES libsparse.a)
FIND_LIBRARY(GRAPHVIZ_twopigen_LIBRARY NAMES libtwopigen.a )

FIND_LIBRARY(GRAPHVIZ_xdot_LIBRARY NAMES libxdot.a)

SET(GRAPHVIZ_STATIC_LIBRARY 
	${GRAPHVIZ_cdt_LIBRARY}
	${GRAPHVIZ_cgraph_LIBRARY}
	${GRAPHVIZ_circogen_LIBRARY}
	${GRAPHVIZ_common_LIBRARY}

	${GRAPHVIZ_dotgen_LIBRARY}
	${GRAPHVIZ_dotgen2_LIBRARY}
	${GRAPHVIZ_fdpgen_LIBRARY}
	${GRAPHVIZ_gvc_LIBRARY}	

	${GRAPHVIZ_gvplugin_core_LIBRARY} 
	${GRAPHVIZ_gvplugin_dot_layout_LIBRARY} 
	${GRAPHVIZ_gvplugin_neato_layout_LIBRARY} 
	${GRAPHVIZ_ingraphs_LIBRARY}

	${GRAPHVIZ_label_LIBRARY} 
	${GRAPHVIZ_neatogen_LIBRARY}
	${GRAPHVIZ_bortho_LIBRARY}
	${GRAPHVIZ_osage_LIBRARY}

	${GRAPHVIZ_pack_LIBRARY} 
	${GRAPHVIZ_patchwork_LIBRARY}
	${GRAPHVIZ_pathplan_LIBRARY} 
	${GRAPHVIZ_sfdpgen_LIBRARY}

	${GRAPHVIZ_sparse_LIBRARY} 
	${GRAPHVIZ_twopigen_LIBRARY} 
        ${GRAPHVIZ_xdot_LIBRARY}

)

# Handle the QUIETLY and REQUIRED arguments and set GRAPHVIZ_FOUND to TRUE if all listed variables are TRUE.
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(GRAPHVIZ_STATIC DEFAULT_MSG GRAPHVIZ_STATIC_LIBRARY GRAPHVIZ_STATIC_INCLUDE_DIR)

# Copy the results to the output variables.
IF(GRAPHVIZ_STATIC_FOUND)
    SET(GRAPHVIZ_STATIC_LIBRARIES ${GRAPHVIZ_STATIC_LIBRARY})
    SET(GRAPHVIZ_STATIC_INCLUDE_DIRS ${GRAPHVIZ_STATIC_INCLUDE_DIR})
ELSE(GRAPHVIZ_STATIC_FOUND)
    SET(GRAPHVIZ_STATIC_LIBRARIES)
    SET(GRAPHVIZ_STATIC_INCLUDE_DIRS)
ENDIF(GRAPHVIZ_STATIC_FOUND)

MARK_AS_ADVANCED(GRAPHVIZ_STATIC_INCLUDE_DIRS GRAPHVIZ_STATIC_LIBRARIES)