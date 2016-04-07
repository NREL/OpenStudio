if(CMAKE_HOST_WIN32)
  # Necessary to do with FindOpenSSL on Windows
  cmake_minimum_required(VERSION 3.2.3)
else()
  cmake_minimum_required(VERSION 2.8.11)
endif()


project(OpenStudio)

set(CMAKE_VERSION_MAJOR 1)
set(CMAKE_VERSION_MINOR 11)
set(CMAKE_VERSION_PATCH 1)

set(CMAKE_VERSION_BUILD "Unknown" CACHE STRING "Build number")
find_package(Git)

if(NOT GIT_FOUND)
  find_program(GIT_EXECUTABLE git HINTS "$ENV{LOCALAPPDATA}/Programs/git/bin")
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

# EnergyPlus Idd version
set(ENERGYPLUS_VERSION_MAJOR 8)
set(ENERGYPLUS_VERSION_MINOR 5)
set(ENERGYPLUS_VERSION_PATCH 0)
set(ENERGYPLUS_VERSION "${ENERGYPLUS_VERSION_MAJOR}.${ENERGYPLUS_VERSION_MINOR}.${ENERGYPLUS_VERSION_PATCH}")
# Build SHA is not required to have a value, but if it does OpenStudio will require this build.
set(ENERGYPLUS_BUILD_SHA "c87e61b44b")

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

# Build test runner targets.
# This is a convenience for Visual Studio users
option(ENABLE_TEST_RUNNER_TARGETS "Create test runner targets" OFF)

set(MAXIMIZE_CPU_USAGE OFF CACHE BOOL "Attempt to fully load the CPU during builds")
mark_as_advanced(MAXIMIZE_CPU_USAGE)

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
    file(DOWNLOAD "http://openstudio-resources.s3.amazonaws.com/dependencies/OpenSSL-Win${OPENSSL_ARCH}-${OPENSSL_VERSION}.zip" "${CMAKE_BINARY_DIR}/${OPENSSL_PATH}.zip" TIMEOUT 120 INACTIVITY_TIMEOUT 120 SHOW_PROGRESS EXPECTED_MD5 ${OPENSSL_EXPECTED_HASH})
    execute_process(COMMAND ${CMAKE_COMMAND} -E remove_directory "${CMAKE_BINARY_DIR}/${OPENSSL_PATH}")
    execute_process(COMMAND ${CMAKE_COMMAND} -E tar xfz "${CMAKE_BINARY_DIR}/${OPENSSL_PATH}.zip" WORKING_DIRECTORY "${CMAKE_BINARY_DIR}")
  endif()
  set(OPENSSL_ROOT_DIR "${CMAKE_BINARY_DIR}/${OPENSSL_PATH}")
endif()

if(MSVC)
  # Visual Studio compiler check
  # http://en.wikipedia.org/wiki/Visual_C%2B%2B#32-bit_and_64-bit_versions
  if    (${CMAKE_C_COMPILER_VERSION} VERSION_LESS "18.0.21005.1")
    message(FATAL_ERROR "Visual Studio earlier than VS2013 is not supported")
  elseif(${CMAKE_C_COMPILER_VERSION} VERSION_EQUAL "18.0.21005.1")
    # VS2013
    message(SEND_ERROR "Update 4 for Visual Studio 2013 is required")
  elseif(${CMAKE_C_COMPILER_VERSION} VERSION_EQUAL "18.00.30501")
    # VS2013 Update 2:
    message(SEND_ERROR "Update 4 for Visual Studio 2013 is required")
  elseif(${CMAKE_C_COMPILER_VERSION} VERSION_EQUAL "18.00.30723")
    message(WARNING "Update 4 for Visual Studio 2013 is strongly recommended")
  elseif(${CMAKE_C_COMPILER_VERSION} VERSION_EQUAL "18.00.31101")
    # VS2013 Update 4: no-op
  elseif(${CMAKE_C_COMPILER_VERSION} VERSION_EQUAL "18.00.40629")
    # VS2013 Update 5: no-op
  endif()

  # Build with Multiple Processes
  set(BUILD_WITH_MULTIPLE_PROCESSES ON CACHE BOOL "/MP compiler flag for full processor utilization")
  mark_as_advanced(BUILD_WITH_MULTIPLE_PROCESSES)
endif()


# Apple Sanity Check
if(APPLE)
  if(NOT CMAKE_OSX_DEPLOYMENT_TARGET)
    message(FATAL_ERROR "OSX Deployment Target Must Be Set")
  endif()

  set(CMAKE_OSX_ARCHITECTURES "x86_64" CACHE STRING "Arch" FORCE)

  if(NOT CMAKE_OSX_SYSROOT)
    message(FATAL_ERROR "OSX Sysroot must be set")
  elseif(CMAKE_OSX_SYSROOT MATCHES ".*10\\.9.*")
    set(OSX_SDK "10.9")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c11 -std=c++11 -stdlib=libc++")
  elseif(CMAKE_OSX_SYSROOT MATCHES ".*10\\.10.*")
    set(OSX_SDK "10.10")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c11 -std=c++11 -stdlib=libc++")
  elseif(CMAKE_OSX_SYSROOT MATCHES ".*10\\.11.*")
    set(OSX_SDK "10.11")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c11 -std=c++11 -stdlib=libc++")
  else()
    message(FATAL_ERROR "OSX Sysroot must use the 10.9, 10.10, or 10.11 SDK")
  endif()

endif()

