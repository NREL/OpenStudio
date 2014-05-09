cmake_minimum_required(VERSION 2.8.11)

set(target_name Test)
project(${target_name})

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
set(Boost_USE_STATIC_LIBS ON)

if(NOT DEFINED BOOST_ROOT)
  set(BOOST_ROOT "${OPENSTUDIO_BUILD_DIR}/Boost-prefix/src/Boost-install/")
endif()

if(NOT DEFINED BOOST_LIBRARYDIR)
  set(BOOST_LIBRARYDIR "${OPENSTUDIO_BUILD_DIR}/Boost-prefix/src/Boost-install/lib/")
endif()

find_package(Boost 1.55.0 COMPONENTS filesystem regex program_options system thread date_time serialization log)
include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIB_DIR})

add_definitions(-DBOOST_UBLAS_CHECK_ENABLE)
add_definitions(-DBOOST_UBLAS_TYPE_CHECK)
add_definitions(-DBOOST_UBLAS_NO_STD_CERR)
add_definitions(-DBOOST_CHRONO_HEADER_ONLY)

string(REGEX MATCH "^.*libboost_log.so$" BOOST_IS_DYN_LIBS "${Boost_LOG_LIBRARY_RELEASE}")
if(BOOST_IS_DYN_LIBS)
  add_definitions(-DBOOST_ALL_DYN_LINK)
endif()

###############################################################################
# Qt
cmake_policy(SET CMP0020 NEW)

find_package(Qt5Widgets REQUIRED)
find_package(Qt5Sql REQUIRED)
find_package(Qt5Network REQUIRED)
find_package(Qt5Xml REQUIRED)
find_package(Qt5WebKit REQUIRED)
find_package(Qt5WebKitWidgets REQUIRED)
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
  Qt5Xml_DIR
)

###############################################################################
# Sample target

add_executable(Test
  ${CMAKE_SOURCE_DIR}/Test.cpp
)

target_link_libraries(${target_name}
  ${Boost_LIBRARIES}
  ${CMAKE_THREAD_LIBS}
  Qt5::Widgets
  Qt5::Sql
  Qt5::Network
  Qt5::Xml
  Qt5::WebKit
  Qt5::WebKitWidgets
  Qt5::Concurrent
  openstudio_utilities
  openstudio_model
)
