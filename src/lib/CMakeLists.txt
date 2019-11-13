# Some operating systems don't like compiling a library with
# no source files

#add_library(openstudiolib_static STATIC empty.cpp)

#target_link_libraries(
# openstudiolib_static
# PUBLIC
# openstudio_utilities
# openstudio_airflow
# openstudio_model
# openstudio_energyplus
# openstudio_measure
# openstudio_osversion
# openstudio_sdd
# openstudio_isomodel
# openstudio_gbxml
# openstudio_radiance
#)
#

# Some operating systems don't like compiling a library with
# no source files
add_library(openstudiolib SHARED empty.cpp)

add_executable(test_openstudiolib test_lib.cpp)
target_link_libraries(test_openstudiolib openstudiolib)

target_link_libraries(
 openstudiolib
 PRIVATE
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
 CONAN_PKG::jsoncpp
 CONAN_PKG::cpprestsdk
 CONAN_PKG::OpenSSL
 CONAN_PKG::pugixml
)

if (MSVC)
  if (NOT USE_LTO)
    message(WARN "Suppressing 121,000+ linker warnings, see src/lib/CMakeLists.txt for details")
    # The actual solution to this is to use the new openstudio_EXPORTS preprocessor
    # definition and make all of the libs truely one large lib with a single set of exports
    # the problem is that somehow using this now causes the openstudio_model.lib to grow too large
    # and we get a linker error

    # The alternative is to suppress these warnings.
    # The problem is that we are using export/import of symbols between function calls
    # within the same library. This may result in less efficient function calls.

    # The potential inefficiency goes away if we use LTO linking
    target_link_libraries(openstudiolib PRIVATE -IGNORE:4217,4049)
  endif()
endif()

target_compile_definitions(openstudiolib INTERFACE "-DSHARED_OS_LIBS")

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
         #         openstudiolib_static 
       NAMESPACE "openstudio::" 
       FILE "${PROJECT_BINARY_DIR}/openstudioConfig.cmake"
       )

install(TARGETS openstudiolib
        EXPORT openstudio
        DESTINATION lib
        COMPONENT "CPPAPI"
        INCLUDES DESTINATION include
        )

# `install(EXPORT)` is for installation package
install(EXPORT openstudio 
        DESTINATION lib/cmake/openstudio
        NAMESPACE "openstudio::"
        COMPONENT "CPPAPI"
        FILE openstudioConfig.cmake)

install(DIRECTORY "${PROJECT_SOURCE_DIR}/src/"
                  "${PROJECT_BINARY_DIR}/src/"
        DESTINATION include
        COMPONENT "CPPAPI"
        FILES_MATCHING 
        PATTERN "*.h"
        PATTERN "*.hpp"
        PATTERN "*.hxx"
        )



