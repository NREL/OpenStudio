message(STATUS "Using CMake ${CMAKE_VERSION}")
cmake_minimum_required(VERSION 3.20.0)
cmake_policy(SET CMP0048 NEW)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# Do not enable compiler specific extensions, for eg on GCC use -std=c++1z (=c++17) and not -std=gnu++17
set(CMAKE_CXX_EXTENSIONS OFF)

# Use ccache if available, has to be before "project()"
find_program(CCACHE_PROGRAM NAMES ccache sccache)
if(CCACHE_PROGRAM)
  # Support: Unix Makefiles and Ninja only
  # RULE_LAUNCH_COMPILE is an internal variable and makes RC compilation fail on windows, hence why we use the C/CXX COMPILER_LAUNCHER instead
  message(STATUS "${CCACHE_PROGRAM} found and enabled")
  set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE_PROGRAM} CACHE FILEPATH "CXX compiler cache used")
  set(CMAKE_C_COMPILER_LAUNCHER ${CCACHE_PROGRAM} CACHE FILEPATH "C compiler cache used")
endif()

# Project macro can only take digits in the version, it splits these out to specific variables
project(OpenStudio VERSION 3.9.1)

include(CMake/compiler_flags.cmake)

# The RT Manifest file and C# SDK only support the digit portion of patch
string(APPEND PROJECT_VERSION_PATCH_DIGIT ${PROJECT_VERSION_PATCH})

# CPack package version does not support extended patch version
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")

# Set a default build type if none was specified
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to 'Release' as none was specified.")
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release"
    "MinSizeRel" "RelWithDebInfo")
endif()

if ( APPLE )
  find_library(SECURITY_FRAMEWORK Security)
endif()

include(CPackComponent)
include(CheckCXXCompilerFlag)

if(POLICY CMP0020)
  cmake_policy(SET CMP0020 NEW)
endif()
if(POLICY CMP0022)
  cmake_policy(SET CMP0022 NEW)
endif()
if(POLICY CMP0042)
  cmake_policy(SET CMP0042 NEW)
endif()
if(POLICY CMP0054)
  cmake_policy(SET CMP0054 NEW)
endif()
if(POLICY CMP0077)
  cmake_policy(SET CMP0077 NEW)
endif()

###############################################################################
#                                  N I N J A                                  #
###############################################################################
# Ninja support: has to be atop for it to take effect before anything else is done
# Add Color Output if Using Ninja
macro(AddCXXFlagIfSupported flag test)
  CHECK_CXX_COMPILER_FLAG(${flag} ${test})
  if(${${test}})
    message("adding ${flag}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${flag}")
  endif()
endmacro()

if("Ninja" STREQUAL ${CMAKE_GENERATOR})
  # Will be passed to OpenStudio.in to deal with the Win32 case
  set(NINJA TRUE)
  # Clang
  if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    AddCXXFlagIfSupported(-fcolor-diagnostics COMPILER_SUPPORTS_fcolor-diagnostics)
  endif()

  # g++
  if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    # For some reason it doesn't say its supported, but it works...
    # AddCXXFlagIfSupported(-fdiagnostics-color COMPILER_SUPPORTS_fdiagnostics-color)
    message(STATUS "Ninja: Forcing -fdiagnostics-color=always")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fdiagnostics-color=always")
  endif()
endif()

# Xcode/Ninja generators undefined MAKE
if(CMAKE_GENERATOR MATCHES "Make")
  set(MAKE "$(MAKE)")
else()
  set(MAKE make)
endif()

###############################################################################
# Project version information
set(PROJECT_VERSION_BUILD "Unknown" CACHE STRING "Build number") # git sha
find_package(Git)

if(NOT GIT_FOUND)
  find_program(GIT_EXECUTABLE git HINTS "$ENV{LOCALAPPDATA}/Programs/git/bin" "C:/Program Files/Git/bin")
  if(NOT GIT_EXECUTABLE_NOTFOUND)
    set(GIT_FOUND TRUE)
  endif()
endif()

if(GIT_FOUND)
  # Git SHA (length 10)
  execute_process(COMMAND "${GIT_EXECUTABLE}" "rev-parse" "--short=10" "HEAD"
                  WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
                  TIMEOUT 10
                  RESULT_VARIABLE RESULT
                  OUTPUT_VARIABLE GIT_VERSION
                  ERROR_QUIET
                  OUTPUT_STRIP_TRAILING_WHITESPACE)
  if(${RESULT} EQUAL 0 AND NOT "${GIT_VERSION}" EQUAL "${PROJECT_VERSION_BUILD}")
    set(PROJECT_VERSION_BUILD ${GIT_VERSION} CACHE STRING "Build number" FORCE) # git sha
  endif()

  # Git branch name
  execute_process(COMMAND "${GIT_EXECUTABLE}" "rev-parse" "--abbrev-ref" "HEAD"
                  WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
                  TIMEOUT 10
                  RESULT_VARIABLE RESULT
                  OUTPUT_VARIABLE GIT_BRANCH
                  ERROR_QUIET
                  OUTPUT_STRIP_TRAILING_WHITESPACE)
  if(${RESULT} EQUAL 0 AND NOT "${GIT_BRANCH}" EQUAL "${PROJECT_GIT_BRANCH}")
    set(PROJECT_GIT_BRANCH ${GIT_BRANCH} CACHE STRING "Git Branch Name" FORCE)
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

# Check if scope has a parent
get_directory_property(hasParent PARENT_DIRECTORY)

# README: what's the difference between all these versions?

# The "OpenStudio_VERSION" ones should be used in CMakeLists.txt
# * OpenStudio_VERSION: includes the prerelease tag if any: '3.0.0-rc1'
# * OpenStudio_VERSION_PATCH: Patch + prelrease tag if any

# The "OPENSTUDIO_VERSION" ones should only be used in OpenStudio.in (aside from using the the long version for naming the installer)
# * OPENSTUDIO_VERSION: just major.minor.patch: '3.0.0'
# * OPENSTUDIO_LONG_VERSION: includes prelrease and build sha: "3.0.0-rc1+ejfbxhza'

# TODO: Modify the more specific variables as needed to indicate prerelease, etc
# Keep in beta in-between release cycles. Set to empty string (or comment out) for official)
set(PROJECT_VERSION_PRERELEASE "alpha")

# OpenStudio version: Only include Major.Minor.Patch, eg "3.0.0", even if you have a prerelease tag
set(OPENSTUDIO_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}")

if (NOT "${PROJECT_VERSION_PRERELEASE}" STREQUAL "")
  message(STATUS "We have a prerelease tag set to '${PROJECT_VERSION_PRERELEASE}'")
  if("${PROJECT_GIT_BRANCH}" STREQUAL "master")
    message(AUTHOR_WARNING "You are building branch 'master', perhaps you shouldn't have included a pre-release tag")
  endif()
  string(APPEND OpenStudio_VERSION "-${PROJECT_VERSION_PRERELEASE}")
  string(APPEND OpenStudio_VERSION_PATCH "-${PROJECT_VERSION_PRERELEASE}")
  # Long version, include prerelease and build metadata. eg '3.0.0-rc1+6383a191dc'
  set(OPENSTUDIO_LONG_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}-${PROJECT_VERSION_PRERELEASE}+${PROJECT_VERSION_BUILD}")
else()
  message(STATUS "We do not have a prerelease tag")
  if(NOT "${PROJECT_GIT_BRANCH}" STREQUAL "master")
    message(AUTHOR_WARNING "You are building another branch than master but you do not have a prerelease tag, consider adding one such as 'beta'")
  endif()
  # Long version, include build metadata. eg '3.0.0+6383a191dc' for an official release
  set(OPENSTUDIO_LONG_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}+${PROJECT_VERSION_BUILD}")
endif()

# Export to parent scope
if(hasParent)
  set(OPENSTUDIO_VERSION "${OPENSTUDIO_VERSION}" PARENT_SCOPE)
  set(OPENSTUDIO_LONG_VERSION "${OPENSTUDIO_LONG_VERSION}" PARENT_SCOPE)
endif()

# EnergyPlus Idd version
set(ENERGYPLUS_VERSION_MAJOR 24)
set(ENERGYPLUS_VERSION_MINOR 2)
set(ENERGYPLUS_VERSION_PATCH 0)
set(ENERGYPLUS_VERSION "${ENERGYPLUS_VERSION_MAJOR}.${ENERGYPLUS_VERSION_MINOR}.${ENERGYPLUS_VERSION_PATCH}")
# Build SHA is not required to have a value, but if it does OpenStudio will require this build.
set(ENERGYPLUS_BUILD_SHA "94a887817b")

# ENERGYPLUS_RELEASE_NAME is used to locate the E+ download
# from the github releases
set(ENERGYPLUS_RELEASE_NAME "v24.2.0a")

set(ENERGYPLUS_REPO "NREL")

# Radiance
set(RADIANCE_VERSION "5.0.a.12")

# configure file with version information
configure_file(${PROJECT_SOURCE_DIR}/OpenStudio.hxx.in ${PROJECT_BINARY_DIR}/src/OpenStudio.hxx)

# Deployment target
if(APPLE)
  set(CMAKE_OSX_DEPLOYMENT_TARGET "10.12" CACHE STRING "Minimum OS X deployment version")
endif()

# compiler id for about boxes
if(MSVC)
  set(ABOUT_COMPILER "${CMAKE_GENERATOR}")
elseif(APPLE)
  set(ABOUT_COMPILER "${CMAKE_CXX_COMPILER_ID} ${CMAKE_OSX_ARCHITECTURES} - OSX ${CMAKE_OSX_DEPLOYMENT_TARGET}")
else()
  set(ABOUT_COMPILER "${CMAKE_CXX_COMPILER_ID}")
endif()

# If Linux, check LSB_RELEASE globally so we can use it after
# But also infer the new E+ package names (starting at 9.4.0)
if(APPLE)

  set(ENERGYPLUS_SYSTEM_VERSION "")

  find_program(UNAME uname)
  execute_process(COMMAND ${UNAME} -m
    OUTPUT_VARIABLE ARCH
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )

