# This file lists and installs the Conan packages needed

# TODO: DO NOT DO `set(CONAN_OPTIONS "")` since some higher level stuff is added via OpenStudioApplication
# CONAN_QT is added by OpenStudioApplication

if(NOT CONAN_OPENSTUDIO_ALREADY_RUN)

  set(CMAKE_CONAN_EXPECTED_HASH 52a255a933397fdce3d0937f9c737e98)
  set(CMAKE_CONAN_VERSION "0.17.0")

  if(EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
    file(MD5 "${CMAKE_BINARY_DIR}/conan.cmake" CMAKE_CONAN_HASH)
  endif()
  if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake"
     OR NOT "${CMAKE_CONAN_HASH}" MATCHES "${CMAKE_CONAN_EXPECTED_HASH}")
    # Put it in CMAKE_BINARY_DIR so we don't end up with two when building OpenStudioApplication
    message(
      STATUS
        "openstudio: Downloading conan.cmake ${CMAKE_CONAN_VERSION} from https://github.com/conan-io/cmake-conan"
    )
    file(
      DOWNLOAD
      "https://github.com/conan-io/cmake-conan/raw/${CMAKE_CONAN_VERSION}/conan.cmake"
      "${CMAKE_BINARY_DIR}/conan.cmake")
  else()
    message(STATUS "openstudio: using existing conan.cmake")
  endif()

  include(${CMAKE_BINARY_DIR}/conan.cmake)

  conan_check(VERSION 1.43.0 REQUIRED)

  message(STATUS "openstudio: RUNNING CONAN")

  # Add NREL remote and place it first in line, since we vendored dependencies to NREL's repo, they will be picked first
  # TJC 2021-04-27 bintray.com is decommissioned as of 2021-05-01. See commercialbuildings as replacement below.
  conan_add_remote(
    NAME nrel INDEX 0 URL
    https://conan.openstudio.net/artifactory/api/conan/openstudio)

  conan_add_remote(
    NAME bincrafters URL
    https://bincrafters.jfrog.io/artifactory/api/conan/public-conan)

  #conan_add_remote(NAME jmarrec
  #  URL https://api.bintray.com/conan/jmarrec/testing)

  # Enable revisions in conan: check if they are already enabled, if not do it and warn user
  execute_process(
    COMMAND ${CONAN_CMD} config get general.revisions_enabled
    OUTPUT_VARIABLE CONAN_REV_STATUS
    ERROR_VARIABLE CONAN_REV_STATUS
    OUTPUT_STRIP_TRAILING_WHITESPACE)

  message(
    STATUS "Conan: config get general.revisions_enabled=${CONAN_REV_STATUS}")
  if(NOT "${CONAN_REV_STATUS}" STREQUAL "True")
    message(
      AUTHOR_WARNING
        "Conan: Force enabling revisions (conan config set general.revisions_enabled=True)"
    )
    execute_process(COMMAND ${CONAN_CMD} config set
                            general.revisions_enabled=True)
  endif()

  # TODO:  list(APPEND CONAN_OPTIONS "fmt:header_only=True")

  # You do want to rebuild packages if there's a newer recipe in the remote (which applies mostly to our own openstudio_ruby where we don't
  # bump the actual package version when we make changes) than the binaries were built with
  # 'outdated' also acts like 'missing': if no binary, will build them.
  # list(APPEND CONAN_BUILD "outdated")
  list(APPEND CONAN_BUILD "missing")

  if(BUILD_TESTING)
    set(CONAN_GTEST "gtest/1.11.0#d0dbd15442f5646c00fdecf832f6f3e3")
  else()
    set(CONAN_GTEST "")
  endif()

  if(BUILD_RUBY_BINDINGS OR BUILD_CLI)
    # Track NREL/stable in general, on a feature branch this could be temporarily switched to NREL/testing
    set(CONAN_RUBY
        "openstudio_ruby/2.7.2@nrel/stable#c32f3c58530990684fdd9510bef676a3")
  endif()

  if(BUILD_BENCHMARK)
    set(CONAN_BENCHMARK "benchmark/1.6.0#abc2853305ae1834eb47ccb84a715c81")
  endif()

  # This will create the conanbuildinfo.cmake in the current binary dir, not the cmake_binary_dir
  conan_cmake_run(
    REQUIRES
    ${CONAN_READLINE}
    ${CONAN_QT}
    ${CONAN_RUBY}
    "openssl/1.1.1m#18c0b6de6e23449e8c862a20d8dc4e87" # force every package to align on the same as our conan-openstudio-ruby
    "boost/1.78.0#c6275b1e5ecf6e51fe8baf1e322bb065"
    "pugixml/1.11#a5f7cda7d93b35886bd4ea135e08f25b"
    "jsoncpp/1.9.5#536d080aa154e5853332339bf576747c"
    "minizip/1.2.11#0658b664480f2a0755b88502743d5d0d" # This depends on zlib/1.2.11, and basically patches it
    "zlib/1.2.11#683857dbd5377d65f26795d4023858f9" # Also needed, so we can find zlib.h and co (+ pinning exactly is good)
    "fmt/8.0.1#826bcee69ee04232ff9864766afa728c"
    "sqlite3/3.37.1#990b9120900d8f9f45294397c0e152b0"
    "cpprestsdk/2.10.18#990b9120900d8f9f45294397c0e152b0"
    "websocketpp/0.8.2#2ec4cd9d431005dd52ee012df204c362"
    "geographiclib/1.52.0#f8aea2d67350aff0e107df2c2c98287e"
    "swig/4.0.2#187cec21277aafbeae11dcc3b936ce74"
    "tinygltf/2.5.0#c8b2aca9505e86312bb42aa0e1c639ec"
    ${CONAN_GTEST}
    ${CONAN_BENCHMARK}
    # Override to avoid dependency mismatches
    #"bzip2/1.0.8#d4a5c7144832d75fc3f349c5346160b0"
    #"libyaml/0.2.5#9e234874df88c3ba7249f6d1368fceaf"
    BASIC_SETUP
    CMAKE_TARGETS
    NO_OUTPUT_DIRS
    OPTIONS
    ${CONAN_OPTIONS}
    BUILD
    ${CONAN_BUILD}
    # Passes `-u, --update`    to conan install: Check updates exist from upstream remotes
    # That and build=outdated should ensure we track the right
    # Now that we pin dependencies, there is no point looking upstream really, so we'll save valuable configuration time by not doing it
    UPDATE)

  set(CONAN_OPENSTUDIO_ALREADY_RUN TRUE)

  #message("CONAN_TARGETS = ${CONAN_TARGETS}")
  message(STATUS "openstudio: DONE RUNNING CONAN")

else()

  message(STATUS "openstudio: CONAN RUN BY CALLING SCRIPT")

endif()
