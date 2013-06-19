SET( target_name openstudio_osversion )

SET( ${target_name}_src
  mainpage.hpp
  OSVersionAPI.hpp
  VersionTranslator.hpp
  VersionTranslator.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/../OpenStudio.hxx
)

SET( ${target_name}_test_src
  test/OSVersionFixture.hpp
  test/OSVersionFixture.cpp
  test/VersionTranslator_GTest.cpp
)

SET( ${target_name}_swig_src
  OSVersion.i
)

SET( ${target_name}_depends
  openstudio_model
)

ADD_LIBRARY( ${target_name} SHARED 
  ${${target_name}_src} 
)

AddPCH(${target_name})

TARGET_LINK_LIBRARIES( ${target_name} ${${target_name}_depends} )

IF( NOT APPLE )
INSTALL(TARGETS ${target_name} 
  RUNTIME DESTINATION bin 
  LIBRARY DESTINATION lib )
ENDIF()

SET (${target_name}_test_depends
  ${${target_name}_depends} 
)

CREATE_SRC_GROUPS( "${${target_name}_src}" )
CREATE_SRC_GROUPS( "${${target_name}_test_src}" )
CREATE_SRC_GROUPS( "${${target_name}_swig_src}" )

CREATE_TEST_TARGETS( ${target_name} "${${target_name}_test_src}" "${${target_name}_test_depends}" )
IF(BUILD_TESTING)

  ADD_DEPENDENCIES("${target_name}_tests"
    openstudio_osversion_resources
  )
    
ENDIF()

MAKE_SWIG_TARGET( OpenStudioOSVersion OSVersion "${CMAKE_CURRENT_SOURCE_DIR}/OSVersion.i" "${${target_name}_swig_src}" ${target_name} OpenStudioModel )

