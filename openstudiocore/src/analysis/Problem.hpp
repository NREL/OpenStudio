/**********************************************************************
 *  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
 *  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **********************************************************************/

#ifndef ANALYSIS_PROBLEM_HPP
#define ANALYSIS_PROBLEM_HPP

#include "AnalysisAPI.hpp"
#include "AnalysisObject.hpp"

#include "Measure.hpp"
#include "WorkflowStep.hpp"

#include "../runmanager/lib/Job.hpp"

#include "../utilities/core/Path.hpp"

#include <QVariant>

namespace openstudio {

class BCLMeasure;
class FileReferenceType;

namespace ruleset {
  class OSArgument;
}

namespace runmanager {
  class Job;
  class JobType;
  class Workflow;
  class WorkItem;
}

namespace analysis {

class Analysis;
class ContinuousVariable;
class DakotaAlgorithm;
class DakotaParametersFile;
class DataPoint;
class DiscreteVariable;
class Function;
class InputVariable;
class UncertaintyDescription;
class UncertaintyDescriptionType;
class Variable;

namespace detail {

  class Analysis_Impl;
  class Problem_Impl;
  class Variable_Impl;
  class DiscreteVariable_Impl;
  class RubyContinuousVariable_Impl;

} // detail

/** WorkflowStepJob is a structure for articulating the detailed runtime results associated with
 *  each WorkflowStep as executed for a particular DataPoint. \link WorkflowStepJob 
 *  WorkflowStepJobs\endlink are constructed by Problem, through the interpretation of 
 *  DataPoint::topLevelJob in the context of the Problem::workflow. */
struct ANALYSIS_API WorkflowStepJob {
  boost::optional<runmanager::Job> job; // initialized unless step evaluates to a null job or
                                        // the preliminary part of a compound measure
  WorkflowStep step;
  boost::optional<Measure> measure;
  boost::optional<QVariant> value; // for variables with explicitly set values (all continuous
                                   // variables and discrete variables where an integer value
                                   // is set, rather than mapped to a measure)
  boost::optional<unsigned> mergedJobIndex;

  /** Returns errors from job if not job->hasMergedJobs(). Returns errors from MergeJobResults
   *  otherwise. */
  boost::optional<runmanager::JobErrors> errors() const;

  /** Returns ouptputFiles from job if not job->hasMergedJobs(). Returns outputFiles from 
   *  MergeJobResults otherwise. */
  boost::optional<runmanager::Files> outputFiles() const;

  /** Constructor for bare runmanager::WorkItem. */
  WorkflowStepJob(const runmanager::Job& t_job,
                  const WorkflowStep& t_step,
                  boost::optional<unsigned> t_mergedJobIndex=boost::none);

  /** Constructor for bare, null runmanager::WorkItem. */
  WorkflowStepJob(const WorkflowStep& t_step);

  /** Constructor for non-null measure. */
  WorkflowStepJob(const runmanager::Job& t_job,
                  const WorkflowStep& t_step,
                  const Measure& t_measure,
                  boost::optional<unsigned> t_mergedJobIndex=boost::none);

  /** Constructor for null measure. */
  WorkflowStepJob(const WorkflowStep& t_step,
                  const Measure& t_measure);

  /** Constructor for explicitly set variable. */
  WorkflowStepJob(const runmanager::Job& t_job,
                  const WorkflowStep &t_step,
                  const QVariant& t_value,
                  boost::optional<unsigned> t_mergedJobIndex=boost::none);

  /** Constructor for explicitly set variable in preliminary part of compound measure. */
  WorkflowStepJob(const WorkflowStep &t_step,
                  const QVariant& t_value);

