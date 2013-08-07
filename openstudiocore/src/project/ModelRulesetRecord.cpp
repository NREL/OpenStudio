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

#include <project/ModelRulesetRecord.hpp>
#include <project/ModelRulesetRecord_Impl.hpp>
#include <project/ModelRuleRecord.hpp>
#include <project/ModelRuleRecord_Impl.hpp>
#include <project/JoinRecord.hpp>
#include <project/ProjectDatabase.hpp>
#include <project/Ruleset_Rule_JoinRecord.hpp>
#include <project/Ruleset_Rule_JoinRecord_Impl.hpp>

#include <ruleset/ModelRuleset.hpp>
#include <ruleset/ModelRule.hpp>

#include <utilities/core/Assert.hpp>

#include <boost/optional/optional.hpp>
#include <boost/foreach.hpp>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

using openstudio::ruleset::ModelRule;
using openstudio::ruleset::ModelRuleset;

namespace openstudio {
namespace project {
  namespace detail {

    ModelRulesetRecord_Impl::ModelRulesetRecord_Impl(
        const openstudio::ruleset::ModelRuleset& modelRuleset, 
        const ProjectDatabase& projectDatabase)
      : RulesetRecord_Impl(modelRuleset, RulesetRecordType::ModelRulesetRecord, projectDatabase)
    {
    }

    ModelRulesetRecord_Impl::ModelRulesetRecord_Impl(const QSqlQuery& query, const ProjectDatabase& projectDatabase)
      : RulesetRecord_Impl(query, projectDatabase)
    {
    }

    openstudio::ruleset::ModelRuleset ModelRulesetRecord_Impl::modelRuleset() const
    {
      std::vector<ModelRuleRecord> modelRuleRecords = this->modelRuleRecords();
      std::vector<ModelRule> modelRules;
      BOOST_FOREACH(const ModelRuleRecord& modelRuleRecord, modelRuleRecords){
        modelRules.push_back(modelRuleRecord.modelRule());
      }
      ModelRuleset result(this->name(), this->handle(), this->uuidLast(), modelRules);
      return result;
    }

    std::vector<ModelRuleRecord> ModelRulesetRecord_Impl::modelRuleRecords() const
    {
      std::vector<ModelRuleRecord> result;
      std::vector<RuleRecord> ruleRecords = Ruleset_Rule_JoinRecord::getRuleRecords(this->getObject<RulesetRecord>());
      BOOST_FOREACH(const RuleRecord& ruleRecord, ruleRecords){
        OS_ASSERT(ruleRecord.optionalCast<ModelRuleRecord>());
        result.push_back(ruleRecord.cast<ModelRuleRecord>());
      }

      return result;
    }

    boost::optional<ObjectRecord> ModelRulesetRecord_Impl::parent() const
    {
      return boost::none;
    }

    std::vector<ObjectRecord> ModelRulesetRecord_Impl::children() const
    {
      return std::vector<ObjectRecord>();
    }

    std::vector<ObjectRecord> ModelRulesetRecord_Impl::resources() const
    {
      std::vector<ModelRuleRecord> result = this->modelRuleRecords();
      return std::vector<ObjectRecord>(result.begin(), result.end());
    }

    std::vector<JoinRecord> ModelRulesetRecord_Impl::joinRecords() const
    {
      std::vector<Ruleset_Rule_JoinRecord> result = JoinRecord::getJoinRecordsForLeftId<Ruleset_Rule_JoinRecord>(this->id(), this->projectDatabase());
      return std::vector<JoinRecord>(result.begin(), result.end());
    }

    void ModelRulesetRecord_Impl::saveRow(const boost::shared_ptr<QSqlDatabase> &database)
    {
      QSqlQuery query(*database);
      this->makeUpdateByIdQuery<ModelRulesetRecord>(query);
      this->bindValues(query);
      assertExec(query);
    }

    void ModelRulesetRecord_Impl::bindValues(QSqlQuery& query) const
    {
      RulesetRecord_Impl::bindValues(query);
    }

    void ModelRulesetRecord_Impl::setLastValues(const QSqlQuery& query, ProjectDatabase& projectDatabase)
    {
      RulesetRecord_Impl::setLastValues(query, projectDatabase);
    }

    bool ModelRulesetRecord_Impl::compareValues(const QSqlQuery& query) const
    {
      bool result = true;

      result = result && RulesetRecord_Impl::compareValues(query);

      return result;
    }

    void ModelRulesetRecord_Impl::saveLastValues()
    {
      RulesetRecord_Impl::saveLastValues();
    }

