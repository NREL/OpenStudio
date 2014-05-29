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

#include "URLSearchPathRecord.hpp"
#include "URLSearchPathRecord_Impl.hpp"
#include "JoinRecord.hpp"
#include "ProjectDatabase.hpp"

#include "../utilities/core/Assert.hpp"

#include <boost/optional/optional.hpp>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

namespace openstudio {
namespace project {

namespace detail {

  URLSearchPathRecord_Impl::URLSearchPathRecord_Impl(const openstudio::URLSearchPath& urlSearchPath, const ProjectDatabase& projectDatabase)
    : ObjectRecord_Impl(projectDatabase, "", "", ""), m_url(urlSearchPath.getUrl()), m_relativity(urlSearchPath.getRelativity()),
      m_iddObjectType(urlSearchPath.getIddObjectType())
    {}

  URLSearchPathRecord_Impl::URLSearchPathRecord_Impl(const QSqlQuery& query, const ProjectDatabase& projectDatabase)
    : ObjectRecord_Impl(projectDatabase, query)
  {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    QVariant value;
    value = query.value(URLSearchPathRecordColumns::url);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_url = QUrl(value.toString());

    value = query.value(URLSearchPathRecordColumns::relativity);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_relativity = openstudio::URLSearchPath::Relative(value.toUInt());

    value = query.value(URLSearchPathRecordColumns::iddObjectType);
    if(value.isValid() && !value.isNull()){
      m_iddObjectType = IddObjectType(value.toString().toStdString());
    }

  }

  URLSearchPath URLSearchPathRecord_Impl::urlSearchPath() const
  {
    if (m_iddObjectType){
      return URLSearchPath(m_url, m_relativity, *m_iddObjectType);
    }
    return URLSearchPath(m_url, m_relativity);
  }

  std::string URLSearchPathRecord_Impl::databaseTableName() const
  {
    return URLSearchPathRecord::databaseTableName();
  }

  boost::optional<ObjectRecord> URLSearchPathRecord_Impl::parent() const
  {
    return boost::none;
  }

  std::vector<ObjectRecord> URLSearchPathRecord_Impl::children() const
  {
    return std::vector<ObjectRecord>();
  }

  std::vector<ObjectRecord> URLSearchPathRecord_Impl::resources() const
  {
    return std::vector<ObjectRecord>();
  }

  std::vector<JoinRecord> URLSearchPathRecord_Impl::joinRecords() const
  {
    return std::vector<JoinRecord>();
  }

  void URLSearchPathRecord_Impl::saveRow(const std::shared_ptr<QSqlDatabase> &database)
  {
    QSqlQuery query(*database);
    this->makeUpdateByIdQuery<URLSearchPathRecord>(query);
    this->bindValues(query);
    assertExec(query);
  }

  void URLSearchPathRecord_Impl::bindValues(QSqlQuery& query) const
  {
    ObjectRecord_Impl::bindValues(query);

    query.bindValue(URLSearchPathRecordColumns::url, m_url.toString());
    query.bindValue(URLSearchPathRecordColumns::relativity, m_relativity);

    if (m_iddObjectType){
      query.bindValue(URLSearchPathRecordColumns::iddObjectType, toQString(m_iddObjectType->valueName()));
    }else{
      query.bindValue(URLSearchPathRecordColumns::iddObjectType, QVariant(QVariant::String));
    }
  }

  void URLSearchPathRecord_Impl::setLastValues(const QSqlQuery& query, ProjectDatabase& projectDatabase)
  {
    ObjectRecord_Impl::setLastValues(query, projectDatabase);

    QVariant value;
    value = query.value(URLSearchPathRecordColumns::url);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_lastUrl = QUrl(value.toString());

    value = query.value(URLSearchPathRecordColumns::relativity);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_lastRelativity = openstudio::URLSearchPath::Relative(value.toUInt());

    value = query.value(URLSearchPathRecordColumns::iddObjectType);
    if(value.isValid() && !value.isNull()){
      m_lastIddObjectType = IddObjectType(value.toString().toStdString());
    }else{
      m_lastIddObjectType.reset();
    }
  }

  bool URLSearchPathRecord_Impl::compareValues(const QSqlQuery& query) const
  {
    bool result = true;

    result = result && ObjectRecord_Impl::compareValues(query);

    QVariant value;
    value = query.value(URLSearchPathRecordColumns::url);
    OS_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_url == QUrl(value.toString()));

