/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2016, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#include "AWSSettingsRecord.hpp"
#include "AWSSettingsRecord_Impl.hpp"

#include "JoinRecord.hpp"

#include "../utilities/cloud/AWSProvider.hpp"
#include "../utilities/cloud/AWSProvider_Impl.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace project {

namespace detail {

  AWSSettingsRecord_Impl::AWSSettingsRecord_Impl(const AWSSettings& awsSettings,
                                                 ProjectDatabase& database)
    : CloudSettingsRecord_Impl(awsSettings, CloudSettingsRecordType::AWSSettingsRecord, database),
      m_userAgreementSigned(awsSettings.userAgreementSigned()),
      m_numWorkers(awsSettings.numWorkers()),
      m_terminationDelayEnabled(awsSettings.terminationDelayEnabled()),
      m_terminationDelay(awsSettings.terminationDelay()),
      m_region(awsSettings.region()),
      m_serverInstanceType(awsSettings.serverInstanceType()),
      m_workerInstanceType(awsSettings.workerInstanceType())
  {
  }

  AWSSettingsRecord_Impl::AWSSettingsRecord_Impl(const QSqlQuery& query, ProjectDatabase& database)
    : CloudSettingsRecord_Impl(query, database)
  {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    QVariant value;

    value = query.value(AWSSettingsRecord::ColumnsType::userAgreementSigned);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_userAgreementSigned = value.toBool();

    value = query.value(AWSSettingsRecord::ColumnsType::numWorkers);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_numWorkers = value.toUInt();

    value = query.value(AWSSettingsRecord::ColumnsType::terminationDelayEnabled);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_terminationDelayEnabled = value.toBool();

    value = query.value(AWSSettingsRecord::ColumnsType::terminationDelay);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_terminationDelay = value.toUInt();

    value = query.value(AWSSettingsRecord::ColumnsType::region);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_region = value.toString().toStdString();

    value = query.value(AWSSettingsRecord::ColumnsType::serverInstanceType);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_serverInstanceType = value.toString().toStdString();

    value = query.value(AWSSettingsRecord::ColumnsType::workerInstanceType);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_workerInstanceType = value.toString().toStdString();
  }

  void AWSSettingsRecord_Impl::saveRow(const std::shared_ptr<QSqlDatabase>& database) {
    QSqlQuery query(*database);
    this->makeUpdateByIdQuery<AWSSettingsRecord>(query);
    this->bindValues(query);
    assertExec(query);
  }

  CloudSettings AWSSettingsRecord_Impl::cloudSettings() const {
    return awsSettings().cast<CloudSettings>();
  }

  AWSSettings AWSSettingsRecord_Impl::awsSettings() const {
    return AWSSettings(handle(),
                       uuidLast(),
                       m_userAgreementSigned,
                       m_numWorkers,
                       m_terminationDelayEnabled,
                       m_terminationDelay,
                       m_region,
                       m_serverInstanceType,
                       m_workerInstanceType);
  }

  void AWSSettingsRecord_Impl::bindValues(QSqlQuery& query) const {
    CloudSettingsRecord_Impl::bindValues(query);

    query.bindValue(AWSSettingsRecord::ColumnsType::userAgreementSigned,m_userAgreementSigned);
    query.bindValue(AWSSettingsRecord::ColumnsType::numWorkers,m_numWorkers);
    query.bindValue(AWSSettingsRecord::ColumnsType::terminationDelayEnabled,m_terminationDelayEnabled);
    query.bindValue(AWSSettingsRecord::ColumnsType::terminationDelay,m_terminationDelay);
    query.bindValue(AWSSettingsRecord::ColumnsType::region,toQString(m_region));
    query.bindValue(AWSSettingsRecord::ColumnsType::serverInstanceType,toQString(m_serverInstanceType));
    query.bindValue(AWSSettingsRecord::ColumnsType::workerInstanceType,toQString(m_workerInstanceType));
  }

  void AWSSettingsRecord_Impl::setLastValues(const QSqlQuery& query, ProjectDatabase& projectDatabase) {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    CloudSettingsRecord_Impl::setLastValues(query,projectDatabase);

    QVariant value;

    value = query.value(AWSSettingsRecord::ColumnsType::userAgreementSigned);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_lastUserAgreementSigned = value.toBool();

    value = query.value(AWSSettingsRecord::ColumnsType::numWorkers);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_lastNumWorkers = value.toUInt();

    value = query.value(AWSSettingsRecord::ColumnsType::terminationDelayEnabled);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_lastTerminationDelayEnabled = value.toBool();

    value = query.value(AWSSettingsRecord::ColumnsType::terminationDelay);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_lastTerminationDelay = value.toUInt();

    value = query.value(AWSSettingsRecord::ColumnsType::region);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_lastRegion = value.toString().toStdString();

    value = query.value(AWSSettingsRecord::ColumnsType::serverInstanceType);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_lastServerInstanceType = value.toString().toStdString();

    value = query.value(AWSSettingsRecord::ColumnsType::workerInstanceType);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_lastWorkerInstanceType = value.toString().toStdString();
  }