# EnergyPlus
if(UNIX)
  if(APPLE)
    set(ENERGYPLUS_EXPECTED_HASH 01c4232f7136e42995c344c389d96a61)
    set(ENERGYPLUS_PLATFORM "Darwin-x86_64")
  elseif(EXISTS "/etc/redhat-release")
    #message(FATAL_ERROR "EnergyPlus ${ENERGYPLUS_VERSION} redhat build missing")
    set(ENERGYPLUS_EXPECTED_HASH ea8cb9e9efc622a85d82fe2306281420)
    set(ENERGYPLUS_PLATFORM "Redhat-x86_64")
    #message(FATAL_ERROR "EnergyPlus ${ENERGYPLUS_PLATFORM} redhat build missing")
  else()
    set(ENERGYPLUS_EXPECTED_HASH 0053d9b145f9c3485bb49f85ddb61f6c)
    set(ENERGYPLUS_PLATFORM "Linux-x86_64")
  endif()
  set(ENERGYPLUS_PATH "EnergyPlus-${ENERGYPLUS_VERSION}-${ENERGYPLUS_BUILD_SHA}-${ENERGYPLUS_PLATFORM}")
  if(EXISTS "${CMAKE_BINARY_DIR}/${ENERGYPLUS_PATH}.tar.bz2")
    file(MD5 "${CMAKE_BINARY_DIR}/${ENERGYPLUS_PATH}.tar.bz2" ENERGYPLUS_HASH)
  endif()
  if(NOT EXISTS "${CMAKE_BINARY_DIR}/${ENERGYPLUS_PATH}.tar.bz2" OR NOT "${ENERGYPLUS_HASH}" MATCHES "${ENERGYPLUS_EXPECTED_HASH}")
    message(STATUS "Downloading EnergyPlus ${ENERGYPLUS_VERSION} (${ENERGYPLUS_PLATFORM})")
    file(DOWNLOAD "http://openstudio-resources.s3.amazonaws.com/dependencies/${ENERGYPLUS_PATH}.tar.bz2" "${CMAKE_BINARY_DIR}/${ENERGYPLUS_PATH}.tar.bz2" TIMEOUT 320 INACTIVITY_TIMEOUT 120 SHOW_PROGRESS EXPECTED_MD5 ${ENERGYPLUS_EXPECTED_HASH})
    execute_process(COMMAND ${CMAKE_COMMAND} -E remove_directory "${CMAKE_BINARY_DIR}/${ENERGYPLUS_PATH}")
    execute_process(COMMAND ${CMAKE_COMMAND} -E tar xfz "${CMAKE_BINARY_DIR}/${ENERGYPLUS_PATH}.tar.bz2" WORKING_DIRECTORY "${CMAKE_BINARY_DIR}")
  endif()
  set(ENV{ENERGYPLUSDIR} "${CMAKE_BINARY_DIR}/${ENERGYPLUS_PATH}/EnergyPlus-${ENERGYPLUS_VERSION_MAJOR}-${ENERGYPLUS_VERSION_MINOR}-${ENERGYPLUS_VERSION_PATCH}")
elseif(WIN32)
  if(CMAKE_CL_64)
    set(ENERGYPLUS_PATH "EnergyPlus-${ENERGYPLUS_VERSION}-${ENERGYPLUS_BUILD_SHA}-Windows-x86_64")
    set(ENERGYPLUS_ARCH 64)
    set(ENERGYPLUS_EXPECTED_HASH ca4d9e956015453948a40a5010c8dbbb)
  else()
    set(ENERGYPLUS_PATH "EnergyPlus-${ENERGYPLUS_VERSION}-${ENERGYPLUS_BUILD_SHA}-Windows-i386")
    set(ENERGYPLUS_ARCH 32)
    set(ENERGYPLUS_EXPECTED_HASH 5ecaaaa815f9867cf10a1decbfc59aa1)
  endif()
  if(EXISTS "${CMAKE_BINARY_DIR}/${ENERGYPLUS_PATH}.zip")
    file(MD5 "${CMAKE_BINARY_DIR}/${ENERGYPLUS_PATH}.zip" ENERGYPLUS_HASH)
  endif()
  if(NOT EXISTS "${CMAKE_BINARY_DIR}/${ENERGYPLUS_PATH}.zip" OR NOT "${ENERGYPLUS_HASH}" MATCHES "${ENERGYPLUS_EXPECTED_HASH}")
    message(STATUS "Downloading EnergyPlus ${ENERGYPLUS_VERSION} (${ENERGYPLUS_ARCH}-bit)")
    file(DOWNLOAD "http://openstudio-resources.s3.amazonaws.com/dependencies/${ENERGYPLUS_PATH}.zip" "${CMAKE_BINARY_DIR}/${ENERGYPLUS_PATH}.zip" TIMEOUT 320 INACTIVITY_TIMEOUT 120 SHOW_PROGRESS EXPECTED_MD5 ${ENERGYPLUS_EXPECTED_HASH})
    execute_process(COMMAND ${CMAKE_COMMAND} -E remove_directory "${CMAKE_BINARY_DIR}/${ENERGYPLUS_PATH}")
    execute_process(COMMAND ${CMAKE_COMMAND} -E tar xfz "${CMAKE_BINARY_DIR}/${ENERGYPLUS_PATH}.zip" WORKING_DIRECTORY "${CMAKE_BINARY_DIR}")
  endif()
  set(ENV{ENERGYPLUSDIR} "${CMAKE_BINARY_DIR}/${ENERGYPLUS_PATH}/EnergyPlus-${ENERGYPLUS_VERSION_MAJOR}-${ENERGYPLUS_VERSION_MINOR}-${ENERGYPLUS_VERSION_PATCH}")

endif()
find_package(EnergyPlus "${ENERGYPLUS_VERSION}" REQUIRED)

# Radiance
set(RADIANCE_VERSION "5.0.a.8")
if(UNIX)
  if(APPLE)
    set(RADIANCE_EXPECTED_HASH c3e94e19d13c60e556c0005ec0bf723f)
    set(RADIANCE_PLATFORM "Darwin")
  elseif(EXISTS "/etc/redhat-release")
    #message(FATAL_ERROR "Radiance ${RADIANCE_VERSION} redhat build missing")
    set(RADIANCE_EXPECTED_HASH f43fcdd76f45654daef69f38cd1f58a6)
    set(RADIANCE_PLATFORM "Redhat-x86_64")
  else()
    set(RADIANCE_EXPECTED_HASH 886ac1764a70f2f7b211231e091d32bf)
    set(RADIANCE_PLATFORM "Linux-x86_64")
  endif()
  set(RADIANCE_PATH "Radiance-${RADIANCE_VERSION}-${RADIANCE_PLATFORM}")
  if(EXISTS "${CMAKE_BINARY_DIR}/${RADIANCE_PATH}.tar.bz2")
    file(MD5 "${CMAKE_BINARY_DIR}/${RADIANCE_PATH}.tar.bz2" RADIANCE_HASH)
  endif()
  if(NOT EXISTS "${CMAKE_BINARY_DIR}/${RADIANCE_PATH}.tar.bz2" OR NOT "${RADIANCE_HASH}" MATCHES "${RADIANCE_EXPECTED_HASH}")
    message(STATUS "Downloading Radiance ${RADIANCE_VERSION} (${RADIANCE_PLATFORM})")
    file(DOWNLOAD "http://openstudio-resources.s3.amazonaws.com/dependencies/Radiance-${RADIANCE_VERSION}-${RADIANCE_PLATFORM}.tar.bz2" "${CMAKE_BINARY_DIR}/${RADIANCE_PATH}.tar.bz2" TIMEOUT 120 INACTIVITY_TIMEOUT 120 SHOW_PROGRESS EXPECTED_MD5 ${RADIANCE_EXPECTED_HASH})
    execute_process(COMMAND ${CMAKE_COMMAND} -E remove_directory "${CMAKE_BINARY_DIR}/${RADIANCE_PATH}")
    execute_process(COMMAND ${CMAKE_COMMAND} -E tar xfz "${CMAKE_BINARY_DIR}/${RADIANCE_PATH}.tar.bz2" WORKING_DIRECTORY "${CMAKE_BINARY_DIR}")
  endif()
  set(RADIANCE_LOCATION "${CMAKE_BINARY_DIR}/radiance")
