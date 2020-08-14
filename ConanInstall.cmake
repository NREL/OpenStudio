# This file lists and installs the Conan packages needed

# TODO: DO NOT DO `set(CONAN_OPTIONS "")` since some higher level stuff is added via OpenStudioApplication
# CONAN_QT is added by OpenStudioApplication


if(NOT CONAN_OPENSTUDIO_ALREADY_RUN)

  set(CMAKE_CONAN_EXPECTED_HASH 773399d30bb924959b86883f95d64df6)
  set(CMAKE_CONAN_VERSION "v0.15")

  if(EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
    file(MD5 "${CMAKE_BINARY_DIR}/conan.cmake" CMAKE_CONAN_HASH)
  endif()
  if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake" OR NOT "${CMAKE_CONAN_HASH}" MATCHES "${CMAKE_CONAN_EXPECTED_HASH}")
    # Put it in CMAKE_BINARY_DIR so we don't end up with two when building OpenStudioApplication
    message(STATUS "openstudio: Downloading conan.cmake ${CMAKE_CONAN_VERSION} from https://github.com/conan-io/cmake-conan")
    file(DOWNLOAD "https://github.com/conan-io/cmake-conan/raw/${CMAKE_CONAN_VERSION}/conan.cmake"
       "${CMAKE_BINARY_DIR}/conan.cmake")
  else()
    message(STATUS "openstudio: using existing conan.cmake")
  endif()

  include(${CMAKE_BINARY_DIR}/conan.cmake)

  conan_check(VERSION 1.28.0 REQUIRED)

  message(STATUS "openstudio: RUNNING CONAN")

  # Add NREL remote and place it first in line, since we vendored dependencies to NREL's repo, they will be picked first
  conan_add_remote(NAME nrel INDEX 0
    URL https://api.bintray.com/conan/commercialbuilding/nrel)

  conan_add_remote(NAME bincrafters
    URL https://api.bintray.com/conan/bincrafters/public-conan)

  #conan_add_remote(NAME jmarrec
  #  URL https://api.bintray.com/conan/jmarrec/testing)

  list(APPEND CONAN_OPTIONS "zlib:minizip=True")
  # TODO:  list(APPEND CONAN_OPTIONS "fmt:header_only=True")

  # You do want to rebuild packages if there's a newer recipe in the remote (which applies mostly to our own openstudio_ruby where we don't
  # bump the actual package version when we make changes) than the binaries were built with
  # 'outdated' also acts like 'missing': if no binary, will build them.
  list(APPEND CONAN_BUILD "outdated")

  if (BUILD_TESTING)
    set(CONAN_GTEST "gtest/1.10.0")
  else()
    set(CONAN_GTEST "")
  endif()

  # DLM: add option for shared libs if we are building shared?

  # This will create the conanbuildinfo.cmake in the current binary dir, not the cmake_binary_dir
  conan_cmake_run(REQUIRES
    ${CONAN_READLINE}
    ${CONAN_QT}
    openssl/1.1.0l # ruby 2.5.5 won't work with 1.1.1x, so use 1.1.0l here to try to force every package to align on the same as ruby
    # Track NREL/stable in general, on a feature branch this could be temporarily switched to NREL/testing
    openstudio_ruby/2.5.5@nrel/testing    # TODO: Temp #@nrel/stable
    boost/1.72.0
    pugixml/1.10
    jsoncpp/1.9.3
    zlib/1.2.11
    fmt/7.0.1
    sqlite3/3.32.3
    cpprestsdk/2.10.16
    websocketpp/0.8.2
    geographiclib/1.50.1
    swig/4.0.2
    ${CONAN_GTEST}

    # Override to avoid dependency mismatches
    bzip2/1.0.8
    BASIC_SETUP CMAKE_TARGETS NO_OUTPUT_DIRS
    OPTIONS ${CONAN_OPTIONS}
    BUILD ${CONAN_BUILD}
    # Passes `-u, --update`    to conan install: Check updates exist from upstream remotes
    # That and build=outdated should ensure we track the right
    UPDATE
  )

  set(CONAN_OPENSTUDIO_ALREADY_RUN TRUE)

  #message("CONAN_TARGETS = ${CONAN_TARGETS}")
  message(STATUS "openstudio: DONE RUNNING CONAN")

else()

  message(STATUS "openstudio: CONAN RUN BY CALLING SCRIPT")

endif()

