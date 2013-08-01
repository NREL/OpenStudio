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

#include <project/DataPoint_DiscretePerturbation_JoinRecord.hpp>
#include <project/DataPoint_DiscretePerturbation_JoinRecord_Impl.hpp>
#include <project/DataPointRecord.hpp>
#include <project/DiscretePerturbationRecord.hpp>

#include <utilities/core/Assert.hpp>

namespace openstudio {
namespace project {

namespace detail {

  DataPoint_DiscretePerturbation_JoinRecord_Impl::DataPoint_DiscretePerturbation_JoinRecord_Impl(
      const DataPointRecord& dataPointRecord,
      const DiscretePerturbationRecord& discretePerturbationRecord)
    : JoinRecord_Impl(dataPointRecord.id(), 
                      dataPointRecord.handle(), 
                      discretePerturbationRecord.id(), 
                      discretePerturbationRecord.handle(), 
                      dataPointRecord.projectDatabase())
  {
    BOOST_ASSERT(dataPointRecord.projectDatabase().handle() == discretePerturbationRecord.projectDatabase().handle());
  }

  DataPoint_DiscretePerturbation_JoinRecord_Impl::DataPoint_DiscretePerturbation_JoinRecord_Impl(
      const QSqlQuery& query, ProjectDatabase& database)
    : JoinRecord_Impl(query, database)
  {}

  std::string DataPoint_DiscretePerturbation_JoinRecord_Impl::databaseTableName() const {
    return DataPoint_DiscretePerturbation_JoinRecord::databaseTableName();
  }

  void DataPoint_DiscretePerturbation_JoinRecord_Impl::saveRow(const boost::shared_ptr<QSqlDatabase> &database)
  {
    QSqlQuery query(*database);
    this->makeUpdateByIdQuery<DataPoint_DiscretePerturbation_JoinRecord>(query);
    this->bindValues(query);
    assertExec(query);
  }

  ObjectRecord DataPoint_DiscretePerturbation_JoinRecord_Impl::leftObject() const {
    return this->dataPointRecord();
  }

  ObjectRecord DataPoint_DiscretePerturbation_JoinRecord_Impl::rightObject() const {
    return this->discretePerturbationRecord();
  }

  DataPointRecord DataPoint_DiscretePerturbation_JoinRecord_Impl::dataPointRecord() const {
    ProjectDatabase database = this->projectDatabase();
    boost::optional<DataPointRecord> dataPointRecord = DataPointRecord::getDataPointRecord(this->leftId(),database);
    BOOST_ASSERT(dataPointRecord);
    return *dataPointRecord;
  }

