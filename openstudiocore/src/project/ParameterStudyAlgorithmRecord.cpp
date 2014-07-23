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

#include "ParameterStudyAlgorithmRecord.hpp"
#include "ParameterStudyAlgorithmRecord_Impl.hpp"
#include "AnalysisRecord.hpp"
#include "JoinRecord.hpp"
#include "FileReferenceRecord.hpp"

#include "../analysis/ParameterStudyAlgorithm.hpp"
#include "../analysis/ParameterStudyAlgorithm_Impl.hpp"
#include "../analysis/ParameterStudyAlgorithmOptions.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/core/Optional.hpp"
#include "../utilities/data/Attribute.hpp"

namespace openstudio {
namespace project {

namespace detail {

  ParameterStudyAlgorithmRecord_Impl::ParameterStudyAlgorithmRecord_Impl(
      const analysis::ParameterStudyAlgorithm& parameterStudyAlgorithm,
      AnalysisRecord& analysisRecord)
    : DakotaAlgorithmRecord_Impl(parameterStudyAlgorithm,
                                 DakotaAlgorithmRecordType::ParameterStudyAlgorithmRecord,
                                 analysisRecord),
      m_algorithmType(parameterStudyAlgorithm.parameterStudyAlgorithmOptions().algorithmType())
  {}

  ParameterStudyAlgorithmRecord_Impl::ParameterStudyAlgorithmRecord_Impl(const QSqlQuery& query,
                                                                         ProjectDatabase& database)
    : DakotaAlgorithmRecord_Impl(query, database)
  {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    QVariant value;

    value = query.value(ParameterStudyAlgorithmRecord::ColumnsType::parameterStudyAlgorithmType);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_algorithmType = analysis::ParameterStudyAlgorithmType(value.toInt());
  }

  std::vector<ObjectRecord> ParameterStudyAlgorithmRecord_Impl::resources() const {
    ObjectRecordVector result;
    return result;
  }

  std::vector<JoinRecord> ParameterStudyAlgorithmRecord_Impl::joinRecords() const {
    JoinRecordVector result;
    return result;
  }

  void ParameterStudyAlgorithmRecord_Impl::saveRow(const std::shared_ptr<QSqlDatabase> &database)
  {
    QSqlQuery query(*database);
    this->makeUpdateByIdQuery<ParameterStudyAlgorithmRecord>(query);
    this->bindValues(query);
    assertExec(query);
  }

  analysis::Algorithm ParameterStudyAlgorithmRecord_Impl::algorithm() const {
    return parameterStudyAlgorithm().cast<analysis::Algorithm>();
  }

  analysis::DakotaAlgorithm ParameterStudyAlgorithmRecord_Impl::dakotaAlgorithm() const {
    return parameterStudyAlgorithm().cast<analysis::DakotaAlgorithm>();
  }

