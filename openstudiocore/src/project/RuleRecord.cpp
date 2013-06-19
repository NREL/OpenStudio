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

#include <project/RuleRecord.hpp>
#include <project/RuleRecord_Impl.hpp>
#include <project/JoinRecord.hpp>
#include <project/ProjectDatabase.hpp>
#include <project/FilterClauseRecord.hpp>
#include <project/FilterClauseRecord_Impl.hpp>
#include <project/ActionClauseRecord.hpp>
#include <project/ActionClauseRecord_Impl.hpp>
#include <project/Rule_Clause_JoinRecord.hpp>
#include <project/Rule_Clause_JoinRecord_Impl.hpp>
#include <project/ModelRuleRecord.hpp>
#include <project/StandardsRuleRecord.hpp>
#include <project/StandardsAttributeCheckRuleRecord.hpp>

#include <ruleset/Rule.hpp>
#include <ruleset/FilterClause.hpp>
#include <ruleset/ActionClause.hpp>

#include <utilities/core/Assert.hpp>

#include <boost/optional/optional.hpp>
#include <boost/foreach.hpp>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

namespace openstudio {
namespace project {
  namespace detail {

    RuleRecord_Impl::RuleRecord_Impl(const openstudio::ruleset::Rule& rule, const RuleRecordType& ruleRecordType, const ProjectDatabase& projectDatabase)
      : ObjectRecord_Impl(projectDatabase, rule.uuid(), rule.name(), rule.name(), "", rule.versionUUID()), 
        m_active(rule.active()), m_ruleRecordType(ruleRecordType)
    {
      LOG(Trace,"Constructing RuleRecord_Impl for " << rule.name() << ".");
    }

    RuleRecord_Impl::RuleRecord_Impl(const QSqlQuery& query, const ProjectDatabase& projectDatabase)
      : ObjectRecord_Impl(projectDatabase, query)
    {
      BOOST_ASSERT(query.isValid());
      BOOST_ASSERT(query.isActive());
      BOOST_ASSERT(query.isSelect());

      QVariant value;

      value = query.value(RuleRecordColumns::active);
      BOOST_ASSERT(value.isValid() && !value.isNull());
      m_active = value.toBool();

      value = query.value(RuleRecordColumns::ruleRecordType);
      BOOST_ASSERT(value.isValid() && !value.isNull());
      m_ruleRecordType = RuleRecordType(value.toInt());
    }

    std::string RuleRecord_Impl::databaseTableName() const
    {
      return RuleRecord::databaseTableName();
    }

    bool RuleRecord_Impl::active() const {
      return m_active;
    }
        
    std::vector<ClauseRecord> RuleRecord_Impl::clauseRecords() const
    {
      std::vector<FilterClauseRecord> result;
      return Rule_Clause_JoinRecord::getClauseRecords(this->getObject<RuleRecord>());
    }

    std::vector<FilterClauseRecord> RuleRecord_Impl::filterClauseRecords() const
    {
      std::vector<FilterClauseRecord> result;
      std::vector<ClauseRecord> clauseRecords = Rule_Clause_JoinRecord::getClauseRecords(this->getObject<RuleRecord>());
      BOOST_FOREACH(const ClauseRecord& clauseRecord, clauseRecords){
        if (clauseRecord.optionalCast<FilterClauseRecord>()){
          result.push_back(clauseRecord.cast<FilterClauseRecord>());
        }
      }

      return result;
    }

    std::vector<ActionClauseRecord> RuleRecord_Impl::actionClauseRecords() const
    {
      std::vector<ActionClauseRecord> result;
      std::vector<ClauseRecord> clauseRecords = Rule_Clause_JoinRecord::getClauseRecords(this->getObject<RuleRecord>());
      BOOST_FOREACH(const ClauseRecord& clauseRecord, clauseRecords){
        if (clauseRecord.optionalCast<ActionClauseRecord>()){
          result.push_back(clauseRecord.cast<ActionClauseRecord>());
        }
      }

      return result;
    }

    void RuleRecord_Impl::bindValues(QSqlQuery& query) const
    {
      ObjectRecord_Impl::bindValues(query);

      query.bindValue(RuleRecordColumns::active, m_active);
      query.bindValue(RuleRecordColumns::ruleRecordType, m_ruleRecordType.value());
    }

    void RuleRecord_Impl::setLastValues(const QSqlQuery& query, ProjectDatabase& projectDatabase)
    {
      BOOST_ASSERT(query.isValid());
      BOOST_ASSERT(query.isActive());
      BOOST_ASSERT(query.isSelect());

      ObjectRecord_Impl::setLastValues(query, projectDatabase);

      QVariant value;
      value = query.value(RuleRecordColumns::active);
      BOOST_ASSERT(value.isValid() && !value.isNull());
      m_lastActive = value.toBool();

      value = query.value(RuleRecordColumns::ruleRecordType);
      BOOST_ASSERT(value.isValid() && !value.isNull());
      m_lastRuleRecordType = RuleRecordType(value.toInt());
    }

    bool RuleRecord_Impl::compareValues(const QSqlQuery& query) const
    {
      bool result = true;

      result = result && ObjectRecord_Impl::compareValues(query);

      QVariant value;
      value = query.value(RuleRecordColumns::active);
      BOOST_ASSERT(value.isValid() && !value.isNull());
      result = result && (m_active == value.toBool());

      value = query.value(RuleRecordColumns::ruleRecordType);
      BOOST_ASSERT(value.isValid() && !value.isNull());
      result = result && (m_ruleRecordType == RuleRecordType(value.toInt()));

      return result;
    }

