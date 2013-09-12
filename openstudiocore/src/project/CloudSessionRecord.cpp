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

#include <project/CloudSessionRecord.hpp>
#include <project/CloudSessionRecord_Impl.hpp>

#include <project/JoinRecord.hpp>
#include <project/ProjectDatabase.hpp>
#include <project/UrlRecord.hpp>
#include <project/VagrantSessionRecord.hpp>
#include <project/VagrantSessionRecord_Impl.hpp>

#include <utilities/cloud/VagrantProvider.hpp>
#include <utilities/cloud/VagrantProvider_Impl.hpp>

#include <utilities/core/Assert.hpp>

namespace openstudio {
namespace project {

namespace detail {

  CloudSessionRecord_Impl::CloudSessionRecord_Impl(const CloudSession& cloudSession,
                                                   const CloudSessionRecordType& cloudSessionRecordType,
                                                   ProjectDatabase& database)
    : ObjectRecord_Impl(database,cloudSession.uuid(),"","","",cloudSession.versionUUID()),
      m_cloudSessionRecordType(cloudSessionRecordType),
      m_sessionId(cloudSession.sessionId())
  {}

  CloudSessionRecord_Impl::CloudSessionRecord_Impl(const QSqlQuery& query, ProjectDatabase& database)
    : ObjectRecord_Impl(database, query)
  {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    QVariant value;

    value = query.value(CloudSessionRecord::ColumnsType::cloudSessionRecordType);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_cloudSessionRecordType = CloudSessionRecordType(value.toInt());

    value = query.value(CloudSessionRecord::ColumnsType::sessionId);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_sessionId = value.toString().toStdString();

    value = query.value(CloudSessionRecord::ColumnsType::serverUrlRecordId);
    if (value.isValid() && !value.isNull()) {
      m_serverUrlRecordId = value.toInt();
    }
  }

  std::string CloudSessionRecord_Impl::databaseTableName() const {
    return CloudSessionRecord::databaseTableName();
  }

  boost::optional<ObjectRecord> CloudSessionRecord_Impl::parent() const {
    return boost::none;
  }

  std::vector<ObjectRecord> CloudSessionRecord_Impl::children() const {
    ObjectRecordVector result;
    if (OptionalUrlRecord sur = serverUrlRecord()) {
      result.push_back(*sur);
    }
    UrlRecordVector workers = workerUrlRecords();
    result.insert(result.end(),workers.begin(),workers.end());
    return result;
  }

  std::vector<ObjectRecord> CloudSessionRecord_Impl::resources() const {
    ObjectRecordVector result;
    return result;
  }

  std::vector<JoinRecord> CloudSessionRecord_Impl::joinRecords() const {
    JoinRecordVector result;
    return result;
  }

  void CloudSessionRecord_Impl::saveRow(const boost::shared_ptr<QSqlDatabase> &database) {
    QSqlQuery query(*database);
    this->makeUpdateByIdQuery<CloudSessionRecord>(query);
    this->bindValues(query);
    assertExec(query);
  }

  std::string CloudSessionRecord_Impl::sessionId() const {
    return m_sessionId;
  }

  boost::optional<UrlRecord> CloudSessionRecord_Impl::serverUrlRecord() const {
    OptionalUrlRecord result;
    if (m_serverUrlRecordId) {
      ProjectDatabase database = projectDatabase();
      result = UrlRecord::getUrlRecord(*m_serverUrlRecordId,database);
    }
    return result;
  }

  std::vector<UrlRecord> CloudSessionRecord_Impl::workerUrlRecords() const {
    UrlRecordVector result;

    // return UrlRecords for which this CloudSessionRecord is the parent and the id is not
    // equal to m_serverUrlRecordId.
    ProjectDatabase database = projectDatabase();
    QSqlQuery query(*(database.qSqlDatabase()));
    std::stringstream ss;
    ss << "SELECT * FROM " << UrlRecord::databaseTableName();
    ss << " WHERE parentDatabaseTableName=:parentDatabaseTableName AND ";
    ss << "parentRecordId=:parentRecordId";
    if (m_serverUrlRecordId) {
      ss << " AND (NOT id=:id)";
    }
    query.prepare(toQString(ss.str()));
    query.bindValue(":parentDatabaseTableName",toQString(databaseTableName()));
    query.bindValue(":parentRecordId",id());
    if (m_serverUrlRecordId) {
      query.bindValue(":id",*m_serverUrlRecordId);
    }
    assertExec(query);
    while (query.next()) {
      UrlRecord workerUrlRecord(query,database);
      result.push_back(workerUrlRecord);
    }

    return result;
  }

