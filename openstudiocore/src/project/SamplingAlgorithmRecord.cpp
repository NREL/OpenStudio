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

#include "SamplingAlgorithmRecord.hpp"
#include "SamplingAlgorithmRecord_Impl.hpp"
#include "AnalysisRecord.hpp"
#include "JoinRecord.hpp"
#include "FileReferenceRecord.hpp"

#include "../analysis/SamplingAlgorithm.hpp"
#include "../analysis/SamplingAlgorithm_Impl.hpp"
#include "../analysis/SamplingAlgorithmOptions.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/core/Optional.hpp"
#include "../utilities/data/Attribute.hpp"

namespace openstudio {
namespace project {

namespace detail {

  SamplingAlgorithmRecord_Impl::SamplingAlgorithmRecord_Impl(
      const analysis::SamplingAlgorithm& samplingAlgorithm,
      AnalysisRecord& analysisRecord)
    : DakotaAlgorithmRecord_Impl(samplingAlgorithm,
                                 DakotaAlgorithmRecordType::SamplingAlgorithmRecord,
                                 analysisRecord),
      m_sampleType(samplingAlgorithm.samplingAlgorithmOptions().sampleType()),
      m_rngType(samplingAlgorithm.samplingAlgorithmOptions().rngType())
  {}

  SamplingAlgorithmRecord_Impl::SamplingAlgorithmRecord_Impl(const QSqlQuery& query,
                                                           ProjectDatabase& database)
    : DakotaAlgorithmRecord_Impl(query, database)
  {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    QVariant value;

    value = query.value(SamplingAlgorithmRecord::ColumnsType::samplingAlgorithmSampleType);
    if (value.isValid() && !value.isNull()) {
      m_sampleType = analysis::SamplingAlgorithmSampleType(value.toInt());
    }

    value = query.value(SamplingAlgorithmRecord::ColumnsType::samplingAlgorithmRNGType);
    if (value.isValid() && !value.isNull()) {
      m_rngType = analysis::SamplingAlgorithmRNGType(value.toInt());
    }
  }

  std::vector<ObjectRecord> SamplingAlgorithmRecord_Impl::resources() const {
    ObjectRecordVector result;
    return result;
  }

  std::vector<JoinRecord> SamplingAlgorithmRecord_Impl::joinRecords() const {
    JoinRecordVector result;
    return result;
  }

  void SamplingAlgorithmRecord_Impl::saveRow(const std::shared_ptr<QSqlDatabase> &database)
  {
    QSqlQuery query(*database);
    this->makeUpdateByIdQuery<SamplingAlgorithmRecord>(query);
    this->bindValues(query);
    assertExec(query);
  }

  analysis::Algorithm SamplingAlgorithmRecord_Impl::algorithm() const {
    return samplingAlgorithm().cast<analysis::Algorithm>();
  }

  analysis::DakotaAlgorithm SamplingAlgorithmRecord_Impl::dakotaAlgorithm() const {
    return samplingAlgorithm().cast<analysis::DakotaAlgorithm>();
  }

