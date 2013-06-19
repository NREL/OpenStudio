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

#include <project/ModelObjectActionSetRelationshipRecord.hpp>
#include <project/ModelObjectActionSetRelationshipRecord_Impl.hpp>
#include <project/JoinRecord.hpp>
#include <project/ProjectDatabase.hpp>
#include <project/FileReferenceRecord.hpp>
#include <project/Rule_Clause_JoinRecord.hpp>
#include <project/Rule_Clause_JoinRecord_Impl.hpp>

#include <ruleset/ModelObjectActionSetRelationship.hpp>

#include <model/Component.hpp>

#include <utilities/core/Assert.hpp>
#include <utilities/core/FileReference.hpp>

#include <boost/optional/optional.hpp>
#include <boost/foreach.hpp>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

namespace openstudio {
namespace project {
namespace detail {

  ModelObjectActionSetRelationshipRecord_Impl::ModelObjectActionSetRelationshipRecord_Impl(
      const openstudio::ruleset::ModelObjectActionSetRelationship& modelObjectActionSetRelationship, 
      const ProjectDatabase& projectDatabase)
    : ModelObjectActionClauseRecord_Impl(modelObjectActionSetRelationship, 
                                         ModelObjectActionClauseRecordType::ModelObjectActionSetRelationshipRecord, 
                                         projectDatabase), 
      m_relationshipName(modelObjectActionSetRelationship.relationshipName())
  {
    FileReference componentReference = modelObjectActionSetRelationship.componentReference();
    ProjectDatabase database = this->projectDatabase();
    FileReferenceRecord componentRecord(componentReference,database);
    m_componentRecordId = componentRecord.id();
  }

  ModelObjectActionSetRelationshipRecord_Impl::ModelObjectActionSetRelationshipRecord_Impl(
      const QSqlQuery& query, 
      const ProjectDatabase& projectDatabase)
    : ModelObjectActionClauseRecord_Impl(query, projectDatabase)
  {
    BOOST_ASSERT(query.isValid());
    BOOST_ASSERT(query.isActive());
    BOOST_ASSERT(query.isSelect());

    QVariant value;
    value = query.value(ClauseRecordColumns::relationshipName);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    m_relationshipName = value.toString().toStdString();

    value = query.value(ClauseRecordColumns::componentRecordId);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    m_componentRecordId = value.toInt();
  }

  FileReferenceRecord ModelObjectActionSetRelationshipRecord_Impl::componentRecord() const
  {
    ProjectDatabase database = this->projectDatabase();
    boost::optional<FileReferenceRecord> result = 
        FileReferenceRecord::getFileReferenceRecord(m_componentRecordId, database);
    BOOST_ASSERT(result);
    return *result;
  }

  openstudio::ruleset::ModelObjectActionSetRelationship ModelObjectActionSetRelationshipRecord_Impl::modelObjectActionSetRelationship() const
  {
    openstudio::ruleset::ModelObjectActionSetRelationship result(m_relationshipName, 
                                                                 componentRecord().fileReference(), 
                                                                 this->handle(), 
                                                                 this->uuidLast());
    return result;
  }

  openstudio::ruleset::ActionClause ModelObjectActionSetRelationshipRecord_Impl::actionClause() const
  {
    return this->modelObjectActionSetRelationship();
  }

  boost::optional<ObjectRecord> ModelObjectActionSetRelationshipRecord_Impl::parent() const
  {
    return boost::none;
  }

  std::vector<ObjectRecord> ModelObjectActionSetRelationshipRecord_Impl::children() const
  {
    return std::vector<ObjectRecord>();
  }

  std::vector<ObjectRecord> ModelObjectActionSetRelationshipRecord_Impl::resources() const
  {
    std::vector<ObjectRecord> result;
    result.push_back(componentRecord());
    return result;
  }

