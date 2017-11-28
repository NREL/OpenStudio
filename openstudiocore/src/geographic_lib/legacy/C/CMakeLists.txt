cmake_minimum_required (VERSION 2.8.4)

project (GeographicLib-legacy-C C)

# Set a default build type for single-configuration cmake generators if
# no build type is set.
if (NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
  set (CMAKE_BUILD_TYPE Release)
endif ()

# Make the compiler more picky.
if (WIN32)
  set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /W4")
else ()
  set (CMAKE_C_FLAGS
    "${CMAKE_C_FLAGS} -Wall -Wextra -Wno-array-bounds -pedantic")
  if (NOT "${CMAKE_C_COMPILER_ID}" STREQUAL "Clang")
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ansi")
  endif ()
endif ()

set (TOOLS direct inverse planimeter geodtest)

foreach (TOOL ${TOOLS})
  add_executable (${TOOL} ${TOOL}.c geodesic.c geodesic.h)
  if (NOT WIN32)
    target_link_libraries (${TOOL} m)
  endif ()
endforeach ()

# Turn on testing
enable_testing ()

# Run the test suite
add_test (NAME geodtest COMMAND geodtest)
