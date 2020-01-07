cmake_minimum_required(VERSION 3.10.2)
cmake_policy(SET CMP0048 NEW)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# Do not enable compiler specific extensions, for eg on GCC use -std=c++1z (=c++17) and not -std=gnu++17
set(CMAKE_CXX_EXTENSIONS OFF)

# Use ccache is available, has to be before "project()"
find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
  # Support Unix Makefiles and Ninja
  set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
endif()

# Project macro can only take digits in the version, it splits these out to specific variables
project(OpenStudio VERSION 3.0.0)

# The RT Manifest file and C# SDK only support the digit portion of patch
string(APPEND PROJECT_VERSION_PATCH_DIGIT ${PROJECT_VERSION_PATCH})

# CPack package version does not support extended patch version
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")

# Modify the more specific variables as needed to indicate prerelease, etc
string(APPEND PROJECT_VERSION "-rc1")
string(APPEND OpenStudio_VERSION "-rc1")
string(APPEND PROJECT_VERSION_PATCH "-rc1")
string(APPEND OpenStudio_VERSION_PATCH "-rc1")

# The PROJECT_SOURCE_DIR here is OpenStudio/, which we will use instead of using CMAKE_SOURCE_DIR
set(PROJECT_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
# We will use BINARY_DIR instead of CMAKE_BINARY_DIR to allow this to be built as a submodule
set(PROJECT_BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}")

#MESSAGE(FATAL_ERROR "CMAKE_CURRENT_SOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR}, BINARY_DIR=${BINARY_DIR}, PROJECT_SOURCE_DIR=${PROJECT_SOURCE_DIR}, CMAKE_SOURCE_DIR=${CMAKE_SOURCE_DIR}, CMAKE_BINARY_DIR=${CMAKE_BINARY_DIR}")

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

include(ExternalProject)
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


# OpenStudio version
set(OPENSTUDIO_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}")
set(OPENSTUDIO_LONG_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}+${PROJECT_VERSION_BUILD}")
# Export to parent scope
if(hasParent)
  set(OPENSTUDIO_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}" PARENT_SCOPE)
  set(OPENSTUDIO_LONG_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}+${PROJECT_VERSION_BUILD}" PARENT_SCOPE)
endif()

# EnergyPlus Idd version
set(ENERGYPLUS_VERSION_MAJOR 9)
set(ENERGYPLUS_VERSION_MINOR 2)
set(ENERGYPLUS_VERSION_PATCH 0)
set(ENERGYPLUS_VERSION "${ENERGYPLUS_VERSION_MAJOR}.${ENERGYPLUS_VERSION_MINOR}.${ENERGYPLUS_VERSION_PATCH}")
# Build SHA is not required to have a value, but if it does OpenStudio will require this build.
set(ENERGYPLUS_BUILD_SHA "921312fa1d")

# ENERGYPLUS_RELEASE_NAME is used to locate the E+ download
# from the github releases
set(ENERGYPLUS_RELEASE_NAME "v9.2.0")

# Radiance
set(RADIANCE_VERSION "5.0.a.12")

# configure file with version information
configure_file(${PROJECT_SOURCE_DIR}/OpenStudio.in ${PROJECT_BINARY_DIR}/src/OpenStudio.hxx)

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

###############################################################################

###############################################################################
# Build options

if(UNIX AND NOT APPLE)
  # the RPATH to be used when installing
  set(CMAKE_INSTALL_RPATH "$ORIGIN/../lib")
endif()

# TODO: should this be only in OpenStudioApplication?
option(BUILD_PAT "Build PAT" OFF)

# Build C++ documentation using Doxygen
# Requires: doxygen
option(BUILD_DOCUMENTATION "Build Documentation" OFF)

# Build CSharp bindings
# Requires: SWIG
if(WIN32)
  option(BUILD_CSHARP_BINDINGS "Build CSharp bindings" OFF)
endif()

# Build Java bindings
# Requires: SWIG
option(BUILD_JAVA_BINDINGS "Build Java bindings" OFF)

# Build V8 Node bindings
# Requires: SWIG, Node
option(BUILD_V8_BINDINGS "Build Node V8 bindings" OFF)

# Build Python bindings
# Requires: SWIG Python
option(BUILD_PYTHON_BINDINGS "Build Python bindings" OFF)

# Build ctest testing
# Requires: EnergyPlus
option(BUILD_TESTING "Build testing targets" OFF)

# Build package
# Requires: EnergyPlus, Radiance
# TODO: this option is actually unused...
# TODO: In any case, should core still produce a package or only in OpenStudioApplication?
option(BUILD_PACKAGE "Build package" OFF)

# Build with OpenSSL support
set(BUILD_WITH_OPENSSL ON CACHE INTERNAL "Build With OpenSSL Support For SSH Connections")

# Build shared OS libraries
option(BUILD_SHARED_LIBS "Build OS libs as shared libraries.  Currently not tested." OFF)

if(BUILD_SHARED_LIBS)
  add_definitions("-DSHARED_OS_LIBS")
endif()


if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  add_definitions(-Qunused-arguments)
endif()

if(WIN32)
  option(BUILD_NUGET_PACKAGE "Build NuGet Package" OFF)
endif()

# Enable runtime checking features
if(CMAKE_COMPILER_IS_GNUCC OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
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

  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${SANITIZER_LINKER_FLAGS}")
  set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${SANITIZER_LINKER_FLAGS}")
  set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} ${SANITIZER_LINKER_FLAGS}")


  mark_as_advanced(ENABLE_THREAD_SANITIZER ENABLE_ADDRESS_SANITIZER ENABLE_UNDEFINED_SANITIZER)

endif()

if(APPLE)
  find_library(COREFOUNDATION_LIBRARY CoreFoundation)
  add_definitions(-D_LIBCPP_ENABLE_CXX17_REMOVED_AUTO_PTR)
  #add_definitions(-DBOOST_SYSTEM_NO_DEPRECATED)
endif()

if(UNIX AND NOT APPLE)
  # Enable libgtest linking
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pthread")
endif()

if(WIN32)
  if(CMAKE_CL_64)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /OPT:REF /OPT:NOICF")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zi")
  else()
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /OPT:REF /OPT:NOICF")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zi")
  endif()
