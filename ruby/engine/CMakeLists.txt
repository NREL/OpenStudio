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

set(configs "RELEASE" "DEBUG" "RELWITHDEBINFO" "MINSIZEREL")
foreach(_config ${configs})
  message(DEBUG "Testing config=${config}")
  set(MODULE_ROOT "${ruby_PACKAGE_FOLDER_${_config}}")
  if (NOT  "${MODULE_ROOT}" STREQUAL "")
    message("Found Ruby package folder for config ${_config} at ${MODULE_ROOT}")
    break()
  endif()
endforeach()

if("${MODULE_ROOT}" STREQUAL "")
  message(FATAL_ERROR "MODULE_ROOT for ruby not found")
endif()


# TODO: Previously just globbing for .rb files, I think we're missing a lot of files, the gemspecs in particular, the .pem SSL certs for rubygems, those rbs files too?
glob_for_file_exts_and_prepare_for_embedded("${MODULE_ROOT}/lib" rubyFiles rubyEmbeddedPaths VERBOSE EXTENSIONS rb gemspec pem js css gif png json rbs yml)
list(APPEND FILES ${rubyFiles})
list(APPEND EMBEDDED_PATHS ${rubyEmbeddedPaths})

set(GEMFILE_FILES
  # "${OPENSTUDIO_GEMS_DIR}/openstudio-gems.gemspec"
  "${PROJECT_BINARY_DIR}/openstudio-gems/Gemfile" "${PROJECT_BINARY_DIR}/openstudio-gems/Gemfile.lock")
foreach( _FILE ${GEMFILE_FILES} )
  file(RELATIVE_PATH LOCATION "${PROJECT_BINARY_DIR}/openstudio-gems" ${_FILE})
  list(APPEND FILES ${_FILE})
  list(APPEND EMBEDDED_PATHS ${LOCATION})
endforeach()

# Note: from export-extensions.cmake
list(LENGTH gemFiles _nFoundGemFiles)
list(LENGTH gemEmbeddedPaths _nFoundGemsEmbeddedPaths)
message("gemFiles=${_nFoundGemFiles}, gemEmbeddedPaths=${_nFoundGemsEmbeddedPaths}")
# glob_for_file_exts_and_prepare_for_embedded("${OPENSTUDIO_GEMS_DIR}" gemFiles gemEmbeddedPaths
#  VERBOSE
#  EXTENSIONS rb data erb js css gif png html idf osm epw ddy stat csv json gemspec gz yml
#)

list(APPEND FILES ${gemFiles})
list(APPEND EMBEDDED_PATHS ${gemEmbeddedPaths})

# DLM: this is temporary, these files should live somewhere else and be included in the shared Ruby lib
file(GLOB_RECURSE OS_FILES FOLLOW_SYMLINKS "${CMAKE_SOURCE_DIR}/ruby/module/openstudio/**/*.rb")
foreach( _FILE ${OS_FILES} )
  file(RELATIVE_PATH LOCATION "${CMAKE_SOURCE_DIR}/ruby/module" ${_FILE})
  list(APPEND FILES ${_FILE})
  list(APPEND EMBEDDED_PATHS ${LOCATION})
endforeach()

set(local_rb_files
  embedded_help.rb
  openstudio_cli.rb
  openstudio_init_extended.rb
  measure_manager.rb
  measure_manager_server.rb
)

foreach (_FILE ${local_rb_files})
  list(APPEND FILES "${CMAKE_CURRENT_SOURCE_DIR}/${_FILE}")
  list(APPEND EMBEDDED_PATHS "${_FILE}")
endforeach()

set(_contents "")
foreach(_line ${FILES})
   set(_contents "${_contents}${_line}\n")
endforeach()
file(WRITE "${PROJECT_BINARY_DIR}/embedded_files.txt" "${_contents}")
set(_contents "")
foreach(_line ${EMBEDDED_PATHS})
   set(_contents "${_contents}${_line}\n")
endforeach()
file(WRITE "${PROJECT_BINARY_DIR}/embedded_paths.txt" "${_contents}")

list(LENGTH FILES nFoundToEmbed)
message("Total files to embed: ${nFoundToEmbed} files")

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
  COMMAND ${CMAKE_COMMAND} -E env SWIG_DIR="${SWIG_DIR}"
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
  fmt::fmt
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

target_compile_definitions(rubyengine PRIVATE openstudio_scriptengine_EXPORTS SHARED_OS_LIBS)

# This is statically linked to libruby.a, so no need to tweak the rpath stuff for that, but we still need to add $ORIGIN so that it finds libopenstudiolib
if(APPLE)
  set_target_properties(rubyengine PROPERTIES
    INSTALL_RPATH "@loader_path"
  )
elseif(UNIX)
  set_target_properties(rubyengine PROPERTIES
    INSTALL_RPATH "$ORIGIN"
  )
endif()

if(BUILD_TESTING)
  set(rubyengine_test_depends
    openstudio_scriptengine
    openstudiolib
    fmt::fmt
  )

  set(rubyengine_test_src
    test/RubyEngine_GTest.cpp
  )

  CREATE_TEST_TARGETS(rubyengine "${rubyengine_test_src}" "${rubyengine_test_depends}")
endif()

install(TARGETS rubyengine EXPORT openstudio DESTINATION ${LIB_DESTINATION_DIR} COMPONENT "CLI")
