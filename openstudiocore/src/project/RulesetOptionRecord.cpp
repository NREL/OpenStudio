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

#include <project/RulesetOptionRecord.hpp>
#include <project/RulesetOptionRecord_Impl.hpp>
#include <project/JoinRecord.hpp>
#include <project/ProjectDatabase.hpp>
#include <project/RulesetRecord.hpp>
#include <project/StandardsRulesetOptionRecord.hpp>

#include <ruleset/RulesetOption.hpp>

#include <utilities/core/Assert.hpp>

#include <boost/optional/optional.hpp>
#include <boost/foreach.hpp>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

namespace openstudio {
namespace project {

namespace detail {

  RulesetOptionRecord_Impl::RulesetOptionRecord_Impl(const openstudio::ruleset::RulesetOption& rulesetOption, const RulesetOptionRecordType& rulesetOptionRecordType, const RulesetRecord& rulesetRecord)
    : ObjectRecord_Impl(rulesetRecord.projectDatabase(), rulesetOption.uuid(), "", "", "", rulesetOption.versionUUID()), m_rulesetOptionRecordType(rulesetOptionRecordType), m_rulesetRecordId(rulesetRecord.id())
  {}

  RulesetOptionRecord_Impl::RulesetOptionRecord_Impl(const openstudio::ruleset::RulesetOption& rulesetOption, const RulesetOptionRecordType& rulesetOptionRecordType, const ProjectDatabase& projectDatabase)
    : ObjectRecord_Impl(projectDatabase, rulesetOption.uuid(), "", "", "", rulesetOption.versionUUID()), m_rulesetOptionRecordType(rulesetOptionRecordType)
  {
  }

  RulesetOptionRecord_Impl::RulesetOptionRecord_Impl(const QSqlQuery& query, const ProjectDatabase& projectDatabase)
    : ObjectRecord_Impl(projectDatabase, query)
  {
    BOOST_ASSERT(query.isValid());
    BOOST_ASSERT(query.isActive());
    BOOST_ASSERT(query.isSelect());

    QVariant value;
    value = query.value(RulesetOptionRecordColumns::rulesetOptionRecordType);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    m_rulesetOptionRecordType = RulesetOptionRecordType(value.toInt());

    value = query.value(RulesetOptionRecordColumns::rulesetRecordId);
    if(value.isValid() && !value.isNull()){
      m_rulesetRecordId = value.toInt();
    }
  }

  std::string RulesetOptionRecord_Impl::databaseTableName() const
  {
    return RulesetOptionRecord::databaseTableName();
  }

  boost::optional<RulesetRecord> RulesetOptionRecord_Impl::rulesetRecord() const
  {
    boost::optional<RulesetRecord> result;
    if (m_rulesetRecordId){
      ProjectDatabase projectDatabase = this->projectDatabase();
      result = RulesetRecord::getRulesetRecord(*m_rulesetRecordId, projectDatabase);
    }
    return result;
  }

  void RulesetOptionRecord_Impl::bindValues(QSqlQuery& query) const
  {
    ObjectRecord_Impl::bindValues(query);

    query.bindValue(RulesetOptionRecordColumns::rulesetOptionRecordType, m_rulesetOptionRecordType.value());
    if (m_rulesetRecordId){
      query.bindValue(RulesetOptionRecordColumns::rulesetRecordId, *m_rulesetRecordId);
    }else{
      query.bindValue(RulesetOptionRecordColumns::rulesetRecordId, QVariant(QVariant::Int));
    }
  }

  void RulesetOptionRecord_Impl::setLastValues(const QSqlQuery& query, ProjectDatabase& projectDatabase)
  {
    BOOST_ASSERT(query.isValid());
    BOOST_ASSERT(query.isActive());
    BOOST_ASSERT(query.isSelect());

    ObjectRecord_Impl::setLastValues(query, projectDatabase);

    QVariant value;
    value = query.value(RulesetOptionRecordColumns::rulesetOptionRecordType);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    m_lastRulesetOptionRecordType = RulesetOptionRecordType(value.toInt());

    value = query.value(RulesetOptionRecordColumns::rulesetRecordId);
    if(value.isValid() && !value.isNull()){
      m_lastRulesetRecordId = value.toInt();
    }else{
      m_lastRulesetRecordId.reset();
    }
  }