endif()

if(BUILD_V8_BINDINGS)
  option(BUILD_NODE_MODULES "Build V8 Bindings as Node Modules" ON)
endif()

# Use PCH
option(USE_PCH "Use precompiled headers" OFF)

if(WIN32)
  add_definitions(-DNOMINMAX)
endif()

if(MSVC AND NOT NINJA)
  # Build with Multiple Processes
  set(BUILD_WITH_MULTIPLE_PROCESSES ON CACHE BOOL "/MP compiler flag for full processor utilization")
  mark_as_advanced(BUILD_WITH_MULTIPLE_PROCESSES)
endif()

###############################################################################

###############################################################################
# Conan

# Note JM 2019-04-24: Another option is to globally set "print_run_commands = True" in ~/.conan/conan.conf
option(CONAN_PRINT_RUN_COMMANDS "Log every conan self.run command" OFF)
mark_as_advanced(CONAN_PRINT_RUN_COMMANDS)
if(CONAN_PRINT_RUN_COMMANDS)
  set(ENV{CONAN_PRINT_RUN_COMMANDS} "1")
  message("Setting CONAN_PRINT_RUN_COMMANDS: $ENV{CONAN_PRINT_RUN_COMMANDS}")
else()
  unset(ENV{CONAN_PRINT_RUN_COMMANDS})
  message("CONAN_PRINT_RUN_COMMANDS: $ENV{CONAN_PRINT_RUN_COMMANDS}")
endif()

# Attempt to run, it'll do the right thing if the calling cmake script already ran it
set(CONAN_INSTALL_FILE "${PROJECT_SOURCE_DIR}/ConanInstall.cmake")
include("${CONAN_INSTALL_FILE}")

# A macro to find a conan related value especially when using multi-config builds (MSVC)
# But it also works with single-config builds
macro(FindValue ValueName)
  set(LocalVar "")
  set(LocalVar $<$<CONFIG:Debug>:${${ValueName}_DEBUG}>$<$<CONFIG:Release>:${${ValueName}_RELEASE}>$<$<CONFIG:RelWithDebInfo>:${$ValueName}_RELWITHDEBINFO}>$<$<CONFIG:MinSizeRel>:${${ValueName}_MINSIZEREL}>
  )
#  list(JOIN LocalVar "" LocalVar)
  string(STRIP ${LocalVar} LocalVar)
  set(CURRENT_${ValueName} $<IF:$<BOOL:${LocalVar}>,${LocalVar},${${ValueName}}>)
  # For debug purposes
  # message(STATUS "Found '${ValueName}' as '${CURRENT_${ValueName}}'")
endmacro()

###############################################################################

get_target_property(RUBY_INCLUDE_DIRS CONAN_PKG::openstudio_ruby INTERFACE_INCLUDE_DIRECTORIES)

# Export to Parent
if(hasParent)
  set(RUBY_INCLUDE_DIRS "${RUBY_INCLUDE_DIRS}" PARENT_SCOPE)
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
endif()


###############################################################################

