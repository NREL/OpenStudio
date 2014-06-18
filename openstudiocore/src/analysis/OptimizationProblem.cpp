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

#include "OptimizationProblem.hpp"
#include "OptimizationProblem_Impl.hpp"

#include "DataPoint.hpp"
#include "Function_Impl.hpp"
#include "OptimizationDataPoint.hpp"
#include "OptimizationDataPoint_Impl.hpp"
#include "WorkflowStep.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/core/Containers.hpp"
#include "../utilities/core/Finder.hpp"
#include "../utilities/core/Json.hpp"

namespace openstudio {
namespace analysis {

namespace detail {

  OptimizationProblem_Impl::OptimizationProblem_Impl(const std::string& name,
                                                     const std::vector<Function>& objectives,
                                                     const std::vector<WorkflowStep>& workflow)
    : Problem_Impl(name,workflow),
      m_objectives(objectives)
  {
    for (Function& objective : m_objectives) {
      objective.onChange();
      connectChild(objective,false);
    }
  }


  OptimizationProblem_Impl::OptimizationProblem_Impl(const std::string& name,
                                                     const std::vector<Function>& objectives,
                                                     const std::vector<WorkflowStep>& workflow,
                                                     const std::vector<Function>& responses)
    : Problem_Impl(name,workflow,responses),
      m_objectives(objectives)
  {
    for (Function& objective : m_objectives) {
      objective.onChange();
      connectChild(objective,false);
    }
  }

  OptimizationProblem_Impl::OptimizationProblem_Impl(
      const std::string& name,
      const std::vector<Function>& objectives,
      const std::vector<Variable>& variables,
      const std::vector<Function>& responses,
      const runmanager::Workflow& simulationWorkflow)
    : Problem_Impl(name,variables,responses,simulationWorkflow),
      m_objectives(objectives)
  {
    for (Function& objective : m_objectives) {
      objective.onChange();
      connectChild(objective,false);
    }
  }

  OptimizationProblem_Impl::OptimizationProblem_Impl(
      const std::string& name,
      const std::vector<Function>& objectives,
      const std::vector<Variable>& variables,
      const runmanager::Workflow& simulationWorkflow)
    : Problem_Impl(name,variables,simulationWorkflow),
      m_objectives(objectives)
  {
    for (Function& objective : m_objectives) {
      objective.onChange();
      connectChild(objective,false);
    }
  }

  OptimizationProblem_Impl::OptimizationProblem_Impl(
      const UUID& uuid,
      const UUID& versionUUID,
      const std::string& name,
      const std::string& displayName,
      const std::string& description,
      const std::vector<Function>& objectives,
      const std::vector<WorkflowStep>& workflow,
      const std::vector<Function>& responses)
    : Problem_Impl(uuid,
                   versionUUID,
                   name,
                   displayName,
                   description,
                   workflow,
                   responses),
      m_objectives(objectives)
  {
    for (Function& objective : m_objectives) {
      connectChild(objective,false);
    }
  }

  OptimizationProblem_Impl::OptimizationProblem_Impl(const OptimizationProblem_Impl &other)
    : Problem_Impl(other)
  {
    for (const Function& function : other.objectives()) {
      m_objectives.push_back(function.clone().cast<Function>());
      connectChild(m_objectives.back(),false);
    }
  }

  AnalysisObject OptimizationProblem_Impl::clone() const {
    std::shared_ptr<OptimizationProblem_Impl> impl(new OptimizationProblem_Impl(*this));
    OptimizationProblem result(impl);
    FunctionVector objectives = result.objectives();
    for (Function& objective : objectives) {
      objective.setParent(result);
    }
    return result;
  }

  bool OptimizationProblem_Impl::isValid(const DataPoint& dataPoint) const {
    if (!dataPoint.optionalCast<OptimizationDataPoint>()) {
      return false;
    }
    return Problem_Impl::isValid(dataPoint);
  }

  std::string OptimizationProblem_Impl::dakotaInFileDescription() const {
    return std::string();
  }

