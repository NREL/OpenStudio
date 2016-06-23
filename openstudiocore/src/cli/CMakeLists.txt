
IF (APPLE)
   find_library(COREFOUNDATION_LIBRARY CoreFoundation )
ENDIF (APPLE)

file(READ "${RUBY_SOURCE_DIR}/enc/encinit.c" ENCINIT_FILE_CONTENT)
string(REGEX MATCHALL "init_enc\\([a-zA-Z0-9_]*\\);" ENCINIT_MATCH "${ENCINIT_FILE_CONTENT}")
set(ENC_INIT_DECLARATIONS "")
set(ENC_INIT_CALLS "")
foreach(MATCH ${ENCINIT_MATCH})
  string(REPLACE "init_enc(" "" MATCH "${MATCH}")
  string(REPLACE ")" "" MATCH "${MATCH}")
  set(ENC_INIT_DECLARATIONS "${ENC_INIT_DECLARATIONS} void Init_${MATCH}();\\\n" )
  set(ENC_INIT_CALLS "${ENC_INIT_CALLS} Init_${MATCH}();\\\n" )
  set(ENC_INIT_CALLS "${ENC_INIT_CALLS} rb_provide(\"enc/${MATCH}.so\");\\\n" )
endforeach()

string(REGEX MATCHALL "init_trans\\([a-zA-Z0-9_]*\\);" TRANSINIT_MATCH "${ENCINIT_FILE_CONTENT}")
set(TRANS_INIT_DECLARATIONS "")
set(TRANS_INIT_CALLS "")
foreach(MATCH ${TRANSINIT_MATCH})
  string(REPLACE "init_trans(" "" MATCH "${MATCH}")
  string(REPLACE ")" "" MATCH "${MATCH}")
  set(TRANS_INIT_DECLARATIONS "${TRANS_INIT_DECLARATIONS} void Init_trans_${MATCH}();\\\n" )
  set(TRANS_INIT_CALLS "${TRANS_INIT_CALLS} Init_trans_${MATCH}();\\\n" )
  set(TRANS_INIT_CALLS "${TRANS_INIT_CALLS} rb_provide(\"enc/trans/${MATCH}.so\");\\\n" )
endforeach()

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
  file(GLOB EXTENSION_LIBS "${RUBY_SOURCE_DIR}/ext/**/*.a")
  set(EXTENSION_LIBS_LIST ${EXTENSION_LIBS})
else()
  file(GLOB EXTENSION_LIBS_RELEASE "${RUBY_SOURCE_DIR}/ext/**/*.lib")
  file(GLOB EXTENSION_LIBS_DEBUG "${RUBY_SOURCE_DIR_DEBUG}/ext/**/*.lib")
  set(EXTENSION_LIBS optimized ${EXTENSION_LIBS_RELEASE} debug ${EXTENSION_LIBS_DEBUG})
  set(EXTENSION_LIBS_LIST ${EXTENSION_LIBS_RELEASE})
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

#foreach( _FILE ${EXTENSION_RB} )
#  file(RELATIVE_PATH LOCATION ${RUBY_POSSIBLE_LIB_DIR} ${_FILE})
#  list(APPEND EMBED_RB ${_FILE} ${LOCATION})
#endforeach()

foreach( _FILE ${EXTENSION_RB} )
  file(RELATIVE_PATH LOCATION ${RUBY_POSSIBLE_LIB_DIR} ${_FILE})
  list(APPEND FILES ${_FILE})
  list(APPEND EMBEDDED_PATHS ${LOCATION})
endforeach()

#add_custom_command(
#  OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/gemstage/gems/openstudio-workflow-1.0.0.alpha.0/lib/openstudio-workflow.rb"
#  COMMAND "gem" "install" "--install-dir" "${CMAKE_CURRENT_BINARY_DIR}/gemstage" "${OPENSTUDIO_WORKFLOW_GEM}"
#  DEPENDS "${OPENSTUDIO_WORKFLOW_GEM}"
#)

file(GLOB_RECURSE WORKFLOW_RB FOLLOW_SYMLINKS "${OPENSTUDIO_WORKFLOW_GEM}/**/*.rb")
file(GLOB_RECURSE WORKFLOW_IDF FOLLOW_SYMLINKS "${OPENSTUDIO_WORKFLOW_GEM}/**/*.idf")

foreach( _FILE ${WORKFLOW_RB} )
  file(RELATIVE_PATH LOCATION "${OPENSTUDIO_WORKFLOW_GEM}/gems" ${_FILE})
  list(APPEND FILES ${_FILE})
  list(APPEND EMBEDDED_PATHS ${LOCATION})
endforeach()

