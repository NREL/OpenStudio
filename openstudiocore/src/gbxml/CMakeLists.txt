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
  ${Boost_LIBRARIES}
  ${CMAKE_THREAD_LIBS}
  openstudio_utilities
  openstudio_model
  openstudio_energyplus
  #zkexpat
)

add_library(${target_name} ${${target_name}_src})
target_link_libraries(${target_name} ${${target_name}_depends})
AddPCH(${target_name})

set(${target_name}_static_depends
  openstudio_model_static
  openstudio_energyplus_static
)
add_library(${target_name}_static ${${target_name}_src})
target_link_libraries(${target_name}_static ${${target_name}_static_depends})

#No need to install static library
#if(NOT APPLE)
#install(TARGETS ${target_name}
#  RUNTIME DESTINATION bin
#  LIBRARY DESTINATION lib
#)
#endif()

CREATE_SRC_GROUPS("${${target_name}_src}")
CREATE_SRC_GROUPS("${${target_name}_test_src}")
CREATE_SRC_GROUPS("${${target_name}_swig_src}")

CREATE_TEST_TARGETS(${target_name} "${${target_name}_test_src}" "${${target_name}_depends}")
if(BUILD_TESTING)
  add_dependencies(${target_name}_tests openstudio_gbxml_resources)
endif()

MAKE_SWIG_TARGET(OpenStudioGBXML gbXML "${CMAKE_CURRENT_SOURCE_DIR}/gbXML.i" "${${target_name}_swig_src}" ${target_name}_static OpenStudioEnergyPlus)

#add_executable(gbXMLToIdf gbXMLToIdf.cpp)
#AddPCH(gbXMLToIdf)

#target_link_libraries(gbXMLToIdf
#  ${${target_name}_depends}
#  openstudio_gbxml
#  ${QT_LIBS}  	
#)

#install(TARGETS gbXMLToIdf
#  BUNDLE DESTINATION .
#  RUNTIME DESTINATION bin
#)