 private:
  WorkflowStepJob();
};

/** \relates WorkflowStepJob */
typedef boost::optional<WorkflowStepJob> OptionalWorkflowStepJob;

/** \relates WorkflowStepJob */
typedef std::vector<WorkflowStepJob> WorkflowStepJobVector;

/** Problem is an AnalysisObject that contains a (building energy) problem formulation stated as
 *  a vector of \link WorkflowStep workflow steps\endlink (\link InputVariable input 
 *  variables\endlink or runmanager::WorkItems), and a vector of response \link Function 
 *  functions\endlink. Each WorkflowStep operates on an IDF or OSM model, either returning a 
 *  modified model or producing other outputs (reports and pieces of data generated for their 
 *  own sake, or for the calculation of response function values). The problem formulation is not
 *  meant to be free standing, but is meant to be solved in the context of an Analysis, using an
 *  analysisdriver::AnalysisDriver. For this reason, Problem is designed to \link createWorkflow
 *  create runmanager::Workflows\endlink that set up and simulate \link DataPoint 
 *  DataPoints\endlink, and then to \link updateDataPoint digest the final results\endlink. */
class ANALYSIS_API Problem : public AnalysisObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  Problem(const std::string& name,
          const std::vector<WorkflowStep>& workflow = std::vector<WorkflowStep>());

  Problem(const std::string& name,
          const std::vector<WorkflowStep>& workflow,
          const std::vector<Function>& responses);

  /** Construct from variables (order will be preserved), response functions, and simulation
   *  workflow. If any of the following requirements are violated, an openstudio::Exception is
   *  thrown:
   *
   *  \li The simulationWorkflow must be convertible to std::vector<runmanager::WorkItem>.
   *  \li The input and output file types must be consistent throughout the resulting chain of
   *      WorkflowSteps.
   *
   *  \deprecated */
  Problem(const std::string& name,
          const std::vector<Variable>& variables,
          const std::vector<Function>& responses,
          const runmanager::Workflow& simulationWorkflow);

  /** \overload \deprecated */
  Problem(const std::string& name,
          const std::vector<Variable>& variables,
          const runmanager::Workflow& simulationWorkflow);

  /** Constructor provided for deserialization; not for general use. */
  Problem(const UUID& uuid,
          const UUID& versionUUID,
          const std::string& name,
          const std::string& displayName,
          const std::string& description,
          const std::vector<WorkflowStep>& workflow,
          const std::vector<Function>& responses);

  virtual ~Problem() {}

  //@}
  /** @name Getters and Queries */
  //@{

  /** Returns the vector of \link WorkflowStep WorkflowSteps\endlink that defines how this Problem
   *  will generate parametrically related \link DataPoint DataPoints\endlink. */
  std::vector<WorkflowStep> workflow() const;

  /** If there is a runmanager::WorkItem of jobType in workflow(), returns the index of the first
   *  such runmanager::WorkItem. Otherwise evaluates to false. */
  boost::optional<int> getWorkflowStepIndexByJobType(const runmanager::JobType& jobType) const;

  int numWorkflowSteps() const;

  /** Returns this problem's (ordered) list of \link InputVariable input variables\endlink, that
   *  is, individual model parameters. After this Problem is attached to a seed model using the
   *  Analysis class, setting each of the variables to a specific value fully defines a building
   *  energy model that is parametrically related to all other models constructed in the same way,
   *  making it possible to search, explore and visualize a particular design space. */
  std::vector<InputVariable> variables() const;

  /** Returns the variable at index, where index refers to the vector returned by variables(). */
  InputVariable getVariable(int index) const;

  /** Returns the index of the InputVariable with uuid, where index refers to the vector returned
   *  by variables(). */
  boost::optional<int> getVariableIndexByUUID(const UUID& uuid) const;

  boost::optional<InputVariable> getVariableByUUID(const UUID& uuid) const;

  /** Returns the number of \link InputVariable variables\endlink in this problem. */
  int numVariables() const;

  /** Returns the number of \link InputVariable variables\endlink in this problem that derive from
   *  ContinuousVariable. */
  int numContinuousVariables() const;

  /** Returns the number of \link InputVariable variables\endlink in this problem that derive from
   *  DiscreteVariable */
  int numDiscreteVariables() const;

  /** Returns the number of discrete variables that have 0-1 measures selected. Such variables
   *  can be thought of as static model tranformations, rather than as true variables, since they
   *  have the same effect every time. */
  int numStaticTransformations() const;

  /** Returns true if numContinuousVariables() == numVariables(). */
  bool allVariablesAreContinuous() const;

