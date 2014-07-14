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

#include "MeasureRecord.hpp"
#include "MeasureRecord_Impl.hpp"
#include "MeasureGroupRecord.hpp"
#include "RubyMeasureRecord.hpp"
#include "RubyMeasureRecord_Impl.hpp"
#include "NullMeasureRecord.hpp"
#include "NullMeasureRecord_Impl.hpp"
#include "ProjectDatabase.hpp"
#include "JoinRecord.hpp"

#include "../analysis/Measure.hpp"
#include "../analysis/NullMeasure.hpp"
#include "../analysis/NullMeasure_Impl.hpp"
#include "../analysis/RubyMeasure.hpp"
#include "../analysis/RubyMeasure_Impl.hpp"

#include "../utilities/core/Assert.hpp"

#include <sstream>

#include <QSqlQuery>
#include <QSqlError>

namespace openstudio {
namespace project {

namespace detail{

  MeasureRecord_Impl::MeasureRecord_Impl(const analysis::Measure& measure,
                                         const MeasureRecordType& measureRecordType,
                                         const MeasureGroupRecord& measureGroupRecord,
                                         int measureVectorIndex)
    : ObjectRecord_Impl(measureGroupRecord.projectDatabase(),
                        measure.uuid(),
                        measure.name(),
                        measure.displayName(),
                        measure.description(),
                        measure.versionUUID()),
      m_measureRecordType(measureRecordType),
      m_variableRecordId(measureGroupRecord.id()),
      m_isSelected(measure.isSelected()),
      m_measureVectorIndex(measureVectorIndex)
  {}

  MeasureRecord_Impl::MeasureRecord_Impl(const analysis::Measure& measure,
                                         const MeasureRecordType& measureRecordType,
                                         ProjectDatabase& database)
    : ObjectRecord_Impl(database,
                        measure.uuid(),
                        measure.name(),
                        measure.displayName(),
                        measure.description(),
                        measure.versionUUID()),
      m_measureRecordType(measureRecordType),
      m_isSelected(measure.isSelected())
  {}

  MeasureRecord_Impl::MeasureRecord_Impl(const QSqlQuery& query, const ProjectDatabase& database)
    : ObjectRecord_Impl(database, query)
  {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    QVariant value;
    value = query.value(MeasureRecordColumns::measureRecordType);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_measureRecordType = MeasureRecordType(value.toInt());

    value = query.value(MeasureRecordColumns::variableRecordId);
    if (value.isValid() && !value.isNull()) {
      m_variableRecordId = value.toInt();
    }

    value = query.value(MeasureRecordColumns::isSelected);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_isSelected = value.toBool();

    value = query.value(MeasureRecordColumns::measureVectorIndex);
    if (value.isValid() && !value.isNull()) {
      m_measureVectorIndex = value.toInt();
    }
  }

  std::string MeasureRecord_Impl::databaseTableName() const
  {
    return MeasureRecord::databaseTableName();
  }

  boost::optional<ObjectRecord> MeasureRecord_Impl::parent() const
  {
    OptionalObjectRecord result;
    if (OptionalMeasureGroupRecord dvr = this->measureGroupRecord()) {
      result = *dvr;
    }
    return result;
  }

  std::vector<ObjectRecord> MeasureRecord_Impl::children() const {
    return ObjectRecordVector();
  }

  std::vector<ObjectRecord> MeasureRecord_Impl::resources() const {
    return ObjectRecordVector();
  }

  std::vector<JoinRecord> MeasureRecord_Impl::joinRecords() const
  {
    return std::vector<JoinRecord>();
  }

  boost::optional<int> MeasureRecord_Impl::variableRecordId() const {
    return m_variableRecordId;
  }

  boost::optional<MeasureGroupRecord> MeasureRecord_Impl::measureGroupRecord() const
  {
    OptionalMeasureGroupRecord result;

    if (m_variableRecordId) {
      ProjectDatabase database = this->projectDatabase();
      result = MeasureGroupRecord::getMeasureGroupRecord(*m_variableRecordId, database);
      OS_ASSERT(result);
    }

    return result;
  }

  bool MeasureRecord_Impl::isSelected() const
  {
    return m_isSelected;
  }

