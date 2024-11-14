option(BUILD_PYTHON_PIP_PACKAGE "Prepare a folder to build a pip module" OFF)

# TODO: Prefer static libraries (on Unix, no effect on Windows)
# Can't make it work on Mac right now (untested on Ubuntu), I get issues building the swig bindings
set(Python_USE_STATIC_LIBS OFF)

# find_package(Python) has the problem that on github actions in particular it'll pick up the most recent python (eg 3.9) from the tool cache
# even if you have used the setup-python action and set it to 3.8
if (PYTHON_VERSION)
  find_package(Python ${PYTHON_VERSION} EXACT REQUIRED COMPONENTS Interpreter Development OPTIONAL_COMPONENTS NumPy)
else()
  find_package(Python 3 EXACT REQUIRED COMPONENTS Interpreter Development OPTIONAL_COMPONENTS NumPy)
endif()

execute_process(COMMAND ${Python_EXECUTABLE} -m pytest --version
  RESULT_VARIABLE _Pytest_STATUS
      OUTPUT_VARIABLE Pytest_Version
      ERROR_QUIET
      OUTPUT_STRIP_TRAILING_WHITESPACE
)
if(_Pytest_STATUS AND NOT _Pytest_STATUS EQUAL 0)
  message(AUTHOR_WARNING "Pytest isn't installed on your system python, so some tests won't be run. Run `pip install pytest`")
  set(Pytest_AVAILABLE OFF)
else()
  message("Found Pytest: ${Pytest_Version}")
  set(Pytest_AVAILABLE ON)
  execute_process(COMMAND ${Python_EXECUTABLE} -m pip show pytest-xdist -q
    RESULT_VARIABLE _Pytest_xdist_STATUS
    ERROR_QUIET
  )
  if(_Pytest_xdist_STATUS AND NOT _Pytest_xdist_STATUS EQUAL 0)
    message(AUTHOR_WARNING "Pytest-xdist isn't installed on your system python, so the pytest runs will not be parallelized")
  else()
    set(Pytest_XDIST_OPTS -n auto)
  endif()
endif()

execute_process(COMMAND ${Python_EXECUTABLE} -c "import requests; print(requests.__version__)"
  RESULT_VARIABLE _PyRequests_STATUS
      OUTPUT_VARIABLE PyRequests_Version
      ERROR_QUIET
      OUTPUT_STRIP_TRAILING_WHITESPACE
)
if(_PyRequests_STATUS AND NOT _PyRequests_STATUS EQUAL 0)
  message(AUTHOR_WARNING "requests isn't installed on your system python, so some tests won't be run. Run `pip install requests`")
  set(PyRequests_AVAILABLE OFF)
else()
  message("Found Python requests: ${PyRequests_Version}")
  set(PyRequests_AVAILABLE ON)
endif()

get_filename_component(Python_PROGRAM_NAME ${Python_EXECUTABLE} NAME)

get_filename_component(RESOLVED_PYTHON_LIBRARY "${Python_LIBRARIES}" REALPATH)
message("RESOLVED_PYTHON_LIBRARY=${RESOLVED_PYTHON_LIBRARY}")
if(WIN32)

  # In case you have both release and debug Python libraries on your system, Python_LIBRARIES might be "optimized;C:/.../python38.lib;debug;C:/.../python38_d.lib"
  # so it can't be used directly, we could use a generator expression to find the $<TARGET_FILE:Python::Python> library used and that'd point directly
  # to the DLL. But it won't work nicely for the install(code ...) command below... so we do a hack, get the optimized one always...
  list(LENGTH Python_LIBRARIES _LEN)
  if (_LEN GREATER 1)
    set (_Python_DOING_RELEASE FALSE)
    foreach(_currentArg ${Python_LIBRARIES})
      if ("x${_currentArg}" STREQUAL "xoptimized")
        set(_Python_DOING_RELEASE TRUE)
      elseif(_Python_DOING_RELEASE)
        get_filename_component(RESOLVED_PYTHON_LIBRARY "${_currentArg}" REALPATH)
        break()
      endif()
    endforeach()
  # else()
    # No-op, already done above
  endif()


else()
  get_target_property(PYTHON_LIB_TYPE Python::Python TYPE)
  # Does NOT follow symlinks. Works fine on windows and mac, but on ubuntu it ships a libpython3.8.so which is just a symlink
  #install(IMPORTED_RUNTIME_ARTIFACTS Python::Python LIBRARY DESTINATION lib)
  #install(FILES ${RESOLVED_PYTHON_LIBRARY} DESTINATION lib)

  # if(NOT PYTHON_LIB_TYPE STREQUAL STATIC_LIBRARY)
  #   message(FATAL_ERROR "We expect to find a static libpython on Unix, but ${PYTHON_LIB_TYPE} found")
  # endif()
endif()

include_directories(SYSTEM ${Python_INCLUDE_DIRS})
set(ALL_PYTHON_BINDING_TARGETS "") # global list of python bindings
set(ALL_PYTHON_BINDING_DEPENDS "") # global list of library dependencies of the generated wrapper cxx files
set(ALL_PYTHON_WRAPPER_FILES "") # global list of generated wrapper cxx files
set(ALL_PYTHON_WRAPPER_TARGETS "") # global list targets that build generated wrapper cxx files
set(ALL_PYTHON_GENERATED_SRCS "") # global list of generated .py files

if(BUILD_PYTHON_PIP_PACKAGE)
  set(PipRepositoryValues "testpypi;pypi" CACHE INTERNAL
    "List of possible values for which repository to upload to")
  set(PYTHON_PIP_REPOSITORY "testpypi" CACHE STRING "Which pip repository to use for upload")
  set_property(CACHE PYTHON_PIP_REPOSITORY PROPERTY STRINGS ${PipRepositoryValues})

  if(NOT PYTHON_PIP_REPOSITORY IN_LIST PipRepositoryValues)
    message(FATAL_ERROR "PYTHON_PIP_REPOSITORY must be one of ${PipRepositoryValues}")
  endif()

  set(PYTHON_PACKAGE_FOLDER "${PROJECT_BINARY_DIR}/Products/python_package")
  file(MAKE_DIRECTORY "${PYTHON_PACKAGE_FOLDER}")
  set(ALL_PYTHON_PACKAGE_TARGETS "") # global list of python bindings
  set(ALL_PYTHON_PACKAGE_GENERATED_SRCS "") # global list of generated .py files for pyhton_package
endif()