elseif(WIN32)
  if(CMAKE_CL_64)
    set(RADIANCE_ARCH 64)
    set(RADIANCE_EXPECTED_HASH 30ea31d1eff986bfd1744e4260c06fb2)
  else()
    set(RADIANCE_ARCH 32)
    set(RADIANCE_EXPECTED_HASH 78eda5d4b0215cd675cc3ec8975e6122)
  endif()
  set(RADIANCE_PATH "Radiance-${RADIANCE_VERSION}-win${RADIANCE_ARCH}")
  if(EXISTS "${CMAKE_BINARY_DIR}/${RADIANCE_PATH}.zip")
    file(MD5 "${CMAKE_BINARY_DIR}/${RADIANCE_PATH}.zip" RADIANCE_HASH)
  endif()
  if(NOT EXISTS "${CMAKE_BINARY_DIR}/${RADIANCE_PATH}.zip" OR NOT "${RADIANCE_HASH}" MATCHES "${RADIANCE_EXPECTED_HASH}")
    message(STATUS "Downloading Radiance ${RADIANCE_VERSION} (${RADIANCE_ARCH}-bit)")
    file(DOWNLOAD "http://openstudio-resources.s3.amazonaws.com/dependencies/Radiance-${RADIANCE_VERSION}-win${RADIANCE_ARCH}.zip" "${CMAKE_BINARY_DIR}/${RADIANCE_PATH}.zip" TIMEOUT 120 INACTIVITY_TIMEOUT 120 SHOW_PROGRESS EXPECTED_MD5 ${RADIANCE_EXPECTED_HASH})
    execute_process(COMMAND ${CMAKE_COMMAND} -E remove_directory "${CMAKE_BINARY_DIR}/${RADIANCE_PATH}")
    execute_process(COMMAND ${CMAKE_COMMAND} -E tar xfz "${CMAKE_BINARY_DIR}/${RADIANCE_PATH}.zip" WORKING_DIRECTORY "${CMAKE_BINARY_DIR}")
  endif()
  set(RADIANCE_LOCATION "${CMAKE_BINARY_DIR}/Radiance")
endif()

# Build Ruby gem
option(BUILD_RUBY_GEM "Build Ruby Gem" OFF)

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

if(BUILD_RUBY_GEM)
  # Build Ruby gem and package up all the libs
  option(BUILD_RUBY_GEM_WITH_LIBS "Package all required libs with the Ruby Gem" ON)

  set(BUILD_PACKAGE ON)
  set(CPACK_BINARY_DEB OFF)
  set(CPACK_BINARY_NSIS OFF)
  set(CPACK_BINARY_RPM OFF)
  set(CPACK_BINARY_STGZ OFF)
  set(CPACK_BINARY_TBZ2 OFF)
  set(CPACK_BINARY_TZ OFF)
  set(CPACK_BINARY_TGZ ON)

  if(CPACK_BINARY_DEB OR CPACK_BINARY_NSIS OR CPACK_BINARY_RPM OR CPACK_BINARY_STGZ
      OR CPACK_BINARY_TBZ2 OR CPACK_BINARY_TZ OR NOT CPACK_BINARY_TGZ OR NOT BUILD_PACKAGE)
    message(FATAL_ERROR "To build the Ruby Gem please enable ONLY CPACK_BINARY_TGZ of the cpack binary options")
  endif()

  message("Enabling Ruby Gem overrides all of your packaging options. To build the Ruby Gem please type 'make package'")
endif()

if(MSVC)
  mark_as_advanced(CMAKE_CONFIGURATION_TYPES)
endif()

# Use PCH
option(USE_PCH "Use precompiled headers" OFF)


# Smart find qt location, looking for 64bit when appropriate
# and preferring opengl installations
if(MSVC)
  if(CMAKE_CL_64)
    set(COMPILER_ARCH_TAG "_64")
  else()
    set(COMPILER_ARCH_TAG "")
  endif()

  if(MSVC_VERSION STREQUAL "1800")
    set(COMPILER_VER_TAG "2013")
  else()
    set(COMPILER_VER_TAG "2014")
  endif()

  file(GLOB_RECURSE QMAKE_LOCATIONS "C:/Qt/qmake.exe")

  message(STATUS "Qt Locations: ${QMAKE_LOCATIONS}")

  if(QMAKE_LOCATIONS MATCHES ".*msvc${COMPILER_VER_TAG}${COMPILER_ARCH_TAG}_opengl.*")
    set(HAS_OPENGL_INSTALL TRUE)
    set(OPENGL_TAG "_opengl")
  else()
    set(HAS_OPENGL_INSTALL FALSE)
    set(OPENGL_TAG "")
  endif()

  set(QT_HINT_DIR "")
  foreach(LOC ${QMAKE_LOCATIONS})
    if(LOC MATCHES ".*msvc${COMPILER_VER_TAG}${COMPILER_ARCH_TAG}${OPENGL_TAG}/")
      get_filename_component(PDIR "${LOC}" DIRECTORY)
      get_filename_component(GPDIR "${PDIR}" DIRECTORY)
      if(HAS_OPENGL_INSTALL)
        if(GPDIR MATCHES ".*opengl.*")
          list(APPEND QT_HINT_DIR "${GPDIR}")
        endif()
      else()
        list(APPEND QT_HINT_DIR "${GPDIR}")
      endif()
    endif()
  endforeach()

  # we want sort, this will do both a lexicographical sort of the version numbers, putting the newest last and picking the
  # longest (aka opengl) last. Then we sort it in reverse to prefer the highest version numbers with opengl
  list(SORT QT_HINT_DIR)
  list(REVERSE QT_HINT_DIR)
else()
  set(QT_HINT_DIR "")
endif()