###############################################################################
# Compiler and system specific options
if(UNIX)

  #set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -fPIC -fno-strict-aliasing")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC -fno-strict-aliasing")

  # all warnings
  #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -fPIC -fno-strict-aliasing -Winvalid-pch -Wnon-virtual-dtor")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -fPIC -fno-strict-aliasing -Winvalid-pch")
  # Treat all warnings as errors
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
  if(APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-overloaded-virtual -ftemplate-depth=1024")
  else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-overloaded-virtual")
    # DLM: had to add this due to issues with boost optional and gcc, may be resolved if we move to std::optional
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-maybe-uninitialized")
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

  add_definitions(/bigobj)

  if(CMAKE_CL_64)
    # ignore conversion from size_t to int for now
    add_definitions(/wd4267)
  endif()

  # treat warnings as errors
  # DLM: only do this for our code, added in later
  # add_definitions(/WX)

  # ignore warnings about the stl being insecure
  add_definitions(-D_CRT_SECURE_NO_WARNINGS -D_SCL_SECURE_NO_WARNINGS)

  # ignore c++17 deprecation warnings generated by boost
  add_definitions(-D_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS)
  add_definitions(-D_SILENCE_FPOS_SEEKPOS_DEPRECATION_WARNING)

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

# If Linux, check LSB_RELEASE globally so we can use it after
if(UNIX AND NOT APPLE)

  find_program(LSB_RELEASE lsb_release)
  # -rs outputs only 16.04, or 18.04
  execute_process(COMMAND ${LSB_RELEASE} -rs
    OUTPUT_VARIABLE LSB_RELEASE_ID_SHORT
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )
  message(STATUS "LSB_RELEASE_ID_SHORT = ${LSB_RELEASE_ID_SHORT}")
endif()

# EnergyPlus

if(UNIX)
  if(APPLE)
    set(ENERGYPLUS_EXPECTED_HASH 49a8eb402738e4146779c15b63b4ad99)
    set(ENERGYPLUS_PLATFORM "Darwin-x86_64")
  elseif(EXISTS "/etc/redhat-release")
    # TODO: There aren't any redhat releases anymore, see PR #3145 too
    set(ENERGYPLUS_EXPECTED_HASH DOES_NOT_EXIST_02ed618aab65bf46c96bf1faf2e1905b)
    set(ENERGYPLUS_PLATFORM "Redhat-x86_64")
  else()
    set(ENERGYPLUS_EXPECTED_HASH 91739b15520092e74ff4eed2a2ee1cbd)
    set(ENERGYPLUS_PLATFORM "Linux-x86_64")
  endif()
  set(ENERGYPLUS_PATH "EnergyPlus-${ENERGYPLUS_VERSION}-${ENERGYPLUS_BUILD_SHA}-${ENERGYPLUS_PLATFORM}")
  if(EXISTS "${PROJECT_BINARY_DIR}/${ENERGYPLUS_PATH}.tar.gz")
    file(MD5 "${PROJECT_BINARY_DIR}/${ENERGYPLUS_PATH}.tar.gz" ENERGYPLUS_HASH)
  endif()
  if(NOT EXISTS "${PROJECT_BINARY_DIR}/${ENERGYPLUS_PATH}.tar.gz" OR NOT "${ENERGYPLUS_HASH}" MATCHES "${ENERGYPLUS_EXPECTED_HASH}")
    message(STATUS "Downloading EnergyPlus ${ENERGYPLUS_VERSION} (${ENERGYPLUS_PLATFORM})")

    if(EXISTS "/etc/redhat-release")
      file(DOWNLOAD "http://openstudio-resources.s3.amazonaws.com/dependencies/${ENERGYPLUS_PATH}.tar.gz" "${PROJECT_BINARY_DIR}/${ENERGYPLUS_PATH}.tar.gz" INACTIVITY_TIMEOUT 320 SHOW_PROGRESS EXPECTED_MD5 ${ENERGYPLUS_EXPECTED_HASH})
    else()
      file(DOWNLOAD "https://github.com/NREL/EnergyPlus/releases/download/${ENERGYPLUS_RELEASE_NAME}/${ENERGYPLUS_PATH}.tar.gz" "${PROJECT_BINARY_DIR}/${ENERGYPLUS_PATH}.tar.gz" INACTIVITY_TIMEOUT 320 SHOW_PROGRESS EXPECTED_MD5 ${ENERGYPLUS_EXPECTED_HASH})
    endif()

    # Remove the old extracted dir if exists
    execute_process(COMMAND ${CMAKE_COMMAND} -E remove_directory "${PROJECT_BINARY_DIR}/${ENERGYPLUS_PATH}")

  endif()

elseif(WIN32)
  if(CMAKE_CL_64)
    set(ENERGYPLUS_PATH "EnergyPlus-${ENERGYPLUS_VERSION}-${ENERGYPLUS_BUILD_SHA}-Windows-x86_64")
    set(ENERGYPLUS_ARCH 64)
    set(ENERGYPLUS_EXPECTED_HASH a95da7a01599d4f3379cc9423e38f155)
  else()
    set(ENERGYPLUS_PATH "EnergyPlus-${ENERGYPLUS_VERSION}-${ENERGYPLUS_BUILD_SHA}-Windows-i386")
    set(ENERGYPLUS_ARCH 32)
    ## TODO: update md5 hash, but not present on GitHub as .zip, only .exe for now at least
    set(ENERGYPLUS_EXPECTED_HASH DOES_NOT_EXIST_a2cbd995acd1928a7baa18407bb8e949)
  endif()
  if(EXISTS "${PROJECT_BINARY_DIR}/${ENERGYPLUS_PATH}.zip")
    file(MD5 "${PROJECT_BINARY_DIR}/${ENERGYPLUS_PATH}.zip" ENERGYPLUS_HASH)
  endif()
  if(NOT EXISTS "${PROJECT_BINARY_DIR}/${ENERGYPLUS_PATH}.zip" OR NOT "${ENERGYPLUS_HASH}" MATCHES "${ENERGYPLUS_EXPECTED_HASH}")
    message(STATUS "Downloading EnergyPlus ${ENERGYPLUS_VERSION} (${ENERGYPLUS_ARCH}-bit)")

    file(DOWNLOAD "https://github.com/NREL/EnergyPlus/releases/download/${ENERGYPLUS_RELEASE_NAME}/${ENERGYPLUS_PATH}.zip" "${PROJECT_BINARY_DIR}/${ENERGYPLUS_PATH}.zip" INACTIVITY_TIMEOUT 120 SHOW_PROGRESS EXPECTED_MD5 ${ENERGYPLUS_EXPECTED_HASH})

    # Remove the old extracted dir if exists
    execute_process(COMMAND ${CMAKE_COMMAND} -E remove_directory "${PROJECT_BINARY_DIR}/${ENERGYPLUS_PATH}")

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
find_package(EnergyPlus "${ENERGYPLUS_VERSION}" REQUIRED)

# Radiance
if(UNIX)
  if(APPLE)
    set(RADIANCE_EXPECTED_HASH 115ef9052ad5011d3fc7cebc2991644f)
    set(RADIANCE_PLATFORM "Darwin")
  elseif(EXISTS "/etc/redhat-release")
    # DLM: note that the installer expects the extracted directory to be named radiance-5.0.a.12-Linux
    set(RADIANCE_EXPECTED_HASH f75898bbfd238a740c1902d385c42b92)
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
  if(EXISTS "/etc/redhat-release")
    message("http://openstudio-resources.s3.amazonaws.com/dependencies/radiance-${RADIANCE_VERSION}-${RADIANCE_PLATFORM}.tar.gz")
    file(DOWNLOAD "http://openstudio-resources.s3.amazonaws.com/dependencies/radiance-${RADIANCE_VERSION}-${RADIANCE_PLATFORM}.tar.gz" "${PROJECT_BINARY_DIR}/${RADIANCE_PATH}.tar.gz" TIMEOUT 120 INACTIVITY_TIMEOUT 120 SHOW_PROGRESS EXPECTED_MD5 ${RADIANCE_EXPECTED_HASH})
  else()
    file(DOWNLOAD "http://github.com/NREL/Radiance/releases/download/${RADIANCE_VERSION}/radiance-${RADIANCE_VERSION}-${RADIANCE_PLATFORM}.tar.gz" "${PROJECT_BINARY_DIR}/${RADIANCE_PATH}.tar.gz" TIMEOUT 120 INACTIVITY_TIMEOUT 120 SHOW_PROGRESS EXPECTED_MD5 ${RADIANCE_EXPECTED_HASH})
  endif()
    execute_process(COMMAND ${CMAKE_COMMAND} -E remove_directory "${PROJECT_BINARY_DIR}/${RADIANCE_PATH}")
    execute_process(COMMAND ${CMAKE_COMMAND} -E tar xfz "${PROJECT_BINARY_DIR}/${RADIANCE_PATH}.tar.gz" WORKING_DIRECTORY "${PROJECT_BINARY_DIR}")
  endif()
  set(RADIANCE_LOCATION "${PROJECT_BINARY_DIR}/radiance-${RADIANCE_VERSION}-${RADIANCE_PLATFORM}/usr/local/radiance")
elseif(WIN32)
  if(CMAKE_CL_64)
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
    execute_process(COMMAND ${CMAKE_COMMAND} -E tar xfz "${PROJECT_BINARY_DIR}/${RADIANCE_PATH}.zip" WORKING_DIRECTORY "${PROJECT_BINARY_DIR}")
  endif()
  set(RADIANCE_LOCATION "${PROJECT_BINARY_DIR}/radiance-${RADIANCE_VERSION}-${RADIANCE_PLATFORM}/")
endif()

# Perl
if(NOT UNIX)
  execute_process(COMMAND ${CMAKE_COMMAND} -E tar xfz "${PROJECT_SOURCE_DIR}/dependencies/strawberry-perl-5.16.2.1-32bit-portable-reduced.tar.bz2"
    WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
    RESULT_VARIABLE rv)
  set(PERL_EXE "${PROJECT_BINARY_DIR}/strawberry-perl-5.16.2.1-32bit-portable-reduced/perl/bin/perl.exe")
endif()

# Mingw based ruby lib (only the dll stub library) from Ruby Installer
# We need this for stand alone ruby bindings
# target to use with typical ruby installations (those that are not msvc)
# Only need this on Windows systems
if(WIN32)
  if(NOT EXISTS ${RUBY_MINGW_LIB_DIR})
    set(RUBY_MINGW_LIB_ZIP_FILENAME "Ruby250-mingw-install.tar.gz")
    set(RUBY_MINGW_LIB_ZIP_EXPECTED_MD5 "ce454cfe886b32f7fe829385bc06af1c")

    set(RUBY_MINGW_LIB_ZIP_LOCAL_PATH "${PROJECT_BINARY_DIR}/${RUBY_MINGW_LIB_ZIP_FILENAME}")
    if(EXISTS "${RUBY_MINGW_LIB_ZIP_LOCAL_PATH}")
      file(MD5 "${RUBY_MINGW_LIB_ZIP_LOCAL_PATH}" RUBY_MINGW_LIB_ZIP_MD5)
    endif()

    if(NOT RUBY_MINGW_LIB_ZIP_MD5 STREQUAL RUBY_MINGW_LIB_ZIP_EXPECTED_MD5)
      message(STATUS "Downloading Ruby Mingw Lib: ${RUBY_MINGW_LIB_ZIP_FILENAME}")
      file(DOWNLOAD "http://openstudio-resources.s3.amazonaws.com/dependencies/${RUBY_MINGW_LIB_ZIP_FILENAME}"
        ${RUBY_MINGW_LIB_ZIP_LOCAL_PATH}
        INACTIVITY_TIMEOUT 120
        SHOW_PROGRESS
        EXPECTED_MD5 ${RUBY_MINGW_LIB_ZIP_EXPECTED_MD5})

      execute_process(COMMAND ${CMAKE_COMMAND} -E tar xfz ${RUBY_MINGW_LIB_ZIP_LOCAL_PATH} WORKING_DIRECTORY "${PROJECT_BINARY_DIR}")
    endif()

    set(RUBY_MINGW_STUB_LIB "${PROJECT_BINARY_DIR}/Ruby-mingw-install/libx64-msvcrt-ruby250.dll.a")
  endif()
endif()


# openstudio gems
# CI will build and upload openstudio gems to s3 at https://github.com/NREL/OpenStudio-gems
# need to  update the MD5sum for each platform and url below
if(UNIX)
   if(APPLE)
     set(OPENSTUDIO_GEMS_ZIP_FILENAME "openstudio3-gems-20200107-darwin.tar.gz")
     set(OPENSTUDIO_GEMS_ZIP_EXPECTED_MD5 "d6e74b25cfbd13ce6fb3fab2ce6fb9b6")
   else()
     set(OPENSTUDIO_GEMS_ZIP_FILENAME "openstudio3-gems-20200107-linux.tar.gz")
     set(OPENSTUDIO_GEMS_ZIP_EXPECTED_MD5 "171aa36059d329183748068115bca6cb")
   endif()
elseif(WIN32)
  if(CMAKE_CL_64)
    set(OPENSTUDIO_GEMS_ZIP_FILENAME "openstudio3-gems-20200107-windows.tar.gz")
    set(OPENSTUDIO_GEMS_ZIP_EXPECTED_MD5 "7997b00bdb1560f1b684bfc5d9f1d56")
  endif()
endif()




set(OPENSTUDIO_GEMS_ZIP_LOCAL_PATH "${PROJECT_BINARY_DIR}/${OPENSTUDIO_GEMS_ZIP_FILENAME}")
set(OPENSTUDIO_GEMS_DIR "${PROJECT_BINARY_DIR}/openstudio-gems")
set(OPENSTUDIO_GEMS_TARGETS_FILE "${OPENSTUDIO_GEMS_DIR}/export-extensions.cmake")
set(OPENSTUDIO_GEMS_INIT_FILE "${OPENSTUDIO_GEMS_DIR}/init-extensions.cpp")
if(EXISTS "${OPENSTUDIO_GEMS_ZIP_LOCAL_PATH}")
  file(MD5 "${OPENSTUDIO_GEMS_ZIP_LOCAL_PATH}" OPENSTUDIO_GEMS_ZIP_MD5)
endif()

if(NOT OPENSTUDIO_GEMS_ZIP_MD5 STREQUAL OPENSTUDIO_GEMS_ZIP_EXPECTED_MD5)
  message(STATUS "Downloading OpenStudio Gems: ${OPENSTUDIO_GEMS_ZIP_FILENAME}")
  file(DOWNLOAD "http://openstudio-resources.s3.amazonaws.com/dependencies/${OPENSTUDIO_GEMS_ZIP_FILENAME}"
    ${OPENSTUDIO_GEMS_ZIP_LOCAL_PATH}
    INACTIVITY_TIMEOUT 120
    SHOW_PROGRESS
    EXPECTED_MD5 ${OPENSTUDIO_GEMS_ZIP_EXPECTED_MD5})

  file(REMOVE_RECURSE "${OPENSTUDIO_GEMS_DIR}")
  execute_process(COMMAND ${CMAKE_COMMAND} -E tar xfz ${OPENSTUDIO_GEMS_ZIP_LOCAL_PATH} WORKING_DIRECTORY "${PROJECT_BINARY_DIR}")

  file(REMOVE_RECURSE "${PROJECT_BINARY_DIR}/src/cli/embedded_files")
endif()

# SWIG

# Note: JM 2019-10-16
# We use conan swig_installer 4.0 which enhances our PATH to include the CONAN_BIN_DIRS_SWIG_INSTALLER already
# So I could just do:
# set(SWIG_EXECUTABLE swig)
# But will have the probblem with the swiglib anyways.
# Plus, it doesn't appear to work on windows either, the PATH doesn't include it.
#find_program(SWIG_EXECUTABLE swig PATHS CURRENT_CONAN_SWIG_INSTALLER_ROOT NO_DEFAULT_PATH)
#if(NOT SWIG_EXECUTABLE)
#  message(FATAL_ERROR "Couldn't find swig, which shouldn't happen. CONAN_BIN_DIRS_SWIG_INSTALLER=${CONAN_BIN_DIRS_SWIG_INSTALLER}, CONAN_SWIG_INSTALLER_ROOT_RELEASE=${CONAN_SWIG_INSTALLER_ROOT_RELEASE}, CURRENT_CONAN_SWIG_INSTALLER_ROOT=${CURRENT_CONAN_SWIG_INSTALLER_ROOT}, ${ENV{SWIG_LIB}}, ${ENV{SWIG_INSTALLER_ROOT}}")
#endif()

# Another thing I tried is to use the `virtualenv` generator (`conan_cmake_run( ... GENERATORS virtualenv)`),
# but you end up having to call the activation script on each call, and the call is dependent on the platform which is even more annoying
# (either cmake -E cmd /C activate.bat, or cmake -E source activate.sh)
# I also tried using CMake to extract the value from the activate.sh script on all plaftorms but it seems even more of a hack

# So instead, we use a generator expression to find the right value. At least it makes sense, and should be robust.
FindValue(CONAN_SWIG_INSTALLER_ROOT)

set(SWIG_EXECUTABLE "${CURRENT_CONAN_SWIG_INSTALLER_ROOT}/bin/swig")

# The conan-provided binary has a built-in swiglib (`swig -swiglib`) that points to the build box on which it was built, which is problematic for us.
set(SWIG_LIB "${CURRENT_CONAN_SWIG_INSTALLER_ROOT}/share/swig/4.0.1")

# Export SWIG to parent scope
if(hasParent)
  set(SWIG_EXECUTABLE "${SWIG_EXECUTABLE}" PARENT_SCOPE)
  set(SWIG_LIB "${SWIG_LIB}" PARENT_SCOPE)
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

# JAVA
if(BUILD_JAVA_BINDINGS)
  find_package(JNI)
  find_package(Java)
  set(ALL_JAVA_BINDING_TARGETS "") # global list
  set(ALL_JAVA_SRC_DIRECTORIES "") # global list
endif()


if(BUILD_NODE_MODULES)
  add_definitions(-DBUILD_NODE_MODULE)
endif()

if(BUILD_PYTHON_BINDINGS)
  # need python
  # TODO: add support for 3.x
  find_package(PythonInterp 2.7 REQUIRED)
  find_package(PythonLibs 2.7 REQUIRED)
  include_directories(SYSTEM ${PYTHON_INCLUDE_DIRS})
  set(ALL_PYTHON_BINDINGS "") # global list of python bindings
  set(ALL_PYTHON_BINDING_DEPENDS "") # global list of library dependencies of the generated wrapper cxx files
  set(ALL_PYTHON_WRAPPER_FILES "") # global list of generated wrapper cxx files
  set(ALL_PYTHON_WRAPPER_TARGETS "") # global list targets that build generated wrapper cxx files
endif()


# Required dependencies

# Threading library
find_package(Threads)
if(UNIX)
  set(CMAKE_THREAD_LIBS "${CMAKE_THREAD_LIBS_INIT}" CACHE STRING "Thread library used.")
  mark_as_advanced(CMAKE_THREAD_LIBS)
endif()

# Zlib
# include_directories(SYSTEM src/zlib)
# add_subdirectory(src/zlib)
# make sure zconf.h can be found
# include_directories(${PROJECT_BINARY_DIR}/src/zlib)

#if(BUILD_TESTING)
#  # force gtest to build as a static library no matter what, otherwise we get error about undefined death test symbols when building shared libs
#  set(TEMP_BUILD_SHARED_LIBS ${BUILD_SHARED_LIBS})
#  set(BUILD_SHARED_LIBS OFF)
#
#  # Google test library
#  set(gtest_force_shared_crt ON CACHE BOOL "gtest_force_shared_crt" FORCE) # in case we are linking with shared or static os libraries
#  set(gtest_build_tests OFF CACHE BOOL "gtest_build_tests" FORCE)
#  set(gtest_build_samples OFF CACHE BOOL "gtest_build_samples" FORCE)
#  set(gtest_disable_pthreads OFF CACHE BOOL "gtest_disable_pthreads" FORCE)
#  add_definitions(-DGTEST_HAS_TR1_TUPLE=0)
#  include_directories(SYSTEM src/google_test/include)
#  add_subdirectory(src/google_test)
#
#  # restore BUILD_SHARED_LIBS
#  set(BUILD_SHARED_LIBS ${TEMP_BUILD_SHARED_LIBS})
#endif()


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
  radiance
  gbxml
  airflow
  isomodel
  osversion
  measure
  sdd
  cli
  install_utility
)

