SET( target_name openstudio_contam )

SET( ${target_name}_src
  ContamAPI.hpp
  ForwardTranslator.hpp
  ForwardTranslator.cpp
  ReverseTranslator.hpp
  ReverseTranslator.cpp
  PrjReader.hpp
  PrjReader.cpp
  PrjWriter.hpp
  PrjWriter.cpp
  PrjObjects.hpp
  PrjObjects.cpp
  PrjDefs.hpp
  PrjData.hpp
  PrjData.cpp
)

# resource files
SET( ${target_name}_qrc
  contam.qrc
)

# generate rules for building source files from the resources
qt4_add_resources(${target_name}_qrcs ${${target_name}_qrc} )

SET( ${target_name}_test_src
  Test/ContamFixture.hpp
  Test/ContamFixture.cpp
  Test/ForwardTranslator_GTest.cpp
)

SET( ${target_name}_swig_src
)

SET( ${target_name}_depends
  ${Boost_LIBRARIES}
  ${CMAKE_THREAD_LIBS}
  ${QT_LIBS}
  openstudio_utilities
  openstudio_model
  zkexpat
  boost_log
)

ADD_LIBRARY( ${target_name} 
             SHARED 
			 ${${target_name}_src} 
			 ${${target_name}_qrcs} )

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
  ADD_DEPENDENCIES(${target_name}_tests openstudio_contam_resources)
ENDIF()

#MAKE_SWIG_TARGET( OpenStudioContam Contam "${CMAKE_CURRENT_SOURCE_DIR}/Contam.i" "${${target_name}_swig_src}" ${target_name} "${target_name}" )

add_executable(OsmToPrj OsmToPrj.cpp)

TARGET_LINK_LIBRARIES( OsmToPrj
  ${${target_name}_depends}
  openstudio_osversion
  openstudio_contam
)

add_executable(PrjToOsm PrjToOsm.cpp)

TARGET_LINK_LIBRARIES( PrjToOsm
  ${${target_name}_depends}
  openstudio_osversion
  openstudio_contam
)

add_executable(FitInfiltration FitInfiltration.cpp)

TARGET_LINK_LIBRARIES( FitInfiltration
  ${${target_name}_depends}
  openstudio_osversion
  openstudio_contam
)

#INSTALL(TARGETS gbXMLToIdf
#    BUNDLE DESTINATION . 
#    RUNTIME DESTINATION bin 
#    )
