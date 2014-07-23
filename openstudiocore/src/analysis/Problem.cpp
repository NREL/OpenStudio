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

#include "Problem.hpp"
#include "Problem_Impl.hpp"

#include "Analysis.hpp"
#include "Analysis_Impl.hpp"
#include "ContinuousVariable.hpp"
#include "ContinuousVariable_Impl.hpp"
#include "DakotaAlgorithm.hpp"
#include "DakotaParametersFile.hpp"
#include "DataPoint.hpp"
#include "DiscreteVariable.hpp"
#include "DiscreteVariable_Impl.hpp"
#include "Measure.hpp"
#include "MeasureGroup.hpp"
#include "MeasureGroup_Impl.hpp"
#include "Function_Impl.hpp"
#include "GenericUncertaintyDescription.hpp"
#include "NullMeasure.hpp"
#include "NullMeasure_Impl.hpp"
#include "OptimizationProblem.hpp"
#include "OptimizationProblem_Impl.hpp"
#include "RubyContinuousVariable.hpp"
#include "RubyContinuousVariable_Impl.hpp"
#include "RubyMeasure.hpp"
#include "RubyMeasure_Impl.hpp"
#include "UncertaintyDescription.hpp"
#include "UncertaintyDescription_Impl.hpp"
#include "WorkflowStep_Impl.hpp"

#include "../runmanager/lib/WorkItem.hpp"
#include "../runmanager/lib/Workflow.hpp"
#include "../runmanager/lib/MergedJobResults.hpp"
#include "../runmanager/lib/RubyJobUtils.hpp"

#include "../ruleset/OSArgument.hpp"

#include "../utilities/idf/URLSearchPath.hpp"

#include "../utilities/data/Attribute.hpp"

