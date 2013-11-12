# - Find EnergyPlus, the main assumption of this finder is that EnergyPlus
#   is installed in the default location, or is pointed to by environment 
#   variable ENERGYPLUSDIR.
#
# We anticipate energyPlus builds from the official release as well as internal
# pre-release and post-release bug fixes.  The official release is installed in 
# 'EnergyPlusV${Major}-${Minor}-${Patch}', the internal releases are installed in
# 'EnergyPlusV${Major}-${Minor}-${Patch}-${Build}'.  This finder assumes that 
# internal releases of the same major/minor/patch versions are prefered to official 
# release builds.
#
# The module defines the following variables:
#  ENERGYPLUS_FOUND - was a compatible version of EnergyPlus found
#  ENERGYPLUS_EXE - path to the EnergyPlus executable
#  ENERGYPLUS_IDD - path to the idd 
#  ENERGYPLUS_WEATHER_DIR - path to the weather dir 
#
# Example usage:
#  FIND_PACKAGE( EnergyPlus 5.0.0 ) # find release or internal versions, prefers internal builds
#  FIND_PACKAGE( EnergyPlus 5.0.0.15 ) # find only internal versions of specific build
#  FIND_PACKAGE( EnergyPlus 5.0.0 EXACT ) # find only release 
#  FIND_PACKAGE( EnergyPlus 5.0.0.15 EXACT ) # find only internal version of specific build

#=============================================================================
# Copyright 2006-2009 Kitware, Inc.
# Copyright 2006 Tristan Carel
#
# Distributed under the OSI-approved BSD License (the "License");
# see accompanying file Copyright.txt for details.
#
# This software is distributed WITHOUT ANY WARRANTY; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the License for more information.
#=============================================================================
# (To distributed this file outside of CMake, substitute the full
#  License text for the above reference.)

SET(EnergyPlus_FOUND FALSE)

IF( WIN32 )
  file( GLOB ENERGYPLUS_POSSIBLE_PATHS "C:/[eE]nergy[pP]lus?*")
ELSEIF( APPLE )
  file( GLOB ENERGYPLUS_POSSIBLE_PATHS "/Applications/[eE]nergy[pP]lus?*" )
ELSE()
  file( GLOB ENERGYPLUS_POSSIBLE_PATHS "/usr/local/[eE]nergy[pP]lus?*" )
ENDIF()

# sort possible paths in reverse order
LIST(APPEND ENERGYPLUS_POSSIBLE_PATHS $ENV{ENERGYPLUSDIR})
LIST(SORT ENERGYPLUS_POSSIBLE_PATHS)
LIST(REVERSE ENERGYPLUS_POSSIBLE_PATHS)

# try to find the first path that matches all of the version requirements where EnergyPlus is found
FOREACH(PATH ${ENERGYPLUS_POSSIBLE_PATHS})

  # extract version from path
  STRING(REGEX REPLACE "^.*[eE]nergy[pP]lus[vV]?[-]?" "" VERSION ${PATH})
  STRING(REGEX REPLACE "-" "." VERSION ${VERSION})
  STRING(REGEX REPLACE "^([0-9]+\\.[0-9]+)\\..*" "\\1" VERSION_MAJOR_MINOR ${VERSION})
 
  # set is match false
  SET(IS_MATCH FALSE)
  
  # if we need exact match
  IF(EnergyPlus_FIND_VERSION_EXACT)
  
    # need exact equal
    IF( ${VERSION} VERSION_EQUAL ${EnergyPlus_FIND_VERSION} )
      SET(IS_MATCH TRUE)
    ENDIF( ${VERSION} VERSION_EQUAL ${EnergyPlus_FIND_VERSION} )
  
  ELSE(EnergyPlus_FIND_VERSION_EXACT)

    # need greater than or equal
    IF( NOT ${VERSION} VERSION_LESS ${EnergyPlus_FIND_VERSION} )
      # but need equal major and minor versions too for I/O compatibility
      IF( ${VERSION_MAJOR_MINOR} VERSION_EQUAL "${EnergyPlus_FIND_VERSION_MAJOR}.${EnergyPlus_FIND_VERSION_MINOR}" )
        SET(IS_MATCH TRUE)
      ENDIF( ${VERSION_MAJOR_MINOR} VERSION_EQUAL "${EnergyPlus_FIND_VERSION_MAJOR}.${EnergyPlus_FIND_VERSION_MINOR}" )
    ENDIF( NOT ${VERSION} VERSION_LESS ${EnergyPlus_FIND_VERSION} )
 
  ENDIF(EnergyPlus_FIND_VERSION_EXACT)
  
  # if match, try to find program and other variables
  IF( IS_MATCH )
    IF( ${VERSION_MAJOR_MINOR} VERSION_LESS "8.0.0")
      FIND_PROGRAM(ENERGYPLUS_EXE "energyplus" PATHS "${PATH}" "${PATH}/bin" NO_DEFAULT_PATH)
    ELSE()
      FIND_PROGRAM(ENERGYPLUS_EXE "EnergyPlus" PATHS "${PATH}" "${PATH}/bin" NO_DEFAULT_PATH)
    ENDIF()

    FIND_FILE(ENERGYPLUS_IDD "Energy+.idd" PATHS "${PATH}" "${PATH}/bin" NO_DEFAULT_PATH)
    FIND_PATH(ENERGYPLUS_WEATHER_DIR "USA_IL_Chicago-OHare.Intl.AP.725300_TMY3.epw" PATHS "${PATH}/WeatherData" NO_DEFAULT_PATH)
  ENDIF( IS_MATCH )

  # break if found everything
  IF( (EXISTS "${ENERGYPLUS_EXE}") AND (EXISTS "${ENERGYPLUS_IDD}") AND (EXISTS "${ENERGYPLUS_WEATHER_DIR}") )
    SET( EnergyPlus_FOUND TRUE)
    BREAK()
  ENDIF()
  
ENDFOREACH(PATH ENERGYPLUS_POSSIBLE_PATHS)

IF(EnergyPlus_FIND_REQUIRED)
  IF(NOT EnergyPlus_FOUND)
    MESSAGE("ERROR: Required package EnergyPlus " ${EnergyPlus_FIND_VERSION} " not found")
  ENDIF(NOT EnergyPlus_FOUND)
ENDIF(EnergyPlus_FIND_REQUIRED)
