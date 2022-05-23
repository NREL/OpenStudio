# This file lists and installs the Conan packages needed

# TODO: DO NOT DO `set(CONAN_OPTIONS "")` since some higher level stuff is added via OpenStudioApplication
# CONAN_QT is added by OpenStudioApplication

if(NOT CONAN_OPENSTUDIO_ALREADY_RUN)

  set(CMAKE_CONAN_EXPECTED_HASH
      5cdb3042632da3efff558924eecefd580a0e786863a857ca097c3d1d43df5dcd)
  set(CMAKE_CONAN_VERSION "0.18.1")

  if(EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
    file(SHA256 "${CMAKE_BINARY_DIR}/conan.cmake" CMAKE_CONAN_HASH)
  elseif(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake"
         OR NOT "${CMAKE_CONAN_HASH}" MATCHES "${CMAKE_CONAN_EXPECTED_HASH}")
    # Put it in CMAKE_BINARY_DIR so we don't end up with two when building OpenStudioApplication
    message(
      STATUS
        "openstudio: Downloading conan.cmake ${CMAKE_CONAN_VERSION} from https://github.com/conan-io/cmake-conan"
    )
    file(
      DOWNLOAD
      "https://raw.githubusercontent.com/conan-io/cmake-conan/${CMAKE_CONAN_VERSION}/conan.cmake"
      "${CMAKE_BINARY_DIR}/conan.cmake"
      EXPECTED_HASH SHA256=${CMAKE_CONAN_EXPECTED_HASH}
      TLS_VERIFY ON)
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
    set(CONAN_GTEST "gtest/1.11.0#8aca975046f1b60c660ee9d066764d69")
  else()
    set(CONAN_GTEST "")
  endif()

  if(BUILD_RUBY_BINDINGS OR BUILD_CLI)
    # Track NREL/stable in general, on a feature branch this could be temporarily switched to NREL/testing
    set(CONAN_RUBY
        "openstudio_ruby/2.7.2@jmarrec/testing#98444b7bc8d391ea1521d7f79d4d4926"
    ) # TODO: pending https://github.com/NREL/conan-openstudio-ruby/pull/39
  endif()

  if(BUILD_BENCHMARK)
    set(CONAN_BENCHMARK "benchmark/1.6.1#94c40ebf065e3b20cab6a4f1b03a65fe")
  endif()

  # This will create the conanbuildinfo.cmake in the current binary dir, not the cmake_binary_dir
  conan_cmake_run(
    REQUIRES
    ${CONAN_READLINE}
    ${CONAN_QT}
    ${CONAN_RUBY}
    "openssl/1.1.1o#213dbdeb846a4b40b4dec36cf2e673d7" # force every package to align on the same as our conan-openstudio-ruby
    "boost/1.79.0#f664bfe40e2245fa9baf1c742591d582"
    "pugixml/1.12.1#5a39f82651eba3e7d6197903a3202e21"
    "jsoncpp/1.9.5#536d080aa154e5853332339bf576747c"
    "minizip/1.2.12#0b5296887a2558500d0323c6c94c8d02" # This depends on zlib/1.2.11, and basically patches it
    "zlib/1.2.12#3b9e037ae1c615d045a06c67d88491ae" # Also needed, so we can find zlib.h and co (+ pinning exactly is good)
    "fmt/8.1.1#b3e969f8561a85087bd0365c09bbf4fb"
    "sqlite3/3.38.5#68f3bf289cde01176e39355869c39ac0"
    "cpprestsdk/2.10.18#df2f6ac88e47cadd9c9e8e0971e00d89"
    "websocketpp/0.8.2#3fd704c4c5388d9c08b11af86f79f616"
    "geographiclib/1.52#76536a9315a003ef3511919310b2fe37"
    "swig/4.0.2#9fcccb1e39eed9acd53a4363d8129be5"
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
