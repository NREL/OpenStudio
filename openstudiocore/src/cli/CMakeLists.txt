if (APPLE)
   find_library(COREFOUNDATION_LIBRARY CoreFoundation )
endif (APPLE)

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

file(GLOB_RECURSE RB_FILES FOLLOW_SYMLINKS "${CMAKE_BINARY_DIR}/openstudio-gems/**/*.rb")
file(GLOB_RECURSE IDF_FILES FOLLOW_SYMLINKS "${CMAKE_BINARY_DIR}/openstudio-gems/**/*.idf")
file(GLOB_RECURSE OSM_FILES FOLLOW_SYMLINKS "${CMAKE_BINARY_DIR}/openstudio-gems/**/*.osm")
file(GLOB_RECURSE EPW_FILES FOLLOW_SYMLINKS "${CMAKE_BINARY_DIR}/openstudio-gems/**/*.epw")
file(GLOB_RECURSE DDY_FILES FOLLOW_SYMLINKS "${CMAKE_BINARY_DIR}/openstudio-gems/**/*.ddy")
file(GLOB_RECURSE STAT_FILES FOLLOW_SYMLINKS "${CMAKE_BINARY_DIR}/openstudio-gems/**/*.stat")
file(GLOB_RECURSE JSON_FILES FOLLOW_SYMLINKS "${CMAKE_BINARY_DIR}/openstudio-gems/**/*.json")
file(GLOB_RECURSE SPEC_FILES FOLLOW_SYMLINKS "${CMAKE_BINARY_DIR}/openstudio-gems/**/*.gemspec")
file(GLOB_RECURSE OS_FILES FOLLOW_SYMLINKS "${CMAKE_CURRENT_SOURCE_DIR}/openstudio/**/*.rb") # DLM: this is temporary, these files should live somewhere else and be included in the shared Ruby lib

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

foreach( _FILE ${OSM_FILES} )
  file(RELATIVE_PATH LOCATION "${CMAKE_BINARY_DIR}/openstudio-gems" ${_FILE})
  list(APPEND FILES ${_FILE})
  list(APPEND EMBEDDED_PATHS ${LOCATION})
endforeach()

foreach( _FILE ${EPW_FILES} )
  file(RELATIVE_PATH LOCATION "${CMAKE_BINARY_DIR}/openstudio-gems" ${_FILE})
  list(APPEND FILES ${_FILE})
  list(APPEND EMBEDDED_PATHS ${LOCATION})
endforeach()

foreach( _FILE ${DDY_FILES} )
  file(RELATIVE_PATH LOCATION "${CMAKE_BINARY_DIR}/openstudio-gems" ${_FILE})
  list(APPEND FILES ${_FILE})
  list(APPEND EMBEDDED_PATHS ${LOCATION})
endforeach()

foreach( _FILE ${STAT_FILES} )
  file(RELATIVE_PATH LOCATION "${CMAKE_BINARY_DIR}/openstudio-gems" ${_FILE})
  list(APPEND FILES ${_FILE})
  list(APPEND EMBEDDED_PATHS ${LOCATION})
endforeach()

foreach( _FILE ${JSON_FILES} )
  file(RELATIVE_PATH LOCATION "${CMAKE_BINARY_DIR}/openstudio-gems" ${_FILE})
  list(APPEND FILES ${_FILE})
  list(APPEND EMBEDDED_PATHS ${LOCATION})
endforeach()

foreach( _FILE ${SPEC_FILES} )
  file(RELATIVE_PATH LOCATION "${CMAKE_BINARY_DIR}/openstudio-gems" ${_FILE})
  list(APPEND FILES ${_FILE})
  list(APPEND EMBEDDED_PATHS ${LOCATION})
endforeach()

# DLM: this is temporary, these files should live somewhere else and be included in the shared Ruby lib
foreach( _FILE ${OS_FILES} )
  file(RELATIVE_PATH LOCATION "${CMAKE_CURRENT_SOURCE_DIR}" ${_FILE})
  list(APPEND FILES ${_FILE})
  list(APPEND EMBEDDED_PATHS ${LOCATION})
endforeach()

list(APPEND FILES "${CMAKE_CURRENT_SOURCE_DIR}/embedded_help.rb")
list(APPEND EMBEDDED_PATHS "embedded_help.rb")

#list(APPEND FILES "${CMAKE_CURRENT_SOURCE_DIR}/scope_test.rb")
#list(APPEND EMBEDDED_PATHS "scope_test.rb")

#list(APPEND FILES "${CMAKE_CURRENT_SOURCE_DIR}/openstudio.rb")
#list(APPEND EMBEDDED_PATHS "openstudio.rb")

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
 init_openstudio
 ${EXTENSION_LIBS} 
 ${ENC_LIBS} 
 openstudio_airflow_static
 openstudio_model_static
 openstudio_energyplus_static
 openstudio_measure_static
 openstudio_osversion_static
 openstudio_sdd_static
 openstudio_isomodel_static
 openstudio_gbxml_static
 openstudio_radiance_static
)

if (UNIX AND NOT APPLE)
  target_link_libraries(openstudio ${RUBY_STATIC_LIBRARY}  "dl" "crypt" "ssl")
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

install(TARGETS openstudio DESTINATION bin COMPONENT "CLI")

if( BUILD_PAT )
  if( APPLE )
    install(TARGETS openstudio
      DESTINATION ParametricAnalysisTool.app/Contents/Resources/OpenStudio/bin/
      COMPONENT PAT
    )
  endif()
endif()


