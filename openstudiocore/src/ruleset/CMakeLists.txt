SET( target_name openstudio_ruleset )

INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_BINARY_DIR}/ )

SET( ${target_name}_src
  RulesetAPI.hpp
  mainpage.hpp

  OSArgument.hpp
  OSArgument.cpp
  OSResult.hpp
  OSResult.cpp
  OSRunner.hpp
  OSRunner.cpp

  RulesetObject.hpp
  RulesetObject_Impl.hpp
  RulesetObject.cpp
  Rule.hpp
  Rule_Impl.hpp
  Rule.cpp
  Clause.hpp
  Clause_Impl.hpp
  Clause.cpp
  FilterClause.hpp
  FilterClause_Impl.hpp
  FilterClause.cpp
  ActionClause.hpp
  ActionClause_Impl.hpp
  ActionClause.cpp
  Ruleset.hpp
  Ruleset_Impl.hpp
  Ruleset.cpp
  RulesetEnums.hpp
  RulesetOption.hpp
  RulesetOption_Impl.hpp
  RulesetOption.cpp

  ConcreteRulesetObjects.hpp
  StandardsRulesetEnums.hpp
  StandardsRuleset.hpp
  StandardsRuleset_Impl.hpp
  StandardsRuleset.cpp
  StandardsRulesetOption.hpp
  StandardsRulesetOption_Impl.hpp
  StandardsRulesetOption.cpp
  StandardsEnumRulesetOption.hpp
  StandardsEnumRulesetOption_Impl.hpp
  StandardsEnumRulesetOption.cpp
  StandardsRule.hpp
  StandardsRule_Impl.hpp
  StandardsRule.cpp
  StandardsAttributeCheckRule.hpp
  StandardsAttributeCheckRule_Impl.hpp
  StandardsAttributeCheckRule.cpp
  StandardsFilterClause.hpp
  StandardsFilterClause_Impl.hpp
  StandardsFilterClause.cpp
  StandardsFilterAvailableModel.hpp
  StandardsFilterAvailableModel_Impl.hpp
  StandardsFilterAvailableModel.cpp
  StandardsFilterAvailableObject.hpp
  StandardsFilterAvailableObject_Impl.hpp
  StandardsFilterAvailableObject.cpp
  StandardsFilterObjectAttribute.hpp
  StandardsFilterObjectAttribute_Impl.hpp
  StandardsFilterObjectAttribute.cpp
  StandardsFilterObjectBooleanAttribute.hpp
  StandardsFilterObjectBooleanAttribute_Impl.hpp
  StandardsFilterObjectBooleanAttribute.cpp
  StandardsFilterObjectEnumAttribute.hpp
  StandardsFilterObjectEnumAttribute_Impl.hpp
  StandardsFilterObjectEnumAttribute.cpp
  StandardsFilterObjectNumericalAttribute.hpp
  StandardsFilterObjectNumericalAttribute_Impl.hpp
  StandardsFilterObjectNumericalAttribute.cpp
  StandardsFilterObjectDoubleAttribute.hpp
  StandardsFilterObjectDoubleAttribute_Impl.hpp
  StandardsFilterObjectDoubleAttribute.cpp
  StandardsFilterObjectIntegerAttribute.hpp
  StandardsFilterObjectIntegerAttribute_Impl.hpp
  StandardsFilterObjectIntegerAttribute.cpp
  StandardsFilterRulesetOption.hpp
  StandardsFilterRulesetOption_Impl.hpp
  StandardsFilterRulesetOption.cpp
  StandardsActionClause.hpp
  StandardsActionClause_Impl.hpp
  StandardsActionClause.cpp
  StandardsActionCloneModel.hpp
  StandardsActionCloneModel_Impl.hpp
  StandardsActionCloneModel.cpp
  StandardsActionGetAttribute.hpp
  StandardsActionGetAttribute_Impl.hpp
  StandardsActionGetAttribute.cpp
  StandardsActionGetObjects.hpp
  StandardsActionGetObjects_Impl.hpp
  StandardsActionGetObjects.cpp
  StandardsActionRuleResult.hpp
  StandardsActionRuleResult_Impl.hpp
  StandardsActionRuleResult.cpp
  StandardsActionRuleSatisfied.hpp
  StandardsActionRuleSatisfied_Impl.hpp
  StandardsActionRuleSatisfied.cpp
  StandardsActionRuleViolated.hpp
  StandardsActionRuleViolated_Impl.hpp
  StandardsActionRuleViolated.cpp

  ModelRuleset.hpp
  ModelRuleset_Impl.hpp
  ModelRuleset.cpp
  ModelRule.hpp
  ModelRule_Impl.hpp
  ModelRule.cpp
  ModelObjectFilterClause.hpp
  ModelObjectFilterClause_Impl.hpp
  ModelObjectFilterClause.cpp
  ModelObjectFilterAttribute.hpp
  ModelObjectFilterAttribute_Impl.hpp
  ModelObjectFilterAttribute.cpp
  ModelObjectFilterBooleanAttribute.hpp
  ModelObjectFilterBooleanAttribute_Impl.hpp
  ModelObjectFilterBooleanAttribute.cpp
  ModelObjectFilterNumericAttribute.hpp
  ModelObjectFilterNumericAttribute_Impl.hpp
  ModelObjectFilterNumericAttribute.cpp
  ModelObjectFilterRelationship.hpp
  ModelObjectFilterRelationship_Impl.hpp
  ModelObjectFilterRelationship.cpp
  ModelObjectFilterStringAttribute.hpp
  ModelObjectFilterStringAttribute_Impl.hpp
  ModelObjectFilterStringAttribute.cpp
  ModelObjectFilterType.hpp
  ModelObjectFilterType_Impl.hpp
  ModelObjectFilterType.cpp
  ModelObjectActionClause.hpp
  ModelObjectActionClause_Impl.hpp
  ModelObjectActionClause.cpp
  ModelObjectActionSetAttribute.hpp
  ModelObjectActionSetAttribute_Impl.hpp
  ModelObjectActionSetAttribute.cpp
  ModelObjectActionSetRelationship.hpp
  ModelObjectActionSetRelationship_Impl.hpp
  ModelObjectActionSetRelationship.cpp

  UserScript.hpp
  UserScript.cpp
  ModelUserScript.hpp
  ModelUserScript.cpp
  WorkspaceUserScript.hpp
  WorkspaceUserScript.cpp
  TranslationUserScript.hpp
  TranslationUserScript.cpp
  UtilityUserScript.hpp
  UtilityUserScript.cpp
  RubyUserScriptArgumentGetter.hpp
  RubyUserScriptArgumentGetter.cpp
)