elseif(UNIX)
  # OS_RELEASE is the result of `uname -r` which is unhelpful (eg '5.4.0-42-generic')
  find_program(LSB_RELEASE lsb_release)
  # -rs outputs only 16.04, or 18.04
  execute_process(COMMAND ${LSB_RELEASE} -rs
    OUTPUT_VARIABLE LSB_RELEASE_VERSION_SHORT
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )

  # -is outputs "Ubuntu" or "Fedora"
  execute_process(COMMAND ${LSB_RELEASE} -is
    OUTPUT_VARIABLE LSB_RELEASE_ID_SHORT
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )

  find_program(UNAME uname)
  execute_process(COMMAND ${UNAME} -m
    OUTPUT_VARIABLE ARCH
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )
  if (ARCH STREQUAL "aarch64")
    set(ARCH "arm64")
  endif()

  # eg: `-Ubuntu18.04`
  set(ENERGYPLUS_SYSTEM_VERSION "-${LSB_RELEASE_ID_SHORT}${LSB_RELEASE_VERSION_SHORT}")

  message(STATUS "LSB_RELEASE_ID_SHORT=${LSB_RELEASE_ID_SHORT}, LSB_RELEASE_VERSION_SHORT=${LSB_RELEASE_VERSION_SHORT}")

elseif(MSVC)
  # no-op
  set(ENERGYPLUS_SYSTEM_VERSION "")
endif()

###############################################################################

###############################################################################
# Build options

# TODO: should this be only in OpenStudioApplication?
option(BUILD_PAT "Build PAT" OFF)

# Build C++ documentation using Doxygen
# Requires: doxygen
option(BUILD_DOCUMENTATION "Build Documentation" OFF)

# Build CSharp bindings
option(BUILD_CSHARP_BINDINGS "Build CSharp bindings" OFF)
option(BUILD_NUGET_PACKAGE "Build NuGet Package" OFF)

# Build Python bindings
# Requires: SWIG Python
option(BUILD_PYTHON_BINDINGS "Build Python bindings" ON)
mark_as_advanced(BUILD_PYTHON_BINDINGS)

# Build ctest testing
# Requires: EnergyPlus
option(BUILD_TESTING "Build testing targets" OFF)

option(BUILD_BENCHMARK "Build benchmarking targets" OFF)
if(BUILD_BENCHMARK)
  configure_file(${PROJECT_SOURCE_DIR}/developer/python/run_benchmark.py ${PROJECT_BINARY_DIR}/run_benchmark.py COPYONLY)
  add_custom_target(run_benchmarks
    COMMAND python run_benchmark.py --quiet --all
    DEPENDS "${PROJECT_BINARY_DIR}/run_benchmark.py"
    WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
  )
endif()

option(BUILD_RUBY_BINDINGS "Build Ruby bindings" ON)
mark_as_advanced(BUILD_RUBY_BINDINGS)
if(CMAKE_SIZEOF_VOID_P EQUAL 4) # 32 bit
  set(BUILD_RUBY_BINDINGS OFF CACHE BOOL "Override option" FORCE)
  message("Ruby bindings are not support on 32 bit platforms, forcing BUILD_RUBY_BINDINGS OFF")
endif()

option(BUILD_CLI "Build OpenStudio command line interface" ON)
mark_as_advanced(BUILD_CLI)
if(NOT BUILD_RUBY_BINDINGS)
  set(BUILD_CLI OFF CACHE BOOL "Override option" FORCE)
  message("OpenStudio CLI depends on Ruby bindings which are disabled, therefore BUILD_CLI is also disabled")
endif()

# Build with OpenSSL support
set(BUILD_WITH_OPENSSL ON CACHE INTERNAL "Build With OpenSSL Support For SSH Connections")

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  add_definitions(-Qunused-arguments)
endif()

include(CheckIncludeFile)
check_include_file("unistd.h" HAVE_UNISTD_H)
if(HAVE_UNISTD_H)
  add_definitions(-DHAVE_UNISTD_H)
endif()

# Enable runtime checking features
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")

  option(ENABLE_COVERAGE "Enable coverage reporting for gcc/clang" OFF)
  if(ENABLE_COVERAGE)
    add_definitions(--coverage -O0 -g)
    set(SANITIZER_LINKER_FLAGS "${SANITIZER_LINKER_FLAGS} --coverage")
  endif()

  option(ENABLE_THREAD_SANITIZER "Enable thread sanitizer testing in gcc/clang" OFF)
  if(ENABLE_THREAD_SANITIZER)
    add_definitions(-fsanitize=thread -g)
    set(SANITIZER_LINKER_FLAGS "${SANITIZER_LINKER_FLAGS} -fsanitize=thread")
  endif()

  option(ENABLE_ADDRESS_SANITIZER "Enable address sanitizer testing in gcc/clang" OFF)
  if(ENABLE_ADDRESS_SANITIZER)
    add_definitions(-fsanitize=address -g)
    set(SANITIZER_LINKER_FLAGS "${SANITIZER_LINKER_FLAGS} -fsanitize=address")
  endif()

  option(ENABLE_UNDEFINED_SANITIZER "Enable undefined behavior sanitizer testing in gcc/clang" OFF)
  if(ENABLE_UNDEFINED_SANITIZER)
    add_definitions(-fsanitize=undefined -g)
    set(SANITIZER_LINKER_FLAGS "${SANITIZER_LINKER_FLAGS} -fsanitize=undefined")
  endif()

  option(ENABLE_NORECOVER_SANITIZER  "When any sanitizer that supports it report an error, exit with non zero" OFF)
  if(ENABLE_NORECOVER_SANITIZER)
    add_definitions(-fno-sanitize-recover=all)
    set(SANITIZER_LINKER_FLAGS "${SANITIZER_LINKER_FLAGS} -fno-sanitize-recover=all")
  endif()

  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${SANITIZER_LINKER_FLAGS}")
  set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${SANITIZER_LINKER_FLAGS}")
  set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} ${SANITIZER_LINKER_FLAGS}")


  mark_as_advanced(ENABLE_COVERAGE ENABLE_THREAD_SANITIZER ENABLE_ADDRESS_SANITIZER ENABLE_UNDEFINED_SANITIZER ENABLE_NORECOVER_SANITIZER)

endif()

if(APPLE)
  # Without this, cmake will give "Module" libraries the ".so" extension on Apple
  # This appears to be a legacy choice by the cmake developers, and seems out of place,
  # so the OpenStudio project will use the library suffix ".dylib"
  find_library(COREFOUNDATION_LIBRARY CoreFoundation)
  # TODO: remove when bumping Boost to 1.81+, cf https://github.com/NREL/OpenStudio/issues/4978
  add_definitions(-DBOOST_NO_CXX98_FUNCTION_BASE)
  add_definitions(-D_HAS_AUTO_PTR_ETC=0)
endif()

if(UNIX AND NOT APPLE)
  # Enable libgtest linking
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pthread")
endif()

if(WIN32)
  if(CMAKE_SIZEOF_VOID_P EQUAL 8) # 64 bit
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /OPT:REF /OPT:NOICF")
  else()
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /OPT:REF /OPT:NOICF")
  endif()
endif()

option(USE_LTO "Use Link-Time-Optimization" OFF)

if (USE_LTO)
  include(CheckIPOSupported)

  check_ipo_supported()
  set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
endif()

if(WIN32)
  add_definitions(-DNOMINMAX)
  # Set the BOOST_USE_WINAPI_VERSION to Windows10
  add_compile_definitions(BOOST_USE_WINAPI_VERSION=BOOST_WINAPI_VERSION_WIN7)
  # This will also align _WIN32_WINNT and WINVER
  # add_compile_definitions(BOOST_USE_WINDOWS_H) # Getting a confict with `typedef GUID UUID` from windows.h
  add_compile_definitions(BOOST_WINAPI_DEFINE_VERSION_MACROS)
  add_compile_definitions(WIN32_LEAN_AND_MEAN) # That excludes stuff that's not used too often, including the GUID stuff
endif()

if(MSVC AND NOT NINJA)
  # Build with Multiple Processes
  set(BUILD_WITH_MULTIPLE_PROCESSES ON CACHE BOOL "/MP compiler flag for full processor utilization")
  mark_as_advanced(BUILD_WITH_MULTIPLE_PROCESSES)
endif()

###############################################################################

###############################################################################
# Conan

if(BUILD_RUBY_BINDINGS)
  find_package(Ruby)
  message("Ruby_INCLUDE_DIRS=${Ruby_INCLUDE_DIRS}")
endif()
find_package(pugixml)
find_package(LibXslt)
find_package(libxml2)
find_package(jsoncpp)
find_package(fmt)
find_package(SQLite3)
find_package(cpprestsdk)
find_package(websocketpp)
find_package(Boost)
find_package(geographiclib)
find_package(SWIG 4.1.1 EXACT REQUIRED CONFIG)
message("SWIG_EXECUTABLE=${SWIG_EXECUTABLE}")
message("SWIG_DIR=${SWIG_DIR}")
find_package(TinyGLTF)
find_package(CLI11)
find_package(minizip)
find_package(OpenSSL)
find_package(ZLIB)
if(BUILD_TESTING)
  find_package(GTest)
endif()
if(BUILD_BENCHMARK)
  find_package(benchmark)
endif()

# A macro to find a conan related value especially when using multi-config builds (MSVC)
# But it also works with single-config builds
macro(FindValue ValueName)
  set(LocalVar "")
  set(LocalVar $<$<CONFIG:Debug>:${${ValueName}_DEBUG}>$<$<CONFIG:Release>:${${ValueName}_RELEASE}>$<$<CONFIG:RelWithDebInfo>:${${ValueName}_RELWITHDEBINFO}>$<$<CONFIG:MinSizeRel>:${${ValueName}_MINSIZEREL}>
  )
#  list(JOIN LocalVar "" LocalVar)
  string(STRIP ${LocalVar} LocalVar)
  set(CURRENT_${ValueName} $<IF:$<BOOL:${LocalVar}>,${LocalVar},${${ValueName}}>)
  # For debug purposes
  # message(STATUS "Found '${ValueName}' as '${CURRENT_${ValueName}}'")
endmacro()

###############################################################################

# Export to Parent
if(hasParent)
  set(Ruby_INCLUDE_DIRS "${Ruby_INCLUDE_DIRS}" PARENT_SCOPE)
endif()

###############################################################################
# CMake control

# High level project configuration
# TODO: Do we actually want everything to go to CMAKE_BINARY_DIR/Products,
# so that when you build OpenStudioApplication you get both OpenStudio (core) and Application in the same place?
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/Products")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/Products")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/Products")

set(LIBRARY_SEARCH_DIRECTORY "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}" "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/Release" "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/Debug")

