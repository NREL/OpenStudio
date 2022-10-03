# This defines a list of import targets for the native extensions: ruby_extension_libs
include("${OPENSTUDIO_GEMS_DIR}/export-extensions.cmake")

# This had been broken for a long time (was using a wrong variable name for the list, so InitMacros.hxx is empty) and it still worked fine. TODO: remove?
set(INIT_CALLS "")
set(INIT_DECLARATIONS "")
foreach(LIB ${ruby_extension_libs})
  get_filename_component(LIB_NAME ${LIB} NAME_WE)
  #list (FIND LIBS "${LIB_NAME}" _index)
  #if( ${_index} GREATER -1 )
  set(INIT_DECLARATIONS "${INIT_DECLARATIONS}  void Init_${LIB_NAME}();\\\n" )
  set(INIT_CALLS "${INIT_CALLS}  Init_${LIB_NAME}();\\\n" )
  set(INIT_CALLS "${INIT_CALLS}  rb_provide(\"${LIB_NAME}.so\");\\\n" )
  #endif()
endforeach()

CONFIGURE_FILE_WITH_CHECKSUM("InitMacros.hxx.in" "${CMAKE_CURRENT_BINARY_DIR}/InitMacros.hxx")

set(MODULE_ROOT "${CONAN_OPENSTUDIO_RUBY_ROOT}")
if ("${MODULE_ROOT}" STREQUAL "")
  set(MODULE_ROOT "${CONAN_OPENSTUDIO_RUBY_ROOT_DEBUG}")
endif()

message("Searching for ruby modules in '${MODULE_ROOT}/lib/**/*.rb'")
file(GLOB_RECURSE  EXTENSION_RB  FOLLOW_SYMLINKS "${MODULE_ROOT}/lib/**/*.rb")

foreach( _FILE ${EXTENSION_RB} )
  file(RELATIVE_PATH LOCATION ${MODULE_ROOT}/lib ${_FILE})
  list(APPEND FILES ${_FILE})
  list(APPEND EMBEDDED_PATHS ${LOCATION})
endforeach()

set(GEMFILE_FILES "${OPENSTUDIO_GEMS_DIR}/openstudio-gems.gemspec" "${PROJECT_BINARY_DIR}/openstudio-gems/Gemfile" "${PROJECT_BINARY_DIR}/openstudio-gems/Gemfile.lock")
file(GLOB_RECURSE RB_FILES FOLLOW_SYMLINKS "${OPENSTUDIO_GEMS_DIR}/**/*.rb")
file(GLOB_RECURSE ERB_FILES FOLLOW_SYMLINKS "${OPENSTUDIO_GEMS_DIR}/**/*.erb")
file(GLOB_RECURSE JS_FILES FOLLOW_SYMLINKS "${OPENSTUDIO_GEMS_DIR}/**/*.js")
file(GLOB_RECURSE CSS_FILES FOLLOW_SYMLINKS "${OPENSTUDIO_GEMS_DIR}/**/*.css")
file(GLOB_RECURSE GIF_FILES FOLLOW_SYMLINKS "${OPENSTUDIO_GEMS_DIR}/**/*.gif")
file(GLOB_RECURSE PNG_FILES FOLLOW_SYMLINKS "${OPENSTUDIO_GEMS_DIR}/**/*.png")
file(GLOB_RECURSE HTML_FILES FOLLOW_SYMLINKS "${OPENSTUDIO_GEMS_DIR}/**/*.html")
file(GLOB_RECURSE IDF_FILES FOLLOW_SYMLINKS "${OPENSTUDIO_GEMS_DIR}/**/*.idf")
file(GLOB_RECURSE OSM_FILES FOLLOW_SYMLINKS "${OPENSTUDIO_GEMS_DIR}/**/*.osm")
file(GLOB_RECURSE EPW_FILES FOLLOW_SYMLINKS "${OPENSTUDIO_GEMS_DIR}/**/*.epw")
file(GLOB_RECURSE DDY_FILES FOLLOW_SYMLINKS "${OPENSTUDIO_GEMS_DIR}/**/*.ddy")
file(GLOB_RECURSE STAT_FILES FOLLOW_SYMLINKS "${OPENSTUDIO_GEMS_DIR}/**/*.stat")
file(GLOB_RECURSE CSV_FILES FOLLOW_SYMLINKS "${OPENSTUDIO_GEMS_DIR}/**/*.csv")
file(GLOB_RECURSE JSON_FILES FOLLOW_SYMLINKS "${OPENSTUDIO_GEMS_DIR}/**/*.json")
file(GLOB_RECURSE SPEC_FILES FOLLOW_SYMLINKS "${OPENSTUDIO_GEMS_DIR}/**/*.gemspec")
file(GLOB_RECURSE GZ_FILES FOLLOW_SYMLINKS "${OPENSTUDIO_GEMS_DIR}/**/*.gz")
file(GLOB_RECURSE YML_FILES FOLLOW_SYMLINKS "${OPENSTUDIO_GEMS_DIR}/**/*.yml")
file(GLOB_RECURSE OS_FILES FOLLOW_SYMLINKS "${CMAKE_SOURCE_DIR}/ruby/module/openstudio/**/*.rb")