# Include the resources, should happen after find EnergyPlus
add_subdirectory(resources)

add_subdirectory(ruby)

foreach(D ${project_directories})
  add_subdirectory(src/${D})
endforeach()

# csharp, after loading projects
if(BUILD_CSHARP_BINDINGS)
  add_subdirectory(csharp)
endif()

# java, after loading projects
if(BUILD_JAVA_BINDINGS)
  add_subdirectory(java)
endif()

# python, after loading projects
if(BUILD_PYTHON_BINDINGS)
  add_subdirectory(python)
endif()

# v8-node
if(BUILD_NODE_MODULES)
  add_subdirectory(v8-node)
endif()

###############################################################################

###############################################################################
# Targets that combine all bindings targets
add_custom_target(ALL_RUBY_BINDINGS)
add_dependencies(ALL_RUBY_BINDINGS ${ALL_RUBY_BINDING_TARGETS})

if(BUILD_PYTHON_BINDINGS)
  add_custom_target(ALL_PYTHON_BINDINGS)
  #add_dependencies(ALL_PYTHON_BINDINGS python_sdk ${ALL_PYTHON_BINDING_TARGETS})
endif()


if(BUILD_CSHARP_BINDINGS)
  add_custom_target(ALL_CSHARP_BINDINGS)
  add_dependencies(ALL_CSHARP_BINDINGS csharp_sdk ${ALL_CSHARP_BINDING_TARGETS})
