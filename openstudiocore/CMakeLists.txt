cmake_minimum_required(VERSION 2.8.11)

project(OpenStudioCore)

cmake_policy(SET CMP0020 NEW)
###############################################################################
# Project version information
set(CMAKE_VERSION_BUILD "Unknown" CACHE STRING "Build number") # git sha

# OpenStudio version
set(OPENSTUDIO_VERSION "${CMAKE_VERSION_MAJOR}.${CMAKE_VERSION_MINOR}.${CMAKE_VERSION_PATCH}")
set(OPENSTUDIO_LONG_VERSION "${CMAKE_VERSION_MAJOR}.${CMAKE_VERSION_MINOR}.${CMAKE_VERSION_PATCH}.${CMAKE_VERSION_BUILD}")

# EnergyPlus Idd version
set(ENERGYPLUS_VERSION "8.1" CACHE INTERNAL "EnergyPlus Version")

# configure file with version information
configure_file(${CMAKE_SOURCE_DIR}/OpenStudio.in ${CMAKE_BINARY_DIR}/src/OpenStudio.hxx)

# universal configuration for mac plists
if(APPLE)
  set(MACOSX_BUNDLE_SHORT_VERSION_STRING "${OPENSTUDIO_VERSION}")
  set(MACOSX_BUNDLE_BUNDLE_VERSION "${CMAKE_VERSION_BUILD}")
  set(MACOSX_BUNDLE_COPYRIGHT "Copyright © 2014 National Renewable Energy Laboratory")
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
# Requires: EnergyPlus
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
  contam
  isomodel
  osversion
  ruleset
  runmanager/lib
  runmanager/app
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
  # all warnings
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -fPIC -fno-strict-aliasing -Winvalid-pch -Wnon-virtual-dtor")
  if(APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-overloaded-virtual -ftemplate-depth=1024")
  else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-overloaded-virtual")
  endif()
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -fPIC -fno-strict-aliasing -std=c11")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
  # effective c++
  #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Weffc++")
  # treat warnings as errors
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
  # all warnings
  #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Wall")
  # create compiler processes on all effective processors
  if(BUILD_WITH_MULTIPLE_PROCESSES)
    add_definitions(/MP)
  endif()
  # ignore decorated name length exceeded
  add_definitions(/wd4503)
  # ignore needs to have dll-interface to be used by clients of class
  # we will not rely on exporting templates instead all libraries must
  # be linked against the same runtime
  add_definitions(/wd4251)
  # treat warnings as errors

  if(NOT CMAKE_CL_64)
    add_definitions(/WX)
  else()
    add_definitions(/bigobj)
  endif()

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
find_package(EnergyPlus "${ENERGYPLUS_VERSION}" REQUIRED)
mark_as_advanced(
  ENERGYPLUS_EXE
  ENERGYPLUS_IDD
  ENERGYPLUS_WEATHER_DIR
)

# Ruby
find_program(RUBY_EXECUTABLE NAMES ruby PATHS $ENV{CMAKE_RUBY_PATH} NO_SYSTEM_ENVIRONMENT_PATH)
find_package(Ruby REQUIRED)


set(RUBY_MODULE_DIR "Ruby")

set(RUBY_MODULE_ARCH_DIR "Ruby/openstudio")

if(WIN32 AND MSVC AND CMAKE_CL_64)
  message(WARNING "64bit Support on Windows is EXPERIMENTAL. The SketchUp plugin will not work. Expect problems with ruby support in general. There are lots of warnings, so we are disabling warnings as errors.")

  if(NOT "${RUBY_VERSION_MAJOR}" EQUAL "2")
    message(SEND_ERROR "64bit Windows support requires Ruby >= 2.0.0")
  endif()
endif()

# Adding math.h definitions to avoid "inconsistent dll linkage" from Ruby 1.8 and VS2013
if(${MSVC} AND ${MSVC_VERSION} EQUAL 1800 AND RUBY_VERSION_MAJOR EQUAL 1 AND RUBY_VERSION_MINOR EQUAL 8)
  add_definitions(-DHAVE_ACOSH -DHAVE_ERF)
endif()


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
    if(WIN32)
      message(SEND_ERROR "You can install OpenSSL for windows by visiting http://www.slproweb.com/products/Win32OpenSSL.html and installing the latest recommended full installation package")
    else()
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



find_package(SWIG 2.0 REQUIRED)
mark_as_advanced(
  SWIG_DIR
  SWIG_EXECUTABLE
  SWIG_VERSION
)

set(ALL_RUBY_BINDING_TARGETS "") # global list

# build rdoc
if(BUILD_DOCUMENTATION)
  if(WIN32)
    find_file(RDOC NAMES rdoc.bat HINTS $ENV{PATH} "C:/Ruby/bin")
    find_file(RI NAMES ri.bat HINTS $ENV{PATH} "C:/Ruby/bin")
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
  find_package(PythonLibs 2.7 REQUIRED)
  include_directories(SYSTEM ${PYTHON_INCLUDE_DIRS})
  set(ALL_PYTHON_BINDING_TARGETS "") # global list
endif()

# Doxygen
if(BUILD_DOCUMENTATION)
  # need doxygen
  find_package(Doxygen REQUIRED)

  option(DOXYGEN_INLINE_INHERITED_MEMBERS_OPTION "Show all inherited members of a class as if those members were ordinary class members" OFF)

  mark_as_advanced(DOXYGEN_INLINE_INHERITED_MEMBERS_OPTION)

  if(DOXYGEN_INLINE_INHERITED_MEMBERS_OPTION)
    set(DOXYGEN_INLINE_INHERITED_MEMBERS "YES")
  else()
    set(DOXYGEN_INLINE_INHERITED_MEMBERS "NO")
  endif()

  # if have dot you have more options
  if(DOXYGEN_DOT_FOUND)

    option(DOXYGEN_COLLABORATION_GRAPH_OPTION "Generate implementation dependency diagrams (inheritance, containment, and class reference)" OFF)
    option(DOXYGEN_CLASS_GRAPH_OPTION "Graph direct and indirect inheritance relations" ON)
    option(DOXYGEN_UML_LOOK_OPTION "Generate graphs in UML format" OFF)
    option(DOXYGEN_GRAPHICAL_HIERARCHY_OPTION "Graphical hierarchy of all classes instead of a textual one" OFF)

    mark_as_advanced(
      DOXYGEN_COLLABORATION_GRAPH_OPTION
      DOXYGEN_CLASS_GRAPH_OPTION
      DOXYGEN_UML_LOOK_OPTION
      DOXYGEN_GRAPHICAL_HIERARCHY_OPTION
    )

    if(DOXYGEN_COLLABORATION_GRAPH_OPTION)
      set(DOXYGEN_COLLABORATION_GRAPH "YES")
    else()
      set(DOXYGEN_COLLABORATION_GRAPH "NO")
    endif()

    if(DOXYGEN_CLASS_GRAPH_OPTION)
      set(DOXYGEN_CLASS_GRAPH "YES")
    else()
      set(DOXYGEN_CLASS_GRAPH "NO")
    endif()

    if(DOXYGEN_UML_LOOK_OPTION)
      set(DOXYGEN_UML_LOOK "YES")
    else()
      set(DOXYGEN_UML_LOOK "NO")
    endif()

    if(DOXYGEN_GRAPHICAL_HIERARCHY_OPTION)
      set(DOXYGEN_GRAPHICAL_HIERARCHY "YES")
    else()
      set(DOXYGEN_GRAPHICAL_HIERARCHY "NO")
    endif()

  endif()

  configure_file(${CMAKE_SOURCE_DIR}/doc/index.html.in ${CMAKE_BINARY_DIR}/doc/index.html)

  set(doxygen_directories ${project_directories})
  # Add the idd files to the documentation list as a separate entry
  list(APPEND doxygen_directories utilities/idd)

  # ETH@20110823 In theory, doxygen tag files let you have links between individual projects.
  # In practice, the links generated do not quite work (at least in the verison of Doxygen that
  # I have on my desktop). I also didn't quite figure out how to make the custom _doc targets
  # dependent on each other. (I didn't try very hard since the tag links don't work right.)

  # Default TAGFILES
  set(DOXY_TAGFILES "")

  foreach(D ${doxygen_directories})
    string(REPLACE "/" "_" DOC_NAME ${D})
    if(${D} MATCHES "utilities/idd")
      # We want to build the idd documentation separately because of the generated files
      set(DOXY_INPUT_DIR "\"${CMAKE_SOURCE_DIR}/src/${D}\"")
      set(DOXY_ADDITIONAL_DIR "\"${CMAKE_BINARY_DIR}/src/${D}\"")
      set(DOXY_ADDITIONAL_FILES "")
      set(DOXY_EXCLUDE_DIR "")
      set(DOXY_DEPENDS "")
    elseif(${D} MATCHES "utilities")
      # Exclude the idd directory when building the utilities documentation
      set(DOXY_INPUT_DIR "\"${CMAKE_SOURCE_DIR}/src/${D}\"")
      set(DOXY_ADDITIONAL_DIR "")
      set(DOXY_ADDITIONAL_FILES "")
      set(DOXY_EXCLUDE_DIR "\"${CMAKE_SOURCE_DIR}/src/utilities/idd\"")
      #set(DOXY_TAGFILES "utilities_idd.tag")
    elseif(${D} MATCHES "model")
      set(DOXY_INPUT_DIR "\"${CMAKE_SOURCE_DIR}/src/${D}\"")
      set(DOXY_ADDITIONAL_DIR "")
      set(DOXY_ADDITIONAL_FILES "")
      set(DOXY_EXCLUDE_DIR "")
      #set(DOXY_TAGFILES "utilities_idd.tag utilities.tag")
    else()
      set(DOXY_INPUT_DIR "\"${CMAKE_SOURCE_DIR}/src/${D}\"")
      set(DOXY_ADDITIONAL_DIR "")
      set(DOXY_ADDITIONAL_FILES "")
      set(DOXY_EXCLUDE_DIR "")
    endif()
    configure_file(${CMAKE_SOURCE_DIR}/Doxyfile.in ${CMAKE_BINARY_DIR}/Doxyfile_${DOC_NAME})

    add_custom_target(openstudio_${DOC_NAME}_doc
      COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/doc/${DOC_NAME}
      COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/doc/${DOC_NAME}
      COMMAND ${DOXYGEN_EXECUTABLE} Doxyfile_${DOC_NAME}
      WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    )

    set(ALL_CPP_DOC_TARGETS ${ALL_CPP_DOC_TARGETS}
      openstudio_${DOC_NAME}_doc
    )
    #list(APPEND ALL_CPP_DOC_TARGETS "openstudio_${DOC_NAME}_doc")

  endforeach()


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

if(MSVC AND MSVC_IS_EXPRESS)
  if(CMAKE_CL_64)
    list(APPEND CMAKE_LIBRARY_PATH "C:/Program Files (x86)/Windows Kits/8.0/Lib/win8/um/x64/")
  else()
    list(APPEND CMAKE_LIBRARY_PATH "C:/Program Files (x86)/Windows Kits/8.0/Lib/win8/um/x86/")
    list(APPEND CMAKE_LIBRARY_PATH "C:/Program Files/Windows Kits/8.0/Lib/win8/um/x86/")
  endif()
endif()

find_package(Qt5Widgets REQUIRED)
find_package(Qt5Sql REQUIRED)
find_package(Qt5Network REQUIRED)
find_package(Qt5Xml REQUIRED)
find_package(Qt5WebKit REQUIRED)
find_package(Qt5WebKitWidgets REQUIRED)
#find_package(Qt5WinExtras)
find_package(Qt5Concurrent REQUIRED)
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
  Qt5WebKit_DIR
  Qt5WebKitWidgets_DIR
  Qt5Widgets_DIR
  #Qt5WinExtras_DIR
  Qt5Xml_DIR
)

