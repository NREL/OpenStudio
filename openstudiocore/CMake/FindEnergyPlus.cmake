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

# Update for 8.2.0 and above:
# As of version 8.2.0, EnergyPlus has a build SHA instead of a possible fourth digit in the version string.
# Finding a specific build with the syntax find_package(EnergyPlus 5.0.0.15) is meaningless for these versions.
# Instead this module will consider the variable ENERGYPLUS_BUILD_SHA, and if set it will only succeed in 
# finding EnergyPlus if a version of EnergyPlus with that SHA has been found.  This behavior is independent of 
# the keyword "EXACT" in the find_package signature. In other words having ENERGYPLUS_BUILD_SHA defined implies EXACT.

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

  # if 8.2.0 or greater then look for a build number in the idd file
  if((${VERSION} VERSION_EQUAL 8.2.0) OR (${VERSION} VERSION_GREATER 8.2.0))
    # we just need to read the first part of this large file
    file(READ "${PATH}/Energy+.idd" IDD_TEXT LIMIT 1000)
    string(REGEX MATCH "!IDD_BUILD [0-9a-z][0-9a-z][0-9a-z][0-9a-z][0-9a-z][0-9a-z][0-9a-z][0-9a-z][0-9a-z][0-9a-z]" BUILD_SHA_LINE "${IDD_TEXT}")
    string(REGEX MATCH "[0-9a-z][0-9a-z][0-9a-z][0-9a-z][0-9a-z][0-9a-z][0-9a-z][0-9a-z][0-9a-z][0-9a-z]" BUILD_SHA "${BUILD_SHA_LINE}")
    set(ENERGYPLUS_GE_8_2_0 true)
  endif()
 
  # set is match false
  set(IS_MATCH FALSE)
  
  # if a build SHA is provided then assume we need an exact match
  if(ENERGYPLUS_GE_8_2_0 AND ENERGYPLUS_BUILD_SHA)
    if(${VERSION} VERSION_EQUAL ${EnergyPlus_FIND_VERSION})
      if(ENERGYPLUS_BUILD_SHA STREQUAL BUILD_SHA)
        set(IS_MATCH TRUE)
      endif()
    endif()
  # if the cmake finder was passed the EXACT flag then check the version for exact match
  elseif(EnergyPlus_FIND_VERSION_EXACT)
    if(${VERSION} VERSION_EQUAL ${EnergyPlus_FIND_VERSION})
      set(IS_MATCH TRUE)
    endif()
  else()
    # need greater than or equal
    if(NOT ${VERSION} VERSION_LESS ${EnergyPlus_FIND_VERSION})
      # but need equal major and minor versions too for I/O compatibility
      # This is risky above 8.2.0, because of possible iteration schema changes within the third digit, but go ahead and allow it
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
    break()
  endif()
  
endforeach()

mark_as_advanced(
  ENERGYPLUS_EXE
  ENERGYPLUS_IDD
  ENERGYPLUS_WEATHER_DIR
)

# These checks may seem redundant to above, but here we add log messages.
# We also check that found version still matches what is requested.
# This is important because found version is cached and may "stick" even after the requested version changes in the project.
# find_package_handle_standard_args takes this into account except it doesn't know that E+ is not backwards 
# compatible and it also doesn't know about EnergyPlus build SHA.
if(ENERGYPLUS_GE_8_2_0 AND ENERGYPLUS_BUILD_SHA)
  if(NOT (ENERGYPLUS_BUILD_SHA STREQUAL BUILD_SHA))
    message(FATAL_ERROR "Found EnergyPlus version: ${VERSION} with build SHA: ${BUILD_SHA}, but project requires build SHA: ${ENERGYPLUS_BUILD_SHA}")
  endif()
endif()

# Even if find_package didn't ask for exact match we need to check this, because E+ is not backwards compatible.
# find_package_handle_standard_args does not know about this quirk.
if(NOT (${VERSION_MAJOR_MINOR} VERSION_EQUAL "${EnergyPlus_FIND_VERSION_MAJOR}.${EnergyPlus_FIND_VERSION_MINOR}"))
  message(FATAL_ERROR "Found EnergyPlus version: ${VERSION} , but project requires ${EnergyPlus_FIND_VERSION_MAJOR}.${EnergyPlus_FIND_VERSION_MINOR}")
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(EnergyPlus FOUND_VAR EnergyPlus_FOUND
  REQUIRED_VARS ENERGYPLUS_EXE ENERGYPLUS_IDD ENERGYPLUS_WEATHER_DIR
  VERSION_VAR VERSION
)

