
if (APPLE)
   find_library(COREFOUNDATION_LIBRARY CoreFoundation )
endif (APPLE)

set(LIBS 
  bigdecimal
  continuation
  coverage
  date_core
  dbm
  digest
  etc
  fcntl
  fiber
  ##fiddle
  ##nkf
  objspace
  pathname
  ##psych
  pty
  ##readline
  ripper
  rbconfig
  sdbm
  socket
  stringio
  strscan
  syslog
  thread
  ##tcltklib
)

set(EXTENSION_LIBS "")
set(EXTENSION_LIBS_LIST "")
if( UNIX )
  file(GLOB EXTENSION_LIBS "${RUBY_INSTALL_DIR}/lib/ext/*.a")
  set(EXTENSION_LIBS_LIST ${EXTENSION_LIBS})

  file(GLOB ENC_LIBS "${RUBY_INSTALL_DIR}/lib/enc/*.a")
  set(ENC_LIBS_LIST ${ENC_LIBS})
else()
  file(GLOB EXTENSION_LIBS_RELEASE "${RUBY_INSTALL_DIR}/lib/ext/*.lib")
  file(GLOB EXTENSION_LIBS_DEBUG "${RUBY_INSTALL_DIR}/../debug/lib/ext/*.lib")
  set(EXTENSION_LIBS optimized ${EXTENSION_LIBS_RELEASE} debug ${EXTENSION_LIBS_DEBUG})
  set(EXTENSION_LIBS_LIST ${EXTENSION_LIBS_RELEASE})

  file(GLOB ENC_LIBS_RELEASE "${RUBY_INSTALL_DIR}/lib/enc/*.lib")
  file(GLOB ENC_LIBS_DEBUG "${RUBY_INSTALL_DIR}/../debug/lib/enc/*.lib")
  set(ENC_LIBS optimized ${ENC_LIBS_RELEASE} debug ${ENC_LIBS_DEBUG})
  set(ENC_LIBS_LIST ${ENC_LIBS_RELEASE})
endif()

set(INIT_CALLS "")
set(INIT_DECLARATIONS "")
foreach(LIB ${EXTENSION_LIBS_LIST})
  get_filename_component(LIB_NAME ${LIB} NAME_WE)
  list (FIND LIBS "${LIB_NAME}" _index)
  if( ${_index} GREATER -1 )
    set(INIT_DECLARATIONS "${INIT_DECLARATIONS}  void Init_${LIB_NAME}();\\\n" )
    set(INIT_CALLS "${INIT_CALLS}  Init_${LIB_NAME}();\\\n" )
    set(INIT_CALLS "${INIT_CALLS}  rb_provide(\"${LIB_NAME}.so\");\\\n" )
  endif()
endforeach()

configure_file("InitMacros.hxx.in" "InitMacros.hxx")

file(GLOB_RECURSE  EXTENSION_RB  FOLLOW_SYMLINKS "${RUBY_POSSIBLE_LIB_DIR}/**/*.rb")

foreach( _FILE ${EXTENSION_RB} )
  file(RELATIVE_PATH LOCATION ${RUBY_POSSIBLE_LIB_DIR} ${_FILE})
  list(APPEND FILES ${_FILE})
  list(APPEND EMBEDDED_PATHS ${LOCATION})
endforeach()

file(GLOB_RECURSE RB_FILES FOLLOW_SYMLINKS "${CMAKE_BINARY_DIR}/openstudio-gems/ruby/2.0.0/bundler/**/*.rb")
file(GLOB_RECURSE IDF_FILES FOLLOW_SYMLINKS "${CMAKE_BINARY_DIR}/openstudio-gems/ruby/2.0.0/bundler/**/*.idf")
file(GLOB_RECURSE JSON_FILES FOLLOW_SYMLINKS "${CMAKE_BINARY_DIR}/openstudio-gems/ruby/2.0.0/bundler/**/*.json")

foreach( _FILE ${RB_FILES} )
  file(RELATIVE_PATH LOCATION "${CMAKE_BINARY_DIR}/openstudio-gems" ${_FILE})
  list(APPEND FILES ${_FILE})
  list(APPEND EMBEDDED_PATHS ${LOCATION})
endforeach()

foreach( _FILE ${IDF_FILES} )
  file(RELATIVE_PATH LOCATION "${CMAKE_BINARY_DIR}/openstudio-gems" ${_FILE})
  list(APPEND FILES ${_FILE})
  list(APPEND EMBEDDED_PATHS ${LOCATION})
endforeach()

foreach( _FILE ${JSON_FILES} )
  file(RELATIVE_PATH LOCATION "${CMAKE_BINARY_DIR}/openstudio-gems" ${_FILE})
  list(APPEND FILES ${_FILE})
  list(APPEND EMBEDDED_PATHS ${LOCATION})
endforeach()

list(APPEND FILES "${CMAKE_CURRENT_SOURCE_DIR}/embedded_help.rb")
list(APPEND EMBEDDED_PATHS "embedded_help.rb")

list(APPEND FILES "${CMAKE_CURRENT_SOURCE_DIR}/scope_test.rb")
list(APPEND EMBEDDED_PATHS "scope_test.rb")

list(APPEND FILES "${CMAKE_CURRENT_SOURCE_DIR}/openstudio.rb")
list(APPEND EMBEDDED_PATHS "openstudio.rb")

