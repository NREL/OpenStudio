cmake_minimum_required(VERSION 2.8.11)

project(OpenStudioCore)

include(ExternalProject)

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
# Project version information
set(CMAKE_VERSION_BUILD "Unknown" CACHE STRING "Build number") # git sha

# OpenStudio version
set(OPENSTUDIO_VERSION "${CMAKE_VERSION_MAJOR}.${CMAKE_VERSION_MINOR}.${CMAKE_VERSION_PATCH}")
set(OPENSTUDIO_LONG_VERSION "${CMAKE_VERSION_MAJOR}.${CMAKE_VERSION_MINOR}.${CMAKE_VERSION_PATCH}.${CMAKE_VERSION_BUILD}")

# EnergyPlus Idd version
set(ENERGYPLUS_VERSION_MAJOR 8)
set(ENERGYPLUS_VERSION_MINOR 5)
set(ENERGYPLUS_VERSION_PATCH 0)
set(ENERGYPLUS_VERSION "${ENERGYPLUS_VERSION_MAJOR}.${ENERGYPLUS_VERSION_MINOR}.${ENERGYPLUS_VERSION_PATCH}")
# Build SHA is not required to have a value, but if it does OpenStudio will require this build.
set(ENERGYPLUS_BUILD_SHA "c87e61b44b")

# Radiance
set(RADIANCE_VERSION "5.0.a.8")

# configure file with version information
configure_file(${CMAKE_SOURCE_DIR}/OpenStudio.in ${CMAKE_BINARY_DIR}/src/OpenStudio.hxx)

# universal configuration for mac plists
if(APPLE)
  set(MACOSX_BUNDLE_SHORT_VERSION_STRING "${OPENSTUDIO_VERSION}")
  set(MACOSX_BUNDLE_BUNDLE_VERSION "${CMAKE_VERSION_BUILD}")
  set(MACOSX_BUNDLE_COPYRIGHT "Copyright © 2016 National Renewable Energy Laboratory")
endif()

# compiler id for about boxes
if(MSVC)
  set(ABOUT_COMPILER "${CMAKE_GENERATOR}")
elseif(APPLE)
  set(ABOUT_COMPILER "${CMAKE_CXX_COMPILER_ID} ${CMAKE_OSX_ARCHITECTURES} - OSX ${CMAKE_OSX_DEPLOYMENT_TARGET}")
else()
  set(ABOUT_COMPILER "${CMAKE_CXX_COMPILER_ID}")
endif()

###############################################################################

###############################################################################
# Build options

if(UNIX AND NOT APPLE)
  # the RPATH to be used when installing
  set(CMAKE_INSTALL_RPATH "$ORIGIN/../lib")
endif()

# Build C++ documentation using Doxygen
# Requires: doxygen
option(BUILD_DOCUMENTATION "Build Documentation" OFF)

# Build CSharp bindings
# Requires: SWIG
option(BUILD_CSHARP_BINDINGS "Build CSharp bindings" OFF)

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
option(BUILD_PACKAGE "Build package" OFF)

# Build with OpenSSL support
set(BUILD_WITH_OPENSSL ON CACHE INTERNAL "Build With OpenSSL Support For SSH Connections")

# Build R package
option(BUILD_R "Build R package" OFF)

# Build Ruby gem
option(BUILD_RUBY_GEM "Build Ruby Gem" OFF)

# Build shared OS libraries
option(BUILD_SHARED_LIBS "Build OS libs as shared libraries" OFF)

# Build the OpenStudio Application
# This option will require shared libs
option(BUILD_OS_APP "Build OS App" ON)

# Use static version of Qt
# This option is used for making static cli
option(Qt5Static "Use Static Qt" Off)

if (BUILD_SHARED_LIBS)
  add_definitions("-DSHARED_OS_LIBS")
endif()

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  add_definitions(-Qunused-arguments)
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

if(UNIX AND NOT APPLE AND BUILD_TESTING)
  # Enable libgtest linking
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lpthread")
endif()

if( WIN32 )
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /OPT:REF /OPT:NOICF")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zi")
endif()


if(BUILD_RUBY_GEM)
  find_program(GEM NAMES gem)
  if(NOT GEM)
    message(SEND_ERROR "Gem tool not found, cannot build gem")
    if(UNIX AND NOT APPLE)
      message(SEND_ERROR "Please install rubygems using your package management system")
    endif()
  endif()

  # Build Ruby gem and package up all the libs
  option(BUILD_RUBY_GEM_WITH_LIBS "Package all required libs with the Ruby Gem" ON)
endif()

if(BUILD_V8_BINDINGS)
  option(BUILD_NODE_MODULES "Build V8 Bindings as Node Modules" ON)
endif()

# Configure vagrant
set(SHOW_VAGRANT "false" CACHE BOOL "Show vagrant option to GUI users")

# Use PCH
option(USE_PCH "Use precompiled headers" OFF)

if(WIN32)
  add_definitions(-DNOMINMAX)
endif()

