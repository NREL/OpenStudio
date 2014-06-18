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

#include "VariableRecord.hpp"
#include "VariableRecord_Impl.hpp"

#include "FunctionRecord.hpp"
#include "InputVariableRecord.hpp"
#include "JoinRecord.hpp"
#include "OutputVariableRecord.hpp"
#include "ProblemRecord.hpp"
#include "ProjectDatabase.hpp"

#include "../analysis/InputVariable.hpp"
#include "../analysis/InputVariable_Impl.hpp"
#include "../analysis/OutputVariable.hpp"
#include "../analysis/OutputVariable_Impl.hpp"

#include "../utilities/math/FloatCompare.hpp"
#include "../utilities/core/Assert.hpp"

#include <sstream>

#include <QSqlQuery>
#include <QSqlError>

namespace openstudio {
namespace project {

namespace detail{

  VariableRecord_Impl::VariableRecord_Impl(const analysis::Variable& variable,
                                           const VariableRecordType& variableRecordType,
                                           ProblemRecord& problemRecord,
                                           int workflowIndex)
    : ObjectRecord_Impl(problemRecord.projectDatabase(),
                        variable.uuid(),
                        variable.name(),
                        variable.displayName(),
                        variable.description(),
                        variable.versionUUID()),
      m_variableVectorIndex(workflowIndex),
      m_variableRecordType(variableRecordType)
  {}

  VariableRecord_Impl::VariableRecord_Impl(const analysis::Variable& variable,
                                           const VariableRecordType& variableRecordType,
                                           FunctionRecord& functionRecord,
                                           int variableVectorIndex,
                                           boost::optional<double> functionCoefficient)
    : ObjectRecord_Impl(functionRecord.projectDatabase(),
                        variable.uuid(),
                        variable.name(),
                        variable.displayName(),
                        variable.description(),
                        variable.versionUUID()),
      m_functionRecordId(functionRecord.id()),
      m_variableVectorIndex(variableVectorIndex),
      m_functionCoefficient(functionCoefficient),
      m_variableRecordType(variableRecordType)
  {}

  VariableRecord_Impl::VariableRecord_Impl(const QSqlQuery& query, const ProjectDatabase& database)
    : ObjectRecord_Impl(database, query)
  {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    QVariant value;

    value = query.value(VariableRecordColumns::functionRecordId);
    if (value.isValid() && !value.isNull()) {
      m_functionRecordId = value.toInt();
    }

    value = query.value(VariableRecordColumns::variableVectorIndex);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_variableVectorIndex = value.toInt();

    value = query.value(VariableRecordColumns::functionCoefficient);
    if (value.isValid() && !value.isNull()) {
      m_functionCoefficient = value.toDouble();
    }

    value = query.value(VariableRecordColumns::variableRecordType);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_variableRecordType = VariableRecordType(value.toInt());
  }

  std::string VariableRecord_Impl::databaseTableName() const
  {
    return VariableRecord::databaseTableName();
  }

  boost::optional<ObjectRecord> VariableRecord_Impl::parent() const
  {
    OptionalObjectRecord result;
    if (m_functionRecordId) {
      result = functionRecord().get();
    }
    return result;
  }

  std::vector<JoinRecord> VariableRecord_Impl::joinRecords() const {
    return JoinRecordVector();
  }

  boost::optional<FunctionRecord> VariableRecord_Impl::functionRecord() const {
    OptionalFunctionRecord result;
    if (m_functionRecordId) {
      ProjectDatabase database = projectDatabase();
      result = FunctionRecord::getFunctionRecord(*m_functionRecordId,database);
    }
    return result;
  }

  int VariableRecord_Impl::variableVectorIndex() const {
    return m_variableVectorIndex;
  }

  boost::optional<double> VariableRecord_Impl::functionCoefficient() const {
    return m_functionCoefficient;
  }

