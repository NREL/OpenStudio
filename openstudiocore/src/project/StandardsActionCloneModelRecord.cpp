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

#include <project/StandardsActionCloneModelRecord.hpp>
#include <project/StandardsActionCloneModelRecord_Impl.hpp>
#include <project/JoinRecord.hpp>
#include <project/ProjectDatabase.hpp>
#include <project/Rule_Clause_JoinRecord.hpp>
#include <project/Rule_Clause_JoinRecord_Impl.hpp>
#include <project/StandardsFilterAvailableModelRecord.hpp>

#include <ruleset/StandardsActionCloneModel.hpp>
#include <ruleset/StandardsFilterAvailableModel.hpp>

#include <utilities/core/Assert.hpp>

#include <boost/optional/optional.hpp>
#include <boost/foreach.hpp>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

namespace openstudio {
namespace project {

namespace detail {

  StandardsActionCloneModelRecord_Impl::StandardsActionCloneModelRecord_Impl(const openstudio::ruleset::StandardsActionCloneModel& standardsActionCloneModel, const ProjectDatabase& projectDatabase)
    : StandardsActionClauseRecord_Impl(standardsActionCloneModel, StandardsActionClauseRecordType::StandardsActionCloneModelRecord, projectDatabase), 
      m_standardsRulesetModelType(standardsActionCloneModel.modelTypeOfClone())
  {
  }

  StandardsActionCloneModelRecord_Impl::StandardsActionCloneModelRecord_Impl(const QSqlQuery& query, const ProjectDatabase& projectDatabase)
    : StandardsActionClauseRecord_Impl(query, projectDatabase)
  {
    BOOST_ASSERT(query.isValid());
    BOOST_ASSERT(query.isActive());
    BOOST_ASSERT(query.isSelect());

    QVariant value;
    value = query.value(ClauseRecordColumns::standardsFilterAvailableModelId);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    m_standardsFilterAvailableModelId = value.toInt();

    value = query.value(ClauseRecordColumns::standardsRulesetModelType);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    m_standardsRulesetModelType = ruleset::StandardsRulesetModelType(value.toString().toStdString());
  }

  openstudio::ruleset::StandardsActionCloneModel StandardsActionCloneModelRecord_Impl::standardsActionCloneModel() const
  {
    StandardsFilterAvailableModelRecord standardsFilterAvailableModelRecord = this->standardsFilterAvailableModelRecord();
    openstudio::ruleset::StandardsActionCloneModel result(standardsFilterAvailableModelRecord.standardsFilterAvailableModel(), m_standardsRulesetModelType, this->handle(), this->uuidLast());
    return result;
  }
    
  openstudio::ruleset::ActionClause StandardsActionCloneModelRecord_Impl::actionClause() const
  {
    return this->standardsActionCloneModel();
  }

  StandardsFilterAvailableModelRecord StandardsActionCloneModelRecord_Impl::standardsFilterAvailableModelRecord() const
  {
    ProjectDatabase projectDatabase = this->projectDatabase();

    QSqlQuery query(*(projectDatabase.qSqlDatabase()));
    query.prepare(toQString("SELECT * FROM " + ClauseRecord::databaseTableName() + " WHERE clauseRecordType=:clauseRecordType AND \
                             filterClauseRecordType=:filterClauseRecordType AND \
                             standardsFilterClauseRecordType=:standardsFilterClauseRecordType AND id=:id "));
    query.bindValue(":clauseRecordType", ClauseRecordType::FilterClauseRecord);
    query.bindValue(":filterClauseRecordType", FilterClauseRecordType::StandardsFilterClauseRecord);
    query.bindValue(":standardsActionClauseRecordType", StandardsActionClauseRecordType::StandardsActionCloneModelRecord);
    query.bindValue(":id", m_standardsFilterAvailableModelId);
    assertExec(query);
    BOOST_ASSERT(query.first());

    return StandardsFilterAvailableModelRecord(query, projectDatabase);
  }

  boost::optional<ObjectRecord> StandardsActionCloneModelRecord_Impl::parent() const
  {
    return boost::none;
  }

