
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

file(
  GENERATE OUTPUT $<TARGET_FILE_DIR:pythonengine>/openstudiodev.py
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
  scriptengine  Python::Python CONAN_PKG::zlib
)

install(TARGETS pythonengine EXPORT openstudio DESTINATION bin COMPONENT "CLI")
