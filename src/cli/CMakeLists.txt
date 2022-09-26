if (APPLE)
   find_library(COREFOUNDATION_LIBRARY CoreFoundation )
endif (APPLE)

set(CLI_SRC
  main.cpp
  RubyCLI.hpp
  RubyCLI.cpp
)

if(WIN32)
  CONFIGURE_FILE_WITH_CHECKSUM(openstudio.rc.in "${CMAKE_CURRENT_BINARY_DIR}/openstudio.rc")

  # This is really just about RC but instead of spending hours trying to figure how to disable /bigobj just for that part,
  # I removed the definition altogether and didn't hit any issue.
  remove_definitions(/bigobj)

  add_executable(openstudio
    ${CLI_SRC}
    "${CMAKE_CURRENT_BINARY_DIR}/openstudio.rc"
  )
else()
  add_executable(openstudio
    ${CLI_SRC}
  )
endif()

target_link_libraries(openstudio
  PRIVATE
  openstudiolib
  openstudio_workflow
  CONAN_PKG::cli11
)

if(BUILD_RUBY_BINDINGS)
  add_dependencies(openstudio rubyengine)
endif()

if(BUILD_PYTHON_BINDINGS)
  add_dependencies(openstudio pythonengine)
endif()

if( APPLE )
  target_link_libraries(openstudio PRIVATE ${COREFOUNDATION_LIBRARY})
endif()

install(TARGETS openstudio EXPORT openstudio COMPONENT "CLI")

if( BUILD_PAT )
  if( APPLE )
    install(TARGETS openstudio
      DESTINATION ParametricAnalysisTool.app/Contents/Resources/OpenStudio/bin/
      COMPONENT PAT
    )
  endif()
endif()


###############################################################################
#                        T E S T I N G:   C T E S T S                         #
###############################################################################

file(GLOB RUBY_TEST_SRC
  # find all CLI test
  "test/test*.rb"

  # Also Run the ruby tests with the cli
  "../../ruby/test/*.rb"
)

# message("CLI: RUBY_TEST_SRC=${RUBY_TEST_SRC}")

# add a test for each unit test
set(RUBY_TEST_REQUIRES "#include test files")
foreach(f ${RUBY_TEST_SRC})

  file(READ "${f}" CONTENTS)
  string(REGEX MATCHALL "def +test_([A-Za-z_0-9 ]+)" FOUND_TESTS ${CONTENTS})

  foreach(HIT ${FOUND_TESTS})
    string(REGEX REPLACE "def +test_([A-Za-z_0-9]+)" "\\1" TEST_NAME ${HIT})
    string(REGEX MATCH "/?([A-Za-z_0-9 ]+)\\.rb" FILE_NAME ${f})
    string(REGEX REPLACE "/?([A-Za-z_0-9 ]+)\\.rb" "\\1" FILE_NAME ${FILE_NAME})
    if(BUILD_TESTING)

        add_test(NAME "CLITest-${FILE_NAME}-${TEST_NAME}"
          COMMAND "${CMAKE_COMMAND}" -E chdir "${CMAKE_CURRENT_BINARY_DIR}"
          "$<TARGET_FILE:openstudio>" "${f}" "--name=test_${TEST_NAME}"
        )

      set_tests_properties("CLITest-${FILE_NAME}-${TEST_NAME}" PROPERTIES TIMEOUT 660 )
    endif()
  endforeach()
endforeach()