    value = query.value(URLSearchPathRecordColumns::relativity);
    OS_ASSERT(value.isValid() && !value.isNull());
    result = result && (static_cast<unsigned>(m_relativity) == value.toUInt());

    value = query.value(URLSearchPathRecordColumns::iddObjectType);
    if (value.isValid() && !value.isNull()){
      result = result && m_iddObjectType && (*m_iddObjectType == IddObjectType(value.toString().toStdString()));
    }else{
      result = result && !m_iddObjectType;
    }

    return result;
  }

  void URLSearchPathRecord_Impl::saveLastValues()
  {
    ObjectRecord_Impl::saveLastValues();

    m_lastUrl = m_url;
    m_lastRelativity = m_relativity;
    m_lastIddObjectType = m_iddObjectType;
  }

  void URLSearchPathRecord_Impl::revertToLastValues()
  {
    ObjectRecord_Impl::revertToLastValues();

    m_url = m_lastUrl;
    m_relativity = m_lastRelativity;
    m_iddObjectType = m_lastIddObjectType;
  }

} // detail

std::string URLSearchPathRecord::databaseTableName() {
  return "URLSearchPathRecords";
}

UpdateByIdQueryData URLSearchPathRecord::updateByIdQueryData() {
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
  
void URLSearchPathRecord::updatePathData(ProjectDatabase database,
                                         const openstudio::path& originalBase,
                                         const openstudio::path& newBase)
{}

URLSearchPathRecord::URLSearchPathRecord(const openstudio::URLSearchPath& uRLSearchPath, ProjectDatabase& projectDatabase)
  : ObjectRecord(std::shared_ptr<detail::URLSearchPathRecord_Impl>(new detail::URLSearchPathRecord_Impl(uRLSearchPath, projectDatabase)), projectDatabase)
{
  OS_ASSERT(getImpl<detail::URLSearchPathRecord_Impl>());
}

URLSearchPathRecord::URLSearchPathRecord(const QSqlQuery& query, ProjectDatabase& projectDatabase)
  : ObjectRecord(std::shared_ptr<detail::URLSearchPathRecord_Impl>(new detail::URLSearchPathRecord_Impl(query, projectDatabase)), projectDatabase)
{
  OS_ASSERT(getImpl<detail::URLSearchPathRecord_Impl>());
}

URLSearchPathRecord::URLSearchPathRecord(std::shared_ptr<detail::URLSearchPathRecord_Impl> impl, ProjectDatabase projectDatabase)
  : ObjectRecord(impl, projectDatabase)
{
  OS_ASSERT(getImpl<detail::URLSearchPathRecord_Impl>());
}

URLSearchPathRecord::URLSearchPathRecord(std::shared_ptr<detail::URLSearchPathRecord_Impl> impl)
  : ObjectRecord(impl)
{
  OS_ASSERT(getImpl<detail::URLSearchPathRecord_Impl>());
}

boost::optional<URLSearchPathRecord> URLSearchPathRecord::factoryFromQuery(
    const QSqlQuery& query, ProjectDatabase& database)
{
  OptionalURLSearchPathRecord result;
  try {
    result = URLSearchPathRecord(query,database);
  }
  catch (const std::exception& e) {
    LOG(Error,"Unable to construct URLSearchPathRecord from query, because '" 
        << e.what() << "'.");
  }
  return result;
}

std::vector<URLSearchPathRecord> URLSearchPathRecord::getURLSearchPathRecords(ProjectDatabase& projectDatabase)
{
  std::vector<URLSearchPathRecord> result;
  
  QSqlQuery query(*(projectDatabase.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + URLSearchPathRecord::databaseTableName()));
  assertExec(query);
  while (query.next()) {
    result.push_back(projectDatabase.getFromQuery<URLSearchPathRecord>(query));
  }
  
  return result;
}

boost::optional<URLSearchPathRecord> URLSearchPathRecord::getURLSearchPathRecord(int id, ProjectDatabase& projectDatabase)
{
  boost::optional<URLSearchPathRecord> result;
  
  QSqlQuery query(*(projectDatabase.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + URLSearchPathRecord::databaseTableName() + " WHERE id=:id"));
  query.bindValue(":id", id);
  assertExec(query);
  if (query.first()) {
    result = projectDatabase.getFromQuery<URLSearchPathRecord>(query);
  }
  
  return result;
}

openstudio::URLSearchPath URLSearchPathRecord::urlSearchPath() const
{
  return getImpl<detail::URLSearchPathRecord_Impl>()->urlSearchPath();
}

} // project
} // openstudio
