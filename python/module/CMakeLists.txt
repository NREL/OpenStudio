foreach(file IN LISTS ALL_PYTHON_GENERATED_SRCS)
  list(APPEND embedded_files ${file})
  cmake_path(GET file FILENAME file_name)
  list(APPEND embedded_paths python/openstudio/${file_name})
endforeach()

embed_files("${embedded_files}" "${embedded_paths}" embedded_src)

# This is a single static library that contains the python bindings
add_library(
  openstudio_pylib
  STATIC
  ${embedded_src}
  InitPythonBindings.hpp
  InitPythonBindings.cpp
)

target_link_libraries(
  openstudio_pylib
  PUBLIC
  openstudiolib
  ${ALL_PYTHON_BINDING_TARGETS}
  Python::Python
)

target_include_directories(
  openstudio_pylib
  PRIVATE
  ${CMAKE_CURRENT_BINARY_DIR}
)

set_target_properties(openstudio_pylib PROPERTIES COMPILE_FLAGS "-Wno-deprecated-declarations -Wno-sign-compare -Wno-register -Wno-conversion-null -Wno-misleading-indentation -Wno-subobject-linkage -fno-gnu-unique")

# This is the stand alone python module
add_library(
  openstudio_pymodule
  MODULE
  PythonAPI.hpp
  openstudio_py.cpp
)

target_link_libraries(
  openstudio_pymodule
  PRIVATE
  openstudio_pylib
)

set_target_properties(openstudio_pymodule PROPERTIES PREFIX "")
set_target_properties(openstudio_pymodule PROPERTIES OUTPUT_NAME openstudio)

set_target_properties(openstudio_pymodule PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/python")
set_target_properties(openstudio_pymodule PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/python")
set_target_properties(openstudio_pymodule PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/python")

install(TARGETS openstudio_pymodule DESTINATION Python COMPONENT "Python")


#if (BUILD_PYTHON_PIP_PACKAGE)
#
#  # configure_file(${CMAKE_CURRENT_SOURCE_DIR}/openstudio_dynamic.py ${PROJECT_BINARY_DIR}/Products/python/openstudio_dynamic.py COPYONLY)
#  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/openstudio.py ${PYTHON_PACKAGE_FOLDER}/openstudio/__init__.py COPYONLY)
#
#  if(PYTHON_PIP_REPOSITORY STREQUAL "testpypi")
#    execute_process( COMMAND ${Python_EXECUTABLE} "${CMAKE_CURRENT_SOURCE_DIR}/find_pypi_tag.py" OUTPUT_VARIABLE PYPI_VERSION)
#  else()
#    execute_process( COMMAND ${Python_EXECUTABLE} "${CMAKE_CURRENT_SOURCE_DIR}/find_pypi_tag.py" "--pypi" OUTPUT_VARIABLE PYPI_VERSION)
#  endif()
#  message("On Repository '${PYTHON_PIP_REPOSITORY}', computed next available version as PYPI_VERSION=${PYPI_VERSION}")
#  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/setup.py ${PYTHON_PACKAGE_FOLDER}/setup.py @ONLY)
#
#  foreach(p ${ALL_PYTHON_PACKAGE_TARGETS})
#    target_link_libraries(${p}
#      PRIVATE
#      openstudiolib
#    )
#  endforeach()
#
#  add_custom_target(python_package
#    COMMAND ${CMAKE_COMMAND} -E rm -Rf "build/" "dist/" "openstudio.egg-info/"
#    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:openstudiolib> "openstudio/"
#    COMMAND ${CMAKE_COMMAND} -E echo "To create the package, cd 'Products/python_package/'"
#    COMMAND ${CMAKE_COMMAND} -E echo "${Python_EXECUTABLE} setup.py bdist_wheel"
#    COMMAND ${CMAKE_COMMAND} -E echo "${Python_EXECUTABLE} -m twine upload --verbose --skip-existing --repository ${PYTHON_PIP_REPOSITORY} dist/*"
#    DEPENDS
#      ${ALL_PYTHON_BINDING_TARGETS}
#      ${ALL_PYTHON_PACKAGE_TARGETS}
#      ${ALL_PYTHON_PACKAGE_GENERATED_SRCS}
#    WORKING_DIRECTORY ${PYTHON_PACKAGE_FOLDER}
#  )
#
#endif()
