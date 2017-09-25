cmake_minimum_required(VERSION 3.7.0)

project(OpenStudioMeta)

set(CMAKE_VERSION_MAJOR 2)
set(CMAKE_VERSION_MINOR 3)
set(CMAKE_VERSION_PATCH 0)

set(CMAKE_VERSION_BUILD "Unknown" CACHE STRING "Build number")
find_package(Git)

if(NOT GIT_FOUND)
  find_program(GIT_EXECUTABLE git HINTS "$ENV{LOCALAPPDATA}/Programs/git/bin" "C:/Program Files/Git/bin")
  if(NOT GIT_EXECUTABLE_NOTFOUND)
    set(GIT_FOUND TRUE)
  endif()
endif()

if(GIT_FOUND)
  execute_process(COMMAND "${GIT_EXECUTABLE}" "rev-parse" "--short=10" "HEAD"
                  WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
                  TIMEOUT 10
                  RESULT_VARIABLE RESULT
                  OUTPUT_VARIABLE GIT_VERSION
                  ERROR_QUIET
                  OUTPUT_STRIP_TRAILING_WHITESPACE)
  if(${RESULT} EQUAL 0 AND NOT "${GIT_VERSION}" EQUAL "${CMAKE_VERSION_BUILD}")
    set(CMAKE_VERSION_BUILD ${GIT_VERSION} CACHE STRING "Build number" FORCE) # git sha
  endif()

  get_filename_component(GIT_DIR "${GIT_EXECUTABLE}" DIRECTORY)
else()
  set(GIT_DIR "")
endif()

find_program(PATCH_EXE patch HINTS "${GIT_DIR}" "${GIT_DIR}/../bin/" "${GIT_DIR}/../usr/bin/")
string(COMPARE EQUAL "${PATCH_EXE}" "PATCH_EXE-NOTFOUND" PATCH_EXE_NOTFOUND)
if(PATCH_EXE_NOTFOUND)
  message(SEND_ERROR "Required program patch not found")
endif()

option(NIGHTLY_BUILD "Use configurations for Nightly Build" OFF)

set(OPENSTUDIO_VERSION "${CMAKE_VERSION_MAJOR}.${CMAKE_VERSION_MINOR}.${CMAKE_VERSION_PATCH}")

# Search for modules in the openstudiocore dir first to override cmake ones
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/openstudiocore/CMake" "${CMAKE_SOURCE_DIR}")

# Build C++ documentation using Doxygen
# Requires: doxygen
option(BUILD_DOCUMENTATION "Build Documentation" OFF)

if(BUILD_DOCUMENTATION)
  find_package(Doxygen REQUIRED)
endif()

# Build CSharp bindings
# Requires: SWIG
option(BUILD_CSHARP_BINDINGS "Build CSharp bindings" OFF)

# Build Java bindings
# Requires: SWIG
option(BUILD_JAVA_BINDINGS "Build Java bindings" OFF)

# Build Node V8 bindings
# Requires: SWIG
option(BUILD_V8_BINDINGS "Build V8 bindings" OFF)

# Build Python bindings
# Requires: SWIG Python
option(BUILD_PYTHON_BINDINGS "Build Python bindings" OFF)

# Build ctest testing
# Requires: EnergyPlus
option(BUILD_TESTING "Build testing targets" OFF)

# Build package
# Requires: EnergyPlus
option(BUILD_PACKAGE "Build package" OFF)

# Build regressions
option(BUILD_REGRESSIONS "Build regressions" OFF)

# Build shared libraries, configuration affects OpenStudioCore
option(BUILD_SHARED_LIBS "Build OS libs as shared libraries" OFF)

set(MAXIMIZE_CPU_USAGE OFF CACHE BOOL "Attempt to fully load the CPU during builds")
mark_as_advanced(MAXIMIZE_CPU_USAGE)

#if(MSVC)
#  # Visual Studio compiler check
#  # http://en.wikipedia.org/wiki/Visual_C%2B%2B#32-bit_and_64-bit_versions
#  if    (${CMAKE_C_COMPILER_VERSION} VERSION_LESS "18.0.21005.1")
#    message(FATAL_ERROR "Visual Studio earlier than VS2013 is not supported")
#  elseif(${CMAKE_C_COMPILER_VERSION} VERSION_EQUAL "18.0.21005.1")
#    # VS2013
#    message(SEND_ERROR "Update 4 for Visual Studio 2013 is required")
#  elseif(${CMAKE_C_COMPILER_VERSION} VERSION_EQUAL "18.00.30501")
#    # VS2013 Update 2:
#    message(SEND_ERROR "Update 4 for Visual Studio 2013 is required")
#  elseif(${CMAKE_C_COMPILER_VERSION} VERSION_EQUAL "18.00.30723")
#    message(WARNING "Update 4 for Visual Studio 2013 is strongly recommended")
#  elseif(${CMAKE_C_COMPILER_VERSION} VERSION_EQUAL "18.00.31101")
#    # VS2013 Update 4: no-op
#  elseif(${CMAKE_C_COMPILER_VERSION} VERSION_EQUAL "18.00.40629")
#    # VS2013 Update 5: no-op
#  endif()
#
#  if ("${CMAKE_GENERATOR}" STREQUAL "Visual Studio 12 2013 Win64")
#    set(CMAKE_GENERATOR_TOOLSET "v120</PlatformToolset><PreferredToolArchitecture>x64</PreferredToolArchitecture><PlatformToolset>v120")
#  elseif ("${CMAKE_GENERATOR}" STREQUAL "Visual Studio 14 2015 Win64")
#    set(CMAKE_GENERATOR_TOOLSET "v140</PlatformToolset><PreferredToolArchitecture>x64</PreferredToolArchitecture><PlatformToolset>v140")
#  else()
#    message(STATUS "This version of Visual Studio will not use the 64 bit linker. Build times will likely suffer.")
#  endif()
#
#  # Build with Multiple Processes
#  set(BUILD_WITH_MULTIPLE_PROCESSES ON CACHE BOOL "/MP compiler flag for full processor utilization")
#  mark_as_advanced(BUILD_WITH_MULTIPLE_PROCESSES)
#endif()


