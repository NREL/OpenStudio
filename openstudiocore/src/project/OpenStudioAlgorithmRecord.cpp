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

#include "OpenStudioAlgorithmRecord.hpp"
#include "OpenStudioAlgorithmRecord_Impl.hpp"
#include "ProjectDatabase.hpp"
#include "AnalysisRecord.hpp"
#include "DesignOfExperimentsRecord.hpp"
#include "SequentialSearchRecord.hpp"

#include "../analysis/DesignOfExperiments.hpp"
#include "../analysis/DesignOfExperiments_Impl.hpp"
#include "../analysis/SequentialSearch.hpp"
#include "../analysis/SequentialSearch_Impl.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace project {

namespace detail {

  OpenStudioAlgorithmRecord_Impl::OpenStudioAlgorithmRecord_Impl(
      const analysis::OpenStudioAlgorithm& openStudioAlgorithm, 
      const OpenStudioAlgorithmRecordType& openStudioAlgorithmRecordType,
      AnalysisRecord& analysisRecord)
    : AlgorithmRecord_Impl(openStudioAlgorithm, 
                           AlgorithmRecordType::OpenStudioAlgorithmRecord,
                           analysisRecord),
      m_openStudioAlgorithmRecordType(openStudioAlgorithmRecordType)
  {}

  OpenStudioAlgorithmRecord_Impl::OpenStudioAlgorithmRecord_Impl(const QSqlQuery& query, 
                                                                 ProjectDatabase& database)
    : AlgorithmRecord_Impl(query, database)
  {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    QVariant value;

    value = query.value(OpenStudioAlgorithmRecord::ColumnsType::openStudioAlgorithmRecordType);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_openStudioAlgorithmRecordType = OpenStudioAlgorithmRecordType(value.toInt());
  }

  void OpenStudioAlgorithmRecord_Impl::saveRow(const std::shared_ptr<QSqlDatabase> &database)
  {
    QSqlQuery query(*database);
    this->makeUpdateByIdQuery<OpenStudioAlgorithmRecord>(query);
    this->bindValues(query);
    assertExec(query);
  }

  void OpenStudioAlgorithmRecord_Impl::bindValues(QSqlQuery& query) const {
    AlgorithmRecord_Impl::bindValues(query);

    query.bindValue(AlgorithmRecordColumns::openStudioAlgorithmRecordType,
                    m_openStudioAlgorithmRecordType.value());
  }

  void OpenStudioAlgorithmRecord_Impl::setLastValues(const QSqlQuery& query, ProjectDatabase& projectDatabase) {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    AlgorithmRecord_Impl::setLastValues(query,projectDatabase);

    QVariant value;

    value = query.value(OpenStudioAlgorithmRecord::ColumnsType::openStudioAlgorithmRecordType);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_lastOpenStudioAlgorithmRecordType = OpenStudioAlgorithmRecordType(value.toInt());
  }

  bool OpenStudioAlgorithmRecord_Impl::compareValues(const QSqlQuery& query) const {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    bool result = AlgorithmRecord_Impl::compareValues(query);

    QVariant value;

    value = query.value(OpenStudioAlgorithmRecord::ColumnsType::openStudioAlgorithmRecordType);
    OS_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_openStudioAlgorithmRecordType == OpenStudioAlgorithmRecordType(value.toInt()));

    return result;
  }

  void OpenStudioAlgorithmRecord_Impl::saveLastValues() {
    AlgorithmRecord_Impl::saveLastValues();

    m_lastOpenStudioAlgorithmRecordType = m_openStudioAlgorithmRecordType;
  }

  void OpenStudioAlgorithmRecord_Impl::revertToLastValues() {
    AlgorithmRecord_Impl::revertToLastValues();

    m_openStudioAlgorithmRecordType = m_lastOpenStudioAlgorithmRecordType;
  }

} // detail

OpenStudioAlgorithmRecord::OpenStudioAlgorithmRecord(std::shared_ptr<detail::OpenStudioAlgorithmRecord_Impl> impl,
                                                     ProjectDatabase database,
                                                     const boost::optional<analysis::OpenStudioAlgorithm>& algorithm)
  : AlgorithmRecord(impl, database, analysis::OptionalAlgorithm(algorithm))
{
  OS_ASSERT(getImpl<detail::OpenStudioAlgorithmRecord_Impl>());
}

