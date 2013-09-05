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

#include <project/VagrantSettingsRecord.hpp>
#include <project/VagrantSettingsRecord_Impl.hpp>

#include <project/ProjectDatabase.hpp>
#include <project/UrlRecord.hpp>

#include <utilities/cloud/VagrantProvider.hpp>
#include <utilities/cloud/VagrantProvider_Impl.hpp>

#include <utilities/core/Assert.hpp>

#include <boost/foreach.hpp>

namespace openstudio {
namespace project {

namespace detail {

  VagrantSettingsRecord_Impl::VagrantSettingsRecord_Impl(const VagrantSettings& vagrantSettings,
                                                         ProjectDatabase& database)
    : CloudSettingsRecord_Impl(vagrantSettings, 
                               CloudSettingsRecordType::VagrantSettingsRecord,
                               database),
      m_userAgreementSigned(vagrantSettings.userAgreementSigned()),
      m_serverPath(vagrantSettings.serverPath()),
      m_workerPath(vagrantSettings.workerPath()),
      m_haltOnStop(vagrantSettings.haltOnStop()),
      m_username(vagrantSettings.username())
  {
  }

  VagrantSettingsRecord_Impl::VagrantSettingsRecord_Impl(const QSqlQuery& query, ProjectDatabase& database)
    : CloudSettingsRecord_Impl(query, database)
  {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    QVariant value;

    value = query.value(VagrantSettingsRecord::ColumnsType::userAgreementSigned);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_userAgreementSigned = value.toBool();

    value = query.value(VagrantSettingsRecord::ColumnsType::serverPath);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_serverPath = toPath(value.toString());

    value = query.value(VagrantSettingsRecord::ColumnsType::serverUrlRecordId);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_serverUrlRecordId = value.toInt();

    value = query.value(VagrantSettingsRecord::ColumnsType::workerPath);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_workerPath = toPath(value.toString());

    value = query.value(VagrantSettingsRecord::ColumnsType::workerUrlRecordId);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_workerUrlRecordId = value.toInt();

    value = query.value(VagrantSettingsRecord::ColumnsType::haltOnStop);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_haltOnStop = value.toBool();

    value = query.value(VagrantSettingsRecord::ColumnsType::username);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_username = value.toString().toStdString();
  }

  std::vector<ObjectRecord> VagrantSettingsRecord_Impl::children() const {
    ObjectRecordVector result;
    result.push_back(serverUrlRecord().cast<ObjectRecord>());
    result.push_back(workerUrlRecord().cast<ObjectRecord>());
    return result;
  }

  void VagrantSettingsRecord_Impl::saveRow(const boost::shared_ptr<QSqlDatabase> &database) {
    QSqlQuery query(*database);
    this->makeUpdateByIdQuery<VagrantSettingsRecord>(query);
    this->bindValues(query);
    assertExec(query);
  }

  UrlRecord VagrantSettingsRecord_Impl::serverUrlRecord() const {
    ProjectDatabase database = projectDatabase();
    return UrlRecord::getUrlRecord(m_serverUrlRecordId,database).get();
  }

  UrlRecord VagrantSettingsRecord_Impl::workerUrlRecord() const {
    ProjectDatabase database = projectDatabase();
    return UrlRecord::getUrlRecord(m_workerUrlRecordId,database).get();
  }

  CloudSettings VagrantSettingsRecord_Impl::cloudSettings() const {
    return vagrantSettings().cast<CloudSettings>();
  }

  VagrantSettings VagrantSettingsRecord_Impl::vagrantSettings() const {
    return VagrantSettings(handle(),
                           uuidLast(),
                           m_userAgreementSigned,
                           m_serverPath,
                           serverUrlRecord().url(),
                           m_workerPath,
                           workerUrlRecord().url(),
                           m_haltOnStop,
                           m_username);
  }

  void VagrantSettingsRecord_Impl::revertToLastRecordIds() {
    m_serverUrlRecordId = m_lastServerUrlRecordId;
    m_workerUrlRecordId = m_lastWorkerUrlRecordId;
  }

  void VagrantSettingsRecord_Impl::setServerUrlRecordId(int id) {
    m_serverUrlRecordId = id;
  }

  void VagrantSettingsRecord_Impl::setWorkerUrlRecordId(int id) {
    m_workerUrlRecordId = id;
  }