if(MSVC)
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

  # extra warnings - DLM: we should shoot for this
  #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wextra")
  # warn the user if a variable declaration shadows another with the same name in the same scope - DLM: we should shoot for this
  #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wshadow")
  # effective c++ - DLM: we should shoot for this
  #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Weffc++")
  # warn about non-portable code - DLM: we should shoot for this
  #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic")
  # treat warnings as errors - DLM: we should shoot for this
  #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
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
  if(BUILD_WITH_MULTIPLE_PROCESSES)
    add_definitions(/MP)
  endif()

  #Enable warning on thread un-safe static member initialization - DLM: we have been bit by this many times
  add_definitions(/w44640)

  # ignore decorated name length exceeded
  add_definitions(/wd4503)

  # ignore needs to have dll-interface to be used by clients of class
  # we will not rely on exporting templates instead all libraries must
  # be linked against the same runtime
  add_definitions(/wd4251)

  if(CMAKE_CL_64)

    add_definitions(/bigobj)

    # ignore conversion from size_t to int for now
    add_definitions(/wd4267)

  endif()

  # treat warnings as errors
  # DLM: only do this for our code, added in later
  # add_definitions(/WX)

  # ignore warnings about the stl being insecure
  add_definitions(-D_CRT_SECURE_NO_WARNINGS -D_SCL_SECURE_NO_WARNINGS)

endif()
###############################################################################


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
    set(ENERGYPLUS_EXPECTED_HASH 01c4232f7136e42995c344c389d96a61)
    set(ENERGYPLUS_PLATFORM "Darwin-x86_64")
  elseif(EXISTS "/etc/redhat-release")
    #message(FATAL_ERROR "EnergyPlus ${ENERGYPLUS_VERSION} redhat build missing")
    set(ENERGYPLUS_EXPECTED_HASH ea8cb9e9efc622a85d82fe2306281420)
    set(ENERGYPLUS_PLATFORM "Redhat-x86_64")
    #message(FATAL_ERROR "EnergyPlus ${ENERGYPLUS_PLATFORM} redhat build missing")
  else()
    set(ENERGYPLUS_EXPECTED_HASH 0053d9b145f9c3485bb49f85ddb61f6c)
    set(ENERGYPLUS_PLATFORM "Linux-x86_64")
  endif()
  set(ENERGYPLUS_PATH "EnergyPlus-${ENERGYPLUS_VERSION}-${ENERGYPLUS_BUILD_SHA}-${ENERGYPLUS_PLATFORM}")
  if(EXISTS "${CMAKE_BINARY_DIR}/${ENERGYPLUS_PATH}.tar.bz2")
    file(MD5 "${CMAKE_BINARY_DIR}/${ENERGYPLUS_PATH}.tar.bz2" ENERGYPLUS_HASH)
  endif()
  if(NOT EXISTS "${CMAKE_BINARY_DIR}/${ENERGYPLUS_PATH}.tar.bz2" OR NOT "${ENERGYPLUS_HASH}" MATCHES "${ENERGYPLUS_EXPECTED_HASH}")
    message(STATUS "Downloading EnergyPlus ${ENERGYPLUS_VERSION} (${ENERGYPLUS_PLATFORM})")
    file(DOWNLOAD "http://openstudio-resources.s3.amazonaws.com/dependencies/${ENERGYPLUS_PATH}.tar.bz2" "${CMAKE_BINARY_DIR}/${ENERGYPLUS_PATH}.tar.bz2" TIMEOUT 320 INACTIVITY_TIMEOUT 120 SHOW_PROGRESS EXPECTED_MD5 ${ENERGYPLUS_EXPECTED_HASH})
    execute_process(COMMAND ${CMAKE_COMMAND} -E remove_directory "${CMAKE_BINARY_DIR}/${ENERGYPLUS_PATH}")
    execute_process(COMMAND ${CMAKE_COMMAND} -E tar xfz "${CMAKE_BINARY_DIR}/${ENERGYPLUS_PATH}.tar.bz2" WORKING_DIRECTORY "${CMAKE_BINARY_DIR}")
  endif()
  set(ENV{ENERGYPLUSDIR} "${CMAKE_BINARY_DIR}/${ENERGYPLUS_PATH}/EnergyPlus-${ENERGYPLUS_VERSION_MAJOR}-${ENERGYPLUS_VERSION_MINOR}-${ENERGYPLUS_VERSION_PATCH}")