    void RuleRecord_Impl::saveLastValues()
    {
      ObjectRecord_Impl::saveLastValues();

      m_lastActive = m_active;
      m_lastRuleRecordType = m_ruleRecordType;
    }

    void RuleRecord_Impl::revertToLastValues()
    {
      ObjectRecord_Impl::revertToLastValues();

      m_active = m_lastActive;
      m_ruleRecordType = m_lastRuleRecordType;
    }

    void RuleRecord_Impl::setRuleRecordType(const RuleRecordType& ruleRecordType)
    {
      m_ruleRecordType = ruleRecordType;
      
      // do not issue on change
    }

  } // detail

  std::string RuleRecord::databaseTableName()
  {
    return "RuleRecords";
  }

  UpdateByIdQueryData RuleRecord::updateByIdQueryData() {
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

  void RuleRecord::updatePathData(ProjectDatabase database,
                                  const openstudio::path& originalBase,
                                  const openstudio::path& newBase)
  {}

  RuleRecord::RuleRecord(const ruleset::Rule& rule, 
                         boost::shared_ptr<detail::RuleRecord_Impl> impl, 
                         ProjectDatabase projectDatabase)
    : ObjectRecord(impl, projectDatabase)
  {
    BOOST_ASSERT(getImpl<detail::RuleRecord_Impl>());

    LOG(Trace,"Constructing RuleRecord for " << rule.name() << ".");

    // need to do in wrapper so we can create join records and with non-const database
    RuleRecord other(getImpl<detail::RuleRecord_Impl>());

    LOG(Trace,"Adding FilterClauseRecords.");

    // add all the filters
    BOOST_FOREACH(const openstudio::ruleset::FilterClause& filter, rule.filters()){
      FilterClauseRecord filterRecord = FilterClauseRecord::factoryFromFilter(filter, projectDatabase);
      if (!Rule_Clause_JoinRecord::getJoinRecord<Rule_Clause_JoinRecord>(this->id(), filterRecord.id(), projectDatabase)){
        Rule_Clause_JoinRecord join(other, filterRecord);
      }
    }

    LOG(Trace,"Adding ActionClauseRecords.");

    // add all the actions
    BOOST_FOREACH(const openstudio::ruleset::ActionClause& action, rule.actions()){
      ActionClauseRecord actionRecord = ActionClauseRecord::factoryFromAction(action, projectDatabase);
      if (!Rule_Clause_JoinRecord::getJoinRecord<Rule_Clause_JoinRecord>(this->id(), actionRecord.id(), projectDatabase)){
        Rule_Clause_JoinRecord join(other, actionRecord);
      }
    }
  }

  RuleRecord::RuleRecord(boost::shared_ptr<detail::RuleRecord_Impl> impl, ProjectDatabase projectDatabase)
    : ObjectRecord(impl, projectDatabase)
  {
    BOOST_ASSERT(getImpl<detail::RuleRecord_Impl>());
  }

  RuleRecord::RuleRecord(boost::shared_ptr<detail::RuleRecord_Impl> impl)
    : ObjectRecord(impl)
  {
    BOOST_ASSERT(getImpl<detail::RuleRecord_Impl>());
  }

  std::vector<RuleRecord> RuleRecord::getRuleRecords(ProjectDatabase& database)
  {
    std::vector<RuleRecord> result;

    QSqlQuery query(*(database.qSqlDatabase()));
    query.prepare(toQString("SELECT * FROM " + RuleRecord::databaseTableName()));
    assertExec(query);
    while (query.next()) {
      boost::optional<RuleRecord> ruleRecord = factoryFromQuery(query, database);
      BOOST_ASSERT(ruleRecord);
      result.push_back(*ruleRecord);
    }

    return result;
  }

  boost::optional<RuleRecord> RuleRecord::getRuleRecord(int id, ProjectDatabase& database)
  {
    boost::optional<RuleRecord> result;

    QSqlQuery query(*(database.qSqlDatabase()));
    query.prepare(toQString("SELECT * FROM " + RuleRecord::databaseTableName() + " WHERE id=:id"));
    query.bindValue(":id", id);
    assertExec(query);
    if (query.first()) {
      result = factoryFromQuery(query, database);
    }

    return result;
  }
    
  boost::optional<RuleRecord> RuleRecord::factoryFromQuery(const QSqlQuery& query, ProjectDatabase& database)
  {
    boost::optional<RuleRecord> result;

    int ruleRecordType = query.value(RuleRecordColumns::ruleRecordType).toInt();

    switch (ruleRecordType){
      case RuleRecordType::ModelRuleRecord:
        result = ModelRuleRecord(query, database);
        break;
      case RuleRecordType::StandardsRuleRecord:
        result = StandardsRuleRecord(query, database);
        break;
      case RuleRecordType::StandardsAttributeCheckRuleRecord:
        result = StandardsAttributeCheckRuleRecord(query, database);
        break;
      default:
        LOG(Error, "Unknown ruleRecordType " << ruleRecordType);
    }

    return result;
  }

  bool RuleRecord::active() const {
    return getImpl<detail::RuleRecord_Impl>()->active();
  }

  std::vector<FilterClauseRecord> RuleRecord::filterClauseRecords() const
  {
    return this->getImpl<detail::RuleRecord_Impl>()->filterClauseRecords();
  }

  std::vector<ActionClauseRecord> RuleRecord::actionClauseRecords() const
  {
    return this->getImpl<detail::RuleRecord_Impl>()->actionClauseRecords();
  }

} // project
} // openstudio
