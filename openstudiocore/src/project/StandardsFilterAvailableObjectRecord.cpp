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

#include <project/StandardsFilterAvailableObjectRecord.hpp>
#include <project/StandardsFilterAvailableObjectRecord_Impl.hpp>
#include <project/JoinRecord.hpp>
#include <project/ProjectDatabase.hpp>
#include <project/Rule_Clause_JoinRecord.hpp>
#include <project/Rule_Clause_JoinRecord_Impl.hpp>
#include <project/StandardsFilterAvailableModelRecord.hpp>

#include <ruleset/StandardsFilterAvailableObject.hpp>
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

  StandardsFilterAvailableObjectRecord_Impl::StandardsFilterAvailableObjectRecord_Impl(
      const openstudio::ruleset::StandardsFilterAvailableObject& standardsFilterAvailableObject, 
      const ProjectDatabase& projectDatabase)
    : StandardsFilterClauseRecord_Impl(
          standardsFilterAvailableObject, 
          StandardsFilterClauseRecordType::StandardsFilterAvailableObjectRecord, 
          projectDatabase), 
      m_objectType(standardsFilterAvailableObject.objectType())
  {
    if (standardsFilterAvailableObject.model()) {
      ProjectDatabase other = projectDatabase;
      StandardsFilterClauseRecord standardsFilterClauseRecord = StandardsFilterClauseRecord::factoryFromFilter(standardsFilterAvailableObject.model().get(), other);
      m_standardsFilterAvailableModelRecordId = standardsFilterClauseRecord.id();
    }
  }

  StandardsFilterAvailableObjectRecord_Impl::StandardsFilterAvailableObjectRecord_Impl(const QSqlQuery& query, const ProjectDatabase& projectDatabase)
    : StandardsFilterClauseRecord_Impl(query, projectDatabase)
  {
    BOOST_ASSERT(query.isValid());
    BOOST_ASSERT(query.isActive());
    BOOST_ASSERT(query.isSelect());

    QVariant value;
    value = query.value(ClauseRecordColumns::standardsFilterAvailableModelRecordId);
    if(value.isValid() && !value.isNull()){
      m_standardsFilterAvailableModelRecordId = value.toInt();
    }

    value = query.value(ClauseRecordColumns::standardsObjectType);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    m_objectType = standardsinterface::ObjectType(value.toString().toStdString());
  }

  openstudio::ruleset::StandardsFilterAvailableObject 
  StandardsFilterAvailableObjectRecord_Impl::standardsFilterAvailableObject() const
  {
    boost::optional<StandardsFilterAvailableModelRecord> standardsFilterAvailableModelRecord = this->standardsFilterAvailableModelRecord();
    if (standardsFilterAvailableModelRecord){
      openstudio::ruleset::StandardsFilterAvailableObject result(standardsFilterAvailableModelRecord->standardsFilterAvailableModel(),
                                                                 m_objectType, 
                                                                 this->handle(), 
                                                                 this->uuidLast());
      return result;                                             
    }
    openstudio::ruleset::StandardsFilterAvailableObject result(m_objectType, 
                                                               this->handle(), 
                                                               this->uuidLast());
    return result;
  }

  openstudio::ruleset::FilterClause StandardsFilterAvailableObjectRecord_Impl::filterClause() const
  {
    return this->standardsFilterAvailableObject();
  }
    
  boost::optional<StandardsFilterAvailableModelRecord> StandardsFilterAvailableObjectRecord_Impl::standardsFilterAvailableModelRecord() const
  {
    boost::optional<StandardsFilterAvailableModelRecord> result;

    if (m_standardsFilterAvailableModelRecordId){
      ProjectDatabase projectDatabase = this->projectDatabase();
      result = StandardsFilterAvailableModelRecord::getStandardsFilterAvailableModelRecord(*m_standardsFilterAvailableModelRecordId, projectDatabase);
    }

    return result;
  }

  boost::optional<ObjectRecord> StandardsFilterAvailableObjectRecord_Impl::parent() const
  {
    return boost::none;
  }

  std::vector<ObjectRecord> StandardsFilterAvailableObjectRecord_Impl::children() const
  {
    std::vector<ObjectRecord> result;

    boost::optional<StandardsFilterAvailableModelRecord> standardsFilterAvailableModel = this->standardsFilterAvailableModelRecord();
    if (standardsFilterAvailableModel){
      result.push_back(*standardsFilterAvailableModel);
    }

    return result;
  }

  std::vector<ObjectRecord> StandardsFilterAvailableObjectRecord_Impl::resources() const
  {
    std::vector<ObjectRecord> result;
    boost::optional<StandardsFilterAvailableModelRecord> oRecord = 
        standardsFilterAvailableModelRecord();
    if (oRecord) { 
      result.push_back(*oRecord);
    }
    return result;
  }

  std::vector<JoinRecord> StandardsFilterAvailableObjectRecord_Impl::joinRecords() const
  {
    std::vector<Rule_Clause_JoinRecord> result = JoinRecord::getJoinRecordsForRightId<Rule_Clause_JoinRecord>(this->id(), this->projectDatabase());
    return std::vector<JoinRecord>(result.begin(), result.end());
  }

  void StandardsFilterAvailableObjectRecord_Impl::saveRow(const boost::shared_ptr<QSqlDatabase> &database)
  {
    QSqlQuery query(*database);
    this->makeUpdateByIdQuery<StandardsFilterAvailableObjectRecord>(query);
    this->bindValues(query);
    assertExec(query);
  }

  void StandardsFilterAvailableObjectRecord_Impl::bindValues(QSqlQuery& query) const
  {
    StandardsFilterClauseRecord_Impl::bindValues(query);

    if (m_standardsFilterAvailableModelRecordId){
      query.bindValue(ClauseRecordColumns::standardsFilterAvailableModelRecordId, *m_standardsFilterAvailableModelRecordId);
    }else{
      query.bindValue(ClauseRecordColumns::standardsFilterAvailableModelRecordId, QVariant(QVariant::Int));
    }

    query.bindValue(ClauseRecordColumns::standardsObjectType, toQString(m_objectType.valueName()));
  }

  void StandardsFilterAvailableObjectRecord_Impl::setLastValues(const QSqlQuery& query, ProjectDatabase& projectDatabase)
  {
    StandardsFilterClauseRecord_Impl::setLastValues(query, projectDatabase);

    QVariant value;
    value = query.value(ClauseRecordColumns::standardsFilterAvailableModelRecordId);
    if(value.isValid() && !value.isNull()){
      m_lastStandardsFilterAvailableModelRecordId = value.toInt();
    }else{
      m_lastStandardsFilterAvailableModelRecordId.reset();
    }

    value = query.value(ClauseRecordColumns::standardsObjectType);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    m_lastObjectType = standardsinterface::ObjectType(value.toString().toStdString());
  }

  bool StandardsFilterAvailableObjectRecord_Impl::compareValues(const QSqlQuery& query) const
  {
    bool result = true;

    result = result && StandardsFilterClauseRecord_Impl::compareValues(query);

    QVariant value;
    value = query.value(ClauseRecordColumns::standardsFilterAvailableModelRecordId);
    if(value.isValid() && !value.isNull()){
      result = result && m_standardsFilterAvailableModelRecordId && (*m_standardsFilterAvailableModelRecordId == value.toInt());
    }else{
      result = result && !m_standardsFilterAvailableModelRecordId;
    }

    value = query.value(ClauseRecordColumns::standardsObjectType);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_objectType == standardsinterface::ObjectType(value.toString().toStdString()));

    return result;
  }

  void StandardsFilterAvailableObjectRecord_Impl::saveLastValues()
  {
    StandardsFilterClauseRecord_Impl::saveLastValues();

    m_lastStandardsFilterAvailableModelRecordId = m_standardsFilterAvailableModelRecordId;  
    m_lastObjectType = m_objectType;  
  }

  void StandardsFilterAvailableObjectRecord_Impl::revertToLastValues()
  {
    StandardsFilterClauseRecord_Impl::revertToLastValues();

    m_standardsFilterAvailableModelRecordId = m_lastStandardsFilterAvailableModelRecordId; 
    m_objectType = m_lastObjectType; 
  }

} // detail

