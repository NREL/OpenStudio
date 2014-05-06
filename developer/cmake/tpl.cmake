# Boost ######################
if(APPLE)
  set(Boost_USE_STATIC_LIBS ON)
endif()
if(WIN32)
  set(Boost_USE_STATIC_LIBS ON)
endif()

set(Boost_ADDITIONAL_VERSIONS "1.40")
find_package(Boost 1.40.0 REQUIRED COMPONENTS filesystem regex program_options system thread date_time serialization)
include_directories(${Boost_INCLUDE_DIR})
mark_as_advanced(Boost_LIB_DIAGNOSTIC_DEFINITIONS)
##############################

# Qt #########################
find_package(Qt4 4.6.0 COMPONENTS QtCore QtGui QtSql QtNetwork QtXml QtWebKit REQUIRED)
include(${QT_USE_FILE})
include_directories(${QT_INCLUDE_DIR})
include_directories(${QT_QTWEBKIT_INCLUDE_DIR})

mark_as_advanced(QT_QMAKE_EXECUTABLE QT_QTMOTIF_INCLUDE_DIR QT_QTMOTIF_LIBRARY_DEBUG QT_QTMOTIF_LIBRARY_RELEASE)

# This sets the windows build that will need the special winmain@16 call. Qt provides
#  this for us in the qtmain.lib file. Using this cmake code will ensure we have it
#  linked into our build. Not needed on Unix/OS X/Linux which is why we have the
#  if(WIN32) conditional.
if(WIN32)
  set(QT_USE_QTMAIN TRUE)
endif()

list(APPEND QT_LIBS ${QT_QTMAIN_LIBRARY})
list(APPEND QT_LIBS ${QT_QTCORE_LIBRARY})
list(APPEND QT_LIBS ${QT_QTCORE_LIB_DEPENDENCIES})
list(APPEND QT_LIBS ${QT_QTGUI_LIBRARY})
list(APPEND QT_LIBS ${QT_QTGUI_LIB_DEPENDENCIES})
list(APPEND QT_LIBS ${QT_QTSQL_LIBRARY})
list(APPEND QT_LIBS ${QT_QTSQL_LIB_DEPENDENCIES})
list(APPEND QT_LIBS ${QT_QTNETWORK_LIBRARY})
list(APPEND QT_LIBS ${QT_QTNETWORK_LIB_DEPENDENCIES})
list(APPEND QT_LIBS ${QT_QTXML_LIBRARY})
list(APPEND QT_LIBS ${QT_QTXML_LIB_DEPENDENCIES})
list(APPEND QT_LIBS ${QT_QTWEBKIT_LIBRARY})
list(APPEND QT_LIBS ${QT_QTWEBKIT_LIB_DEPENDENCIES})
##############################

# OpenStudio #################

add_definitions(-DBOOST_LOG_NO_LIB) # disable autolinking
add_definitions(-DBOOST_LOG_SETUP_NO_LIB) # disable autolinking
add_definitions(-DBOOST_LOG_NO_COMPILER_TLS) # disable autolinking

include_directories(${OPENSTUDIOCORE_INCLUDE_DIR})
include_directories("${OPENSTUDIOCORE_INCLUDE_DIR}/dependencies/BoostLog")
include_directories("${OPENSTUDIOCORE_BUILD_DIR}/src")

link_directories(${OPENSTUDIOCORE_LIB_DIR})
link_directories(${BOOSTLOG_LIB_DIR})

