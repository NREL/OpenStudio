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

  conan_check(VERSION 1.0.0 REQUIRED)

  message(STATUS "openstudio: RUNNING CONAN")

  conan_add_remote(NAME bincrafters
    URL https://api.bintray.com/conan/bincrafters/public-conan)
  conan_add_remote(NAME nrel
    URL https://api.bintray.com/conan/commercialbuilding/nrel)

  # TODO: eventually delete and replace with the official NREL one (and update ruby in conan_cmake_run below)
  conan_add_remote(NAME lefticus
    URL https://api.bintray.com/conan/lefticus/nrel)
  conan_add_remote(NAME jmarrec
    URL https://api.bintray.com/conan/jmarrec/testing)

  # Convenience variable to set a consistent version for individual boost packages
  set(BOOST_VERSION "1.69.0")

  set(CONAN_OPENSSL "OpenSSL/1.1.0g@conan/stable")
  set(CONAN_BOOST_ASIO "boost_asio/${BOOST_VERSION}@bincrafters/stable")
  set(CONAN_WEBSOCKETPP "websocketpp/0.8.1@bincrafters/stable")
  list(APPEND CONAN_OPTIONS "zlib:minizip=True")
  # You do want to rebuild packages if there's a newer recipe in the remote (which applies mostly to our own openstudio_ruby where we don't
  # bump the actual package version when we make changes
  # 'outdated' also acts like 'missing': if no binary, will build them.
  list(APPEND CONAN_BUILD "outdated")

  if (MSVC)
    # No-op
  elseif (APPLE)
    # Do we really need to force build these?
    list(APPEND CONAN_BUILD "boost_asio")
    list(APPEND CONAN_BUILD "websocketpp")
  else()
    list(APPEND CONAN_OPTIONS "jsoncpp:use_pic=True")
    list(APPEND CONAN_BUILD "boost_asio")
    list(APPEND CONAN_BUILD "websocketpp")
  endif()

  if (BUILD_TESTING)
    set(CONAN_GTEST "gtest/1.8.1@bincrafters/stable")
  else()
    set(CONAN_GTEST "")
  endif()

  # DLM: add option for shared libs if we are building shared?

  # This will create the conanbuildinfo.cmake in the current binary dir, not the cmake_binary_dir
  conan_cmake_run(REQUIRES
    ${CONAN_QT}
    ${CONAN_OPENSSL}
    ${CONAN_BOOST_ASIO}
    openstudio_ruby/2.5.5@jmarrec/testing # Replace with NREL when stable
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
    pugixml/1.9@bincrafters/stable
    jsoncpp/1.8.4@theirix/stable
    zlib/1.2.11@conan/stable
    fmt/5.2.1@bincrafters/stable
    sqlite3/3.27.2@bincrafters/stable
    cpprestsdk/2.10.13@bincrafters/stable
    ${CONAN_WEBSOCKETPP}
    geographiclib/1.49@bincrafters/stable
    ${CONAN_GTEST}
    BASIC_SETUP CMAKE_TARGETS NO_OUTPUT_DIRS
    OPTIONS ${CONAN_OPTIONS}
    BUILD ${CONAN_BUILD}
  )

  set(CONAN_OPENSTUDIO_ALREADY_RUN TRUE)

  #message("CONAN_TARGETS = ${CONAN_TARGETS}")
  message(STATUS "openstudio: DONE RUNNING CONAN")

else()

  message(STATUS "openstudio: CONAN RUN BY CALLING SCRIPT")

endif()
