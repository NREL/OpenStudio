if(BUILD_TESTING)
  if (Pytest_AVAILABLE)
    include("../Pytest.cmake")

    pytest_discover_tests(PythonBindings)
  endif()
endif()