macro(find_openstudiocore_library name)

  find_library(${name}_DEBUG_LIB
    NAME ${name}
    PATHS ${OPENSTUDIOCORE_LIB_DIR}Debug
    NO_DEFAULT_PATH
  )

  find_library(${name}_RELEASE_LIB
    NAME ${name}
    PATHS ${OPENSTUDIOCORE_LIB_DIR}
          ${OPENSTUDIOCORE_LIB_DIR}Release
    NO_DEFAULT_PATH
  )

  find_library(${name}_RELWITHDEBINFO_LIB
    NAME ${name}
    PATHS ${OPENSTUDIOCORE_LIB_DIR}RelWithDebInfo
    NO_DEFAULT_PATH
  )

  find_library(${name}_MINSIZEREL_LIB
    NAME ${name}
    PATHS ${OPENSTUDIOCORE_LIB_DIR}MinSizeRel
    NO_DEFAULT_PATH
  )

  if(${name}_DEBUG_LIB)
    list(APPEND OPENSTUDIOCORE_LIBS debug ${${name}_DEBUG_LIB})
  endif()

  if(${name}_RELEASE_LIB)
    list(APPEND OPENSTUDIOCORE_LIBS optimized ${${name}_RELEASE_LIB})
  endif()

  list(APPEND OPENSTUDIOCORE_DEBUG_LIBS ${${name}_DEBUG_LIB})
  list(APPEND OPENSTUDIOCORE_RELEASE_LIBS ${${name}_RELEASE_LIB})
  list(APPEND OPENSTUDIOCORE_RELWITHDEBINFO_LIBS ${${name}_RELWITHDEBINFO_LIB})
  list(APPEND OPENSTUDIOCORE_MINSIZEREL_LIBS ${${name}_MINSIZEREL_LIB})

  find_program( ${name}_DEBUG_DLL
    NAME ${name}.dll
    PATHS ${OPENSTUDIOCORE_LIB_DIR}Debug
    NO_DEFAULT_PATH
  )

  find_program( ${name}_RELEASE_DLL
    NAME ${name}.dll
    PATHS ${OPENSTUDIOCORE_LIB_DIR}
          ${OPENSTUDIOCORE_LIB_DIR}Release
    NO_DEFAULT_PATH
  )

  find_program( ${name}_RELWITHDEBINFO_DLL
    NAME ${name}.dll
    PATHS ${OPENSTUDIOCORE_LIB_DIR}RelWithDebInfo
    NO_DEFAULT_PATH
  )

  find_program( ${name}_MINSIZEREL_DLL
    NAME ${name}.dll
    PATHS ${OPENSTUDIOCORE_LIB_DIR}MinSizeRel
    NO_DEFAULT_PATH
  )

  list(APPEND OPENSTUDIOCORE_DEBUG_DLLS ${${name}_DEBUG_DLL})
  list(APPEND OPENSTUDIOCORE_RELEASE_DLLS ${${name}_RELEASE_DLL})
  list(APPEND OPENSTUDIOCORE_RELWITHDEBINFO_DLLS ${${name}_RELWITHDEBINFO_DLL})
  list(APPEND OPENSTUDIOCORE_MINSIZEREL_DLLS ${${name}_MINSIZEREL_DLL})

endmacro()

find_openstudiocore_library("openstudio_utilities")
find_openstudiocore_library("openstudio_model")
find_openstudiocore_library("openstudio_osversion")

if(WIN32)
  find_openstudiocore_library("qtwinmigrate")
endif()

find_openstudiocore_library("zkexpat")
find_openstudiocore_library("zkqwt")

#find_library(${name}_DEBUG_LIB
#  NAME ${name}
#  PATHS ${OPENSTUDIOCORE_LIB_DIR}
#        ${OPENSTUDIOCORE_LIB_DIR}Debug
#  NO_DEFAULT_PATH
#)

find_library(boost_log_DEBUG_LIB
  NAME boost_log
  PATHS ${BoostLog_DIR}
        ${BoostLog_DIR}Debug
  NO_DEFAULT_PATH
)

find_library(boost_log_RELEASE_LIB
  NAME boost_log
  PATHS ${BoostLog_DIR}
        ${BoostLog_DIR}Release
  NO_DEFAULT_PATH
)

find_library(boost_log_RELWITHDEBINFO_LIB
  NAME boost_log
  PATHS ${BoostLog_DIR}
        ${BoostLog_DIR}RelWithDebInfo
  NO_DEFAULT_PATH
)

find_library(boost_log_MINSIZEREL_LIB
  NAME boost_log
  PATHS ${BoostLog_DIR}
        ${BoostLog_DIR}MinSizeRel
  NO_DEFAULT_PATH
)

#find_package(BoostLog REQUIRED)
#include_directories(${BoostLog_INCLUDE_DIRS})
#link_directories(${BoostLog_LIBRARY_DIRS})

##############################
