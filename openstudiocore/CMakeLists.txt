cmake_minimum_required(VERSION 2.8.11)

project(OpenStudioCore)

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
if(NOT ENERGYPLUS_VERSION_MAJOR)
  set(ENERGYPLUS_VERSION_MAJOR 8)
endif()
if(NOT ENERGYPLUS_VERSION_MINOR)
  set(ENERGYPLUS_VERSION_MINOR 3)
endif()
if(NOT ENERGYPLUS_VERSION_PATCH)
  set(ENERGYPLUS_VERSION_PATCH 0)
endif()
set(ENERGYPLUS_VERSION "${ENERGYPLUS_VERSION_MAJOR}.${ENERGYPLUS_VERSION_MINOR}.${ENERGYPLUS_VERSION_PATCH}")
# Build SHA is not required to have a value, but if it does OpenStudio will require this build.
if(NOT ENERGYPLUS_BUILD_SHA)
  set(ENERGYPLUS_BUILD_SHA "6d97d074ea")
endif()

# configure file with version information
configure_file(${CMAKE_SOURCE_DIR}/OpenStudio.in ${CMAKE_BINARY_DIR}/src/OpenStudio.hxx)

# universal configuration for mac plists
if(APPLE)
  set(MACOSX_BUNDLE_SHORT_VERSION_STRING "${OPENSTUDIO_VERSION}")
  set(MACOSX_BUNDLE_BUNDLE_VERSION "${CMAKE_VERSION_BUILD}")
  set(MACOSX_BUNDLE_COPYRIGHT "Copyright © 2015 National Renewable Energy Laboratory")
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

# Build test runner targets.
# This is a convenience for Visual Studio users
option(ENABLE_TEST_RUNNER_TARGETS "Create test runner targets" OFF)

# Build with OpenSSL support
set(BUILD_WITH_OPENSSL ON CACHE INTERNAL "Build With OpenSSL Support For SSH Connections")

# Build R package
option(BUILD_R "Build R package" OFF)

# Build Ruby gem
option(BUILD_RUBY_GEM "Build Ruby Gem" OFF)


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

if(UNIX AND NOT APPLE AND BUILD_TESTING)
  # Enable libgtest linking
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lpthread")
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
# Sub projects

# Each subfolder of src is a major sub-project
set(project_directories
  generateiddfactory
  utilities
  model
  energyplus
  radiance
  gbxml
  bimserver
  airflow
  isomodel
  osversion
  ruleset
  runmanager/lib
  analysis
  project
  analysisdriver
  resultsviewer
  model_editor
  openstudio_lib
  openstudio_app
  pat_app
  sdd
)


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
  add_definitions(/WX)

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
set(ENV{ENERGYPLUSDIR} ${ENERGYPLUS_LOCATION})
find_package(EnergyPlus "${ENERGYPLUS_VERSION}" REQUIRED)

# Ruby
find_program(RUBY_EXECUTABLE NAMES ruby PATHS $ENV{CMAKE_RUBY_PATH} NO_SYSTEM_ENVIRONMENT_PATH)

find_package(Ruby 2)

if(NOT RUBY_FOUND)
  find_path(RUBY_CONFIG_INCLUDE_DIR
    NAMES ruby/config.h config.h
    HINTS
    /usr/include/x86_64-linux-gnu/ruby-2.0.0
  )
  find_package(Ruby 2.0 REQUIRED)
endif()

set(RUBY_MODULE_DIR "Ruby")

set(RUBY_MODULE_ARCH_DIR "Ruby/openstudio")

#We want to use the site_ruby folder if this is intended for /usr/local install
#If it is intended for /usr install, we want to use the main ruby module folder
if(${CMAKE_INSTALL_PREFIX} STREQUAL "/usr" AND RUBY_ARCH_DIR MATCHES "^/usr/lib.*/.*")
  string(REGEX REPLACE "^/usr/" "" RUBY_MODULE_ARCH_DIR ${RUBY_ARCH_DIR})
  string(REGEX REPLACE "^/usr/" "" RUBY_MODULE_DIR ${RUBY_RUBY_LIB_DIR})
