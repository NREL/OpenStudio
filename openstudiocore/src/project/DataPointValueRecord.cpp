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

#include "DataPointValueRecord.hpp"
#include "DataPointValueRecord_Impl.hpp"

#include "JoinRecord.hpp"
#include "DataPointRecord.hpp"
#include "FunctionRecord.hpp"
#include "ContinuousVariableRecord.hpp"

#include "../utilities/math/FloatCompare.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace project {

namespace detail {

  DataPointValueRecord_Impl::DataPointValueRecord_Impl(double dataPointValue, 
                                                       DataPointRecord& dataPointRecord,
                                                       FunctionRecord& functionRecord)
    : ObjectRecord_Impl(dataPointRecord.projectDatabase(),"","",""),
      m_dataPointRecordId(dataPointRecord.id()),
      m_functionRecordId(functionRecord.id()),
      m_dataPointValue(dataPointValue)
  {}

  DataPointValueRecord_Impl::DataPointValueRecord_Impl(
      double dataPointValue, 
      DataPointRecord& dataPointRecord,
      ContinuousVariableRecord& continuousVariableRecord)
    : ObjectRecord_Impl(dataPointRecord.projectDatabase(),"","",""),
      m_dataPointRecordId(dataPointRecord.id()),
      m_continuousVariableRecordId(continuousVariableRecord.id()),
      m_dataPointValue(dataPointValue)
  {}

  DataPointValueRecord_Impl::DataPointValueRecord_Impl(const QSqlQuery& query, 
                                                       ProjectDatabase& database)
    : ObjectRecord_Impl(database, query)
  {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    QVariant value;

    value = query.value(DataPointValueRecord::ColumnsType::dataPointRecordId);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_dataPointRecordId = value.toInt();

    value = query.value(DataPointValueRecord::ColumnsType::functionRecordId);
    if (value.isValid() && !value.isNull()) {
      m_functionRecordId = value.toInt();
    }

    value = query.value(DataPointValueRecord::ColumnsType::continuousVariableRecordId);
    if (value.isValid() && !value.isNull()) {
      m_continuousVariableRecordId = value.toInt();
    }

    value = query.value(DataPointValueRecord::ColumnsType::dataPointValue);
    m_dataPointValue = value.toDouble();
  }

  std::string DataPointValueRecord_Impl::databaseTableName() const {
    return DataPointValueRecord::databaseTableName();
  }

  boost::optional<ObjectRecord> DataPointValueRecord_Impl::parent() const {
    return dataPointRecord();
  }

  std::vector<ObjectRecord> DataPointValueRecord_Impl::children() const {
    ObjectRecordVector result;
    return result;
  }

  std::vector<ObjectRecord> DataPointValueRecord_Impl::resources() const {
    ObjectRecordVector result;
    if (m_functionRecordId) {
      result.push_back(functionRecord().get());
    }
    if (m_continuousVariableRecordId) {
      result.push_back(continuousVariableRecord().get());
    }
    return result;
  }

  std::vector<JoinRecord> DataPointValueRecord_Impl::joinRecords() const {
    JoinRecordVector result;
    return result;
  }

  void DataPointValueRecord_Impl::saveRow(const std::shared_ptr<QSqlDatabase> &database)
  {
    QSqlQuery query(*database);
    this->makeUpdateByIdQuery<DataPointValueRecord>(query);
    this->bindValues(query);
    assertExec(query);
  }

  DataPointRecord DataPointValueRecord_Impl::dataPointRecord() const {
    ProjectDatabase database = projectDatabase();
    return DataPointRecord::getDataPointRecord(m_dataPointRecordId,database).get();
  }

  boost::optional<FunctionRecord> DataPointValueRecord_Impl::functionRecord() const {
    OptionalFunctionRecord result;
    if (m_functionRecordId) {
      ProjectDatabase database = projectDatabase();
      result = FunctionRecord::getFunctionRecord(*m_functionRecordId,database);
    }
    return result;
  }

  boost::optional<int> DataPointValueRecord_Impl::continuousVariableRecordId() const {
    return m_continuousVariableRecordId;
  }

  boost::optional<ContinuousVariableRecord> 
  DataPointValueRecord_Impl::continuousVariableRecord() const
  {
    OptionalContinuousVariableRecord result;
    if (m_continuousVariableRecordId) {
      ProjectDatabase database = projectDatabase();
      result = ContinuousVariableRecord::getContinuousVariableRecord(*m_continuousVariableRecordId,
                                                                     database);
    }
    return result;
  }

