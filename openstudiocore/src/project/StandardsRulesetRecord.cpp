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

#include <project/StandardsRulesetRecord.hpp>
#include <project/StandardsRulesetRecord_Impl.hpp>
#include <project/StandardsRuleRecord.hpp>
#include <project/StandardsRuleRecord_Impl.hpp>
#include <project/StandardsAttributeCheckRuleRecord.hpp>
#include <project/StandardsAttributeCheckRuleRecord_Impl.hpp>
#include <project/JoinRecord.hpp>
#include <project/ProjectDatabase.hpp>
#include <project/Ruleset_Rule_JoinRecord.hpp>
#include <project/Ruleset_Rule_JoinRecord_Impl.hpp>
#include <project/StandardsRulesetOptionRecord.hpp>

#include <ruleset/StandardsRulesetOption.hpp>
#include <ruleset/StandardsRuleset.hpp>
#include <ruleset/StandardsRule.hpp>
#include <ruleset/StandardsAttributeCheckRule.hpp>
#include <ruleset/StandardsAttributeCheckRule_Impl.hpp>

#include <utilities/core/Assert.hpp>

#include <boost/optional/optional.hpp>
#include <boost/foreach.hpp>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

using openstudio::ruleset::StandardsRule;
using openstudio::ruleset::StandardsRuleset;
using openstudio::ruleset::StandardsRulesetOption;

namespace openstudio {
namespace project {
  namespace detail {

    StandardsRulesetRecord_Impl::StandardsRulesetRecord_Impl(const openstudio::ruleset::StandardsRuleset& standardsRuleset, const ProjectDatabase& projectDatabase)
      : RulesetRecord_Impl(standardsRuleset, RulesetRecordType::StandardsRulesetRecord, projectDatabase)
    {}

    StandardsRulesetRecord_Impl::StandardsRulesetRecord_Impl(const QSqlQuery& query, const ProjectDatabase& projectDatabase)
      : RulesetRecord_Impl(query, projectDatabase)
    {}

    openstudio::ruleset::StandardsRuleset StandardsRulesetRecord_Impl::standardsRuleset() const
    {
      std::vector<StandardsRulesetOptionRecord> standardsRulesetOptionRecords = this->standardsRulesetOptionRecords();
      std::vector<StandardsRulesetOption> standardsRulesetOptions;
      BOOST_FOREACH(const StandardsRulesetOptionRecord& standardsRulesetOptionRecord, standardsRulesetOptionRecords){
        standardsRulesetOptions.push_back(standardsRulesetOptionRecord.standardsRulesetOption());
      }

      std::vector<StandardsRuleRecord> standardsRuleRecords = this->standardsRuleRecords();
      std::vector<StandardsRule> standardsRules;
      BOOST_FOREACH(const StandardsRuleRecord& standardsRuleRecord, standardsRuleRecords){
        standardsRules.push_back(standardsRuleRecord.standardsRule());
      }
      
      StandardsRuleset result(this->name(), this->handle(), this->uuidLast(), standardsRulesetOptions, standardsRules);
      return result;
    }

    std::vector<StandardsRulesetOptionRecord> StandardsRulesetRecord_Impl::standardsRulesetOptionRecords() const
    {
      std::vector<StandardsRulesetOptionRecord> result;

      ProjectDatabase projectDatabase = this->projectDatabase();

      QSqlQuery query(*(projectDatabase.qSqlDatabase()));
      query.prepare(toQString("SELECT * FROM " + RulesetOptionRecord::databaseTableName() + " WHERE rulesetRecordId=:id AND \
                               rulesetOptionRecordType=:rulesetOptionRecordType"));
      query.bindValue(":id", this->id());
      query.bindValue(":rulesetOptionRecordType", RulesetOptionRecordType::StandardsRulesetOptionRecord);
      assertExec(query);
      while (query.next()) {
        boost::optional<StandardsRulesetOptionRecord> standardsRulesetOptionRecord = StandardsRulesetOptionRecord::factoryFromQuery(query, projectDatabase);
        BOOST_ASSERT(standardsRulesetOptionRecord);
        result.push_back(*standardsRulesetOptionRecord);
      }

      return result;
    }

