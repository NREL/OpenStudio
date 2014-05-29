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

#include "DiscreteVariableRecord.hpp"
#include "DiscreteVariableRecord_Impl.hpp"

#include "JoinRecord.hpp"
#include "MeasureGroupRecord.hpp"
#include "MeasureGroupRecord_Impl.hpp"

#include "../analysis/MeasureGroup.hpp"
#include "../analysis/MeasureGroup_Impl.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace project {

namespace detail {

  DiscreteVariableRecord_Impl::DiscreteVariableRecord_Impl(const analysis::DiscreteVariable& discreteVariable,
                                                           const DiscreteVariableRecordType& discreteVariableRecordType,
                                                           ProblemRecord& problemRecord,
                                                           int workflowIndex)
    : InputVariableRecord_Impl(discreteVariable,
                               InputVariableRecordType::DiscreteVariableRecord,
                               problemRecord,
                               workflowIndex),
      m_discreteVariableRecordType(discreteVariableRecordType)
  {
  }

  DiscreteVariableRecord_Impl::DiscreteVariableRecord_Impl(const analysis::DiscreteVariable& discreteVariable,
                                                           const DiscreteVariableRecordType& discreteVariableRecordType,
                                                           FunctionRecord& functionRecord,
                                                           int variableVectorIndex,
                                                           boost::optional<double> functionCoefficient)
    : InputVariableRecord_Impl(discreteVariable,
                               InputVariableRecordType::DiscreteVariableRecord,
                               functionRecord,
                               variableVectorIndex,
                               functionCoefficient),
      m_discreteVariableRecordType(discreteVariableRecordType)
  {
  }

  DiscreteVariableRecord_Impl::DiscreteVariableRecord_Impl(const QSqlQuery& query, ProjectDatabase& database)
    : InputVariableRecord_Impl(query, database)
  {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    QVariant value;

    value = query.value(DiscreteVariableRecord::ColumnsType::discreteVariableRecordType);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_discreteVariableRecordType = DiscreteVariableRecordType(value.toInt());
  }

  void DiscreteVariableRecord_Impl::saveRow(ProjectDatabase& projectDatabase) {
    QSqlQuery query(*(projectDatabase.qSqlDatabase()));
    this->makeUpdateByIdQuery<DiscreteVariableRecord>(query);
    this->bindValues(query);
    assertExec(query);
  }

  void DiscreteVariableRecord_Impl::bindValues(QSqlQuery& query) const {
    InputVariableRecord_Impl::bindValues(query);

    query.bindValue(DiscreteVariableRecord::ColumnsType::discreteVariableRecordType,m_discreteVariableRecordType.value());
  }

  void DiscreteVariableRecord_Impl::setLastValues(const QSqlQuery& query, ProjectDatabase& projectDatabase) {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    InputVariableRecord_Impl::setLastValues(query,projectDatabase);

    QVariant value;

    value = query.value(DiscreteVariableRecord::ColumnsType::discreteVariableRecordType);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_lastDiscreteVariableRecordType = DiscreteVariableRecordType(value.toInt());
  }

  bool DiscreteVariableRecord_Impl::compareValues(const QSqlQuery& query) const {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    bool result = InputVariableRecord_Impl::compareValues(query);

    QVariant value;

    value = query.value(DiscreteVariableRecord::ColumnsType::discreteVariableRecordType);
    OS_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_discreteVariableRecordType == DiscreteVariableRecordType(value.toInt()));

    return result;
  }

  void DiscreteVariableRecord_Impl::saveLastValues() {
    InputVariableRecord_Impl::saveLastValues();

    m_lastDiscreteVariableRecordType = m_discreteVariableRecordType;
  }

  void DiscreteVariableRecord_Impl::revertToLastValues() {
    InputVariableRecord_Impl::revertToLastValues();

    m_discreteVariableRecordType = m_lastDiscreteVariableRecordType;
  }

} // detail

