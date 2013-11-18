# Find Dakota. Searches in C:/ (Windows), /Applications/ (Mac), /usr/local/ 
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
#  FIND_PACKAGE( Dakota 5.3.1 )
#  FIND_PACKAGE( Dakota 5.3.1 REQUIRED )
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

  # check actual version against required version
  SET(IS_MATCH FALSE)

  # extract version from path
  STRING(REGEX REPLACE "^.*[dD][aA][kK][oO][tT][aA][-]?" "" VERSION ${PATH})
  
  IF(NOT ${VERSION} EQUAL "")
    # keep extracting
    STRING(REGEX REPLACE "^([0-9]+\\.[0-9]+\\.?[0-9]*).*" "\\1" VERSION ${VERSION})
    STRING(REGEX REPLACE "^([0-9]+\\.[0-9]+)\\..*" "\\1" VERSION_MAJOR_MINOR ${VERSION})
    
    # check actual versus required version
    IF(NOT ${VERSION} VERSION_LESS ${Dakota_FIND_VERSION})
      # ensure equal major and minor version numbers for I/O compatibility
      IF(${VERSION_MAJOR_MINOR} VERSION_EQUAL "${Dakota_FIND_VERSION_MAJOR}.${Dakota_FIND_VERSION_MINOR}")
        SET(IS_MATCH TRUE)
      ENDIF()
    ENDIF(NOT ${VERSION} VERSION_LESS ${Dakota_FIND_VERSION})    
  ELSE(NOT ${VERSION} EQUAL "")
    # no actual version found, assume okay but put out warning
    SET(IS_MATCH TRUE)
    MESSAGE(WARNING "No version information in Dakota filepath " ${PATH} ". Assuming that Dakota is version " ${Dakota_FIND_VERSION} ". To supress this warning in the future, change the Dakota folder name to something like dakota-" ${Dakota_FIND_VERSION} ", remove the cache entry for DAKOTA_EXE, and reconfigure.")
  ENDIF(NOT ${VERSION} EQUAL "")
  MESSAGE(STATUS "Found DAKOTA version string " ${VERSION} ".")

  IF(IS_MATCH)
    FIND_PROGRAM(DAKOTA_EXE "dakota" PATHS "${PATH}" "${PATH}/bin" NO_DEFAULT_PATH)
  ENDIF(IS_MATCH)
  
  IF (EXISTS "${DAKOTA_EXE}")
    SET (DAKOTA_FOUND TRUE)
    BREAK()
  ELSE (EXISTS "${DAKOTA_EXE}")
    IF(IS_MATCH)
      MESSAGE(WARNING "Path " ${DAKOTA_EXE} " does not exist. Looked for the Dakota executable in " ${PATH} " and " ${PATH} "/bin. If these paths do not match, remove the DAKOTA_EXE cache entry and try again." )
    ENDIF(IS_MATCH)
  ENDIF(EXISTS "${DAKOTA_EXE}")

ENDFOREACH(PATH DAKOTA_POSSIBLE_PATHS)

IF (Dakota_FIND_REQUIRED AND (NOT DAKOTA_FOUND))
  MESSAGE(SEND_ERROR "ERROR: Required package Dakota " ${Dakota_FIND_VERSION} " not found.")
ENDIF (Dakota_FIND_REQUIRED AND (NOT DAKOTA_FOUND))