boost::optional<OpenStudioAlgorithmRecord> OpenStudioAlgorithmRecord::factoryFromQuery(
    const QSqlQuery& query, ProjectDatabase& database)
{
  OptionalOpenStudioAlgorithmRecord result;

  QVariant value = query.value(AlgorithmRecordColumns::openStudioAlgorithmRecordType);
  if (!value.isValid() || value.isNull()) {
    return boost::none;
  }
  int openStudioAlgorithmRecordType = value.toInt();

  switch (openStudioAlgorithmRecordType) {
    case OpenStudioAlgorithmRecordType::DesignOfExperimentsRecord : 
      result = DesignOfExperimentsRecord(query, database).cast<OpenStudioAlgorithmRecord>();
     break;
    case OpenStudioAlgorithmRecordType::SequentialSearchRecord :
      result = SequentialSearchRecord(query, database).cast<OpenStudioAlgorithmRecord>();
     break;
    default :
      LOG(Error,"Unknown OpenStudioAlgorithmRecordType " << openStudioAlgorithmRecordType);
      return boost::none;
  }

  return result;
}

OpenStudioAlgorithmRecord OpenStudioAlgorithmRecord::factoryFromOpenStudioAlgorithm(
    const analysis::OpenStudioAlgorithm& openStudioAlgorithm, AnalysisRecord& analysisRecord)
{
  if (openStudioAlgorithm.optionalCast<analysis::DesignOfExperiments>()) {
    return DesignOfExperimentsRecord(openStudioAlgorithm.cast<analysis::DesignOfExperiments>(),
                                     analysisRecord);
  }
  else if (openStudioAlgorithm.optionalCast<analysis::SequentialSearch>()) {
    return SequentialSearchRecord(openStudioAlgorithm.cast<analysis::SequentialSearch>(),
                                  analysisRecord);
  }

  OS_ASSERT(false);
  return OpenStudioAlgorithmRecord(std::shared_ptr<detail::OpenStudioAlgorithmRecord_Impl>());
}

std::vector<OpenStudioAlgorithmRecord> OpenStudioAlgorithmRecord::getOpenStudioAlgorithmRecords(
    ProjectDatabase& database) 
{
  std::vector<OpenStudioAlgorithmRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + AlgorithmRecord::databaseTableName() + " WHERE algorithmRecordType=:algorithmRecordType"));
  query.bindValue(":algorithmRecordType", AlgorithmRecordType::OpenStudioAlgorithmRecord);
  assertExec(query);
  while (query.next()) {
    OptionalOpenStudioAlgorithmRecord openStudioAlgorithmRecord = OpenStudioAlgorithmRecord::factoryFromQuery(query, database);
    if (openStudioAlgorithmRecord) {
      result.push_back(*openStudioAlgorithmRecord);
    }
  }

  return result;
}

boost::optional<OpenStudioAlgorithmRecord> OpenStudioAlgorithmRecord::getOpenStudioAlgorithmRecord(int id, ProjectDatabase& database) {
  boost::optional<OpenStudioAlgorithmRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + AlgorithmRecord::databaseTableName() + " WHERE algorithmRecordType=:algorithmRecordType AND id=:id"));
  query.bindValue(":algorithmRecordType", AlgorithmRecordType::OpenStudioAlgorithmRecord);
  query.bindValue(":id",id);
  assertExec(query);
  if (query.first()) {
    result = OpenStudioAlgorithmRecord::factoryFromQuery(query, database);
  }

  return result;
}

analysis::OpenStudioAlgorithm OpenStudioAlgorithmRecord::openStudioAlgorithm() const {
  return getImpl<detail::OpenStudioAlgorithmRecord_Impl>()->openStudioAlgorithm();
}

/// @cond
OpenStudioAlgorithmRecord::OpenStudioAlgorithmRecord(std::shared_ptr<detail::OpenStudioAlgorithmRecord_Impl> impl)
  : AlgorithmRecord(impl)
{}
/// @endcond

} // project
} // openstudio

