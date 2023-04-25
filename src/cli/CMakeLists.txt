if (APPLE)
   find_library(COREFOUNDATION_LIBRARY CoreFoundation )
endif (APPLE)

set(CLI_SRC
  main.cpp
  RubyCLI.hpp
  RubyCLI.cpp
  RunCommand.hpp
  RunCommand.cpp
  UpdateCommand.hpp
  UpdateCommand.cpp
  MeasureUpdateCommand.hpp
  MeasureUpdateCommand.cpp
)

if(WIN32)
  CONFIGURE_FILE_WITH_CHECKSUM(openstudio.rc.in "${CMAKE_CURRENT_BINARY_DIR}/openstudio.rc")

  # This is really just about RC but instead of spending hours trying to figure how to disable /bigobj just for that part,
  # I removed the definition altogether and didn't hit any issue.
  remove_definitions(/bigobj)

  add_executable(openstudio
    ${CLI_SRC}
    "${CMAKE_CURRENT_BINARY_DIR}/openstudio.rc"
  )
else()
  add_executable(openstudio
    ${CLI_SRC}
  )
endif()

target_link_libraries(openstudio
  PRIVATE
  openstudiolib
  openstudio_workflow
  CONAN_PKG::cli11
)

if(BUILD_RUBY_BINDINGS)
  add_dependencies(openstudio rubyengine)
endif()

if(BUILD_PYTHON_BINDINGS)
  add_dependencies(openstudio pythonengine)
endif()

if( APPLE )
  target_link_libraries(openstudio PRIVATE ${COREFOUNDATION_LIBRARY})
endif()



# it goes into bin/ and we want to find:
# ../lib/libopenstudiolib.dylib
if(APPLE)
  set_target_properties(openstudio PROPERTIES
    INSTALL_RPATH "@executable_path;@executable_path/../lib/"
  )
elseif(UNIX)
  set_target_properties(openstudio PROPERTIES
    INSTALL_RPATH "$ORIGIN:$ORIGIN/../lib/")
endif()

install(TARGETS openstudio EXPORT openstudio COMPONENT "CLI")

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