elseif(WIN32)
  if(CMAKE_CL_64)
    set(ENERGYPLUS_PATH "EnergyPlus-${ENERGYPLUS_VERSION}-${ENERGYPLUS_BUILD_SHA}-Windows-x86_64")
    set(ENERGYPLUS_ARCH 64)
    set(ENERGYPLUS_EXPECTED_HASH ca4d9e956015453948a40a5010c8dbbb)
  else()
    set(ENERGYPLUS_PATH "EnergyPlus-${ENERGYPLUS_VERSION}-${ENERGYPLUS_BUILD_SHA}-Windows-i386")
    set(ENERGYPLUS_ARCH 32)
    set(ENERGYPLUS_EXPECTED_HASH 5ecaaaa815f9867cf10a1decbfc59aa1)
  endif()
  if(EXISTS "${CMAKE_BINARY_DIR}/${ENERGYPLUS_PATH}.zip")
    file(MD5 "${CMAKE_BINARY_DIR}/${ENERGYPLUS_PATH}.zip" ENERGYPLUS_HASH)
  endif()
  if(NOT EXISTS "${CMAKE_BINARY_DIR}/${ENERGYPLUS_PATH}.zip" OR NOT "${ENERGYPLUS_HASH}" MATCHES "${ENERGYPLUS_EXPECTED_HASH}")
    message(STATUS "Downloading EnergyPlus ${ENERGYPLUS_VERSION} (${ENERGYPLUS_ARCH}-bit)")
    file(DOWNLOAD "http://openstudio-resources.s3.amazonaws.com/dependencies/${ENERGYPLUS_PATH}.zip" "${CMAKE_BINARY_DIR}/${ENERGYPLUS_PATH}.zip" TIMEOUT 320 INACTIVITY_TIMEOUT 120 SHOW_PROGRESS EXPECTED_MD5 ${ENERGYPLUS_EXPECTED_HASH})
    execute_process(COMMAND ${CMAKE_COMMAND} -E remove_directory "${CMAKE_BINARY_DIR}/${ENERGYPLUS_PATH}")
    execute_process(COMMAND ${CMAKE_COMMAND} -E tar xfz "${CMAKE_BINARY_DIR}/${ENERGYPLUS_PATH}.zip" WORKING_DIRECTORY "${CMAKE_BINARY_DIR}")
  endif()
  set(ENV{ENERGYPLUSDIR} "${CMAKE_BINARY_DIR}/${ENERGYPLUS_PATH}/EnergyPlus-${ENERGYPLUS_VERSION_MAJOR}-${ENERGYPLUS_VERSION_MINOR}-${ENERGYPLUS_VERSION_PATCH}")
endif()
find_package(EnergyPlus "${ENERGYPLUS_VERSION}" REQUIRED)

# Radiance
if(UNIX)
  if(APPLE)
    set(RADIANCE_EXPECTED_HASH c3e94e19d13c60e556c0005ec0bf723f)
    set(RADIANCE_PLATFORM "Darwin")
  elseif(EXISTS "/etc/redhat-release")
    #message(FATAL_ERROR "Radiance ${RADIANCE_VERSION} redhat build missing")
    set(RADIANCE_EXPECTED_HASH f43fcdd76f45654daef69f38cd1f58a6)
    set(RADIANCE_PLATFORM "Redhat-x86_64")
  else()
    set(RADIANCE_EXPECTED_HASH 886ac1764a70f2f7b211231e091d32bf)
    set(RADIANCE_PLATFORM "Linux-x86_64")
  endif()
  set(RADIANCE_PATH "Radiance-${RADIANCE_VERSION}-${RADIANCE_PLATFORM}")
  if(EXISTS "${CMAKE_BINARY_DIR}/${RADIANCE_PATH}.tar.bz2")
    file(MD5 "${CMAKE_BINARY_DIR}/${RADIANCE_PATH}.tar.bz2" RADIANCE_HASH)
  endif()
  if(NOT EXISTS "${CMAKE_BINARY_DIR}/${RADIANCE_PATH}.tar.bz2" OR NOT "${RADIANCE_HASH}" MATCHES "${RADIANCE_EXPECTED_HASH}")
    message(STATUS "Downloading Radiance ${RADIANCE_VERSION} (${RADIANCE_PLATFORM})")
    file(DOWNLOAD "http://openstudio-resources.s3.amazonaws.com/dependencies/Radiance-${RADIANCE_VERSION}-${RADIANCE_PLATFORM}.tar.bz2" "${CMAKE_BINARY_DIR}/${RADIANCE_PATH}.tar.bz2" TIMEOUT 120 INACTIVITY_TIMEOUT 120 SHOW_PROGRESS EXPECTED_MD5 ${RADIANCE_EXPECTED_HASH})
    execute_process(COMMAND ${CMAKE_COMMAND} -E remove_directory "${CMAKE_BINARY_DIR}/${RADIANCE_PATH}")
    execute_process(COMMAND ${CMAKE_COMMAND} -E tar xfz "${CMAKE_BINARY_DIR}/${RADIANCE_PATH}.tar.bz2" WORKING_DIRECTORY "${CMAKE_BINARY_DIR}")
  endif()
  set(RADIANCE_LOCATION "${CMAKE_BINARY_DIR}/radiance")
elseif(WIN32)
  if(CMAKE_CL_64)
    set(RADIANCE_ARCH 64)
    set(RADIANCE_EXPECTED_HASH 30ea31d1eff986bfd1744e4260c06fb2)
  else()
    set(RADIANCE_ARCH 32)
    set(RADIANCE_EXPECTED_HASH 78eda5d4b0215cd675cc3ec8975e6122)
  endif()
  set(RADIANCE_PATH "Radiance-${RADIANCE_VERSION}-win${RADIANCE_ARCH}")
  if(EXISTS "${CMAKE_BINARY_DIR}/${RADIANCE_PATH}.zip")
    file(MD5 "${CMAKE_BINARY_DIR}/${RADIANCE_PATH}.zip" RADIANCE_HASH)
  endif()
  if(NOT EXISTS "${CMAKE_BINARY_DIR}/${RADIANCE_PATH}.zip" OR NOT "${RADIANCE_HASH}" MATCHES "${RADIANCE_EXPECTED_HASH}")
    message(STATUS "Downloading Radiance ${RADIANCE_VERSION} (${RADIANCE_ARCH}-bit)")
    file(DOWNLOAD "http://openstudio-resources.s3.amazonaws.com/dependencies/Radiance-${RADIANCE_VERSION}-win${RADIANCE_ARCH}.zip" "${CMAKE_BINARY_DIR}/${RADIANCE_PATH}.zip" TIMEOUT 120 INACTIVITY_TIMEOUT 120 SHOW_PROGRESS EXPECTED_MD5 ${RADIANCE_EXPECTED_HASH})
    execute_process(COMMAND ${CMAKE_COMMAND} -E remove_directory "${CMAKE_BINARY_DIR}/${RADIANCE_PATH}")
    execute_process(COMMAND ${CMAKE_COMMAND} -E tar xfz "${CMAKE_BINARY_DIR}/${RADIANCE_PATH}.zip" WORKING_DIRECTORY "${CMAKE_BINARY_DIR}")
  endif()
  set(RADIANCE_LOCATION "${CMAKE_BINARY_DIR}/Radiance")