    std::vector<StandardsRuleRecord> StandardsRulesetRecord_Impl::standardsRuleRecords() const
    {
      std::vector<StandardsRuleRecord> result;
      std::vector<RuleRecord> ruleRecords = Ruleset_Rule_JoinRecord::getRuleRecords(this->getObject<RulesetRecord>());
      BOOST_FOREACH(const RuleRecord& ruleRecord, ruleRecords){
        BOOST_ASSERT(ruleRecord.optionalCast<StandardsRuleRecord>());
        result.push_back(ruleRecord.cast<StandardsRuleRecord>());
      }

      return result;
    }

    boost::optional<ObjectRecord> StandardsRulesetRecord_Impl::parent() const
    {
      return boost::none;
    }

    std::vector<ObjectRecord> StandardsRulesetRecord_Impl::children() const
    {
      std::vector<StandardsRulesetOptionRecord> standardsRulesetOptionRecords = this->standardsRulesetOptionRecords();
      return std::vector<ObjectRecord>(standardsRulesetOptionRecords.begin(), standardsRulesetOptionRecords.end());
    }

    std::vector<ObjectRecord> StandardsRulesetRecord_Impl::resources() const
    {
      std::vector<StandardsRuleRecord> result = this->standardsRuleRecords();
      return std::vector<ObjectRecord>(result.begin(), result.end());
    }

    std::vector<JoinRecord> StandardsRulesetRecord_Impl::joinRecords() const
    {
      std::vector<Ruleset_Rule_JoinRecord> result = JoinRecord::getJoinRecordsForLeftId<Ruleset_Rule_JoinRecord>(this->id(), this->projectDatabase());
      return std::vector<JoinRecord>(result.begin(), result.end());
    }

    void StandardsRulesetRecord_Impl::saveRow(const boost::shared_ptr<QSqlDatabase> &database)
    {
      QSqlQuery query(*database);
      this->makeUpdateByIdQuery<StandardsRulesetRecord>(query);
      this->bindValues(query);
      assertExec(query);
    }

    void StandardsRulesetRecord_Impl::bindValues(QSqlQuery& query) const
    {
      RulesetRecord_Impl::bindValues(query);
    }

    void StandardsRulesetRecord_Impl::setLastValues(const QSqlQuery& query, ProjectDatabase& projectDatabase)
    {
      RulesetRecord_Impl::setLastValues(query, projectDatabase);
    }

    bool StandardsRulesetRecord_Impl::compareValues(const QSqlQuery& query) const
    {
      bool result = true;

      result = result && RulesetRecord_Impl::compareValues(query);

      return result;
    }

    void StandardsRulesetRecord_Impl::saveLastValues()
    {
      RulesetRecord_Impl::saveLastValues();
    }

    void StandardsRulesetRecord_Impl::revertToLastValues()
    {
      RulesetRecord_Impl::revertToLastValues();
    }

  } // detail

  StandardsRulesetRecord::StandardsRulesetRecord(const openstudio::ruleset::StandardsRuleset& standardsRuleset, ProjectDatabase& projectDatabase)
    : RulesetRecord(boost::shared_ptr<detail::StandardsRulesetRecord_Impl>(new detail::StandardsRulesetRecord_Impl(standardsRuleset, projectDatabase)), projectDatabase)
  {
    BOOST_ASSERT(getImpl<detail::StandardsRulesetRecord_Impl>());

    // need to do in wrapper so we can create join records and with non-const database
    StandardsRulesetRecord other(getImpl<detail::StandardsRulesetRecord_Impl>());

    // add all the options
    BOOST_FOREACH(const openstudio::ruleset::StandardsRulesetOption& option, standardsRuleset.optionsNoClone()){
      StandardsRulesetOptionRecord::factoryFromOption(option, other);
    }

    // add all the rules
    BOOST_FOREACH(const openstudio::ruleset::StandardsRule& rule, standardsRuleset.rules()){
      ruleset::OptionalStandardsAttributeCheckRule oAttributeCheckRule = rule.optionalCast<ruleset::StandardsAttributeCheckRule>();
      if (oAttributeCheckRule) {
        ruleset::StandardsAttributeCheckRule acRule = *oAttributeCheckRule;
        StandardsAttributeCheckRuleRecord standardsAttributeCheckRuleRecord(acRule, projectDatabase);
        if (!Ruleset_Rule_JoinRecord::getJoinRecord<Ruleset_Rule_JoinRecord>(this->id(), standardsAttributeCheckRuleRecord.id(), projectDatabase)){
          Ruleset_Rule_JoinRecord join(other, standardsAttributeCheckRuleRecord);
        }
      }
      else {
        StandardsRuleRecord standardsRuleRecord(rule, projectDatabase);
        if (!Ruleset_Rule_JoinRecord::getJoinRecord<Ruleset_Rule_JoinRecord>(this->id(), standardsRuleRecord.id(), projectDatabase)){
          Ruleset_Rule_JoinRecord join(other, standardsRuleRecord);
        }
      }
    }
  }

