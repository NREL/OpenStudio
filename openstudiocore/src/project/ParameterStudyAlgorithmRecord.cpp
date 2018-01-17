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