# set CMAKE_PREFIX_PATH to specify specific Qt version
find_package(Qt5Widgets HINTS ${QT_HINT_DIR}) #Qt5Core, Qt5Gui, Qt5Widgets
find_package(Qt5Sql HINTS ${QT_HINT_DIR}) #Qt5Core, Qt5Sql
find_package(Qt5Network HINTS ${QT_HINT_DIR}) #Qt5Core, Qt5Network
find_package(Qt5Xml HINTS ${QT_HINT_DIR}) #Qt5Core, Qt5Xml
#find_package(Qt5WinExtras HINTS ${QT_HINT_DIR}) #Qt5Core, Qt5Gui, Qt5WinExtras
find_package(Qt5Concurrent HINTS ${QT_HINT_DIR}) #Qt5Core, Qt5Concurrent
find_package(Qt5PrintSupport HINTS ${QT_HINT_DIR}) #Qt5Core, Qt5Gui, Qt5PrintSupport, Qt5Widgets
mark_as_advanced(
  Qt5Concurrent_DIR
  Qt5Core_DIR
  Qt5Gui_DIR
  Qt5Gui_EGL_LIBRARY
  Qt5Location_DIR
  Qt5Multimedia_DIR
  Qt5MultimediaWidgets_DIR
  Qt5Network_DIR
  Qt5OpenGL_DIR
  Qt5Positioning_DIR
  Qt5PrintSupport_DIR
  Qt5Qml_DIR
  Qt5Quick_DIR
  Qt5Sensors_DIR
  Qt5Sql_DIR
  Qt5Widgets_DIR
  #Qt5WinExtras_DIR
  Qt5Xml_DIR
)

# Build Qt
if(Qt5Widgets_DIR)
  option(BUILD_QT "Build Qt" OFF)

  if(${Qt5Widgets_VERSION} VERSION_LESS "5.2.1")
    message(FATAL_ERROR "Minimum supported Qt is v5.2.1")
  endif()
  if(${Qt5Widgets_VERSION} VERSION_LESS "5.4.0")
    find_package(Qt5WebKit HINTS ${QT_HINT_DIR}) #Qt5Core, Qt5Gui, Qt5Network, Qt5WebKit
    find_package(Qt5WebKitWidgets HINTS ${QT_HINT_DIR}) #Qt5Core, Qt5Gui, Qt5Network, Qt5WebKit, Qt5WebKitWidgets, Qt5Widgets
    mark_as_advanced(Qt5WebKit_DIR Qt5WebKitWidgets_DIR)
  else()
    find_package(Qt5WebEngine HINTS ${QT_HINT_DIR}) #Qt5Core, Qt5Gui, Qt5Network, Qt5Qml, Qt5Quick, Qt5WebEngine
    find_package(Qt5WebEngineWidgets HINTS ${QT_HINT_DIR}) #Qt5Core, Qt5Gui, Qt5Network, Qt5Qml, Qt5Quick, Qt5WebEngine, Qt5WebEngineWidgets, Qt5Widgets
    mark_as_advanced(Qt5WebEngine_DIR Qt5WebEngineWidgets_DIR)
  endif()
elseif(UNIX)
  option(BUILD_QT "Build Qt" ON)
endif()

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
  Boost_DIR
  BOOST_THREAD_LIBRARY
  ProcessorCount_cmd_getconf
  ProcessorCount_cmd_sysctl
)

# Boost
if(APPLE)
  set(Boost_USE_STATIC_LIBS ON)
elseif(WIN32)
  set(Boost_USE_STATIC_LIBS ON)

  #uncomment all of this if we want to force dynamic libs on windows
  #  set(Boost_USE_STATIC_LIBS OFF)
  #  add_definitions(-DBOOST_THREAD_USE_DLL -DBOOST_THREAD_DYN_LINK -DBOOST_PROGRAM_OPTIONS_DYN_LINK -DBOOST_REGEX_DYN_LINK -DBOOST_FILESYSTEM_DYN_LINK -DBOOST_SYSTEM_DYN_LINK  -DBOOST_DATE_TIME_DYN_LINK)
  #  link_directories(${Boost_LIBRARY_DIRS})
  #  if(MSVC)
  #    #Ignore dll specific warnings that are out of our hands to control, coming from external projects
  #    add_definitions(/wd4251 /wd4275)
  #  endif()
endif()

# DLM: should we allow boost to be linked dynamically on Unix or should we force static for all platforms?

find_package(Boost 1.55.0 COMPONENTS filesystem regex program_options system thread date_time log QUIET)

# Check default Boost install location for Windows if Boost has not been found, and display missing components
if(MSVC AND NOT Boost_FOUND AND EXISTS "C:/local/boost_1_55_0/")
  if(CMAKE_CL_64)
    set(BOOST_ARCH 64)
  else()
    set(BOOST_ARCH 32)
  endif()
  if(MSVC12)
    set(BOOST_MSVC 12.0)
  endif()

  if(EXISTS "C:/local/boost_1_55_0/lib${BOOST_ARCH}-msvc-${BOOST_MSVC}")
    set(BOOST_ROOT "C:/local/boost_1_55_0")
    set(BOOST_LIBRARYDIR "C:/local/boost_1_55_0/lib${BOOST_ARCH}-msvc-${BOOST_MSVC}")
    find_package(Boost 1.55.0 COMPONENTS filesystem regex program_options system thread date_time log QUIET)
    if(NOT Boost_FOUND)
      message(STATUS "Could NOT find Boost\n${Boost_ERROR_REASON}")
    endif()
  endif()
elseif(NOT Boost_FOUND)
  message(STATUS "Could NOT find Boost\n${Boost_ERROR_REASON}")
endif()

if(Boost_FOUND)
  option(BUILD_BOOST "Build Boost" OFF)
else()
  option(BUILD_BOOST "Build Boost" ON)
endif()


find_package(SWIG 3.0.0)
mark_as_advanced(
  SWIG_DIR
  SWIG_EXECUTABLE
  SWIG_VERSION
)

# Currently MacOS (with swig < 3.0.3) and V8 require special builds
# as of 2015-02-05. This should be re-checked after swig > 3.0.5 is released.
# 64-bit MSVC also requires BUILD_SWIG
if(SWIG_FOUND AND NOT BUILD_V8_BINDINGS AND NOT (APPLE AND ${SWIG_VERSION} VERSION_LESS "3.0.3") AND NOT (WIN32 AND MSVC AND CMAKE_CL_64))
  option(BUILD_SWIG "Build SWIG" OFF)
else()
  option(BUILD_SWIG "Build SWIG" ON)
endif()

