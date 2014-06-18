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

#include "AlgorithmRecord.hpp"
#include "AlgorithmRecord_Impl.hpp"

#include "ProjectDatabase.hpp"
#include "AnalysisRecord.hpp"
#include "OpenStudioAlgorithmRecord.hpp"
#include "DakotaAlgorithmRecord.hpp"
#include "AttributeRecord.hpp"
#include "AttributeRecord_Impl.hpp"

#include "../analysis/Algorithm.hpp"
#include "../analysis/OpenStudioAlgorithm.hpp"
#include "../analysis/OpenStudioAlgorithm_Impl.hpp"
#include "../analysis/DakotaAlgorithm.hpp"
#include "../analysis/DakotaAlgorithm_Impl.hpp"

#include "../utilities/data/Attribute.hpp"
#include "../utilities/core/Assert.hpp"
#include "../utilities/core/Compare.hpp"

namespace openstudio {
namespace project {

namespace detail {

  AlgorithmRecord_Impl::AlgorithmRecord_Impl(const analysis::Algorithm& algorithm,
                                             const AlgorithmRecordType& algorithmRecordType,
                                             AnalysisRecord& analysisRecord)
    : ObjectRecord_Impl(analysisRecord.projectDatabase(),
                        algorithm.uuid(),
                        algorithm.name(),
                        algorithm.displayName(),
                        algorithm.description(),
                        algorithm.versionUUID()),
      m_algorithmRecordType(algorithmRecordType),
      m_complete(algorithm.isComplete()),
      m_failed(algorithm.failed()),
      m_iter(algorithm.iter())
  {}

  AlgorithmRecord_Impl::AlgorithmRecord_Impl(const QSqlQuery& query, ProjectDatabase& database)
    : ObjectRecord_Impl(database, query)
  {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    QVariant value;

    value = query.value(AlgorithmRecordColumns::algorithmRecordType);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_algorithmRecordType = AlgorithmRecordType(value.toInt());

    value = query.value(AlgorithmRecordColumns::complete);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_complete = value.toBool();

    value = query.value(AlgorithmRecordColumns::failed);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_failed = value.toBool();

    value = query.value(AlgorithmRecordColumns::iter);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_iter = value.toInt();

  }

  std::string AlgorithmRecord_Impl::databaseTableName() const {
    return AlgorithmRecord::databaseTableName();
  }

  boost::optional<ObjectRecord> AlgorithmRecord_Impl::parent() const {
    return analysisRecord();
  }

  std::vector<ObjectRecord> AlgorithmRecord_Impl::children() const {
    ObjectRecordVector result;

    ProjectDatabase database = projectDatabase();
    QSqlQuery query(*(database.qSqlDatabase()));
    query.prepare(toQString("SELECT * FROM " + AttributeRecord::databaseTableName() + " WHERE algorithmRecordId=:algorithmRecordId"));
    query.bindValue(":algorithmRecordId",id());
    assertExec(query);
    while (query.next()) {
      result.push_back(database.getFromQuery<AttributeRecord>(query));
    }

    return result;
  }

  AnalysisRecord AlgorithmRecord_Impl::analysisRecord() const {
    AnalysisRecordVector candidates;

    ProjectDatabase database = projectDatabase();
    QSqlQuery query(*(database.qSqlDatabase()));
    query.prepare(toQString("SELECT * FROM " + AnalysisRecord::databaseTableName() + " WHERE algorithmRecordId=:algorithmRecordId"));
    query.bindValue(":algorithmRecordId",id());
    assertExec(query);
    while (query.next()) {
      candidates.push_back(AnalysisRecord(query,database));
    }

    OS_ASSERT(candidates.size() == 1u);

    return candidates[0];
  }

  bool AlgorithmRecord_Impl::complete() const {
    return m_complete;
  }

  bool AlgorithmRecord_Impl::failed() const {
    return m_failed;
  }

  int AlgorithmRecord_Impl::iter() const {
    return m_iter;
  }

  std::vector<Attribute> AlgorithmRecord_Impl::options() const {
    AttributeVector result;
    for (const ObjectRecord& child : children()) {
      if (OptionalAttributeRecord attributeRecord = child.optionalCast<AttributeRecord>()) {
        result.push_back(attributeRecord->attribute());
      }
    }
    return result;
  }

  void AlgorithmRecord_Impl::reset() {
    m_complete = false;
    m_failed = false;
    m_iter = -1;
    onChange();
  }

