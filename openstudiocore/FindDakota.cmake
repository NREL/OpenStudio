# Find Dakota. Searches in C:/ (Windows), /Applications/ (Mac), /user/local/ 
# (Linux) for folders whose names start with [dD][aA][kK][oO][tT][aA]?*. Also 
# checks the environment variable DAKOTADIR. In all cases, looks for 
# bin/dakota.exe or bin/dakota (depending on platform).
#
# The module defines the following variables:
#  DAKOTA_FOUND - indicator variable
#  DAKOTA_EXE   - path to dakota.exe
#
# If a version number is specified, only 
#
# Example usage:
#  FIND_PACKAGE( Dakota 5.3.1)
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

# try to find the first path that matches all of the version requirements
FOREACH(PATH ${DAKOTA_POSSIBLE_PATHS})

  # extract version from path
  
  
  # check actual version against required version

  FIND_PROGRAM(DAKOTA_EXE "dakota" PATHS "${PATH}" "${PATH}/bin" NO_DEFAULT_PATH)
  
  IF (EXISTS "${DAKOTA_EXE}")
    SET (DAKOTA_FOUND TRUE)
    BREAK()
  ENDIF(EXISTS "${DAKOTA_EXE}")

ENDFOREACH(PATH DAKOTA_POSSIBLE_PATHS)

IF (Dakota_FIND_REQUIRED AND (NOT DAKOTA_FOUND))
  MESSAGE(SEND_ERROR "Required package Dakota not found.")
ENDIF (Dakota_FIND_REQUIRED AND (NOT DAKOTA_FOUND))