endif()

# Ruby

set(RUBY_INSTALL_DIR "" CACHE PATH "Path to Ruby Install")
mark_as_advanced(RUBY_INSTALL_DIR)

if(NOT EXISTS ${RUBY_INSTALL_DIR})
  if( WIN32 )
  elseif( APPLE )
    set(RUBY_ZIP_FILENAME "ruby_2_0_0_osx_static.tar.gz")
    set(RUBY_ZIP_EXPECTED_MD5 "87875206d6255d4b75e9a5fcbb273e19")
  elseif(EXISTS "/etc/redhat-release")
  else()
  endif()

  set(RUBY_ZIP_LOCAL_PATH "${CMAKE_BINARY_DIR}/${RUBY_ZIP_FILENAME}")
  if(EXISTS "${RUBY_ZIP_LOCAL_PATH}")
    file(MD5 "${RUBY_ZIP_LOCAL_PATH}" RUBY_ZIP_MD5)
  endif()

  if( NOT RUBY_ZIP_MD5 STREQUAL RUBY_ZIP_EXPECTED_MD5)
    file(DOWNLOAD "http://openstudio-resources.s3.amazonaws.com/dependencies/${RUBY_ZIP_FILENAME}"
      ${RUBY_ZIP_LOCAL_PATH}
      INACTIVITY_TIMEOUT 120
      SHOW_PROGRESS
      EXPECTED_MD5 ${RUBY_ZIP_EXPECTED_MD5})
      execute_process(COMMAND ${CMAKE_COMMAND} -E tar xfz ${RUBY_ZIP_LOCAL_PATH} WORKING_DIRECTORY "${CMAKE_BINARY_DIR}")
  endif()

  set(RUBY_INSTALL_DIR "${CMAKE_BINARY_DIR}/Ruby-install/")
  find_file( RUBY_CONFIG_INCLUDE_DIR config.h PATHS "${RUBY_INSTALL_DIR}/include/" )
endif()
find_package(Ruby 2.0 REQUIRED)

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
    #install(FILES "${LIBEAY32_PATH}" "${SSLEAY32_PATH}" DESTINATION "bin")
    #install(FILES "${LIBEAY32_PATH}" "${SSLEAY32_PATH}" DESTINATION ${RUBY_MODULE_ARCH_DIR})
  endif()
endif()

find_package(SWIG 3.0.0 REQUIRED)
mark_as_advanced(
  SWIG_DIR
  SWIG_EXECUTABLE
  SWIG_VERSION
)

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
  set(ALL_CSHARP_BINDING_TARGETS "") # global list
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
  find_package(PythonInterp 2.7 REQUIRED)
  find_package(PythonLibs 2.7 REQUIRED)
  include_directories(SYSTEM ${PYTHON_INCLUDE_DIRS})
  set(ALL_PYTHON_BINDING_TARGETS "") # global list


endif()

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
  add_dependencies(openstudio_utilities_doc
    openstudio_utilities_idd_doc
  )
  add_dependencies(openstudio_model_doc
    openstudio_utilities_doc
    openstudio_utilities_idd_doc
  )


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
  # Google test library
  set(gtest_force_shared_crt OFF)
  set(gtest_build_tests OFF)
  set(gtest_build_samples OFF)
  set(gtest_disable_pthreads OFF)
  add_definitions(-DGTEST_HAS_TR1_TUPLE=0) 
  include_directories(SYSTEM src/google_test/include)
  add_subdirectory(src/google_test)
endif()