list(APPEND FILES "${CMAKE_CURRENT_SOURCE_DIR}/openstudio_cli.rb")
list(APPEND EMBEDDED_PATHS "openstudio_cli.rb")

list(APPEND FILES "${CMAKE_CURRENT_SOURCE_DIR}/measure_manager.rb")
list(APPEND EMBEDDED_PATHS "measure_manager.rb")

list(APPEND FILES "${CMAKE_CURRENT_SOURCE_DIR}/measure_manager_server.rb")
list(APPEND EMBEDDED_PATHS "measure_manager_server.rb")

embed_files("${FILES}" "${EMBEDDED_PATHS}" OUTPUT)

set_source_files_properties(EmbeddedScripting.i
  PROPERTIES CPLUSPLUS true
)

include_directories(${RUBY_INCLUDE_DIRS} ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_BINARY_DIR} ${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR} )

add_custom_command(
  OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/SWIGRubyRuntime.hxx"
  COMMAND "${SWIG_EXECUTABLE}"
          "-ruby"
          -external-runtime "${CMAKE_CURRENT_BINARY_DIR}/SWIGRubyRuntime.hxx"
)

add_custom_command(
  OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/embedded_scripting_wrap.cxx"
  COMMAND "${SWIG_EXECUTABLE}"
          "-ruby"
          "-c++"
          -o "${CMAKE_CURRENT_BINARY_DIR}/embedded_scripting_wrap.cxx"
          "-fvirtual"
          "-I${CMAKE_SOURCE_DIR}/src"
          "-I${CMAKE_BINARY_DIR}/src"
          "-D_WINDOWS"
          "-Fmicrosoft"
          "${CMAKE_CURRENT_SOURCE_DIR}/EmbeddedScripting.i"
  DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/EmbeddedScripting.i"
          "${CMAKE_CURRENT_BINARY_DIR}/embedded_files.hxx"
          "EmbeddedHelp.hpp"
)

set_source_files_properties(${EMBED_SOURCE_FILES} PROPERTIES HEADER_FILE_ONLY TRUE)

source_group(embedded_files FILES ${OUTPUT})

add_executable(openstudio
 main.cpp
 "${CMAKE_CURRENT_BINARY_DIR}/SWIGRubyRuntime.hxx"
 RubyException.hpp
 RubyInterpreter.hpp
 "${CMAKE_CURRENT_BINARY_DIR}/embedded_scripting_wrap.cxx"
 ${OUTPUT}
)

set_source_files_properties("${CMAKE_CURRENT_BINARY_DIR}/embedded_scripting_wrap.cxx" PROPERTIES COMPILE_FLAGS "-DRUBY_EMBEDDED" )

add_definitions(-DRUBY_EXTCONF_H="osruby_config.h")

target_link_libraries(openstudio 
 ${EXTENSION_LIBS} 
 ${ENC_LIBS} 
 ruby_OpenStudioAirflow
 ruby_OpenStudioModelCore
 ruby_OpenStudioModelSimulation
 ruby_OpenStudioUtilitiesCore
 ruby_OpenStudioUtilitiesPlot
 ruby_OpenStudioEnergyPlus
 ##ruby_OpenStudioModelEditor
 ruby_OpenStudioOSVersion
 ruby_OpenStudioUtilitiesData
 ruby_OpenStudioUtilitiesSql
 ##ruby_OpenStudioGBXML
 ruby_OpenStudioModelGenerators
 ##ruby_OpenStudioRadiance
 #ruby_OpenStudioUtilitiesEconomics
 ruby_OpenStudioUtilitiesTime
 ##ruby_OpenStudioISOModel
 ruby_OpenStudioModelGeometry
 ##ruby_OpenStudioSDD
 ruby_OpenStudioUtilitiesFileTypes
 ruby_OpenStudioUtilitiesUnits
 ##ruby_OpenStudioLib
 ruby_OpenStudioModelHVAC
 ruby_OpenStudioUtilities
 ruby_OpenStudioUtilitiesGeometry
 ruby_OpenStudioMeasure
 ruby_OpenStudioModelRefrigeration
 ruby_OpenStudioUtilitiesBCL
 ruby_OpenStudioUtilitiesIdd
 ruby_OpenStudioModel
 ruby_OpenStudioModelResources
 ruby_OpenStudioUtilitiesCloud
 ruby_OpenStudioUtilitiesIdf
)

target_link_libraries(openstudio
  openstudio_utilities
  ${Boost_LIBRARIES}
  ${QT_LIBS}
  init_openstudio
)

if (UNIX AND NOT APPLE)
  target_link_libraries(openstudio ${RUBY_STATIC_LIBRARY}  "dl" "crypt")
elseif(WIN32)
  target_link_libraries(openstudio debug ${RUBY_STATIC_LIBRARY_DEBUG} optimized ${RUBY_STATIC_LIBRARY})
  target_link_libraries(openstudio wsock32 ws2_32 Dbghelp Shlwapi)
  set_target_properties(openstudio PROPERTIES LINK_FLAGS_DEBUG "/NODEFAULTLIB:MSVCRT")
else()
  target_link_libraries(openstudio ${RUBY_STATIC_LIBRARY} )
endif()

if( APPLE )
  target_link_libraries(openstudio ${COREFOUNDATION_LIBRARY} )
endif()

install(TARGETS openstudio DESTINATION bin)


