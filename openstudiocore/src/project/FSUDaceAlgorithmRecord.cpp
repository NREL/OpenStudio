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

#include "FSUDaceAlgorithmRecord.hpp"
#include "FSUDaceAlgorithmRecord_Impl.hpp"
#include "AnalysisRecord.hpp"
#include "JoinRecord.hpp"
#include "FileReferenceRecord.hpp"

#include "../analysis/FSUDaceAlgorithm.hpp"
#include "../analysis/FSUDaceAlgorithm_Impl.hpp"
#include "../analysis/FSUDaceAlgorithmOptions.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/core/Optional.hpp"
#include "../utilities/data/Attribute.hpp"

namespace openstudio {
namespace project {

namespace detail {

  FSUDaceAlgorithmRecord_Impl::FSUDaceAlgorithmRecord_Impl(
      const analysis::FSUDaceAlgorithm& fsudaceAlgorithm,
      AnalysisRecord& analysisRecord)
    : DakotaAlgorithmRecord_Impl(fsudaceAlgorithm,
                                 DakotaAlgorithmRecordType::FSUDaceAlgorithmRecord,
                                 analysisRecord),
      m_algorithmType(fsudaceAlgorithm.fsudaceAlgorithmOptions().algorithmType()),
      m_trialType(fsudaceAlgorithm.fsudaceAlgorithmOptions().trialType())
  {}

  FSUDaceAlgorithmRecord_Impl::FSUDaceAlgorithmRecord_Impl(const QSqlQuery& query,
                                                           ProjectDatabase& database)
    : DakotaAlgorithmRecord_Impl(query, database)
  {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    QVariant value;

    value = query.value(FSUDaceAlgorithmRecord::ColumnsType::fsudaceAlgorithmType);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_algorithmType = analysis::FSUDaceAlgorithmType(value.toInt());

    value = query.value(FSUDaceAlgorithmRecord::ColumnsType::fsudaceTrialType);
    if (value.isValid() && !value.isNull()) {
      m_trialType = analysis::FSUDaceCvtTrialType(value.toInt());
    }
  }

  std::vector<ObjectRecord> FSUDaceAlgorithmRecord_Impl::resources() const {
    ObjectRecordVector result;
    return result;
  }

  std::vector<JoinRecord> FSUDaceAlgorithmRecord_Impl::joinRecords() const {
    JoinRecordVector result;
    return result;
  }

  void FSUDaceAlgorithmRecord_Impl::saveRow(const std::shared_ptr<QSqlDatabase> &database)
  {
    QSqlQuery query(*database);
    this->makeUpdateByIdQuery<FSUDaceAlgorithmRecord>(query);
    this->bindValues(query);
    assertExec(query);
  }

  analysis::Algorithm FSUDaceAlgorithmRecord_Impl::algorithm() const {
    return fsudaceAlgorithm().cast<analysis::Algorithm>();
  }

  analysis::DakotaAlgorithm FSUDaceAlgorithmRecord_Impl::dakotaAlgorithm() const {
    return fsudaceAlgorithm().cast<analysis::DakotaAlgorithm>();
  }

  analysis::FSUDaceAlgorithm FSUDaceAlgorithmRecord_Impl::fsudaceAlgorithm() const {
    analysis::FSUDaceAlgorithmOptions opts(m_algorithmType, m_trialType, options());
    OptionalFileReference restartFile, outFile;
    OptionalFileReferenceRecord ofr = restartFileReferenceRecord();
    if (ofr) {
      restartFile = ofr->fileReference();
    }
    ofr = outFileReferenceRecord();
    if (ofr) {
      outFile = ofr->fileReference();
    }

    boost::optional<runmanager::Job> job;
    if (this->jobUUID()){
      try {
        job = this->projectDatabase().runManager().getJob(this->jobUUID().get());
      }
      catch (const std::exception& e) {
        LOG(Error, "Job " << toString(this->jobUUID().get()) << " not found in RunManager. "
            << e.what());
      }
    }

    return analysis::FSUDaceAlgorithm(handle(),
                                    uuidLast(),
                                    displayName(),
                                    description(),
                                    complete(),
                                    failed(),
                                    iter(),
                                    opts,
                                    restartFile,
                                    outFile,
                                    job);
  }

  void FSUDaceAlgorithmRecord_Impl::bindValues(QSqlQuery& query) const {
    DakotaAlgorithmRecord_Impl::bindValues(query);

    query.bindValue(FSUDaceAlgorithmRecord::ColumnsType::fsudaceAlgorithmType,m_algorithmType.value());

    if (m_trialType) {
       query.bindValue(FSUDaceAlgorithmRecord::ColumnsType::fsudaceTrialType,m_trialType->value());
    }
    else {
      query.bindValue(FSUDaceAlgorithmRecord::ColumnsType::fsudaceTrialType,QVariant(QVariant::Int));
    }
  }

  void FSUDaceAlgorithmRecord_Impl::setLastValues(const QSqlQuery& query,
                          ProjectDatabase& projectDatabase) {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    DakotaAlgorithmRecord_Impl::setLastValues(query,projectDatabase);

    QVariant value;

    value = query.value(FSUDaceAlgorithmRecord::ColumnsType::fsudaceAlgorithmType);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_lastAlgorithmType = analysis::FSUDaceAlgorithmType(value.toInt());

    value = query.value(FSUDaceAlgorithmRecord::ColumnsType::fsudaceTrialType);
    if (value.isValid() && !value.isNull()) {
      m_lastTrialType = analysis::FSUDaceCvtTrialType(value.toInt());
    }
    else {
      m_lastTrialType.reset();
    }
  }