  bool MeasureRecord_Impl::setIsSelected(bool isSelected)
  {
    if (m_isSelected != isSelected){
      m_isSelected = isSelected;
      this->onChange();
    }
    return true;
  }

  boost::optional<int> MeasureRecord_Impl::measureVectorIndex() const {
    return m_measureVectorIndex;
  }

  void MeasureRecord_Impl::bindValues(QSqlQuery& query) const
  {
    ObjectRecord_Impl::bindValues(query);

    query.bindValue(MeasureRecordColumns::measureRecordType, m_measureRecordType.value());
    if (m_variableRecordId) {
      query.bindValue(MeasureRecordColumns::variableRecordId, *m_variableRecordId);
    }
    else {
      query.bindValue(MeasureRecordColumns::variableRecordId, QVariant(QVariant::Int));
    }
    query.bindValue(MeasureRecordColumns::isSelected, m_isSelected);
    if (m_measureVectorIndex) {
      query.bindValue(MeasureRecordColumns::measureVectorIndex, *m_measureVectorIndex);
    }
    else {
      query.bindValue(MeasureRecordColumns::measureVectorIndex, QVariant(QVariant::Int));
    }
  }

  void MeasureRecord_Impl::setLastValues(const QSqlQuery& query, ProjectDatabase& projectDatabase)
  {
    ObjectRecord_Impl::setLastValues(query, projectDatabase);

    QVariant value;
    value = query.value(MeasureRecordColumns::measureRecordType);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_lastMeasureRecordType = MeasureRecordType(value.toInt());

    value = query.value(MeasureRecordColumns::variableRecordId);
    if (value.isValid() && !value.isNull()) {
      m_lastVariableRecordId = value.toInt();
    }
    else {
      m_lastVariableRecordId.reset();
    }

    value = query.value(MeasureRecordColumns::isSelected);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_lastIsSelected = value.toBool();

    value = query.value(MeasureRecordColumns::measureVectorIndex);
    if (value.isValid() && !value.isNull()) {
      m_lastMeasureVectorIndex = value.toInt();
    }
    else {
      m_lastMeasureVectorIndex.reset();
    }
  }

  bool MeasureRecord_Impl::compareValues(const QSqlQuery& query) const
  {
    bool result = true;

    result = result && ObjectRecord_Impl::compareValues(query);

    QVariant value;
    value = query.value(MeasureRecordColumns::measureRecordType);
    OS_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_measureRecordType == MeasureRecordType(value.toInt()));

    value = query.value(MeasureRecordColumns::variableRecordId);
    if (value.isValid() && !value.isNull()) {
      result = result && m_variableRecordId && (*m_variableRecordId == value.toInt());
    }
    else {
      result = result && !m_variableRecordId;
    }

    value = query.value(MeasureRecordColumns::isSelected);
    OS_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_isSelected == value.toBool());

    value = query.value(MeasureRecordColumns::measureVectorIndex);
    if (value.isValid() && !value.isNull()) {
      result = result && m_measureVectorIndex && (*m_measureVectorIndex == value.toInt());
    }
    else {
      result = result && !m_measureVectorIndex;
    }

    return result;
  }

  void MeasureRecord_Impl::saveLastValues()
  {
    ObjectRecord_Impl::saveLastValues();

    m_lastMeasureRecordType = m_measureRecordType;
    m_lastVariableRecordId = m_variableRecordId;
    m_lastIsSelected = m_isSelected;
    m_lastMeasureVectorIndex = m_measureVectorIndex;
  }

  void MeasureRecord_Impl::revertToLastValues()
  {
    ObjectRecord_Impl::revertToLastValues();

    m_measureRecordType = m_lastMeasureRecordType;
    m_variableRecordId = m_lastVariableRecordId;
    m_isSelected = m_lastIsSelected;
    m_measureVectorIndex = m_lastMeasureVectorIndex;
  }

} // detail

std::string MeasureRecord::databaseTableName()
{
  return "MeasureRecords";
}

