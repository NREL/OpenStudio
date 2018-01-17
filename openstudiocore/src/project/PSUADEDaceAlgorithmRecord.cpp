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

#include "PSUADEDaceAlgorithmRecord.hpp"
#include "PSUADEDaceAlgorithmRecord_Impl.hpp"
#include "AnalysisRecord.hpp"
#include "JoinRecord.hpp"
#include "FileReferenceRecord.hpp"

#include "../analysis/PSUADEDaceAlgorithm.hpp"
#include "../analysis/PSUADEDaceAlgorithm_Impl.hpp"
#include "../analysis/PSUADEDaceAlgorithmOptions.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/core/Optional.hpp"
#include "../utilities/data/Attribute.hpp"

namespace openstudio {
namespace project {

namespace detail {

  PSUADEDaceAlgorithmRecord_Impl::PSUADEDaceAlgorithmRecord_Impl(
      const analysis::PSUADEDaceAlgorithm& psuadedaceAlgorithm,
      AnalysisRecord& analysisRecord)
    : DakotaAlgorithmRecord_Impl(psuadedaceAlgorithm,
                                 DakotaAlgorithmRecordType::PSUADEDaceAlgorithmRecord,
                                 analysisRecord)
  {}

  PSUADEDaceAlgorithmRecord_Impl::PSUADEDaceAlgorithmRecord_Impl(const QSqlQuery& query,
                                                           ProjectDatabase& database)
    : DakotaAlgorithmRecord_Impl(query, database)
  {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());
  }

  std::vector<ObjectRecord> PSUADEDaceAlgorithmRecord_Impl::resources() const {
    ObjectRecordVector result;
    return result;
  }

  std::vector<JoinRecord> PSUADEDaceAlgorithmRecord_Impl::joinRecords() const {
    JoinRecordVector result;
    return result;
  }

  void PSUADEDaceAlgorithmRecord_Impl::saveRow(const std::shared_ptr<QSqlDatabase> &database)
  {
    QSqlQuery query(*database);
    this->makeUpdateByIdQuery<PSUADEDaceAlgorithmRecord>(query);
    this->bindValues(query);
    assertExec(query);
  }

  analysis::Algorithm PSUADEDaceAlgorithmRecord_Impl::algorithm() const {
    return psuadedaceAlgorithm().cast<analysis::Algorithm>();
  }

  analysis::DakotaAlgorithm PSUADEDaceAlgorithmRecord_Impl::dakotaAlgorithm() const {
    return psuadedaceAlgorithm().cast<analysis::DakotaAlgorithm>();
  }