  bool RulesetOptionRecord_Impl::compareValues(const QSqlQuery& query) const
  {
    bool result = true;

    result = result && ObjectRecord_Impl::compareValues(query);

    QVariant value;
    value = query.value(RulesetOptionRecordColumns::rulesetOptionRecordType);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_rulesetOptionRecordType == RulesetOptionRecordType(value.toInt()));

    value = query.value(RulesetOptionRecordColumns::rulesetRecordId);
    if(value.isValid() && !value.isNull()){
      result = result && m_rulesetRecordId && (*m_rulesetRecordId == value.toInt());
    }else{
      result = result && !m_rulesetRecordId;
    }

    return result;
  }

  void RulesetOptionRecord_Impl::saveLastValues()
  {
    ObjectRecord_Impl::saveLastValues();

    m_lastRulesetOptionRecordType = m_rulesetOptionRecordType;
    m_lastRulesetRecordId = m_rulesetRecordId;
  }

  void RulesetOptionRecord_Impl::revertToLastValues()
  {
    ObjectRecord_Impl::revertToLastValues();

    m_rulesetOptionRecordType = m_lastRulesetOptionRecordType;
    m_rulesetRecordId = m_lastRulesetRecordId;
  }

} // detail

std::string RulesetOptionRecord::databaseTableName() {
  return "RulesetOptionRecords";
}

UpdateByIdQueryData RulesetOptionRecord::updateByIdQueryData() {
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

void RulesetOptionRecord::updatePathData(ProjectDatabase database,
                                         const openstudio::path& originalBase,
                                         const openstudio::path& newBase)
{}

RulesetOptionRecord::RulesetOptionRecord(boost::shared_ptr<detail::RulesetOptionRecord_Impl> impl, ProjectDatabase projectDatabase)
  : ObjectRecord(impl, projectDatabase)
{
  BOOST_ASSERT(getImpl<detail::RulesetOptionRecord_Impl>());
}

RulesetOptionRecord::RulesetOptionRecord(boost::shared_ptr<detail::RulesetOptionRecord_Impl> impl)
  : ObjectRecord(impl)
{
  BOOST_ASSERT(getImpl<detail::RulesetOptionRecord_Impl>());
}

std::vector<RulesetOptionRecord> RulesetOptionRecord::getRulesetOptionRecords(ProjectDatabase& database)
{
  std::vector<RulesetOptionRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + RulesetOptionRecord::databaseTableName()));
  assertExec(query);
  while (query.next()) {
    boost::optional<RulesetOptionRecord> rulesetOptionRecord = factoryFromQuery(query, database);
    BOOST_ASSERT(rulesetOptionRecord);
    result.push_back(*rulesetOptionRecord);
  }

  return result;

}

boost::optional<RulesetOptionRecord> RulesetOptionRecord::getRulesetOptionRecord(int id, ProjectDatabase& database)
{
  boost::optional<RulesetOptionRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + RulesetOptionRecord::databaseTableName() + " WHERE id=:id"));
  query.bindValue(":id", id);
  assertExec(query);
  if (query.first()) {
    result = factoryFromQuery(query, database);
  }

  return result;
}

boost::optional<RulesetOptionRecord> RulesetOptionRecord::factoryFromQuery(const QSqlQuery& query, ProjectDatabase& database)
{
  boost::optional<RulesetOptionRecord> result;

  int rulesetOptionRecordType = query.value(RulesetOptionRecordColumns::rulesetOptionRecordType).toInt();

  switch (rulesetOptionRecordType){
  case RulesetOptionRecordType::StandardsRulesetOptionRecord:
    result = StandardsRulesetOptionRecord::factoryFromQuery(query, database);
    break;
  default:
    LOG(Error, "Unknown rulesetOptionRecordType " << rulesetOptionRecordType);
  }

  return result;
}

boost::optional<RulesetRecord> RulesetOptionRecord::rulesetRecord() const
{
  return this->getImpl<detail::RulesetOptionRecord_Impl>()->rulesetRecord();
}

} // project
} // openstudio
