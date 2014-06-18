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

#include "ProblemRecord.hpp"
#include "ProblemRecord_Impl.hpp"

#include "FunctionRecord.hpp"
#include "InputVariableRecord.hpp"
#include "JoinRecord.hpp"
#include "MeasureGroupRecord.hpp"
#include "MeasureGroupRecord_Impl.hpp"
#include "OptimizationProblemRecord.hpp"
#include "ProjectDatabase.hpp"
#include "WorkflowRecord.hpp"

#include "../analysis/InputVariable.hpp"
#include "../analysis/OptimizationProblem.hpp"
#include "../analysis/OptimizationProblem_Impl.hpp"
#include "../analysis/WorkflowStep.hpp"

#include "../runmanager/lib/RubyJobUtils.hpp"
#include "../runmanager/lib/Workflow.hpp"
#include "../runmanager/lib/WorkItem.hpp"

#include "../utilities/core/Assert.hpp"

#include <boost/optional/optional.hpp>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QFile>

using namespace openstudio::analysis;

namespace openstudio {
namespace project {

namespace detail {

  ProblemRecord_Impl::ProblemRecord_Impl(const analysis::Problem& problem,
                                         const ProblemRecordType& problemRecordType,
                                         ProjectDatabase& database)
    : ObjectRecord_Impl(database,problem.uuid(),
                        problem.name(),
                        problem.displayName(),
                        problem.description(),
                        problem.versionUUID()),
      m_problemRecordType(problemRecordType)
  {}

  ProblemRecord_Impl::ProblemRecord_Impl(const QSqlQuery& query, ProjectDatabase& database)
    : ObjectRecord_Impl(database, query)
  {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    QVariant value;

    value = query.value(ProblemRecordColumns::problemRecordType);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_problemRecordType = ProblemRecordType(value.toInt());
  }

  std::string ProblemRecord_Impl::databaseTableName() const {
    return ProblemRecord::databaseTableName();
  }

  boost::optional<ObjectRecord> ProblemRecord_Impl::parent() const {
    return boost::none;
  }

  std::vector<ObjectRecord> ProblemRecord_Impl::children() const {
    ObjectRecordVector result;

    InputVariableRecordVector inputVariableRecords = this->inputVariableRecords();
    result.insert(result.end(),inputVariableRecords.begin(),inputVariableRecords.end());

    WorkflowRecordVector workflowRecords = this->workflowRecords();
    result.insert(result.end(),workflowRecords.begin(),workflowRecords.end());

    FunctionRecordVector responseRecords = this->responseRecords();
    result.insert(result.end(),responseRecords.begin(),responseRecords.end());

    return result;
  }

  std::vector<ObjectRecord> ProblemRecord_Impl::resources() const {
    ObjectRecordVector result;
    return result;
  }

  std::vector<JoinRecord> ProblemRecord_Impl::joinRecords() const {
    JoinRecordVector result;
    return result;
  }

  void ProblemRecord_Impl::saveRow(const std::shared_ptr<QSqlDatabase> &database)
  {
    QSqlQuery query(*database);
    this->makeUpdateByIdQuery<ProblemRecord>(query);
    this->bindValues(query);
    assertExec(query);
  }

  std::vector<InputVariableRecord> ProblemRecord_Impl::inputVariableRecords() const {
    InputVariableRecordVector result;

    ProjectDatabase database = projectDatabase();
    QSqlQuery query(*(database.qSqlDatabase()));
    query.prepare(toQString("SELECT * FROM " + VariableRecord::databaseTableName() +
        " WHERE problemRecordId=:problemRecordId AND variableRecordType=:variableRecordType " +
        "ORDER BY variableVectorIndex"));
    query.bindValue(":problemRecordId",id());
    query.bindValue(":variableRecordType",VariableRecordType::InputVariableRecord);
    assertExec(query);
    OptionalInt previousIndex;
    bool resort(false);
    while (query.next()) {
      result.push_back(InputVariableRecord::factoryFromQuery(query, database).get());
      int index = result.back().variableVectorIndex();
      if (previousIndex) {
        if (index <= previousIndex.get()) {
          resort = true; // if variables are changed around, order in database itself can
                         // get out of date (as compared to database + dirty records).
        }
      }
      previousIndex = index;
    }

    if (resort) {
      VariableRecordVariableVectorIndexLess comparator;
      std::sort(result.begin(),result.end(),comparator);
    }

    return result;
  }