# GeographicLib
set(GEOGRAPHICLIB_LIB_TYPE STATIC)
set(GEOGRAPHICLIB_STATIC_LIB ON)
set(GEOGRAPHICLIB_SHARED_LIB OFF)
set(GEOGRAPHICLIB_DOCUMENTATION OFF)
set(BUILD_NETGEOGRAPHICLIB OFF)
set(GEOGRAPHICLIB_PRECISION 2)
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
    endif()
  elseif( APPLE )
  elseif(EXISTS "/etc/redhat-release")
  else()
  endif()

  set(BOOST_ZIP_LOCAL_PATH "${CMAKE_BINARY_DIR}/${BOOST_ZIP_FILENAME}")
  if(EXISTS "${BOOST_ZIP_LOCAL_PATH}")
    file(MD5 "${BOOST_ZIP_LOCAL_PATH}" BOOST_ZIP_MD5)
  endif()

  if( NOT BOOST_ZIP_MD5 STREQUAL BOOST_ZIP_EXPECTED_MD5)
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
 		if( Qt5Static )
      set(QT_ZIP_FILENAME "qt_5_6_msvc2013_64_static_temp.zip")
      set(QT_ZIP_EXPECTED_MD5 "3f0e1a505e707c9f1bde31e05f17fc47")
    else()
      set(QT_ZIP_FILENAME "qt_5_6_msvc2013_64_shared_temp.zip")
      set(QT_ZIP_EXPECTED_MD5 "2ac0ed2909710350f1c6acd5f38a8b33")
    endif()

  elseif( APPLE )
    if( Qt5Static )
      set(QT_ZIP_FILENAME "qt_5_6_osx_static_temp.tar.gz")
      set(QT_ZIP_EXPECTED_MD5 "094985d51cc686cdb665c45fff9c088f")
    else()
      set(QT_ZIP_FILENAME "qt_5_6_osx_shared.tar.gz")
      set(QT_ZIP_EXPECTED_MD5 "326887393d55b5da97a1be9764fb0d1b")
    endif()

  elseif(EXISTS "/etc/redhat-release")
    if( Qt5Static )
      set(QT_ZIP_FILENAME "qt_5_6_redhat_64_static_temp.tar.gz")
      set(QT_ZIP_EXPECTED_MD5 "4c62bd59d1c4ebc14594fe98b33c58fd")
    else()
      message(SEND_ERROR "Shared Redhat Qt build not available")
      #set(QT_ZIP_FILENAME "qt_5_6_redhat_64_shared.tar.gz")
      #set(QT_ZIP_EXPECTED_MD5 "52424fdb4d553b94156de088881506b5")
    endif()

  else()
    #ubuntu
		if( Qt5Static )
      set(QT_ZIP_FILENAME "qt_5_6_linux_64_static_temp.tar.gz")
      set(QT_ZIP_EXPECTED_MD5 "1377b18696829ccd7d5b1906b05d9857")
    else()
      message(SEND_ERROR "Shared Ubuntu Qt build not available")
      #set(QT_ZIP_FILENAME "qt_5_6_linux_64_static.tar.gz")
      #set(QT_ZIP_EXPECTED_MD5 "52424fdb4d553b94156de088881506b5")
    endif()

  endif()

  set(QT_ZIP_LOCAL_PATH "${CMAKE_BINARY_DIR}/${QT_ZIP_FILENAME}")
  if(EXISTS "${QT_ZIP_LOCAL_PATH}")
    file(MD5 "${QT_ZIP_LOCAL_PATH}" QT_ZIP_MD5)
  endif()

  if( NOT QT_ZIP_MD5 STREQUAL QT_ZIP_EXPECTED_MD5)
    file(DOWNLOAD "http://openstudio-resources.s3.amazonaws.com/dependencies/${QT_ZIP_FILENAME}"
      ${QT_ZIP_LOCAL_PATH}
      INACTIVITY_TIMEOUT 120
      SHOW_PROGRESS
      EXPECTED_MD5 ${QT_ZIP_EXPECTED_MD5})
      execute_process(COMMAND ${CMAKE_COMMAND} -E tar xfz ${QT_ZIP_LOCAL_PATH} WORKING_DIRECTORY "${CMAKE_BINARY_DIR}")
  endif()

  set(QT_INSTALL_DIR "${CMAKE_BINARY_DIR}/Qt-install/")
endif()

find_package(Qt5Core 5.6.0 REQUIRED HINTS ${QT_INSTALL_DIR})
find_package(Qt5Widgets 5.6.0 REQUIRED HINTS ${QT_INSTALL_DIR})
find_package(Qt5Sql 5.6.0 REQUIRED HINTS ${QT_INSTALL_DIR})
find_package(Qt5Network 5.6.0 REQUIRED HINTS ${QT_INSTALL_DIR})
find_package(Qt5Xml 5.6.0 REQUIRED HINTS ${QT_INSTALL_DIR})
find_package(Qt5Concurrent 5.6.0 REQUIRED HINTS ${QT_INSTALL_DIR})
find_package(Qt5PrintSupport 5.6.0 REQUIRED HINTS ${QT_INSTALL_DIR})
find_package(Qt5Gui 5.6.0 REQUIRED HINTS ${QT_INSTALL_DIR})

if( WIN32 )
  find_package(Qt5WinExtras 5.6.0 REQUIRED HINTS ${QT_INSTALL_DIR})
endif()

set(CMAKE_AUTOMOC OFF)

# list of all Qt libs and dependencies we are using
list(APPEND QT_LIBS Qt5::Core)
list(APPEND QT_LIBS Qt5::Gui)
list(APPEND QT_LIBS Qt5::Widgets)
list(APPEND QT_LIBS Qt5::Sql)
list(APPEND QT_LIBS Qt5::Network)
list(APPEND QT_LIBS Qt5::Xml)
list(APPEND QT_LIBS Qt5::PrintSupport)
if( WIN32 )
	list(APPEND QT_LIBS Qt5::WinExtras)
endif()

get_target_property(QtCore_TYPE Qt5::Core TYPE)

if( Qt5Static )
  if(NOT QtCore_TYPE STREQUAL "STATIC_LIBRARY")
    message(ERROR "A shared library version of Qt was found, but the project is configured for static Qt")
  endif()
  add_definitions(-DQT_STATIC)
