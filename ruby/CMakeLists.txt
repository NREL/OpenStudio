if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "^(Apple)?Clang$")
  # using Clang
  set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -undefined dynamic_lookup")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  # using GCC
  #set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} --unresolved-symbols=ignore-all")
  set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} --unresolved-symbols=ignore-all")
endif()


include_directories(${CMAKE_CURRENT_BINARY_DIR} ${PROJECT_BINARY_DIR} ${PROJECT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR} )

# common library for openstudio.so and openstudio.exe

add_library( init_openstudio STATIC
  init_openstudio.cpp
  RubyAPI.hpp
)
target_include_directories(init_openstudio PRIVATE ${RUBY_INCLUDE_DIRS})

if(MSVC)
  # TODO: JM 2019-01-04: @macumber it seems like all three targets use the same COMPILE_FLAGS,
  # so instead of defining that three times, perhaps modify CMAKE_CXX_FLAGS?
  # wd4996=no deprecated warnings ; wd5033=register
  set_target_properties(init_openstudio PROPERTIES COMPILE_FLAGS "/bigobj /wd4996 /wd5033")
elseif (UNIX)
  # Disable register warnings
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-error=register -Wno-register")
endif()

target_link_libraries(init_openstudio
 ruby_OpenStudioAirflow
 ruby_OpenStudioModelCore
 ruby_OpenStudioModelSimulation
 ruby_OpenStudioUtilitiesCore
 ruby_OpenStudioUtilitiesPlot
 ruby_OpenStudioEnergyPlus
 ruby_OpenStudioOSVersion
 ruby_OpenStudioUtilitiesData
 ruby_OpenStudioUtilitiesSql
 ruby_OpenStudioGBXML
 ruby_OpenStudioModelGenerators
 ruby_OpenStudioRadiance
 ruby_OpenStudioUtilitiesTime
 ruby_OpenStudioISOModel
 ruby_OpenStudioModelGeometry
 ruby_OpenStudioSDD
 ruby_OpenStudioUtilitiesFileTypes
 ruby_OpenStudioUtilitiesUnits
 ruby_OpenStudioModelHVAC
 ruby_OpenStudioModelZoneHVAC
 ruby_OpenStudioModelAvailabilityManager
 ruby_OpenStudioModelPlantEquipmentOperationScheme
 ruby_OpenStudioModelStraightComponent
 ruby_OpenStudioModelAirflow
 ruby_OpenStudioUtilities
 ruby_OpenStudioUtilitiesGeometry
 ruby_OpenStudioMeasure
 ruby_OpenStudioModelRefrigeration
 ruby_OpenStudioUtilitiesBCL
 ruby_OpenStudioUtilitiesIdd
 ruby_OpenStudioModel
 ruby_OpenStudioModelResources
 ruby_OpenStudioUtilitiesIdf
)

# openstudio ruby bindings to load into normal ruby

add_library(openstudio_rb MODULE
  RubyAPI.hpp
  openstudio_rb.cpp
)
set_target_properties(openstudio_rb PROPERTIES PREFIX "")
set_target_properties(openstudio_rb PROPERTIES OUTPUT_NAME openstudio)
target_include_directories(openstudio_rb PRIVATE ${RUBY_INCLUDE_DIRS})

# change output directory so the openstudio.pdb file does not conflict between this target and the CLI
set_target_properties(openstudio_rb PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/ruby/")
set_target_properties(openstudio_rb PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/ruby/")
set_target_properties(openstudio_rb PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/ruby/")

if(APPLE)
  set_target_properties(openstudio_rb PROPERTIES SUFFIX ".bundle" )
else()
  set_target_properties(openstudio_rb PROPERTIES SUFFIX ".so" )
endif()

if(MSVC)
  set_target_properties(openstudio_rb PROPERTIES COMPILE_FLAGS "/bigobj /wd4996 /wd5033")
endif()

add_custom_command(TARGET openstudio_rb
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/openstudio.rb $<TARGET_FILE_DIR:openstudio_rb>/openstudio.rb
)

file(GLOB_RECURSE OPENSTUDIO_FILES FOLLOW_SYMLINKS RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}/openstudio/" "${CMAKE_CURRENT_SOURCE_DIR}/openstudio/**/*.*")
foreach(OPENSTUDIO_FILE ${OPENSTUDIO_FILES})
  add_custom_command(TARGET openstudio_rb
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_SOURCE_DIR}/openstudio/${OPENSTUDIO_FILE}" "$<TARGET_FILE_DIR:openstudio_rb>/openstudio/${OPENSTUDIO_FILE}"
  )
endforeach()

target_link_libraries(openstudio_rb
  init_openstudio
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

if( WIN32 )
  target_link_libraries(openstudio_rb ${RUBY_MINGW_STUB_LIB})
endif()

if (APPLE)
  target_link_libraries(openstudio_rb
    ${OPENSSL_CRYPTO_LIBRARY}
    ${OPENSSL_SSL_LIBRARY}
  )
endif()

install(TARGETS openstudio_rb DESTINATION Ruby COMPONENT "RubyAPI")
install(FILES openstudio.rb DESTINATION Ruby COMPONENT "RubyAPI")
install(DIRECTORY openstudio DESTINATION Ruby COMPONENT "RubyAPI")
