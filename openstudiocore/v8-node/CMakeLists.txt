# currently we are using only the ResultsViewer resources for Ruby tests


# find all tests

IF(BUILD_TESTING)
  file(GLOB V8_NODE_TESTS "test/*_test.js")
  foreach(f ${V8_NODE_TESTS})
    get_filename_component(FileName "${f}" NAME_WE)

    add_test(NAME "V8NodeTest_${FileName}"
      COMMAND "${CMAKE_COMMAND}" -E chdir "${CMAKE_CURRENT_BINARY_DIR}"
      "${NODE_BIN_DIR}/node" "${f}"
    )

    set_property(TEST V8NodeTest_${FileName}
      PROPERTY ENVIRONMENT
      "NODE_PATH=${CMAKE_BINARY_DIR}/Products/v8"
    )
  endforeach()


  file(GLOB JASMINE_TESTS "jasmine/*")
  foreach(f ${JASMINE_TESTS})
    get_filename_component(FileName "${f}" NAME_WE)

    add_test(NAME "V8JasmineTest_${FileName}"
      COMMAND "${CMAKE_COMMAND}" -E chdir "${CMAKE_CURRENT_BINARY_DIR}"
      "${NODE_BIN_DIR}/jasmine-node" "${f}"
    )

    set_property(TEST V8JasmineTest_${FileName}
      PROPERTY ENVIRONMENT
      "NODE_PATH=${CMAKE_BINARY_DIR}/Products/v8"
      "PATH=${NODE_BIN_DIR}:$ENV{PATH}"
    )
  endforeach()
endif()

if(APPLE OR WIN32)
  if(BUILD_NODE_MODULES)
    install(FILES "OpenStudio.js" DESTINATION "node/openstudio/")
  else()
    install(FILES "OpenStudio.js" DESTINATION "v8/openstudio/")
  endif()
else()
  if(BUILD_NODE_MODULES)
    install(FILES "OpenStudio.js" DESTINATION "lib/openstudio-${OPENSTUDIO_VERSION}/node")
  else()
    install(FILES "OpenStudio.js" DESTINATION "lib/openstudio-${OPENSTUDIO_VERSION}/v8")
  endif()
endif()
