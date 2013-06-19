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

#include <project/Ruleset_Rule_JoinRecord.hpp>
#include <project/Ruleset_Rule_JoinRecord_Impl.hpp>
#include <project/RulesetRecord.hpp>
#include <project/RulesetRecord_Impl.hpp>
#include <project/RuleRecord.hpp>
#include <project/RuleRecord_Impl.hpp>
#include <project/ProjectDatabase.hpp>

#include <utilities/core/Assert.hpp>

#include <boost/optional/optional.hpp>
#include <boost/foreach.hpp>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

namespace openstudio {
namespace project {
  namespace detail {

    Ruleset_Rule_JoinRecord_Impl::Ruleset_Rule_JoinRecord_Impl(const RulesetRecord& rulesetRecord, const RuleRecord& ruleRecord)
      : JoinRecord_Impl(rulesetRecord.id(), rulesetRecord.handle(), ruleRecord.id(), ruleRecord.handle(), rulesetRecord.projectDatabase())
    {
      BOOST_ASSERT(rulesetRecord.projectDatabase().handle() == ruleRecord.projectDatabase().handle());
    }

    Ruleset_Rule_JoinRecord_Impl::Ruleset_Rule_JoinRecord_Impl(const QSqlQuery& query, const ProjectDatabase& database)
      : JoinRecord_Impl(query, database)
    {
    }

    ObjectRecord Ruleset_Rule_JoinRecord_Impl::leftObject() const
    {
      return this->rulesetRecord();
    }

    ObjectRecord Ruleset_Rule_JoinRecord_Impl::rightObject() const
    {
      return this->ruleRecord();
    }

    RulesetRecord Ruleset_Rule_JoinRecord_Impl::rulesetRecord() const
    {
      ProjectDatabase projectDatabase = this->projectDatabase();
      boost::optional<RulesetRecord> rulesetRecord = RulesetRecord::getRulesetRecord(this->leftId(), projectDatabase);
      BOOST_ASSERT(rulesetRecord);
      return *rulesetRecord;
    }

    RuleRecord Ruleset_Rule_JoinRecord_Impl::ruleRecord() const
    {
      ProjectDatabase projectDatabase = this->projectDatabase();
      boost::optional<RuleRecord> ruleRecord = RuleRecord::getRuleRecord(this->rightId(), projectDatabase);
      BOOST_ASSERT(ruleRecord);
      return *ruleRecord;
    }
    
    std::string Ruleset_Rule_JoinRecord_Impl::databaseTableName() const
    {
      return Ruleset_Rule_JoinRecord::databaseTableName();
    }
    
    void Ruleset_Rule_JoinRecord_Impl::saveRow(const boost::shared_ptr<QSqlDatabase> &database)
    {
      QSqlQuery query(*database);
      this->makeUpdateByIdQuery<Ruleset_Rule_JoinRecord>(query);
      this->bindValues(query);
      assertExec(query);
    }

  } // detail

  std::string Ruleset_Rule_JoinRecord::databaseTableName()
  {
    return "Ruleset_Rule_JoinRecords";
  }

  UpdateByIdQueryData Ruleset_Rule_JoinRecord::updateByIdQueryData() {
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

  Ruleset_Rule_JoinRecord::Ruleset_Rule_JoinRecord(const RulesetRecord& rulesetRecord, const RuleRecord& ruleRecord)
    : JoinRecord(boost::shared_ptr<detail::Ruleset_Rule_JoinRecord_Impl>(new detail::Ruleset_Rule_JoinRecord_Impl(rulesetRecord, ruleRecord)), rulesetRecord.projectDatabase()) 
  {
    BOOST_ASSERT(getImpl<detail::Ruleset_Rule_JoinRecord_Impl>());
  }

  Ruleset_Rule_JoinRecord::Ruleset_Rule_JoinRecord(boost::shared_ptr<detail::Ruleset_Rule_JoinRecord_Impl> impl, ProjectDatabase projectDatabase)
    : JoinRecord(impl, projectDatabase)
  {
    BOOST_ASSERT(getImpl<detail::Ruleset_Rule_JoinRecord_Impl>());
  }  
  
  Ruleset_Rule_JoinRecord::Ruleset_Rule_JoinRecord(boost::shared_ptr<detail::Ruleset_Rule_JoinRecord_Impl> impl)
    : JoinRecord(impl)
  {
    BOOST_ASSERT(getImpl<detail::Ruleset_Rule_JoinRecord_Impl>());
  }

  std::vector<RulesetRecord> Ruleset_Rule_JoinRecord::getRulesetRecords(const RuleRecord& ruleRecord)
  {
    ProjectDatabase database = ruleRecord.projectDatabase();

    std::vector<RulesetRecord> result;

    // first get RulesetRecord ids
    std::vector<int> rulesetRecordIds;

    QSqlQuery query(*(database.qSqlDatabase()));
    query.prepare(toQString("SELECT * FROM " + Ruleset_Rule_JoinRecord::databaseTableName() + " WHERE rightId=:rightId"));
    query.bindValue(":rightId", ruleRecord.id());
    assertExec(query);
    while (query.next()) {
      QVariant value;
      value = query.value(JoinRecordColumns::leftId);
      BOOST_ASSERT(value.isValid() && !value.isNull());
      rulesetRecordIds.push_back(value.toInt());
    }

    // now get RulesetRecords
    BOOST_FOREACH(int id, rulesetRecordIds){
      boost::optional<RulesetRecord> rulesetRecord = RulesetRecord::getRulesetRecord(id, database);
      BOOST_ASSERT(rulesetRecord);
      result.push_back(*rulesetRecord);
    }

    return result;
  }

  std::vector<RuleRecord> Ruleset_Rule_JoinRecord::getRuleRecords(const RulesetRecord& rulesetRecord)
  {
    ProjectDatabase database = rulesetRecord.projectDatabase();

    std::vector<RuleRecord> result;

    // first get RuleRecord ids
    std::vector<int> ruleRecordIds;

    QSqlQuery query(*(database.qSqlDatabase()));
    query.prepare(toQString("SELECT * FROM " + Ruleset_Rule_JoinRecord::databaseTableName() + " WHERE leftId=:leftId"));
    query.bindValue(":leftId", rulesetRecord.id());
    assertExec(query);
    while (query.next()) {
      QVariant value;
      value = query.value(JoinRecordColumns::rightId);
      BOOST_ASSERT(value.isValid() && !value.isNull());
      ruleRecordIds.push_back(value.toInt());
    }

    // now get RuleRecords
    BOOST_FOREACH(int id, ruleRecordIds){
      boost::optional<RuleRecord> ruleRecord = RuleRecord::getRuleRecord(id, database);
      BOOST_ASSERT(ruleRecord);
      result.push_back(*ruleRecord);
    }

    return result;
  }

  RulesetRecord Ruleset_Rule_JoinRecord::rulesetRecord() const
  {
    return this->getImpl<detail::Ruleset_Rule_JoinRecord_Impl>()->rulesetRecord();
  }

  RuleRecord Ruleset_Rule_JoinRecord::ruleRecord() const
  {
    return this->getImpl<detail::Ruleset_Rule_JoinRecord_Impl>()->ruleRecord();
  }

} // project
} // openstudio
