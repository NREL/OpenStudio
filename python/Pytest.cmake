#[=======================================================================[.rst:
Pytest
----------

This module defines functions to discover Pytest tests and register them
into CMake's CTests.

.. command:: pytest_discover_tests

  Automatically add tests with CTest by querying the compiled test executable
  for available tests::

    pytest_discover_tests(<NAME>
                          [NO_ADD_OPENSTUDIO_BINDINGS_TO_PATH]
                          [WORKING_DIRECTORY <dir>]
                          [PYTHON_PATH_PREPEND <path>...]
    )

  ``pytest_discover_tests()`` will collect all pytest test cases and register
  commands that execute them through CTest.

  The options are:

  ``NO_ADD_OPENSTUDIO_BINDINGS_TO_PATH``
    If specified, will **not** prepend the location of the built
    openstudio python bindings to the PYTHONPATH so pytest can find them

  ``WORKING_DIRECTORY dir``
    Specifies the directory in which to run test case collection and test case
    execution. If this option is not provided,
    the current source directory (at the point of call) is used.

  ``PYTHON_PATH_PREPEND path...``
    Any paths you want to prepend to the PYTHONPATH to find some modules.

#]=======================================================================]

#------------------------------------------------------------------------------
function(pytest_discover_tests NAME)

  set(prefix "")
  set(valueLessKeywords NO_ADD_OPENSTUDIO_BINDINGS_TO_PATH)
  set(singleValueKeywords WORKING_DIRECTORY)
  set(multiValueKeywords PYTHON_PATH_PREPEND)
  cmake_parse_arguments(
    PARSE_ARGV 1 # Start at one with NAME is the first param
      "${prefix}"
      "${valueLessKeywords}"
      "${singleValueKeywords}"
      "${multiValueKeywords}"
    )

  if (_UNPARSED_ARGUMENTS)
    message(FATAL_ERROR "Extra unknown arguments were passed: ${_UNPARSED_ARGUMENTS}")
  endif()
  if (_KEYWORDS_MISSING_VALUES)
    message(FATAL_ERROR "Keywords missing values: ${_KEYWORDS_MISSING_VALUES}")
  endif()

  if(NOT _WORKING_DIRECTORY)
    set(_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
  endif()

  # Convert to a list of paths, prepend as needed
  cmake_path(CONVERT "$ENV{PYTHONPATH}" TO_CMAKE_PATH_LIST _pythonpath)
  if (_PYTHON_PATH_PREPEND)
    list(PREPEND _pythonpath ${_PYTHON_PATH_PREPEND})
  endif()
  if (NOT ${_NO_ADD_OPENSTUDIO_BINDINGS_TO_PATH})
    message("Adding openstudio bindings to path")
    list(PREPEND _pythonpath "${PROJECT_BINARY_DIR}/Products/python")
  endif()
  # Then convert path to a ";" or ":" separated list. Use NORMALIZE so it's turned into posixpath with a / slash, which is what python wants (I believe)
  cmake_path(CONVERT "${_pythonpath}" TO_NATIVE_PATH_LIST _pythonpath NORMALIZE)

  set(pytest_collection_args --collect-only --quiet)
  set(pytest_base_command ${Python_EXECUTABLE} -m pytest)

  execute_process(
    COMMAND ${CMAKE_COMMAND} -E env "PYTHONPATH=${_pythonpath}"
            ${pytest_base_command} ${pytest_collection_args}
    WORKING_DIRECTORY ${_WORKING_DIRECTORY}
    OUTPUT_VARIABLE pytest_collect_output
    OUTPUT_STRIP_TRAILING_WHITESPACE
    RESULT_VARIABLE pytest_collect_result
  )
  if(NOT ${pytest_collect_result} EQUAL 0)
    message(AUTHOR_WARNING
      "Error running Pytest discovery! Return code: ${pytest_collect_result}\n"
      "command: `PYTHONPATH=${_pythonpath} ${pytest_base_command} ${pytest_collection_args}`\n"
      "Error: ${pytest_collect_output}\n"
      "Working directory: ${_WORKING_DIRECTORY}"
    )
    return()
  endif()

  # Spitlines
  string(REPLACE "\n" ";" pytest_collect_output "${pytest_collect_output}")

  set(pytest_testcase_regex "([^:]+)(::([^:]+))?::([^:]+)")

  set(pytest_found_tests)

  foreach(test_case ${pytest_collect_output})
    string(REGEX MATCHALL ${pytest_testcase_regex} test_case_match "${test_case}")

    if(test_case_match)
      set(test_case_file ${CMAKE_MATCH_1})
      set(test_case_class ${CMAKE_MATCH_3})
      set(test_case_func ${CMAKE_MATCH_4})

      get_filename_component(test_case_file_we ${test_case_file} NAME_WE)

      if(test_case_class)
        set(test_case_name "${NAME}.${test_case_file_we}.${test_case_class}.${test_case_func}")
      else()
        set(test_case_name "${NAME}.${test_case_file_we}.${test_case_func}")
      endif()

      list(APPEND pytest_found_tests ${test_case_name})
      add_test(
        NAME "${test_case_name}"
        COMMAND ${pytest_base_command} ${test_case}
        WORKING_DIRECTORY ${_WORKING_DIRECTORY}
      )
      # Pytest Exit code 5 means all tests were deselected
      set_tests_properties("${test_case_name}" PROPERTIES SKIP_RETURN_CODE 5)
      set_tests_properties("${test_case_name}" PROPERTIES ENVIRONMENT "PYTHONPATH=${_pythonpath}")
    endif()

  endforeach()
  list(LENGTH pytest_found_tests pytest_found_tests_num)
  message(STATUS "For ${NAME}, discovered ${pytest_found_tests_num} pytest test cases in ${_WORKING_DIRECTORY}")

endfunction()

#------------------------------------------------------------------------------
