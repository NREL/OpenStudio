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

#include <project/ModelRuleRecord.hpp>
#include <project/ModelRuleRecord_Impl.hpp>
#include <project/FilterClauseRecord.hpp>
#include <project/FilterClauseRecord_Impl.hpp>
#include <project/ActionClauseRecord.hpp>
#include <project/ActionClauseRecord_Impl.hpp>
#include <project/ProjectDatabase.hpp>
#include <project/Rule_Clause_JoinRecord.hpp>
#include <project/Rule_Clause_JoinRecord_Impl.hpp>
#include <project/Ruleset_Rule_JoinRecord.hpp>
#include <project/Ruleset_Rule_JoinRecord_Impl.hpp>

#include <ruleset/ModelRule.hpp>
#include <ruleset/FilterClause.hpp>
#include <ruleset/ActionClause.hpp>

#include <utilities/core/Assert.hpp>

#include <boost/optional/optional.hpp>
#include <boost/foreach.hpp>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

using openstudio::ruleset::FilterClause;
using openstudio::ruleset::ActionClause;

namespace openstudio {
namespace project {
  namespace detail {

    ModelRuleRecord_Impl::ModelRuleRecord_Impl(const openstudio::ruleset::ModelRule& modelRule, const ProjectDatabase& projectDatabase)
      : RuleRecord_Impl(modelRule, RuleRecordType::ModelRuleRecord, projectDatabase)
    {
      LOG(Trace,"Constructing ModelRuleRecord_Impl for " << modelRule.name() << ".");
    }

    ModelRuleRecord_Impl::ModelRuleRecord_Impl(const QSqlQuery& query, const ProjectDatabase& projectDatabase)
      : RuleRecord_Impl(query, projectDatabase)
    {
    }

    openstudio::ruleset::ModelRule ModelRuleRecord_Impl::modelRule() const
    {
      std::vector<FilterClause> filters;
      std::vector<ActionClause> actions;
      std::vector<ClauseRecord> clauseRecords = this->clauseRecords();
      BOOST_FOREACH(const ClauseRecord& clauseRecord, clauseRecords){
        if (OptionalFilterClauseRecord fcr = clauseRecord.optionalCast<FilterClauseRecord>()) {
          FilterClause filterClause = fcr->filterClause();
          filters.push_back(filterClause);
        }
        else if (OptionalActionClauseRecord acr = clauseRecord.optionalCast<ActionClauseRecord>()) {
          ActionClause actionClause = acr->actionClause();
          actions.push_back(actionClause);
        }
      }

      openstudio::ruleset::ModelRule result(this->name(), 
                                            this->active(),
                                            this->handle(), 
                                            this->uuidLast(), 
                                            filters, 
                                            actions);
      
      return result;    
    }

    boost::optional<ObjectRecord> ModelRuleRecord_Impl::parent() const
    {
      return boost::none;
    }

    std::vector<ObjectRecord> ModelRuleRecord_Impl::children() const
    {
      return std::vector<ObjectRecord>();
    }

    std::vector<ObjectRecord> ModelRuleRecord_Impl::resources() const
    {
      std::vector<ClauseRecord> result = this->clauseRecords();
      return std::vector<ObjectRecord>(result.begin(), result.end());
    }

    std::vector<JoinRecord> ModelRuleRecord_Impl::joinRecords() const
    {
      std::vector<JoinRecord> result;
      std::vector<Rule_Clause_JoinRecord> clauseJoins = JoinRecord::getJoinRecordsForLeftId<Rule_Clause_JoinRecord>(this->id(), this->projectDatabase());
      result.insert(result.end(), clauseJoins.begin(), clauseJoins.end());
      std::vector<Ruleset_Rule_JoinRecord> rulesetJoins = JoinRecord::getJoinRecordsForRightId<Ruleset_Rule_JoinRecord>(this->id(), this->projectDatabase());
      result.insert(result.end(), rulesetJoins.begin(), rulesetJoins.end());
      return result;
    }

    void ModelRuleRecord_Impl::saveRow(const boost::shared_ptr<QSqlDatabase> &database)
    {
      QSqlQuery query(*database);
      this->makeUpdateByIdQuery<ModelRuleRecord>(query);
      this->bindValues(query);
      assertExec(query);
    }