  void VariableRecord_Impl::bindValues(QSqlQuery& query) const
  {
    ObjectRecord_Impl::bindValues(query);

    if (m_functionRecordId) {
      query.bindValue(VariableRecordColumns::functionRecordId, *m_functionRecordId);
    }
    else {
      query.bindValue(VariableRecordColumns::functionRecordId, QVariant(QVariant::Int));
    }

    query.bindValue(VariableRecordColumns::variableVectorIndex, m_variableVectorIndex);

    if (m_functionCoefficient) {
      query.bindValue(VariableRecordColumns::functionCoefficient, *m_functionCoefficient);
    }
    else {
      query.bindValue(VariableRecordColumns::functionCoefficient, QVariant(QVariant::Double));
    }

    query.bindValue(VariableRecordColumns::variableRecordType, m_variableRecordType.value());

  }

  void VariableRecord_Impl::setLastValues(const QSqlQuery& query, ProjectDatabase& projectDatabase)
  {
    ObjectRecord_Impl::setLastValues(query, projectDatabase);

    QVariant value;

    value = query.value(VariableRecordColumns::functionRecordId);
    if (value.isValid() && !value.isNull()) {
      m_lastFunctionRecordId = value.toInt();
    }
    else {
      m_lastFunctionRecordId.reset();
    }

    value = query.value(VariableRecordColumns::variableVectorIndex);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_lastVariableVectorIndex = value.toInt();

    value = query.value(VariableRecordColumns::functionCoefficient);
    if (value.isValid() && !value.isNull()) {
      m_lastFunctionCoefficient = value.toDouble();
    }
    else {
      m_lastFunctionCoefficient.reset();
    }

    value = query.value(VariableRecordColumns::variableRecordType);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_lastVariableRecordType = VariableRecordType(value.toInt());
  }

  bool VariableRecord_Impl::compareValues(const QSqlQuery& query) const
  {
    bool result = true;

    result = result && ObjectRecord_Impl::compareValues(query);

    QVariant value;

    value = query.value(VariableRecordColumns::functionRecordId);
    if (value.isValid() && !value.isNull()) {
      result = result && m_functionRecordId && (*m_functionRecordId == value.toInt());
    }
    else {
      result = result && !m_functionRecordId;
    }

    value = query.value(VariableRecordColumns::variableVectorIndex);
    OS_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_variableVectorIndex == value.toInt());

    value = query.value(VariableRecordColumns::functionCoefficient);
    if (value.isValid() && !value.isNull()) {
      result = result && m_functionCoefficient && equal(*m_functionCoefficient,value.toDouble());
    }
    else {
      result = result && !m_functionCoefficient;
    }

    value = query.value(VariableRecordColumns::variableRecordType);
    OS_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_variableRecordType == VariableRecordType(value.toInt()));

    return result;
  }

  void VariableRecord_Impl::saveLastValues()
  {
    ObjectRecord_Impl::saveLastValues();

    m_lastFunctionRecordId = m_functionRecordId;
    m_lastVariableVectorIndex = m_variableVectorIndex;
    m_lastFunctionCoefficient = m_functionCoefficient;
    m_lastVariableRecordType = m_variableRecordType;
  }

  void VariableRecord_Impl::revertToLastValues()
  {
    ObjectRecord_Impl::revertToLastValues();

    m_functionRecordId = m_lastFunctionRecordId;
    m_variableVectorIndex = m_lastVariableVectorIndex;
    m_functionCoefficient = m_lastFunctionCoefficient;
    m_variableRecordType = m_lastVariableRecordType;
  }

} // detail

std::string VariableRecord::databaseTableName()
{
  return "VariableRecords";
}

