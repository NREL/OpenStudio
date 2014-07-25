# Boost ######################
set(Boost_USE_STATIC_LIBS ON)

find_package(Boost 1.55.0 COMPONENTS filesystem regex program_options system thread date_time serialization log)
include_directories(${Boost_INCLUDE_DIR})

add_definitions(-DBOOST_UBLAS_CHECK_ENABLE)
add_definitions(-DBOOST_UBLAS_TYPE_CHECK)
add_definitions(-DBOOST_UBLAS_NO_STD_CERR)
add_definitions(-DBOOST_CHRONO_HEADER_ONLY)
##############################

# Qt #########################
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

##############################

# OpenStudio #################

add_definitions(-DBOOST_LOG_NO_LIB) # disable autolinking
add_definitions(-DBOOST_LOG_SETUP_NO_LIB) # disable autolinking
add_definitions(-DBOOST_LOG_NO_COMPILER_TLS) # disable autolinking

include_directories(${OPENSTUDIOCORE_INCLUDE_DIR})
include_directories("${OPENSTUDIOCORE_BUILD_DIR}/src")

link_directories(${OPENSTUDIOCORE_LIB_DIR})

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

##############################