    void ModelRuleRecord_Impl::bindValues(QSqlQuery& query) const
    {
      RuleRecord_Impl::bindValues(query);
    }

    void ModelRuleRecord_Impl::setLastValues(const QSqlQuery& query, ProjectDatabase& projectDatabase)
    {
      RuleRecord_Impl::setLastValues(query, projectDatabase);
    }

    bool ModelRuleRecord_Impl::compareValues(const QSqlQuery& query) const
    {
      bool result = true;

      result = result && RuleRecord_Impl::compareValues(query);

      return result;
    }

    void ModelRuleRecord_Impl::saveLastValues()
    {
      RuleRecord_Impl::saveLastValues();
    }

    void ModelRuleRecord_Impl::revertToLastValues()
    {
      RuleRecord_Impl::revertToLastValues();
    }

  } // detail

  ModelRuleRecord::ModelRuleRecord(const openstudio::ruleset::ModelRule& modelRule, ProjectDatabase& projectDatabase)
    : RuleRecord(modelRule, boost::shared_ptr<detail::ModelRuleRecord_Impl>(new detail::ModelRuleRecord_Impl(modelRule, projectDatabase)), projectDatabase)
  {
    LOG(Trace,"Constructing ModelRuleRecord for " << modelRule.name() << ".");
    OS_ASSERT(getImpl<detail::ModelRuleRecord_Impl>());
  }

  ModelRuleRecord::ModelRuleRecord(const QSqlQuery& query, ProjectDatabase& projectDatabase)
    : RuleRecord(boost::shared_ptr<detail::ModelRuleRecord_Impl>(new detail::ModelRuleRecord_Impl(query, projectDatabase)), projectDatabase)
  {
    OS_ASSERT(getImpl<detail::ModelRuleRecord_Impl>());
  }

  ModelRuleRecord::ModelRuleRecord(boost::shared_ptr<detail::ModelRuleRecord_Impl> impl, ProjectDatabase projectDatabase)
    : RuleRecord(impl, projectDatabase)
  {
    OS_ASSERT(getImpl<detail::ModelRuleRecord_Impl>());
  }

  ModelRuleRecord::ModelRuleRecord(boost::shared_ptr<detail::ModelRuleRecord_Impl> impl)
    : RuleRecord(impl)
  {
    OS_ASSERT(getImpl<detail::ModelRuleRecord_Impl>());
  }

  boost::optional<ModelRuleRecord> ModelRuleRecord::factoryFromQuery(
      const QSqlQuery& query, ProjectDatabase& database)
  {
    OptionalModelRuleRecord result;
    try {
      result = ModelRuleRecord(query,database);
    }
    catch (const std::exception& e) {
      LOG(Error,"Unable to construct ModelRuleRecord from query, because '" 
          << e.what() << "'.");
    }
    return result;
  }

  std::vector<ModelRuleRecord> ModelRuleRecord::getModelRuleRecords(ProjectDatabase& projectDatabase)
  {
    std::vector<ModelRuleRecord> result;

    QSqlQuery query(*(projectDatabase.qSqlDatabase()));
    query.prepare(toQString("SELECT * FROM " + RuleRecord::databaseTableName() + " WHERE ruleRecordType=:ruleRecordType"));
    query.bindValue(":ruleRecordType", RuleRecordType::ModelRuleRecord);
    assertExec(query);
    while (query.next()) {
      result.push_back(ModelRuleRecord(query, projectDatabase));
    }

    return result;
  }
  
  boost::optional<ModelRuleRecord> ModelRuleRecord::getModelRuleRecord(int id, ProjectDatabase& projectDatabase)
  {
    boost::optional<ModelRuleRecord> result;

    QSqlQuery query(*(projectDatabase.qSqlDatabase()));
    query.prepare(toQString("SELECT * FROM " + RuleRecord::databaseTableName() + " WHERE id=:id AND ruleRecordType=:ruleRecordType"));
    query.bindValue(":id", id);
    query.bindValue(":ruleRecordType", RuleRecordType::ModelRuleRecord);
    assertExec(query);
    if (query.first()) {
      result = ModelRuleRecord(query, projectDatabase);
    }

    return result;
  }

  openstudio::ruleset::ModelRule ModelRuleRecord::modelRule() const
  {
    return getImpl<detail::ModelRuleRecord_Impl>()->modelRule();
  }

} // project
} // openstudio
