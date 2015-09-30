#ifndef ANALYSIS_I
#define ANALYSIS_I

#ifdef SWIGPYTHON
%module openstudioanalysis
#endif

#define UTILITIES_API
#define MODEL_API
#define RULESET_API
#define ANALYSIS_API

%include <utilities/core/CommonInclude.i>
%import <utilities/core/CommonImport.i>
%import <utilities/Utilities.i>
%import <model/Model.i>
%import <ruleset/Ruleset.i>
%import <runmanager/lib/RunManager.i>

%ignore openstudio::detail;
%ignore openstudio::model::detail;
%ignore openstudio::runmanager::detail;
%ignore openstudio::ruleset::detail;
%ignore openstudio::analysis::detail;

%{
  #include <analysis/ConcreteAnalysisObjects.hpp>
  #include <analysis/DakotaParametersFile.hpp>
  #include <analysis/DakotaParametersFile_Impl.hpp>  
  
  #include <runmanager/lib/Workflow.hpp>
  #include <runmanager/lib/WorkItem.hpp>
  #include <runmanager/lib/SimulationEngine.hpp>
  #include <runmanager/lib/ErrorEstimation.hpp>
  #include <runmanager/lib/RunManagerWatcher.hpp>

  #include <ruleset/OSArgument.hpp>
  
  #include <utilities/core/PathWatcher.hpp>
  #include <utilities/core/UpdateManager.hpp>
  #include <utilities/bcl/LocalBCL.hpp>
  #include <utilities/bcl/RemoteBCL.hpp>
  #include <utilities/idf/WorkspaceWatcher.hpp>
  #include <utilities/idf/WorkspaceObjectWatcher.hpp>
  
  using namespace openstudio;
  using namespace openstudio::model;
  using namespace openstudio::ruleset;
  using namespace openstudio::analysis;
  
  // to be ignored
  class QAction;
  class QComboBox;
  class QDialog;
  class QDomNode;
  class QDomElement;
  class QDomDocument;
  class QMainWindow;
  class QNetworkAccessManager;
  namespace openstudio{
    namespace runmanager{
      class JobStatusWidget;
      namespace detail {
        class JobItem;
      }
    }
  }
  
%}

// it seems that SWIG tries to create conversions of QObjects to these
%ignore QAction;
%ignore QComboBox;
%ignore QDialog;
%ignore QDomNode;
%ignore QDomElement;
%ignore QDomDocument;
%ignore QMainWindow;
%ignore QNetworkAccessManager;
%ignore openstudio::runmanager::JobStatusWidget;
%ignore openstudio::runmanager::detail::JobItem;

