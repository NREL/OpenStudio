if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  # using Clang
  set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -undefined dynamic_lookup")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  # using GCC
  set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} --unresolved-symbols=ignore-all")
endif()

include_directories(${RUBY_INCLUDE_DIRS} ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_BINARY_DIR} ${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR} )

add_library( init_openstudio STATIC
  init_openstudio.cpp
  RubyAPI.hpp
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
  openstudio_rb
  MODULE
  openstudio_rb.cpp
)

set_target_properties(openstudio_rb PROPERTIES PREFIX "")
set_target_properties(openstudio_rb PROPERTIES OUTPUT_NAME openstudio)
if(APPLE)
  set_target_properties(openstudio_rb PROPERTIES SUFFIX ".bundle" )
else()
  set_target_properties(openstudio_rb PROPERTIES SUFFIX ".so" )
endif()

target_link_libraries(openstudio_rb init_openstudio)

if( WIN32 )
  target_link_libraries(openstudio_rb ${RUBY_MINGW_STUB_LIB})
endif()

install(TARGETS openstudio_rb DESTINATION bin)

add_library(
  openstudio_gui_rb
  MODULE
  openstudio_gui_rb.cpp
)

target_link_libraries( openstudio_gui_rb ruby_OpenStudioModelEditor)

set_target_properties(openstudio_gui_rb PROPERTIES PREFIX "")
set_target_properties(openstudio_gui_rb PROPERTIES OUTPUT_NAME openstudio_gui)
if(APPLE)
  set_target_properties(openstudio_gui_rb PROPERTIES SUFFIX ".bundle" )
else()
  set_target_properties(openstudio_gui_rb PROPERTIES SUFFIX ".so" )
endif()

if( WIN32 )
  target_link_libraries(openstudio_gui_rb ${RUBY_MINGW_STUB_LIB})
endif()

install(TARGETS openstudio_gui_rb DESTINATION bin)