foreach( _FILE ${WORKFLOW_IDF} )
  file(RELATIVE_PATH LOCATION "${OPENSTUDIO_WORKFLOW_GEM}/gems" ${_FILE})
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
          "-D_WINDOWS"
          "-Fmicrosoft"
          "${CMAKE_CURRENT_SOURCE_DIR}/EmbeddedScripting.i"
  DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/EmbeddedScripting.i"
          "${CMAKE_CURRENT_BINARY_DIR}/embedded_files.hxx"
          "EmbeddedHelp.hpp"
)

set_source_files_properties(${EMBED_SOURCE_FILES} PROPERTIES HEADER_FILE_ONLY TRUE)

add_executable(openstudio
 main.cpp
 "${CMAKE_CURRENT_BINARY_DIR}/SWIGRubyRuntime.hxx"
 RubyException.hpp
 RubyInterpreter.hpp
 "${CMAKE_CURRENT_BINARY_DIR}/embedded_scripting_wrap.cxx"
 ${OUTPUT}
)

set_source_files_properties("${CMAKE_CURRENT_BINARY_DIR}/embedded_scripting_wrap.cxx" PROPERTIES COMPILE_FLAGS "-DRUBY_EMBEDDED" )

set(ENCODING_LIB "")
set(TRANSLATION_LIB "")
if( UNIX )
  set(ENCODING_LIB "${RUBY_SOURCE_DIR}/enc/libenc.a")
  set(TRANSLATION_LIB "${RUBY_SOURCE_DIR}/enc/libtrans.a")
else()
  target_link_libraries(openstudio wsock32 ws2_32 Iphlpapi Dbghelp Shlwapi)
  set(ENCODING_LIB optimized "${RUBY_SOURCE_DIR}/enc/libenc.lib" debug "${RUBY_SOURCE_DIR_DEBUG}/enc/libenc.lib")
  set(TRANSLATION_LIB optimized "${RUBY_SOURCE_DIR}/enc/libtrans.lib" debug "${RUBY_SOURCE_DIR_DEBUG}/enc/libtrans.lib")
endif()

add_definitions(-DRUBY_EXTCONF_H="osruby_config.h")

# TODO: use finder or something better
if( WIN32 )
  set(OTHER_EXT_LIBS optimized 
    "${RUBY_SOURCE_DIR}/ext/json/generator/generator.lib" 
    "${RUBY_SOURCE_DIR}/ext/json/parser/parser.lib"
    "${RUBY_SOURCE_DIR}/ext/digest/md5/md5.lib"
    "${RUBY_SOURCE_DIR}/ext/digest/rmd160/rmd160.lib"
    "${RUBY_SOURCE_DIR}/ext/digest/sha1/sha1.lib"
    "${RUBY_SOURCE_DIR}/ext/digest/sha2/sha2.lib"
    debug
    "${RUBY_SOURCE_DIR_DEBUG}/ext/json/generator/generator.lib" 
    "${RUBY_SOURCE_DIR_DEBUG}/ext/json/parser/parser.lib"
    "${RUBY_SOURCE_DIR_DEBUG}/ext/digest/md5/md5.lib"
    "${RUBY_SOURCE_DIR_DEBUG}/ext/digest/rmd160/rmd160.lib"
    "${RUBY_SOURCE_DIR_DEBUG}/ext/digest/sha1/sha1.lib"
    "${RUBY_SOURCE_DIR_DEBUG}/ext/digest/sha2/sha2.lib")
else()
  set(OTHER_EXT_LIBS 
    "${RUBY_SOURCE_DIR}/ext/json/generator/generator.a" 
    "${RUBY_SOURCE_DIR}/ext/json/parser/parser.a"
    "${RUBY_SOURCE_DIR}/ext/rbconfig/sizeof/sizeof.a"
    "${RUBY_SOURCE_DIR}/ext/digest/md5/md5.a"
    "${RUBY_SOURCE_DIR}/ext/digest/rmd160/rmd160.a"
    "${RUBY_SOURCE_DIR}/ext/digest/sha1/sha1.a"
    "${RUBY_SOURCE_DIR}/ext/digest/sha2/sha2.a")
endif()

target_link_libraries(openstudio 
 ${EXTENSION_LIBS} 
 ${OTHER_EXT_LIBS}
 ${ENCODING_LIB}
 ${TRANSLATION_LIB}
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
)

if (UNIX AND NOT APPLE)
  target_link_libraries(openstudio ${RUBY_LIBRARY}  "dl" "crypt")
elseif(WIN32)
  target_link_libraries(openstudio debug ${RUBY_LIBRARY} optimized ${RUBY_LIBRARY})
  set_target_properties(openstudio PROPERTIES LINK_FLAGS_DEBUG "/NODEFAULTLIB:MSVCRT")
else()
  target_link_libraries(openstudio ${RUBY_LIBRARY} )
endif()


if( APPLE )
  target_link_libraries(openstudio ${COREFOUNDATION_LIBRARY} )
endif()

install(TARGETS openstudio DESTINATION bin)