  boost::optional<DataPoint> OptimizationProblem_Impl::createDataPoint(
        const std::vector<QVariant>& variableValues) const
  {
    OptionalDataPoint result;
    OptimizationDataPoint candidate(getPublicObject<OptimizationProblem>(),variableValues);
    if (isValid(candidate)) {
      result = candidate;
    }
    return result;
  }

  boost::optional<DataPoint> OptimizationProblem_Impl::createDataPoint(
      const std::vector<DiscretePerturbation>& perturbations) const
  {
    OptionalDataPoint result;
    result = Problem_Impl::createDataPoint(perturbations);
    if (result) {
      result = OptimizationDataPoint(getPublicObject<OptimizationProblem>(),
                                     result->variableValues());
    }
    return result;
  }

  boost::optional<DataPoint> OptimizationProblem_Impl::createDataPoint(
      const openstudio::path& dakotaParametersFile) const
  {
    return boost::none;
  }

  void OptimizationProblem_Impl::updateDataPoint(DataPoint& dataPoint,
                                                 const runmanager::Job& completedJob) const
  {
    Problem_Impl::updateDataPoint(dataPoint,completedJob);
    DoubleVector objectiveFunctionValues;
    for (const Function& objectiveFunction : objectives()) {
      objectiveFunctionValues.push_back(objectiveFunction.getValue(dataPoint));
    }
    dataPoint.cast<OptimizationDataPoint>().setObjectiveValues(objectiveFunctionValues);
  }

  boost::optional<std::string> OptimizationProblem_Impl::getDakotaResultsFile(
      const DataPoint& dataPoint) const
  {
    return boost::none;
  }

  std::vector<Function> OptimizationProblem_Impl::objectives() const {
    return m_objectives;
  }

  int OptimizationProblem_Impl::numObjectives() const {
    return m_objectives.size();
  }

  void OptimizationProblem_Impl::pushObjective(const Function& objective) {
    m_objectives.push_back(objective);
    m_objectives.back().onChange();
    connectChild(m_objectives.back(),true);
    onChange(AnalysisObject_Impl::InvalidatesResults);
  }

  bool OptimizationProblem_Impl::insertObjective(int index, const Function& objective) {
    if ((index < 0) || (index >= numObjectives())) {
      return false;
    }
    auto it = m_objectives.begin();
    for (int count = 0; count < index; ++count, ++it);
    it = m_objectives.insert(it,objective);
    for (int i = index, n = int(m_objectives.size()); i < n; ++i) {
      m_objectives[i].onChange();
    }
    connectChild(*it,true);
    onChange(AnalysisObject_Impl::InvalidatesResults);
    return true;
  }

  bool OptimizationProblem_Impl::eraseObjective(const Function& objective) {
    auto it = std::find_if(
          m_objectives.begin(),
          m_objectives.end(),
          std::bind(uuidsEqual<Function,Function>,std::placeholders::_1,objective));
    if (it == m_objectives.end()) {
      return false;
    }
    int index = int(it - m_objectives.begin());
    disconnectChild(*it);
    m_objectives.erase(it);
    for (int i = index, n = int(m_objectives.size()); i < n; ++i) {
      m_objectives[i].onChange();
    }
    onChange(AnalysisObject_Impl::InvalidatesResults);
    return true;
  }

  bool OptimizationProblem_Impl::swapObjectives(const Function& objective1,
                                                const Function& objective2)
  {
    auto it1 = std::find_if(
          m_objectives.begin(),
          m_objectives.end(),
          std::bind(uuidsEqual<Function,Function>,std::placeholders::_1,objective1));
    auto it2 = std::find_if(
          m_objectives.begin(),
          m_objectives.end(),
          std::bind(uuidsEqual<Function,Function>,std::placeholders::_1,objective2));
    if ((it1 == m_objectives.end()) || it2 == m_objectives.end()) {
      return false;
    }
    Function temp = *it1;
    int index1 = int(it1 - m_objectives.begin());
    int index2 = int(it2 - m_objectives.begin());
    m_objectives.at(index1) = *it2;
    m_objectives.at(index2) = temp;
    m_objectives[index1].onChange();
    m_objectives[index2].onChange();
    onChange(AnalysisObject_Impl::InvalidatesResults);
    return true;
  }

