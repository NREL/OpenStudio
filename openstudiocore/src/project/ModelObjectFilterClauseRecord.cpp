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

#include <project/ModelObjectFilterClauseRecord.hpp>
#include <project/ModelObjectFilterClauseRecord_Impl.hpp>
#include <project/JoinRecord.hpp>
#include <project/ProjectDatabase.hpp>
#include <project/ModelObjectFilterTypeRecord.hpp>
#include <project/ModelObjectFilterAttributeRecord.hpp>
#include <project/ModelObjectFilterRelationshipRecord.hpp>

#include <ruleset/ModelObjectFilterClause.hpp>
#include <ruleset/ModelObjectFilterType.hpp>
#include <ruleset/ModelObjectFilterType_Impl.hpp>
#include <ruleset/ModelObjectFilterAttribute.hpp>
#include <ruleset/ModelObjectFilterAttribute_Impl.hpp>
#include <ruleset/ModelObjectFilterRelationship.hpp>
#include <ruleset/ModelObjectFilterRelationship_Impl.hpp>

#include <utilities/core/Assert.hpp>

#include <boost/optional/optional.hpp>
#include <boost/foreach.hpp>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

namespace openstudio {
namespace project {

namespace detail {

  ModelObjectFilterClauseRecord_Impl::ModelObjectFilterClauseRecord_Impl(const openstudio::ruleset::ModelObjectFilterClause& modelObjectFilterClause, const ModelObjectFilterClauseRecordType& ModelObjectFilterClauseRecordType, const ProjectDatabase& projectDatabase)
    : FilterClauseRecord_Impl(modelObjectFilterClause, FilterClauseRecordType::ModelObjectFilterClauseRecord, projectDatabase), 
      m_modelObjectFilterClauseRecordType(ModelObjectFilterClauseRecordType)
  {
    LOG(Trace,"Constructing ModelObjectFilterClauseRecord of type " 
        << m_modelObjectFilterClauseRecordType.valueName() << ".");
  }

  ModelObjectFilterClauseRecord_Impl::ModelObjectFilterClauseRecord_Impl(const QSqlQuery& query, const ProjectDatabase& projectDatabase)
    : FilterClauseRecord_Impl(query, projectDatabase)
  {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    QVariant value;
    value = query.value(ClauseRecordColumns::modelObjectFilterClauseRecordType);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_modelObjectFilterClauseRecordType = ModelObjectFilterClauseRecordType(value.toInt());
  }

  void ModelObjectFilterClauseRecord_Impl::bindValues(QSqlQuery& query) const
  {
    FilterClauseRecord_Impl::bindValues(query);

    query.bindValue(ClauseRecordColumns::modelObjectFilterClauseRecordType, m_modelObjectFilterClauseRecordType.value());
  }

  void ModelObjectFilterClauseRecord_Impl::setLastValues(const QSqlQuery& query, ProjectDatabase& projectDatabase)
  {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    FilterClauseRecord_Impl::setLastValues(query, projectDatabase);

    QVariant value;
    value = query.value(ClauseRecordColumns::modelObjectFilterClauseRecordType);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_lastModelObjectFilterClauseRecordType = ModelObjectFilterClauseRecordType(value.toInt());
  }

  bool ModelObjectFilterClauseRecord_Impl::compareValues(const QSqlQuery& query) const
  {
    bool result = true;

    result = result && FilterClauseRecord_Impl::compareValues(query);

    QVariant value;
    value = query.value(ClauseRecordColumns::modelObjectFilterClauseRecordType);
    OS_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_modelObjectFilterClauseRecordType == ModelObjectFilterClauseRecordType(value.toInt()));

    return result;
  }

  void ModelObjectFilterClauseRecord_Impl::saveLastValues()
  {
    FilterClauseRecord_Impl::saveLastValues();

    m_lastModelObjectFilterClauseRecordType = m_modelObjectFilterClauseRecordType; 
  }

