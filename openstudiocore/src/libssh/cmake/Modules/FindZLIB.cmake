# - Try to find ZLIB
# Once done this will define
#
#  ZLIB_FOUND - system has ZLIB
#  ZLIB_INCLUDE_DIRS - the ZLIB include directory
#  ZLIB_LIBRARIES - Link these to use ZLIB
#  ZLIB_DEFINITIONS - Compiler switches required for using ZLIB
#
#  Copyright (c) 2009 Andreas Schneider <mail@cynapses.org>
#
#  Redistribution and use is allowed according to the terms of the New
#  BSD license.
#  For details see the accompanying COPYING-CMAKE-SCRIPTS file.
#


if (ZLIB_LIBRARIES AND ZLIB_INCLUDE_DIRS)
  # in cache already
  set(ZLIB_FOUND TRUE)
else (ZLIB_LIBRARIES AND ZLIB_INCLUDE_DIRS)

  find_path(ZLIB_INCLUDE_DIR
    NAMES
      zlib.h
    PATHS
      ${_ZLIB_DIR}/include
      /usr/include
      /usr/local/include
      /opt/local/include
      /sw/include
  )
  mark_as_advanced(ZLIB_INCLUDE_DIR)

  find_library(Z_LIBRARY
    NAMES
      z
      zlib
      zlib1
    PATHS
      ${_ZLIB_DIR}/lib
      /usr/lib
      /usr/local/lib
      /opt/local/lib
      /sw/lib
  )
  mark_as_advanced(Z_LIBRARY)

  if (Z_LIBRARY)
    set(Z_FOUND TRUE)
  endif (Z_LIBRARY)

  set(ZLIB_INCLUDE_DIRS
    ${ZLIB_INCLUDE_DIR}
  )

  if (Z_FOUND)
    set(ZLIB_LIBRARIES
      ${ZLIB_LIBRARIES}
      ${Z_LIBRARY}
    )
  endif (Z_FOUND)

  if (ZLIB_INCLUDE_DIRS AND ZLIB_LIBRARIES)
     set(ZLIB_FOUND TRUE)
  endif (ZLIB_INCLUDE_DIRS AND ZLIB_LIBRARIES)

  if (ZLIB_FOUND)
    if (NOT ZLIB_FIND_QUIETLY)
      message(STATUS "Found ZLIB: ${ZLIB_LIBRARIES}")
    endif (NOT ZLIB_FIND_QUIETLY)
  else (ZLIB_FOUND)
    if (ZLIB_FIND_REQUIRED)
      message(FATAL_ERROR "Could not find ZLIB")
    endif (ZLIB_FIND_REQUIRED)
  endif (ZLIB_FOUND)

  # show the ZLIB_INCLUDE_DIRS and ZLIB_LIBRARIES variables only in the advanced view
  mark_as_advanced(ZLIB_INCLUDE_DIRS ZLIB_LIBRARIES)

endif (ZLIB_LIBRARIES AND ZLIB_INCLUDE_DIRS)