# Apple Sanity Check
if(APPLE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c11 -std=c++11 -stdlib=libc++")
  #if(NOT CMAKE_OSX_DEPLOYMENT_TARGET)
  #  message(FATAL_ERROR "OSX Deployment Target Must Be Set")
  #endif()

  #set(CMAKE_OSX_ARCHITECTURES "x86_64" CACHE STRING "Arch" FORCE)

  #if(NOT CMAKE_OSX_SYSROOT)
  #  message(FATAL_ERROR "OSX Sysroot must be set")
  #elseif(CMAKE_OSX_SYSROOT MATCHES ".*10\\.9.*")
  #  set(OSX_SDK "10.9")
  #  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c11 -std=c++11 -stdlib=libc++")
  #elseif(CMAKE_OSX_SYSROOT MATCHES ".*10\\.10.*")
  #  set(OSX_SDK "10.10")
  #  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c11 -std=c++11 -stdlib=libc++")
  #elseif(CMAKE_OSX_SYSROOT MATCHES ".*10\\.11.*")
  #  set(OSX_SDK "10.11")
  #  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c11 -std=c++11 -stdlib=libc++")
  #else()
  #  message(FATAL_ERROR "OSX Sysroot must use the 10.9, 10.10, or 10.11 SDK")
  #endif()

endif()


if(BUILD_V8_BINDINGS)
  if(WIN32)
    message(SEND_ERROR "V8 is only tested and supported on Unix like systems")
  endif()

  option(BUILD_NODE_MODULES "Build V8 Bindings as Node Modules" ON)

  if(BUILD_NODE_MODULES)
    # Build Node for linking to Node modules
    option(BUILD_NODE "Build Node" ON)
  else()
    option(BUILD_V8 "Build V8" ON)
  endif()

endif()

if(MSVC)
  mark_as_advanced(CMAKE_CONFIGURATION_TYPES)
endif()

# Use PCH
option(USE_PCH "Use precompiled headers" OFF)

include(ExternalProject)
include(ProcessorCount)

# set number of cores to be used when building boost and qt, idea is you will do make -jX so boost and qt will build in parallel
ProcessorCount(CPUCOUNT)
if(CPUCOUNT EQUAL 0)
  set(CPUCOUNT 1)
endif()

if(UNIX)
  if(CPUCOUNT EQUAL 1)
    set(BOOST_CPUCOUNT "1")
    set(QT_CPUCOUNT "1")
  elseif(CPUCOUNT EQUAL 2)
    set(BOOST_CPUCOUNT "1")
    set(QT_CPUCOUNT "1")
  elseif(CPUCOUNT EQUAL 4)
    set(BOOST_CPUCOUNT "1")
    set(QT_CPUCOUNT "3")
  elseif(CPUCOUNT EQUAL 8)
    set(BOOST_CPUCOUNT "2")
    set(QT_CPUCOUNT "6")
  elseif(CPUCOUNT EQUAL 12)
    set(BOOST_CPUCOUNT "3")
    set(QT_CPUCOUNT "9")
  elseif(CPUCOUNT EQUAL 16)
    set(BOOST_CPUCOUNT "4")
    set(QT_CPUCOUNT "12")
  else()
    set(BOOST_CPUCOUNT "1")
    set(QT_CPUCOUNT "1")
  endif()
endif()

mark_as_advanced(
  ProcessorCount_cmd_getconf
  ProcessorCount_cmd_sysctl
)

if(BUILD_WITH_MULTIPLE_PROCESSES)
  add_definitions(/MP)
endif()

# Build with OpenSSL support
set(BUILD_WITH_OPENSSL ON CACHE INTERNAL "Build With OpenSSL Support For SSH Connections")
if(UNIX)
  find_package(OpenSSL)
  if(NOT ${OPENSSL_FOUND})
    message(SEND_ERROR "OpenSSL could not be found, and is required for HTTPS communication")
    message(SEND_ERROR "Please install OpenSSL development libraries using your package management system (possibly libssl-dev)")
  else()
    mark_as_advanced(
      LIB_EAY_DEBUG
      LIB_EAY_RELEASE
      SSL_EAY_DEBUG
      SSL_EAY_RELEASE
    )
  endif()
elseif(WIN32)
  if(CMAKE_CL_64)
    set(OPENSSL_ARCH 64)
    set(OPENSSL_EXPECTED_HASH 3f9c88523634c7b1e27f4026e7c3f3ee)
  else()
    set(OPENSSL_ARCH 32)
    set(OPENSSL_EXPECTED_HASH fb2aa1a8ceb85f68055044a85f1dbc94)
  endif()
  set(OPENSSL_VERSION "1.0.2a")
  set(OPENSSL_PATH "OpenSSL-Win${OPENSSL_ARCH}")
  if(EXISTS "${CMAKE_BINARY_DIR}/${OPENSSL_PATH}.zip")
    file(MD5 "${CMAKE_BINARY_DIR}/${OPENSSL_PATH}.zip" OPENSSL_HASH)
  endif()
  if(NOT EXISTS "${CMAKE_BINARY_DIR}/${OPENSSL_PATH}.zip" OR NOT "${OPENSSL_HASH}" MATCHES "${OPENSSL_EXPECTED_HASH}")
    message(STATUS "Downloading OpenSSL ${OPENSSL_VERSION} (${OPENSSL_ARCH}-bit)")
    file(DOWNLOAD "http://openstudio-resources.s3.amazonaws.com/dependencies/OpenSSL-Win${OPENSSL_ARCH}-${OPENSSL_VERSION}.zip" "${CMAKE_BINARY_DIR}/${OPENSSL_PATH}.zip" INACTIVITY_TIMEOUT 120 SHOW_PROGRESS EXPECTED_MD5 ${OPENSSL_EXPECTED_HASH})
    execute_process(COMMAND ${CMAKE_COMMAND} -E remove_directory "${CMAKE_BINARY_DIR}/${OPENSSL_PATH}")
    execute_process(COMMAND ${CMAKE_COMMAND} -E tar xfz "${CMAKE_BINARY_DIR}/${OPENSSL_PATH}.zip" WORKING_DIRECTORY "${CMAKE_BINARY_DIR}")
  endif()
  set(OPENSSL_ROOT_DIR "${CMAKE_BINARY_DIR}/${OPENSSL_PATH}")
endif()

if(BUILD_NODE)
  # OSX can work with a static ruby library
  ExternalProject_Add(Node
    URL http://nodejs.org/dist/v0.10.28/node-v0.10.28.tar.gz
    CONFIGURE_COMMAND cd ${CMAKE_BINARY_DIR}/Node-prefix/src/Node && sh -c "./configure --prefix=${CMAKE_BINARY_DIR}/Node-prefix/src/Node-install"
    BUILD_COMMAND cd ${CMAKE_BINARY_DIR}/Node-prefix/src/Node && $(MAKE)
    INSTALL_COMMAND cd ${CMAKE_BINARY_DIR}/Node-prefix/src/Node && $(MAKE) install && $(MAKE) install-includes && ../Node-install/bin/npm install jasmine-node node-gyp -g && ../Node-install/bin/node ../Node-install/bin/node-gyp install
  )
  set(NODE_BIN_DIR ${CMAKE_BINARY_DIR}/Node-prefix/src/Node-install/bin/)
  set(NODE_INCLUDE_DIR "$ENV{HOME}/.node-gyp/0.10.28")
endif()

if(BUILD_V8)
  ExternalProject_Add(V8
    SVN_REPOSITORY http://v8.googlecode.com/svn/branches/3.19
    CONFIGURE_COMMAND cd ${CMAKE_BINARY_DIR}/V8-prefix/src/V8 && $(MAKE) dependencies
    BUILD_COMMAND cd ${CMAKE_BINARY_DIR}/V8-prefix/src/V8 && $(MAKE) x64
    INSTALL_COMMAND ""
  )
  set(V8_INCLUDE_DIR ${CMAKE_BINARY_DIR}/V8-prefix/src/V8/include)
endif()

# Ruby
option(BUILD_RUBY "Build Ruby" OFF)
if( BUILD_RUBY )
  if( NOT UNIX )

    ExternalProject_Add(ZLib
      URL_HASH SHA256=c3e5e9fdd5004dcb542feda5ee4f0ff0744628baf8ed2dd5d66f8ca1197cb1a1
      URL http://zlib.net/zlib-1.2.11.tar.gz
      BUILD_COMMAND "${CMAKE_COMMAND}" --build "${CMAKE_BINARY_DIR}/ZLib-prefix/src/ZLib-build" --config Release --config Debug --target INSTALL
      CMAKE_CACHE_ARGS
        -DINSTALL_BIN_DIR:STRING=${CMAKE_BINARY_DIR}/ZLib-prefix/src/ZLib-install/bin/
        -DINSTALL_INC_DIR:STRING=${CMAKE_BINARY_DIR}/ZLib-prefix/src/ZLib-install/include/
        -DINSTALL_LIB_DIR:STRING=${CMAKE_BINARY_DIR}/ZLib-prefix/src/ZLib-install/lib/
        -DINSTALL_MAN_DIR:STRING=${CMAKE_BINARY_DIR}/ZLib-prefix/src/ZLib-install/share/man/
        -DINSTALL_PKGCONFIG_DIR:STRING=${CMAKE_BINARY_DIR}/ZLib-prefix/src/ZLib-install/share/pkgconfig/
    )
  endif()

  if( UNIX )
    set(RUBY_EXT_CONFIG "")
    if (NOT Apple)
      # DLM: still missing: curses, dbm, gdbm
      # sudo apt-get -y install ruby autoconf automake libtool bison libgdbm-dev zlib1g-dev libffi-dev libssl-dev
      # sudo apt-get -y install build-essential libssl-dev libreadline-dev zlib1g-dev
      set(RUBY_EXT_CONFIG "--with-openssl-dir=/usr --with-readline-dir=/usr --with-zlib-dir=/usr")
    endif()

    # KSB: For Mac, you may need to install some things...
    # brew install homebrew/dupes/zlib
    # brew link --overwrite --force homebrew/dupes/zlib
    # brew install autoconf automake libtool
    # brew link autoconf automake libtool
    ExternalProject_Add(Ruby
      URL https://codeload.github.com/ruby/ruby/tar.gz/v2_2_4
      URL_MD5 3f0d280116092dc5278b6cff31948aad
      BUILD_IN_SOURCE 1
      PATCH_COMMAND ${PATCH_EXE} -p1 < ${CMAKE_SOURCE_DIR}/dependencies/ruby/Ruby.patch
      CONFIGURE_COMMAND autoconf && sh -c "./configure --with-static-linked-ext ${RUBY_EXT_CONFIG} --disable-install-doc --prefix=${CMAKE_BINARY_DIR}/Ruby-prefix/src/Ruby-install"
      BUILD_COMMAND $(MAKE) # cd ext/openssl && ../../ruby extconf.rb && cd ../readline && ../../ruby extconf.rb && cd ../../ && $(MAKE)
      INSTALL_COMMAND $(MAKE) install && sh -c "cp ./rbconfig.rb ${CMAKE_BINARY_DIR}/Ruby-prefix/src/Ruby-install/lib/ruby/2.2.0/." && sh -c "rm -rf ./ext/-test-" && sh -c "mkdir ${CMAKE_BINARY_DIR}/Ruby-prefix/src/Ruby-install/lib/ext" && sh -c "mkdir ${CMAKE_BINARY_DIR}/Ruby-prefix/src/Ruby-install/lib/enc" && sh -c "find ./ext -name '*.a' | xargs -I {} cp {} ${CMAKE_BINARY_DIR}/Ruby-prefix/src/Ruby-install/lib/ext/" && sh -c "find ./enc -name '*.a' | xargs -I {} cp {} ${CMAKE_BINARY_DIR}/Ruby-prefix/src/Ruby-install/lib/enc"
    )
    find_file( RUBY_CONFIG_INCLUDE_DIR config.h PATHS "${CMAKE_BINARY_DIR}/Ruby-prefix/src/Ruby-install/include/" )
    set(RUBY_LIBRARY "${CMAKE_BINARY_DIR}/Ruby-prefix/src/Ruby-install/lib/libruby-static.a")
    set(RUBY_INCLUDE_DIR "${CMAKE_BINARY_DIR}/Ruby-prefix/src/Ruby-install/include/ruby-2.2.0")
    set(RUBY_EXECUTABLE "${CMAKE_BINARY_DIR}/Ruby-prefix/src/Ruby-install/bin/ruby")
    set(RUBY_SOURCE_DIR "${CMAKE_BINARY_DIR}/Ruby-prefix/src/Ruby")
    set(RUBY_INSTALL_DIR "${CMAKE_BINARY_DIR}/Ruby-prefix/src/Ruby-install")
  else()
    configure_file("${CMAKE_SOURCE_DIR}/dependencies/ruby/build_ruby.bat.in" "${CMAKE_BINARY_DIR}/build_ruby.bat")
    # This will require bison, sed, and at least ruby 1.8 in your path
    # Please install DevKit even though the build will be done by msvc the ruby build system uses some traditionally unix tools
    ExternalProject_Add(Ruby
      DEPENDS ZLib
      URL https://codeload.github.com/ruby/ruby/tar.gz/v2_2_4
      URL_MD5 3f0d280116092dc5278b6cff31948aad
      BUILD_IN_SOURCE 1
      PATCH_COMMAND ${PATCH_EXE} -p1 < ${CMAKE_SOURCE_DIR}/dependencies/ruby/Ruby.patch
      CONFIGURE_COMMAND ""
      BUILD_COMMAND cmd /C "${CMAKE_BINARY_DIR}/build_ruby.bat"
      INSTALL_COMMAND ""
    )
    set(RUBY_INSTALL_DIR "${CMAKE_BINARY_DIR}/Ruby-prefix/src/Ruby-install")
  endif()
endif()

# OpenStudio Gems

option(BUILD_OPENSTUDIO_GEMS "Build OpenStudio Gems" OFF)
if(BUILD_OPENSTUDIO_GEMS)
  # depends on system Ruby and Git, Ruby must be same version as OpenStudio
  if (WIN32)
    file(GLOB POTENTIAL_RUBY_LOCATIONS "c:/ruby*2.2.*/bin")
    get_filename_component(POTENTIAL_TAR_LOCATIONS ${GIT_EXECUTABLE} DIRECTORY )
    list(APPEND POTENTIAL_TAR_LOCATIONS "${POTENTIAL_TAR_LOCATIONS}/../bin" "${POTENTIAL_TAR_LOCATIONS}/../usr/bin")
  endif()
  find_program(RUBY_EXE ruby HINTS ${POTENTIAL_RUBY_LOCATIONS})
  find_program(TAR_EXE tar HINTS ${POTENTIAL_TAR_LOCATIONS})
  set(OPENSTUDIO_GEMS_DIR "${CMAKE_BINARY_DIR}/openstudio-gems")
  add_custom_target(openstudio-gems ALL
                    ${RUBY_EXE} "${CMAKE_SOURCE_DIR}/dependencies/ruby/build_openstudio_gems.rb" "${OPENSTUDIO_GEMS_DIR}" "${TAR_EXE}" "2.2.4"
                    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/dependencies/ruby/"
                    DEPENDS "${CMAKE_SOURCE_DIR}/dependencies/ruby/Gemfile" "${CMAKE_SOURCE_DIR}/dependencies/ruby/build_openstudio_gems.rb")
endif()

option(BUILD_BOOST "Build Boost" OFF)
if(BUILD_BOOST)
  if(APPLE)
    set(apple-flags "-std=c11 -std=c++11 -stdlib=libc++")
    ExternalProject_Add(Boost
      URL http://openstudio-resources.s3.amazonaws.com/dependencies/boost_1_55_0.tar.gz
      URL_MD5 93780777cfbf999a600f62883bd54b17
      CONFIGURE_COMMAND cd ${CMAKE_BINARY_DIR}/Boost-prefix/src/Boost && ./bootstrap.sh
      PATCH_COMMAND ${PATCH_EXE} -p1 < ${CMAKE_SOURCE_DIR}/dependencies/Boost/xcode_51.patch && ${PATCH_EXE} -p1 < ${CMAKE_SOURCE_DIR}/dependencies/Boost/fchmodat.patch
      BUILD_COMMAND cd ${CMAKE_BINARY_DIR}/Boost-prefix/src/Boost && ./b2 toolset=clang cxxflags=${apple-flags} linkflags=${apple-flags} variant=debug,release define=BOOST_CHRONO_HEADER_ONLY --layout=tagged --with-filesystem --with-regex --with-program_options --with-system --with-thread --with-date_time --with-log --prefix=${CMAKE_BINARY_DIR}/Boost-prefix/src/Boost-install -j${BOOST_CPUCOUNT} install
      INSTALL_COMMAND ""
    )
    set(BOOST_ROOT ${CMAKE_BINARY_DIR}/Boost-prefix/src/Boost-install)
    set(BOOST_LIBRARYDIR ${CMAKE_BINARY_DIR}/Boost-prefix/src/Boost-install/lib)
  elseif(UNIX)
    ExternalProject_Add(Boost
      URL http://openstudio-resources.s3.amazonaws.com/dependencies/boost_1_55_0.tar.gz
      URL_MD5 93780777cfbf999a600f62883bd54b17
      CONFIGURE_COMMAND cd ${CMAKE_BINARY_DIR}/Boost-prefix/src/Boost && ./bootstrap.sh
      PATCH_COMMAND ${PATCH_EXE} -p1 < ${CMAKE_SOURCE_DIR}/dependencies/Boost/gcc_5_no_cxx11.patch
      BUILD_COMMAND cd ${CMAKE_BINARY_DIR}/Boost-prefix/src/Boost && ./b2 cxxflags=-fPIC link=static variant=debug,release define=BOOST_CHRONO_HEADER_ONLY --layout=tagged --with-atomic --with-filesystem --with-regex --with-program_options --with-system --with-thread --with-date_time --with-log --prefix=${CMAKE_BINARY_DIR}/Boost-prefix/src/Boost-install -j${BOOST_CPUCOUNT} install
      INSTALL_COMMAND ""
    )
    set(BOOST_ROOT ${CMAKE_BINARY_DIR}/Boost-prefix/src/Boost-install)
    set(BOOST_LIBRARYDIR ${CMAKE_BINARY_DIR}/Boost-prefix/src/Boost-install/lib)
  else()
    # The call to cmd and VS_UNICODE_OUTPUT is to fix an oddity where boost configuration complains about missing ICU when it shouldn't
    # be trying to find it

    set(B2_TOOLSET "")
    if(MSVC12)
      set(B2_TOOLSET "toolset=msvc-12.0")
    endif()

    if(CMAKE_CL_64)
      ExternalProject_Add(Boost
        URL http://openstudio-resources.s3.amazonaws.com/dependencies/boost_1_55_0.tar.gz
        URL_MD5 93780777cfbf999a600f62883bd54b17
        CONFIGURE_COMMAND cmd /C "cd ${CMAKE_BINARY_DIR}/Boost-prefix/src/Boost && bootstrap.bat"
        PATCH_COMMAND "${PATCH_EXE}" "${CMAKE_BINARY_DIR}/Boost-prefix/src/Boost/boost/archive/iterators/transform_width.hpp" "${CMAKE_SOURCE_DIR}/dependencies/Boost/transform_width.diff"
        BUILD_COMMAND cmd /C "set VS_UNICODE_OUTPUT=& cd ${CMAKE_BINARY_DIR}/Boost-prefix/src/Boost && b2 ${B2_TOOLSET} link=static address-model=64 define=BOOST_CHRONO_HEADER_ONLY --build-type=minimal --with-filesystem --with-regex --with-program_options --with-system --with-thread --with-date_time --with-log --prefix=${CMAKE_BINARY_DIR}/Boost-prefix/src/Boost-install -j${CPUCOUNT}" install
        INSTALL_COMMAND ""
      )
    else()
      ExternalProject_Add(Boost
        URL http://openstudio-resources.s3.amazonaws.com/dependencies/boost_1_55_0.tar.gz
        URL_MD5 93780777cfbf999a600f62883bd54b17
        CONFIGURE_COMMAND cmd /C "cd ${CMAKE_BINARY_DIR}/Boost-prefix/src/Boost && bootstrap.bat"
        PATCH_COMMAND "${PATCH_EXE}" "${CMAKE_BINARY_DIR}/Boost-prefix/src/Boost/boost/archive/iterators/transform_width.hpp" "${CMAKE_SOURCE_DIR}/dependencies/Boost/transform_width.diff"
        BUILD_COMMAND cmd /C "set VS_UNICODE_OUTPUT=& cd ${CMAKE_BINARY_DIR}/Boost-prefix/src/Boost && b2 ${B2_TOOLSET} link=static address-model=32 define=BOOST_CHRONO_HEADER_ONLY --build-type=minimal --with-filesystem --with-regex --with-program_options --with-system --with-thread --with-date_time --with-log --prefix=${CMAKE_BINARY_DIR}/Boost-prefix/src/Boost-install -j${CPUCOUNT}" install
        INSTALL_COMMAND ""
      )
    endif()

    set(BOOST_ROOT ${CMAKE_BINARY_DIR}/Boost-prefix/src/Boost-install)
    set(BOOST_LIBRARYDIR ${CMAKE_BINARY_DIR}/Boost-prefix/src/Boost-install/lib)
  endif()
endif()

option(BUILD_QT "Build Qt" OFF)
option(Qt5Static "Build Static Qt" OFF)

if(BUILD_QT)

  if(APPLE)
    execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/Qt-prefix/src/Qt-install")
    if( Qt5Static )
      set(QT_CONFIGURE_COMMAND "-v -release -opensource -qt-pcre -qt-harfbuzz -static -openssl -no-pch -no-ssse3 -qt-sql-sqlite -plugin-sql-sqlite -skip enginio -no-compile-examples -nomake examples -nomake tests -nomake tools -skip qtmultimedia -confirm-license -prefix ${CMAKE_BINARY_DIR}/Qt-prefix/src/Qt-static-install")
    else()
      message(WARNING "Shared Apple Qt build untested")
      set(QT_CONFIGURE_COMMAND "-v -release -opensource -qt-pcre -qt-harfbuzz -shared -openssl -no-pch -no-ssse3 -qt-sql-sqlite -plugin-sql-sqlite -skip enginio -no-compile-examples -nomake examples -nomake tests -nomake tools -skip qtmultimedia -confirm-license -prefix ${CMAKE_BINARY_DIR}/Qt-prefix/src/Qt-install")
    endif()
    ExternalProject_Add(Qt
      URL http://openstudio-resources.s3.amazonaws.com/dependencies/qt-everywhere-opensource-src-5.6.1.tar.gz
      URL_MD5 ed16ef2a30c674f91f8615678005d44c
      CONFIGURE_COMMAND sh -c "./configure ${QT_CONFIGURE_COMMAND}"
      PATCH_COMMAND ""
      BUILD_COMMAND sh -c "make -j${CPUCOUNT} && make install"
      INSTALL_COMMAND ""
      BUILD_IN_SOURCE 1
    )
    set(QT_INSTALL_DIR "${CMAKE_BINARY_DIR}/Qt-prefix/src/Qt-install/")

  elseif( UNIX )
    # install deps according to here http://doc.qt.io/qt-5/linux-requirements.html
    # also reference https://wiki.qt.io/Building_Qt_5_from_Git
    # need to apply the workaround at https://bugreports.qt.io/browse/QTBUG-52605 manually
    if( Qt5Static )
      set(QT_CONFIGURE_COMMAND "-v -release -opensource -static -openssl -no-pch -no-ssse3 -qt-sql-sqlite -plugin-sql-sqlite -skip enginio -no-compile-examples -nomake examples -nomake tests -nomake tools -skip qtmultimedia -qt-xcb -confirm-license  -no-qml-debug -prefix ${CMAKE_BINARY_DIR}/Qt-prefix/src/Qt-static-install")
    else()
      message(WARNING "Shared Unix Qt build untested")
      set(QT_CONFIGURE_COMMAND "-v -release -opensource -shared -openssl -no-pch -no-ssse3 -qt-sql-sqlite -plugin-sql-sqlite -skip enginio -no-compile-examples -nomake examples -nomake tests -nomake tools -skip qtmultimedia -qt-xcb -confirm-license  -no-qml-debug -prefix ${CMAKE_BINARY_DIR}/Qt-prefix/src/Qt-install")
    endif()
    ExternalProject_Add(Qt
      URL http://openstudio-resources.s3.amazonaws.com/dependencies/qt-everywhere-opensource-src-5.6.1.tar.gz
      URL_MD5 ed16ef2a30c674f91f8615678005d44c
      CONFIGURE_COMMAND sh -c "./configure ${QT_CONFIGURE_COMMAND}"
      PATCH_COMMAND ${PATCH_EXE} -p1 < ${CMAKE_SOURCE_DIR}/dependencies/qt/qstatic_version_tag.patch
      BUILD_COMMAND sh -c "make -j${CPUCOUNT} && make install"
      INSTALL_COMMAND ""
      BUILD_IN_SOURCE 1
    )
    set(QT_INSTALL_DIR "${CMAKE_BINARY_DIR}/Qt-prefix/src/Qt-install/")

  else()
    # to initialize command prompt run: "C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\vcvarsall.bat" amd64
    configure_file("${CMAKE_SOURCE_DIR}/dependencies/qt/build_static_qt.bat.in" "${CMAKE_BINARY_DIR}/build_static_qt.bat")
    configure_file("${CMAKE_SOURCE_DIR}/dependencies/qt/build_shared_qt.bat.in" "${CMAKE_BINARY_DIR}/build_shared_qt.bat")
    if( Qt5Static )
      set(QT_BUILD_SCRIPT "build_static_qt.bat")
    else()
      set(QT_BUILD_SCRIPT "build_shared_qt.bat")
    endif()
    ExternalProject_Add(Qt
      URL http://openstudio-resources.s3.amazonaws.com/dependencies/qt-everywhere-opensource-src-5.6.1.zip
      URL_MD5 c18a4904d9773894437c10c25f93e50f
      BUILD_IN_SOURCE 1
      CONFIGURE_COMMAND ""
      PATCH_COMMAND ""
      BUILD_COMMAND cmd /C "${CMAKE_BINARY_DIR}/${QT_BUILD_SCRIPT}"
      INSTALL_COMMAND ""
    )
    set(QT_INSTALL_DIR "${CMAKE_BINARY_DIR}/Qt-prefix/src/Qt-install/")
  endif()

else()
  set(QT_INSTALL_DIR "")
endif()

option(BUILD_DVIEW "Build DView" OFF)
if( BUILD_DVIEW )
    ExternalProject_Add(curl
      URL https://curl.haxx.se/download/curl-7.55.1.tar.gz
      CONFIGURE_COMMAND ""
      BUILD_COMMAND ""
      INSTALL_COMMAND ""
    )

    ExternalProject_Add(lk
      GIT_REPOSITORY git://github.com/NREL/lk
      GIT_TAG develop
      CONFIGURE_COMMAND ""
      BUILD_COMMAND ""
      INSTALL_COMMAND ""
    )

    ExternalProject_Add(DView
      DEPENDS lk curl
      GIT_REPOSITORY https://github.com/kbenne/wex.git
      GIT_TAG develop
      CMAKE_CACHE_ARGS
        -DCMAKE_OSX_ARCHITECTURES:STRING=${CMAKE_OSX_ARCHITECTURES}
        -DCMAKE_OSX_DEPLOYMENT_TARGET:STRING=${CMAKE_OSX_DEPLOYMENT_TARGET}
        -DCMAKE_OSX_SYSROOT:STRING=${CMAKE_OSX_SYSROOT}
        -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
        -DLK_DIR:DIR=${CMAKE_BINARY_DIR}/lk-prefix/src/lk/
        -DCURL_DIR:DIR=${CMAKE_BINARY_DIR}/curl-prefix/src/curl/
        -DCMAKE_INSTALL_PREFIX:DIR=${CMAKE_BINARY_DIR}/DView-prefix/src/DView-install
        -DwxWidgets_EXCLUDE_COMMON_LIBRARIES:BOOL=false
        -DwxWidgets_USE_STATIC:BOOL=true
    )
endif()

set(OpenStudioCore_DIR ${CMAKE_BINARY_DIR}/OSCore-prefix/src/OSCore-build)

# Set up the dependencies for OpenStudioCore

if(BUILD_BOOST)
  list(APPEND OpenStudioCore_DEPENDS Boost)
endif()

if(BUILD_RUBY)
  list(APPEND OpenStudioCore_DEPENDS Ruby)
endif()

if(BUILD_NODE)
  list(APPEND OpenStudioCore_DEPENDS Node)
endif()

if(BUILD_V8)
  list(APPEND OpenStudioCore_DEPENDS V8)
endif()

if(MSVC)
  set(OS_BUILD_COMMAND BUILD_COMMAND "${CMAKE_COMMAND}" --build "${CMAKE_BINARY_DIR}/OSCore-prefix/src/OSCore-build" --config $(Configuration) -- /maxcpucount)
else()
  set(OS_BUILD_COMMAND "")
endif()

ExternalProject_Add(OSCore
  DEPENDS ${OpenStudioCore_DEPENDS}
  SOURCE_DIR ${CMAKE_SOURCE_DIR}/openstudiocore
  ${OS_BUILD_COMMAND}
  CMAKE_CACHE_ARGS
    -DCMAKE_OSX_ARCHITECTURES:STRING=${CMAKE_OSX_ARCHITECTURES}
    -DCMAKE_OSX_DEPLOYMENT_TARGET:STRING=${CMAKE_OSX_DEPLOYMENT_TARGET}
    -DCMAKE_OSX_SYSROOT:STRING=${CMAKE_OSX_SYSROOT}
    -DBUILD_DOCUMENTATION:BOOL=${BUILD_DOCUMENTATION}
    -DBUILD_CSHARP_BINDINGS:BOOL=${BUILD_CSHARP_BINDINGS}
    -DBUILD_JAVA_BINDINGS:BOOL=${BUILD_JAVA_BINDINGS}
    -DBUILD_V8_BINDINGS:BOOL=${BUILD_V8_BINDINGS}
    -DBUILD_NODE_MODULES:BOOL=${BUILD_NODE_MODULES}
    -DBUILD_PYTHON_BINDINGS:BOOL=${BUILD_PYTHON_BINDINGS}
    -DBUILD_TESTING:BOOL=${BUILD_TESTING}
    -DBUILD_PACKAGE:BOOL=${BUILD_PACKAGE}
    -DBUILD_PAT:BOOL=${BUILD_PAT}
    -DBUILD_WITH_MULTIPLE_PROCESSES:BOOL=${BUILD_WITH_MULTIPLE_PROCESSES}
    -DUSE_PCH:BOOL=${USE_PCH}
    -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
    -DCMAKE_CXX_COMPILER:STRING=${CMAKE_CXX_COMPILER}
    -DCMAKE_CXX_FLAGS:STRING=${CMAKE_CXX_FLAGS}
    -DCMAKE_C_FLAGS:STRING=${CMAKE_C_FLAGS}
    -DCMAKE_EXE_LINKER_FLAGS:STRING=${CMAKE_EXE_LINKER_FLAGS}
    -DCMAKE_MODULE_LINKER_FLAGS:STRING=${CMAKE_MODULE_LINKER_FLAGS}
    -DCMAKE_SHARED_LINKER_FLAGS:STRING=${CMAKE_SHARED_LINKER_FLAGS}
    -DCMAKE_INSTALL_PREFIX:STRING=${CMAKE_INSTALL_PREFIX}
    -DCMAKE_PREFIX_PATH:STRING=${CMAKE_PREFIX_PATH}
    -DBOOST_ROOT:STRING=${BOOST_ROOT}
    -DBOOST_LIBRARYDIR:STRING=${BOOST_LIBRARYDIR}
    -DQT_INSTALL_DIR:PATH=${QT_INSTALL_DIR}
    -DBUILD_SHARED_LIBS:BOOL=${BUILD_SHARED_LIBS}
    -DNODE_BIN_DIR:STRING=${NODE_BIN_DIR}
    -DNODE_INCLUDE_DIR:STRING=${NODE_INCLUDE_DIR}
    -DV8_INCLUDE_DIR:STRING=${V8_INCLUDE_DIR}
    -DDOXYGEN_EXECUTABLE:STRING=${DOXYGEN_EXECUTABLE}
    -DDOXYGEN_DOT_EXECUTABLE:STRING=${DOXYGEN_DOT_EXECUTABLE}
    -DMAXIMIZE_CPU_USAGE:BOOL=${MAXIMIZE_CPU_USAGE}
    -DRUBY_INSTALL_DIR:PATH=${RUBY_INSTALL_DIR}
    INSTALL_COMMAND ""
)

#if(BUILD_QT AND MSVC)
#  file(GLOB dlls ${CMAKE_BINARY_DIR}/Qt-prefix/src/Qt-install/bin/*.dll)
#  foreach(dll ${dlls})
#    get_filename_component(filename "${dll}" NAME_WE)
#    ExternalProject_Add_Step(OSCore "Install${filename}ReleaseDLL"
#      COMMAND ${CMAKE_COMMAND} -E copy_if_different "${dll}" "${CMAKE_BINARY_DIR}/OSCore-prefix/src/OSCore-build/Products/Release"
#      DEPENDERS configure
#    )
#    ExternalProject_Add_Step(OSCore "Install${filename}DebugDLL"
#      COMMAND ${CMAKE_COMMAND} -E copy_if_different "${dll}" "${CMAKE_BINARY_DIR}/OSCore-prefix/src/OSCore-build/Products/Debug"
#      DEPENDERS configure
#    )
#    ExternalProject_Add_Step(OSCore "Install${filename}RelWithDebInfoDLL"
#      COMMAND ${CMAKE_COMMAND} -E copy_if_different "${dll}" "${CMAKE_BINARY_DIR}/OSCore-prefix/src/OSCore-build/Products/RelWithDebInfo"
#      DEPENDERS configure
#    )
#    ExternalProject_Add_Step(OSCore "Install${filename}MinSizeRelDLL"
#      COMMAND ${CMAKE_COMMAND} -E copy_if_different "${dll}" "${CMAKE_BINARY_DIR}/OSCore-prefix/src/OSCore-build/Products/MinSizeRel"
#      DEPENDERS configure
#    )
#  endforeach()
#endif()

#if(MSVC)
#  ExternalProject_Add_Step(OSCore MakeRubyReleaseFolder
#    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/OSCore-prefix/src/OSCore-build/Products/Release
#    DEPENDERS configure
#  )
#  ExternalProject_Add_Step(OSCore MakeRubyDebugFolder
#    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/OSCore-prefix/src/OSCore-build/Products/Debug
#    DEPENDERS configure
#  )
#  ExternalProject_Add_Step(OSCore MakeRubyRelWithDebInfoFolder
#    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/OSCore-prefix/src/OSCore-build/Products/RelWithDebInfo
#    DEPENDERS configure
#  )
#  ExternalProject_Add_Step(OSCore MakeRubyMinSizeRelFolder
#    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/OSCore-prefix/src/OSCore-build/Products/MinSizeRel
#    DEPENDERS configure
#  )
#
#
#  ExternalProject_Add_Step(OSCore MakeRubyReleaseDLL
#    COMMAND ${CMAKE_COMMAND} -E copy "${RUBY_DLLPATH}" "${CMAKE_BINARY_DIR}/OSCore-prefix/src/OSCore-build/Products/Release/${RUBY_DLL}"
#    DEPENDERS configure
#  )
#  ExternalProject_Add_Step(OSCore MakeRubyDebugDLL
#    COMMAND ${CMAKE_COMMAND} -E copy "${RUBY_DLLPATH}" "${CMAKE_BINARY_DIR}/OSCore-prefix/src/OSCore-build/Products/Debug/${RUBY_DLL}"
#    DEPENDERS configure
#  )
#  ExternalProject_Add_Step(OSCore MakeRubyRelWithDebInfoDLL
#    COMMAND ${CMAKE_COMMAND} -E copy "${RUBY_DLLPATH}" "${CMAKE_BINARY_DIR}/OSCore-prefix/src/OSCore-build/Products/RelWithDebInfo/${RUBY_DLL}"
#    DEPENDERS configure
#  )
#  ExternalProject_Add_Step(OSCore MakeRubyMinSizeRelDLL
#    COMMAND ${CMAKE_COMMAND} -E copy "${RUBY_DLLPATH}" "${CMAKE_BINARY_DIR}/OSCore-prefix/src/OSCore-build/Products/MinSizeRel/${RUBY_DLL}"
#    DEPENDERS configure
#  )
#
#endif()


set(OPENSTUDIOCORE_BUILD_DIR ${CMAKE_BINARY_DIR}/OSCore-prefix/src/OSCore-build)
set(OPENSTUDIOCORE_LIB_DIR ${CMAKE_BINARY_DIR}/OSCore-prefix/src/OSCore-build/Products)
set(OPENSTUDIOCORE_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/openstudiocore/src)
set(OPENSTUDIOCORE_ROOT_DIR ${CMAKE_SOURCE_DIR}/openstudiocore/)

###############################################################################
# Use CPack
if(BUILD_PACKAGE)
  include(OpenStudioCPack.cmake)
endif()
###############################################################################


if(BUILD_REGRESSIONS)
  ExternalProject_Add(Regressions
    DEPENDS OSCore
    GIT_REPOSITORY git://github.com/NREL/OpenStudio-resources
    GIT_TAG develop
    CMAKE_CACHE_ARGS
      -DOPENSTUDIO_BUILD_DIR:PATH=${CMAKE_BINARY_DIR}
      -DQT_INSTALL_DIR:PATH=${QT_INSTALL_DIR}
    INSTALL_COMMAND ""
  )
endif()