  analysis::PSUADEDaceAlgorithm PSUADEDaceAlgorithmRecord_Impl::psuadedaceAlgorithm() const {
    analysis::PSUADEDaceAlgorithmOptions opts(options());
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

    return analysis::PSUADEDaceAlgorithm(handle(),
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

  void PSUADEDaceAlgorithmRecord_Impl::bindValues(QSqlQuery& query) const {
    DakotaAlgorithmRecord_Impl::bindValues(query);
  }

  void PSUADEDaceAlgorithmRecord_Impl::setLastValues(const QSqlQuery& query, ProjectDatabase& projectDatabase) {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    DakotaAlgorithmRecord_Impl::setLastValues(query,projectDatabase);
  }

  bool PSUADEDaceAlgorithmRecord_Impl::compareValues(const QSqlQuery& query) const {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    bool result = DakotaAlgorithmRecord_Impl::compareValues(query);

    return result;
  }

  void PSUADEDaceAlgorithmRecord_Impl::saveLastValues() {
    DakotaAlgorithmRecord_Impl::saveLastValues();
  }

  void PSUADEDaceAlgorithmRecord_Impl::revertToLastValues() {
    DakotaAlgorithmRecord_Impl::revertToLastValues();
  }

} // detail

PSUADEDaceAlgorithmRecord::PSUADEDaceAlgorithmRecord(
    const analysis::PSUADEDaceAlgorithm& psuadedaceAlgorithm,
    AnalysisRecord& analysisRecord)
  : DakotaAlgorithmRecord(std::shared_ptr<detail::PSUADEDaceAlgorithmRecord_Impl>(
        new detail::PSUADEDaceAlgorithmRecord_Impl(psuadedaceAlgorithm, analysisRecord)),
        analysisRecord.projectDatabase(),
        psuadedaceAlgorithm)
{
  OS_ASSERT(getImpl<detail::PSUADEDaceAlgorithmRecord_Impl>());

  constructRelatedRecords(psuadedaceAlgorithm);
}

PSUADEDaceAlgorithmRecord::PSUADEDaceAlgorithmRecord(const QSqlQuery& query, ProjectDatabase& database)
  : DakotaAlgorithmRecord(std::shared_ptr<detail::PSUADEDaceAlgorithmRecord_Impl>(
        new detail::PSUADEDaceAlgorithmRecord_Impl(query, database)),
        database,
        boost::optional<analysis::DakotaAlgorithm>())
{
  OS_ASSERT(getImpl<detail::PSUADEDaceAlgorithmRecord_Impl>());
}

PSUADEDaceAlgorithmRecord::PSUADEDaceAlgorithmRecord(
    std::shared_ptr<detail::PSUADEDaceAlgorithmRecord_Impl> impl,
    ProjectDatabase database)
  : DakotaAlgorithmRecord(impl, database, boost::optional<analysis::DakotaAlgorithm>())
{
  OS_ASSERT(getImpl<detail::PSUADEDaceAlgorithmRecord_Impl>());
}

boost::optional<PSUADEDaceAlgorithmRecord> PSUADEDaceAlgorithmRecord::factoryFromQuery(
    const QSqlQuery& query, ProjectDatabase& database)
{
  OptionalPSUADEDaceAlgorithmRecord result;

  try {
    result = PSUADEDaceAlgorithmRecord(query,database);
  }

  catch (const std::exception& e) {
     LOG(Error,"Unable to construct PSUADEDaceAlgorithmRecord from query, because '"
         << e.what() << "'.");
  }

  return result;
}

std::vector<PSUADEDaceAlgorithmRecord> PSUADEDaceAlgorithmRecord::getPSUADEDaceAlgorithmRecords(ProjectDatabase& database) {
  std::vector<PSUADEDaceAlgorithmRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + AlgorithmRecord::databaseTableName() +
      " WHERE algorithmRecordType=:algorithmRecordType AND " +
      "dakotaAlgorithmRecordType=:dakotaAlgorithmRecordType"));
  query.bindValue(":dakotaAlgorithmRecordType", DakotaAlgorithmRecordType::PSUADEDaceAlgorithmRecord);
  assertExec(query);
  while (query.next()) {
    result.push_back(PSUADEDaceAlgorithmRecord(query, database));
  }

  return result;
}

boost::optional<PSUADEDaceAlgorithmRecord> PSUADEDaceAlgorithmRecord::getPSUADEDaceAlgorithmRecord(int id, ProjectDatabase& database) {
  boost::optional<PSUADEDaceAlgorithmRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + AlgorithmRecord::databaseTableName() +
      " WHERE algorithmRecordType=:algorithmRecordType AND " +
      "dakotaAlgorithmRecordType=:dakotaAlgorithmRecordType AND " +
      "id=:id"));
  query.bindValue(":dakotaAlgorithmRecordType", DakotaAlgorithmRecordType::PSUADEDaceAlgorithmRecord);
  query.bindValue(":id",id);
  assertExec(query);
  if (query.first()) {
    result = PSUADEDaceAlgorithmRecord(query, database);
  }

  return result;
}

analysis::PSUADEDaceAlgorithm PSUADEDaceAlgorithmRecord::psuadedaceAlgorithm() const {
  return getImpl<detail::PSUADEDaceAlgorithmRecord_Impl>()->psuadedaceAlgorithm();

}

/// @cond
PSUADEDaceAlgorithmRecord::PSUADEDaceAlgorithmRecord(std::shared_ptr<detail::PSUADEDaceAlgorithmRecord_Impl> impl)
  : DakotaAlgorithmRecord(impl)
{}
/// @endcond

} // project
} // openstudio