  void VagrantSettingsRecord_Impl::bindValues(QSqlQuery& query) const {
    CloudSettingsRecord_Impl::bindValues(query);

    query.bindValue(VagrantSettingsRecord::ColumnsType::userAgreementSigned,m_userAgreementSigned);
    query.bindValue(VagrantSettingsRecord::ColumnsType::serverPath,toQString(m_serverPath));
    query.bindValue(VagrantSettingsRecord::ColumnsType::serverUrlRecordId,m_serverUrlRecordId);
    query.bindValue(VagrantSettingsRecord::ColumnsType::workerPath,toQString(m_workerPath));
    query.bindValue(VagrantSettingsRecord::ColumnsType::workerUrlRecordId,m_workerUrlRecordId);
    query.bindValue(VagrantSettingsRecord::ColumnsType::haltOnStop,m_haltOnStop);
    query.bindValue(VagrantSettingsRecord::ColumnsType::username,toQString(m_username));
  }

  void VagrantSettingsRecord_Impl::setLastValues(const QSqlQuery& query, ProjectDatabase& projectDatabase) {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    CloudSettingsRecord_Impl::setLastValues(query,projectDatabase);

    QVariant value;

    value = query.value(VagrantSettingsRecord::ColumnsType::userAgreementSigned);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_lastUserAgreementSigned = value.toBool();

    value = query.value(VagrantSettingsRecord::ColumnsType::serverPath);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_lastServerPath = toPath(value.toString());

    value = query.value(VagrantSettingsRecord::ColumnsType::serverUrlRecordId);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_lastServerUrlRecordId = value.toInt();

    value = query.value(VagrantSettingsRecord::ColumnsType::workerPath);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_lastWorkerPath = toPath(value.toString());

    value = query.value(VagrantSettingsRecord::ColumnsType::workerUrlRecordId);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_lastWorkerUrlRecordId = value.toInt();

    value = query.value(VagrantSettingsRecord::ColumnsType::haltOnStop);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_lastHaltOnStop = value.toBool();

    value = query.value(VagrantSettingsRecord::ColumnsType::username);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_lastUsername = value.toString().toStdString();
  }

  bool VagrantSettingsRecord_Impl::compareValues(const QSqlQuery& query) const {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    bool result = CloudSettingsRecord_Impl::compareValues(query);

    QVariant value;

    value = query.value(VagrantSettingsRecord::ColumnsType::userAgreementSigned);
    OS_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_userAgreementSigned == value.toBool());

    value = query.value(VagrantSettingsRecord::ColumnsType::serverPath);
    OS_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_serverPath == toPath(value.toString()));

    value = query.value(VagrantSettingsRecord::ColumnsType::serverUrlRecordId);
    OS_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_serverUrlRecordId == value.toInt());

    value = query.value(VagrantSettingsRecord::ColumnsType::workerPath);
    OS_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_workerPath == toPath(value.toString()));

    value = query.value(VagrantSettingsRecord::ColumnsType::workerUrlRecordId);
    OS_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_workerUrlRecordId == value.toInt());

    value = query.value(VagrantSettingsRecord::ColumnsType::haltOnStop);
    OS_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_haltOnStop == value.toBool());

    value = query.value(VagrantSettingsRecord::ColumnsType::username);
    OS_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_username == value.toString().toStdString());

    return result;
  }

  void VagrantSettingsRecord_Impl::saveLastValues() {
    CloudSettingsRecord_Impl::saveLastValues();

    m_lastUserAgreementSigned = m_userAgreementSigned;
    m_lastServerPath = m_serverPath;
    m_lastServerUrlRecordId = m_serverUrlRecordId;
    m_lastWorkerPath = m_workerPath;
    m_lastWorkerUrlRecordId = m_workerUrlRecordId;
    m_lastHaltOnStop = m_haltOnStop;
    m_lastUsername = m_username;
  }

  void VagrantSettingsRecord_Impl::revertToLastValues() {
    CloudSettingsRecord_Impl::revertToLastValues();

    m_userAgreementSigned = m_lastUserAgreementSigned;
    m_serverPath = m_lastServerPath;
    m_serverUrlRecordId = m_lastServerUrlRecordId;
    m_workerPath = m_lastWorkerPath;
    m_workerUrlRecordId = m_lastWorkerUrlRecordId;
    m_haltOnStop = m_lastHaltOnStop;
    m_username = m_lastUsername;
  }

} // detail

VagrantSettingsRecord::VagrantSettingsRecord(const VagrantSettings& vagrantSettings, ProjectDatabase& database)
  : CloudSettingsRecord(boost::shared_ptr<detail::VagrantSettingsRecord_Impl>(
        new detail::VagrantSettingsRecord_Impl(vagrantSettings, database)),
        database)
{
  OS_ASSERT(getImpl<detail::VagrantSettingsRecord_Impl>());
  constructRelatedRecords(vagrantSettings,database);
}

