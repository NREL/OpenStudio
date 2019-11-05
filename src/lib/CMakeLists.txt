# Some operating systems don't like compiling a library with
# no source files

add_library(openstudiolib_static STATIC empty.cpp)

target_link_libraries(
 openstudiolib_static
 PUBLIC
 openstudio_airflow
 openstudio_model
 openstudio_energyplus
 openstudio_measure
 openstudio_osversion
 openstudio_sdd
 openstudio_isomodel
 openstudio_gbxml
 openstudio_radiance
)

add_library(openstudiolib SHARED empty.cpp)

set(LINK_STATIC_LIBS TRUE)

if (LINK_STATIC_LIBS)
  if (MSVC)
    set(ENABLE_LINK_ALL "/WHOLEARCHIVE")
    set(DISABLE_LINK_ALL "")
  elseif (APPLE)
    set(ENABLE_LINK_ALL "-all_load")
    set(DISABLE_LINK_ALL "")
  else()
    set(ENABLE_LINK_ALL "-Wl,--whole-archive")
    set(DISABLE_LINK_ALL "-Wl,--no-whole-archive")
  endif()
endif()

target_link_libraries(
 openstudiolib
 PRIVATE
 ${ENABLE_LINK_ALL}
 openstudio_utilities
 openstudio_airflow
 openstudio_model
 openstudio_energyplus
 openstudio_measure
 openstudio_osversion
 openstudio_sdd
 openstudio_isomodel
 openstudio_gbxml
 openstudio_radiance
 ${DISABLE_LINK_ALL}
)


target_link_libraries(
 openstudiolib
 INTERFACE
 CONAN_PKG::boost_core
 CONAN_PKG::boost_filesystem
 CONAN_PKG::boost_crc
 CONAN_PKG::boost_regex
 CONAN_PKG::boost_algorithm
 CONAN_PKG::boost_uuid
 CONAN_PKG::boost_log
 CONAN_PKG::boost_program_options
 CONAN_PKG::boost_numeric_ublas
 CONAN_PKG::boost_functional
 CONAN_PKG::boost_geometry
)

# We cannot make the libs that make up the shared library public dependency at all, if we do
# then we have to export and install them as well, which we probably don't want to do.

# If we try to include directories from the dependent libraries directly, we have to use
# generator expressions to get the specific configuration's INTERFACE_INCLUDE_DIRECTORIES

# This also does not seem to do what we ultimately want, as it would also require
# that we export the .a files.

# So here we are, I'm manually exporting the directories, it's probably a mistake
# TODO reevaluate this.


target_include_directories(
  openstudiolib
  INTERFACE
  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>
  $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}>
  $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/src>
  )

# `export` command is only for use within the build tree
export(TARGETS
         openstudio_airflow
         openstudio_model
         openstudio_energyplus
         openstudio_measure
         openstudio_osversion
         openstudio_sdd
         openstudio_isomodel
         openstudio_gbxml
         openstudio_radiance
         openstudio_utilities
         openstudiolib
         openstudiolib_static 
       NAMESPACE "openstudio::" 
       FILE "${PROJECT_BINARY_DIR}/openstudioConfig.cmake"
       )

install(TARGETS openstudiolib
        EXPORT openstudio
        DESTINATION lib
        COMPONENT "CPPCLI"
        INCLUDES DESTINATION include
        )

# `install(EXPORT)` is for installation package
install(EXPORT openstudio 
        DESTINATION lib/cmake/openstudio
        NAMESPACE "openstudio::"
        COMPONENT "CPPCLI"
        FILE openstudioConfig.cmake)

install(DIRECTORY "${PROJECT_SOURCE_DIR}/src/"
                  "${PROJECT_BINARY_DIR}/src/"
        DESTINATION include
        COMPONENT "CPPCLI"
        FILES_MATCHING 
        PATTERN "*.h"
        PATTERN "*.hpp"
        PATTERN "*.hxx"
        )