else()
  if(QtCore_TYPE STREQUAL "STATIC_LIBRARY")
    message(ERROR "A static library version of Qt was found, but the project is configured for shared Qt")
  endif()
endif()

if(BUILD_OS_APP)
  if( Qt5Static )
		message(ERROR "Project is configured for static Qt, which is not a valid configuration when building OS App")
  endif()
endif()

if( NOT Qt5Static )
	find_package(Qt5Quick 5.6.0 REQUIRED HINTS ${QT_INSTALL_DIR})
	find_package(Qt5Qml 5.6.0 REQUIRED HINTS ${QT_INSTALL_DIR})
	find_package(Qt5WebChannel 5.6.0 REQUIRED HINTS ${QT_INSTALL_DIR})
	find_package(Qt5Positioning 5.6.0 REQUIRED HINTS ${QT_INSTALL_DIR})
	find_package(Qt5WebEngine 5.6.0 REQUIRED HINTS ${QT_INSTALL_DIR})
	find_package(Qt5WebEngineWidgets 5.6.0 REQUIRED HINTS ${QT_INSTALL_DIR})

  list(APPEND QT_LIBS Qt5::Quick)
  list(APPEND QT_LIBS Qt5::Qml)
  list(APPEND QT_LIBS Qt5::WebEngine)
  list(APPEND QT_LIBS Qt5::WebEngineCore)
  list(APPEND QT_LIBS Qt5::WebEngineWidgets)
  list(APPEND QT_LIBS Qt5::WebChannel)
  list(APPEND QT_LIBS Qt5::Positioning)
  list(APPEND QT_LIBS Qt5::Concurrent)
else()
	if( UNIX )
  	list(APPEND QT_LIBS "${QT_INSTALL_DIR}/plugins/sqldrivers/libqsqlite.a")
  	list(APPEND QT_LIBS "${QT_INSTALL_DIR}/lib/libqtharfbuzzng.a")
  	find_package(OpenGL REQUIRED)
  	list(APPEND QT_LIBS "${OPENGL_LIBRARIES}")
	endif()

	if( UNIX AND NOT APPLE )
	  find_library(png_lib png)
	  list(APPEND QT_LIBS ${png_lib})
	  if(EXISTS "/etc/redhat-release")
	    list(APPEND QT_LIBS "${QT_INSTALL_DIR}/lib/libqtpcre.a")
	    find_library(rt_lib rt)
	    list(APPEND QT_LIBS ${rt_lib})
	    find_library(jpeg_lib jpeg)
	    list(APPEND QT_LIBS ${jpeg_lib})
	    find_library(glib_lib glib-2.0)
	    list(APPEND QT_LIBS ${glib_lib})
	    find_library(gthread_lib gthread-2.0)
	    list(APPEND QT_LIBS ${gthread_lib})
	    find_library(db_lib db)
	    list(APPEND QT_LIBS ${db_lib})
	  else()
	    find_library(pcre16_lib pcre16)
	    list(APPEND QT_LIBS ${pcre16_lib})
	  endif()
	  find_library(crypto_lib crypto)
	  list(APPEND QT_LIBS ${crypto_lib})
	endif()

	if( APPLE )
  	find_library(corefoundation_lib CoreFoundation)
  	find_library(cfnetwork_lib CFNetwork)
  	find_library(coregraphics_lib CoreGraphics)
  	find_library(carbon_lib Carbon)
  	find_library(appkit_lib AppKit)
  	find_library(systemconfiguration_lib SystemConfiguration)
  	find_library(security_lib Security)
  	list(APPEND QT_LIBS ${corefoundation_lib})
  	list(APPEND QT_LIBS ${cfnetwork_lib})
  	list(APPEND QT_LIBS ${coregraphics_lib})
  	list(APPEND QT_LIBS ${carbon_lib})
  	list(APPEND QT_LIBS ${appkit_lib})
  	list(APPEND QT_LIBS ${systemconfiguration_lib})
  	list(APPEND QT_LIBS ${security_lib})
  	list(APPEND QT_LIBS "${QT_INSTALL_DIR}/lib/libqtpcre.a")
	endif()

	if( WIN32 )
	  find_library(HARFBUZZ qtharfbuzzng PATHS "${QT_INSTALL_DIR}/lib" NO_DEFAULT_PATH)
	  find_library(PCRE qtpcre PATHS "${QT_INSTALL_DIR}/lib" NO_DEFAULT_PATH)
	  find_library(SQLITE qsqlite PATHS "${QT_INSTALL_DIR}/plugins/sqldrivers/" NO_DEFAULT_PATH)
	  find_library(WIN_PLATFORM qwindows PATHS "${QT_INSTALL_DIR}/plugins/platforms/" NO_DEFAULT_PATH)

	  find_library(PLATFORM_SUPPORT Qt5PlatformSupport PATHS "${QT_INSTALL_DIR}/lib" NO_DEFAULT_PATH)
	  find_library(FREETYPE qtfreetype PATHS "${QT_INSTALL_DIR}/lib" NO_DEFAULT_PATH)
	
	  mark_as_advanced(PLATFORM_SUPPORT FREETYPE)
	
	  list(APPEND QT_LIBS ${PLATFORM_SUPPORT})
	  list(APPEND QT_LIBS ${FREETYPE})
	
	  mark_as_advanced(HARFBUZZ PCRE SQLITE WIN_PLATFORM)
	
	  list(APPEND QT_LIBS ${HARFBUZZ})
	  list(APPEND QT_LIBS ${PCRE})
	  list(APPEND QT_LIBS ${SQLITE})
	  list(APPEND QT_LIBS ${WIN_PLATFORM})
	
	  list(APPEND QT_LIBS Crypt32)
	  list(APPEND QT_LIBS imm32)
	  list(APPEND QT_LIBS winmm)
	  list(APPEND QT_LIBS Gdi32)
	
	  list(APPEND QT_LIBS wsock32)
	  list(APPEND QT_LIBS ws2_32)
	  list(APPEND QT_LIBS iphlpapi)
	  list(APPEND QT_LIBS dbghelp)
	  list(APPEND QT_LIBS Shlwapi)
    
    if(BUILD_WITH_OPENSSL)
      link_directories("${OPENSSL_ROOT_DIR}/lib/VC/static")
      list(APPEND QT_LIBS ssleay32MT)
      list(APPEND QT_LIBS libeay32MT)
    endif()
    
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

