SET( target_name openstudio_ruleset )

INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_BINARY_DIR}/ )

SET( ${target_name}_src
  mainpage.hpp
  ModelUserScript.cpp
  ModelUserScript.hpp
  OSArgument.cpp
  OSArgument.hpp
  OSResult.cpp
  OSResult.hpp
  OSRunner.cpp
  OSRunner.hpp
  ReportingUserScript.cpp
  ReportingUserScript.hpp
  RubyUserScriptArgumentGetter.cpp
  RubyUserScriptArgumentGetter.hpp
  RulesetAPI.hpp
  TranslationUserScript.cpp
  TranslationUserScript.hpp
  UserScript.cpp
  UserScript.hpp
  UtilityUserScript.cpp
  UtilityUserScript.hpp
  WorkspaceUserScript.cpp
  WorkspaceUserScript.hpp
  RubyUserScriptArgumentGetter.cpp
  EmbeddedRubyUserScriptArgumentGetter.hpp
)

SET( ${target_name}_test_src
  test/RulesetFixture.hpp
  test/RulesetFixture.cpp
  test/UserScript_GTest.cpp
  test/OSArgument_GTest.cpp
  test/EmbeddedRuby_GTest.cpp
)

SET( ${target_name}_swig_src
  Ruleset.i
)

SET( ${target_name}_depends
  openstudio_osversion
)


ADD_LIBRARY( ${target_name} SHARED
  ${${target_name}_src}
  ${${target_name}_moc_src}
)

AddPCH(${target_name})

TARGET_LINK_LIBRARIES(${target_name} ${${target_name}_depends} )

IF( NOT APPLE )
INSTALL(TARGETS ${target_name}
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib )
ENDIF()

#IF(UNIX)
#   SET_TARGET_PROPERTIES( ${target_name} PROPERTIES COMPILE_DEFINITIONS "LINUX" )
#ENDIF()
# IF(MINGW)
#   SET_TARGET_PROPERTIES( ${target_name} PROPERTIES COMPILE_DEFINITIONS "WIN_GCC" )
# ENDIF()
IF(MSVC)
  SET_TARGET_PROPERTIES( ${target_name} PROPERTIES COMPILE_DEFINITIONS "WIN_MVC" )
ENDIF()
# IF(APPLE)
#   SET_TARGET_PROPERTIES( ${target_name} PROPERTIES COMPILE_DEFINITIONS "DARWIN" )
# ENDIF()

CREATE_SRC_GROUPS( "${${target_name}_src}" )
CREATE_SRC_GROUPS( "${${target_name}_test_src}" )
CREATE_SRC_GROUPS( "${${target_name}_swig_src}" )

CREATE_TEST_TARGETS( ${target_name} "${${target_name}_test_src}" "${${target_name}_depends};${RUBY_LIBRARY}")
MAKE_SWIG_TARGET( OpenStudioRuleset ruleset "${CMAKE_CURRENT_SOURCE_DIR}/Ruleset.i" "${${target_name}_swig_src}" ${target_name} OpenStudioOSVersion)

IF (BUILD_TESTING)
  ADD_DEPENDENCIES( "${target_name}_tests"
                    "openstudio_energyplus_resources"
                    "openstudio_standardsinterface_resources"
                    "ruby_OpenStudioUtilitiesCore"
                    "ruby_OpenStudioUtilitiesBCL"
                    "ruby_OpenStudioUtilitiesIdd"
                    "ruby_OpenStudioUtilitiesIdf"
                    "ruby_OpenStudioUtilities"
                    "ruby_OpenStudioModel"
                    "ruby_OpenStudioModelCore"
                    "ruby_OpenStudioModelSimulation"
                    "ruby_OpenStudioModelResources"
                    "ruby_OpenStudioModelGeometry"
                    "ruby_OpenStudioModelHVAC"
                    "ruby_OpenStudioEnergyPlus"
                    "ruby_OpenStudioRuleset" )
ENDIF ()

