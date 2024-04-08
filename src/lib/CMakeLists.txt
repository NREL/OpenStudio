
# Some operating systems don't like compiling a library with
# no source files
add_library(openstudiolib SHARED empty.cpp)


target_link_libraries(
 openstudiolib
 PRIVATE
 openstudio_utilities
 openstudio_airflow
 openstudio_model
 openstudio_energyplus
 openstudio_epjson
 openstudio_measure
 openstudio_osversion
 openstudio_sdd
 openstudio_isomodel
 openstudio_gbxml
 openstudio_gltf
 openstudio_radiance
)

target_link_libraries(
 openstudiolib
 INTERFACE
 boost::boost
 JsonCpp::JsonCpp
 cpprestsdk::cpprestsdk
 openssl::openssl
 pugixml::pugixml
 LibXml2::LibXml2
 fmt::fmt
)

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

install(TARGETS openstudiolib
        EXPORT openstudio
        DESTINATION ${LIB_DESTINATION_DIR}
        COMPONENT "CLI"  # CLI is no longer self-contained, it needs it
        INCLUDES DESTINATION include
          include/openstudio
        )

install(DIRECTORY "${PROJECT_SOURCE_DIR}/src/"
                  "${PROJECT_BINARY_DIR}/src/"
        DESTINATION include/openstudio
        COMPONENT "CPPAPI"
        FILES_MATCHING
        PATTERN "*.h"
        PATTERN "*.hpp"
        PATTERN "*.hxx"
        PATTERN "*.i"
        )