# Each subfolder of src is a major sub-project
set(project_directories
  generateiddfactory
  utilities
  model
  energyplus
  radiance
  gbxml
  #bimserver
  airflow
  isomodel
  osversion
  measure
  #resultsviewer
  #model_editor
  #openstudio_lib
  sdd
  cli
)
if(WIN32)
  list(APPEND project_directories "qtwinmigrate")
endif()

if(BUILD_OS_APP)
  list(APPEND project_directories "qwt")
  #list(APPEND project_directories "gbxml")
  list(APPEND project_directories "bimserver")
  list(APPEND project_directories "resultsviewer")
  list(APPEND project_directories "model_editor")
  list(APPEND project_directories "openstudio_lib")
  list(APPEND project_directories "openstudio_app")
endif()

# Include the resources, should happen after find EnergyPlus
add_subdirectory(resources)

add_subdirectory(ruby)

foreach(D ${project_directories})
  add_subdirectory(src/${D})
endforeach()

# Make sure resultsviewer has its resources built
#add_dependencies(ResultsViewer ResultsViewer_resources)

# ruby, after loading projects
#add_subdirectory(ruby)

# csharp, after loading projects
if(BUILD_CSHARP_BINDINGS)
  add_subdirectory(csharp)
endif()

# java, after loading projects
if(BUILD_JAVA_BINDINGS)
  add_subdirectory(java)
endif()

# v8-node
if(BUILD_NODE_MODULES)
  add_subdirectory(v8-node)
endif()

# R, after loading projects
if(BUILD_R)
  add_subdirectory(r-scripts)
endif()

###############################################################################

###############################################################################
# Targets that combine all bindings targets
add_custom_target(ALL_RUBY_BINDINGS)
add_dependencies(ALL_RUBY_BINDINGS ${ALL_RUBY_BINDING_TARGETS})

if(BUILD_PYTHON_BINDINGS)
  add_custom_target(ALL_PYTHON_BINDINGS)
  add_dependencies(ALL_PYTHON_BINDINGS ${ALL_PYTHON_BINDING_TARGETS})

    # PYTHONPATH=~/openstudio/build/OSCore-prefix/src/OSCore-build/Products/python/ python testpath.py


  add_test(NAME "PythonSanityCheck"
    COMMAND "${PYTHON_EXECUTABLE}" "${CMAKE_SOURCE_DIR}/python/testpath.py"
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/Products/python"
  )

  if (MSVC)
    file(TO_NATIVE_PATH "${CMAKE_BINARY_DIR}" NATIVE_BINARY_DIR)
    add_custom_command(
      OUTPUT "${CMAKE_BINARY_DIR}/python_copied.stamp"
      COMMAND ${CMAKE_COMMAND} -E touch "${CMAKE_BINARY_DIR}/python_copied.stamp"
      COMMAND cmd /C del "${NATIVE_BINARY_DIR}\\Products\\python\\$<CONFIGURATION>\\*.py"
      COMMAND cmd /C copy "${NATIVE_BINARY_DIR}\\Products\\python\\*.py" "${NATIVE_BINARY_DIR}\\Products\\python\\$<CONFIGURATION>"
      DEPENDS ALL_PYTHON_BINDINGS
    )

    add_custom_target(PYTHON_SDK ALL
      DEPENDS "${CMAKE_BINARY_DIR}/python_copied.stamp"
    )

    set_property(TEST PythonSanityCheck
      PROPERTY ENVIRONMENT
      "PYTHONPATH=${CMAKE_BINARY_DIR}/Products/python/$<CONFIGURATION>"
    )
  else()
    set_property(TEST PythonSanityCheck
      PROPERTY ENVIRONMENT
      "PYTHONPATH=${CMAKE_BINARY_DIR}/Products/python"
    )
  endif()

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
if(BUILD_DOCUMENTATION)

  add_custom_target(ALL_DOXYGEN)
  add_dependencies(ALL_DOXYGEN ${ALL_CPP_DOC_TARGETS})

  add_custom_target(ALL_RDOC)
  add_dependencies(ALL_RDOC ${ALL_RDOC_TARGETS})

  # Zip documentation
  if(WIN32)
    if(NOT EXISTS "${CMAKE_BINARY_DIR}/7za.exe")
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
      WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/ruby/rdocify
    )
  endif()

endif()