  void AlgorithmRecord_Impl::bindValues(QSqlQuery& query) const {
    ObjectRecord_Impl::bindValues(query);

    query.bindValue(AlgorithmRecordColumns::algorithmRecordType, m_algorithmRecordType.value());
    query.bindValue(AlgorithmRecordColumns::complete,m_complete);
    query.bindValue(AlgorithmRecordColumns::failed,m_failed);
    query.bindValue(AlgorithmRecordColumns::iter,m_iter);
  }

  void AlgorithmRecord_Impl::setLastValues(const QSqlQuery& query, ProjectDatabase& projectDatabase) {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    ObjectRecord_Impl::setLastValues(query,projectDatabase);

    QVariant value;

    value = query.value(AlgorithmRecordColumns::algorithmRecordType);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_lastAlgorithmRecordType = AlgorithmRecordType(value.toInt());

    value = query.value(AlgorithmRecordColumns::complete);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_lastComplete = value.toBool();

    value = query.value(AlgorithmRecordColumns::failed);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_lastFailed = value.toBool();

    value = query.value(AlgorithmRecordColumns::iter);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_lastIter = value.toInt();
  }

  bool AlgorithmRecord_Impl::compareValues(const QSqlQuery& query) const {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    bool result = ObjectRecord_Impl::compareValues(query);

    QVariant value;

    value = query.value(AlgorithmRecordColumns::algorithmRecordType);
    OS_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_algorithmRecordType == AlgorithmRecordType(value.toInt()));

    value = query.value(AlgorithmRecordColumns::complete);
    OS_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_complete == value.toBool());

    value = query.value(AlgorithmRecordColumns::failed);
    OS_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_failed == value.toBool());

    value = query.value(AlgorithmRecordColumns::iter);
    OS_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_iter == value.toInt());

    return result;
  }

  void AlgorithmRecord_Impl::saveLastValues() {
    ObjectRecord_Impl::saveLastValues();

    m_lastAlgorithmRecordType = m_algorithmRecordType;
    m_lastComplete = m_complete;
    m_lastFailed = m_failed;
    m_lastIter = m_iter;
  }

  void AlgorithmRecord_Impl::revertToLastValues() {
    ObjectRecord_Impl::revertToLastValues();

    m_algorithmRecordType = m_lastAlgorithmRecordType;
    m_complete = m_lastComplete;
    m_failed = m_lastFailed;
    m_iter = m_lastIter;
  }

} // detail

AlgorithmRecord::AlgorithmRecord(std::shared_ptr<detail::AlgorithmRecord_Impl> impl,
                                 ProjectDatabase database,
                                 const boost::optional<analysis::Algorithm>& algorithm)
: ObjectRecord(impl, database)
{
  OS_ASSERT(getImpl<detail::AlgorithmRecord_Impl>());
  if (algorithm) {
    constructRelatedRecords(*algorithm);
  }
}

void AlgorithmRecord::constructRelatedRecords(const analysis::Algorithm& algorithm) {
  AttributeVector options = algorithm.options().options();
  AttributeVector dbOptions = getImpl<detail::AlgorithmRecord_Impl>()->options();
  AlgorithmRecord copyOfThis(getImpl<detail::AlgorithmRecord_Impl>());
  for (const Attribute& option : options) {
    // find in dbOptions
    auto dbIt = std::find_if(dbOptions.begin(),dbOptions.end(),
                             std::bind(uuidsEqual<Attribute,Attribute>,std::placeholders::_1,option));
    // if not there, or if different versionUUID, save it
    if ((dbIt == dbOptions.end()) || (option.versionUUID() != dbIt->versionUUID())) {
      AttributeRecord algOptionRecord(option,copyOfThis);
    }

    // if there, erase from dbOptions
    if (dbIt != dbOptions.end()) {
      dbOptions.erase(dbIt);
    }
  }

  // any attributes left in dbOptions should be removed from the database
  ProjectDatabase database = projectDatabase();
  for (const Attribute& toRemove : dbOptions) {
    AttributeRecord dbOptionRecord = database.getObjectRecordByHandle<AttributeRecord>(toRemove.uuid()).get();
    database.removeRecord(dbOptionRecord);
  }
}

std::string AlgorithmRecord::databaseTableName() {
  return "AlgorithmRecords";
}

