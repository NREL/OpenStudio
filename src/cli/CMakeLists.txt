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
  MeasureManager.hpp
  MeasureManager.cpp
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
  CLI11::CLI11
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
  # openstudio_tests is a test target for C++ based tests that extend across multiple
  # compiled units such as the case for openstudio cli.
  # For example, here is a good place to test issues that are concerned with
  # the complexities of sharing data between the Python and Ruby script engines.
  set(openstudio_tests_src
    "test/Logger_GTest.cpp"
  )
  CREATE_TEST_TARGETS(openstudio "${openstudio_tests_src}" openstudiolib openstudio_workflow)
  add_dependencies(openstudio_tests openstudio)

  # Just for convenience: having to VT the seb model which is at 1.11.5 can be very time consuming, especially on debug builds
  add_custom_target(update_seb_osm_in_build_dir
    COMMAND $<TARGET_FILE:openstudio> execute_ruby_script "${PROJECT_BINARY_DIR}/resources/Examples/compact_osw/update_seb_model.rb"
    DEPENDS openstudio
  )

  add_test(NAME OpenStudioCLI.help
    COMMAND $<TARGET_FILE:openstudio> --help
  )

  add_test(NAME OpenStudioCLI.Classic.run_compact_ruby_only_osw
    COMMAND $<TARGET_FILE:openstudio> classic run -w compact_ruby_only.osw
    WORKING_DIRECTORY "${PROJECT_BINARY_DIR}/resources/Examples/compact_osw/"
  )
  set_tests_properties(OpenStudioCLI.Classic.run_compact_ruby_only_osw PROPERTIES RESOURCE_LOCK "compact_osw")

  add_test(NAME OpenStudioCLI.openstudio_version
    COMMAND $<TARGET_FILE:openstudio> openstudio_version
  )

  add_test(NAME OpenStudioCLI.energyplus_version
    COMMAND $<TARGET_FILE:openstudio> energyplus_version
  )

  add_test(NAME OpenStudioCLI.ruby_version
    COMMAND $<TARGET_FILE:openstudio> ruby_version
  )

  add_test(NAME OpenStudioCLI.python_version
    COMMAND $<TARGET_FILE:openstudio> python_version
  )
  set_tests_properties(OpenStudioCLI.python_version PROPERTIES
    PASS_REGULAR_EXPRESSION "3\.12\.[0-9]+"
  )

  add_test(NAME OpenStudioCLI.ruby_execute_line
    COMMAND $<TARGET_FILE:openstudio> -e "puts(OpenStudio::Model::Model.new())"
  )

  add_test(NAME OpenStudioCLI.Classic.ruby_execute_line_no_chop_single_quote
    COMMAND $<TARGET_FILE:openstudio> classic -e "puts 'hello world'"
  )

  add_test(NAME OpenStudioCLI.ruby_execute_line_no_chop_single_quote
    COMMAND $<TARGET_FILE:openstudio> -e "puts 'hello world'"
  )

  add_test(NAME OpenStudioCLI.ruby_execute_line.embedded_gem
    COMMAND $<TARGET_FILE:openstudio> -e "require 'minitest'"
  )

  add_test(NAME OpenStudioCLI.ruby_execute_line.embedded_native_gem
    COMMAND $<TARGET_FILE:openstudio> -e "require 'oga'"
  )

  add_test(NAME OpenStudioCLI.python_execute_line
    COMMAND $<TARGET_FILE:openstudio> -c "print(openstudio.model.Model())"
  )

  add_test(NAME OpenStudioCLI.execute_ruby_script
    COMMAND $<TARGET_FILE:openstudio> execute_ruby_script ${CMAKE_CURRENT_SOURCE_DIR}/test/execute_ruby_script_with_arguments_test.rb OpenStudioLabs "National Renewable Energy Laboratory"
  )
  set_tests_properties(OpenStudioCLI.execute_ruby_script PROPERTIES
    PASS_REGULAR_EXPRESSION "Hello from OpenStudioLabs, at National Renewable Energy Laboratory"
  )

  # Ensure we don't get these
  # > eval:1: warning: already initialized constant OpenStudio::Airflow::AirflowElement::DR_DOOR
  # > eval:1: warning: previous definition of DR_DOOR was here
  add_test(NAME OpenStudioCLI.execute_ruby_script.no_previous_definition
    COMMAND $<TARGET_FILE:openstudio> execute_ruby_script ${CMAKE_CURRENT_SOURCE_DIR}/test/execute_ruby_script_with_arguments_test.rb OpenStudioLabs "National Renewable Energy Laboratory"
  )
  set_tests_properties(OpenStudioCLI.execute_ruby_script.no_previous_definition PROPERTIES
    FAIL_REGULAR_EXPRESSION "previous definition of;already initialized constant"
  )

  add_test(NAME OpenStudioCLI.execute_ruby_script.forward_flags
    COMMAND $<TARGET_FILE:openstudio> execute_ruby_script ${CMAKE_CURRENT_SOURCE_DIR}/test/execute_ruby_script_with_arguments_test.rb -x "National Renewable Energy Laboratory"
  )
  set_tests_properties(OpenStudioCLI.execute_ruby_script.forward_flags PROPERTIES
    PASS_REGULAR_EXPRESSION "Hello from -x, at National Renewable Energy Laboratory"
  )

  add_test(NAME OpenStudioCLI.execute_ruby_script.forward_flags.omit_execute
    COMMAND $<TARGET_FILE:openstudio> ${CMAKE_CURRENT_SOURCE_DIR}/test/execute_ruby_script_with_arguments_test.rb -x "National Renewable Energy Laboratory"
  )
  set_tests_properties(OpenStudioCLI.execute_ruby_script.forward_flags PROPERTIES
    PASS_REGULAR_EXPRESSION "Hello from -x, at National Renewable Energy Laboratory"
  )

  add_test(NAME OpenStudioCLI.execute_python_script
    COMMAND $<TARGET_FILE:openstudio> execute_python_script ${CMAKE_CURRENT_SOURCE_DIR}/test/execute_python_script_with_arguments_test.py OpenStudioLabs "National Renewable Energy Laboratory"
  )
  set_tests_properties(OpenStudioCLI.execute_python_script PROPERTIES
    PASS_REGULAR_EXPRESSION "Hello from OpenStudioLabs, at National Renewable Energy Laboratory"
  )

  add_test(NAME OpenStudioCLI.execute_python_script.forward_flags
    COMMAND $<TARGET_FILE:openstudio> execute_python_script ${CMAKE_CURRENT_SOURCE_DIR}/test/execute_python_script_with_arguments_test.py -x "National Renewable Energy Laboratory"
  )
  set_tests_properties(OpenStudioCLI.execute_python_script.forward_flags PROPERTIES
    PASS_REGULAR_EXPRESSION "Hello from -x, at National Renewable Energy Laboratory"
  )

  add_test(NAME OpenStudioCLI.execute_python_script.forward_flags.omit_execute
    COMMAND $<TARGET_FILE:openstudio> ${CMAKE_CURRENT_SOURCE_DIR}/test/execute_python_script_with_arguments_test.py -x "National Renewable Energy Laboratory"
  )
  set_tests_properties(OpenStudioCLI.execute_python_script.forward_flags PROPERTIES
    PASS_REGULAR_EXPRESSION "Hello from -x, at National Renewable Energy Laboratory"
  )

  # Test for 5315
  add_test(NAME OpenStudioCLI.EmbeddedScripting.uuid
    COMMAND $<TARGET_FILE:openstudio> ${CMAKE_CURRENT_SOURCE_DIR}/test/ensure_uuid_can_be_loaded.rb
  )

  add_test(NAME OpenStudioCLI.Classic.Run_RubyOnly
    COMMAND $<TARGET_FILE:openstudio> classic run -w compact_ruby_only.osw
    WORKING_DIRECTORY "${PROJECT_BINARY_DIR}/resources/Examples/compact_osw/"
  )
  set_tests_properties(OpenStudioCLI.Classic.Run_RubyOnly PROPERTIES RESOURCE_LOCK "compact_osw")

  add_test(NAME OpenStudioCLI.Run_RubyOnly
    COMMAND $<TARGET_FILE:openstudio> run -w compact_ruby_only.osw
    WORKING_DIRECTORY "${PROJECT_BINARY_DIR}/resources/Examples/compact_osw/"
  )
  set_tests_properties(OpenStudioCLI.Run_RubyOnly PROPERTIES RESOURCE_LOCK "compact_osw")


  add_test(NAME OpenStudioCLI.Classic.Run_RubyOnly.absolute_path
    COMMAND $<TARGET_FILE:openstudio> classic run -w "'${PROJECT_BINARY_DIR}/resources/Examples/compact_osw/compact_ruby_only.osw'"
    WORKING_DIRECTORY "${PROJECT_BINARY_DIR}/resources/Examples/compact_osw/"
  )
  set_tests_properties(OpenStudioCLI.Classic.Run_RubyOnly.absolute_path PROPERTIES RESOURCE_LOCK "compact_osw")

  add_test(NAME OpenStudioCLI.Run_RubyOnly.absolute_path
    COMMAND $<TARGET_FILE:openstudio> run -w "'${PROJECT_BINARY_DIR}/resources/Examples/compact_osw/compact_ruby_only.osw'"
    WORKING_DIRECTORY "${PROJECT_BINARY_DIR}/resources/Examples/compact_osw/"
  )
  set_tests_properties(OpenStudioCLI.Run_RubyOnly.absolute_path PROPERTIES RESOURCE_LOCK "compact_osw")


  add_test(NAME OpenStudioCLI.Run_PythonOnly
    COMMAND $<TARGET_FILE:openstudio> run -w compact_python_only.osw
    WORKING_DIRECTORY "${PROJECT_BINARY_DIR}/resources/Examples/compact_osw/"
  )
  set_tests_properties(OpenStudioCLI.Run_PythonOnly PROPERTIES RESOURCE_LOCK "compact_osw")

  add_test(NAME OpenStudioCLI.Run_PythonOnly_TwoMeasure
    COMMAND $<TARGET_FILE:openstudio> run -w compact_python_only_twomeasures.osw
    WORKING_DIRECTORY "${PROJECT_BINARY_DIR}/resources/Examples/compact_osw/"
  )
  set_tests_properties(OpenStudioCLI.Run_PythonOnly_TwoMeasure PROPERTIES RESOURCE_LOCK "compact_osw")

  add_test(NAME OpenStudioCLI.Run_RubyPython
    COMMAND $<TARGET_FILE:openstudio> run -w compact_ruby_then_python.osw
    WORKING_DIRECTORY "${PROJECT_BINARY_DIR}/resources/Examples/compact_osw/"
  )
  set_tests_properties(OpenStudioCLI.Run_RubyPython PROPERTIES RESOURCE_LOCK "compact_osw")

  add_test(NAME OpenStudioCLI.Run_PythonRuby
    COMMAND $<TARGET_FILE:openstudio> run -w compact_python_then_ruby.osw
    WORKING_DIRECTORY "${PROJECT_BINARY_DIR}/resources/Examples/compact_osw/"
  )
  set_tests_properties(OpenStudioCLI.Run_PythonRuby PROPERTIES RESOURCE_LOCK "compact_osw")

  add_test(NAME OpenStudioCLI.Run_AlfalfaWorkflow
    COMMAND ${CMAKE_COMMAND} "-DCMD1=$<TARGET_FILE:openstudio> run -m -w compact_alfalfa.osw"
    "-DCMD2=${CMAKE_COMMAND} -E cat run/alfalfa.json"
    -P ${CMAKE_SOURCE_DIR}/CMake/RunCommands.cmake
    WORKING_DIRECTORY "${PROJECT_BINARY_DIR}/resources/Examples/compact_osw"
  )
  set_tests_properties(OpenStudioCLI.Run_AlfalfaWorkflow PROPERTIES RESOURCE_LOCK "compact_osw")

  add_test(NAME OpenStudioCLI.Run_RubyPythonPlugin
    COMMAND $<TARGET_FILE:openstudio> run --show-stdout -w python_plugin_jinja_erb.osw
    WORKING_DIRECTORY "${PROJECT_BINARY_DIR}/resources/workflow/python_plugin/"
  )
  set_tests_properties(OpenStudioCLI.Run_RubyPythonPlugin PROPERTIES RESOURCE_LOCK "python_plugin")
  set_tests_properties(OpenStudioCLI.Run_RubyPythonPlugin PROPERTIES
    PASS_REGULAR_EXPRESSION "HI FROM ERB PYTHON PLUGIN[\r\n\t ]*HI FROM JINJA PYTHON PLUGIN"
  )

  # ======================== Workflows should fail ========================
  add_test(NAME OpenStudioCLI.Run_Validate.MissingAMeasure
    COMMAND $<TARGET_FILE:openstudio> run --show-stdout -w missing_a_measure.osw
    WORKING_DIRECTORY "${PROJECT_BINARY_DIR}/resources/workflow/invalid_measures/"
  )
  set_tests_properties(OpenStudioCLI.Run_Validate.MissingAMeasure PROPERTIES
    WILL_FAIL TRUE
    RESOURCE_LOCK "invalid_measures"
  )

  add_test(NAME OpenStudioCLI.Run_Validate.UnloadableMeasure
    COMMAND $<TARGET_FILE:openstudio> run --show-stdout -w unloadable_measure.osw
    WORKING_DIRECTORY "${PROJECT_BINARY_DIR}/resources/workflow/invalid_measures/"
  )
  set_tests_properties(OpenStudioCLI.Run_Validate.UnloadableMeasure PROPERTIES
    WILL_FAIL TRUE
    RESOURCE_LOCK "invalid_measures"
  )

  add_test(NAME OpenStudioCLI.Run_Validate.WrongMeasureTypeOrder
    COMMAND $<TARGET_FILE:openstudio> run --show-stdout -w wrong_measure_type_order.osw
    WORKING_DIRECTORY "${PROJECT_BINARY_DIR}/resources/workflow/invalid_measures/"
  )
  set_tests_properties(OpenStudioCLI.Run_Validate.WrongMeasureTypeOrder PROPERTIES
    WILL_FAIL TRUE
    RESOURCE_LOCK "invalid_measures"
  )

  # Classic
  add_test(NAME OpenStudioCLI.Classic.Run_Validate.MissingAMeasure
    COMMAND $<TARGET_FILE:openstudio> classic run --show-stdout -w missing_a_measure.osw
    WORKING_DIRECTORY "${PROJECT_BINARY_DIR}/resources/workflow/invalid_measures/"
  )
  set_tests_properties(OpenStudioCLI.Classic.Run_Validate.MissingAMeasure PROPERTIES
    WILL_FAIL TRUE
    RESOURCE_LOCK "invalid_measures"
  )

  add_test(NAME OpenStudioCLI.Classic.Run_Validate.UnloadableMeasure
    COMMAND $<TARGET_FILE:openstudio> classic run --show-stdout -w unloadable_measure.osw
    WORKING_DIRECTORY "${PROJECT_BINARY_DIR}/resources/workflow/invalid_measures/"
  )
  set_tests_properties(OpenStudioCLI.Classic.Run_Validate.UnloadableMeasure PROPERTIES
    WILL_FAIL TRUE
    RESOURCE_LOCK "invalid_measures"
  )

  add_test(NAME OpenStudioCLI.Classic.Run_Validate.WrongMeasureTypeOrder
    COMMAND $<TARGET_FILE:openstudio> classic run --show-stdout -w wrong_measure_type_order.osw
    WORKING_DIRECTORY "${PROJECT_BINARY_DIR}/resources/workflow/invalid_measures/"
  )
  set_tests_properties(OpenStudioCLI.Classic.Run_Validate.WrongMeasureTypeOrder PROPERTIES
    WILL_FAIL TRUE
    RESOURCE_LOCK "invalid_measures"
  )
  # ====================== End Workflows should fail ======================

  if (Pytest_AVAILABLE)
    add_test(NAME OpenStudioCLI.test_loglevel
      COMMAND ${Python_EXECUTABLE} -m pytest --verbose ${Pytest_XDIST_OPTS} --os-cli-path $<TARGET_FILE:openstudio> "${CMAKE_CURRENT_SOURCE_DIR}/test/test_loglevel.py"
    )

    # No Xdist on purpose here
    add_test(NAME OpenStudioCLI.test_with_analysis
      COMMAND ${Python_EXECUTABLE} -m pytest --verbose --os-cli-path $<TARGET_FILE:openstudio> "${CMAKE_CURRENT_SOURCE_DIR}/test/test_with_analysis.py"
      WORKING_DIRECTORY "${PROJECT_BINARY_DIR}/resources/workflow/with_analysis/local/"
    )

    add_test(NAME OpenStudioCLI.test_runner_errors
      COMMAND ${Python_EXECUTABLE} -m pytest --verbose --os-cli-path $<TARGET_FILE:openstudio> "${CMAKE_CURRENT_SOURCE_DIR}/test/test_runner_registers_error.py"
      WORKING_DIRECTORY "${PROJECT_BINARY_DIR}/resources/workflow/runner_errors/"
    )

    add_test(NAME OpenStudioCLI.test_outdated_measure
      COMMAND ${Python_EXECUTABLE} -m pytest --verbose --os-cli-path $<TARGET_FILE:openstudio> "${CMAKE_CURRENT_SOURCE_DIR}/test/test_outdated_measure.py"
      WORKING_DIRECTORY "${PROJECT_BINARY_DIR}/resources/workflow/outdated_measures/"
    )

    add_test(NAME OpenStudioCLI.test_output_files
      COMMAND ${Python_EXECUTABLE} -m pytest --verbose --os-cli-path $<TARGET_FILE:openstudio> "${CMAKE_CURRENT_SOURCE_DIR}/test/test_output_files.py"
      WORKING_DIRECTORY "${PROJECT_BINARY_DIR}/resources/workflow/output_test/"
    )

    file(MAKE_DIRECTORY "${PROJECT_BINARY_DIR}/Testing/")
    add_test(NAME OpenStudioCLI.test_bcl_measure_templates
      COMMAND ${Python_EXECUTABLE} -m pytest --verbose --os-cli-path $<TARGET_FILE:openstudio> "${CMAKE_CURRENT_SOURCE_DIR}/test/test_bcl_measure_templates.py"
      WORKING_DIRECTORY "${PROJECT_BINARY_DIR}/Testing/"
    )


  else()
    # TODO: Remove. Fallback on these for now, as I don't know if CI has pytest installed
    add_test(NAME OpenStudioCLI.Classic.test_logger_rb
      COMMAND ${Python_EXECUTABLE} "${CMAKE_CURRENT_SOURCE_DIR}/test/run_test_logger.py" $<TARGET_FILE:openstudio> --classic ${CMAKE_CURRENT_SOURCE_DIR}/test/logger_test.rb
    )

    add_test(NAME OpenStudioCLI.test_logger_rb
      COMMAND ${Python_EXECUTABLE} "${CMAKE_CURRENT_SOURCE_DIR}/test/run_test_logger.py" $<TARGET_FILE:openstudio> ${CMAKE_CURRENT_SOURCE_DIR}/test/logger_test.rb
    )

    add_test(NAME OpenStudioCLI.test_logger_py
      COMMAND ${Python_EXECUTABLE} "${CMAKE_CURRENT_SOURCE_DIR}/test/run_test_logger.py" $<TARGET_FILE:openstudio> ${CMAKE_CURRENT_SOURCE_DIR}/test/logger_test.py
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
  add_test(NAME OpenStudioCLI.Classic.execute_ruby_script.forward_flags.optparse.path_forwardslash
    COMMAND $<TARGET_FILE:openstudio> classic execute_ruby_script execute_ruby_script_optparse_path.rb -x "./test_folder/hello.xml"
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/test/"
  )
  add_test(NAME OpenStudioCLI.Classic.execute_ruby_script.forward_flags.forward_help
    COMMAND $<TARGET_FILE:openstudio> classic execute_ruby_script ${CMAKE_CURRENT_SOURCE_DIR}/test/execute_ruby_script_optparse_path.rb --help
  )
  set_tests_properties(OpenStudioCLI.Classic.execute_ruby_script.forward_flags.forward_help PROPERTIES
    PASS_REGULAR_EXPRESSION "The Ruby help description."
  )

  if (WIN32)
    # Posix paths don't understand a backward slash anyways
    add_test(NAME OpenStudioCLI.Classic.execute_ruby_script.forward_flags.optparse.path_backwardslash
      COMMAND $<TARGET_FILE:openstudio> classic execute_ruby_script execute_ruby_script_optparse_path.rb -x ".\\test_folder\\hello.xml"
      WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/test/"
    )
  endif()
  # Labs versions
  add_test(NAME OpenStudioCLI.execute_ruby_script.forward_flags.optparse.path_forwardslash
    COMMAND $<TARGET_FILE:openstudio> execute_ruby_script execute_ruby_script_optparse_path.rb -x "./test_folder/hello.xml"
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/test/"
  )

  add_test(NAME OpenStudioCLI.execute_ruby_script.forward_flags.forward_help
    COMMAND $<TARGET_FILE:openstudio> execute_ruby_script ${CMAKE_CURRENT_SOURCE_DIR}/test/execute_ruby_script_optparse_path.rb --help
  )
  set_tests_properties(OpenStudioCLI.execute_ruby_script.forward_flags.forward_help PROPERTIES
    PASS_REGULAR_EXPRESSION "The Ruby help description."
  )

  add_test(NAME OpenStudioCLI.execute_python_script.forward_flags.forward_help
    COMMAND $<TARGET_FILE:openstudio> execute_python_script ${CMAKE_CURRENT_SOURCE_DIR}/test/execute_python_script_argparse_path.py --help
  )
  set_tests_properties(OpenStudioCLI.execute_python_script.forward_flags.forward_help PROPERTIES
    PASS_REGULAR_EXPRESSION "The Python help description."
  )

  if (WIN32)
    add_test(NAME OpenStudioCLI.execute_ruby_script.forward_flags.optparse.path_backwardslash
      COMMAND $<TARGET_FILE:openstudio> execute_ruby_script execute_ruby_script_optparse_path.rb -x ".\\test_folder\\hello.xml"
      WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/test/"
    )
  endif()

  # Historical CLI versions
  add_test(NAME OpenStudioCLI.Classic.execute_ruby_script.forward_flags.path_forwardslash
    COMMAND $<TARGET_FILE:openstudio> classic execute_ruby_script execute_ruby_script_with_arguments_test.rb -x "./test_folder/hello.xml"
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/test/"
  )
  set_tests_properties(OpenStudioCLI.Classic.execute_ruby_script.forward_flags.path_forwardslash PROPERTIES
      PASS_REGULAR_EXPRESSION "Hello from -x, at ./test_folder/hello.xml"
  )

  add_test(NAME OpenStudioCLI.Classic.execute_ruby_script.forward_flags.path_backwardslash
    COMMAND $<TARGET_FILE:openstudio> classic execute_ruby_script execute_ruby_script_with_arguments_test.rb -x ".\\test_folder\\hello.xml"
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/test/"
  )
  set_tests_properties(OpenStudioCLI.Classic.execute_ruby_script.forward_flags.path_backwardslash PROPERTIES
    # Cmake regexes require double escaping the \ escape, this tests that `Hello from -x, at .\test_folder\hello.xml` is printed to the console
    PASS_REGULAR_EXPRESSION "Hello from -x, at .\\\\test_folder\\\\hello.xml"
  )

  # Labs versions
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
    PASS_REGULAR_EXPRESSION "Hello from -x, at .\\\\test_folder\\\\hello.xml"
  )

  add_test(NAME OpenStudioCLI.execute_python_script.forward_flags.path_forwardslash
    COMMAND $<TARGET_FILE:openstudio> execute_python_script execute_python_script_with_arguments_test.py -x "./test_folder/hello.xml"
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/test/"
  )
  set_tests_properties(OpenStudioCLI.execute_python_script.forward_flags.path_forwardslash PROPERTIES
      PASS_REGULAR_EXPRESSION "Hello from -x, at ./test_folder/hello.xml"
  )

  add_test(NAME OpenStudioCLI.execute_python_script.forward_flags.path_backwardslash
    COMMAND $<TARGET_FILE:openstudio> execute_python_script execute_python_script_with_arguments_test.py -x ".\\test_folder\\hello.xml"
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/test/"
  )
  set_tests_properties(OpenStudioCLI.execute_python_script.forward_flags.path_backwardslash PROPERTIES
    PASS_REGULAR_EXPRESSION "Hello from -x, at .\\\\test_folder\\\\hello.xml"
  )

  # ============ EndForward a Path properly no matter the slashes employed ============

  # ============ Native Ruby Gems / Python Modules - C extensions =============
  if (Python_NumPy_FOUND)

    if(NOT EXISTS "${Python_ROOT_DIR}")
      # Python_STDLIB: we expect it to be
      # Unix: ~/.pyenv/versions/3.8.12/lib/python3.8 or
      #       or on CI: /usr/lib/python3.8/ ... with numpy if install via pip3 and not apt install python3-numpy in `/usr/local/lib/python3.8/dist-packages/`
      # Windows C:\Python38\Lib
      cmake_path(GET Python_STDLIB PARENT_PATH Python_ROOT_DIR)
      if(UNIX)
        cmake_path(GET Python_ROOT_DIR PARENT_PATH Python_ROOT_DIR)
      endif()
    endif()

    if(UNIX)
      if(EXISTS "${Python_SITELIB}")
        set(PYTHON_PATH "${Python_SITELIB}" "${Python_STDLIB}/lib-dynload")
      else()
        set(PYTHON_PATH "${Python_STDLIB}/lib-dynload")
      endif()

      if(NOT APPLE)
        set(EXTRA_LOCAL_DIST "/usr/local/lib/python3.8/dist-packages")
        if (EXISTS "${EXTRA_LOCAL_DIST}")
          list(APPEND PYTHON_PATH "${EXTRA_LOCAL_DIST}")
        endif()
      endif()
    else()
      set(PYTHON_PATH "$<SHELL_PATH:${Python_SITELIB}>")
    endif()

    message(DEBUG "PYTHON_PATH=${PYTHON_PATH}")

    add_test(NAME OpenStudioCLI.execute_python_script.numpy.explicit_sys_path_insert
      COMMAND $<TARGET_FILE:openstudio> execute_python_script execute_python_script_with_numpy.py ${Python_STDLIB}
      WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/test/"
    )

    add_test(NAME OpenStudioCLI.execute_python_script.numpy.python_path
      COMMAND $<TARGET_FILE:openstudio>
                "$<$<BOOL:${PYTHON_PATH}>:--python_path;$<JOIN:${PYTHON_PATH},;--python_path;>>"
                execute_python_script execute_python_script_with_numpy.py
      COMMAND_EXPAND_LISTS
      WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/test/"
    )

    add_test(NAME OpenStudioCLI.execute_python_script.numpy.python_home
      COMMAND $<TARGET_FILE:openstudio>
                --python_home "$<SHELL_PATH:${Python_ROOT_DIR}>"
                execute_python_script execute_python_script_with_numpy.py
      WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/test/"
    )
  else()
    message(AUTHOR_WARNING "Cannot run the python numpy test, as numpy isn't installed on your system python")
  endif()

  if (Python_EXECUTABLE)
    add_test(NAME OpenStudioCLI.minitest_fail
      COMMAND ${Python_EXECUTABLE} "${CMAKE_CURRENT_SOURCE_DIR}/test/run_minitest_fail.py" $<TARGET_FILE:openstudio>
    )
    if (SYSTEM_RUBY_EXECUTABLE)
      add_test(NAME RubyTest.minitest_fail
        COMMAND ${Python_EXECUTABLE} "${CMAKE_CURRENT_SOURCE_DIR}/test/run_minitest_fail.py" "${SYSTEM_RUBY_EXECUTABLE}"
      )
    endif()
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

      set(CTEST_NAME "CLITest-${FILE_NAME}-${TEST_NAME}")

      add_test(NAME "${CTEST_NAME}"
        COMMAND "${CMAKE_COMMAND}" -E chdir "${CMAKE_CURRENT_BINARY_DIR}"
        "$<TARGET_FILE:openstudio>" "${f}" "--name=test_${TEST_NAME}"
        )

      set_tests_properties("${CTEST_NAME}" PROPERTIES TIMEOUT 660 )
    endforeach()

  endforeach()

endif()