  bool AWSSettingsRecord_Impl::compareValues(const QSqlQuery& query) const {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    bool result = CloudSettingsRecord_Impl::compareValues(query);

    QVariant value;

    value = query.value(AWSSettingsRecord::ColumnsType::userAgreementSigned);
    OS_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_userAgreementSigned == value.toBool());

    value = query.value(AWSSettingsRecord::ColumnsType::numWorkers);
    OS_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_numWorkers == value.toUInt());

    value = query.value(AWSSettingsRecord::ColumnsType::terminationDelayEnabled);
    OS_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_terminationDelayEnabled == value.toBool());

    value = query.value(AWSSettingsRecord::ColumnsType::terminationDelay);
    OS_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_terminationDelay == value.toUInt());

    value = query.value(AWSSettingsRecord::ColumnsType::region);
    OS_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_region == value.toString().toStdString());

    value = query.value(AWSSettingsRecord::ColumnsType::serverInstanceType);
    OS_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_serverInstanceType == value.toString().toStdString());

    value = query.value(AWSSettingsRecord::ColumnsType::workerInstanceType);
    OS_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_workerInstanceType == value.toString().toStdString());

    return result;
  }

  void AWSSettingsRecord_Impl::saveLastValues() {
    CloudSettingsRecord_Impl::saveLastValues();

    m_lastUserAgreementSigned = m_userAgreementSigned;
    m_lastNumWorkers = m_numWorkers;
    m_lastTerminationDelayEnabled = m_terminationDelayEnabled;
    m_lastTerminationDelay = m_terminationDelay;
    m_lastRegion = m_region;
    m_lastServerInstanceType = m_serverInstanceType;
    m_lastWorkerInstanceType = m_workerInstanceType;
  }

  void AWSSettingsRecord_Impl::revertToLastValues() {
    CloudSettingsRecord_Impl::revertToLastValues();

    m_userAgreementSigned = m_lastUserAgreementSigned;
    m_numWorkers = m_lastNumWorkers;
    m_terminationDelayEnabled = m_lastTerminationDelayEnabled;
    m_terminationDelay = m_lastTerminationDelay;
    m_region = m_lastRegion;
    m_serverInstanceType = m_lastServerInstanceType;
    m_workerInstanceType = m_lastWorkerInstanceType;
  }

} // detail

AWSSettingsRecord::AWSSettingsRecord(const AWSSettings& awsSettings, ProjectDatabase& database)
  : CloudSettingsRecord(std::shared_ptr<detail::AWSSettingsRecord_Impl>(
        new detail::AWSSettingsRecord_Impl(awsSettings, database)),
        database)
{
  OS_ASSERT(getImpl<detail::AWSSettingsRecord_Impl>());
}

AWSSettingsRecord::AWSSettingsRecord(const QSqlQuery& query, ProjectDatabase& database)
  : CloudSettingsRecord(std::shared_ptr<detail::AWSSettingsRecord_Impl>(
        new detail::AWSSettingsRecord_Impl(query, database)),
        database)
{
  OS_ASSERT(getImpl<detail::AWSSettingsRecord_Impl>());
}

boost::optional<AWSSettingsRecord> AWSSettingsRecord::factoryFromQuery(const QSqlQuery& query, ProjectDatabase& database)
{
  OptionalAWSSettingsRecord result;

  try {
    result = AWSSettingsRecord(query,database);
  }
  catch (const std::exception& e) {
    LOG(Error,"Unable to construct AWSSettingsRecord from query, because '" << e.what() << "'.");
  }

  return result;
}

std::vector<AWSSettingsRecord> AWSSettingsRecord::getAWSSettingsRecords(ProjectDatabase& database) {
  std::vector<AWSSettingsRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + CloudSettingsRecord::databaseTableName() + " WHERE cloudSettingsRecordType=:cloudSettingsRecordType"));
  query.bindValue(":cloudSettingsRecordType", CloudSettingsRecordType::AWSSettingsRecord);
  assertExec(query);
  while (query.next()) {
    result.push_back(AWSSettingsRecord(query, database));
  }

  return result;
}

boost::optional<AWSSettingsRecord> AWSSettingsRecord::getAWSSettingsRecord(int id, ProjectDatabase& database) {
  boost::optional<AWSSettingsRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + CloudSettingsRecord::databaseTableName() + " WHERE cloudSettingsRecordType=:cloudSettingsRecordType AND id=:id"));
  query.bindValue(":cloudSettingsRecordType", CloudSettingsRecordType::AWSSettingsRecord);
  query.bindValue(":id",id);
  assertExec(query);
  if (query.first()) {
    result = AWSSettingsRecord(query, database);
  }

  return result;
}

AWSSettings AWSSettingsRecord::awsSettings() const {
  return getImpl<detail::AWSSettingsRecord_Impl>()->awsSettings();
}

/// @cond
AWSSettingsRecord::AWSSettingsRecord(std::shared_ptr<detail::AWSSettingsRecord_Impl> impl)
  : CloudSettingsRecord(impl)
{}

AWSSettingsRecord::AWSSettingsRecord(std::shared_ptr<detail::AWSSettingsRecord_Impl> impl,
                                     ProjectDatabase database)
  : CloudSettingsRecord(impl, database)
{
  OS_ASSERT(getImpl<detail::AWSSettingsRecord_Impl>());
}
/// @endcond

} // project
} // openstudio