  void CloudSessionRecord_Impl::revertToLastRecordIds() {
    m_serverUrlRecordId = m_lastServerUrlRecordId;
  }

  void CloudSessionRecord_Impl::setServerUrlRecordId(int id) {
    m_serverUrlRecordId = id;
  }

  void CloudSessionRecord_Impl::clearServerUrlRecordId() {
    m_serverUrlRecordId.reset();
  }

  void CloudSessionRecord_Impl::bindValues(QSqlQuery& query) const {
    ObjectRecord_Impl::bindValues(query);

    query.bindValue(CloudSessionRecord::ColumnsType::cloudSessionRecordType,m_cloudSessionRecordType.value());
    query.bindValue(CloudSessionRecord::ColumnsType::sessionId,toQString(m_sessionId));
    if (m_serverUrlRecordId) {
      query.bindValue(CloudSessionRecord::ColumnsType::serverUrlRecordId,*m_serverUrlRecordId);
    }
    else {
      query.bindValue(CloudSessionRecord::ColumnsType::serverUrlRecordId,QVariant(QVariant::Int));
    }
  }

  void CloudSessionRecord_Impl::setLastValues(const QSqlQuery& query, ProjectDatabase& projectDatabase) {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    ObjectRecord_Impl::setLastValues(query,projectDatabase);

    QVariant value;

    value = query.value(CloudSessionRecord::ColumnsType::cloudSessionRecordType);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_lastCloudSessionRecordType = CloudSessionRecordType(value.toInt());

    value = query.value(CloudSessionRecord::ColumnsType::sessionId);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_lastSessionId = value.toString().toStdString();

    value = query.value(CloudSessionRecord::ColumnsType::serverUrlRecordId);
    if (value.isValid() && !value.isNull()) {
      m_lastServerUrlRecordId = value.toInt();
    }
    else {
      m_lastServerUrlRecordId.reset();
    }
  }

  bool CloudSessionRecord_Impl::compareValues(const QSqlQuery& query) const {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    bool result = ObjectRecord_Impl::compareValues(query);

    QVariant value;

    value = query.value(CloudSessionRecord::ColumnsType::cloudSessionRecordType);
    OS_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_cloudSessionRecordType == CloudSessionRecordType(value.toInt()));

    value = query.value(CloudSessionRecord::ColumnsType::sessionId);
    OS_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_sessionId == value.toString().toStdString());

    value = query.value(CloudSessionRecord::ColumnsType::serverUrlRecordId);
    if (value.isValid() && !value.isNull()) {
      result = result && m_serverUrlRecordId && (*m_serverUrlRecordId == value.toInt());
    }
    else {
      result = result && !m_serverUrlRecordId;
    }

    return result;
  }

  void CloudSessionRecord_Impl::saveLastValues() {
    ObjectRecord_Impl::saveLastValues();

    m_lastCloudSessionRecordType = m_cloudSessionRecordType;
    m_lastSessionId = m_sessionId;
    m_lastServerUrlRecordId = m_serverUrlRecordId;
  }

  void CloudSessionRecord_Impl::revertToLastValues() {
    ObjectRecord_Impl::revertToLastValues();

    m_cloudSessionRecordType = m_lastCloudSessionRecordType;
    m_sessionId = m_lastSessionId;
    m_serverUrlRecordId = m_lastServerUrlRecordId;
  }

} // detail

std::string CloudSessionRecord::databaseTableName() {
  return "CloudSessionRecords";
}