#include "../utilities/bcl/BCLMeasure.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/core/Compare.hpp"
#include "../utilities/core/Containers.hpp"
#include "../utilities/core/FileReference.hpp"
#include "../utilities/core/Finder.hpp"
#include "../utilities/core/Json.hpp"
#include "../utilities/core/Optional.hpp"
#include "../utilities/core/URLHelpers.hpp"

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
    for (WorkflowStep& step : m_workflow) {
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
    for (WorkflowStep& step : m_workflow) {
      step.onChange();
      connectChild(step,false);
    }
    for (Function& response : m_responses) {
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
    for (WorkflowStep& step : m_workflow) {
      step.onChange();
      connectChild(step,false);
      if (step.isInputVariable()) {
        OS_ASSERT(step.inputVariable().parent());
        OS_ASSERT(step.inputVariable().parent().get() == step);
      }
    }
    for (Function& response : m_responses) {
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
    for (WorkflowStep& step : m_workflow) {
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
    for (WorkflowStep& step : m_workflow) {
      connectChild(step,false);
    }
    for (Function& response : m_responses) {
      connectChild(response,false);
    }
  }

  Problem_Impl::Problem_Impl(const Problem_Impl &other)
    : AnalysisObject_Impl(other)
  {
    for (const WorkflowStep& step : other.workflow()) {
      m_workflow.push_back(step.clone().cast<WorkflowStep>());
      connectChild(m_workflow.back(),false);
    }
    for (const Function& response : other.responses()) {
      m_responses.push_back(response.clone().cast<Function>());
      connectChild(m_responses.back(),false);
    }
  }

  AnalysisObject Problem_Impl::clone() const {
    std::shared_ptr<Problem_Impl> impl(new Problem_Impl(*this));
    Problem result(impl);
    WorkflowStepVector workflow = result.workflow();
    for (WorkflowStep& step : workflow) {
      step.setParent(result);
    }
    FunctionVector responses = result.responses();
    for (Function& response : responses) {
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
    for (const WorkflowStep& step : workflow()) {
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
    for (const WorkflowStep& step : workflow()) {
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
    for (const WorkflowStep& step : workflow()) {
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
    for (const WorkflowStep& step : workflow()) {
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
    for (const DiscreteVariable& dvar : dvars) {
      if (dvar.numValidValues(true) < 2) {
        ++result;
      }
    }

    return result;
  }

  bool Problem_Impl::allVariablesAreContinuous() const {
    return !allVariablesAreDiscrete();
  }

  bool Problem_Impl::allVariablesAreContinuousOrStaticTransformations() const {
    for (const InputVariable& variable : variables()) {
      if (OptionalDiscreteVariable oDiscrete = variable.optionalCast<DiscreteVariable>()) {
        if (oDiscrete->numValidValues(true) > 1) {
          return false;
        }
      }
    }
    return true;
  }

  bool Problem_Impl::allVariablesAreDiscrete() const {
    for (const InputVariable& variable : variables()) {
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
    for (const Function& response : responses()) {
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
    for (const WorkflowStep& step : workflow()) {
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
    for (const InputVariable& myVariable : myVariables) {
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
    bool foundChild(false), inCompoundMeasure(false);
    for (unsigned i = 0, n = workflow.size(); i < n; ++i) {
      // if true, previous and this are in a compound measure together
      // -------- ?
      //  p   c | n
      // --------
      bool prevInCompoundMeasure = inCompoundMeasure;
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
      const std::vector<Measure>& measures) const
  {
    return getVariableValues(std::vector<OptionalMeasure>(
        measures.begin(),measures.end()));
  }

  std::vector<QVariant> Problem_Impl::getVariableValues(
    const std::vector< boost::optional<Measure> >& measures) const
  {
    std::vector<QVariant> result, intermediate;

    int n = numVariables();
    int m = measures.size();
    if (m > n) {
      LOG(Warn,"Cannot determine variable values for Problem '" << name() <<
          "' from measure vector of length " << measures.size() <<
          ", because this problem has " << n << " variables.");
      return result;
    }

    InputVariableVector variables = this->variables();
    for (int i = 0; i < n && i < m; ++i) {
      OptionalMeasureGroup measureGroup = variables[i].optionalCast<MeasureGroup>();
      if (measureGroup) {
        if (!measures[i]) {
          intermediate.push_back(QVariant(QVariant::Int));
          continue;
        }
        OptionalInt measureIndex = measureGroup->getIndexByUUID(measures[i].get());
        if (!measureIndex) {
          LOG(Warn,"Cannot determine variable values for Problem '" << name()
              << "' from the provided measure vector, because the measure at index " <<
              i << " is not in the variable at index " << i << " (getMeasureByUUID).");
          return result;
        }
        intermediate.push_back(QVariant(*measureIndex));
        continue;
      }
      // OptionalContinuousVariable continuousVariable = variables[i].optionalCast<ContinuousVariable>();
      // OS_ASSERT(continuousVariable);
      intermediate.push_back(QVariant(QVariant::Double));
    }
    result = intermediate;
    return result;
  }

  std::vector<boost::optional<Measure> > Problem_Impl::getMeasures(
      const std::vector<QVariant>& variableValues) const
  {
    std::vector<OptionalMeasure> result, intermediate;

    int n = numVariables();
    int m = variableValues.size();
    if (m > n) {
      LOG(Warn,"Cannot determine discrete measures for Problem '" << name() <<
          "' from variable values vector of length " << m <<
          ", because this problem has " << n << " variables.");
      return result;
    }

    InputVariableVector variables = this->variables();
    for (int i = 0; i < n && i < m; ++i) {
      OptionalMeasureGroup measureGroup = variables[i].optionalCast<MeasureGroup>();
      if (measureGroup) {
        QVariant value = variableValues[i];
        if (value.isNull()) {
          intermediate.push_back(boost::none);
          continue;
        }
        if (!measureGroup->isValid(value)) {
          LOG(Warn,"Cannot determine discrete measures for Problem '" << name() <<
          "', because the value given for variable " << i << ", '" << measureGroup->name()
          << "', is non-null and invalid.");
          return result;
        }
        intermediate.push_back(measureGroup->getMeasure(value.toInt()));
        continue;
      }
      intermediate.push_back(boost::none);
    }

    result = intermediate;
    return result;
  }

  boost::optional<int> Problem_Impl::combinatorialSize(bool selectedOnly) const {
    OptionalInt result;
    int intermediate(0);
    InputVariableVector variables = this->variables();
    for (const InputVariable& variable : variables) {
      OptionalDiscreteVariable discreteVariable = variable.optionalCast<DiscreteVariable>();
      if (!discreteVariable) {
        return result;
      }
      int thisVariableSize = discreteVariable->numValidValues(selectedOnly);
      if (thisVariableSize == 0) {
        LOG(Warn,"Problem '" << name() << "' contains a DiscreteVariable '" << variable.name() <<
            "' with either no measures, or no selected measures. In either case, " <<
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
    auto it = candidates.begin();
    for (int count = 0; count < index; ++count, ++it);
    candidates.insert(it,step);
    if (!checkWorkflow(candidates)) {
      return false;
    }

    m_workflow = candidates;
    // mark variables with changed indices as dirty
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
    auto it = std::find_if(
          candidates.begin(),
          candidates.end(),
          std::bind(uuidsEqual<WorkflowStep,WorkflowStep>,std::placeholders::_1,step));
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

    auto it1 = std::find_if(
          candidates.begin(),
          candidates.end(),
          std::bind(uuidsEqual<WorkflowStep,WorkflowStep>,std::placeholders::_1,step1));
    auto it2 = std::find_if(
          candidates.begin(),
          candidates.end(),
          std::bind(uuidsEqual<WorkflowStep,WorkflowStep>,std::placeholders::_1,step2));
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
    for (WorkflowStep& step : m_workflow) {
      disconnectChild(step);
    }
    m_workflow = workflow;
    for (WorkflowStep& step : m_workflow) {
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
    for (WorkflowStep& step : m_workflow) {
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
    auto it = m_responses.begin();
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
    auto it = std::find_if(
          m_responses.begin(),
          m_responses.end(),
          std::bind(uuidsEqual<Function,Function>,std::placeholders::_1,response));
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
    auto it1 = std::find_if(
          m_responses.begin(),
          m_responses.end(),
          std::bind(uuidsEqual<Function,Function>,std::placeholders::_1,response1));
    auto it2 = std::find_if(
          m_responses.begin(),
          m_responses.end(),
          std::bind(uuidsEqual<Function,Function>,std::placeholders::_1,response2));
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
    for (Function& response : m_responses) {
      disconnectChild(response);
    }
    m_responses = responses;
    for (Function& response : m_responses) {
      response.onChange();
      connectChild(response,true);
    }
    onChange(AnalysisObject_Impl::InvalidatesResults);
  }

  void Problem_Impl::clearResponseFunctions() {
    for (Function& response : m_responses) {
      disconnectChild(response);
    }
    m_responses.clear();
    onChange(AnalysisObject_Impl::InvalidatesResults);
  }

  bool Problem_Impl::updateMeasure(const BCLMeasure& newVersion,
                                   const std::vector<ruleset::OSArgument>& newArguments,
                                   bool keepOldArgumentsIfNewEmpty)
  {
    bool result = true;
    UUID measureUUID = newVersion.uuid();
    WorkflowStepVector steps = workflow();
    OptionalRubyMeasure compoundRubyMeasure;
    std::vector<RubyContinuousVariable> compoundVariables;
    for (WorkflowStep& step : steps) {
      // see if should clear compound variable
      if (compoundRubyMeasure) {
        // see if should clear
        bool clearCompound = true;
        if (step.isInputVariable() && step.inputVariable().optionalCast<RubyContinuousVariable>()) {
          RubyContinuousVariable rcv = step.inputVariable().cast<RubyContinuousVariable>();
          if (rcv.measure() == compoundRubyMeasure.get()) {
            clearCompound = false;
          }
        }
        if (clearCompound) {
          result = result && updateMeasureForCompoundRubyMeasure(
                newVersion,
                newArguments,
                keepOldArgumentsIfNewEmpty,
                *compoundRubyMeasure,
                compoundVariables);
          compoundRubyMeasure.reset();
          compoundVariables.clear();
        }
      }

      if (step.isInputVariable()) {
        InputVariable variable = step.inputVariable();

        if (compoundRubyMeasure) {
          compoundVariables.push_back(variable.cast<RubyContinuousVariable>());
        }
        else if (OptionalMeasureGroup mg = variable.optionalCast<MeasureGroup>()) {
          MeasureVector dps = mg->measures(false);
          for (Measure& dp : dps) {
            if (OptionalRubyMeasure rm = dp.optionalCast<RubyMeasure>()) {
              if (rm->usesBCLMeasure() && (rm->measureUUID() == measureUUID)) {
                bool ok(true);
                if (newArguments.empty() && keepOldArgumentsIfNewEmpty) {
                  OSArgumentVector currentArguments = rm->arguments();
                  ok = rm->setMeasure(newVersion);
                  if (ok) {
                    rm->setArguments(currentArguments);
                  }
                }
                else {
                  ok = rm->updateMeasure(newVersion,newArguments);
                }
                if (!ok) {
                  // bad match between file types
                  ok = mg->erase(*rm);
                  if (!ok) {
                    result = false;
                  }
                }
              }
            }
          }
        }
        else if (OptionalRubyContinuousVariable rcv = variable.optionalCast<RubyContinuousVariable>()) {
          if (rcv->measure().usesBCLMeasure() &&
              (rcv->measure().measureUUID() == measureUUID))
          {
            compoundRubyMeasure = rcv->measure();
            compoundVariables.push_back(*rcv);
          }
        }
      }
      else {
        runmanager::WorkItem workItem = step.workItem();
        if (workItem.type == runmanager::JobType::UserScript) {
          // compare BCLMeasure uuids
          try {
            runmanager::RubyJobBuilder rjb(workItem);
            if (rjb.bclMeasureUUID() && (rjb.bclMeasureUUID().get() == measureUUID)) {
              // update this WorkItem if arguments may have changed
              bool updateWorkItem = false;
              if (newArguments.empty()) {
                if (!keepOldArgumentsIfNewEmpty) {
                  // clear WorkItem arguments if it has any
                  if (!runmanager::RubyJobBuilder::toOSArguments(workItem.params).empty()) {
                    updateWorkItem = true;
                  }
                }
              }
              else {
                // go ahead and do the swap no matter what
                updateWorkItem = true;
              }
              if (updateWorkItem) {
                rjb = runmanager::RubyJobBuilder(newVersion,newArguments);
                runmanager::WorkItem newWorkItem = rjb.toWorkItem();
                step.set(newWorkItem);
              }
            }
          }
          catch (...) {}
        }
      }
    }

    if (compoundRubyMeasure) {
      result = result && updateMeasureForCompoundRubyMeasure(
            newVersion,
            newArguments,
            keepOldArgumentsIfNewEmpty,
            *compoundRubyMeasure,
            compoundVariables);
      compoundRubyMeasure.reset();
      compoundVariables.clear();
    }

    return result;
  }

  std::string Problem_Impl::dakotaInFileDescription(const DakotaAlgorithm& algorithm) const {
    ContinuousVariableVector continuousVariables = subsetCastVector<ContinuousVariable>(variables());
    DiscreteVariableVector discreteVariables = subsetCastVector<DiscreteVariable>(variables());

    // screen discrete variables with only one option (treat as static transformations, not variables)
    auto it = discreteVariables.begin();
    while (it != discreteVariables.end()) {
      if (it->numValidValues(true) < 2) {
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
          ss << " " << double(dvar.numValidValues(true)) - 0.5;
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
        ss << " " << dvar.numValidValues(true);
      }
      ss << std::endl << "          set_values" << std::endl;
      for (int i = 0; i < n_ddvs; ++i) {
        DiscreteVariable dvar = vars[ddvs[i]].cast<DiscreteVariable>();
        ss << "           ";
        for (int j : dvar.validValues(true)) {
          ss << " " << j;
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
        for (const OptionalAttribute& value : values1) {
          if (value){
            ss << " " << value->toString();
          }
        }
        ss << std::endl << "          " << string2 << std::endl << "           ";
        for (const OptionalAttribute& value : values2) {
          if (value){
            ss << " " << value->toString();
          }
        }
        ss << std::endl;
        if (anyLower) {
          ss << "          lower_bounds" << std::endl << "           ";
          for (const OptionalAttribute& value : lowerBounds) {
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
          for (const OptionalAttribute& value : upperBounds) {
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
        for (const OptionalAttribute& value : abscissas) {
          std::vector<double> attVector = getDoubleVectorFromAttribute(value.get());
          for (const auto & att : attVector) {
            ss << att << " ";
          }
        }
        ss << std::endl << "          " << string1 << std::endl << "           ";
        for (const OptionalAttribute& value : values1) {
          std::vector<double> attVector = getDoubleVectorFromAttribute(value.get());
          for (const auto & att : attVector) {
            ss << att << " ";
          }
        }
        ss << std::endl;
        if (!numPairs.empty()) {
           ss << "          num_pairs" << std::endl << "           ";
          for (const OptionalAttribute& value : numPairs) {
            if (value){
              ss << " " << value->toString();
            }
          }
          ss << std::endl;
        }
      }
      else {
        for (const AttributeDescription& attDesc : udescs[0].attributeDescriptions()) {
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
            for (const OptionalAttribute& value : values) {
              if (value) {
                if (value.get().valueType() == AttributeValueType::AttributeVector) {
                  ss << " ";
                  std::vector<double> attVector = getDoubleVectorFromAttribute(value.get());
                  for (const auto & att : attVector) {
                    ss << att << " ";
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
      const std::vector<Measure>& measures) const
  {
    OptionalDataPoint result;
    DataPoint candidate(getPublicObject<Problem>(),getVariableValues(measures));
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
          int value(0);
          if (cvValues[i] > 0.0) {
            value = std::floor(cvValues[i] + 0.5 - std::numeric_limits<double>::epsilon());
          }
          if (!dvar->isValid(QVariant(value))) {
            LOG(Error,"Discrete variable represented as " << cvValues[i]
                << " rounded to invalid value " << value << ".");
            return result;
          }
          variableValues[cdvs[i]] = QVariant(value);
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

    // put in values for skipped (0-1 selected measures) discrete variables
    for (int i = 0; i < n; ++i) {
      if (variableValues[i].isNull()) {
        // should be skipped discrete variable
        if (OptionalDiscreteVariable dvar = vars[i].optionalCast<DiscreteVariable>()) {
          int np = dvar->numValidValues(true);
          if (np < 2) {
            // 0-1 measure discrete variables are screened from DAKOTA
            if (np == 1) {
              variableValues[i] = QVariant(dvar->validValues(true)[0]);
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
    OptionalRubyMeasure compoundRubyMeasure;
    OptionalRubyMeasure originalCompoundRubyMeasure;
    unsigned i = 0;
    for (const WorkflowStep& step : workflow()) {

      if (compoundRubyMeasure) {

        if (step.isInputVariable()) {
          if (OptionalRubyContinuousVariable rcv = step.inputVariable().optionalCast<RubyContinuousVariable>()) {
            if (rcv->measure() == *originalCompoundRubyMeasure) {
              // add argument to existing perturbation
              OSArgument arg = rcv->argument();
              arg.setValue(values[i].toDouble());
              compoundRubyMeasure->addArgument(arg);
              ++i;
              continue;
            }
          }
        }

        // instantiate the compoundRubyMeasure and then deal with the next item
        result.push_back(compoundRubyMeasure->createWorkItem(rubyIncludeDirectory));
        compoundRubyMeasure.reset();
        originalCompoundRubyMeasure.reset();
      }

      if (step.isWorkItem()) {
        runmanager::WorkItem workItem = step.workItem();
        if (workItem.type == runmanager::JobType::UserScript
            || workItem.type == runmanager::JobType::Ruby) {
          runmanager::RubyJobBuilder rjb(workItem);
          rjb.setIncludeDir(rubyIncludeDirectory);
          workItem = rjb.toWorkItem();
        }
        result.push_back(workItem);
      }
      else {
        InputVariable variable = step.inputVariable();
        if (OptionalRubyContinuousVariable rcv = variable.optionalCast<RubyContinuousVariable>()) {
          // start a compound measure
          originalCompoundRubyMeasure = rcv->measure();
          compoundRubyMeasure = originalCompoundRubyMeasure->clone().cast<RubyMeasure>();
          OSArgument arg = rcv->argument();
          arg.setValue(values[i].toDouble());
          compoundRubyMeasure->addArgument(arg);
        }
        else {
          result.push_back(variable.createWorkItem(values[i],rubyIncludeDirectory));
        }
        ++i;
      }
    }

    if (compoundRubyMeasure) {
      result.push_back(compoundRubyMeasure->createWorkItem(rubyIncludeDirectory));
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

    for (const Err& error : jobErrors.allErrors) {
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
      FileReferenceVector xmlOutputData;
      for (const runmanager::FileInfo& file : allFiles.getAllByExtension("ossr").files()) {
        xmlOutputData.push_back(FileReference(file.fullPath));
      }
      for (const runmanager::FileInfo& file : allFiles.getAllByExtension("xml").files()) {
        xmlOutputData.push_back(FileReference(file.fullPath));
      }
      dataPoint.setXmlOutputData(xmlOutputData);
    }
    catch (...) {}

    // load output attributes from file (so don't pay this cost at an inconvenient time)
    dataPoint.outputAttributes();

    // Determine response function values
    try {
      DoubleVector responseValues;
      for (const Function& response : responses()) {
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
      unsigned numMergedJobs(0), mergedJobIndex(0);
      if (currentJob->hasMergedJobs()) {
        numMergedJobs = currentJob->mergedJobResults().size();
      }
      for (WorkflowStepVector::const_iterator it = workflow.begin(), itEnd = workflow.end();
           it != itEnd; ++it)
      {
        WorkflowStep currentStep = *it;
        if (currentStep.isInputVariable()) {
          InputVariable var = currentStep.inputVariable();
          // compound measure?
          OptionalWorkflowStep nextStep;
          auto jit = it; ++jit;
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
              // must be null measure
              OS_ASSERT(var.optionalCast<MeasureGroup>());
              MeasureGroup mg = var.cast<MeasureGroup>();
              Measure measure = mg.getMeasure(dataPoint);
              OS_ASSERT(measure.optionalCast<NullMeasure>());
              if (!optimize) {
                // include in results
                if (currentJob) {
                  result.push_back(WorkflowStepJob(*currentJob,currentStep,measure));
                }
                else {
                  result.push_back(WorkflowStepJob(currentStep,measure));
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
                  if (currentJob->hasMergedJobs()) {
                    numMergedJobs = currentJob->mergedJobResults().size();
                  }
                  else {
                    numMergedJobs = 0;
                  }
                  mergedJobIndex = 0;
                }
              }
            }
            else {
              // non-null job
              bool getNextJob(true);
              if (OptionalMeasureGroup dvar = var.optionalCast<MeasureGroup>()) {
                Measure measure = dvar->getMeasure(dataPoint);
                if (OptionalNullMeasure nmeasure = measure.optionalCast<NullMeasure>()) {
                  // null measure, non-null job, do not correspond
                  // only include if !optimize
                  if (!optimize) {
                    result.push_back(WorkflowStepJob(currentStep,measure));
                  }
                  getNextJob = false;
                }
                else {
                  if (numMergedJobs > 0u) {
                    result.push_back(WorkflowStepJob(*currentJob,currentStep,measure,mergedJobIndex));
                    ++mergedJobIndex;
                  }
                  else {
                    result.push_back(WorkflowStepJob(*currentJob,currentStep,measure));
                  }
                }
              }
              else {
                if (numMergedJobs > 0u) {
                  result.push_back(WorkflowStepJob(*currentJob,currentStep,QVariant(var.getValue(dataPoint)),mergedJobIndex));
                  ++mergedJobIndex;
                }
                else {
                  result.push_back(WorkflowStepJob(*currentJob,currentStep,QVariant(var.getValue(dataPoint))));
                }
              }

              if (getNextJob) {
                if ((numMergedJobs == 0) || (mergedJobIndex == numMergedJobs)) {
                  std::vector<runmanager::Job> childJobs = currentJob->children();
                  if (childJobs.empty()) {
                    currentJob.reset();
                  }
                  else {
                    OS_ASSERT(childJobs.size() == 1u);
                    currentJob = childJobs[0];
                    if (currentJob->hasMergedJobs()) {
                      numMergedJobs = currentJob->mergedJobResults().size();
                    }
                    else {
                      numMergedJobs = 0;
                    }
                    mergedJobIndex = 0;
                  }
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
            if (numMergedJobs > 0u) {
              result.push_back(WorkflowStepJob(*currentJob,currentStep,OptionalUnsigned(mergedJobIndex)));
              ++mergedJobIndex;
            }
            else {
              result.push_back(WorkflowStepJob(*currentJob,currentStep));
            }
          }

          if (getNextJob) {
            if ((numMergedJobs == 0) || (mergedJobIndex == numMergedJobs)) {
              std::vector<runmanager::Job> childJobs = currentJob->children();
              if (childJobs.empty()) {
                currentJob.reset();
              }
              else {
                OS_ASSERT(childJobs.size() == 1u);
                currentJob = childJobs[0];
                if (currentJob->hasMergedJobs()) {
                  numMergedJobs = currentJob->mergedJobResults().size();
                }
                else {
                  numMergedJobs = 0;
                }
                mergedJobIndex = 0;
              }
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
      for (double value : dataPoint.responseValues()) {
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
        int np = dv->numValidValues(true);
        if (np > 1 && dakotaAlgorithm.requiresContinuousVariables()) {
          // 0-1 measure discrete variables are screened from DAKOTA
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
          auto it = result.find(udesc->type());
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

  QVariant Problem_Impl::toVariant() const {
    QVariantMap problemData = AnalysisObject_Impl::toVariant().toMap();

    problemData["problem_type"] = QString("Problem");

    // basic variables for creating workflow list
    QVariantList workflowList;
    int index(0), variableIndex(0);

    // additional variables for inverting storage of compound ruby variables
    QVariantMap stepMap;
    OptionalRubyMeasure compoundRubyMeasure;
    QVariantList variablesList;

    // create workflow list
    for (const WorkflowStep& step : workflow()) {

      if (compoundRubyMeasure) {
        // see if chain is still going
        if (step.isInputVariable()) {
          if (OptionalRubyContinuousVariable rcv = step.inputVariable().optionalCast<RubyContinuousVariable>()) {
            if (rcv->measure().uuidAndVersionEqual(compoundRubyMeasure.get())) {
              // still going -- add this variable to list
              QVariantMap variableMap = step.toVariant().toMap();
              variableMap["variable_index"] = variableIndex;
              variablesList.push_back(variableMap);
              ++variableIndex;
              ++index;
              continue;
            }
          }
        }

        // otherwise, complete this step and move on
        stepMap["variables"] = QVariant(variablesList);
        workflowList.push_back(stepMap);
        variablesList.clear();
        variableIndex = 0;
        compoundRubyMeasure.reset();
      }

      if (step.isInputVariable() &&
          step.inputVariable().optionalCast<RubyContinuousVariable>())
      {
        RubyContinuousVariable rcv = step.inputVariable().cast<RubyContinuousVariable>();
        compoundRubyMeasure = rcv.measure();
        stepMap = compoundRubyMeasure->toVariant().toMap();
        stepMap["workflow_index"] = QVariant(index);
        stepMap["workflow_step_type"] = QString("Measure");
        QVariantMap variableMap = step.toVariant().toMap();
        variableMap["variable_index"] = variableIndex;
        variablesList.push_back(variableMap);
        ++variableIndex;
      }
      else {
        stepMap = step.toVariant().toMap();
        stepMap["workflow_index"] = QVariant(index);
        workflowList.push_back(stepMap);
      }

      ++index;
    }
    if (compoundRubyMeasure) {
      // save out last RubyMeasure step
      stepMap["variables"] = QVariant(variablesList);
      workflowList.push_back(stepMap);
      variablesList.clear();
      variableIndex = 0;
      compoundRubyMeasure.reset();
    }
    problemData["workflow"] = QVariant(workflowList);

    if (!responses().empty()) {
      QVariantList responsesList;
      index = 0;
      for (const Function& response : responses()) {
        QVariantMap responseMap = response.toVariant().toMap();
        responseMap["response_index"] = QVariant(index);
        responsesList.push_back(responseMap);
        ++index;
      }
      problemData["responses"] = QVariant(responsesList);
    }

    return QVariant(problemData);
  }

  Problem Problem_Impl::factoryFromVariant(const QVariant& variant, const VersionString& version) {
    QVariantMap map = variant.toMap();

    if (!map.contains("problem_type")) {
      LOG_AND_THROW("Unable to find Problem in expected location.");
    }

    std::string problemType = map["problem_type"].toString().toStdString();
    if (problemType == "Problem") {
      return Problem_Impl::fromVariant(variant,version);
    }
    if (problemType == "OptimizationProblem") {
      return OptimizationProblem_Impl::fromVariant(variant,version);
    }

    LOG_AND_THROW("Unexpected problem_type " << problemType << ".");
    return OptionalProblem().get();
  }

  Problem Problem_Impl::fromVariant(const QVariant& variant, const VersionString& version) {
    QVariantMap map = variant.toMap();

    std::vector<std::pair<int,WorkflowStep> > workflowIntermediate;
    for (const QVariant& workflowListItem : map["workflow"].toList()) {
      QVariantMap stepMap = workflowListItem.toMap();
      std::string workflowStepType = stepMap["workflow_step_type"].toString().toStdString();

      if (workflowStepType == "Measure") {
        // do the inversion for compound Ruby measures here
        Measure measure = detail::Measure_Impl::factoryFromVariant(workflowListItem,version);
        int index = stepMap["workflow_index"].toInt();
        InputVariableVector vars = deserializeOrderedVector(
              stepMap["variables"].toList(),
              "variable_index",
              std::function<InputVariable(const QVariant&)>(std::bind(static_cast<InputVariable(*)(const QVariant&, const Measure&, const VersionString&)>(&detail::InputVariable_Impl::factoryFromVariant), std::placeholders::_1, measure, version)));
        for (const InputVariable& var : vars) {
          workflowIntermediate.push_back(std::make_pair(index,WorkflowStep(var,boost::optional<runmanager::WorkItem>())));
          ++index;
        }
      }
      else {
        int index = stepMap["workflow_index"].toInt();
        WorkflowStep step = WorkflowStep_Impl::factoryFromVariant(workflowListItem,version);
        workflowIntermediate.push_back(std::make_pair(index,step));
      }

    }

    std::sort(workflowIntermediate.begin(),
              workflowIntermediate.end(),
              FirstOfPairLess<std::pair<int,WorkflowStep> >());
    WorkflowStepVector workflow;
    std::transform(workflowIntermediate.begin(),
                   workflowIntermediate.end(),
                   std::back_inserter(workflow),
                   GetSecondOfPair<int,WorkflowStep>());

    FunctionVector responses;
    if (map.contains("responses")) {
      responses = deserializeOrderedVector(
            map["responses"].toList(),
            "response_index",
            std::function<Function (const QVariant&)>(std::bind(analysis::detail::Function_Impl::factoryFromVariant,std::placeholders::_1,version)));
    }

    return Problem(toUUID(map["uuid"].toString().toStdString()),
                   toUUID(map["version_uuid"].toString().toStdString()),
                   map.contains("name") ? map["name"].toString().toStdString() : std::string(),
                   map.contains("display_name") ? map["display_name"].toString().toStdString() : std::string(),
                   map.contains("description") ? map["description"].toString().toStdString() : std::string(),
                   workflow,
                   responses);
  }

  void Problem_Impl::updateInputPathData(const openstudio::path& originalBase,
                                         const openstudio::path& newBase)
  {
    // workflow steps
    WorkflowStepVector steps = workflow();
    for (WorkflowStep& step : steps) {
      step.getImpl<detail::WorkflowStep_Impl>()->updateInputPathData(originalBase,newBase);
    }

    // responses
    //
    // currently does nothing, because the only variables that have path data are
    // input variables, and any of those used by a response should also be in workflow().
    //
    FunctionVector functions = responses();
    for (Function& func : functions) {
      func.getImpl<detail::Function_Impl>()->updateInputPathData(originalBase,newBase);
    }
  }

  QVariant Problem_Impl::toServerFormulationVariant() const {
    QVariantMap map;

    InputVariableVector vars = variables();
    unsigned mgCnt(0), rcvCnt(0);
    for (const auto & var : vars) {
      if (var.optionalCast<MeasureGroup>()) {
        ++mgCnt;
        continue;
      }
      if (var.optionalCast<RubyContinuousVariable>()) {
        ++rcvCnt;
      }
    }
    map["num_measure_groups"] = QVariant(mgCnt);
    map["num_ruby_continuous_variables"] = QVariant(rcvCnt);

    return QVariant(map);
  }

  std::vector<WorkflowStep> Problem_Impl::convertVariablesAndWorkflowToWorkflowSteps(
      const std::vector<Variable>& variables,
      const runmanager::Workflow& simulationWorkflow) const
  {
    WorkflowStepVector workflow;
    for (const Variable& variable : variables) {
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

    // DLM: add the unknown type here, note this is treating unknown more like none
    allTypes.insert(FileReferenceType::Unknown);

    if (OptionalAnalysisObject parent = this->parent()) {
      currentType = parent->cast<Analysis>().seed().fileType();
      allTypes.insert(*currentType);
    }

    OptionalRubyMeasure compoundRubyMeasure;
    for (const WorkflowStep& step : workflow) {

      if (compoundRubyMeasure) {
        // see if chain is still going
        if (step.isInputVariable()) {
          if (OptionalRubyContinuousVariable rcv = step.inputVariable().optionalCast<RubyContinuousVariable>()) {
            if (rcv->measure().uuidAndVersionEqual(compoundRubyMeasure.get())) {
              continue;
            }
          }
        }

        // otherwise, evaluate compound and evaluate current step on its own
        currentType = compoundRubyMeasure->outputFileType();
        allTypes.insert(*currentType);
        compoundRubyMeasure.reset();
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
    // RubyMeasure
    if (nextStep) {
      if (step.isInputVariable() && nextStep->isInputVariable()) {
        OptionalRubyContinuousVariable rcv, nrcv;
        rcv = step.inputVariable().optionalCast<RubyContinuousVariable>();
        nrcv = nextStep->inputVariable().optionalCast<RubyContinuousVariable>();
        if (rcv && nrcv) {
          if (rcv->measure().uuidAndVersionEqual(nrcv->measure())) {
            return true;
          }
        }
      }
    }
    return false;
  }

  bool Problem_Impl::updateMeasureForCompoundRubyMeasure(
      const BCLMeasure& newVersion,
      const std::vector<ruleset::OSArgument>& newArguments,
      bool keepOldArgumentsIfNewEmpty,
      RubyMeasure& measure,
      std::vector<RubyContinuousVariable>& variables)
  {
    bool ok(true);
    BoolVector keep(variables.size(),true);
    if (newArguments.empty() && keepOldArgumentsIfNewEmpty) {
      // no argument changes needed
      OSArgumentVector currentArguments = measure.arguments();
      ok = measure.setMeasure(newVersion);
      if (ok) {
        measure.setArguments(currentArguments);
      }
    }
    else {
      // trim newArguments down to those not called out by variables
      OSArgumentVector argsSubsetForMeasure = newArguments;
      int index(0);
      for (const RubyContinuousVariable& var : variables) {
        NameFinder<OSArgument> finder(var.argument().name(),true);
        auto it = std::find_if(argsSubsetForMeasure.begin(),
                               argsSubsetForMeasure.end(),
                               finder);
        if (it != argsSubsetForMeasure.end()) {
          argsSubsetForMeasure.erase(it);
        }
        else {
          keep[index] = false;
        }
        ++index;
      }

      // try to update measure
      ok = measure.updateMeasure(newVersion,argsSubsetForMeasure);
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
                                 const WorkflowStep& t_step,
                                 boost::optional<unsigned> t_mergedJobIndex)
  : job(t_job),
    step(t_step),
    mergedJobIndex(t_mergedJobIndex)
{}

WorkflowStepJob::WorkflowStepJob(const WorkflowStep& t_step)
  : step(t_step)
{}

WorkflowStepJob::WorkflowStepJob(const runmanager::Job& t_job,
                                 const WorkflowStep& t_step,
                                 const Measure& t_measure,
                                 boost::optional<unsigned> t_mergedJobIndex)
  : job(t_job),
    step(t_step),
    measure(t_measure),
    mergedJobIndex(t_mergedJobIndex)
{}

WorkflowStepJob::WorkflowStepJob(const WorkflowStep& t_step,
                                 const Measure& t_measure)
  : step(t_step),
    measure(t_measure)
{}

WorkflowStepJob::WorkflowStepJob(const runmanager::Job& t_job,
                                 const WorkflowStep &t_step,
                                 const QVariant& t_value,
                                 boost::optional<unsigned> t_mergedJobIndex)
  : job(t_job),
    step(t_step),
    value(t_value),
    mergedJobIndex(t_mergedJobIndex)
{}

WorkflowStepJob::WorkflowStepJob(const WorkflowStep &t_step,
                                 const QVariant& t_value)
  : step(t_step),
    value(t_value)
{}

boost::optional<runmanager::JobErrors> WorkflowStepJob::errors() const {
  if (job) {
    if (job->hasMergedJobs()) {
      OS_ASSERT(mergedJobIndex);
      return job->mergedJobResults()[*mergedJobIndex].errors;
    }
    else {
      return job->errors();
    }
  }
  return boost::none;
}

boost::optional<runmanager::Files> WorkflowStepJob::outputFiles() const {
  if (job) {
    if (job->hasMergedJobs()) {
      OS_ASSERT(mergedJobIndex);
      return job->mergedJobResults()[*mergedJobIndex].outputFiles;
    }
    else {
      return runmanager::Files(job->outputFiles());
    }
  }
  return boost::none;
}

Problem::Problem(const std::string& name,
                 const std::vector<WorkflowStep>& workflow)
  : AnalysisObject(std::shared_ptr<detail::Problem_Impl>(
        new detail::Problem_Impl(name,workflow)))
{
  Problem copyOfThis(getImpl<detail::Problem_Impl>());
  for (const WorkflowStep& step : workflow) {
    step.setParent(copyOfThis);
  }
}

Problem::Problem(const std::string& name,
                 const std::vector<WorkflowStep>& workflow,
                 const std::vector<Function>& responses)
  : AnalysisObject(std::shared_ptr<detail::Problem_Impl>(
        new detail::Problem_Impl(name,workflow,responses)))
{
  Problem copyOfThis(getImpl<detail::Problem_Impl>());
  for (const WorkflowStep& step : workflow) {
    step.setParent(copyOfThis);
  }
  for (const Function& response : responses) {
    response.setParent(copyOfThis);
  }
}

Problem::Problem(const std::string& name,
                 const std::vector<Variable>& variables,
                 const std::vector<Function>& responses,
                 const runmanager::Workflow& simulationWorkflow)
  : AnalysisObject(std::shared_ptr<detail::Problem_Impl>(
        new detail::Problem_Impl(name,variables,responses,simulationWorkflow)))
{
  Problem copyOfThis(getImpl<detail::Problem_Impl>());
  for (const WorkflowStep& step : workflow()) {
    step.setParent(copyOfThis);
  }
  for (const Function& response : responses) {
    response.setParent(copyOfThis);
  }
}

Problem::Problem(const std::string& name,
                 const std::vector<Variable>& variables,
                 const runmanager::Workflow& simulationWorkflow)
  : AnalysisObject(std::shared_ptr<detail::Problem_Impl>(
        new detail::Problem_Impl(name,variables,simulationWorkflow)))
{
  Problem copyOfThis(getImpl<detail::Problem_Impl>());
  for (const WorkflowStep& step : workflow()) {
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
  : AnalysisObject(std::shared_ptr<detail::Problem_Impl>(
        new detail::Problem_Impl(uuid,
                                 versionUUID,
                                 name,
                                 displayName,
                                 description,
                                 workflow,
                                 responses)))
{
  Problem copyOfThis(getImpl<detail::Problem_Impl>());
  for (const WorkflowStep& step : workflow) {
    step.setParent(copyOfThis);
  }
  for (const Function& response : responses) {
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
    const std::vector<Measure>& measures) const
{
  return getImpl<detail::Problem_Impl>()->getVariableValues(measures);
}

std::vector<QVariant> Problem::getVariableValues(
    const std::vector< boost::optional<Measure> >& measures) const
{
  return getImpl<detail::Problem_Impl>()->getVariableValues(measures);
}

std::vector<boost::optional<Measure> > Problem::getMeasures(
    const std::vector<QVariant>& variableValues) const
{
  return getImpl<detail::Problem_Impl>()->getMeasures(variableValues);
}

std::vector<boost::optional<Measure> > Problem::getDiscretePerturbations(
    const std::vector<QVariant>& variableValues) const
{
  return getImpl<detail::Problem_Impl>()->getMeasures(variableValues);
}

boost::optional<int> Problem::combinatorialSize(bool selectedOnly) const {
  return getImpl<detail::Problem_Impl>()->combinatorialSize(selectedOnly);
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
    const std::vector<Measure>& measures) const
{
  return getImpl<detail::Problem_Impl>()->createDataPoint(measures);
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
Problem::Problem(std::shared_ptr<detail::Problem_Impl> impl)
  : AnalysisObject(impl)
{}
/// @endcond

} // analysis
} // openstudio


