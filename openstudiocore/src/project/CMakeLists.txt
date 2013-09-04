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
  CloudSessionRecord.hpp
  CloudSessionRecord_Impl.hpp
  CloudSessionRecord.cpp
  CloudSettingsRecord.hpp
  CloudSettingsRecord_Impl.hpp
  CloudSettingsRecord.cpp
  FileReferenceRecord.hpp
  FileReferenceRecord_Impl.hpp
  FileReferenceRecord.cpp
  TagRecord.hpp
  TagRecord_Impl.hpp
  TagRecord.cpp
  UrlRecord.hpp
  UrlRecord_Impl.hpp
  UrlRecord.cpp
  URLSearchPathRecord.hpp
  URLSearchPathRecord_Impl.hpp
  URLSearchPathRecord.cpp
  VagrantSessionRecord.hpp
  VagrantSessionRecord_Impl.hpp
  VagrantSessionRecord.cpp
  VagrantSettingsRecord.hpp
  VagrantSettingsRecord_Impl.hpp
  VagrantSettingsRecord.cpp

  # runmanager

  WorkflowRecord.hpp
  WorkflowRecord_Impl.hpp
  WorkflowRecord.cpp

  # ruleset

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
  DataPoint_Measure_JoinRecord.hpp
  DataPoint_Measure_JoinRecord_Impl.hpp
  DataPoint_Measure_JoinRecord.cpp
  DDACEAlgorithmRecord.hpp
  DDACEAlgorithmRecord_Impl.hpp
  DDACEAlgorithmRecord.cpp
  DesignOfExperimentsRecord.hpp
  DesignOfExperimentsRecord_Impl.hpp
  DesignOfExperimentsRecord.cpp
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
  MeasureGroupRecord.hpp
  MeasureGroupRecord_Impl.hpp
  MeasureGroupRecord.cpp
  MeasureRecord.hpp
  MeasureRecord_Impl.hpp
  MeasureRecord.cpp
  NullMeasureRecord.hpp
  NullMeasureRecord_Impl.hpp
  NullMeasureRecord.cpp
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
  RubyMeasureRecord.hpp
  RubyMeasureRecord_Impl.hpp
  RubyMeasureRecord.cpp
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
  CloudSessionRecord_Impl.hpp
  CloudSettingsRecord_Impl.hpp
  FileReferenceRecord_Impl.hpp
  TagRecord_Impl.hpp
  UrlRecord_Impl.hpp
  URLSearchPathRecord_Impl.hpp
  VagrantSessionRecord_Impl.hpp
  VagrantSettingsRecord_Impl.hpp

  # runmanager

  WorkflowRecord_Impl.hpp

  # ruleset

  OSArgumentRecord_Impl.hpp

  # analysis

  AlgorithmRecord_Impl.hpp
  AnalysisRecord_Impl.hpp
  ContinuousVariableRecord_Impl.hpp
  DakotaAlgorithmRecord_Impl.hpp
  DataPointRecord_Impl.hpp
  DataPointValueRecord_Impl.hpp
  DataPoint_Measure_JoinRecord_Impl.hpp
  DDACEAlgorithmRecord_Impl.hpp
  DesignOfExperimentsRecord_Impl.hpp
  DiscreteVariableRecord_Impl.hpp
  FSUDaceAlgorithmRecord_Impl.hpp
  FunctionRecord_Impl.hpp
  InputVariableRecord_Impl.hpp
  LinearFunctionRecord_Impl.hpp
  MeasureGroupRecord_Impl.hpp
  MeasureRecord_Impl.hpp
  NullMeasureRecord_Impl.hpp
  OpenStudioAlgorithmRecord_Impl.hpp
  OptimizationDataPointRecord_Impl.hpp
  OptimizationProblemRecord_Impl.hpp
  OutputAttributeVariableRecord_Impl.hpp
  OutputVariableRecord_Impl.hpp
  ParameterStudyAlgorithmRecord_Impl.hpp
  ProblemRecord_Impl.hpp
  PSUADEDaceAlgorithmRecord_Impl.hpp
  RubyContinuousVariableRecord_Impl.hpp
  RubyMeasureRecord_Impl.hpp
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
  Test/MeasureGroupRecord_GTest.cpp
  Test/FileReferenceRecord_GTest.cpp
  Test/ProblemRecord_GTest.cpp
  Test/ProjectDatabaseRecord_GTest.cpp
  Test/RubyMeasureRecord_GTest.cpp
  Test/RubyContinuousVariableRecord_GTest.cpp
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