  double DataPointValueRecord_Impl::dataPointValue() const {
    return m_dataPointValue;
  }
  void DataPointValueRecord_Impl::bindValues(QSqlQuery& query) const {
    ObjectRecord_Impl::bindValues(query);

    query.bindValue(DataPointValueRecordColumns::dataPointRecordId,m_dataPointRecordId);
    if (m_functionRecordId) {
      query.bindValue(DataPointValueRecordColumns::functionRecordId,*m_functionRecordId);
    }
    else {
      query.bindValue(DataPointValueRecordColumns::functionRecordId,QVariant(QVariant::Int));
    }
    if (m_continuousVariableRecordId) {
      query.bindValue(DataPointValueRecordColumns::continuousVariableRecordId,
                      *m_continuousVariableRecordId);
    }
    else {
      query.bindValue(DataPointValueRecordColumns::continuousVariableRecordId,
                      QVariant(QVariant::Int));
    }
    query.bindValue(DataPointValueRecordColumns::dataPointValue,m_dataPointValue);
  }

  void DataPointValueRecord_Impl::setLastValues(const QSqlQuery& query, ProjectDatabase& projectDatabase) {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    ObjectRecord_Impl::setLastValues(query,projectDatabase);

    QVariant value;

    value = query.value(DataPointValueRecordColumns::dataPointRecordId);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_lastDataPointRecordId = value.toInt();

    value = query.value(DataPointValueRecordColumns::functionRecordId);
    if (value.isValid() && !value.isNull()) {
      m_lastFunctionRecordId = value.toInt();
    }
    else {
      m_lastFunctionRecordId.reset();
    }

    value = query.value(DataPointValueRecordColumns::continuousVariableRecordId);
    if (value.isValid() && !value.isNull()) {
      m_lastContinuousVariableRecordId = value.toInt();
    }
    else {
      m_lastContinuousVariableRecordId.reset();
    }

    value = query.value(DataPointValueRecordColumns::dataPointValue);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_lastDataPointValue = value.toDouble();
  }

  bool DataPointValueRecord_Impl::compareValues(const QSqlQuery& query) const {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    bool result = ObjectRecord_Impl::compareValues(query);

    QVariant value;

    value = query.value(DataPointValueRecordColumns::dataPointRecordId);
    OS_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_dataPointRecordId == value.toInt());

    value = query.value(DataPointValueRecordColumns::functionRecordId);
    if (value.isValid() && !value.isNull()) {
      result = result && m_functionRecordId && (*m_functionRecordId == value.toInt());
    }
    else {
      result = result && !m_functionRecordId;
    }

    value = query.value(DataPointValueRecordColumns::continuousVariableRecordId);
    if (value.isValid() && !value.isNull()) {
      result = result && m_continuousVariableRecordId && 
               (*m_continuousVariableRecordId == value.toInt());
    }
    else {
      result = result && !m_continuousVariableRecordId;
    }

    value = query.value(DataPointValueRecordColumns::dataPointValue);
    result = result && equal(m_dataPointValue,value.toDouble());

    return result;
  }

  void DataPointValueRecord_Impl::saveLastValues() {
    ObjectRecord_Impl::saveLastValues();

    m_lastDataPointRecordId = m_dataPointRecordId;
    m_lastFunctionRecordId = m_functionRecordId;
    m_lastContinuousVariableRecordId = m_continuousVariableRecordId;
    m_lastDataPointValue = m_dataPointValue;
  }

  void DataPointValueRecord_Impl::revertToLastValues() {
    ObjectRecord_Impl::revertToLastValues();

    m_dataPointRecordId = m_lastDataPointRecordId;
    m_functionRecordId = m_lastFunctionRecordId;
    m_continuousVariableRecordId = m_lastContinuousVariableRecordId;
    m_dataPointValue = m_lastDataPointValue;
  }

} // detail

DataPointValueRecord::DataPointValueRecord(double dataPointValue, 
                                           DataPointRecord& dataPointRecord,
                                           FunctionRecord& functionRecord)
  : ObjectRecord(std::shared_ptr<detail::DataPointValueRecord_Impl>(
        new detail::DataPointValueRecord_Impl(dataPointValue, dataPointRecord, functionRecord)),
        dataPointRecord.projectDatabase())
{
  OS_ASSERT(getImpl<detail::DataPointValueRecord_Impl>());
}

