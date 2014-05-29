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

#include "DDACEAlgorithmRecord.hpp"
#include "DDACEAlgorithmRecord_Impl.hpp"
#include "AnalysisRecord.hpp"
#include "JoinRecord.hpp"
#include "FileReferenceRecord.hpp"

#include "../analysis/DDACEAlgorithm.hpp"
#include "../analysis/DDACEAlgorithm_Impl.hpp"
#include "../analysis/DDACEAlgorithmOptions.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/core/Optional.hpp"
#include "../utilities/data/Attribute.hpp"

namespace openstudio {
namespace project {

namespace detail {

  DDACEAlgorithmRecord_Impl::DDACEAlgorithmRecord_Impl(
      const analysis::DDACEAlgorithm& ddaceAlgorithm,
      AnalysisRecord& analysisRecord)
    : DakotaAlgorithmRecord_Impl(ddaceAlgorithm,
                                 DakotaAlgorithmRecordType::DDACEAlgorithmRecord,
                                 analysisRecord),
      m_algorithmType(ddaceAlgorithm.ddaceAlgorithmOptions().algorithmType())
  {}

  DDACEAlgorithmRecord_Impl::DDACEAlgorithmRecord_Impl(const QSqlQuery& query,
                                                       ProjectDatabase& database)
    : DakotaAlgorithmRecord_Impl(query, database)
  {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    QVariant value;

    value = query.value(DDACEAlgorithmRecord::ColumnsType::ddaceAlgorithmType);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_algorithmType = analysis::DDACEAlgorithmType(value.toInt());
  }

  std::vector<ObjectRecord> DDACEAlgorithmRecord_Impl::resources() const {
    ObjectRecordVector result;
    return result;
  }

  std::vector<JoinRecord> DDACEAlgorithmRecord_Impl::joinRecords() const {
    JoinRecordVector result;
    return result;
  }

  void DDACEAlgorithmRecord_Impl::saveRow(const std::shared_ptr<QSqlDatabase> &database)
  {
    QSqlQuery query(*database);
    this->makeUpdateByIdQuery<DDACEAlgorithmRecord>(query);
    this->bindValues(query);
    assertExec(query);
  }

  analysis::Algorithm DDACEAlgorithmRecord_Impl::algorithm() const {
    return ddaceAlgorithm().cast<analysis::Algorithm>();
  }

  analysis::DakotaAlgorithm DDACEAlgorithmRecord_Impl::dakotaAlgorithm() const {
    return ddaceAlgorithm().cast<analysis::DakotaAlgorithm>();
  }

