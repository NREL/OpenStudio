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

  conan_check(VERSION 1.48.0 REQUIRED)

  message(STATUS "openstudio: RUNNING CONAN")

  # Add NREL remote and place it first in line, since we vendored dependencies to NREL's repo, they will be picked first
  # TJC 2021-04-27 bintray.com is decommissioned as of 2021-05-01. See commercialbuildings as replacement below.
  if(LSB_RELEASE_ID_SHORT MATCHES "CentOS")
    # Use this specific remote where I uploaded the centos-build packages
    # The os is still Linux, the compiler is still GCC. But the GLIBC used is **way older**
    conan_add_remote(NAME openstudio-centos INDEX 0
      URL https://conan.openstudio.net/artifactory/api/conan/openstudio-centos)

    # Pass `-D_GLIBCXX_USE_CXX11_ABI=0` to make sure it detects libstdc++ and not libstdc++1
    add_definitions(-D_GLIBCXX_USE_CXX11_ABI=0)
    # Centos uses a different channel recipe for ruby
    if(BUILD_RUBY_BINDINGS OR BUILD_CLI)
      # Track NREL/stable in general, on a feature branch this could be temporarily switched to NREL/testing
      set(CONAN_RUBY "openstudio_ruby/2.7.2@nrel/centos#ae043c41b4bec82e98ca765ce8b32a11")
    endif()

  else()
    conan_add_remote(NAME nrel INDEX 0
      URL https://conan.openstudio.net/artifactory/api/conan/openstudio)

    if(BUILD_RUBY_BINDINGS OR BUILD_CLI)
      # Track NREL/stable in general, on a feature branch this could be temporarily switched to NREL/testing
      # TODO: temp, revert to stable soon
      set(CONAN_RUBY "openstudio_ruby/2.7.2@nrel/testing#d66e3b66568b13acf3b16d866bec68d0")
     endif()
  endif()

  # conan_add_remote(
  #   NAME bincrafters URL
  #   https://bincrafters.jfrog.io/artifactory/api/conan/public-conan)

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

  if(APPLE)
    # #4120 - global is the 'default' visibility in gcc/clang
    list(APPEND CONAN_OPTIONS "boost:visibility=global")
  endif()

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


  # Build ALL dependencies to avoid problems with the way too old CentOS GLIBC
  # Please read: `developer/conan/binary_incompatility_glibc.md`
  if(DEFINED CONAN_FIRST_TIME_BUILD_ALL)
    if(CONAN_FIRST_TIME_BUILD_ALL)
      message("FIRST TIME: FORCE BUILDING ALL CONAN PACKAGES")
      # TODO: Try to avoid rebuilding everything...?
      set(CONAN_BUILD "all") # This works, but it's gonna be sloooowww
      # if(LSB_RELEASE_ID_SHORT MATCHES "Ubuntu" AND LSB_RELEASE_VERSION_SHORT MATCHES "18.04")
      #   # build only the ones that are problematic
      #   # boost: undefined reference to statx
      #   list(APPEND CONAN_BUILD "boost")
      #   # This isn't enough for Ubuntu 18.04, I also get another issue: undefined reference to fnctl64 (fnctl -> fnclt64 happened in GLIBC 2.28) when linking libopenstudio.lib
      #   # but I do not know which package it's coming from
      # endif()
      # Force switch it off for the next time
      set(CONAN_FIRST_TIME_BUILD_ALL OFF  CACHE BOOL OFF FORCE)
    endif()
  endif()
  message("CONAN_BUILD=${CONAN_BUILD}")

  if(BUILD_BENCHMARK)
    set(CONAN_BENCHMARK "benchmark/1.6.1#94c40ebf065e3b20cab6a4f1b03a65fe")
  endif()

  # msys2/cci.latest is not supported on win32. We don't expect to run it on something else than an x86_64 anyways
  if(CMAKE_SIZEOF_VOID_P EQUAL 4) # 32 bit
    set(CONAN_FORCE_SETTINGS_BUILD SETTINGS_BUILD arch=x86_64)
    message(AUTHOR_WARNING "Forcing --settings:build arch=x86_64")
    if(NOT WIN32)
      message(AUTHOR_WARNING "You are trying to build on x86 on a non-windows, this is completely untested and unsupported")
    endif()
  endif()

  set(CONAN_UPDATE "UPDATE")

  if(CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
    set(CONAN_CMAKE_MULTI ON)
    if (NOT CONAN_CONFIGURATION_TYPES)
      # Cache it, and let user change it (if they want to pull RelWithDebInfo for eg)
      set(CONAN_CONFIGURATION_TYPES "Release;Debug" CACHE STRING "List of configurations for which you want to fetch conan packages")
      mark_as_advanced(CONAN_CONFIGURATION_TYPES)
    endif()
    message(STATUS "Conan: Using cmake_multi generator")
    set(CONAN_GENERATOR "cmake_multi")
  else()
    message(STATUS "Conan: Using cmake generator")
    set(CONAN_CMAKE_MULTI OFF)
    set(CONAN_GENERATOR "cmake")
    if (NOT CONAN_CONFIGURATION_TYPES)
      set(CONAN_CONFIGURATION_TYPES ${CMAKE_BUILD_TYPE})
    endif()
  endif()


  message(STATUS "Conan: conan_cmake_configure")
  # This will create the conanfile.txt
  conan_cmake_configure(
    REQUIRES
    ${CONAN_READLINE}
    ${CONAN_QT}
    ${CONAN_RUBY}
    "openssl/1.1.1o#213dbdeb846a4b40b4dec36cf2e673d7" # force every package to align on the same as our conan-openstudio-ruby
    "boost/1.79.0#f664bfe40e2245fa9baf1c742591d582"
    "pugixml/1.12.1#5a39f82651eba3e7d6197903a3202e21"
    "libxml2/2.9.14#fc433aeebfe525657d73334c61f96944"
    "libxslt/1.1.34#9085031f5b9b2bb328ad615cd1bf1282"
    "jsoncpp/1.9.5#536d080aa154e5853332339bf576747c"
    "minizip/1.2.12#0b5296887a2558500d0323c6c94c8d02" # This depends on zlib, and basically patches it
    "zlib/1.2.12#3b9e037ae1c615d045a06c67d88491ae" # Also needed, so we can find zlib.h and co (+ pinning exactly is good)
    "fmt/9.1.0#811e918ca4b4e0b9ddd6d5a2883efa82"
    "sqlite3/3.38.5#010911927ce1889b5cf824f45e7cd3d2"
    "cpprestsdk/2.10.18#df2f6ac88e47cadd9c9e8e0971e00d89"
    "websocketpp/0.8.2#3fd704c4c5388d9c08b11af86f79f616"
    "geographiclib/1.52#76536a9315a003ef3511919310b2fe37"
    "swig/4.0.2#9fcccb1e39eed9acd53a4363d8129be5"
    "tinygltf/2.5.0#c8b2aca9505e86312bb42aa0e1c639ec"
    "cli11/2.2.0#33cd38722fa134b15ae308dfb4e6c942"
    ${CONAN_GTEST}
    ${CONAN_BENCHMARK}
    # Override to avoid dependency mismatches
    #"bzip2/1.0.8#d4a5c7144832d75fc3f349c5346160b0"
    #"libyaml/0.2.5#9e234874df88c3ba7249f6d1368fceaf"
    GENERATORS ${CONAN_GENERATOR}
  )

  foreach(build_type ${CONAN_CONFIGURATION_TYPES})
    conan_cmake_autodetect(settings BUILD_TYPE ${build_type})
    message(STATUS "Conan: Autodetected settings for build type ${build_type}: ${settings}")

    # Avoid polluting with cppstd which prevents downloading some existing binary packages (like boost)
    # Former deprecated conan_cmake_run was NOT adding compiler.cppstd
    foreach(ARG ${settings})
      if(${ARG} MATCHES "compiler.cppstd=(.*)")
        message("Removing ${ARG}")
        list(REMOVE_ITEM settings ${ARG})
      endif()
    endforeach()

    conan_cmake_install(PATH_OR_REFERENCE .
      BUILD ${CONAN_BUILD}
      OPTIONS ${CONAN_OPTIONS}
      SETTINGS ${settings}
      ${CONAN_FORCE_SETTINGS_BUILD}
      ${CONAN_UPDATE}
    )
  endforeach()

  # Loads the conanbuildinfo.cmake / conanbuildinfo_multi.cmake
  conan_load_buildinfo()
  # conan_basic_setup in the conanbuildinfo.cmake. TARGETS => set cmake targets, NO_OUTPUT_DIRS => Don't modify the BIN / LIB folders etc
  conan_basic_setup(TARGETS NO_OUTPUT_DIRS KEEP_RPATHS)

  set(CONAN_OPENSTUDIO_ALREADY_RUN TRUE)

  #message("CONAN_TARGETS = ${CONAN_TARGETS}")
  message(STATUS "openstudio: DONE RUNNING CONAN")

else()

  message(STATUS "openstudio: CONAN RUN BY CALLING SCRIPT")

endif()