foreach( _FILE ${GEMFILE_FILES} )
  file(RELATIVE_PATH LOCATION "${PROJECT_BINARY_DIR}/openstudio-gems" ${_FILE})
  list(APPEND FILES ${_FILE})
  list(APPEND EMBEDDED_PATHS ${LOCATION})
endforeach()

foreach( _FILE ${RB_FILES} )
  file(RELATIVE_PATH LOCATION "${PROJECT_BINARY_DIR}/openstudio-gems" ${_FILE})
  list(APPEND FILES ${_FILE})
  list(APPEND EMBEDDED_PATHS ${LOCATION})
endforeach()

foreach( _FILE ${ERB_FILES} )
  file(RELATIVE_PATH LOCATION "${PROJECT_BINARY_DIR}/openstudio-gems" ${_FILE})
  list(APPEND FILES ${_FILE})
  list(APPEND EMBEDDED_PATHS ${LOCATION})
endforeach()

foreach( _FILE ${JS_FILES} )
  file(RELATIVE_PATH LOCATION "${PROJECT_BINARY_DIR}/openstudio-gems" ${_FILE})
  list(APPEND FILES ${_FILE})
  list(APPEND EMBEDDED_PATHS ${LOCATION})
endforeach()

foreach( _FILE ${CSS_FILES} )
  file(RELATIVE_PATH LOCATION "${PROJECT_BINARY_DIR}/openstudio-gems" ${_FILE})
  list(APPEND FILES ${_FILE})
  list(APPEND EMBEDDED_PATHS ${LOCATION})
endforeach()

foreach( _FILE ${GIF_FILES} )
  file(RELATIVE_PATH LOCATION "${PROJECT_BINARY_DIR}/openstudio-gems" ${_FILE})
  list(APPEND FILES ${_FILE})
  list(APPEND EMBEDDED_PATHS ${LOCATION})
endforeach()

foreach( _FILE ${PNG_FILES} )
  file(RELATIVE_PATH LOCATION "${PROJECT_BINARY_DIR}/openstudio-gems" ${_FILE})
  list(APPEND FILES ${_FILE})
  list(APPEND EMBEDDED_PATHS ${LOCATION})
endforeach()

foreach( _FILE ${HTML_FILES} )
  file(RELATIVE_PATH LOCATION "${PROJECT_BINARY_DIR}/openstudio-gems" ${_FILE})
  list(APPEND FILES ${_FILE})
  list(APPEND EMBEDDED_PATHS ${LOCATION})
endforeach()

foreach( _FILE ${IDF_FILES} )
  file(RELATIVE_PATH LOCATION "${PROJECT_BINARY_DIR}/openstudio-gems" ${_FILE})
  list(APPEND FILES ${_FILE})
  list(APPEND EMBEDDED_PATHS ${LOCATION})
