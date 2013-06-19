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

#include <project/ClauseRecord.hpp>
#include <project/ClauseRecord_Impl.hpp>
#include <project/JoinRecord.hpp>
#include <project/ProjectDatabase.hpp>
#include <project/FilterClauseRecord.hpp>
#include <project/ActionClauseRecord.hpp>

#include <ruleset/Clause.hpp>

#include <utilities/core/Assert.hpp>

#include <boost/optional/optional.hpp>
#include <boost/foreach.hpp>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

namespace openstudio {
namespace project {

namespace detail {

  ClauseRecord_Impl::ClauseRecord_Impl(const openstudio::ruleset::Clause& clause, const ClauseRecordType& clauseRecordType, const ProjectDatabase& projectDatabase)
    : ObjectRecord_Impl(projectDatabase, clause.uuid(), "", "", "", clause.versionUUID()), m_clauseRecordType(clauseRecordType)
  {}

  ClauseRecord_Impl::ClauseRecord_Impl(const QSqlQuery& query, const ProjectDatabase& projectDatabase)
    : ObjectRecord_Impl(projectDatabase, query)
  {
    BOOST_ASSERT(query.isValid());
    BOOST_ASSERT(query.isActive());
    BOOST_ASSERT(query.isSelect());

    QVariant value;
    value = query.value(ClauseRecordColumns::clauseRecordType);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    m_clauseRecordType = ClauseRecordType(value.toInt());
  }

  std::string ClauseRecord_Impl::databaseTableName() const
  {
    return ClauseRecord::databaseTableName();
  }

  void ClauseRecord_Impl::bindValues(QSqlQuery& query) const
  {
    ObjectRecord_Impl::bindValues(query);

    query.bindValue(ClauseRecordColumns::clauseRecordType, m_clauseRecordType.value());
  }

  void ClauseRecord_Impl::setLastValues(const QSqlQuery& query, ProjectDatabase& projectDatabase)
  {
    BOOST_ASSERT(query.isValid());
    BOOST_ASSERT(query.isActive());
    BOOST_ASSERT(query.isSelect());

    ObjectRecord_Impl::setLastValues(query, projectDatabase);

    QVariant value;
    value = query.value(ClauseRecordColumns::clauseRecordType);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    m_lastClauseRecordType = ClauseRecordType(value.toInt());
  }

  bool ClauseRecord_Impl::compareValues(const QSqlQuery& query) const
  {
    bool result = true;

    result = result && ObjectRecord_Impl::compareValues(query);

    QVariant value;
    value = query.value(ClauseRecordColumns::clauseRecordType);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_clauseRecordType == ClauseRecordType(value.toInt()));

    return result;
  }

  void ClauseRecord_Impl::saveLastValues()
  {
    ObjectRecord_Impl::saveLastValues();

    m_lastClauseRecordType = m_clauseRecordType;
  }

  void ClauseRecord_Impl::revertToLastValues()
  {
    ObjectRecord_Impl::revertToLastValues();

    m_clauseRecordType = m_lastClauseRecordType;
  }

} // detail

std::string ClauseRecord::databaseTableName() {
  return "ClauseRecords";
}

UpdateByIdQueryData ClauseRecord::updateByIdQueryData() {
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
      BOOST_ASSERT(*it == expectedValue);
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

void ClauseRecord::updatePathData(ProjectDatabase database,
                                  const openstudio::path& originalBase,
                                  const openstudio::path& newBase)
{
  // At this time, no ClauseRecords contain path data.
}

ClauseRecord::ClauseRecord(boost::shared_ptr<detail::ClauseRecord_Impl> impl, ProjectDatabase projectDatabase)
  : ObjectRecord(impl, projectDatabase)
{
  BOOST_ASSERT(getImpl<detail::ClauseRecord_Impl>());
}

ClauseRecord::ClauseRecord(boost::shared_ptr<detail::ClauseRecord_Impl> impl)
  : ObjectRecord(impl)
{
  BOOST_ASSERT(getImpl<detail::ClauseRecord_Impl>());
}

std::vector<ClauseRecord> ClauseRecord::getClauseRecords(ProjectDatabase& database)
{
  std::vector<ClauseRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + ClauseRecord::databaseTableName()));
  assertExec(query);
  while (query.next()) {
    boost::optional<ClauseRecord> clauseRecord = factoryFromQuery(query, database);
    BOOST_ASSERT(clauseRecord);
    result.push_back(*clauseRecord);
  }

  return result;

}

boost::optional<ClauseRecord> ClauseRecord::getClauseRecord(int id, ProjectDatabase& database)
{
  boost::optional<ClauseRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + ClauseRecord::databaseTableName() + " WHERE id=:id"));
  query.bindValue(":id", id);
  assertExec(query);
  if (query.first()) {
    result = factoryFromQuery(query, database);
  }

  return result;
}

boost::optional<ClauseRecord> ClauseRecord::factoryFromQuery(const QSqlQuery& query, ProjectDatabase& database)
{
  boost::optional<ClauseRecord> result;

  int clauseRecordType = query.value(ClauseRecordColumns::clauseRecordType).toInt();

  switch (clauseRecordType){
  case ClauseRecordType::FilterClauseRecord:
    result = FilterClauseRecord::factoryFromQuery(query, database);
    break;
  case ClauseRecordType::ActionClauseRecord:
    result = ActionClauseRecord::factoryFromQuery(query, database);
    break;
  default:
    LOG(Error, "Unknown clauseRecordType " << clauseRecordType);
  }

  return result;
}

} // project
} // openstudio
