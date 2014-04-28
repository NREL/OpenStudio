SET( target_name openstudio_sdd )

SET( ${target_name}_src
  SDDAPI.hpp
  mainpage.hpp
  ForwardTranslator.hpp
  ForwardTranslator.cpp
  ReverseTranslator.hpp
  ReverseTranslator.cpp
  MapEnvelope.cpp
  MapGeometry.cpp
  MapSchedules.cpp
  MapHVAC.cpp
)

SET( ${target_name}_test_src
  Test/SDDFixture.hpp
  Test/SDDFixture.cpp
  Test/ForwardTranslator_GTest.cpp
  Test/ReverseTranslator_GTest.cpp
)

SET( ${target_name}_swig_src
  SDD.i
)

SET( ${target_name}_depends
  ${Boost_LIBRARIES}
  ${CMAKE_THREAD_LIBS}
  ${QT_LIBS}
  openstudio_utilities
  openstudio_model
  openstudio_energyplus
  openstudio_osversion
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
  ADD_DEPENDENCIES(${target_name}_tests openstudio_sdd_resources)
ENDIF()

MAKE_SWIG_TARGET( OpenStudioSDD SDD "${CMAKE_CURRENT_SOURCE_DIR}/SDD.i" "${${target_name}_swig_src}" ${target_name}   "OpenStudioEnergyPlus;OpenStudioOSVersion" )
