if(BUILD_TESTING)

  # find all tests
  file(GLOB RUBY_TEST_SRC "./*.rb")


  # **NOTE**: we do not want to grab the conan one, which is statically built on all platforms, and is a msvc build on windows
  # Instead, we want to grab the regularly installed one on your system

  set(_RUBY_POSSIBLE_EXECUTABLE_NAMES
    ruby
    ruby3.2
    ruby3.2.2
    ruby32
    ruby322
  )

  # TODO: this isn't great but I haven't found a better way to locate the system ruby (and avoid the conan one)
  find_program(SYSTEM_RUBY_EXECUTABLE NAMES ${_RUBY_POSSIBLE_EXECUTABLE_NAMES}
    HINTS
      "/usr/local/rvm/rubies/ruby-3.2.2/bin/"
      "/usr/local/rvm/rubies/ruby-3.2.2/bin/ruby"
      "/usr/share/rvm/rubies/ruby-3.2.2/bin/"
      "$ENV{HOME}/.rvm/rubies/ruby-3.2.2/bin/"
      "$ENV{HOME}/.rbenv/versions/3.2.2/bin/"

      "C:/Ruby32-x64/bin/"

       "/usr/local/ruby322/bin/"
       "/usr/local/ruby32/bin/"
       "/usr/bin/"
       "/usr/local/bin/"

    NO_DEFAULT_PATH)

  if(SYSTEM_RUBY_EXECUTABLE)
    # Validate the version
    execute_process (COMMAND "${SYSTEM_RUBY_EXECUTABLE}" -e "puts RUBY_VERSION"
      RESULT_VARIABLE _result
      OUTPUT_VARIABLE _system_ruby_version
      ERROR_QUIET
      OUTPUT_STRIP_TRAILING_WHITESPACE)
    if(_result OR (_system_ruby_version VERSION_LESS 3.2) OR (_system_ruby_version VERSION_GREATER_EQUAL 3.3))
      if(_result)
        message(WARNING "Cannot use the interpreter \"${SYSTEM_RUBY_EXECUTABLE}\"")
      else()
        message(WARNING "Wrong version \"${_system_ruby_version}\" for the interpreter \"${SYSTEM_RUBY_EXECUTABLE}\", not >= 3.2 < 3.3")
      endif()
      set_property (CACHE SYSTEM_RUBY_EXECUTABLE PROPERTY VALUE "SYSTEM_RUBY_EXECUTABLE-NOTFOUND")
    else()
      message(STATUS "Found SYSTEM_RUBY_EXECUTABLE=${SYSTEM_RUBY_EXECUTABLE} with version ${_system_ruby_version}")
    endif()
   endif()

  if(NOT SYSTEM_RUBY_EXECUTABLE)
    message(WARNING "A valid system ruby (3.2.2 or near) wasn't found, you won't be able to run the `ctest -R RubyTest` command and the tests won't be created at all.")
  else()
    # add a test for each unit test
    set(RUBY_TEST_REQUIRES "#include test files")
    foreach(f ${RUBY_TEST_SRC})

      file(READ "${f}" CONTENTS)
      string(REGEX MATCHALL "def +test_([A-Za-z_0-9 ]+)" FOUND_TESTS ${CONTENTS})

      foreach(HIT ${FOUND_TESTS})
        string(REGEX REPLACE "def +test_([A-Za-z_0-9]+)" "\\1" TEST_NAME ${HIT})
        string(REGEX MATCH "/?([A-Za-z_0-9 ]+)\\.rb" FILE_NAME ${f})
        string(REGEX REPLACE "/?([A-Za-z_0-9 ]+)\\.rb" "\\1" FILE_NAME ${FILE_NAME})

        set(CTEST_NAME "RubyTest-${FILE_NAME}-${TEST_NAME}")

        # Call with Ruby itself
        add_test(NAME "${CTEST_NAME}"
          COMMAND "${CMAKE_COMMAND}" -E chdir "${CMAKE_CURRENT_BINARY_DIR}"
          "${SYSTEM_RUBY_EXECUTABLE}" "-I" "$<TARGET_FILE_DIR:openstudio_rb>" "${f}" "--name=test_${TEST_NAME}"
        )
        set_tests_properties("${CTEST_NAME}" PROPERTIES TIMEOUT 660 )
      endforeach()

    endforeach()

  endif()
endif()