%define SWIG_ANALYSISOBJECT(_name)
  %ignore std::vector<openstudio::analysis::_name>::vector(size_type);
  %ignore std::vector<openstudio::analysis::_name>::resize(size_type);
  %template(_name##Vector) std::vector<openstudio::analysis::_name>;
  %template(Optional##_name) boost::optional<openstudio::analysis::_name>;
  
  %include <analysis/##_name##.hpp>
  
  %extend openstudio::analysis::AnalysisObject {
    %template(to_##_name) optionalCast<openstudio::analysis::##_name>;
  }
%enddef

%define SWIG_ALGORITHMOPTIONS(_name)
  %ignore std::vector<openstudio::analysis::_name>::vector(size_type);
  %ignore std::vector<openstudio::analysis::_name>::resize(size_type);
  %template(_name##Vector) std::vector<openstudio::analysis::_name>;
  %template(Optional##_name) boost::optional<openstudio::analysis::_name>;
  
  %include <analysis/##_name##.hpp>
  
  %extend openstudio::analysis::AlgorithmOptions {
    %template(to_##_name) optionalCast<openstudio::analysis::##_name>;
  }  
%enddef

%template(OptionalFSUDaceCvtTrialType) boost::optional<openstudio::analysis::FSUDaceCvtTrialType>;
%template(OptionalSamplingAlgorithmRNGType) boost::optional<openstudio::analysis::SamplingAlgorithmRNGType>;
%template(OptionalSamplingAlgorithmSampleType) boost::optional<openstudio::analysis::SamplingAlgorithmSampleType>;
%template(UncertaintyDescriptionTypeVector) std::vector<openstudio::analysis::UncertaintyDescriptionType>;
%template(UncertaintyDescriptionTypeIntVectorMap) std::map<openstudio::analysis::UncertaintyDescriptionType, std::vector<int> >;
%include <analysis/AnalysisEnums.hpp>


// base class
SWIG_ANALYSISOBJECT(AnalysisObject);

// measures
SWIG_ANALYSISOBJECT(Measure);

#if defined(SWIGCSHARP) || defined(SWIGJAVA)
%template(OptionalMeasureVector) std::vector<boost::optional<openstudio::analysis::Measure> >;
#endif

#if defined(SWIGJAVA)
%ignore openstudio::analysis::UncertaintyDescription::type;
#endif


SWIG_ANALYSISOBJECT(NullMeasure);
SWIG_ANALYSISOBJECT(RubyMeasure);

// variables
SWIG_ANALYSISOBJECT(Variable);
SWIG_ANALYSISOBJECT(InputVariable);
SWIG_ANALYSISOBJECT(DiscreteVariable);
SWIG_ANALYSISOBJECT(MeasureGroup);
SWIG_ANALYSISOBJECT(ContinuousVariable);
SWIG_ANALYSISOBJECT(RubyContinuousVariable);

%rename(AnalysisOutputVariable) openstudio::analysis::OutputVariable;

%ignore std::vector<openstudio::analysis::OutputVariable>::vector(size_type);
%ignore std::vector<openstudio::analysis::OutputVariable>::resize(size_type);
%template(AnalysisOutputVariableVector) std::vector<openstudio::analysis::OutputVariable>;
%template(OptionalAnalysisOutputVariable) boost::optional<openstudio::analysis::OutputVariable>;

%include <analysis/OutputVariable.hpp>

%extend openstudio::analysis::AnalysisObject {
  %template(to_AnalysisOutputVariable) optionalCast<openstudio::analysis::OutputVariable>;
}

SWIG_ANALYSISOBJECT(OutputAttributeVariable);
SWIG_ANALYSISOBJECT(UncertaintyDescription);

// functions
SWIG_ANALYSISOBJECT(Function);
SWIG_ANALYSISOBJECT(LinearFunction);

// small struct in Problem.hpp
%ignore std::vector<openstudio::analysis::WorkflowStepJob>::vector(size_type);
%ignore std::vector<openstudio::analysis::WorkflowStepJob>::resize(size_type);
%template(WorkflowStepJobVector) std::vector<openstudio::analysis::WorkflowStepJob>;
%template(OptionalWorkflowStepJob) boost::optional<openstudio::analysis::WorkflowStepJob>;

// problems
SWIG_ANALYSISOBJECT(WorkflowStep);
SWIG_ANALYSISOBJECT(Problem);
SWIG_ANALYSISOBJECT(OptimizationProblem);

// algorithm options
SWIG_ALGORITHMOPTIONS(AlgorithmOptions);
SWIG_ALGORITHMOPTIONS(DesignOfExperimentsOptions);
SWIG_ALGORITHMOPTIONS(SequentialSearchOptions);
SWIG_ALGORITHMOPTIONS(DakotaAlgorithmOptions);
SWIG_ALGORITHMOPTIONS(DDACEAlgorithmOptions);
SWIG_ALGORITHMOPTIONS(FSUDaceAlgorithmOptions);
SWIG_ALGORITHMOPTIONS(ParameterStudyAlgorithmOptions);
SWIG_ALGORITHMOPTIONS(PSUADEDaceAlgorithmOptions);
SWIG_ALGORITHMOPTIONS(SamplingAlgorithmOptions);

// algorithms
SWIG_ANALYSISOBJECT(Algorithm);
SWIG_ANALYSISOBJECT(OpenStudioAlgorithm);
SWIG_ANALYSISOBJECT(DesignOfExperiments);
SWIG_ANALYSISOBJECT(SequentialSearch);
SWIG_ANALYSISOBJECT(DakotaAlgorithm);
SWIG_ANALYSISOBJECT(DDACEAlgorithm);
SWIG_ANALYSISOBJECT(FSUDaceAlgorithm);
SWIG_ANALYSISOBJECT(ParameterStudyAlgorithm);
SWIG_ANALYSISOBJECT(PSUADEDaceAlgorithm);
SWIG_ANALYSISOBJECT(SamplingAlgorithm);

// data points
SWIG_ANALYSISOBJECT(DataPoint);
SWIG_ANALYSISOBJECT(OptimizationDataPoint);

// analysis
SWIG_ANALYSISOBJECT(Analysis);

// probability distributions for uncertainty descrtiptions
SWIG_ANALYSISOBJECT(NormalDistribution);
SWIG_ANALYSISOBJECT(LognormalDistribution);
SWIG_ANALYSISOBJECT(UniformDistribution);
SWIG_ANALYSISOBJECT(LoguniformDistribution);
SWIG_ANALYSISOBJECT(TriangularDistribution);
SWIG_ANALYSISOBJECT(ExponentialDistribution);
SWIG_ANALYSISOBJECT(BetaDistribution);
SWIG_ANALYSISOBJECT(GammaDistribution);
SWIG_ANALYSISOBJECT(GumbelDistribution);
SWIG_ANALYSISOBJECT(FrechetDistribution);
SWIG_ANALYSISOBJECT(WeibullDistribution);
SWIG_ANALYSISOBJECT(HistogramBinDistribution);
SWIG_ANALYSISOBJECT(PoissonDistribution);
SWIG_ANALYSISOBJECT(BinomialDistribution);
SWIG_ANALYSISOBJECT(NegativeBinomialDistribution);
SWIG_ANALYSISOBJECT(GeometricDistribution);
SWIG_ANALYSISOBJECT(HypergeometricDistribution);
SWIG_ANALYSISOBJECT(HistogramPointDistribution);

// DakotaParametersFile -- Swigged to help with debugging.
%ignore std::vector<openstudio::analysis::DakotaParametersFile>::vector(size_type);
%ignore std::vector<openstudio::analysis::DakotaParametersFile>::resize(size_type);
%template(DakotaParametersFileVector) std::vector<openstudio::analysis::DakotaParametersFile>;
%template(OptionalDakotaParametersFile) boost::optional<openstudio::analysis::DakotaParametersFile>;
%include <analysis/DakotaParametersFile.hpp>

#endif //ANALYSIS_I

