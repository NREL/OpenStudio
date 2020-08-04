
# DLM: currently there are some multiply defined symbols, force them for now, investigate later
#set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} /FORCE:MULTIPLE")

# ignore deprecated warnings since we will export deprecated methods
#add_definitions(/wd4996)

# ignore macro redefinition
#add_definitions(/wd4005)

## build a single python dll
#add_library(
#  openstudio_python
#  MODULE
#  ${ALL_PYTHON_WRAPPER_FILES}
#)
#add_dependencies(openstudio_python ${ALL_PYTHON_WRAPPER_TARGETS})

#set_source_files_properties(${ALL_PYTHON_WRAPPER_FILES} PROPERTIES GENERATED TRUE)

#target_link_libraries(openstudio_python
# ${ALL_PYTHON_BINDING_DEPENDS}
# ${Boost_LIBRARIES}
# ${Python_LIBRARIES}
#)

#install(TARGETS openstudio_python DESTINATION Python CONFIGURATIONS COMPONENT "Python")

install(FILES "openstudio.py" DESTINATION Python COMPONENT "Python")
#add_custom_command(TARGET openstudio_python
  #POST_BUILD
  #COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/openstudio_dynamic.py $<TARGET_FILE_DIR:openstudio_python>/__init__.py
#)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/openstudio.py ${PROJECT_BINARY_DIR}/Products/python/openstudio.py COPYONLY)
# configure_file(${CMAKE_CURRENT_SOURCE_DIR}/openstudio_dynamic.py ${PROJECT_BINARY_DIR}/Products/python/openstudio_dynamic.py COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/openstudio.py ${PROJECT_BINARY_DIR}/Products/python/openstudio/__init__.py COPYONLY)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/setup.py ${PROJECT_BINARY_DIR}/Products/python/setup.py @ONLY)

set(CMAKE_INSTALL_RPATH "$ORIGIN")
foreach(p ${ALL_PYTHON_BINDING_TARGETS})
  target_link_libraries(${p}
    openstudiolib
  )
endforeach()

# convenience target to build the SDK
add_custom_target(python_sdk ALL
  DEPENDS ${ALL_PYTHON_BINDINGS}
)