UpdateByIdQueryData VariableRecord::updateByIdQueryData() {
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

void VariableRecord::createIndices(QSqlDatabase& qSqlDatabase, const std::string& databaseTableName)
{
  ObjectRecord::createIndices(qSqlDatabase,databaseTableName);

  QSqlQuery query(qSqlDatabase);
  query.prepare(toQString("CREATE INDEX " + databaseTableName + "problemRecordIdIndex ON " + databaseTableName + " (problemRecordId)"));
  assertExec(query);

  query.prepare(toQString("CREATE INDEX " + databaseTableName + "functionRecordIdIndex ON " + databaseTableName + " (functionRecordId)"));
  assertExec(query);
}

void VariableRecord::updatePathData(ProjectDatabase database,
                                    const openstudio::path& originalBase,
                                    const openstudio::path& newBase)
{}

boost::optional<VariableRecord> VariableRecord::factoryFromQuery(const QSqlQuery& query,
                                                                 ProjectDatabase& database)
{
  OptionalVariableRecord result;

  int variableRecordType = query.value(VariableRecordColumns::variableRecordType).toInt();

  switch (variableRecordType){
    case VariableRecordType::InputVariableRecord :
      result = InputVariableRecord::factoryFromQuery(query,database);
      break;
    case VariableRecordType::OutputVariableRecord :
      result = OutputVariableRecord::factoryFromQuery(query,database);
      break;
    default:
      LOG(Error, "Unknown VariableRecordType " << variableRecordType);
  }

  return result;
}

VariableRecord VariableRecord::factoryFromVariable(const analysis::Variable& variable,
                                                   FunctionRecord& functionRecord,
                                                   int variableVectorIndex,
                                                   boost::optional<double> functionCoefficient)
{
  if (analysis::OptionalInputVariable inputVariable = variable.optionalCast<analysis::InputVariable>())
  {
    return InputVariableRecord::factoryFromInputVariable(*inputVariable,
                                                         functionRecord,
                                                         variableVectorIndex,
                                                         functionCoefficient);
  }
  if (analysis::OptionalOutputVariable outputVariable = variable.optionalCast<analysis::OutputVariable>())
  {
    return OutputVariableRecord::factoryFromOutputVariable(*outputVariable,
                                                           functionRecord,
                                                           variableVectorIndex,
                                                           functionCoefficient);
  }

  OS_ASSERT(false);
  return VariableRecord(std::shared_ptr<detail::VariableRecord_Impl>());
}

std::vector<VariableRecord> VariableRecord::getVariableRecords(ProjectDatabase& database)
{
  std::vector<VariableRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + VariableRecord::databaseTableName()));
  assertExec(query);
  while (query.next()) {
    boost::optional<VariableRecord> variable = factoryFromQuery(query, database);
    OS_ASSERT(variable);
    result.push_back(*variable);
  }

  return result;
}

boost::optional<VariableRecord> VariableRecord::getVariableRecord(int id, ProjectDatabase& database)
{
  boost::optional<VariableRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + VariableRecord::databaseTableName() + " WHERE id=:id"));
  query.bindValue(":id", id);
  assertExec(query);
  if (query.first()) {
    result = factoryFromQuery(query, database);
  }

  return result;
}

boost::optional<FunctionRecord> VariableRecord::functionRecord() const {
  return getImpl<detail::VariableRecord_Impl>()->functionRecord();
}

int VariableRecord::variableVectorIndex() const {
  return getImpl<detail::VariableRecord_Impl>()->variableVectorIndex();
}

boost::optional<double> VariableRecord::functionCoefficient() const {
  return getImpl<detail::VariableRecord_Impl>()->functionCoefficient();
}

analysis::Variable VariableRecord::variable() const {
  return getImpl<detail::VariableRecord_Impl>()->variable();
}

VariableRecord::VariableRecord(std::shared_ptr<detail::VariableRecord_Impl> impl,
                               ProjectDatabase projectDatabase)
  : ObjectRecord(impl, projectDatabase)
{
  OS_ASSERT(getImpl<detail::VariableRecord_Impl>());
}

VariableRecord::VariableRecord(std::shared_ptr<detail::VariableRecord_Impl> impl)
  : ObjectRecord(impl)
{
  OS_ASSERT(getImpl<detail::VariableRecord_Impl>());
}

bool VariableRecordVariableVectorIndexLess::operator()(const VariableRecord& left,
                                                       const VariableRecord& right) const
{
  return (left.variableVectorIndex() < right.variableVectorIndex());
}

} // project
} // openstudio

