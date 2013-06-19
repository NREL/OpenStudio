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

#include <project/StandardsRuleRecord.hpp>
#include <project/StandardsRuleRecord_Impl.hpp>
#include <project/FilterClauseRecord.hpp>
#include <project/ActionClauseRecord.hpp>
#include <project/StandardsAttributeCheckRuleRecord.hpp>
#include <project/ProjectDatabase.hpp>
#include <project/Rule_Clause_JoinRecord.hpp>
#include <project/Rule_Clause_JoinRecord_Impl.hpp>
#include <project/Ruleset_Rule_JoinRecord.hpp>
#include <project/Ruleset_Rule_JoinRecord_Impl.hpp>

#include <ruleset/StandardsRule.hpp>
#include <ruleset/StandardsAttributeCheckRule.hpp>
#include <ruleset/StandardsAttributeCheckRule_Impl.hpp>
#include <ruleset/StandardsFilterClause.hpp>
#include <ruleset/StandardsFilterClause_Impl.hpp>
#include <ruleset/StandardsActionClause.hpp>
#include <ruleset/StandardsActionClause_Impl.hpp>

#include <utilities/core/Assert.hpp>
#include <utilities/core/Containers.hpp>

#include <boost/optional/optional.hpp>
#include <boost/foreach.hpp>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

using openstudio::ruleset::FilterClause;
using openstudio::ruleset::ActionClause;
using openstudio::ruleset::StandardsFilterClause;
using openstudio::ruleset::StandardsFilterClauseVector;
using openstudio::ruleset::StandardsActionClause;

namespace openstudio {
namespace project {
  namespace detail {

    StandardsRuleRecord_Impl::StandardsRuleRecord_Impl(const openstudio::ruleset::StandardsRule& standardsRule, 
                                                       const ProjectDatabase& projectDatabase)
      : RuleRecord_Impl(standardsRule, RuleRecordType::StandardsRuleRecord, projectDatabase)
    {
      // DLM@20110601: temp code
      if (standardsRule.optionalCast<ruleset::StandardsAttributeCheckRule>()){
        setRuleRecordType(RuleRecordType::StandardsAttributeCheckRuleRecord);
      }
    }

    StandardsRuleRecord_Impl::StandardsRuleRecord_Impl(const openstudio::ruleset::StandardsRule& standardsRule, 
                                                       const RuleRecordType& ruleRecordType, 
                                                       const ProjectDatabase& projectDatabase)
      : RuleRecord_Impl(standardsRule, ruleRecordType, projectDatabase)
    {}

    StandardsRuleRecord_Impl::StandardsRuleRecord_Impl(const QSqlQuery& query, const ProjectDatabase& projectDatabase)
      : RuleRecord_Impl(query, projectDatabase)
    {}

    openstudio::ruleset::StandardsRule StandardsRuleRecord_Impl::standardsRule() const
    {
      std::vector<FilterClauseRecord> filterClauseRecords = this->filterClauseRecords();
      std::vector<ActionClauseRecord> actionClauseRecords = this->actionClauseRecords();

      // nominally get filters and actions
      std::vector<FilterClause> filters;
      BOOST_FOREACH(const FilterClauseRecord& filterClauseRecord, filterClauseRecords){
        FilterClause filterClause = filterClauseRecord.filterClause();
        filters.push_back(filterClause);
      }
      std::vector<ActionClause> actions;
      BOOST_FOREACH(const ActionClauseRecord& actionClauseRecord, actionClauseRecords){
        ActionClause actionClause = actionClauseRecord.actionClause();
        actions.push_back(actionClause);
      }

      // clean up cross-references
      StandardsFilterClauseVector standardsFilters = castVector<StandardsFilterClause>(filters);
      BOOST_FOREACH(StandardsFilterClause& filter,standardsFilters) {
        filter.replaceReferencedFilters(standardsFilters);
      }
      BOOST_FOREACH(ActionClause& temp,actions) {
        ruleset::StandardsActionClause action = temp.cast<StandardsActionClause>();
        action.replaceReferencedFilters(standardsFilters);
      }

      openstudio::ruleset::StandardsRule result(this->name(), 
                                                this->active(), 
                                                this->handle(), 
                                                this->uuidLast(), 
                                                filters, 
                                                actions);
      
      return result;    
    }

    boost::optional<ObjectRecord> StandardsRuleRecord_Impl::parent() const
    {
      return boost::none;
    }

    std::vector<ObjectRecord> StandardsRuleRecord_Impl::children() const
    {
      return std::vector<ObjectRecord>();
    }

    std::vector<ObjectRecord> StandardsRuleRecord_Impl::resources() const
    {
      std::vector<ClauseRecord> result = this->clauseRecords();
      return std::vector<ObjectRecord>(result.begin(), result.end());
    }

    std::vector<JoinRecord> StandardsRuleRecord_Impl::joinRecords() const
    {
      std::vector<JoinRecord> result;
      std::vector<Rule_Clause_JoinRecord> clauseJoins = JoinRecord::getJoinRecordsForLeftId<Rule_Clause_JoinRecord>(this->id(), this->projectDatabase());
      result.insert(result.end(), clauseJoins.begin(), clauseJoins.end());
      std::vector<Ruleset_Rule_JoinRecord> rulesetJoins = JoinRecord::getJoinRecordsForRightId<Ruleset_Rule_JoinRecord>(this->id(), this->projectDatabase());
      result.insert(result.end(), rulesetJoins.begin(), rulesetJoins.end());
      return result;
    }