set(CMAKE_AUTOMOC OFF)

#Adding libraries to the list of includes
include_directories(${Qt5Widgets_INCLUDE_DIRS})
include_directories(${Qt5Sql_INCLUDE_DIRS})
include_directories(${Qt5Network_INCLUDE_DIRS})
include_directories(${Qt5Xml_INCLUDE_DIRS})
include_directories(${Qt5WebKit_INCLUDE_DIRS})
include_directories(${Qt5WebKitWidgets_INCLUDE_DIRS})
#include_directories(${Qt5WinExtras_INCLUDE_DIRS})
include_directories(${Qt5Concurrent_INCLUDE_DIRS})

#include_directories is supposed to be a set, however there are tons of Qt duplicate includes
get_directory_property(dupdirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} INCLUDE_DIRECTORIES)
list(REMOVE_DUPLICATES dupdirs)
set_directory_properties(PROPERTIES INCLUDE_DIRECTORIES "${dupdirs}")

# list of all Qt libs and dependencies we are using
list(APPEND QT_LIBS ${Qt5Widgets_LIBRARIES})
list(APPEND QT_LIBS ${Qt5Sql_LIBRARIES})
list(APPEND QT_LIBS ${Qt5Network_LIBRARIES})
list(APPEND QT_LIBS ${Qt5Xml_LIBRARIES})
list(APPEND QT_LIBS ${Qt5WebKit_LIBRARIES})
list(APPEND QT_LIBS ${Qt5WebKitWidgets_LIBRARIES})
#list(APPEND QT_LIBS ${Qt5WinExtras_LIBRARIES})
list(APPEND QT_LIBS ${Qt5Concurrent_LIBRARIES})


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


