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

#include <project/StandardsAttributeCheckRuleRecord.hpp>
#include <project/StandardsAttributeCheckRuleRecord_Impl.hpp>
#include <project/FilterClauseRecord.hpp>
#include <project/ActionClauseRecord.hpp>
#include <project/ProjectDatabase.hpp>
#include <project/Rule_Clause_JoinRecord.hpp>
#include <project/Rule_Clause_JoinRecord_Impl.hpp>
#include <project/Ruleset_Rule_JoinRecord.hpp>
#include <project/Ruleset_Rule_JoinRecord_Impl.hpp>

#include <ruleset/StandardsAttributeCheckRule.hpp>
#include <ruleset/StandardsAttributeCheckRule_Impl.hpp>
#include <ruleset/StandardsFilterClause.hpp>
#include <ruleset/StandardsFilterClause_Impl.hpp>
#include <ruleset/StandardsActionClause.hpp>
#include <ruleset/StandardsActionClause_Impl.hpp>

#include <utilities/core/Containers.hpp>

#include <boost/foreach.hpp>

using openstudio::ruleset::FilterClause;
using openstudio::ruleset::ActionClause;
using openstudio::ruleset::StandardsFilterClause;
using openstudio::ruleset::StandardsFilterClauseVector;
using openstudio::ruleset::StandardsActionClause;

namespace openstudio {
namespace project {

namespace detail {

  StandardsAttributeCheckRuleRecord_Impl::StandardsAttributeCheckRuleRecord_Impl(
      const openstudio::ruleset::StandardsAttributeCheckRule& standardsAttributeCheckRule, 
      const ProjectDatabase& projectDatabase)
    : StandardsRuleRecord_Impl(standardsAttributeCheckRule, 
                               RuleRecordType::StandardsAttributeCheckRuleRecord, 
                               projectDatabase)
  {}

  StandardsAttributeCheckRuleRecord_Impl::StandardsAttributeCheckRuleRecord_Impl(
      const QSqlQuery& query, const ProjectDatabase& projectDatabase)
    : StandardsRuleRecord_Impl(query,projectDatabase)
  {}

  openstudio::ruleset::StandardsRule StandardsAttributeCheckRuleRecord_Impl::standardsRule() const 
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

    ruleset::StandardsAttributeCheckRule result(this->name(), 
                                                this->active(), 
                                                this->handle(), 
                                                this->uuidLast(), 
                                                filters, 
                                                actions);
        
    return result.cast<ruleset::StandardsRule>();    
  }

  void StandardsAttributeCheckRuleRecord_Impl::saveRow(const boost::shared_ptr<QSqlDatabase> &database)
  {
    QSqlQuery query(*database);
    this->makeUpdateByIdQuery<StandardsAttributeCheckRuleRecord>(query);
    this->bindValues(query);
    assertExec(query);
  }

  void StandardsAttributeCheckRuleRecord_Impl::bindValues(QSqlQuery& query) const {
    StandardsRuleRecord_Impl::bindValues(query);
  }

  void StandardsAttributeCheckRuleRecord_Impl::setLastValues(
      const QSqlQuery& query, ProjectDatabase& projectDatabase)
  {
    StandardsRuleRecord_Impl::setLastValues(query, projectDatabase);
  }

  bool StandardsAttributeCheckRuleRecord_Impl::compareValues(const QSqlQuery& query) const 
  {
    bool result(true);
    result = result && StandardsRuleRecord_Impl::compareValues(query);
    return result;
  }

  void StandardsAttributeCheckRuleRecord_Impl::saveLastValues() {
    StandardsRuleRecord_Impl::saveLastValues();
  }

  void StandardsAttributeCheckRuleRecord_Impl::revertToLastValues() {
    StandardsRuleRecord_Impl::revertToLastValues();
  }

} // detail

StandardsAttributeCheckRuleRecord::StandardsAttributeCheckRuleRecord(
    const openstudio::ruleset::StandardsAttributeCheckRule& standardsAttributeCheckRule, 
    ProjectDatabase& projectDatabase)
  : StandardsRuleRecord(standardsAttributeCheckRule,
        boost::shared_ptr<detail::StandardsAttributeCheckRuleRecord_Impl>(new detail::StandardsAttributeCheckRuleRecord_Impl(standardsAttributeCheckRule,projectDatabase)),
        projectDatabase)
{
  BOOST_ASSERT(getImpl<detail::StandardsAttributeCheckRuleRecord_Impl>());
}

StandardsAttributeCheckRuleRecord::StandardsAttributeCheckRuleRecord(
    const QSqlQuery& query,ProjectDatabase& projectDatabase)
  : StandardsRuleRecord(boost::shared_ptr<detail::StandardsAttributeCheckRuleRecord_Impl>(
        new detail::StandardsAttributeCheckRuleRecord_Impl(query,projectDatabase)),
        projectDatabase)
{
  BOOST_ASSERT(getImpl<detail::StandardsAttributeCheckRuleRecord_Impl>());
}

StandardsAttributeCheckRuleRecord::StandardsAttributeCheckRuleRecord(
        boost::shared_ptr<detail::StandardsAttributeCheckRuleRecord_Impl> impl, 
        ProjectDatabase projectDatabase)
  : StandardsRuleRecord(impl,projectDatabase)
{
  BOOST_ASSERT(getImpl<detail::StandardsAttributeCheckRuleRecord_Impl>());
}
          
boost::optional<StandardsAttributeCheckRuleRecord> StandardsAttributeCheckRuleRecord::factoryFromQuery(const QSqlQuery& query, 
                                                                 ProjectDatabase& database)
{
  OptionalStandardsAttributeCheckRuleRecord result;
  try {
    result = StandardsAttributeCheckRuleRecord(query,database);
  }
  catch (const std::exception& e) {
    LOG(Error,"Unable to construct StandardsAttributeCheckRuleRecord from query, because '" 
        << e.what() << "'.");
  }
  return result;
}

std::vector<StandardsAttributeCheckRuleRecord> StandardsAttributeCheckRuleRecord::getStandardsAttributeCheckRuleRecords(
    ProjectDatabase& projectDatabase)
{
  std::vector<StandardsAttributeCheckRuleRecord> result;

  QSqlQuery query(*(projectDatabase.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + RuleRecord::databaseTableName() + " WHERE ruleRecordType=:ruleRecordType"));
  query.bindValue(":ruleRecordType", RuleRecordType::StandardsAttributeCheckRuleRecord);
  assertExec(query);
  while (query.next()) {
    result.push_back(StandardsAttributeCheckRuleRecord(query, projectDatabase));
  }

  return result;
}
    
boost::optional<StandardsAttributeCheckRuleRecord> StandardsAttributeCheckRuleRecord::getStandardsAttributeCheckRuleRecord(
        int id, ProjectDatabase& projectDatabase)
{
  boost::optional<StandardsAttributeCheckRuleRecord> result;

  QSqlQuery query(*(projectDatabase.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + RuleRecord::databaseTableName() + " WHERE id=:id AND ruleRecordType=:ruleRecordType"));
  query.bindValue(":id", id);
  query.bindValue(":ruleRecordType", RuleRecordType::StandardsAttributeCheckRuleRecord);
  assertExec(query);
  if (query.first()) {
    result = StandardsAttributeCheckRuleRecord(query, projectDatabase);
  }

  return result;
}

StandardsAttributeCheckRuleRecord::StandardsAttributeCheckRuleRecord(
        boost::shared_ptr<detail::StandardsAttributeCheckRuleRecord_Impl> impl)
  : StandardsRuleRecord(impl)
{}

} // project
} // openstudio