if(NOT WIN32)
  find_program(RUBY_EXECUTABLE NAMES ruby2.0 ruby PATHS $ENV{CMAKE_RUBY_PATH} NO_SYSTEM_ENVIRONMENT_PATH)

  find_package(Ruby 2.0)

  if(NOT RUBY_FOUND)
    find_path(RUBY_CONFIG_INCLUDE_DIR
      NAMES ruby/config.h config.h
      HINTS /usr/include/x86_64-linux-gnu/ruby-2.0.0
    )
    find_package(Ruby 2.0)
  endif()
endif()

if(APPLE)
  option(BUILD_RUBY "Build Ruby" OFF)
elseif(UNIX)
  if(RUBY_FOUND)
    option(BUILD_RUBY "Build Ruby" OFF)
  else()
    option(BUILD_RUBY "Build Ruby" ON)
  endif()
endif()

if(NOT BUILD_SWIG)
  if(${SWIG_VERSION} VERSION_LESS "2.0.10")
    set(BUILD_SWIG ON)
  endif()
endif()

if(BUILD_WITH_MULTIPLE_PROCESSES)
  add_definitions(/MP)
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

if(BUILD_RUBY AND UNIX)
  ExternalProject_Add(Ruby
    URL http://openstudio-resources.s3.amazonaws.com/dependencies/ruby-2.0.0-p645.tar.gz
    URL_MD5 49919bba0c855eaf8e247108c7933a62
    CONFIGURE_COMMAND cd ${CMAKE_BINARY_DIR}/Ruby-prefix/src/Ruby && sh -c "./configure --enable-shared --without-tcl --without-tk --disable-install-doc --prefix=${CMAKE_BINARY_DIR}/Ruby-prefix/src/Ruby-install CFLAGS=\"-fPIC\""
    BUILD_COMMAND cd ${CMAKE_BINARY_DIR}/Ruby-prefix/src/Ruby && $(MAKE)
    INSTALL_COMMAND cd ${CMAKE_BINARY_DIR}/Ruby-prefix/src/Ruby && $(MAKE) install
  )
  set(RUBY_EXECUTABLE ${CMAKE_BINARY_DIR}/Ruby-prefix/src/Ruby-install/bin/ruby)

  if(APPLE)
    set(RUBY_LIBRARY ${CMAKE_BINARY_DIR}/Ruby-prefix/src/Ruby-install/lib/libruby.2.0.0.dylib)
  else()
    set(RUBY_LIBRARY ${CMAKE_BINARY_DIR}/Ruby-prefix/src/Ruby-install/lib/libruby.so)
  endif()
endif()

# Always unpack Ruby on Windows
if(WIN32)
  if(CMAKE_CL_64)
    ExternalProject_Add(Ruby
      URL http://openstudio-resources.s3.amazonaws.com/dependencies/ruby-2.0.0-p645-aws-win64.zip
      URL_MD5 4ad4ddc213a623b74f18f4c486651bc3
      PATCH_COMMAND ${PATCH_EXE} -p1 < ${CMAKE_SOURCE_DIR}/dependencies/ruby/x64-mingw32.patch
      CONFIGURE_COMMAND ""
      BUILD_COMMAND ""
      INSTALL_COMMAND ""
    )
    set(RUBY_EXECUTABLE ${CMAKE_BINARY_DIR}/Ruby-prefix/src/Ruby/bin/ruby.exe)
    set(RUBY_DLL x64-msvcrt-ruby200.dll)
    set(RUBY_DLLPATH "${CMAKE_BINARY_DIR}/Ruby-prefix/src/Ruby/bin/${RUBY_DLL}")
    set(RUBY_LIBRARY "${CMAKE_BINARY_DIR}/Ruby-prefix/src/Ruby/lib/libx64-msvcrt-ruby200.dll.a")
  else()
    ExternalProject_Add(Ruby
      URL http://openstudio-resources.s3.amazonaws.com/dependencies/ruby-2.0.0-p645-aws-win32.zip
      URL_MD5 712839ed5e7c21043026420ddd82b88c
      PATCH_COMMAND ${PATCH_EXE} -p1 < ${CMAKE_SOURCE_DIR}/dependencies/ruby/i386-mingw32.patch
      CONFIGURE_COMMAND ""
      BUILD_COMMAND ""
      INSTALL_COMMAND ""
    )
    set(RUBY_EXECUTABLE ${CMAKE_BINARY_DIR}/Ruby-prefix/src/Ruby/bin/ruby.exe)
    set(RUBY_DLL msvcrt-ruby200.dll)
    set(RUBY_DLLPATH "${CMAKE_BINARY_DIR}/Ruby-prefix/src/Ruby/bin/${RUBY_DLL}")
    set(RUBY_LIBRARY "${CMAKE_BINARY_DIR}/Ruby-prefix/src/Ruby/lib/libmsvcrt-ruby200.dll.a")
  endif()

  # Install required gems
  set(ENV{BUNDLE_PATH} "${CMAKE_BINARY_DIR}/Ruby-prefix/src/Ruby/lib/ruby/gems/2.0.0")
  add_custom_command(TARGET Ruby
                     POST_BUILD
                     COMMAND "${CMAKE_BINARY_DIR}/Ruby-prefix/src/Ruby/bin/ruby.exe" "${CMAKE_BINARY_DIR}/Ruby-prefix/src/Ruby/bin/bundle" "install" "--no-cache"
                     COMMAND "${CMAKE_BINARY_DIR}/Ruby-prefix/src/Ruby/bin/ruby.exe" "${CMAKE_BINARY_DIR}/Ruby-prefix/src/Ruby/bin/bundle" "clean" "--force"
                     WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/dependencies/ruby"
                     COMMENT "Installing gems..."
  )

endif()


