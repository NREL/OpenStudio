# Find Dakota. Searches in C:/ (Windows), /Applications/ (Mac), /user/local/ (Linux) for folders
# named [dD][aA][kK][oO][tT][aA]?*. Also checks for environment variable DAKOTADIR.
#
# The module defines the following variables:
#  DAKOTA_FOUND - indicator variable
#  DAKOTA_EXE   - path to dakota.exe
#
# This module does not yet locate particular verions of DAKOTA.
#
# Example usage:
#  FIND_PACKAGE( Dakota )
#===============================================================================

SET(DAKOTA_FOUND FALSE)

IF( WIN32 )
  file( GLOB DAKOTA_POSSIBLE_PATHS "C:/[dD][aA][kK][oO][tT][aA]*" )
ELSEIF ( APPLE )
  file( GLOB DAKOTA_POSSIBLE_PATHS "/Applications/[dD][aA][kK][oO][tT][aA]*" )
ELSE()
  file( GLOB DAKOTA_POSSIBLE_PATHS "/usr/local/[dD][aA][kK][oO][tT][aA]*" )
  LIST(APPEND DAKOTA_POSSIBLE_PATHS "/usr/local")
ENDIF()

LIST(APPEND DAKOTA_POSSIBLE_PATHS $ENV{DAKOTADIR})
LIST(SORT DAKOTA_POSSIBLE_PATHS)
LIST(REVERSE DAKOTA_POSSIBLE_PATHS)

FOREACH(PATH ${DAKOTA_POSSIBLE_PATHS})

  FIND_PROGRAM(DAKOTA_EXE "dakota" PATHS "${PATH}" "${PATH}/bin" NO_DEFAULT_PATH)
  
  IF (EXISTS "${DAKOTA_EXE}")
    SET (DAKOTA_FOUND TRUE)
    BREAK()
  ENDIF(EXISTS "${DAKOTA_EXE}")

ENDFOREACH(PATH DAKOTA_POSSIBLE_PATHS)

IF (Dakota_FIND_REQUIRED AND (NOT DAKOTA_FOUND))
  MESSAGE(SEND_ERROR "Required package Dakota not found.")
ENDIF (Dakota_FIND_REQUIRED AND (NOT DAKOTA_FOUND))