endif()


if(BUILD_JAVA_BINDINGS)
  add_custom_target(ALL_JAVA_BINDINGS)
  #add_dependencies(ALL_JAVA_BINDINGS java_sdk ${ALL_JAVA_BINDING_TARGETS})
endif()


if(BUILD_V8_BINDINGS)
  add_custom_target(ALL_V8_BINDINGS)
  add_dependencies(ALL_V8_BINDINGS ${ALL_V8_BINDING_TARGETS})
endif()

set(MAXIMIZE_CPU_USAGE OFF CACHE BOOL "Attempt to fully load the CPU during builds")
mark_as_advanced(MAXIMIZE_CPU_USAGE)


###############################################################################
# Targets that combine all documentation targets

# Doxygen
if(BUILD_DOCUMENTATION)
  # need doxygen
  find_package(Doxygen REQUIRED)

  # if have dot you have more options
  #if(DOXYGEN_DOT_FOUND)

  #endif()

  configure_file(${PROJECT_SOURCE_DIR}/doc/index.html ${PROJECT_BINARY_DIR}/doc/index.html COPYONLY)

  set(doxygen_directories ${project_directories})
  # Add the idd files to the documentation list as a separate entry, along with shared_gui_components
  list(APPEND doxygen_directories utilities/idd shared_gui_components)

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
      COMMAND ${PATCH_EXE} "${DOC_NAME}/html/search/search.js" < "${PROJECT_SOURCE_DIR}/doc/patch/search.js.patch"
      DEPENDS openstudio_doc_resources
      WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/doc
    )

    set(ALL_CPP_DOC_TARGETS ${ALL_CPP_DOC_TARGETS}
      openstudio_${DOC_NAME}_doc
    )

  endforeach()

  # Set up dependencies after all targets have been created
  add_dependencies(openstudio_utilities
    CreateEmbeddedSource
  )
  add_dependencies(openstudio_utilities_doc
    openstudio_utilities_idd_doc
  )
  add_dependencies(openstudio_model_doc
    openstudio_utilities_doc
    openstudio_utilities_idd_doc
  )

