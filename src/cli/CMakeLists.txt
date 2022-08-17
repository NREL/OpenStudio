if (APPLE)
   find_library(COREFOUNDATION_LIBRARY CoreFoundation )
endif (APPLE)

if(WIN32)
  CONFIGURE_FILE_WITH_CHECKSUM(openstudio.rc.in "${CMAKE_CURRENT_BINARY_DIR}/openstudio.rc")
endif()

if(WIN32)

  # This is really just about RC but instead of spending hours trying to figure how to disable /bigobj just for that part,
  # I removed the definition altogether and didn't hit any issue.
  remove_definitions(/bigobj)

  add_executable(openstudio
    main.cpp
    ###    "${CMAKE_CURRENT_BINARY_DIR}/SWIGRubyRuntime.hxx"
    ###    RubyException.hpp
    ###    RubyInterpreter.hpp
    ###    "${CMAKE_CURRENT_BINARY_DIR}/embedded_scripting_wrap.cxx"
    ###    ${OUTPUT}
    ###    "${CMAKE_CURRENT_BINARY_DIR}/openstudio.rc"
  )
else()
  add_executable(openstudio
    main.cpp
    ####"${CMAKE_CURRENT_BINARY_DIR}/SWIGRubyRuntime.hxx"
    ####RubyException.hpp
    ####RubyInterpreter.hpp
    ####"${CMAKE_CURRENT_BINARY_DIR}/embedded_scripting_wrap.cxx"
    ####${OUTPUT}
  )
endif()


##### -Wno-deprecated-declaration, /wd4996: suppresses deprecated warning
##### -Wno-register, /wd5033: ISO C++1z does not allow ‘register’ storage class specifier
####if(MSVC)
####  set_target_properties(openstudio PROPERTIES COMPILE_FLAGS "/bigobj /wd4996 /wd5033")
####  set_source_files_properties("${CMAKE_CURRENT_BINARY_DIR}/embedded_scripting_wrap.cxx" PROPERTIES COMPILE_FLAGS "-DRUBY_EMBEDDED" )
####else()
####  set_target_properties(openstudio PROPERTIES COMPILE_FLAGS "-Wno-deprecated-declarations -Wno-register")
####  set_source_files_properties("${CMAKE_CURRENT_BINARY_DIR}/embedded_scripting_wrap.cxx" PROPERTIES COMPILE_FLAGS "-DRUBY_EMBEDDED -Wno-unused-variable" )
####endif()
####
####add_definitions(-DRUBY_EXTCONF_H="osruby_config.h")
####
##### We are setting up a set of artificial dependencies here so that
##### cmake knows how to order the libraries properly
#####
##### Better: properly specifying these libs in the conan recipe
##### At the moment this is not possible due to some of them coming from the OS and some
##### being built by this project
##### TODO: Continue to make the conan deps more robust and get this sorted out
####add_library(additional_ruby_libs empty.cpp)
####add_library(ruby_libs empty.cpp)
####target_link_libraries(ruby_libs PUBLIC CONAN_PKG::openstudio_ruby)
####target_link_libraries(ruby_libs PUBLIC additional_ruby_libs)
####
####target_link_libraries(openstudio PRIVATE init_openstudio ruby_libs ${ALL_RUBY_BINDING_TARGETS})
####
####include("${OPENSTUDIO_GEMS_DIR}/export-extensions.cmake")

target_link_libraries(openstudio
 PRIVATE
 openstudio_utilities
 # openstudio_airflow
 # openstudio_model
 # openstudio_energyplus
 # openstudio_epjson
 # openstudio_measure
 # openstudio_osversion
 # openstudio_sdd
 # openstudio_isomodel
 # openstudio_gbxml
 # openstudio_gltf
 # openstudio_radiance
 # ${ruby_extension_libs}
)

add_dependencies(openstudio rubyengine)

####if (UNIX AND NOT APPLE)
####  target_link_libraries(additional_ruby_libs dl crypt anl)
####  target_link_libraries(openstudio PRIVATE "icui18n" "icuuc" "gmp")
####elseif(WIN32)
####  target_link_libraries(openstudio PRIVATE wsock32 ws2_32 Dbghelp Shlwapi Iphlpapi)
####  set_target_properties(openstudio PROPERTIES LINK_FLAGS_DEBUG "/NODEFAULTLIB:MSVCRT")
####else()
####endif()

if( APPLE )
  target_link_libraries(openstudio PRIVATE ${COREFOUNDATION_LIBRARY})
endif()

install(TARGETS openstudio EXPORT openstudio DESTINATION bin COMPONENT "CLI")
###install(FILES openstudio_cli.rb DESTINATION Ruby COMPONENT "RubyAPI")
###install(FILES measure_manager.rb DESTINATION Ruby COMPONENT "RubyAPI")
###install(FILES measure_manager_server.rb DESTINATION Ruby COMPONENT "RubyAPI")

if( BUILD_PAT )
  if( APPLE )
    install(TARGETS openstudio
      DESTINATION ParametricAnalysisTool.app/Contents/Resources/OpenStudio/bin/
      COMPONENT PAT
    )
  endif()
endif()


###################################################################################
#####                        T E S T I N G:   C T E S T S                         #
###################################################################################
####
####file(GLOB RUBY_TEST_SRC
####  # find all CLI test
####  "test/test*.rb"
####
####  # Also Run the ruby tests with the cli
####  "../../ruby/test/*.rb"
####)
####
##### message("CLI: RUBY_TEST_SRC=${RUBY_TEST_SRC}")
####
##### add a test for each unit test
####set(RUBY_TEST_REQUIRES "#include test files")
####foreach(f ${RUBY_TEST_SRC})
####
####  file(READ "${f}" CONTENTS)
####  string(REGEX MATCHALL "def +test_([A-Za-z_0-9 ]+)" FOUND_TESTS ${CONTENTS})
####
####  foreach(HIT ${FOUND_TESTS})
####    string(REGEX REPLACE "def +test_([A-Za-z_0-9]+)" "\\1" TEST_NAME ${HIT})
####    string(REGEX MATCH "/?([A-Za-z_0-9 ]+)\\.rb" FILE_NAME ${f})
####    string(REGEX REPLACE "/?([A-Za-z_0-9 ]+)\\.rb" "\\1" FILE_NAME ${FILE_NAME})
####    if(BUILD_TESTING)
####
####        add_test(NAME "CLITest-${FILE_NAME}-${TEST_NAME}"
####          COMMAND "${CMAKE_COMMAND}" -E chdir "${CMAKE_CURRENT_BINARY_DIR}"
####          "$<TARGET_FILE:openstudio>" "${f}" "--name=test_${TEST_NAME}"
####        )
####
####      set_tests_properties("CLITest-${FILE_NAME}-${TEST_NAME}" PROPERTIES TIMEOUT 660 )
####    endif()
####  endforeach()
####endforeach()