endforeach()

foreach( _FILE ${OSM_FILES} )
  file(RELATIVE_PATH LOCATION "${PROJECT_BINARY_DIR}/openstudio-gems" ${_FILE})
  list(APPEND FILES ${_FILE})
  list(APPEND EMBEDDED_PATHS ${LOCATION})
endforeach()

foreach( _FILE ${EPW_FILES} )
  file(RELATIVE_PATH LOCATION "${PROJECT_BINARY_DIR}/openstudio-gems" ${_FILE})
  list(APPEND FILES ${_FILE})
  list(APPEND EMBEDDED_PATHS ${LOCATION})
endforeach()

foreach( _FILE ${DDY_FILES} )
  file(RELATIVE_PATH LOCATION "${PROJECT_BINARY_DIR}/openstudio-gems" ${_FILE})
  list(APPEND FILES ${_FILE})
  list(APPEND EMBEDDED_PATHS ${LOCATION})
endforeach()

foreach( _FILE ${STAT_FILES} )
  file(RELATIVE_PATH LOCATION "${PROJECT_BINARY_DIR}/openstudio-gems" ${_FILE})
  list(APPEND FILES ${_FILE})
  list(APPEND EMBEDDED_PATHS ${LOCATION})
endforeach()

foreach( _FILE ${CSV_FILES} )
  file(RELATIVE_PATH LOCATION "${PROJECT_BINARY_DIR}/openstudio-gems" ${_FILE})
  list(APPEND FILES ${_FILE})
  list(APPEND EMBEDDED_PATHS ${LOCATION})
endforeach()

foreach( _FILE ${JSON_FILES} )
  file(RELATIVE_PATH LOCATION "${PROJECT_BINARY_DIR}/openstudio-gems" ${_FILE})
  list(APPEND FILES ${_FILE})
  list(APPEND EMBEDDED_PATHS ${LOCATION})
endforeach()

foreach( _FILE ${SPEC_FILES} )
  file(RELATIVE_PATH LOCATION "${PROJECT_BINARY_DIR}/openstudio-gems" ${_FILE})
  list(APPEND FILES ${_FILE})
  list(APPEND EMBEDDED_PATHS ${LOCATION})
endforeach()

foreach( _FILE ${GZ_FILES} )
  file(RELATIVE_PATH LOCATION "${PROJECT_BINARY_DIR}/openstudio-gems" ${_FILE})
  list(APPEND FILES ${_FILE})
  list(APPEND EMBEDDED_PATHS ${LOCATION})
endforeach()

foreach( _FILE ${YML_FILES} )
  file(RELATIVE_PATH LOCATION "${PROJECT_BINARY_DIR}/openstudio-gems" ${_FILE})
  list(APPEND FILES ${_FILE})
  list(APPEND EMBEDDED_PATHS ${LOCATION})
endforeach()

# DLM: this is temporary, these files should live somewhere else and be included in the shared Ruby lib
foreach( _FILE ${OS_FILES} )
  file(RELATIVE_PATH LOCATION "${CMAKE_SOURCE_DIR}/ruby/module" ${_FILE})
  list(APPEND FILES ${_FILE})
  list(APPEND EMBEDDED_PATHS ${LOCATION})
endforeach()

list(APPEND FILES "${CMAKE_CURRENT_SOURCE_DIR}/embedded_help.rb")
list(APPEND EMBEDDED_PATHS "embedded_help.rb")

list(APPEND FILES "${CMAKE_CURRENT_SOURCE_DIR}/openstudio_cli.rb")
list(APPEND EMBEDDED_PATHS "openstudio_cli.rb")

list(APPEND FILES "${CMAKE_CURRENT_SOURCE_DIR}/openstudio_init_extended.rb")
list(APPEND EMBEDDED_PATHS "openstudio_init_extended.rb")