  /** Returns true if all DiscreteVariables have either zero or one measure selected. Such
   *  discrete variables can be thought of as model transformations, rather than variables,
   *  and can be hidden from \link DakotaAlgorithm DakotaAlgorithms\endlink. */
  bool allVariablesAreContinuousOrStaticTransformations() const;

  /** Returns true if numContinuousVariables() == 0. */
  bool allVariablesAreDiscrete() const;

  /** Returns this problem's response functions. A response function is typically a simulation
   *  output like site energy use (GJ) that is expected to be dependent on the variables' values,
   *  and is important to the design or research question at hand (which is often centered around
   *  some aspect of building performance). */
  std::vector<Function> responses() const;

  boost::optional<Function> getResponseByUUID(const UUID& uuid) const;

  /** Returns the number of response functions associated with this problem. */
  int numResponses() const;

  /** Returns the input file type expected by workflow(). */
  boost::optional<FileReferenceType> inputFileType() const;

  /** Returns true if dataPoint.variableValues() make sense in the context of this Problem. */
  bool isValid(const DataPoint& dataPoint) const;

  /** Returns true if childStep is in this Problem, and proposedInputFileType and
   *  proposedOutputFileType would be acceptable in that position. Returns false otherwise. */
  bool fileTypesAreCompatible(
      const WorkflowStep& childStep,
      const boost::optional<FileReferenceType>& proposedInputFileType,
      const boost::optional<FileReferenceType>& proposedOutputFileType) const;

  /** Returns true if childVariable is in this Problem, and proposedInputFileType and
   *  proposedOutputFileType would be acceptable in that position. Returns false otherwise. */
  bool fileTypesAreCompatible(
      const InputVariable& childVariable,
      const boost::optional<FileReferenceType>& proposedInputFileType,
      const boost::optional<FileReferenceType>& proposedOutputFileType) const;

  //@}
  /** @name Getters and Queries for Discrete Problems */
  //@{

  /** Converts measures to a vector of variable values stored in QVariant format. (In this
   *  case, all of the QVariants will be of type int.) */
  std::vector<QVariant> getVariableValues(const std::vector<Measure>& measures) const;

  /** Converts measures to a vector of variable values stored in QVariant format, including
   *  null QVariant values (of the correct type, int or double) as necessary. */
  std::vector<QVariant> getVariableValues(
    const std::vector< boost::optional<Measure> >& measures) const;

  /** Converts variableValues to a vector of \link Measure
   *  Measures\endlink, leaving gaps for continuous variables by inserting
   *  boost::nones in the appropriate locations. */
  std::vector<boost::optional<Measure> > getMeasures(
      const std::vector<QVariant>& variableValues) const;

  /** \deprecated Forwards to re-named method getMeasures. */
  std::vector<boost::optional<Measure> > getDiscretePerturbations(
      const std::vector<QVariant>& variableValues) const;

  /** If allVariablesAreDiscrete(), returns the number of \link DataPoint DataPoints\endlink that
   *  would have to be simulated to populate the full mesh for this problem. If
   *  selectedOnly, the returned value represents the computational effort necessary
   *  to run DesignOfExperiments. If not selectedOnly, the returned value represents
   *  the maximum size of the problem, if all discrete possibilities were to be turned on. */
  boost::optional<int> combinatorialSize(bool selectedOnly) const;

  //@}
  /** @name Setters */
  //@{

  bool push(const WorkflowStep& step);

  bool insert(int index, const WorkflowStep& step);

  bool erase(const WorkflowStep& step);

  bool swap(const WorkflowStep& step1, const WorkflowStep& step2);

  bool setWorkflow(const std::vector<WorkflowStep>& workflow);

  void clearWorkflow();

  void pushResponse(const Function& response);

  bool insertResponse(int index, const Function& response);

  bool eraseResponse(const Function& response);

  bool swapResponses(const Function& response1,const Function& response2);

  void setResponseFunctions(const std::vector<Function>& responses);

  void clearResponseFunctions();

  //@}
  /** @name Actions */
  //@{