  std::vector<WorkflowRecord> ProblemRecord_Impl::workflowRecords() const {
    WorkflowRecordVector result;

    ProjectDatabase database = projectDatabase();
    QSqlQuery query(*(database.qSqlDatabase()));
    query.prepare(toQString("SELECT * FROM " + WorkflowRecord::databaseTableName() +
                            " WHERE problemRecordId=:problemRecordId ORDER BY workflowIndex"));
    query.bindValue(":problemRecordId",id());
    assertExec(query);
    OptionalInt previousIndex;
    bool resort(false);
    while (query.next()) {
      result.push_back(WorkflowRecord::factoryFromQuery(query, database).get());
      int index = result.back().workflowIndex();
      if (previousIndex) {
        if (index < previousIndex.get()) {
          resort = true;
        }
      }
      previousIndex = index;
    }

    if (resort) {
      WorkflowRecordWorkflowIndexLess comparator;
      std::sort(result.begin(),result.end(),comparator);
    }

    return result;
  }

  std::vector<FunctionRecord> ProblemRecord_Impl::responseRecords() const {
    FunctionRecordVector result;

    ProjectDatabase database = projectDatabase();
    QSqlQuery query(*(database.qSqlDatabase()));
    query.prepare(toQString("SELECT * FROM " + FunctionRecord::databaseTableName() +
                  " WHERE problemRecordId=:problemRecordId AND functionType=:functionType " +
                  "ORDER BY functionVectorIndex"));
    query.bindValue(":problemRecordId",id());
    query.bindValue(":functionType",FunctionType::Response);
    assertExec(query);
    OptionalInt previousIndex;
    bool resort(false);
    while (query.next()) {
      result.push_back(FunctionRecord::factoryFromQuery(query, database).get());
      int index = result.back().functionVectorIndex();
      if (previousIndex) {
        if (index <= previousIndex.get()) {
          resort = true; // if functions are moved around, order in database itself can
                         // get out of date (as compared to database + dirty records).
        }
      }
      previousIndex = index;
    }

    if (resort) {
      FunctionRecordFunctionVectorIndexLess comparator;
      std::sort(result.begin(),result.end(),comparator);
    }

    return result;
  }