set(LIB_DESTINATION_DIR lib)
if(WIN32)
  # Windows does not have RPATH handling, so we just put everything next to the CLI in the bin/ folder
  set(LIB_DESTINATION_DIR bin)
endif()

# TODO: move App macros to a local ProjectMacros.cmake
# Include project specific macros
include("${PROJECT_SOURCE_DIR}/ProjectMacros.cmake")

# Search first in the binary dir, where conan will install finders, then
# search for modules in the root dir to override cmake ones
# Start with ROOT, then PROJECT_BINARY_DIR
list(APPEND CMAKE_MODULE_PATH "${CMAKE_BINARY_DIR} ${PROJECT_BINARY_DIR}" "${PROJECT_SOURCE_DIR}/CMake")

# Use CTest
set(ALL_TESTING_TARGETS "") # global list
if(BUILD_TESTING)
  enable_testing()
  include(CTest)
  include(GoogleTest)
endif()

###############################################################################

###############################################################################
# Compiler and system specific options
if(UNIX)

  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -fPIC -fno-strict-aliasing")

  # all warnings
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -fPIC -fno-strict-aliasing -Winvalid-pch -Wnon-virtual-dtor -Wno-narrowing")
  # TODO: temp for gcc-11 which throws narrowing conversions in Path_GTest.cpp due to int codepoints
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-narrowing")
  # Treat all warnings as errors
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
  if(APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-overloaded-virtual -ftemplate-depth=1024")
  else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-overloaded-virtual")
    if (NOT CMAKE_CXX_COMPILER_ID MATCHES "Clang")
      # DLM: had to add this due to issues with boost optional and gcc, may be resolved if we move to std::optional
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-maybe-uninitialized")
      if (CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-uninitialized")
      endif()
    endif()
  endif()

  # Note: CMAKE_CXX_STANDARD (with no extensions) already sets -std=c++17 (or std=c++1z).
  if(APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
  endif()
endif()

if(MINGW)
  # all warnings
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
  # effective c++
  #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Weffc++")
  # treat warnings as errors
  #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")

  # wchar not supported for MinGW
  #add_definitions(-DBOOST_LOG_USE_CHAR)
  #add_definitions(-DBOOST_LOG_NO_COMPILER_TLS)
endif()

if(MSVC)
  # warning level 3
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W3")

  # warning level 4 - DLM: we should shoot for this
  #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")

  # all warnings - DLM: probably too high to ever use
  #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Wall")

  # create compiler processes on all effective processors
  # /MP is useless for ninja and will cause problems. BUILD_WITH_MULTIPLE_PROCESSES shouldn't have been set if ninja but let's be safe
  if(BUILD_WITH_MULTIPLE_PROCESSES AND NOT NINJA)
    add_definitions(/MP)
  endif()

  # Enable warning on thread un-safe static member initialization - DLM: we have been bit by this many times
  # TODO: JM 2019-09-19: Disabling for now
  # Using the righg flag (/w14460 and not the previously mispelled /w44640) will make compilation fail with many warnings treated as error,
  # eg: IddRegex.cpp all scoped `const static std::string result` inside functions will issue that and make it fatal
  # add_definitions(/w14640)

  # ignore decorated name length exceeded
  add_definitions(/wd4503)

  # ignore needs to have dll-interface to be used by clients of class
  # we will not rely on exporting templates instead all libraries must
  # be linked against the same runtime
  add_definitions(/wd4251)

  # ignore base class not dll-exported error
  add_definitions(/wd4275)

  add_definitions(/bigobj)

  if(CMAKE_SIZEOF_VOID_P EQUAL 8) # 64 bit
    # ignore conversion from size_t to int for now
    add_definitions(/wd4267)
  else()
    # ignore conversion from 'type1' to 'type2', possible loss of data
    add_definitions(/wd4244)
  endif()

  # treat warnings as errors
  # DLM: only do this for our code, added in later
  # add_definitions(/WX)

  # ignore warnings about the stl being insecure
  add_definitions(-D_CRT_SECURE_NO_WARNINGS -D_SCL_SECURE_NO_WARNINGS)

  # ignore c++17 deprecation warnings generated by boost
  add_definitions(-D_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS)
  add_definitions(-D_SILENCE_FPOS_SEEKPOS_DEPRECATION_WARNING)

  # ignore c++20 deprecation warnings generated by fmt
  add_definitions(-D_SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING)
  add_definitions(-D_SILENCE_ALL_MS_EXT_DEPRECATION_WARNINGS)

  add_compile_options(/utf-8)

endif()

##############################################################################


###############################################################################
# Check version of gcc
if(CMAKE_COMPILER_IS_GNUCC)
  execute_process(COMMAND ${CMAKE_C_COMPILER} -dumpversion
                  OUTPUT_VARIABLE GCC_VERSION)
endif()


###############################################################################
# Dependencies


# EnergyPlus

if(UNIX)
  if(APPLE)
    # TODO: temp for #5281 - Resigned with a different entitlements to avoid an issue when we pip install stuff into the E+ dir with native comps (numpy for eg)
    set(ENERGYPLUS_REPO "jmarrec")
    set(ENERGYPLUS_RELEASE_NAME "v24.2.0a-entitlements")

    if (ARCH MATCHES "arm64")
      set(ENERGYPLUS_EXPECTED_HASH 29616d6aa23e2fc0f71362da9794ef08)
      set(ENERGYPLUS_PLATFORM "Darwin-macOS13-arm64")
    else()
      set(ENERGYPLUS_EXPECTED_HASH 21bdad40fbc560b4b382469ef9f96936)
      set(ENERGYPLUS_PLATFORM "Darwin-macOS12.1-x86_64")
    endif()
  elseif(LSB_RELEASE_ID_SHORT MATCHES "CentOS")
    set(ENERGYPLUS_EXPECTED_HASH 4000e06b944daf4f2b0841ac0d3873b5)
    set(ENERGYPLUS_PLATFORM "Linux-CentOS7.9.2009-x86_64")
  else()
    if(LSB_RELEASE_VERSION_SHORT MATCHES "24.04")
      if (ARCH MATCHES "arm64")
        set(ENERGYPLUS_EXPECTED_HASH afc61e89b7f9e2658c3d99778be50be0)
      else()
        set(ENERGYPLUS_EXPECTED_HASH 22b3e5f6071d6011ffeeacf854a97c73)
      endif()
    elseif(LSB_RELEASE_VERSION_SHORT MATCHES "22.04")
      if (ARCH MATCHES "arm64")
        set(ENERGYPLUS_EXPECTED_HASH 3c18cb8063ec30ef591109ddf8842bf0)
      else()
        set(ENERGYPLUS_EXPECTED_HASH 9f11a16e6f8a7d658791c5389d700ef1)
      endif()
    else() # e.g., 18.04, 20.04
      message(FATAL_ERROR "EnergyPlus no longer provides packages for Ubuntu < 22.04")
    endif()
    set(ENERGYPLUS_PLATFORM "Linux${ENERGYPLUS_SYSTEM_VERSION}-${ARCH}")
  endif()
  set(ENERGYPLUS_PATH "EnergyPlus-${ENERGYPLUS_VERSION}-${ENERGYPLUS_BUILD_SHA}-${ENERGYPLUS_PLATFORM}")
  if(EXISTS "${PROJECT_BINARY_DIR}/${ENERGYPLUS_PATH}.tar.gz")
    file(MD5 "${PROJECT_BINARY_DIR}/${ENERGYPLUS_PATH}.tar.gz" ENERGYPLUS_HASH)
  endif()
  if(NOT EXISTS "${PROJECT_BINARY_DIR}/${ENERGYPLUS_PATH}.tar.gz" OR NOT "${ENERGYPLUS_HASH}" MATCHES "${ENERGYPLUS_EXPECTED_HASH}")
    message(STATUS "Downloading EnergyPlus ${ENERGYPLUS_VERSION} (${ENERGYPLUS_PLATFORM})")
    message(STATUS "https://github.com/${ENERGYPLUS_REPO}/EnergyPlus/releases/download/${ENERGYPLUS_RELEASE_NAME}/${ENERGYPLUS_PATH}.tar.gz")

    if(LSB_RELEASE_ID_SHORT MATCHES "CentOS")
      file(DOWNLOAD
           "https://github.com/${ENERGYPLUS_REPO}/EnergyPlus/releases/download/${ENERGYPLUS_RELEASE_NAME}/${ENERGYPLUS_PATH}.tar.gz" "${PROJECT_BINARY_DIR}/${ENERGYPLUS_PATH}.tar.gz"
           # "http://openstudio-resources.s3.amazonaws.com/dependencies/${ENERGYPLUS_PATH}.tar.gz" "${PROJECT_BINARY_DIR}/${ENERGYPLUS_PATH}.tar.gz"
           INACTIVITY_TIMEOUT 300 # 5-min timeout
           SHOW_PROGRESS
           EXPECTED_MD5 ${ENERGYPLUS_EXPECTED_HASH})
    else()
      file(DOWNLOAD "https://github.com/${ENERGYPLUS_REPO}/EnergyPlus/releases/download/${ENERGYPLUS_RELEASE_NAME}/${ENERGYPLUS_PATH}.tar.gz" "${PROJECT_BINARY_DIR}/${ENERGYPLUS_PATH}.tar.gz"
           INACTIVITY_TIMEOUT 300 # 5-min timeout
           SHOW_PROGRESS
           EXPECTED_MD5 ${ENERGYPLUS_EXPECTED_HASH})
    endif()

    execute_process(COMMAND ${CMAKE_COMMAND} -E remove_directory "${PROJECT_BINARY_DIR}/${ENERGYPLUS_PATH}")
    file(REMOVE "${PROJECT_BINARY_DIR}/python/engine/pip_install_done.stamp")
  endif()

elseif(WIN32)
  if(CMAKE_SIZEOF_VOID_P EQUAL 8) # 64 bit
    set(ENERGYPLUS_PATH "EnergyPlus-${ENERGYPLUS_VERSION}-${ENERGYPLUS_BUILD_SHA}-Windows-x86_64")
    set(ENERGYPLUS_ARCH 64)
    set(ENERGYPLUS_EXPECTED_HASH 6a2d1d43d64490c87d894ed58f71ccdc)
  else()
    set(ENERGYPLUS_PATH "EnergyPlus-${ENERGYPLUS_VERSION}-${ENERGYPLUS_BUILD_SHA}-Windows-i386")
    set(ENERGYPLUS_ARCH 32)
    set(ENERGYPLUS_EXPECTED_HASH 7f0c2901399f43bff6e23c01a5992df1)
  endif()
  if(EXISTS "${PROJECT_BINARY_DIR}/${ENERGYPLUS_PATH}.zip")
    file(MD5 "${PROJECT_BINARY_DIR}/${ENERGYPLUS_PATH}.zip" ENERGYPLUS_HASH)
  endif()
  if(NOT EXISTS "${PROJECT_BINARY_DIR}/${ENERGYPLUS_PATH}.zip" OR NOT "${ENERGYPLUS_HASH}" MATCHES "${ENERGYPLUS_EXPECTED_HASH}")
    message(STATUS "Downloading EnergyPlus ${ENERGYPLUS_VERSION} (${ENERGYPLUS_ARCH}-bit)")

    file(DOWNLOAD "https://github.com/${ENERGYPLUS_REPO}/EnergyPlus/releases/download/${ENERGYPLUS_RELEASE_NAME}/${ENERGYPLUS_PATH}.zip" "${PROJECT_BINARY_DIR}/${ENERGYPLUS_PATH}.zip"
         INACTIVITY_TIMEOUT 300 # 5 minute timeout
         SHOW_PROGRESS
         EXPECTED_MD5 ${ENERGYPLUS_EXPECTED_HASH})

    execute_process(COMMAND ${CMAKE_COMMAND} -E remove_directory "${PROJECT_BINARY_DIR}/${ENERGYPLUS_PATH}")
    file(REMOVE "${PROJECT_BINARY_DIR}/python/engine/pip_install_done.stamp")
  endif()

endif()

if((DEFINED ENERGYPLUS_EXE) AND NOT (ENERGYPLUS_EXE STREQUAL ""))
  get_filename_component(OLD_ENERGYPLUS_EXTRACTED_DIR "${ENERGYPLUS_EXE}" DIRECTORY)
  if(NOT "${OLD_ENERGYPLUS_EXTRACTED_DIR}" MATCHES ".*${ENERGYPLUS_PATH}.*")

    message(STATUS "Removing old extracted dir = ${OLD_ENERGYPLUS_EXTRACTED_DIR}")
    # Remove the old extracted dir
    execute_process(COMMAND ${CMAKE_COMMAND} -E remove_directory "${OLD_ENERGYPLUS_EXTRACTED_DIR}")

    # Unset variables so that FindEnergyPlus is called again
    unset(ENERGYPLUS_FOUND CACHE)
    unset(ENERGYPLUS_EXE CACHE)
    unset(ENERGYPLUS_IDD CACHE)
    unset(ENERGYPLUS_WEATHER_DIR CACHE)

  endif()
endif()


# Allows placing the archive manually in the build dir too
if (NOT EXISTS "${ENERGYPLUS_PATH}")
  unset(ENERGYPLUS_FOUND CACHE)
  unset(ENERGYPLUS_EXE CACHE)
  unset(ENERGYPLUS_IDD CACHE)
  unset(ENERGYPLUS_WEATHER_DIR CACHE)
  if (UNIX)
    execute_process(COMMAND ${CMAKE_COMMAND} -E tar xfz "${PROJECT_BINARY_DIR}/${ENERGYPLUS_PATH}.tar.gz" WORKING_DIRECTORY "${PROJECT_BINARY_DIR}")
  else()
    execute_process(COMMAND ${CMAKE_COMMAND} -E tar xfz "${PROJECT_BINARY_DIR}/${ENERGYPLUS_PATH}.zip" WORKING_DIRECTORY "${PROJECT_BINARY_DIR}")
  endif()
endif()

# Prior to E+ 9.1.0 (OS 2.7.2), zip/tar.gz would extract to something like `./EnergyPlus-9.0.0-buildsha-Linux-x86_64/EnergyPlus-9-0-0`
if (ENERGYPLUS_VERSION VERSION_LESS "9.1.0")
  set(ENV{ENERGYPLUSDIR} "${PROJECT_BINARY_DIR}/${ENERGYPLUS_PATH}/EnergyPlus-${ENERGYPLUS_VERSION_MAJOR}-${ENERGYPLUS_VERSION_MINOR}-${ENERGYPLUS_VERSION_PATCH}")
else()
  # After 9.1.0, due to QtIFW/Mac packaging (NREL/EnergPlus#7148), it is going directly to `./EnergyPlus-9.1.0-buildsha-Linux-x86_64/`
  set(ENV{ENERGYPLUSDIR} "${PROJECT_BINARY_DIR}/${ENERGYPLUS_PATH}")
endif()

# Find EnergyPlus, which will set the ENERGYPLUS_XXX variables and cache them
find_package(EnergyPlus "${ENERGYPLUS_VERSION}" REQUIRED MODULE)
get_filename_component(ENERGYPLUS_DIR "${ENERGYPLUS_EXE}" DIRECTORY)

# Radiance
if(UNIX)
  if(APPLE)
    set(RADIANCE_EXPECTED_HASH 115ef9052ad5011d3fc7cebc2991644f)
    set(RADIANCE_PLATFORM "Darwin")
  elseif(EXISTS "/etc/redhat-release")
    # DLM: note that the installer expects the extracted directory to be named radiance-5.0.a.12-Linux
    set(RADIANCE_EXPECTED_HASH 1e0ef7909c99cd353191d63b085dc6a9)
    set(RADIANCE_PLATFORM "Redhat")
  else()
    set(RADIANCE_EXPECTED_HASH 7b9e4df2b6050fe51c6e138e4c0e6a5a)
    set(RADIANCE_PLATFORM "Linux")
  endif()
  set(RADIANCE_PATH "radiance-${RADIANCE_VERSION}-${RADIANCE_PLATFORM}")
  if(EXISTS "${PROJECT_BINARY_DIR}/${RADIANCE_PATH}.tar.gz")
    file(MD5 "${PROJECT_BINARY_DIR}/${RADIANCE_PATH}.tar.gz" RADIANCE_HASH)
  endif()
  if(NOT EXISTS "${PROJECT_BINARY_DIR}/${RADIANCE_PATH}.tar.gz" OR NOT "${RADIANCE_HASH}" MATCHES "${RADIANCE_EXPECTED_HASH}")
  message(STATUS "Downloading Radiance ${RADIANCE_VERSION} (${RADIANCE_PLATFORM})")
  if(EXISTS "/etc/redhat-release" OR LSB_RELEASE_ID_SHORT MATCHES "CentOS")
    message("http://openstudio-resources.s3.amazonaws.com/dependencies/radiance-${RADIANCE_VERSION}-${RADIANCE_PLATFORM}.tar.gz")
    file(DOWNLOAD "http://openstudio-resources.s3.amazonaws.com/dependencies/radiance-${RADIANCE_VERSION}-${RADIANCE_PLATFORM}.tar.gz" "${PROJECT_BINARY_DIR}/${RADIANCE_PATH}.tar.gz" TIMEOUT 120 INACTIVITY_TIMEOUT 120 SHOW_PROGRESS EXPECTED_MD5 ${RADIANCE_EXPECTED_HASH})
  else()
    file(DOWNLOAD "http://github.com/NREL/Radiance/releases/download/${RADIANCE_VERSION}/radiance-${RADIANCE_VERSION}-${RADIANCE_PLATFORM}.tar.gz" "${PROJECT_BINARY_DIR}/${RADIANCE_PATH}.tar.gz" TIMEOUT 120 INACTIVITY_TIMEOUT 120 SHOW_PROGRESS EXPECTED_MD5 ${RADIANCE_EXPECTED_HASH})
  endif()
    execute_process(COMMAND ${CMAKE_COMMAND} -E remove_directory "${PROJECT_BINARY_DIR}/${RADIANCE_PATH}")

  endif()
  set(RADIANCE_LOCATION "${PROJECT_BINARY_DIR}/radiance-${RADIANCE_VERSION}-${RADIANCE_PLATFORM}/usr/local/radiance")
elseif(WIN32)
  if(CMAKE_SIZEOF_VOID_P EQUAL 8) # 64 bit
    set(RADIANCE_ARCH 64)
    set(RADIANCE_EXPECTED_HASH 7bd7a418d181365b4153c8bc146ede06)
  else()
    message(WARNING "32 bit Radiance build is not available")
    message(WARNING "Using 64 bit build of Radiance with a 32 bit build of OpenStudio")
    set(RADIANCE_ARCH 64)
    set(RADIANCE_EXPECTED_HASH 7bd7a418d181365b4153c8bc146ede06)
    #set(RADIANCE_ARCH 32)
    #set(RADIANCE_EXPECTED_HASH 78eda5d4b0215cd675cc3ec8975e6122)
  endif()
  set(RADIANCE_PATH "radiance-${RADIANCE_VERSION}-win${RADIANCE_ARCH}")
  set(RADIANCE_PLATFORM "win${RADIANCE_ARCH}")
  if(EXISTS "${PROJECT_BINARY_DIR}/${RADIANCE_PATH}.zip")
    file(MD5 "${PROJECT_BINARY_DIR}/${RADIANCE_PATH}.zip" RADIANCE_HASH)
  endif()
  if(NOT EXISTS "${PROJECT_BINARY_DIR}/${RADIANCE_PATH}.zip" OR NOT "${RADIANCE_HASH}" MATCHES "${RADIANCE_EXPECTED_HASH}")
    message(STATUS "Downloading Radiance ${RADIANCE_VERSION} (${RADIANCE_ARCH}-bit)")
    file(DOWNLOAD "http://github.com/NREL/Radiance/releases/download/${RADIANCE_VERSION}/Radiance-${RADIANCE_VERSION}-win${RADIANCE_ARCH}.zip" "${PROJECT_BINARY_DIR}/${RADIANCE_PATH}.zip" TIMEOUT 120 INACTIVITY_TIMEOUT 120 SHOW_PROGRESS EXPECTED_MD5 ${RADIANCE_EXPECTED_HASH})
    execute_process(COMMAND ${CMAKE_COMMAND} -E remove_directory "${PROJECT_BINARY_DIR}/${RADIANCE_PATH}")
  endif()
  set(RADIANCE_LOCATION "${PROJECT_BINARY_DIR}/radiance-${RADIANCE_VERSION}-${RADIANCE_PLATFORM}/")
endif()

# Allows placing the archive manually in the build dir too
if (NOT EXISTS "${PROJECT_BINARY_DIR}/radiance-${RADIANCE_VERSION}-${RADIANCE_PLATFORM}")
  if (UNIX)
    execute_process(COMMAND ${CMAKE_COMMAND} -E tar xfz "${PROJECT_BINARY_DIR}/${RADIANCE_PATH}.tar.gz" WORKING_DIRECTORY "${PROJECT_BINARY_DIR}")
  else()
    execute_process(COMMAND ${CMAKE_COMMAND} -E tar xfz "${PROJECT_BINARY_DIR}/${RADIANCE_PATH}.zip" WORKING_DIRECTORY "${PROJECT_BINARY_DIR}")
  endif()
endif()

# Perl
if(NOT UNIX)
  execute_process(COMMAND ${CMAKE_COMMAND} -E tar xfz "${PROJECT_SOURCE_DIR}/dependencies/strawberry-perl-5.16.2.1-32bit-portable-reduced.tar.bz2"
    WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
    RESULT_VARIABLE rv)
  set(PERL_EXE "${PROJECT_BINARY_DIR}/strawberry-perl-5.16.2.1-32bit-portable-reduced/perl/bin/perl.exe")
endif()

include(FetchRubyMinGW.cmake)
FetchRubyMinGW()

if(BUILD_CLI)
  # CI will build and upload openstudio gems to s3 at https://github.com/NREL/openstudio-gems
  # need to  update the MD5sum for each platform and url below

  set(OPENSTUDIO_GEMS_BASEURL "http://openstudio-resources.s3.amazonaws.com/dependencies")

  # TODO: temp
  set(OPENSTUDIO_GEMS_BASEURL "https://github.com/NREL/openstudio-gems/releases/download/v3.9.0")

  # To use the package produced by a PR to https://github.com/NREL/openstudio-gems
  set(USE_OPENSTUDIO_GEMS_PR FALSE)
  if (USE_OPENSTUDIO_GEMS_PR)
    set(OPENSTUDIO_GEMS_BASEURL "http://openstudio-sdk-dependencies.s3.amazonaws.com/openstudio-gems")
    set(OPENSTUDIO_GEMS_PR_NUMBER "PR-67")
  endif()

  if(UNIX)
     if(APPLE)
       if (ARCH MATCHES arm64)
         set(OPENSTUDIO_GEMS_ZIP_FILENAME "openstudio3-gems-20241114-darwin_arm64-3.2.2.tar.gz")
         set(OPENSTUDIO_GEMS_ZIP_EXPECTED_MD5 "63ae214118b20ce98bc8552c8e151e0d")
       else()
         set(OPENSTUDIO_GEMS_ZIP_FILENAME "openstudio3-gems-20241114-darwin-3.2.2.tar.gz")
         set(OPENSTUDIO_GEMS_ZIP_EXPECTED_MD5 "53c20b79ee601eeb347cf9e5c1986943")
       endif()
     else()
       if (ARCH MATCHES "arm64")
         set(OPENSTUDIO_GEMS_ZIP_FILENAME "openstudio3-gems-20241114-linux_arm64-3.2.2.tar.gz")
         set(OPENSTUDIO_GEMS_ZIP_EXPECTED_MD5 "782980448f88ef5383536cada86d01d5")
       else()
         set(OPENSTUDIO_GEMS_ZIP_FILENAME "openstudio3-gems-20241114-linux-3.2.2.tar.gz")
         set(OPENSTUDIO_GEMS_ZIP_EXPECTED_MD5 "37ab31c45fbb71421f94a05bcea61b21")
       endif()
       if (USE_OPENSTUDIO_GEMS_PR)
         set(OPENSTUDIO_GEMS_BASEURL "${OPENSTUDIO_GEMS_BASEURL}/openstudio-gems-linux/${OPENSTUDIO_GEMS_PR_NUMBER}")
       endif()
     endif()
  elseif(WIN32)
    # OpenStudio gems are only supported on 64 bit windows
    set(OPENSTUDIO_GEMS_ZIP_FILENAME "openstudio3-gems-20241114-windows-3.2.2.tar.gz")
    set(OPENSTUDIO_GEMS_ZIP_EXPECTED_MD5 "0029fe4e06dd9ffef39dff4908237357")
    if (USE_OPENSTUDIO_GEMS_PR)
      set(OPENSTUDIO_GEMS_BASEURL "${OPENSTUDIO_GEMS_BASEURL}/openstudio-gems-windows/${OPENSTUDIO_GEMS_PR_NUMBER}")
    endif()
  endif()

  set(OPENSTUDIO_GEMS_ZIP_LOCAL_PATH "${PROJECT_BINARY_DIR}/${OPENSTUDIO_GEMS_ZIP_FILENAME}")
  set(OPENSTUDIO_GEMS_DIR "${PROJECT_BINARY_DIR}/openstudio-gems")
  set(OPENSTUDIO_GEMS_TARGETS_FILE "${OPENSTUDIO_GEMS_DIR}/export-extensions.cmake")
  set(OPENSTUDIO_GEMS_INIT_FILE "${OPENSTUDIO_GEMS_DIR}/init-extensions.cpp")
  if(EXISTS "${OPENSTUDIO_GEMS_ZIP_LOCAL_PATH}")
    file(MD5 "${OPENSTUDIO_GEMS_ZIP_LOCAL_PATH}" OPENSTUDIO_GEMS_ZIP_MD5)
  endif()

  # If you do not have the right tar.gz: download, and remove extracted dir
  if(NOT OPENSTUDIO_GEMS_ZIP_MD5 STREQUAL OPENSTUDIO_GEMS_ZIP_EXPECTED_MD5)
    message(STATUS "Downloading OpenStudio Gems: ${OPENSTUDIO_GEMS_BASEURL}/${OPENSTUDIO_GEMS_ZIP_FILENAME}")
    file(
      DOWNLOAD "${OPENSTUDIO_GEMS_BASEURL}/${OPENSTUDIO_GEMS_ZIP_FILENAME}"
      ${OPENSTUDIO_GEMS_ZIP_LOCAL_PATH}
      INACTIVITY_TIMEOUT 300 # 5 minute timeout
      SHOW_PROGRESS
      EXPECTED_MD5 ${OPENSTUDIO_GEMS_ZIP_EXPECTED_MD5})

    file(REMOVE_RECURSE "${OPENSTUDIO_GEMS_DIR}")
  endif()

  # If the extracted dir has a version.txt, it must match, otherwise remove it
  if(EXISTS "${OPENSTUDIO_GEMS_DIR}/version.txt")
    file(READ ${OPENSTUDIO_GEMS_DIR}/version.txt EXTRACTED_TAR_GZ_NAME)
    string(STRIP ${EXTRACTED_TAR_GZ_NAME} EXTRACTED_TAR_GZ_NAME)
    if(NOT EXTRACTED_TAR_GZ_NAME STREQUAL OPENSTUDIO_GEMS_ZIP_FILENAME)
      message("Extracted gem dir ${OPENSTUDIO_GEMS_DIR} has the wrong version, removing it")
      file(REMOVE_RECURSE "${OPENSTUDIO_GEMS_DIR}")
    endif()
  else()
    # Until the openstudio-gems package from my PR-51 goes onto develop, assume you want to remove it to avoid leaving it in place when you switch back to develop
    if(EXISTS "${OPENSTUDIO_GEMS_DIR}" AND NOT USE_OPENSTUDIO_GEMS_PR)
      file(REMOVE_RECURSE "${OPENSTUDIO_GEMS_DIR}")
    endif()
  endif()

  # If you do not have the extracted dir, then extract the tar.gz
  if(NOT EXISTS "${OPENSTUDIO_GEMS_DIR}")
    execute_process(COMMAND ${CMAKE_COMMAND} -E tar xfz ${OPENSTUDIO_GEMS_ZIP_LOCAL_PATH} WORKING_DIRECTORY "${PROJECT_BINARY_DIR}")

    file(REMOVE_RECURSE "${PROJECT_BINARY_DIR}/src/cli/embedded_files")
  endif()
endif()

# SWIG

# Note: JM 2019-10-16
# We use conan swig_installer 4.0 which enhances our PATH to include the CONAN_BIN_DIRS_SWIG_INSTALLER already
# So I could just do:
# set(SWIG_EXECUTABLE swig)
# But will have the probblem with the swiglib anyways.
# Plus, it doesn't appear to work on windows either, the PATH doesn't include it.
#find_program(SWIG_EXECUTABLE swig PATHS CURRENT_CONAN_SWIG_ROOT NO_DEFAULT_PATH)
#if(NOT SWIG_EXECUTABLE)
#  message(FATAL_ERROR "Couldn't find swig, which shouldn't happen. CONAN_BIN_DIRS_SWIG_INSTALLER=${CONAN_BIN_DIRS_SWIG_INSTALLER}, CONAN_SWIG_ROOT_RELEASE=${CONAN_SWIG_ROOT_RELEASE}, CURRENT_CONAN_SWIG_ROOT=${CURRENT_CONAN_SWIG_ROOT}, ${ENV{SWIG_DIR}}, ${ENV{SWIG_INSTALLER_ROOT}}")
#endif()

# Another thing I tried is to use the `virtualenv` generator (`conan_cmake_run( ... GENERATORS virtualenv)`),
# but you end up having to call the activation script on each call, and the call is dependent on the platform which is even more annoying
# (either cmake -E cmd /C activate.bat, or cmake -E source activate.sh)
# I also tried using CMake to extract the value from the activate.sh script on all plaftorms but it seems even more of a hack

# So instead, we use a generator expression to find the right value. At least it makes sense, and should be robust.
FindValue(swig_PACKAGE_FOLDER)

# set(SWIG_EXECUTABLE "${CURRENT_swig_PACKAGE_FOLDER}/bin/swig")

# The conan-provided binary has a built-in swiglib (`swig -swiglib`) that points to the build box on which it was built, which is problematic for us.
# set(SWIG_DIR "${CURRENT_swig_PACKAGE_FOLDER}/bin/swiglib")

# Export SWIG to parent scope
if(hasParent)
  set(SWIG_EXECUTABLE "${SWIG_EXECUTABLE}" PARENT_SCOPE)
  set(SWIG_DIR "${SWIG_DIR}" PARENT_SCOPE)
endif()

set(ALL_RUBY_BINDING_TARGETS "") # global list

# build rdoc
if(BUILD_DOCUMENTATION)
  if(WIN32)
    get_filename_component(RUBY_BIN_DIR "${RUBY_EXECUTABLE}" DIRECTORY)
    find_file(RDOC NAMES rdoc.bat HINTS $ENV{PATH} "${RUBY_BIN_DIR}")
    find_file(RI NAMES ri.bat HINTS $ENV{PATH} "${RUBY_BIN_DIR}")
  else()
    find_program(RDOC NAMES rdoc)
    find_program(RI NAMES ri)
  endif()

  mark_as_advanced(
    RDOC
    RI
  )

  set(ALL_RDOC_TARGETS ${ALL_RUBY_BINDING_TARGETS})
endif()

# C#
if(BUILD_CSHARP_BINDINGS)
  set(ALL_CSHARP_BINDING_DEPENDS "") # global list of library dependencies of the generated wrapper cxx files
  set(ALL_CSHARP_WRAPPER_FILES "") # global list of generated wrapper cxx files
  set(ALL_CSHARP_WRAPPER_TARGETS "") # global list targets that build generated wrapper cxx files
endif()

if(BUILD_NODE_MODULES)
  add_definitions(-DBUILD_NODE_MODULE)
endif()

if(BUILD_PYTHON_BINDINGS)
  include(python/SetupPython.cmake)
endif()

# Threading library
find_package(Threads)
if(UNIX)
  set(CMAKE_THREAD_LIBS "${CMAKE_THREAD_LIBS_INIT}" CACHE STRING "Thread library used.")
  mark_as_advanced(CMAKE_THREAD_LIBS)
endif()

###############################################################################
# Add to include path

# Project source directory
include_directories("${PROJECT_SOURCE_DIR}/src/")

# Project binary directory
include_directories("${PROJECT_BINARY_DIR}/src/")

###############################################################################

###############################################################################
# Add project sub directories

if(MSVC)
  # treat warnings as errors
  add_definitions(/WX)
elseif()
  add_definitions(-Werror)
endif()

include("embedded/EmbedFiles.cmake")
add_subdirectory("embedded")
if(BUILD_PAT)
  add_subdirectory("pat")
endif()

# Each subfolder of src is a major sub-project
set(project_directories
  generateiddfactory
  utilities
  model
  energyplus
  epjson
  radiance
  gbxml
  gltf
  airflow
  isomodel
  osversion
  alfalfa
  measure
  sdd
  lib
  install_utility
  scriptengine
  workflow
)

# resources must be added after find EnergyPlus
add_subdirectory(resources)

foreach(D ${project_directories})
  add_subdirectory(src/${D})
endforeach()

if(BUILD_RUBY_BINDINGS)
  add_subdirectory(ruby)
endif()

if(BUILD_CLI)
  add_subdirectory(src/cli)
endif()

add_dependencies(openstudio_utilities
  CreateEmbeddedSource
)

# csharp, after loading projects
if(BUILD_CSHARP_BINDINGS)
  add_subdirectory(csharp)
endif()

# python, after loading projects
if(BUILD_PYTHON_BINDINGS)
  add_subdirectory(python)
endif()

###############################################################################

###############################################################################
# Targets that combine all bindings targets
if(BUILD_RUBY_BINDINGS)
  add_custom_target(ALL_RUBY_BINDINGS)
  add_dependencies(ALL_RUBY_BINDINGS ${ALL_RUBY_BINDING_TARGETS})
endif()

if(BUILD_PYTHON_BINDINGS)
  add_custom_target(ALL_PYTHON_BINDINGS)
  add_dependencies(ALL_PYTHON_BINDINGS ${ALL_PYTHON_BINDING_TARGETS})
endif()


if(BUILD_CSHARP_BINDINGS)
  add_custom_target(ALL_CSHARP_BINDINGS)
  add_dependencies(ALL_CSHARP_BINDINGS csharp_sdk ${ALL_CSHARP_BINDING_TARGETS})
endif()


set(MAXIMIZE_CPU_USAGE OFF CACHE BOOL "Attempt to fully load the CPU during builds")
mark_as_advanced(MAXIMIZE_CPU_USAGE)


###############################################################################
# Targets that combine all documentation targets

# Doxygen
if(BUILD_DOCUMENTATION)
  # need doxygen, 1.8.7...<1.8.9 is what it's tested on
  find_package(Doxygen REQUIRED) # Should we do `REQUIRED` dot here?
  if(DOXYGEN_VERSION VERSION_LESS 1.8.7 OR DOXYGEN_VERSION VERSION_GREATER_EQUAL 1.8.9)
    message(AUTHOR_WARNING "Found Doxygen version ${DOXYGEN_VERSION}: the historically tested range is 1.8.7...<1.8.9 so it's possible the resulting documentation (CSS or search feature in particular) will be broken (some tests have been conducted with 1.10.0)")
  endif()

  # at 1.9.5: searchLabel removed
  # at 1.10.0: js was modernized, using let and not var
  set(DOXYGEN_SEARCH_PATCH_FILE "${PROJECT_SOURCE_DIR}/doc/patch/search.js.patch")
  if(DOXYGEN_VERSION VERSION_GREATER_EQUAL 1.10.0)
    set(DOXYGEN_SEARCH_PATCH_FILE "${PROJECT_SOURCE_DIR}/doc/patch/search-1.10.0.js.patch")
  elseif(DOXYGEN_VERSION VERSION_GREATER_EQUAL 1.9.5)
    set(DOXYGEN_SEARCH_PATCH_FILE "${PROJECT_SOURCE_DIR}/doc/patch/search-1.9.5.js.patch")
  endif()

  set(DOXYGEN_HTML_HEADER "${PROJECT_SOURCE_DIR}/doc/html/header.html")
  if (DOXYGEN_VERSION VERSION_GREATER_EQUAL 1.8.9)
    set(DOXYGEN_HTML_HEADER "${PROJECT_SOURCE_DIR}/doc/html/header-1.8.9.html") # Adds searchdata.js
  endif()

  configure_file(${PROJECT_SOURCE_DIR}/doc/index.html ${PROJECT_BINARY_DIR}/doc/index.html COPYONLY)

  set(doxygen_directories ${project_directories})
  # Add the idd files to the documentation list as a separate entry
  list(APPEND doxygen_directories utilities/idd)

  # Remove directories we don't want documentation generated for
  list(REMOVE_ITEM doxygen_directories generateiddfactory)

  add_custom_target(openstudio_doc_resources
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECT_SOURCE_DIR}/doc/css" "css"
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECT_SOURCE_DIR}/doc/fonts" "fonts"
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECT_SOURCE_DIR}/doc/js" "js"
    WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/doc
  )

  foreach(D ${doxygen_directories})
    string(REPLACE "/" "_" DOC_NAME ${D})

    set(DOXY_INPUT_DIR "\"${PROJECT_SOURCE_DIR}/src/${D}\"")
    set(DOXY_ADDITIONAL_DIR "")
    set(DOXY_EXCLUDE_DIR "")
    set(DOXY_TAGFILES "")


    # Specific settings
    if(${D} STREQUAL "utilities/idd")
      # We want to build the idd documentation separately because of the generated files
      set(DOXY_ADDITIONAL_DIR "\"${PROJECT_BINARY_DIR}/src/${D}\"")
    elseif(${D} STREQUAL "utilities")
      # Exclude the idd directory when building the utilities documentation
      set(DOXY_EXCLUDE_DIR "\"${PROJECT_SOURCE_DIR}/src/utilities/idd\"")
      set(DOXY_TAGFILES "utilities_idd/utilities_idd.tag=../../utilities_idd/html")
    elseif(${D} STREQUAL "model")
      set(DOXY_TAGFILES "utilities_idd/utilities_idd.tag=../../utilities_idd/html utilities/utilities.tag=../../utilities/html")
    endif()
    configure_file(${PROJECT_SOURCE_DIR}/Doxyfile.in ${PROJECT_BINARY_DIR}/doc/${DOC_NAME}.cfg)

    add_custom_target(openstudio_${DOC_NAME}_doc
      COMMAND ${CMAKE_COMMAND} -E remove_directory "${DOC_NAME}"
      COMMAND ${CMAKE_COMMAND} -E make_directory "${DOC_NAME}"
      COMMAND ${DOXYGEN_EXECUTABLE} ${DOC_NAME}.cfg
      COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECT_SOURCE_DIR}/doc/search" "${DOC_NAME}/html/search"
      COMMAND ${PATCH_EXE} "${DOC_NAME}/html/search/search.js" < "${DOXYGEN_SEARCH_PATCH_FILE}"
      DEPENDS openstudio_doc_resources
      WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/doc
    )

    set(ALL_CPP_DOC_TARGETS ${ALL_CPP_DOC_TARGETS}
      openstudio_${DOC_NAME}_doc
    )

  endforeach()

  # Set up dependencies after all targets have been created
  add_dependencies(openstudio_utilities_doc
    openstudio_utilities_idd_doc
  )
  add_dependencies(openstudio_model_doc
    openstudio_utilities_doc
    openstudio_utilities_idd_doc
  )

  add_custom_target(ALL_DOXYGEN)
  add_dependencies(ALL_DOXYGEN ${ALL_CPP_DOC_TARGETS})

  add_custom_target(ALL_RDOC)
  add_dependencies(ALL_RDOC ${ALL_RDOC_TARGETS})

  # Zip documentation
  if(WIN32)
    if(NOT EXISTS "${PROJECT_BINARY_DIR}/7za.exe")
      message(STATUS "Downloading 7za.zip")
      file(DOWNLOAD "http://openstudio-resources.s3.amazonaws.com/dependencies/7za.zip" "${PROJECT_BINARY_DIR}/7za.zip" TIMEOUT 120 INACTIVITY_TIMEOUT 120 EXPECTED_MD5 860077d3f314e939abab680218a3febe)
      execute_process(COMMAND ${CMAKE_COMMAND} -E tar xfz "${PROJECT_BINARY_DIR}/7za.zip" WORKING_DIRECTORY "${PROJECT_BINARY_DIR}")
    endif()
    add_custom_command(TARGET ALL_DOXYGEN
      POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E remove OpenStudio-${OpenStudio_VERSION}-doc.zip
      COMMAND ${PROJECT_BINARY_DIR}/7za a -mmt -mx9 -tzip OpenStudio-${OpenStudio_VERSION}-doc.zip * -r -x!*.cfg -x!*.tag -x!*.map -x!*.md5
      COMMAND ${PROJECT_BINARY_DIR}/7za d OpenStudio-${OpenStudio_VERSION}-doc.zip index.html
      WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/doc
    )
    add_custom_command(TARGET ALL_RDOC
      POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E remove OpenStudio-${OpenStudio_VERSION}-rdoc.zip
      COMMAND ${PROJECT_BINARY_DIR}/7za a -mmt -mx9 -tzip OpenStudio-${OpenStudio_VERSION}-rdoc.zip * -r
      WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/rdoc
    )
  endif()