  std::vector<JoinRecord> ModelObjectActionSetRelationshipRecord_Impl::joinRecords() const
  {
    std::vector<Rule_Clause_JoinRecord> result = JoinRecord::getJoinRecordsForRightId<Rule_Clause_JoinRecord>(this->id(), this->projectDatabase());
    return std::vector<JoinRecord>(result.begin(), result.end());
  }

  void ModelObjectActionSetRelationshipRecord_Impl::saveRow(const boost::shared_ptr<QSqlDatabase> &database)
  {
    QSqlQuery query(*database);
    this->makeUpdateByIdQuery<ModelObjectActionSetRelationshipRecord>(query);
    this->bindValues(query);
    assertExec(query);
  }

  void ModelObjectActionSetRelationshipRecord_Impl::bindValues(QSqlQuery& query) const
  {
    ModelObjectActionClauseRecord_Impl::bindValues(query);

    query.bindValue(ClauseRecordColumns::relationshipName, toQString(m_relationshipName));
    query.bindValue(ClauseRecordColumns::componentRecordId, m_componentRecordId);
  }

  void ModelObjectActionSetRelationshipRecord_Impl::setLastValues(const QSqlQuery& query, ProjectDatabase& projectDatabase)
  {
    ModelObjectActionClauseRecord_Impl::setLastValues(query, projectDatabase);

    QVariant value;
    value = query.value(ClauseRecordColumns::relationshipName);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    m_lastRelationshipName = value.toString().toStdString();

    value = query.value(ClauseRecordColumns::componentRecordId);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    m_lastComponentRecordId = value.toInt();
  }

  bool ModelObjectActionSetRelationshipRecord_Impl::compareValues(const QSqlQuery& query) const
  {
    bool result = true;

    result = result && ModelObjectActionClauseRecord_Impl::compareValues(query);

    QVariant value;
    value = query.value(ClauseRecordColumns::relationshipName);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_relationshipName == value.toString().toStdString());

    value = query.value(ClauseRecordColumns::componentRecordId);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    result = result &&(m_componentRecordId == value.toInt());

    return result;
  }

  void ModelObjectActionSetRelationshipRecord_Impl::saveLastValues()
  {
    ModelObjectActionClauseRecord_Impl::saveLastValues();

    m_lastRelationshipName = m_relationshipName;            
    m_lastComponentRecordId = m_componentRecordId;   
  }

