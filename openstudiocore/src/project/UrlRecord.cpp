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

#include "UrlRecord.hpp"
#include "UrlRecord_Impl.hpp"

#include "CloudSessionRecord.hpp"
#include "CloudSettingsRecord.hpp"
#include "JoinRecord.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace project {

namespace detail {

  UrlRecord_Impl::UrlRecord_Impl(const openstudio::Url& url,
                                 ObjectRecord& parentRecord)
    : ObjectRecord_Impl(parentRecord.projectDatabase(),
                        url.toString().toStdString(),"",""),
      m_parentDatabaseTableName(parentRecord.databaseTableName()),
      m_parentRecordId(parentRecord.id()),
      m_url(url)
  {
  }

  UrlRecord_Impl::UrlRecord_Impl(const QSqlQuery& query, ProjectDatabase& database)
    : ObjectRecord_Impl(database, query)
  {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    QVariant value;

    value = query.value(UrlRecord::ColumnsType::parentDatabaseTableName);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_parentDatabaseTableName = value.toString().toStdString();

    value = query.value(UrlRecord::ColumnsType::parentRecordId);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_parentRecordId = value.toInt();

    value = query.value(UrlRecord::ColumnsType::url);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_url = openstudio::Url(value.toString());
  }

  std::string UrlRecord_Impl::databaseTableName() const {
    return UrlRecord::databaseTableName();
  }

  boost::optional<ObjectRecord> UrlRecord_Impl::parent() const {
    OptionalObjectRecord result;
    ProjectDatabase database = projectDatabase();
    if (m_parentDatabaseTableName == CloudSessionRecord::databaseTableName()) {
      result = CloudSessionRecord::getCloudSessionRecord(m_parentRecordId,database);
    }
    else if (m_parentDatabaseTableName == CloudSettingsRecord::databaseTableName()) {
      result = CloudSettingsRecord::getCloudSettingsRecord(m_parentRecordId,database);
    }
    else {
      OS_ASSERT(false);
    }
    return result;
  }

  std::vector<ObjectRecord> UrlRecord_Impl::children() const {
    ObjectRecordVector result;
    return result;
  }

  std::vector<ObjectRecord> UrlRecord_Impl::resources() const {
    ObjectRecordVector result;
    return result;
  }

  std::vector<JoinRecord> UrlRecord_Impl::joinRecords() const {
    JoinRecordVector result;
    return result;
  }

  void UrlRecord_Impl::saveRow(const std::shared_ptr<QSqlDatabase> &database) {
    QSqlQuery query(*database);
    this->makeUpdateByIdQuery<UrlRecord>(query);
    this->bindValues(query);
    assertExec(query);
  }

  openstudio::Url UrlRecord_Impl::url() const {
    return m_url;
  }

  void UrlRecord_Impl::bindValues(QSqlQuery& query) const {
    ObjectRecord_Impl::bindValues(query);

    query.bindValue(UrlRecord::ColumnsType::parentDatabaseTableName,toQString(m_parentDatabaseTableName));
    query.bindValue(UrlRecord::ColumnsType::parentRecordId,m_parentRecordId);
    query.bindValue(UrlRecord::ColumnsType::url,m_url.isEmpty() ? QString("") : m_url.toString());
  }

  void UrlRecord_Impl::setLastValues(const QSqlQuery& query, ProjectDatabase& projectDatabase) {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    ObjectRecord_Impl::setLastValues(query,projectDatabase);

    QVariant value;

    value = query.value(UrlRecord::ColumnsType::parentDatabaseTableName);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_lastParentDatabaseTableName = value.toString().toStdString();

    value = query.value(UrlRecord::ColumnsType::parentRecordId);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_lastParentRecordId = value.toInt();

    value = query.value(UrlRecord::ColumnsType::url);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_lastUrl = openstudio::Url(value.toString());
  }

  bool UrlRecord_Impl::compareValues(const QSqlQuery& query) const {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    bool result = ObjectRecord_Impl::compareValues(query);

    QVariant value;

    value = query.value(UrlRecord::ColumnsType::parentDatabaseTableName);
    OS_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_parentDatabaseTableName == value.toString().toStdString());

    value = query.value(UrlRecord::ColumnsType::parentRecordId);
    OS_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_parentRecordId == value.toInt());

    value = query.value(UrlRecord::ColumnsType::url);
    OS_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_url == openstudio::Url(value.toString()));

    return result;
  }

  void UrlRecord_Impl::saveLastValues() {
    ObjectRecord_Impl::saveLastValues();

    m_lastParentDatabaseTableName = m_parentDatabaseTableName;
    m_lastParentRecordId = m_parentRecordId;
    m_lastUrl = m_url;
  }

  void UrlRecord_Impl::revertToLastValues() {
    ObjectRecord_Impl::revertToLastValues();

    m_parentDatabaseTableName = m_lastParentDatabaseTableName;
    m_parentRecordId = m_lastParentRecordId;
    m_url = m_lastUrl;
  }

} // detail

UrlRecord::UrlRecord(const openstudio::Url& url, ObjectRecord& parentRecord)
  : ObjectRecord(std::shared_ptr<detail::UrlRecord_Impl>(
        new detail::UrlRecord_Impl(url, parentRecord)),
        parentRecord.projectDatabase())
{
  OS_ASSERT(getImpl<detail::UrlRecord_Impl>());
}

UrlRecord::UrlRecord(const QSqlQuery& query, ProjectDatabase& database)
  : ObjectRecord(std::shared_ptr<detail::UrlRecord_Impl>(
        new detail::UrlRecord_Impl(query, database)),
        database)
{
  OS_ASSERT(getImpl<detail::UrlRecord_Impl>());
}

std::string UrlRecord::databaseTableName() {
  return "UrlRecords";
}

UpdateByIdQueryData UrlRecord::updateByIdQueryData() {
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

void UrlRecord::updatePathData(ProjectDatabase database,
                               const openstudio::path& originalBase,
                               const openstudio::path& newBase)
{
  // the urls stored in this table are not relative to the .osp location
}

boost::optional<UrlRecord> UrlRecord::factoryFromQuery(const QSqlQuery& query, ProjectDatabase& database)
{
  OptionalUrlRecord result;

  try {
    result = UrlRecord(query,database);
  }
  catch (const std::exception& e) {
    LOG(Error,"Unable to construct UrlRecord from query, because '"
        << e.what() << "'.");
  }

  return result;
}

std::vector<UrlRecord> UrlRecord::getUrlRecords(ProjectDatabase& database) {
  std::vector<UrlRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + UrlRecord::databaseTableName()));
  assertExec(query);
  while (query.next()) {
    result.push_back(UrlRecord(query, database));
  }

  return result;
}

boost::optional<UrlRecord> UrlRecord::getUrlRecord(int id, ProjectDatabase& database) {
  boost::optional<UrlRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + UrlRecord::databaseTableName() + " WHERE id=:id"));
  query.bindValue(":id",id);
  assertExec(query);
  if (query.first()) {
    result = UrlRecord(query, database);
  }

  return result;
}

openstudio::Url UrlRecord::url() const {
  return getImpl<detail::UrlRecord_Impl>()->url();
}

/// @cond
UrlRecord::UrlRecord(std::shared_ptr<detail::UrlRecord_Impl> impl)
  : ObjectRecord(impl)
{}

UrlRecord::UrlRecord(std::shared_ptr<detail::UrlRecord_Impl> impl,
                     ProjectDatabase database)
  : ObjectRecord(impl, database)
{
  OS_ASSERT(getImpl<detail::UrlRecord_Impl>());
}
/// @endcond

} // project
} // openstudio

