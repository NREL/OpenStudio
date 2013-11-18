# - Find MongoDB
# Find the MongoDB includes and client library
# This module defines
#  MongoDB_INCLUDE_DIR, where to find mongo/client/dbclient.h
#  MongoDB_LIBRARIES, the libraries needed to use MongoDB.
#  MongoDB_FOUND, If false, do not try to use MongoDB.

if(MongoDB_INCLUDE_DIR AND MongoDB_LIBRARIES)
   set(MongoDB_FOUND TRUE)

else(MongoDB_INCLUDE_DIR AND MongoDB_LIBRARIES)

  find_path(MongoDB_INCLUDE_DIR mongo/client/dbclient.h
      ${CMAKE_INCLUDE_PATH}
      /usr/include/
      /usr/local/include/
      /usr/include/mongo/
      /usr/local/include/mongo/
      /opt/mongo/include/
      $ENV{ProgramFiles}/Mongo/*/include
      $ENV{SystemDrive}/Mongo/*/include
      )
  
  if(WIN32)
    
    find_library(MongoDB_LIBRARIES_RELEASE NAMES mongoclient
        PATHS
        ${CMAKE_LIBRARY_PATH}/mongo 
        $ENV{ProgramFiles}/Mongo/*/lib
        $ENV{SystemDrive}/Mongo/*/lib
        )
    
    IF(MongoDB_LIBRARIES_RELEASE)
      SET(MongoDB_LIBRARIES "${MongoDB_LIBRARIES}optimized;${MongoDB_LIBRARIES_RELEASE};")
    ENDIF(MongoDB_LIBRARIES_RELEASE)
  
    find_library(MongoDB_LIBRARIES_DEBUG NAMES mongoclientd
        PATHS
        ${CMAKE_LIBRARY_PATH}/mongo 
        $ENV{ProgramFiles}/Mongo/*/lib
        $ENV{SystemDrive}/Mongo/*/lib
        )
    
    IF(MongoDB_LIBRARIES_DEBUG)
      SET(MongoDB_LIBRARIES "${MongoDB_LIBRARIES}debug;${MongoDB_LIBRARIES_DEBUG};")
    ENDIF(MongoDB_LIBRARIES_DEBUG)     
    
    SET(MongoDB_LIBRARIES "${MongoDB_LIBRARIES}" CACHE STRING "Mongo libraries")

  else(WIN32)
    find_library(MongoDB_LIBRARIES NAMES mongoclient
        PATHS
        /usr/lib
      /usr/lib64
        /usr/lib/mongo
      /usr/lib64/mongo
        /usr/local/lib
      /usr/local/lib64
        /usr/local/lib/mongo
      /usr/local/lib64/mongo
      /opt/mongo/lib
      /opt/mongo/lib64
        )
  endif(WIN32)

  if(MongoDB_INCLUDE_DIR AND MongoDB_LIBRARIES)
    set(MongoDB_FOUND TRUE)
    message(STATUS "Found MongoDB: ${MongoDB_INCLUDE_DIR}, ${MongoDB_LIBRARIES}")
  else(MongoDB_INCLUDE_DIR AND MongoDB_LIBRARIES)
    set(MongoDB_FOUND FALSE)
    if (MongoDB_FIND_REQUIRED)
      message(FATAL_ERROR "MongoDB not found.")
    else (MongoDB_FIND_REQUIRED)
      message(STATUS "MongoDB not found.")
    endif (MongoDB_FIND_REQUIRED)
  endif(MongoDB_INCLUDE_DIR AND MongoDB_LIBRARIES)

  mark_as_advanced(MongoDB_INCLUDE_DIR MongoDB_LIBRARIES MongoDB_LIBRARIES_RELEASE MongoDB_LIBRARIES_DEBUG)

endif(MongoDB_INCLUDE_DIR AND MongoDB_LIBRARIES)