UpdateByIdQueryData AlgorithmRecord::updateByIdQueryData() {
  static UpdateByIdQueryData result;
  if (result.queryString.empty()) {
    // numeric column identifiers
    result.columnValues = ColumnsType::getValues();

    // query string
    std::stringstream ss;
    ss << "UPDATE " << databaseTableName() << " SET ";
    int expectedValue = 0;
    for (auto it = result.columnValues.begin(),
         itend = result.columnValues.end(); it != itend; ++it)
    {
      // require 0 based columns, don't skip any
      OS_ASSERT(*it == expectedValue);
      // column name is name, type is description
      ss << ColumnsType::valueName(*it) << "=:" << ColumnsType::valueName(*it);
      // is this the last column?
      auto nextIt = it;
      ++nextIt;
      if (nextIt == itend) {
        ss << " ";
      }
      else {
        ss << ", ";
      }
      ++expectedValue;
    }
    ss << "WHERE id=:id";
    result.queryString = ss.str();

    // null values
    for (const auto & columnValue : result.columnValues)
    {
      // bind all values to avoid parameter mismatch error
      if (istringEqual(ColumnsType::valueDescription(columnValue), "INTEGER")) {
        result.nulls.push_back(QVariant(QVariant::Int));
      }
      else {
        result.nulls.push_back(QVariant(QVariant::String));
      }
    }
  }
  return result;
}

void AlgorithmRecord::updatePathData(ProjectDatabase database,
                                     const openstudio::path& originalBase,
                                     const openstudio::path& newBase)
{
  // at this time, there are no AlgorithmRecords with path data.
}

boost::optional<AlgorithmRecord> AlgorithmRecord::factoryFromQuery(const QSqlQuery& query,
                                                                   ProjectDatabase& database)
{
  OptionalAlgorithmRecord result;

  int algorithmRecordType = query.value(AlgorithmRecordColumns::algorithmRecordType).toInt();

  switch (algorithmRecordType) {
    case AlgorithmRecordType::OpenStudioAlgorithmRecord :
      {
        OptionalOpenStudioAlgorithmRecord candidate = OpenStudioAlgorithmRecord::factoryFromQuery(query,database);
        if (candidate) {
          result = candidate->cast<AlgorithmRecord>();
        }
      }
     break;
    case AlgorithmRecordType::DakotaAlgorithmRecord :
      {
        OptionalDakotaAlgorithmRecord candidate = DakotaAlgorithmRecord::factoryFromQuery(query,database);
        if (candidate) {
          result = candidate->cast<AlgorithmRecord>();
        }
      }
     break;
    default :
      LOG(Error,"Unknown AlgorithmRecordType " << algorithmRecordType);
      return boost::none;
  }

  return result;
}

AlgorithmRecord AlgorithmRecord::factoryFromAlgorithm(const analysis::Algorithm& algorithm,
                                                      AnalysisRecord& analysisRecord)
{
  OptionalAlgorithmRecord result;
  if (algorithm.optionalCast<analysis::OpenStudioAlgorithm>()) {
    result = OpenStudioAlgorithmRecord::factoryFromOpenStudioAlgorithm(
        algorithm.cast<analysis::OpenStudioAlgorithm>(),
        analysisRecord);
  }
  else if (algorithm.optionalCast<analysis::DakotaAlgorithm>()) {
    result = DakotaAlgorithmRecord::factoryFromDakotaAlgorithm(
        algorithm.cast<analysis::DakotaAlgorithm>(),
        analysisRecord);
  }

  OS_ASSERT(result);
  return *result;
}

std::vector<AlgorithmRecord> AlgorithmRecord::getAlgorithmRecords(ProjectDatabase& database) {
  std::vector<AlgorithmRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + AlgorithmRecord::databaseTableName()));
  assertExec(query);
  while (query.next()) {
    OptionalAlgorithmRecord algorithmRecord = AlgorithmRecord::factoryFromQuery(query, database);
    if (algorithmRecord) {
      result.push_back(*algorithmRecord);
    }
  }

  return result;
}

boost::optional<AlgorithmRecord> AlgorithmRecord::getAlgorithmRecord(int id, ProjectDatabase& database) {
  boost::optional<AlgorithmRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + AlgorithmRecord::databaseTableName() + " WHERE id=:id"));
  query.bindValue(":id",id);
  assertExec(query);
  if (query.first()) {
    result = AlgorithmRecord::factoryFromQuery(query, database);
  }

  return result;
}

AnalysisRecord AlgorithmRecord::analysisRecord() const {
  return getImpl<detail::AlgorithmRecord_Impl>()->analysisRecord();
}

bool AlgorithmRecord::isComplete() const {
  return getImpl<detail::AlgorithmRecord_Impl>()->complete();
}

analysis::Algorithm AlgorithmRecord::algorithm() const {
  return getImpl<detail::AlgorithmRecord_Impl>()->algorithm();
}

void AlgorithmRecord::reset() {
  getImpl<detail::AlgorithmRecord_Impl>()->reset();
}

/// @cond
AlgorithmRecord::AlgorithmRecord(std::shared_ptr<detail::AlgorithmRecord_Impl> impl)
  : ObjectRecord(impl)
{}
/// @endcond

} // project
} // openstudio