elseif(${CMAKE_INSTALL_PREFIX} STREQUAL "/usr/local" AND RUBY_SITEARCH_DIR MATCHES "^/usr/local/lib.*/.*")
  string(REGEX REPLACE "^/usr/local/" "" RUBY_MODULE_ARCH_DIR ${RUBY_SITEARCH_DIR})
  string(REGEX REPLACE "^/usr/local/" "" RUBY_MODULE_DIR ${RUBY_SITELIB_DIR})
endif()

if(UNIX AND NOT APPLE)
  set(RUBY_MODULE_SITE_DIR "${RUBY_MODULE_DIR}")

  set(RUBY_MODULE_DIR "lib/openstudio-${OPENSTUDIO_VERSION}/ruby/${RUBY_VERSION_MAJOR}.${RUBY_VERSION_MINOR}")
  set(RUBY_MODULE_ARCH_DIR "lib/openstudio-${OPENSTUDIO_VERSION}/ruby/${RUBY_VERSION_MAJOR}.${RUBY_VERSION_MINOR}")
endif()

message(STATUS "Ruby module code installing to: ${RUBY_MODULE_DIR}")
message(STATUS "Ruby module arch library installing to: ${RUBY_MODULE_ARCH_DIR}")


# OpenSSL / libssh support
if(BUILD_WITH_OPENSSL)

  find_package(OpenSSL)
  if(NOT ${OPENSSL_FOUND})
    message(SEND_ERROR "OpenSSL could not be found, and is required for HTTPS communication")
    if(UNIX)
      message(SEND_ERROR "Please install OpenSSL development libraries using your package management system (possibly libssl-dev)")
    endif()
  elseif(WIN32)
    set(LIBEAY32_PATH "${OPENSSL_ROOT_DIR}/libeay32.dll")
    set(SSLEAY32_PATH "${OPENSSL_ROOT_DIR}/ssleay32.dll")
    file(COPY "${LIBEAY32_PATH}" "${SSLEAY32_PATH}" DESTINATION "${CMAKE_BINARY_DIR}/Products/Release/")
    file(COPY "${LIBEAY32_PATH}" "${SSLEAY32_PATH}" DESTINATION "${CMAKE_BINARY_DIR}/Products/Debug/")
    file(COPY "${LIBEAY32_PATH}" "${SSLEAY32_PATH}" DESTINATION "${CMAKE_BINARY_DIR}/Products/RelWithDebInfo/")
    file(COPY "${LIBEAY32_PATH}" "${SSLEAY32_PATH}" DESTINATION "${CMAKE_BINARY_DIR}/Products/MinSizeRel/")
    install(FILES "${LIBEAY32_PATH}" "${SSLEAY32_PATH}" DESTINATION "bin")
    install(FILES "${LIBEAY32_PATH}" "${SSLEAY32_PATH}" DESTINATION ${RUBY_MODULE_ARCH_DIR})
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
###############################################################################

###############################################################################
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

# Google test library
add_definitions(-DGTEST_HAS_TR1_TUPLE=0)
include_directories(SYSTEM ${GTEST_INCLUDE_DIR})
link_directories(${GTEST_LIB_DIR})

# Boost
if(APPLE)
  set(Boost_USE_STATIC_LIBS ON)