if(BUILD_SWIG)
  if(UNIX)
    # We patch it up with a version of pcre we provide to avoid having to have the requirement locally
    ExternalProject_Add(SWIG
      URL http://openstudio-resources.s3.amazonaws.com/dependencies/swig-3.0.7.tar.gz
      URL_MD5 7fff46c84b8c630ede5b0f0827e3d90a

      PATCH_COMMAND cp ${CMAKE_SOURCE_DIR}/dependencies/pcre-8.31.tar.bz2 ${CMAKE_BINARY_DIR}/SWIG-prefix/src/SWIG-build && cd ${CMAKE_BINARY_DIR}/SWIG-prefix/src/SWIG-build && ../SWIG/Tools/pcre-build.sh
      CONFIGURE_COMMAND ../SWIG/configure --prefix=${CMAKE_BINARY_DIR}/SWIG-prefix/src/SWIG-install
      BUILD_COMMAND cd ${CMAKE_BINARY_DIR}/SWIG-prefix/src/SWIG-build && $(MAKE)
      INSTALL_COMMAND cd ${CMAKE_BINARY_DIR}/SWIG-prefix/src/SWIG-build && $(MAKE) install
      )
    set(SWIG_EXECUTABLE ${CMAKE_BINARY_DIR}/SWIG-prefix/src/SWIG-install/bin/swig)
  else()
    # SWIG requires MinGW to compile on windows, so we just copy in the prebuilt binary
    ExternalProject_Add(SWIG
      URL http://openstudio-resources.s3.amazonaws.com/dependencies/swigwin-3.0.7.zip
      URL_MD5 d8b5a9ce49c819cc1bfc1e797b85ba7a
      CONFIGURE_COMMAND ""
      BUILD_COMMAND ""
      INSTALL_COMMAND ""
    )
    set(SWIG_EXECUTABLE ${CMAKE_BINARY_DIR}/SWIG-prefix/src/SWIG/swig.exe)
  endif()
endif()

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

ExternalProject_Add(GoogleTest
  URL http://openstudio-resources.s3.amazonaws.com/dependencies/gtest-1.7.0.tar.gz
  URL_MD5 bad74f626d18f724cad4c9fe2e6ef27d
  PATCH_COMMAND "${CMAKE_COMMAND}" -E copy_directory ${CMAKE_SOURCE_DIR}/dependencies/GoogleTest ${CMAKE_BINARY_DIR}/GoogleTest-prefix/src/GoogleTest
  INSTALL_COMMAND ""
  CMAKE_CACHE_ARGS
  -DCMAKE_OSX_ARCHITECTURES:STRING=${CMAKE_OSX_ARCHITECTURES}
  -DCMAKE_OSX_DEPLOYMENT_TARGET:STRING=${CMAKE_OSX_DEPLOYMENT_TARGET}
  -DCMAKE_OSX_SYSROOT:STRING=${CMAKE_OSX_SYSROOT}
  -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
  -DCMAKE_CXX_COMPILER:STRING=${CMAKE_CXX_COMPILER}
  -DCMAKE_CXX_FLAGS:STRING=${CMAKE_CXX_FLAGS}
  -DCMAKE_EXE_LINKER_FLAGS:STRING=${CMAKE_EXE_LINKER_FLAGS}
  -DCMAKE_MODULE_LINKER_FLAGS:STRING=${CMAKE_MODULE_LINKER_FLAGS}
  -DCMAKE_SHARED_LINKER_FLAGS:STRING=${CMAKE_SHARED_LINKER_FLAGS}
)

set(GTEST_INCLUDE_DIR ${CMAKE_BINARY_DIR}/GoogleTest-prefix/src/GoogleTest/include)
set(GTEST_LIB_DIR ${CMAKE_BINARY_DIR}/GoogleTest-prefix/src/GoogleTest-build)

ExternalProject_Add(GeographicLib
  URL http://openstudio-resources.s3.amazonaws.com/dependencies/GeographicLib-1.45.zip
  URL_MD5 c3c8f989d700fc88c8ce4584cb298cee
  #PATCH_COMMAND "${CMAKE_COMMAND}" -E copy_directory ${CMAKE_SOURCE_DIR}/dependencies/GeographicLib ${CMAKE_BINARY_DIR}/GeographicLib-prefix/src/GeographicLib
  INSTALL_COMMAND ""
  CMAKE_CACHE_ARGS
  -DCMAKE_OSX_ARCHITECTURES:STRING=${CMAKE_OSX_ARCHITECTURES}
  -DCMAKE_OSX_DEPLOYMENT_TARGET:STRING=${CMAKE_OSX_DEPLOYMENT_TARGET}
  -DCMAKE_OSX_SYSROOT:STRING=${CMAKE_OSX_SYSROOT}
  -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
  -DCMAKE_CXX_COMPILER:STRING=${CMAKE_CXX_COMPILER}
  -DCMAKE_CXX_FLAGS:STRING=${CMAKE_CXX_FLAGS}
  -DCMAKE_EXE_LINKER_FLAGS:STRING=${CMAKE_EXE_LINKER_FLAGS}
  -DCMAKE_MODULE_LINKER_FLAGS:STRING=${CMAKE_MODULE_LINKER_FLAGS}
  -DCMAKE_SHARED_LINKER_FLAGS:STRING=${CMAKE_SHARED_LINKER_FLAGS}
  -DGEOGRAPHICLIB_LIB_TYPE:STRING=STATIC
  -DCMAKE_POSITION_INDEPENDENT_CODE:BOOL=ON
)

# Don't confuse "GEOGRAPHICLIB"_Dir as something that refers to the 
# actual library compiled unit. This variable just refers to where the project is at.
# We will use this to find the cmake config file provided by the project,
# and that config file will tell us everything we need to know about the project's targets.
set(GeographicLib_DIR ${CMAKE_BINARY_DIR}/GeographicLib-prefix/src/GeographicLib-build/)

