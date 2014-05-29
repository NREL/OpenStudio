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

