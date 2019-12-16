# This file lists and installs the Conan packages needed

# TODO: DO NOT DO `set(CONAN_OPTIONS "")` since some higher level stuff is added via OpenStudioApplication
# CONAN_QT is added by OpenStudioApplication


if(NOT CONAN_OPENSTUDIO_ALREADY_RUN)

  set(CMAKE_CONAN_EXPECTED_HASH 709180234748692a642f9e5c4d80d328)
  set(CMAKE_CONAN_VERSION "v0.14")

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

  conan_check(VERSION 1.20.0 REQUIRED)

  message(STATUS "openstudio: RUNNING CONAN")

  conan_add_remote(NAME bincrafters
    URL https://api.bintray.com/conan/bincrafters/public-conan)
  conan_add_remote(NAME nrel
    URL https://api.bintray.com/conan/commercialbuilding/nrel)
  # TODO: Temp
  conan_add_remote(NAME jmarrec
    URL https://api.bintray.com/conan/jmarrec/testing)

  # Convenience variable to set a consistent version for individual boost packages
  set(BOOST_VERSION "1.69.0")

  list(APPEND CONAN_OPTIONS "zlib:minizip=True")
  # You do want to rebuild packages if there's a newer recipe in the remote (which applies mostly to our own openstudio_ruby where we don't
  # bump the actual package version when we make changes) than the binaries were built with
  # 'outdated' also acts like 'missing': if no binary, will build them.
  list(APPEND CONAN_BUILD "outdated")

  if (MSVC)
    # No-op
  elseif (APPLE)
    # No-op
  else()
    # No-op: jsoncpp is fine in 1.9.2, it uses fPIC (not use_pic) and defaults to true
  endif()

  if (BUILD_TESTING)
    set(CONAN_GTEST "gtest/1.10.0") # or 1.8.1?
  else()
    set(CONAN_GTEST "")
  endif()

  # DLM: add option for shared libs if we are building shared?

  # This will create the conanbuildinfo.cmake in the current binary dir, not the cmake_binary_dir
  conan_cmake_run(REQUIRES
    ${CONAN_READLINE}
    ${CONAN_QT}
    openssl/1.1.1d # or openssl/1.1.0l
    # Track NREL/stable in general, on a feature branch this could be temporarily switched to NREL/testing
    openstudio_ruby/2.5.5@nrel/stable
    boost_asio/${BOOST_VERSION}@bincrafters/stable
    boost_program_options/${BOOST_VERSION}@bincrafters/stable
    boost_regex/${BOOST_VERSION}@bincrafters/stable
    boost_filesystem/${BOOST_VERSION}@bincrafters/stable
    boost_crc/${BOOST_VERSION}@bincrafters/stable
    boost_algorithm/${BOOST_VERSION}@bincrafters/stable
    boost_uuid/${BOOST_VERSION}@bincrafters/stable
    boost_log/${BOOST_VERSION}@bincrafters/stable
    boost_numeric_ublas/${BOOST_VERSION}@bincrafters/stable
    boost_functional/${BOOST_VERSION}@bincrafters/stable
    boost_geometry/${BOOST_VERSION}@bincrafters/stable
    pugixml/1.10@bincrafters/stable
    jsoncpp/1.9.2
    zlib/1.2.11
    fmt/6.0.0
    sqlite3/3.30.1 # Or 3.29.0
    cpprestsdk/2.10.14@bincrafters/stable
    websocketpp/0.8.1@jmarrec/stable
    # websocketpp/0.8.1@jmarrec/stable # TODO: Temp to avoid hitting https://github.com/bincrafters/community/issues/1069
    geographiclib/1.49@bincrafters/stable
    swig_installer/4.0.1@bincrafters/stable
    ${CONAN_GTEST}
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