if(BUILD_QT)
  set(Qt5Widgets_DIR ${CMAKE_BINARY_DIR}/Qt-prefix/src/Qt-install/lib/cmake/Qt5Widgets)
  set(Qt5Sql_DIR ${CMAKE_BINARY_DIR}/Qt-prefix/src/Qt-install/lib/cmake/Qt5Sql)
  set(Qt5Network_DIR ${CMAKE_BINARY_DIR}/Qt-prefix/src/Qt-install/lib/cmake/Qt5Network)
  set(Qt5Xml_DIR ${CMAKE_BINARY_DIR}/Qt-prefix/src/Qt-install/lib/cmake/Qt5Xml)
  if(${Qt5Widgets_VERSION} VERSION_LESS "5.4.0")
    set(Qt5WebKit_DIR ${CMAKE_BINARY_DIR}/Qt-prefix/src/Qt-install/lib/cmake/Qt5WebKit)
    set(Qt5WebKitWidgets_DIR ${CMAKE_BINARY_DIR}/Qt-prefix/src/Qt-install/lib/cmake/Qt5WebKitWidgets)
  else()
    set(Qt5WebEngine_DIR ${CMAKE_BINARY_DIR}/Qt-prefix/src/Qt-install/lib/cmake/Qt5WebEngine)
    set(Qt5WebEngineWidgets_DIR ${CMAKE_BINARY_DIR}/Qt-prefix/src/Qt-install/lib/cmake/Qt5WebEngineWidgets)
  endif()
  set(Qt5Concurrent_DIR ${CMAKE_BINARY_DIR}/Qt-prefix/src/Qt-install/lib/cmake/Qt5Concurrent)
  set(Qt5PrintSupport_DIR ${CMAKE_BINARY_DIR}/Qt-prefix/src/Qt-install/lib/cmake/Qt5PrintSupport)

  if(APPLE)
    # We're going to explicitly use the SDK specified in the configuration by the user
    #set(APPLE_SDK_COMMAND "-sdk \"${CMAKE_OSX_SYSROOT}\"" )
    set(APPLE_SDK_COMMAND "-sdk macosx${OSX_SDK}")

    # At the moment we are building all of debug,release,x86 and x86_64
    # it would appear that calling "make" in some way that CMake can detect that we are calling
    # make (and attempt to pass parameters to the child make jobs) is breaking our ability to install
    # the below code circumvents that by calling make through a call to "sh"
    ExternalProject_Add(Qt
      URL http://openstudio-resources.s3.amazonaws.com/dependencies/qt-everywhere-opensource-src-5.3.2.tar.gz
      URL_MD5 febb001129927a70174467ecb508a682
      CONFIGURE_COMMAND /bin/sh -c "cd ${CMAKE_BINARY_DIR}/Qt-prefix/src/Qt/ && ./configure -v ${APPLE_SDK_COMMAND} -platform macx-clang-64 -release -opensource -openssl -no-pch -no-ssse3 -qt-sql-sqlite -plugin-sql-sqlite -skip enginio -no-compile-examples -nomake examples -nomake tests -nomake tools -skip qtmultimedia -confirm-license -prefix ${CMAKE_BINARY_DIR}/Qt-prefix/src/Qt-install"
      PATCH_COMMAND ${PATCH_EXE} -p1 < ${CMAKE_SOURCE_DIR}/dependencies/qt/qmacstyle_mac.patch
      BUILD_COMMAND /bin/sh -c "cd ${CMAKE_BINARY_DIR}/Qt-prefix/src/Qt/ && make -j${QT_CPUCOUNT}"
      INSTALL_COMMAND /bin/sh -c "cd ${CMAKE_BINARY_DIR}/Qt-prefix/src/Qt/ && make -j1 install"
    )
  elseif(UNIX)
    message(WARNING "Qt5 super build is currently untested")
    ExternalProject_Add(Qt
      URL http://openstudio-resources.s3.amazonaws.com/dependencies/qt-everywhere-opensource-src-5.3.2.tar.gz
      URL_MD5 febb001129927a70174467ecb508a682
      CONFIGURE_COMMAND ${CMAKE_BINARY_DIR}/Qt-prefix/src/Qt/configure -debug-and-release -opensource -confirm-license -shared -qt-sql-sqlite -qt-xcb -openssl -nomake examples -nomake tests -plugindir ${CMAKE_BINARY_DIR}/Qt-prefix/src/Qt-install/share/openstudio/qtplugins -qtlibinfix OpenStudio -prefix ${CMAKE_BINARY_DIR}/Qt-prefix/src/Qt-install
      BUILD_COMMAND $(MAKE) -f ${CMAKE_BINARY_DIR}/Qt-prefix/src/Qt-build/Makefile
      INSTALL_COMMAND cd ${CMAKE_BINARY_DIR}/Qt-prefix/src/Qt-build && $(MAKE) install
    )
  else()
    message(FATAL_ERROR "Qt5 cannot currently be built for Windows with the super build.  Please download a Qt installer from http://qt.io/")
  endif()

  #Update this for Windows if BUILD_QT is enabled for WINDOWS
  if(UNIX)
    list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR}/Qt-prefix/src/Qt-install/lib/cmake)
  endif()
endif()


set(OpenStudioCore_DIR ${CMAKE_BINARY_DIR}/OSCore-prefix/src/OSCore-build)

# Set up the dependencies for OpenStudioCore
set(OpenStudioCore_DEPENDS GoogleTest)

list(APPEND OpenStudioCore_DEPENDS GeographicLib)

if(BUILD_QT)
  list(APPEND OpenStudioCore_DEPENDS Qt)
endif()

if(BUILD_SWIG)
  list(APPEND OpenStudioCore_DEPENDS SWIG)
endif()

if(BUILD_BOOST)
  list(APPEND OpenStudioCore_DEPENDS Boost)
endif()

