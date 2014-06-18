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

#include "AWSSessionRecord.hpp"
#include "AWSSessionRecord_Impl.hpp"

#include "ProjectDatabase.hpp"
#include "UrlRecord.hpp"

#include "../utilities/cloud/AWSProvider.hpp"
#include "../utilities/cloud/AWSProvider_Impl.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/core/Containers.hpp"

namespace openstudio {
namespace project {

namespace detail {

  AWSSessionRecord_Impl::AWSSessionRecord_Impl(const AWSSession& awsSession,
                                               ProjectDatabase& database)
    : CloudSessionRecord_Impl(awsSession,
                              CloudSessionRecordType::AWSSessionRecord,
                              database),
      m_numServerProcessors(awsSession.numServerProcessors()),
      m_numWorkerProcessors(awsSession.numWorkerProcessors()),
      m_privateKey(awsSession.privateKey()),
      m_timestamp(awsSession.timestamp()),
      m_region(awsSession.region()),
      m_serverInstanceType(awsSession.serverInstanceType()),
      m_workerInstanceType(awsSession.workerInstanceType())
  {
  }

  AWSSessionRecord_Impl::AWSSessionRecord_Impl(const QSqlQuery& query, ProjectDatabase& database)
    : CloudSessionRecord_Impl(query, database)
  {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    QVariant value;

    value = query.value(AWSSessionRecord::ColumnsType::numServerProcessors);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_numServerProcessors = value.toUInt();

    value = query.value(AWSSessionRecord::ColumnsType::numWorkerProcessors);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_numWorkerProcessors = value.toUInt();

    value = query.value(AWSSessionRecord::ColumnsType::privateKey);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_privateKey = value.toString().toStdString();

    value = query.value(AWSSessionRecord::ColumnsType::timestamp);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_timestamp = value.toString().toStdString();

    value = query.value(AWSSessionRecord::ColumnsType::region);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_region = value.toString().toStdString();

    value = query.value(AWSSessionRecord::ColumnsType::serverInstanceType);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_serverInstanceType = value.toString().toStdString();

    value = query.value(AWSSessionRecord::ColumnsType::workerInstanceType);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_workerInstanceType = value.toString().toStdString();
  }

  void AWSSessionRecord_Impl::saveRow(const std::shared_ptr<QSqlDatabase>& database) {
    QSqlQuery query(*database);
    this->makeUpdateByIdQuery<AWSSessionRecord>(query);
    this->bindValues(query);
    assertExec(query);
  }

  CloudSession AWSSessionRecord_Impl::cloudSession() const {
    return awsSession().cast<CloudSession>();
  }

  AWSSession AWSSessionRecord_Impl::awsSession() const {
    OptionalUrl serverUrl;
    std::string serverId;
    if (OptionalUrlRecord sur = serverUrlRecord()) {
      serverUrl = sur->url();
      serverId = sur->name();
    }
    UrlVector workerUrls;
    StringVector workerIds;
    for (const UrlRecord& wur : workerUrlRecords()) {
      workerUrls.push_back(wur.url());
      workerIds.push_back(wur.name());
    }
    return AWSSession(handle(),
                      uuidLast(),
                      sessionId(),
                      serverUrl,
                      serverId,
                      m_numServerProcessors,
                      workerUrls,
                      workerIds,
                      m_numWorkerProcessors,
                      m_privateKey,
                      m_timestamp,
                      m_region,
                      m_serverInstanceType,
                      m_workerInstanceType);
  }

  void AWSSessionRecord_Impl::bindValues(QSqlQuery& query) const {
    CloudSessionRecord_Impl::bindValues(query);

    query.bindValue(AWSSessionRecord::ColumnsType::numServerProcessors,m_numServerProcessors);
    query.bindValue(AWSSessionRecord::ColumnsType::numWorkerProcessors,m_numWorkerProcessors);
    query.bindValue(AWSSessionRecord::ColumnsType::privateKey,toQString(m_privateKey));
    query.bindValue(AWSSessionRecord::ColumnsType::timestamp,toQString(m_timestamp));
    query.bindValue(AWSSessionRecord::ColumnsType::region,toQString(m_region));
    query.bindValue(AWSSessionRecord::ColumnsType::serverInstanceType,toQString(m_serverInstanceType));
    query.bindValue(AWSSessionRecord::ColumnsType::workerInstanceType,toQString(m_workerInstanceType));
  }

  void AWSSessionRecord_Impl::setLastValues(const QSqlQuery& query, ProjectDatabase& projectDatabase) {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    CloudSessionRecord_Impl::setLastValues(query,projectDatabase);

    QVariant value;

    value = query.value(AWSSessionRecord::ColumnsType::numServerProcessors);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_lastNumServerProcessors = value.toUInt();

    value = query.value(AWSSessionRecord::ColumnsType::numWorkerProcessors);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_lastNumWorkerProcessors = value.toUInt();

    value = query.value(AWSSessionRecord::ColumnsType::privateKey);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_lastPrivateKey = value.toString().toStdString();

    value = query.value(AWSSessionRecord::ColumnsType::timestamp);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_lastTimestamp = value.toString().toStdString();

    value = query.value(AWSSessionRecord::ColumnsType::region);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_lastRegion = value.toString().toStdString();

    value = query.value(AWSSessionRecord::ColumnsType::serverInstanceType);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_lastServerInstanceType = value.toString().toStdString();

    value = query.value(AWSSessionRecord::ColumnsType::workerInstanceType);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_lastWorkerInstanceType = value.toString().toStdString();
  }

