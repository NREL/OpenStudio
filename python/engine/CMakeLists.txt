add_custom_command(
  OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/SWIGPythonRuntime.hxx"
  COMMAND ${CMAKE_COMMAND} -E env SWIG_DIR="${SWIG_DIR}"
          "${SWIG_EXECUTABLE}"
          "-python"
          -external-runtime "${CMAKE_CURRENT_BINARY_DIR}/SWIGPythonRuntime.hxx"
)

configure_file(PythonConfig.hxx.in PythonConfig.hxx)

add_library(
  pythonengine
  MODULE
  PythonEngine.hpp
  PythonEngine.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/SWIGPythonRuntime.hxx
)

file(GENERATE
  OUTPUT $<TARGET_FILE_DIR:pythonengine>/openstudiodev.py
  INPUT openstudiodev.py.in
)

target_include_directories(pythonengine
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(
  pythonengine
  PRIVATE
  openstudiolib
  openstudio_scriptengine
  #openstudio_utilities_minimal
  fmt::fmt
  Python::Python
)

# tp_print is deprecated at of 3.8.
target_compile_options(pythonengine PRIVATE
  "$<${is_msvc_genex}:/wd4996>"
  "$<${is_gnu_or_clang_genex}:-Wno-deprecated-declarations>"
)

target_compile_definitions(pythonengine PRIVATE openstudio_scriptengine_EXPORTS SHARED_OS_LIBS)

# Add a command to pip install the requirements into the E+ folder and generate a stamp file
# Make it depend on the requirements.txt, so that it reruns when it changes
add_custom_command(
  OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/pip_install_done.stamp"
  COMMAND ${CMAKE_COMMAND} -E touch "${CMAKE_CURRENT_BINARY_DIR}/pip_install_done.stamp"
  COMMAND ${CMAKE_COMMAND} -E env --unset=PIP_REQUIRE_VIRTUALENV ${Python_EXECUTABLE}
    -m pip install --target=${ENERGYPLUS_DIR}/python_lib --upgrade -r ${PROJECT_SOURCE_DIR}/python/requirements.txt
  DEPENDS ${PROJECT_SOURCE_DIR}/python/requirements.txt
)

# And a target that calls the above command. It is NOT called by default
add_custom_target(pip_install
  DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/pip_install_done.stamp"
)

if(BUILD_CLI)
  # When you build CLI, we want the pip_install to run, so add it as a dependency
  # which will ensure it's run (beforehand, at **build** not configure time)
  add_dependencies(pythonengine pip_install)
endif()

if(BUILD_TESTING)
  set(pythonengine_test_depends
    openstudio_scriptengine
    openstudiolib
    fmt::fmt
  )

  set(pythonengine_test_src
    test/PythonEngine_GTest.cpp
  )

  CREATE_TEST_TARGETS(pythonengine "${pythonengine_test_src}" "${pythonengine_test_depends}")
endif()

install(TARGETS pythonengine EXPORT openstudio DESTINATION ${LIB_DESTINATION_DIR} COMPONENT "CLI")

# it goes into lib/ and we want to find:
# ../EnergyPlus/libpython3.8.dylib
if(APPLE)
  set_target_properties(pythonengine PROPERTIES
    INSTALL_RPATH "@loader_path;@loader_path/../EnergyPlus/"
  )

  install(CODE [[
    execute_process(COMMAND "install_name_tool" -change "$<TARGET_FILE:Python::Python>" "@rpath/$<TARGET_FILE_NAME:Python::Python>" "${CMAKE_INSTALL_PREFIX}/lib/$<TARGET_FILE_NAME:pythonengine>")
  ]]
    COMPONENT "CLI"
  )

elseif(UNIX)
  set_target_properties(pythonengine PROPERTIES
    INSTALL_RPATH "$ORIGIN:$ORIGIN/../EnergyPlus/"
  )
elseif(WIN32)
  # Windows does not have RPATH handling, so just copy the DLL to the bin folder
  install(IMPORTED_RUNTIME_ARTIFACTS Python::Python LIBRARY DESTINATION ${LIB_DESTINATION_DIR} COMPONENT "CLI")
endif()