endif()


###############################################################################
# Export targets and generate OpenStudioCoreConfig.cmake

configure_file("${PROJECT_SOURCE_DIR}/OpenStudioCoreConfig.cmake.in"
  "${PROJECT_BINARY_DIR}/OpenStudioCoreConfig.cmake" @ONLY)

if(UNIX AND NOT APPLE)
  set(examplesdir share/openstudio-${OpenStudio_VERSION}/examples)
else()
  set(examplesdir Examples)
endif()

# Install additional Documents, such as release notes
install(FILES "${PROJECT_SOURCE_DIR}/developer/doc/ReleaseNotes/OpenStudio_Release_Notes_3_7_0_20231117.pdf" DESTINATION .)
install(FILES "${PROJECT_SOURCE_DIR}/LICENSE.md" DESTINATION .)
install(FILES "${PROJECT_SOURCE_DIR}/copyright.txt" DESTINATION .)

###############################################################################

if(NOT UNIX)
  install(DIRECTORY "${PROJECT_BINARY_DIR}/strawberry-perl-5.16.2.1-32bit-portable-reduced/perl" DESTINATION ./Perl/ COMPONENT Radiance USE_SOURCE_PERMISSIONS)

  #set(CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS_SKIP ON)
  include(InstallRequiredSystemLibraries)
  if(CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS)
    install(FILES ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS} DESTINATION "bin")
    install(FILES ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS} DESTINATION "Ruby" COMPONENT "RubyAPI")
    if (BUILD_CSHARP_BINDINGS)
      install(FILES ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS} DESTINATION "CSharp/openstudio/" COMPONENT "CSharpAPI")
    endif()
  endif()
