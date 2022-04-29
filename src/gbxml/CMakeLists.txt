set(target_name openstudio_gbxml)

set(${target_name}_src
  gbXMLAPI.hpp
  mainpage.hpp
  ForwardTranslator.hpp
  ForwardTranslator.cpp
  ReverseTranslator.hpp
  ReverseTranslator.cpp
  MapEnvelope.cpp
  MapSchedules.cpp
)

set(${target_name}_test_src
  Test/gbXMLFixture.hpp
  Test/gbXMLFixture.cpp
  Test/ForwardTranslator_GTest.cpp
  Test/ReverseTranslator_GTest.cpp
)

set(${target_name}_swig_src
  gbXML.i
)

set(${target_name}_depends
  ${openstudio_energyplus_depends}
)
set(${target_name}_depends ${${target_name}_depends} PARENT_SCOPE)

add_library(${target_name} OBJECT ${${target_name}_src})
target_link_libraries(${target_name} PUBLIC ${${target_name}_depends})
target_compile_definitions(${target_name} PRIVATE openstudio_gbxml_EXPORTS SHARED_OS_LIBS)
add_dependencies(${target_name} GenerateIddFactoryRun)


set(${target_name}_test_depends
  openstudiolib
)

#CREATE_SRC_GROUPS("${${target_name}_src}")
#CREATE_SRC_GROUPS("${${target_name}_test_src}")
#CREATE_SRC_GROUPS("${${target_name}_swig_src}")

if(BUILD_TESTING)
  CREATE_TEST_TARGETS(${target_name} "${${target_name}_test_src}" "${${target_name}_test_depends}")
  add_dependencies(${target_name}_tests openstudio_gbxml_resources)
endif()

MAKE_SWIG_TARGET(OpenStudioGBXML gbXML "${CMAKE_CURRENT_SOURCE_DIR}/gbXML.i" "${${target_name}_swig_src}" ${target_name} OpenStudioEnergyPlus)