  StandardsRulesetRecord::StandardsRulesetRecord(const QSqlQuery& query, ProjectDatabase& projectDatabase)
    : RulesetRecord(boost::shared_ptr<detail::StandardsRulesetRecord_Impl>(new detail::StandardsRulesetRecord_Impl(query, projectDatabase)), projectDatabase)
  {
    BOOST_ASSERT(getImpl<detail::StandardsRulesetRecord_Impl>());
  }

  StandardsRulesetRecord::StandardsRulesetRecord(boost::shared_ptr<detail::StandardsRulesetRecord_Impl> impl, ProjectDatabase projectDatabase)
    : RulesetRecord(impl, projectDatabase)
  {
    BOOST_ASSERT(getImpl<detail::StandardsRulesetRecord_Impl>());
  }

  StandardsRulesetRecord::StandardsRulesetRecord(boost::shared_ptr<detail::StandardsRulesetRecord_Impl> impl)
    : RulesetRecord(impl)
  {
    BOOST_ASSERT(getImpl<detail::StandardsRulesetRecord_Impl>());
  }

  boost::optional<StandardsRulesetRecord> StandardsRulesetRecord::factoryFromQuery(const QSqlQuery& query, 
                                                                   ProjectDatabase& database)
  {
    OptionalStandardsRulesetRecord result;
    try {
      result = StandardsRulesetRecord(query,database);
    }
    catch (const std::exception& e) {
      LOG(Error,"Unable to construct StandardsRulesetRecord from query, because '" 
          << e.what() << "'.");
    }
    return result;
  }

  std::vector<StandardsRulesetRecord> StandardsRulesetRecord::getStandardsRulesetRecords(ProjectDatabase& projectDatabase)
  {
    std::vector<StandardsRulesetRecord> result;

    QSqlQuery query(*(projectDatabase.qSqlDatabase()));
    query.prepare(toQString("SELECT * FROM " + RulesetRecord::databaseTableName() + " WHERE rulesetRecordType=:rulesetRecordType"));
    query.bindValue(":rulesetRecordType", RulesetRecordType::StandardsRulesetRecord);
    assertExec(query);
    while (query.next()) {
      result.push_back(StandardsRulesetRecord(query, projectDatabase));
    }

    return result;
  }
  
  boost::optional<StandardsRulesetRecord> StandardsRulesetRecord::getStandardsRulesetRecord(int id, ProjectDatabase& projectDatabase)
  {
    boost::optional<StandardsRulesetRecord> result;

    QSqlQuery query(*(projectDatabase.qSqlDatabase()));
    query.prepare(toQString("SELECT * FROM " + RulesetRecord::databaseTableName() + " WHERE id=:id AND rulesetRecordType=:rulesetRecordType"));
    query.bindValue(":id", id);
    query.bindValue(":rulesetRecordType", RulesetRecordType::StandardsRulesetRecord);
    assertExec(query);
    if (query.first()) {
      result = StandardsRulesetRecord(query, projectDatabase);
    }

    return result;
  }

  openstudio::ruleset::StandardsRuleset StandardsRulesetRecord::standardsRuleset() const
  {
    return getImpl<detail::StandardsRulesetRecord_Impl>()->standardsRuleset();
  }

  std::vector<StandardsRulesetOptionRecord> StandardsRulesetRecord::standardsRulesetOptionRecords() const
  {
    return getImpl<detail::StandardsRulesetRecord_Impl>()->standardsRulesetOptionRecords();
  }

  std::vector<StandardsRuleRecord> StandardsRulesetRecord::standardsRuleRecords() const
  {
    return getImpl<detail::StandardsRulesetRecord_Impl>()->standardsRuleRecords();
  }

} // project
} // openstudio