StandardsFilterAvailableObjectRecord::StandardsFilterAvailableObjectRecord(
    const openstudio::ruleset::StandardsFilterAvailableObject& standardsFilterAvailableObject, 
    ProjectDatabase& projectDatabase)
  : StandardsFilterClauseRecord(
        boost::shared_ptr<detail::StandardsFilterAvailableObjectRecord_Impl>(
        new detail::StandardsFilterAvailableObjectRecord_Impl(standardsFilterAvailableObject, 
                                                              projectDatabase)), 
        projectDatabase)
{
  BOOST_ASSERT(getImpl<detail::StandardsFilterAvailableObjectRecord_Impl>());
}

StandardsFilterAvailableObjectRecord::StandardsFilterAvailableObjectRecord(
    const QSqlQuery& query, 
    ProjectDatabase& projectDatabase)
  : StandardsFilterClauseRecord(
        boost::shared_ptr<detail::StandardsFilterAvailableObjectRecord_Impl>(
        new detail::StandardsFilterAvailableObjectRecord_Impl(query, projectDatabase)), 
        projectDatabase)
{
  BOOST_ASSERT(getImpl<detail::StandardsFilterAvailableObjectRecord_Impl>());
}

StandardsFilterAvailableObjectRecord::StandardsFilterAvailableObjectRecord(
    boost::shared_ptr<detail::StandardsFilterAvailableObjectRecord_Impl> impl, 
    ProjectDatabase projectDatabase)
  : StandardsFilterClauseRecord(impl, projectDatabase)
{
  BOOST_ASSERT(getImpl<detail::StandardsFilterAvailableObjectRecord_Impl>());
}

