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

#include <project/RulesetRecord.hpp>
#include <project/RulesetRecord_Impl.hpp>
#include <project/JoinRecord.hpp>
#include <project/ProjectDatabase.hpp>
#include <project/ModelRulesetRecord.hpp>

#include <ruleset/Ruleset.hpp>

#include <utilities/core/Assert.hpp>

#include <boost/optional/optional.hpp>
#include <boost/foreach.hpp>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

namespace openstudio {
namespace project {

namespace detail {

  RulesetRecord_Impl::RulesetRecord_Impl(const openstudio::ruleset::Ruleset& ruleset, const RulesetRecordType& rulesetRecordType, const ProjectDatabase& projectDatabase)
    : ObjectRecord_Impl(projectDatabase, ruleset.uuid(), ruleset.name(), ruleset.name(), "", ruleset.versionUUID()), m_rulesetRecordType(rulesetRecordType)
  {
  }

  RulesetRecord_Impl::RulesetRecord_Impl(const QSqlQuery& query, const ProjectDatabase& projectDatabase)
    : ObjectRecord_Impl(projectDatabase, query)
  {
    BOOST_ASSERT(query.isValid());
    BOOST_ASSERT(query.isActive());
    BOOST_ASSERT(query.isSelect());

    QVariant value;
    value = query.value(RulesetRecordColumns::rulesetRecordType);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    m_rulesetRecordType = RulesetRecordType(value.toInt());
  }

  std::string RulesetRecord_Impl::databaseTableName() const
  {
    return RulesetRecord::databaseTableName();
  }

  void RulesetRecord_Impl::bindValues(QSqlQuery& query) const
  {
    ObjectRecord_Impl::bindValues(query);

    query.bindValue(RulesetRecordColumns::rulesetRecordType, m_rulesetRecordType.value());
  }

  void RulesetRecord_Impl::setLastValues(const QSqlQuery& query, ProjectDatabase& projectDatabase)
  {
    BOOST_ASSERT(query.isValid());
    BOOST_ASSERT(query.isActive());
    BOOST_ASSERT(query.isSelect());

    ObjectRecord_Impl::setLastValues(query, projectDatabase);

    QVariant value;
    value = query.value(RulesetRecordColumns::rulesetRecordType);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    m_lastRulesetRecordType = RulesetRecordType(value.toInt());
  }

  bool RulesetRecord_Impl::compareValues(const QSqlQuery& query) const
  {
    bool result = true;

    result = result && ObjectRecord_Impl::compareValues(query);

    QVariant value;
    value = query.value(RulesetRecordColumns::rulesetRecordType);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_rulesetRecordType == RulesetRecordType(value.toInt()));

    return result;
  }

  void RulesetRecord_Impl::saveLastValues()
  {
    ObjectRecord_Impl::saveLastValues();

    m_lastRulesetRecordType = m_rulesetRecordType;
  }

  void RulesetRecord_Impl::revertToLastValues()
  {
    ObjectRecord_Impl::revertToLastValues();

    m_rulesetRecordType = m_lastRulesetRecordType;
  }

} // detail

std::string RulesetRecord::databaseTableName() {
  return "RulesetRecords";
}

UpdateByIdQueryData RulesetRecord::updateByIdQueryData() {
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

void RulesetRecord::updatePathData(ProjectDatabase database,
                                   const openstudio::path& originalBase,
                                   const openstudio::path& newBase)
{}

RulesetRecord::RulesetRecord(boost::shared_ptr<detail::RulesetRecord_Impl> impl, ProjectDatabase projectDatabase)
  : ObjectRecord(impl, projectDatabase)
{
  BOOST_ASSERT(getImpl<detail::RulesetRecord_Impl>());
}

RulesetRecord::RulesetRecord(boost::shared_ptr<detail::RulesetRecord_Impl> impl)
  : ObjectRecord(impl)
{
  BOOST_ASSERT(getImpl<detail::RulesetRecord_Impl>());
}

std::vector<RulesetRecord> RulesetRecord::getRulesetRecords(ProjectDatabase& database)
{
  std::vector<RulesetRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + RulesetRecord::databaseTableName()));
  assertExec(query);
  while (query.next()) {
    boost::optional<RulesetRecord> rulesetRecord = factoryFromQuery(query, database);
    BOOST_ASSERT(rulesetRecord);
    result.push_back(*rulesetRecord);
  }

  return result;
}

boost::optional<RulesetRecord> RulesetRecord::getRulesetRecord(int id, ProjectDatabase& database)
{
  boost::optional<RulesetRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + RulesetRecord::databaseTableName() + " WHERE id=:id"));
  query.bindValue(":id", id);
  assertExec(query);
  if (query.first()) {
    result = factoryFromQuery(query, database);
  }

  return result;
}

boost::optional<RulesetRecord> RulesetRecord::factoryFromQuery(const QSqlQuery& query, ProjectDatabase& database)
{
  boost::optional<RulesetRecord> result;

  int rulesetRecordType = query.value(RulesetRecordColumns::rulesetRecordType).toInt();

  switch (rulesetRecordType){
  case RulesetRecordType::ModelRulesetRecord:
    result = ModelRulesetRecord(query, database);
    break;
  default:
    LOG(Error, "Unknown rulesetRecordType " << rulesetRecordType);
  }

  return result;
}

} // project
} // openstudio
