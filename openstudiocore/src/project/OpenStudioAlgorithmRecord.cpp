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

