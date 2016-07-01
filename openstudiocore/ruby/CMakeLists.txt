
include_directories(${RUBY_INCLUDE_DIRS} ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_BINARY_DIR} ${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR} )

# Do this at cmake time instead of build time to keep
# all network activity isolated to configure time
#execute_process(
#  COMMAND bundler "install" "--path=${CMAKE_CURRENT_BINARY_DIR}/gems"
#  WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
#)
#
#file(GLOB_RECURSE RB_FILES FOLLOW_SYMLINKS "${CMAKE_CURRENT_BINARY_DIR}/gems/ruby/2.0.0/bundler/**/*.rb")
#file(GLOB_RECURSE IDF_FILES FOLLOW_SYMLINKS "${CMAKE_CURRENT_BINARY_DIR}/gems/ruby/2.0.0/bundler/**/*.idf")
#
#list(APPEND EMBED_FILES ${RB_FILES})
#list(APPEND EMBED_FILES ${IDF_FILES})
#
#foreach( _FILE ${EMBED_FILES} )
#  file(RELATIVE_PATH LOCATION "${CMAKE_CURRENT_BINARY_DIR}/gems/ruby/2.0.0/bundler" ${_FILE})
#  list(APPEND FILES ${_FILE})
#  list(APPEND EMBEDDED_PATHS ${LOCATION})
#endforeach()
#
#embed_files("${FILES}" "${EMBEDDED_PATHS}" EMBEDDED_SOURCE)
#set_source_files_properties(${EMBED_SOURCE} PROPERTIES HEADER_FILE_ONLY TRUE)

add_library( init_openstudio
  init_openstudio.cpp
  #${EMBEDDED_SOURCE}
)

target_link_libraries(init_openstudio
 ruby_OpenStudioAirflow
 ruby_OpenStudioModelCore
 ruby_OpenStudioModelSimulation
 ruby_OpenStudioUtilitiesCore
 ruby_OpenStudioUtilitiesPlot
 ruby_OpenStudioEnergyPlus
 ##ruby_OpenStudioModelEditor
 ruby_OpenStudioOSVersion
 ruby_OpenStudioUtilitiesData
 ruby_OpenStudioUtilitiesSql
 ##ruby_OpenStudioGBXML
 ruby_OpenStudioModelGenerators
 ##ruby_OpenStudioRadiance
 #ruby_OpenStudioUtilitiesEconomics
 ruby_OpenStudioUtilitiesTime
 ##ruby_OpenStudioISOModel
 ruby_OpenStudioModelGeometry
 ##ruby_OpenStudioSDD
 ruby_OpenStudioUtilitiesFileTypes
 ruby_OpenStudioUtilitiesUnits
 ##ruby_OpenStudioLib
 ruby_OpenStudioModelHVAC
 ruby_OpenStudioUtilities
 ruby_OpenStudioUtilitiesGeometry
 ruby_OpenStudioMeasure
 ruby_OpenStudioModelRefrigeration
 ruby_OpenStudioUtilitiesBCL
 ruby_OpenStudioUtilitiesIdd
 ruby_OpenStudioModel
 ruby_OpenStudioModelResources
 ruby_OpenStudioUtilitiesCloud
 ruby_OpenStudioUtilitiesIdf
 openstudio_utilities
 ${Boost_LIBRARIES}
 ${QT_LIBS}
)

add_library(
  openstudiorb
  MODULE
  openstudio_ruby.cpp
)

#target_link_libraries(openstudiorb ${RUBY_LIBRARY})
# fix this
target_link_libraries(openstudiorb /System/Library/Frameworks/Ruby.framework)
if( APPLE )
  target_link_libraries(openstudiorb ${corefoundation_lib})
endif()

set_target_properties(openstudiorb PROPERTIES PREFIX "")
set_target_properties(openstudiorb PROPERTIES OUTPUT_NAME openstudio)
if(APPLE)
  set_target_properties(openstudiorb PROPERTIES SUFFIX ".bundle" )
endif()

target_link_libraries(openstudiorb init_openstudio)

