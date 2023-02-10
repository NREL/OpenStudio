install(FILES "openstudio.py" DESTINATION Python COMPONENT "Python")

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/openstudio.py ${PROJECT_BINARY_DIR}/Products/python/openstudio.py COPYONLY)

foreach(p ${ALL_PYTHON_BINDING_TARGETS})
  #target_link_libraries(${p}
  #  PRIVATE
  #  openstudiolib
  #)
  if(UNIX AND NOT APPLE)
    target_link_libraries(${p}
      PRIVATE
      Python::Python
    )
  endif()
  #target_compile_definitions(${p} PUBLIC -DSWIGRUNTIME -DSWIGRUNTIMEINLINE)
  #target_compile_definitions(${p} PUBLIC -DSWIGRUNTIME)
endforeach()

# convenience target to build the SDK
add_custom_target(python_sdk ALL
  DEPENDS
    ${ALL_PYTHON_BINDING_TARGETS}
    ${ALL_PYTHON_GENERATED_SRCS}
)


if (BUILD_PYTHON_PIP_PACKAGE)

  # configure_file(${CMAKE_CURRENT_SOURCE_DIR}/openstudio_dynamic.py ${PROJECT_BINARY_DIR}/Products/python/openstudio_dynamic.py COPYONLY)
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/openstudio.py ${PYTHON_PACKAGE_FOLDER}/openstudio/__init__.py COPYONLY)

  if(PYTHON_PIP_REPOSITORY STREQUAL "testpypi")
    execute_process( COMMAND ${Python_EXECUTABLE} "${CMAKE_CURRENT_SOURCE_DIR}/find_pypi_tag.py" OUTPUT_VARIABLE PYPI_VERSION)
  else()
    execute_process( COMMAND ${Python_EXECUTABLE} "${CMAKE_CURRENT_SOURCE_DIR}/find_pypi_tag.py" "--pypi" OUTPUT_VARIABLE PYPI_VERSION)
  endif()
  message("On Repository '${PYTHON_PIP_REPOSITORY}', computed next available version as PYPI_VERSION=${PYPI_VERSION}")
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/setup.py ${PYTHON_PACKAGE_FOLDER}/setup.py @ONLY)

  foreach(p ${ALL_PYTHON_PACKAGE_TARGETS})
    target_link_libraries(${p}
      PRIVATE
      openstudiolib
    )
  endforeach()

  add_custom_target(python_package
    COMMAND ${CMAKE_COMMAND} -E rm -Rf "build/" "dist/" "openstudio.egg-info/"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:openstudiolib> "openstudio/"
    COMMAND ${CMAKE_COMMAND} -E echo "To create the package, cd 'Products/python_package/'"
    COMMAND ${CMAKE_COMMAND} -E echo "${Python_EXECUTABLE} setup.py bdist_wheel"
    COMMAND ${CMAKE_COMMAND} -E echo "${Python_EXECUTABLE} -m twine upload --verbose --skip-existing --repository ${PYTHON_PIP_REPOSITORY} dist/*"
    DEPENDS
      ${ALL_PYTHON_BINDING_TARGETS}
      ${ALL_PYTHON_PACKAGE_TARGETS}
      ${ALL_PYTHON_PACKAGE_GENERATED_SRCS}
    WORKING_DIRECTORY ${PYTHON_PACKAGE_FOLDER}
  )

endif()
