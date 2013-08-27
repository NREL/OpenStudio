
# Boost ######################
IF(APPLE)
  set(Boost_USE_STATIC_LIBS ON)  
ENDIF()
IF (WIN32)
  set(Boost_USE_STATIC_LIBS ON)  
ENDIF (WIN32)

SET( Boost_ADDITIONAL_VERSIONS "1.40" )
FIND_PACKAGE( Boost 1.40.0 REQUIRED COMPONENTS filesystem regex program_options system thread date_time serialization)
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )
MARK_AS_ADVANCED( Boost_LIB_DIAGNOSTIC_DEFINITIONS )
##############################

# Qt #########################
FIND_PACKAGE (Qt4 4.6.0 COMPONENTS QtCore QtGui QtSql QtNetwork QtXml QtWebKit REQUIRED)
INCLUDE( ${QT_USE_FILE} )
INCLUDE_DIRECTORIES( ${QT_INCLUDE_DIR} )
INCLUDE_DIRECTORIES( ${QT_QTWEBKIT_INCLUDE_DIR} )

MARK_AS_ADVANCED( QT_QMAKE_EXECUTABLE QT_QTMOTIF_INCLUDE_DIR QT_QTMOTIF_LIBRARY_DEBUG QT_QTMOTIF_LIBRARY_RELEASE )

# This sets the windows build that will need the special winmain@16 call. Qt provides
#  this for us in the qtmain.lib file. Using this cmake code will ensure we have it
#  linked into our build. Not needed on Unix/OS X/Linux which is why we have the 
#  IF(WIN32) conditional.
IF (WIN32)
    SET (QT_USE_QTMAIN TRUE)
ENDIF (WIN32)

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
LIST( APPEND QT_LIBS ${QT_QTWEBKIT_LIBRARY})
LIST( APPEND QT_LIBS ${QT_QTWEBKIT_LIB_DEPENDENCIES})
##############################

# OpenStudio #################

ADD_DEFINITIONS( -DBOOST_LOG_NO_LIB ) # disable autolinking
ADD_DEFINITIONS( -DBOOST_LOG_SETUP_NO_LIB ) # disable autolinking
ADD_DEFINITIONS( -DBOOST_LOG_NO_COMPILER_TLS ) # disable autolinkin

include_directories(${OPENSTUDIOCORE_INCLUDE_DIR})
include_directories("${OPENSTUDIOCORE_INCLUDE_DIR}/dependencies/BoostLog")
include_directories("${OPENSTUDIOCORE_BUILD_DIR}/src")

link_directories( ${OPENSTUDIOCORE_LIB_DIR} )
link_directories( ${BOOSTLOG_LIB_DIR} )

MACRO( FIND_OPENSTUDIOCORE_LIBRARY name )

  find_library( ${name}_DEBUG_LIB 
    NAME ${name} 
    PATHS ${OPENSTUDIOCORE_LIB_DIR}Debug
    NO_DEFAULT_PATH 
  )
  
  find_library( ${name}_RELEASE_LIB 
    NAME ${name}
    PATHS ${OPENSTUDIOCORE_LIB_DIR}
          ${OPENSTUDIOCORE_LIB_DIR}Release
    NO_DEFAULT_PATH 
  )
    
  find_library( ${name}_RELWITHDEBINFO_LIB 
    NAME ${name} 
    PATHS ${OPENSTUDIOCORE_LIB_DIR}RelWithDebInfo
    NO_DEFAULT_PATH 
  )
  
  find_library( ${name}_MINSIZEREL_LIB 
    NAME ${name}
    PATHS ${OPENSTUDIOCORE_LIB_DIR}MinSizeRel
    NO_DEFAULT_PATH 
  )

  if( ${name}_DEBUG_LIB )
    list( APPEND OPENSTUDIOCORE_LIBS debug ${${name}_DEBUG_LIB} )
  endif()

  if( ${name}_RELEASE_LIB )
    list( APPEND OPENSTUDIOCORE_LIBS optimized ${${name}_RELEASE_LIB} )
  endif()
  
  list( APPEND OPENSTUDIOCORE_DEBUG_LIBS ${${name}_DEBUG_LIB} )    
  list( APPEND OPENSTUDIOCORE_RELEASE_LIBS ${${name}_RELEASE_LIB} )    
  list( APPEND OPENSTUDIOCORE_RELWITHDEBINFO_LIBS ${${name}_RELWITHDEBINFO_LIB} )    
  list( APPEND OPENSTUDIOCORE_MINSIZEREL_LIBS ${${name}_MINSIZEREL_LIB} )  
  
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

  list( APPEND OPENSTUDIOCORE_DEBUG_DLLS ${${name}_DEBUG_DLL} )    
  list( APPEND OPENSTUDIOCORE_RELEASE_DLLS ${${name}_RELEASE_DLL} )    
  list( APPEND OPENSTUDIOCORE_RELWITHDEBINFO_DLLS ${${name}_RELWITHDEBINFO_DLL} )    
  list( APPEND OPENSTUDIOCORE_MINSIZEREL_DLLS ${${name}_MINSIZEREL_DLL} )    
    
ENDMACRO()

FIND_OPENSTUDIOCORE_LIBRARY("openstudio_utilities")
FIND_OPENSTUDIOCORE_LIBRARY("openstudio_model")
FIND_OPENSTUDIOCORE_LIBRARY("openstudio_osversion")

IF( WIN32 )
  FIND_OPENSTUDIOCORE_LIBRARY("qtwinmigrate")
ENDIF( WIN32 )

FIND_OPENSTUDIOCORE_LIBRARY("zkexpat")
FIND_OPENSTUDIOCORE_LIBRARY("zkqwt")

#find_library( ${name}_DEBUG_LIB 
#    NAME ${name} 
#    PATHS ${OPENSTUDIOCORE_LIB_DIR}
#          ${OPENSTUDIOCORE_LIB_DIR}Debug
#    NO_DEFAULT_PATH )

find_library( boost_log_DEBUG_LIB 
  NAME boost_log 
  PATHS ${BoostLog_DIR}
        ${BoostLog_DIR}Debug
  NO_DEFAULT_PATH )

find_library( boost_log_RELEASE_LIB 
  NAME boost_log
  PATHS ${BoostLog_DIR}
        ${BoostLog_DIR}Release
  NO_DEFAULT_PATH )
  
find_library( boost_log_RELWITHDEBINFO_LIB 
  NAME boost_log
  PATHS ${BoostLog_DIR}
        ${BoostLog_DIR}RelWithDebInfo
  NO_DEFAULT_PATH )

find_library( boost_log_MINSIZEREL_LIB 
  NAME boost_log
  PATHS ${BoostLog_DIR}
        ${BoostLog_DIR}MinSizeRel
  NO_DEFAULT_PATH )
    
#find_package(BoostLog REQUIRED)
#INCLUDE_DIRECTORIES( ${BoostLog_INCLUDE_DIRS} )
#LINK_DIRECTORIES( ${BoostLog_LIBRARY_DIRS} )

##############################