endif()

set(CPACK_PACKAGE_VENDOR "National Renewable Energy Laboratory")
set(CPACK_PACKAGE_VERSION_MAJOR ${OpenStudio_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${OpenStudio_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${OpenStudio_VERSION_PATCH}) # This one includes the prerelease tag if any
# set(CPACK_DEBIAN_PACKAGE_DEPENDS "")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libgomp1, libx11-6") # This is for EnergyPlus

# Default the debian package name to include version to allow several versions to be installed concurrently instead of overwriting any existing one
# CMAKE_PROJECT_HOMEPAGE_URL
set(CPACK_DEBIAN_PACKAGE_NAME "openstudio-${OpenStudio_VERSION}")
set(CPACK_RPM_PACKAGE_NAME "openstudio-${OpenStudio_VERSION}")
# CPACK_DEBIAN_PACKAGE_DESCRIPTION defaults to this one too. dpkg-deb -I xxx.deb will show this description
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "OpenStudio is a cross-platform collection of software tools to support whole building energy modeling using EnergyPlus and advanced daylight analysis using Radiance")
set(CPACK_DEBIAN_PACKAGE_HOMEPAGE "https://www.openstudio.net")
set(CPACK_RPM_PACKAGE_URL "https://www.openstudio.net")

# The actual .deb file name on disk
set(CPACK_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}-${OPENSTUDIO_LONG_VERSION}-${CMAKE_SYSTEM_NAME}")
set(CPACK_PACKAGE_CONTACT "openstudio@nrel.gov")

if(APPLE)
  set(CPACK_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}-${OPENSTUDIO_LONG_VERSION}-${CMAKE_SYSTEM_NAME}-${ARCH}")
  set(CPACK_IFW_TARGET_DIRECTORY /Applications/OpenStudio-${OpenStudio_VERSION}/)
elseif(UNIX)
  # Default method doesn't use IFW but Deb, so this one is probably useless (but harmless)
  set(CPACK_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}-${OPENSTUDIO_LONG_VERSION}-${LSB_RELEASE_ID_SHORT}-${LSB_RELEASE_VERSION_SHORT}-${ARCH}")
  set(CPACK_IFW_TARGET_DIRECTORY /usr/local/openstudio-${OpenStudio_VERSION}/)
  # These two will set the .deb install path correctly
  set(CPACK_SET_DESTDIR ON)
  set(CPACK_INSTALL_PREFIX /usr/local/openstudio-${OpenStudio_VERSION}/)
  if(LSB_RELEASE_ID_SHORT MATCHES "CentOS")
    set(CPACK_RPM_PACKAGE_RELOCATABLE OFF)
  endif()

  # Add a symlink to the CLI: /usr/local/bin/openstudio should point to /usr/local/openstudio-${OpenStudio_VERSION}/bin/openstudio,
  # UNLESS it's from the OpenStudioApplication
  if (NOT hasParent)
    # # Add an arbitrary link (broken) at build/openstudio (if build/ is your build folder) that already points to the **future** /usr/local/openstudio-x.y.z/bin/openstudio
    # execute_process(COMMAND ln -sf ${CPACK_INSTALL_PREFIX}/bin/openstudio ${CMAKE_CURRENT_BINARY_DIR}/openstudio)
    # # Plus a versioned one
    # execute_process(COMMAND ln -sf ${CPACK_INSTALL_PREFIX}/bin/openstudio ${CMAKE_CURRENT_BINARY_DIR}/openstudio-${OpenStudio_VERSION})
    # # Have this link be installed with the .deb package in /usr/local/bin
    # install(FILES ${CMAKE_CURRENT_BINARY_DIR}/openstudio DESTINATION /usr/local/bin COMPONENT CLI)
    # install(FILES ${CMAKE_CURRENT_BINARY_DIR}/openstudio-${OpenStudio_VERSION} DESTINATION /usr/local/bin COMPONENT CLI)

    # We use a postinstallation script so we don't get conflicts when we have OSApp installed first (or another OpenStudio (sdk) version)
    set(POSTINST_FILE "${PROJECT_BINARY_DIR}/postinst")
    set(POSTRM_FILE "${PROJECT_BINARY_DIR}/postrm")
    configure_file(${PROJECT_SOURCE_DIR}/debian/postinst.in ${POSTINST_FILE} @ONLY)
    configure_file(${PROJECT_SOURCE_DIR}/debian/postrm.in ${POSTRM_FILE} @ONLY)
    execute_process(COMMAND chmod 755 "${POSTINST_FILE}")
    execute_process(COMMAND chmod 755 "${POSTRM_FILE}")

    set(CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA "${CPACK_DEBIAN_BIN_PACKAGE_CONTROL_EXTRA};${POSTINST_FILE};${POSTRM_FILE}")
    set(CPACK_RPM_POST_INSTALL_SCRIPT_FILE "${POSTINST_FILE}")
    set(CPACK_RPM_POST_UNINSTALL_SCRIPT_FILE "${POSTRM_FILE}")
  endif()

  # TODO: for now since Mac and Windows installers aren't doing it for core, not doing it for Unix either.
  ## Copy the icons
  # set(ICON_FOLDER "${PROJECT_SOURCE_DIR}/icons")
  ## Cf. http://standards.freedesktop.org/icon-theme-spec/icon-theme-spec-latest.html#directory_layout
  #foreach(_size 16 32 48 64 128 256)

  #  # Install OSM mimetypes icons
  #  install(
  #    FILES "${ICON_FOLDER}/osm_${_size}.png"
  #    DESTINATION "/usr/share/icons/hicolor/${_size}x${_size}/mimetypes"
  #    RENAME application-x-openstudio.png
  #  )
  #endforeach()

endif()

if(WIN32)
  set(CPACK_PACKAGE_INSTALL_DIRECTORY "openstudio-${OpenStudio_VERSION}")
  set(CPACK_IFW_TARGET_DIRECTORY "C:/${CPACK_PACKAGE_INSTALL_DIRECTORY}")
  set(CPACK_BINARY_IFW ON CACHE BOOL "Enable to build IFW packages")
  set(CPACK_BINARY_NSIS OFF CACHE BOOL "Enable to build NSIS packages")
endif()

if(LSB_RELEASE_ID_SHORT MATCHES "CentOS")
  set(CPACK_RPM_SPEC_MORE_DEFINE "%define __python python3")
  # tk (wish), tcsh and perl for radiance
  set(CPACK_RPM_PACKAGE_REQUIRES "libicu libicu-devel tk tcsh perl")
endif()

message(STATUS "Installer name is set to '${CPACK_PACKAGE_FILE_NAME}'")

if (BUILD_CLI)
  install(FILES "${PROJECT_BINARY_DIR}/openstudio-gems/Gemfile" DESTINATION ./Ruby/ COMPONENT "RubyAPI")
  install(FILES "${PROJECT_BINARY_DIR}/openstudio-gems/Gemfile.lock" DESTINATION ./Ruby/ COMPONENT "RubyAPI")
  install(FILES "${PROJECT_BINARY_DIR}/openstudio-gems/openstudio-gems.gemspec" DESTINATION ./Ruby/ COMPONENT "RubyAPI" )
endif()

file(GLOB ENERGYPLUS_FILES "${ENERGYPLUS_DIR}/energyplus*")
file(GLOB ENERGYPLUS_LIB_FILES "${ENERGYPLUS_DIR}/libenergyplus*")
file(GLOB EXPAND_OBJECTS "${ENERGYPLUS_DIR}/ExpandObjects*")

install(FILES "${ENERGYPLUS_IDD}" DESTINATION ./EnergyPlus/ COMPONENT EnergyPlus)
# Get epJSON schema too
string(REPLACE ".idd" ".schema.epJSON" ENERGYPLUS_EPJSON_SCHEMA ${ENERGYPLUS_IDD})
install(FILES "${ENERGYPLUS_EPJSON_SCHEMA}" DESTINATION ./EnergyPlus/ COMPONENT EnergyPlus)
install(PROGRAMS ${ENERGYPLUS_FILES} DESTINATION ./EnergyPlus/ COMPONENT EnergyPlus)
install(PROGRAMS ${ENERGYPLUS_LIB_FILES} DESTINATION ./EnergyPlus/ COMPONENT EnergyPlus)
install(PROGRAMS "${EXPAND_OBJECTS}" DESTINATION ./EnergyPlus/ COMPONENT EnergyPlus)
install(DIRECTORY "${ENERGYPLUS_DIR}/python_lib" DESTINATION ./EnergyPlus/ COMPONENT EnergyPlus)
install(DIRECTORY "${ENERGYPLUS_DIR}/pyenergyplus" DESTINATION ./EnergyPlus/ COMPONENT EnergyPlus)

if(APPLE)
  install(PROGRAMS "${ENERGYPLUS_DIR}/libintl.8.dylib" DESTINATION ./EnergyPlus/ COMPONENT EnergyPlus)
  #install(PROGRAMS "${ENERGYPLUS_DIR}/Python" DESTINATION ./EnergyPlus/ COMPONENT EnergyPlus)
  file(GLOB ENERGYPLUS_PYTHON_LIBS "${ENERGYPLUS_DIR}/libpython*")
  foreach(python_lib IN LISTS ENERGYPLUS_PYTHON_LIBS)
    install(PROGRAMS "${python_lib}" DESTINATION ./EnergyPlus/ COMPONENT EnergyPlus)
  endforeach()
elseif(WIN32)
  file(GLOB ENERGYPLUS_MSVC_FILES "${ENERGYPLUS_DIR}/msvc*.dll")
  foreach(msvc_file IN LISTS ENERGYPLUS_MSVC_FILES)
    install(PROGRAMS "${msvc_file}" DESTINATION ./EnergyPlus/ COMPONENT EnergyPlus)
  endforeach()
  file(GLOB ENERGYPLUS_VCRUNTIME_FILES "${ENERGYPLUS_DIR}/vcruntime*.dll")
  foreach(vcruntime_file IN LISTS ENERGYPLUS_VCRUNTIME_FILES)
    install(PROGRAMS "${vcruntime_file}" DESTINATION ./EnergyPlus/ COMPONENT EnergyPlus)
  endforeach()
  file(GLOB ENERGYPLUS_UCRT_FILES "${ENERGYPLUS_DIR}/ucrt*.dll")
  foreach(ucrt_file IN LISTS ENERGYPLUS_UCRT_FILES)
    install(PROGRAMS "${ucrt_file}" DESTINATION ./EnergyPlus/ COMPONENT EnergyPlus)
  endforeach()
  file(GLOB ENERGYPLUS_VCOMP_FILES "${ENERGYPLUS_DIR}/vcomp*.dll")
  foreach(vcomp_file IN LISTS ENERGYPLUS_VCOMP_FILES)
    install(PROGRAMS "${vcomp_file}" DESTINATION ./EnergyPlus/ COMPONENT EnergyPlus)
  endforeach()
  file(GLOB ENERGYPLUS_MSAPI_FILES "${ENERGYPLUS_DIR}/api-ms*.dll")
  foreach(msapi_file IN LISTS ENERGYPLUS_MSAPI_FILES)
    install(PROGRAMS "${msapi_file}" DESTINATION ./EnergyPlus/ COMPONENT EnergyPlus)
  endforeach()
  # python37.dll
  file(GLOB ENERGYPLUS_PYTHON_LIBS "${ENERGYPLUS_DIR}/python*.dll")
  foreach(python_lib IN LISTS ENERGYPLUS_PYTHON_LIBS)
    install(PROGRAMS "${python_lib}" DESTINATION ./EnergyPlus/ COMPONENT EnergyPlus)
  endforeach()
  #install(PROGRAMS "${ENERGYPLUS_DIR}/concrt140.dll" DESTINATION ./EnergyPlus/ COMPONENT EnergyPlus)
elseif(UNIX)
    file(GLOB ENERGYPLUS_PYTHON_LIBS "${ENERGYPLUS_DIR}/libpython*")
    foreach(python_lib IN LISTS ENERGYPLUS_PYTHON_LIBS)
      install(PROGRAMS "${python_lib}" DESTINATION ./EnergyPlus/ COMPONENT EnergyPlus)
    endforeach()
endif()

if(WIN32)
  install(DIRECTORY "${PROJECT_BINARY_DIR}/${RADIANCE_PATH}/" DESTINATION ./Radiance/ COMPONENT Radiance USE_SOURCE_PERMISSIONS)
else()
  string(REPLACE "-Redhat" "-Linux" RADIANCE_PATH ${RADIANCE_PATH})
  install(DIRECTORY "${PROJECT_BINARY_DIR}/${RADIANCE_PATH}/usr/local/radiance/" DESTINATION ./Radiance/ COMPONENT Radiance USE_SOURCE_PERMISSIONS)
endif()

if(BUILD_PAT AND APPLE)
  install(FILES "${ENERGYPLUS_IDD}" DESTINATION ./ParametricAnalysisTool.app/Contents/Resources/EnergyPlus/ COMPONENT PAT)
  install(PROGRAMS ${ENERGYPLUS_FILES} DESTINATION ./ParametricAnalysisTool.app/Contents/Resources/EnergyPlus/ COMPONENT PAT)
  install(PROGRAMS ${ENERGYPLUS_LIB_FILES} DESTINATION ./ParametricAnalysisTool.app/Contents/Resources/EnergyPlus/ COMPONENT PAT)
  install(PROGRAMS "${EXPAND_OBJECTS}" DESTINATION ./ParametricAnalysisTool.app/Contents/Resources/EnergyPlus/ COMPONENT PAT)
  install(PROGRAMS "${ENERGYPLUS_DIR}/libgfortran.5.dylib" DESTINATION ./ParametricAnalysisTool.app/Contents/Resources/EnergyPlus/ COMPONENT PAT)
  if (NOT ARCH MATCHES arm64)
    install(PROGRAMS "${ENERGYPLUS_DIR}/libquadmath.0.dylib" DESTINATION ./ParametricAnalysisTool.app/Contents/Resources/EnergyPlus/ COMPONENT PAT)
  endif()
  install(DIRECTORY "${PROJECT_BINARY_DIR}/${RADIANCE_PATH}/usr/local/radiance/" DESTINATION ./ParametricAnalysisTool.app/Contents/Resources/Radiance/ COMPONENT PAT USE_SOURCE_PERMISSIONS)
endif()

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
  "${PROJECT_BINARY_DIR}/openstudioConfigVersion.cmake"
  VERSION ${CMAKE_PROJECT_VERSION}
  COMPATIBILITY AnyNewerVersion
)