###############################################################################
# Export targets and generate OpenStudioCoreConfig.cmake

set(TARGETS_FILE ${CMAKE_BINARY_DIR}/OpenStudioTargets.cmake)

set(all_lib_targets
  openstudio_osversion
  openstudio_model
  #openstudio_modeleditor
  openstudio_utilities
  openstudio_energyplus
  openstudio_measure
  openstudio_radiance
  openstudio_gbxml
  #openstudio_bimserver
  openstudio_isomodel
  openstudio_lib
  openstudio_sdd
  openstudio_airflow
  #qwt
  miniziplib
  sqlite
  jsoncpp
  ${ALL_RUBY_BINDING_TARGETS}
)
if(WIN32)
  list(APPEND all_lib_targets "qtwinmigrate")
endif()

if(QtCore_TYPE STREQUAL "SHARED_LIBRARY")
  list(APPEND all_lib_targets "openstudio_modeleditor")
  #list(APPEND all_lib_targets "openstudio_gbxml")
  list(APPEND all_lib_targets "openstudio_bimserver")
  #list(APPEND all_lib_targets "openstudio_lib")
  list(APPEND all_lib_targets "qwt")
endif()

export(TARGETS
  ${all_lib_targets}
  FILE "${TARGETS_FILE}"
)

set(all_exe_targets
  GenerateIddFactory
  gbXMLToIdf
  #OpenStudio
  #ResultsViewer
)

if(QtCore_TYPE STREQUAL "SHARED_LIBRARY")
  list(APPEND all_exe_targets "OpenStudio")
  list(APPEND all_exe_targets "ResultsViewer")
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

# Used by some Ruby tests, remove eventually
#install(FILES "${CMAKE_SOURCE_DIR}/resources/resultsviewer/SmallOffice/SmallOffice.idf"
#        DESTINATION "${examplesdir}/resultsviewer/SmallOffice/")
# Unnecessary files or just left out for size?
#install(FILES ${CMAKE_BINARY_DIR}/resources/resultsviewer/SmallOffice/eplustbl.htm
#        DESTINATION ${examplesdir}/resultsviewer/SmallOffice/)
#install(FILES ${CMAKE_BINARY_DIR}/resources/resultsviewer/SmallOffice/eplusout.sql
#        DESTINATION ${examplesdir}/resultsviewer/SmallOffice/)


# Install additional Documents, such as release notes
#install(FILES "${CMAKE_SOURCE_DIR}/../doc/ReleaseNotes/OpenStudio_Release_Notes_1_10_0_20151218.pdf" DESTINATION .)
install(FILES "${CMAKE_SOURCE_DIR}/../doc/ReleaseNotes/ReleaseNotes.md" DESTINATION .)

###############################################################################


if(UNIX AND NOT APPLE)
  # if we build on Linux and we built our own Qt, let's grab said Qt and install it
  if(CMAKE_PREFIX_PATH MATCHES ".*Qt-install.*")
    install(DIRECTORY "${CMAKE_BINARY_DIR}/../../../Qt-prefix/src/Qt-install/lib"
            DESTINATION .)
    install(DIRECTORY "${CMAKE_BINARY_DIR}/../../../Qt-prefix/src/Qt-install/share"
            DESTINATION .)
  endif()
endif()

if(NOT UNIX)
  #execute_process(COMMAND ${CMAKE_COMMAND} -E tar xfz "${CMAKE_SOURCE_DIR}/../dependencies/strawberry-perl-5.16.2.1-32bit-portable-reduced.tar.bz2"
  #  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
  #  RESULT_VARIABLE rv)

  #install(DIRECTORY "${CMAKE_BINARY_DIR}/strawberry-perl-5.16.2.1-32bit-portable-reduced" DESTINATION .)

  #set(CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS_SKIP ON)
  include(InstallRequiredSystemLibraries)
  if(CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS)
    install(FILES ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS}
      DESTINATION "bin")
  endif()
endif()

## Package Radiance
#set(RADIANCE_LOCATION "" CACHE PATH "Location of Radiance to Package")
#if(APPLE)
#  install(DIRECTORY "${RADIANCE_LOCATION}" DESTINATION "sharedresources" USE_SOURCE_PERMISSIONS)
#elseif(MSVC)
#  install(DIRECTORY "${RADIANCE_LOCATION}" DESTINATION "share/openstudio" USE_SOURCE_PERMISSIONS)
#else()
#  install(DIRECTORY "${RADIANCE_LOCATION}" DESTINATION "share/openstudio-${OPENSTUDIO_VERSION}" USE_SOURCE_PERMISSIONS)
#endif()
#
## Package EnergyPlus
#get_filename_component(ENERGYPLUS_DIR "${ENERGYPLUS_EXE}" DIRECTORY)
#if(APPLE)
#  install(DIRECTORY "${ENERGYPLUS_DIR}" DESTINATION "sharedresources" USE_SOURCE_PERMISSIONS)
#elseif(MSVC)
#  install(DIRECTORY "${ENERGYPLUS_DIR}" DESTINATION "share/openstudio" USE_SOURCE_PERMISSIONS)
#else()
#  install(DIRECTORY "${ENERGYPLUS_DIR}" DESTINATION "share/openstudio-${OPENSTUDIO_VERSION}" USE_SOURCE_PERMISSIONS)
#endif()
