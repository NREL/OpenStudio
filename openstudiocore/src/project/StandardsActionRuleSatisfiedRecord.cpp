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

#include <project/StandardsActionRuleSatisfiedRecord.hpp>
#include <project/StandardsActionRuleSatisfiedRecord_Impl.hpp>
#include <project/JoinRecord.hpp>
#include <project/ProjectDatabase.hpp>
#include <project/Rule_Clause_JoinRecord.hpp>
#include <project/Rule_Clause_JoinRecord_Impl.hpp>
#include <project/StandardsRulesetOptionRecord.hpp>
#include <project/StandardsFilterObjectAttributeRecord.hpp>

#include <ruleset/StandardsActionRuleSatisfied.hpp>
#include <ruleset/StandardsFilterObjectAttribute.hpp>
#include <ruleset/StandardsRulesetOption.hpp>

#include <utilities/core/Assert.hpp>

#include <boost/optional/optional.hpp>
#include <boost/foreach.hpp>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

namespace openstudio {
namespace project {

namespace detail {

  StandardsActionRuleSatisfiedRecord_Impl::StandardsActionRuleSatisfiedRecord_Impl(const openstudio::ruleset::StandardsActionRuleSatisfied& standardsActionRuleSatisfied, const ProjectDatabase& projectDatabase)
    : StandardsActionRuleResultRecord_Impl(standardsActionRuleSatisfied, StandardsActionRuleResultRecordType::StandardsActionRuleSatisfiedRecord, projectDatabase)
  {
  }

  StandardsActionRuleSatisfiedRecord_Impl::StandardsActionRuleSatisfiedRecord_Impl(const QSqlQuery& query, const ProjectDatabase& projectDatabase)
    : StandardsActionRuleResultRecord_Impl(query, projectDatabase)
  {
  }

  openstudio::ruleset::StandardsActionRuleSatisfied StandardsActionRuleSatisfiedRecord_Impl::standardsActionRuleSatisfied() const
  {
    StandardsFilterObjectAttributeRecord standardsFilterObjectAttributeRecord = this->standardsFilterObjectAttributeRecord();
    openstudio::ruleset::StandardsActionRuleSatisfied result(standardsFilterObjectAttributeRecord.standardsFilterObjectAttribute(), this->handle(), this->uuidLast());
    return result;
  }

  openstudio::ruleset::ActionClause StandardsActionRuleSatisfiedRecord_Impl::actionClause() const
  {
    return this->standardsActionRuleSatisfied();
  }

  boost::optional<ObjectRecord> StandardsActionRuleSatisfiedRecord_Impl::parent() const
  {
    return boost::none;
  }

  std::vector<ObjectRecord> StandardsActionRuleSatisfiedRecord_Impl::children() const
  {
    return std::vector<ObjectRecord>();
  }

  std::vector<ObjectRecord> StandardsActionRuleSatisfiedRecord_Impl::resources() const
  {
    StandardsFilterObjectAttributeRecord record = this->standardsFilterObjectAttributeRecord();
    return std::vector<ObjectRecord>(1u,record);
  }

  std::vector<JoinRecord> StandardsActionRuleSatisfiedRecord_Impl::joinRecords() const
  {
    std::vector<Rule_Clause_JoinRecord> result = JoinRecord::getJoinRecordsForRightId<Rule_Clause_JoinRecord>(this->id(), this->projectDatabase());
    return std::vector<JoinRecord>(result.begin(), result.end());
  }

  void StandardsActionRuleSatisfiedRecord_Impl::saveRow(const boost::shared_ptr<QSqlDatabase> &database)
  {
    QSqlQuery query(*database);
    this->makeUpdateByIdQuery<StandardsActionRuleSatisfiedRecord>(query);
    this->bindValues(query);
    assertExec(query);
  }

  void StandardsActionRuleSatisfiedRecord_Impl::bindValues(QSqlQuery& query) const
  {
    StandardsActionRuleResultRecord_Impl::bindValues(query);
  }

  void StandardsActionRuleSatisfiedRecord_Impl::setLastValues(const QSqlQuery& query, ProjectDatabase& projectDatabase)
  {
    StandardsActionRuleResultRecord_Impl::setLastValues(query, projectDatabase);
  }

  bool StandardsActionRuleSatisfiedRecord_Impl::compareValues(const QSqlQuery& query) const
  {
    bool result = true;

    result = result && StandardsActionRuleResultRecord_Impl::compareValues(query);

    return result;
  }

  void StandardsActionRuleSatisfiedRecord_Impl::saveLastValues()
  {
    StandardsActionRuleResultRecord_Impl::saveLastValues();
  }

