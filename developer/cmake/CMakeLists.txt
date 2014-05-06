cmake_minimum_required(VERSION 2.8)

set(target_name Test)
project(${target_name})

###############################################################################
# Compatibility settings

add_definitions(-DBOOST_FILESYSTEM_VERSION=2)

###############################################################################
# OpenStudio

set(OPENSTUDIO_BUILD_DIR "" CACHE PATH "Build or install directory for OpenStudio build to link against")
set(OPENSTUDIOCORE_BUILD_DIR "${OPENSTUDIO_BUILD_DIR}/OpenStudioCore-prefix/src/OpenStudioCore-build")
include("${OPENSTUDIO_BUILD_DIR}/OpenStudioCore-prefix/src/OpenStudioCore-build/OpenStudioCoreConfig.cmake")
include_directories(${OpenStudioCore_INCLUDE_DIRS})

# copy dlls at configuration time
if(MSVC)
  file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/Debug")
  file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/Release")
  file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/RelWithDebInfo")
  list(APPEND CMAKE_LIBRARY_PATH ${OPENSTUDIO_LIB_DIR})

  file(GLOB DEBUG_DLLS "${OPENSTUDIO_BUILD_DIR}/OpenStudioCore-prefix/src/OpenStudioCore-build/Products/Debug/*.dll")
  file(GLOB RELEASE_DLLS "${OPENSTUDIO_BUILD_DIR}/OpenStudioCore-prefix/src/OpenStudioCore-build/Products/Release/*.dll")
  file(GLOB RELDBG_DLLS "${OPENSTUDIO_BUILD_DIR}/OpenStudioCore-prefix/src/OpenStudioCore-build/Products/RelWithDebInfo/*.dll")

  list(APPEND ALL_DLLS ${DEBUG_DLLS})
  list(APPEND ALL_DLLS ${RELEASE_DLLS})
  list(APPEND ALL_DLLS ${RELDBG_DLLS})
  list(LENGTH ALL_DLLS NUM_DLLS)
  message("Copying ${NUM_DLLS} DLLs under ${OPENSTUDIO_BUILD_DIR} to ${CMAKE_BINARY_DIR}")
  file(COPY ${DEBUG_DLLS} DESTINATION "${CMAKE_BINARY_DIR}/Debug/")
  file(COPY ${RELEASE_DLLS} DESTINATION "${CMAKE_BINARY_DIR}/Release/")
  file(COPY ${RELDBG_DLLS} DESTINATION "${CMAKE_BINARY_DIR}/RelWithDebInfo/")
endif()

link_directories(${OpenStudioCore_LIBRARY_DIRS})

###############################################################################
# Boost
if(APPLE)
  set(Boost_USE_STATIC_LIBS ON)
endif()
if(WIN32)
  set(Boost_USE_STATIC_LIBS ON)
  #uncomment all of this if we want to force dynamic libs on windows
  #  set(Boost_USE_STATIC_LIBS OFF)
  #  add_definitions(-DBOOST_THREAD_USE_DLL -DBOOST_THREAD_DYN_LINK -DBOOST_PROGRAM_OPTIONS_DYN_LINK -DBOOST_REGEX_DYN_LINK -DBOOST_FILESYSTEM_DYN_LINK -DBOOST_SYSTEM_DYN_LINK -DBOOST_SERIALIZATION_DYN_LINK -DBOOST_DATE_TIME_DYN_LINK)
  #  link_directories(${Boost_LIBRARY_DIRS})
  #  if(MSVC)
  #    #Ignore dll specific warnings that are out of our hands to control, coming from external projects
  #    add_definitions( "/wd4251 /wd4275")
  #  endif()
endif()

set(Boost_ADDITIONAL_VERSIONS "1.40")

if(NOT DEFINED BOOST_ROOT)
  set(BOOST_ROOT "${OPENSTUDIO_BUILD_DIR}/Boost-prefix/src/Boost-install/")
endif(NOT DEFINED BOOST_ROOT)

if(NOT DEFINED BOOST_LIBRARYDIR)
  set(BOOST_LIBRARYDIR "${OPENSTUDIO_BUILD_DIR}/Boost-prefix/src/Boost-install/lib/")