DataPointValueRecord::DataPointValueRecord(double dataPointValue, 
                                           DataPointRecord& dataPointRecord,
                                           ContinuousVariableRecord& continuousVariableRecord)
  : ObjectRecord(std::shared_ptr<detail::DataPointValueRecord_Impl>(
        new detail::DataPointValueRecord_Impl(dataPointValue, 
                                              dataPointRecord, 
                                              continuousVariableRecord)),
        dataPointRecord.projectDatabase())
{
  OS_ASSERT(getImpl<detail::DataPointValueRecord_Impl>());
}

DataPointValueRecord::DataPointValueRecord(const QSqlQuery& query, ProjectDatabase& database)
  : ObjectRecord(std::shared_ptr<detail::DataPointValueRecord_Impl>(
        new detail::DataPointValueRecord_Impl(query, database)),
        database)
{
  OS_ASSERT(getImpl<detail::DataPointValueRecord_Impl>());
}

DataPointValueRecord::DataPointValueRecord(std::shared_ptr<detail::DataPointValueRecord_Impl> impl,
                                           ProjectDatabase database)
  : ObjectRecord(impl, database)
{
  OS_ASSERT(getImpl<detail::DataPointValueRecord_Impl>());
}

std::string DataPointValueRecord::databaseTableName() {
  return "DataPointValueRecords";
}

UpdateByIdQueryData DataPointValueRecord::updateByIdQueryData() {
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

void DataPointValueRecord::createIndices(QSqlDatabase& qSqlDatabase, const std::string& databaseTableName)
{
  ObjectRecord::createIndices(qSqlDatabase,databaseTableName);

  QSqlQuery query(qSqlDatabase);
  query.prepare(toQString("CREATE INDEX " + databaseTableName + "dataPointRecordIdIndex ON " + databaseTableName + " (dataPointRecordId)"));
  assertExec(query);

  query.prepare(toQString("CREATE INDEX " + databaseTableName + "functionRecordIdIndex ON " + databaseTableName + " (functionRecordId)"));
  assertExec(query);

  query.prepare(toQString("CREATE INDEX " + databaseTableName + "continuousVariableRecordIdIndex ON " + databaseTableName + " (continuousVariableRecordId)"));
  assertExec(query);
}

void DataPointValueRecord::updatePathData(ProjectDatabase database,
                                          const openstudio::path& originalBase,
                                          const openstudio::path& newBase)
{}

boost::optional<DataPointValueRecord> DataPointValueRecord::factoryFromQuery(
    const QSqlQuery& query, ProjectDatabase& database)
{
  OptionalDataPointValueRecord result;
  try {
    result = DataPointValueRecord(query,database);
  }
  catch (const std::exception& e) {
    LOG(Error,"Unable to construct DataPointValueRecord from query, because '" 
        << e.what() << "'.");
  }
  return result;
}

std::vector<DataPointValueRecord> DataPointValueRecord::getDataPointValueRecords(
    ProjectDatabase& database) 
{
  std::vector<DataPointValueRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + DataPointValueRecord::databaseTableName()));
  assertExec(query);
  while (query.next()) {
    result.push_back(DataPointValueRecord(query, database));
  }

  return result;
}

boost::optional<DataPointValueRecord> DataPointValueRecord::getDataPointValueRecord(int id, ProjectDatabase& database) {
  boost::optional<DataPointValueRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + DataPointValueRecord::databaseTableName() + 
      " WHERE id=:id"));
  query.bindValue(":id",id);
  assertExec(query);
  if (query.first()) {
    result = DataPointValueRecord(query, database);
  }

  return result;
}

DataPointRecord DataPointValueRecord::dataPointRecord() const {
  return getImpl<detail::DataPointValueRecord_Impl>()->dataPointRecord();
}

boost::optional<FunctionRecord> DataPointValueRecord::functionRecord() const {
  return getImpl<detail::DataPointValueRecord_Impl>()->functionRecord();
}

boost::optional<int> DataPointValueRecord::continuousVariableRecordId() const {
  return getImpl<detail::DataPointValueRecord_Impl>()->continuousVariableRecordId();
}

boost::optional<ContinuousVariableRecord> DataPointValueRecord::continuousVariableRecord() const {
  return getImpl<detail::DataPointValueRecord_Impl>()->continuousVariableRecord();
}

double DataPointValueRecord::dataPointValue() const {
  return getImpl<detail::DataPointValueRecord_Impl>()->dataPointValue();
}

/// @cond
DataPointValueRecord::DataPointValueRecord(std::shared_ptr<detail::DataPointValueRecord_Impl> impl)
  : ObjectRecord(impl)
{}
/// @endcond

} // project
} // openstudio

