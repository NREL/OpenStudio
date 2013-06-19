CMAKE_MINIMUM_REQUIRED( VERSION 2.8 )
 
PROJECT(BoostLog)



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

SET( Boost_ADDITIONAL_VERSIONS "1.40" "1.47" "1.47.0" )
FIND_PACKAGE( Boost 1.40.0 REQUIRED COMPONENTS filesystem regex program_options system thread date_time serialization)
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )

INCLUDE_DIRECTORIES( ./ )

IF(WIN32)
  # C:/Program Files/Microsoft SDKs/Windows/v6.0A/bin/MC.Exe
  FIND_PROGRAM( MC_EXECUTABLE mc REQUIRED PATHS 
                "C:/Program Files/Microsoft SDKs/Windows/v6.0A/bin/" 
                "C:/Program Files/Microsoft SDKs/Windows/v7.0/bin/" 
               )
  MARK_AS_ADVANCED( MC_EXECUTABLE )
  ADD_CUSTOM_COMMAND( OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/libs/log/src/simple_event_log.h
    COMMAND ${MC_EXECUTABLE} simple_event_log.mc
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/libs/log/src/simple_event_log.mc
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/libs/log/src
  )
ENDIF()

IF(UNIX)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC ")
ENDIF()

#IF(UNIX)
#  STRING(REGEX REPLACE "\\-Werror" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
#  STRING(REGEX REPLACE "\\-Wall" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
#  STRING(REGEX REPLACE "\\-Weffc\\+\\+" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
#  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w")
#ENDIF(UNIX)
#IF(MINGW)
#  STRING(REGEX REPLACE "\\-Werror" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
#  STRING(REGEX REPLACE "\\-Wall" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
#  STRING(REGEX REPLACE "\\-Weffc\\+\\+" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
#  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w")
#ENDIF(MINGW)
#IF(MSVC)
#  # remove all warnings
#  STRING(REGEX REPLACE "\\/Wall" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
#  # remove treat warnings as errors
#  STRING(REGEX REPLACE "\\/WX" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
#  # remove set warning level 3
#  STRING(REGEX REPLACE "\\/W3" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
#  # disable warnings
#  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /w")
#ENDIF(MSVC)

#ADD_DEFINITIONS(  -DBOOST_LOG_USE_WCHAR_T  )
#ADD_DEFINITIONS(  -DBOOST_LOG_NO_THREADS  )
ADD_DEFINITIONS( -DBOOST_LOG_NO_LIB ) # disable autolinking
ADD_DEFINITIONS( -DBOOST_LOG_SETUP_NO_LIB ) # disable autolinking
ADD_DEFINITIONS( -DBOOST_LOG_NO_COMPILER_TLS )
ADD_DEFINITIONS(-DBOOST_FILESYSTEM_VERSION=2)

#Uncomment next 2 if we want to build boost_log dynamically
#ADD_DEFINITIONS( -DBOOST_LOG_BUILDING_THE_LIB )
#TARGET_LINK_LIBRARIES( boost_log ${Boost_LIBRARIES} )



#ADD_DEFINITIONS(  -DBOOST_LOG_NO_LIB )
#ADD_DEFINITIONS(  -DBOOST_LOG_SETUP_NO_LIB )

SET( boost_log_src
  libs/log/src/attribute_set.cpp
  libs/log/src/attribute_values_view.cpp
  libs/log/src/slim_string.cpp
  libs/log/src/code_conversion.cpp
  libs/log/src/core.cpp
  libs/log/src/stream_provider.cpp
  libs/log/src/severity_level.cpp
  libs/log/src/global_logger_storage.cpp
  libs/log/src/named_scope.cpp
  libs/log/src/process_id.cpp
  libs/log/src/timer.cpp
  libs/log/src/sink_frontends.cpp
  libs/log/src/text_ostream_backend.cpp
  libs/log/src/text_file_backend.cpp
  libs/log/src/syslog_backend.cpp
  libs/log/src/thread_specific.cpp
  libs/log/src/exceptions.cpp
  
  # setup
  libs/log/src/parser_utils.cpp
  libs/log/src/init_from_stream.cpp
  libs/log/src/filter_parser.cpp
  libs/log/src/formatter_parser.cpp
)

IF(UNIX)
  SET( platform_src
  )
ELSEIF(WIN32)
  SET( platform_src
    libs/log/src/event_log_backend.cpp
    libs/log/src/nt6_event_log_backend.cpp
    libs/log/src/debug_output_backend.cpp
    libs/log/src/simple_event_log.h
  )
ENDIF()

ADD_LIBRARY( boost_log 
  ${boost_log_src}
  ${platform_src}
)

#ADD_EXECUTABLE( advanced_usage libs/log/example/advanced_usage/main.cpp )
#ADD_EXECUTABLE( async_log libs/log/example/async_log/main.cpp )
#ADD_EXECUTABLE( basic_usage libs/log/example/basic_usage/main.cpp )
#ADD_EXECUTABLE( event_log libs/log/example/event_log/main.cpp )
#ADD_EXECUTABLE( multiple_files libs/log/example/multiple_files/main.cpp )
#ADD_EXECUTABLE( multiple_threads libs/log/example/multiple_threads/main.cpp )
#ADD_EXECUTABLE( rotating_file libs/log/example/rotating_file/main.cpp )
#ADD_EXECUTABLE( settings_file libs/log/example/settings_file/main.cpp )
#ADD_EXECUTABLE( syslog libs/log/example/syslog/main.cpp )
#ADD_EXECUTABLE( xml_file libs/log/example/xml_file/main.cpp )


#TARGET_LINK_LIBRARIES( advanced_usage boost_log ${Boost_LIBRARIES} )
#TARGET_LINK_LIBRARIES( async_log boost_log ${Boost_LIBRARIES} )
#TARGET_LINK_LIBRARIES( basic_usage boost_log ${Boost_LIBRARIES} )
#TARGET_LINK_LIBRARIES( event_log boost_log ${Boost_LIBRARIES} )
#TARGET_LINK_LIBRARIES( multiple_files boost_log ${Boost_LIBRARIES} )
#TARGET_LINK_LIBRARIES( multiple_threads boost_log ${Boost_LIBRARIES} )
#TARGET_LINK_LIBRARIES( rotating_file boost_log ${Boost_LIBRARIES} )
#TARGET_LINK_LIBRARIES( settings_file boost_log ${Boost_LIBRARIES} )
#TARGET_LINK_LIBRARIES( syslog boost_log ${Boost_LIBRARIES} )
#TARGET_LINK_LIBRARIES( xml_file boost_log ${Boost_LIBRARIES} )


configure_file("${CMAKE_SOURCE_DIR}/BoostLogConfig.cmake.in"
  "${CMAKE_BINARY_DIR}/BoostLogConfig.cmake" @ONLY)