elseif(WIN32)
  set(Boost_USE_STATIC_LIBS ON)

  # uncomment all of this if we want to force dynamic libs on windows
  #set(Boost_USE_STATIC_LIBS OFF)
  #add_definitions(-DBOOST_THREAD_USE_DLL -DBOOST_THREAD_DYN_LINK -DBOOST_PROGRAM_OPTIONS_DYN_LINK  -DBOOST_REGEX_DYN_LINK  -DBOOST_FILESYSTEM_DYN_LINK -DBOOST_SYSTEM_DYN_LINK -DBOOST_DATE_TIME_DYN_LINK)
  #link_directories(${Boost_LIBRARY_DIRS})
  #if(MSVC)
    ##Ignore dll specific warnings that are out of our hands to control, coming from external projects
    #add_definitions(/wd4251 /wd4275)
  #endif()
endif()

mark_as_advanced(
  Boost_DIR
  BOOST_THREAD_LIBRARY
)

find_package(Boost 1.55.0 REQUIRED COMPONENTS filesystem regex program_options system thread date_time log)
include_directories(SYSTEM ${Boost_INCLUDE_DIR})

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

# check if boost is being linked dynamically
# DLM: should we allow this on Unix or should we force static for all platforms?
string(REGEX MATCH "^.*libboost_log.so$" BOOST_IS_DYN_LIBS "${Boost_LOG_LIBRARY_RELEASE}")
if(BOOST_IS_DYN_LIBS)
  add_definitions(-DBOOST_ALL_DYN_LINK)
endif()

# SQLite definitions, used in sqlite and litesql
add_definitions(-DSQLITE_THREADSAFE=1) # 1 is default, serial access
#add_definitions(-DSQLITE_DEBUG)
#add_definitions(-DSQLITE_MEMDEBUG)

#--------------------------------------------------------------------------------
# Qt

find_package(Qt5Widgets REQUIRED)
find_package(Qt5Sql REQUIRED)
find_package(Qt5Network REQUIRED)
find_package(Qt5Xml REQUIRED)
#find_package(Qt5WinExtras)
find_package(Qt5Concurrent REQUIRED)
find_package(Qt5PrintSupport REQUIRED)
mark_as_advanced(
  Qt5Concurrent_DIR
  Qt5Core_DIR
  Qt5Gui_DIR
  Qt5Gui_EGL_LIBRARY
  Qt5Location_DIR
  Qt5Multimedia_DIR
  Qt5MultimediaWidgets_DIR
  Qt5Network_DIR
  Qt5OpenGL_DIR
  Qt5Positioning_DIR
  Qt5PrintSupport_DIR
  Qt5Qml_DIR
  Qt5Quick_DIR
  Qt5Sensors_DIR
  Qt5Sql_DIR
  Qt5Widgets_DIR
  #Qt5WinExtras_DIR
  Qt5Xml_DIR
)

if(${Qt5Widgets_VERSION} VERSION_LESS "5.4.0")
  find_package(Qt5WebKit REQUIRED)
  find_package(Qt5WebKitWidgets REQUIRED)
  mark_as_advanced(Qt5WebKit_DIR Qt5WebKitWidgets_DIR)
else()
  find_package(Qt5WebEngine REQUIRED)
  find_package(Qt5WebEngineWidgets REQUIRED)
  mark_as_advanced(Qt5WebEngine_DIR Qt5WebEngineWidgets_DIR)
endif()

set(CMAKE_AUTOMOC OFF)

#Adding libraries to the list of includes
include_directories(${Qt5Widgets_INCLUDE_DIRS})
include_directories(${Qt5Sql_INCLUDE_DIRS})
include_directories(${Qt5Network_INCLUDE_DIRS})
include_directories(${Qt5Xml_INCLUDE_DIRS})
if(${Qt5Widgets_VERSION} VERSION_LESS "5.4.0")
  include_directories(${Qt5WebKit_INCLUDE_DIRS})
  include_directories(${Qt5WebKitWidgets_INCLUDE_DIRS})
else()
  include_directories(${Qt5WebEngine_INCLUDE_DIRS})
  include_directories(${Qt5WebEngineWidgets_INCLUDE_DIRS})