  analysis::ParameterStudyAlgorithm ParameterStudyAlgorithmRecord_Impl::parameterStudyAlgorithm() const {
    analysis::ParameterStudyAlgorithmOptions opts(m_algorithmType, options());
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

    return analysis::ParameterStudyAlgorithm(handle(),
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

  void ParameterStudyAlgorithmRecord_Impl::bindValues(QSqlQuery& query) const {
    DakotaAlgorithmRecord_Impl::bindValues(query);

    query.bindValue(
        ParameterStudyAlgorithmRecord::ColumnsType::parameterStudyAlgorithmType,
        m_algorithmType.value());
  }

  void ParameterStudyAlgorithmRecord_Impl::setLastValues(const QSqlQuery& query,
                                                         ProjectDatabase& projectDatabase) {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    DakotaAlgorithmRecord_Impl::setLastValues(query,projectDatabase);

    QVariant value;

    value = query.value(ParameterStudyAlgorithmRecord::ColumnsType::parameterStudyAlgorithmType);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_lastAlgorithmType = analysis::ParameterStudyAlgorithmType(value.toInt());
  }

  bool ParameterStudyAlgorithmRecord_Impl::compareValues(const QSqlQuery& query) const {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    bool result = DakotaAlgorithmRecord_Impl::compareValues(query);

    QVariant value;

    value = query.value(ParameterStudyAlgorithmRecord::ColumnsType::parameterStudyAlgorithmType);
    OS_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_algorithmType == analysis::ParameterStudyAlgorithmType(value.toInt()));

    return result;
  }

  void ParameterStudyAlgorithmRecord_Impl::saveLastValues() {
    DakotaAlgorithmRecord_Impl::saveLastValues();

    // m_lastDATAMEMBERNAME = m_DATAMEMBERNAME;
  }

  void ParameterStudyAlgorithmRecord_Impl::revertToLastValues() {
    DakotaAlgorithmRecord_Impl::revertToLastValues();

    m_algorithmType = m_lastAlgorithmType;
  }

} // detail

ParameterStudyAlgorithmRecord::ParameterStudyAlgorithmRecord(
    const analysis::ParameterStudyAlgorithm& parameterStudyAlgorithm,
    AnalysisRecord& analysisRecord)
  : DakotaAlgorithmRecord(std::shared_ptr<detail::ParameterStudyAlgorithmRecord_Impl>(
        new detail::ParameterStudyAlgorithmRecord_Impl(parameterStudyAlgorithm, analysisRecord)),
        analysisRecord.projectDatabase(),
        parameterStudyAlgorithm)
{
  OS_ASSERT(getImpl<detail::ParameterStudyAlgorithmRecord_Impl>());

  constructRelatedRecords(parameterStudyAlgorithm);
}

ParameterStudyAlgorithmRecord::ParameterStudyAlgorithmRecord(const QSqlQuery& query,
                                                             ProjectDatabase& database)
  : DakotaAlgorithmRecord(std::shared_ptr<detail::ParameterStudyAlgorithmRecord_Impl>(
        new detail::ParameterStudyAlgorithmRecord_Impl(query, database)),
        database,
        boost::none)
{
  OS_ASSERT(getImpl<detail::ParameterStudyAlgorithmRecord_Impl>());
}

ParameterStudyAlgorithmRecord::ParameterStudyAlgorithmRecord(
    std::shared_ptr<detail::ParameterStudyAlgorithmRecord_Impl> impl,
    ProjectDatabase database)
  : DakotaAlgorithmRecord(impl, database, boost::none)
{
  OS_ASSERT(getImpl<detail::ParameterStudyAlgorithmRecord_Impl>());
}

boost::optional<ParameterStudyAlgorithmRecord> ParameterStudyAlgorithmRecord::factoryFromQuery(
    const QSqlQuery& query, ProjectDatabase& database)
{
  OptionalParameterStudyAlgorithmRecord result;

  try {
    result = ParameterStudyAlgorithmRecord(query,database);
  }

  catch (const std::exception& e) {
     LOG(Error,"Unable to construct ParameterStudyAlgorithmRecord from query, because '"
         << e.what() << "'.");
  }

  return result;
}

std::vector<ParameterStudyAlgorithmRecord> ParameterStudyAlgorithmRecord::getParameterStudyAlgorithmRecords(
    ProjectDatabase& database) {
  std::vector<ParameterStudyAlgorithmRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + AlgorithmRecord::databaseTableName() +
      " WHERE algorithmRecordType=:algorithmRecordType AND " +
      "dakotaAlgorithmRecordType=:dakotaAlgorithmRecordType"));
  query.bindValue(":dakotaAlgorithmRecordType", DakotaAlgorithmRecordType::ParameterStudyAlgorithmRecord);
  assertExec(query);
  while (query.next()) {
    result.push_back(ParameterStudyAlgorithmRecord(query, database));
  }

  return result;
}

boost::optional<ParameterStudyAlgorithmRecord> ParameterStudyAlgorithmRecord::getParameterStudyAlgorithmRecord(
    int id, ProjectDatabase& database) {
  boost::optional<ParameterStudyAlgorithmRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + AlgorithmRecord::databaseTableName() +
      " WHERE algorithmRecordType=:algorithmRecordType AND " +
      "dakotaAlgorithmRecordType=:dakotaAlgorithmRecordType AND " +
      "id=:id"));
  query.bindValue(":dakotaAlgorithmRecordType", DakotaAlgorithmRecordType::ParameterStudyAlgorithmRecord);
  query.bindValue(":id",id);
  assertExec(query);
  if (query.first()) {
    result = ParameterStudyAlgorithmRecord(query, database);
  }

  return result;
}

analysis::ParameterStudyAlgorithm ParameterStudyAlgorithmRecord::parameterStudyAlgorithm() const {
  return getImpl<detail::ParameterStudyAlgorithmRecord_Impl>()->parameterStudyAlgorithm();
}

/// @cond
ParameterStudyAlgorithmRecord::ParameterStudyAlgorithmRecord(std::shared_ptr<detail::ParameterStudyAlgorithmRecord_Impl> impl)
  : DakotaAlgorithmRecord(impl)
{}
/// @endcond

} // project
} // openstudio

