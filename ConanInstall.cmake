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

  # Enable revisions in conan: check if they are already enabled, if not do it an warn user
  execute_process(
    COMMAND ${CONAN_CMD} config get general.revisions_enabled
    OUTPUT_VARIABLE CONAN_REV_STATUS
    ERROR_VARIABLE CONAN_REV_STATUS
    OUTPUT_STRIP_TRAILING_WHITESPACE)

  message(STATUS "Conan: config get general.revisions_enabled=${CONAN_REV_STATUS}")
  if (NOT "${CONAN_REV_STATUS}" STREQUAL "True")
    message(AUTHOR_WARNING "Conan: Force enabling revisions (conan config set general.revisions_enabled=True)")
    execute_process(COMMAND ${CONAN_CMD} config set general.revisions_enabled=True)
  endif()

  list(APPEND CONAN_OPTIONS "zlib:minizip=True")
  # TODO:  list(APPEND CONAN_OPTIONS "fmt:header_only=True")

  # You do want to rebuild packages if there's a newer recipe in the remote (which applies mostly to our own openstudio_ruby where we don't
  # bump the actual package version when we make changes) than the binaries were built with
  # 'outdated' also acts like 'missing': if no binary, will build them.
  # list(APPEND CONAN_BUILD "outdated")
  list(APPEND CONAN_BUILD "missing")

  if (BUILD_TESTING)
    set(CONAN_GTEST "gtest/1.10.0#ef88ba8e54f5ffad7d706062d0731a40")
  else()
    set(CONAN_GTEST "")
  endif()

  if(BUILD_RUBY_BINDINGS)
    set(CONAN_RUBY "openstudio_ruby/2.5.5@nrel/testing")
  endif()

  # This will create the conanbuildinfo.cmake in the current binary dir, not the cmake_binary_dir
  conan_cmake_run(REQUIRES
    ${CONAN_READLINE}
    ${CONAN_QT}
    "openssl/1.1.0l#7f3fa5cfcfba31fffa344c71a9795176" # ruby 2.5.5 won't work with 1.1.1x, so use 1.1.0l here to try to force every package to align on the same as ruby
    # Track NREL/stable in general, on a feature branch this could be temporarily switched to NREL/testing
    "openstudio_ruby/2.5.5@nrel/stable#29449dcdcc813fb3f4730365902afc3c"    # TODO: Temp #@nrel/stable
    "boost/1.73.0#4129a76c9b83c300fc103e36d1908792"
    "pugixml/1.10#64b3ebc897bb9d9854c8a2443bf112a8"
    "jsoncpp/1.9.3#073a6d3cb40911d7c8027bddb6ae7dbf"
    "zlib/1.2.11#0df31fd24179543f5720ec7beb2a88d7"
    "fmt/7.0.1#0580b1492b1dddb43b1768e68f25c43c"
    "sqlite3/3.32.3#914492672c458f8be511e3800c14c717"
    "cpprestsdk/2.10.16#d097ff9a8719d9d0ed34293c2ebd90ed"
    "websocketpp/0.8.2#6d77b9b8a2368fa5fd5377af0c0ca211"
    "geographiclib/1.50.1#b1a7966385dead17ec170b25a99cf71b"
    "swig/4.0.2#bfafb16cd2bea6af3b8003163abcbd09"
    ${CONAN_GTEST}

    # Override to avoid dependency mismatches
    #"bzip2/1.0.8#d4a5c7144832d75fc3f349c5346160b0"
    #"libyaml/0.2.5#9e234874df88c3ba7249f6d1368fceaf"
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