export(
  EXPORT
  openstudio
       NAMESPACE "openstudio::"
       FILE "${PROJECT_BINARY_DIR}/openstudioConfig.cmake"
       )

install(
  EXPORT openstudio
        DESTINATION lib/cmake/openstudio
        NAMESPACE "openstudio::"
        COMPONENT "CPPAPI"
  FILE openstudioConfig.cmake
)

install(
  FILES
    "${CMAKE_CURRENT_BINARY_DIR}/openstudioConfigVersion.cmake"
  DESTINATION
    lib/cmake/openstudio/
  COMPONENT "CPPAPI"
)

install(
  FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/FetchRubyMinGW.cmake"
  DESTINATION
    lib/cmake/openstudio/
  COMPONENT "CPPAPI"
)

if (NOT DELAY_INCLUDE_CPACK)
  include(CPack)
endif()

include(CPackIFW)

cpack_add_component(CLI
  DISPLAY_NAME "Command Line Interface"
  DESCRIPTION "Command Line Interface"
)

cpack_add_component(RubyAPI
  DISPLAY_NAME "Ruby API"
  DESCRIPTION "Ruby API"
)

cpack_add_component(EnergyPlus
  DISPLAY_NAME "EnergyPlus"
  DESCRIPTION "Minimal EnergyPlus installation"
)