  std::vector<ObjectRecord> StandardsActionCloneModelRecord_Impl::children() const
  {
    std::vector<ObjectRecord> result;
    result.push_back(this->standardsFilterAvailableModelRecord());
    return result;
  }

  std::vector<ObjectRecord> StandardsActionCloneModelRecord_Impl::resources() const
  {
    return std::vector<ObjectRecord>();
  }

  std::vector<JoinRecord> StandardsActionCloneModelRecord_Impl::joinRecords() const
  {
    std::vector<Rule_Clause_JoinRecord> result = JoinRecord::getJoinRecordsForRightId<Rule_Clause_JoinRecord>(this->id(), this->projectDatabase());
    return std::vector<JoinRecord>(result.begin(), result.end());
  }

  void StandardsActionCloneModelRecord_Impl::saveRow(const boost::shared_ptr<QSqlDatabase> &database)
  {
    QSqlQuery query(*database);
    this->makeUpdateByIdQuery<StandardsActionCloneModelRecord>(query);
    this->bindValues(query);
    assertExec(query);
  }

  void StandardsActionCloneModelRecord_Impl::bindValues(QSqlQuery& query) const
  {
    StandardsActionClauseRecord_Impl::bindValues(query);

    query.bindValue(ClauseRecordColumns::standardsFilterAvailableModelId, m_standardsFilterAvailableModelId);
    query.bindValue(ClauseRecordColumns::standardsRulesetModelType, toQString(m_standardsRulesetModelType.valueName()));
  }

  void StandardsActionCloneModelRecord_Impl::setLastValues(const QSqlQuery& query, ProjectDatabase& projectDatabase)
  {
    StandardsActionClauseRecord_Impl::setLastValues(query, projectDatabase);

    QVariant value;
    value = query.value(ClauseRecordColumns::standardsFilterAvailableModelId);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    m_lastStandardsFilterAvailableModelId = value.toInt();

    value = query.value(ClauseRecordColumns::standardsRulesetModelType);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    m_lastStandardsRulesetModelType = ruleset::StandardsRulesetModelType(value.toString().toStdString());
  }

  bool StandardsActionCloneModelRecord_Impl::compareValues(const QSqlQuery& query) const
  {
    bool result = true;

    result = result && StandardsActionClauseRecord_Impl::compareValues(query);

    QVariant value;
    value = query.value(ClauseRecordColumns::standardsFilterAvailableModelId);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_standardsFilterAvailableModelId == value.toInt());

    value = query.value(ClauseRecordColumns::standardsRulesetModelType);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_standardsRulesetModelType == ruleset::StandardsRulesetModelType(value.toString().toStdString()));

    return result;
  }

  void StandardsActionCloneModelRecord_Impl::saveLastValues()
  {
    StandardsActionClauseRecord_Impl::saveLastValues();

    m_lastStandardsFilterAvailableModelId = m_standardsFilterAvailableModelId;    
    m_lastStandardsRulesetModelType = m_standardsRulesetModelType;             

  }

  void StandardsActionCloneModelRecord_Impl::revertToLastValues()
  {
    StandardsActionClauseRecord_Impl::revertToLastValues();

    m_standardsFilterAvailableModelId = m_lastStandardsFilterAvailableModelId;    
    m_standardsRulesetModelType = m_lastStandardsRulesetModelType;           
  }

} // detail

StandardsActionCloneModelRecord::StandardsActionCloneModelRecord(const openstudio::ruleset::StandardsActionCloneModel& standardsActionCloneModel, ProjectDatabase& projectDatabase)
  : StandardsActionClauseRecord(boost::shared_ptr<detail::StandardsActionCloneModelRecord_Impl>(new detail::StandardsActionCloneModelRecord_Impl(standardsActionCloneModel, projectDatabase)), projectDatabase)
{
  BOOST_ASSERT(getImpl<detail::StandardsActionCloneModelRecord_Impl>());
}

