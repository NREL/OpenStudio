CMAKE_MINIMUM_REQUIRED( VERSION 2.8.7 )
 
PROJECT(OpenStudioCore)

CMAKE_POLICY(SET CMP0020 NEW)
###############################################################################
# Project version information
SET(CMAKE_VERSION_BUILD "Unknown" CACHE STRING "Build number") # git sha

# OpenStudio version
SET(OPENSTUDIO_VERSION "${CMAKE_VERSION_MAJOR}.${CMAKE_VERSION_MINOR}.${CMAKE_VERSION_PATCH}")
SET(OPENSTUDIO_LONG_VERSION "${CMAKE_VERSION_MAJOR}.${CMAKE_VERSION_MINOR}.${CMAKE_VERSION_PATCH}.${CMAKE_VERSION_BUILD}")

# EnergyPlus Idd version 
SET(ENERGYPLUS_VERSION "8.1" CACHE INTERNAL "EnergyPlus Version")

# Dakota version
SET(DAKOTA_VERSION "5.3.1")

# configure file with version information
CONFIGURE_FILE( ${CMAKE_SOURCE_DIR}/OpenStudio.in ${CMAKE_BINARY_DIR}/src/OpenStudio.hxx )

# universal configuration for mac plists
IF( APPLE )
  SET(MACOSX_BUNDLE_SHORT_VERSION_STRING "${OPENSTUDIO_VERSION}")
  SET(MACOSX_BUNDLE_BUNDLE_VERSION "${CMAKE_VERSION_BUILD}")
  SET(MACOSX_BUNDLE_COPYRIGHT "Copyright © 2014 National Renewable Energy Laboratory")
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

# Build Java bindings
# Requires: SWIG
OPTION( BUILD_JAVA_BINDINGS "Build Java bindings" OFF )

# Build V8 Node bindings
# Requires: SWIG, Node
OPTION( BUILD_V8_BINDINGS "Build Node V8 bindings" OFF )

# Build Python bindings
# Requires: SWIG Python
OPTION( BUILD_PYTHON_BINDINGS "Build Python bindings" OFF )

# Build ctest testing
# Requires: EnergyPlus
OPTION( BUILD_TESTING "Build testing targets" OFF )

# Build package
# Requires: EnergyPlus
OPTION( BUILD_PACKAGE "Build package" OFF )

# Build test runner targets.
# This is a convenience for Visual Studio users
OPTION( ENABLE_TEST_RUNNER_TARGETS "Create test runner targets" OFF )

# Build with OpenSSL support
SET( BUILD_WITH_OPENSSL ON CACHE INTERNAL "Build With OpenSSL Support For SSH Connections" )

# Build R package
OPTION( BUILD_R "Build R package" OFF )

# Build Ruby gem
OPTION( BUILD_RUBY_GEM "Build Ruby Gem" OFF )

IF (BUILD_RUBY_GEM)
  FIND_PROGRAM( GEM NAMES gem )
  IF( NOT GEM )
    MESSAGE(SEND_ERROR "Gem tool not found, cannot build gem")
    IF(UNIX AND NOT APPLE)
      MESSAGE(SEND_ERROR "Please install rubygems using your package management system")
    ENDIF()    
  ENDIF()

  # Build Ruby gem and package up all the libs
  OPTION( BUILD_RUBY_GEM_WITH_LIBS "Package all required libs with the Ruby Gem" ON )
ENDIF()

IF (BUILD_V8_BINDINGS)
  OPTION( BUILD_NODE_MODULES "Build V8 Bindings as Node Modules" ON )
ENDIF()

# Configure vagrant 
SET(SHOW_VAGRANT "false" CACHE BOOL "Show vagrant option to GUI users")

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

# Enable support for boost > 1.49 ish by forcing boost_filesystem_version 3
ADD_DEFINITIONS(-DBOOST_FILESYSTEM_VERSION=3)

IF(WIN32)
  ADD_DEFINITIONS(-DNOMINMAX)
ENDIF()