list(APPEND FILES "${CMAKE_CURRENT_SOURCE_DIR}/measure_manager.rb")
list(APPEND EMBEDDED_PATHS "measure_manager.rb")

list(APPEND FILES "${CMAKE_CURRENT_SOURCE_DIR}/measure_manager_server.rb")
list(APPEND EMBEDDED_PATHS "measure_manager_server.rb")

embed_files("${FILES}" "${EMBEDDED_PATHS}" OUTPUT)

set_source_files_properties(EmbeddedScripting.i
  PROPERTIES CPLUSPLUS true
)

include_directories(${CMAKE_CURRENT_BINARY_DIR}
  ${PROJECT_BINARY_DIR}
  ${PROJECT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${OPENSTUDIO_GEMS_DIR}
)

add_custom_command(
  OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/embedded_scripting_wrap.cxx"
  COMMAND ${CMAKE_COMMAND} -E env SWIG_LIB="${SWIG_LIB}"
          "${SWIG_EXECUTABLE}"
          "-ruby"
          "-c++"
          -o "${CMAKE_CURRENT_BINARY_DIR}/embedded_scripting_wrap.cxx"
          "-fvirtual"
          "-I${PROJECT_SOURCE_DIR}/src"
          "-I${PROJECT_BINARY_DIR}/src"
          "-I${OPENSTUDIO_GEMS_DIR}"
          "-D_WINDOWS"
          "-Fmicrosoft"
          "${CMAKE_CURRENT_SOURCE_DIR}/EmbeddedScripting.i"
  DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/EmbeddedScripting.i"
          "${CMAKE_CURRENT_BINARY_DIR}/embedded_files.hxx"
          "EmbeddedHelp.hpp"
)

set_source_files_properties(${EMBED_SOURCE_FILES} PROPERTIES HEADER_FILE_ONLY TRUE)
source_group(embedded_files FILES ${OUTPUT})

add_library(rubyengine
  MODULE
  RubyEngine.hpp
  RubyEngine.cpp
  InitRubyEngine.cpp
  GC_Value.hpp
  ${OUTPUT}
  "${CMAKE_CURRENT_BINARY_DIR}/embedded_scripting_wrap.cxx"
)

include(GenerateExportHeader)
generate_export_header(rubyengine)

target_compile_options(rubyengine PRIVATE
  $<${is_msvc_genex}:/bigobj>
)
# -Wno-deprecated-declaration, /wd4996: suppresses deprecated warning
target_compile_options(rubyengine PRIVATE
  $<${is_msvc_genex}:/wd4996>
  $<${is_gnu_or_clang_genex}:-Wno-deprecated-declarations>
)
# -Wno-register, /wd5033: ISO C++1z does not allow "register" storage class specifier
target_compile_options(rubyengine PRIVATE
  $<${is_msvc_genex}:/wd5033>
  $<${is_gnu_or_clang_genex}:-Wno-register>
)
set_source_files_properties("${CMAKE_CURRENT_BINARY_DIR}/embedded_scripting_wrap.cxx"
  PROPERTIES COMPILE_FLAGS "-DRUBY_EMBEDDED $<${is_gnu_or_clang_genex}:-Wno-unused-variable>"
)

#add_definitions(-DRUBY_EXTCONF_H="osruby_config.h")

target_link_libraries(rubyengine
  PRIVATE
  openstudio_scriptengine
  openstudiolib
  rubybindings
  rubyinterpreter
  CONAN_PKG::fmt
  ${ruby_extension_libs}
  ${ALL_RUBY_BINDING_TARGETS}
)

if( WIN32 )
  target_link_libraries(rubyengine
    PRIVATE
    wsock32 ws2_32 Dbghelp Shlwapi Iphlpapi
  )
  target_link_options(rubyengine PRIVATE $<$<CONFIG:Debug>:/NODEFAULTLIB:MSVCRT>)
endif()

install(TARGETS rubyengine EXPORT openstudio DESTINATION bin COMPONENT "CLI")