  analysis::Problem ProblemRecord_Impl::problem() const {
    InputVariableRecordVector inputVariableRecords = this->inputVariableRecords();
    WorkflowRecordVector workflowRecords = this->workflowRecords();

    analysis::WorkflowStepVector workflow;

    // fix-up UserScript WorkItem paths
    ProjectDatabase database = projectDatabase();
    openstudio::path originalBasePath = database.originalBasePath();
    openstudio::path newBasePath = database.newBasePath();
    bool fixupPaths = false;
    if (originalBasePath != newBasePath) {
      fixupPaths = true;
    }

    // mesh InputVariables and WorkItems together to form overall workflow
    int ivrIndex(0); // index into input variable records
    int wrIndex(0);  // index into workflow records
    int wIndex(0);   // index into workflow
    int ivrN = inputVariableRecords.size();
    int wrN = workflowRecords.size();
    OptionalInt ivrWIndex; // saved index into workflow for next input variable
    OptionalInt wrWIndex;  // saved index into workflow for next workflow record
    if (!inputVariableRecords.empty()) {
      ivrWIndex = inputVariableRecords[ivrIndex].variableVectorIndex();
    }
    if (!workflowRecords.empty()) {
      wrWIndex = workflowRecords[wrIndex].workflowIndex();
    }
    for (int i = 0, n = ivrN + wrN; i < n; ++i) {
      if (!wrWIndex || (ivrWIndex && (*ivrWIndex < *wrWIndex))) {
        // InputVariable is next
        OS_ASSERT(*ivrWIndex == wIndex); // saved and expected index into workflow match
        OS_ASSERT(ivrIndex < ivrN);      // there is an input variable record to deserialize
        workflow.push_back(WorkflowStep(inputVariableRecords[ivrIndex].inputVariable()));
        ++ivrIndex; // go to next input variable record
        ++wIndex;   // and next index into workflow
        OS_ASSERT(wIndex == int(workflow.size())); // next index into workflow should match current size
        if (ivrIndex < ivrN) {
          ivrWIndex = inputVariableRecords[ivrIndex].variableVectorIndex();
        }
        else {
          ivrWIndex.reset(); // no more input variables to deserialize
        }
      }
      else {
        // Workflow is next
        OS_ASSERT(wrWIndex);
        int temp = *wrWIndex; // for debugging
        OS_ASSERT(temp == wIndex); // saved index into workflow should match expected value
        OS_ASSERT(wrIndex < wrN);  // there is a workflow record to deserialize
        std::vector<runmanager::WorkItem> workItems = workflowRecords[wrIndex].workflow().toWorkItems();
        for (const runmanager::WorkItem& workItem : workItems) {
          if (fixupPaths && (workItem.type == runmanager::JobType::UserScript || workItem.type == runmanager::JobType::Ruby)) {
            LOG(Debug, "Updating paths for ruby / userscript job. Keyname: " + workItem.jobkeyname);
            // hoping that this resets the location of UserScriptAdapter.rb
            runmanager::RubyJobBuilder rjb(workItem,originalBasePath,newBasePath);
            runmanager::WorkItem refreshedWorkItem = rjb.toWorkItem();
            workflow.push_back(refreshedWorkItem);
          }
          else {
            workflow.push_back(workItem);
          }
        }
        ++wrIndex; // go to next workflow record
        wIndex += workItems.size(); // update next expected workflow index
        OS_ASSERT(wIndex == int(workflow.size())); // next index into workflow should match current size
        if (wrIndex < wrN) {
          wrWIndex = workflowRecords[wrIndex].workflowIndex();
        }
        else {
          wrWIndex.reset(); // no more workflow records to deserialize
        }
      }
    }

    analysis::FunctionVector responses;
    FunctionRecordVector responseRecords = this->responseRecords();
    for (const FunctionRecord responseRecord : responseRecords) {
      responses.push_back(responseRecord.function());
    }

    return analysis::Problem(handle(),
                             uuidLast(),
                             name(),
                             displayName(),
                             description(),
                             workflow,
                             responses);
  }

  boost::optional<int> ProblemRecord_Impl::combinatorialSize(bool selectedMeasuresOnly) const {
    int result(1);
    InputVariableRecordVector inputVariableRecords = this->inputVariableRecords();
    for (const InputVariableRecord& inputVariableRecord : inputVariableRecords) {
      OptionalMeasureGroupRecord omgr = inputVariableRecord.optionalCast<MeasureGroupRecord>();
      if (!omgr) {
        return boost::none;
      }
      result *= omgr->numMeasures(selectedMeasuresOnly);
    }
    return result;
  }

  void ProblemRecord_Impl::bindValues(QSqlQuery& query) const
  {
    ObjectRecord_Impl::bindValues(query);

    query.bindValue(ProblemRecordColumns::problemRecordType, m_problemRecordType.value());
  }

  void ProblemRecord_Impl::setLastValues(const QSqlQuery& query, ProjectDatabase& projectDatabase)
  {
    ObjectRecord_Impl::setLastValues(query, projectDatabase);

    QVariant value;

    value = query.value(ProblemRecordColumns::problemRecordType);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_lastProblemRecordType = ProblemRecordType(value.toInt());
  }

