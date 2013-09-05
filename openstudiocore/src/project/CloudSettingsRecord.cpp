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

#include <project/CloudSettingsRecord.hpp>
#include <project/CloudSettingsRecord_Impl.hpp>

#include <project/JoinRecord.hpp>
#include <project/VagrantSettingsRecord.hpp>
#include <project/VagrantSettingsRecord_Impl.hpp>

#include <utilities/cloud/VagrantProvider.hpp>
#include <utilities/cloud/VagrantProvider_Impl.hpp>

#include <utilities/core/Assert.hpp>

namespace openstudio {
namespace project {

namespace detail {

  CloudSettingsRecord_Impl::CloudSettingsRecord_Impl(const CloudSettings& cloudSettings,
                                                     const CloudSettingsRecordType& cloudSettingsRecordType,
                                                     ProjectDatabase& database)
    : ObjectRecord_Impl(database,cloudSettings.uuid(),"","","",cloudSettings.versionUUID()),
      m_cloudSettingsRecordType(cloudSettingsRecordType)
  {
    OS_ASSERT(false);
    // TODO: Initialize data members, check constructor call for base class.
  }

  CloudSettingsRecord_Impl::CloudSettingsRecord_Impl(const QSqlQuery& query, ProjectDatabase& database)
    : ObjectRecord_Impl(database, query)
  {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    QVariant value;

    value = query.value(CloudSettingsRecord::ColumnsType::cloudSettingsRecordType);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_cloudSettingsRecordType = CloudSettingsRecordType(value.toInt());
  }

  std::string CloudSettingsRecord_Impl::databaseTableName() const {
    return CloudSettingsRecord::databaseTableName();
  }

  boost::optional<ObjectRecord> CloudSettingsRecord_Impl::parent() const {
    return boost::none;
  }

  std::vector<ObjectRecord> CloudSettingsRecord_Impl::children() const {
    ObjectRecordVector result;
    return result;
  }

  std::vector<ObjectRecord> CloudSettingsRecord_Impl::resources() const {
    ObjectRecordVector result;
    return result;
  }

  std::vector<JoinRecord> CloudSettingsRecord_Impl::joinRecords() const {
    JoinRecordVector result;
    return result;
  }

  void CloudSettingsRecord_Impl::saveRow(const boost::shared_ptr<QSqlDatabase> &database) {
    QSqlQuery query(*database);
    this->makeUpdateByIdQuery<CloudSettingsRecord>(query);
    this->bindValues(query);
    assertExec(query);
  }

  void CloudSettingsRecord_Impl::bindValues(QSqlQuery& query) const {
    ObjectRecord_Impl::bindValues(query);

    query.bindValue(CloudSettingsRecord::ColumnsType::cloudSettingsRecordType,m_cloudSettingsRecordType.value());
  }

  void CloudSettingsRecord_Impl::setLastValues(const QSqlQuery& query, ProjectDatabase& projectDatabase) {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    ObjectRecord_Impl::setLastValues(query,projectDatabase);

    QVariant value;

    value = query.value(CloudSettingsRecord::ColumnsType::cloudSettingsRecordType);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_lastCloudSettingsRecordType = CloudSettingsRecordType(value.toInt());
  }

  bool CloudSettingsRecord_Impl::compareValues(const QSqlQuery& query) const {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    bool result = ObjectRecord_Impl::compareValues(query);

    QVariant value;

    value = query.value(CloudSettingsRecord::ColumnsType::cloudSettingsRecordType);
    OS_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_cloudSettingsRecordType == CloudSettingsRecordType(value.toInt()));

    return result;
  }

  void CloudSettingsRecord_Impl::saveLastValues() {
    ObjectRecord_Impl::saveLastValues();

    m_lastCloudSettingsRecordType = m_cloudSettingsRecordType;
  }

  void CloudSettingsRecord_Impl::revertToLastValues() {
    ObjectRecord_Impl::revertToLastValues();

    m_cloudSettingsRecordType = m_lastCloudSettingsRecordType;
  }

} // detail

std::string CloudSettingsRecord::databaseTableName() {
  return "CloudSettingsRecords";
}

UpdateByIdQueryData CloudSettingsRecord::updateByIdQueryData() {
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
      OS_ASSERT(*it == expectedValue);
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

void CloudSettingsRecord::updatePathData(ProjectDatabase database,
                                         const openstudio::path& originalBase,
                                         const openstudio::path& newBase)
{
  // path data in this table is not relative to the .osp location
}

boost::optional<CloudSettingsRecord> CloudSettingsRecord::factoryFromQuery(const QSqlQuery& query, ProjectDatabase& database)
{
  OptionalCloudSettingsRecord result;

  int cloudSettingsRecordType = query.value(CloudSettingsRecordColumns::cloudSettingsRecordType).toInt();

  switch (cloudSettingsRecordType) {
    case CloudSettingsRecordType::VagrantSettingsRecord : 
      result = VagrantSettingsRecord(query, database).cast<CloudSettingsRecord>();
     break;
    default :
      LOG(Error,"Unknown CloudSettingsRecordType " << cloudSettingsRecordType);
      return boost::none;
  }

  return result;
}

CloudSettingsRecord CloudSettingsRecord::factoryFromCloudSettings(const CloudSettings& cloudSettings, ProjectDatabase& database)
{
  if (cloudSettings.optionalCast<VagrantSettings>()) {
    return VagrantSettingsRecord(cloudSettings.cast<VagrantSettings>(), database);
  }

  OS_ASSERT(false);
  return CloudSettingsRecord(boost::shared_ptr<detail::CloudSettingsRecord_Impl>());
}

std::vector<CloudSettingsRecord> CloudSettingsRecord::getCloudSettingsRecords(ProjectDatabase& database) {
  std::vector<CloudSettingsRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + CloudSettingsRecord::databaseTableName()));
  assertExec(query);
  while (query.next()) {
    OptionalCloudSettingsRecord cloudSettingsRecord = CloudSettingsRecord::factoryFromQuery(query, database);
    if (cloudSettingsRecord) {
      result.push_back(*cloudSettingsRecord);
    }
  }

  return result;
}

boost::optional<CloudSettingsRecord> CloudSettingsRecord::getCloudSettingsRecord(int id, ProjectDatabase& database) {
  boost::optional<CloudSettingsRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + CloudSettingsRecord::databaseTableName() + " WHERE id=:id"));
  query.bindValue(":id",id);
  assertExec(query);
  if (query.first()) {
    result = CloudSettingsRecord::factoryFromQuery(query, database);
  }

  return result;
}

CloudSettings CloudSettingsRecord::cloudSettings() const {
  return getImpl<detail::CloudSettingsRecord_Impl>()->cloudSettings();
}

/// @cond
CloudSettingsRecord::CloudSettingsRecord(boost::shared_ptr<detail::CloudSettingsRecord_Impl> impl)
  : ObjectRecord(impl)
{}

CloudSettingsRecord::CloudSettingsRecord(boost::shared_ptr<detail::CloudSettingsRecord_Impl> impl,
                                         ProjectDatabase database)
  : ObjectRecord(impl, database)
{
  OS_ASSERT(getImpl<detail::CloudSettingsRecord_Impl>());
}
/// @endcond

} // project
} // openstudio

