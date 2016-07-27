if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  # using Clang
  set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -undefined dynamic_lookup")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  # using GCC
  set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} --unresolved-symbols=ignore-all")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
  # using Intel C++
  set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} --unresolved-symbols=ignore-all")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  # using Visual Studio C++
  set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} /FORCE:UNRESOLVED")
endif()

include_directories(${RUBY_INCLUDE_DIRS} ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_BINARY_DIR} ${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR} )

add_library( init_openstudio
  init_openstudio.cpp
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

set_target_properties(openstudiorb PROPERTIES PREFIX "")
set_target_properties(openstudiorb PROPERTIES OUTPUT_NAME openstudio)
if(APPLE)
  set_target_properties(openstudiorb PROPERTIES SUFFIX ".bundle" )
endif()

target_link_libraries(openstudiorb init_openstudio)

install(TARGETS openstudiorb DESTINATION bin)