  bool ProblemRecord_Impl::compareValues(const QSqlQuery& query) const
  {
    bool result = ObjectRecord_Impl::compareValues(query);

    QVariant value;

    value = query.value(ProblemRecordColumns::problemRecordType);
    OS_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_problemRecordType == ProblemRecordType(value.toInt()));

    return result;
  }

  void ProblemRecord_Impl::saveLastValues()
  {
    ObjectRecord_Impl::saveLastValues();

    m_lastProblemRecordType = m_problemRecordType;
  }

  void ProblemRecord_Impl::revertToLastValues()
  {
    ObjectRecord_Impl::revertToLastValues();

    m_problemRecordType = m_lastProblemRecordType;
  }

} // detail

std::string ProblemRecord::databaseTableName()
{
  return "ProblemRecords";
}

UpdateByIdQueryData ProblemRecord::updateByIdQueryData() {
  static UpdateByIdQueryData result;
  if (result.queryString.empty()) {
    // numeric column identifiers
    result.columnValues = ColumnsType::getValues();

    // query string
    std::stringstream ss;
    ss << "UPDATE " << databaseTableName() << " SET ";
    int expectedValue = 0;
    for (auto it = result.columnValues.begin(),
         itend = result.columnValues.end(); it != itend; ++it)
    {
      // require 0 based columns, don't skip any
      OS_ASSERT(*it == expectedValue);
      // column name is name, type is description
      ss << ColumnsType::valueName(*it) << "=:" << ColumnsType::valueName(*it);
      // is this the last column?
      auto nextIt = it;
      ++nextIt;
      if (nextIt == itend) {
        ss << " ";
      }
      else {
        ss << ", ";
      }
      ++expectedValue;
    }
    ss << "WHERE id=:id";
    result.queryString = ss.str();

    // null values
    for (const auto & columnValue : result.columnValues)
    {
      // bind all values to avoid parameter mismatch error
      if (istringEqual(ColumnsType::valueDescription(columnValue), "INTEGER")) {
        result.nulls.push_back(QVariant(QVariant::Int));
      }
      else {
        result.nulls.push_back(QVariant(QVariant::String));
      }
    }
  }
  return result;
}

void ProblemRecord::updatePathData(ProjectDatabase database,
                                   const openstudio::path& originalBase,
                                   const openstudio::path& newBase)
{}

boost::optional<ProblemRecord> ProblemRecord::factoryFromQuery(const QSqlQuery& query,
                                                               ProjectDatabase& database)
{
  OptionalProblemRecord result;

  int problemRecordType = query.value(ProblemRecordColumns::problemRecordType).toInt();

  switch (problemRecordType) {
    case ProblemRecordType::ProblemRecord :
      result = ProblemRecord(query, database);
     break;
    case ProblemRecordType::OptimizationProblemRecord :
      result = OptimizationProblemRecord(query, database);
     break;
    default :
      LOG(Error,"Unknown ProblemRecordType " << problemRecordType);
  }

  return result;
}

ProblemRecord ProblemRecord::factoryFromProblem(const analysis::Problem& problem,
                                                ProjectDatabase& database)
{
  LOG(Trace,"Creating ProblemRecord from Problem of unknown type.");
  if (problem.optionalCast<analysis::OptimizationProblem>()) {
    LOG(Trace,"Creating OptimizationProblemRecord.");
    return OptimizationProblemRecord(problem.cast<analysis::OptimizationProblem>(),database);
  }
  LOG(Trace,"Creating ProblemRecord.");
  return ProblemRecord(problem,database);
}

std::vector<ProblemRecord> ProblemRecord::getProblemRecords(ProjectDatabase& database)
{
  std::vector<ProblemRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + ProblemRecord::databaseTableName()));
  assertExec(query);
  while (query.next()) {
    OptionalProblemRecord problemRecord = ProblemRecord::factoryFromQuery(query, database);
    if (problemRecord) {
      result.push_back(*problemRecord);
    }
  }

  return result;
}

