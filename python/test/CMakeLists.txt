if(BUILD_TESTING)
  if (Pytest_AVAILABLE)
    add_test(NAME PythonBindings.usability
      COMMAND ${Python_EXECUTABLE} -m pytest --verbose ${Pytest_XDIST_OPTS} "${CMAKE_CURRENT_SOURCE_DIR}/test_usability.py"
    )
    set_tests_properties(PythonBindings.usability PROPERTIES ENVIRONMENT PYTHONPATH=${PROJECT_BINARY_DIR}/Products/python)
  endif()
endif()