endif()


if(BUILD_DOCUMENTATION)

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
      COMMAND ${CMAKE_COMMAND} -E remove OpenStudio-${OPENSTUDIO_VERSION}-doc.zip
      COMMAND ${PROJECT_BINARY_DIR}/7za a -mmt -mx9 -tzip OpenStudio-${OPENSTUDIO_VERSION}-doc.zip * -r -x!*.cfg -x!*.tag -x!*.map -x!*.md5
      COMMAND ${PROJECT_BINARY_DIR}/7za d OpenStudio-${OPENSTUDIO_VERSION}-doc.zip index.html
      WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/doc
    )
    add_custom_command(TARGET ALL_RDOC
      POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E remove OpenStudio-${OPENSTUDIO_VERSION}-rdoc.zip
      COMMAND ${PROJECT_BINARY_DIR}/7za a -mmt -mx9 -tzip OpenStudio-${OPENSTUDIO_VERSION}-rdoc.zip * -r
      WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/rdoc
    )
  endif()

endif()


###############################################################################
# Export targets and generate OpenStudioCoreConfig.cmake

set(TARGETS_FILE ${PROJECT_BINARY_DIR}/OpenStudioTargets.cmake)

set(all_lib_targets
  openstudio_osversion
  openstudio_model
  openstudio_utilities
  openstudio_energyplus
  openstudio_measure
  openstudio_radiance
  openstudio_gbxml
  openstudio_isomodel
  openstudio_sdd
  openstudio_airflow
  ${ALL_RUBY_BINDING_TARGETS}
)

