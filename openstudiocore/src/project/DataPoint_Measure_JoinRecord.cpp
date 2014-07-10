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

#include "DataPoint_Measure_JoinRecord.hpp"
#include "DataPoint_Measure_JoinRecord_Impl.hpp"
#include "DataPointRecord.hpp"
#include "MeasureRecord.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace project {

namespace detail {

  DataPoint_Measure_JoinRecord_Impl::DataPoint_Measure_JoinRecord_Impl(
      const DataPointRecord& dataPointRecord,
      const MeasureRecord& measureRecord)
    : JoinRecord_Impl(dataPointRecord.id(), 
                      dataPointRecord.handle(), 
                      measureRecord.id(),
                      measureRecord.handle(),
                      dataPointRecord.projectDatabase())
  {
    OS_ASSERT(dataPointRecord.projectDatabase().handle() == measureRecord.projectDatabase().handle());
  }

  DataPoint_Measure_JoinRecord_Impl::DataPoint_Measure_JoinRecord_Impl(
      const QSqlQuery& query, ProjectDatabase& database)
    : JoinRecord_Impl(query, database)
  {}

  std::string DataPoint_Measure_JoinRecord_Impl::databaseTableName() const {
    return DataPoint_Measure_JoinRecord::databaseTableName();
  }

  void DataPoint_Measure_JoinRecord_Impl::saveRow(const std::shared_ptr<QSqlDatabase> &database)
  {
    QSqlQuery query(*database);
    this->makeUpdateByIdQuery<DataPoint_Measure_JoinRecord>(query);
    this->bindValues(query);
    assertExec(query);
  }

  ObjectRecord DataPoint_Measure_JoinRecord_Impl::leftObject() const {
    return this->dataPointRecord();
  }

  ObjectRecord DataPoint_Measure_JoinRecord_Impl::rightObject() const {
    return this->measureRecord();
  }

  DataPointRecord DataPoint_Measure_JoinRecord_Impl::dataPointRecord() const {
    ProjectDatabase database = this->projectDatabase();
    boost::optional<DataPointRecord> dataPointRecord = DataPointRecord::getDataPointRecord(this->leftId(),database);
    OS_ASSERT(dataPointRecord);
    return *dataPointRecord;
  }

  MeasureRecord DataPoint_Measure_JoinRecord_Impl::measureRecord() const {
    ProjectDatabase database = this->projectDatabase();
    boost::optional<MeasureRecord> measureRecord = MeasureRecord::getMeasureRecord(this->rightId(),database);
    OS_ASSERT(measureRecord);
    return *measureRecord;
  }

} // detail

DataPoint_Measure_JoinRecord::DataPoint_Measure_JoinRecord(
    const DataPointRecord& dataPointRecord,
    const MeasureRecord& measureRecord)
  : JoinRecord(std::shared_ptr<detail::DataPoint_Measure_JoinRecord_Impl>(
        new detail::DataPoint_Measure_JoinRecord_Impl(dataPointRecord, measureRecord)),
        dataPointRecord.projectDatabase())
{
  OS_ASSERT(getImpl<detail::DataPoint_Measure_JoinRecord_Impl>());
}

DataPoint_Measure_JoinRecord::DataPoint_Measure_JoinRecord(
    std::shared_ptr<detail::DataPoint_Measure_JoinRecord_Impl> impl,
    ProjectDatabase database)
  : JoinRecord(impl, database)
{
  OS_ASSERT(getImpl<detail::DataPoint_Measure_JoinRecord_Impl>());
}

std::string DataPoint_Measure_JoinRecord::databaseTableName() {
  return "DataPoint_Measure_JoinRecords";
}

UpdateByIdQueryData DataPoint_Measure_JoinRecord::updateByIdQueryData() {
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

std::vector<MeasureRecord>
DataPoint_Measure_JoinRecord::getMeasureRecords(
    const DataPointRecord& dataPointRecord) 
{
  ProjectDatabase database = dataPointRecord.projectDatabase();

  std::vector<MeasureRecord> result;
  std::vector<int> measureRecordIds;

  QSqlQuery query(*database.qSqlDatabase());
  query.prepare(toQString("SELECT * FROM " + 
      DataPoint_Measure_JoinRecord::databaseTableName() +
      " WHERE leftId=:leftId"));
  query.bindValue(":leftId", dataPointRecord.id());
  assertExec(query);
  while (query.next()) {
    QVariant value;
    value = query.value(JoinRecordColumns::rightId);
    OS_ASSERT(value.isValid() && !value.isNull());
    measureRecordIds.push_back(value.toInt());
  }

  for (int id : measureRecordIds) {
    boost::optional<MeasureRecord> measureRecord =
        MeasureRecord::getMeasureRecord(id,database);
    OS_ASSERT(measureRecord);
    result.push_back(*measureRecord);
  }

  return result;
}

std::vector<DataPointRecord> DataPoint_Measure_JoinRecord::getDataPointRecords(
    const MeasureRecord& measureRecord)
{
  ProjectDatabase database = measureRecord.projectDatabase();

  std::vector<DataPointRecord> result;
  std::vector<int> dataPointRecordIds;

  QSqlQuery query(*database.qSqlDatabase());
  query.prepare(toQString("SELECT * FROM " + 
      DataPoint_Measure_JoinRecord::databaseTableName() + " WHERE rightId=:rightId"));
  query.bindValue(":rightId", measureRecord.id());
  assertExec(query);
  while (query.next()) {
    QVariant value;
    value = query.value(JoinRecordColumns::leftId);
    OS_ASSERT(value.isValid() && !value.isNull());
    dataPointRecordIds.push_back(value.toInt());
  }

  for (int id : dataPointRecordIds) {
    boost::optional<DataPointRecord> dataPointRecord = 
        DataPointRecord::getDataPointRecord(id,database);
    OS_ASSERT(dataPointRecord);
    result.push_back(*dataPointRecord);
  }

  return result;
}

DataPointRecord DataPoint_Measure_JoinRecord::dataPointRecord() const {
  return getImpl<detail::DataPoint_Measure_JoinRecord_Impl>()->dataPointRecord();
}

MeasureRecord DataPoint_Measure_JoinRecord::measureRecord() const {
  return getImpl<detail::DataPoint_Measure_JoinRecord_Impl>()->measureRecord();
}

/// @cond
DataPoint_Measure_JoinRecord::DataPoint_Measure_JoinRecord(
    std::shared_ptr<detail::DataPoint_Measure_JoinRecord_Impl> impl)
  : JoinRecord(impl)
{}
/// @endcond


} // project
} // openstudio


