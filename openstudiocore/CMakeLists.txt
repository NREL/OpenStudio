cmake_minimum_required(VERSION 3.7.0)
cmake_policy(SET CMP0048 NEW)

# Use ccache is available, has to be before "project()"
find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
  # Support Unix Makefiles and Ninja
  set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
endif()

project(OpenStudio VERSION 2.9.0)

include(ExternalProject)
include(CPackComponent)
include(CheckCXXCompilerFlag)

if( POLICY CMP0022 )
  cmake_policy(SET CMP0022 NEW)
endif()
cmake_policy(SET CMP0020 NEW)
if(NOT ${CMAKE_VERSION} VERSION_LESS "3.0")
  cmake_policy(SET CMP0042 NEW)
  if(NOT ${CMAKE_VERSION} VERSION_LESS "3.1")
    cmake_policy(SET CMP0054 NEW)
  endif()
endif()

###############################################################################
#                                  N I N J A                                  #
###############################################################################
# Ninja support: has to be atop for it to take effect before anything else is done
# Add Color Output if Using Ninja
macro(AddCXXFlagIfSupported flag test)
  CHECK_CXX_COMPILER_FLAG(${flag} ${test})
  if(${${test}})
    message("adding ${flag}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${flag}")
  endif()
endmacro()

if("Ninja" STREQUAL ${CMAKE_GENERATOR})
  # Will be passed to OpenStudio.in to deal with the Win32 case
  set(NINJA TRUE)
  # Clang
  if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    AddCXXFlagIfSupported(-fcolor-diagnostics COMPILER_SUPPORTS_fcolor-diagnostics)
  endif()

  # g++
  if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    # For some reason it doesn't say its supported, but it works...
    # AddCXXFlagIfSupported(-fdiagnostics-color COMPILER_SUPPORTS_fdiagnostics-color)
    message(STATUS "Ninja: Forcing -fdiagnostics-color=always")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fdiagnostics-color=always")
  endif()
endif()

# Xcode/Ninja generators undefined MAKE
if(CMAKE_GENERATOR MATCHES "Make")
  set(MAKE "$(MAKE)")
else()
  set(MAKE make)
endif()

###############################################################################
# Project version information
set(PROJECT_VERSION_BUILD "Unknown" CACHE STRING "Build number") # git sha
find_package(Git)

if(NOT GIT_FOUND)
  find_program(GIT_EXECUTABLE git HINTS "$ENV{LOCALAPPDATA}/Programs/git/bin" "C:/Program Files/Git/bin")
  if(NOT GIT_EXECUTABLE_NOTFOUND)
    set(GIT_FOUND TRUE)
  endif()
endif()

if(GIT_FOUND)
  execute_process(COMMAND "${GIT_EXECUTABLE}" "rev-parse" "--short=10" "HEAD"
                  WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
                  TIMEOUT 10
                  RESULT_VARIABLE RESULT
                  OUTPUT_VARIABLE GIT_VERSION
                  ERROR_QUIET
                  OUTPUT_STRIP_TRAILING_WHITESPACE)
  if(${RESULT} EQUAL 0 AND NOT "${GIT_VERSION}" EQUAL "${PROJECT_VERSION_BUILD}")
    set(PROJECT_VERSION_BUILD ${GIT_VERSION} CACHE STRING "Build number" FORCE) # git sha
  endif()

  get_filename_component(GIT_DIR "${GIT_EXECUTABLE}" DIRECTORY)
else()
  set(GIT_DIR "")
endif()

find_program(PATCH_EXE patch HINTS "${GIT_DIR}" "${GIT_DIR}/../bin/" "${GIT_DIR}/../usr/bin/")
string(COMPARE EQUAL "${PATCH_EXE}" "PATCH_EXE-NOTFOUND" PATCH_EXE_NOTFOUND)
if(PATCH_EXE_NOTFOUND)
  message(SEND_ERROR "Required program patch not found")
endif()

# OpenStudio version
set(OPENSTUDIO_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}")
set(OPENSTUDIO_LONG_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}.${PROJECT_VERSION_BUILD}")

# EnergyPlus Idd version
set(ENERGYPLUS_VERSION_MAJOR 9)
set(ENERGYPLUS_VERSION_MINOR 2)
set(ENERGYPLUS_VERSION_PATCH 0)
set(ENERGYPLUS_VERSION "${ENERGYPLUS_VERSION_MAJOR}.${ENERGYPLUS_VERSION_MINOR}.${ENERGYPLUS_VERSION_PATCH}")
# Build SHA is not required to have a value, but if it does OpenStudio will require this build.
set(ENERGYPLUS_BUILD_SHA "d67e9e635f")

# ENERGYPLUS_RELEASE_NAME is used to locate the E+ download
# from the github releases
set(ENERGYPLUS_RELEASE_NAME "v9.2.0-Alpha2")

# Radiance
set(RADIANCE_VERSION "5.0.a.12")

# configure file with version information
configure_file(${CMAKE_SOURCE_DIR}/OpenStudio.in ${CMAKE_BINARY_DIR}/src/OpenStudio.hxx)

# universal configuration for mac plists
if(APPLE)
  set(MACOSX_BUNDLE_SHORT_VERSION_STRING "${OPENSTUDIO_VERSION}")
  set(MACOSX_BUNDLE_BUNDLE_VERSION "${PROJECT_VERSION_BUILD}")
  set(MACOSX_BUNDLE_COPYRIGHT "Copyright © 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.")
  set(CMAKE_OSX_DEPLOYMENT_TARGET "10.11" CACHE STRING "Minimum OS X deployment version")
endif()

# compiler id for about boxes
if(MSVC)
  set(ABOUT_COMPILER "${CMAKE_GENERATOR}")
elseif(APPLE)
  set(ABOUT_COMPILER "${CMAKE_CXX_COMPILER_ID} ${CMAKE_OSX_ARCHITECTURES} - OSX ${CMAKE_OSX_DEPLOYMENT_TARGET}")
else()
  set(ABOUT_COMPILER "${CMAKE_CXX_COMPILER_ID}")
endif()

# Set a default build type if none was specified
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to 'Release' as none was specified.")
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release"
    "MinSizeRel" "RelWithDebInfo")
endif()

###############################################################################

###############################################################################
# Build options

if(UNIX AND NOT APPLE)
  # the RPATH to be used when installing
  set(CMAKE_INSTALL_RPATH "$ORIGIN/../lib")
endif()

option(BUILD_PAT "Build PAT" OFF)

# Build C++ documentation using Doxygen
# Requires: doxygen
option(BUILD_DOCUMENTATION "Build Documentation" OFF)

# Build CSharp bindings
# Requires: SWIG
if(WIN32)
  option(BUILD_CSHARP_BINDINGS "Build CSharp bindings" OFF)
endif()

# Build Java bindings
# Requires: SWIG
option(BUILD_JAVA_BINDINGS "Build Java bindings" OFF)

# Build V8 Node bindings
# Requires: SWIG, Node
option(BUILD_V8_BINDINGS "Build Node V8 bindings" OFF)

# Build Python bindings
# Requires: SWIG Python
option(BUILD_PYTHON_BINDINGS "Build Python bindings" OFF)

# Build ctest testing
# Requires: EnergyPlus
option(BUILD_TESTING "Build testing targets" OFF)

# Build package
# Requires: EnergyPlus, Radiance
# TODO: this option is actually unused...
option(BUILD_PACKAGE "Build package" OFF)

# Build with OpenSSL support
set(BUILD_WITH_OPENSSL ON CACHE INTERNAL "Build With OpenSSL Support For SSH Connections")

# Build shared OS libraries
option(BUILD_SHARED_LIBS "Build OS libs as shared libraries" OFF)

# Build the OpenStudio Application
# This option will require shared libs
option(BUILD_OS_APP "Build OS App" ON)

if (BUILD_SHARED_LIBS)
  add_definitions("-DSHARED_OS_LIBS")
endif()

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  add_definitions(-Qunused-arguments)
endif()

if( WIN32 )
  option(BUILD_NUGET_PACKAGE "Build NuGet Package" OFF)
endif()

# Enable runtime checking features
if(CMAKE_COMPILER_IS_GNUCC OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  option(ENABLE_THREAD_SANITIZER "Enable thread sanitizer testing in gcc/clang" OFF)
  if(ENABLE_THREAD_SANITIZER)
    add_definitions(-fsanitize=thread -g)
    set(SANITIZER_LINKER_FLAGS "${SANITIZER_LINKER_FLAGS} -fsanitize=thread")
  endif()

  option(ENABLE_ADDRESS_SANITIZER "Enable address sanitizer testing in gcc/clang" OFF)
  if(ENABLE_ADDRESS_SANITIZER)
    add_definitions(-fsanitize=address -g)
    set(SANITIZER_LINKER_FLAGS "${SANITIZER_LINKER_FLAGS} -fsanitize=address")
  endif()

  option(ENABLE_UNDEFINED_SANITIZER "Enable undefined behavior sanitizer testing in gcc/clang" OFF)
  if(ENABLE_UNDEFINED_SANITIZER)
    add_definitions(-fsanitize=undefined -g)
    set(SANITIZER_LINKER_FLAGS "${SANITIZER_LINKER_FLAGS} -fsanitize=undefined")
  endif()

  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${SANITIZER_LINKER_FLAGS}")
  set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${SANITIZER_LINKER_FLAGS}")
  set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} ${SANITIZER_LINKER_FLAGS}")


  mark_as_advanced(ENABLE_THREAD_SANITIZER ENABLE_ADDRESS_SANITIZER ENABLE_UNDEFINED_SANITIZER)

endif()

IF (APPLE)
   find_library(COREFOUNDATION_LIBRARY CoreFoundation )
ENDIF (APPLE)

if(UNIX AND NOT APPLE)
  # Enable libgtest linking
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pthread")
endif()

if( WIN32 )
  if(CMAKE_CL_64)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /OPT:REF /OPT:NOICF /MAXILKSIZE:0x7FFFFFFF")
    set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} /MAXILKSIZE:0x7FFFFFFF")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /MAXILKSIZE:0x7FFFFFFF")
    set(CMAKE_STATIC_LINKER_FLAGS "${CMAKE_STATIC_LINKER_FLAGS} /MAXILKSIZE:0x7FFFFFFF")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zi")
  else()
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /OPT:REF /OPT:NOICF")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zi")
  endif()
endif()

if(BUILD_V8_BINDINGS)
  option(BUILD_NODE_MODULES "Build V8 Bindings as Node Modules" ON)
endif()

# Use PCH
option(USE_PCH "Use precompiled headers" OFF)

if(WIN32)
  add_definitions(-DNOMINMAX)
endif()

# /MP is useless for ninja (and nmake), and gets passed to rc.exe which will fatal out.
if(MSVC AND NOT NINJA)
  # Build with Multiple Processes
  set(BUILD_WITH_MULTIPLE_PROCESSES ON CACHE BOOL "/MP compiler flag for full processor utilization")
  mark_as_advanced(BUILD_WITH_MULTIPLE_PROCESSES)
endif()

###############################################################################

###############################################################################
# CMake control

# High level project configuration
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Products")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Products")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Products")

set(LIBRARY_SEARCH_DIRECTORY "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}" "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/Release" "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/Debug")

# Include project specific macros
include(ProjectMacros.cmake)

# Search for modules in the root dir first to override cmake ones
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake")

# Use CTest
set(ALL_TESTING_TARGETS "") # global list
if(BUILD_TESTING)
  enable_testing()
  include(CTest)
endif()
###############################################################################

