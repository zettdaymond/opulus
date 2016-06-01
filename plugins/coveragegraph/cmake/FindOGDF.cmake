# Look for the header file.
FIND_PATH(OGDF_INCLUDE_DIR Graph.h PATH_SUFFIXES include/ogdf/basic/ include/ogdf)

# Look for the library.
find_library(OGDF_coin_LIBRARY names libCOIN.a libcoin.a libCoin.a COIN.a coin.a Coin.a COIN coin Coin PATH_SUFFIXES lib)
find_library(OGDF_ogdf_LIBRARY names libOGDF.a libogdf.a libOgdf.a OGDF.a ogdf.a Ogdf.a OGDF ogdf Ogdf PATH_SUFFIXES lib)

set(OGDF_LIBRARY ${OGDF_coin_LIBRARY} ${OGDF_ogdf_LIBRARY})

# Handle the QUIETLY and REQUIRED arguments and set OGDF_FOUND to TRUE if all listed variables are TRUE.
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(OGDF DEFAULT_MSG OGDF_LIBRARY OGDF_INCLUDE_DIR)

# Copy the results to the output variables.
IF(OGDF_FOUND)
    SET(OGDF_LIBRARIES ${OGDF_LIBRARY})
    SET(OGDF_INCLUDE_DIRS ${OGDF_INCLUDE_DIR})
ELSE(OGDF_FOUND)
    SET(OGDF_LIBRARIES)
    SET(OGDF_INCLUDE_DIRS)
ENDIF(OGDF_FOUND)

MARK_AS_ADVANCED(OGDF_LIBRARIES OGDF_INCLUDE_DIRS)
