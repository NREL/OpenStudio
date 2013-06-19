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

#include <project/StandardsActionRuleResultRecord.hpp>
#include <project/StandardsActionRuleResultRecord_Impl.hpp>
#include <project/JoinRecord.hpp>
#include <project/ProjectDatabase.hpp>
#include <project/StandardsActionRuleSatisfiedRecord.hpp>
#include <project/StandardsActionRuleViolatedRecord.hpp>
#include <project/StandardsFilterObjectAttributeRecord.hpp>

#include <ruleset/StandardsActionRuleResult.hpp>
#include <ruleset/StandardsActionRuleSatisfied.hpp>
#include <ruleset/StandardsActionRuleSatisfied_Impl.hpp>
#include <ruleset/StandardsActionRuleViolated.hpp>
#include <ruleset/StandardsActionRuleViolated_Impl.hpp>
#include <ruleset/StandardsFilterObjectAttribute.hpp>
#include <ruleset/StandardsFilterObjectAttribute_Impl.hpp>

#include <utilities/core/Assert.hpp>

#include <boost/optional/optional.hpp>
#include <boost/foreach.hpp>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

namespace openstudio {
namespace project {

namespace detail {

  StandardsActionRuleResultRecord_Impl::StandardsActionRuleResultRecord_Impl(const openstudio::ruleset::StandardsActionRuleResult& standardsActionRuleResult, const StandardsActionRuleResultRecordType& StandardsActionRuleResultRecordType, const ProjectDatabase& projectDatabase)
    : StandardsActionClauseRecord_Impl(standardsActionRuleResult, StandardsActionClauseRecordType::StandardsActionRuleResultRecord, projectDatabase), m_standardsActionRuleResultRecordType(StandardsActionRuleResultRecordType)
  {
    ProjectDatabase other = projectDatabase;
    StandardsFilterObjectAttributeRecord standardsFilterObjectAttributeRecord = StandardsFilterObjectAttributeRecord::factoryFromFilter(standardsActionRuleResult.attributeTest(), other);
    m_standardsFilterObjectAttributeRecordId = standardsFilterObjectAttributeRecord.id();

    // how to set 
    m_standardsFilterAvailableModelRecordId = boost::none;
  }

  StandardsActionRuleResultRecord_Impl::StandardsActionRuleResultRecord_Impl(const QSqlQuery& query, const ProjectDatabase& projectDatabase)
    : StandardsActionClauseRecord_Impl(query, projectDatabase)
  {
    BOOST_ASSERT(query.isValid());
    BOOST_ASSERT(query.isSelect());

    QVariant value;
    BOOST_ASSERT(query.isActive());
    value = query.value(ClauseRecordColumns::standardsActionRuleResultRecordType);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    m_standardsActionRuleResultRecordType = StandardsActionRuleResultRecordType(value.toString().toStdString());

    value = query.value(ClauseRecordColumns::standardsFilterObjectAttributeRecordId);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    m_standardsFilterObjectAttributeRecordId = value.toInt();

    value = query.value(ClauseRecordColumns::standardsFilterAvailableModelRecordId);
    if(value.isValid() && !value.isNull()){
      m_standardsFilterAvailableModelRecordId = value.toInt();
    }
  }

  void StandardsActionRuleResultRecord_Impl::bindValues(QSqlQuery& query) const
  {
    StandardsActionClauseRecord_Impl::bindValues(query);

    query.bindValue(ClauseRecordColumns::standardsActionRuleResultRecordType, toQString(m_standardsActionRuleResultRecordType.valueName()));
    
    query.bindValue(ClauseRecordColumns::standardsFilterObjectAttributeRecordId, m_standardsFilterObjectAttributeRecordId);
    
    if (m_standardsFilterAvailableModelRecordId){
      query.bindValue(ClauseRecordColumns::standardsFilterAvailableModelRecordId, *m_standardsFilterAvailableModelRecordId);
    }else{
      query.bindValue(ClauseRecordColumns::standardsFilterAvailableModelRecordId, QVariant(QVariant::Int));
    }
  }