set(all_exe_targets
  GenerateIddFactory
  #gbXMLToIdf
  openstudio
)

if(UNIX AND NOT APPLE)
  foreach(targ ${all_lib_targets} ${all_exe_targets} ${ALL_TESTING_TARGETS})
    set_target_properties(${targ} PROPERTIES VERSION ${OPENSTUDIO_VERSION})
  endforeach()
endif()

configure_file("${PROJECT_SOURCE_DIR}/OpenStudioCoreConfig.cmake.in"
  "${PROJECT_BINARY_DIR}/OpenStudioCoreConfig.cmake" @ONLY)


if(UNIX AND NOT APPLE)
  set(examplesdir share/openstudio-${OPENSTUDIO_VERSION}/examples)
else()
  set(examplesdir Examples)
endif()

# Install additional Documents, such as release notes
install(FILES "${PROJECT_SOURCE_DIR}/developer/doc/ReleaseNotes/OpenStudio_Release_Notes_2_9_0_20191011.pdf" DESTINATION .)
install(FILES "${PROJECT_SOURCE_DIR}//LICENSE.md" DESTINATION .)
install(FILES "${PROJECT_SOURCE_DIR}//CHANGELOG.md" DESTINATION .)
install(FILES "${PROJECT_SOURCE_DIR}//copyright.txt" DESTINATION .)

###############################################################################

if(NOT UNIX)
  install(DIRECTORY "${PROJECT_BINARY_DIR}/strawberry-perl-5.16.2.1-32bit-portable-reduced/perl" DESTINATION ./Perl/ COMPONENT Radiance USE_SOURCE_PERMISSIONS)

  #set(CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS_SKIP ON)
  include(InstallRequiredSystemLibraries)
  if(CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS)
    install(FILES ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS} DESTINATION "bin")
    install(FILES ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS} DESTINATION "Ruby" COMPONENT "RubyAPI")
    install(FILES ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS} DESTINATION "CSharp/openstudio/" COMPONENT "CSharpAPI")
  endif()
endif()