boost::optional<DiscreteVariableRecord> DiscreteVariableRecord::factoryFromQuery(
    const QSqlQuery& query,
    ProjectDatabase& database)
{
  OptionalDiscreteVariableRecord result;

  int discreteVariableRecordType = query.value(VariableRecordColumns::discreteVariableRecordType).toInt();

  switch (discreteVariableRecordType) {
    case DiscreteVariableRecordType::MeasureGroupRecord :
      result = MeasureGroupRecord(query, database).cast<DiscreteVariableRecord>();
     break;
    default :
      LOG(Error,"Unknown DiscreteVariableRecordType " << discreteVariableRecordType);
      return boost::none;
  }

  return result;
}

DiscreteVariableRecord DiscreteVariableRecord::factoryFromDiscreteVariable(
    const analysis::DiscreteVariable& discreteVariable,
    ProblemRecord& problemRecord,
    int workflowIndex)
{
  if (discreteVariable.optionalCast<analysis::MeasureGroup>()) {
    return MeasureGroupRecord(discreteVariable.cast<analysis::MeasureGroup>(),
                              problemRecord,
                              workflowIndex);
  }

  OS_ASSERT(false);
  return DiscreteVariableRecord(std::shared_ptr<detail::DiscreteVariableRecord_Impl>());
}

DiscreteVariableRecord DiscreteVariableRecord::factoryFromDiscreteVariable(
    const analysis::DiscreteVariable& discreteVariable,
    FunctionRecord& functionRecord,
    int variableVectorIndex,
    boost::optional<double> functionCoefficient)
{
  if (discreteVariable.optionalCast<analysis::MeasureGroup>()) {
    return MeasureGroupRecord(discreteVariable.cast<analysis::MeasureGroup>(),
                              functionRecord,
                              variableVectorIndex,
                              functionCoefficient);
  }

  OS_ASSERT(false);
  return DiscreteVariableRecord(std::shared_ptr<detail::DiscreteVariableRecord_Impl>());
}

std::vector<DiscreteVariableRecord> DiscreteVariableRecord::getDiscreteVariableRecords(
    ProjectDatabase& database)
{
  std::vector<DiscreteVariableRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + VariableRecord::databaseTableName() +
                          " WHERE variableRecordType=:variableRecordType AND " +
                          "inputVariableRecordType=:inputVariableRecordType"));
  query.bindValue(":variableRecordType", VariableRecordType::InputVariableRecord);
  query.bindValue(":inputVariableRecordType", InputVariableRecordType::DiscreteVariableRecord);
  assertExec(query);
  while (query.next()) {
    OptionalDiscreteVariableRecord discreteVariableRecord = DiscreteVariableRecord::factoryFromQuery(query, database);
    if (discreteVariableRecord) {
      result.push_back(*discreteVariableRecord);
    }
  }

  return result;
}

boost::optional<DiscreteVariableRecord> DiscreteVariableRecord::getDiscreteVariableRecord(int id, ProjectDatabase& database) {
  boost::optional<DiscreteVariableRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + VariableRecord::databaseTableName() +
                          " WHERE variableRecordType=:variableRecordType AND " +
                          "inputVariableRecordType=:inputVariableRecordType AND " +
                          "id=:id"));
  query.bindValue(":variableRecordType", VariableRecordType::InputVariableRecord);
  query.bindValue(":inputVariableRecordType", InputVariableRecordType::DiscreteVariableRecord);
  query.bindValue(":id",id);
  assertExec(query);
  if (query.first()) {
    result = DiscreteVariableRecord::factoryFromQuery(query, database);
  }

  return result;
}

analysis::DiscreteVariable DiscreteVariableRecord::discreteVariable() const {
  return getImpl<detail::DiscreteVariableRecord_Impl>()->discreteVariable();
}

/// @cond
DiscreteVariableRecord::DiscreteVariableRecord(std::shared_ptr<detail::DiscreteVariableRecord_Impl> impl)
  : InputVariableRecord(impl)
{}

DiscreteVariableRecord::DiscreteVariableRecord(std::shared_ptr<detail::DiscreteVariableRecord_Impl> impl,
                                               ProjectDatabase database,
                                               const boost::optional<analysis::DiscreteVariable>& discreteVariable)
  : InputVariableRecord(impl, database, discreteVariable ? discreteVariable->cast<analysis::InputVariable>() : analysis::OptionalInputVariable())
{
  OS_ASSERT(getImpl<detail::DiscreteVariableRecord_Impl>());
}
/// @endcond

} // project
} // openstudio