  void OptimizationProblem_Impl::setObjectiveFunctions(const std::vector<Function>& objectives) {
    for (Function& objective : m_objectives) {
      disconnectChild(objective);
    }
    m_objectives = objectives;
    for (Function& objective : m_objectives) {
      objective.onChange();
      connectChild(objective,true);
    }
    onChange(AnalysisObject_Impl::InvalidatesResults);
  }

  void OptimizationProblem_Impl::clearObjectiveFunctions() {
    for (Function& objective : m_objectives) {
      disconnectChild(objective);
    }
    m_objectives.clear();
    onChange(AnalysisObject_Impl::InvalidatesResults);
  }

  QVariant OptimizationProblem_Impl::toVariant() const {
    QVariantMap problemData = Problem_Impl::toVariant().toMap();

    problemData["problem_type"] = QString("OptimizationProblem");

    if (!objectives().empty()) {
      QVariantList objectivesList;
      int index(0);
      for (const Function& objective : objectives()) {
        QVariantMap objectiveMap = objective.toVariant().toMap();
        objectiveMap["objective_index"] = QVariant(index);
        objectivesList.push_back(objectiveMap);
        ++index;
      }
      problemData["objectives"] = QVariant(objectivesList);
    }

    return QVariant(problemData);
  }

  OptimizationProblem OptimizationProblem_Impl::fromVariant(const QVariant& variant, const VersionString& version) {
    Problem slice = Problem_Impl::fromVariant(variant,version);

    QVariantMap map = variant.toMap();
    FunctionVector objectives;
    if (map.contains("objectives")) {
      objectives = deserializeOrderedVector(
            map["objectives"].toList(),
            "objective_index",
            std::function<Function (const QVariant&)>(std::bind(analysis::detail::Function_Impl::factoryFromVariant,std::placeholders::_1,version)));
    }

    return OptimizationProblem(slice.uuid(),
                               slice.versionUUID(),
                               slice.name(),
                               slice.displayName(),
                               slice.description(),
                               objectives,
                               slice.workflow(),
                               slice.responses());
  }

