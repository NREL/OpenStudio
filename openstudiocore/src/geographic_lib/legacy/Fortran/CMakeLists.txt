cmake_minimum_required (VERSION 2.8.4)

project (GeographicLib-legacy-Fortran Fortran)

# Set a default build type for single-configuration cmake generators if
# no build type is set.
if (NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
  set (CMAKE_BUILD_TYPE Release)
endif ()

set (TOOLS geoddirect geodinverse planimeter geodtest)

foreach (TOOL ${TOOLS})
  add_executable (${TOOL} ${TOOL}.for geodesic.for geodesic.inc)
endforeach ()
set_target_properties (${TOOLS} PROPERTIES COMPILE_FLAGS
  "-Wall -Wextra -pedantic -std=f95 -fimplicit-none -Wno-compare-reals")

# Work alikes for NGS geodesic tools.  This uses legacy code from NGS
# and so they trigger multiple errors and warnings if compiled with the
# compile flags above.
add_executable (ngsforward ngsforward.for ngscommon.for geodesic.for)
add_executable (ngsinverse ngsinverse.for ngscommon.for geodesic.for)

# Turn on testing
enable_testing ()

# Run the test suite
add_test (NAME geodtest COMMAND geodtest)