SET( ${target_name}_moc

  RulesetObject_Impl.hpp
  Rule_Impl.hpp
  Clause_Impl.hpp
  FilterClause_Impl.hpp
  ActionClause_Impl.hpp
  Ruleset_Impl.hpp
  RulesetOption_Impl.hpp

  StandardsRuleset_Impl.hpp
  StandardsRulesetOption_Impl.hpp
  StandardsEnumRulesetOption_Impl.hpp
  StandardsRule_Impl.hpp
  StandardsAttributeCheckRule_Impl.hpp
  StandardsFilterClause_Impl.hpp
  StandardsFilterAvailableModel_Impl.hpp
  StandardsFilterAvailableObject_Impl.hpp
  StandardsFilterObjectAttribute_Impl.hpp
  StandardsFilterObjectNumericalAttribute_Impl.hpp
  StandardsFilterObjectBooleanAttribute_Impl.hpp
  StandardsFilterObjectDoubleAttribute_Impl.hpp
  StandardsFilterObjectEnumAttribute_Impl.hpp
  StandardsFilterObjectIntegerAttribute_Impl.hpp
  StandardsFilterRulesetOption_Impl.hpp
  StandardsActionClause_Impl.hpp
  StandardsActionCloneModel_Impl.hpp
  StandardsActionGetAttribute_Impl.hpp
  StandardsActionGetObjects_Impl.hpp
  StandardsActionRuleResult_Impl.hpp
  StandardsActionRuleSatisfied_Impl.hpp
  StandardsActionRuleViolated_Impl.hpp

  ModelRuleset_Impl.hpp
  ModelRule_Impl.hpp
  ModelObjectFilterClause_Impl.hpp
  ModelObjectFilterAttribute_Impl.hpp
  ModelObjectFilterBooleanAttribute_Impl.hpp
  ModelObjectFilterNumericAttribute_Impl.hpp
  ModelObjectFilterRelationship_Impl.hpp
  ModelObjectFilterStringAttribute_Impl.hpp
  ModelObjectFilterType_Impl.hpp
  ModelObjectActionClause_Impl.hpp
  ModelObjectActionSetAttribute_Impl.hpp
  ModelObjectActionSetRelationship_Impl.hpp
)

qt4_wrap_cpp( ${target_name}_moc_src ${${target_name}_moc} )

SET( ${target_name}_test_src
  test/RulesetFixture.hpp
  test/RulesetFixture.cpp
  test/ModelRule_GTest.cpp
  test/ModelRuleset_GTest.cpp
  test/ModelObjectFilterClause_GTest.cpp
  test/StandardsRuleset_GTest.cpp
  test/StandardsRulesetOption_GTest.cpp
  test/StandardsRule_GTest.cpp
  test/StandardsFilterClause_GTest.cpp
  test/StandardsActionClause_GTest.cpp
  test/UserScript_GTest.cpp
  test/OSArgument_GTest.cpp
  test/EmbeddedRuby_GTest.cpp
)

SET( ${target_name}_swig_src
  Ruleset.i
)

SET( ${target_name}_depends
  openstudio_osversion
  openstudio_standardsinterface
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
MAKE_SWIG_TARGET( OpenStudioRuleset ruleset "${CMAKE_CURRENT_SOURCE_DIR}/Ruleset.i" "${${target_name}_swig_src}" ${target_name} OpenStudioOSVersion OpenStudioStandardsInterface )

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