  analysis::SamplingAlgorithm SamplingAlgorithmRecord_Impl::samplingAlgorithm() const {
    analysis::SamplingAlgorithmOptions opts(m_sampleType,
                                            m_rngType,
                                            options());
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

    return analysis::SamplingAlgorithm(handle(),
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

  void SamplingAlgorithmRecord_Impl::bindValues(QSqlQuery& query) const {
    DakotaAlgorithmRecord_Impl::bindValues(query);

    if (m_sampleType) {
       query.bindValue(SamplingAlgorithmRecord::ColumnsType::samplingAlgorithmSampleType,m_sampleType->value());
    }
    else {
      query.bindValue(SamplingAlgorithmRecord::ColumnsType::samplingAlgorithmSampleType,QVariant(QVariant::Int));
    }

    if (m_rngType) {
       query.bindValue(SamplingAlgorithmRecord::ColumnsType::samplingAlgorithmRNGType,m_rngType->value());
    }
    else {
      query.bindValue(SamplingAlgorithmRecord::ColumnsType::samplingAlgorithmRNGType,QVariant(QVariant::Int));
    }
  }

  void SamplingAlgorithmRecord_Impl::setLastValues(const QSqlQuery& query,
                          ProjectDatabase& projectDatabase) {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    DakotaAlgorithmRecord_Impl::setLastValues(query,projectDatabase);

    QVariant value;

    value = query.value(SamplingAlgorithmRecord::ColumnsType::samplingAlgorithmSampleType);
    if (value.isValid() && !value.isNull()) {
      m_lastSampleType = analysis::SamplingAlgorithmSampleType(value.toInt());
    }
    else {
      m_lastSampleType.reset();
    }

    value = query.value(SamplingAlgorithmRecord::ColumnsType::samplingAlgorithmRNGType);
    if (value.isValid() && !value.isNull()) {
      m_lastRNGType = analysis::SamplingAlgorithmRNGType(value.toInt());
    }
    else {
      m_lastRNGType.reset();
    }
  }

  bool SamplingAlgorithmRecord_Impl::compareValues(const QSqlQuery& query) const {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    bool result = DakotaAlgorithmRecord_Impl::compareValues(query);

    QVariant value;

    value = query.value(SamplingAlgorithmRecord::ColumnsType::samplingAlgorithmSampleType);
    if (value.isValid() && !value.isNull()) {
      result = result && m_sampleType && (*m_sampleType == analysis::SamplingAlgorithmSampleType(value.toInt()));
    }
    else {
      result = result && !m_sampleType;
    }

    value = query.value(SamplingAlgorithmRecord::ColumnsType::samplingAlgorithmRNGType);
    if (value.isValid() && !value.isNull()) {
      result = result && m_rngType && (*m_rngType == analysis::SamplingAlgorithmRNGType(value.toInt()));
    }
    else {
      result = result && !m_rngType;
    }

    return result;
  }

  void SamplingAlgorithmRecord_Impl::saveLastValues() {
    DakotaAlgorithmRecord_Impl::saveLastValues();

    m_lastSampleType = m_sampleType;
    m_lastRNGType = m_rngType;
  }

  void SamplingAlgorithmRecord_Impl::revertToLastValues() {
    DakotaAlgorithmRecord_Impl::revertToLastValues();

    m_sampleType = m_lastSampleType;
    m_rngType = m_lastRNGType;
  }

} // detail

SamplingAlgorithmRecord::SamplingAlgorithmRecord(const analysis::SamplingAlgorithm& samplingAlgorithm,
                                               AnalysisRecord& analysisRecord)
  : DakotaAlgorithmRecord(std::shared_ptr<detail::SamplingAlgorithmRecord_Impl>(
        new detail::SamplingAlgorithmRecord_Impl(samplingAlgorithm, analysisRecord)),
        analysisRecord.projectDatabase(),
        samplingAlgorithm)
{
  OS_ASSERT(getImpl<detail::SamplingAlgorithmRecord_Impl>());

  constructRelatedRecords(samplingAlgorithm);
}

SamplingAlgorithmRecord::SamplingAlgorithmRecord(const QSqlQuery& query, ProjectDatabase& database)
  : DakotaAlgorithmRecord(std::shared_ptr<detail::SamplingAlgorithmRecord_Impl>(
        new detail::SamplingAlgorithmRecord_Impl(query, database)),
        database,
        boost::none)
{
  OS_ASSERT(getImpl<detail::SamplingAlgorithmRecord_Impl>());
}

SamplingAlgorithmRecord::SamplingAlgorithmRecord(
  std::shared_ptr<detail::SamplingAlgorithmRecord_Impl> impl,
    ProjectDatabase database)
  : DakotaAlgorithmRecord(impl, database, boost::none)
{
  OS_ASSERT(getImpl<detail::SamplingAlgorithmRecord_Impl>());
}

boost::optional<SamplingAlgorithmRecord> SamplingAlgorithmRecord::factoryFromQuery(
    const QSqlQuery& query, ProjectDatabase& database)
{
  OptionalSamplingAlgorithmRecord result;

  try {
    result = SamplingAlgorithmRecord(query,database);
  }

  catch (const std::exception& e) {
     LOG(Error,"Unable to construct SamplingAlgorithmRecord from query, because '"
         << e.what() << "'.");
  }

  return result;
}

std::vector<SamplingAlgorithmRecord> SamplingAlgorithmRecord::getSamplingAlgorithmRecords(ProjectDatabase& database) {
  std::vector<SamplingAlgorithmRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + AlgorithmRecord::databaseTableName() +
      " WHERE algorithmRecordType=:algorithmRecordType AND " +
      "dakotaAlgorithmRecordType=:dakotaAlgorithmRecordType"));
  query.bindValue(":dakotaAlgorithmRecordType", DakotaAlgorithmRecordType::SamplingAlgorithmRecord);
  assertExec(query);
  while (query.next()) {
    result.push_back(SamplingAlgorithmRecord(query, database));
  }

  return result;
}

boost::optional<SamplingAlgorithmRecord> SamplingAlgorithmRecord::getSamplingAlgorithmRecord(
  int id, ProjectDatabase& database) {
  boost::optional<SamplingAlgorithmRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + AlgorithmRecord::databaseTableName() +
      " WHERE algorithmRecordType=:algorithmRecordType AND " +
      "dakotaAlgorithmRecordType=:dakotaAlgorithmRecordType AND " +
      "id=:id"));
  query.bindValue(":dakotaAlgorithmRecordType", DakotaAlgorithmRecordType::SamplingAlgorithmRecord);
  query.bindValue(":id",id);
  assertExec(query);
  if (query.first()) {
    result = SamplingAlgorithmRecord(query, database);
  }

  return result;
}

analysis::SamplingAlgorithm SamplingAlgorithmRecord::samplingAlgorithm() const {
  return getImpl<detail::SamplingAlgorithmRecord_Impl>()->samplingAlgorithm();
}

/// @cond
SamplingAlgorithmRecord::SamplingAlgorithmRecord(std::shared_ptr<detail::SamplingAlgorithmRecord_Impl> impl)
  : DakotaAlgorithmRecord(impl)
{}
/// @endcond

} // project
} // openstudio