  bool AWSSessionRecord_Impl::compareValues(const QSqlQuery& query) const {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    bool result = CloudSessionRecord_Impl::compareValues(query);

    QVariant value;

    value = query.value(AWSSessionRecord::ColumnsType::numServerProcessors);
    OS_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_numServerProcessors == value.toUInt());

    value = query.value(AWSSessionRecord::ColumnsType::numWorkerProcessors);
    OS_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_numWorkerProcessors == value.toUInt());

    value = query.value(AWSSessionRecord::ColumnsType::privateKey);
    OS_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_privateKey == value.toString().toStdString());

    value = query.value(AWSSessionRecord::ColumnsType::timestamp);
    OS_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_timestamp == value.toString().toStdString());

    value = query.value(AWSSessionRecord::ColumnsType::region);
    OS_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_region == value.toString().toStdString());

    value = query.value(AWSSessionRecord::ColumnsType::serverInstanceType);
    OS_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_serverInstanceType == value.toString().toStdString());

    value = query.value(AWSSessionRecord::ColumnsType::workerInstanceType);
    OS_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_workerInstanceType == value.toString().toStdString());

    return result;
  }

  void AWSSessionRecord_Impl::saveLastValues() {
    CloudSessionRecord_Impl::saveLastValues();

    m_lastNumServerProcessors = m_numServerProcessors;
    m_lastNumWorkerProcessors = m_numWorkerProcessors;
    m_lastPrivateKey = m_privateKey;
    m_lastTimestamp = m_timestamp;
    m_lastRegion = m_region;
    m_lastServerInstanceType = m_serverInstanceType;
    m_lastWorkerInstanceType = m_workerInstanceType;
  }

  void AWSSessionRecord_Impl::revertToLastValues() {
    CloudSessionRecord_Impl::revertToLastValues();

    m_numServerProcessors = m_lastNumServerProcessors;
    m_numWorkerProcessors = m_lastNumWorkerProcessors;
    m_privateKey = m_lastPrivateKey;
    m_timestamp = m_lastTimestamp;
    m_region = m_lastRegion;
    m_serverInstanceType = m_lastServerInstanceType;
    m_workerInstanceType = m_lastWorkerInstanceType;
  }

} // detail

AWSSessionRecord::AWSSessionRecord(const AWSSession& awsSession, ProjectDatabase& database)
  : CloudSessionRecord(std::shared_ptr<detail::AWSSessionRecord_Impl>(
        new detail::AWSSessionRecord_Impl(awsSession, database)),
        database)
{
  OS_ASSERT(getImpl<detail::AWSSessionRecord_Impl>());
  constructRelatedRecords(awsSession,database);
}

AWSSessionRecord::AWSSessionRecord(const QSqlQuery& query, ProjectDatabase& database)
  : CloudSessionRecord(std::shared_ptr<detail::AWSSessionRecord_Impl>(
        new detail::AWSSessionRecord_Impl(query, database)),
        database)
{
  OS_ASSERT(getImpl<detail::AWSSessionRecord_Impl>());
}

boost::optional<AWSSessionRecord> AWSSessionRecord::factoryFromQuery(const QSqlQuery& query, ProjectDatabase& database)
{
  OptionalAWSSessionRecord result;

  try {
    result = AWSSessionRecord(query,database);
  }
  catch (const std::exception& e) {
    LOG(Error,"Unable to construct AWSSessionRecord from query, because '" << e.what() << "'.");
  }

  return result;
}

std::vector<AWSSessionRecord> AWSSessionRecord::getAWSSessionRecords(ProjectDatabase& database) {
  std::vector<AWSSessionRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + CloudSessionRecord::databaseTableName() + " WHERE cloudSessionRecordType=:cloudSessionRecordType"));
  query.bindValue(":cloudSessionRecordType", CloudSessionRecordType::AWSSessionRecord);
  assertExec(query);
  while (query.next()) {
    result.push_back(AWSSessionRecord(query, database));
  }

  return result;
}

boost::optional<AWSSessionRecord> AWSSessionRecord::getAWSSessionRecord(int id, ProjectDatabase& database) {
  boost::optional<AWSSessionRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + CloudSessionRecord::databaseTableName() + " WHERE cloudSessionRecordType=:cloudSessionRecordType AND id=:id"));
  query.bindValue(":cloudSessionRecordType", CloudSessionRecordType::AWSSessionRecord);
  query.bindValue(":id",id);
  assertExec(query);
  if (query.first()) {
    result = AWSSessionRecord(query, database);
  }

  return result;
}

AWSSession AWSSessionRecord::awsSession() const {
  return getImpl<detail::AWSSessionRecord_Impl>()->awsSession();
}

/// @cond
AWSSessionRecord::AWSSessionRecord(std::shared_ptr<detail::AWSSessionRecord_Impl> impl)
  : CloudSessionRecord(impl)
{}

AWSSessionRecord::AWSSessionRecord(std::shared_ptr<detail::AWSSessionRecord_Impl> impl,
                                   ProjectDatabase database)
  : CloudSessionRecord(impl, database)
{
  OS_ASSERT(getImpl<detail::AWSSessionRecord_Impl>());
}
/// @endcond

} // project
} // openstudio

