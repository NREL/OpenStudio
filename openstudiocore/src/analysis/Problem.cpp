/**********************************************************************
*  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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

#include <analysis/Problem.hpp>
#include <analysis/Problem_Impl.hpp>

#include <analysis/Analysis.hpp>
#include <analysis/Analysis_Impl.hpp>
#include <analysis/ContinuousVariable.hpp>
#include <analysis/ContinuousVariable_Impl.hpp>
#include <analysis/DakotaAlgorithm.hpp>
#include <analysis/DakotaParametersFile.hpp>
#include <analysis/DataPoint.hpp>
#include <analysis/DiscreteVariable.hpp>
#include <analysis/DiscreteVariable_Impl.hpp>
#include <analysis/DiscretePerturbation.hpp>
#include <analysis/Function_Impl.hpp>
#include <analysis/GenericUncertaintyDescription.hpp>
#include <analysis/NullPerturbation.hpp>
#include <analysis/NullPerturbation_Impl.hpp>
#include <analysis/RubyContinuousVariable.hpp>
#include <analysis/RubyContinuousVariable_Impl.hpp>
#include <analysis/RubyPerturbation.hpp>
#include <analysis/RubyPerturbation_Impl.hpp>
#include <analysis/UncertaintyDescription.hpp>
#include <analysis/UncertaintyDescription_Impl.hpp>
#include <analysis/WorkflowStep_Impl.hpp>

#include <runmanager/lib/WorkItem.hpp>
#include <runmanager/lib/Workflow.hpp>

#include <ruleset/OSArgument.hpp>

#include <utilities/idf/URLSearchPath.hpp>

#include <utilities/data/Attribute.hpp>

#include <utilities/bcl/BCLMeasure.hpp>

#include <utilities/core/Assert.hpp>
#include <utilities/core/FileReference.hpp>
#include <utilities/core/Finder.hpp>
#include <utilities/core/URLHelpers.hpp>
#include <utilities/core/Containers.hpp>
#include <utilities/core/Optional.hpp>
#include <utilities/core/Compare.hpp>

#include <boost/foreach.hpp>
#include <boost/bind.hpp>

#include <sstream>
#include <limits>

using namespace openstudio::ruleset;

namespace openstudio {
namespace analysis {

namespace detail {

  Problem_Impl::Problem_Impl(const std::string& name,
                             const std::vector<WorkflowStep>& workflow)
    : AnalysisObject_Impl(name),
      m_workflow(workflow)
  {
    bool ok = checkWorkflow(workflow);
    if (!ok) {
      LOG_AND_THROW("Could not construct Problem because of inconsistency in chain of "
                    << "energy model types induced by workflow.");
    }
    BOOST_FOREACH(WorkflowStep& step, m_workflow) {
      step.onChange();
      connectChild(step,false);
    }
  }

  Problem_Impl::Problem_Impl(const std::string& name,
                             const std::vector<WorkflowStep>& workflow,
                             const std::vector<Function>& responses)
    : AnalysisObject_Impl(name),
      m_workflow(workflow),
      m_responses(responses)
  {
    bool ok = checkWorkflow(workflow);
    if (!ok) {
      LOG_AND_THROW("Could not construct Problem because of inconsistency in chain of "
                    << "energy model types induced by workflow.");
    }
    BOOST_FOREACH(WorkflowStep& step, m_workflow) {
      step.onChange();
      connectChild(step,false);
    }
    BOOST_FOREACH(Function& response,m_responses) {
      response.onChange();
      connectChild(response,false);
    }
  }

  Problem_Impl::Problem_Impl(const std::string& name,
                             const std::vector<Variable>& variables,
                             const std::vector<Function>& responses,
                             const runmanager::Workflow& simulationWorkflow)
    : AnalysisObject_Impl(name),
      m_responses(responses)
  {
    m_workflow = convertVariablesAndWorkflowToWorkflowSteps(variables,simulationWorkflow);
    bool ok = checkWorkflow(m_workflow);
    if (!ok) {
      LOG_AND_THROW("Could not construct Problem because of inconsistency in chain of "
                    << "energy model types induced by workflow.");
    }
    BOOST_FOREACH(WorkflowStep& step, m_workflow) {
      step.onChange();
      connectChild(step,false);
      if (step.isInputVariable()) {
        OS_ASSERT(step.inputVariable().parent());
        OS_ASSERT(step.inputVariable().parent().get() == step);
      }
    }
    BOOST_FOREACH(Function& response,m_responses) {
      response.onChange();
      connectChild(response,false);
    }
  }

  Problem_Impl::Problem_Impl(const std::string& name,
                             const std::vector<Variable>& variables,
                             const runmanager::Workflow& simulationWorkflow)
    : AnalysisObject_Impl(name)
  {
    m_workflow = convertVariablesAndWorkflowToWorkflowSteps(variables,simulationWorkflow);
    bool ok = checkWorkflow(m_workflow);
    if (!ok) {
      LOG_AND_THROW("Could not construct Problem because of inconsistency in chain of "
                    << "energy model types induced by workflow.");
    }
    BOOST_FOREACH(WorkflowStep& step, m_workflow) {
      step.onChange();
      connectChild(step,false);
      if (step.isInputVariable()) {
        OS_ASSERT(step.inputVariable().parent());
        OS_ASSERT(step.inputVariable().parent().get() == step);
      }
    }
  }

  Problem_Impl::Problem_Impl(const UUID& uuid,
                             const UUID& versionUUID,
                             const std::string& name,
                             const std::string& displayName,
                             const std::string& description,
                             const std::vector<WorkflowStep>& workflow,
                             const std::vector<Function>& responses)
    : AnalysisObject_Impl(uuid,versionUUID,name,displayName,description),
      m_workflow(workflow),
      m_responses(responses)
  {
    BOOST_FOREACH(WorkflowStep& step, m_workflow) {
      connectChild(step,false);
    }
    BOOST_FOREACH(Function& response,m_responses) {
      connectChild(response,false);
    }
  }

  Problem_Impl::Problem_Impl(const Problem_Impl &other)
    : AnalysisObject_Impl(other)
  {
    BOOST_FOREACH(const WorkflowStep& step,other.workflow()) {
      m_workflow.push_back(step.clone().cast<WorkflowStep>());
      connectChild(m_workflow.back(),false);
    }
    BOOST_FOREACH(const Function& response,other.responses()) {
      m_responses.push_back(response.clone().cast<Function>());
      connectChild(m_responses.back(),false);
    }
  }

  AnalysisObject Problem_Impl::clone() const {
    boost::shared_ptr<Problem_Impl> impl(new Problem_Impl(*this));
    Problem result(impl);
    WorkflowStepVector workflow = result.workflow();
    BOOST_FOREACH(WorkflowStep& step,workflow) {
      step.setParent(result);
    }
    FunctionVector responses = result.responses();
    BOOST_FOREACH(Function& response,responses) {
      response.setParent(result);
    }
    return result;
  }

  std::vector<WorkflowStep> Problem_Impl::workflow() const {
    return m_workflow;
  }

  boost::optional<int> Problem_Impl::getWorkflowStepIndexByJobType(
      const runmanager::JobType& jobType) const
  {
    OptionalInt result;
    int index = 0;
    BOOST_FOREACH(const WorkflowStep& step,workflow()) {
      if (step.isWorkItem() && (step.workItemType() == jobType)) {
        result = index;
        break;
      }
      ++index;
    }
    return result;
  }

  int Problem_Impl::numWorkflowSteps() const {
    return int(m_workflow.size());
  }

  std::vector<InputVariable> Problem_Impl::variables() const {
    InputVariableVector result;
    Problem copyOfThis = getPublicObject<Problem>(); // for debug asserts below
    int i = 0;
    BOOST_FOREACH(const WorkflowStep& step,workflow()) {
      if (step.isInputVariable()) {
        result.push_back(step.inputVariable());
        // debug asserts
        OS_ASSERT(result.back().parent());
        OS_ASSERT(result.back().parent().get() == step);
        OS_ASSERT(step.parent());
        OS_ASSERT(step.parent().get() == copyOfThis);
      }
      ++i;
    }
    return result;
  }

  InputVariable Problem_Impl::getVariable(int index) const {
    return variables()[index];
  }

  boost::optional<int> Problem_Impl::getVariableIndexByUUID(const UUID& uuid) const {
    OptionalInt result;
    int index = 0;
    BOOST_FOREACH(const WorkflowStep& step,workflow()) {
      if (step.isInputVariable()) {
        if (step.inputVariable().uuid() == uuid) {
          result = index;
          break;
        }
        ++index;
      }
    }
    return result;
  }

  boost::optional<InputVariable> Problem_Impl::getVariableByUUID(const UUID& uuid) const {
    OptionalInputVariable result;
    BOOST_FOREACH(const WorkflowStep& step,workflow()) {
      if (step.isInputVariable()) {
        InputVariable variable = step.inputVariable();
        if (variable.uuid() == uuid) {
          result = variable;
          break;
        }
      }
    }
    return result;
  }

  int Problem_Impl::numVariables() const {
    return variables().size();
  }

  int Problem_Impl::numContinuousVariables() const {
    InputVariableVector vars = variables();
    ContinuousVariableVector cvars = subsetCastVector<ContinuousVariable>(vars);
    return cvars.size();
  }

  int Problem_Impl::numDiscreteVariables() const {
    InputVariableVector vars = variables();
    DiscreteVariableVector dvars = subsetCastVector<DiscreteVariable>(vars);
    return dvars.size();
  }

  int Problem_Impl::numStaticTransformations() const {
    InputVariableVector vars = variables();
    DiscreteVariableVector dvars = subsetCastVector<DiscreteVariable>(vars);

    int result(0);
    BOOST_FOREACH(const DiscreteVariable& dvar,dvars) {
      if (dvar.numPerturbations(true) < 2) {
        ++result;
      }
    }

    return result;
  }

  bool Problem_Impl::allVariablesAreContinuous() const {
    return !allVariablesAreDiscrete();
  }

  bool Problem_Impl::allVariablesAreContinuousOrStaticTransformations() const {
    BOOST_FOREACH(const InputVariable& variable,variables()) {
      if (OptionalDiscreteVariable oDiscrete = variable.optionalCast<DiscreteVariable>()) {
        if (oDiscrete->numPerturbations(true) > 1) {
          return false;
        }
      }
    }
    return true;
  }

  bool Problem_Impl::allVariablesAreDiscrete() const {
    BOOST_FOREACH(const InputVariable& variable,variables()) {
      OptionalDiscreteVariable oDiscrete = variable.optionalCast<DiscreteVariable>();
      if (!oDiscrete) { return false; }
    }
    return true;
  }

  std::vector<Function> Problem_Impl::responses() const {
    return m_responses;
  }

  boost::optional<Function> Problem_Impl::getResponseByUUID(const UUID& uuid) const {
    OptionalFunction result;
    BOOST_FOREACH(const Function& response, responses()) {
      if (response.uuid() == uuid) {
        result = response;
        break;
      }
    }
    return result;
  }

  int Problem_Impl::numResponses() const {
    return m_responses.size();
  }

  boost::optional<FileReferenceType> Problem_Impl::inputFileType() const {
    OptionalFileReferenceType result;
    BOOST_FOREACH(const WorkflowStep& step, workflow()) {
      result = step.inputFileType();
      if (result) {
        break;
      }
    }
    return result;
  }

  bool Problem_Impl::isValid(const DataPoint& dataPoint) const {
    // should already be pointing to this Problem
    if (dataPoint.problem() != getPublicObject<Problem>()) {
      LOG(Warn,"DataPoint is not valid for Problem '" << name() << "', " << toString(uuid()) <<
          ", because it points to a different Problem '" << dataPoint.problem().name() << "', "
          << toString(dataPoint.problem().uuid()) << ".");
      return false;
    }

    // check that variableValues are reasonable.
    std::vector<QVariant> variableValues = dataPoint.variableValues();
    InputVariableVector myVariables = variables();

    if (variableValues.size() != myVariables.size()) {
      LOG(Warn,"DataPoint is not valid for Problem '" << name() << "', because Problem has "
          << myVariables.size() << " variables, and the DataPoint has " << variableValues.size()
          << " variable values.");
      return false;
    }

    unsigned i = 0;
    BOOST_FOREACH(const InputVariable& myVariable, myVariables) {
      if (!myVariable.isValid(variableValues[i])) {
        LOG(Warn,"DataPoint is not valid for Problem '" << name() << "', because the value "
            << variableValues[i].toString().toStdString() << " is not valid for Variable '"
            << myVariable.name() << "'.");
        return false;
      }
      ++i;
    }

    return true;
  }

  bool Problem_Impl::fileTypesAreCompatible(
      const WorkflowStep& childStep,
      const boost::optional<FileReferenceType>& proposedInputFileType,
      const boost::optional<FileReferenceType>& proposedOutputFileType) const
  {
    OptionalFileReferenceType currentType;
    if (parent()) {
      currentType = parent()->cast<Analysis>().seed().fileType();
    }
    WorkflowStepVector workflow = this->workflow();
    bool foundChild(false), inCompoundMeasure(false), prevInCompoundMeasure(false);
    for (unsigned i = 0, n = workflow.size(); i < n; ++i) {
      // if true, previous and this are in a compound measure together
      // -------- ?
      //  p   c | n
      // --------
      prevInCompoundMeasure = inCompoundMeasure;
      WorkflowStep step = workflow[i];
      OptionalWorkflowStep nextStep;
      if (i+1 < n) {
        nextStep = workflow[i+1];
      }
      // if true, this and next are in a compound measure together
      //  ? --------
      //  p | c   n
      //  ? --------
      inCompoundMeasure = areInCompoundMeasure(step,nextStep);
      OptionalFileReferenceType stepInputType = step.inputFileType();
      OptionalFileReferenceType stepOutputType = step.outputFileType();

      if (step == childStep) {
        // for now, fail if in the middle of a compound measure
        // TODO: Change the interface of this test, or have alternative test that
        // checks for whether fully qualified proposal (proposedStep) is okay,
        // including maintaining integrity of compound measures
        if (prevInCompoundMeasure && inCompoundMeasure) {
          return false;
        }

        // check proposedInputFileType
        if (prevInCompoundMeasure) {
          // haven't handled output of compound measure yet
          if (stepOutputType) {
            currentType = stepOutputType; // ok b/c only chains of RubyContinuousVariables right now
          }
        }

        if (currentType && proposedInputFileType && (currentType.get() != proposedInputFileType.get())) {
          return false;
        }

        if (!currentType && proposedInputFileType) {
          currentType = proposedInputFileType;
        }

        if (proposedOutputFileType) {
          currentType = proposedOutputFileType;
        }

        inCompoundMeasure = false;
        foundChild = true;
        continue;
      }

      // update currentType

      if (!prevInCompoundMeasure) {
        // input type
        if (stepInputType) {
          if (!currentType) {
            currentType = stepInputType;
          }
          else {
            if (!(currentType.get() == stepInputType.get())) {
              if (foundChild) {
                return false;
              }
              else {
                LOG(Error,"Problem workflow has gotten out of sync. At step " << i
                    << ", was expecting it to want a " << currentType.get().valueName()
                    << ", but it wants a " << stepInputType.get().valueName());
              }
            }
          }
          if (foundChild) {
            // have checked past childStep against non-null input type
            return true;
          }
        }
      }

      if (!inCompoundMeasure) {
        // outputType
        if (stepOutputType) {
          if (foundChild) {
            // have checked past childStep and somehow come to a WorkflowStep that
            // takes null model type and outputs non-null model type
            // strange condition to hit this and not above, so log debug message
            LOG(Debug,"After finding a child WorkflowStep, had null input file type and "
                << "non-null output file type. This is strange and may indicate a bug.");
            return true;
          }
          currentType = stepOutputType;
        }
      }

    }

    return foundChild;
  }

  bool Problem_Impl::fileTypesAreCompatible(
      const InputVariable& childVariable,
      const boost::optional<FileReferenceType>& proposedInputFileType,
      const boost::optional<FileReferenceType>& proposedOutputFileType) const
  {
    if (childVariable.parent() && childVariable.parent()->optionalCast<WorkflowStep>()) {
      return fileTypesAreCompatible(childVariable.parent()->cast<WorkflowStep>(),
                                    proposedInputFileType,
                                    proposedOutputFileType);
    }
    return false;
  }

  std::vector<QVariant> Problem_Impl::getVariableValues(
      const std::vector<DiscretePerturbation>& perturbations) const
  {
    return getVariableValues(std::vector<OptionalDiscretePerturbation>(
        perturbations.begin(),perturbations.end()));
  }

  std::vector<QVariant> Problem_Impl::getVariableValues(
    const std::vector< boost::optional<DiscretePerturbation> >& perturbations) const
  {
    std::vector<QVariant> result, intermediate;

    int n = numVariables();
    int m = perturbations.size();
    if (m > n) {
      LOG(Warn,"Cannot determine variable values for Problem '" << name() <<
          "' from perturbation vector of length " << perturbations.size() <<
          ", because this problem has " << n << " variables.");
      return result;
    }

    InputVariableVector variables = this->variables();
    for (int i = 0; i < n && i < m; ++i) {
      OptionalDiscreteVariable discreteVariable = variables[i].optionalCast<DiscreteVariable>();
      if (discreteVariable) {
        if (!perturbations[i]) {
          intermediate.push_back(QVariant(QVariant::Int));
          continue;
        }
        OptionalInt perturbationIndex = discreteVariable->getIndexByUUID(perturbations[i].get());
        if (!perturbationIndex) {
          LOG(Warn,"Cannot determine variable values for Problem '" << name()
              << "' from the provided perturbation vector, because the perturbation at index " <<
              i << " is not in the variable at index " << i << " (getPerturbationByUUID).");
          return result;
        }
        intermediate.push_back(QVariant(*perturbationIndex));
        continue;
      }
      // OptionalContinuousVariable continuousVariable = variables[i].optionalCast<ContinuousVariable>();
      // OS_ASSERT(continuousVariable);
      intermediate.push_back(QVariant(QVariant::Double));
    }
    result = intermediate;
    return result;
  }

  std::vector<boost::optional<DiscretePerturbation> > Problem_Impl::getDiscretePerturbations(
      const std::vector<QVariant>& variableValues) const
  {
    std::vector<OptionalDiscretePerturbation> result, intermediate;

    int n = numVariables();
    int m = variableValues.size();
    if (m > n) {
      LOG(Warn,"Cannot determine discrete perturbations for Problem '" << name() <<
          "' from variable values vector of length " << m <<
          ", because this problem has " << n << " variables.");
      return result;
    }

    InputVariableVector variables = this->variables();
    for (int i = 0; i < n && i < m; ++i) {
      OptionalDiscreteVariable discreteVariable = variables[i].optionalCast<DiscreteVariable>();
      if (discreteVariable) {
        QVariant value = variableValues[i];
        if (value.isNull()) {
          intermediate.push_back(boost::none);
          continue;
        }
        if (!discreteVariable->isValid(value)) {
          LOG(Warn,"Cannot determine discrete perturbations for Problem '" << name() <<
          "', because the value given for variable " << i << ", '" << discreteVariable->name()
          << "', is non-null and invalid.");
          return result;
        }
        intermediate.push_back(discreteVariable->getPerturbation(value.toInt()));
        continue;
      }
      intermediate.push_back(boost::none);
    }

    result = intermediate;
    return result;
  }

  boost::optional<int> Problem_Impl::combinatorialSize(bool selectedPerturbationsOnly) const {
    OptionalInt result;
    int intermediate(0);
    InputVariableVector variables = this->variables();
    BOOST_FOREACH(const InputVariable& variable, variables) {
      OptionalDiscreteVariable discreteVariable = variable.optionalCast<DiscreteVariable>();
      if (!discreteVariable) {
        return result;
      }
      int thisVariableSize = discreteVariable->numPerturbations(selectedPerturbationsOnly);
      if (thisVariableSize == 0) {
        LOG(Warn,"Problem '" << name() << "' contains a DiscreteVariable '" << variable.name() <<
            "' with either no perturbations, or no selected perturbations. In either case, " <<
            "completely null DiscreteVariables (those with no valid integer value) are not " <<
            "supported at this time.");
        return result;
      }
      if (intermediate == 0) {
        intermediate = thisVariableSize;
      }
      else {
        intermediate *= thisVariableSize;
      }
    }
    result = intermediate;
    return result;
  }

  bool Problem_Impl::push(const WorkflowStep& step) {
    WorkflowStepVector candidates = workflow();
    candidates.push_back(step);
    if (!checkWorkflow(candidates)) {
      return false;
    }

    m_workflow.push_back(step);
    m_workflow.back().onChange();
    connectChild(m_workflow.back(),true);
    if (step.isInputVariable()) {
      onChange(AnalysisObject_Impl::InvalidatesDataPoints);
    }
    else {
      onChange(AnalysisObject_Impl::InvalidatesResults);
    }
    return true;
  }

  bool Problem_Impl::insert(int index, const WorkflowStep& step) {
    if ((index < 0) || (index > numWorkflowSteps())) {
      return false;
    }
    if (index == numWorkflowSteps()) {
      return push(step);
    }

    WorkflowStepVector candidates = workflow();
    WorkflowStepVector::iterator it = candidates.begin();
    for (int count = 0; count < index; ++count, ++it);
    candidates.insert(it,step);
    if (!checkWorkflow(candidates)) {
      return false;
    }

    m_workflow = candidates;
    // mark variables with changed indicies as dirty
    for (int i = index, n = int(m_workflow.size()); i < n; ++i) {
      m_workflow[i].onChange();
    }
    connectChild(m_workflow[index],true);
    if (step.isInputVariable()) {
      onChange(AnalysisObject_Impl::InvalidatesDataPoints);
      OS_ASSERT(step.inputVariable().parent());
      OS_ASSERT(step.inputVariable().parent().get() == step);
      OS_ASSERT(step.inputVariable().parent().get() == m_workflow[index]);
    }
    else {
      onChange(AnalysisObject_Impl::InvalidatesResults);
    }
    return true;
  }

  bool Problem_Impl::erase(const WorkflowStep& step) {
    WorkflowStepVector candidates = workflow();
    WorkflowStepVector::iterator it = std::find_if(
          candidates.begin(),
          candidates.end(),
          boost::bind(uuidsEqual<WorkflowStep,WorkflowStep>,_1,step));
    if (it == candidates.end()) {
      return false;
    }
    WorkflowStep toErase = *it;
    int index = int(it - candidates.begin());
    candidates.erase(it);
    if (!checkWorkflow(candidates)) {
      return false;
    }

    disconnectChild(toErase);
    m_workflow = candidates;
    for (int i = index, n = numWorkflowSteps(); i < n; ++i) {
      m_workflow[i].onChange();
    }
    if (toErase.isInputVariable()) {
      onChange(AnalysisObject_Impl::InvalidatesDataPoints);
    }
    else {
      onChange(AnalysisObject_Impl::InvalidatesResults);
    }
    return true;
  }

  bool Problem_Impl::swap(const WorkflowStep& step1, const WorkflowStep& step2) {
    WorkflowStepVector candidates = workflow();

    WorkflowStepVector::iterator it1 = std::find_if(
          candidates.begin(),
          candidates.end(),
          boost::bind(uuidsEqual<WorkflowStep,WorkflowStep>,_1,step1));
    WorkflowStepVector::iterator it2 = std::find_if(
          candidates.begin(),
          candidates.end(),
          boost::bind(uuidsEqual<WorkflowStep,WorkflowStep>,_1,step2));
    if ((it1 == candidates.end()) || (it2 == candidates.end())) {
      return false;
    }
    WorkflowStep temp = *it1;
    int index1 = int(it1 - candidates.begin());
    int index2 = int(it2 - candidates.begin());
    candidates.at(index1) = *it2;
    candidates.at(index2) = temp;

    if (!checkWorkflow(candidates)) {
      return false;
    }

    m_workflow = candidates;
    m_workflow[index1].onChange();
    m_workflow[index2].onChange();
    if (step1.isInputVariable() || step2.isInputVariable()) {
      onChange(AnalysisObject_Impl::InvalidatesDataPoints);
    }
    else {
      onChange(AnalysisObject_Impl::InvalidatesResults);
    }
    return true;
  }

  bool Problem_Impl::setWorkflow(const std::vector<WorkflowStep>& workflow) {
    if (!checkWorkflow(workflow)) {
      return false;
    }

    int pNumVars = numVariables();
    BOOST_FOREACH(WorkflowStep& step,m_workflow) {
      disconnectChild(step);
    }
    m_workflow = workflow;
    BOOST_FOREACH(WorkflowStep& step,m_workflow) {
      step.onChange();
      connectChild(step,true);
    }
    int numVars = numVariables();
    if ((pNumVars == 0) && (numVars == 0)) {
      onChange(AnalysisObject_Impl::InvalidatesResults);
    }
    else {
      onChange(AnalysisObject_Impl::InvalidatesDataPoints);
    }
    return true;
  }

  void Problem_Impl::clearWorkflow() {
    int pNumVars = numVariables();
    BOOST_FOREACH(WorkflowStep& step,m_workflow) {
      disconnectChild(step);
    }
    m_workflow.clear();
    if (pNumVars == 0) {
      onChange(AnalysisObject_Impl::InvalidatesResults);
    }
    else {
      onChange(AnalysisObject_Impl::InvalidatesDataPoints);
    }
  }

  void Problem_Impl::pushResponse(const Function& response) {
    m_responses.push_back(response);
    m_responses.back().onChange();
    connectChild(m_responses.back(),true);
    onChange(AnalysisObject_Impl::InvalidatesResults);
  }

  bool Problem_Impl::insertResponse(int index, const Function& response) {
    if ((index < 0) || (index >= numResponses())) {
      return false;
    }
    FunctionVector::iterator it = m_responses.begin();
    for (int count = 0; count < index; ++count, ++it);
    it = m_responses.insert(it,response);
    for(int i = index, n = int(m_responses.size()); i < n; ++i) {
      m_responses[i].onChange();
    }
    connectChild(*it,true);
    onChange(AnalysisObject_Impl::InvalidatesResults);
    return true;
  }

  bool Problem_Impl::eraseResponse(const Function& response) {
    FunctionVector::iterator it = std::find_if(
          m_responses.begin(),
          m_responses.end(),
          boost::bind(uuidsEqual<Function,Function>,_1,response));
    if (it == m_responses.end()) {
      return false;
    }
    int index = int(it - m_responses.begin());
    disconnectChild(*it);
    m_responses.erase(it);
    for (int i = index, n = int(m_responses.size()); i < n; ++i) {
      m_responses[i].onChange();
    }
    onChange(AnalysisObject_Impl::InvalidatesResults);
    return true;
  }

  bool Problem_Impl::swapResponses(const Function& response1,const Function& response2) {
    FunctionVector::iterator it1 = std::find_if(
          m_responses.begin(),
          m_responses.end(),
          boost::bind(uuidsEqual<Function,Function>,_1,response1));
    FunctionVector::iterator it2 = std::find_if(
          m_responses.begin(),
          m_responses.end(),
          boost::bind(uuidsEqual<Function,Function>,_1,response2));
    if ((it1 == m_responses.end()) || it2 == m_responses.end()) {
      return false;
    }
    Function temp = *it1;
    int index1 = int(it1 - m_responses.begin());
    int index2 = int(it2 - m_responses.begin());
    m_responses.at(index1) = *it2;
    m_responses.at(index2) = temp;
    m_responses[index1].onChange();
    m_responses[index2].onChange();
    onChange(AnalysisObject_Impl::InvalidatesResults);
    return true;
  }

  void Problem_Impl::setResponseFunctions(const std::vector<Function>& responses) {
    BOOST_FOREACH(Function& response,m_responses) {
      disconnectChild(response);
    }
    m_responses = responses;
    BOOST_FOREACH(Function& response,m_responses) {
      response.onChange();
      connectChild(response,true);
    }
    onChange(AnalysisObject_Impl::InvalidatesResults);
  }

  void Problem_Impl::clearResponseFunctions() {
    BOOST_FOREACH(Function& response,m_responses) {
      disconnectChild(response);
    }
    m_responses.clear();
    onChange(AnalysisObject_Impl::InvalidatesResults);
  }

  bool Problem_Impl::updateMeasure(const BCLMeasure& newVersion,
                                   const std::vector<ruleset::OSArgument>& newArguments,
                                   bool keepOldArgumentsIfNewEmpty)
  {
    // TODO: Also update Ruby WorkItems.
    bool result = true;
    UUID measureUUID = newVersion.uuid();
    InputVariableVector variables = this->variables();
    OptionalRubyPerturbation compoundRubyPerturbation;
    std::vector<RubyContinuousVariable> compoundVariables;
    BOOST_FOREACH(const InputVariable& variable, variables) {
      if (compoundRubyPerturbation) {
        // see if should clear
        bool clearCompound = false;
        if (OptionalRubyContinuousVariable rcv = variable.optionalCast<RubyContinuousVariable>()) {
          if (rcv->perturbation() == compoundRubyPerturbation.get()) {
            clearCompound = true;
          }
        }
        else {
          clearCompound = true;
        }
        if (clearCompound) {
          result = result && updateMeasureForCompoundRubyPerturbation(
                newVersion,
                newArguments,
                keepOldArgumentsIfNewEmpty,
                *compoundRubyPerturbation,
                compoundVariables);
          compoundRubyPerturbation.reset();
          compoundVariables.clear();
        }
      }

      if (compoundRubyPerturbation) {
        compoundVariables.push_back(variable.cast<RubyContinuousVariable>());
      }
      else if (OptionalDiscreteVariable dv = variable.optionalCast<DiscreteVariable>()) {
        DiscretePerturbationVector dps = dv->perturbations(false);
        BOOST_FOREACH(DiscretePerturbation& dp,dps) {
          if (OptionalRubyPerturbation rp = dp.optionalCast<RubyPerturbation>()) {
            if (rp->usesBCLMeasure() && (rp->measureUUID() == measureUUID)) {
              bool ok(true);
              if (newArguments.empty() && keepOldArgumentsIfNewEmpty) {
                OSArgumentVector currentArguments = rp->arguments();
                ok = rp->setMeasure(newVersion);
                if (ok) {
                  rp->setArguments(currentArguments);
                }
              }
              else {
                ok = rp->updateMeasure(newVersion,newArguments);
              }
              if (!ok) {
                // bad match between file types
                ok = dv->erase(*rp);
                if (!ok) {
                  result = false;
                }
              }
            }
          }
        }
      }
      else if (OptionalRubyContinuousVariable rcv = variable.optionalCast<RubyContinuousVariable>()) {
        if (rcv->perturbation().usesBCLMeasure() &&
            (rcv->perturbation().measureUUID() == measureUUID))
        {
          compoundRubyPerturbation = rcv->perturbation();
          compoundVariables.push_back(*rcv);
        }
      }
    }

    return result;
  }

  std::string Problem_Impl::dakotaInFileDescription(const DakotaAlgorithm& algorithm) const {
    ContinuousVariableVector continuousVariables = subsetCastVector<ContinuousVariable>(variables());
    DiscreteVariableVector discreteVariables = subsetCastVector<DiscreteVariable>(variables());

    // screen discrete variables with only one option (treat as static transformations, not variables)
    DiscreteVariableVector::iterator it = discreteVariables.begin();
    while (it != discreteVariables.end()) {
      if (it->numPerturbations(true) < 2u) {
        it = discreteVariables.erase(it);
        continue;
      }
      ++it;
    }

    std::stringstream ss;
    ss << "variables," << std::endl;

    InputVariableVector vars = variables();
    IntVector cdvs = getContinuousDesignVariableIndices(algorithm);
    IntVector ddvs = getDiscreteDesignVariableIndices(algorithm);
    std::map<UncertaintyDescriptionType,std::vector<int> > uv_types = getUncertainVariableIndices(algorithm);

    // continuous design variables
    int n_cdvs = cdvs.size();
    if (n_cdvs > 0) {
      ss << "        continuous_design = " << n_cdvs << std::endl;
      ss << "          lower_bounds" << std::endl << "           ";
      for (int i = 0; i < n_cdvs; ++i) {
        if (OptionalContinuousVariable cvar = vars[cdvs[i]].optionalCast<ContinuousVariable>()) {
          if (OptionalDouble min = cvar->minimum()) {
            ss << " " << boost::lexical_cast<std::string>(*min);
          }
          else {
            ss << " -DBL_MAX";
          }
        }
        else {
          ss << " -0.5";
        }
      }
      ss << std::endl;
      ss << "          upper_bounds" << std::endl << "           ";
      for (int i = 0; i < n_cdvs; ++i) {
        if (OptionalContinuousVariable cvar = vars[cdvs[i]].optionalCast<ContinuousVariable>()) {
          if (OptionalDouble max = cvar->maximum()) {
            ss << " " << boost::lexical_cast<std::string>(*max);
          }
          else {
            ss << " +DBL_MAX";
          }
        }
        else {
          DiscreteVariable dvar = vars[cdvs[i]].cast<DiscreteVariable>();
          ss << " " << double(dvar.numPerturbations(true)) - 0.5;
        }
      }
      ss << std::endl;
    }

    // discrete design variables
    int n_ddvs = ddvs.size();
    if (n_ddvs > 0) {
      ss << "        discrete_design_set_integer = " << n_ddvs << std::endl;
      ss << "          num_set_values" << std::endl << "           ";
      for (int i = 0; i < n_ddvs; ++i) {
        DiscreteVariable dvar = vars[ddvs[i]].cast<DiscreteVariable>();
        ss << " " << dvar.numPerturbations(true);
      }
      ss << std::endl << "          set_values" << std::endl;
      for (int i = 0; i < n_ddvs; ++i) {
        DiscreteVariable dvar = vars[ddvs[i]].cast<DiscreteVariable>();
        ss << "           ";
        int j(0);
        BOOST_FOREACH(const DiscretePerturbation& dpert, dvar.perturbations(false)) {
          if (dpert.isSelected()) {
            ss << " " << j;
          }
          ++j;
        }
        ss << std::endl;
      }
    }
    ss << std::endl;

    // uncertain variables
    for (std::map<UncertaintyDescriptionType,std::vector<int> >::const_iterator
         it = uv_types.begin(), itEnd = uv_types.end(); it != itEnd; ++it)
    {
      IntVector uvs = it->second;
      int n_uvs = uvs.size();
      GenericUncertaintyDescriptionVector udescs;
      for (int i = 0; i < n_uvs; ++i) {
        udescs.push_back(vars[uvs[i]].uncertaintyDescription().get().cast<GenericUncertaintyDescription>());
      }
      UncertaintyDescription uncertDesc = (vars[uvs[0]].uncertaintyDescription().get());
      ss << "        " << uncertDesc.type().valueName() << " = " << n_uvs << std::endl;
      if (uncertDesc.type() == UncertaintyDescriptionType::lognormal_uncertain) {
        std::vector<OptionalAttribute> values1(n_uvs,OptionalAttribute());
        std::vector<OptionalAttribute> values2(n_uvs,OptionalAttribute());
        std::vector<OptionalAttribute> lowerBounds(n_uvs,OptionalAttribute());
        std::vector<OptionalAttribute> upperBounds(n_uvs,OptionalAttribute());
        std::string string1;
        std::string string2;
        bool anyLower = false;
        bool anyUpper = false;
        for (int i = 0; i < n_uvs; ++i) {
          if (udescs[i].isSet("lambdas") && udescs[i].isSet("zetas")) {
            values1[i] = udescs[i].getAttribute("lambdas");
            values2[i] = udescs[i].getAttribute("zetas");
            string1 = "lambdas";
            string2 = "zetas";
          }
          else if (udescs[i].isSet("means") && udescs[i].isSet("error_factors")) {
            values1[i] = udescs[i].getAttribute("means");
            values2[i] = udescs[i].getAttribute("error_factors");
            string1 = "means";
            string2 = "error_factors";
          }
          else if (udescs[i].isSet("means") && udescs[i].isSet("std_deviations")) {
            values1[i] = udescs[i].getAttribute("means");
            values2[i] = udescs[i].getAttribute("std_deviations");
            string1 = "means";
            string2 = "std_deviations";
          }
          else {
            LOG(Debug,"Either Lambdas and Zeta, Means and Error Factors, or Means and Standard Deviations must be set in a lognormal distribution.");
          }
          if (udescs[i].isSet("lower_bounds")) {
            anyLower = true;
            lowerBounds[i] = udescs[i].getAttribute("lower_bounds");
          }
          if (udescs[i].isSet("upper_bounds")) {
            anyUpper = true;
            upperBounds[i] = udescs[i].getAttribute("upper_bounds");
          }
        }
        ss << "          " << string1 << std::endl << "           ";
        BOOST_FOREACH(const OptionalAttribute& value, values1) {
          if (value){
            ss << " " << value->toString();
          }
        }
        ss << std::endl << "          " << string2 << std::endl << "           ";
        BOOST_FOREACH(const OptionalAttribute& value, values2) {
          if (value){
            ss << " " << value->toString();
          }
        }
        ss << std::endl;
        if (anyLower) {
          ss << "          lower_bounds" << std::endl << "           ";
          BOOST_FOREACH(const OptionalAttribute& value, lowerBounds) {
            if (value){
              ss << " " << value->toString();
            }
            else {
              ss << " " << toString(std::numeric_limits<double>::epsilon());
            }
          }
          ss << std::endl;
        }
        if (anyUpper) {
          ss << "          upper_bounds" << std::endl << "           ";
          BOOST_FOREACH(const OptionalAttribute& value, upperBounds) {
            if (value){
              ss << " " << value->toString();
            }
            else {
              double maxValue = std::numeric_limits<double>::max();
              maxValue *= (1.0 - 1.0E-5); // make sure is < DAKOTA's max
              ss << " " << toString(maxValue);
            }
          }
          ss << std::endl;
        }
      }
      else if (uncertDesc.type() == UncertaintyDescriptionType::histogram_bin_uncertain) {
        std::vector<OptionalAttribute> abscissas(n_uvs,OptionalAttribute());
        std::vector<OptionalAttribute> values1(n_uvs,OptionalAttribute());
        std::vector<OptionalAttribute> numPairs(n_uvs,OptionalAttribute());
        std::string string1;
        for (int i = 0; i < n_uvs; ++i) {
          if (udescs[i].isSet("abscissas") && udescs[i].isSet("ordinates")) {
            abscissas[i] = udescs[i].getAttribute("abscissas");
            values1[i] = udescs[i].getAttribute("ordinates");
            numPairs[i] = udescs[i].getAttribute("num_pairs");
            string1 = "ordinates";
          }
          else if (udescs[i].isSet("abscissas") && udescs[i].isSet("counts")) {
            abscissas[i] = udescs[i].getAttribute("abscissas");
            values1[i] = udescs[i].getAttribute("counts");
            numPairs[i] = udescs[i].getAttribute("num_pairs");
            string1 = "counts";
          }
          else {
            LOG(Debug,"Abscissas and either counts or ordinates must be set in a histogram bin distribution.");
          }
        }
        ss << "          abscissas" << std::endl << "           ";
        BOOST_FOREACH(const OptionalAttribute& value, abscissas) {
          std::vector<double> attVector = getDoubleVectorFromAttribute(value.get());
          for (std::vector<double>::iterator i = attVector.begin(); i != attVector.end(); ++i) {
            ss << *i << " ";
          }
        }
        ss << std::endl << "          " << string1 << std::endl << "           ";
        BOOST_FOREACH(const OptionalAttribute& value, values1) {
          std::vector<double> attVector = getDoubleVectorFromAttribute(value.get());
          for (std::vector<double>::iterator i = attVector.begin(); i != attVector.end(); ++i) {
            ss << *i << " ";
          }
        }
        ss << std::endl;
        if (!numPairs.empty()) {
           ss << "          num_pairs" << std::endl << "           ";
          BOOST_FOREACH(const OptionalAttribute& value, numPairs) {
            if (value){
              ss << " " << value->toString();
            }
          }
          ss << std::endl;
        }
      }
      else {
        BOOST_FOREACH(const AttributeDescription& attDesc, udescs[0].attributeDescriptions()) {
          std::vector<OptionalAttribute> values(n_uvs,OptionalAttribute());
          bool atLeastOne = false;
          for (int i = 0; i < n_uvs; ++i) {
            if (attDesc.required || udescs[i].isSet(attDesc.name)) {
              values[i] = udescs[i].getAttribute(attDesc.name);
              atLeastOne = true;
            }
          }
          if (atLeastOne) {
            ss << "          " << attDesc.name << std::endl << "           ";
            BOOST_FOREACH(const OptionalAttribute& value, values) {
              if (value) {
                if (value.get().valueType() == AttributeValueType::AttributeVector) {
                  ss << " ";
                  std::vector<double> attVector = getDoubleVectorFromAttribute(value.get());
                  for (std::vector<double>::iterator i = attVector.begin(); i != attVector.end(); ++i) {
                    ss << *i << " ";
                  }
                }
                else {
                  ss << " " << value->toString();
                }
              }
              else {
                ss << " " << attDesc.defaultValue.toString().toStdString();
              }
            }
            ss << std::endl;
          }
        }
      }
    }
    ss << std::endl;

    // responses
    int nResponse = numResponses();
    if (nResponse == 0) {
      nResponse = 1; // return value of first variable so DAKOTA will run
    }
    ss << "responses," << std::endl;
    ss << "        num_response_functions = " << nResponse << std::endl;
    ss << "        no_gradients" << std::endl;
    ss << "        no_hessians" << std::endl;
    ss << std::endl;

    return ss.str();
  }

  boost::optional<DataPoint> Problem_Impl::createDataPoint(
        const std::vector<QVariant>& variableValues) const
  {
    OptionalDataPoint result;
    DataPoint candidate(getPublicObject<Problem>(),variableValues);
    if (isValid(candidate)) {
      result = candidate;
    }
    return result;
  }

  boost::optional<DataPoint> Problem_Impl::createDataPoint(
      const std::vector<DiscretePerturbation>& perturbations) const
  {
    OptionalDataPoint result;
    DataPoint candidate(getPublicObject<Problem>(),getVariableValues(perturbations));
    if (isValid(candidate)) {
      result = candidate;
    }
    return result;
  }

  boost::optional<DataPoint> Problem_Impl::createDataPoint(const DakotaParametersFile& params,
                                                           const DakotaAlgorithm& algorithm) const
  {
    OptionalDataPoint result;
    InputVariableVector vars = variables();
    int n = numVariables();

    // variable values vector of proper length
    std::vector<QVariant> variableValues(n);

    // populate continuous variables
    IntVector cdvs = getContinuousDesignVariableIndices(algorithm);
    DoubleVector cvValues = params.continuousDesignVariableValues();
    if (cdvs.size() != cvValues.size()) {
      LOG(Error,"Cannot use DakotaParametersFile to create DataPoint because was expecting " << cdvs.size() <<
          " continuous design variables, but got " << cvValues.size() << ".");
      return result;
    }
    int cvn = cdvs.size();
    for (int i = 0; i < cvn; ++i) {
      if (algorithm.requiresContinuousVariables()) {
        if (OptionalDiscreteVariable dvar = vars[cdvs[i]].optionalCast<DiscreteVariable>()) {
          // round value and set as integer
          int index(0);
          if (cvValues[i] > 0.0) {
            index = std::floor(cvValues[i] + 0.5);
          }
          int np = dvar->numPerturbations(true);
          if (index >= np) {
            LOG(Warn,"Discrete variable represented as " << cvValues[i]
                << " rounded to invalid index " << index << " of " << np - 1 << ".");
            index = np - 1;
          }
          index = dvar->getIndexByUUID(dvar->perturbations(true)[index]).get();
          variableValues[cdvs[i]] = QVariant(index);
          continue;
        }
      }
      variableValues[cdvs[i]] = QVariant(cvValues[i]);
    }

    // populate discrete variables
    IntVector ddvs = getDiscreteDesignVariableIndices(algorithm);
    IntVector dvValues = params.discreteDesignIntegerVariableValues();
    if (ddvs.size() != dvValues.size()) {
      LOG(Error,"Cannot use DakotaParametersFile to create DataPoint because was expecting " << ddvs.size() <<
          " discrete design variables, but got " << dvValues.size() << ".");
      return result;
    }
    int dvn = ddvs.size();
    if (algorithm.requiresContinuousVariables()) {
      OS_ASSERT(dvn == 0);
    }
    for (int i = 0; i < dvn; ++i) {
      variableValues[ddvs[i]] = QVariant(dvValues[i]);
    }

    // populate uncertain variables
    std::map<UncertaintyDescriptionType,std::vector<int> > uv_types = getUncertainVariableIndices(algorithm);
    for (std::map<UncertaintyDescriptionType,std::vector<int> >::const_iterator it = uv_types.begin(),
         itEnd = uv_types.end(); it != itEnd; ++it)
    {
      std::vector<QVariant> uvValues = getUncertainVariableValues(params,it->first);
      if (it->second.size() != uvValues.size()) {
        LOG(Error,"Cannot use DakotaParametersFile to create DataPoint because was expecting "
            << it->second.size() << " uncertain variables of type '" << it->first.valueName()
            << "', but got " << uvValues.size() << ".");
        return result;
      }
      int uvn = it->second.size();
      for (int i = 0; i < uvn; ++i) {
        variableValues[it->second[i]] = uvValues[i];
      }
    }

    // put in values for skipped (0-1 selected perturbations) discrete variables
    for (int i = 0; i < n; ++i) {
      if (variableValues[i].isNull()) {
        // should be skipped discrete variable
        if (OptionalDiscreteVariable dvar = vars[i].optionalCast<DiscreteVariable>()) {
          int np = dvar->numPerturbations(true);
          if (np < 2) {
            // 0-1 perturbation discrete variables are screened from DAKOTA
            if (np == 1) {
              int index = dvar->getIndexByUUID(dvar->perturbations(true)[0]).get();
              variableValues[i] = QVariant(index);
            }
            continue;
          }
        }
        LOG(Error,"Unexpected null variable value for Variable '" << vars[i].name() << "'.");
        return result;
      }
    }

    // assemble and check DataPoint
    DataPoint candidate(getPublicObject<Problem>(),variableValues);
    if (isValid(candidate)) {
      result = candidate;
    }

    return result;
  }

  runmanager::Workflow Problem_Impl::createWorkflow(
      const DataPoint& dataPoint,
      const openstudio::path& rubyIncludeDirectory) const
  {
    std::vector<runmanager::WorkItem> result; // converted to Workflow at end
    std::vector<QVariant> values = dataPoint.variableValues();
    if (int(values.size()) != numVariables()) {
      LOG_AND_THROW("DataPoint is invalid. Has wrong number of variable values. Was expecting "
                    << numVariables() << ", got " << values.size() << ".");
    }
    OptionalRubyPerturbation compoundRubyPerturbation;
    OptionalRubyPerturbation originalCompoundRubyPerturbation;
    unsigned i = 0;
    BOOST_FOREACH(const WorkflowStep& step, workflow()) {

      if (compoundRubyPerturbation) {

        if (step.isInputVariable()) {
          if (OptionalRubyContinuousVariable rcv = step.inputVariable().optionalCast<RubyContinuousVariable>()) {
            if (rcv->perturbation() == *originalCompoundRubyPerturbation) {
              // add argument to existing pertubation
              OSArgument arg = rcv->argument();
              arg.setValue(values[i].toDouble());
              compoundRubyPerturbation->addArgument(arg);
              ++i;
              continue;
            }
          }
        }

        // instantiate the compoundRubyPerturbation and then deal with the next item
        result.push_back(compoundRubyPerturbation->createWorkItem(rubyIncludeDirectory));
        compoundRubyPerturbation.reset();
        originalCompoundRubyPerturbation.reset();
      }

      if (step.isWorkItem()) {
        result.push_back(step.workItem());
      }
      else {
        InputVariable variable = step.inputVariable();
        if (OptionalRubyContinuousVariable rcv = variable.optionalCast<RubyContinuousVariable>()) {
          // start a compound measure
          originalCompoundRubyPerturbation = rcv->perturbation();
          compoundRubyPerturbation = originalCompoundRubyPerturbation->clone().cast<RubyPerturbation>();
          OSArgument arg = rcv->argument();
          arg.setValue(values[i].toDouble());
          compoundRubyPerturbation->addArgument(arg);
        }
        else {
          result.push_back(variable.createWorkItem(values[i],rubyIncludeDirectory));
        }
        ++i;
      }
    }

    if (compoundRubyPerturbation) {
      result.push_back(compoundRubyPerturbation->createWorkItem(rubyIncludeDirectory));
    }

    // put a bow on it
    runmanager::Workflow simulationWorkflow(result);
    simulationWorkflow.addParam(runmanager::JobParam("flatoutdir"));
    return simulationWorkflow;
  }

  void Problem_Impl::updateDataPoint(DataPoint& dataPoint,
                                     const runmanager::Job& completedJob) const
  {
    OS_ASSERT((completedJob.treeStatus() == runmanager::TreeStatusEnum::Finished) ||
                 (completedJob.treeStatus() == runmanager::TreeStatusEnum::Failed));

    dataPoint.markComplete();

    // Log warnings and errors
    openstudio::runmanager::JobErrors jobErrors = completedJob.treeErrors();

    typedef std::pair<runmanager::ErrorType, std::string> Err;

    BOOST_FOREACH(const Err& error, jobErrors.allErrors) {
      if (error.first == runmanager::ErrorType::Error)
      {
        LOG(Error, error.first.valueName() << ": " << error.second);
      } else if (error.first == runmanager::ErrorType::Warning) {
        LOG(Warn, error.first.valueName() << ": " << error.second);
      } else {
        LOG(Info, error.first.valueName() << ": " << error.second);
      }
    }

    // Mark failed and return if job did not complete successfully
    if ((completedJob.treeStatus() == runmanager::TreeStatusEnum::Failed) ||
        !jobErrors.succeeded())
    {
      dataPoint.markFailed();
      return;
    }

    runmanager::Files allFiles = completedJob.treeAllFiles();

    // Add input files
    try {
      openstudio::path osmInputDataPath = allFiles.getLastByExtension("osm").fullPath;
      dataPoint.setOsmInputData(FileReference(osmInputDataPath));
    }
    catch (...) {}

    try {
      openstudio::path idfInputDataPath = allFiles.getLastByExtension("idf").fullPath;
      dataPoint.setIdfInputData(FileReference(idfInputDataPath));
    }
    catch (...) {}

    // Add output files
    try {
      openstudio::path sqlOutputDataPath = allFiles.getLastByExtension("sql").fullPath;
      dataPoint.setSqlOutputData(FileReference(sqlOutputDataPath));
    }
    catch (...) {}

    try {
      openstudio::path xmlOutputDataPath = allFiles.getLastByExtension("xml").fullPath;
      dataPoint.setXmlOutputData(FileReference(xmlOutputDataPath));
    }
    catch (...) {}

    // Determine response function values
    try {
      DoubleVector responseValues;
      BOOST_FOREACH(const Function& response,responses()) {
        responseValues.push_back(response.getValue(dataPoint));
      }
      dataPoint.setResponseValues(responseValues);
    }
    catch (std::exception& e) {
      dataPoint.markFailed();
      LOG(Error,"Unable to extract response function values for the DataPoint at "
          << toString(dataPoint.directory()) << ", because " << e.what() << ".");
    }
  }

  std::vector<WorkflowStepJob> Problem_Impl::getJobsByWorkflowStep(const DataPoint& dataPoint,
                                                                   bool optimize) const
  {
    WorkflowStepJobVector result;
    if (boost::optional<runmanager::Job> topJob = dataPoint.topLevelJob()) {
      WorkflowStepVector workflow = this->workflow();
      boost::optional<runmanager::Job> currentJob = *topJob;
      for (WorkflowStepVector::const_iterator it = workflow.begin(), itEnd = workflow.end();
           it != itEnd; ++it)
      {
        WorkflowStep currentStep = *it;
        if (currentStep.isInputVariable()) {
          InputVariable var = currentStep.inputVariable();
          // compound measure?
          OptionalWorkflowStep nextStep;
          WorkflowStepVector::const_iterator jit = it; ++jit;
          if (jit != itEnd) {
            nextStep = *jit;
          }
          if (areInCompoundMeasure(currentStep,nextStep)) {
            // yes
            // no job yet, eventual job cannot be null, current step must be continuous variable
            OS_ASSERT(var.optionalCast<ContinuousVariable>());
            result.push_back(WorkflowStepJob(currentStep,QVariant(var.getValue(dataPoint))));
          }
          else {
            // maybe, doesn't matter
            // is currentJob null?
            if ((!currentJob) || (currentJob->jobType() == runmanager::JobType::Null)) {
              // must be null perturbation
              OS_ASSERT(var.optionalCast<DiscreteVariable>());
              DiscreteVariable dvar = var.cast<DiscreteVariable>();
              DiscretePerturbation dpert = dvar.getPerturbation(dataPoint);
              OS_ASSERT(dpert.optionalCast<NullPerturbation>());
              if (!optimize) {
                // include in results
                if (currentJob) {
                  result.push_back(WorkflowStepJob(*currentJob,currentStep,dpert));
                }
                else {
                  result.push_back(WorkflowStepJob(currentStep,dpert));
                }
              }
              // get next job
              if (currentJob) {
                std::vector<runmanager::Job> childJobs = currentJob->children();
                if (childJobs.empty()) {
                  currentJob.reset();
                }
                else {
                  OS_ASSERT(childJobs.size() == 1u);
                  currentJob = childJobs[0];
                }
              }
            }
            else {
              // non-null job
              bool getNextJob(true);
              if (OptionalDiscreteVariable dvar = var.optionalCast<DiscreteVariable>()) {
                DiscretePerturbation dpert = dvar->getPerturbation(dataPoint);
                if (OptionalNullPerturbation npert = dpert.optionalCast<NullPerturbation>()) {
                  // null perturbation, non-null job, do not correspond
                  // only include if !optimize
                  if (!optimize) {
                    result.push_back(WorkflowStepJob(currentStep,dpert));
                  }
                  getNextJob = false;
                }
                else {
                  result.push_back(WorkflowStepJob(*currentJob,currentStep,dpert));
                }
              }
              else {
                result.push_back(WorkflowStepJob(*currentJob,currentStep,QVariant(var.getValue(dataPoint))));
              }

              if (getNextJob) {
                std::vector<runmanager::Job> childJobs = currentJob->children();
                if (childJobs.empty()) {
                  currentJob.reset();
                }
                else {
                  OS_ASSERT(childJobs.size() == 1u);
                  currentJob = childJobs[0];
                }
              }
            }
          }
        }
        else {
          // is WorkItem
          bool getNextJob(true);
          runmanager::WorkItem workItem = currentStep.workItem();
          if (workItem.type == runmanager::JobType::Null) {
            if (!currentJob || (currentJob->jobType() != runmanager::JobType::Null)) {
              getNextJob = false;
            }
            // null work item
            if (!optimize) {
              if (currentJob && (currentJob->jobType() == runmanager::JobType::Null)) {
                result.push_back(WorkflowStepJob(*currentJob,currentStep));
              }
              else {
                result.push_back(WorkflowStepJob(currentStep));
              }
            }
          }
          else {
            // non-null work item
            OS_ASSERT(currentJob);
            OS_ASSERT(currentJob->jobType() == workItem.type);
            result.push_back(WorkflowStepJob(*currentJob,currentStep));
          }

          if (getNextJob) {
            std::vector<runmanager::Job> childJobs = currentJob->children();
            if (childJobs.empty()) {
              currentJob.reset();
            }
            else {
              OS_ASSERT(childJobs.size() == 1u);
              currentJob = childJobs[0];
            }
          }

        }
      }
    }
    return result;
  }

  boost::optional<std::string> Problem_Impl::getDakotaResultsFile(const DataPoint& dataPoint) const {
    std::stringstream ss;
    if (numResponses() > 0) {
      int fnn = 1;
      BOOST_FOREACH(double value,dataPoint.responseValues()) {
        ss << toString(value) << " response_fn_" << fnn << std::endl;
        ++fnn;
      }
    }
    else {
      // parrot back first variable value
      double value(0.0);
      std::vector<QVariant> vals = dataPoint.variableValues();
      if (!vals.empty()) {
        value = vals[0].toDouble();
      }
      ss << toString(value) << " response_fn_1" << std::endl;
    }
    return ss.str();
  }

  std::vector<int> Problem_Impl::getContinuousDesignVariableIndices(const DakotaAlgorithm& dakotaAlgorithm) const {
    IntVector result;
    InputVariableVector vars = variables();
    for (int i = 0, n = vars.size(); i < n; ++i) {
      if (OptionalContinuousVariable cv = vars[i].optionalCast<ContinuousVariable>()) {
        if (OptionalUncertaintyDescription udesc = cv->uncertaintyDescription()) {
          if (!dakotaAlgorithm.isCompatible(udesc->type())) {
            result.push_back(i);
          }
        }
        else {
          result.push_back(i);
        }
      }
      else {
        // discrete variable
        OptionalDiscreteVariable dv = vars[i].optionalCast<DiscreteVariable>();
        int np = dv->numPerturbations(true);
        if (np > 1 && dakotaAlgorithm.requiresContinuousVariables()) {
          // 0-1 perturbation discrete variables are screened from DAKOTA
          result.push_back(i);
        }
      }
    }
    return result;
  }

  std::vector<int> Problem_Impl::getDiscreteDesignVariableIndices(const DakotaAlgorithm& dakotaAlgorithm) const {
    IntVector result;

    if (dakotaAlgorithm.requiresContinuousVariables()) {
      return result;
    }

    InputVariableVector vars = variables();
    for (int i = 0, n = vars.size(); i < n; ++i) {
      if (OptionalDiscreteVariable dv = vars[i].optionalCast<DiscreteVariable>()) {
        if (OptionalUncertaintyDescription udesc = dv->uncertaintyDescription()) {
          if (!dakotaAlgorithm.isCompatible(udesc->type())) {
            result.push_back(i);
          }
        }
        else {
          result.push_back(i);
        }
      }
    }
    return result;
  }

  std::map<UncertaintyDescriptionType,std::vector<int> > Problem_Impl::getUncertainVariableIndices(
      const DakotaAlgorithm& dakotaAlgorithm) const
  {
    std::map<UncertaintyDescriptionType,std::vector<int> > result;
    InputVariableVector vars = variables();
    for (int i = 0, n = vars.size(); i < n; ++i) {
      if (OptionalUncertaintyDescription udesc = vars[i].uncertaintyDescription()) {
        if (dakotaAlgorithm.isCompatible(udesc->type())) {
          std::map<UncertaintyDescriptionType,std::vector<int> >::iterator it = result.find(udesc->type());
          if (it == result.end()) {
            result[udesc->type()] = IntVector(1u,i);
          }
          else {
            it->second.push_back(i);
          }
        }
      }
    }
    return result;
  }

  std::vector<WorkflowStep> Problem_Impl::convertVariablesAndWorkflowToWorkflowSteps(
      const std::vector<Variable>& variables,
      const runmanager::Workflow& simulationWorkflow) const
  {
    WorkflowStepVector workflow;
    BOOST_FOREACH(const Variable& variable,variables) {
      OptionalInputVariable oiv = variable.optionalCast<InputVariable>();
      if (!oiv) {
        LOG_AND_THROW("The primary variable chain must consist entirely of InputVariables.");
      }
      workflow.push_back(*oiv);
    }
    std::vector<runmanager::WorkItem> simulationWorkItems = simulationWorkflow.toWorkItems();
    workflow.insert(workflow.end(),simulationWorkItems.begin(),simulationWorkItems.end());
    return workflow;
  }

  bool Problem_Impl::checkWorkflow(const std::vector<WorkflowStep>& workflow) const {
    OptionalFileReferenceType currentType; // main-line energy model format
    std::set<FileReferenceType> allTypes;  // all types seen so far

    if (OptionalAnalysisObject parent = this->parent()) {
      currentType = parent->cast<Analysis>().seed().fileType();
      allTypes.insert(*currentType);
    }

    OptionalRubyPerturbation compoundRubyPerturbation;
    BOOST_FOREACH(const WorkflowStep& step, workflow) {

      if (compoundRubyPerturbation) {
        // see if chain is still going
        if (step.isInputVariable()) {
          if (OptionalRubyContinuousVariable rcv = step.inputVariable().optionalCast<RubyContinuousVariable>()) {
            if (rcv->perturbation() == compoundRubyPerturbation.get()) {
              continue;
            }
          }
        }

        // otherwise, evaluate compound and evaluate current step on its own
        currentType = compoundRubyPerturbation->outputFileType();
        allTypes.insert(*currentType);
        compoundRubyPerturbation.reset();
      }

      OptionalFileReferenceType inputFileType = step.inputFileType();
      OptionalFileReferenceType outputFileType = step.outputFileType();
      bool mismatch = false;
      if (currentType && inputFileType) {
        // if step outputs a new energy model, file types must be purely consistent
        if (outputFileType && isEnergyModelType(*outputFileType)) {
          if (*inputFileType != *currentType) {
            mismatch = true;
          }
        }
        else {
          // otherwise, just have to have seen primary input file type before
          if (allTypes.find(*inputFileType) == allTypes.end()) {
            mismatch = true;
          }
        }
      }
      else if (inputFileType) {
        currentType = inputFileType;
        allTypes.insert(*currentType);
      }
      if (mismatch) {
        std::stringstream ss;
        ss << "Inconsistent workflow chain. The next step is ";
        if (step.isWorkItem()) {
          ss << "a WorkItem of type " << step.workItemType().valueName() << " and expects ";
        }
        else {
          ss << "InputVariable '" << step.inputVariable().name() << "', which expects ";
        }
        ss << "a file of type '*." << inputFileType->valueDescription() << "', but the ";
        ss << "available file would be of type '*." << currentType->valueDescription() << "'.";
        LOG(Info,ss.str());
        return false;
      }

      if (outputFileType) {
        if (isEnergyModelType(*outputFileType)) {
          currentType = outputFileType;
          allTypes.insert(*currentType);
        }
        else {
          allTypes.insert(*outputFileType);
        }
      }

    }

    return true;
  }

  bool Problem_Impl::isEnergyModelType(const FileReferenceType& fileType) const {
    if (fileType == FileReferenceType::OSM) {
      return true;
    }
    if (fileType == FileReferenceType::IDF) {
      return true;
    }
    return false;
  }

  bool Problem_Impl::areInCompoundMeasure(const WorkflowStep& step,
                                          const boost::optional<WorkflowStep>& nextStep) const
  {
    // right now, only kind is chain of RubyContinuousVariables with same underlying
    // RubyPerturbation
    if (nextStep) {
      if (step.isInputVariable() && nextStep->isInputVariable()) {
        OptionalRubyContinuousVariable rcv, nrcv;
        rcv = step.inputVariable().optionalCast<RubyContinuousVariable>();
        nrcv = nextStep->inputVariable().optionalCast<RubyContinuousVariable>();
        if (rcv && nrcv) {
          if (rcv->perturbation() == nrcv->perturbation()) {
            return true;
          }
        }
      }
    }
    return false;
  }

  bool Problem_Impl::updateMeasureForCompoundRubyPerturbation(
      const BCLMeasure& newVersion,
      const std::vector<ruleset::OSArgument>& newArguments,
      bool keepOldArgumentsIfNewEmpty,
      RubyPerturbation& perturbation,
      std::vector<RubyContinuousVariable>& variables)
  {
    bool ok(true);
    BoolVector keep(variables.size(),true);
    if (newArguments.empty() && keepOldArgumentsIfNewEmpty) {
      // no argument changes needed
      OSArgumentVector currentArguments = perturbation.arguments();
      ok = perturbation.setMeasure(newVersion);
      if (ok) {
        perturbation.setArguments(currentArguments);
      }
    }
    else {
      // trim newArguments down to those not called out by variables
      OSArgumentVector argsSubsetForPerturbation = newArguments;
      int index(0);
      BOOST_FOREACH(const RubyContinuousVariable& var,variables) {
        NameFinder<OSArgument> finder(var.argument().name(),true);
        OSArgumentVector::iterator it = std::find_if(argsSubsetForPerturbation.begin(),
                                                     argsSubsetForPerturbation.end(),
                                                     finder);
        if (it != argsSubsetForPerturbation.end()) {
          argsSubsetForPerturbation.erase(it);
        }
        else {
          keep[index] = false;
        }
        ++index;
      }

      // try to update perturbation
      ok = perturbation.updateMeasure(newVersion,argsSubsetForPerturbation);
      // ETH@20130212 - updateMeasure may not quite do correct check of file
      // types for compound Ruby variables, but getting this exactly right is low
      // priority right now.
    }
    if (!ok) {
      keep = BoolVector(variables.size(),false);
    }
    // try to get rid of non-keeper variables
    for (int i = 0, n = keep.size(); i < n; ++i) {
      if (!keep[i]) {
        ok = ok && erase(variables[i]);
      }
      // ETH@20130212 - Should also check/update keepers either here or above. Again,
      // not high priority right now, and should be fine in most situations.
    }
    return ok;
  }

} // detail

WorkflowStepJob::WorkflowStepJob(const runmanager::Job& t_job,
                                 const WorkflowStep& t_step)
  : job(t_job),
    step(t_step)
{}

WorkflowStepJob::WorkflowStepJob(const WorkflowStep& t_step)
  : step(t_step)
{}

WorkflowStepJob::WorkflowStepJob(const runmanager::Job& t_job,
                                 const WorkflowStep& t_step,
                                 const DiscretePerturbation& t_discretePerturbation)
  : job(t_job),
    step(t_step),
    discretePerturbation(t_discretePerturbation)
{}

WorkflowStepJob::WorkflowStepJob(const WorkflowStep& t_step,
                                 const DiscretePerturbation& t_discretePerturbation)
  : step(t_step),
    discretePerturbation(t_discretePerturbation)
{}

WorkflowStepJob::WorkflowStepJob(const runmanager::Job& t_job,
                                 const WorkflowStep &t_step,
                                 const QVariant& t_value)
  : job(t_job),
    step(t_step),
    value(t_value)
{}

WorkflowStepJob::WorkflowStepJob(const WorkflowStep &t_step,
                                 const QVariant& t_value)
  : step(t_step),
    value(t_value)
{}

Problem::Problem(const std::string& name,
                 const std::vector<WorkflowStep>& workflow)
  : AnalysisObject(boost::shared_ptr<detail::Problem_Impl>(
        new detail::Problem_Impl(name,workflow)))
{
  Problem copyOfThis(getImpl<detail::Problem_Impl>());
  BOOST_FOREACH(const WorkflowStep& step, workflow) {
    step.setParent(copyOfThis);
  }
}

Problem::Problem(const std::string& name,
                 const std::vector<WorkflowStep>& workflow,
                 const std::vector<Function>& responses)
  : AnalysisObject(boost::shared_ptr<detail::Problem_Impl>(
        new detail::Problem_Impl(name,workflow,responses)))
{
  Problem copyOfThis(getImpl<detail::Problem_Impl>());
  BOOST_FOREACH(const WorkflowStep& step, workflow) {
    step.setParent(copyOfThis);
  }
  BOOST_FOREACH(const Function& response,responses) {
    response.setParent(copyOfThis);
  }
}

Problem::Problem(const std::string& name,
                 const std::vector<Variable>& variables,
                 const std::vector<Function>& responses,
                 const runmanager::Workflow& simulationWorkflow)
  : AnalysisObject(boost::shared_ptr<detail::Problem_Impl>(
        new detail::Problem_Impl(name,variables,responses,simulationWorkflow)))
{
  Problem copyOfThis(getImpl<detail::Problem_Impl>());
  BOOST_FOREACH(const WorkflowStep& step, workflow()) {
    step.setParent(copyOfThis);
  }
  BOOST_FOREACH(const Function& response,responses) {
    response.setParent(copyOfThis);
  }
}

Problem::Problem(const std::string& name,
                 const std::vector<Variable>& variables,
                 const runmanager::Workflow& simulationWorkflow)
  : AnalysisObject(boost::shared_ptr<detail::Problem_Impl>(
        new detail::Problem_Impl(name,variables,simulationWorkflow)))
{
  Problem copyOfThis(getImpl<detail::Problem_Impl>());
  BOOST_FOREACH(const WorkflowStep& step, workflow()) {
    step.setParent(copyOfThis);
  }
}

Problem::Problem(const UUID& uuid,
                 const UUID& versionUUID,
                 const std::string& name,
                 const std::string& displayName,
                 const std::string& description,
                 const std::vector<WorkflowStep>& workflow,
                 const std::vector<Function>& responses)
  : AnalysisObject(boost::shared_ptr<detail::Problem_Impl>(
        new detail::Problem_Impl(uuid,
                                 versionUUID,
                                 name,
                                 displayName,
                                 description,
                                 workflow,
                                 responses)))
{
  Problem copyOfThis(getImpl<detail::Problem_Impl>());
  BOOST_FOREACH(const WorkflowStep& step, workflow) {
    step.setParent(copyOfThis);
  }
  BOOST_FOREACH(const Function& response,responses) {
    response.setParent(copyOfThis);
  }
}

std::vector<WorkflowStep> Problem::workflow() const {
  return getImpl<detail::Problem_Impl>()->workflow();
}

boost::optional<int> Problem::getWorkflowStepIndexByJobType(
    const runmanager::JobType& jobType) const
{
  return getImpl<detail::Problem_Impl>()->getWorkflowStepIndexByJobType(jobType);
}

int Problem::numWorkflowSteps() const {
  return getImpl<detail::Problem_Impl>()->numWorkflowSteps();
}

std::vector<InputVariable> Problem::variables() const {
  return getImpl<detail::Problem_Impl>()->variables();
}

InputVariable Problem::getVariable(int index) const {
  return getImpl<detail::Problem_Impl>()->getVariable(index);
}

boost::optional<int> Problem::getVariableIndexByUUID(const UUID& uuid) const {
  return getImpl<detail::Problem_Impl>()->getVariableIndexByUUID(uuid);
}

boost::optional<InputVariable> Problem::getVariableByUUID(const UUID& uuid) const {
  return getImpl<detail::Problem_Impl>()->getVariableByUUID(uuid);
}

int Problem::numVariables() const {
  return getImpl<detail::Problem_Impl>()->numVariables();
}

int Problem::numContinuousVariables() const {
  return getImpl<detail::Problem_Impl>()->numContinuousVariables();
}

int Problem::numDiscreteVariables() const {
  return getImpl<detail::Problem_Impl>()->numDiscreteVariables();
}

int Problem::numStaticTransformations() const {
  return getImpl<detail::Problem_Impl>()->numStaticTransformations();
}

bool Problem::allVariablesAreContinuous() const {
  return getImpl<detail::Problem_Impl>()->allVariablesAreContinuous();
}

bool Problem::allVariablesAreContinuousOrStaticTransformations() const {
  return getImpl<detail::Problem_Impl>()->allVariablesAreContinuousOrStaticTransformations();
}

bool Problem::allVariablesAreDiscrete() const {
  return getImpl<detail::Problem_Impl>()->allVariablesAreDiscrete();
}

std::vector<Function> Problem::responses() const {
  return getImpl<detail::Problem_Impl>()->responses();
}

boost::optional<Function> Problem::getResponseByUUID(const UUID& uuid) const {
  return getImpl<detail::Problem_Impl>()->getResponseByUUID(uuid);
}

int Problem::numResponses() const {
  return getImpl<detail::Problem_Impl>()->numResponses();
}

boost::optional<FileReferenceType> Problem::inputFileType() const {
  return getImpl<detail::Problem_Impl>()->inputFileType();
}

bool Problem::isValid(const DataPoint& dataPoint) const {
  return getImpl<detail::Problem_Impl>()->isValid(dataPoint);
}

bool Problem::fileTypesAreCompatible(
    const WorkflowStep& childStep,
    const boost::optional<FileReferenceType>& proposedInputFileType,
    const boost::optional<FileReferenceType>& proposedOutputFileType) const
{
  return getImpl<detail::Problem_Impl>()->fileTypesAreCompatible(childStep,
                                                                 proposedInputFileType,
                                                                 proposedOutputFileType);
}

bool Problem::fileTypesAreCompatible(
    const InputVariable& childVariable,
    const boost::optional<FileReferenceType>& proposedInputFileType,
    const boost::optional<FileReferenceType>& proposedOutputFileType) const
{
  return getImpl<detail::Problem_Impl>()->fileTypesAreCompatible(childVariable,
                                                                 proposedInputFileType,
                                                                 proposedOutputFileType);
}

std::vector<QVariant> Problem::getVariableValues(
    const std::vector<DiscretePerturbation>& perturbations) const
{
  return getImpl<detail::Problem_Impl>()->getVariableValues(perturbations);
}

std::vector<QVariant> Problem::getVariableValues(
    const std::vector< boost::optional<DiscretePerturbation> >& perturbations) const
{
  return getImpl<detail::Problem_Impl>()->getVariableValues(perturbations);
}

std::vector<boost::optional<DiscretePerturbation> > Problem::getDiscretePerturbations(
    const std::vector<QVariant>& variableValues) const
{
  return getImpl<detail::Problem_Impl>()->getDiscretePerturbations(variableValues);
}

boost::optional<int> Problem::combinatorialSize(bool selectedPerturbationsOnly) const {
  return getImpl<detail::Problem_Impl>()->combinatorialSize(selectedPerturbationsOnly);
}

bool Problem::push(const WorkflowStep& step) {
  return getImpl<detail::Problem_Impl>()->push(step);
}

bool Problem::insert(int index, const WorkflowStep& step) {
  return getImpl<detail::Problem_Impl>()->insert(index,step);
}

bool Problem::erase(const WorkflowStep& step) {
  return getImpl<detail::Problem_Impl>()->erase(step);
}

bool Problem::swap(const WorkflowStep& step1, const WorkflowStep& step2) {
  return getImpl<detail::Problem_Impl>()->swap(step1, step2);
}

bool Problem::setWorkflow(const std::vector<WorkflowStep>& workflow) {
  return getImpl<detail::Problem_Impl>()->setWorkflow(workflow);
}

void Problem::clearWorkflow() {
  getImpl<detail::Problem_Impl>()->clearWorkflow();
}

void Problem::pushResponse(const Function& response) {
  return getImpl<detail::Problem_Impl>()->pushResponse(response);
}

bool Problem::insertResponse(int index, const Function& response) {
  return getImpl<detail::Problem_Impl>()->insertResponse(index,response);
}

bool Problem::eraseResponse(const Function& response) {
  return getImpl<detail::Problem_Impl>()->eraseResponse(response);
}

bool Problem::swapResponses(const Function& response1,const Function& response2) {
  return getImpl<detail::Problem_Impl>()->swapResponses(response1,response2);
}

void Problem::setResponseFunctions(const std::vector<Function>& responses) {
  getImpl<detail::Problem_Impl>()->setResponseFunctions(responses);
}

void Problem::clearResponseFunctions() {
  getImpl<detail::Problem_Impl>()->clearResponseFunctions();
}

bool Problem::updateMeasure(const BCLMeasure& newVersion,
                            const std::vector<ruleset::OSArgument>& newArguments,
                            bool keepOldArgumentsIfNewEmpty)
{
  return getImpl<detail::Problem_Impl>()->updateMeasure(newVersion,
                                                        newArguments,
                                                        keepOldArgumentsIfNewEmpty);
}

std::string Problem::dakotaInFileDescription(const DakotaAlgorithm& algorithm) const {
  return getImpl<detail::Problem_Impl>()->dakotaInFileDescription(algorithm);
}

boost::optional<DataPoint> Problem::createDataPoint(
    const std::vector<QVariant>& variableValues) const
{
  return getImpl<detail::Problem_Impl>()->createDataPoint(variableValues);
}

boost::optional<DataPoint> Problem::createDataPoint(
    const std::vector<DiscretePerturbation>& perturbations) const
{
  return getImpl<detail::Problem_Impl>()->createDataPoint(perturbations);
}

boost::optional<DataPoint> Problem::createDataPoint(const DakotaParametersFile& params,
                                                    const DakotaAlgorithm& algorithm) const
{
  return getImpl<detail::Problem_Impl>()->createDataPoint(params,algorithm);
}

runmanager::Workflow Problem::createWorkflow(
    const DataPoint& dataPoint,
    const openstudio::path& rubyIncludeDirectory) const
{
  return getImpl<detail::Problem_Impl>()->createWorkflow(dataPoint,rubyIncludeDirectory);
}

void Problem::updateDataPoint(DataPoint& dataPoint,
                              const runmanager::Job& completedJob) const
{
  getImpl<detail::Problem_Impl>()->updateDataPoint(dataPoint,completedJob);
}

std::vector<WorkflowStepJob> Problem::getJobsByWorkflowStep(const DataPoint& dataPoint,
                                                            bool optimize) const
{
  return getImpl<detail::Problem_Impl>()->getJobsByWorkflowStep(dataPoint,optimize);
}

boost::optional<std::string> Problem::getDakotaResultsFile(const DataPoint& dataPoint) const {
  return getImpl<detail::Problem_Impl>()->getDakotaResultsFile(dataPoint);
}

std::vector<int> Problem::getContinuousDesignVariableIndices(const DakotaAlgorithm& dakotaAlgorithm) const
{
  return getImpl<detail::Problem_Impl>()->getContinuousDesignVariableIndices(dakotaAlgorithm);
}

std::vector<int> Problem::getDiscreteDesignVariableIndices(const DakotaAlgorithm& dakotaAlgorithm) const {
  return getImpl<detail::Problem_Impl>()->getDiscreteDesignVariableIndices(dakotaAlgorithm);
}

std::map<UncertaintyDescriptionType,std::vector<int> > Problem::getUncertainVariableIndices(
  const DakotaAlgorithm& dakotaAlgorithm) const
{
  return getImpl<detail::Problem_Impl>()->getUncertainVariableIndices(dakotaAlgorithm);
}

/// @cond
Problem::Problem(boost::shared_ptr<detail::Problem_Impl> impl)
  : AnalysisObject(impl)
{}
/// @endcond

} // analysis
} // openstudio