endif()
#include_directories(${Qt5WinExtras_INCLUDE_DIRS})
include_directories(${Qt5Concurrent_INCLUDE_DIRS})
include_directories(${Qt5PrintSupport_INCLUDE_DIRS})

#include_directories is supposed to be a set, however there are tons of Qt duplicate includes
get_directory_property(dupdirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} INCLUDE_DIRECTORIES)
list(REMOVE_DUPLICATES dupdirs)
set_directory_properties(PROPERTIES INCLUDE_DIRECTORIES "${dupdirs}")

# list of all Qt libs and dependencies we are using
list(APPEND QT_LIBS ${Qt5Widgets_LIBRARIES})
list(APPEND QT_LIBS ${Qt5Sql_LIBRARIES})
list(APPEND QT_LIBS ${Qt5Network_LIBRARIES})
list(APPEND QT_LIBS ${Qt5Xml_LIBRARIES})
if(${Qt5Widgets_VERSION} VERSION_LESS "5.4.0")
  list(APPEND QT_LIBS ${Qt5WebKit_LIBRARIES})
  list(APPEND QT_LIBS ${Qt5WebKitWidgets_LIBRARIES})
else()
  list(APPEND QT_LIBS ${Qt5WebEngine_LIBRARIES})
  list(APPEND QT_LIBS ${Qt5WebEngineWidgets_LIBRARIES})
endif()
#list(APPEND QT_LIBS ${Qt5WinExtras_LIBRARIES})
list(APPEND QT_LIBS ${Qt5Concurrent_LIBRARIES})
list(APPEND QT_LIBS ${Qt5PrintSupport_LIBRARIES})

### recreating QT_LIBRARY_DIR for qt5
if(WIN32 AND TARGET Qt5::Core)
  get_property(_Qt5_Core_LOCATION TARGET Qt5::Core PROPERTY LOCATION)
  get_filename_component(QT_LIBRARY_DIR "${_Qt5_Core_LOCATION}" DIRECTORY)
else()
  set(QT_LIBRARY_DIR "")
endif()


###############################################################################
# qtwinmigrate
if(WIN32)
  add_subdirectory(src/qtwinmigrate)
  include_directories(src/qtwinmigrate)
endif()
###############################################################################

###############################################################################
# qwt
add_subdirectory(src/qwt)
###############################################################################

###############################################################################
## expat
add_subdirectory(src/expat)
###############################################################################

################################################################################
## litesql
include_directories(SYSTEM src/litesql/include src/litesql/library ${CMAKE_BINARY_DIR}/litesql)
add_subdirectory(src/litesql)
################################################################################

################################################################################
## sqlite
add_subdirectory(src/sqlite)
################################################################################

###############################################################################
# Add to include path

# Project source directory
include_directories("${CMAKE_HOME_DIRECTORY}/src/")

# Project binary directory
include_directories("${CMAKE_BINARY_DIR}/src/")
###############################################################################

###############################################################################
# Add project sub directories

# Include the resources, should happen after find EnergyPlus
add_subdirectory(resources)


# Set up embedded ruby for Unix use with 'aws-sdk'