VagrantSettingsRecord::VagrantSettingsRecord(const QSqlQuery& query, ProjectDatabase& database)
  : CloudSettingsRecord(boost::shared_ptr<detail::VagrantSettingsRecord_Impl>(
        new detail::VagrantSettingsRecord_Impl(query, database)),
        database)
{
  OS_ASSERT(getImpl<detail::VagrantSettingsRecord_Impl>());
}

boost::optional<VagrantSettingsRecord> VagrantSettingsRecord::factoryFromQuery(const QSqlQuery& query, ProjectDatabase& database)
{
  OptionalVagrantSettingsRecord result;

  try {
    result = VagrantSettingsRecord(query,database);
  }
  catch (const std::exception& e) {
    LOG(Error,"Unable to construct VagrantSettingsRecord from query, because '"
        << e.what() << "'.");
  }

  return result;
}

std::vector<VagrantSettingsRecord> VagrantSettingsRecord::getVagrantSettingsRecords(ProjectDatabase& database) {
  std::vector<VagrantSettingsRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + CloudSettingsRecord::databaseTableName() + " WHERE cloudSettingsRecordType=:cloudSettingsRecordType"));
  query.bindValue(":cloudSettingsRecordType", CloudSettingsRecordType::VagrantSettingsRecord);
  assertExec(query);
  while (query.next()) {
    result.push_back(VagrantSettingsRecord(query, database));
  }

  return result;
}

boost::optional<VagrantSettingsRecord> VagrantSettingsRecord::getVagrantSettingsRecord(int id, ProjectDatabase& database) {
  boost::optional<VagrantSettingsRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + CloudSettingsRecord::databaseTableName() + " WHERE cloudSettingsRecordType=:cloudSettingsRecordType AND id=:id"));
  query.bindValue(":cloudSettingsRecordType", CloudSettingsRecordType::VagrantSettingsRecord);
  query.bindValue(":id",id);
  assertExec(query);
  if (query.first()) {
    result = VagrantSettingsRecord(query, database);
  }

  return result;
}

UrlRecord VagrantSettingsRecord::serverUrlRecord() const {
  return getImpl<detail::VagrantSettingsRecord_Impl>()->serverUrlRecord();
}

UrlRecord VagrantSettingsRecord::workerUrlRecord() const {
  return getImpl<detail::VagrantSettingsRecord_Impl>()->workerUrlRecord();
}

VagrantSettings VagrantSettingsRecord::vagrantSettings() const {
  return getImpl<detail::VagrantSettingsRecord_Impl>()->vagrantSettings();
}

/// @cond
VagrantSettingsRecord::VagrantSettingsRecord(boost::shared_ptr<detail::VagrantSettingsRecord_Impl> impl)
  : CloudSettingsRecord(impl)
{}

VagrantSettingsRecord::VagrantSettingsRecord(boost::shared_ptr<detail::VagrantSettingsRecord_Impl> impl,
                                             ProjectDatabase database)
  : CloudSettingsRecord(impl, database)
{
  OS_ASSERT(getImpl<detail::VagrantSettingsRecord_Impl>());
}
/// @endcond

void VagrantSettingsRecord::constructRelatedRecords(const VagrantSettings& vagrantSettings, ProjectDatabase& database) {
  VagrantSettingsRecord copyOfThis(getImpl<detail::VagrantSettingsRecord_Impl>());
  bool isNew = database.isNewRecord(copyOfThis);
  if (!isNew) {
    getImpl<detail::VagrantSettingsRecord_Impl>()->revertToLastRecordIds();
  }

  if (isNew || (getImpl<detail::VagrantSettingsRecord_Impl>()->lastUuidLast() != vagrantSettings.versionUUID())) {
    // remove any existing UrlRecords that have this object as its parent
    ObjectRecordVector childUrls = children();
    BOOST_FOREACH(ObjectRecord& childUrl,childUrls) {
      database.removeRecord(childUrl);
    }
    // create new UrlRecords 
    UrlRecord serverUrlRecord(vagrantSettings.serverUrl(),copyOfThis);
    getImpl<detail::VagrantSettingsRecord_Impl>()->setServerUrlRecordId(serverUrlRecord.id());
    UrlRecord workerUrlRecord(vagrantSettings.workerUrl(),copyOfThis);
    getImpl<detail::VagrantSettingsRecord_Impl>()->setWorkerUrlRecordId(workerUrlRecord.id());
  }
}

} // project
} // openstudio