  /** Updates objects using BCLMeasures with the same UUID as newVersion to use newVersion.
   *  Merges argument designations and values with newArguments if (not newArguments.empty())
   *  or (not keepOldArgumentsIfNewEmpty). Returns true if operation proceeds smoothly. Returns
   *  false if any old versions have to be left in place (perhaps because of input/output
   *  file type conflicts). */
  bool updateMeasure(const BCLMeasure& newVersion,
                     const std::vector<ruleset::OSArgument>& newArguments,
                     bool keepOldArgumentsIfNewEmpty);

  /** Returns a description of this problem for using in dakota.in files. */
  std::string dakotaInFileDescription(const DakotaAlgorithm& algorithm) const;

  /** Constructs a new (incomplete) DataPoint for this problem using variableValues (if
   *  possible). The size and contents of variableValues must be compatible with variables() for
   *  this operation to be successful. */
  boost::optional<DataPoint> createDataPoint(const std::vector<QVariant>& variableValues) const;

  /** Returns a DataPoint if measures can be transformed into a valid set of variableValues.
   *  Only works if allVariablesAreDiscrete(). */
  boost::optional<DataPoint> createDataPoint(
      const std::vector<Measure>& measures) const;

  /** Attempts to create a new DataPoint from params. Returns that DataPoint if possible; returns
   *  boost::none otherwise. Not for general use. AnalysisDriver uses this method to translate
   *  requests from DAKOTA. */
  boost::optional<DataPoint> createDataPoint(const DakotaParametersFile& params,
                                             const DakotaAlgorithm& algorithm) const;

  /** Returns the runmanager::Workflow created by instantiating all of the InputVariable 
   *  WorkflowSteps as runmanager::WorkItems using the variable values specified in dataPoint. */
  runmanager::Workflow createWorkflow(
      const DataPoint& dataPoint,
      const openstudio::path& rubyIncludeDirectory=openstudio::path()) const;

  /** Updates dataPoint post-simulation. If the completedJob is successful, it is mined for relevant
   *  files and other data, which then becomes accessible through dataPoint. */
  void updateDataPoint(DataPoint& dataPoint,
                       const runmanager::Job& completedJob) const;

  /** Returns the jobs stored in dataPoint broken down by WorkflowStep. Can be used to extract
   *  errors and warnings on a per-WorkflowStep basis. If optimize, steps that evaluate to null
   *  jobs are removed from the return vector. */
  std::vector<WorkflowStepJob> getJobsByWorkflowStep(const DataPoint& dataPoint,
                                                     bool optimize=false) const;

  /** Returns the DAKOTA results file string corresponding to dataPoint, if dataPoint completed
   *  successfully. Returns boost::none otherwise. */
  boost::optional<std::string> getDakotaResultsFile(const DataPoint& dataPoint) const;

  //@}
  /** @name Dakota Algorithm Helpers */
  //@{

  /** Returns the indices into variables() that correspond to variables that DAKOTA will treat as
   *  continuous design variables. */
  std::vector<int> getContinuousDesignVariableIndices(const DakotaAlgorithm& dakotaAlgorithm) const;

  /** Returns the indices into variables() that correspond to variables that DAKOTA will treat as
   *  discrete design variables. */
  std::vector<int> getDiscreteDesignVariableIndices(const DakotaAlgorithm& dakotaAlgorithm) const;

  /** Returns the indices into variables() that correspond to variables that DAKOTA will treat as
   *  uncertain variables, broken down by UncertaintyDescriptionType. */
  std::map<UncertaintyDescriptionType,std::vector<int> > getUncertainVariableIndices(
    const DakotaAlgorithm& dakotaAlgorithm) const;

  //@}
 protected:
  /// @cond
  typedef detail::Problem_Impl ImplType;

  friend class AnalysisObject;
  friend class detail::AnalysisObject_Impl;
  friend class detail::Problem_Impl;

  explicit Problem(std::shared_ptr<detail::Problem_Impl> impl);

  friend class Analysis;              // for calling setParent
  friend class detail::Analysis_Impl; // for calling setParent

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.analysis.Problem");
};

/** \relates Problem*/
typedef boost::optional<Problem> OptionalProblem;

/** \relates Problem*/
typedef std::vector<Problem> ProblemVector;

} // analysis
} // openstudio

#endif // ANALYSIS_PROBLEM_HPP

