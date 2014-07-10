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

#include "WorkflowRecord.hpp"
#include "WorkflowRecord_Impl.hpp"

#include "JoinRecord.hpp"
#include "ProblemRecord.hpp"
#include "ProjectDatabase.hpp"

#include "../runmanager/lib/RunManager.hpp"
#include "../runmanager/lib/Workflow.hpp"

#include "../utilities/core/Assert.hpp"

#include <boost/optional/optional.hpp>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

namespace openstudio {
namespace project {

namespace detail {

  WorkflowRecord_Impl::WorkflowRecord_Impl(const runmanager::Workflow& workflow,
                                           ProblemRecord& problemRecord,
                                           int workflowIndex)
    : ObjectRecord_Impl(problemRecord.projectDatabase(),
                        workflow.uuid(),
                        "",
                        "",
                        "",
                        createUUID()),
      m_problemRecordId(problemRecord.id()),
      m_workflowIndex(workflowIndex)
  {
    openstudio::runmanager::RunManager runManager = problemRecord.projectDatabase().runManager();
    try {
      m_runManagerWorkflowKey = runManager.persistWorkflow(workflow);
    }
    catch (const std::exception& e) {
      LOG(Error,"Could not persist workflow, because '" << e.what() << "'.");
    }
  }

  WorkflowRecord_Impl::WorkflowRecord_Impl(const QSqlQuery& query, const ProjectDatabase& projectDatabase)
    : ObjectRecord_Impl(projectDatabase, query)
  {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    QVariant value;

    value = query.value(WorkflowRecordColumns::runManagerWorkflowKey);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_runManagerWorkflowKey = value.toString().toStdString();

    value = query.value(WorkflowRecordColumns::problemRecordId);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_problemRecordId = value.toInt();

    value = query.value(WorkflowRecordColumns::workflowIndex);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_workflowIndex = value.toInt();

  }

  std::string WorkflowRecord_Impl::databaseTableName() const {
    return WorkflowRecord::databaseTableName();
  }

  boost::optional<ObjectRecord> WorkflowRecord_Impl::parent() const {
    return problemRecord();
  }

  std::vector<ObjectRecord> WorkflowRecord_Impl::children() const
  {
    return std::vector<ObjectRecord>();
  }

  std::vector<ObjectRecord> WorkflowRecord_Impl::resources() const
  {
    return std::vector<ObjectRecord>();
  }

  std::vector<JoinRecord> WorkflowRecord_Impl::joinRecords() const
  {
    return std::vector<JoinRecord>();
  }

  void WorkflowRecord_Impl::saveRow(const std::shared_ptr<QSqlDatabase> &database)
  {
    QSqlQuery query(*database);
    this->makeUpdateByIdQuery<WorkflowRecord>(query);
    this->bindValues(query);
    assertExec(query);
  }

  ProblemRecord WorkflowRecord_Impl::problemRecord() const {
    ProjectDatabase database = projectDatabase();
    return ProblemRecord::getProblemRecord(m_problemRecordId,database).get();
  }

  std::string WorkflowRecord_Impl::runManagerWorkflowKey() const {
    return m_runManagerWorkflowKey;
  }

  int WorkflowRecord_Impl::workflowIndex() const {
    return m_workflowIndex;
  }

  openstudio::runmanager::Workflow WorkflowRecord_Impl::workflow() const {
    ProjectDatabase projectDatabase = this->projectDatabase();

    openstudio::runmanager::Workflow result;
    openstudio::runmanager::RunManager runManager = projectDatabase.runManager();
    try {
      result = runManager.loadWorkflow(m_runManagerWorkflowKey);
      result.setUUID(handle());
    }
    catch (...) {
      LOG(Error,"Unable to load workflow from RunManager using key "
          << m_runManagerWorkflowKey << ". Returning empty Workflow.");
    }

    return result;
  }

  void WorkflowRecord_Impl::bindValues(QSqlQuery& query) const
  {
    ObjectRecord_Impl::bindValues(query);

    query.bindValue(WorkflowRecordColumns::runManagerWorkflowKey, toQString(m_runManagerWorkflowKey));
    query.bindValue(WorkflowRecordColumns::problemRecordId, m_problemRecordId);
    query.bindValue(WorkflowRecordColumns::workflowIndex, m_workflowIndex);
  }

  void WorkflowRecord_Impl::setLastValues(const QSqlQuery& query, ProjectDatabase& projectDatabase)
  {
    ObjectRecord_Impl::setLastValues(query, projectDatabase);

    QVariant value;

    value = query.value(WorkflowRecordColumns::runManagerWorkflowKey);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_lastRunManagerWorkflowKey = value.toString().toStdString();

    value = query.value(WorkflowRecordColumns::problemRecordId);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_lastProblemRecordId = value.toInt();

    value = query.value(WorkflowRecordColumns::workflowIndex);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_lastWorkflowIndex = value.toInt();
  }

