if(BUILD_TESTING)

  # find all tests
  file(GLOB RUBY_TEST_SRC "./*.rb")

  # TODO: It doesn't not work with this executable that's in build/Ruby-install/bin
  # message("RUBY_EXECUTABLE=${RUBY_EXECUTABLE}")

  set(_RUBY_POSSIBLE_EXECUTABLE_NAMES
    ruby
    ruby2.7
    ruby2.7.2
    ruby27
    ruby272)

  # TODO: this isn't great but I haven't found a better way to locate the system ruby (and avoid the one in build/Ruby-install/)
  find_program(SYSTEM_RUBY_EXECUTABLE NAMES ${_RUBY_POSSIBLE_EXECUTABLE_NAMES}
    HINTS
      "/usr/local/rvm/rubies/ruby-2.7.2/bin/"
      "/usr/local/rvm/rubies/ruby-2.7.2/bin/ruby"
      "/usr/share/rvm/rubies/ruby-2.7.2/bin/"
      "$ENV{HOME}/.rvm/rubies/ruby-2.7.2/bin/"

      "C:/Ruby27-x64/bin/"

       "/usr/local/ruby272/bin/"
       "/usr/local/ruby27/bin/"
       "/usr/bin/"
       "/usr/local/bin/"


    NO_DEFAULT_PATH)

  if(NOT SYSTEM_RUBY_EXECUTABLE)
    message(WARNING "Your system ruby wasn't found, you won't be able to run the `ctest -R RubyTest` command and the tests won't be created at all.")
  else()
    message(STATUS "Found SYSTEM_RUBY_EXECUTABLE=${SYSTEM_RUBY_EXECUTABLE}")

    # add a test for each unit test
    set(RUBY_TEST_REQUIRES "#include test files")
    foreach(f ${RUBY_TEST_SRC})

      file(READ "${f}" CONTENTS)
      string(REGEX MATCHALL "def +test_([A-Za-z_0-9 ]+)" FOUND_TESTS ${CONTENTS})

      foreach(HIT ${FOUND_TESTS})
        string(REGEX REPLACE "def +test_([A-Za-z_0-9]+)" "\\1" TEST_NAME ${HIT})
        string(REGEX MATCH "/?([A-Za-z_0-9 ]+)\\.rb" FILE_NAME ${f})
        string(REGEX REPLACE "/?([A-Za-z_0-9 ]+)\\.rb" "\\1" FILE_NAME ${FILE_NAME})

        # Call with Ruby itself
        add_test(NAME "RubyTest-${FILE_NAME}-${TEST_NAME}"
          COMMAND "${CMAKE_COMMAND}" -E chdir "${CMAKE_CURRENT_BINARY_DIR}"
          "${SYSTEM_RUBY_EXECUTABLE}" "-I" "$<TARGET_FILE_DIR:openstudio_rb>" "${f}" "--name=test_${TEST_NAME}"
        )

        set_tests_properties("RubyTest-${FILE_NAME}-${TEST_NAME}" PROPERTIES TIMEOUT 660 )
      endforeach()
    endforeach()

  endif()
endif()