  void StandardsActionRuleSatisfiedRecord_Impl::revertToLastValues()
  {
    StandardsActionRuleResultRecord_Impl::revertToLastValues();
  }

} // detail

StandardsActionRuleSatisfiedRecord::StandardsActionRuleSatisfiedRecord(const openstudio::ruleset::StandardsActionRuleSatisfied& standardsActionRuleSatisfied, ProjectDatabase& projectDatabase)
  : StandardsActionRuleResultRecord(boost::shared_ptr<detail::StandardsActionRuleSatisfiedRecord_Impl>(new detail::StandardsActionRuleSatisfiedRecord_Impl(standardsActionRuleSatisfied, projectDatabase)), projectDatabase)
{
  BOOST_ASSERT(getImpl<detail::StandardsActionRuleSatisfiedRecord_Impl>());
}

StandardsActionRuleSatisfiedRecord::StandardsActionRuleSatisfiedRecord(const QSqlQuery& query, ProjectDatabase& projectDatabase)
  : StandardsActionRuleResultRecord(boost::shared_ptr<detail::StandardsActionRuleSatisfiedRecord_Impl>(new detail::StandardsActionRuleSatisfiedRecord_Impl(query, projectDatabase)), projectDatabase)
{
  BOOST_ASSERT(getImpl<detail::StandardsActionRuleSatisfiedRecord_Impl>());
}

StandardsActionRuleSatisfiedRecord::StandardsActionRuleSatisfiedRecord(boost::shared_ptr<detail::StandardsActionRuleSatisfiedRecord_Impl> impl, ProjectDatabase projectDatabase)
  : StandardsActionRuleResultRecord(impl, projectDatabase)
{
  BOOST_ASSERT(getImpl<detail::StandardsActionRuleSatisfiedRecord_Impl>());
}

/// @cond
StandardsActionRuleSatisfiedRecord::StandardsActionRuleSatisfiedRecord(boost::shared_ptr<detail::StandardsActionRuleSatisfiedRecord_Impl> impl)
  : StandardsActionRuleResultRecord(impl)
{
  BOOST_ASSERT(getImpl<detail::StandardsActionRuleSatisfiedRecord_Impl>());
}
/// @endcond

boost::optional<StandardsActionRuleSatisfiedRecord> 
StandardsActionRuleSatisfiedRecord::factoryFromQuery(const QSqlQuery& query, 
                                                     ProjectDatabase& database)
{
  OptionalStandardsActionRuleSatisfiedRecord result;
  try {
    result = StandardsActionRuleSatisfiedRecord(query,database);
  }
  catch (const std::exception& e) {
    LOG(Error,"Unable to construct StandardsActionRuleSatisfiedRecord from query, because '" 
        << e.what() << "'.");
  }
  return result;
}

std::vector<StandardsActionRuleSatisfiedRecord> StandardsActionRuleSatisfiedRecord::getStandardsActionRuleSatisfiedRecords(ProjectDatabase& database) {
  std::vector<StandardsActionRuleSatisfiedRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + ClauseRecord::databaseTableName() + 
      " WHERE clauseRecordType=:clauseRecordType" +
      " AND actionClauseRecordType=:actionClauseRecordType" +
      " AND standardsActionClauseRecordType=:standardsActionClauseRecordType" +
      " AND standardsActionRuleResultRecordType=:standardsActionRuleResultRecordType"));
  query.bindValue(":clauseRecordType",ClauseRecordType::ActionClauseRecord);
  query.bindValue(":actionClauseRecordType",ActionClauseRecordType::StandardsActionClauseRecord);
  query.bindValue(":standardsActionClauseRecordType", StandardsActionClauseRecordType::StandardsActionRuleResultRecord);
  query.bindValue(":standardsActionRuleResultRecordType", StandardsActionRuleResultRecordType::StandardsActionRuleSatisfiedRecord);
  assertExec(query);
  while (query.next()) {
    result.push_back(StandardsActionRuleSatisfiedRecord(query, database));
  }

  return result;
}

boost::optional<StandardsActionRuleSatisfiedRecord> StandardsActionRuleSatisfiedRecord::getStandardsActionRuleSatisfiedRecord(int id, ProjectDatabase& database) {
  boost::optional<StandardsActionRuleSatisfiedRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + ClauseRecord::databaseTableName() + 
      " WHERE clauseRecordType=:clauseRecordType" +
      " AND actionClauseRecordType=:actionClauseRecordType" +
      " AND standardsActionClauseRecordType=:standardsActionClauseRecordType" +
      " AND standardsActionRuleResultRecordType=:standardsActionRuleResultRecordType AND id=:id"));
  query.bindValue(":clauseRecordType",ClauseRecordType::ActionClauseRecord);
  query.bindValue(":actionClauseRecordType",ActionClauseRecordType::StandardsActionClauseRecord);
  query.bindValue(":standardsActionClauseRecordType", StandardsActionClauseRecordType::StandardsActionRuleResultRecord);
  query.bindValue(":standardsActionRuleResultRecordType", StandardsActionRuleResultRecordType::StandardsActionRuleSatisfiedRecord);
  query.bindValue(":id",id);
  assertExec(query);
  if (query.first()) {
    result = StandardsActionRuleSatisfiedRecord(query, database);
  }

  return result;
}

openstudio::ruleset::StandardsActionRuleSatisfied StandardsActionRuleSatisfiedRecord::standardsActionRuleSatisfied() const
{
  return this->getImpl<detail::StandardsActionRuleSatisfiedRecord_Impl>()->standardsActionRuleSatisfied();
}

} // project
} // openstudio