if(BUILD_TESTING)

  add_test(NAME OpenStudioCLI.help
    COMMAND $<TARGET_FILE:openstudio> --help
  )

  add_test(NAME OpenStudioCLI.run_compact_ruby_only_osw
    COMMAND $<TARGET_FILE:openstudio> run -w compact_ruby_only.osw
    WORKING_DIRECTORY "${PROJECT_BINARY_DIR}/resources/Examples/compact_osw/"
  )
  set_tests_properties(OpenStudioCLI.run_compact_ruby_only_osw PROPERTIES RESOURCE_LOCK "compact_osw")

  add_test(NAME OpenStudioCLI.Labs.openstudio_version
    COMMAND $<TARGET_FILE:openstudio> labs openstudio_version
  )

  add_test(NAME OpenStudioCLI.Labs.energyplus_version
    COMMAND $<TARGET_FILE:openstudio> labs energyplus_version
  )

  add_test(NAME OpenStudioCLI.Labs.ruby_version
    COMMAND $<TARGET_FILE:openstudio> labs ruby_version
  )

  add_test(NAME OpenStudioCLI.Labs.python_version
    COMMAND $<TARGET_FILE:openstudio> labs python_version
  )
  set_tests_properties(OpenStudioCLI.Labs.python_version PROPERTIES
    PASS_REGULAR_EXPRESSION "3\.8\.1[0-9]"
  )

  add_test(NAME OpenStudioCLI.Labs.ruby_execute_line
    COMMAND $<TARGET_FILE:openstudio> labs -e "puts(OpenStudio::Model::Model.new())"
  )

  add_test(NAME OpenStudioCLI.ruby_execute_line_no_chop_single_quote
    COMMAND $<TARGET_FILE:openstudio> -e "puts 'hello world'"
  )

  add_test(NAME OpenStudioCLI.Labs.ruby_execute_line_no_chop_single_quote
    COMMAND $<TARGET_FILE:openstudio> labs -e "puts 'hello world'"
  )

  add_test(NAME OpenStudioCLI.Labs.ruby_execute_line.embedded_gem
    COMMAND $<TARGET_FILE:openstudio> labs -e "require 'minitest'"
  )

  add_test(NAME OpenStudioCLI.Labs.ruby_execute_line.embedded_native_gem
    COMMAND $<TARGET_FILE:openstudio> labs -e "require 'oga'"
  )

  add_test(NAME OpenStudioCLI.Labs.python_execute_line
    COMMAND $<TARGET_FILE:openstudio> labs -c "print(openstudio.model.Model())"
  )

  add_test(NAME OpenStudioCLI.Labs.execute_ruby_script
    COMMAND $<TARGET_FILE:openstudio> labs execute_ruby_script ${CMAKE_CURRENT_SOURCE_DIR}/test/execute_ruby_script_with_arguments_test.rb OpenStudioLabs "National Renewable Energy Laboratory"
  )
  set_tests_properties(OpenStudioCLI.Labs.execute_ruby_script PROPERTIES
    PASS_REGULAR_EXPRESSION "Hello from OpenStudioLabs, at National Renewable Energy Laboratory"
  )

  # Ensure we don't get these
  # > eval:1: warning: already initialized constant OpenStudio::Airflow::AirflowElement::DR_DOOR
  # > eval:1: warning: previous definition of DR_DOOR was here
  add_test(NAME OpenStudioCLI.Labs.execute_ruby_script.no_previous_definition
    COMMAND $<TARGET_FILE:openstudio> labs execute_ruby_script ${CMAKE_CURRENT_SOURCE_DIR}/test/execute_ruby_script_with_arguments_test.rb OpenStudioLabs "National Renewable Energy Laboratory"
  )
  set_tests_properties(OpenStudioCLI.Labs.execute_ruby_script.no_previous_definition PROPERTIES
    FAIL_REGULAR_EXPRESSION "previous definition of;already initialized constant"
  )

  add_test(NAME OpenStudioCLI.Labs.execute_ruby_script.forward_flags
    COMMAND $<TARGET_FILE:openstudio> labs execute_ruby_script ${CMAKE_CURRENT_SOURCE_DIR}/test/execute_ruby_script_with_arguments_test.rb -x "National Renewable Energy Laboratory"
  )
  set_tests_properties(OpenStudioCLI.Labs.execute_ruby_script.forward_flags PROPERTIES
    PASS_REGULAR_EXPRESSION "Hello from -x, at National Renewable Energy Laboratory"
  )

  add_test(NAME OpenStudioCLI.Labs.execute_ruby_script.forward_flags.omit_execute
    COMMAND $<TARGET_FILE:openstudio> labs ${CMAKE_CURRENT_SOURCE_DIR}/test/execute_ruby_script_with_arguments_test.rb -x "National Renewable Energy Laboratory"
  )
  set_tests_properties(OpenStudioCLI.Labs.execute_ruby_script.forward_flags PROPERTIES
    PASS_REGULAR_EXPRESSION "Hello from -x, at National Renewable Energy Laboratory"
  )

  add_test(NAME OpenStudioCLI.Labs.execute_python_script
    COMMAND $<TARGET_FILE:openstudio> labs execute_python_script ${CMAKE_CURRENT_SOURCE_DIR}/test/execute_python_script_with_arguments_test.py OpenStudioLabs "National Renewable Energy Laboratory"
  )
  set_tests_properties(OpenStudioCLI.Labs.execute_python_script PROPERTIES
    PASS_REGULAR_EXPRESSION "Hello from OpenStudioLabs, at National Renewable Energy Laboratory"
  )

  add_test(NAME OpenStudioCLI.Labs.execute_python_script.forward_flags
    COMMAND $<TARGET_FILE:openstudio> labs execute_python_script ${CMAKE_CURRENT_SOURCE_DIR}/test/execute_python_script_with_arguments_test.py -x "National Renewable Energy Laboratory"
  )
  set_tests_properties(OpenStudioCLI.Labs.execute_python_script.forward_flags PROPERTIES
    PASS_REGULAR_EXPRESSION "Hello from -x, at National Renewable Energy Laboratory"
  )

  add_test(NAME OpenStudioCLI.Labs.execute_python_script.forward_flags.omit_execute
    COMMAND $<TARGET_FILE:openstudio> labs ${CMAKE_CURRENT_SOURCE_DIR}/test/execute_python_script_with_arguments_test.py -x "National Renewable Energy Laboratory"
  )
  set_tests_properties(OpenStudioCLI.Labs.execute_python_script.forward_flags PROPERTIES
    PASS_REGULAR_EXPRESSION "Hello from -x, at National Renewable Energy Laboratory"
  )

  add_test(NAME OpenStudioCLI.Run_RubyOnly
    COMMAND $<TARGET_FILE:openstudio> run -w compact_ruby_only.osw
    WORKING_DIRECTORY "${PROJECT_BINARY_DIR}/resources/Examples/compact_osw/"
  )
  set_tests_properties(OpenStudioCLI.Run_RubyOnly PROPERTIES RESOURCE_LOCK "compact_osw")

  add_test(NAME OpenStudioCLI.Labs.Run_RubyOnly
    COMMAND $<TARGET_FILE:openstudio> labs run -w compact_ruby_only.osw
    WORKING_DIRECTORY "${PROJECT_BINARY_DIR}/resources/Examples/compact_osw/"
  )
  set_tests_properties(OpenStudioCLI.Labs.Run_RubyOnly PROPERTIES RESOURCE_LOCK "compact_osw")


  add_test(NAME OpenStudioCLI.Run_RubyOnly.absolute_path
    COMMAND $<TARGET_FILE:openstudio> run -w "'${PROJECT_BINARY_DIR}/resources/Examples/compact_osw/compact_ruby_only.osw'"
    WORKING_DIRECTORY "${PROJECT_BINARY_DIR}/resources/Examples/compact_osw/"
  )
  set_tests_properties(OpenStudioCLI.Run_RubyOnly.absolute_path PROPERTIES RESOURCE_LOCK "compact_osw")

  add_test(NAME OpenStudioCLI.Labs.Run_RubyOnly.absolute_path
    COMMAND $<TARGET_FILE:openstudio> labs run -w "'${PROJECT_BINARY_DIR}/resources/Examples/compact_osw/compact_ruby_only.osw'"
    WORKING_DIRECTORY "${PROJECT_BINARY_DIR}/resources/Examples/compact_osw/"
  )
  set_tests_properties(OpenStudioCLI.Labs.Run_RubyOnly.absolute_path PROPERTIES RESOURCE_LOCK "compact_osw")


  add_test(NAME OpenStudioCLI.Labs.Run_PythonOnly
    COMMAND $<TARGET_FILE:openstudio> labs run -w compact_python_only.osw
    WORKING_DIRECTORY "${PROJECT_BINARY_DIR}/resources/Examples/compact_osw/"
  )
  set_tests_properties(OpenStudioCLI.Labs.Run_PythonOnly PROPERTIES RESOURCE_LOCK "compact_osw")

  add_test(NAME OpenStudioCLI.Labs.Run_PythonOnly_TwoMeasure
    COMMAND $<TARGET_FILE:openstudio> labs run -w compact_python_only_twomeasures.osw
    WORKING_DIRECTORY "${PROJECT_BINARY_DIR}/resources/Examples/compact_osw/"
  )
  set_tests_properties(OpenStudioCLI.Labs.Run_PythonOnly_TwoMeasure PROPERTIES RESOURCE_LOCK "compact_osw")

  add_test(NAME OpenStudioCLI.Labs.Run_RubyPython
    COMMAND $<TARGET_FILE:openstudio> labs run -w compact_ruby_and_python.osw
    WORKING_DIRECTORY "${PROJECT_BINARY_DIR}/resources/Examples/compact_osw/"
  )
  set_tests_properties(OpenStudioCLI.Labs.Run_RubyPython PROPERTIES RESOURCE_LOCK "compact_osw")

  if (Pytest_AVAILABLE)
    add_test(NAME OpenStudioCLI.Labs.test_loglevel
      COMMAND ${Python_EXECUTABLE} -m pytest --verbose ${Pytest_XDIST_OPTS} --os-cli-path $<TARGET_FILE:openstudio> "${CMAKE_CURRENT_SOURCE_DIR}/test/test_loglevel.py"
    )
  else()
    # TODO: Remove. Fallback on these for now, as I don't know if CI has pytest installed
    add_test(NAME OpenStudioCLI.test_logger_rb
      COMMAND ${Python_EXECUTABLE} "${CMAKE_CURRENT_SOURCE_DIR}/test/run_test_logger.py" $<TARGET_FILE:openstudio> ${CMAKE_CURRENT_SOURCE_DIR}/test/logger_test.rb
    )

    add_test(NAME OpenStudioCLI.Labs.test_logger_rb
      COMMAND ${Python_EXECUTABLE} "${CMAKE_CURRENT_SOURCE_DIR}/test/run_test_logger.py" $<TARGET_FILE:openstudio> --labs ${CMAKE_CURRENT_SOURCE_DIR}/test/logger_test.rb
    )

    add_test(NAME OpenStudioCLI.Labs.test_logger_py
      COMMAND ${Python_EXECUTABLE} "${CMAKE_CURRENT_SOURCE_DIR}/test/run_test_logger.py" $<TARGET_FILE:openstudio> --labs ${CMAKE_CURRENT_SOURCE_DIR}/test/logger_test.py
    )
  endif()


  # ============ #4856 - Forward a Path properly no matter the slashes employed ============

  # Note: I picked the name `test_folder`on purpose, because `\t` is an escape sequence, so I could test this up too.
  # `.\test_folder\hello.xml`
  #   ~~          ##
  #    |           |
  #   escape       |
  #                <-- not an escape sequence, neither in C, ruby or Python

  # Test via optparse and check file exists
  # Historical CLI versions
    add_test(NAME OpenStudioCLI.execute_ruby_script.forward_flags.optparse.path_forwardslash
    COMMAND $<TARGET_FILE:openstudio> execute_ruby_script execute_ruby_script_optparse_path.rb -x "./test_folder/hello.xml"
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/test/"
  )
  if (WIN32)
    # Posix paths don't understand a backward slash anyways
    add_test(NAME OpenStudioCLI.execute_ruby_script.forward_flags.optparse.path_backwardslash
      COMMAND $<TARGET_FILE:openstudio> execute_ruby_script execute_ruby_script_optparse_path.rb -x ".\\test_folder\\hello.xml"
      WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/test/"
    )
  endif()
  # Labs versions
  add_test(NAME OpenStudioCLI.Labs.execute_ruby_script.forward_flags.optparse.path_forwardslash
    COMMAND $<TARGET_FILE:openstudio> labs execute_ruby_script execute_ruby_script_optparse_path.rb -x "./test_folder/hello.xml"
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/test/"
  )
  if (WIN32)
    add_test(NAME OpenStudioCLI.Labs.execute_ruby_script.forward_flags.optparse.path_backwardslash
      COMMAND $<TARGET_FILE:openstudio> labs execute_ruby_script execute_ruby_script_optparse_path.rb -x ".\\test_folder\\hello.xml"
      WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/test/"
    )
  endif()

  # Historical CLI versions
  add_test(NAME OpenStudioCLI.execute_ruby_script.forward_flags.path_forwardslash
    COMMAND $<TARGET_FILE:openstudio> execute_ruby_script execute_ruby_script_with_arguments_test.rb -x "./test_folder/hello.xml"
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/test/"
  )
  set_tests_properties(OpenStudioCLI.execute_ruby_script.forward_flags.path_forwardslash PROPERTIES
      PASS_REGULAR_EXPRESSION "Hello from -x, at ./test_folder/hello.xml"
  )

  add_test(NAME OpenStudioCLI.execute_ruby_script.forward_flags.path_backwardslash
    COMMAND $<TARGET_FILE:openstudio> execute_ruby_script execute_ruby_script_with_arguments_test.rb -x ".\\test_folder\\hello.xml"
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/test/"
  )
  set_tests_properties(OpenStudioCLI.execute_ruby_script.forward_flags.path_backwardslash PROPERTIES
    # Cmake regexes require double escaping the \ escape, this tests that `Hello from -x, at .\test_folder\hello.xml` is printed to the console
    PASS_REGULAR_EXPRESSION "Hello from -x, at .\\\\test_folder\\\\hello.xml"
  )

  # Labs versions
  add_test(NAME OpenStudioCLI.Labs.execute_ruby_script.forward_flags.path_forwardslash
    COMMAND $<TARGET_FILE:openstudio> labs execute_ruby_script execute_ruby_script_with_arguments_test.rb -x "./test_folder/hello.xml"
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/test/"
  )
  set_tests_properties(OpenStudioCLI.Labs.execute_ruby_script.forward_flags.path_forwardslash PROPERTIES
      PASS_REGULAR_EXPRESSION "Hello from -x, at ./test_folder/hello.xml"
  )

  add_test(NAME OpenStudioCLI.Labs.execute_ruby_script.forward_flags.path_backwardslash
    COMMAND $<TARGET_FILE:openstudio> labs execute_ruby_script execute_ruby_script_with_arguments_test.rb -x ".\\test_folder\\hello.xml"
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/test/"
  )
  set_tests_properties(OpenStudioCLI.Labs.execute_ruby_script.forward_flags.path_backwardslash PROPERTIES
    PASS_REGULAR_EXPRESSION "Hello from -x, at .\\\\test_folder\\\\hello.xml"
  )

  add_test(NAME OpenStudioCLI.Labs.execute_python_script.forward_flags.path_forwardslash
    COMMAND $<TARGET_FILE:openstudio> labs execute_python_script execute_python_script_with_arguments_test.py -x "./test_folder/hello.xml"
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/test/"
  )
  set_tests_properties(OpenStudioCLI.Labs.execute_python_script.forward_flags.path_forwardslash PROPERTIES
      PASS_REGULAR_EXPRESSION "Hello from -x, at ./test_folder/hello.xml"
  )

  add_test(NAME OpenStudioCLI.Labs.execute_python_script.forward_flags.path_backwardslash
    COMMAND $<TARGET_FILE:openstudio> labs execute_python_script execute_python_script_with_arguments_test.py -x ".\\test_folder\\hello.xml"
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/test/"
  )
  set_tests_properties(OpenStudioCLI.Labs.execute_python_script.forward_flags.path_backwardslash PROPERTIES
    PASS_REGULAR_EXPRESSION "Hello from -x, at .\\\\test_folder\\\\hello.xml"
  )

  # ============ EndForward a Path properly no matter the slashes employed ============

  # ============ Native Ruby Gems / Python Modules - C extensions =============
  if (Python_NumPy_FOUND)

    if(NOT Python_ROOT_DIR)
      # Python_STDLIB: we expect it to be
      # Unix: ~/.pyenv/versions/3.8.12/lib/python3.8
      # Windows C:\Python38\Lib
      cmake_path(GET Python_STDLIB PARENT_PATH Python_ROOT_DIR)
      if(UNIX)
        cmake_path(GET Python_ROOT_DIR PARENT_PATH Python_ROOT_DIR)
      endif()
    endif()

    add_test(NAME OpenStudioCLI.Labs.execute_python_script.numpy.explicit_sys_path_insert
      COMMAND $<TARGET_FILE:openstudio> labs execute_python_script execute_python_script_with_numpy.py ${Python_STDLIB}
      WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/test/"
    )

    if(UNIX)
      add_test(NAME OpenStudioCLI.Labs.execute_python_script.numpy.python_path
        COMMAND $<TARGET_FILE:openstudio> labs
                  --python_path "${Python_SITELIB}" # == "${Python_STDLIB}/site-packages"
                  --python_path "${Python_STDLIB}/lib-dynload"
                  execute_python_script execute_python_script_with_numpy.py
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/test/"
      )
    else()
      add_test(NAME OpenStudioCLI.Labs.execute_python_script.numpy.python_path
        COMMAND $<TARGET_FILE:openstudio> labs
                  --python_path "$<SHELL_PATH:${Python_SITELIB}>"
                  execute_python_script execute_python_script_with_numpy.py
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/test/"
      )
    endif()

    add_test(NAME OpenStudioCLI.Labs.execute_python_script.numpy.python_home
      COMMAND $<TARGET_FILE:openstudio> labs
                --python_home "$<SHELL_PATH:${Python_ROOT_DIR}>"
                execute_python_script execute_python_script_with_numpy.py
      WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/test/"
    )
  else()
    message(AUTHOR_WARNING "Cannot run the python numpy test, as numpy isn't installed on your system python")
  endif()

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


          add_test(NAME "CLITest-${FILE_NAME}-${TEST_NAME}"
            COMMAND "${CMAKE_COMMAND}" -E chdir "${CMAKE_CURRENT_BINARY_DIR}"
            "$<TARGET_FILE:openstudio>" "${f}" "--name=test_${TEST_NAME}"
          )

        set_tests_properties("CLITest-${FILE_NAME}-${TEST_NAME}" PROPERTIES TIMEOUT 660 )
    endforeach()
  endforeach()
endif()