  void StandardsActionRuleResultRecord_Impl::setLastValues(const QSqlQuery& query, ProjectDatabase& projectDatabase)
  {
    BOOST_ASSERT(query.isValid());
    BOOST_ASSERT(query.isActive());
    BOOST_ASSERT(query.isSelect());

    StandardsActionClauseRecord_Impl::setLastValues(query, projectDatabase);

    QVariant value;
    value = query.value(ClauseRecordColumns::standardsActionRuleResultRecordType);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    m_lastStandardsActionRuleResultRecordType = StandardsActionRuleResultRecordType(value.toString().toStdString());

    value = query.value(ClauseRecordColumns::standardsFilterObjectAttributeRecordId);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    m_lastStandardsFilterObjectAttributeRecordId = value.toInt();

    value = query.value(ClauseRecordColumns::standardsFilterAvailableModelRecordId);
    if(value.isValid() && !value.isNull()){
      m_lastStandardsFilterAvailableModelRecordId = value.toInt();
    }else{
      m_lastStandardsFilterAvailableModelRecordId.reset();
    }
  }

  StandardsFilterObjectAttributeRecord StandardsActionRuleResultRecord_Impl::standardsFilterObjectAttributeRecord() const
  {
    ProjectDatabase projectDatabase = this->projectDatabase();
    boost::optional<StandardsFilterObjectAttributeRecord> result = StandardsFilterObjectAttributeRecord::getStandardsFilterObjectAttributeRecord(m_standardsFilterObjectAttributeRecordId, projectDatabase);
    BOOST_ASSERT(result);
    return *result;
  }

  bool StandardsActionRuleResultRecord_Impl::compareValues(const QSqlQuery& query) const
  {
    bool result = true;

    result = result && StandardsActionClauseRecord_Impl::compareValues(query);

    QVariant value;
    value = query.value(ClauseRecordColumns::standardsActionRuleResultRecordType);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_standardsActionRuleResultRecordType == StandardsActionRuleResultRecordType(value.toString().toStdString()));
    
    value = query.value(ClauseRecordColumns::standardsFilterObjectAttributeRecordId);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_standardsFilterObjectAttributeRecordId == value.toInt());

    value = query.value(ClauseRecordColumns::standardsFilterObjectAttributeRecordId);
    if(value.isValid() && !value.isNull()){
      result = result && m_standardsFilterAvailableModelRecordId && (*m_standardsFilterAvailableModelRecordId == value.toInt());
    }else{
      result = result && !m_standardsFilterAvailableModelRecordId;
    }

    return result;
  }

  void StandardsActionRuleResultRecord_Impl::saveLastValues()
  {
    StandardsActionClauseRecord_Impl::saveLastValues();

    m_lastStandardsActionRuleResultRecordType = m_standardsActionRuleResultRecordType; 
    m_lastStandardsFilterObjectAttributeRecordId = m_standardsFilterObjectAttributeRecordId; 
    m_lastStandardsFilterAvailableModelRecordId = m_standardsFilterAvailableModelRecordId; 
  }

  void StandardsActionRuleResultRecord_Impl::revertToLastValues()
  {
    StandardsActionClauseRecord_Impl::revertToLastValues();

    m_standardsActionRuleResultRecordType = m_lastStandardsActionRuleResultRecordType;
    m_standardsFilterObjectAttributeRecordId = m_lastStandardsFilterObjectAttributeRecordId;
    m_standardsFilterAvailableModelRecordId = m_lastStandardsFilterAvailableModelRecordId;
  }

} // detail

StandardsActionRuleResultRecord::StandardsActionRuleResultRecord(boost::shared_ptr<detail::StandardsActionRuleResultRecord_Impl> impl, ProjectDatabase projectDatabase)
  : StandardsActionClauseRecord(impl, projectDatabase)
{
  BOOST_ASSERT(getImpl<detail::StandardsActionRuleResultRecord_Impl>());
}

/// @cond
StandardsActionRuleResultRecord::StandardsActionRuleResultRecord(boost::shared_ptr<detail::StandardsActionRuleResultRecord_Impl> impl)
  : StandardsActionClauseRecord(impl)
{
  BOOST_ASSERT(getImpl<detail::StandardsActionRuleResultRecord_Impl>());
}
/// @endcond

