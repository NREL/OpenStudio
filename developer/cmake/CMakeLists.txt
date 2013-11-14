CMAKE_MINIMUM_REQUIRED( VERSION 2.8 )

SET( target_name Test)
PROJECT( ${target_name} )

###############################################################################
# Compatibility settings

ADD_DEFINITIONS(-DBOOST_FILESYSTEM_VERSION=2)

###############################################################################
# OpenStudio

SET( OPENSTUDIO_BUILD_DIR "" CACHE PATH "Build or install directory for OpenStudio build to link against")
SET( OPENSTUDIOCORE_BUILD_DIR "${OPENSTUDIO_BUILD_DIR}/OpenStudioCore-prefix/src/OpenStudioCore-build")
INCLUDE ( "${OPENSTUDIO_BUILD_DIR}/OpenStudioCore-prefix/src/OpenStudioCore-build/OpenStudioCoreConfig.cmake" )
INCLUDE_DIRECTORIES( ${OpenStudioCore_INCLUDE_DIRS} )

# copy dlls at configuration time
IF(MSVC)
  FILE(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/Debug")
  FILE(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/Release")
  FILE(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/RelWithDebInfo")
  LIST( APPEND CMAKE_LIBRARY_PATH ${OPENSTUDIO_LIB_DIR} )

  FILE(GLOB DEBUG_DLLS "${OPENSTUDIO_BUILD_DIR}/OpenStudioCore-prefix/src/OpenStudioCore-build/Products/Debug/*.dll")
  FILE(GLOB RELEASE_DLLS "${OPENSTUDIO_BUILD_DIR}/OpenStudioCore-prefix/src/OpenStudioCore-build/Products/Release/*.dll")
  FILE(GLOB RELDBG_DLLS "${OPENSTUDIO_BUILD_DIR}/OpenStudioCore-prefix/src/OpenStudioCore-build/Products/RelWithDebInfo/*.dll")

  LIST( APPEND ALL_DLLS ${DEBUG_DLLS} )
  LIST( APPEND ALL_DLLS ${RELEASE_DLLS} )
  LIST( APPEND ALL_DLLS ${RELDBG_DLLS} )
  LIST( LENGTH ALL_DLLS NUM_DLLS )
  MESSAGE("Copying ${NUM_DLLS} DLLs under ${OPENSTUDIO_BUILD_DIR} to ${CMAKE_BINARY_DIR}")
  FILE(COPY ${DEBUG_DLLS} DESTINATION "${CMAKE_BINARY_DIR}/Debug/")
  FILE(COPY ${RELEASE_DLLS} DESTINATION "${CMAKE_BINARY_DIR}/Release/")
  FILE(COPY ${RELDBG_DLLS} DESTINATION "${CMAKE_BINARY_DIR}/RelWithDebInfo/")
ENDIF()

LINK_DIRECTORIES(${OpenStudioCore_LIBRARY_DIRS})

###############################################################################
# Boost
IF(APPLE)
  SET(Boost_USE_STATIC_LIBS ON)  
ENDIF()
IF (WIN32)
  SET(Boost_USE_STATIC_LIBS ON)  
  #uncomment all of this if we want to force dynamic libs on windows
  #  set(Boost_USE_STATIC_LIBS OFF)  
  #  ADD_DEFINITIONS( -DBOOST_THREAD_USE_DLL -DBOOST_THREAD_DYN_LINK -DBOOST_PROGRAM_OPTIONS_DYN_LINK  -DBOOST_REGEX_DYN_LINK  -DBOOST_FILESYSTEM_DYN_LINK -DBOOST_SYSTEM_DYN_LINK -DBOOST_SERIALIZATION_DYN_LINK  -DBOOST_DATE_TIME_DYN_LINK)
  #  LINK_DIRECTORIES( ${Boost_LIBRARY_DIRS} )
  #  IF (MSVC)
  #    #Ignore dll specific warnings that are out of our hands to control, coming from external projects
  #    ADD_DEFINITIONS( "/wd4251 /wd4275" )
  #  ENDIF (MSVC)
ENDIF (WIN32)

SET( Boost_ADDITIONAL_VERSIONS "1.40" )

IF(NOT DEFINED BOOST_ROOT)
  SET(BOOST_ROOT "${OPENSTUDIO_BUILD_DIR}/Boost-prefix/src/Boost-install/")
ENDIF(NOT DEFINED BOOST_ROOT)

IF(NOT DEFINED BOOST_LIBRARYDIR)
  SET(BOOST_LIBRARYDIR "${OPENSTUDIO_BUILD_DIR}/Boost-prefix/src/Boost-install/lib/")
ENDIF(NOT DEFINED BOOST_LIBRARYDIR)

FIND_PACKAGE( Boost 1.40.0 REQUIRED COMPONENTS filesystem regex program_options system thread date_time serialization )
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )
LINK_DIRECTORIES( ${Boost_LIB_DIR} )
MARK_AS_ADVANCED( Boost_LIB_DIAGNOSTIC_DEFINITIONS )

###############################################################################
# BoostLog
ADD_DEFINITIONS( -DBOOST_LOG_NO_LIB ) # disable autolinking
ADD_DEFINITIONS( -DBOOST_LOG_SETUP_NO_LIB ) # disable autolinking

FIND_PACKAGE(BoostLog REQUIRED HINTS "${OPENSTUDIO_BUILD_DIR}/BoostLog-prefix/src/BoostLog-build")
INCLUDE_DIRECTORIES( ${BoostLog_INCLUDE_DIRS} )
LINK_DIRECTORIES( ${BoostLog_LIBRARY_DIRS} )

MESSAGE(${BoostLog_INCLUDE_DIRS})
MESSAGE(${BoostLog_LIBRARY_DIRS})

###############################################################################
# Qt
FIND_PACKAGE (Qt4 4.6.0 COMPONENTS QtCore QtGui QtNetwork QtXml REQUIRED)
INCLUDE_DIRECTORIES( ${QT_INCLUDE_DIR} )
#LIST( APPEND CMAKE_PROGRAM_PATH ${QT_LIBRARY_DIR})
#LIST( APPEND CMAKE_LIBRARY_PATH ${QT_LIBRARY_DIR})

# QT_USE_FILE is a variable defined by FIND_PACKAGE( Qt4 ) that contains a path to
# a CMake script that sets up this directory for building with Qt4
INCLUDE( ${QT_USE_FILE} )
MARK_AS_ADVANCED( QT_QMAKE_EXECUTABLE QT_QTMOTIF_INCLUDE_DIR QT_QTMOTIF_LIBRARY_DEBUG QT_QTMOTIF_LIBRARY_RELEASE )

# This sets the windows build that will need the special winmain@16 call. Qt provides
#  this for us in the qtmain.lib file. Using this cmake code will ensure we have it
#  linked into our build. Not needed on Unix/OS X/Linux which is why we have the 
#  IF(WIN32) conditional.
IF (WIN32)
    SET (QT_USE_QTMAIN TRUE)
ENDIF (WIN32)

# QT_USE_FILE is a variable defined by FIND_PACKAGE( Qt4 ) that contains a path to
# a CMake script that sets up this directory for building with Qt4
INCLUDE( ${QT_USE_FILE} )

#Adding the qtnetwork library to the includes
INCLUDE_DIRECTORIES( ${QT_QTSQL_INCLUDE_DIR} )
INCLUDE_DIRECTORIES( ${QT_QTNETWORK_INCLUDE_DIR} )
INCLUDE_DIRECTORIES( ${QT_QTXML_INCLUDE_DIR} )

# list of all Qt libs and dependencies we are using
LIST( APPEND QT_LIBS ${QT_QTSQL_LIBRARY})
LIST( APPEND QT_LIBS ${QT_QTGUI_LIBRARY})
LIST( APPEND QT_LIBS ${QT_QTGUI_LIB_DEPENDENCIES})
LIST( APPEND QT_LIBS ${QT_QTCORE_LIBRARY})
LIST( APPEND QT_LIBS ${QT_QTCORE_LIB_DEPENDENCIES})
LIST( APPEND QT_LIBS ${QT_QTMAIN_LIBRARY})
LIST( APPEND QT_LIBS ${QT_QTNETWORK_LIBRARY})
LIST( APPEND QT_LIBS ${QT_QTNETWORK_LIB_DEPENDENCIES})
LIST( APPEND QT_LIBS ${QT_QTXML_LIBRARY})
LIST( APPEND QT_LIBS ${QT_QTXML_LIB_DEPENDENCIES})

###############################################################################
# QJSON

FIND_PACKAGE(QJSON REQUIRED HINTS "${OPENSTUDIO_BUILD_DIR}/QJSON-prefix/src/QJSON-build")
INCLUDE_DIRECTORIES( ${QJSON_INCLUDE_DIR} )
LINK_DIRECTORIES( ${QJSON_LIBRARY_DIRS} )

###############################################################################
# Sample target

ADD_EXECUTABLE( Test
  ${CMAKE_SOURCE_DIR}/Test.cpp
)

TARGET_LINK_LIBRARIES( ${target_name} 
  ${Boost_LIBRARIES}
  ${CMAKE_THREAD_LIBS}
  ${QT_LIBS}
  boost_log
  openstudio_utilities
  openstudio_model
)