# Set up embedded ruby for use with 'aws-sdk'

if(WIN32)
  set(AWS_RUBY_PATH ruby-2.0.0-aws-win32)
  set(AWS_RUBY_DESTINATION ".")
  set(AWS_RUBY_EXTENSION ".zip")

  set(AWS_RUBY_URL http://developer.nrel.gov/downloads/buildings/openstudio/src/ruby-2.0.0p353-aws-win32.zip)
  set(AWS_RUBY_CHECKSUM 1f6e4348099b26e92909db62dcd3b173)
elseif(APPLE)
  set(AWS_RUBY_PATH ruby-2.0.0-aws-Darwin)
  set(AWS_RUBY_DESTINATION ".")

  set(AWS_RUBY_URL http://developer.nrel.gov/downloads/buildings/openstudio/src/ruby-2.0.0p353-aws-Darwin.tar.bz2)
  set(AWS_RUBY_CHECKSUM 75963f2c6996af47a3ad69ac84d3d120)
  set(AWS_RUBY_EXTENSION ".tar.bz2")
elseif(UNIX AND CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64")
  set(AWS_RUBY_PATH ruby-2.0.0-aws-Linux-x86_64)
  set(AWS_RUBY_DESTINATION "lib/openstudio-${OPENSTUDIO_VERSION}")

  set(AWS_RUBY_URL http://developer.nrel.gov/downloads/buildings/openstudio/src/ruby-2.0.0p353-aws-Linux-x86_64.tar.bz2)
  set(AWS_RUBY_CHECKSUM 74d3a8ab4e1e3fb44c308f2aaa8e5732)
  set(AWS_RUBY_EXTENSION ".tar.bz2")
elseif(UNIX)
  set(AWS_RUBY_PATH ruby-2.0.0-aws-Linux-i686)
  set(AWS_RUBY_DESTINATION "lib/openstudio-${OPENSTUDIO_VERSION}")

  set(AWS_RUBY_URL http://developer.nrel.gov/downloads/buildings/openstudio/src/ruby-2.0.0p353-aws-Linux-i686.tar.bz2)
  set(AWS_RUBY_CHECKSUM e51ce64b3e03ed3a66f1f7e0a22b5b7e)
  set(AWS_RUBY_EXTENSION ".tar.bz2")
endif()

file(DOWNLOAD ${AWS_RUBY_URL} "${CMAKE_BINARY_DIR}/${AWS_RUBY_PATH}${AWS_RUBY_EXTENSION}" TIMEOUT 120 INACTIVITY_TIMEOUT 120)
execute_process(COMMAND ${CMAKE_COMMAND} -E remove_directory "${CMAKE_BINARY_DIR}/${AWS_RUBY_PATH}")
execute_process(COMMAND ${CMAKE_COMMAND} -E tar xfz "${CMAKE_BINARY_DIR}/${AWS_RUBY_PATH}${AWS_RUBY_EXTENSION}" WORKING_DIRECTORY "${CMAKE_BINARY_DIR}")
execute_process(COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_HOME_DIRECTORY}/ruby/aws-sdk-patches/connection_pool.rb" "${CMAKE_BINARY_DIR}/${AWS_RUBY_PATH}/lib/ruby/gems/2.0.0/gems/aws-sdk-1.31.3/lib/aws/core/http")
install(DIRECTORY "${CMAKE_BINARY_DIR}/${AWS_RUBY_PATH}" DESTINATION "${AWS_RUBY_DESTINATION}")

# end ruby aws-sdk info

foreach(D ${project_directories})
  add_subdirectory(src/${D})
endforeach()

# Make sure resultsviewer has its resources built
add_dependencies(ResultsViewer ResultsViewer_resources)

# ruby, after loading projects
add_subdirectory(src/ruby_cpp)
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


###############################################################################
# Targets that combine all documentation targets
if(BUILD_DOCUMENTATION)

  add_custom_target(ALL_DOXYGEN)
  add_dependencies(ALL_DOXYGEN ${ALL_CPP_DOC_TARGETS})

  add_custom_target(ALL_RDOC)
  add_dependencies(ALL_RDOC ${ALL_RDOC_TARGETS})

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
  openstudio_isomodel
  openstudio_lib
  openstudio_sdd
  openstudio_contam
  ${QTWINMIGRATE_EXPORT}
  zkexpat
  zkqwt
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
  RunManager
  ruby_cpp
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
install(FILES "${CMAKE_SOURCE_DIR}/../doc/ReleaseNotes/OpenStudio_Release_Notes_1_4_0_20140620.pdf" DESTINATION .)

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

  get_filename_component(RubyEXEPath "${RUBY_EXECUTABLE}" PATH)
  get_filename_component(RubyPath "${RubyEXEPath}" PATH)
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




if(PACKAGE_RADIANCE_AND_ENERGYPLUS)
  set(ENERGYPLUS_LOCATION "" CACHE PATH "Location of EnergyPlus to Package")
  set(RADIANCE_LOCATION "" CACHE PATH "Location of Radiance to Package")

  file(COPY "${ENERGYPLUS_LOCATION}" DESTINATION "${CMAKE_BINARY_DIR}/EnergyPlus/")
  file(COPY "${RADIANCE_LOCATION}" DESTINATION "${CMAKE_BINARY_DIR}/Radiance/")

  if(APPLE)
    install(DIRECTORY "${CMAKE_BINARY_DIR}/EnergyPlus/" DESTINATION "sharedresources" USE_SOURCE_PERMISSIONS)
    install(DIRECTORY "${CMAKE_BINARY_DIR}/Radiance/" DESTINATION "sharedresources" USE_SOURCE_PERMISSIONS)
  else()
    install(DIRECTORY "${CMAKE_BINARY_DIR}/EnergyPlus/" DESTINATION "share/openstudio-${OPENSTUDIO_VERSION}" USE_SOURCE_PERMISSIONS)
    install(DIRECTORY "${CMAKE_BINARY_DIR}/Radiance/" DESTINATION "share/openstudio-${OPENSTUDIO_VERSION}" USE_SOURCE_PERMISSIONS)
  endif()

endif()