    void ModelRulesetRecord_Impl::revertToLastValues()
    {
      RulesetRecord_Impl::revertToLastValues();
    }

  } // detail

  ModelRulesetRecord::ModelRulesetRecord(const openstudio::ruleset::ModelRuleset& modelRuleset, ProjectDatabase& projectDatabase)
    : RulesetRecord(boost::shared_ptr<detail::ModelRulesetRecord_Impl>(new detail::ModelRulesetRecord_Impl(modelRuleset, projectDatabase)), projectDatabase)
  {
    OS_ASSERT(getImpl<detail::ModelRulesetRecord_Impl>());

    // need to do in wrapper so we can create join records and with non-const database
    ModelRulesetRecord other(getImpl<detail::ModelRulesetRecord_Impl>());

    LOG(Trace,"Constructing ModelRulesetRecord for " << modelRuleset.name() << ".");

    // add all the rules
    BOOST_FOREACH(const openstudio::ruleset::ModelRule& rule, modelRuleset.rules()){
      ModelRuleRecord modelRuleRecord(rule, projectDatabase);
      if (!Ruleset_Rule_JoinRecord::getJoinRecord<Ruleset_Rule_JoinRecord>(this->id(), modelRuleRecord.id(), projectDatabase)){
        Ruleset_Rule_JoinRecord join(other, modelRuleRecord);
      }
    }
  }

  ModelRulesetRecord::ModelRulesetRecord(const QSqlQuery& query, ProjectDatabase& projectDatabase)
    : RulesetRecord(boost::shared_ptr<detail::ModelRulesetRecord_Impl>(new detail::ModelRulesetRecord_Impl(query, projectDatabase)), projectDatabase)
  {
    OS_ASSERT(getImpl<detail::ModelRulesetRecord_Impl>());
  }

  ModelRulesetRecord::ModelRulesetRecord(boost::shared_ptr<detail::ModelRulesetRecord_Impl> impl, ProjectDatabase projectDatabase)
    : RulesetRecord(impl, projectDatabase)
  {
    OS_ASSERT(getImpl<detail::ModelRulesetRecord_Impl>());
  }

  ModelRulesetRecord::ModelRulesetRecord(boost::shared_ptr<detail::ModelRulesetRecord_Impl> impl)
    : RulesetRecord(impl)
  {
    OS_ASSERT(getImpl<detail::ModelRulesetRecord_Impl>());
  }

  boost::optional<ModelRulesetRecord> ModelRulesetRecord::factoryFromQuery(
      const QSqlQuery& query, ProjectDatabase& database)
  {
    OptionalModelRulesetRecord result;
    try {
      result = ModelRulesetRecord(query,database);
    }
    catch (const std::exception& e) {
      LOG(Error,"Unable to construct ModelRulesetRecord from query, because '" 
          << e.what() << "'.");
    }
    return result;
  }

  std::vector<ModelRulesetRecord> ModelRulesetRecord::getModelRulesetRecords(ProjectDatabase& projectDatabase)
  {
    std::vector<ModelRulesetRecord> result;

    QSqlQuery query(*(projectDatabase.qSqlDatabase()));
    query.prepare(toQString("SELECT * FROM " + RulesetRecord::databaseTableName() + " WHERE rulesetRecordType=:rulesetRecordType"));
    query.bindValue(":rulesetRecordType", RulesetRecordType::ModelRulesetRecord);
    assertExec(query);
    while (query.next()) {
      result.push_back(ModelRulesetRecord(query, projectDatabase));
    }

    return result;
  }
  
  boost::optional<ModelRulesetRecord> ModelRulesetRecord::getModelRulesetRecord(int id, ProjectDatabase& projectDatabase)
  {
    boost::optional<ModelRulesetRecord> result;

    QSqlQuery query(*(projectDatabase.qSqlDatabase()));
    query.prepare(toQString("SELECT * FROM " + RulesetRecord::databaseTableName() + " WHERE id=:id AND rulesetRecordType=:rulesetRecordType"));
    query.bindValue(":id", id);
    query.bindValue(":rulesetRecordType", RulesetRecordType::ModelRulesetRecord);
    assertExec(query);
    if (query.first()) {
      result = ModelRulesetRecord(query, projectDatabase);
    }

    return result;
  }

  openstudio::ruleset::ModelRuleset ModelRulesetRecord::modelRuleset() const
  {
    return getImpl<detail::ModelRulesetRecord_Impl>()->modelRuleset();
  }

  std::vector<ModelRuleRecord> ModelRulesetRecord::modelRuleRecords() const
  {
    return getImpl<detail::ModelRulesetRecord_Impl>()->modelRuleRecords();
  }

} // project
} // openstudio