  DiscretePerturbationRecord DataPoint_DiscretePerturbation_JoinRecord_Impl::discretePerturbationRecord() const {
    ProjectDatabase database = this->projectDatabase();
    boost::optional<DiscretePerturbationRecord> discretePerturbationRecord = DiscretePerturbationRecord::getDiscretePerturbationRecord(this->rightId(),database);
    BOOST_ASSERT(discretePerturbationRecord);
    return *discretePerturbationRecord;
  }

} // detail

DataPoint_DiscretePerturbation_JoinRecord::DataPoint_DiscretePerturbation_JoinRecord(
    const DataPointRecord& dataPointRecord,
    const DiscretePerturbationRecord& discretePerturbationRecord)
  : JoinRecord(boost::shared_ptr<detail::DataPoint_DiscretePerturbation_JoinRecord_Impl>(
        new detail::DataPoint_DiscretePerturbation_JoinRecord_Impl(dataPointRecord, discretePerturbationRecord)), 
        dataPointRecord.projectDatabase())
{
  BOOST_ASSERT(getImpl<detail::DataPoint_DiscretePerturbation_JoinRecord_Impl>());
}

DataPoint_DiscretePerturbation_JoinRecord::DataPoint_DiscretePerturbation_JoinRecord(
    boost::shared_ptr<detail::DataPoint_DiscretePerturbation_JoinRecord_Impl> impl,
    ProjectDatabase database)
  : JoinRecord(impl, database)
{
  BOOST_ASSERT(getImpl<detail::DataPoint_DiscretePerturbation_JoinRecord_Impl>());
}

std::string DataPoint_DiscretePerturbation_JoinRecord::databaseTableName() {
  return "DataPoint_DiscretePerturbation_JoinRecords";
}

UpdateByIdQueryData DataPoint_DiscretePerturbation_JoinRecord::updateByIdQueryData() {
  static UpdateByIdQueryData result;
  if (result.queryString.empty()) {
    // numeric column identifiers
    result.columnValues = ColumnsType::getValues();

    // query string
    std::stringstream ss;
    ss << "UPDATE " << databaseTableName() << " SET ";
    int expectedValue = 0;
    for (std::set<int>::const_iterator it = result.columnValues.begin(), 
         itend = result.columnValues.end(); it != itend; ++it)
    {
      // require 0 based columns, don't skip any
      BOOST_ASSERT(*it == expectedValue);
      // column name is name, type is description
      ss << ColumnsType::valueName(*it) << "=:" << ColumnsType::valueName(*it);
      // is this the last column?
      std::set<int>::const_iterator nextIt = it;
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
    for (std::set<int>::const_iterator it = result.columnValues.begin(), 
         itend = result.columnValues.end(); it != itend; ++it)
    {
      // bind all values to avoid parameter mismatch error
      if (istringEqual(ColumnsType::valueDescription(*it), "INTEGER")) {
        result.nulls.push_back(QVariant(QVariant::Int));
      }
      else {
        result.nulls.push_back(QVariant(QVariant::String));
      }
    }
  }
  return result;
}

std::vector<DiscretePerturbationRecord> 
DataPoint_DiscretePerturbation_JoinRecord::getDiscretePerturbationRecords(
    const DataPointRecord& dataPointRecord) 
{
  ProjectDatabase database = dataPointRecord.projectDatabase();

  std::vector<DiscretePerturbationRecord> result;
  std::vector<int> discretePerturbationRecordIds;

  QSqlQuery query(*database.qSqlDatabase());
  query.prepare(toQString("SELECT * FROM " + 
      DataPoint_DiscretePerturbation_JoinRecord::databaseTableName() + 
      " WHERE leftId=:leftId"));
  query.bindValue(":leftId", dataPointRecord.id());
  assertExec(query);
  while (query.next()) {
    QVariant value;
    value = query.value(JoinRecordColumns::rightId);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    discretePerturbationRecordIds.push_back(value.toInt());
  }

  BOOST_FOREACH(int id, discretePerturbationRecordIds) {
    boost::optional<DiscretePerturbationRecord> discretePerturbationRecord = 
        DiscretePerturbationRecord::getDiscretePerturbationRecord(id,database);
    BOOST_ASSERT(discretePerturbationRecord);
    result.push_back(*discretePerturbationRecord);
  }

  return result;
}

std::vector<DataPointRecord> DataPoint_DiscretePerturbation_JoinRecord::getDataPointRecords(
    const DiscretePerturbationRecord& discretePerturbationRecord) 
{
  ProjectDatabase database = discretePerturbationRecord.projectDatabase();

  std::vector<DataPointRecord> result;
  std::vector<int> dataPointRecordIds;

  QSqlQuery query(*database.qSqlDatabase());
  query.prepare(toQString("SELECT * FROM " + 
      DataPoint_DiscretePerturbation_JoinRecord::databaseTableName() + " WHERE rightId=:rightId"));
  query.bindValue(":rightId", discretePerturbationRecord.id());
  assertExec(query);
  while (query.next()) {
    QVariant value;
    value = query.value(JoinRecordColumns::leftId);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    dataPointRecordIds.push_back(value.toInt());
  }

  BOOST_FOREACH(int id, dataPointRecordIds) {
    boost::optional<DataPointRecord> dataPointRecord = 
        DataPointRecord::getDataPointRecord(id,database);
    BOOST_ASSERT(dataPointRecord);
    result.push_back(*dataPointRecord);
  }

  return result;
}

DataPointRecord DataPoint_DiscretePerturbation_JoinRecord::dataPointRecord() const {
  return getImpl<detail::DataPoint_DiscretePerturbation_JoinRecord_Impl>()->dataPointRecord();
}

DiscretePerturbationRecord DataPoint_DiscretePerturbation_JoinRecord::discretePerturbationRecord() const {
  return getImpl<detail::DataPoint_DiscretePerturbation_JoinRecord_Impl>()->discretePerturbationRecord();
}

/// @cond
DataPoint_DiscretePerturbation_JoinRecord::DataPoint_DiscretePerturbation_JoinRecord(
    boost::shared_ptr<detail::DataPoint_DiscretePerturbation_JoinRecord_Impl> impl)
  : JoinRecord(impl)
{}
/// @endcond


} // project
} // openstudio