  analysis::DDACEAlgorithm DDACEAlgorithmRecord_Impl::ddaceAlgorithm() const {
    analysis::DDACEAlgorithmOptions opts(m_algorithmType, options());
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

    return analysis::DDACEAlgorithm(handle(),
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

  void DDACEAlgorithmRecord_Impl::bindValues(QSqlQuery& query) const {
    DakotaAlgorithmRecord_Impl::bindValues(query);

    query.bindValue(DDACEAlgorithmRecord::ColumnsType::ddaceAlgorithmType,m_algorithmType.value());
  }

  void DDACEAlgorithmRecord_Impl::setLastValues(const QSqlQuery& query, ProjectDatabase& projectDatabase) {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    DakotaAlgorithmRecord_Impl::setLastValues(query,projectDatabase);

    QVariant value;

    value = query.value(DDACEAlgorithmRecord::ColumnsType::ddaceAlgorithmType);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_lastAlgorithmType = analysis::DDACEAlgorithmType(value.toInt());
  }

  bool DDACEAlgorithmRecord_Impl::compareValues(const QSqlQuery& query) const {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    bool result = DakotaAlgorithmRecord_Impl::compareValues(query);

    QVariant value;

    value = query.value(DDACEAlgorithmRecord::ColumnsType::ddaceAlgorithmType);
    OS_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_algorithmType == analysis::DDACEAlgorithmType(value.toInt()));

    return result;
  }

  void DDACEAlgorithmRecord_Impl::saveLastValues() {
    DakotaAlgorithmRecord_Impl::saveLastValues();

    m_lastAlgorithmType = m_algorithmType;
  }

  void DDACEAlgorithmRecord_Impl::revertToLastValues() {
    DakotaAlgorithmRecord_Impl::revertToLastValues();

    m_algorithmType = m_lastAlgorithmType;
  }

} // detail

DDACEAlgorithmRecord::DDACEAlgorithmRecord(const analysis::DDACEAlgorithm& ddaceAlgorithm,
                                           AnalysisRecord& analysisRecord)
  : DakotaAlgorithmRecord(std::shared_ptr<detail::DDACEAlgorithmRecord_Impl>(
        new detail::DDACEAlgorithmRecord_Impl(ddaceAlgorithm, analysisRecord)),
        analysisRecord.projectDatabase(),
        ddaceAlgorithm)
{
  OS_ASSERT(getImpl<detail::DDACEAlgorithmRecord_Impl>());

  constructRelatedRecords(ddaceAlgorithm);
}

DDACEAlgorithmRecord::DDACEAlgorithmRecord(const QSqlQuery& query, ProjectDatabase& database)
  : DakotaAlgorithmRecord(std::shared_ptr<detail::DDACEAlgorithmRecord_Impl>(
        new detail::DDACEAlgorithmRecord_Impl(query, database)),
        database,
        boost::optional<analysis::DakotaAlgorithm>())
{
  OS_ASSERT(getImpl<detail::DDACEAlgorithmRecord_Impl>());
}

DDACEAlgorithmRecord::DDACEAlgorithmRecord(std::shared_ptr<detail::DDACEAlgorithmRecord_Impl> impl,
                                           ProjectDatabase database)
  : DakotaAlgorithmRecord(impl, database, boost::optional<analysis::DakotaAlgorithm>())
{
  OS_ASSERT(getImpl<detail::DDACEAlgorithmRecord_Impl>());
}

boost::optional<DDACEAlgorithmRecord> DDACEAlgorithmRecord::factoryFromQuery(
    const QSqlQuery& query, ProjectDatabase& database)
{
  OptionalDDACEAlgorithmRecord result;

  try {
    result = DDACEAlgorithmRecord(query,database);
  }
  catch (const std::exception& e) {
    LOG(Error,"Unable to construct DDACEAlgorithmRecord from query, because '"
        << e.what() << "'.");
  }

  return result;
}

std::vector<DDACEAlgorithmRecord> DDACEAlgorithmRecord::getDDACEAlgorithmRecords(ProjectDatabase& database) {
  std::vector<DDACEAlgorithmRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + AlgorithmRecord::databaseTableName() +
      " WHERE algorithmRecordType=:algorithmRecordType AND " +
      "dakotaAlgorithmRecordType=:dakotaAlgorithmRecordType"));
  query.bindValue(":algorithmRecordType", AlgorithmRecordType::DakotaAlgorithmRecord);
  query.bindValue(":dakotaAlgorithmRecordType", DakotaAlgorithmRecordType::DDACEAlgorithmRecord);
  assertExec(query);
  while (query.next()) {
    result.push_back(DDACEAlgorithmRecord(query, database));
  }

  return result;
}

boost::optional<DDACEAlgorithmRecord> DDACEAlgorithmRecord::getDDACEAlgorithmRecord(int id, ProjectDatabase& database) {
  boost::optional<DDACEAlgorithmRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + AlgorithmRecord::databaseTableName() +
      " WHERE algorithmRecordType=:algorithmRecordType AND " +
      "dakotaAlgorithmRecordType=:dakotaAlgorithmRecordType AND " +
      "id=:id"));
  query.bindValue(":algorithmRecordType", AlgorithmRecordType::DakotaAlgorithmRecord);
  query.bindValue(":dakotaAlgorithmRecordType", DakotaAlgorithmRecordType::DDACEAlgorithmRecord);
  query.bindValue(":id",id);
  assertExec(query);
  if (query.first()) {
    result = DDACEAlgorithmRecord(query, database);
  }

  return result;
}

analysis::DDACEAlgorithm DDACEAlgorithmRecord::ddaceAlgorithm() const {
  return getImpl<detail::DDACEAlgorithmRecord_Impl>()->ddaceAlgorithm();
}

/// @cond
DDACEAlgorithmRecord::DDACEAlgorithmRecord(std::shared_ptr<detail::DDACEAlgorithmRecord_Impl> impl)
  : DakotaAlgorithmRecord(impl)
{}
/// @endcond

} // project
} // openstudio