  bool WorkflowRecord_Impl::compareValues(const QSqlQuery& query) const
  {
    bool result = true;

    result = result && ObjectRecord_Impl::compareValues(query);

    QVariant value;

    value = query.value(WorkflowRecordColumns::runManagerWorkflowKey);
    OS_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_runManagerWorkflowKey == value.toString().toStdString());

    value = query.value(WorkflowRecordColumns::problemRecordId);
    OS_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_problemRecordId == value.toInt());

    value = query.value(WorkflowRecordColumns::workflowIndex);
    OS_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_workflowIndex == value.toInt());

    return result;
  }

  void WorkflowRecord_Impl::saveLastValues()
  {
    ObjectRecord_Impl::saveLastValues();

    m_lastRunManagerWorkflowKey = m_runManagerWorkflowKey;
    m_lastProblemRecordId = m_problemRecordId;
    m_lastWorkflowIndex = m_workflowIndex;
  }

  void WorkflowRecord_Impl::revertToLastValues()
  {
    ObjectRecord_Impl::revertToLastValues();

    m_runManagerWorkflowKey = m_lastRunManagerWorkflowKey;
    m_problemRecordId = m_lastProblemRecordId;
    m_workflowIndex = m_lastWorkflowIndex;
  }

} // detail

WorkflowRecord::WorkflowRecord(const runmanager::Workflow& workflow,
                               ProblemRecord& problemRecord,
                               int workflowIndex)
  : ObjectRecord(std::shared_ptr<detail::WorkflowRecord_Impl>(
                   new detail::WorkflowRecord_Impl(workflow,
                                                   problemRecord,
                                                   workflowIndex)),
                 problemRecord.projectDatabase())
{
  OS_ASSERT(getImpl<detail::WorkflowRecord_Impl>());
}

WorkflowRecord::WorkflowRecord(const QSqlQuery& query,ProjectDatabase& database)
  : ObjectRecord(std::shared_ptr<detail::WorkflowRecord_Impl>(
                   new detail::WorkflowRecord_Impl(query,database)),
                   database)
{
  OS_ASSERT(getImpl<detail::WorkflowRecord_Impl>());
}

std::string WorkflowRecord::databaseTableName()
{
  return "WorkflowRecords";
}

UpdateByIdQueryData WorkflowRecord::updateByIdQueryData() {
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

void WorkflowRecord::updatePathData(ProjectDatabase database,
                                    const openstudio::path& originalBase,
                                    const openstudio::path& newBase)
{}

boost::optional<WorkflowRecord> WorkflowRecord::factoryFromQuery(const QSqlQuery& query,
                                                                 ProjectDatabase& database)
{
  OptionalWorkflowRecord result;
  try {
    result = WorkflowRecord(query,database);
  }
  catch (const std::exception& e) {
    LOG(Error,"Unable to construct WorkflowRecord from query, because '"
        << e.what() << "'.");
  }
  return result;
}

std::vector<WorkflowRecord> WorkflowRecord::getWorkflowRecords(ProjectDatabase& projectDatabase)
{
  std::vector<WorkflowRecord> result;

  QSqlQuery query(*(projectDatabase.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + WorkflowRecord::databaseTableName()));
  assertExec(query);
  while (query.next()) {
    result.push_back(projectDatabase.getFromQuery<WorkflowRecord>(query));
  }

  return result;
}

boost::optional<WorkflowRecord> WorkflowRecord::getWorkflowRecord(int id, ProjectDatabase& projectDatabase)
{
  boost::optional<WorkflowRecord> result;

  QSqlQuery query(*(projectDatabase.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + WorkflowRecord::databaseTableName() + " WHERE id=:id"));
  query.bindValue(":id", id);
  assertExec(query);
  if (query.first()) {
    result = projectDatabase.getFromQuery<WorkflowRecord>(query);
  }

  return result;
}

ProblemRecord WorkflowRecord::problemRecord() const {
  return getImpl<detail::WorkflowRecord_Impl>()->problemRecord();
}

std::string WorkflowRecord::runManagerWorkflowKey() const {
  return getImpl<detail::WorkflowRecord_Impl>()->runManagerWorkflowKey();
}

int WorkflowRecord::workflowIndex() const {
  return getImpl<detail::WorkflowRecord_Impl>()->workflowIndex();
}

runmanager::Workflow WorkflowRecord::workflow() const {
  return getImpl<detail::WorkflowRecord_Impl>()->workflow();
}

/// @cond
WorkflowRecord::WorkflowRecord(std::shared_ptr<detail::WorkflowRecord_Impl> impl)
  : ObjectRecord(impl)
{
  OS_ASSERT(getImpl<detail::WorkflowRecord_Impl>());
}
/// @endcond

bool WorkflowRecordWorkflowIndexLess::operator()(const WorkflowRecord& left, 
                                                 const WorkflowRecord& right) const
{
  return (left.workflowIndex() < right.workflowIndex());
}

} // project
} // openstudio
