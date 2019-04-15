if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "^(Apple)?Clang$")
  # using Clang
  set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -undefined dynamic_lookup")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  # using GCC
  #set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} --unresolved-symbols=ignore-all")
  set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} --unresolved-symbols=ignore-all")
endif()

#add_definitions(-DQT_NO_VERSION_TAGGING)

include_directories(${RUBY_INCLUDE_DIRS} ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_BINARY_DIR} ${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR} )

# common library for openstudio.so and openstudio.exe

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
 ##ruby_OpenStudioModelEditor # linked by openstudio.so only, for SketchUp plugin
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
 #openstudio_utilities
 #${Boost_LIBRARIES}
 #${QT_STATIC_LIBS}
)

# openstudio ruby bindings to load into normal ruby

add_library(
  openstudio_rb
  MODULE
  openstudio_rb.cpp
)

set_target_properties(openstudio_rb PROPERTIES PREFIX "")
set_target_properties(openstudio_rb PROPERTIES OUTPUT_NAME openstudio)

# change output directory so the openstudio.pdb file does not conflict between this target and the CLI
set_target_properties(openstudio_rb PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/ruby/")
set_target_properties(openstudio_rb PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/ruby/")
set_target_properties(openstudio_rb PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/ruby/")
if(APPLE)
  set_target_properties(openstudio_rb PROPERTIES SUFFIX ".bundle" )
else()
  set_target_properties(openstudio_rb PROPERTIES SUFFIX ".so" )
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
  ruby_OpenStudioModelEditor
  openstudio_airflow_static
  openstudio_model_static
  openstudio_energyplus_static
  openstudio_measure_static
  openstudio_osversion_static
  openstudio_sdd_static
  openstudio_isomodel_static
  openstudio_gbxml_static
  openstudio_radiance_static
  openstudio_modeleditor_static
)

if( UNIX AND NOT APPLE )
  # DLM: dependencies of Qt Static plugins do not seem to be linked in to openstudio.so on Unix. Force these to link fully.
  target_link_libraries(openstudio_rb
    -Wl,--whole-archive "${QT_STATIC_INSTALL_DIR}/lib/libxcb-static.a" -Wl,--no-whole-archive
  )
  target_link_libraries(openstudio_rb
    -Wl,--whole-archive "${QT_STATIC_INSTALL_DIR}/lib/libQt5PlatformSupport.a" -Wl,--no-whole-archive
  )
  target_link_libraries(openstudio_rb
    -Wl,--whole-archive "${QT_STATIC_INSTALL_DIR}/lib/libQt5XcbQpa.a" -Wl,--no-whole-archive
  )
  target_link_libraries(openstudio_rb
    -Wl,--whole-archive "${QT_STATIC_INSTALL_DIR}/lib/libQt5Sql.a" -Wl,--no-whole-archive
  )
endif()

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

if( BUILD_PAT )
  if( APPLE )
    install(TARGETS openstudio_rb
      DESTINATION ParametricAnalysisTool.app/Contents/Resources/OpenStudio/Ruby/
      COMPONENT PAT
    )
    install(FILES openstudio.rb
      DESTINATION ParametricAnalysisTool.app/Contents/Resources/OpenStudio/Ruby/
      COMPONENT PAT
    )
    install(DIRECTORY openstudio
      DESTINATION ParametricAnalysisTool.app/Contents/Resources/OpenStudio/Ruby/
      COMPONENT PAT
    )
  endif()
endif()

