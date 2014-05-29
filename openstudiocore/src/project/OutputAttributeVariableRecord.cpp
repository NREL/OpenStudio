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

#include "OutputAttributeVariableRecord.hpp"
#include "OutputAttributeVariableRecord_Impl.hpp"

#include "JoinRecord.hpp"
#include "FunctionRecord.hpp"

#include "../analysis/OutputAttributeVariable.hpp"
#include "../analysis/OutputAttributeVariable_Impl.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace project {

namespace detail {

  OutputAttributeVariableRecord_Impl::OutputAttributeVariableRecord_Impl(
      const analysis::OutputAttributeVariable& outputAttributeVariable,
      FunctionRecord& functionRecord,
      int variableVectorIndex,
      boost::optional<double> functionCoefficient)
    : OutputVariableRecord_Impl(outputAttributeVariable,
                                OutputVariableRecordType::OutputAttributeVariableRecord,
                                functionRecord,
                                variableVectorIndex,
                                functionCoefficient),
      m_attributeName(outputAttributeVariable.attributeName())
  {}

  OutputAttributeVariableRecord_Impl::OutputAttributeVariableRecord_Impl(
      const QSqlQuery& query, ProjectDatabase& database)
    : OutputVariableRecord_Impl(query, database)
  {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    QVariant value;

    value = query.value(OutputAttributeVariableRecord::ColumnsType::attributeName);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_attributeName = value.toString().toStdString();
  }

  std::vector<ObjectRecord> OutputAttributeVariableRecord_Impl::children() const {
    ObjectRecordVector result;
    return result;
  }

  std::vector<ObjectRecord> OutputAttributeVariableRecord_Impl::resources() const {
    ObjectRecordVector result;
    return result;
  }

  void OutputAttributeVariableRecord_Impl::saveRow(const std::shared_ptr<QSqlDatabase> &database)
  {
    QSqlQuery query(*database);
    this->makeUpdateByIdQuery<OutputAttributeVariableRecord>(query);
    this->bindValues(query);
    assertExec(query);
  }

  analysis::Variable OutputAttributeVariableRecord_Impl::variable() const {
    return outputAttributeVariable().cast<analysis::Variable>();
  }

  analysis::OutputVariable OutputAttributeVariableRecord_Impl::outputVariable() const
  {
    return outputAttributeVariable().cast<analysis::OutputVariable>();
  }

  analysis::OutputAttributeVariable
  OutputAttributeVariableRecord_Impl::outputAttributeVariable() const
  {
    return analysis::OutputAttributeVariable(handle(),
                                             uuidLast(),
                                             name(),
                                             displayName(),
                                             description(),
                                             m_attributeName);
  }

  void OutputAttributeVariableRecord_Impl::bindValues(QSqlQuery& query) const {
    OutputVariableRecord_Impl::bindValues(query);

    query.bindValue(OutputAttributeVariableRecord::ColumnsType::attributeName,
                    toQString(m_attributeName));
  }

  void OutputAttributeVariableRecord_Impl::setLastValues(const QSqlQuery& query, ProjectDatabase& projectDatabase) {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    OutputVariableRecord_Impl::setLastValues(query,projectDatabase);

    QVariant value;

    value = query.value(OutputAttributeVariableRecord::ColumnsType::attributeName);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_lastAttributeName = value.toString().toStdString();
  }

  bool OutputAttributeVariableRecord_Impl::compareValues(const QSqlQuery& query) const {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    bool result = OutputVariableRecord_Impl::compareValues(query);

    QVariant value;

    value = query.value(OutputAttributeVariableRecord::ColumnsType::attributeName);
    OS_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_attributeName == value.toString().toStdString());

    return result;
  }

  void OutputAttributeVariableRecord_Impl::saveLastValues() {
    OutputVariableRecord_Impl::saveLastValues();

    m_lastAttributeName = m_attributeName;
  }

  void OutputAttributeVariableRecord_Impl::revertToLastValues() {
    OutputVariableRecord_Impl::revertToLastValues();

    m_attributeName = m_lastAttributeName;
  }

} // detail