boost::optional<ProblemRecord> ProblemRecord::getProblemRecord(int id, ProjectDatabase& database)
{
  boost::optional<ProblemRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + ProblemRecord::databaseTableName() + " WHERE id=:id"));
  query.bindValue(":id", id);
  assertExec(query);
  if (query.first()) {
    result = ProblemRecord::factoryFromQuery(query, database);
  }

  return result;
}

std::vector<InputVariableRecord> ProblemRecord::inputVariableRecords() const {
  return getImpl<detail::ProblemRecord_Impl>()->inputVariableRecords();
}

std::vector<WorkflowRecord> ProblemRecord::workflowRecords() const {
  return getImpl<detail::ProblemRecord_Impl>()->workflowRecords();
}

std::vector<FunctionRecord> ProblemRecord::responseRecords() const {
  return getImpl<detail::ProblemRecord_Impl>()->responseRecords();
}

analysis::Problem ProblemRecord::problem() const {
  return getImpl<detail::ProblemRecord_Impl>()->problem();
}

boost::optional<int> ProblemRecord::combinatorialSize(bool selectedMeasuresOnly) const {
  return getImpl<detail::ProblemRecord_Impl>()->combinatorialSize(selectedMeasuresOnly);
}

ProblemRecord::ProblemRecord(std::shared_ptr<detail::ProblemRecord_Impl> impl,
                             ProjectDatabase database)
  : ObjectRecord(impl, database)
{
  OS_ASSERT(getImpl<detail::ProblemRecord_Impl>());
}

/// @cond
ProblemRecord::ProblemRecord(std::shared_ptr<detail::ProblemRecord_Impl> impl)
  : ObjectRecord(impl)
{
  OS_ASSERT(getImpl<detail::ProblemRecord_Impl>());
}
/// @endcond

void ProblemRecord::constructRelatedRecords(const analysis::Problem& problem) {
  ProblemRecord copyOfThis(getImpl<detail::ProblemRecord_Impl>());
  ProjectDatabase database = copyOfThis.projectDatabase();
  bool isNew = database.isNewRecord(copyOfThis);

  // Save child WorkflowSteps
  // Workflows do not have consistent UUIDs, so always remove
  if (!isNew) {
    WorkflowRecordVector oldWorkflowRecords = workflowRecords();
    for (WorkflowRecord& oldRecord : oldWorkflowRecords) {
      database.removeRecord(oldRecord);
    }
  }
  // Construct new records
  int i = 0;
  std::vector<UUID> inputVariableUUIDs;
  std::vector<runmanager::WorkItem> workflow;
  OptionalInt workflowIndex;
  for (const analysis::WorkflowStep& step : problem.workflow()) {
    if (step.isInputVariable()) {
      if (workflowIndex) {
        runmanager::Workflow rmWorkflow(workflow);
        rmWorkflow.setName(toString(createUUID())); // unique name so deletion ok in removeRecord
        WorkflowRecord newWorkflowRecord(rmWorkflow,
                                         copyOfThis,
                                         *workflowIndex);
        workflow.clear();
        workflowIndex.reset();
      }
      analysis::InputVariable inputVariable = step.inputVariable();
      inputVariableUUIDs.push_back(inputVariable.uuid());
      if (inputVariable.isDirty() || isNew) {
        InputVariableRecord newInputVariableRecord =
            InputVariableRecord::factoryFromInputVariable(inputVariable,copyOfThis,i);
      }
    }
    else {
      if (!workflowIndex) {
        workflowIndex = i;
      }
      workflow.push_back(step.workItem());
    }
    ++i;
  }
  if (workflowIndex) {
    runmanager::Workflow rmWorkflow(workflow);
    rmWorkflow.setName(toString(createUUID())); // unique name so deletion ok in removeRecord
    WorkflowRecord newWorkflowRecord(rmWorkflow,
                                     copyOfThis,
                                     *workflowIndex);
    workflow.clear();
    workflowIndex.reset();
  }
  // Clear out deprecated records
  if (!isNew) {
    removeInputVariableRecords(inputVariableUUIDs,database);
  }

  // Save child response functions
  i = 0;
  std::vector<UUID> responseUUIDs;
  for (const analysis::Function& response : problem.responses()) {
    responseUUIDs.push_back(response.uuid());
    if (response.isDirty() || isNew) {
      FunctionRecord newFunctionRecord = FunctionRecord::factoryFromFunction(
          response,
          copyOfThis,
          FunctionType(FunctionType::Response),
          i);
    }
    ++i;
  }
  if (!isNew) {
    removeResponseRecords(responseUUIDs,database);
  }
}

