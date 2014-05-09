set(target_name openstudio_analysis)

set(${target_name}_src
  mainpage.hpp
  AnalysisAPI.hpp
  AnalysisEnums.hpp
  AnalysisObject.hpp
  AnalysisObject_Impl.hpp
  AnalysisObject.cpp
  ConcreteAnalysisObjects.hpp
  Algorithm.hpp
  Algorithm_Impl.hpp
  Algorithm.cpp
  AlgorithmOptions.hpp
  AlgorithmOptions_Impl.hpp
  AlgorithmOptions.cpp
  Analysis.hpp
  Analysis_Impl.hpp
  Analysis.cpp
  BetaDistribution.hpp
  BetaDistribution.cpp
  BinomialDistribution.hpp
  BinomialDistribution.cpp
  ContinuousVariable.hpp
  ContinuousVariable_Impl.hpp
  ContinuousVariable.cpp
  DakotaAlgorithm.hpp
  DakotaAlgorithm_Impl.hpp
  DakotaAlgorithm.cpp
  DakotaAlgorithmOptions.hpp
  DakotaAlgorithmOptions_Impl.hpp
  DakotaAlgorithmOptions.cpp
  DakotaParametersFile.hpp
  DakotaParametersFile_Impl.hpp
  DakotaParametersFile.cpp
  DataPoint.hpp
  DataPoint_Impl.hpp
  DataPoint.cpp
  DDACEAlgorithm.hpp
  DDACEAlgorithm_Impl.hpp
  DDACEAlgorithm.cpp
  DDACEAlgorithmOptions.hpp
  DDACEAlgorithmOptions_Impl.hpp
  DDACEAlgorithmOptions.cpp
  DesignOfExperiments.hpp
  DesignOfExperiments_Impl.hpp
  DesignOfExperiments.cpp
  DesignOfExperimentsOptions.hpp
  DesignOfExperimentsOptions_Impl.hpp
  DesignOfExperimentsOptions.cpp
  DiscreteVariable.hpp
  DiscreteVariable_Impl.hpp
  DiscreteVariable.cpp
  ExponentialDistribution.hpp
  ExponentialDistribution.cpp
  FrechetDistribution.hpp
  FrechetDistribution.cpp
  FSUDaceAlgorithm.cpp
  FSUDaceAlgorithm.hpp
  FSUDaceAlgorithm_Impl.hpp
  FSUDaceAlgorithmOptions.cpp
  FSUDaceAlgorithmOptions.hpp
  FSUDaceAlgorithmOptions_Impl.hpp
  Function.hpp
  Function_Impl.hpp
  Function.cpp
  GammaDistribution.hpp
  GammaDistribution.cpp
  GenericUncertaintyDescription.hpp
  GenericUncertaintyDescription.cpp
  GeometricDistribution.hpp
  GeometricDistribution.cpp
  GumbelDistribution.hpp
  GumbelDistribution.cpp
  HistogramBinDistribution.hpp
  HistogramBinDistribution.cpp
  HistogramPointDistribution.hpp
  HistogramPointDistribution.cpp
  HypergeometricDistribution.hpp
  HypergeometricDistribution.cpp
  InputVariable.hpp
  InputVariable_Impl.hpp
  InputVariable.cpp
  LinearFunction.hpp
  LinearFunction_Impl.hpp
  LinearFunction.cpp
  LognormalDistribution.hpp
  LognormalDistribution.cpp
  LoguniformDistribution.hpp
  LoguniformDistribution.cpp
  Measure.hpp
  Measure_Impl.hpp
  Measure.cpp
  MeasureGroup.hpp
  MeasureGroup_Impl.hpp
  MeasureGroup.cpp
  NegativeBinomialDistribution.hpp${${target_name}_moc_src}
  NegativeBinomialDistribution.cpp
  NormalDistribution.hpp
  NormalDistribution.cpp
  NullMeasure.hpp
  NullMeasure_Impl.hpp
  NullMeasure.cpp
  PoissonDistribution.hpp
  PoissonDistribution.cpp
  RubyContinuousVariable.hpp
  RubyContinuousVariable_Impl.hpp
  RubyContinuousVariable.cpp
  RubyMeasure.hpp
  RubyMeasure_Impl.hpp
  RubyMeasure.cpp
  OpenStudioAlgorithm.hpp
  OpenStudioAlgorithm_Impl.hpp
  OpenStudioAlgorithm.cpp
  OptimizationDataPoint.hpp
  OptimizationDataPoint_Impl.hpp
  OptimizationDataPoint.cpp
  OptimizationProblem.hpp
  OptimizationProblem_Impl.hpp
  OptimizationProblem.cpp
  OutputAttributeVariable.hpp
  OutputAttributeVariable_Impl.hpp
  OutputAttributeVariable.cpp
  OutputVariable.hpp
  OutputVariable_Impl.hpp
  OutputVariable.cpp
  ParameterStudyAlgorithm.hpp
  ParameterStudyAlgorithm_Impl.hpp
  ParameterStudyAlgorithm.cpp
  ParameterStudyAlgorithmOptions.hpp
  ParameterStudyAlgorithmOptions_Impl.hpp
  ParameterStudyAlgorithmOptions.cpp
  Problem.hpp
  Problem_Impl.hpp
  Problem.cpp
  PSUADEDaceAlgorithm.cpp
  PSUADEDaceAlgorithm.hpp
  PSUADEDaceAlgorithm_Impl.hpp
  PSUADEDaceAlgorithmOptions.cpp
  PSUADEDaceAlgorithmOptions.hpp
  PSUADEDaceAlgorithmOptions_Impl.hpp
  SamplingAlgorithm.cpp
  SamplingAlgorithm.hpp
  SamplingAlgorithm_Impl.hpp
  SamplingAlgorithmOptions.cpp
  SamplingAlgorithmOptions.hpp
  SamplingAlgorithmOptions_Impl.hpp
  SequentialSearch.hpp
  SequentialSearch_Impl.hpp
  SequentialSearch.cpp
  SequentialSearchOptions.hpp
  SequentialSearchOptions_Impl.hpp
  SequentialSearchOptions.cpp
  TriangularDistribution.hpp
  TriangularDistribution.cpp
  UncertaintyDescription.hpp
  UncertaintyDescription_Impl.hpp
  UncertaintyDescription.cpp
  UniformDistribution.hpp
  UniformDistribution.cpp
  Variable.hpp
  Variable_Impl.hpp
  Variable.cpp
  WeibullDistribution.hpp
  WeibullDistribution.cpp
  WorkflowStep.hpp
  WorkflowStep_Impl.hpp
  WorkflowStep.cpp
)

