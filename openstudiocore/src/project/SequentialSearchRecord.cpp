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

#include "SequentialSearchRecord.hpp"
#include "SequentialSearchRecord_Impl.hpp"
#include "AnalysisRecord.hpp"

#include "../analysis/SequentialSearch.hpp"
#include "../analysis/SequentialSearch_Impl.hpp"
#include "../analysis/SequentialSearchOptions.hpp"

#include "JoinRecord.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/core/Optional.hpp"
#include "../utilities/data/Attribute.hpp"

namespace openstudio {
namespace project {

namespace detail {

  SequentialSearchRecord_Impl::SequentialSearchRecord_Impl(
      const analysis::SequentialSearch& sequentialSearch, 
      AnalysisRecord& analysisRecord)
    : OpenStudioAlgorithmRecord_Impl(sequentialSearch, 
                                     OpenStudioAlgorithmRecordType::SequentialSearchRecord, 
                                     analysisRecord)
  {}

  SequentialSearchRecord_Impl::SequentialSearchRecord_Impl(const QSqlQuery& query, ProjectDatabase& database)
    : OpenStudioAlgorithmRecord_Impl(query, database)
  {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());
  }

  std::vector<ObjectRecord> SequentialSearchRecord_Impl::resources() const {
    ObjectRecordVector result;
    return result;
  }

  std::vector<JoinRecord> SequentialSearchRecord_Impl::joinRecords() const {
    JoinRecordVector result;
    return result;
  }

  void SequentialSearchRecord_Impl::saveRow(const std::shared_ptr<QSqlDatabase> &database)
  {
    QSqlQuery query(*database);
    this->makeUpdateByIdQuery<SequentialSearchRecord>(query);
    this->bindValues(query);
    assertExec(query);
  }

  analysis::Algorithm SequentialSearchRecord_Impl::algorithm() const {
    return sequentialSearch().cast<analysis::Algorithm>();
  }

  analysis::OpenStudioAlgorithm SequentialSearchRecord_Impl::openStudioAlgorithm() const {
    return sequentialSearch().cast<analysis::OpenStudioAlgorithm>();
  }

  analysis::SequentialSearch SequentialSearchRecord_Impl::sequentialSearch() const {
    analysis::SequentialSearchOptions opts(options());
    return analysis::SequentialSearch(handle(),
                                      uuidLast(),
                                      displayName(),
                                      description(),
                                      complete(),
                                      failed(),
                                      iter(),
                                      opts);
  }

  void SequentialSearchRecord_Impl::bindValues(QSqlQuery& query) const {
    OpenStudioAlgorithmRecord_Impl::bindValues(query);
  }

  void SequentialSearchRecord_Impl::setLastValues(const QSqlQuery& query, ProjectDatabase& projectDatabase) {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    OpenStudioAlgorithmRecord_Impl::setLastValues(query,projectDatabase);
  }

  bool SequentialSearchRecord_Impl::compareValues(const QSqlQuery& query) const {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    bool result = OpenStudioAlgorithmRecord_Impl::compareValues(query);

    return result;
  }

  void SequentialSearchRecord_Impl::saveLastValues() {
    OpenStudioAlgorithmRecord_Impl::saveLastValues();
  }

  void SequentialSearchRecord_Impl::revertToLastValues() {
    OpenStudioAlgorithmRecord_Impl::revertToLastValues();
  }

} // detail

SequentialSearchRecord::SequentialSearchRecord(const analysis::SequentialSearch& sequentialSearch, 
                                               AnalysisRecord& analysisRecord)
  : OpenStudioAlgorithmRecord(std::shared_ptr<detail::SequentialSearchRecord_Impl>(
        new detail::SequentialSearchRecord_Impl(sequentialSearch, analysisRecord)),
        analysisRecord.projectDatabase(),
        sequentialSearch)
{
  OS_ASSERT(getImpl<detail::SequentialSearchRecord_Impl>());
}

SequentialSearchRecord::SequentialSearchRecord(const QSqlQuery& query, ProjectDatabase& database)
  : OpenStudioAlgorithmRecord(std::shared_ptr<detail::SequentialSearchRecord_Impl>(
        new detail::SequentialSearchRecord_Impl(query, database)),
        database,
        boost::optional<analysis::OpenStudioAlgorithm>())
{
  OS_ASSERT(getImpl<detail::SequentialSearchRecord_Impl>());
}

SequentialSearchRecord::SequentialSearchRecord(
    std::shared_ptr<detail::SequentialSearchRecord_Impl> impl,
    ProjectDatabase database)
    : OpenStudioAlgorithmRecord(impl, database, boost::optional<analysis::OpenStudioAlgorithm>())
{
  OS_ASSERT(getImpl<detail::SequentialSearchRecord_Impl>());
}

boost::optional<SequentialSearchRecord> SequentialSearchRecord::factoryFromQuery(
    const QSqlQuery& query, ProjectDatabase& database)
{
  OptionalSequentialSearchRecord result;
  try {
    result = SequentialSearchRecord(query,database);
  }
  catch (const std::exception& e) {
    LOG(Error,"Unable to construct SequentialSearchRecord from query, because '" 
        << e.what() << "'.");
  }
  return result;
}

std::vector<SequentialSearchRecord> SequentialSearchRecord::getSequentialSearchRecords(
    ProjectDatabase& database) 
{
  std::vector<SequentialSearchRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + AlgorithmRecord::databaseTableName() + 
                          " WHERE algorithmRecordType=:algorithmRecordType AND " +
                          "openStudioAlgorithmRecordType=:openStudioAlgorithmRecordType"));
  query.bindValue(":algorithmRecordType", AlgorithmRecordType::OpenStudioAlgorithmRecord);
  query.bindValue(":openStudioAlgorithmRecordType", OpenStudioAlgorithmRecordType::SequentialSearchRecord);
  assertExec(query);
  while (query.next()) {
    result.push_back(SequentialSearchRecord(query, database));
  }

  return result;
}

boost::optional<SequentialSearchRecord> SequentialSearchRecord::getSequentialSearchRecord(
    int id, ProjectDatabase& database) 
{
  boost::optional<SequentialSearchRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + AlgorithmRecord::databaseTableName() + 
      " WHERE algorithmRecordType=:algorithmRecordType AND " +
      "openStudioAlgorithmRecordType=:openStudioAlgorithmRecordType AND " +
      "id=:id"));
  query.bindValue(":algorithmRecordType", AlgorithmRecordType::OpenStudioAlgorithmRecord);
  query.bindValue(":openStudioAlgorithmRecordType", OpenStudioAlgorithmRecordType::SequentialSearchRecord);
  query.bindValue(":id",id);
  assertExec(query);
  if (query.first()) {
    result = SequentialSearchRecord(query, database);
  }

  return result;
}

analysis::SequentialSearch SequentialSearchRecord::sequentialSearch() const {
  return getImpl<detail::SequentialSearchRecord_Impl>()->sequentialSearch();
}

/// @cond
SequentialSearchRecord::SequentialSearchRecord(std::shared_ptr<detail::SequentialSearchRecord_Impl> impl)
  : OpenStudioAlgorithmRecord(impl)
{}
/// @endcond

} // project
} // openstudio