endif(NOT DEFINED BOOST_LIBRARYDIR)

find_package(Boost 1.40.0 REQUIRED COMPONENTS filesystem regex program_options system thread date_time serialization)
include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIB_DIR})
mark_as_advanced(Boost_LIB_DIAGNOSTIC_DEFINITIONS)

###############################################################################
# BoostLog
add_definitions(-DBOOST_LOG_NO_LIB) # disable autolinking
add_definitions(-DBOOST_LOG_SETUP_NO_LIB) # disable autolinking

find_package(BoostLog REQUIRED HINTS "${OPENSTUDIO_BUILD_DIR}/BoostLog-prefix/src/BoostLog-build")
include_directories(${BoostLog_INCLUDE_DIRS})
link_directories(${BoostLog_LIBRARY_DIRS})

message(${BoostLog_INCLUDE_DIRS})
message(${BoostLog_LIBRARY_DIRS})

###############################################################################
# Qt
find_package(Qt4 4.6.0 COMPONENTS QtCore QtGui QtNetwork QtXml REQUIRED)
include_directories(${QT_INCLUDE_DIR})
#list( APPEND CMAKE_PROGRAM_PATH ${QT_LIBRARY_DIR})
#list( APPEND CMAKE_LIBRARY_PATH ${QT_LIBRARY_DIR})

# QT_USE_FILE is a variable defined by FIND_PACKAGE( Qt4 ) that contains a path to
# a CMake script that sets up this directory for building with Qt4
include(${QT_USE_FILE})
mark_as_advanced(QT_QMAKE_EXECUTABLE QT_QTMOTIF_INCLUDE_DIR QT_QTMOTIF_LIBRARY_DEBUG QT_QTMOTIF_LIBRARY_RELEASE)

# This sets the windows build that will need the special winmain@16 call. Qt provides
#  this for us in the qtmain.lib file. Using this cmake code will ensure we have it
#  linked into our build. Not needed on Unix/OS X/Linux which is why we have the
#  if(WIN32) conditional.
if(WIN32)
  set(QT_USE_QTMAIN TRUE)
endif()

# QT_USE_FILE is a variable defined by FIND_PACKAGE( Qt4 ) that contains a path to
# a CMake script that sets up this directory for building with Qt4
include(${QT_USE_FILE})

#Adding the qtnetwork library to the includes
include_directories(${QT_QTSQL_INCLUDE_DIR})
include_directories(${QT_QTNETWORK_INCLUDE_DIR})
include_directories(${QT_QTXML_INCLUDE_DIR})

# list of all Qt libs and dependencies we are using
list(APPEND QT_LIBS ${QT_QTSQL_LIBRARY})
list(APPEND QT_LIBS ${QT_QTGUI_LIBRARY})
list(APPEND QT_LIBS ${QT_QTGUI_LIB_DEPENDENCIES})
list(APPEND QT_LIBS ${QT_QTCORE_LIBRARY})
list(APPEND QT_LIBS ${QT_QTCORE_LIB_DEPENDENCIES})
list(APPEND QT_LIBS ${QT_QTMAIN_LIBRARY})
list(APPEND QT_LIBS ${QT_QTNETWORK_LIBRARY})
list(APPEND QT_LIBS ${QT_QTNETWORK_LIB_DEPENDENCIES})
list(APPEND QT_LIBS ${QT_QTXML_LIBRARY})
list(APPEND QT_LIBS ${QT_QTXML_LIB_DEPENDENCIES})

###############################################################################
# QJSON

find_package(QJSON REQUIRED HINTS "${OPENSTUDIO_BUILD_DIR}/QJSON-prefix/src/QJSON-build")
include_directories(${QJSON_INCLUDE_DIR})
link_directories(${QJSON_LIBRARY_DIRS})

###############################################################################
# Sample target

add_executable(Test
  ${CMAKE_SOURCE_DIR}/Test.cpp
)

target_link_libraries(${target_name}
  ${Boost_LIBRARIES}
  ${CMAKE_THREAD_LIBS}
  ${QT_LIBS}
  boost_log
  openstudio_utilities
  openstudio_model
)