ProblemRecord::ProblemRecord(const analysis::Problem& problem, ProjectDatabase& database)
  : ObjectRecord(std::shared_ptr<detail::ProblemRecord_Impl>(
        new detail::ProblemRecord_Impl(problem, ProblemRecordType::ProblemRecord, database)),
        database)
{
  OS_ASSERT(getImpl<detail::ProblemRecord_Impl>());

  constructRelatedRecords(problem);
}

ProblemRecord::ProblemRecord(const QSqlQuery& query, ProjectDatabase& database)
  : ObjectRecord(std::shared_ptr<detail::ProblemRecord_Impl>(
        new detail::ProblemRecord_Impl(query, database)),
        database)
{
  OS_ASSERT(getImpl<detail::ProblemRecord_Impl>());
}

void ProblemRecord::removeInputVariableRecords(const std::vector<UUID>& uuidsToKeep,
                                               ProjectDatabase& database)
{
  QSqlQuery query(*(database.qSqlDatabase()));
  std::stringstream ss;
  ss << "SELECT * FROM " + VariableRecord::databaseTableName() +
        " WHERE (problemRecordId=:problemRecordId) AND (variableRecordType=:variableRecordType) " +
        "AND (handle NOT IN (";
  std::string sep("");
  for (const UUID& handle : uuidsToKeep) {
    ss << sep << "'" << toString(handle) << "'";
    sep = std::string(", ");
  }
  ss << "))";
  query.prepare(toQString(ss.str()));
  query.bindValue(":problemRecordId",id());
  query.bindValue(":variableRecordType",VariableRecordType::InputVariableRecord);
  assertExec(query);
  while (query.next()) {
    OptionalInputVariableRecord inputVariableRecord = InputVariableRecord::factoryFromQuery(query, database);
    if (inputVariableRecord) {
      database.removeRecord(*inputVariableRecord);
    }
  }
}

void ProblemRecord::removeResponseRecords(const std::vector<UUID>& uuidsToKeep,
                                          ProjectDatabase& database)
{
  QSqlQuery query(*(database.qSqlDatabase()));
  std::stringstream ss;
  ss << "SELECT * FROM " + FunctionRecord::databaseTableName() + " WHERE ";
  ss << "(problemRecordId=:problemRecordId) AND (functionType=:functionType) AND ";
  ss << "(handle NOT IN (";
  std::string sep("");
  for (const UUID& handle : uuidsToKeep) {
    ss << sep << "'" << toString(handle) << "'";
    sep = std::string(", ");
  }
  ss << "))";
  query.prepare(toQString(ss.str()));
  query.bindValue(":problemRecordId",id());
  query.bindValue(":functionType",FunctionType(FunctionType::Response).value());
  assertExec(query);
  while (query.next()) {
    OptionalFunctionRecord functionRecord = FunctionRecord::factoryFromQuery(query, database);
    if (functionRecord) {
      database.removeRecord(*functionRecord);
    }
  }
}

} // project
} // openstudio
