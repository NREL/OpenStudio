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

#include <project/Rule_Clause_JoinRecord.hpp>
#include <project/Rule_Clause_JoinRecord_Impl.hpp>
#include <project/RuleRecord.hpp>
#include <project/RuleRecord_Impl.hpp>
#include <project/ClauseRecord.hpp>
#include <project/ClauseRecord_Impl.hpp>
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

    Rule_Clause_JoinRecord_Impl::Rule_Clause_JoinRecord_Impl(const RuleRecord& ruleRecord, const ClauseRecord& clauseRecord)
      : JoinRecord_Impl(ruleRecord.id(), ruleRecord.handle(), clauseRecord.id(), clauseRecord.handle(), ruleRecord.projectDatabase())
    {
      OS_ASSERT(ruleRecord.projectDatabase().handle() == clauseRecord.projectDatabase().handle());
    }

    Rule_Clause_JoinRecord_Impl::Rule_Clause_JoinRecord_Impl(const QSqlQuery& query, const ProjectDatabase& database)
      : JoinRecord_Impl(query, database)
    {
    }

    ObjectRecord Rule_Clause_JoinRecord_Impl::leftObject() const
    {
      return this->ruleRecord();
    }

    ObjectRecord Rule_Clause_JoinRecord_Impl::rightObject() const
    {
      return this->clauseRecord();
    }

    RuleRecord Rule_Clause_JoinRecord_Impl::ruleRecord() const
    {
      ProjectDatabase projectDatabase = this->projectDatabase();
      boost::optional<RuleRecord> ruleRecord = RuleRecord::getRuleRecord(this->leftId(), projectDatabase);
      OS_ASSERT(ruleRecord);
      return *ruleRecord;
    }

    ClauseRecord Rule_Clause_JoinRecord_Impl::clauseRecord() const
    {
      ProjectDatabase projectDatabase = this->projectDatabase();
      boost::optional<ClauseRecord> clauseRecord = ClauseRecord::getClauseRecord(this->rightId(), projectDatabase);
      OS_ASSERT(clauseRecord);
      return *clauseRecord;
    }
    
    std::string Rule_Clause_JoinRecord_Impl::databaseTableName() const
    {
      return Rule_Clause_JoinRecord::databaseTableName();
    }
    
    void Rule_Clause_JoinRecord_Impl::saveRow(const boost::shared_ptr<QSqlDatabase> &database)
    {
      QSqlQuery query(*database);
      this->makeUpdateByIdQuery<Rule_Clause_JoinRecord>(query);
      this->bindValues(query);
      assertExec(query);
    }

  } // detail

  std::string Rule_Clause_JoinRecord::databaseTableName()
  {
    return "Rule_Clause_JoinRecords";
  }

  UpdateByIdQueryData Rule_Clause_JoinRecord::updateByIdQueryData() {
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
        OS_ASSERT(*it == expectedValue);
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

  Rule_Clause_JoinRecord::Rule_Clause_JoinRecord(const RuleRecord& ruleRecord, const ClauseRecord& clauseRecord)
    : JoinRecord(boost::shared_ptr<detail::Rule_Clause_JoinRecord_Impl>(new detail::Rule_Clause_JoinRecord_Impl(ruleRecord, clauseRecord)), ruleRecord.projectDatabase()) 
  {
    OS_ASSERT(getImpl<detail::Rule_Clause_JoinRecord_Impl>());
  }

  Rule_Clause_JoinRecord::Rule_Clause_JoinRecord(boost::shared_ptr<detail::Rule_Clause_JoinRecord_Impl> impl, ProjectDatabase projectDatabase)
    : JoinRecord(impl, projectDatabase)
  {
    OS_ASSERT(getImpl<detail::Rule_Clause_JoinRecord_Impl>());
  }  
  
  Rule_Clause_JoinRecord::Rule_Clause_JoinRecord(boost::shared_ptr<detail::Rule_Clause_JoinRecord_Impl> impl)
    : JoinRecord(impl)
  {
    OS_ASSERT(getImpl<detail::Rule_Clause_JoinRecord_Impl>());
  }

  std::vector<RuleRecord> Rule_Clause_JoinRecord::getRuleRecords(const ClauseRecord& clauseRecord)
  {
    ProjectDatabase database = clauseRecord.projectDatabase();

    std::vector<RuleRecord> result;

    // first get RuleRecord ids
    std::vector<int> ruleRecordIds;

    QSqlQuery query(*(database.qSqlDatabase()));
    query.prepare(toQString("SELECT * FROM " + Rule_Clause_JoinRecord::databaseTableName() + " WHERE rightId=:rightId"));
    query.bindValue(":rightId", clauseRecord.id());
    assertExec(query);
    while (query.next()) {
      QVariant value;
      value = query.value(JoinRecordColumns::leftId);
      OS_ASSERT(value.isValid() && !value.isNull());
      ruleRecordIds.push_back(value.toInt());
    }

    // now get RuleRecords
    BOOST_FOREACH(int id, ruleRecordIds){
      boost::optional<RuleRecord> ruleRecord = RuleRecord::getRuleRecord(id, database);
      OS_ASSERT(ruleRecord);
      result.push_back(*ruleRecord);
    }

    return result;
  }

  std::vector<ClauseRecord> Rule_Clause_JoinRecord::getClauseRecords(const RuleRecord& ruleRecord)
  {
    ProjectDatabase database = ruleRecord.projectDatabase();

    std::vector<ClauseRecord> result;

    // first get ClauseRecord ids
    std::vector<int> clauseRecordIds;

    QSqlQuery query(*(database.qSqlDatabase()));
    query.prepare(toQString("SELECT * FROM " + Rule_Clause_JoinRecord::databaseTableName() + " WHERE leftId=:leftId"));
    query.bindValue(":leftId", ruleRecord.id());
    assertExec(query);
    while (query.next()) {
      QVariant value;
      value = query.value(JoinRecordColumns::rightId);
      OS_ASSERT(value.isValid() && !value.isNull());
      clauseRecordIds.push_back(value.toInt());
    }

    // now get ClauseRecords
    BOOST_FOREACH(int id, clauseRecordIds){
      boost::optional<ClauseRecord> clauseRecord = ClauseRecord::getClauseRecord(id, database);
      OS_ASSERT(clauseRecord);
      result.push_back(*clauseRecord);
    }

    return result;
  }

  RuleRecord Rule_Clause_JoinRecord::ruleRecord() const
  {
    return this->getImpl<detail::Rule_Clause_JoinRecord_Impl>()->ruleRecord();
  }

  ClauseRecord Rule_Clause_JoinRecord::clauseRecord() const
  {
    return this->getImpl<detail::Rule_Clause_JoinRecord_Impl>()->clauseRecord();
  }

} // project
} // openstudio
