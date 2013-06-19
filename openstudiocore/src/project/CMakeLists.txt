SET( target_name openstudio_project )

SET( ${target_name}_src
  ProjectAPI.hpp
  mainpage.hpp
  JoinRecord.hpp
  JoinRecord_Impl.hpp
  JoinRecord.cpp
  ObjectRecord.hpp
  ObjectRecord_Impl.hpp
  ObjectRecord.cpp
  ConcreteObjectRecords.hpp
  ConcreteJoinRecords.hpp
  ProjectDatabase.hpp
  ProjectDatabase_Impl.hpp
  ProjectDatabase.cpp
  ProjectDatabaseRecord.hpp
  ProjectDatabaseRecord_Impl.hpp
  ProjectDatabaseRecord.cpp
  Record.hpp
  Record_Impl.hpp
  Record.cpp

  # utilities

  AttributeRecord.hpp
  AttributeRecord_Impl.hpp
  AttributeRecord.cpp
  FileReferenceRecord.hpp
  FileReferenceRecord_Impl.hpp
  FileReferenceRecord.cpp
  TagRecord.hpp
  TagRecord_Impl.hpp
  TagRecord.cpp
  URLSearchPathRecord.hpp
  URLSearchPathRecord_Impl.hpp
  URLSearchPathRecord.cpp

  # runmanager

  WorkflowRecord.hpp
  WorkflowRecord_Impl.hpp
  WorkflowRecord.cpp

  # ruleset

  ActionClauseRecord.hpp
  ActionClauseRecord_Impl.hpp
  ActionClauseRecord.cpp
  ClauseRecord.hpp
  ClauseRecord_Impl.hpp
  ClauseRecord.cpp
  FilterClauseRecord.hpp
  FilterClauseRecord_Impl.hpp
  FilterClauseRecord.cpp
  ModelObjectActionClauseRecord.hpp
  ModelObjectActionClauseRecord_Impl.hpp
  ModelObjectActionClauseRecord.cpp
  ModelObjectActionSetAttributeRecord.hpp
  ModelObjectActionSetAttributeRecord_Impl.hpp
  ModelObjectActionSetAttributeRecord.cpp
  ModelObjectActionSetRelationshipRecord.hpp
  ModelObjectActionSetRelationshipRecord_Impl.hpp
  ModelObjectActionSetRelationshipRecord.cpp
  ModelObjectFilterAttributeRecord.hpp
  ModelObjectFilterAttributeRecord_Impl.hpp
  ModelObjectFilterAttributeRecord.cpp
  ModelObjectFilterBooleanAttributeRecord.hpp
  ModelObjectFilterBooleanAttributeRecord_Impl.hpp
  ModelObjectFilterBooleanAttributeRecord.cpp
  ModelObjectFilterClauseRecord.hpp
  ModelObjectFilterClauseRecord_Impl.hpp
  ModelObjectFilterClauseRecord.cpp
  ModelObjectFilterNumericAttributeRecord.hpp
  ModelObjectFilterNumericAttributeRecord_Impl.hpp
  ModelObjectFilterNumericAttributeRecord.cpp
  ModelObjectFilterRelationshipRecord.hpp
  ModelObjectFilterRelationshipRecord_Impl.hpp
  ModelObjectFilterRelationshipRecord.cpp
  ModelObjectFilterStringAttributeRecord.hpp
  ModelObjectFilterStringAttributeRecord_Impl.hpp
  ModelObjectFilterStringAttributeRecord.cpp
  ModelObjectFilterTypeRecord.hpp
  ModelObjectFilterTypeRecord_Impl.hpp
  ModelObjectFilterTypeRecord.cpp
  ModelRuleRecord.hpp
  ModelRuleRecord_Impl.hpp
  ModelRuleRecord.cpp
  ModelRulesetRecord.hpp
  ModelRulesetRecord_Impl.hpp
  ModelRulesetRecord.cpp
  Rule_Clause_JoinRecord.hpp
  Rule_Clause_JoinRecord_Impl.hpp
  Rule_Clause_JoinRecord.cpp
  RuleRecord.hpp
  RuleRecord_Impl.hpp
  RuleRecord.cpp
  Ruleset_Rule_JoinRecord.hpp
  Ruleset_Rule_JoinRecord_Impl.hpp
  Ruleset_Rule_JoinRecord.cpp
  RulesetOptionRecord.hpp
  RulesetOptionRecord_Impl.hpp
  RulesetOptionRecord.cpp
  RulesetRecord.hpp
  RulesetRecord_Impl.hpp
  RulesetRecord.cpp
  StandardsActionClauseRecord.hpp
  StandardsActionClauseRecord_Impl.hpp
  StandardsActionClauseRecord.cpp
  StandardsActionCloneModelRecord.hpp
  StandardsActionCloneModelRecord_Impl.hpp
  StandardsActionCloneModelRecord.cpp
  StandardsActionRuleResultRecord.hpp
  StandardsActionRuleResultRecord_Impl.hpp
  StandardsActionRuleResultRecord.cpp
  StandardsActionRuleSatisfiedRecord.hpp
  StandardsActionRuleSatisfiedRecord_Impl.hpp
  StandardsActionRuleSatisfiedRecord.cpp
  StandardsActionRuleViolatedRecord.hpp
  StandardsActionRuleViolatedRecord_Impl.hpp
  StandardsActionRuleViolatedRecord.cpp
  StandardsAttributeCheckRuleRecord.hpp
  StandardsAttributeCheckRuleRecord_Impl.hpp
  StandardsAttributeCheckRuleRecord.cpp
  StandardsEnumRulesetOptionRecord.hpp
  StandardsEnumRulesetOptionRecord_Impl.hpp
  StandardsEnumRulesetOptionRecord.cpp
  StandardsFilterAvailableModelRecord.hpp
  StandardsFilterAvailableModelRecord_Impl.hpp
  StandardsFilterAvailableModelRecord.cpp
  StandardsFilterAvailableObjectRecord.hpp
  StandardsFilterAvailableObjectRecord_Impl.hpp
  StandardsFilterAvailableObjectRecord.cpp
  StandardsFilterClauseRecord.hpp
  StandardsFilterClauseRecord_Impl.hpp
  StandardsFilterClauseRecord.cpp
  StandardsFilterObjectAttributeRecord.hpp
  StandardsFilterObjectAttributeRecord_Impl.hpp
  StandardsFilterObjectAttributeRecord.cpp
  StandardsFilterObjectBooleanAttributeRecord.hpp
  StandardsFilterObjectBooleanAttributeRecord_Impl.hpp
  StandardsFilterObjectBooleanAttributeRecord.cpp
  StandardsFilterObjectDoubleAttributeRecord.hpp
  StandardsFilterObjectDoubleAttributeRecord_Impl.hpp
  StandardsFilterObjectDoubleAttributeRecord.cpp
  StandardsFilterObjectEnumAttributeRecord.hpp
  StandardsFilterObjectEnumAttributeRecord_Impl.hpp
  StandardsFilterObjectEnumAttributeRecord.cpp
  StandardsFilterObjectIntegerAttributeRecord.hpp
  StandardsFilterObjectIntegerAttributeRecord_Impl.hpp
  StandardsFilterObjectIntegerAttributeRecord.cpp
  StandardsFilterRulesetOptionRecord.hpp
  StandardsFilterRulesetOptionRecord_Impl.hpp
  StandardsFilterRulesetOptionRecord.cpp
  StandardsRuleRecord.hpp
  StandardsRuleRecord_Impl.hpp
  StandardsRuleRecord.cpp
  StandardsRulesetOptionRecord.hpp
  StandardsRulesetOptionRecord_Impl.hpp
  StandardsRulesetOptionRecord.cpp
  StandardsRulesetRecord.hpp
  StandardsRulesetRecord_Impl.hpp
  StandardsRulesetRecord.cpp
  OSArgumentRecord.hpp
  OSArgumentRecord_Impl.hpp
  OSArgumentRecord.cpp

  # analysis

  AlgorithmRecord.hpp
  AlgorithmRecord_Impl.hpp
  AlgorithmRecord.cpp
  AnalysisRecord.hpp
  AnalysisRecord_Impl.hpp
  AnalysisRecord.cpp
  ContinuousVariableRecord.hpp
  ContinuousVariableRecord_Impl.hpp
  ContinuousVariableRecord.cpp
  DakotaAlgorithmRecord.hpp
  DakotaAlgorithmRecord_Impl.hpp
  DakotaAlgorithmRecord.cpp
  DataPointRecord.hpp
  DataPointRecord_Impl.hpp
  DataPointRecord.cpp
  DataPointValueRecord.hpp
  DataPointValueRecord_Impl.hpp
  DataPointValueRecord.cpp
  DataPoint_DiscretePerturbation_JoinRecord.hpp
  DataPoint_DiscretePerturbation_JoinRecord_Impl.hpp
  DataPoint_DiscretePerturbation_JoinRecord.cpp
  DDACEAlgorithmRecord.hpp
  DDACEAlgorithmRecord_Impl.hpp
  DDACEAlgorithmRecord.cpp
  DesignOfExperimentsRecord.hpp
  DesignOfExperimentsRecord_Impl.hpp
  DesignOfExperimentsRecord.cpp
  DiscretePerturbationRecord.hpp
  DiscretePerturbationRecord_Impl.hpp
  DiscretePerturbationRecord.cpp
  DiscreteVariableRecord.hpp
  DiscreteVariableRecord_Impl.hpp
  DiscreteVariableRecord.cpp
  FSUDaceAlgorithmRecord.hpp
  FSUDaceAlgorithmRecord_Impl.hpp
  FSUDaceAlgorithmRecord.cpp
  FunctionRecord.hpp
  FunctionRecord_Impl.hpp
  FunctionRecord.cpp
  InputVariableRecord.hpp
  InputVariableRecord_Impl.hpp
  InputVariableRecord.cpp
  LinearFunctionRecord.hpp
  LinearFunctionRecord_Impl.hpp
  LinearFunctionRecord.cpp
  ModelRulesetContinuousVariableRecord.hpp
  ModelRulesetContinuousVariableRecord_Impl.hpp
  ModelRulesetContinuousVariableRecord.cpp
  ModelRulesetPerturbationRecord.hpp
  ModelRulesetPerturbationRecord_Impl.hpp
  ModelRulesetPerturbationRecord.cpp
  NullPerturbationRecord.hpp
  NullPerturbationRecord_Impl.hpp
  NullPerturbationRecord.cpp
  OpenStudioAlgorithmRecord.hpp
  OpenStudioAlgorithmRecord_Impl.hpp
  OpenStudioAlgorithmRecord.cpp
  OptimizationDataPointRecord.hpp
  OptimizationDataPointRecord_Impl.hpp
  OptimizationDataPointRecord.cpp
  OptimizationProblemRecord.hpp
  OptimizationProblemRecord_Impl.hpp
  OptimizationProblemRecord.cpp
  OutputAttributeVariableRecord.hpp
  OutputAttributeVariableRecord_Impl.hpp
  OutputAttributeVariableRecord.cpp
  OutputVariableRecord.hpp
  OutputVariableRecord_Impl.hpp
  OutputVariableRecord.cpp
  ParameterStudyAlgorithmRecord.hpp
  ParameterStudyAlgorithmRecord_Impl.hpp
  ParameterStudyAlgorithmRecord.cpp
  ProblemRecord.hpp
  ProblemRecord_Impl.hpp
  ProblemRecord.cpp
  PSUADEDaceAlgorithmRecord.hpp
  PSUADEDaceAlgorithmRecord_Impl.hpp
  PSUADEDaceAlgorithmRecord.cpp
  RubyContinuousVariableRecord.hpp
  RubyContinuousVariableRecord_Impl.hpp
  RubyContinuousVariableRecord.cpp
  RubyPerturbationRecord.hpp
  RubyPerturbationRecord_Impl.hpp
  RubyPerturbationRecord.cpp
  SamplingAlgorithmRecord.hpp
  SamplingAlgorithmRecord_Impl.hpp
  SamplingAlgorithmRecord.cpp
  SequentialSearchRecord.hpp
  SequentialSearchRecord_Impl.hpp
  SequentialSearchRecord.cpp
  VariableRecord.hpp
  VariableRecord_Impl.hpp
  VariableRecord.cpp
)