StandardsFilterAvailableObjectRecord::StandardsFilterAvailableObjectRecord(
    boost::shared_ptr<detail::StandardsFilterAvailableObjectRecord_Impl> impl)
  : StandardsFilterClauseRecord(impl)
{
  BOOST_ASSERT(getImpl<detail::StandardsFilterAvailableObjectRecord_Impl>());
}

boost::optional<StandardsFilterAvailableObjectRecord> StandardsFilterAvailableObjectRecord::factoryFromQuery(const QSqlQuery& query, 
                                                                 ProjectDatabase& database)
{
  OptionalStandardsFilterAvailableObjectRecord result;
  try {
    result = StandardsFilterAvailableObjectRecord(query,database);
  }
  catch (const std::exception& e) {
    LOG(Error,"Unable to construct StandardsFilterAvailableObjectRecord from query, because '" 
        << e.what() << "'.");
  }
  return result;
}

std::vector<StandardsFilterAvailableObjectRecord> StandardsFilterAvailableObjectRecord::getStandardsFilterAvailableObjectRecords(ProjectDatabase& projectDatabase)
{
  std::vector<StandardsFilterAvailableObjectRecord> result;

  QSqlQuery query(*(projectDatabase.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + ClauseRecord::databaseTableName() + " WHERE clauseRecordType=:clauseRecordType AND\
                           filterClauseRecordType=:filterClauseRecordType AND standardsFilterClauseRecordType=:standardsFilterClauseRecordType"));
  query.bindValue(":clauseRecordType", ClauseRecordType::FilterClauseRecord);
  query.bindValue(":filterClauseRecordType", FilterClauseRecordType::StandardsFilterClauseRecord);
  query.bindValue(":standardsFilterClauseRecordType", StandardsFilterClauseRecordType::StandardsFilterAvailableObjectRecord);
  assertExec(query);
  while (query.next()) {
    result.push_back(StandardsFilterAvailableObjectRecord(query, projectDatabase));
  }

  return result;
}

boost::optional<StandardsFilterAvailableObjectRecord> StandardsFilterAvailableObjectRecord::getStandardsFilterAvailableObjectRecord(
    int id, ProjectDatabase& projectDatabase)
{
  boost::optional<StandardsFilterAvailableObjectRecord> result;

  QSqlQuery query(*(projectDatabase.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + ClauseRecord::databaseTableName() + " WHERE id=:id AND clauseRecordType=:clauseRecordType AND\
                           filterClauseRecordType=:filterClauseRecordType AND standardsFilterClauseRecordType=:standardsFilterClauseRecordType"));
  query.bindValue(":id", id);
  query.bindValue(":clauseRecordType", ClauseRecordType::FilterClauseRecord);
  query.bindValue(":filterClauseRecordType", FilterClauseRecordType::StandardsFilterClauseRecord);
  query.bindValue(":standardsFilterClauseRecordType", StandardsFilterClauseRecordType::StandardsFilterAvailableObjectRecord);
  assertExec(query);
  if (query.first()) {
    result = StandardsFilterAvailableObjectRecord(query, projectDatabase);
  }

  return result;
}

openstudio::ruleset::StandardsFilterAvailableObject 
StandardsFilterAvailableObjectRecord::standardsFilterAvailableObject() const
{
  return this->getImpl<detail::StandardsFilterAvailableObjectRecord_Impl>()->standardsFilterAvailableObject();
}

boost::optional<StandardsFilterAvailableModelRecord> 
StandardsFilterAvailableObjectRecord::standardsFilterAvailableModelRecord() const
{
  return getImpl<detail::StandardsFilterAvailableObjectRecord_Impl>()->standardsFilterAvailableModelRecord();
}

} // project
} // openstudio
