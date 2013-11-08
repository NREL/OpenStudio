cmake_minimum_required(VERSION 2.8)

PROJECT(SimXml)

Find_Package( OpenStudioCore REQUIRED )
set(RUBY_MODULE_DIR ${OpenStudioCore_RUBY_MODULE_DIR})
set(RUBY_MODULE_ARCH_DIR ${OpenStudioCore_RUBY_MODULE_ARCH_DIR})

# Let's use the cxx flags from the OpenStudioCore build to ensure compatibility
SET(CMAKE_CXX_FLAGS ${OpenStudioCore_CMAKE_CXX_FLAGS})

LINK_DIRECTORIES( ${OpenStudioCore_LIBRARY_DIRS} )
INCLUDE_DIRECTORIES( ${OpenStudioCore_INCLUDE_DIRS} )
INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_SOURCE_DIR} )

# Enable support for boost > 1.42 ish by forcing boost_filesystem_version 2
ADD_DEFINITIONS(-DBOOST_FILESYSTEM_VERSION=2)


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
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /WX")
  # ignore warnings about the stl being insecure
  ADD_DEFINITIONS(-D_CRT_SECURE_NO_WARNINGS -D_SCL_SECURE_NO_WARNINGS)
  # only build eefg dll on msvc
  LIST(APPEND project_directories eefg)
ENDIF(MSVC)


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

SET( Boost_ADDITIONAL_VERSIONS "1.40" "1.47.0" "1.47")
FIND_PACKAGE( Boost 1.40.0 REQUIRED COMPONENTS filesystem regex program_options system thread date_time serialization)
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )
MARK_AS_ADVANCED( Boost_LIB_DIAGNOSTIC_DEFINITIONS )

# Boost-log
ADD_DEFINITIONS( -DBOOST_LOG_NO_LIB ) # disable autolinking
ADD_DEFINITIONS( -DBOOST_LOG_SETUP_NO_LIB ) # disable autolinking


find_package(BoostLog REQUIRED)
INCLUDE_DIRECTORIES( ${BoostLog_INCLUDE_DIRS} )
LINK_DIRECTORIES( ${BoostLog_LIBRARY_DIRS} )

find_package(QJSON REQUIRED)
INCLUDE_DIRECTORIES( ${QJSON_INCLUDE_DIR} )
LINK_DIRECTORIES( ${QJSON_LIBRARY_DIRS} )

# include after other boost include directories for cases where boost version < 1.47
INCLUDE_DIRECTORIES( SYSTEM ${CMAKE_SOURCE_DIR}/../dependencies/boost_geometry )



FIND_PACKAGE (Qt4 4.6.0 COMPONENTS QtCore QtGui QtSql QtNetwork QtXml QtXmlPatterns REQUIRED)
INCLUDE_DIRECTORIES( ${QT_INCLUDE_DIR} )
INCLUDE( ${QT_USE_FILE} )

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

FIND_PROGRAM( RUBY_EXECUTABLE NAMES ruby PATHS $ENV{CMAKE_RUBY_PATH} NO_SYSTEM_ENVIRONMENT_PATH )
FIND_PACKAGE( Ruby REQUIRED )



#
set(USE_PCH false)
include(${OpenStudioCore_CMAKE_DIR}/ProjectMacros.cmake)


SET( CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Products" )
SET( CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Products" )
SET( CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Products" )


###############################################################################
# Add the genxsd executable
INCLUDE_DIRECTORIES( xsdgen )
ADD_SUBDIRECTORY( xsdgen )
###############################################################################

###############################################################################
# Add the XML library
INCLUDE_DIRECTORIES( xmllib )
ADD_SUBDIRECTORY( xmllib )
###############################################################################

###############################################################################
# Add the SimXML library
INCLUDE_DIRECTORIES( simxmllib )
INCLUDE_DIRECTORIES("${CMAKE_BINARY_DIR}/simxmllib")
ADD_SUBDIRECTORY( simxmllib )
###############################################################################

SET( target_name openstudio_simxml )

SET( ${target_name}_src
  SimXMLAPI.hpp
  ForwardTranslator.hpp
  ForwardTranslator.cpp
  ReverseTranslator.hpp
  ReverseTranslator.cpp
)

SET( ${target_name}_test_src
  Test/SimXMLFixture.hpp
  Test/SimXMLFixture.cpp
  Test/ForwardTranslator_GTest.cpp
  Test/ReverseTranslator_GTest.cpp
)

SET( ${target_name}_swig_src
  SimXML.i
)

SET( ${target_name}_depends
  ${Boost_LIBRARIES}
  ${CMAKE_THREAD_LIBS}
  ${QT_LIBS}
  simxmllib
  openstudio_utilities
  openstudio_model
  openstudio_energyplus
  zkexpat
  boost_log
)

ADD_LIBRARY( ${target_name} SHARED ${${target_name}_src} )
TARGET_LINK_LIBRARIES( ${target_name} ${${target_name}_depends} )

IF( NOT APPLE )
INSTALL(TARGETS ${target_name} 
  RUNTIME DESTINATION bin 
  LIBRARY DESTINATION lib )
ENDIF()

CREATE_SRC_GROUPS( "${${target_name}_src}" )
CREATE_SRC_GROUPS( "${${target_name}_test_src}" )
CREATE_SRC_GROUPS( "${${target_name}_swig_src}" )

CREATE_TEST_TARGETS( ${target_name} "${${target_name}_test_src}" "${${target_name}_depends}")
IF(BUILD_TESTING)
  ADD_DEPENDENCIES(${target_name}_tests openstudio_simxml_resources)
ENDIF()

MAKE_SWIG_TARGET( OpenStudioSimXML SimXML "${CMAKE_CURRENT_SOURCE_DIR}/SimXML.i" "${${target_name}_swig_src}" ${target_name} "" )