###############################################################################
# Compiler and system specific options
if(UNIX)

  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -fPIC -fno-strict-aliasing")

  # all warnings
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -fPIC -fno-strict-aliasing -Winvalid-pch -Wnon-virtual-dtor")
  if(APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-overloaded-virtual -ftemplate-depth=1024")
  else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-overloaded-virtual")
  endif()

  if(APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -stdlib=libc++")
  else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
  endif()
endif()
if(MINGW)
  # all warnings
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
  # effective c++
  #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Weffc++")
  # treat warnings as errors
  #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")

  # wchar not supported for MinGW
  add_definitions(-DBOOST_LOG_USE_CHAR)
  add_definitions(-DBOOST_LOG_NO_COMPILER_TLS)
endif()
if(MSVC)
  # warning level 3
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W3")

  # warning level 4 - DLM: we should shoot for this
  #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")

  # all warnings - DLM: probably too high to ever use
  #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Wall")

  # create compiler processes on all effective processors
  # /MP is useless for ninja and will cause problems. BUILD_WITH_MULTIPLE_PROCESSES shouldn't have been set if ninja but let's be safe
  if(BUILD_WITH_MULTIPLE_PROCESSES AND NOT NINJA)
    add_definitions(/MP)
  endif()

  #Enable warning on thread un-safe static member initialization - DLM: we have been bit by this many times
  # TODO: JM 2019-09-19: Disabling for now
  # Using the righ flag (/w14460 and not the previously mispelled /w44640) will make compilation fail with many warnings treated as error,
  # eg: IddRegex.cpp all scoped `const static std::string result` inside functions will issue that and make it fatal
  # add_definitions(/w14640)

  # ignore decorated name length exceeded
  add_definitions(/wd4503)

  # ignore needs to have dll-interface to be used by clients of class
  # we will not rely on exporting templates instead all libraries must
  # be linked against the same runtime
  add_definitions(/wd4251)

  add_definitions(/bigobj)

  if(CMAKE_CL_64)
    # ignore conversion from size_t to int for now
    add_definitions(/wd4267)
  endif()

  # treat warnings as errors
  # DLM: only do this for our code, added in later
  # add_definitions(/WX)

  # ignore warnings about the stl being insecure
  add_definitions(-D_CRT_SECURE_NO_WARNINGS -D_SCL_SECURE_NO_WARNINGS)

endif()

##############################################################################


###############################################################################
# Check version of gcc
if(CMAKE_COMPILER_IS_GNUCC)
  execute_process(COMMAND ${CMAKE_C_COMPILER} -dumpversion
                  OUTPUT_VARIABLE GCC_VERSION)
endif()


###############################################################################
# Dependencies

# EnergyPlus

if(UNIX)
  if(APPLE)
    set(ENERGYPLUS_EXPECTED_HASH f9642bb57a5c4974b1f4a2818ea5776f)
    set(ENERGYPLUS_PLATFORM "Darwin-x86_64")
  elseif(EXISTS "/etc/redhat-release")
    # TODO: There aren't any redhat releases anymore, see PR #3145 too
    set(ENERGYPLUS_EXPECTED_HASH DOES_NOT_EXIST_02ed618aab65bf46c96bf1faf2e1905b)
    set(ENERGYPLUS_PLATFORM "Redhat-x86_64")
  else()
    set(ENERGYPLUS_EXPECTED_HASH 2f39127e55c3726315d509df4cdc2059)
    set(ENERGYPLUS_PLATFORM "Linux-x86_64")
  endif()
  set(ENERGYPLUS_PATH "EnergyPlus-${ENERGYPLUS_VERSION}-${ENERGYPLUS_BUILD_SHA}-${ENERGYPLUS_PLATFORM}")
  if(EXISTS "${CMAKE_BINARY_DIR}/${ENERGYPLUS_PATH}.tar.gz")
    file(MD5 "${CMAKE_BINARY_DIR}/${ENERGYPLUS_PATH}.tar.gz" ENERGYPLUS_HASH)
  endif()
  if(NOT EXISTS "${CMAKE_BINARY_DIR}/${ENERGYPLUS_PATH}.tar.gz" OR NOT "${ENERGYPLUS_HASH}" MATCHES "${ENERGYPLUS_EXPECTED_HASH}")
    message(STATUS "Downloading EnergyPlus ${ENERGYPLUS_VERSION} (${ENERGYPLUS_PLATFORM})")

    if(EXISTS "/etc/redhat-release")
      file(DOWNLOAD "http://openstudio-resources.s3.amazonaws.com/dependencies/${ENERGYPLUS_PATH}.tar.gz" "${CMAKE_BINARY_DIR}/${ENERGYPLUS_PATH}.tar.gz" INACTIVITY_TIMEOUT 320 SHOW_PROGRESS EXPECTED_MD5 ${ENERGYPLUS_EXPECTED_HASH})
    else()
      file(DOWNLOAD "https://github.com/NREL/EnergyPlus/releases/download/${ENERGYPLUS_RELEASE_NAME}/${ENERGYPLUS_PATH}.tar.gz" "${CMAKE_BINARY_DIR}/${ENERGYPLUS_PATH}.tar.gz" INACTIVITY_TIMEOUT 320 SHOW_PROGRESS EXPECTED_MD5 ${ENERGYPLUS_EXPECTED_HASH})
    endif()

    # Remove the old extracted dir if exists
    execute_process(COMMAND ${CMAKE_COMMAND} -E remove_directory "${CMAKE_BINARY_DIR}/${ENERGYPLUS_PATH}")

  endif()

elseif(WIN32)
  if(CMAKE_CL_64)
    set(ENERGYPLUS_PATH "EnergyPlus-${ENERGYPLUS_VERSION}-${ENERGYPLUS_BUILD_SHA}-Windows-x86_64")
    set(ENERGYPLUS_ARCH 64)
    set(ENERGYPLUS_EXPECTED_HASH 7c10b31d823920f99bfae1526f42dbce)
  else()
    set(ENERGYPLUS_PATH "EnergyPlus-${ENERGYPLUS_VERSION}-${ENERGYPLUS_BUILD_SHA}-Windows-i386")
    set(ENERGYPLUS_ARCH 32)
    ## TODO: update md5 hash, but not present on GitHub as .zip, only .exe for now at least
    set(ENERGYPLUS_EXPECTED_HASH DOES_NOT_EXIST_a2cbd995acd1928a7baa18407bb8e949)
  endif()
  if(EXISTS "${CMAKE_BINARY_DIR}/${ENERGYPLUS_PATH}.zip")
    file(MD5 "${CMAKE_BINARY_DIR}/${ENERGYPLUS_PATH}.zip" ENERGYPLUS_HASH)
  endif()
  if(NOT EXISTS "${CMAKE_BINARY_DIR}/${ENERGYPLUS_PATH}.zip" OR NOT "${ENERGYPLUS_HASH}" MATCHES "${ENERGYPLUS_EXPECTED_HASH}")
    message(STATUS "Downloading EnergyPlus ${ENERGYPLUS_VERSION} (${ENERGYPLUS_ARCH}-bit)")

    file(DOWNLOAD "https://github.com/NREL/EnergyPlus/releases/download/${ENERGYPLUS_RELEASE_NAME}/${ENERGYPLUS_PATH}.zip" "${CMAKE_BINARY_DIR}/${ENERGYPLUS_PATH}.zip" INACTIVITY_TIMEOUT 120 SHOW_PROGRESS EXPECTED_MD5 ${ENERGYPLUS_EXPECTED_HASH})

    # Remove the old extracted dir if exists
    execute_process(COMMAND ${CMAKE_COMMAND} -E remove_directory "${CMAKE_BINARY_DIR}/${ENERGYPLUS_PATH}")

  endif()

endif()

# Allows placing the archive manually in the build dir too
if (NOT EXISTS "${ENERGYPLUS_PATH}")
  unset(ENERGYPLUS_FOUND CACHE)
  unset(ENERGYPLUS_EXE CACHE)
  unset(ENERGYPLUS_IDD CACHE)
  unset(ENERGYPLUS_WEATHER_DIR CACHE)
  if (UNIX)
    execute_process(COMMAND ${CMAKE_COMMAND} -E tar xfz "${CMAKE_BINARY_DIR}/${ENERGYPLUS_PATH}.tar.gz" WORKING_DIRECTORY "${CMAKE_BINARY_DIR}")
  else()
    execute_process(COMMAND ${CMAKE_COMMAND} -E tar xfz "${CMAKE_BINARY_DIR}/${ENERGYPLUS_PATH}.zip" WORKING_DIRECTORY "${CMAKE_BINARY_DIR}")
  endif()
endif()

# Prior to E+ 9.1.0 (OS 2.7.2), zip/tar.gz would extract to something like `./EnergyPlus-9.0.0-buildsha-Linux-x86_64/EnergyPlus-9-0-0`
if (ENERGYPLUS_VERSION VERSION_LESS "9.1.0")
  set(ENV{ENERGYPLUSDIR} "${PROJECT_BINARY_DIR}/${ENERGYPLUS_PATH}/EnergyPlus-${ENERGYPLUS_VERSION_MAJOR}-${ENERGYPLUS_VERSION_MINOR}-${ENERGYPLUS_VERSION_PATCH}")
else()
  # After 9.1.0, due to QtIFW/Mac packaging (NREL/EnergPlus#7148), it is going directly to `./EnergyPlus-9.1.0-buildsha-Linux-x86_64/`
  set(ENV{ENERGYPLUSDIR} "${PROJECT_BINARY_DIR}/${ENERGYPLUS_PATH}")
endif()

# Find EnergyPlus, which will set the ENERGYPLUS_XXX variables and cache them
find_package(EnergyPlus "${ENERGYPLUS_VERSION}" REQUIRED)

# Radiance
if(UNIX)
  if(APPLE)
    set(RADIANCE_EXPECTED_HASH 115ef9052ad5011d3fc7cebc2991644f)
    set(RADIANCE_PLATFORM "Darwin")
  elseif(EXISTS "/etc/redhat-release")
    # DLM: note that the installer expects the extracted directory to be named radiance-5.0.a.12-Linux
    set(RADIANCE_EXPECTED_HASH f75898bbfd238a740c1902d385c42b92)
    set(RADIANCE_PLATFORM "Redhat")
  else()
    set(RADIANCE_EXPECTED_HASH 7b9e4df2b6050fe51c6e138e4c0e6a5a)
    set(RADIANCE_PLATFORM "Linux")
  endif()
  set(RADIANCE_PATH "radiance-${RADIANCE_VERSION}-${RADIANCE_PLATFORM}")
  if(EXISTS "${CMAKE_BINARY_DIR}/${RADIANCE_PATH}.tar.gz")
    file(MD5 "${CMAKE_BINARY_DIR}/${RADIANCE_PATH}.tar.gz" RADIANCE_HASH)
  endif()
  if(NOT EXISTS "${CMAKE_BINARY_DIR}/${RADIANCE_PATH}.tar.gz" OR NOT "${RADIANCE_HASH}" MATCHES "${RADIANCE_EXPECTED_HASH}")
  message(STATUS "Downloading Radiance ${RADIANCE_VERSION} (${RADIANCE_PLATFORM})")
  if(EXISTS "/etc/redhat-release")
    message("http://openstudio-resources.s3.amazonaws.com/dependencies/radiance-${RADIANCE_VERSION}-${RADIANCE_PLATFORM}.tar.gz")
    file(DOWNLOAD "http://openstudio-resources.s3.amazonaws.com/dependencies/radiance-${RADIANCE_VERSION}-${RADIANCE_PLATFORM}.tar.gz" "${CMAKE_BINARY_DIR}/${RADIANCE_PATH}.tar.gz" TIMEOUT 120 INACTIVITY_TIMEOUT 120 SHOW_PROGRESS EXPECTED_MD5 ${RADIANCE_EXPECTED_HASH})
  else()
    file(DOWNLOAD "http://github.com/NREL/Radiance/releases/download/${RADIANCE_VERSION}/radiance-${RADIANCE_VERSION}-${RADIANCE_PLATFORM}.tar.gz" "${CMAKE_BINARY_DIR}/${RADIANCE_PATH}.tar.gz" TIMEOUT 120 INACTIVITY_TIMEOUT 120 SHOW_PROGRESS EXPECTED_MD5 ${RADIANCE_EXPECTED_HASH})
  endif()
    execute_process(COMMAND ${CMAKE_COMMAND} -E remove_directory "${CMAKE_BINARY_DIR}/${RADIANCE_PATH}")
    execute_process(COMMAND ${CMAKE_COMMAND} -E tar xfz "${CMAKE_BINARY_DIR}/${RADIANCE_PATH}.tar.gz" WORKING_DIRECTORY "${CMAKE_BINARY_DIR}")
  endif()
  set(RADIANCE_LOCATION "${CMAKE_BINARY_DIR}/radiance-${RADIANCE_VERSION}-${RADIANCE_PLATFORM}/usr/local/radiance")
elseif(WIN32)
  if(CMAKE_CL_64)
    set(RADIANCE_ARCH 64)
    set(RADIANCE_EXPECTED_HASH 7bd7a418d181365b4153c8bc146ede06)
  else()
    message(WARNING "32 bit Radiance build is not available")
    message(WARNING "Using 64 bit build of Radiance with a 32 bit build of OpenStudio")
    set(RADIANCE_ARCH 64)
    set(RADIANCE_EXPECTED_HASH 7bd7a418d181365b4153c8bc146ede06)
    #set(RADIANCE_ARCH 32)
    #set(RADIANCE_EXPECTED_HASH 78eda5d4b0215cd675cc3ec8975e6122)
  endif()
  set(RADIANCE_PATH "radiance-${RADIANCE_VERSION}-win${RADIANCE_ARCH}")
  set(RADIANCE_PLATFORM "win${RADIANCE_ARCH}")
  if(EXISTS "${CMAKE_BINARY_DIR}/${RADIANCE_PATH}.zip")
    file(MD5 "${CMAKE_BINARY_DIR}/${RADIANCE_PATH}.zip" RADIANCE_HASH)
  endif()
  if(NOT EXISTS "${CMAKE_BINARY_DIR}/${RADIANCE_PATH}.zip" OR NOT "${RADIANCE_HASH}" MATCHES "${RADIANCE_EXPECTED_HASH}")
    message(STATUS "Downloading Radiance ${RADIANCE_VERSION} (${RADIANCE_ARCH}-bit)")
    file(DOWNLOAD "http://github.com/NREL/Radiance/releases/download/${RADIANCE_VERSION}/Radiance-${RADIANCE_VERSION}-win${RADIANCE_ARCH}.zip" "${CMAKE_BINARY_DIR}/${RADIANCE_PATH}.zip" TIMEOUT 120 INACTIVITY_TIMEOUT 120 SHOW_PROGRESS EXPECTED_MD5 ${RADIANCE_EXPECTED_HASH})
    execute_process(COMMAND ${CMAKE_COMMAND} -E remove_directory "${CMAKE_BINARY_DIR}/${RADIANCE_PATH}")
    execute_process(COMMAND ${CMAKE_COMMAND} -E tar xfz "${CMAKE_BINARY_DIR}/${RADIANCE_PATH}.zip" WORKING_DIRECTORY "${CMAKE_BINARY_DIR}")
  endif()
  set(RADIANCE_LOCATION "${CMAKE_BINARY_DIR}/radiance-${RADIANCE_VERSION}-${RADIANCE_PLATFORM}/")
endif()

# Perl
if(NOT UNIX)
  execute_process(COMMAND ${CMAKE_COMMAND} -E tar xfz "${CMAKE_SOURCE_DIR}/../dependencies/strawberry-perl-5.16.2.1-32bit-portable-reduced.tar.bz2"
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    RESULT_VARIABLE rv)
  set(PERL_EXE "${CMAKE_BINARY_DIR}/strawberry-perl-5.16.2.1-32bit-portable-reduced/perl/bin/perl.exe")
endif()

# Ruby

set(RUBY_INSTALL_DIR "" CACHE PATH "Path to Ruby Install")
mark_as_advanced(RUBY_INSTALL_DIR)

if(NOT EXISTS ${RUBY_INSTALL_DIR})
  if( WIN32 )
    if(CMAKE_CL_64)
      set(RUBY_ZIP_FILENAME "ruby_2_2_4_msvc2013_static_2.tar.gz")
      set(RUBY_ZIP_EXPECTED_MD5 "74cc63d6f997097b8e650999a1da717f")
    else()
      set(RUBY_ZIP_FILENAME "ruby_2_2_4_msvc2013_32_static_2.tar.gz")
      set(RUBY_ZIP_EXPECTED_MD5 "DOES_NOT_EXIST_4c2055305b105b0c8210697aaa596419")
    endif()
  elseif( APPLE )
    set(RUBY_ZIP_FILENAME "ruby_2_2_4_osx_static.tar.gz")
    set(RUBY_ZIP_EXPECTED_MD5 "7ac16f5791d210d425aef268d97bf734")
  elseif(EXISTS "/etc/redhat-release")
    set(RUBY_ZIP_FILENAME "ruby_2_2_4_redhat_static.tar.gz")
    set(RUBY_ZIP_EXPECTED_MD5 "d130a17b23cc532975665ee4e9095c8d")
  else()
    set(RUBY_ZIP_FILENAME "ruby_2_2_4_linux_static.tar.gz")
    set(RUBY_ZIP_EXPECTED_MD5 "d238febf646382fe1b323bcdf16ea0e2")
  endif()

  set(RUBY_ZIP_LOCAL_PATH "${CMAKE_BINARY_DIR}/${RUBY_ZIP_FILENAME}")
  if(EXISTS "${RUBY_ZIP_LOCAL_PATH}")
    file(MD5 "${RUBY_ZIP_LOCAL_PATH}" RUBY_ZIP_MD5)
  endif()

  if( NOT RUBY_ZIP_MD5 STREQUAL RUBY_ZIP_EXPECTED_MD5)
    message(STATUS "Downloading Ruby: ${RUBY_ZIP_FILENAME}")
    file(DOWNLOAD "http://openstudio-resources.s3.amazonaws.com/dependencies/${RUBY_ZIP_FILENAME}"
      ${RUBY_ZIP_LOCAL_PATH}
      INACTIVITY_TIMEOUT 120
      SHOW_PROGRESS
      EXPECTED_MD5 ${RUBY_ZIP_EXPECTED_MD5})
      execute_process(COMMAND ${CMAKE_COMMAND} -E tar xfz ${RUBY_ZIP_LOCAL_PATH} WORKING_DIRECTORY "${CMAKE_BINARY_DIR}")
  endif()

  if( WIN32 )
    set(RUBY_INSTALL_DIR "${CMAKE_BINARY_DIR}/Ruby-install/release")
  else()
    set(RUBY_INSTALL_DIR "${CMAKE_BINARY_DIR}/Ruby-install/")
  endif()
endif()

list(APPEND CMAKE_PREFIX_PATH "${RUBY_INSTALL_DIR}")
if( WIN32 )
  find_package(Ruby 2.2 REQUIRED)
  get_filename_component(RUBY_LIBRARY_NAME_WE ${RUBY_LIBRARY} NAME_WE)
  get_filename_component(RUBY_LIBRARY_DIRECTORY ${RUBY_LIBRARY} DIRECTORY)
  get_filename_component(RUBY_LIBRARY_EXT ${RUBY_LIBRARY} EXT)
  set(RUBY_STATIC_LIBRARY "${RUBY_LIBRARY_DIRECTORY}/${RUBY_LIBRARY_NAME_WE}-static${CMAKE_STATIC_LIBRARY_SUFFIX}")
  set(RUBY_STATIC_LIBRARY_DEBUG "${RUBY_LIBRARY_DIRECTORY}/../../debug/lib/${RUBY_LIBRARY_NAME_WE}-static${CMAKE_STATIC_LIBRARY_SUFFIX}")
  set(RUBY_LIBRARY_DEBUG "${RUBY_LIBRARY_DIRECTORY}/../../debug/lib/${RUBY_LIBRARY_NAME_WE}${CMAKE_STATIC_LIBRARY_SUFFIX}")
else()
  set(RUBY_STATIC_LIBRARY "${RUBY_INSTALL_DIR}/lib/libruby-static.a")
  list(APPEND RUBY_INCLUDE_DIRS "${RUBY_INSTALL_DIR}/include/ruby-2.2.0")
  list(APPEND RUBY_INCLUDE_DIRS "${RUBY_INSTALL_DIR}/include/ruby-2.2.0/x86_64-linux")
  list(APPEND RUBY_INCLUDE_DIRS "${RUBY_INSTALL_DIR}/include/ruby-2.2.0/x86_64-darwin15")
  set(RUBY_INCLUDE_DIR "${RUBY_INSTALL_DIR}/include/ruby-2.2.0")
  set(RUBY_EXECUTABLE "${RUBY_INSTALL_DIR}/bin/ruby")
  set(RUBY_POSSIBLE_LIB_DIR "${RUBY_INSTALL_DIR}/lib")
endif()

# Mingw based ruby lib (only the dll stub library) from Ruby Installer
# We need this for stand alone ruby bindings
# target to use with typical ruby installations (those that are not msvc)
# Only need this on Windows systems
if( WIN32 )
if(NOT EXISTS ${RUBY_MINGW_LIB_DIR})
  set(RUBY_MINGW_LIB_ZIP_FILENAME "Ruby-mingw-install.tar.gz")
  set(RUBY_MINGW_LIB_ZIP_EXPECTED_MD5 "f88386bf2eee08e06e84ee981e43dc98")

  set(RUBY_MINGW_LIB_ZIP_LOCAL_PATH "${CMAKE_BINARY_DIR}/${RUBY_MINGW_LIB_ZIP_FILENAME}")
  if(EXISTS "${RUBY_MINGW_LIB_ZIP_LOCAL_PATH}")
    file(MD5 "${RUBY_MINGW_LIB_ZIP_LOCAL_PATH}" RUBY_MINGW_LIB_ZIP_MD5)
  endif()

  if( NOT RUBY_MINGW_LIB_ZIP_MD5 STREQUAL RUBY_MINGW_LIB_ZIP_EXPECTED_MD5)
    message(STATUS "Downloading Ruby Mingw Lib: ${RUBY_MINGW_LIB_ZIP_FILENAME}")
    file(DOWNLOAD "http://openstudio-resources.s3.amazonaws.com/dependencies/${RUBY_MINGW_LIB_ZIP_FILENAME}"
      ${RUBY_MINGW_LIB_ZIP_LOCAL_PATH}
      INACTIVITY_TIMEOUT 120
      SHOW_PROGRESS
      EXPECTED_MD5 ${RUBY_MINGW_LIB_ZIP_EXPECTED_MD5})
      execute_process(COMMAND ${CMAKE_COMMAND} -E tar xfz ${RUBY_MINGW_LIB_ZIP_LOCAL_PATH} WORKING_DIRECTORY "${CMAKE_BINARY_DIR}")
  endif()

  set(RUBY_MINGW_STUB_LIB "${CMAKE_BINARY_DIR}/Ruby-mingw-install/libx64-msvcrt-ruby220.dll.a")
endif()
endif()

# openstudio gems

# build and release openstudio gems at https://github.com/NREL/OpenStudio-gems
# upload the openstudio-gems-DATE.tar.gz to the dependencies location on s3, then update the MD5sum and url below
set(OPENSTUDIO_GEMS_ZIP_FILENAME "openstudio-gems-20190916.tar.gz")
set(OPENSTUDIO_GEMS_ZIP_EXPECTED_MD5 "14f08cf581b5e3951e91f7949ff5fafd")

set(OPENSTUDIO_GEMS_ZIP_LOCAL_PATH "${CMAKE_BINARY_DIR}/${OPENSTUDIO_GEMS_ZIP_FILENAME}")
if(EXISTS "${OPENSTUDIO_GEMS_ZIP_LOCAL_PATH}")
  file(MD5 "${OPENSTUDIO_GEMS_ZIP_LOCAL_PATH}" OPENSTUDIO_GEMS_ZIP_MD5)
endif()

if( NOT OPENSTUDIO_GEMS_ZIP_MD5 STREQUAL OPENSTUDIO_GEMS_ZIP_EXPECTED_MD5)
  message(STATUS "Downloading OpenStudio Gems: ${OPENSTUDIO_GEMS_ZIP_FILENAME}")
  file(DOWNLOAD "http://openstudio-resources.s3.amazonaws.com/dependencies/${OPENSTUDIO_GEMS_ZIP_FILENAME}"
    ${OPENSTUDIO_GEMS_ZIP_LOCAL_PATH}
    INACTIVITY_TIMEOUT 120
    SHOW_PROGRESS
    EXPECTED_MD5 ${OPENSTUDIO_GEMS_ZIP_EXPECTED_MD5})

  file(REMOVE_RECURSE "${CMAKE_BINARY_DIR}/openstudio-gems")
  execute_process(COMMAND ${CMAKE_COMMAND} -E tar xfz ${OPENSTUDIO_GEMS_ZIP_LOCAL_PATH} WORKING_DIRECTORY "${CMAKE_BINARY_DIR}")

  file(REMOVE_RECURSE "${CMAKE_BINARY_DIR}/src/cli/embedded_files")
endif()

# OpenSSL / libssh support
if(BUILD_WITH_OPENSSL)
  if(UNIX)
    find_package(OpenSSL)
    if(NOT ${OPENSSL_FOUND})
      message(SEND_ERROR "OpenSSL could not be found, and is required for HTTPS communication")
      message(SEND_ERROR "Please install OpenSSL development libraries using your package management system (possibly libssl-dev)")
    else()
      mark_as_advanced(
        LIB_EAY_DEBUG
        LIB_EAY_RELEASE
        SSL_EAY_DEBUG
        SSL_EAY_RELEASE
      )
    endif()
  elseif(WIN32)
    if(CMAKE_CL_64)
      set(OPENSSL_ARCH 64)
      set(OPENSSL_EXPECTED_HASH 3f9c88523634c7b1e27f4026e7c3f3ee)
    else()
      set(OPENSSL_ARCH 32)
      set(OPENSSL_EXPECTED_HASH fb2aa1a8ceb85f68055044a85f1dbc94)
    endif()
    set(OPENSSL_VERSION "1.0.2a")
    set(OPENSSL_PATH "OpenSSL-Win${OPENSSL_ARCH}")
    if(EXISTS "${CMAKE_BINARY_DIR}/${OPENSSL_PATH}.zip")
      file(MD5 "${CMAKE_BINARY_DIR}/${OPENSSL_PATH}.zip" OPENSSL_HASH)
    endif()
    if(NOT EXISTS "${CMAKE_BINARY_DIR}/${OPENSSL_PATH}.zip" OR NOT EXISTS "${CMAKE_BINARY_DIR}/${OPENSSL_PATH}" OR NOT "${OPENSSL_HASH}" MATCHES "${OPENSSL_EXPECTED_HASH}")
      message(STATUS "Downloading OpenSSL ${OPENSSL_VERSION} (${OPENSSL_ARCH}-bit)")
      file(DOWNLOAD "http://openstudio-resources.s3.amazonaws.com/dependencies/OpenSSL-Win${OPENSSL_ARCH}-${OPENSSL_VERSION}.zip" "${CMAKE_BINARY_DIR}/${OPENSSL_PATH}.zip" TIMEOUT 120 INACTIVITY_TIMEOUT 120 SHOW_PROGRESS EXPECTED_MD5 ${OPENSSL_EXPECTED_HASH})
      execute_process(COMMAND ${CMAKE_COMMAND} -E remove_directory "${CMAKE_BINARY_DIR}/${OPENSSL_PATH}")
      execute_process(COMMAND ${CMAKE_COMMAND} -E tar xfz "${CMAKE_BINARY_DIR}/${OPENSSL_PATH}.zip" WORKING_DIRECTORY "${CMAKE_BINARY_DIR}")
    endif()
    set(OPENSSL_ROOT_DIR "${CMAKE_BINARY_DIR}/${OPENSSL_PATH}")
    set(LIBEAY32_PATH "${OPENSSL_ROOT_DIR}/libeay32.dll")
    set(SSLEAY32_PATH "${OPENSSL_ROOT_DIR}/ssleay32.dll")
    file(COPY "${LIBEAY32_PATH}" "${SSLEAY32_PATH}" DESTINATION "${CMAKE_BINARY_DIR}/Products/Release/")
    file(COPY "${LIBEAY32_PATH}" "${SSLEAY32_PATH}" DESTINATION "${CMAKE_BINARY_DIR}/Products/Debug/")
    file(COPY "${LIBEAY32_PATH}" "${SSLEAY32_PATH}" DESTINATION "${CMAKE_BINARY_DIR}/Products/RelWithDebInfo/")
    file(COPY "${LIBEAY32_PATH}" "${SSLEAY32_PATH}" DESTINATION "${CMAKE_BINARY_DIR}/Products/MinSizeRel/")
    install(FILES "${LIBEAY32_PATH}" "${SSLEAY32_PATH}" DESTINATION "bin")
    #install(FILES "${LIBEAY32_PATH}" "${SSLEAY32_PATH}" DESTINATION ${RUBY_MODULE_ARCH_DIR})
  endif()
endif()

# DView
option(BUILD_DVIEW "Build DView" OFF)

if( BUILD_DVIEW )
  set(DVIEW_INSTALL_DIR "" CACHE PATH "Path to DView Install")
  mark_as_advanced(DVIEW_INSTALL_DIR)

  if(NOT EXISTS ${DVIEW_INSTALL_DIR})
    if( WIN32 )
      set(DVIEW_ZIP_FILENAME "DView-windows-20180621.zip")
      set(DVIEW_ZIP_EXPECTED_MD5 "8443b09cbfcc118943127b006b14dbca")
    elseif( APPLE )
      set(DVIEW_ZIP_FILENAME "DView-osx-20180621.tar.gz")
      set(DVIEW_ZIP_EXPECTED_MD5 "41038cbf7c472522e700d850d9de1133")
    else()
      set(DVIEW_ZIP_FILENAME "DView-linux-20180621.tar.gz")
      set(DVIEW_ZIP_EXPECTED_MD5 "c1d3fb6e736668eb4bdf0a5af2b4506c")
    endif()

    set(DVIEW_ZIP_LOCAL_PATH "${CMAKE_BINARY_DIR}/${DVIEW_ZIP_FILENAME}")
    if(EXISTS "${DVIEW_ZIP_LOCAL_PATH}")
      file(MD5 "${DVIEW_ZIP_LOCAL_PATH}" RUBY_ZIP_MD5)
    endif()

    if( NOT DVIEW_ZIP_MD5 STREQUAL DVIEW_ZIP_EXPECTED_MD5)
      message(STATUS "Downloading DView: ${DVIEW_ZIP_FILENAME}")
      file(DOWNLOAD "http://openstudio-resources.s3.amazonaws.com/dependencies/${DVIEW_ZIP_FILENAME}"
        ${DVIEW_ZIP_LOCAL_PATH}
        INACTIVITY_TIMEOUT 120
        SHOW_PROGRESS
        EXPECTED_MD5 ${DVIEW_ZIP_EXPECTED_MD5})
        execute_process(COMMAND ${CMAKE_COMMAND} -E tar xfz ${DVIEW_ZIP_LOCAL_PATH} WORKING_DIRECTORY "${CMAKE_BINARY_DIR}")
    endif()

    set(DVIEW_INSTALL_DIR "${CMAKE_BINARY_DIR}/DView-install")
  endif()

  if( WIN32 )
    install(PROGRAMS "${DVIEW_INSTALL_DIR}/DView.exe" DESTINATION ./bin COMPONENT "DView")
  elseif( APPLE )
    install(DIRECTORY "${DVIEW_INSTALL_DIR}/DView.app" DESTINATION . USE_SOURCE_PERMISSIONS COMPONENT "DView")
  else()
    install(PROGRAMS "${DVIEW_INSTALL_DIR}/DView" DESTINATION ./bin COMPONENT "DView")
  endif()

endif()

# SWIG

# using forked version of SWIG at https://github.com/macumber/swig/tree/openstudio_swig_3_0_12
if(UNIX)
  # We patch it up with a version of pcre we provide to avoid having to have the requirement locally
  ExternalProject_Add(SWIG
    URL http://openstudio-resources.s3.amazonaws.com/dependencies/swig-3.0.12.tar.gz
    URL_MD5 a7d384966974ed79455a455b517ad83d
    PATCH_COMMAND cp ${CMAKE_SOURCE_DIR}/../dependencies/pcre-8.31.tar.bz2 ${CMAKE_BINARY_DIR}/SWIG-prefix/src/SWIG && cd ${CMAKE_BINARY_DIR}/SWIG-prefix/src/SWIG && ./Tools/pcre-build.sh
    CONFIGURE_COMMAND ./autogen.sh && ./configure --prefix=${CMAKE_BINARY_DIR}/SWIG-prefix/src/SWIG-install
    BUILD_COMMAND ${MAKE}
    INSTALL_COMMAND ${MAKE} install
    BUILD_IN_SOURCE 1
  )
  set(SWIG_EXECUTABLE ${CMAKE_BINARY_DIR}/SWIG-prefix/src/SWIG-install/bin/swig)
else()
  # SWIG requires MinGW to compile on windows, so we just copy in the prebuilt binary
  set(SWIG_ZIP "swigwin-3.0.12.zip")
  set(SWIG_DIR "swigwin-3.0.12")
  set(SWIG_EXPECTED_HASH "8ee412d1278ba3cac22e58f26a842918")
  if(EXISTS "${CMAKE_BINARY_DIR}/${SWIG_ZIP}")
    file(MD5 "${CMAKE_BINARY_DIR}/${SWIG_ZIP}" SWIG_HASH)
  endif()
  if(NOT EXISTS "${CMAKE_BINARY_DIR}/${SWIG_ZIP}" OR NOT EXISTS "${CMAKE_BINARY_DIR}/${SWIG_DIR}" OR NOT "${SWIG_HASH}" MATCHES "${SWIG_EXPECTED_HASH}")
    message(STATUS "Downloading SWIG)")
    file(DOWNLOAD "http://openstudio-resources.s3.amazonaws.com/dependencies/${SWIG_ZIP}" "${CMAKE_BINARY_DIR}/${SWIG_ZIP}" TIMEOUT 120 INACTIVITY_TIMEOUT 120 SHOW_PROGRESS EXPECTED_MD5 ${SWIG_EXPECTED_HASH})
    execute_process(COMMAND ${CMAKE_COMMAND} -E remove_directory "${CMAKE_BINARY_DIR}/${SWIG_DIR}")
    execute_process(COMMAND ${CMAKE_COMMAND} -E tar xfz "${CMAKE_BINARY_DIR}/${SWIG_ZIP}" WORKING_DIRECTORY "${CMAKE_BINARY_DIR}")
  endif()

  set(SWIG_EXECUTABLE ${CMAKE_BINARY_DIR}/${SWIG_DIR}/swig.exe)
endif()


set(ALL_RUBY_BINDING_TARGETS "") # global list

# build rdoc
if(BUILD_DOCUMENTATION)
  if(WIN32)
    get_filename_component(RUBY_BIN_DIR "${RUBY_EXECUTABLE}" DIRECTORY)
    find_file(RDOC NAMES rdoc.bat HINTS $ENV{PATH} "${RUBY_BIN_DIR}")
    find_file(RI NAMES ri.bat HINTS $ENV{PATH} "${RUBY_BIN_DIR}")
  else()
    find_program(RDOC NAMES rdoc)
    find_program(RI NAMES ri)
  endif()

  mark_as_advanced(
    RDOC
    RI
  )

  set(ALL_RDOC_TARGETS ${ALL_RUBY_BINDING_TARGETS})
endif()

# C#
if(BUILD_CSHARP_BINDINGS)
  set(ALL_CSHARP_BINDING_DEPENDS "") # global list of library dependencies of the generated wrapper cxx files
  set(ALL_CSHARP_WRAPPER_FILES "") # global list of generated wrapper cxx files
  set(ALL_CSHARP_WRAPPER_TARGETS "") # global list targets that build generated wrapper cxx files
endif()

# JAVA
if(BUILD_JAVA_BINDINGS)
  find_package(JNI)
  find_package(Java)
  set(ALL_JAVA_BINDING_TARGETS "") # global list
  set(ALL_JAVA_SRC_DIRECTORIES "") # global list
endif()


if(BUILD_NODE_MODULES)
  add_definitions(-DBUILD_NODE_MODULE)
endif()

if(BUILD_PYTHON_BINDINGS)
  # need python
  # TODO: add support for 3.x
  find_package(PythonInterp 2.7 REQUIRED)
  find_package(PythonLibs 2.7 REQUIRED)
  include_directories(SYSTEM ${PYTHON_INCLUDE_DIRS})
  set(ALL_PYTHON_BINDINGS "") # global list of python bindings
  set(ALL_PYTHON_BINDING_DEPENDS "") # global list of library dependencies of the generated wrapper cxx files
  set(ALL_PYTHON_WRAPPER_FILES "") # global list of generated wrapper cxx files
  set(ALL_PYTHON_WRAPPER_TARGETS "") # global list targets that build generated wrapper cxx files
endif()


# Required dependencies

# Threading library
find_package(Threads)
if(UNIX)
  set(CMAKE_THREAD_LIBS "${CMAKE_THREAD_LIBS_INIT}" CACHE STRING "Thread library used.")
  mark_as_advanced(CMAKE_THREAD_LIBS)
endif()

# Zlib
include_directories(SYSTEM src/zlib)
add_subdirectory(src/zlib)
# make sure zconf.h can be found
include_directories(${CMAKE_BINARY_DIR}/src/zlib)

if(BUILD_TESTING)
  # force gtest to build as a static library no matter what, otherwise we get error about undefined death test symbols when building shared libs
  set(TEMP_BUILD_SHARED_LIBS ${BUILD_SHARED_LIBS})
  set(BUILD_SHARED_LIBS OFF)

  # Google test library
  set(gtest_force_shared_crt ON CACHE BOOL "gtest_force_shared_crt" FORCE) # in case we are linking with shared or static os libraries
  set(gtest_build_tests OFF CACHE BOOL "gtest_build_tests" FORCE)
  set(gtest_build_samples OFF CACHE BOOL "gtest_build_samples" FORCE)
  set(gtest_disable_pthreads OFF CACHE BOOL "gtest_disable_pthreads" FORCE)
  add_definitions(-DGTEST_HAS_TR1_TUPLE=0)
  include_directories(SYSTEM src/google_test/include)
  add_subdirectory(src/google_test)

  # restore BUILD_SHARED_LIBS
  set(BUILD_SHARED_LIBS ${TEMP_BUILD_SHARED_LIBS})
endif()

# GeographicLib
set(GEOGRAPHICLIB_LIB_TYPE "STATIC" CACHE INTERNAL "Build Static Lib")
set(GEOGRAPHICLIB_STATIC_LIB ON CACHE INTERNAL "Build Static Lib")
set(GEOGRAPHICLIB_SHARED_LIB OFF CACHE INTERNAL "Build Shared Lib")
set(GEOGRAPHICLIB_DOCUMENTATION OFF CACHE INTERNAL "Build Documenation")
set(BUILD_NETGEOGRAPHICLIB OFF CACHE INTERNAL "Build .NET")
set(GEOGRAPHICLIB_PRECISION "2" CACHE INTERNAL "Precision")
include_directories(SYSTEM src/geographic_lib/include)
add_subdirectory(src/geographic_lib)

# Boost
set(BOOST_INSTALL_DIR "" CACHE PATH "Path to Boost Install")
mark_as_advanced(BOOST_INSTALL_DIR)

if(NOT EXISTS ${BOOST_INSTALL_DIR})
  if( WIN32 )
    if(CMAKE_CL_64)
      set(BOOST_ZIP_FILENAME "boost_1_55_0_msvc2013_64_static.zip")
      set(BOOST_ZIP_EXPECTED_MD5 "3ff90a94d025f139e290296ed0b41263")
    else()
      set(BOOST_ZIP_FILENAME "boost_1_55_0_msvc2013_32_static.zip")
      set(BOOST_ZIP_EXPECTED_MD5 "4ebc2a84ef13ef1853fcf3e6c27fc10c")
    endif()
  elseif( APPLE )
      set(BOOST_ZIP_FILENAME "boost_1_55_0_osx.tar.gz")
      set(BOOST_ZIP_EXPECTED_MD5 "9e4c01a3f5c63e3bbf5cc76021eceb75")
  elseif(EXISTS "/etc/redhat-release")
    set(BOOST_ZIP_FILENAME "boost_1_55_0_redhat.tar.gz")
    set(BOOST_ZIP_EXPECTED_MD5 "1517315d5b07fe5e1bdb135ecdc0213d")
  else()
    find_program(LSB_RELEASE lsb_release)
    execute_process(COMMAND ${LSB_RELEASE} -r
      OUTPUT_VARIABLE LSB_RELEASE_ID_SHORT
      OUTPUT_STRIP_TRAILING_WHITESPACE
    )
	#message("LSB_RELEASE_ID_SHORT = ${LSB_RELEASE_ID_SHORT}")
	if(LSB_RELEASE_ID_SHORT MATCHES "16.04")
	  # ubuntu 16.04
      set(BOOST_ZIP_FILENAME "boost_1_55_0_xenial.tar.gz")
      set(BOOST_ZIP_EXPECTED_MD5 "45bf97fbdcde070aca3cc6181cd69362")
	else()
	  # ubuntu 14.04
      set(BOOST_ZIP_FILENAME "boost_1_55_0_linux.tar.gz")
      set(BOOST_ZIP_EXPECTED_MD5 "6b206a76268cd1a98d00d4aafb897ac9")
	endif()
  endif()

  set(BOOST_ZIP_LOCAL_PATH "${CMAKE_BINARY_DIR}/${BOOST_ZIP_FILENAME}")
  if(EXISTS "${BOOST_ZIP_LOCAL_PATH}")
    file(MD5 "${BOOST_ZIP_LOCAL_PATH}" BOOST_ZIP_MD5)
  endif()

  if( NOT BOOST_ZIP_MD5 STREQUAL BOOST_ZIP_EXPECTED_MD5)
    message(STATUS "Downloading Boost: ${BOOST_ZIP_FILENAME}")
    file(DOWNLOAD "http://openstudio-resources.s3.amazonaws.com/dependencies/${BOOST_ZIP_FILENAME}"
      ${BOOST_ZIP_LOCAL_PATH}
      INACTIVITY_TIMEOUT 120
      SHOW_PROGRESS
      EXPECTED_MD5 ${BOOST_ZIP_EXPECTED_MD5})
      execute_process(COMMAND ${CMAKE_COMMAND} -E tar xfz ${BOOST_ZIP_LOCAL_PATH} WORKING_DIRECTORY "${CMAKE_BINARY_DIR}")
  endif()
  set(BOOST_INSTALL_DIR "${CMAKE_BINARY_DIR}/Boost-install/")
endif()
set(BOOST_ROOT "${BOOST_INSTALL_DIR}")
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost 1.55.0 REQUIRED COMPONENTS filesystem regex program_options system thread date_time log)
include_directories(SYSTEM ${Boost_INCLUDE_DIR})
mark_as_advanced(
  Boost_DIR
  BOOST_THREAD_LIBRARY
)

# require these run time checks
add_definitions(-DBOOST_UBLAS_CHECK_ENABLE)
add_definitions(-DBOOST_UBLAS_TYPE_CHECK)
add_definitions(-DBOOST_UBLAS_NO_STD_CERR)

if(UNIX AND NOT APPLE)
  add_definitions(-DBOOST_NO_CXX11_SCOPED_ENUMS)
endif()

# use chrono as a header only library
add_definitions(-DBOOST_CHRONO_HEADER_ONLY)

# do not autolink with serialization
add_definitions(-DBOOST_SERIALIZATION_NO_LIB)

# SQLite definitions, used in sqlite and litesql
add_definitions(-DSQLITE_THREADSAFE=1) # 1 is default, serial access

# Qt
set(QT_INSTALL_DIR "" CACHE PATH "Path to Qt Install")
mark_as_advanced(QT_INSTALL_DIR)

if(NOT EXISTS ${QT_INSTALL_DIR})
  # download qt
  if( WIN32 )
    if(CMAKE_CL_64)
      set(QT_ZIP_FILENAME "qt_5_6_msvc2013_64_shared_temp.zip")
      set(QT_ZIP_EXPECTED_MD5 "2ac0ed2909710350f1c6acd5f38a8b33")
    else()
      set(QT_ZIP_FILENAME "qt_5_6_msvc2013_32_shared_temp.zip")
      set(QT_ZIP_EXPECTED_MD5 "25abd383e7db2dfd971b4a4ef145002b")
    endif()
  elseif( APPLE )
      set(QT_ZIP_FILENAME "qt_5_6_osx_shared.tar.gz")
      set(QT_ZIP_EXPECTED_MD5 "326887393d55b5da97a1be9764fb0d1b")
  elseif(EXISTS "/etc/redhat-release")
    set(QT_ZIP_FILENAME "qt_5_6_redhat_shared.tar.gz")
    set(QT_ZIP_EXPECTED_MD5 "12576637037b8d5428e2d63a5e298131")
  else()
    set(QT_ZIP_FILENAME "qt_5_6_linux_shared.tar.gz")
    set(QT_ZIP_EXPECTED_MD5 "3322cce1806ef134391d94a73adf3f15")
  endif()

  set(QT_ZIP_LOCAL_PATH "${CMAKE_BINARY_DIR}/${QT_ZIP_FILENAME}")
  if(EXISTS "${QT_ZIP_LOCAL_PATH}")
    file(MD5 "${QT_ZIP_LOCAL_PATH}" QT_ZIP_MD5)
  endif()

  if( NOT QT_ZIP_MD5 STREQUAL QT_ZIP_EXPECTED_MD5)
    message(STATUS "Downloading Qt: ${QT_ZIP_FILENAME}")
    file(DOWNLOAD "http://openstudio-resources.s3.amazonaws.com/dependencies/${QT_ZIP_FILENAME}"
      ${QT_ZIP_LOCAL_PATH}
      INACTIVITY_TIMEOUT 120
      SHOW_PROGRESS
      EXPECTED_MD5 ${QT_ZIP_EXPECTED_MD5})
      execute_process(COMMAND ${CMAKE_COMMAND} -E tar xfz ${QT_ZIP_LOCAL_PATH} WORKING_DIRECTORY "${CMAKE_BINARY_DIR}")
  endif()

  set(QT_INSTALL_DIR "${CMAKE_BINARY_DIR}/Qt-install/")
endif()

find_package(Qt5Core 5.6.1 REQUIRED PATHS ${QT_INSTALL_DIR} NO_DEFAULT_PATH)
find_package(Qt5Widgets 5.6.1 REQUIRED PATHS ${QT_INSTALL_DIR} NO_DEFAULT_PATH)
find_package(Qt5Sql 5.6.1 REQUIRED PATHS ${QT_INSTALL_DIR} NO_DEFAULT_PATH)
find_package(Qt5Network 5.6.1 REQUIRED PATHS ${QT_INSTALL_DIR} NO_DEFAULT_PATH)
find_package(Qt5Xml 5.6.1 REQUIRED PATHS ${QT_INSTALL_DIR} NO_DEFAULT_PATH)
find_package(Qt5Concurrent 5.6.1 REQUIRED PATHS ${QT_INSTALL_DIR} NO_DEFAULT_PATH)
find_package(Qt5PrintSupport 5.6.1 REQUIRED PATHS ${QT_INSTALL_DIR} NO_DEFAULT_PATH)
find_package(Qt5Gui 5.6.1 REQUIRED PATHS ${QT_INSTALL_DIR} NO_DEFAULT_PATH)
find_package(Qt5Quick 5.6.1 REQUIRED PATHS ${QT_INSTALL_DIR} NO_DEFAULT_PATH)
find_package(Qt5Qml 5.6.1 REQUIRED PATHS ${QT_INSTALL_DIR} NO_DEFAULT_PATH)
find_package(Qt5WebChannel 5.6.1 REQUIRED PATHS ${QT_INSTALL_DIR} NO_DEFAULT_PATH)
find_package(Qt5Positioning 5.6.1 REQUIRED PATHS ${QT_INSTALL_DIR} NO_DEFAULT_PATH)

if(BUILD_OS_APP)
  find_package(Qt5WebEngine 5.6.1 REQUIRED PATHS ${QT_INSTALL_DIR} NO_DEFAULT_PATH)
  find_package(Qt5WebEngineWidgets 5.6.1 REQUIRED PATHS ${QT_INSTALL_DIR} NO_DEFAULT_PATH)
  #find_package(Qt53DCore 5.6.1 REQUIRED PATHS ${QT_INSTALL_DIR} NO_DEFAULT_PATH)
  #find_package(Qt53DInput 5.6.1 REQUIRED PATHS ${QT_INSTALL_DIR} NO_DEFAULT_PATH)
  #find_package(Qt53DRender 5.6.1 REQUIRED PATHS ${QT_INSTALL_DIR} NO_DEFAULT_PATH)

  if( APPLE )
    set(QtWebEngineProcess "${QT_INSTALL_DIR}/lib/QtWebEngineCore.framework/Helpers/QtWebEngineProcess.app" CACHE PATH "Path to the QtWebEngineProcess")
  else()
    find_program(QtWebEngineProcess NAMES QtWebEngineProcess PATHS "${QT_INSTALL_DIR}/bin/" "${QT_INSTALL_DIR}/libexec/" "${QT_INSTALL_DIR}/lib/QtWebEngineCore.framework/Helpers" NO_DEFAULT_PATH)
  endif()
  find_file(icudtl NAMES icudtl.dat PATHS "${QT_INSTALL_DIR}/resources/" "${QT_INSTALL_DIR}/lib/QtWebEngineCore.framework/Resources" NO_DEFAULT_PATH)
  find_file(qweb_resources NAMES qtwebengine_resources.pak PATHS "${QT_INSTALL_DIR}/resources/" "${QT_INSTALL_DIR}/lib/QtWebEngineCore.framework/Resources" NO_DEFAULT_PATH)
  find_file(qweb_resources_100 NAMES qtwebengine_resources_100p.pak PATHS "${QT_INSTALL_DIR}/resources/" "${QT_INSTALL_DIR}/lib/QtWebEngineCore.framework/Resources" NO_DEFAULT_PATH)
  find_file(qweb_resources_200 NAMES qtwebengine_resources_200p.pak PATHS "${QT_INSTALL_DIR}/resources/" "${QT_INSTALL_DIR}/lib/QtWebEngineCore.framework/Resources" NO_DEFAULT_PATH)

  list(APPEND QT_WEB_LIBS Qt5::Core)
  list(APPEND QT_WEB_LIBS Qt5::Gui)
  list(APPEND QT_WEB_LIBS Qt5::WebEngine)
  list(APPEND QT_WEB_LIBS Qt5::WebEngineCore)
  list(APPEND QT_WEB_LIBS Qt5::WebEngineWidgets)
  #list(APPEND QT_WEB_LIBS Qt5::3DCore)
  #list(APPEND QT_WEB_LIBS Qt5::3DInput)
  #list(APPEND QT_WEB_LIBS Qt5::3DRender)
  set_target_properties( ${QT_WEB_LIBS} PROPERTIES INTERFACE_LINK_LIBRARIES "" )

  if(NOT APPLE)

    find_package(Qt5WebChannel 5.6.1 REQUIRED PATHS ${QT_INSTALL_DIR} NO_DEFAULT_PATH)
    list(APPEND QT_WEB_LIBS Qt5::WebChannel)

    find_package(Qt5Quick 5.6.1 REQUIRED PATHS ${QT_INSTALL_DIR} NO_DEFAULT_PATH)
    list(APPEND QT_WEB_LIBS Qt5::Quick)

    find_package(Qt5Qml 5.6.1 REQUIRED PATHS ${QT_INSTALL_DIR} NO_DEFAULT_PATH)
    list(APPEND QT_WEB_LIBS Qt5::Qml)

    find_package(Qt5Positioning 5.6.1 REQUIRED PATHS ${QT_INSTALL_DIR} NO_DEFAULT_PATH)
    list(APPEND QT_WEB_LIBS Qt5::Positioning)

    if(UNIX)
      find_package(Qt5DBus 5.6.1 REQUIRED PATHS ${QT_INSTALL_DIR} NO_DEFAULT_PATH)
      list(APPEND QT_WEB_LIBS Qt5::DBus)

      # there does not appear to be a normal qt finder for this
      find_library(Qt5XcbQpaLib NAMES libQt5XcbQpa.so.5 PATHS ${QT_INSTALL_DIR}/lib NO_DEFAULT_PATH)
    endif()
  endif()
endif()

list(APPEND QT_LIBS Qt5::Core)
list(APPEND QT_LIBS Qt5::Widgets)
list(APPEND QT_LIBS Qt5::Sql)
list(APPEND QT_LIBS Qt5::Network)
list(APPEND QT_LIBS Qt5::Xml)
list(APPEND QT_LIBS Qt5::Concurrent)
list(APPEND QT_LIBS Qt5::PrintSupport)
list(APPEND QT_LIBS Qt5::Gui)

if( WIN32 )
  find_package(Qt5WinExtras 5.6.1 REQUIRED PATHS ${QT_INSTALL_DIR} NO_DEFAULT_PATH)
  list(APPEND QT_LIBS Qt5::WinExtras)

  find_library(QT_MAIN_LIB NAMES qtmain PATHS "${QT_INSTALL_DIR}/lib" NO_DEFAULT_PATH)
  find_library(QT_MAIN_LIB_D NAMES qtmaind PATHS "${QT_INSTALL_DIR}/lib" NO_DEFAULT_PATH)
endif()

list(APPEND QT_INCLUDES ${Qt5Core_INCLUDE_DIRS})
list(APPEND QT_INCLUDES ${Qt5Concurrent_INCLUDE_DIRS})
list(APPEND QT_INCLUDES ${Qt5Widgets_INCLUDE_DIRS})
list(APPEND QT_INCLUDES ${Qt5Xml_INCLUDE_DIRS})
list(APPEND QT_INCLUDES ${Qt5Sql_INCLUDE_DIRS})
list(APPEND QT_INCLUDES ${Qt5Network_INCLUDE_DIRS})
list(APPEND QT_INCLUDES ${Qt5Gui_INCLUDE_DIRS})
list(APPEND QT_INCLUDES ${Qt5PrintSupport_INCLUDE_DIRS})

if( UNIX )
  list(APPEND QT_INCLUDES ${Qt5XcbQpa_INCLUDE_DIRS})
endif()

set(CMAKE_AUTOMOC OFF)

# download qt static
if( WIN32 )
  if(CMAKE_CL_64)
    set(QT_STATIC_ZIP_FILENAME "qt_5_6_msvc2013_64_static.zip")
    set(QT_STATIC_ZIP_EXPECTED_MD5 "e3ea40ee280fc209e50d9aaf4762ea53")
  else()
    set(QT_STATIC_ZIP_FILENAME "qt_5_6_msvc2013_32_static_temp.zip")
    set(QT_STATIC_ZIP_EXPECTED_MD5 "d03d2fae40cba06f8b4887c8dce64a07")
  endif()
elseif( APPLE )
  set(QT_STATIC_ZIP_FILENAME "qt_5_6_1_osx_static_openssl_linked_1.0.2q.tar.gz")
  set(QT_STATIC_ZIP_EXPECTED_MD5 "3fd24225db9a26cef48cca8d87859d0f")
elseif(EXISTS "/etc/redhat-release")
  set(QT_STATIC_ZIP_FILENAME "qt_5_6_redhat_64_static.tar.gz")
  set(QT_STATIC_ZIP_EXPECTED_MD5 "52159660cbdd95a41ab84a2c43140f11")
else()
  set(QT_STATIC_ZIP_FILENAME "qt_5_6_linux_64_static.tar.gz")
  set(QT_STATIC_ZIP_EXPECTED_MD5 "77e24a44ae6c194ce1f2c3cf4aaa8a48")
endif()

set(QT_STATIC_ZIP_LOCAL_PATH "${CMAKE_BINARY_DIR}/${QT_STATIC_ZIP_FILENAME}")
if(EXISTS "${QT_STATIC_ZIP_LOCAL_PATH}")
  file(MD5 "${QT_STATIC_ZIP_LOCAL_PATH}" QT_STATIC_ZIP_MD5)
endif()

if( NOT QT_STATIC_ZIP_MD5 STREQUAL QT_STATIC_ZIP_EXPECTED_MD5)
  message(STATUS "Downloading Qt Static: ${QT_STATIC_ZIP_FILENAME}")
  file(DOWNLOAD "http://openstudio-resources.s3.amazonaws.com/dependencies/${QT_STATIC_ZIP_FILENAME}"
    ${QT_STATIC_ZIP_LOCAL_PATH}
    INACTIVITY_TIMEOUT 120
    SHOW_PROGRESS
    EXPECTED_MD5 ${QT_STATIC_ZIP_EXPECTED_MD5})
    execute_process(COMMAND ${CMAKE_COMMAND} -E tar xfz ${QT_STATIC_ZIP_LOCAL_PATH} WORKING_DIRECTORY "${CMAKE_BINARY_DIR}")
endif()

set(QT_STATIC_INSTALL_DIR "${CMAKE_BINARY_DIR}/Qt-static-install/" CACHE PATH "QT Static Path")

set(QT_DEFS -DQT_STATIC)

list(APPEND QT_STATIC_INCLUDES "${QT_STATIC_INSTALL_DIR}/include/")
list(APPEND QT_STATIC_INCLUDES "${QT_STATIC_INSTALL_DIR}/include/QtCore")
list(APPEND QT_STATIC_INCLUDES "${QT_STATIC_INSTALL_DIR}/include/QtWidgets")
list(APPEND QT_STATIC_INCLUDES "${QT_STATIC_INSTALL_DIR}/include/QtXml")
list(APPEND QT_STATIC_INCLUDES "${QT_STATIC_INSTALL_DIR}/include/QtSql")
list(APPEND QT_STATIC_INCLUDES "${QT_STATIC_INSTALL_DIR}/include/QtNetwork")
list(APPEND QT_STATIC_INCLUDES "${QT_STATIC_INSTALL_DIR}/include/QtGui")
list(APPEND QT_STATIC_INCLUDES "${QT_STATIC_INSTALL_DIR}/include/QtGui/5.6.1/QtGui")

FIND_QT_STATIC_LIB("QtCore;Qt5Core" "${QT_STATIC_INSTALL_DIR}/lib/")
FIND_QT_STATIC_LIB("QtWidgets;Qt5Widgets" "${QT_STATIC_INSTALL_DIR}/lib/")
FIND_QT_STATIC_LIB("QtXml;Qt5Xml" "${QT_STATIC_INSTALL_DIR}/lib/")
FIND_QT_STATIC_LIB("QtSql;Qt5Sql" "${QT_STATIC_INSTALL_DIR}/lib/")
FIND_QT_STATIC_LIB("QtNetwork;Qt5Network" "${QT_STATIC_INSTALL_DIR}/lib/")
FIND_QT_STATIC_LIB("Qt5Gui;Qt5Gui" "${QT_STATIC_INSTALL_DIR}/lib/")
FIND_QT_STATIC_LIB("QtPrintSupport;Qt5PrintSupport" "${QT_STATIC_INSTALL_DIR}/lib/")

if( WIN32 )
  FIND_QT_STATIC_LIB("Qt5WinExtras" "${QT_STATIC_INSTALL_DIR}/lib/")
  FIND_QT_STATIC_LIB("qtmain" "${QT_STATIC_INSTALL_DIR}/lib/")
endif()

if( UNIX )
  FIND_QT_STATIC_LIB("QtDBus;Qt5DBus" "${QT_STATIC_INSTALL_DIR}/lib/")

  find_package(OpenGL REQUIRED)
  list(APPEND QT_STATIC_LIBS "${OPENGL_LIBRARIES}")
  list(APPEND QT_STATIC_LIBS_D "${OPENGL_LIBRARIES}")
endif()

if( UNIX AND NOT APPLE )
  # http://doc.qt.io/qt-5/linux-requirements.html
  # DLM: the order of linking for static archives matter because symbols will only be included if they are needed by previous libraries
  # in some cases we may need to relink libraries a second time
  #list(APPEND QT_STATIC_LIBS "${QT_STATIC_INSTALL_DIR}/plugins/platforms/libqxcb.a")
  #list(APPEND QT_STATIC_LIBS "${QT_STATIC_INSTALL_DIR}/plugins/sqldrivers/libqsqlite.a")

  FIND_QT_STATIC_LIB("qxcb" "${QT_STATIC_INSTALL_DIR}/plugins/platforms/")
  FIND_QT_STATIC_LIB("qsqlite" "${QT_STATIC_INSTALL_DIR}/plugins/sqldrivers/")

  FIND_QT_STATIC_LIB("qtpcre" "${QT_STATIC_INSTALL_DIR}/lib/")
  FIND_QT_STATIC_LIB("qtharfbuzzng" "${QT_STATIC_INSTALL_DIR}/lib/")
  FIND_QT_STATIC_LIB("Qt5XcbQpa" "${QT_STATIC_INSTALL_DIR}/lib/")
  FIND_QT_STATIC_LIB("Qt5PlatformSupport" "${QT_STATIC_INSTALL_DIR}/lib/")
  FIND_QT_STATIC_LIB("Qt5Sql" "${QT_STATIC_INSTALL_DIR}/lib/")
  FIND_QT_STATIC_LIB("Qt5Gui" "${QT_STATIC_INSTALL_DIR}/lib/")
  FIND_QT_STATIC_LIB("Qt5DBus" "${QT_STATIC_INSTALL_DIR}/lib/")
  FIND_QT_STATIC_LIB("Qt5Core" "${QT_STATIC_INSTALL_DIR}/lib/")
  FIND_QT_STATIC_LIB("xcb-static" "${QT_STATIC_INSTALL_DIR}/lib/")

  FIND_QT_STATIC_LIB("expat" "*")

  FIND_QT_STATIC_LIB("png" "*")
  FIND_QT_STATIC_LIB("libjpeg;libjpeg.so;libjpeg.so.8" "*")
  FIND_QT_STATIC_LIB("freetype" "*")

  FIND_QT_STATIC_LIB("libdbus-1.so" "*")
  FIND_QT_STATIC_LIB("dbus-glib-1;libdbus-glib-1.so.2" "*")

  FIND_QT_STATIC_LIB("glib-2.0" "*")
  FIND_QT_STATIC_LIB("X11.so" "*")
  FIND_QT_STATIC_LIB("xcb.so" "*")
  FIND_QT_STATIC_LIB("X11-xcb.so" "*")
  FIND_QT_STATIC_LIB("Xext.so" "*")
  FIND_QT_STATIC_LIB("crypto.so" "*")

  if(EXISTS "/etc/redhat-release")
    FIND_QT_STATIC_LIB("rt" "*")

    FIND_QT_STATIC_LIB("gthread-2.0" "*")
    FIND_QT_STATIC_LIB("dbus-glib-1;libdbus-glib-1.so.2" "*")
    FIND_QT_STATIC_LIB("gobject-2.0" "*")
    FIND_QT_STATIC_LIB("db" "*")
    FIND_QT_STATIC_LIB("icui18n" "*")
    FIND_QT_STATIC_LIB("icuuc" "*")
    FIND_QT_STATIC_LIB("z.so" "*")
  else()
    FIND_QT_STATIC_LIB("fontconfig.so" "*")
    FIND_QT_STATIC_LIB("SM.so" "*")
    FIND_QT_STATIC_LIB("ICE.so" "*")
    FIND_QT_STATIC_LIB("Xi.so" "*")
    FIND_QT_STATIC_LIB("z.so" "*")
  endif()
endif()

if( APPLE )
  # DLM: the order of linking for static archives matter because symbols will only be included if they are needed by previous libraries
  # in some cases we may need to relink libraries a second time

  FIND_QT_STATIC_LIB("CoreFoundation" "*")
  FIND_QT_STATIC_LIB("CFNetwork" "*")
  FIND_QT_STATIC_LIB("CoreGraphics" "*")
  FIND_QT_STATIC_LIB("Carbon" "*")
  FIND_QT_STATIC_LIB("AppKit" "*")
  FIND_QT_STATIC_LIB("SystemConfiguration" "*")
  FIND_QT_STATIC_LIB("Security" "*")

  FIND_QT_STATIC_LIB("qtpcre" "${QT_STATIC_INSTALL_DIR}/lib/")
  FIND_QT_STATIC_LIB("qcocoa" "${QT_STATIC_INSTALL_DIR}/plugins/platforms/")
  FIND_QT_STATIC_LIB("qsqlite" "${QT_STATIC_INSTALL_DIR}/plugins/sqldrivers/")
  FIND_QT_STATIC_LIB("qtfreetype" "${QT_STATIC_INSTALL_DIR}/lib/")
  FIND_QT_STATIC_LIB("poly2tri" "${QT_STATIC_INSTALL_DIR}/lib/")
  FIND_QT_STATIC_LIB("qtharfbuzzng" "${QT_STATIC_INSTALL_DIR}/lib/")
  FIND_QT_STATIC_LIB("Qt5PlatformSupport" "${QT_STATIC_INSTALL_DIR}/lib/")
  FIND_QT_STATIC_LIB("qtpcre" "${QT_STATIC_INSTALL_DIR}/lib/")

  FIND_QT_STATIC_LIB("CoreFoundation" "*")
  FIND_QT_STATIC_LIB("CFNetwork" "*")
  FIND_QT_STATIC_LIB("CoreGraphics" "*")
  FIND_QT_STATIC_LIB("Carbon" "*")
  FIND_QT_STATIC_LIB("AppKit" "*")
  FIND_QT_STATIC_LIB("SystemConfiguration" "*")
  FIND_QT_STATIC_LIB("Security" "*")
  FIND_QT_STATIC_LIB("IOKit" "*")
  FIND_QT_STATIC_LIB("cups" "*")
endif()

if( WIN32 )
  FIND_QT_STATIC_LIB("Qt5PlatformSupport" "${QT_STATIC_INSTALL_DIR}/lib/")
  FIND_QT_STATIC_LIB("qtfreetype" "${QT_STATIC_INSTALL_DIR}/lib/")
  FIND_QT_STATIC_LIB("qtharfbuzzng" "${QT_STATIC_INSTALL_DIR}/lib/")
  FIND_QT_STATIC_LIB("qtpcre" "${QT_STATIC_INSTALL_DIR}/lib/")
  FIND_QT_STATIC_LIB("qsqlite" "${QT_STATIC_INSTALL_DIR}/plugins/sqldrivers/")
  FIND_QT_STATIC_LIB("qwindows" "${QT_STATIC_INSTALL_DIR}/plugins/platforms/")

  FIND_QT_STATIC_LIB("Crypt32" "*")
  FIND_QT_STATIC_LIB("imm32" "*")
  FIND_QT_STATIC_LIB("winmm" "*")
  FIND_QT_STATIC_LIB("Gdi32" "*")
  FIND_QT_STATIC_LIB("wsock32" "*")
  FIND_QT_STATIC_LIB("ws2_32" "*")
  FIND_QT_STATIC_LIB("iphlpapi" "*")
  FIND_QT_STATIC_LIB("dbghelp" "*")
  FIND_QT_STATIC_LIB("Shlwapi" "*")

  if(BUILD_WITH_OPENSSL)
    FIND_QT_STATIC_LIB("ssleay32MT" "${OPENSSL_ROOT_DIR}/lib/VC/static")
    FIND_QT_STATIC_LIB("libeay32MT" "${OPENSSL_ROOT_DIR}/lib/VC/static")
  endif()
endif()

###############################################################################
## jsoncpp
add_subdirectory(src/jsoncpp)
###############################################################################

################################################################################
## sqlite
add_subdirectory(src/sqlite)
################################################################################

###############################################################################
# Add to include path

# Project source directory
include_directories("${CMAKE_HOME_DIRECTORY}/src/")

# For QWinWidget
include_directories("${CMAKE_HOME_DIRECTORY}/src/qtwinmigrate")

# Project binary directory
include_directories("${CMAKE_BINARY_DIR}/src/")
###############################################################################

###############################################################################
# Add project sub directories

if(MSVC)
  # treat warnings as errors
  add_definitions(/WX)
endif()

include("embedded/EmbedFiles.cmake")
add_subdirectory("embedded")
if( BUILD_PAT )
  add_subdirectory("pat")
endif()

# Each subfolder of src is a major sub-project
set(project_directories
  generateiddfactory
  utilities
  model
  energyplus
  radiance
  gbxml
  airflow
  isomodel
  osversion
  measure
  sdd
  cli
  model_editor
  install_utility
)
if(WIN32)
  list(APPEND project_directories "qtwinmigrate")
endif()

if(BUILD_OS_APP)
  list(APPEND project_directories "bimserver")
  list(APPEND project_directories "openstudio_lib")
  list(APPEND project_directories "openstudio_app")
  list(APPEND project_directories "qwt")
endif()

# Include the resources, should happen after find EnergyPlus
add_subdirectory(resources)

add_subdirectory(ruby)

if(BUILD_OS_APP)
  add_subdirectory(sketchup_plugin)
endif()

foreach(D ${project_directories})
  add_subdirectory(src/${D})
endforeach()

# csharp, after loading projects
if(BUILD_CSHARP_BINDINGS)
  add_subdirectory(csharp)
endif()

# java, after loading projects
if(BUILD_JAVA_BINDINGS)
  add_subdirectory(java)
endif()

# python, after loading projects
if(BUILD_PYTHON_BINDINGS)
  add_subdirectory(python)
endif()

# v8-node
if(BUILD_NODE_MODULES)
  add_subdirectory(v8-node)
endif()

###############################################################################

###############################################################################
# Targets that combine all bindings targets
add_custom_target(ALL_RUBY_BINDINGS)
add_dependencies(ALL_RUBY_BINDINGS ${ALL_RUBY_BINDING_TARGETS})

if(BUILD_PYTHON_BINDINGS)
  add_custom_target(ALL_PYTHON_BINDINGS)
  #add_dependencies(ALL_PYTHON_BINDINGS python_sdk ${ALL_PYTHON_BINDING_TARGETS})
endif()


if(BUILD_CSHARP_BINDINGS)
  add_custom_target(ALL_CSHARP_BINDINGS)
  add_dependencies(ALL_CSHARP_BINDINGS csharp_sdk ${ALL_CSHARP_BINDING_TARGETS})
endif()

if(BUILD_JAVA_BINDINGS)
  add_custom_target(ALL_JAVA_BINDINGS)
  #add_dependencies(ALL_JAVA_BINDINGS java_sdk ${ALL_JAVA_BINDING_TARGETS})
endif()


if(BUILD_V8_BINDINGS)
  add_custom_target(ALL_V8_BINDINGS)
  add_dependencies(ALL_V8_BINDINGS ${ALL_V8_BINDING_TARGETS})
endif()

set(MAXIMIZE_CPU_USAGE OFF CACHE BOOL "Attempt to fully load the CPU during builds")
mark_as_advanced(MAXIMIZE_CPU_USAGE)


###############################################################################
# Targets that combine all documentation targets

# Doxygen
if(BUILD_DOCUMENTATION)
  # need doxygen
  find_package(Doxygen REQUIRED)

  # if have dot you have more options
  #if(DOXYGEN_DOT_FOUND)

  #endif()

  configure_file(${CMAKE_SOURCE_DIR}/doc/index.html ${CMAKE_BINARY_DIR}/doc/index.html COPYONLY)

  set(doxygen_directories ${project_directories})
  # Add the idd files to the documentation list as a separate entry, along with shared_gui_components
  list(APPEND doxygen_directories utilities/idd shared_gui_components)

  # Remove directories we don't want documentation generated for
  list(REMOVE_ITEM doxygen_directories generateiddfactory)

  add_custom_target(openstudio_doc_resources
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/doc/css" "css"
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/doc/fonts" "fonts"
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/doc/js" "js"
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/doc
  )

  foreach(D ${doxygen_directories})
    string(REPLACE "/" "_" DOC_NAME ${D})

    set(DOXY_INPUT_DIR "\"${CMAKE_SOURCE_DIR}/src/${D}\"")
    set(DOXY_ADDITIONAL_DIR "")
    set(DOXY_EXCLUDE_DIR "")
    set(DOXY_TAGFILES "")


    # Specific settings
    if(${D} STREQUAL "utilities/idd")
      # We want to build the idd documentation separately because of the generated files
      set(DOXY_ADDITIONAL_DIR "\"${CMAKE_BINARY_DIR}/src/${D}\"")
    elseif(${D} STREQUAL "utilities")
      # Exclude the idd directory when building the utilities documentation
      set(DOXY_EXCLUDE_DIR "\"${CMAKE_SOURCE_DIR}/src/utilities/idd\"")
      set(DOXY_TAGFILES "utilities_idd/utilities_idd.tag=../../utilities_idd/html")
    elseif(${D} STREQUAL "model")
      set(DOXY_TAGFILES "utilities_idd/utilities_idd.tag=../../utilities_idd/html utilities/utilities.tag=../../utilities/html")
    endif()
    configure_file(${CMAKE_SOURCE_DIR}/Doxyfile.in ${CMAKE_BINARY_DIR}/doc/${DOC_NAME}.cfg)

    add_custom_target(openstudio_${DOC_NAME}_doc
      COMMAND ${CMAKE_COMMAND} -E remove_directory "${DOC_NAME}"
      COMMAND ${CMAKE_COMMAND} -E make_directory "${DOC_NAME}"
      COMMAND ${DOXYGEN_EXECUTABLE} ${DOC_NAME}.cfg
      COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/doc/search" "${DOC_NAME}/html/search"
      COMMAND ${PATCH_EXE} "${DOC_NAME}/html/search/search.js" < "${CMAKE_SOURCE_DIR}/doc/patch/search.js.patch"
      DEPENDS openstudio_doc_resources
      WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/doc
    )

    set(ALL_CPP_DOC_TARGETS ${ALL_CPP_DOC_TARGETS}
      openstudio_${DOC_NAME}_doc
    )

  endforeach()

  # Set up dependencies after all targets have been created
  add_dependencies(openstudio_utilities
    CreateEmbeddedSource
  )
  add_dependencies(openstudio_utilities_doc
    openstudio_utilities_idd_doc
  )
  add_dependencies(openstudio_model_doc
    openstudio_utilities_doc
    openstudio_utilities_idd_doc
  )

endif()


if(BUILD_DOCUMENTATION)

  add_custom_target(ALL_DOXYGEN)
  add_dependencies(ALL_DOXYGEN ${ALL_CPP_DOC_TARGETS})

  add_custom_target(ALL_RDOC)
  add_dependencies(ALL_RDOC ${ALL_RDOC_TARGETS})

  # Zip documentation
  if(WIN32)
    if(NOT EXISTS "${CMAKE_BINARY_DIR}/7za.exe")
      message(STATUS "Downloading 7za.zip")
      file(DOWNLOAD "http://openstudio-resources.s3.amazonaws.com/dependencies/7za.zip" "${CMAKE_BINARY_DIR}/7za.zip" TIMEOUT 120 INACTIVITY_TIMEOUT 120 EXPECTED_MD5 860077d3f314e939abab680218a3febe)
      execute_process(COMMAND ${CMAKE_COMMAND} -E tar xfz "${CMAKE_BINARY_DIR}/7za.zip" WORKING_DIRECTORY "${CMAKE_BINARY_DIR}")
    endif()
    add_custom_command(TARGET ALL_DOXYGEN
      POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E remove OpenStudio-${OPENSTUDIO_VERSION}-doc.zip
      COMMAND ${CMAKE_BINARY_DIR}/7za a -mmt -mx9 -tzip OpenStudio-${OPENSTUDIO_VERSION}-doc.zip * -r -x!*.cfg -x!*.tag -x!*.map -x!*.md5
      COMMAND ${CMAKE_BINARY_DIR}/7za d OpenStudio-${OPENSTUDIO_VERSION}-doc.zip index.html
      WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/doc
    )
    add_custom_command(TARGET ALL_RDOC
      POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E remove OpenStudio-${OPENSTUDIO_VERSION}-rdoc.zip
      COMMAND ${CMAKE_BINARY_DIR}/7za a -mmt -mx9 -tzip OpenStudio-${OPENSTUDIO_VERSION}-rdoc.zip * -r
      WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/rdoc
    )
  endif()

endif()


###############################################################################
# Export targets and generate OpenStudioCoreConfig.cmake

set(TARGETS_FILE ${CMAKE_BINARY_DIR}/OpenStudioTargets.cmake)

set(all_lib_targets
  openstudio_osversion
  openstudio_model
  openstudio_utilities
  openstudio_energyplus
  openstudio_measure
  openstudio_radiance
  openstudio_gbxml
  openstudio_isomodel
  openstudio_sdd
  openstudio_airflow
  miniziplib
  sqlite
  jsoncpp
  ${ALL_RUBY_BINDING_TARGETS}
)

if(WIN32)
  list(APPEND all_lib_targets "qtwinmigrate")
endif()

if(BUILD_OS_APP)
  list(APPEND all_lib_targets "openstudio_modeleditor")
  list(APPEND all_lib_targets "openstudio_bimserver")
  list(APPEND all_lib_targets "openstudio_lib")
endif()

set(all_exe_targets
  GenerateIddFactory
  #gbXMLToIdf
  openstudio
)

if(BUILD_OS_APP)
  list(APPEND all_exe_targets "OpenStudioApp")
endif()

if(UNIX AND NOT APPLE)
  foreach(targ ${all_lib_targets} ${all_exe_targets} ${ALL_TESTING_TARGETS})
    set_target_properties(${targ} PROPERTIES VERSION ${OPENSTUDIO_VERSION})
  endforeach()
endif()

configure_file("${CMAKE_SOURCE_DIR}/OpenStudioCoreConfig.cmake.in"
  "${CMAKE_BINARY_DIR}/OpenStudioCoreConfig.cmake" @ONLY)


if(UNIX AND NOT APPLE)
  set(examplesdir share/openstudio-${OPENSTUDIO_VERSION}/examples)
else()
  set(examplesdir Examples)
endif()

# Install additional Documents, such as release notes
install(FILES "${CMAKE_SOURCE_DIR}/../developer/doc/ReleaseNotes/OpenStudio_Release_Notes_2_8_0_20181012.pdf" DESTINATION .)
install(FILES "${CMAKE_SOURCE_DIR}/../LICENSE.md" DESTINATION .)
install(FILES "${CMAKE_SOURCE_DIR}/../CHANGELOG.md" DESTINATION .)
install(FILES "${CMAKE_SOURCE_DIR}/../copyright.txt" DESTINATION .)

###############################################################################

if(NOT UNIX)
  install(DIRECTORY "${CMAKE_BINARY_DIR}/strawberry-perl-5.16.2.1-32bit-portable-reduced/perl" DESTINATION ./Perl/ COMPONENT Radiance USE_SOURCE_PERMISSIONS )

  #set(CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS_SKIP ON)
  include(InstallRequiredSystemLibraries)
  if(CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS)
    install(FILES ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS} DESTINATION "bin")
    install(FILES ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS} DESTINATION "Ruby" COMPONENT "RubyAPI")
    install(FILES ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS} DESTINATION "CSharp/openstudio/" COMPONENT "CSharpAPI")
  endif()