  bool FSUDaceAlgorithmRecord_Impl::compareValues(const QSqlQuery& query) const {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    bool result = DakotaAlgorithmRecord_Impl::compareValues(query);

    QVariant value;

    value = query.value(FSUDaceAlgorithmRecord::ColumnsType::fsudaceAlgorithmType);
    OS_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_algorithmType == analysis::FSUDaceAlgorithmType(value.toInt()));

    value = query.value(FSUDaceAlgorithmRecord::ColumnsType::fsudaceTrialType);
    if (value.isValid() && !value.isNull()) {
      result = result && m_trialType && (*m_trialType == analysis::FSUDaceCvtTrialType(value.toInt()));
    }
    else {
      result = result && !m_trialType;
    }

    return result;
  }

  void FSUDaceAlgorithmRecord_Impl::saveLastValues() {
    DakotaAlgorithmRecord_Impl::saveLastValues();

    m_lastAlgorithmType = m_algorithmType;
    m_lastTrialType = m_trialType;
  }

  void FSUDaceAlgorithmRecord_Impl::revertToLastValues() {
    DakotaAlgorithmRecord_Impl::revertToLastValues();

    m_algorithmType = m_lastAlgorithmType;
    m_trialType = m_lastTrialType;
  }

} // detail

FSUDaceAlgorithmRecord::FSUDaceAlgorithmRecord(const analysis::FSUDaceAlgorithm& fsudaceAlgorithm,
                                               AnalysisRecord& analysisRecord)
  : DakotaAlgorithmRecord(std::shared_ptr<detail::FSUDaceAlgorithmRecord_Impl>(
        new detail::FSUDaceAlgorithmRecord_Impl(fsudaceAlgorithm, analysisRecord)),
        analysisRecord.projectDatabase(),
        fsudaceAlgorithm)
{
  OS_ASSERT(getImpl<detail::FSUDaceAlgorithmRecord_Impl>());

  constructRelatedRecords(fsudaceAlgorithm);
}

FSUDaceAlgorithmRecord::FSUDaceAlgorithmRecord(const QSqlQuery& query, ProjectDatabase& database)
  : DakotaAlgorithmRecord(std::shared_ptr<detail::FSUDaceAlgorithmRecord_Impl>(
        new detail::FSUDaceAlgorithmRecord_Impl(query, database)),
        database,
        boost::none)
{
  OS_ASSERT(getImpl<detail::FSUDaceAlgorithmRecord_Impl>());
}

FSUDaceAlgorithmRecord::FSUDaceAlgorithmRecord(
  std::shared_ptr<detail::FSUDaceAlgorithmRecord_Impl> impl,
    ProjectDatabase database)
  : DakotaAlgorithmRecord(impl, database, boost::none)
{
  OS_ASSERT(getImpl<detail::FSUDaceAlgorithmRecord_Impl>());
}

boost::optional<FSUDaceAlgorithmRecord> FSUDaceAlgorithmRecord::factoryFromQuery(
    const QSqlQuery& query, ProjectDatabase& database)
{
  OptionalFSUDaceAlgorithmRecord result;

  try {
    result = FSUDaceAlgorithmRecord(query,database);
  }

  catch (const std::exception& e) {
     LOG(Error,"Unable to construct FSUDaceAlgorithmRecord from query, because '"
         << e.what() << "'.");
  }

  return result;
}

std::vector<FSUDaceAlgorithmRecord> FSUDaceAlgorithmRecord::getFSUDaceAlgorithmRecords(ProjectDatabase& database) {
  std::vector<FSUDaceAlgorithmRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + AlgorithmRecord::databaseTableName() +
      " WHERE algorithmRecordType=:algorithmRecordType AND " +
      "dakotaAlgorithmRecordType=:dakotaAlgorithmRecordType"));
  query.bindValue(":dakotaAlgorithmRecordType", DakotaAlgorithmRecordType::FSUDaceAlgorithmRecord);
  assertExec(query);
  while (query.next()) {
    result.push_back(FSUDaceAlgorithmRecord(query, database));
  }

  return result;
}

boost::optional<FSUDaceAlgorithmRecord> FSUDaceAlgorithmRecord::getFSUDaceAlgorithmRecord(
  int id, ProjectDatabase& database) {
  boost::optional<FSUDaceAlgorithmRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + AlgorithmRecord::databaseTableName() +
      " WHERE algorithmRecordType=:algorithmRecordType AND " +
      "dakotaAlgorithmRecordType=:dakotaAlgorithmRecordType AND " +
      "id=:id"));
  query.bindValue(":dakotaAlgorithmRecordType", DakotaAlgorithmRecordType::FSUDaceAlgorithmRecord);
  query.bindValue(":id",id);
  assertExec(query);
  if (query.first()) {
    result = FSUDaceAlgorithmRecord(query, database);
  }

  return result;
}

analysis::FSUDaceAlgorithm FSUDaceAlgorithmRecord::fsudaceAlgorithm() const {
  return getImpl<detail::FSUDaceAlgorithmRecord_Impl>()->fsudaceAlgorithm();
}

/// @cond
FSUDaceAlgorithmRecord::FSUDaceAlgorithmRecord(std::shared_ptr<detail::FSUDaceAlgorithmRecord_Impl> impl)
  : DakotaAlgorithmRecord(impl)
{}
/// @endcond

} // project
} // openstudio

