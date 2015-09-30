namespace openstudio {
namespace analysis {

/** \mainpage OpenStudio Analysis
 *
 *  \section quick_links_analysis Quick Links
 *
 *  Base classes in alphabetical order:
 *
 *  \li Analysis
 *  \li Algorithm
 *  \li AlgorithmOptions
 *  \li DataPoint
 *  \li Function
 *  \li Problem
 *  \li UncertaintyDescription
 *  \li Variable
 *
 *  All of the classes listed above, and the classes derived from them, derive from AnalysisObject, 
 *  which follows the pImpl idiom, and specifies basic meta-data (UUID, version UUID, name, display 
 *  name, and description). Because of the pImpl idiom, shallow copies of objects (made using 
 *  assignment or copy construction) share data with each other. To get a deep copy, use 
 *  AnalysisObject::clone.
 *
 *  Available analysis types:
 *
 *  \li \ref analysis_custom_points - Users can explicitly add their own \link DataPoint
 *                                    DataPoints\endlink to an Analysis (including iteratively with
 *                                    a driver Ruby script)
 *  \li \ref ms - Algorithms for parameter studies and multivariate sampling
 *  \li \ref ms_sa - Algorithms for multivariate sampling with sensitivity analysis capabilities
 *  \li \ref sa_uq - Algorithms for multivariate sampling that account for uncertainty 
 *                   descriptions and have sensitivity analysis and uncertainty quantification 
 *                   capabilities
 *  \li \ref optimization - Algorithms for design optimization  
 *  
 *  \section overview_analysis Overview
 *
 *  The OpenStudio %Analysis sub-project supports the formulation and solution of energy-efficient 
 *  building design problems. %Analysis works in concert with the entire analysis framework, see 
 *  Figure 1.
 *
 *  \image html AnalysisFrameworkDependencyStack_Analysis.png "Figure 1. Analysis Framework Dependency Stack"
 *  
 *  In particular, %Analysis allows for the in-memory formulation of problems; problems are 
 *  serialized to disk using project::ProjectDatabase and related classes; and problems are 
 *  solved by instantiating \link Analysis Analyses\endlink with a seed model and running them 
 *  using an analysisdriver::AnalysisDriver. AnalysisDrivers coordinate the operations of 
 *  %Analysis, %Project, and %RunManager, with the optional application of an Algorithm. 
 *  Running an Analysis with an AnalysisDriver produces \link DataPoint DataPoints\endlink, 
 *  each of which provides an interface to the results of a single runmanager::Workflow 
 *  (instantiated as a runmanager::Job with children). 
 *
 *  \section problem_formulation Problem Formulation
 *
 *  To conduct an OpenStudio analysis, one must first formulate a Problem (or an 
 *  OptimizationProblem, see \ref optimization). A Problem consists of a name, an ordered vector of 
 *  \link WorkflowStep WorkflowSteps\endlink, and an optional vector of response \link Function 
 *  Functions\endlink. The \link WorkflowStep WorkflowSteps\endlink, each of which represents an
 *  InputVariable or a runmanager::WorkItem, are maintained in a specific order to give users 
 *  precise control over how a perturbed building model is created. For instance, if one variable 
 *  adds Lights objects, and another variable changes the lighting power of all Lights objects, the 
 *  order in which these variables are applied matters and should be explicitly set by the user. 
 *  Analyses can be conducted on OSM, IDF, or a mixture of both; and in a debugging phase, one may 
 *  not yet be interested in conducting simulations; thus we give the user precise control over all
 *  of the \link WorkflowStep WorkflowSteps\endlink.
 * 
 *  The Problem class was designed to be nominally shareable between projects, however, the 
 *  practicality of sharing a given formulation will depend on the specifics of how the \link
 *  WorkflowStep WorkflowSteps\endlink are defined.
 *
 *    \subsection discrete_variables Discrete Variables
 *
 *    A DiscreteVariable has a name, and consists of an ordered vector of \link DiscretePerturbation
 *    DiscretePerturbations\endlink. Any InputVariable must implement InputVariable::createWorkItem, 
 *    returning a runmanager::WorkItem that will set the variable to the appropriate value. For 
 *    \link DiscreteVariable DiscreteVariables\endlink, this is equivalent to returning a WorkItem 
 *    that will apply the DiscretePerturbation at the vector index passed to the createWorkItem 
 *    method (as a QVariant of type Int). Therefore, the bulk of the createWorkItem work is performed 
 *    by the individual DiscretePerturbation classes. 
 *
 *    Each type of DiscretePertubation returns a different type of WorkItem. In particular, a 
 *    NullPerturbation returns a null WorkItem that does nothing to its input model. A 
 *    ModelRulesetPerturbation returns a WorkItem that will (when it is converted to a Job and run 
 *    by RunManager) apply its ruleset::ModelRuleset to an OpenStudio Model, creating an output 
 *    (perturbed) OpenStudio Model. A RubyPerturbation returns a WorkItem that will run a Ruby 
 *    script on an input file, producing an output file. RubyPerturbations may run on OpenStudio 
 *    models (OSM) or EnergyPlus models (IDF). What type of model is loaded as input, and what type 
 *    is serialized as output, must be specified during the construction of a RubyPerturbation, 
 *    either implicitly as part of a BCLMeasure, or explicitly using the FileReferenceType 
 *    enumeration in utilities/core/FileReference.hpp.
 *  
 *    \subsection continuous_variables Continuous Variables
 *
 *    ModelRulesetContinuousVariable works similarly to a DiscreteVariable with \link 
 *    ModelRulesetPerturbation ModelRulesetPerturbations\endlink, except that it operates on a 
 *    single (real-valued) attribute, and the value of that attribute is not specified up front. 
 *    \link RubyContinuousVariable RubyContinuousVariables\endlink can be attached to individual
 *    ruleset::OSArguments in a Ruby script. Multiple such variables all referencing the same 
 *    RubyPerturbation can be chained together as adjacent \link WorkflowStep WorkflowSteps\endlink.
 *
 *    \subsection runmanager_workitems RunManager WorkItems
 *
 *    Individual runmanager::WorkItems can be placed anywhere in the Problem::workflow() vector. 
 *    Almost always, some WorkItems for running EnergyPlus are placed toward the end of the 
 *    workflow. For instance, if all of the \link InputVariable InputVariables\endlink operate on 
 *    OpenStudio Models, then the end of the workflow might be specified as:
 *
 *    \code
 *    // problem and its input variables defined above. then,
 *    problem.push(runmanager::WorkItem(runmanager::JobType::ModelToIdf));
 *    problem.push(runmanager::WorkItem(runmanager::JobType::ExpandObjects));
 *    problem.push(runmanager::WorkItem(runmanager::JobType::EnergyPlusPreProcess));
 *    problem.push(runmanager::WorkItem(runmanager::JobType::EnergyPlus));
 *    problem.push(runmanager::WorkItem(runmanager::JobType::OpenStudioPostProcess));
 *    // this code uses implicit construction of WorkflowSteps from runmanager::WorkItems
 *    \endcode
 *
 *    You may also wish to specify a BCLMeasure with measureFunction() == MeasureFunction::Report 
 *    as your final post-process step;
 *    
 *    \code
 *    BCLMeasure reportMeasure = BCLMeasure::load(toPath("path/to/measure/folder")).get();
 *    runmanager::RubyJobBuilder rjb(reportMeasure);
 *    problem.push(rbj.toWorkItem());
 *    \endcode
 *
 *    If any of your \link InputVariable InputVariables\endlink operate directly on IDF, then you 
 *    might want to create the InputVariable and then insert it before the EnergyPlusPreProcess 
 *    job. Problem enables this with the method Problem::getWorkflowStepIndexByJobType, which 
 *    inspects the runmanager::WorkItems in the Problem::workflow vector: 
 *
 *    \code
 *    // problem and idfInputVariable, which is an object of type InputVariable with 
 *    // .inputFileType() == FileReferenceType::IDF, defined above. 
 *    // Also assume that the standard simulation workflow from above is already in problem.
 *    int index = problem.getWorkflowStepIndexByJobType(runmanager::JobType::EnergyPlusPreProcess).get();
 *    problem.insert(index,idfInputVariable);
 *    // note that this code used implicit construction of WorkflowStep from InputVariable
 *    \endcode
 *
 *    \subsection response_functions Response Functions
 *
 *    In addition to specifying how parametrically related models are to be generated, a Problem 
 *    formulation may also specify the particular metrics that should be used to evaluate the 
 *    resulting models. This is done with \link Problem::responses responses\endlink, which at this 
 *    time must be \link LinearFunction LinearFunctions\endlink. Most of the time, at least one 
 *    of the variables in a given response function will be of type OutputAttributeVariable.
 *
 *    \link OutputAttributeVariable OutputAttributeVariables\endlink call out an attributeName to 
 *    be looked up after a DataPoint has been successfully simulated. The attributeName 
 *    should correspond to an Attribute written out to a report.xml file produced by a post-
 *    processing job, see, for instance, runmanager::JobType::EnergyPlusPostProcess, and 
 *    openstudio\runmanager\rubyscripts\PostProcess.rb (and similar scripts in that folder). The 
 *    post-process job that produces the attribute called out by OutputAttributeVariable must be 
 *    part of the Problem's workflow.
 *
 *  \section analysis_custom_points Analysis with Custom Points
 *
 *  Once you have a problem formulation, you can construct an Analysis object by specifying a name, 
 *  the Problem to be solved, and a seed model (OSM or IDF) in the form of a FileReference 
 *  (utilities/core/FileReference.hpp). Whether or not you specify an Algorithm, you can 
 *  always construct and add your own \link DataPoint DataPoints\endlink to be run by an 
 *  analysisdriver::AnalysisDriver.
 *
 *  To create a new DataPoint, a value for each of the Problem's variables must be specified using
 *  a std::vector<QVariant>. If the variable is discrete, the QVariant must be convertible to int, 
 *  and as that integer will be interpreted as an index into DiscreteVariable::perturbations(false), 
 *  it must be >= 0 and < DiscreteVariable::numPerturbations(false). If the variable is continuous,
 *  the QVariant must be convertible to double and should make sense in the context of that 
 *  variable. In any case, the size of the std::vector<QVariant> must be equal to 
 *  %Analysis::problem().numVariables().
 *
 *  Once the variable values are specified, a new DataPoint is constructed by calling 
 *  Problem::createDataPoint, and is added to the Analysis with Analysis::addDataPoint. Assuming the
 *  variable values were valid, and that a DataPoint with the same variable values is not already 
 *  in the Analysis, the new DataPoint will now be in the list Analysis::dataPointsToQueue, and it 
 *  will not have any results.
 *
 *  \section analysis_algorithms Analysis with Algorithms
 *
 *  Algorithms are automated methods for generating \link DataPoint DataPoints\endlink. OpenStudio
 *  contains two sorts of algorithms: \link OpenStudioAlgorithm OpenStudioAlgorithms\endlink, 
 *  which are directly specified in the OpenStudio code base through the definition of 
 *  OpenStudioAlgorithm::createNextIteration; and \link DakotaAlgorithm DakotaAlgoritms\endlink, 
 *  which rely on DAKOTA (http://dakota.sandia.gov/software.html) to specify the variable values
 *  to run. Either type of algorithm may be specified during the construction of an Analysis. 
 *  If present, the AnalysisDriver will call createNextIteration or kick off a runmanager::DakotaJob
 *  as appropriate.
 *
 *  \section ms Multivariate Sampling
 *
 *  \subsection design_of_experiments OpenStudio Algorithm: Design of Experiments
 *
 *  If all of a Problem's variables are discrete, then a DesignOfExperiments Algorithm can be 
 *  applied. If this is done, and the analysis is run with an analysisdriver::AnalysisDriver, 
 *  DesignOfExperiments::createNextIteration will be called by the AnalysisDriver, and the 
 *  resulting new \link DataPoint DataPoints\endlink will be run, with the results serialized to a
 *  project::ProjectDatabase. At this time only full factorial analyses are supported, and all of 
 *  the runs are queued in the first and only iteration of the algorithm.
 *
 *  \subsection parameterstudy DAKOTA Algorithm: ParameterStudyAlgorithm
 *
 *  If DAKOTA is installed on your system, then a ParameterStudyAlgorithm can be specified to do
 *  basic parameter scans. Types of scans that add to (rather than duplicate) what is available
 *  in OpenStudio without DAKOTA include ParameterStudyAlgorithmType::vector_parameter_study and 
 *  ParameterStudyAlgorithmType::centered_parameter_study.
 *
 *  \section ms_sa Multivariate Sampling and Sensitivity Analysis
 *   
 *  \subsection ddace DAKOTA Algorithm: DDACEAlgorithm
 * 
 *  A DAKOTA DDACEAlgorithm can be specified to sample a parameter space stochastically (rather 
 *  than deterministically like DesignOfExperiments and ParameterStudyAlgorithm). To get started, 
 *  specify the sampling type in a DDACEAlgorithmOptions object using the enum DDACEAlgorithmType. 
 *  By default, the DDACEAlgorithmOptions::seed will be explicitly set to support analysis restart
 *  (by AnalysisDriver in coordination with DAKOTA) in case of a runtime failure or disruption.
 *  See DDACEAlgorithmOptions and the DAKOTA documentation for detailed information.
 *
 *  \subsection fsudace DAKOTA Algorithm: FSUDaceAlgorithm
 *
 *  FSUDaceAlgorithm is similar to DDACEAlgorithm, but provides different sampling methods, see 
 *  FSUDaceAlgorithmType, FSUDaceAlgorithmOptions, and the DAKOTA documentation. By default, the
 *  FSUDaceAlgorithmOptions::seed will be explicitly set to support analysis restart (by
 *  AnalysisDriver in coordination with DAKOTA) in case of a runtime failure or disruption.
 *
 *  \subsection psuadedace DAKOTA Algorithm: PSUADEDaceAlgorithm
 *
 *  PSUADEDaceAlgorithm exposes the PSUADE Morris One-At-A-Time (MOAT) sensitivity analysis method
 *  available through DAKOTA, see PSUADEDaceAlgorithmOptions, and the DAKOTA documentation. By 
 *  default, the PSUADEDaceAlgorithmOptions::seed will be explicitly set to support analysis restart 
 *  (by AnalysisDriver in coordination with DAKOTA) in case of a runtime failure or disruption. See 
 *  the DAKOTA documentation for detailed information.
 *
 *  \section sa_uq Sensitivity Analysis and Uncertainty Quantification
 *
 *  \subsection sampling DAKOTA Algorithm: SamplingAlgorithm
 *
 *  The DAKOTA SamplingAlgorithm is like DDACE in that it provides random (Monte Carlo) sampling and
 *  latin hypercube sampling (LHS). In addition, it samples across a number of
 *  \link UncertaintyDescriptionType uncertainty description types\endlink, for both continuous
 *  and discrete variables. (DDACE only works with continuous variables. To make it more broadly
 *  useful, we sometime make our discrete variables look continuous to DAKOTA when using DDACE.) Such 
 *  sampling enables sensitivity analysis and uncertainty quantification. 
 *
 *  To use this algorithm effectively, create one or more \link UncertaintyDescription 
 *  UncertaintyDescriptions\endlink using GenericUncertaintyDescription or one of the 
 *  UncertaintyDescriptionType-specific interfaces (e.g. NormalDistribution, LognormalDistribution,
 *  PoissonDistribution, HistogramPointDistribution). 
 *
 *  As with the Dace algorithms, SamplingAlgorithm::seed is explicitly set by default, to support
 *  analysis restart (by AnalysisDriver in coordination with DAKOTA) in case of a runtime failure or
 *  disruption.
 *
 *  \section optimization Optimization
 *
 *  OpenStudio currently supports unconstrained optimization problems with two linear objective 
 *  functions, and discrete variables. 
 *
 *    \subsection optimization_problem_formulation Optimization Problem Formulation
 *
 *    An OptimizationProblem is a Problem that also has one or more objective functions. At this 
 *    time, a LinearFunction class is provided for the purpose of constructing objective functions. 
 *    Often, an objective will be a single high-level output variable as in:
 *
 *    \code
 *    OutputAttributeContinuousVariable var("Cooling Electricity Use","EndUses.Electricity.Cooling.General");
 *    LinearFunction objective1(var.name(),VariableVector(1u,var.cast<Variable>()));
 *    \endcode
 *
 *    Once a vector of \link Function Functions\endlink is created, along with all the information 
 *    required for a basic Problem, an OptimizationProblem may be constructed and used in the same way.
 *    At this time, all objective functions are to be minimized, so if you want maximization, please
 *    multiply your objective function by -1 (by specifying appropriate LinearFunction coefficients).
 *
 *    \subsection sequential_search OpenStudio Algorithm: Sequential Search
 * 
 *    \link OptimizationProblem OptimizationProblems\endlink with two objective functions and 
 *    Problem::allVariablesAreDiscrete()), can be solved using the SequentialSearch algorithm. 
 *
 *    SequentialSearch is a greedy heuristic algorithm that works by tracing out a "minimum curve"
 *    for one of the two objectives. Which objective is "minimized first" must be specified up front 
 *    using the class SequentialSearchOptions. As the objective functions are indexed starting at 
 *    zero, usually the index 1 would be specified, as this naturally corresponds to the y-axis on a 
 *    plot of the objective function values. The minimum curve starts at the baseline point (first 
 *    perturbation specified for each variable), and then moves always in the direction of improving
 *    the other objective function, that is, the objective function that is not minimized first. In 
 *    previous work, an economic objective has been minimized first, while always improving an energy
 *    use objective, which works well here at NREL since we are not generally interested in cheaper 
 *    buildings that use more energy. This methodology does not work as well when both objectives 
 *    are equally important, such as when cooling energy and heating energy are traded off. However,
 *    this deficiency can be overcome by solving first with one, and then again with the other 
 *    objective function being minimized first.
 *
 */

} // analysis
} // openstudio