if(UNIX)
  if(APPLE)
    set(AWS_RUBY_PATH ruby-2.0.0-aws-Darwin)
    set(AWS_RUBY_DESTINATION ".")
    set(AWS_RUBY_URL http://openstudio-resources.s3.amazonaws.com/dependencies/ruby-2.0.0-p645-aws-Darwin.tar.bz2)
    set(AWS_RUBY_CHECKSUM 49bceb609b5c1d6d41f824115af46ec4)
  elseif(CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64")
    set(AWS_RUBY_PATH ruby-2.0.0-aws-Linux-x86_64)
    set(AWS_RUBY_DESTINATION "lib/openstudio-${OPENSTUDIO_VERSION}")
    set(AWS_RUBY_URL http://openstudio-resources.s3.amazonaws.com/dependencies/ruby-2.0.0-p645-aws-Linux-x86_64.tar.bz2)
    set(AWS_RUBY_CHECKSUM c46c9e93770749208222aa43ca99fe62)
  endif()

  file(DOWNLOAD ${AWS_RUBY_URL} "${CMAKE_BINARY_DIR}/${AWS_RUBY_PATH}.tar.bz2" TIMEOUT 120 INACTIVITY_TIMEOUT 120)
  execute_process(COMMAND ${CMAKE_COMMAND} -E remove_directory "${CMAKE_BINARY_DIR}/${AWS_RUBY_PATH}")
  execute_process(COMMAND ${CMAKE_COMMAND} -E tar xfz "${CMAKE_BINARY_DIR}/${AWS_RUBY_PATH}.tar.bz2" WORKING_DIRECTORY "${CMAKE_BINARY_DIR}")

  # Install required gems
  set(ENV{BUNDLE_PATH} "${CMAKE_BINARY_DIR}/${AWS_RUBY_PATH}/lib/ruby/gems/2.0.0")
  execute_process(COMMAND "${CMAKE_BINARY_DIR}/${AWS_RUBY_PATH}/bin/ruby" "${CMAKE_BINARY_DIR}/${AWS_RUBY_PATH}/bin/bundle" "install" "--no-cache"
                  WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/../dependencies/ruby"
                  TIMEOUT 120
  )
  execute_process(COMMAND "${CMAKE_BINARY_DIR}/${AWS_RUBY_PATH}/bin/ruby" "${CMAKE_BINARY_DIR}/${AWS_RUBY_PATH}/bin/bundle" "clean" "--force"
                  WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/../dependencies/ruby"
                  TIMEOUT 120
  )

  install(
    DIRECTORY "${CMAKE_BINARY_DIR}/${AWS_RUBY_PATH}"
    DESTINATION "${AWS_RUBY_DESTINATION}"
    USE_SOURCE_PERMISSIONS
  )
endif()

# end ruby aws-sdk info

foreach(D ${project_directories})
  add_subdirectory(src/${D})
endforeach()

# Make sure resultsviewer has its resources built
add_dependencies(ResultsViewer ResultsViewer_resources)

# ruby, after loading projects
add_subdirectory(ruby)

if(UNIX AND NOT APPLE)

  file(RELATIVE_PATH relative "${CMAKE_INSTALL_PREFIX}/${RUBY_MODULE_SITE_DIR}" "${CMAKE_INSTALL_PREFIX}/${RUBY_MODULE_DIR}")

  install(CODE "
    execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory \"\$ENV{DESTDIR}/\${CMAKE_INSTALL_PREFIX}/${RUBY_MODULE_SITE_DIR}\")
    execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink \"${relative}/openstudio.rb\" \"\$ENV{DESTDIR}/\${CMAKE_INSTALL_PREFIX}/${RUBY_MODULE_SITE_DIR}/openstudio.rb\")
    execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink \"${relative}/openstudio\" \"\$ENV{DESTDIR}/\${CMAKE_INSTALL_PREFIX}/${RUBY_MODULE_SITE_DIR}/openstudio\")
    execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink \"${relative}/cloud\" \"\$ENV{DESTDIR}/\${CMAKE_INSTALL_PREFIX}/${RUBY_MODULE_SITE_DIR}/cloud\")
  ")

endif()

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

if(WIN32)
  set(QTWINMIGRATE_EXPORT "qtwinmigrate")
else()
  set(QTWINMIGRATE_EXPORT "")
endif()

set(all_lib_targets
  openstudio_runmanager
  openstudio_project
  openstudio_osversion
  openstudio_model
  openstudio_modeleditor
  openstudio_utilities
  openstudio_energyplus
  openstudio_analysisdriver
  openstudio_ruleset
  openstudio_analysis
  openstudio_radiance
  openstudio_gbxml
  openstudio_bimserver
  openstudio_isomodel
  openstudio_lib
  openstudio_sdd
  openstudio_airflow
  ${QTWINMIGRATE_EXPORT}
  zkexpat
  qwt
  miniziplib
  sqlite
  litesql
  litesql-util
  ${ALL_RUBY_BINDING_TARGETS}
)

export(TARGETS
  ${all_lib_targets}
  FILE "${TARGETS_FILE}"
)

set(all_exe_targets
  GenerateIddFactory
  gbXMLToIdf
  OpenStudio
  litesql-gen
  ResultsViewer
  Pat
)

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
install(FILES "${CMAKE_SOURCE_DIR}/resources/resultsviewer/SmallOffice/SmallOffice.idf"
        DESTINATION "${examplesdir}/resultsviewer/SmallOffice/")
# Unnecessary files or just left out for size?
#install(FILES ${CMAKE_BINARY_DIR}/resources/resultsviewer/SmallOffice/eplustbl.htm
#        DESTINATION ${examplesdir}/resultsviewer/SmallOffice/)
#install(FILES ${CMAKE_BINARY_DIR}/resources/resultsviewer/SmallOffice/eplusout.sql
#        DESTINATION ${examplesdir}/resultsviewer/SmallOffice/)


# Install additional Documents, such as release notes
install(FILES "${CMAKE_SOURCE_DIR}/../doc/ReleaseNotes/OpenStudio_Release_Notes_1_9_0_20150925.pdf" DESTINATION .)

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
  execute_process(COMMAND ${CMAKE_COMMAND} -E tar xfz "${CMAKE_SOURCE_DIR}/../dependencies/strawberry-perl-5.16.2.1-32bit-portable-reduced.tar.bz2"
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    RESULT_VARIABLE rv)

  install(DIRECTORY "${CMAKE_BINARY_DIR}/strawberry-perl-5.16.2.1-32bit-portable-reduced" DESTINATION .)

  get_filename_component(RubyEXEPath "${RUBY_EXECUTABLE}" DIRECTORY)
  get_filename_component(RubyPath "${RubyEXEPath}" DIRECTORY)
  install(DIRECTORY "${RubyPath}/" DESTINATION "ruby-install/ruby")

  set(CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS_SKIP ON)
  include(InstallRequiredSystemLibraries)
  if(CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS)
    install(FILES ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS}
      DESTINATION "bin")

    # Install the system libraries to the ruby dir
    install(FILES ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS}
      DESTINATION "Ruby/openstudio")

    install(FILES ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS}
      DESTINATION "ruby-install/ruby/bin")
  endif()
endif()

# Package Radiance
set(RADIANCE_LOCATION "" CACHE PATH "Location of Radiance to Package")
if(APPLE)
  install(DIRECTORY "${RADIANCE_LOCATION}" DESTINATION "sharedresources" USE_SOURCE_PERMISSIONS)
elseif(MSVC)
  install(DIRECTORY "${RADIANCE_LOCATION}" DESTINATION "share/openstudio" USE_SOURCE_PERMISSIONS)
else()
  install(DIRECTORY "${RADIANCE_LOCATION}" DESTINATION "share/openstudio-${OPENSTUDIO_VERSION}" USE_SOURCE_PERMISSIONS)
endif()

# Package EnergyPlus
get_filename_component(ENERGYPLUS_DIR "${ENERGYPLUS_EXE}" DIRECTORY)
if(APPLE)
  install(DIRECTORY "${ENERGYPLUS_DIR}" DESTINATION "sharedresources" USE_SOURCE_PERMISSIONS)
elseif(MSVC)
  install(DIRECTORY "${ENERGYPLUS_DIR}" DESTINATION "share/openstudio" USE_SOURCE_PERMISSIONS)
else()
  install(DIRECTORY "${ENERGYPLUS_DIR}" DESTINATION "share/openstudio-${OPENSTUDIO_VERSION}" USE_SOURCE_PERMISSIONS)
endif()
