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

#ifndef ANALYSIS_PROBLEM_IMPL_HPP
#define ANALYSIS_PROBLEM_IMPL_HPP

#include "AnalysisAPI.hpp"
#include "AnalysisObject_Impl.hpp"

#include "Function.hpp"
#include "Problem.hpp"

#include "../utilities/core/FileReference.hpp"

namespace openstudio {

class BCLMeasure;

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

class RubyMeasure;
class RubyContinuousVariable;

namespace detail {

  /** Problem_Impl is a AnalysisObject_Impl that is the implementation class for Problem.*/
  class ANALYSIS_API Problem_Impl : public AnalysisObject_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    Problem_Impl(const std::string& name,
                 const std::vector<WorkflowStep>& workflow);

    Problem_Impl(const std::string& name,
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
    Problem_Impl(const std::string& name,
                 const std::vector<Variable>& variables,
                 const std::vector<Function>& responses,
                 const runmanager::Workflow& simulationWorkflow);

    /** \overload \deprecated */
    Problem_Impl(const std::string& name,
                 const std::vector<Variable>& variables,
                 const runmanager::Workflow& simulationWorkflow);

    /** Constructor provided for deserialization; not for general use. */
    Problem_Impl(const UUID& uuid,
                 const UUID& versionUUID,
                 const std::string& name,
                 const std::string& displayName,
                 const std::string& description,
                 const std::vector<WorkflowStep>& workflow,
                 const std::vector<Function>& responses);

    Problem_Impl(const Problem_Impl& other);

    virtual ~Problem_Impl() {}

    virtual AnalysisObject clone() const;

    //@}
    /** @name Getters and Queries */
    //@{

    std::vector<WorkflowStep> workflow() const;

    /** If there is a runmanager::WorkItem of jobType in workflow(), returns the index of the first
     *  such runmanager::WorkItem. Otherwise evaluates to false. */
    boost::optional<int> getWorkflowStepIndexByJobType(const runmanager::JobType& jobType) const;

    int numWorkflowSteps() const;

    /** Returns this problem's (ordered) list of \link InputVariable input variables \endlink, that
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

    /** Returns the number of \link InputVariable variables \endlink in this problem. */
    int numVariables() const;

    /** Returns the number of \link InputVariable variables \endlink in this problem that derive from
     *  ContinuousVariable. */
    int numContinuousVariables() const;

    /** Returns the number of \link InputVariable variables \endlink in this problem that derive from
     *  DiscreteVariable */
    int numDiscreteVariables() const;

    /** Number of discrete variables with 0-1 measures selected. */
    int numStaticTransformations() const;

    bool allVariablesAreContinuous() const;

    /** Returns true if all DiscreteVariables only consist of one measure. Such discrete
     *  discrete variables can be thought of as model transformations, rather than variables,
     *  and can be hidden from \link DakotaAlgorithm DakotaAlgorithms \endlink. */
    bool allVariablesAreContinuousOrStaticTransformations() const;

    bool allVariablesAreDiscrete() const;

    std::vector<Function> responses() const;

    boost::optional<Function> getResponseByUUID(const UUID& uuid) const;

    int numResponses() const;

    boost::optional<FileReferenceType> inputFileType() const;

    virtual bool isValid(const DataPoint& dataPoint) const;

    /** Returns true if childVariable is in this Problem, and proposedInputFileType and
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
    std::vector<QVariant> getVariableValues(
        const std::vector<Measure>& measures) const;

    /** Converts measures to a vector of variable values stored in QVariant format, including
     *  null QVariant values (of the correct type, int or double) as necessary. */
    std::vector<QVariant> getVariableValues(
      const std::vector< boost::optional<Measure> >& measures) const;

    /** Converts variableValues to a vector of \link Measure
     *  Measures\endlink, leaving gaps for continuous variables by inserting
     *  boost::nones in the appropriate locations. */
    std::vector<boost::optional<Measure> > getMeasures(
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
    virtual std::string dakotaInFileDescription(const DakotaAlgorithm& algorithm) const;

    virtual boost::optional<DataPoint> createDataPoint(
        const std::vector<QVariant>& variableValues) const;

    /** Returns a DataPoint if measures can be transformed into a valid set of variableValues.
     *  Only works if allVariablesAreDiscrete(). */
    virtual boost::optional<DataPoint> createDataPoint(
        const std::vector<Measure>& measures) const;

    /** Attemps create a new DataPoint from params. Returns that DataPoint if possible; returns
     *  boost::none otherwise. */
    virtual boost::optional<DataPoint> createDataPoint(const DakotaParametersFile& params,
                                                       const DakotaAlgorithm& algorithm) const;

    /** Returns the Workflow created by prepending all the Variable jobs to the simulation
     *  workflow. */
    virtual runmanager::Workflow createWorkflow(
        const DataPoint& dataPoint,
        const openstudio::path& rubyIncludeDirectory) const;

    virtual void updateDataPoint(DataPoint& dataPoint,
                                 const runmanager::Job& completedJob) const;

    /** Returns the jobs stored in dataPoint broken down by WorkflowStep. Can be used to extract
     *  errors and warnings on a per-WorkflowStep basis. If optimize, steps that evaluate to null
     *  jobs are removed from the return vector. */
    std::vector<WorkflowStepJob> getJobsByWorkflowStep(const DataPoint& dataPoint,
                                                       bool optimize=false) const;

    /** Returns the DAKOTA results file string corresponding to dataPoint, if dataPoint completed
     *  successfully. Returns boost::none otherwise. */
    virtual boost::optional<std::string> getDakotaResultsFile(const DataPoint& dataPoint) const;

    //@}
    /** @name Dakota Algorithm Helpers */
    //@{

    std::vector<int> getContinuousDesignVariableIndices(const DakotaAlgorithm& dakotaAlgorithm) const;

    std::vector<int> getDiscreteDesignVariableIndices(const DakotaAlgorithm& dakotaAlgorithm) const;

    std::map<UncertaintyDescriptionType,std::vector<int> > getUncertainVariableIndices(const DakotaAlgorithm& dakotaAlgorithm) const;

    //@}
    /** @name Protected in or Absent from Public Class */
    //@{

    virtual QVariant toVariant() const;

    static Problem factoryFromVariant(const QVariant& variant, const VersionString& version);

    static Problem fromVariant(const QVariant& variant, const VersionString& version);

    virtual QVariant toServerFormulationVariant() const;

    /// Relocate path data from originalBase to newBase.
    virtual void updateInputPathData(const openstudio::path& originalBase,
                                     const openstudio::path& newBase);

    //@}
   protected:
    std::vector<WorkflowStep> m_workflow; // InputVariables and runmanager WorkItems applied in order
    std::vector<Function> m_responses; // response functions.

   private:
    std::vector<WorkflowStep> convertVariablesAndWorkflowToWorkflowSteps(const std::vector<Variable>& variables,
                                                                         const runmanager::Workflow& simulationWorkflow) const;

    bool checkWorkflow(const std::vector<WorkflowStep>& workflow) const;

    /** Returns true if fileType corresponds to a supported energy model type.
     *  Currently must be OSM or IDF. */
    bool isEnergyModelType(const FileReferenceType& fileType) const;

    bool areInCompoundMeasure(const WorkflowStep& step,
                              const boost::optional<WorkflowStep>& nextStep) const;

    bool updateMeasureForCompoundRubyMeasure(const BCLMeasure& newVersion,
                                             const std::vector<ruleset::OSArgument>& newArguments,
                                             bool keepOldArgumentsIfNewEmpty,
                                             RubyMeasure& measure,
                                             std::vector<RubyContinuousVariable>& variables);

    REGISTER_LOGGER("openstudio.analysis.Problem");
  };

} // detail

} // model
} // openstudio

#endif // ANALYSIS_PROBLEM_IMPL_HPP
