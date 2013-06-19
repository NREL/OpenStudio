project(libssh C)

# Required cmake version
cmake_minimum_required(VERSION 2.6.0)

#ignore warnings from libssh
if (MSVC)
  ADD_DEFINITIONS( "/wd4244 /wd4996 /wd4013 /wd4305" )
endif()

# global needed variables
set(APPLICATION_NAME ${PROJECT_NAME})

set(APPLICATION_VERSION_MAJOR "0")
set(APPLICATION_VERSION_MINOR "4")
set(APPLICATION_VERSION_PATCH "5")

set(APPLICATION_VERSION "${APPLICATION_VERSION_MAJOR}.${APPLICATION_VERSION_MINOR}.${APPLICATION_VERSION_PATCH}")

# SOVERSION scheme: CURRENT.AGE.REVISION
#   If there was an incompatible interface change:
#     Increment CURRENT. Set AGE and REVISION to 0
#   If there was a compatible interface change:
#     Increment AGE. Set REVISION to 0
#   If the source code was changed, but there were no interface changes:
#     Increment REVISION.
set(LIBRARY_VERSION "4.1.2")
set(LIBRARY_SOVERSION "4")

# where to look first for cmake modules, before ${CMAKE_ROOT}/Modules/ is checked
set(CMAKE_MODULE_PATH
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules
)

# add definitions
include(DefineCMakeDefaults)
include(DefineCompilerFlags)
include(DefineInstallationPaths)
include(DefineOptions.cmake)
# include(CPackConfig.cmake)

# disallow in-source build
include(MacroEnsureOutOfSourceBuild)
macro_ensure_out_of_source_build("${PROJECT_NAME} requires an out of source build. Please create a separate build directory and run 'cmake /path/to/${PROJECT_NAME} [options]' there.")

# add macros
include(MacroAddPlugin)
include(MacroCopyFile)

# search for libraries
if (WITH_LIBZ)
  find_package(ZLIB REQUIRED)
endif()

if (WITH_GCRYPT)
  find_package(GCrypt REQUIRED)
  if (NOT GCRYPT_FOUND)
    message(FATAL_ERROR "Could not find GCrypt")
  endif (NOT GCRYPT_FOUND)
else (WITH_GCRYPT)
  find_package(OpenSSL)
  if (NOT OPENSSL_FOUND)
    find_package(GCrypt)
    if (NOT GCRYPT_FOUND)
      message(FATAL_ERROR "Could not find OpenSSL or GCrypt")
    endif (NOT GCRYPT_FOUND)
  endif (NOT OPENSSL_FOUND)
endif(WITH_GCRYPT)

# config.h checks
include(ConfigureChecks.cmake)
configure_file(config.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config.h)

# check subdirectories
# add_subdirectory(doc)
add_subdirectory(include)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include" "${CMAKE_CURRENT_BINARY_DIR}")
add_subdirectory(libssh)

# build samples

# if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
#  add_subdirectory(examples)
# endif (CMAKE_SYSTEM_NAME STREQUAL "Linux")