SET( ${target_name}_moc
  JoinRecord_Impl.hpp
  ObjectRecord_Impl.hpp
  ProjectDatabase_Impl.hpp
  ProjectDatabaseRecord_Impl.hpp
  Record_Impl.hpp

  # utilities

  AttributeRecord_Impl.hpp
  FileReferenceRecord_Impl.hpp
  TagRecord_Impl.hpp
  URLSearchPathRecord_Impl.hpp

  # runmanager

  WorkflowRecord_Impl.hpp

  # ruleset

  ActionClauseRecord_Impl.hpp
  ClauseRecord_Impl.hpp
  FilterClauseRecord_Impl.hpp
  ModelObjectActionClauseRecord_Impl.hpp
  ModelObjectActionSetAttributeRecord_Impl.hpp
  ModelObjectActionSetRelationshipRecord_Impl.hpp
  ModelObjectFilterAttributeRecord_Impl.hpp
  ModelObjectFilterBooleanAttributeRecord_Impl.hpp
  ModelObjectFilterClauseRecord_Impl.hpp
  ModelObjectFilterNumericAttributeRecord_Impl.hpp
  ModelObjectFilterRelationshipRecord_Impl.hpp
  ModelObjectFilterStringAttributeRecord_Impl.hpp
  ModelObjectFilterTypeRecord_Impl.hpp
  ModelRuleRecord_Impl.hpp
  ModelRulesetRecord_Impl.hpp
  Rule_Clause_JoinRecord_Impl.hpp
  RuleRecord_Impl.hpp
  Ruleset_Rule_JoinRecord_Impl.hpp
  RulesetOptionRecord_Impl.hpp
  RulesetRecord_Impl.hpp
  StandardsActionClauseRecord_Impl.hpp
  StandardsActionCloneModelRecord_Impl.hpp
  StandardsActionRuleResultRecord_Impl.hpp
  StandardsActionRuleSatisfiedRecord_Impl.hpp
  StandardsActionRuleViolatedRecord_Impl.hpp
  StandardsAttributeCheckRuleRecord_Impl.hpp
  StandardsEnumRulesetOptionRecord_Impl.hpp
  StandardsFilterAvailableModelRecord_Impl.hpp
  StandardsFilterAvailableObjectRecord_Impl.hpp
  StandardsFilterClauseRecord_Impl.hpp
  StandardsFilterObjectAttributeRecord_Impl.hpp
  StandardsFilterObjectBooleanAttributeRecord_Impl.hpp
  StandardsFilterObjectDoubleAttributeRecord_Impl.hpp
  StandardsFilterObjectEnumAttributeRecord_Impl.hpp
  StandardsFilterObjectIntegerAttributeRecord_Impl.hpp
  StandardsFilterRulesetOptionRecord_Impl.hpp
  StandardsRuleRecord_Impl.hpp
  StandardsRulesetOptionRecord_Impl.hpp
  StandardsRulesetRecord_Impl.hpp
  OSArgumentRecord_Impl.hpp

  # analysis

  AlgorithmRecord_Impl.hpp
  AnalysisRecord_Impl.hpp
  ContinuousVariableRecord_Impl.hpp
  DakotaAlgorithmRecord_Impl.hpp
  DataPointRecord_Impl.hpp
  DataPointValueRecord_Impl.hpp
  DataPoint_DiscretePerturbation_JoinRecord_Impl.hpp
  DDACEAlgorithmRecord_Impl.hpp
  DesignOfExperimentsRecord_Impl.hpp
  DiscretePerturbationRecord_Impl.hpp
  DiscreteVariableRecord_Impl.hpp
  FSUDaceAlgorithmRecord_Impl.hpp
  FunctionRecord_Impl.hpp
  InputVariableRecord_Impl.hpp
  LinearFunctionRecord_Impl.hpp
  ModelRulesetContinuousVariableRecord_Impl.hpp
  ModelRulesetPerturbationRecord_Impl.hpp
  NullPerturbationRecord_Impl.hpp
  OpenStudioAlgorithmRecord_Impl.hpp
  OptimizationDataPointRecord_Impl.hpp
  OptimizationProblemRecord_Impl.hpp
  OutputAttributeVariableRecord_Impl.hpp
  OutputVariableRecord_Impl.hpp
  ParameterStudyAlgorithmRecord_Impl.hpp
  ProblemRecord_Impl.hpp
  PSUADEDaceAlgorithmRecord_Impl.hpp
  RubyContinuousVariableRecord_Impl.hpp
  RubyPerturbationRecord_Impl.hpp
  SamplingAlgorithmRecord_Impl.hpp
  SequentialSearchRecord_Impl.hpp
  VariableRecord_Impl.hpp
)