StandardsActionCloneModelRecord::StandardsActionCloneModelRecord(const QSqlQuery& query, ProjectDatabase& projectDatabase)
  : StandardsActionClauseRecord(boost::shared_ptr<detail::StandardsActionCloneModelRecord_Impl>(new detail::StandardsActionCloneModelRecord_Impl(query, projectDatabase)), projectDatabase)
{
  BOOST_ASSERT(getImpl<detail::StandardsActionCloneModelRecord_Impl>());
}

StandardsActionCloneModelRecord::StandardsActionCloneModelRecord(boost::shared_ptr<detail::StandardsActionCloneModelRecord_Impl> impl, ProjectDatabase projectDatabase)
  : StandardsActionClauseRecord(impl, projectDatabase)
{
  BOOST_ASSERT(getImpl<detail::StandardsActionCloneModelRecord_Impl>());
}

/// @cond
StandardsActionCloneModelRecord::StandardsActionCloneModelRecord(boost::shared_ptr<detail::StandardsActionCloneModelRecord_Impl> impl)
  : StandardsActionClauseRecord(impl)
{
  BOOST_ASSERT(getImpl<detail::StandardsActionCloneModelRecord_Impl>());
}
/// @endcond

boost::optional<StandardsActionCloneModelRecord> StandardsActionCloneModelRecord::factoryFromQuery(const QSqlQuery& query, 
                                                                 ProjectDatabase& database)
{
  OptionalStandardsActionCloneModelRecord result;
  try {
    result = StandardsActionCloneModelRecord(query,database);
  }
  catch (const std::exception& e) {
    LOG(Error,"Unable to construct StandardsActionCloneModelRecord from query, because '" 
        << e.what() << "'.");
  }
  return result;
}

std::vector<StandardsActionCloneModelRecord> StandardsActionCloneModelRecord::getStandardsActionCloneModelRecords(ProjectDatabase& database) {
  std::vector<StandardsActionCloneModelRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + ClauseRecord::databaseTableName() + 
      " WHERE clauseRecordType=:clauseRecordType" +
      " AND actionClauseRecordType=:actionClauseRecordType" +
      " AND standardsActionClauseRecordType=:standardsActionClauseRecordType"));
  query.bindValue(":clauseRecordType",ClauseRecordType::ActionClauseRecord);
  query.bindValue(":actionClauseRecordType",ActionClauseRecordType::StandardsActionClauseRecord);
  query.bindValue(":standardsActionClauseRecordType", StandardsActionClauseRecordType::StandardsActionCloneModelRecord);
  assertExec(query);
  while (query.next()) {
    result.push_back(StandardsActionCloneModelRecord(query, database));
  }

  return result;
}

boost::optional<StandardsActionCloneModelRecord> StandardsActionCloneModelRecord::getStandardsActionCloneModelRecord(int id, ProjectDatabase& database) {
  boost::optional<StandardsActionCloneModelRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + ClauseRecord::databaseTableName() + 
      " WHERE clauseRecordType=:clauseRecordType" +
      " AND actionClauseRecordType=:actionClauseRecordType" +
      " AND standardsActionClauseRecordType=:standardsActionClauseRecordType AND id=:id"));
  query.bindValue(":clauseRecordType",ClauseRecordType::ActionClauseRecord);
  query.bindValue(":actionClauseRecordType",ActionClauseRecordType::StandardsActionClauseRecord);
  query.bindValue(":standardsActionClauseRecordType", StandardsActionClauseRecordType::StandardsActionCloneModelRecord);
  query.bindValue(":id",id);
  assertExec(query);
  if (query.first()) {
    result = StandardsActionCloneModelRecord(query, database);
  }

  return result;
}

openstudio::ruleset::StandardsActionCloneModel StandardsActionCloneModelRecord::standardsActionCloneModel() const
{
  return this->getImpl<detail::StandardsActionCloneModelRecord_Impl>()->standardsActionCloneModel();
}
    
StandardsFilterAvailableModelRecord StandardsActionCloneModelRecord::standardsFilterAvailableModelRecord() const
{
  return this->getImpl<detail::StandardsActionCloneModelRecord_Impl>()->standardsFilterAvailableModelRecord();
}

} // project
} // openstudio
