add_custom_command(
  OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/SWIGPythonRuntime.hxx"
  COMMAND ${CMAKE_COMMAND} -E env SWIG_LIB="${SWIG_LIB}"
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
  openstudio_scriptengine
  openstudio_utilities_minimal
  CONAN_PKG::fmt
  Python::Python
)

# tp_print is deprecated at of 3.8.
target_compile_options(pythonengine PRIVATE
  "$<${is_msvc_genex}:/wd4996>"
  "$<${is_gnu_or_clang_genex}:-Wno-deprecated-declarations>"
)

target_compile_definitions(pythonengine PRIVATE openstudio_scriptengine_EXPORTS SHARED_OS_LIBS)

# it goes into lib/ and we want to find:
# ../EnergyPlus/libpython3.8.dylib
if(APPLE)
  set_target_properties(pythonengine PROPERTIES
    MACOSX_RPATH ON
    INSTALL_RPATH "@loader_path;@loader_path/../EnergyPlus/")
elseif(UNIX)
  set_target_properties(pythonengine PROPERTIES
    INSTALL_RPATH "$ORIGIN:$ORIGIN/../EnergyPlus/")
endif()

install(TARGETS pythonengine EXPORT openstudio DESTINATION lib COMPONENT "CLI")
