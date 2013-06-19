SET( target_name openstudio_eefg )

SET( ${target_name}_src
  EefgAPI.hpp
  Eefg.hpp
  Eefg.cpp
)

SET( ${target_name}_test_src
  Test/Eefg_GTest.cpp
)

SET( ${target_name}_depends
  openstudio_utilities
)

CREATE_SRC_GROUPS( "${${target_name}_src}" )

ADD_LIBRARY( ${target_name} SHARED ${${target_name}_src} )
SET_TARGET_PROPERTIES( ${target_name} PROPERTIES OUTPUT_NAME "${target_name}${CMAKE_SHARD_LIBRARY_SUFFIX}" )
TARGET_LINK_LIBRARIES( ${target_name} ${${target_name}_depends} )

INSTALL(TARGETS ${target_name} 
  RUNTIME DESTINATION bin 
  LIBRARY DESTINATION lib 
  ARCHIVE DESTINATION lib)

CREATE_TEST_TARGETS( "${target_name}" "${${target_name}_test_src}" "${${target_name}_depends}" )
IF( BUILD_TESTING )
  ADD_DEPENDENCIES( "${target_name}_tests" "openstudio_energyplus_resources" )
ENDIF()

#ADD_LIBRARY( ${target_name}_lib ${${target_name}_src} )
#TARGET_LINK_LIBRARIES( ${target_name}_lib ${${target_name}_depends} )

#CREATE_TEST_TARGETS( "${target_name}_lib" "${${target_name}_test_src}" "${${target_name}_depends}" )
#IF( BUILD_TESTING )
#  ADD_DEPENDENCIES( "${target_name}_lib_tests" "openstudio_energyplus_resources" )
#ENDIF()

#INSTALL(TARGETS ${target_name}_lib 
#  RUNTIME DESTINATION bin 
#  LIBRARY DESTINATION lib 
#  ARCHIVE DESTINATION lib)