set(CPACK_PACKAGE_VENDOR "National Renewable Energy Laboratory")
set(CPACK_PACKAGE_VERSION_MAJOR ${OpenStudio_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${OpenStudio_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${OpenStudio_VERSION_PATCH})
# set(CPACK_DEBIAN_PACKAGE_DEPENDS "")

# Default the debian package name to include version to allow several versions to be installed concurrently instead of overwriting any existing one
set(CPACK_DEBIAN_PACKAGE_NAME "openstudio-${OpenStudio_VERSION}")
# CPACK_DEBIAN_PACKAGE_DESCRIPTION defaults to this one too. dpkg-deb -I xxx.deb will show this description
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "OpenStudio is a cross-platform collection of software tools to support whole building energy modeling using EnergyPlus and advanced daylight analysis using Radiance")
set(CPACK_DEBIAN_PACKAGE_HOMEPAGE "https://www.openstudio.net")

# The actual .deb file name on disk
set(CPACK_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}-${OPENSTUDIO_LONG_VERSION}-${CMAKE_SYSTEM_NAME}")
set(CPACK_PACKAGE_CONTACT "openstudio@nrel.gov")

if(APPLE)
  set(CPACK_IFW_TARGET_DIRECTORY /Applications/OpenStudio-${OpenStudio_VERSION}/)
elseif(UNIX)
  # Default method doesn't use IFW but Deb, so this one is probably useless (but harmless)
  set(CPACK_IFW_TARGET_DIRECTORY /usr/local/openstudio-${OpenStudio_VERSION}/)
  # These two will set the .deb install path correctly
  set(CPACK_SET_DESTDIR ON)
  set(CPACK_INSTALL_PREFIX /usr/local/openstudio-${OpenStudio_VERSION}/)
  # Add a symlink to the CLI: /usr/local/bin/openstudio should point to /usr/local/openstudio-${OpenStudio_VERSION}/bin/openstudio
  # Add an arbitrary link (broken) at build/openstudio (if build/ is your build folder) that already points to the **future** /usr/local/openstudio-x.y.z/bin/openstudio
  execute_process(COMMAND ln -sf ${CPACK_INSTALL_PREFIX}bin/openstudio ${CMAKE_CURRENT_BINARY_DIR}/openstudio)
  # Plus a versioned one
  execute_process(COMMAND ln -sf ${CPACK_INSTALL_PREFIX}bin/openstudio ${CMAKE_CURRENT_BINARY_DIR}/openstudio-${OpenStudio_VERSION})
  # Have this link be installed with the .deb package in /usr/local/bin
  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/openstudio DESTINATION /usr/local/bin COMPONENT CLI)
  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/openstudio-${OpenStudio_VERSION} DESTINATION /usr/local/bin COMPONENT CLI)

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

install(FILES "${PROJECT_BINARY_DIR}/openstudio-gems/Gemfile" DESTINATION ./Ruby/ COMPONENT "RubyAPI")
install(FILES "${PROJECT_BINARY_DIR}/openstudio-gems/Gemfile.lock" DESTINATION ./Ruby/ COMPONENT "RubyAPI")
install(FILES "${PROJECT_BINARY_DIR}/openstudio-gems/openstudio-gems.gemspec" DESTINATION ./Ruby/ COMPONENT "RubyAPI" )

get_filename_component(ENERGYPLUS_DIR "${ENERGYPLUS_EXE}" DIRECTORY)
file(GLOB ENERGYPLUS_FILES "${ENERGYPLUS_DIR}/energyplus*")
file(GLOB ENERGYPLUS_LIB_FILES "${ENERGYPLUS_DIR}/libenergyplus*")
file(GLOB EXPAND_OBJECTS "${ENERGYPLUS_DIR}/ExpandObjects*")

install(FILES "${ENERGYPLUS_IDD}" DESTINATION ./EnergyPlus/ COMPONENT EnergyPlus)
install(PROGRAMS ${ENERGYPLUS_FILES} DESTINATION ./EnergyPlus/ COMPONENT EnergyPlus)
install(PROGRAMS ${ENERGYPLUS_LIB_FILES} DESTINATION ./EnergyPlus/ COMPONENT EnergyPlus)
install(PROGRAMS "${EXPAND_OBJECTS}" DESTINATION ./EnergyPlus/ COMPONENT EnergyPlus)

if(APPLE)
  install(PROGRAMS "${ENERGYPLUS_DIR}/libgfortran.5.dylib" DESTINATION ./EnergyPlus/ COMPONENT EnergyPlus)
  install(PROGRAMS "${ENERGYPLUS_DIR}/libquadmath.0.dylib" DESTINATION ./EnergyPlus/ COMPONENT EnergyPlus)
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
  file(GLOB ENERGYPLUS_MSAPI_FILES "${ENERGYPLUS_DIR}/api-ms*.dll")
  foreach(msapi_file IN LISTS ENERGYPLUS_MSAPI_FILES)
    install(PROGRAMS "${msapi_file}" DESTINATION ./EnergyPlus/ COMPONENT EnergyPlus)
  endforeach()
  #install(PROGRAMS "${ENERGYPLUS_PATH}/libgfortran.5.dylib" DESTINATION ./EnergyPlus/)
  #install(PROGRAMS "${ENERGYPLUS_PATH}/libquadmath.0.dylib" DESTINATION ./EnergyPlus/)
else()
  #install(PROGRAMS "${ENERGYPLUS_PATH}/libgfortran.5.dylib" DESTINATION ./EnergyPlus/)
  #install(PROGRAMS "${ENERGYPLUS_PATH}/libquadmath.0.dylib" DESTINATION ./EnergyPlus/)
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
  install(PROGRAMS "${ENERGYPLUS_DIR}/libquadmath.0.dylib" DESTINATION ./ParametricAnalysisTool.app/Contents/Resources/EnergyPlus/ COMPONENT PAT)
  install(DIRECTORY "${PROJECT_BINARY_DIR}/${RADIANCE_PATH}/usr/local/radiance/" DESTINATION ./ParametricAnalysisTool.app/Contents/Resources/Radiance/ COMPONENT PAT USE_SOURCE_PERMISSIONS)
endif()

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

if( BUILD_NUGET_PACKAGE )
  configure_file( OpenStudio.nuspec.in OpenStudio.nuspec )

  add_custom_target(nuget_package
    COMMAND NuGet pack OpenStudio.nuspec -BasePath "${PROJECT_BINARY_DIR}/" -properties Configuration=$<CONFIGURATION>
    DEPENDS csharp_sdk
    WORKING_DIRECTORY "${PROJECT_BINARY_DIR}"
  )
endif()

