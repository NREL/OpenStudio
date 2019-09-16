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
)

set(${target_name}_swig_src
  Measure.i
)

CREATE_SRC_GROUPS("${${target_name}_src}")
# CREATE_SRC_GROUPS("${${target_name}_test_src}")
CREATE_SRC_GROUPS("${${target_name}_swig_src}")

set(${target_name}_depends
  openstudio_osversion
)

add_library(${target_name}
  ${${target_name}_src}
)

target_link_libraries(${target_name} ${${target_name}_depends})

# No need to install static lib
#if(NOT APPLE)
#  install(TARGETS ${target_name}
#    RUNTIME DESTINATION bin
#    LIBRARY DESTINATION lib
#  )
#endif()

if(MSVC)
  target_compile_definitions(${target_name} PUBLIC ${WIN_MVC})
endif()

CREATE_TEST_TARGETS(${target_name} "${${target_name}_test_src}" "${${target_name}_depends};${COREFOUNDATION_LIBRARY}")

MAKE_SWIG_TARGET(OpenStudioMeasure measure "${CMAKE_CURRENT_SOURCE_DIR}/Measure.i" "${${target_name}_swig_src}" ${target_name} OpenStudioOSVersion)