  void ModelObjectActionSetRelationshipRecord_Impl::revertToLastValues()
  {
    ModelObjectActionClauseRecord_Impl::revertToLastValues();

    m_relationshipName = m_lastRelationshipName;           
    m_componentRecordId = m_lastComponentRecordId;   
  }

} // detail

ModelObjectActionSetRelationshipRecord::ModelObjectActionSetRelationshipRecord(
    const openstudio::ruleset::ModelObjectActionSetRelationship& modelObjectActionSetRelationship, 
    ProjectDatabase& projectDatabase)
  : ModelObjectActionClauseRecord(boost::shared_ptr<detail::ModelObjectActionSetRelationshipRecord_Impl>(new detail::ModelObjectActionSetRelationshipRecord_Impl(modelObjectActionSetRelationship, projectDatabase)), projectDatabase)
{
  BOOST_ASSERT(getImpl<detail::ModelObjectActionSetRelationshipRecord_Impl>());
}

ModelObjectActionSetRelationshipRecord::ModelObjectActionSetRelationshipRecord(const QSqlQuery& query, ProjectDatabase& projectDatabase)
  : ModelObjectActionClauseRecord(boost::shared_ptr<detail::ModelObjectActionSetRelationshipRecord_Impl>(new detail::ModelObjectActionSetRelationshipRecord_Impl(query, projectDatabase)), projectDatabase)
{
  BOOST_ASSERT(getImpl<detail::ModelObjectActionSetRelationshipRecord_Impl>());
}

ModelObjectActionSetRelationshipRecord::ModelObjectActionSetRelationshipRecord(boost::shared_ptr<detail::ModelObjectActionSetRelationshipRecord_Impl> impl, ProjectDatabase projectDatabase)
  : ModelObjectActionClauseRecord(impl, projectDatabase)
{
  BOOST_ASSERT(getImpl<detail::ModelObjectActionSetRelationshipRecord_Impl>());
}

ModelObjectActionSetRelationshipRecord::ModelObjectActionSetRelationshipRecord(boost::shared_ptr<detail::ModelObjectActionSetRelationshipRecord_Impl> impl)
  : ModelObjectActionClauseRecord(impl)
{
  BOOST_ASSERT(getImpl<detail::ModelObjectActionSetRelationshipRecord_Impl>());
}

boost::optional<ModelObjectActionSetRelationshipRecord> ModelObjectActionSetRelationshipRecord::factoryFromQuery(
    const QSqlQuery& query, ProjectDatabase& database)
{
  OptionalModelObjectActionSetRelationshipRecord result;
  try {
    result = ModelObjectActionSetRelationshipRecord(query,database);
  }
  catch (const std::exception& e) {
    LOG(Error,"Unable to construct ModelObjectActionSetRelationshipRecord from query, because '" 
        << e.what() << "'.");
  }
  return result;
}

std::vector<ModelObjectActionSetRelationshipRecord> 
ModelObjectActionSetRelationshipRecord::getModelObjectActionSetRelationshipRecords(ProjectDatabase& database) 
{
  std::vector<ModelObjectActionSetRelationshipRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + ClauseRecord::databaseTableName() + 
      " WHERE clauseRecordType=:clauseRecordType AND" +
      " actionClauseRecordType=:actionClauseRecordType AND" +
      " modelObjectActionClauseRecordType=:modelObjectActionClauseRecordType"));
  query.bindValue(":clauseRecordType", ClauseRecordType::ActionClauseRecord);
  query.bindValue(":actionClauseRecordType", ActionClauseRecordType::ModelObjectActionClauseRecord);    
  query.bindValue(":modelObjectActionClauseRecordType", ModelObjectActionClauseRecordType::ModelObjectActionSetRelationshipRecord);
  assertExec(query);
  while (query.next()) {
    result.push_back(ModelObjectActionSetRelationshipRecord(query, database));
  }

  return result;
}

boost::optional<ModelObjectActionSetRelationshipRecord> 
ModelObjectActionSetRelationshipRecord::getModelObjectActionSetRelationshipRecord(int id, ProjectDatabase& database) 
{
  boost::optional<ModelObjectActionSetRelationshipRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + ClauseRecord::databaseTableName() + 
      " WHERE clauseRecordType=:clauseRecordType AND" +
      " actionClauseRecordType=:actionClauseRecordType AND" +
      " modelObjectActionClauseRecordType=:modelObjectActionClauseRecordType AND id=:id"));
  query.bindValue(":clauseRecordType", ClauseRecordType::ActionClauseRecord);
  query.bindValue(":actionClauseRecordType", ActionClauseRecordType::ModelObjectActionClauseRecord);    
  query.bindValue(":modelObjectActionClauseRecordType", ModelObjectActionClauseRecordType::ModelObjectActionSetRelationshipRecord);
  query.bindValue(":id",id);
  assertExec(query);
  if (query.first()) {
    result = ModelObjectActionSetRelationshipRecord(query, database);
  }

  return result;
}


FileReferenceRecord ModelObjectActionSetRelationshipRecord::componentRecord() const
{
  return this->getImpl<detail::ModelObjectActionSetRelationshipRecord_Impl>()->componentRecord();
}

openstudio::ruleset::ModelObjectActionSetRelationship ModelObjectActionSetRelationshipRecord::modelObjectActionSetRelationship() const
{
  return this->getImpl<detail::ModelObjectActionSetRelationshipRecord_Impl>()->modelObjectActionSetRelationship();
}

} // project
} // openstudio