UpdateByIdQueryData MeasureRecord::updateByIdQueryData() {
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

void MeasureRecord::updatePathData(ProjectDatabase database,
                                                const openstudio::path& originalBase,
                                                const openstudio::path& newBase)
{}

MeasureRecord::MeasureRecord(std::shared_ptr<detail::MeasureRecord_Impl> impl, ProjectDatabase database)
  : ObjectRecord(impl, database)
{
  OS_ASSERT(getImpl<detail::MeasureRecord_Impl>());
}

MeasureRecord::MeasureRecord(std::shared_ptr<detail::MeasureRecord_Impl> impl)
  : ObjectRecord(impl)
{
  OS_ASSERT(getImpl<detail::MeasureRecord_Impl>());
}

std::vector<MeasureRecord> MeasureRecord::getMeasureRecords(ProjectDatabase& database)
{
  std::vector<MeasureRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + MeasureRecord::databaseTableName()));
  assertExec(query);
  while (query.next()) {
    boost::optional<MeasureRecord> measure = factoryFromQuery(query, database);
    OS_ASSERT(measure);
    result.push_back(*measure);
  }

  return result;
}

boost::optional<MeasureRecord> MeasureRecord::getMeasureRecord(int id, ProjectDatabase& database)
{
  boost::optional<MeasureRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + MeasureRecord::databaseTableName() + " WHERE id=:id"));
  query.bindValue(":id", id);
  assertExec(query);
  if(query.first()) {
    boost::optional<MeasureRecord> measure = factoryFromQuery(query, database);
    OS_ASSERT(measure);
    result = measure;
  }

  return result;
}

boost::optional<MeasureRecord> MeasureRecord::factoryFromQuery(const QSqlQuery& query, ProjectDatabase& database)
{
  std::shared_ptr<detail::MeasureRecord_Impl> impl;

  int measureRecordType = query.value(MeasureRecordColumns::measureRecordType).toInt();

  switch (measureRecordType){
    case MeasureRecordType::RubyMeasureRecord:
      impl = std::shared_ptr<detail::RubyMeasureRecord_Impl>(new detail::RubyMeasureRecord_Impl(query, database));
      break;
    case MeasureRecordType::NullMeasureRecord:
      impl = std::shared_ptr<detail::NullMeasureRecord_Impl>(new detail::NullMeasureRecord_Impl(query, database));
      break;
    default:
      LOG(Error, "Unknown measureRecordType " << measureRecordType);
      return boost::none;
  }

  return MeasureRecord(impl, database);
}

MeasureRecord MeasureRecord::factoryFromMeasure(
    const analysis::Measure& measure,
    MeasureGroupRecord& measureGroupRecord,
    int measureVectorIndex)
{
  if (measure.optionalCast<analysis::NullMeasure>()) {
    return NullMeasureRecord(measure.cast<analysis::NullMeasure>(),
                                  measureGroupRecord,
                                  measureVectorIndex);
  }
  else if (measure.optionalCast<analysis::RubyMeasure>()) {
    return RubyMeasureRecord(measure.cast<analysis::RubyMeasure>(),
                                  measureGroupRecord,
                                  measureVectorIndex);
  }

  OS_ASSERT(false);
  return MeasureRecord(std::shared_ptr<detail::MeasureRecord_Impl>());
}

boost::optional<int> MeasureRecord::variableRecordId() const {
  return getImpl<detail::MeasureRecord_Impl>()->variableRecordId();
}

boost::optional<MeasureGroupRecord> MeasureRecord::measureGroupRecord() const
{
  return getImpl<detail::MeasureRecord_Impl>()->measureGroupRecord();
}

bool MeasureRecord::isSelected() const
{
  return getImpl<detail::MeasureRecord_Impl>()->isSelected();
}

bool MeasureRecord::setIsSelected(bool isSelected)
{
  return getImpl<detail::MeasureRecord_Impl>()->setIsSelected(isSelected);
}

boost::optional<int> MeasureRecord::measureVectorIndex() const {
  return getImpl<detail::MeasureRecord_Impl>()->measureVectorIndex();
}

analysis::Measure MeasureRecord::measure() const {
  return getImpl<detail::MeasureRecord_Impl>()->measure();
}

bool MeasureRecordMeasureVectorIndexLess::operator()(
    const MeasureRecord& left,
    const MeasureRecord& right) const
{
  OptionalInt leftIndex = left.measureVectorIndex();
  OptionalInt rightIndex = right.measureVectorIndex();
  if (leftIndex && rightIndex) {
    return (leftIndex.get() < rightIndex.get());
  }
  return false;
}

} // project
} // openstudio

