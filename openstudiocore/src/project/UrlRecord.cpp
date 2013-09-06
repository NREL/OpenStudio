/**********************************************************************
 *  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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

#include <project/UrlRecord.hpp>
#include <project/UrlRecord_Impl.hpp>

#include <project/CloudSessionRecord.hpp>
#include <project/CloudSettingsRecord.hpp>
#include <project/JoinRecord.hpp>

#include <utilities/core/Assert.hpp>

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

  void UrlRecord_Impl::saveRow(const boost::shared_ptr<QSqlDatabase> &database) {
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
    query.bindValue(UrlRecord::ColumnsType::url,m_url.toString());
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
  : ObjectRecord(boost::shared_ptr<detail::UrlRecord_Impl>(
        new detail::UrlRecord_Impl(url, parentRecord)),
        parentRecord.projectDatabase())
{
  OS_ASSERT(getImpl<detail::UrlRecord_Impl>());
}

UrlRecord::UrlRecord(const QSqlQuery& query, ProjectDatabase& database)
  : ObjectRecord(boost::shared_ptr<detail::UrlRecord_Impl>(
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
    for (std::set<int>::const_iterator it = result.columnValues.begin(),
         itend = result.columnValues.end(); it != itend; ++it)
    {
      // require 0 based columns, don't skip any
      OS_ASSERT(*it == expectedValue);
      // column name is name, type is description
      ss << ColumnsType::valueName(*it) << "=:" << ColumnsType::valueName(*it);
      // is this the last column?
      std::set<int>::const_iterator nextIt = it;
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
    for (std::set<int>::const_iterator it = result.columnValues.begin(),
         itend = result.columnValues.end(); it != itend; ++it)
    {
      // bind all values to avoid parameter mismatch error
      if (istringEqual(ColumnsType::valueDescription(*it), "INTEGER")) {
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
UrlRecord::UrlRecord(boost::shared_ptr<detail::UrlRecord_Impl> impl)
  : ObjectRecord(impl)
{}

UrlRecord::UrlRecord(boost::shared_ptr<detail::UrlRecord_Impl> impl,
                     ProjectDatabase database)
  : ObjectRecord(impl, database)
{
  OS_ASSERT(getImpl<detail::UrlRecord_Impl>());
}
/// @endcond

} // project
} // openstudio