    void StandardsRuleRecord_Impl::saveRow(const boost::shared_ptr<QSqlDatabase> &database)
    {
      QSqlQuery query(*database);
      this->makeUpdateByIdQuery<StandardsRuleRecord>(query);
      this->bindValues(query);
      assertExec(query);
    }

    void StandardsRuleRecord_Impl::bindValues(QSqlQuery& query) const
    {
      RuleRecord_Impl::bindValues(query);
    }

    void StandardsRuleRecord_Impl::setLastValues(const QSqlQuery& query, ProjectDatabase& projectDatabase)
    {
      RuleRecord_Impl::setLastValues(query, projectDatabase);
    }

    bool StandardsRuleRecord_Impl::compareValues(const QSqlQuery& query) const
    {
      bool result = true;

      result = result && RuleRecord_Impl::compareValues(query);

      return result;
    }

    void StandardsRuleRecord_Impl::saveLastValues()
    {
      RuleRecord_Impl::saveLastValues();
    }

    void StandardsRuleRecord_Impl::revertToLastValues()
    {
      RuleRecord_Impl::revertToLastValues();
    }

  } // detail

  StandardsRuleRecord::StandardsRuleRecord(const openstudio::ruleset::StandardsRule& standardsRule, ProjectDatabase& projectDatabase)
    : RuleRecord(standardsRule, boost::shared_ptr<detail::StandardsRuleRecord_Impl>(new detail::StandardsRuleRecord_Impl(standardsRule, projectDatabase)), projectDatabase)
  {
    BOOST_ASSERT(getImpl<detail::StandardsRuleRecord_Impl>());
  }

  StandardsRuleRecord::StandardsRuleRecord(const QSqlQuery& query, ProjectDatabase& projectDatabase)
    : RuleRecord(boost::shared_ptr<detail::StandardsRuleRecord_Impl>(new detail::StandardsRuleRecord_Impl(query, projectDatabase)), projectDatabase)
  {
    BOOST_ASSERT(getImpl<detail::StandardsRuleRecord_Impl>());
  }

  StandardsRuleRecord::StandardsRuleRecord(const ruleset::StandardsRule& standardsRule, boost::shared_ptr<detail::StandardsRuleRecord_Impl> impl, ProjectDatabase projectDatabase)
    : RuleRecord(standardsRule, impl, projectDatabase)
  {
    BOOST_ASSERT(getImpl<detail::StandardsRuleRecord_Impl>());
  }

  StandardsRuleRecord::StandardsRuleRecord(boost::shared_ptr<detail::StandardsRuleRecord_Impl> impl, ProjectDatabase projectDatabase)
    : RuleRecord(impl, projectDatabase)
  {
    BOOST_ASSERT(getImpl<detail::StandardsRuleRecord_Impl>());
  }

  StandardsRuleRecord::StandardsRuleRecord(boost::shared_ptr<detail::StandardsRuleRecord_Impl> impl)
    : RuleRecord(impl)
  {
    BOOST_ASSERT(getImpl<detail::StandardsRuleRecord_Impl>());
  }

  boost::optional<StandardsRuleRecord> StandardsRuleRecord::factoryFromQuery(const QSqlQuery& query, 
                                                                   ProjectDatabase& database)
  {
    OptionalStandardsRuleRecord result;
    try {
      result = StandardsRuleRecord(query,database);
    }
    catch (const std::exception& e) {
      LOG(Error,"Unable to construct StandardsRuleRecord from query, because '" 
          << e.what() << "'.");
    }
    return result;
  }

  std::vector<StandardsRuleRecord> StandardsRuleRecord::getStandardsRuleRecords(ProjectDatabase& projectDatabase)
  {
    std::vector<StandardsRuleRecord> result;

    QSqlQuery query(*(projectDatabase.qSqlDatabase()));
    query.prepare(toQString("SELECT * FROM " + RuleRecord::databaseTableName() + " WHERE ruleRecordType=:ruleRecordType"));
    query.bindValue(":ruleRecordType", RuleRecordType::StandardsRuleRecord);
    assertExec(query);
    while (query.next()) {
      result.push_back(StandardsRuleRecord(query, projectDatabase));
    }

    BOOST_FOREACH(const StandardsAttributeCheckRuleRecord standardsAttributeCheckRuleRecord, StandardsAttributeCheckRuleRecord::getStandardsAttributeCheckRuleRecords(projectDatabase)){
      result.push_back(standardsAttributeCheckRuleRecord);
    }

    return result;
  }
  
  boost::optional<StandardsRuleRecord> StandardsRuleRecord::getStandardsRuleRecord(int id, ProjectDatabase& projectDatabase)
  {
    boost::optional<StandardsRuleRecord> result;

    QSqlQuery query(*(projectDatabase.qSqlDatabase()));
    query.prepare(toQString("SELECT * FROM " + RuleRecord::databaseTableName() + " WHERE id=:id AND ruleRecordType=:ruleRecordType"));
    query.bindValue(":id", id);
    query.bindValue(":ruleRecordType", RuleRecordType::StandardsRuleRecord);
    assertExec(query);
    if (query.first()) {
      result = StandardsRuleRecord(query, projectDatabase);
    }

    if (!result){
      result = StandardsAttributeCheckRuleRecord::getStandardsAttributeCheckRuleRecord(id, projectDatabase);

    }

    return result;
  }

  openstudio::ruleset::StandardsRule StandardsRuleRecord::standardsRule() const
  {
    return getImpl<detail::StandardsRuleRecord_Impl>()->standardsRule();
  }

} // project
} // openstudio