  void ModelObjectFilterClauseRecord_Impl::revertToLastValues()
  {
    FilterClauseRecord_Impl::revertToLastValues();

    m_modelObjectFilterClauseRecordType = m_lastModelObjectFilterClauseRecordType;
  }

} // detail

ModelObjectFilterClauseRecord::ModelObjectFilterClauseRecord(boost::shared_ptr<detail::ModelObjectFilterClauseRecord_Impl> impl, ProjectDatabase projectDatabase)
  : FilterClauseRecord(impl, projectDatabase)
{
  OS_ASSERT(getImpl<detail::ModelObjectFilterClauseRecord_Impl>());
}

ModelObjectFilterClauseRecord::ModelObjectFilterClauseRecord(boost::shared_ptr<detail::ModelObjectFilterClauseRecord_Impl> impl)
  : FilterClauseRecord(impl)
{
  OS_ASSERT(getImpl<detail::ModelObjectFilterClauseRecord_Impl>());
}

std::vector<ModelObjectFilterClauseRecord> 
ModelObjectFilterClauseRecord::getModelObjectFilterClauseRecords(ProjectDatabase& database) 
{
  std::vector<ModelObjectFilterClauseRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + ClauseRecord::databaseTableName() + 
      " WHERE clauseRecordType=:clauseRecordType AND filterClauseRecordType=:filterClauseRecordType"));
  query.bindValue(":clauseRecordType", ClauseRecordType::FilterClauseRecord);
  query.bindValue(":filterClauseRecordType", FilterClauseRecordType::ModelObjectFilterClauseRecord);
  assertExec(query);
  while (query.next()) {
    OptionalModelObjectFilterClauseRecord modelObjectFilterClauseRecord = ModelObjectFilterClauseRecord::factoryFromQuery(query, database);
    if (modelObjectFilterClauseRecord) {
      result.push_back(*modelObjectFilterClauseRecord);
    }
  }

  return result;
}

boost::optional<ModelObjectFilterClauseRecord> ModelObjectFilterClauseRecord::getModelObjectFilterClauseRecord(int id, ProjectDatabase& database) {
  boost::optional<ModelObjectFilterClauseRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + ClauseRecord::databaseTableName() + 
      " WHERE clauseRecordType=:clauseRecordType AND filterClauseRecordType=:filterClauseRecordType AND id=:id"));
  query.bindValue(":clauseRecordType", ClauseRecordType::FilterClauseRecord);
  query.bindValue(":filterClauseRecordType", FilterClauseRecordType::ModelObjectFilterClauseRecord);
  query.bindValue(":id",id);
  assertExec(query);
  if (query.first()) {
    result = ModelObjectFilterClauseRecord::factoryFromQuery(query, database);
  }

  return result;
}

ModelObjectFilterClauseRecord ModelObjectFilterClauseRecord::factoryFromFilter(const openstudio::ruleset::ModelObjectFilterClause& filter, ProjectDatabase& projectDatabase)
{
  if (filter.optionalCast<ruleset::ModelObjectFilterType>()){
    LOG(Trace,"About to construct ModelObjectFilterTypeRecord.");
    return ModelObjectFilterTypeRecord(filter.cast<ruleset::ModelObjectFilterType>(), projectDatabase);
  }else if(filter.optionalCast<ruleset::ModelObjectFilterAttribute>()){
    LOG(Trace,"About to construct ModelObjectFilterAttributeRecord.");
    return ModelObjectFilterAttributeRecord::factoryFromFilter(filter.cast<ruleset::ModelObjectFilterAttribute>(), projectDatabase);
  }else if(filter.optionalCast<ruleset::ModelObjectFilterRelationship>()) {
    LOG(Trace,"About to construct ModelObjectFilterRelationshipRecord.");
    return ModelObjectFilterRelationshipRecord(filter.cast<ruleset::ModelObjectFilterRelationship>(),projectDatabase);
  }

  LOG(Trace,"ModelObjectFilterClauseRecord::factoryFromFilter fell through.");
  OS_ASSERT(false);
  return ModelObjectFilterClauseRecord(boost::shared_ptr<detail::ModelObjectFilterClauseRecord_Impl>());
}

boost::optional<ModelObjectFilterClauseRecord> ModelObjectFilterClauseRecord::factoryFromQuery(const QSqlQuery& query, ProjectDatabase& database)
{
  boost::optional<ModelObjectFilterClauseRecord> result;

  int modelObjectFilterClauseRecordType = query.value(ClauseRecordColumns::modelObjectFilterClauseRecordType).toInt();

  switch (modelObjectFilterClauseRecordType){
    case ModelObjectFilterClauseRecordType::ModelObjectFilterTypeRecord:
      result = ModelObjectFilterTypeRecord(query, database);
      break;     
    case ModelObjectFilterClauseRecordType::ModelObjectFilterAttributeRecord:
      result = ModelObjectFilterAttributeRecord::factoryFromQuery(query, database);
      break;  
    case ModelObjectFilterClauseRecordType::ModelObjectFilterRelationshipRecord:
      result = ModelObjectFilterRelationshipRecord::factoryFromQuery(query,database);
      break;
    default:
      LOG(Error, "Unknown modelObjectFilterClauseRecordType " << modelObjectFilterClauseRecordType);
  }

  return result;
}

} // project
} // openstudio
