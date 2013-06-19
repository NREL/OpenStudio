# Generate DataDictionary
ADD_CUSTOM_COMMAND(
  OUTPUT "DataDictionary.xml" 
         "ObjectType.hxx" 
         "BooleanAttribute.hxx" 
         "IntegerAttribute.hxx" 
         "DoubleAttribute.hxx" 
         "EnumAttribute.hxx"
         "EnumAttributeValues.hxx"
         "DataDictionary.cxx"
  COMMAND GenerateDataDictionary "--outdir=${CMAKE_CURRENT_BINARY_DIR}" 
          "${CMAKE_SOURCE_DIR}/resources/standardsinterface/DataDictionary/Envelope_Objects.csv"
          "${CMAKE_SOURCE_DIR}/resources/standardsinterface/DataDictionary/Envelope_Attributes.csv"
          "${CMAKE_SOURCE_DIR}/resources/standardsinterface/DataDictionary/HVAC_Objects.csv"
          "${CMAKE_SOURCE_DIR}/resources/standardsinterface/DataDictionary/HVAC_Attributes.csv"
          "${CMAKE_SOURCE_DIR}/resources/standardsinterface/DataDictionary/Lighting_Objects.csv"
          "${CMAKE_SOURCE_DIR}/resources/standardsinterface/DataDictionary/Lighting_Attributes.csv"
          "${CMAKE_SOURCE_DIR}/resources/standardsinterface/DataDictionary/Project_Objects.csv"
          "${CMAKE_SOURCE_DIR}/resources/standardsinterface/DataDictionary/Project_Attributes.csv"
          "${CMAKE_SOURCE_DIR}/resources/standardsinterface/DataDictionary/Project_Measurements.csv"
  DEPENDS 
  GenerateDataDictionary
  "${CMAKE_SOURCE_DIR}/resources/standardsinterface/DataDictionary/Envelope_Objects.csv"
  "${CMAKE_SOURCE_DIR}/resources/standardsinterface/DataDictionary/Envelope_Attributes.csv"
  "${CMAKE_SOURCE_DIR}/resources/standardsinterface/DataDictionary/HVAC_Objects.csv"
  "${CMAKE_SOURCE_DIR}/resources/standardsinterface/DataDictionary/HVAC_Attributes.csv"
  "${CMAKE_SOURCE_DIR}/resources/standardsinterface/DataDictionary/Lighting_Objects.csv"
  "${CMAKE_SOURCE_DIR}/resources/standardsinterface/DataDictionary/Lighting_Attributes.csv"
  "${CMAKE_SOURCE_DIR}/resources/standardsinterface/DataDictionary/Project_Objects.csv"
  "${CMAKE_SOURCE_DIR}/resources/standardsinterface/DataDictionary/Project_Attributes.csv"
  "${CMAKE_SOURCE_DIR}/resources/standardsinterface/DataDictionary/Project_Measurements.csv"
)

SET( target_name openstudio_standardsinterface )

INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_BINARY_DIR}/ )

SET( ${target_name}_src
  StandardsInterfaceAPI.hpp
  mainpage.hpp
  Result.hpp
  Result.cpp
  AgnosticInterface.hpp
  OpenStudioStandardsInterface.hpp
  OpenStudioStandardsInterface.cpp
  Title24Interface.hpp
  Title24Interface.cpp
  DataDictionary.hpp
  DataDictionary.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/DataDictionary.cxx
  ${CMAKE_CURRENT_BINARY_DIR}/ObjectType.hxx
  ${CMAKE_CURRENT_BINARY_DIR}/BooleanAttribute.hxx
  ${CMAKE_CURRENT_BINARY_DIR}/IntegerAttribute.hxx
  ${CMAKE_CURRENT_BINARY_DIR}/DoubleAttribute.hxx
  ${CMAKE_CURRENT_BINARY_DIR}/EnumAttribute.hxx
  ${CMAKE_CURRENT_BINARY_DIR}/EnumAttributeValues.hxx
  ${CMAKE_SOURCE_DIR}/resources/standardsinterface/DataDictionary/DataDictionary.xml # to make copy happen
)

SET( ${target_name}_test_src
  test/StandardsInterfaceFixture.hpp
  test/StandardsInterfaceFixture.cpp
  test/Result_GTest.cpp
  test/DataDictionary_GTest.cpp
  test/OpenStudioStandardsInterface_GTest.cpp
  test/Title24Interface_GTest.cpp
)

SET( ${target_name}_swig_src
  StandardsInterface.i
)

SET( ${target_name}_depends
  openstudio_model
)

ADD_LIBRARY( ${target_name} SHARED ${${target_name}_src} )
TARGET_LINK_LIBRARIES(${target_name} ${${target_name}_depends} )
AddPCH(${target_name})

IF( NOT APPLE )
INSTALL(TARGETS ${target_name}
  RUNTIME DESTINATION bin 
  LIBRARY DESTINATION lib )
ENDIF()

CREATE_SRC_GROUPS( "${${target_name}_src}" )
CREATE_SRC_GROUPS( "${${target_name}_test_src}" )
CREATE_SRC_GROUPS( "${${target_name}_swig_src}" )

SET( ${target_name}_test_depends
  ${${target_name}_depends}
  openstudio_runmanager
)

CREATE_TEST_TARGETS( ${target_name} "${${target_name}_test_src}" "${${target_name}_test_depends}")
MAKE_SWIG_TARGET( OpenStudioStandardsInterface StandardsInterface "${CMAKE_CURRENT_SOURCE_DIR}/StandardsInterface.i" "${${target_name}_swig_src}" ${target_name} OpenStudioModel )