endif()

set(CPACK_PACKAGE_VENDOR "National Renewable Energy Laboratory")
set(CPACK_PACKAGE_VERSION_MAJOR ${OpenStudio_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${OpenStudio_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${OpenStudio_VERSION_PATCH})
# Trusty (14.04) uses 3.0-0, all other including Xenial (16.04) use 3.0-0v5
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libwxgtk3.0-0v5 (>= 3.0.0) | libwxgtk3.0-0 (>=3.0.0)")
# Default the debian package name to include version to allow several versions to be installed concurrently instead of overwriting any existing one
set(CPACK_DEBIAN_PACKAGE_NAME "openstudio-${OpenStudio_VERSION}")
# CPACK_DEBIAN_PACKAGE_DESCRIPTION defaults to this one too. dpkg-deb -I xxx.deb will show this description
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "OpenStudio is a cross-platform collection of software tools to support whole building energy modeling using EnergyPlus and advanced daylight analysis using Radiance")
set(CPACK_DEBIAN_PACKAGE_HOMEPAGE "https://www.openstudio.net")

# The actual .deb file name on disk
set(CPACK_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}-${OPENSTUDIO_LONG_VERSION}-${CMAKE_SYSTEM_NAME}")
set(CPACK_PACKAGE_CONTACT "openstudio@nrel.gov")

if( APPLE )
  set( CPACK_IFW_TARGET_DIRECTORY /Applications/OpenStudio-${OpenStudio_VERSION}/ )
elseif( UNIX )
  # Default method doesn't use IFW but Deb, so this one is probably useless (but harmless)
  set( CPACK_IFW_TARGET_DIRECTORY /usr/local/openstudio-${OpenStudio_VERSION}/ )
  # These two will set the .deb install path correctly
  set( CPACK_SET_DESTDIR ON )
  set( CPACK_INSTALL_PREFIX /usr/local/openstudio-${OpenStudio_VERSION}/ )
  # Add a symlink to the CLI: /usr/local/bin/openstudio should point to /usr/local/openstudio-${OpenStudio_VERSION}/bin/openstudio
  # Add an arbitrary link (broken) at build/openstudio (if build/ is your build folder) that already points to the **future** /usr/local/openstudio-x.y.z/bin/openstudio
  execute_process(COMMAND ln -sf ${CPACK_INSTALL_PREFIX}bin/openstudio openstudio)
  # Plus a versioned one
  execute_process(COMMAND ln -sf ${CPACK_INSTALL_PREFIX}bin/openstudio openstudio-${OpenStudio_VERSION})
  # Have this link be installed with the .deb package in /usr/local/bin
  install(FILES ${CMAKE_BINARY_DIR}/openstudio DESTINATION /usr/local/bin COMPONENT CLI)
  install(FILES ${CMAKE_BINARY_DIR}/openstudio-${OpenStudio_VERSION} DESTINATION /usr/local/bin COMPONENT CLI)

  # Same for OpenStudioApp
  execute_process(COMMAND ln -sf ${CPACK_INSTALL_PREFIX}bin/OpenStudioApp OpenStudioApp)
  execute_process(COMMAND ln -sf ${CPACK_INSTALL_PREFIX}bin/OpenStudioApp OpenStudioApp-${OpenStudio_VERSION})

  # Have this link be installed with the .deb package in /usr/local/bin
  install(FILES ${CMAKE_BINARY_DIR}/OpenStudioApp DESTINATION /usr/local/bin COMPONENT CLI)
  install(FILES ${CMAKE_BINARY_DIR}/OpenStudioApp-${OpenStudio_VERSION} DESTINATION /usr/local/bin COMPONENT CLI)


  # Create a proper App with file associations

  # Install the .desktop manifest (allows the App to be seen in the Dash and adding to the dock, and to map it a mimetype)
  install(FILES "${CMAKE_SOURCE_DIR}/openstudioapp.desktop" DESTINATION /usr/share/applications)
  # Install the XML mime info
  install(FILES "${CMAKE_SOURCE_DIR}/x-openstudio.xml" DESTINATION /usr/share/mime/application)

  # Copy the icons
  # Cf. http://standards.freedesktop.org/icon-theme-spec/icon-theme-spec-latest.html#directory_layout
  foreach(_size 16 32 48 64 128 256)
    # Copy OS App icons
    install(
      FILES "${CMAKE_SOURCE_DIR}/icons/os_${_size}.png"
      DESTINATION "/usr/share/icons/hicolor/${_size}x${_size}/apps"
      RENAME openstudioapp.png
    )
    # Install OSM mimetypes icons
    install(
      FILES "${CMAKE_SOURCE_DIR}/icons/osm_${_size}.png"
      DESTINATION "/usr/share/icons/hicolor/${_size}x${_size}/mimetypes"
      RENAME application-x-openstudio.png
    )
  endforeach()

  # OS App has a 512 and 1024 too
  foreach(_size 512 1024)
    # Copy OS App icons
    install(
      FILES "${CMAKE_SOURCE_DIR}/icons/os_${_size}.png"
      DESTINATION "/usr/share/icons/hicolor/${_size}x${_size}/apps"
      RENAME openstudioapp.png
    )
  endforeach()

  ## try to set up the menu system
  #find_program(XDG-MIME_EXECUTABLE xdg-mime)
  #find_program(XDG-DESKTOP-MENU_EXECUTABLE xdg-desktop-menu)
  #find_program(UPDATE-MIME-DATABASE-EXECUTABLE update-mime-database)
  #find_program(GTK-UPDATE-ICON-CACHE-EXECUTABLE gtk-update-icon-cache)


  ## Install the Desktop Menu
  ## xdg-desktop-menu install --novendor /usr/share/applications/openstudioapp.desktop
  #INSTALL(CODE "execute_process(COMMAND ${XDG-DESKTOP-MENU_EXECUTABLE} install --novendor --mode system /usr/share/applications/openstudioapp.desktop)")

  ## Register the mime description: this places it in /usr/share/mime/packages/
  ## sudo xdg-mime install --novendor --mode system x-openstudio.xml
  #INSTALL(CODE "execute_process(COMMAND ${MIME_EXECUTABLE} install --novendor --mode system /usr/share/mime/application/x-openstudio.xml)")
  #INSTALL(CODE "execute_process(COMMAND echo COUCOU)")

  ## Register the application launcher with the registered mimetype
  ## sudo xdg-mime default openstudio.desktop application/x-openstudio
  #INSTALL(CODE "execute_process(COMMAND ${MIME_EXECUTABLE} default /usr/share/applications/openstudio.desktop application/x-openstudio)")


  ## Update the mime cache and icon cache
  ## sudo update-mime-database /usr/share/mime
  #INSTALL(CODE "execute_process(COMMAND ${UPDATE-MIME-DATABASE-EXECUTABLE} /usr/share/mime)")
  ## sudo gtk-update-icon-cache /usr/share/icons/hicolor/ -f
  #INSTALL(CODE "execute_process(COMMAND ${GTK-UPDATE-ICON-CACHE-EXECUTABLE} /usr/share/icons/hicolor/ -f)")


endif()

if(WIN32)
  set( CPACK_PACKAGE_INSTALL_DIRECTORY "openstudio-${OpenStudio_VERSION}" )
  set( CPACK_IFW_TARGET_DIRECTORY "C:/${CPACK_PACKAGE_INSTALL_DIRECTORY}" )
  set( CPACK_BINARY_NSIS OFF )
  set( CPACK_BINARY_IFW ON )
endif()

install(FILES "${CMAKE_BINARY_DIR}/openstudio-gems/Gemfile" DESTINATION ./Ruby/ COMPONENT "RubyAPI" )
install(FILES "${CMAKE_BINARY_DIR}/openstudio-gems/Gemfile.lock" DESTINATION ./Ruby/ COMPONENT "RubyAPI" )
install(FILES "${CMAKE_BINARY_DIR}/openstudio-gems/openstudio-gems.gemspec" DESTINATION ./Ruby/ COMPONENT "RubyAPI" )

get_filename_component(ENERGYPLUS_DIR "${ENERGYPLUS_EXE}" DIRECTORY)
file(GLOB ENERGYPLUS_FILES "${ENERGYPLUS_DIR}/energyplus*")
file(GLOB ENERGYPLUS_LIB_FILES "${ENERGYPLUS_DIR}/libenergyplus*")
file(GLOB EXPAND_OBJECTS "${ENERGYPLUS_DIR}/ExpandObjects*")

install(FILES "${ENERGYPLUS_IDD}" DESTINATION ./EnergyPlus/ COMPONENT EnergyPlus )
install(PROGRAMS ${ENERGYPLUS_FILES} DESTINATION ./EnergyPlus/ COMPONENT EnergyPlus )
install(PROGRAMS ${ENERGYPLUS_LIB_FILES} DESTINATION ./EnergyPlus/ COMPONENT EnergyPlus )
install(PROGRAMS "${EXPAND_OBJECTS}" DESTINATION ./EnergyPlus/ COMPONENT EnergyPlus )

if( APPLE )
  install(PROGRAMS "${ENERGYPLUS_DIR}/libgfortran.5.dylib" DESTINATION ./EnergyPlus/ COMPONENT EnergyPlus )
  install(PROGRAMS "${ENERGYPLUS_DIR}/libquadmath.0.dylib" DESTINATION ./EnergyPlus/ COMPONENT EnergyPlus )
elseif( WIN32 )
  file(GLOB ENERGYPLUS_MSVC_FILES "${ENERGYPLUS_DIR}/msvc*.dll")
  foreach(msvc_file IN LISTS ENERGYPLUS_MSVC_FILES)
    install(PROGRAMS "${msvc_file}" DESTINATION ./EnergyPlus/ COMPONENT EnergyPlus)
  endforeach()
  file(GLOB ENERGYPLUS_VCRUNTIME_FILES "${ENERGYPLUS_DIR}/vcruntime*.dll")
  foreach(vcruntime_file IN LISTS ENERGYPLUS_VCRUNTIME_FILES)
    install(PROGRAMS "${vcruntime_file}" DESTINATION ./EnergyPlus/ COMPONENT EnergyPlus)
  endforeach()
  file(GLOB ENERGYPLUS_UCRT_FILES "${ENERGYPLUS_DIR}/ucrt*.dll")
  foreach(ucrt_file IN LISTS ENERGYPLUS_UCRT_FILES)
    install(PROGRAMS "${ucrt_file}" DESTINATION ./EnergyPlus/ COMPONENT EnergyPlus)
  endforeach()
  file(GLOB ENERGYPLUS_MSAPI_FILES "${ENERGYPLUS_DIR}/api-ms*.dll")
  foreach(msapi_file IN LISTS ENERGYPLUS_MSAPI_FILES)
    install(PROGRAMS "${msapi_file}" DESTINATION ./EnergyPlus/ COMPONENT EnergyPlus)
  endforeach()
  #install(PROGRAMS "${ENERGYPLUS_PATH}/libgfortran.5.dylib" DESTINATION ./EnergyPlus/)
  #install(PROGRAMS "${ENERGYPLUS_PATH}/libquadmath.0.dylib" DESTINATION ./EnergyPlus/)
else()
  #install(PROGRAMS "${ENERGYPLUS_PATH}/libgfortran.5.dylib" DESTINATION ./EnergyPlus/)
  #install(PROGRAMS "${ENERGYPLUS_PATH}/libquadmath.0.dylib" DESTINATION ./EnergyPlus/)
endif()

if ( WIN32 )
  install(DIRECTORY "${CMAKE_BINARY_DIR}/${RADIANCE_PATH}/" DESTINATION ./Radiance/ COMPONENT Radiance USE_SOURCE_PERMISSIONS )
else()
  string(REPLACE "-Redhat" "-Linux" RADIANCE_PATH ${RADIANCE_PATH})
  install(DIRECTORY "${CMAKE_BINARY_DIR}/${RADIANCE_PATH}/usr/local/radiance/" DESTINATION ./Radiance/ COMPONENT Radiance USE_SOURCE_PERMISSIONS )
endif()

if( BUILD_PAT AND APPLE )
  install(FILES "${ENERGYPLUS_IDD}" DESTINATION ./ParametricAnalysisTool.app/Contents/Resources/EnergyPlus/ COMPONENT PAT )
  install(PROGRAMS ${ENERGYPLUS_FILES} DESTINATION ./ParametricAnalysisTool.app/Contents/Resources/EnergyPlus/ COMPONENT PAT )
  install(PROGRAMS ${ENERGYPLUS_LIB_FILES} DESTINATION ./ParametricAnalysisTool.app/Contents/Resources/EnergyPlus/ COMPONENT PAT )
  install(PROGRAMS "${EXPAND_OBJECTS}" DESTINATION ./ParametricAnalysisTool.app/Contents/Resources/EnergyPlus/ COMPONENT PAT )
  install(PROGRAMS "${ENERGYPLUS_DIR}/libgfortran.5.dylib" DESTINATION ./ParametricAnalysisTool.app/Contents/Resources/EnergyPlus/ COMPONENT PAT )
  install(PROGRAMS "${ENERGYPLUS_DIR}/libquadmath.0.dylib" DESTINATION ./ParametricAnalysisTool.app/Contents/Resources/EnergyPlus/ COMPONENT PAT )
  install(DIRECTORY "${CMAKE_BINARY_DIR}/${RADIANCE_PATH}/usr/local/radiance/" DESTINATION ./ParametricAnalysisTool.app/Contents/Resources/Radiance/ COMPONENT PAT USE_SOURCE_PERMISSIONS )
endif()

include(CPack)
include(CPackIFW)

cpack_add_component(CLI
  DISPLAY_NAME "Command Line Interface"
  DESCRIPTION "Command Line Interface"
)

cpack_add_component(RubyAPI
  DISPLAY_NAME "Ruby API"
  DESCRIPTION "Ruby API"
)

cpack_add_component(EnergyPlus
  DISPLAY_NAME "EnergyPlus"
  DESCRIPTION "Minimal EnergyPlus installation"
)

cpack_add_component(Radiance
  DISPLAY_NAME "Radiance"
  DESCRIPTION "Full Radiance installation"
)

cpack_ifw_configure_component(CLI
  SCRIPT src/cli/install_operations.qs
  REQUIRES_ADMIN_RIGHTS
)

cpack_ifw_configure_component(RubyAPI
  REQUIRES_ADMIN_RIGHTS
)

cpack_ifw_configure_component(EnergyPlus
  REQUIRES_ADMIN_RIGHTS
)

cpack_ifw_configure_component(Radiance
  REQUIRES_ADMIN_RIGHTS
)

cpack_ifw_configure_component(Unspecified
  SCRIPT src/install_utility/install_operations.qs
  REQUIRES_ADMIN_RIGHTS
)

if(BUILD_CSHARP_BINDINGS)
  cpack_add_component(CSharpAPI
    DISPLAY_NAME "C# API"
    DESCRIPTION "C# API"
  )

  cpack_ifw_configure_component(CSharpAPI
    REQUIRES_ADMIN_RIGHTS
  )
endif()

if(BUILD_OS_APP)

  cpack_add_component(Resources
    DISPLAY_NAME "Application Resources"
    DESCRIPTION "Resource Files used by the OpenStudio Application and SketchUp Plugin"
  )

  cpack_add_component(SketchUpPlugin
    DISPLAY_NAME "SketchUp Plugin"
    DESCRIPTION "SketchUp Plugin"
  )

  cpack_add_component(OpenStudioApp
    DISPLAY_NAME "OpenStudio Application"
    DESCRIPTION "OpenStudio Application"
  )

  cpack_ifw_configure_component(Resources
    REQUIRES_ADMIN_RIGHTS
  )

  cpack_ifw_configure_component(SketchUpPlugin
    DEPENDS Resources
    SCRIPT sketchup_plugin/install_operations.qs
    REQUIRES_ADMIN_RIGHTS
  )

  cpack_ifw_configure_component(OpenStudioApp
    DEPENDS Resources CLI
    SCRIPT src/openstudio_app/install_operations.qs
    REQUIRES_ADMIN_RIGHTS
  )
endif()

if(BUILD_PAT)
  cpack_add_component(PAT
    DISPLAY_NAME "Parametric Analysis Tool"
    DESCRIPTION "Parametric Analysis Tool"
  )

  cpack_ifw_configure_component(PAT
    DEPENDS CLI RubyAPI
    SCRIPT pat/install_operations.qs
    REQUIRES_ADMIN_RIGHTS
  )
endif()

if(BUILD_DVIEW)
  cpack_add_component(DView
    DISPLAY_NAME "DView"
    DESCRIPTION "DView Application for viewing results"
  )

  cpack_ifw_configure_component(DView
    REQUIRES_ADMIN_RIGHTS
  )
endif()

if( BUILD_NUGET_PACKAGE )
  configure_file( OpenStudio.nuspec.in OpenStudio.nuspec )

  add_custom_target(nuget_package
    COMMAND NuGet pack OpenStudio.nuspec -BasePath "${CMAKE_BINARY_DIR}/" -properties Configuration=$<CONFIGURATION>
    DEPENDS csharp_sdk
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
  )
endif()