UpdateByIdQueryData CloudSessionRecord::updateByIdQueryData() {
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

void CloudSessionRecord::updatePathData(ProjectDatabase database,
                                        const openstudio::path& originalBase,
                                        const openstudio::path& newBase)
{
  // no path data in this table
}

boost::optional<CloudSessionRecord> CloudSessionRecord::factoryFromQuery(const QSqlQuery& query, ProjectDatabase& database)
{
  OptionalCloudSessionRecord result;

  int cloudSessionRecordType = query.value(CloudSessionRecordColumns::cloudSessionRecordType).toInt();

  switch (cloudSessionRecordType) {
    case CloudSessionRecordType::VagrantSessionRecord : 
      result = VagrantSessionRecord(query, database).cast<CloudSessionRecord>();
     break;
    default :
      LOG(Error,"Unknown CloudSessionRecordType " << cloudSessionRecordType);
      return boost::none;
  }

  return result;
}

CloudSessionRecord CloudSessionRecord::factoryFromCloudSession(const CloudSession& cloudSession, 
                                                               ProjectDatabase& database)
{
  if (cloudSession.optionalCast<VagrantSession>()) {
    return VagrantSessionRecord(cloudSession.cast<VagrantSession>(), database);
  }

  OS_ASSERT(false);
  return CloudSessionRecord(boost::shared_ptr<detail::CloudSessionRecord_Impl>());
}

std::vector<CloudSessionRecord> CloudSessionRecord::getCloudSessionRecords(ProjectDatabase& database) {
  std::vector<CloudSessionRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + CloudSessionRecord::databaseTableName()));
  assertExec(query);
  while (query.next()) {
    OptionalCloudSessionRecord cloudSessionRecord = CloudSessionRecord::factoryFromQuery(query, database);
     if (cloudSessionRecord) {
       result.push_back(*cloudSessionRecord);
     }
  }

  return result;
}

boost::optional<CloudSessionRecord> CloudSessionRecord::getCloudSessionRecord(int id, ProjectDatabase& database) {
  boost::optional<CloudSessionRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + CloudSessionRecord::databaseTableName() + " WHERE id=:id"));
  query.bindValue(":id",id);
  assertExec(query);
  if (query.first()) {
    result = CloudSessionRecord::factoryFromQuery(query, database);
  }

  return result;
}

std::string CloudSessionRecord::sessionId() const {
  return getImpl<detail::CloudSessionRecord_Impl>()->sessionId();
}

boost::optional<UrlRecord> CloudSessionRecord::serverUrlRecord() const {
  return getImpl<detail::CloudSessionRecord_Impl>()->serverUrlRecord();
}

std::vector<UrlRecord> CloudSessionRecord::workerUrlRecords() const {
  return getImpl<detail::CloudSessionRecord_Impl>()->workerUrlRecords();
}

CloudSession CloudSessionRecord::cloudSession() const {
  return getImpl<detail::CloudSessionRecord_Impl>()->cloudSession();
}

/// @cond
CloudSessionRecord::CloudSessionRecord(boost::shared_ptr<detail::CloudSessionRecord_Impl> impl)
  : ObjectRecord(impl)
{}

CloudSessionRecord::CloudSessionRecord(boost::shared_ptr<detail::CloudSessionRecord_Impl> impl,
                                       ProjectDatabase database)
  : ObjectRecord(impl, database)
{
  OS_ASSERT(getImpl<detail::CloudSessionRecord_Impl>());
}
/// @endcond

void CloudSessionRecord::constructRelatedRecords(const CloudSession& cloudSession,
                                                 ProjectDatabase& database)
{
  CloudSessionRecord copyOfThis(getImpl<detail::CloudSessionRecord_Impl>());
  bool isNew = database.isNewRecord(copyOfThis);
  if (!isNew) {
    getImpl<detail::CloudSessionRecord_Impl>()->revertToLastRecordIds();
  }
  
  if (isNew || (getImpl<detail::CloudSessionRecord_Impl>()->lastUuidLast() != cloudSession.versionUUID())) {
    // remove any existing UrlRecords that have this object as its parent
    ObjectRecordVector childUrls = children();
    BOOST_FOREACH(ObjectRecord& childUrl,childUrls) {
      database.removeRecord(childUrl);
    }
    // create new UrlRecords 
    if (OptionalUrl serverUrl = cloudSession.serverUrl()) {
      UrlRecord serverRecord(*serverUrl,copyOfThis);
      getImpl<detail::CloudSessionRecord_Impl>()->setServerUrlRecordId(serverRecord.id());
    }
    BOOST_FOREACH(const Url& workerUrl, cloudSession.workerUrls()) {
      UrlRecord workerRecord(workerUrl,copyOfThis);
    }
  }
}

} // project
} // openstudio

