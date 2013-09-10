CMAKE_MINIMUM_REQUIRED( VERSION 2.8 )
 
PROJECT(OpenStudioCore)

###############################################################################
# Project version information
  # We are not currently using the svn revision info because it is proving to be unreliable
  # svn revision info
  #find_package(Subversion)
  #if(Subversion_FOUND)
  #  Subversion_WC_INFO(${CMAKE_CURRENT_SOURCE_DIR} OS_REV)
  #  SET(SUBVERSION_REVISION ${OS_REV_WC_REVISION})
  #endif(Subversion_FOUND)
    SET(SUBVERSION_REVISION "Unknown")
SET(CMAKE_VERSION_BUILD "${SUBVERSION_REVISION}" CACHE STRING "Build number") # svn revision

# OpenStudio version
SET(OPENSTUDIO_VERSION "${CMAKE_VERSION_MAJOR}.${CMAKE_VERSION_MINOR}.${CMAKE_VERSION_PATCH}")

# EnergyPlus Idd version 
SET(ENERGYPLUS_VERSION "8.0")

# Dakota version
SET(DAKOTA_VERSION "5.3.1")

# configure file with version information
CONFIGURE_FILE( ${CMAKE_SOURCE_DIR}/OpenStudio.in ${CMAKE_BINARY_DIR}/src/OpenStudio.hxx )

# universal configuration for mac plists
IF( APPLE )
  SET(MACOSX_BUNDLE_SHORT_VERSION_STRING "${OPENSTUDIO_VERSION}")
  SET(MACOSX_BUNDLE_BUNDLE_VERSION "${CMAKE_VERSION_BUILD}")
  SET(MACOSX_BUNDLE_COPYRIGHT "Copyright © 2013 National Renewable Energy Laboratory")
ENDIF()

###############################################################################

###############################################################################
# Build options

IF( UNIX AND NOT APPLE )
  # the RPATH to be used when installing
  SET(CMAKE_INSTALL_RPATH "$ORIGIN/../lib")
ENDIF()

# Build C++ documentation using Doxygen
# Requires: doxygen
OPTION( BUILD_DOCUMENTATION "Build Documentation" OFF )

# Build CSharp bindings
# Requires: SWIG
OPTION( BUILD_CSHARP_BINDINGS "Build CSharp bindings" OFF )

# Build V8 Node bindings
# Requires: SWIG, Node
OPTION( BUILD_V8_BINDINGS "Build Node V8 bindings" OFF )

# Build Python bindings
# Requires: SWIG Python
OPTION( BUILD_PYTHON_BINDINGS "Build Python bindings" OFF )

# Build ctest testing
# Requires: EnergyPLus
OPTION( BUILD_TESTING "Build testing targets" OFF )

# Build package
# Requires: EnergyPlus
OPTION( BUILD_PACKAGE "Build package" OFF )

# Build test runner targets.
# This is a convenience for Visual Studio users
OPTION( ENABLE_TEST_RUNNER_TARGETS "Create test runner targets" OFF )

# Build with OpenSSL support
OPTION( BUILD_WITH_OPENSSL "Build With OpenSSL Support For SSH Connections" ON )

# Build SimXML
OPTION( BUILD_SIMXML "Build with SimXml support" OFF )

# Build SDD
OPTION( BUILD_SDD "Build with SDD support" ON )

# Build R package
OPTION( BUILD_R "Build R package" OFF )

# Build Ruby gem
OPTION( BUILD_RUBY_GEM "Build Ruby Gem" OFF )

IF (BUILD_RUBY_GEM)
  FIND_PROGRAM( GEM NAMES gem )
  IF( NOT GEM )
    MESSAGE(SEND_ERROR "gem tool not found, cannot build gem")
  ENDIF()

  # Build Ruby gem and package up all the libs
  OPTION( BUILD_RUBY_GEM_WITH_LIBS "Package all required libs with the Ruby Gem" ON )
ENDIF()

IF (BUILD_V8_BINDINGS)
  OPTION( BUILD_NODE_MODULES "Build V8 Bindings as Node Modules" ON )
ENDIF()

# Require that DAKOTA_FOUND
#
# Some code (certain analysisdriver tests) are conditionally compiled depending on 
# the value of this variable.
#
# Dakota is an optional third party algorithm library that we use through its command line
# interface.
OPTION( REQUIRE_DAKOTA "Ensure Dakota (third party analysis tool) is found" OFF )
MARK_AS_ADVANCED( REQUIRE_DAKOTA )

# Use PCH
OPTION( USE_PCH "Use precompiled headers" OFF )

# Enable support for boost > 1.42 ish by forcing boost_filesystem_version 2
ADD_DEFINITIONS(-DBOOST_FILESYSTEM_VERSION=2)


IF(MSVC)
  # Build with Multiple Processes
  OPTION( BUILD_WITH_MULTIPLE_PROCESSES "/MP compiler flag for full processor utilization" ON )
  MARK_AS_ADVANCED( BUILD_WITH_MULTIPLE_PROCESSES )
ENDIF()

###############################################################################

###############################################################################
# Sub projects

# Each subfolder of src is a major sub-project
SET( project_directories
  generateiddfactory
  utilities
  model  
  energyplus
  radiance
  gbxml
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
  plugin
  openstudio_app
  pat_app
)

IF( BUILD_SDD )
  LIST(APPEND project_directories sdd)
ENDIF( BUILD_SDD ) 