if(BUILD_RUBY OR WIN32)
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
    -DGTEST_LIB_DIR:PATH=${GTEST_LIB_DIR}
    -DGTEST_INCLUDE_DIR:PATH=${GTEST_INCLUDE_DIR}
    -DGeographicLib_DIR:PATH=${GeographicLib_DIR}
    -DCMAKE_OSX_ARCHITECTURES:STRING=${CMAKE_OSX_ARCHITECTURES}
    -DCMAKE_OSX_DEPLOYMENT_TARGET:STRING=${CMAKE_OSX_DEPLOYMENT_TARGET}
    -DCMAKE_OSX_SYSROOT:STRING=${CMAKE_OSX_SYSROOT}
    -DENERGYPLUS_VERSION_MAJOR:STRING=${ENERGYPLUS_VERSION_MAJOR}
    -DENERGYPLUS_VERSION_MINOR:STRING=${ENERGYPLUS_VERSION_MINOR}
    -DENERGYPLUS_VERSION_PATCH:STRING=${ENERGYPLUS_VERSION_PATCH}
    -DENERGYPLUS_BUILD_SHA:STRING=${ENERGYPLUS_BUILD_SHA}
    -DBUILD_DOCUMENTATION:BOOL=${BUILD_DOCUMENTATION}
    -DBUILD_CSHARP_BINDINGS:BOOL=${BUILD_CSHARP_BINDINGS}
    -DBUILD_JAVA_BINDINGS:BOOL=${BUILD_JAVA_BINDINGS}
    -DBUILD_V8_BINDINGS:BOOL=${BUILD_V8_BINDINGS}
    -DBUILD_NODE_MODULES:BOOL=${BUILD_NODE_MODULES}
    -DBUILD_PYTHON_BINDINGS:BOOL=${BUILD_PYTHON_BINDINGS}
    -DBUILD_TESTING:BOOL=${BUILD_TESTING}
    -DBUILD_PACKAGE:BOOL=${BUILD_PACKAGE}
    -DENABLE_TEST_RUNNER_TARGETS:BOOL=${ENABLE_TEST_RUNNER_TARGETS}
    -DBUILD_WITH_MULTIPLE_PROCESSES:BOOL=${BUILD_WITH_MULTIPLE_PROCESSES}
    -DBUILD_WITH_OPENSSL:BOOL=${BUILD_WITH_OPENSSL}
    -DBUILD_RUBY_GEM:BOOL=${BUILD_RUBY_GEM}
    -DBUILD_RUBY_GEM_WITH_LIBS:BOOL=${BUILD_RUBY_GEM_WITH_LIBS}
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
    -DCMAKE_VERSION_MAJOR:STRING=${CMAKE_VERSION_MAJOR}
    -DCMAKE_VERSION_MINOR:STRING=${CMAKE_VERSION_MINOR}
    -DCMAKE_VERSION_PATCH:STRING=${CMAKE_VERSION_PATCH}
    -DCMAKE_VERSION_BUILD:STRING=${CMAKE_VERSION_BUILD}
    -DBOOST_ROOT:STRING=${BOOST_ROOT}
    -DBOOST_LIBRARYDIR:STRING=${BOOST_LIBRARYDIR}
    -DQt5Widgets_DIR:STRING=${Qt5Widgets_DIR}
    -DQt5Sql_DIR:STRING=${Qt5Sql_DIR}
    -DQt5Network_DIR:STRING=${Qt5Network_DIR}
    -DQt5Xml_DIR:STRING=${Qt5Xml_DIR}
    -DQt5WebKit_DIR:STRING=${Qt5WebKit_DIR}
    -DQt5WebKitWidgets_DIR:STRING=${Qt5WebKitWidgets_DIR}
    -DQt5WebEngine_DIR:STRING=${Qt5WebEngine_DIR}
    -DQt5WebEngineWidgets_DIR:STRING=${Qt5WebEngineWidgets_DIR}
    #-DQt5WinExtras_DIR:STRING=${Qt5WinExtras_DIR}
    -DQt5Concurrent_DIR:STRING=${Qt5Concurrent_DIR}
    -DQt5PrintSupport_DIR:STRING=${Qt5PrintSupport_DIR}
    -DSWIG_EXECUTABLE:STRING=${SWIG_EXECUTABLE}
    -DRUBY_EXECUTABLE:STRING=${RUBY_EXECUTABLE}
    -DNODE_BIN_DIR:STRING=${NODE_BIN_DIR}
    -DNODE_INCLUDE_DIR:STRING=${NODE_INCLUDE_DIR}
    -DV8_INCLUDE_DIR:STRING=${V8_INCLUDE_DIR}
    -DRUBY_LIBRARY:STRING=${RUBY_LIBRARY}
    -DDOXYGEN_EXECUTABLE:STRING=${DOXYGEN_EXECUTABLE}
    -DDOXYGEN_DOT_EXECUTABLE:STRING=${DOXYGEN_DOT_EXECUTABLE}
    -DENERGYPLUS_LOCATION:PATH=$ENV{ENERGYPLUSDIR}
    -DRADIANCE_LOCATION:PATH=${RADIANCE_LOCATION}
    -DOPENSSL_ROOT_DIR:PATH=${OPENSSL_ROOT_DIR}
    -DPATCH_EXE:PATH=${PATCH_EXE}
    -DMAXIMIZE_CPU_USAGE:BOOL=${MAXIMIZE_CPU_USAGE}
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

if(MSVC)
  ExternalProject_Add_Step(OSCore MakeRubyReleaseFolder
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/OSCore-prefix/src/OSCore-build/Products/Release
    DEPENDERS configure
  )
  ExternalProject_Add_Step(OSCore MakeRubyDebugFolder
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/OSCore-prefix/src/OSCore-build/Products/Debug
    DEPENDERS configure
  )
  ExternalProject_Add_Step(OSCore MakeRubyRelWithDebInfoFolder
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/OSCore-prefix/src/OSCore-build/Products/RelWithDebInfo
    DEPENDERS configure
  )
  ExternalProject_Add_Step(OSCore MakeRubyMinSizeRelFolder
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/OSCore-prefix/src/OSCore-build/Products/MinSizeRel
    DEPENDERS configure
  )


  ExternalProject_Add_Step(OSCore MakeRubyReleaseDLL
    COMMAND ${CMAKE_COMMAND} -E copy "${RUBY_DLLPATH}" "${CMAKE_BINARY_DIR}/OSCore-prefix/src/OSCore-build/Products/Release/${RUBY_DLL}"
    DEPENDERS configure
  )
  ExternalProject_Add_Step(OSCore MakeRubyDebugDLL
    COMMAND ${CMAKE_COMMAND} -E copy "${RUBY_DLLPATH}" "${CMAKE_BINARY_DIR}/OSCore-prefix/src/OSCore-build/Products/Debug/${RUBY_DLL}"
    DEPENDERS configure
  )
  ExternalProject_Add_Step(OSCore MakeRubyRelWithDebInfoDLL
    COMMAND ${CMAKE_COMMAND} -E copy "${RUBY_DLLPATH}" "${CMAKE_BINARY_DIR}/OSCore-prefix/src/OSCore-build/Products/RelWithDebInfo/${RUBY_DLL}"
    DEPENDERS configure
  )
  ExternalProject_Add_Step(OSCore MakeRubyMinSizeRelDLL
    COMMAND ${CMAKE_COMMAND} -E copy "${RUBY_DLLPATH}" "${CMAKE_BINARY_DIR}/OSCore-prefix/src/OSCore-build/Products/MinSizeRel/${RUBY_DLL}"
    DEPENDERS configure
  )

endif()


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
      -DQt5Widgets_DIR:STRING=${Qt5Widgets_DIR}
      -DQt5Sql_DIR:STRING=${Qt5Sql_DIR}
      -DQt5Network_DIR:STRING=${Qt5Network_DIR}
      -DQt5Xml_DIR:STRING=${Qt5Xml_DIR}
      -DQt5WebKit_DIR:STRING=${Qt5WebKit_DIR}
      -DQt5WebKitWidgets_DIR:STRING=${Qt5WebKitWidgets_DIR}
      -DQt5WebEngine_DIR:STRING=${Qt5WebEngine_DIR}
      -DQt5WebEngineWidgets_DIR:STRING=${Qt5WebEngineWidgets_DIR}
      #-DQt5WinExtras_DIR:STRING=${Qt5WinExtras_DIR}
      -DQt5Concurrent_DIR:STRING=${Qt5Concurrent_DIR}
      -DQt5PrintSupport_DIR:STRING=${Qt5PrintSupport_DIR}
    INSTALL_COMMAND ""
  )
endif()