qt4_wrap_cpp( ${target_name}_moc_src ${${target_name}_moc} )

SET( ${target_name}_depends
  openstudio_energyplus
  openstudio_runmanager
  openstudio_ruleset
  openstudio_analysis
)

ADD_LIBRARY( ${target_name} SHARED
  ${${target_name}_src}
  ${${target_name}_moc_src}
 )
TARGET_LINK_LIBRARIES(${target_name} ${${target_name}_depends} )

IF( NOT APPLE )
INSTALL(TARGETS ${target_name}
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib )
ENDIF()

SET( ${target_name}_test_src
  Test/ProjectFixture.hpp
  Test/ProjectFixture.cpp
  Test/AlgorithmRecord_GTest.cpp
  Test/AnalysisRecord_GTest.cpp
  Test/AttributeRecord_GTest.cpp
  Test/DataPointRecord_GTest.cpp
  Test/DiscreteVariableRecord_GTest.cpp
  Test/FileReferenceRecord_GTest.cpp
  Test/ModelObjectFilterTypeRecord_GTest.cpp
  Test/ModelRuleRecord_GTest.cpp
  Test/ModelRulesetRecord_GTest.cpp
  Test/ProblemRecord_GTest.cpp
  Test/ProjectDatabaseRecord_GTest.cpp
  Test/RubyPerturbationRecord_GTest.cpp
  Test/RubyContinuousVariableRecord_GTest.cpp
  Test/StandardsAttributeCheckRuleRecord_GTest.cpp
  Test/StandardsRulesetRecord_GTest.cpp
  Test/TagRecord_GTest.cpp
  Test/URLSearchPathRecord_GTest.cpp
  Test/WorkflowRecord_GTest.cpp
  Test/ProjectVersioningFixture.hpp
  Test/ProjectVersioningFixture.cpp
  Test/Versioning_GTest.cpp
)

SET( project_swig_src
  Project.i
)

MAKE_SWIG_TARGET( OpenStudioProject project "${CMAKE_CURRENT_SOURCE_DIR}/Project.i" "${project_swig_src}" "openstudio_project" "OpenStudioRunManager;OpenStudioRuleset;OpenStudioAnalysis")

CREATE_TEST_TARGETS( ${target_name} "${${target_name}_test_src}" "${${target_name}_depends}" )

AddPCH(${target_name})

IF( BUILD_TESTING )
  ADD_DEPENDENCIES(${target_name}_tests
      openstudio_runmanager_resources
      openstudio_project_resources)
ENDIF()