set(${target_name}_moc
  Analysis_Impl.hpp
  AnalysisObject_Impl.hpp
)

set(${target_name}_test_src
  test/AnalysisFixture.hpp
  test/AnalysisFixture.cpp
  test/Algorithm_GTest.cpp
  test/Analysis_GTest.cpp
  test/Problem_GTest.cpp
  test/ContinuousVariable_GTest.cpp
  test/DiscreteVariable_GTest.cpp
  test/Distributions_GTest.cpp
  test/DataPoint_GTest.cpp
  test/DesignOfExperiments_GTest.cpp
  test/SequentialSearch_GTest.cpp
  test/DDACEAlgorithm_GTest.cpp
  test/FSUDaceAlgorithm_GTest.cpp
  test/ParameterStudyAlgorithm_GTest.cpp
  test/PSUADEDaceAlgorithm_GTest.cpp
  test/RubyMeasure_GTest.cpp
  test/SamplingAlgorithm_GTest.cpp
  test/UncertaintyDescription_GTest.cpp
)

set(${target_name}_swig_src
  Analysis.i
)

set(${target_name}_depends
  openstudio_energyplus
  openstudio_ruleset
  openstudio_runmanager
)

qt5_wrap_cpp_minimally(${target_name}_moc_src ${${target_name}_moc})

add_library(${target_name} SHARED
  ${${target_name}_src}
  ${${target_name}_moc_src}
)
target_link_libraries(${target_name} ${${target_name}_depends})


if(NOT APPLE)
  install(TARGETS ${target_name}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
  )
endif()

CREATE_SRC_GROUPS("${${target_name}_src}")
CREATE_SRC_GROUPS("${${target_name}_test_src}")
CREATE_SRC_GROUPS("${${target_name}_swig_src}")

CREATE_TEST_TARGETS(${target_name} "${${target_name}_test_src}" "${${target_name}_depends}")
MAKE_SWIG_TARGET(OpenStudioAnalysis Analysis "${CMAKE_CURRENT_SOURCE_DIR}/Analysis.i" "${${target_name}_swig_src}" "${target_name}" "OpenStudioEnergyPlus;OpenStudioRuleset;OpenStudioRunManager")

AddPCH(${target_name})

#if(BUILD_TESTING)
#  add_dependencies("${target_name}_tests" "")
#endif()