###############################################################################

###############################################################################
# CMake control  

# High level project configuration
SET( CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Products" )
SET( CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Products" )
SET( CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Products" )

SET ( LIBRARY_SEARCH_DIRECTORY "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}" "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/Release" "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/Debug" )

# Include project specific macros
INCLUDE(ProjectMacros.cmake)

# Search for modules in the root dir first to override cmake ones 
LIST(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR})

# Use CTest
IF( BUILD_TESTING )
  ENABLE_TESTING()
  INCLUDE(CTest) 
ENDIF( BUILD_TESTING )
###############################################################################

###############################################################################
# Compiler and system specific options
IF(UNIX)
  # all warnings
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -fPIC -fno-strict-aliasing -Winvalid-pch")
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-overloaded-virtual")
  SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -fPIC -fno-strict-aliasing --std=c99")
  # effective c++
  #SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Weffc++")
  # treat warnings as errors
  #SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
ENDIF(UNIX)
IF(MINGW)
  # all warnings
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
  # effective c++
  #SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Weffc++")
  # treat warnings as errors
  #SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
  
  # wchar not supported for MinGW
  ADD_DEFINITIONS(-DBOOST_LOG_USE_CHAR)
  ADD_DEFINITIONS(-DBOOST_LOG_NO_COMPILER_TLS)
ENDIF(MINGW)
IF(MSVC)
  # warning level 3
  SET(CMAKE_CXX_WARNING_LEVEL 3)
  # all warnings
  #SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Wall")
  # create compiler processes on all effective processors
  IF(BUILD_WITH_MULTIPLE_PROCESSES)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
  ENDIF(BUILD_WITH_MULTIPLE_PROCESSES)
  # ignore first seen as struct now using class
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4099")
  # ignore decorated name length exceeded
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4503")
  # ignore needs to have dll-interface to be used by clients of class
  # we will not rely on exporting templates instead all libraries must 
  # be linked against the same runtime
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4251")
  # treat warnings as errors
  
  IF(NOT CMAKE_CL_64)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /WX")
  ELSE()
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /bigobj")
  ENDIF()
  
  # ignore warnings about the stl being insecure
  ADD_DEFINITIONS(-D_CRT_SECURE_NO_WARNINGS -D_SCL_SECURE_NO_WARNINGS)
  # only build eefg dll on msvc
  LIST(APPEND project_directories eefg)
ENDIF(MSVC)
###############################################################################


###############################################################################
# Check version of gcc
IF (CMAKE_COMPILER_IS_GNUCC)
  EXECUTE_PROCESS(COMMAND ${CMAKE_C_COMPILER} -dumpversion
                  OUTPUT_VARIABLE GCC_VERSION)
ENDIF()



###############################################################################
# Conditionally required dependencies 

# EnergyPlus
#IF( BUILD_TESTING OR BUILD_PACKAGE)
  FIND_PACKAGE(EnergyPlus "${ENERGYPLUS_VERSION}" REQUIRED)
  MARK_AS_ADVANCED(
    ENERGYPLUS_EXE
    ENERGYPLUS_IDD
    ENERGYPLUS_WEATHER_DIR
  )
#ENDIF( BUILD_TESTING OR BUILD_PACKAGE )

# OpenSSL
IF( ${BUILD_WITH_OPENSSL} )

  # OpenSSL / libssh support
  FIND_PACKAGE( OpenSSL )

  IF( NOT ${OPENSSL_FOUND} )
    MESSAGE(SEND_ERROR "OpenSSL could not be found, building without ssh support for BCL and remote jobs")
    IF ( WIN32 )
      MESSAGE(SEND_ERROR "You can install OpenSSL for windows by visiting http://www.slproweb.com/products/Win32OpenSSL.html and installing the latest recommended full installation package")
    ELSE()
      MESSAGE(SEND_ERROR "Please install OpenSSL development libraries using your package management system (possibly libssl-dev)")
    ENDIF()
  ENDIF()

  #enable libssh
  ADD_DEFINITIONS(-DLIBSSH_STATIC)

  # add sub directory
  ADD_SUBDIRECTORY( src/libssh )
  INCLUDE_DIRECTORIES( SYSTEM src/libssh/include/ )

ELSE()
  MESSAGE(WARNING "Building without ssh support for BCL and remote jobs")
ENDIF()

# Ruby
FIND_PROGRAM( RUBY_EXECUTABLE NAMES ruby PATHS $ENV{CMAKE_RUBY_PATH} NO_SYSTEM_ENVIRONMENT_PATH )
FIND_PACKAGE( Ruby REQUIRED )


SET(RUBY_MODULE_DIR "Ruby")

SET(RUBY_MODULE_ARCH_DIR "Ruby/openstudio")

IF(WIN32 AND MSVC AND CMAKE_CL_64)
  MESSAGE(WARNING "64bit Support on Windows is EXPERIMENTAL. The Sketchup plugin will not work. Expect problems with ruby support in general. There are lots of warnings, so we are disabling warnings as errors.") 

  IF (NOT "${RUBY_VERSION_MAJOR}" EQUAL "2")
    MESSAGE(SEND_ERROR "64bit Windows support requires Ruby >= 2.0.0")
  ENDIF()
ENDIF()


#We want to use the site_ruby folder if this is intended for /usr/local install
#If it is intended for /usr install, we want to use the main ruby module folder
IF( ${CMAKE_INSTALL_PREFIX} STREQUAL "/usr" AND RUBY_ARCH_DIR MATCHES "^/usr/lib.*/.*")
  STRING(REGEX REPLACE "^/usr/" "" RUBY_MODULE_ARCH_DIR ${RUBY_ARCH_DIR} )
  STRING(REGEX REPLACE "^/usr/" "" RUBY_MODULE_DIR ${RUBY_RUBY_LIB_DIR} )
ELSEIF( ${CMAKE_INSTALL_PREFIX} STREQUAL "/usr/local" AND RUBY_SITEARCH_DIR MATCHES "^/usr/local/lib.*/.*" )
  STRING(REGEX REPLACE "^/usr/local/" "" RUBY_MODULE_ARCH_DIR ${RUBY_SITEARCH_DIR} )
  STRING(REGEX REPLACE "^/usr/local/" "" RUBY_MODULE_DIR ${RUBY_SITELIB_DIR} )
ENDIF()

MESSAGE(STATUS "Ruby module code installing to: ${RUBY_MODULE_DIR}")
MESSAGE(STATUS "Ruby module arch library installing to: ${RUBY_MODULE_ARCH_DIR}")

FIND_PACKAGE( SWIG 1.3.40 REQUIRED )
MARK_AS_ADVANCED(  
  SWIG_DIR
  SWIG_EXECUTABLE
  SWIG_VERSION
)

SET( ALL_RUBY_BINDING_TARGETS "" ) # global list

# build rdoc
IF( BUILD_DOCUMENTATION )
  IF(WIN32)
    FIND_FILE( RDOC NAMES rdoc.bat HINTS $ENV{PATH} "C:/Ruby/bin" )
    FIND_FILE( RI NAMES ri.bat HINTS $ENV{PATH} "C:/Ruby/bin"  )
  ELSE(WIN32)
    FIND_PROGRAM( RDOC NAMES rdoc )
    FIND_PROGRAM( RI NAMES ri )
  ENDIF(WIN32)
  
  MARK_AS_ADVANCED(
    RDOC
    RI
  ) 
  
  SET( ALL_RDOC_TARGETS ${ALL_RUBY_BINDING_TARGETS} )
ENDIF( BUILD_DOCUMENTATION )

# C#
IF ( BUILD_CSHARP_BINDINGS )
  SET( ALL_CSHARP_BINDING_TARGETS "" ) # global list
ENDIF()

IF(BUILD_NODE_MODULES)
  ADD_DEFINITIONS(-DBUILD_NODE_MODULE)
ENDIF()

IF( BUILD_PYTHON_BINDINGS )
  # need python
  Find_Package( PythonLibs 2.7 REQUIRED )
  INCLUDE_DIRECTORIES( SYSTEM ${PYTHON_INCLUDE_DIRS} )
  SET( ALL_PYTHON_BINDING_TARGETS "" ) # global list
ENDIF()

IF (REQUIRE_DAKOTA) 
  FIND_PACKAGE(Dakota "${DAKOTA_VERSION}" REQUIRED)
  MARK_AS_ADVANCED(DAKOTA_EXE)
ELSE (REQUIRE_DAKOTA)
  # If DAKOTA_FOUND not yet set, and DAKOTA_FOUND comes back false, emit a message
  SET(EMIT_DAKOTA_WARNING false)
  IF (NOT DEFINED DAKOTA_FOUND)
    SET(EMIT_DAKOTA_WARNING true)
  ENDIF(NOT DEFINED DAKOTA_FOUND)
  FIND_PACKAGE(Dakota "${DAKOTA_VERSION}")
  IF (DAKOTA_FOUND)
    MARK_AS_ADVANCED(DAKOTA_EXE)
  ELSE (DAKOTA_FOUND)
    IF (EMIT_DAKOTA_WARNING)
      MESSAGE(STATUS "DAKOTA was not found on your system. openstudio_analysisdriver_tests that depend on Dakota will not be built.")
    ENDIF(EMIT_DAKOTA_WARNING)
  ENDIF(DAKOTA_FOUND)
ENDIF(REQUIRE_DAKOTA)
SET(DAKOTA_FOUND ${DAKOTA_FOUND} CACHE INTERNAL "Boolean indicating whether Dakota was found.")

# Doxygen
IF( BUILD_DOCUMENTATION )
  # need doxygen
  Find_Package( Doxygen REQUIRED )
  
  OPTION( DOXYGEN_INLINE_INHERITED_MEMBERS_OPTION "Show all inherited members of a class as if those members were ordinary class members" OFF )
  
  MARK_AS_ADVANCED(
    DOXYGEN_INLINE_INHERITED_MEMBERS_OPTION
  )
  
  IF( DOXYGEN_INLINE_INHERITED_MEMBERS_OPTION )
    SET( DOXYGEN_INLINE_INHERITED_MEMBERS "YES")
  ELSE()
    SET( DOXYGEN_INLINE_INHERITED_MEMBERS "NO")
  ENDIF()
  
  # if have dot you have more options
  IF(DOXYGEN_DOT_FOUND)
  
    OPTION( DOXYGEN_COLLABORATION_GRAPH_OPTION "Generate implementation dependency diagrams (inheritance, containment, and class reference)" OFF )
    OPTION( DOXYGEN_CLASS_GRAPH_OPTION "Graph direct and indirect inheritance relations" ON )
    OPTION( DOXYGEN_UML_LOOK_OPTION "Generate graphs in UML format" OFF )
    OPTION( DOXYGEN_GRAPHICAL_HIERARCHY_OPTION "Graphical hierarchy of all classes instead of a textual one" OFF )
    
    MARK_AS_ADVANCED( 
      DOXYGEN_COLLABORATION_GRAPH_OPTION
      DOXYGEN_CLASS_GRAPH_OPTION
      DOXYGEN_UML_LOOK_OPTION
      DOXYGEN_GRAPHICAL_HIERARCHY_OPTION
    )
    
    IF( DOXYGEN_COLLABORATION_GRAPH_OPTION )
      SET( DOXYGEN_COLLABORATION_GRAPH "YES")
    ELSE()
      SET( DOXYGEN_COLLABORATION_GRAPH "NO")
    ENDIF()
      
    IF( DOXYGEN_CLASS_GRAPH_OPTION )
      SET( DOXYGEN_CLASS_GRAPH "YES")
    ELSE()
      SET( DOXYGEN_CLASS_GRAPH "NO")
    ENDIF() 
   
    IF( DOXYGEN_UML_LOOK_OPTION )
      SET( DOXYGEN_UML_LOOK "YES")
    ELSE()
      SET( DOXYGEN_UML_LOOK "NO")
    ENDIF()
    
    IF( DOXYGEN_GRAPHICAL_HIERARCHY_OPTION )
      SET ( DOXYGEN_GRAPHICAL_HIERARCHY "YES" )
    ELSE()
      SET ( DOXYGEN_GRAPHICAL_HIERARCHY "NO" )
    ENDIF()
    
  ENDIF()
  
  CONFIGURE_FILE(${CMAKE_SOURCE_DIR}/doc/index.html.in ${CMAKE_BINARY_DIR}/doc/index.html)

  SET( doxygen_directories ${project_directories} )
  # Add the idd files to the documentation list as a separate entry
  LIST( APPEND doxygen_directories utilities/idd )

  # ETH@20110823 In theory, doxygen tag files let you have links between individual projects.
  # In practice, the links generated do not quite work (at least in the verison of Doxygen that
  # I have on my desktop). I also didn't quite figure out how to make the custom _doc targets
  # dependent on each other. (I didn't try very hard since the tag links don't work right.)

  # Default TAGFILES
  SET( DOXY_TAGFILES "" )
  
  FOREACH( D ${doxygen_directories} )
    STRING( REPLACE "/" "_" DOC_NAME ${D} )
    IF( ${D} MATCHES "utilities/idd" )
      # We want to build the idd documentation seperately because of the generated files
      SET( DOXY_INPUT_DIR "\"${CMAKE_SOURCE_DIR}/src/${D}\"" )
      SET( DOXY_ADDITIONAL_DIR "\"${CMAKE_BINARY_DIR}/src/${D}\"" )
      SET( DOXY_ADDITIONAL_FILES "" )
      SET( DOXY_EXCLUDE_DIR "" )
      SET( DOXY_DEPENDS "" )
    ELSEIF( ${D} MATCHES "utilities" )
      # Exclude the idd directory when building the utilites documentation
      SET( DOXY_INPUT_DIR "\"${CMAKE_SOURCE_DIR}/src/${D}\"" )
      SET( DOXY_ADDITIONAL_DIR "" )
      SET( DOXY_ADDITIONAL_FILES "" )
      SET( DOXY_EXCLUDE_DIR "\"${CMAKE_SOURCE_DIR}/src/utilities/idd\"" )
#      SET( DOXY_TAGFILES "utilities_idd.tag" )
    ELSEIF( ${D} MATCHES "model" )
      SET( DOXY_INPUT_DIR "\"${CMAKE_SOURCE_DIR}/src/${D}\"" )
      SET( DOXY_ADDITIONAL_DIR "" )
      SET( DOXY_ADDITIONAL_FILES "" )
      SET( DOXY_EXCLUDE_DIR "" )
#      SET( DOXY_TAGFILES "utilities_idd.tag utilities.tag" )      
    ELSEIF( ${D} MATCHES "standardsinterface" )
      SET( DOXY_INPUT_DIR "\"${CMAKE_SOURCE_DIR}/src/${D}\"" )
      SET( DOXY_ADDITIONAL_DIR "\"${CMAKE_BINARY_DIR}/src/${D}\"" )
      SET( DOXY_ADDITIONAL_FILES "" )
      SET( DOXY_EXCLUDE_DIR "" )
#      SET( DOXY_TAGFILES "utilities_idd.tag utilities.tag model.tag" )
    ELSE()
      SET( DOXY_INPUT_DIR "\"${CMAKE_SOURCE_DIR}/src/${D}\"" )
      SET( DOXY_ADDITIONAL_DIR "" )
      SET( DOXY_ADDITIONAL_FILES "" )
      SET( DOXY_EXCLUDE_DIR "" )
    ENDIF()
    CONFIGURE_FILE( ${CMAKE_SOURCE_DIR}/Doxyfile.in ${CMAKE_BINARY_DIR}/Doxyfile_${DOC_NAME} )
    
    ADD_CUSTOM_TARGET( openstudio_${DOC_NAME}_doc
      COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/doc/${DOC_NAME}
      COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/doc/${DOC_NAME}
      COMMAND ${DOXYGEN_EXECUTABLE} Doxyfile_${DOC_NAME}
      WORKING_DIRECTORY ${CMAKE_BINARY_DIR}  
    )
  
    set( ALL_CPP_DOC_TARGETS ${ALL_CPP_DOC_TARGETS}
      openstudio_${DOC_NAME}_doc
      )
    #LIST( APPEND ALL_CPP_DOC_TARGETS "openstudio_${DOC_NAME}_doc" )
  
  ENDFOREACH()

  
ENDIF()
###############################################################################

###############################################################################
# Required dependencies 

# Threading library
FIND_PACKAGE(Threads)
IF( UNIX )
  SET(CMAKE_THREAD_LIBS "${CMAKE_THREAD_LIBS_INIT}" CACHE STRING "Thread library used.")
  MARK_AS_ADVANCED( CMAKE_THREAD_LIBS )
ENDIF( UNIX )

# Zlib
INCLUDE_DIRECTORIES( SYSTEM src/zlib )
ADD_SUBDIRECTORY( src/zlib )

# Google test library
ADD_DEFINITIONS( -DGTEST_HAS_TR1_TUPLE=0 )
INCLUDE_DIRECTORIES( SYSTEM ${GTEST_INCLUDE_DIR} )
link_directories( ${GTEST_LIB_DIR} )

# Boost
IF(APPLE)
  set(Boost_USE_STATIC_LIBS ON)  
ENDIF()
IF (WIN32)
  set(Boost_USE_STATIC_LIBS ON)  

  #uncomment all of this if we want to force dynamic libs on windows
#  set(Boost_USE_STATIC_LIBS OFF)  
#  ADD_DEFINITIONS( -DBOOST_THREAD_USE_DLL -DBOOST_THREAD_DYN_LINK -DBOOST_PROGRAM_OPTIONS_DYN_LINK  -DBOOST_REGEX_DYN_LINK  -DBOOST_FILESYSTEM_DYN_LINK -DBOOST_SYSTEM_DYN_LINK -DBOOST_SERIALIZATION_DYN_LINK  -DBOOST_DATE_TIME_DYN_LINK)
#  LINK_DIRECTORIES( ${Boost_LIBRARY_DIRS} )
#  IF (MSVC)
#    #Ignore dll specific warnings that are out of our hands to control, coming from external projects
#    ADD_DEFINITIONS( "/wd4251 /wd4275" )
#  ENDIF (MSVC)
ENDIF (WIN32)

MARK_AS_ADVANCED(Boost_DIR)

SET( Boost_ADDITIONAL_VERSIONS "1.40" )
FIND_PACKAGE( Boost 1.40.0 REQUIRED COMPONENTS filesystem regex program_options system thread date_time serialization)
INCLUDE_DIRECTORIES(SYSTEM ${Boost_INCLUDE_DIR} )
MARK_AS_ADVANCED( Boost_LIB_DIAGNOSTIC_DEFINITIONS )

# include after other boost include directories for cases where boost version < 1.47
INCLUDE_DIRECTORIES( SYSTEM ${CMAKE_SOURCE_DIR}/../dependencies/boost_geometry )

# require these run time checks
add_definitions(-DBOOST_UBLAS_CHECK_ENABLE)
add_definitions(-DBOOST_UBLAS_TYPE_CHECK)
add_definitions(-DBOOST_UBLAS_NO_STD_CERR)

# Boost-log
ADD_DEFINITIONS( -DBOOST_LOG_NO_LIB ) # disable autolinking
ADD_DEFINITIONS( -DBOOST_LOG_SETUP_NO_LIB ) # disable autolinking

find_package(BoostLog REQUIRED)
INCLUDE_DIRECTORIES( SYSTEM ${BoostLog_INCLUDE_DIRS} )
LINK_DIRECTORIES( ${BoostLog_LIBRARY_DIRS} )

# QJSON 

find_package(QJSON REQUIRED)
INCLUDE_DIRECTORIES( SYSTEM ${QJSON_INCLUDE_DIR} )
LINK_DIRECTORIES( ${QJSON_LIBRARY_DIRS} )

# SQLite definitions, used in sqlite and litesql
ADD_DEFINITIONS( -DSQLITE_THREADSAFE=1 ) # 1 is default, serial access
#ADD_DEFINITIONS( -DSQLITE_DEBUG )
#ADD_DEFINITIONS( -DSQLITE_MEMDEBUG )

#--------------------------------------------------------------------------------
# Qt
FIND_PACKAGE (Qt4 4.6.0 COMPONENTS QtCore QtGui QtSql QtNetwork QtXml QtXmlPatterns REQUIRED)

# Newer versions of cmake have a sqlite plugin finder built into the Qt package finder.  
# This is for older versions.  Sometime before cmake version 2.8.7.
IF( NOT QT_QSQLITE_PLUGIN_RELEASE )
  SET(CMAKE_FIND_LIBRARY_SUFFIXES_DEFAULT ${CMAKE_FIND_LIBRARY_SUFFIXES})
  SET(CMAKE_FIND_LIBRARY_SUFFIXES ${CMAKE_SHARED_MODULE_SUFFIX} ${CMAKE_SHARED_LIBRARY_SUFFIX})
  FIND_LIBRARY( QT_QSQLITE_PLUGIN_RELEASE NAMES qsqlite qsqlite4 HINTS "${QT_PLUGINS_DIR}/sqldrivers" )
  SET(CMAKE_FIND_LIBRARY_SUFFIXES ${CMAKE_FIND_LIBRARY_SUFFIXES_DEFAULT})
ENDIF()

INCLUDE_DIRECTORIES(SYSTEM ${QT_INCLUDE_DIR} )
#LIST( APPEND CMAKE_PROGRAM_PATH ${QT_LIBRARY_DIR})
#LIST( APPEND CMAKE_LIBRARY_PATH ${QT_LIBRARY_DIR})
#--------------------------------------------------------------------------------
# QT_USE_FILE is a variable defined by FIND_PACKAGE( Qt4 ) that contains a path to
# a CMake script that sets up this directory for building with Qt4
SET(QT_INCLUDE_DIRS_NO_SYSTEM false)

INCLUDE( ${QT_USE_FILE} )
MARK_AS_ADVANCED( QT_QMAKE_EXECUTABLE QT_QTMOTIF_INCLUDE_DIR QT_QTMOTIF_LIBRARY_DEBUG QT_QTMOTIF_LIBRARY_RELEASE )

# This sets the windows build that will need the special winmain@16 call. Qt provides
#  this for us in the qtmain.lib file. Using this cmake code will ensure we have it
#  linked into our build. Not needed on Unix/OS X/Linux which is why we have the 
#  IF(WIN32) conditional.
IF (WIN32)
    SET (QT_USE_QTMAIN TRUE)
ENDIF (WIN32)

#--------------------------------------------------------------------------------
# QT_USE_FILE is a variable defined by FIND_PACKAGE( Qt4 ) that contains a path to
# a CMake script that sets up this directory for building with Qt4
INCLUDE( ${QT_USE_FILE} )

#Adding libraries to the list of includes
INCLUDE_DIRECTORIES( SYSTEM ${QT_QTGUI_INCLUDE_DIR} )
INCLUDE_DIRECTORIES( SYSTEM ${QT_QTSQL_INCLUDE_DIR} )
INCLUDE_DIRECTORIES( SYSTEM ${QT_QTNETWORK_INCLUDE_DIR} )
INCLUDE_DIRECTORIES( SYSTEM ${QT_QTXML_INCLUDE_DIR} )
INCLUDE_DIRECTORIES( SYSTEM ${QT_QTXMLPATTERNS_INCLUDE_DIR} )
INCLUDE_DIRECTORIES( SYSTEM ${QT_QTWEBKIT_INCLUDE_DIR} )

# list of all Qt libs and dependencies we are using
LIST( APPEND QT_LIBS ${QT_QTMAIN_LIBRARY})
LIST( APPEND QT_LIBS ${QT_QTCORE_LIBRARY})
LIST( APPEND QT_LIBS ${QT_QTCORE_LIB_DEPENDENCIES})
LIST( APPEND QT_LIBS ${QT_QTGUI_LIBRARY})
LIST( APPEND QT_LIBS ${QT_QTGUI_LIB_DEPENDENCIES})
LIST( APPEND QT_LIBS ${QT_QTSQL_LIBRARY})
LIST( APPEND QT_LIBS ${QT_QTSQL_LIB_DEPENDENCIES})
LIST( APPEND QT_LIBS ${QT_QTNETWORK_LIBRARY})
LIST( APPEND QT_LIBS ${QT_QTNETWORK_LIB_DEPENDENCIES})
LIST( APPEND QT_LIBS ${QT_QTXML_LIBRARY})
LIST( APPEND QT_LIBS ${QT_QTXML_LIB_DEPENDENCIES})
LIST( APPEND QT_LIBS ${QT_QTXMLPATTERNS_LIBRARY})
LIST( APPEND QT_LIBS ${QT_QTXMLPATTERNS_LIB_DEPENDENCIES})

#QT_QSQLITE_PLUGIN_RELEASE


# VTK #######################
find_package(VTK REQUIRED)

include(${VTK_USE_FILE})
##############################

# vtkCharts ##################
find_library( VTK_SIMPLE_CHARTS_RELEASE_LIB 
  NAME vtkSimpleCharts 
  PATHS ${VTK_CHARTS_LIB_DIR}
        ${VTK_CHARTS_LIB_DIR}/Release
  NO_DEFAULT_PATH )

find_library( VTK_SIMPLE_CHARTS_DEBUG_LIB 
  NAME vtkSimpleCharts 
  PATHS ${VTK_CHARTS_LIB_DIR}
        ${VTK_CHARTS_LIB_DIR}/Debug
  NO_DEFAULT_PATH )

add_definitions(-DvtkCharts_USE_QT)

include_directories(SYSTEM ${VTK_CHARTS_INCLUDE_DIR})

link_directories( ${VTK_CHARTS_LIB_DIR} )
##############################







###############################################################################
# qtwinmigrate
IF(WIN32)
  add_subdirectory( src/qtwinmigrate )
  include_directories( src/qtwinmigrate )
ENDIF(WIN32)
###############################################################################

###############################################################################
# qwt
add_subdirectory( src/qwt )
###############################################################################

###############################################################################
## expat
add_subdirectory( src/expat )
###############################################################################

################################################################################
## litesql
include_directories( SYSTEM src/litesql/include src/litesql/library ${CMAKE_BINARY_DIR}/litesql )
add_subdirectory( src/litesql )
################################################################################

################################################################################
## sqlite
add_subdirectory( src/sqlite )
################################################################################

###############################################################################
# Add to include path

# Project source directory
INCLUDE_DIRECTORIES( "${CMAKE_HOME_DIRECTORY}/src/" )

# Project binary directory
INCLUDE_DIRECTORIES( "${CMAKE_BINARY_DIR}/src/" )
###############################################################################

###############################################################################
# Add project sub directories

# Include the resources, should happen after find EnergyPlus
ADD_SUBDIRECTORY(resources)


# Set up embedded ruby for use with 'aws-sdk'

IF (WIN32)
  SET(AWS_RUBY_PATH ruby-2.0.0-aws-win32)
  SET(AWS_RUBY_DESTINATION ".")
  SET(AWS_RUBY_EXTENSION ".zip")

  SET(AWS_RUBY_URL http://developer.nrel.gov/downloads/buildings/openstudio/src/ruby-2.0.0-aws-win32.zip)
  SET(AWS_RUBY_CHECKSUM 47a865d1be6c03d09cc0fc652adb53f9)
ELSEIF(APPLE)
  SET(AWS_RUBY_PATH ruby-2.0.0-aws-Darwin)
  SET(AWS_RUBY_DESTINATION ".")

  SET(AWS_RUBY_URL http://developer.nrel.gov/downloads/buildings/openstudio/src/ruby-2.0.0-aws-Darwin.tar.bz2)
  SET(AWS_RUBY_CHECKSUM ffc7dea105f3b6e3215ab2a32e7127ce)
  SET(AWS_RUBY_EXTENSION ".tar.bz2")

  #  SET(AWS_RUBY_URL http://developer.nrel.gov/downloads/buildings/openstudio/src/ruby-2.0.0-aws-Darwin.tar.gz)
  #  SET(AWS_RUBY_CHECKSUM 60e404dd025ce65ca583bb7e671e1c09)
  #  SET(AWS_RUBY_EXTENSION ".tar.gz")
ELSEIF(UNIX)
  SET(AWS_RUBY_PATH ruby-2.0.0-aws-Linux-x86_64)
  SET(AWS_RUBY_DESTINATION "lib/openstudio")

  SET(AWS_RUBY_URL http://developer.nrel.gov/downloads/buildings/openstudio/src/ruby-2.0.0-aws-Linux-x86_64.tar.bz2)
  SET(AWS_RUBY_CHECKSUM b2f8b2a41be2a63eca58b73dab908493)
  SET(AWS_RUBY_EXTENSION ".tar.bz2")

  #  SET(AWS_RUBY_URL http://developer.nrel.gov/downloads/buildings/openstudio/src/ruby-2.0.0-aws-Linux-x86_64.tar.gz)
  #  SET(AWS_RUBY_CHECKSUM c1636829b77859d77c272c6d6da766cf)
  #  SET(AWS_RUBY_EXTENSION ".tar.gz")
ENDIF()

FILE(DOWNLOAD ${AWS_RUBY_URL} "${CMAKE_BINARY_DIR}/${AWS_RUBY_PATH}${AWS_RUBY_EXTENSION}" TIMEOUT 120 INACTIVITY_TIMEOUT 120 EXPECTED_MD5 ${AWS_RUBY_CHECKSUM})
execute_process(COMMAND ${CMAKE_COMMAND} -E remove_directory "${CMAKE_BINARY_DIR}/${AWS_RUBY_PATH}")
EXECUTE_PROCESS(COMMAND ${CMAKE_COMMAND} -E tar xfz "${CMAKE_BINARY_DIR}/${AWS_RUBY_PATH}${AWS_RUBY_EXTENSION}" WORKING_DIRECTORY "${CMAKE_BINARY_DIR}")
INSTALL( DIRECTORY "${CMAKE_BINARY_DIR}/${AWS_RUBY_PATH}" DESTINATION "${AWS_RUBY_DESTINATION}" )

# end ruby aws-sdk info

FOREACH( D ${project_directories} )
  ADD_SUBDIRECTORY( src/${D} )
ENDFOREACH()

# Make sure resultsviewer has its resources built
ADD_DEPENDENCIES( ResultsViewer ResultsViewer_resources )

# ruby, after loading projects
ADD_SUBDIRECTORY( src/ruby_cpp )
ADD_SUBDIRECTORY( ruby )

# build epw2wea for supporting gendaymtx
ADD_SUBDIRECTORY( src/epw2wea )

# csharp, after loading projects
IF(BUILD_CSHARP_BINDINGS)
  ADD_SUBDIRECTORY( csharp )
ENDIF(BUILD_CSHARP_BINDINGS)

# v8-node
IF(BUILD_NODE_MODULES)
  ADD_SUBDIRECTORY( v8-node )
ENDIF()

# R, after loading projects
IF(BUILD_R)
  ADD_SUBDIRECTORY( r-scripts )
ENDIF(BUILD_R)

###############################################################################

###############################################################################
# Targets that combine all bindings targets
ADD_CUSTOM_TARGET( ALL_RUBY_BINDINGS )
ADD_DEPENDENCIES( ALL_RUBY_BINDINGS ${ALL_RUBY_BINDING_TARGETS} )

if ( BUILD_PYTHON_BINDINGS )
  ADD_CUSTOM_TARGET( ALL_PYTHON_BINDINGS )
  ADD_DEPENDENCIES( ALL_PYTHON_BINDINGS ${ALL_PYTHON_BINDING_TARGETS} )
endif()


if ( BUILD_CSHARP_BINDINGS )
  ADD_CUSTOM_TARGET( ALL_CSHARP_BINDINGS )
  ADD_DEPENDENCIES( ALL_CSHARP_BINDINGS csharp_sdk ${ALL_CSHARP_BINDING_TARGETS} )
endif ( BUILD_CSHARP_BINDINGS )

IF(BUILD_V8_BINDINGS)
  ADD_CUSTOM_TARGET( ALL_V8_BINDINGS )
  ADD_DEPENDENCIES( ALL_V8_BINDINGS ${ALL_V8_BINDING_TARGETS} )
ENDIF(BUILD_V8_BINDINGS)


###############################################################################
# Targets that combine all documentation targets
if( BUILD_DOCUMENTATION )

  ADD_CUSTOM_TARGET( ALL_DOXYGEN )
  ADD_DEPENDENCIES( ALL_DOXYGEN ${ALL_CPP_DOC_TARGETS} )

  ADD_CUSTOM_TARGET( ALL_RDOC )
  ADD_DEPENDENCIES( ALL_RDOC ${ALL_RDOC_TARGETS} )

endif( BUILD_DOCUMENTATION )


###############################################################################
# Export targets and generate OpenStudioCoreConfig.cmake

set(TARGETS_FILE ${CMAKE_BINARY_DIR}/OpenStudioTargets.cmake)

IF( BUILD_WITH_OPENSSL )
  SET(LIBSSH_EXPORT "libssh_static")
ELSE( BUILD_WITH_OPENSSL )
  SET(LIBSSH_EXPORT "")
ENDIF( BUILD_WITH_OPENSSL )

IF( WIN32 )
  SET(QTWINMIGRATE_EXPORT "qtwinmigrate")
ELSE( WIN32 )
  SET(QTWINMIGRATE_EXPORT "")
ENDIF( WIN32 )

export(TARGETS openstudio_runmanager 
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
               openstudio_lib
               openstudio_plugin
               openstudio_sdd
               ${LIBSSH_EXPORT}
               ${QTWINMIGRATE_EXPORT}
               zkexpat
               zkqwt
               miniziplib
               sqlite
               litesql
               litesql-util
               ${ALL_RUBY_BINDING_TARGETS}
       FILE "${TARGETS_FILE}")

configure_file("${CMAKE_SOURCE_DIR}/OpenStudioCoreConfig.cmake.in"
  "${CMAKE_BINARY_DIR}/OpenStudioCoreConfig.cmake" @ONLY)


IF( UNIX AND NOT APPLE ) 
  SET(examplesdir share/openstudio/examples)
ELSE()
  set(examplesdir Examples)
ENDIF()
  
# Used by some Ruby tests, remove eventually
INSTALL(FILES "${CMAKE_SOURCE_DIR}/resources/resultsviewer/SmallOffice/SmallOffice.idf"
        DESTINATION "${examplesdir}/resultsviewer/SmallOffice/" )
# Unnecessary files or just left out for size?
#INSTALL(FILES ${CMAKE_BINARY_DIR}/resources/resultsviewer/SmallOffice/eplustbl.htm
#        DESTINATION ${examplesdir}/resultsviewer/SmallOffice/ )
#INSTALL(FILES ${CMAKE_BINARY_DIR}/resources/resultsviewer/SmallOffice/eplusout.sql
#        DESTINATION ${examplesdir}/resultsviewer/SmallOffice/ )


# Install additional Documents, such as release notes
install( FILES "${CMAKE_SOURCE_DIR}/../doc/ReleaseNotes/OpenStudio_Release_Notes_1_0_0_20130628.pdf" DESTINATION . )

###############################################################################


IF (UNIX AND NOT APPLE)
  # if we build on Linux and we built our own Qt, let's grab said Qt and install it
  IF(QT_QMAKE_EXECUTABLE MATCHES ".*Qt-install.*")
    INSTALL(DIRECTORY "${CMAKE_BINARY_DIR}/../../../Qt-prefix/src/Qt-install/lib" 
            DESTINATION .)
    INSTALL(DIRECTORY "${CMAKE_BINARY_DIR}/../../../Qt-prefix/src/Qt-install/share" 
            DESTINATION .)
  ENDIF()
ENDIF()

IF (NOT UNIX)
  execute_process(COMMAND ${CMAKE_COMMAND} -E tar xfz "${CMAKE_SOURCE_DIR}/../dependencies/strawberry-perl-5.16.2.1-32bit-portable-reduced.tar.bz2"
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    RESULT_VARIABLE rv)
    
  install( DIRECTORY "${CMAKE_BINARY_DIR}/strawberry-perl-5.16.2.1-32bit-portable-reduced" DESTINATION . )
  
  execute_process(COMMAND ${CMAKE_COMMAND} -E remove_directory "${CMAKE_BINARY_DIR}/ruby-install")
  
  execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/ruby-install"
    RESULT_VARIABLE rv)
  
  execute_process(COMMAND ${CMAKE_COMMAND} -E tar xfz "${CMAKE_SOURCE_DIR}/../dependencies/ruby-1.8.6-msvc.zip"
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/ruby-install"
    RESULT_VARIABLE rv)
    
  install( DIRECTORY "${CMAKE_BINARY_DIR}/ruby-install" DESTINATION . )

  SET(CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS_SKIP ON)
  INCLUDE(InstallRequiredSystemLibraries)
  IF(CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS)
    INSTALL(FILES ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS}
      DESTINATION "bin")

    # Install the system libraries to the ruby dir
    INSTALL(FILES ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS}
      DESTINATION "Ruby/openstudio")
  ENDIF(CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS)
ENDIF()