OutputAttributeVariableRecord::OutputAttributeVariableRecord(
    const analysis::OutputAttributeVariable& outputAttributeVariable,
    FunctionRecord& functionRecord,
    int variableVectorIndex,
    boost::optional<double> functionCoefficient)
  : OutputVariableRecord(std::shared_ptr<detail::OutputAttributeVariableRecord_Impl>(
        new detail::OutputAttributeVariableRecord_Impl(outputAttributeVariable,
                                                       functionRecord,
                                                       variableVectorIndex,
                                                       functionCoefficient)),
        functionRecord.projectDatabase())
{
  OS_ASSERT(getImpl<detail::OutputAttributeVariableRecord_Impl>());
}

OutputAttributeVariableRecord::OutputAttributeVariableRecord(
    const QSqlQuery& query, ProjectDatabase& database)
  : OutputVariableRecord(std::shared_ptr<detail::OutputAttributeVariableRecord_Impl>(
        new detail::OutputAttributeVariableRecord_Impl(query, database)),
        database)
{
  OS_ASSERT(getImpl<detail::OutputAttributeVariableRecord_Impl>());
}

OutputAttributeVariableRecord::OutputAttributeVariableRecord(
    std::shared_ptr<detail::OutputAttributeVariableRecord_Impl> impl,
    ProjectDatabase database)
  : OutputVariableRecord(impl, database)
{
  OS_ASSERT(getImpl<detail::OutputAttributeVariableRecord_Impl>());
}

boost::optional<OutputAttributeVariableRecord> OutputAttributeVariableRecord::factoryFromQuery(
    const QSqlQuery& query, ProjectDatabase& database)
{
  OptionalOutputAttributeVariableRecord result;
  try {
    result = OutputAttributeVariableRecord(query,database);
  }
  catch (const std::exception& e) {
    LOG(Error,"Unable to construct OutputAttributeVariableRecord from query, because '"
        << e.what() << "'.");
  }
  return result;
}

std::vector<OutputAttributeVariableRecord>
OutputAttributeVariableRecord::getOutputAttributeVariableRecords(
    ProjectDatabase& database)
{
  std::vector<OutputAttributeVariableRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + VariableRecord::databaseTableName() +
      " WHERE variableRecordType=:variableRecordType AND " +
      "outputVariableRecordType=:outputVariableRecordType"));
  query.bindValue(":variableRecordType", VariableRecordType::OutputVariableRecord);
  query.bindValue(":outputVariableRecordType",OutputVariableRecordType::OutputAttributeVariableRecord);
  assertExec(query);
  while (query.next()) {
    result.push_back(OutputAttributeVariableRecord(query, database));
  }

  return result;
}

boost::optional<OutputAttributeVariableRecord>
OutputAttributeVariableRecord::getOutputAttributeVariableRecord(
    int id, ProjectDatabase& database)
{
  boost::optional<OutputAttributeVariableRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + VariableRecord::databaseTableName() +
      " WHERE variableRecordType=:variableRecordType AND " +
      "outputVariableRecordType=:outputVariableRecordType AND id=:id"));
  query.bindValue(":variableRecordType", VariableRecordType::OutputVariableRecord);
  query.bindValue(":outputVariableRecordType",OutputVariableRecordType::OutputAttributeVariableRecord);
  query.bindValue(":id",id);
  assertExec(query);
  if (query.first()) {
    result = OutputAttributeVariableRecord(query, database);
  }

  return result;
}

analysis::OutputAttributeVariable
OutputAttributeVariableRecord::outputAttributeVariable() const
{
  return getImpl<detail::OutputAttributeVariableRecord_Impl>()->outputAttributeVariable();
}

/// @cond
OutputAttributeVariableRecord::OutputAttributeVariableRecord(std::shared_ptr<detail::OutputAttributeVariableRecord_Impl> impl)
  : OutputVariableRecord(impl)
{}
/// @endcond

} // project
} // openstudio