cpack_add_component(Radiance
  DISPLAY_NAME "Radiance"
  DESCRIPTION "Full Radiance installation"
)

cpack_add_component(CPPAPI
  DISPLAY_NAME "C++ API"
  DESCRIPTION "Libraries and headers for OpenStudio C++ API"
)

cpack_ifw_configure_component(CLI
  SCRIPT src/cli/install_operations.qs
  REQUIRES_ADMIN_RIGHTS
)

cpack_ifw_configure_component(RubyAPI
  REQUIRES_ADMIN_RIGHTS
)

cpack_ifw_configure_component(EnergyPlus
  REQUIRES_ADMIN_RIGHTS
)

cpack_ifw_configure_component(Radiance
  REQUIRES_ADMIN_RIGHTS
)

cpack_ifw_configure_component(Unspecified
  SCRIPT src/install_utility/install_operations.qs
  REQUIRES_ADMIN_RIGHTS
)

if(BUILD_CSHARP_BINDINGS)
  cpack_add_component(CSharpAPI
    DISPLAY_NAME "C# API"
    DESCRIPTION "C# API"
  )

  cpack_ifw_configure_component(CSharpAPI
    REQUIRES_ADMIN_RIGHTS
  )
endif()

if(BUILD_PAT)
  cpack_add_component(PAT
    DISPLAY_NAME "Parametric Analysis Tool"
    DESCRIPTION "Parametric Analysis Tool"
  )

  cpack_ifw_configure_component(PAT
    DEPENDS CLI RubyAPI
    SCRIPT pat/install_operations.qs
    REQUIRES_ADMIN_RIGHTS
  )
