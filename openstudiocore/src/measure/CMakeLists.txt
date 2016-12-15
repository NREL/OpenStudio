set(target_name openstudio_measure)

include_directories(${CMAKE_CURRENT_BINARY_DIR}/)

set(${target_name}_src
  mainpage.hpp
  EmbeddedRubyMeasureInfoGetter.hpp
  EnergyPlusMeasure.cpp
  EnergyPlusMeasure.hpp
  MeasureAPI.hpp
  ModelMeasure.cpp
  ModelMeasure.hpp
  OSArgument.cpp
  OSArgument.hpp
  OSMeasure.cpp
  OSMeasure.hpp
  OSMeasureInfoGetter.cpp
  OSMeasureInfoGetter.hpp
  OSOutput.cpp
  OSOutput.hpp  
  OSRunner.cpp
  OSRunner.hpp
  ReportingMeasure.cpp
  ReportingMeasure.hpp
)

set(${target_name}_test_src
  test/MeasureFixture.hpp
  test/MeasureFixture.cpp
  test/OSRunner_GTest.cpp
  test/OSMeasure_GTest.cpp
  test/OSOutput_GTest.cpp
  test/OSArgument_GTest.cpp
  #test/EmbeddedRuby_GTest.cpp
)

set(${target_name}_swig_src
  Measure.i
)

set(${target_name}_depends
  openstudio_osversion
)


add_library(${target_name} 
  ${${target_name}_src}
  ${${target_name}_moc_src}
)

AddPCH(${target_name})

target_link_libraries(${target_name} ${${target_name}_depends})

# No need to install static lib
#if(NOT APPLE)
#  install(TARGETS ${target_name}
#    RUNTIME DESTINATION bin
#    LIBRARY DESTINATION lib
#  )
#endif()

#if(UNIX)
#  set_target_properties(${target_name} PROPERTIES COMPILE_DEFINITIONS "LINUX")
#endif()
#if(MINGW)
#  set_target_properties(${target_name} PROPERTIES COMPILE_DEFINITIONS "WIN_GCC")
#endif()
if(MSVC)
  set_target_properties(${target_name} PROPERTIES COMPILE_DEFINITIONS "WIN_MVC")
endif()
#if(APPLE)
#  set_target_properties(${target_name} PROPERTIES COMPILE_DEFINITIONS "DARWIN")
#endif()

CREATE_SRC_GROUPS("${${target_name}_src}")
# CREATE_SRC_GROUPS("${${target_name}_test_src}")
CREATE_SRC_GROUPS("${${target_name}_swig_src}")

set(${target_name}_static_depends
  openstudio_osversion_static
)

add_library(${target_name}_static
  ${${target_name}_src}
  ${${target_name}_moc_src}
)

target_link_libraries(${target_name}_static ${${target_name}_static_depends})

if( MSVC )
  target_compile_definitions(${target_name}_static PUBLIC ${WIN_MVC})
endif()

#CREATE_TEST_TARGETS(${target_name} "${${target_name}_test_src}" "${${target_name}_depends};${RUBY_LIBRARY};${COREFOUNDATION_LIBRARY}")
CREATE_TEST_TARGETS(${target_name} "${${target_name}_test_src}" "${${target_name}_depends};${COREFOUNDATION_LIBRARY}")

MAKE_SWIG_TARGET(OpenStudioMeasure measure "${CMAKE_CURRENT_SOURCE_DIR}/Measure.i" "${${target_name}_swig_src}" ${target_name}_static OpenStudioOSVersion)

# if(BUILD_TESTING)
#  add_dependencies("${target_name}_tests"
#    "openstudio_energyplus_resources"
#    "ruby_OpenStudioUtilitiesCore"
#    "ruby_OpenStudioUtilitiesBCL"
#    "ruby_OpenStudioUtilitiesIdd"
#    "ruby_OpenStudioUtilitiesIdf"
#    "ruby_OpenStudioUtilities"
#    "ruby_OpenStudioModel"
#    "ruby_OpenStudioModelCore"
#    "ruby_OpenStudioModelSimulation"
#    "ruby_OpenStudioModelResources"
#    "ruby_OpenStudioModelGeometry"
#    "ruby_OpenStudioModelHVAC"
#    "ruby_OpenStudioEnergyPlus"
#    "ruby_OpenStudioMeasure"
#  )
#  
#  if(APPLE)
#    # the following script will change the bindings to prefer the version of libruby included with SketchUp to the system library, preventing loading two different copies of libruby
#    add_custom_command(TARGET ${target_name}_tests POST_BUILD COMMAND ${RUBY_EXECUTABLE} "${CMAKE_SOURCE_DIR}/SketchUpInstallName.rb" "$<TARGET_FILE:${target_name}_tests>")
#  endif() 
#endif()

