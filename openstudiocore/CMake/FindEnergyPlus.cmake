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
#  find_package(EnergyPlus 5.0.0) # find release or internal versions, prefers internal builds
#  find_package(EnergyPlus 5.0.0.15) # find only internal versions of specific build
#  find_package(EnergyPlus 5.0.0 EXACT) # find only release 
#  find_package(EnergyPlus 5.0.0.15 EXACT) # find only internal version of specific build

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

set(EnergyPlus_FOUND FALSE)

if(WIN32)
  file(GLOB ENERGYPLUS_POSSIBLE_PATHS "C:/[eE]nergy[pP]lus?*")
elseif(APPLE)
  file(GLOB ENERGYPLUS_POSSIBLE_PATHS "/Applications/[eE]nergy[pP]lus?*")
else()
  file(GLOB ENERGYPLUS_POSSIBLE_PATHS "/usr/local/[eE]nergy[pP]lus?*")
endif()

# sort possible paths in reverse order
list(APPEND ENERGYPLUS_POSSIBLE_PATHS $ENV{ENERGYPLUSDIR})
list(SORT ENERGYPLUS_POSSIBLE_PATHS)
list(REVERSE ENERGYPLUS_POSSIBLE_PATHS)

# try to find the first path that matches all of the version requirements where EnergyPlus is found
foreach(PATH ${ENERGYPLUS_POSSIBLE_PATHS})

  # extract version from path
  string(REGEX REPLACE "^.*[eE]nergy[pP]lus[vV]?[-]?" "" VERSION ${PATH})
  string(REGEX REPLACE "-" "." VERSION ${VERSION})
  string(REGEX REPLACE "^([0-9]+\\.[0-9]+)\\..*" "\\1" VERSION_MAJOR_MINOR ${VERSION})
 
  # set is match false
  set(IS_MATCH FALSE)
  
  # if we need exact match
  if(EnergyPlus_FIND_VERSION_EXACT)
  
    # need exact equal
    if(${VERSION} VERSION_EQUAL ${EnergyPlus_FIND_VERSION})
      set(IS_MATCH TRUE)
    endif()
  
  else()

    # need greater than or equal
    if(NOT ${VERSION} VERSION_LESS ${EnergyPlus_FIND_VERSION})
      # but need equal major and minor versions too for I/O compatibility
      if(${VERSION_MAJOR_MINOR} VERSION_EQUAL "${EnergyPlus_FIND_VERSION_MAJOR}.${EnergyPlus_FIND_VERSION_MINOR}")
        set(IS_MATCH TRUE)
      endif()
    endif()
 
  endif()
  
  # if match, try to find program and other variables
  if(IS_MATCH)
    if(${VERSION_MAJOR_MINOR} VERSION_LESS "8.0.0")
      find_program(ENERGYPLUS_EXE "energyplus" PATHS "${PATH}" "${PATH}/bin" NO_DEFAULT_PATH)
    else()
      find_program(ENERGYPLUS_EXE "EnergyPlus" PATHS "${PATH}" "${PATH}/bin" NO_DEFAULT_PATH)
    endif()

    find_file(ENERGYPLUS_IDD "Energy+.idd" PATHS "${PATH}" "${PATH}/bin" NO_DEFAULT_PATH)
    find_path(ENERGYPLUS_WEATHER_DIR "USA_IL_Chicago-OHare.Intl.AP.725300_TMY3.epw" PATHS "${PATH}/WeatherData" NO_DEFAULT_PATH)
  endif()

  # break if found everything
  if((EXISTS "${ENERGYPLUS_EXE}") AND (EXISTS "${ENERGYPLUS_IDD}") AND (EXISTS "${ENERGYPLUS_WEATHER_DIR}"))
    set(EnergyPlus_FOUND TRUE)
    break()
  endif()
  
endforeach()

if(EnergyPlus_FIND_REQUIRED)
  if(NOT EnergyPlus_FOUND)
    message("ERROR: Required package EnergyPlus " ${EnergyPlus_FIND_VERSION} " not found")
  endif()
endif()
