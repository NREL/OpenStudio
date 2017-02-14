project (GeographicLib)

# Version information
set (PROJECT_VERSION_MAJOR 1)
set (PROJECT_VERSION_MINOR 45)
set (PROJECT_VERSION_PATCH 0)
set (PROJECT_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}")
if (PROJECT_VERSION_PATCH GREATER 0)
  set (PROJECT_VERSION "${PROJECT_VERSION}.${PROJECT_VERSION_PATCH}")
endif ()

if (DEFINED CPACK_PACKAGE_VERSION_COUNT)

  # majic (version 0.1.9 and later) invokes cmake defining, e.g.,
  #   -D CPACK_PACKAGE_VERSION=1.37-001-SNAPSHOT
  #   -D CPACK_PACKAGE_VERSION_COUNT=2
  #   -D CPACK_PACKAGE_VERSION_MAJOR=1
  #   -D CPACK_PACKAGE_VERSION_MINOR=36
  #   -D CPACK_PACKAGE_VERSION_SUFFIX=-001-SNAPSHOT
  # Check that the version numbers are consistent.
  if (CPACK_PACKAGE_VERSION_COUNT EQUAL 2)
    set (CPACK_PACKAGE_VERSION_PATCH 0)
  elseif (CPACK_PACKAGE_VERSION_COUNT LESS 2)
    message (FATAL_ERROR "CPACK_PACKAGE_VERSION_COUNT must be 2 or more")
  endif ()
  if (NOT (
        CPACK_PACKAGE_VERSION_MAJOR EQUAL PROJECT_VERSION_MAJOR AND
        CPACK_PACKAGE_VERSION_MINOR EQUAL PROJECT_VERSION_MINOR AND
        CPACK_PACKAGE_VERSION_PATCH EQUAL PROJECT_VERSION_PATCH))
    message (FATAL_ERROR "Inconsistency in CPACK and PROJECT version numbers")
  endif ()
  set (PROJECT_VERSION ${CPACK_PACKAGE_VERSION})

