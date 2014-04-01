SET( target_name openstudio_gbxml )

SET( ${target_name}_src
  gbXMLAPI.hpp
  mainpage.hpp
  ForwardTranslator.hpp
  ForwardTranslator.cpp
  ReverseTranslator.hpp
  ReverseTranslator.cpp
  MapEnvelope.cpp
  MapSchedules.cpp
)

SET( ${target_name}_test_src
  Test/gbXMLFixture.hpp
  Test/gbXMLFixture.cpp
  Test/ForwardTranslator_GTest.cpp
  Test/ReverseTranslator_GTest.cpp
)

SET( ${target_name}_swig_src
  gbXML.i
)

SET( ${target_name}_depends
  ${Boost_LIBRARIES}
  ${CMAKE_THREAD_LIBS}
  ${QT_LIBS}
  openstudio_utilities
  openstudio_model
  openstudio_energyplus
  zkexpat
)

ADD_LIBRARY( ${target_name} SHARED ${${target_name}_src} )
TARGET_LINK_LIBRARIES( ${target_name} ${${target_name}_depends} )
AddPCH(${target_name})

#IF( NOT APPLE )
INSTALL(TARGETS ${target_name} 
  RUNTIME DESTINATION bin 
  LIBRARY DESTINATION lib )
#ENDIF()

CREATE_SRC_GROUPS( "${${target_name}_src}" )
CREATE_SRC_GROUPS( "${${target_name}_test_src}" )
CREATE_SRC_GROUPS( "${${target_name}_swig_src}" )

CREATE_TEST_TARGETS( ${target_name} "${${target_name}_test_src}" "${${target_name}_depends}")
IF(BUILD_TESTING)
  ADD_DEPENDENCIES(${target_name}_tests openstudio_gbxml_resources)
ENDIF()

MAKE_SWIG_TARGET( OpenStudioGBXML gbXML "${CMAKE_CURRENT_SOURCE_DIR}/gbXML.i" "${${target_name}_swig_src}" ${target_name} OpenStudioEnergyPlus)

add_executable(gbXMLToIdf gbXMLToIdf.cpp)
AddPCH(gbXMLToIdf)

TARGET_LINK_LIBRARIES( gbXMLToIdf
  ${${target_name}_depends}
  openstudio_gbxml
)

INSTALL(TARGETS gbXMLToIdf
  BUNDLE DESTINATION . 
  RUNTIME DESTINATION bin 
)
