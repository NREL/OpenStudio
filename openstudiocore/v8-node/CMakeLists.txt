# currently we are using only the ResultsViewer resources for Ruby tests


# find all tests

IF(BUILD_TESTING)
  file(GLOB V8_NODE_TESTS "test/*_test.js")
  FOREACH( f ${V8_NODE_TESTS} )
    get_filename_component(FileName "${f}" NAME_WE)

    ADD_TEST(NAME "V8NodeTest_${FileName}"
      COMMAND "${CMAKE_COMMAND}" -E chdir "${CMAKE_CURRENT_BINARY_DIR}"
      "${NODE_BIN_DIR}/node" "${f}"
      )

    set_property(TEST V8NodeTest_${FileName}
      PROPERTY ENVIRONMENT
      "NODE_PATH=${CMAKE_BINARY_DIR}/Products/v8"
      )
  ENDFOREACH()


  file(GLOB JASMINE_TESTS "jasmine/*")
  FOREACH( f ${JASMINE_TESTS} )
    get_filename_component(FileName "${f}" NAME_WE)

    ADD_TEST(NAME "V8JasmineTest_${FileName}"
      COMMAND "${CMAKE_COMMAND}" -E chdir "${CMAKE_CURRENT_BINARY_DIR}"
      "${NODE_BIN_DIR}/jasmine-node" "${f}"
      )

    set_property(TEST V8JasmineTest_${FileName}
      PROPERTY ENVIRONMENT
      "NODE_PATH=${CMAKE_BINARY_DIR}/Products/v8"
      "PATH=${NODE_BIN_DIR}:$ENV{PATH}"
      )
  ENDFOREACH()
ENDIF(BUILD_TESTING)

IF (APPLE OR WIN32)
  IF(BUILD_NODE_MODULES)
    INSTALL(FILES "OpenStudio.js" DESTINATION "node/openstudio/" )
  ELSE()
    INSTALL(FILES "OpenStudio.js" DESTINATION "v8/openstudio/" )
  ENDIF()
ELSE()
  IF(BUILD_NODE_MODULES)
    INSTALL(FILES "OpenStudio.js" DESTINATION "lib/openstudio/node" )
  ELSE()
    INSTALL(FILES "OpenStudio.js" DESTINATION "lib/openstudio/v8" )
  ENDIF()
ENDIF()