  void OptimizationProblem_Impl::updateInputPathData(const openstudio::path& originalBase,
                                                     const openstudio::path& newBase)
  {
    Problem_Impl::updateInputPathData(originalBase,newBase);

    // objectives
    //
    // currently does nothing, because the only variables that have path data are
    // input variables, and any of those used by an objective should also be in workflow().
    //
    FunctionVector functions = objectives();
    for (Function& func : functions) {
      func.getImpl<detail::Function_Impl>()->updateInputPathData(originalBase,newBase);
    }
  }

} // detail

OptimizationProblem::OptimizationProblem(const std::string& name,
                                         const std::vector<Function>& objectives,
                                         const std::vector<WorkflowStep>& workflow)
  : Problem(std::shared_ptr<detail::OptimizationProblem_Impl>(
                new detail::OptimizationProblem_Impl(name,
                                                     objectives,
                                                     workflow)))
{
  OptimizationProblem copyOfThis(getImpl<detail::OptimizationProblem_Impl>());
  for (const Function& objective : objectives) {
    objective.setParent(copyOfThis);
  }
  for (const WorkflowStep& step : workflow) {
    step.setParent(copyOfThis);
  }
}

OptimizationProblem::OptimizationProblem(const std::string& name,
                                         const std::vector<Function>& objectives,
                                         const std::vector<WorkflowStep>& workflow,
                                         const std::vector<Function>& responses)
  : Problem(std::shared_ptr<detail::OptimizationProblem_Impl>(
                new detail::OptimizationProblem_Impl(name,
                                                     objectives,
                                                     workflow,
                                                     responses)))
{
  OptimizationProblem copyOfThis(getImpl<detail::OptimizationProblem_Impl>());
  for (const Function& objective : objectives) {
    objective.setParent(copyOfThis);
  }
  for (const WorkflowStep& step : workflow) {
    step.setParent(copyOfThis);
  }
  for (const Function& response : responses) {
    response.setParent(copyOfThis);
  }
}

OptimizationProblem::OptimizationProblem(const std::string& name,
                                         const std::vector<Function>& objectives,
                                         const std::vector<Variable>& variables,
                                         const std::vector<Function>& responses,
                                         const runmanager::Workflow& simulationWorkflow)
  : Problem(std::shared_ptr<detail::OptimizationProblem_Impl>(
                new detail::OptimizationProblem_Impl(name,
                                                     objectives,
                                                     variables,
                                                     responses,
                                                     simulationWorkflow)))
{
  OptimizationProblem copyOfThis(getImpl<detail::OptimizationProblem_Impl>());
  for (const Function& objective : objectives) {
    objective.setParent(copyOfThis);
  }
  for (const WorkflowStep& step : workflow()) {
    step.setParent(copyOfThis);
  }
  for (const Function& response : responses) {
    response.setParent(copyOfThis);
  }
}

OptimizationProblem::OptimizationProblem(const std::string& name,
                                         const std::vector<Function>& objectives,
                                         const std::vector<Variable>& variables,
                                         const runmanager::Workflow& simulationWorkflow)
  : Problem(std::shared_ptr<detail::OptimizationProblem_Impl>(
                new detail::OptimizationProblem_Impl(name,
                                                     objectives,
                                                     variables,
                                                     simulationWorkflow)))
{
  OptimizationProblem copyOfThis(getImpl<detail::OptimizationProblem_Impl>());
  for (const Function& objective : objectives) {
    objective.setParent(copyOfThis);
  }
  for (const WorkflowStep& step : workflow()) {
    step.setParent(copyOfThis);
  }
}

OptimizationProblem::OptimizationProblem(const UUID& uuid,
                                         const UUID& versionUUID,
                                         const std::string& name,
                                         const std::string& displayName,
                                         const std::string& description,
                                         const std::vector<Function>& objectives,
                                         const std::vector<WorkflowStep>& workflow,
                                         const std::vector<Function>& responses)
  : Problem(std::shared_ptr<detail::OptimizationProblem_Impl>(
                new detail::OptimizationProblem_Impl(uuid,
                                                     versionUUID,
                                                     name,
                                                     displayName,
                                                     description,
                                                     objectives,
                                                     workflow,
                                                     responses)))
{
  OptimizationProblem copyOfThis(getImpl<detail::OptimizationProblem_Impl>());
  for (const Function& objective : objectives) {
    objective.setParent(copyOfThis);
  }
  for (const WorkflowStep& step : workflow) {
    step.setParent(copyOfThis);
  }
  for (const Function& response : responses) {
    response.setParent(copyOfThis);
  }
}

std::vector<Function> OptimizationProblem::objectives() const {
  return getImpl<detail::OptimizationProblem_Impl>()->objectives();
}

int OptimizationProblem::numObjectives() const {
  return getImpl<detail::OptimizationProblem_Impl>()->numObjectives();
}

void OptimizationProblem::pushObjective(const Function& objective) {
  getImpl<detail::OptimizationProblem_Impl>()->pushObjective(objective);
}

bool OptimizationProblem::insertObjective(int index, const Function& objective) {
  return getImpl<detail::OptimizationProblem_Impl>()->insertObjective(index,objective);
}

bool OptimizationProblem::eraseObjective(const Function& objective) {
  return getImpl<detail::OptimizationProblem_Impl>()->eraseObjective(objective);
}

bool OptimizationProblem::swapObjectives(const Function& objective1,
                                         const Function& objective2)
{
  return getImpl<detail::OptimizationProblem_Impl>()->swapObjectives(objective1,objective2);
}

void OptimizationProblem::setObjectiveFunctions(const std::vector<Function>& objectives) {
  getImpl<detail::OptimizationProblem_Impl>()->setObjectiveFunctions(objectives);
}

void OptimizationProblem::clearObjectiveFunctions() {
  getImpl<detail::OptimizationProblem_Impl>()->clearObjectiveFunctions();
}

/// @cond
OptimizationProblem::OptimizationProblem(std::shared_ptr<detail::OptimizationProblem_Impl> impl)
  : Problem(impl)
{}
/// @endcond

} // analysis
} // openstudio