IF(MSVC)
  # Build with Multiple Processes
  SET( BUILD_WITH_MULTIPLE_PROCESSES ON CACHE BOOL "/MP compiler flag for full processor utilization" )
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
SET( CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Products" )
SET( CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Products" )
SET( CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Products" )

SET ( LIBRARY_SEARCH_DIRECTORY "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}" "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/Release" "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/Debug" )

# Include project specific macros
INCLUDE(ProjectMacros.cmake)

# Search for modules in the root dir first to override cmake ones 
LIST(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake")

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
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -fPIC -fno-strict-aliasing -Winvalid-pch -Wnon-virtual-dtor")
  IF(APPLE)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-overloaded-virtual -ftemplate-depth=1024")
  ELSE()
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-overloaded-virtual")
  ENDIF()
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

ENDIF(MSVC)
###############################################################################


###############################################################################
# Check version of gcc
IF (CMAKE_COMPILER_IS_GNUCC)
  EXECUTE_PROCESS(COMMAND ${CMAKE_C_COMPILER} -dumpversion
                  OUTPUT_VARIABLE GCC_VERSION)
ENDIF()



###############################################################################
# Dependencies 

# EnergyPlus
#IF( BUILD_TESTING OR BUILD_PACKAGE)
  FIND_PACKAGE(EnergyPlus "${ENERGYPLUS_VERSION}" REQUIRED)
  MARK_AS_ADVANCED(
    ENERGYPLUS_EXE
    ENERGYPLUS_IDD
    ENERGYPLUS_WEATHER_DIR
  )
#ENDIF( BUILD_TESTING OR BUILD_PACKAGE )

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


# OpenSSL / libssh support
IF( BUILD_WITH_OPENSSL )

  FIND_PACKAGE( OpenSSL )
  IF( NOT ${OPENSSL_FOUND} )
    MESSAGE(SEND_ERROR "OpenSSL could not be found, and is required for HTTPS communication")
    IF ( WIN32 )
      MESSAGE(SEND_ERROR "You can install OpenSSL for windows by visiting http://www.slproweb.com/products/Win32OpenSSL.html and installing the latest recommended full installation package")
    ELSE()
      MESSAGE(SEND_ERROR "Please install OpenSSL development libraries using your package management system (possibly libssl-dev)")
    ENDIF()
  ELSEIF(WIN32)
    SET(LIBEAY32_PATH "${OPENSSL_ROOT_DIR}/libeay32.dll")
    SET(SSLEAY32_PATH "${OPENSSL_ROOT_DIR}/ssleay32.dll")
    FILE(COPY "${LIBEAY32_PATH}" "${SSLEAY32_PATH}" DESTINATION "${CMAKE_BINARY_DIR}/Products/Release/")
    FILE(COPY "${LIBEAY32_PATH}" "${SSLEAY32_PATH}" DESTINATION "${CMAKE_BINARY_DIR}/Products/Debug/")
    FILE(COPY "${LIBEAY32_PATH}" "${SSLEAY32_PATH}" DESTINATION "${CMAKE_BINARY_DIR}/Products/RelWithDebInfo/")
    FILE(COPY "${LIBEAY32_PATH}" "${SSLEAY32_PATH}" DESTINATION "${CMAKE_BINARY_DIR}/Products/MinSizeRel/")
    INSTALL(FILES "${LIBEAY32_PATH}" "${SSLEAY32_PATH}" DESTINATION "bin")
    INSTALL(FILES "${LIBEAY32_PATH}" "${SSLEAY32_PATH}" DESTINATION ${RUBY_MODULE_ARCH_DIR})
  ENDIF()
  
  #enable libssh
  #ADD_DEFINITIONS(-DLIBSSH_STATIC)
  
  # add sub directory
  #ADD_SUBDIRECTORY( src/libssh )
  #INCLUDE_DIRECTORIES( SYSTEM src/libssh/include/ )
      
ENDIF()



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

# JAVA
IF ( BUILD_JAVA_BINDINGS )
  find_package(JNI)
  find_package(Java)
  SET( ALL_JAVA_BINDING_TARGETS "" ) # global list
  SET( ALL_JAVA_SRC_DIRECTORIES "" ) # global list
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
      # We want to build the idd documentation separately because of the generated files
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

SET( Boost_ADDITIONAL_VERSIONS "1.55" )
FIND_PACKAGE( Boost 1.55.0 REQUIRED COMPONENTS filesystem regex program_options system thread date_time serialization log)
INCLUDE_DIRECTORIES(SYSTEM ${Boost_INCLUDE_DIR} )
MARK_AS_ADVANCED( Boost_LIB_DIAGNOSTIC_DEFINITIONS )

# require these run time checks
add_definitions(-DBOOST_UBLAS_CHECK_ENABLE)
add_definitions(-DBOOST_UBLAS_TYPE_CHECK)
add_definitions(-DBOOST_UBLAS_NO_STD_CERR)

# use chrono as a header only library
add_definitions(-DBOOST_CHRONO_HEADER_ONLY)

# check if boost is being linked dynamically
# DLM: should we allow this on Unix or should we force static for all platforms?
STRING(REGEX MATCH "^.*libboost_log.so$" BOOST_IS_DYN_LIBS "${Boost_LOG_LIBRARY_RELEASE}" )
IF (BOOST_IS_DYN_LIBS)
  add_definitions(-DBOOST_ALL_DYN_LINK)
ENDIF()

# SQLite definitions, used in sqlite and litesql
ADD_DEFINITIONS( -DSQLITE_THREADSAFE=1 ) # 1 is default, serial access
#ADD_DEFINITIONS( -DSQLITE_DEBUG )
#ADD_DEFINITIONS( -DSQLITE_MEMDEBUG )

#--------------------------------------------------------------------------------
# Qt
FIND_PACKAGE (Qt5Widgets REQUIRED)
FIND_PACKAGE (Qt5Sql REQUIRED)
FIND_PACKAGE (Qt5Network REQUIRED)
FIND_PACKAGE (Qt5Xml REQUIRED)
FIND_PACKAGE (Qt5WebKit REQUIRED)
FIND_PACKAGE (Qt5WebKitWidgets REQUIRED)
#FIND_PACKAGE (Qt5WinExtras)
FIND_PACKAGE (Qt5Concurrent REQUIRED)
MARK_AS_ADVANCED(Qt5Concurrent_DIR)
MARK_AS_ADVANCED(Qt5Core_DIR)
MARK_AS_ADVANCED(Qt5Gui_DIR)
MARK_AS_ADVANCED(Qt5Gui_EGL_LIBRARY)
MARK_AS_ADVANCED(Qt5Multimedia_DIR)
MARK_AS_ADVANCED(Qt5MultimediaWidgets_DIR)
MARK_AS_ADVANCED(Qt5Network_DIR)
MARK_AS_ADVANCED(Qt5OpenGL_DIR)
MARK_AS_ADVANCED(Qt5Positioning_DIR)
MARK_AS_ADVANCED(Qt5PrintSupport_DIR)
MARK_AS_ADVANCED(Qt5Qml_DIR)
MARK_AS_ADVANCED(Qt5Quick_DIR)
MARK_AS_ADVANCED(Qt5Sensors_DIR)
MARK_AS_ADVANCED(Qt5Sql_DIR)
MARK_AS_ADVANCED(Qt5WebKit_DIR)
MARK_AS_ADVANCED(Qt5WebKitWidgets_DIR)
MARK_AS_ADVANCED(Qt5Widgets_DIR)
#MARK_AS_ADVANCED(Qt5WinExtras_DIR)
MARK_AS_ADVANCED(Qt5Xml_DIR)

SET (CMAKE_AUTOMOC OFF)

#Adding libraries to the list of includes
INCLUDE_DIRECTORIES( ${Qt5Widgets_INCLUDE_DIRS} )
INCLUDE_DIRECTORIES( ${Qt5Sql_INCLUDE_DIRS} )
INCLUDE_DIRECTORIES( ${Qt5Network_INCLUDE_DIRS} )
INCLUDE_DIRECTORIES( ${Qt5Xml_INCLUDE_DIRS} )
INCLUDE_DIRECTORIES( ${Qt5WebKit_INCLUDE_DIRS} )
INCLUDE_DIRECTORIES( ${Qt5WebKitWidgets_INCLUDE_DIRS} )
#INCLUDE_DIRECTORIES( ${Qt5WinExtras_INCLUDE_DIRS} )
INCLUDE_DIRECTORIES( ${Qt5Concurrent_INCLUDE_DIRS} )

#INCLUDE_DIRECTORIES is supposed to be a set, however there are tons of Qt duplicate includes
get_directory_property(dupdirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} INCLUDE_DIRECTORIES)
list(REMOVE_DUPLICATES dupdirs)
set_directory_properties(PROPERTIES INCLUDE_DIRECTORIES "${dupdirs}")

# list of all Qt libs and dependencies we are using
LIST( APPEND QT_LIBS ${Qt5Widgets_LIBRARIES})
LIST( APPEND QT_LIBS ${Qt5Sql_LIBRARIES})
LIST( APPEND QT_LIBS ${Qt5Network_LIBRARIES})
LIST( APPEND QT_LIBS ${Qt5Xml_LIBRARIES})
LIST( APPEND QT_LIBS ${Qt5WebKit_LIBRARIES})
LIST( APPEND QT_LIBS ${Qt5WebKitWidgets_LIBRARIES})
#LIST( APPEND QT_LIBS ${Qt5WinExtras_LIBRARIES})
LIST( APPEND QT_LIBS ${Qt5Concurrent_LIBRARIES})


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

  SET(AWS_RUBY_URL http://developer.nrel.gov/downloads/buildings/openstudio/src/ruby-2.0.0p353-aws-win32.zip)
  SET(AWS_RUBY_CHECKSUM 1f6e4348099b26e92909db62dcd3b173)
ELSEIF(APPLE)
  SET(AWS_RUBY_PATH ruby-2.0.0-aws-Darwin)
  SET(AWS_RUBY_DESTINATION ".")

  SET(AWS_RUBY_URL http://developer.nrel.gov/downloads/buildings/openstudio/src/ruby-2.0.0p353-aws-Darwin.tar.bz2)
  SET(AWS_RUBY_CHECKSUM 75963f2c6996af47a3ad69ac84d3d120)
  SET(AWS_RUBY_EXTENSION ".tar.bz2")
ELSEIF(UNIX AND CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64")
  SET(AWS_RUBY_PATH ruby-2.0.0-aws-Linux-x86_64)
  SET(AWS_RUBY_DESTINATION "lib/openstudio")

  SET(AWS_RUBY_URL http://developer.nrel.gov/downloads/buildings/openstudio/src/ruby-2.0.0p353-aws-Linux-x86_64.tar.bz2)
  SET(AWS_RUBY_CHECKSUM 74d3a8ab4e1e3fb44c308f2aaa8e5732)
  SET(AWS_RUBY_EXTENSION ".tar.bz2")
ELSEIF(UNIX)
  SET(AWS_RUBY_PATH ruby-2.0.0-aws-Linux-i686)
  SET(AWS_RUBY_DESTINATION "lib/openstudio")

  SET(AWS_RUBY_URL http://developer.nrel.gov/downloads/buildings/openstudio/src/ruby-2.0.0p353-aws-Linux-i686.tar.bz2)
  SET(AWS_RUBY_CHECKSUM e51ce64b3e03ed3a66f1f7e0a22b5b7e)
  SET(AWS_RUBY_EXTENSION ".tar.bz2")
ENDIF()

FILE(DOWNLOAD ${AWS_RUBY_URL} "${CMAKE_BINARY_DIR}/${AWS_RUBY_PATH}${AWS_RUBY_EXTENSION}" TIMEOUT 120 INACTIVITY_TIMEOUT 120 )
execute_process(COMMAND ${CMAKE_COMMAND} -E remove_directory "${CMAKE_BINARY_DIR}/${AWS_RUBY_PATH}")
EXECUTE_PROCESS(COMMAND ${CMAKE_COMMAND} -E tar xfz "${CMAKE_BINARY_DIR}/${AWS_RUBY_PATH}${AWS_RUBY_EXTENSION}" WORKING_DIRECTORY "${CMAKE_BINARY_DIR}")
execute_process(COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_HOME_DIRECTORY}/ruby/aws-sdk-patches/connection_pool.rb" "${CMAKE_BINARY_DIR}/${AWS_RUBY_PATH}/lib/ruby/gems/2.0.0/gems/aws-sdk-1.31.3/lib/aws/core/http")
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

# java, after loading projects
IF(BUILD_JAVA_BINDINGS)
  ADD_SUBDIRECTORY( java )
ENDIF(BUILD_JAVA_BINDINGS)

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

if ( BUILD_JAVA_BINDINGS )
  ADD_CUSTOM_TARGET( ALL_JAVA_BINDINGS )
  #  ADD_DEPENDENCIES( ALL_JAVA_BINDINGS java_sdk ${ALL_JAVA_BINDING_TARGETS} )
endif ( BUILD_JAVA_BINDINGS )


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

#IF( BUILD_WITH_OPENSSL )
#  SET(LIBSSH_EXPORT "libssh_static")
#ELSE( BUILD_WITH_OPENSSL )
#  SET(LIBSSH_EXPORT "")
#ENDIF( BUILD_WITH_OPENSSL )

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
install( FILES "${CMAKE_SOURCE_DIR}/../doc/ReleaseNotes/OpenStudio_Release_Notes_1_3_0_20140328.pdf" DESTINATION . )

###############################################################################


IF (UNIX AND NOT APPLE)
  # if we build on Linux and we built our own Qt, let's grab said Qt and install it
  IF(CMAKE_PREFIX_PATH MATCHES ".*Qt-install.*")
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
  
  get_filename_component(RubyEXEPath "${RUBY_EXECUTABLE}" PATH )
  get_filename_component(RubyPath "${RubyEXEPath}" PATH )
  install( DIRECTORY "${RubyPath}/" DESTINATION "ruby-install/ruby" )

  SET(CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS_SKIP ON)
  INCLUDE(InstallRequiredSystemLibraries)
  IF(CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS)
    INSTALL(FILES ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS}
      DESTINATION "bin")

    # Install the system libraries to the ruby dir
    INSTALL(FILES ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS}
      DESTINATION "Ruby/openstudio")
      
    INSTALL(FILES ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS}
      DESTINATION "ruby-install/ruby/bin")
  ENDIF(CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS)
ENDIF()




IF (PACKAGE_RADIANCE_AND_ENERGYPLUS)
  SET(ENERGYPLUS_LOCATION "" CACHE PATH "Location of EnergyPlus to Package")
  SET(RADIANCE_LOCATION "" CACHE PATH "Location of Radiance to Package")

  FILE(COPY "${ENERGYPLUS_LOCATION}" DESTINATION "${CMAKE_BINARY_DIR}/EnergyPlus/")
  FILE(COPY "${RADIANCE_LOCATION}" DESTINATION "${CMAKE_BINARY_DIR}/Radiance/")

  IF (APPLE)
    INSTALL(DIRECTORY "${CMAKE_BINARY_DIR}/EnergyPlus/" DESTINATION "sharedresources" USE_SOURCE_PERMISSIONS)
    INSTALL(DIRECTORY "${CMAKE_BINARY_DIR}/Radiance/" DESTINATION "sharedresources" USE_SOURCE_PERMISSIONS)
  ELSE()
    INSTALL(DIRECTORY "${CMAKE_BINARY_DIR}/EnergyPlus/" DESTINATION "share/openstudio" USE_SOURCE_PERMISSIONS)
    INSTALL(DIRECTORY "${CMAKE_BINARY_DIR}/Radiance/" DESTINATION "share/openstudio" USE_SOURCE_PERMISSIONS)
  ENDIF()

ENDIF()
