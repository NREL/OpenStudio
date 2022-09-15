if (APPLE)
   find_library(COREFOUNDATION_LIBRARY CoreFoundation )
endif (APPLE)

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
file(GLOB_RECURSE OS_FILES FOLLOW_SYMLINKS "${CMAKE_CURRENT_SOURCE_DIR}/openstudio/**/*.rb") # DLM: this is temporary, these files should live somewhere else and be included in the shared Ruby lib

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
  file(RELATIVE_PATH LOCATION "${CMAKE_CURRENT_SOURCE_DIR}" ${_FILE})
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

# Must call CMake itself in order to set the SWIG_LIB env var for add_custom_command
add_custom_command(
  OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/SWIGRubyRuntime.hxx"
  COMMAND ${CMAKE_COMMAND} -E env SWIG_LIB="${SWIG_LIB}"
          "${SWIG_EXECUTABLE}"
          "-ruby"
          -external-runtime "${CMAKE_CURRENT_BINARY_DIR}/SWIGRubyRuntime.hxx"
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

if(WIN32)
  CONFIGURE_FILE_WITH_CHECKSUM(openstudio.rc.in "${CMAKE_CURRENT_BINARY_DIR}/openstudio.rc")
endif()

if(WIN32)

  # This is really just about RC but instead of spending hours trying to figure how to disable /bigobj just for that part,
  # I removed the definition altogether and didn't hit any issue.
  remove_definitions(/bigobj)

  add_executable(openstudio
    main.cpp
    "${CMAKE_CURRENT_BINARY_DIR}/SWIGRubyRuntime.hxx"
    RubyException.hpp
    RubyInterpreter.hpp
    "${CMAKE_CURRENT_BINARY_DIR}/embedded_scripting_wrap.cxx"
    ${OUTPUT}
    "${CMAKE_CURRENT_BINARY_DIR}/openstudio.rc"
  )
else()
  add_executable(openstudio
    main.cpp
    "${CMAKE_CURRENT_BINARY_DIR}/SWIGRubyRuntime.hxx"
    RubyException.hpp
    RubyInterpreter.hpp
    "${CMAKE_CURRENT_BINARY_DIR}/embedded_scripting_wrap.cxx"
    ${OUTPUT}
  )
endif()


# -Wno-deprecated-declaration, /wd4996: suppresses deprecated warning
# -Wno-register, /wd5033: ISO C++1z does not allow ‘register’ storage class specifier
if(MSVC)
  set_target_properties(openstudio PROPERTIES COMPILE_FLAGS "/bigobj /wd4996 /wd5033")
  set_source_files_properties("${CMAKE_CURRENT_BINARY_DIR}/embedded_scripting_wrap.cxx" PROPERTIES COMPILE_FLAGS "-DRUBY_EMBEDDED" )
else()
  set_target_properties(openstudio PROPERTIES COMPILE_FLAGS "-Wno-deprecated-declarations -Wno-register")
  set_source_files_properties("${CMAKE_CURRENT_BINARY_DIR}/embedded_scripting_wrap.cxx" PROPERTIES COMPILE_FLAGS "-DRUBY_EMBEDDED -Wno-unused-variable" )
endif()

# We are setting up a set of artificial dependencies here so that
# cmake knows how to order the libraries properly
#
# Better: properly specifying these libs in the conan recipe
# At the moment this is not possible due to some of them coming from the OS and some
# being built by this project
# TODO: Continue to make the conan deps more robust and get this sorted out
add_library(additional_ruby_libs empty.cpp)
add_library(ruby_libs empty.cpp)
target_link_libraries(ruby_libs PUBLIC CONAN_PKG::openstudio_ruby)
target_link_libraries(ruby_libs PUBLIC additional_ruby_libs)

target_link_libraries(openstudio PRIVATE init_openstudio ruby_libs ${ALL_RUBY_BINDING_TARGETS})

include("${OPENSTUDIO_GEMS_DIR}/export-extensions.cmake")

target_link_libraries(openstudio
 PRIVATE
 openstudio_utilities
 openstudio_airflow
 openstudio_model
 openstudio_energyplus
 openstudio_epjson
 openstudio_measure
 openstudio_osversion
 openstudio_sdd
 openstudio_isomodel
 openstudio_gbxml
 openstudio_gltf
 openstudio_radiance
 ${ruby_extension_libs}
)

if (UNIX AND NOT APPLE)
  target_link_libraries(additional_ruby_libs dl crypt anl)
  target_link_libraries(openstudio PRIVATE "icui18n" "icuuc" "gmp")
elseif(WIN32)
  target_link_libraries(openstudio PRIVATE wsock32 ws2_32 Dbghelp Shlwapi Iphlpapi)
  set_target_properties(openstudio PROPERTIES LINK_FLAGS_DEBUG "/NODEFAULTLIB:MSVCRT")
else()
endif()

if( APPLE )
  target_link_libraries(openstudio PRIVATE ${COREFOUNDATION_LIBRARY})
endif()

install(TARGETS openstudio EXPORT openstudio DESTINATION bin COMPONENT "CLI")
install(FILES openstudio_cli.rb DESTINATION Ruby COMPONENT "RubyAPI")
install(FILES measure_manager.rb DESTINATION Ruby COMPONENT "RubyAPI")
install(FILES measure_manager_server.rb DESTINATION Ruby COMPONENT "RubyAPI")

if( BUILD_PAT )
  if( APPLE )
    install(TARGETS openstudio
      DESTINATION ParametricAnalysisTool.app/Contents/Resources/OpenStudio/bin/
      COMPONENT PAT
    )
  endif()
endif()


###############################################################################
#                        T E S T I N G:   C T E S T S                         #
###############################################################################

file(GLOB RUBY_TEST_SRC
  # find all CLI test
  "test/test*.rb"

  # Also Run the ruby tests with the cli
  "../../ruby/test/*.rb"
)

# message("CLI: RUBY_TEST_SRC=${RUBY_TEST_SRC}")

# add a test for each unit test
set(RUBY_TEST_REQUIRES "#include test files")
foreach(f ${RUBY_TEST_SRC})

  file(READ "${f}" CONTENTS)
  string(REGEX MATCHALL "def +test_([A-Za-z_0-9 ]+)" FOUND_TESTS ${CONTENTS})

  foreach(HIT ${FOUND_TESTS})
    string(REGEX REPLACE "def +test_([A-Za-z_0-9]+)" "\\1" TEST_NAME ${HIT})
    string(REGEX MATCH "/?([A-Za-z_0-9 ]+)\\.rb" FILE_NAME ${f})
    string(REGEX REPLACE "/?([A-Za-z_0-9 ]+)\\.rb" "\\1" FILE_NAME ${FILE_NAME})
    if(BUILD_TESTING)

        add_test(NAME "CLITest-${FILE_NAME}-${TEST_NAME}"
          COMMAND "${CMAKE_COMMAND}" -E chdir "${CMAKE_CURRENT_BINARY_DIR}"
          "$<TARGET_FILE:openstudio>" "${f}" "--name=test_${TEST_NAME}"
        )

      set_tests_properties("CLITest-${FILE_NAME}-${TEST_NAME}" PROPERTIES TIMEOUT 660 )
    endif()
  endforeach()
endforeach()