endif()

if( BUILD_NUGET_PACKAGE AND NOT BUILD_CSHARP_BINDINGS)
  message("If BUILD_NUGET_PACKAGE=ON, you must use BUILD_CSHARP_BINDINGS=ON too")
  # Rest moved to csharp/CMakeLists.txt
endif()

if (MSVC)
  if (NOT USE_LTO)
    message(WARNING "Suppressing 121,000+ linker warnings, see src/lib/CMakeLists.txt for details")
    # The actual solution to this is to use the new openstudio_EXPORTS preprocessor
    # definition and make all of the libs truely one large lib with a single set of exports
    # the problem is that somehow using this now causes the openstudio_model.lib to grow too large
    # and we get a linker error

    # The alternative is to suppress these warnings.
    # The problem is that we are using export/import of symbols between function calls
    # within the same library. This may result in less efficient function calls.

    # The potential inefficiency goes away if we use LTO linking
    target_link_libraries(openstudiolib PRIVATE -IGNORE:4217,4049)
    if(CMAKE_SIZEOF_VOID_P EQUAL 8) # only applies to 64 bit windows platforms
      if (BUILD_RUBY_BINDINGS)
        target_link_libraries(openstudio_rb PRIVATE -IGNORE:4217,4049)
      endif()
      if (BUILD_CLI)
        target_link_libraries(openstudio PRIVATE -IGNORE:4217,4049)
      endif()
    endif()
  endif()
endif()

