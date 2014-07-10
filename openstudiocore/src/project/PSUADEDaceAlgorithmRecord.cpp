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