std::vector<StandardsActionRuleResultRecord> StandardsActionRuleResultRecord::getStandardsActionRuleResultRecords(ProjectDatabase& database) {
  std::vector<StandardsActionRuleResultRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + ClauseRecord::databaseTableName() + 
      " WHERE clauseRecordType=:clauseRecordType" +
      " AND actionClauseRecordType=:actionClauseRecordType" +
      " AND standardsActionClauseRecordType=:standardsActionClauseRecordType"));
  query.bindValue(":clauseRecordType",ClauseRecordType::ActionClauseRecord);
  query.bindValue(":actionClauseRecordType",ActionClauseRecordType::StandardsActionClauseRecord);
  query.bindValue(":standardsActionClauseRecordType", StandardsActionClauseRecordType::StandardsActionRuleResultRecord);
  assertExec(query);
  while (query.next()) {
    OptionalStandardsActionRuleResultRecord standardsActionRuleResultRecord = StandardsActionRuleResultRecord::factoryFromQuery(query, database);
    if (standardsActionRuleResultRecord) {
      result.push_back(*standardsActionRuleResultRecord);
    }
  }

  return result;
}

boost::optional<StandardsActionRuleResultRecord> StandardsActionRuleResultRecord::getStandardsActionRuleResultRecord(int id, ProjectDatabase& database) {
  boost::optional<StandardsActionRuleResultRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + ClauseRecord::databaseTableName() + 
      " WHERE clauseRecordType=:clauseRecordType" +
      " AND actionClauseRecordType=:actionClauseRecordType" +
      " AND standardsActionClauseRecordType=:standardsActionClauseRecordType AND id=:id"));
  query.bindValue(":clauseRecordType",ClauseRecordType::ActionClauseRecord);
  query.bindValue(":actionClauseRecordType",ActionClauseRecordType::StandardsActionClauseRecord);
  query.bindValue(":standardsActionClauseRecordType", StandardsActionClauseRecordType::StandardsActionRuleResultRecord);
  query.bindValue(":id",id);
  assertExec(query);
  if (query.first()) {
    result = StandardsActionRuleResultRecord::factoryFromQuery(query, database);
  }

  return result;
}

StandardsActionRuleResultRecord StandardsActionRuleResultRecord::factoryFromAction(const openstudio::ruleset::StandardsActionRuleResult& action, ProjectDatabase& projectDatabase)
{
  if (action.optionalCast<ruleset::StandardsActionRuleSatisfied>()){
    return StandardsActionRuleSatisfiedRecord(action.cast<ruleset::StandardsActionRuleSatisfied>(), projectDatabase);
  }else if (action.optionalCast<ruleset::StandardsActionRuleViolated>()){
    return StandardsActionRuleViolatedRecord(action.cast<ruleset::StandardsActionRuleViolated>(), projectDatabase);
  }

  BOOST_ASSERT(false);
  return StandardsActionRuleResultRecord(boost::shared_ptr<detail::StandardsActionRuleResultRecord_Impl>());
}

boost::optional<StandardsActionRuleResultRecord> StandardsActionRuleResultRecord::factoryFromQuery(const QSqlQuery& query, ProjectDatabase& database)
{
  boost::optional<StandardsActionRuleResultRecord> result;

  int standardsActionRuleResultRecordType = query.value(ClauseRecordColumns::standardsActionRuleResultRecordType).toInt();

  switch (standardsActionRuleResultRecordType){
    case StandardsActionRuleResultRecordType::StandardsActionRuleSatisfiedRecord:
      result = StandardsActionRuleSatisfiedRecord(query, database);
      break;  
    case StandardsActionRuleResultRecordType::StandardsActionRuleViolatedRecord:
      result = StandardsActionRuleViolatedRecord(query, database);
      break;     
    default:
      LOG(Error, "Unknown standardsActionRuleResultRecordType " << standardsActionRuleResultRecordType);
  }

  return result;
}

StandardsFilterObjectAttributeRecord StandardsActionRuleResultRecord::standardsFilterObjectAttributeRecord() const
{
  return this->getImpl<detail::StandardsActionRuleResultRecord_Impl>()->standardsFilterObjectAttributeRecord();
}

} // project
} // openstudio