else ()

  set (CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
  set (CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
  set (CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
  set (CPACK_PACKAGE_VERSION ${PROJECT_VERSION})

endif ()

# The library version tracks the numbering given by libtool in the
# autoconf set up.
set (LIBVERSION_API 14)
set (LIBVERSION_BUILD 14.2.1)
string (TOLOWER ${PROJECT_NAME} PROJECT_NAME_LOWER)
string (TOUPPER ${PROJECT_NAME} PROJECT_NAME_UPPER)

cmake_minimum_required (VERSION 2.8.4) # This version was released 2011-02-16

# User-settable variables

# (1) COMMON_INSTALL_PATH governs the installation convention.  If it
# is on ON (the Linux default), the installation is to a common
# directory, e.g., /usr/local.  If it is OFF (the Windows default),
# the installation directory contains the package name, e.g.,
# c:/pkg/GeographicLib-1.22.  The installation directories for the
# documentation, cmake configuration, python and matlab interfaces all
# depend on the variable with deeper paths relative to
# CMAKE_INSTALL_PREFIX being used when it's ON.

if (WIN32)
  option (COMMON_INSTALL_PATH "Use a common installation path for packages" OFF)
else ()
  option (COMMON_INSTALL_PATH "Use a common installation path for packages" ON)
endif ()

# The use of PACKAGE_PATH and INSTALL_PATH is now DEPRECATED.
# (2) PACKAGE_PATH and INSTALL_PATH govern the find_package search
# path and the installation directory.  (find_package is not used by
# GeographicLib since it doesn't depend on other packages.  However
# PACKAGE_PATH is used here for uniformity with other packages which
# adopt the same conventions.)
#
# If PACKAGE_PATH is defined, it is prepended to CMAKE_PREFIX_PATH.
#
# If INSTALL_PATH is not specified but PACKAGE_PATH is, then
# INSTALL_PATH is set to
#   ${PACKAGE_PATH}, if COMMON_INSTALL_PATH is ON;
#   ${PACKAGE_PATH}/${PROJECT_NAME}-${PROJECT_VERSION}, otherwise.
#
# If INSTALL_PATH is now defined, then set CMAKE_INSTALL_PREFIX to
# INSTALL_PATH.
#
# Typically, only PACKAGE_PATH needs to be specified, e.g.,
# cmake -D PACKAGE_PATH=/opt .. (on Linux)
#   => CMAKE_PREFIX_PATH=/opt   CMAKE_INSTALL_PREFIX=/opt
# cmake -D PACKAGE_PATH=C:/pkg .. (on Windows)
#   => CMAKE_PREFIX_PATH=C:/pkg CMAKE_INSTALL_PREFIX=C:/pkg/GeographicLib-1.22

if (PACKAGE_PATH)
  set (CMAKE_PREFIX_PATH ${PACKAGE_PATH} ${CMAKE_PREFIX_PATH})
  message (STATUS "CMAKE_PREFIX_PATH set to ${CMAKE_PREFIX_PATH}")
endif ()

if (NOT INSTALL_PATH AND PACKAGE_PATH)
  if (COMMON_INSTALL_PATH)
    set (INSTALL_PATH ${PACKAGE_PATH} CACHE PATH "Installation directory" FORCE)
  else ()
    set (INSTALL_PATH ${PACKAGE_PATH}/${PROJECT_NAME}-${PROJECT_VERSION}
      CACHE PATH "Installation directory" FORCE)
  endif ()
endif ()
if (INSTALL_PATH)
  file (TO_CMAKE_PATH ${INSTALL_PATH} CMAKE_INSTALL_PREFIX)
  message (STATUS "CMAKE_INSTALL_PREFIX set to ${CMAKE_INSTALL_PREFIX}")
endif ()

# (3) Where to look for data files.  Various classes look in the geoids,
# gravity, magnetic, subdirectories of ${GEOGRAPHICLIB_DATA}.
if (WIN32)
  # The binary installers for the data files for Windows are created
  # with Inno Setup which uses {commonappdata} which (since Windows
  # Vista) is C:/ProgramData.
  set (GEOGRAPHICLIB_DATA
    "C:/ProgramData/GeographicLib"
    CACHE PATH "Location for data for GeographicLib")
else ()
  set (GEOGRAPHICLIB_DATA
    "/usr/local/share/GeographicLib"
    CACHE PATH "Location for data for GeographicLib")
endif ()

# (4) Build which libraries?  Possible values are SHARED, STATIC, BOTH.
#if (MSVC)
#  set (GEOGRAPHICLIB_LIB_TYPE STATIC CACHE STRING
#    "Types of library generated: SHARED, STATIC (default), or BOTH")
#else ()
#  set (GEOGRAPHICLIB_LIB_TYPE SHARED CACHE STRING
#    "Types of library generated: SHARED (default), STATIC, or BOTH")
#endif ()
#set_property (CACHE GEOGRAPHICLIB_LIB_TYPE
#  PROPERTY STRINGS "SHARED" "STATIC" "BOTH")

if (GEOGRAPHICLIB_LIB_TYPE STREQUAL "BOTH")
  set (GEOGRAPHICLIB_SHARED_LIB ON)
  set (GEOGRAPHICLIB_STATIC_LIB ON)
  set (GEOGRAPHICLIB_LIB_TYPE_VAL 2)
elseif (GEOGRAPHICLIB_LIB_TYPE STREQUAL "SHARED")
  set (GEOGRAPHICLIB_SHARED_LIB ON)
  set (GEOGRAPHICLIB_STATIC_LIB OFF)
  set (GEOGRAPHICLIB_LIB_TYPE_VAL 1)
elseif (GEOGRAPHICLIB_LIB_TYPE STREQUAL "STATIC")
  set (GEOGRAPHICLIB_SHARED_LIB OFF)
  set (GEOGRAPHICLIB_STATIC_LIB ON)
  set (GEOGRAPHICLIB_LIB_TYPE_VAL 0)
else ()
  message (FATAL_ERROR
    "Bad value of GEOGRAPHICLIB_LIB_TYPE, \"${GEOGRAPHICLIB_LIB_TYPE}\" "
    "(should be SHARED, STATIC or BOTH)")
endif ()

if (GEOGRAPHICLIB_STATIC_LIB)
  set (PROJECT_STATIC_LIBRARIES GeographicLib_STATIC)
  set (PROJECT_STATIC_DEFINITIONS -DGEOGRAPHICLIB_SHARED_LIB=0)
else ()
  set (PROJECT_STATIC_LIBRARIES)
  set (PROJECT_STATIC_DEFINITIONS)
endif ()

if (GEOGRAPHICLIB_SHARED_LIB)
  set (PROJECT_SHARED_LIBRARIES GeographicLib)
  set (PROJECT_LIBRARIES ${PROJECT_SHARED_LIBRARIES})
  set (PROJECT_SHARED_DEFINITIONS -DGEOGRAPHICLIB_SHARED_LIB=1)
  set (PROJECT_DEFINITIONS ${PROJECT_SHARED_DEFINITIONS})
else ()
  set (PROJECT_SHARED_LIBRARIES)
  set (PROJECT_LIBRARIES ${PROJECT_STATIC_LIBRARIES})
  set (PROJECT_SHARED_DEFINITIONS)
  set (PROJECT_DEFINITIONS ${PROJECT_STATIC_DEFINITIONS})
endif ()

# (5) Create the documentation?  This depends on whether doxygen can be
# found.  If this is OFF, then links will be provided to the online
# documentation on Sourceforge.
option (GEOGRAPHICLIB_DOCUMENTATION
  "Use doxygen to create the documentation" OFF)

# (6) Build .NET wrapper library NETGeographicLib.  This only applies to
# Windows.  Default is OFF, because, currently, most people don't use
# this interface.
option (BUILD_NETGEOGRAPHICLIB "Build NETGeographicLib library" OFF)

# (7) Set the default "real" precision.  This should probably be left
# at 2 (double).
set (GEOGRAPHICLIB_PRECISION 2 CACHE STRING
  "Precision: 1 = float, 2 = double, 3 = extended, 4 = quadruple, 5 = variable")
set_property (CACHE GEOGRAPHICLIB_PRECISION PROPERTY STRINGS 1 2 3 4 5)

# (8) When making a binary package, should we include the debug version
# of the library?  This applies to MSVC only, because that's the
# platform where debug and release compilations do not inter-operate.
# It requires building as follows:
#   cmake --build . --config Debug   --target ALL_BUILD
#   cmake --build . --config Release --target ALL_BUILD
#   cmake --build . --config Release --target PACKAGE
option (PACKAGE_DEBUG_LIBS
  "Include debug versions of library in binary package" OFF)

set (LIBNAME Geographic)
if (MSVC OR CMAKE_CONFIGURATION_TYPES)
  # For multi-config systems and for Visual Studio, the debug version of
  # the library is called Geographic_d.
  set (CMAKE_DEBUG_POSTFIX _d)
endif ()

if (NOT MSVC)
  # Set the run time path for shared libraries for non-Windows machines.
  # (1) include link path for external packages (not needed with
  # GeographicLib because there are no external packages).  This only
  # makes sense for native builds.
  if (NOT CMAKE_CROSSCOMPILING)
    set (CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
  endif ()
  # (2) include installed path for GeographicLib.
  if (NOT APPLE)
    # Use relative path so that package is relocatable
    set (CMAKE_INSTALL_RPATH "\$ORIGIN/../lib${LIB_SUFFIX}")
  else ()
    if (CMAKE_VERSION VERSION_LESS 2.8.12)
      # Use absolute path with MacOSx
      set (CMAKE_INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/lib${LIB_SUFFIX}")
    else ()
      # cmake 2.8.12 introduced a way to make the package relocatable.
      # See also INSTALL_RPATH property on the tools.
      set (CMAKE_MACOSX_RPATH ON)
    endif ()
  endif ()
endif ()

include (CheckTypeSize)
check_type_size ("long double" LONG_DOUBLE BUILTIN_TYPES_ONLY)
set (GEOGRAPHICLIB_HAVE_LONG_DOUBLE ${HAVE_LONG_DOUBLE})
include (TestBigEndian)
test_big_endian (GEOGRAPHICLIB_WORDS_BIGENDIAN)

# Make the compiler more picky.
if (MSVC)
  string (REGEX REPLACE "/W[0-4]" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  # Turn on parallel builds for Visual Studio
  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /MP")
else ()
  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
  # check for C++11 support.  This flag is *not* propagated to clients
  # that use GeographicLib.  However, this is of no consequence.  When
  # the client code is being compiled (and the GeographicLib headers
  # being included), work-alike substitutions are used.
  include (CheckCXXCompilerFlag)
  # Not yet ready for C++14 (problem with MPFR C++)
  foreach (_F 11 0x)
    set (CXX11_FLAG "-std=c++${_F}")
    set (_T CXX11TEST${_F})
    check_cxx_compiler_flag (${CXX11_FLAG} ${_T})
    if (${_T})
      break ()
    endif ()
    unset (CXX11_FLAG)
  endforeach ()
  if (CXX11_FLAG)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX11_FLAG}")
  endif ()
endif ()

include (CheckCXXSourceCompiles)
set (CMAKE_REQUIRED_FLAGS "${CMAKE_CXX_FLAGS}")
# Check whether the C++11 math function: std::expm1, std::atanh,
# etc. are available.  This flag is *not* propagated to clients that use
# GeographicLib.  However, this is of no consequence.  When the client
# code is being compiled (and the GeographicLib headers being included),
# work-alike substitutions are used.
check_cxx_source_compiles (
  "#include <cmath>
int main() {
  int q;
  return int(std::hypot(3.0, 4.0) + std::expm1(0.5) +
             std::log1p(2.0) + std::asinh(10.0) +
             std::atanh(0.8) + std::cbrt(8.0) +
             std::fma(1.0, 2.0, 3.0) + std::remquo(100.0, 90.0, &q) +
             std::remainder(100.0, 90.0)) +
             std::isfinite(4.0) + std::isnan(0.0);
}\n" CXX11_MATH)
if (CXX11_MATH)
  add_definitions (-DGEOGRAPHICLIB_CXX11_MATH=1)
else ()
  add_definitions (-DGEOGRAPHICLIB_CXX11_MATH=0)
endif ()

# Check whether the C++11 static_assert macro is available.  This flag
# is *not* propagated to clients that use GeographicLib.  However, this
# is of no consequence.  When the client code is being compiled (and the
# GeographicLib headers being included), a work-alike substitution is
# used.
check_cxx_source_compiles (
  "#include <cmath>
int main() {
  static_assert(true, \"static assert test\");
  return 0;
}\n" CXX11_STATIC_ASSERT)
if (CXX11_STATIC_ASSERT)
  add_definitions (-DGEOGRAPHICLIB_HAS_STATIC_ASSERT=1)
else ()
  add_definitions (-DGEOGRAPHICLIB_HAS_STATIC_ASSERT=0)
endif ()

# Set the include directories.  Look in ${PROJECT_BINARY_DIR}/include
# first because that's where Config.h will be
include_directories ("${PROJECT_BINARY_DIR}/include" include)

if (GEOGRAPHICLIB_PRECISION EQUAL 1)
  message (WARNING "Compiling with floats which results in poor accuracy")
elseif (GEOGRAPHICLIB_PRECISION EQUAL 2)
  # This is the default
elseif (GEOGRAPHICLIB_PRECISION EQUAL 3)
  if (WIN32)
    message (WARNING
      "Cannot support long double on Windows, switching to double")
    set (GEOGRAPHICLIB_PRECISION 2)
  endif ()
elseif (GEOGRAPHICLIB_PRECISION EQUAL 4)
  if (CMAKE_CXX_COMPILER_ID STREQUAL GNU AND CXX11_MATH)
    # quad precision numbers appeared in Boost 1.54.
    find_package (Boost 1.54)
    if (Boost_FOUND)
      include_directories ("${Boost_INCLUDE_DIRS}")
      set (QUAD_LIBRARIES quadmath)
      if (NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS 4.8)
        # Enable Q suffix for quad precision in g++ 4.8 and later
        set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fext-numeric-literals")
      endif ()
      # Suppress the warnings that boost can spew out, e.g.,
      #   "typedef 'boost_concept_check905' locally defined but not used"
      set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-local-typedefs")
    endif ()
  endif ()
  if (NOT QUAD_LIBRARIES)
    message (WARNING "Cannot support quad precision, switching to double")
    set (GEOGRAPHICLIB_PRECISION 2)
  endif ()
elseif (GEOGRAPHICLIB_PRECISION EQUAL 5)
  if (CXX11_MATH)
    # Install MPFR C++ version 3.6.2 or later from
    # http://www.holoborodko.com/pavel/mpfr and install mpreal.h in the
    # include directory.  NOTE: MPFR C++ is covered by the GPL; be sure
    # to abide by the terms of this license.
    #
    # For Linux, use system versions of mpfr and gmp.  For Apple, use
    # brew install mpfr.  Recent versions of mpfr (3.0 or later) work
    # fine.  For Windows, download MPFR-MPIR-x86-x64-MSVC2010.zip from
    # the MPFR C++ site and unpack in the top-level directory.  The
    # Windows build only works with GEOGRAPHICLIB_LIB_TYPE=STATIC.
    # NOTE: mpfr, gmp, and mpir are covered by the LGPL; be sure to
    # abide by the terms of this license.
    #
    # Need Visual Studio 12 2013 or later, g++ 4.5 or later; not sure
    # about clang.
    if (WIN32)
      if (MSVC AND NOT MSVC_VERSION LESS 1800)
        if (CMAKE_SIZEOF_VOID_P EQUAL 8)
          set (_ARCH x64)
        else ()
          set (_ARCH Win32)
        endif ()
        include_directories (mpfr_mpir_x86_x64_msvc2010/mpfr
          mpfr_mpir_x86_x64_msvc2010/mpir/dll/${_ARCH}/Release)
        # These are C libraries so it's OK to use release versions for
        # debug builds.  Also these work for later versions of Visual
        # Studio (specifically version 12).
        link_directories (mpfr_mpir_x86_x64_msvc2010/mpfr/dll/${_ARCH}/Release
          mpfr_mpir_x86_x64_msvc2010/mpir/dll/${_ARCH}/Release)
        set (MPFR_LIBRARIES mpfr mpir)
        # Suppress the myriad of "conditional expression is constant"
        # warnings
        set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4127")
      endif ()
    else ()
      if (APPLE)
        include_directories (/usr/local/include)
        link_directories (/usr/local/lib)
      endif ()
      # g++ before 4.5 doesn't work (no explicit cast operator)
      if (NOT (CMAKE_CXX_COMPILER_ID STREQUAL GNU AND
            CMAKE_CXX_COMPILER_VERSION VERSION_LESS 4.5))
        set (MPFR_LIBRARIES mpfr gmp)
      endif ()
    endif ()
  endif ()
  if (NOT MPFR_LIBRARIES)
    message (WARNING "Cannot support mpfr, switching to double")
    set (GEOGRAPHICLIB_PRECISION 2)
  endif ()
endif ()

if (APPLE AND GEOGRAPHICLIB_PRECISION EQUAL 2)
  if (CMAKE_SYSTEM_PROCESSOR MATCHES "i.86" OR
      CMAKE_SYSTEM_PROCESSOR MATCHES "amd64" OR
      CMAKE_SYSTEM_PROCESSOR MATCHES "x86")
    set (CMAKE_OSX_ARCHITECTURES "i386;x86_64")
  endif ()
endif ()

# Create a Config.h to expose system information to the compiler
configure_file (
  include/GeographicLib/Config.h.in
  include/GeographicLib/Config.h
  @ONLY )

# The documentation depends on doxygen.
if (GEOGRAPHICLIB_DOCUMENTATION)
  set (DOXYGEN_SKIP_DOT ON)
  # Version 1.8.7 or later needed for &hellip;
  find_package (Doxygen 1.8.7)
  find_program (JSDOC jsdoc)
endif ()

# The man pages are written as pod files and converted to nroff format,
# C++ code, and html.  Because this require tools that may not be
# available on an end-user's system, the creation of the final
# documentation is therefore only done in "MAINTAINER" mode.  The
# maintainer runs "make distrib-all" which installs the transformed
# documentation files into the source tree.  Skip Apple here because
# man/makeusage.sh uses "head --lines -4" to drop the last 4 lines of a
# file and there's no simple equivalent for MacOSX
if (NOT WIN32 AND NOT APPLE)
  find_program (HAVE_POD2MAN pod2man)
  find_program (HAVE_POD2HTML pod2html)
  find_program (HAVE_COL col)
endif ()
if (HAVE_POD2MAN AND HAVE_POD2HTML AND HAVE_COL AND GEOGRAPHICLIB_DOCUMENTATION)
  set (MAINTAINER ON)
else ()
  set (MAINTAINER OFF)
endif ()
if (MAINTAINER)
  add_custom_target (distrib-all)
  add_dependencies (distrib-all distrib-man)
endif ()

if (NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
  # Set a default build type for single-configuration cmake generators
  # if no build type is set.
  set (CMAKE_BUILD_TYPE Release)
endif ()

# Set output directories for Windows so that executables and dlls are
# put in the same place
if (WIN32)
  # static libaries
  set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")
  # shared libraries
  set (CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")
  # executables and dlls
  set (CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")
endif ()

# The list of tools (to be installed into, e.g., /usr/local/bin)
set (TOOLS CartConvert ConicProj GeodesicProj GeoConvert GeodSolve
  GeoidEval Gravity MagneticField Planimeter RhumbSolve TransverseMercatorProj)
# The list of scripts (to be installed into, e.g., /usr/local/sbin)
set (SCRIPTS
  geographiclib-get-geoids geographiclib-get-gravity geographiclib-get-magnetic)

set_property (GLOBAL PROPERTY USE_FOLDERS ON)

# The list of subdirectories to process
add_subdirectory (src)
add_subdirectory (include/GeographicLib)
#add_subdirectory (tools)
#add_subdirectory (man)
#add_subdirectory (doc)
#add_subdirectory (js)
#add_subdirectory (matlab)
#add_subdirectory (python/geographiclib)
#add_subdirectory (examples)
if (MSVC AND BUILD_NETGEOGRAPHICLIB)
  if (GEOGRAPHICLIB_PRECISION EQUAL 2)
    set (NETGEOGRAPHICLIB_LIBRARIES NETGeographicLib)
    set (NETLIBNAME NETGeographic)
    add_subdirectory (dotnet/NETGeographicLib)
    add_subdirectory (dotnet/examples/ManagedCPP)
  else ()
    message (WARNING "Build of NETGeographicLib only works with doubles")
  endif ()
endif ()
add_subdirectory (cmake)
if (EXISTS ${PROJECT_SOURCE_DIR}/tests/CMakeLists.txt)
  add_subdirectory (tests)
endif ()

## Packaging support; we deal with
## (1) a source distribution: cmake make a tar.gz file and the zip file
## is created from this.  Only the maintainer can do this, because of
## the need to generate additional documentation files.
## (2) a binary distribution: code is included for Linux, Apple, and
## Windows, but only the Windows distribution has been exercised.
#
## Need to ensure that system dlls get included in a binary distribution
#if (NOT DEFINED CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS_NO_WARNINGS)
#  # Visual Studio Express does include redistributable components so
#  # squelch the warning.
#  set (CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS_NO_WARNINGS ON)
#endif ()
#set (CMAKE_INSTALL_DEBUG_LIBRARIES ON)
#include (InstallRequiredSystemLibraries)
#
## The configuration of CPack is via variable that need to be set before
## the include (CPack).
#set (CPACK_PACKAGE_CONTACT charles@karney.com)
#set (CPACK_PACKAGE_VENDOR "GeographicLib")
#set (CPACK_PACKAGE_DESCRIPTION_SUMMARY
#  "GeographicLib library, utilities, and documentation")
## The list of files to be excluded from the source distribution.
#set (CPACK_SOURCE_IGNORE_FILES
#  "#"
#  "~\$"
#  "/\\\\.git"
#  "${PROJECT_SOURCE_DIR}/BUILD"
#  "${PROJECT_SOURCE_DIR}/(tests|testdata|cgi-bin|.*\\\\.cache)/"
#  "${PROJECT_SOURCE_DIR}/(distrib|.*-distrib|.*-installer|geodesic-papers)/"
#  "${PROJECT_SOURCE_DIR}/[^/]*\\\\.(html|kmz|pdf)\$"
#  "${PROJECT_SOURCE_DIR}/(autogen|biblio)\\\\.sh\$"
#  "${PROJECT_SOURCE_DIR}/(geodesic-biblio.txt|makefile-admin|[^/]*\\\\.png)\$"
#  "${PROJECT_SOURCE_DIR}/matlab/.*blurb.txt\$" )
#set (CPACK_SOURCE_GENERATOR TGZ)
#
#set (CPACK_RESOURCE_FILE_LICENSE ${PROJECT_SOURCE_DIR}/LICENSE.txt)
#set (CPACK_PACKAGE_INSTALL_DIRECTORY "${PROJECT_NAME}-${PROJECT_VERSION}")
#set (CPACK_SOURCE_PACKAGE_FILE_NAME "${CPACK_PACKAGE_INSTALL_DIRECTORY}")
#
#if (WIN32)
#  # The Windows binary packager is NSIS.  Set the necessary variables
#  # for this.
#  set (CPACK_NSIS_CONTACT "charles@karney.com")
#  set (CPACK_NSIS_URL_INFO_ABOUT "http://geographiclib.sf.net")
#  set (CPACK_NSIS_HELP_LINK "mailto:charles@karney.com")
#  if (CMAKE_SIZEOF_VOID_P EQUAL 8)
#    # Hardcode the prefix for Visual Studio 11
#    set (CPACK_NSIS_INSTALL_ROOT "C:\\\\pkg-vc11-x64")
#    set (CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_INSTALL_DIRECTORY}-win64")
#    set (CPACK_NSIS_PACKAGE_NAME "${PROJECT_NAME} x64 ${PROJECT_VERSION}")
#    set (CPACK_PACKAGE_INSTALL_REGISTRY_KEY
#      "${PROJECT_NAME}-x64-${PROJECT_VERSION}")
#  else ()
#    # Hardcode the prefix for Visual Studio 11
#    set (CPACK_NSIS_INSTALL_ROOT "C:\\\\pkg-vc11")
#    set (CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_INSTALL_DIRECTORY}-win32")
#    set (CPACK_NSIS_PACKAGE_NAME "${PROJECT_NAME} ${PROJECT_VERSION}")
#    set (CPACK_PACKAGE_INSTALL_REGISTRY_KEY
#      "${PROJECT_NAME}-${PROJECT_VERSION}")
#  endif ()
#  set (CPACK_NSIS_DISPLAY_NAME ${CPACK_NSIS_PACKAGE_NAME})
#  set (CPACK_NSIS_MENU_LINKS
#    "http://geographiclib.sf.net/${PROJECT_VERSION}/index.html"
#    "Library documentation"
#    "http://geographiclib.sf.net/${PROJECT_VERSION}/utilities.html"
#    "Utilities documentation"
#    "http://geographiclib.sf.net" "GeographicLib home page"
#    "https://sf.net/projects/geographiclib/" "Main project page")
#  set (CPACK_NSIS_MODIFY_PATH ON)
#elseif (APPLE)
#  # Not tested
#  set (CPACK_GENERATOR Bundle)
#  set (CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_INSTALL_DIRECTORY}-darwin")
#else ()
#  # Not tested
#  set (CPACK_GENERATOR TGZ)
#endif ()
#
#include (CPack)
#
## Another maintainer-specific target is building the source distribution
## via the target dist.  This calls package_source to make a tar.gz file.
## However this needs to be touched up to support the vanilla Makefiles
## provided with GeographicLib.  This entails
## (1) creating Makefile (which includes Makefile.mk);
## (2) creating a bare-bones Config.h (with just the version information);
## (3) making sure that make thinks the generated documentation files are
## up-to-date.
## Then a new tar.gz file and zip file are created.  To avoid potential
## problems with directory permissions, tar and zip are told only to
## archive the files.
#if (MAINTAINER)
#  add_custom_target (dist
#    COMMAND ${CMAKE_MAKE_PROGRAM} package_source
#    COMMAND
#      cd _CPack_Packages/Linux-Source/TGZ/${CPACK_SOURCE_PACKAGE_FILE_NAME} &&
#      echo include Makefile.mk > Makefile &&
#      sed -e "s/Unconfigured/${PROJECT_VERSION}/"
#      -e "s/MAJOR .*/MAJOR ${CPACK_PACKAGE_VERSION_MAJOR}/"
#      -e "s/MINOR .*/MINOR ${CPACK_PACKAGE_VERSION_MINOR}/"
#      -e "s/PATCH .*/PATCH ${CPACK_PACKAGE_VERSION_PATCH}/"
#      include/GeographicLib/Config.h > include/GeographicLib/Config.h.new &&
#      mv include/GeographicLib/Config.h.new include/GeographicLib/Config.h
#    COMMAND
#      cd _CPack_Packages/Linux-Source/TGZ/${CPACK_SOURCE_PACKAGE_FILE_NAME} &&
#      touch man/[A-Za-z]*.usage man/[A-Za-z]*.1 man/[A-Za-z]*.1.html &&
#      chmod -R g-w .
#    COMMAND
#      cd _CPack_Packages/Linux-Source/TGZ &&
#      find ${CPACK_SOURCE_PACKAGE_FILE_NAME} -type f |
#      tar cfzT ${CMAKE_BINARY_DIR}/${CPACK_SOURCE_PACKAGE_FILE_NAME}.tar.gz -
#    COMMAND
#      rm -f ${CMAKE_BINARY_DIR}/${CPACK_SOURCE_PACKAGE_FILE_NAME}.zip &&
#      rsync -a --delete
#      _CPack_Packages/Linux-Source/TGZ/${CPACK_SOURCE_PACKAGE_FILE_NAME}
#      _CPack_Packages/Linux-Source/TGZ.DOS/ &&
#      cd _CPack_Packages/Linux-Source/TGZ.DOS &&
#      find . -type f |
#      egrep -v '/\(compile|config[^/]*|depcomp|install-sh|missing|[Mm]akefile[^/]*|[^/]*\\.\(ac|am|csproj|eps|kmz|m4|pdf|png|resx|settings|sh|sln|vcproj|vcxproj\)\)$$' |
#      xargs unix2dos -q -k &&
#      find ${CPACK_SOURCE_PACKAGE_FILE_NAME} -type f |
#      zip -q ${CMAKE_BINARY_DIR}/${CPACK_SOURCE_PACKAGE_FILE_NAME}.zip -@
#  )
#  add_dependencies (dist distrib-all)
#endif ()

# Add a test target; the tests are in tools.
enable_testing ()
