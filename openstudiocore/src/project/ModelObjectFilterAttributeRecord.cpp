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

#include <project/ModelObjectFilterAttributeRecord.hpp>
#include <project/ModelObjectFilterAttributeRecord_Impl.hpp>
#include <project/JoinRecord.hpp>
#include <project/ProjectDatabase.hpp>
#include <project/ModelObjectFilterBooleanAttributeRecord.hpp>
#include <project/ModelObjectFilterNumericAttributeRecord.hpp>
#include <project/ModelObjectFilterStringAttributeRecord.hpp>

#include <ruleset/ModelObjectFilterBooleanAttribute.hpp>
#include <ruleset/ModelObjectFilterBooleanAttribute_Impl.hpp>
#include <ruleset/ModelObjectFilterNumericAttribute.hpp>
#include <ruleset/ModelObjectFilterNumericAttribute_Impl.hpp>
#include <ruleset/ModelObjectFilterStringAttribute.hpp>
#include <ruleset/ModelObjectFilterStringAttribute_Impl.hpp>

#include <utilities/core/Assert.hpp>

#include <boost/optional/optional.hpp>
#include <boost/foreach.hpp>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

namespace openstudio {
namespace project {

namespace detail {

  ModelObjectFilterAttributeRecord_Impl::ModelObjectFilterAttributeRecord_Impl(const openstudio::ruleset::ModelObjectFilterAttribute& modelObjectFilterAttribute, const ModelObjectFilterAttributeRecordType& modelObjectFilterAttributeRecordType, const ProjectDatabase& projectDatabase)
    : ModelObjectFilterClauseRecord_Impl(modelObjectFilterAttribute, 
                                         ModelObjectFilterClauseRecordType::ModelObjectFilterAttributeRecord, 
                                         projectDatabase), 
      m_modelObjectFilterAttributeRecordType(modelObjectFilterAttributeRecordType),
      m_attributeName(modelObjectFilterAttribute.attributeName())
  {
  }

  ModelObjectFilterAttributeRecord_Impl::ModelObjectFilterAttributeRecord_Impl(const QSqlQuery& query, const ProjectDatabase& projectDatabase)
    : ModelObjectFilterClauseRecord_Impl(query, projectDatabase)
  {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    QVariant value;
    value = query.value(ClauseRecordColumns::modelObjectFilterAttributeRecordType);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_modelObjectFilterAttributeRecordType = ModelObjectFilterAttributeRecordType(value.toInt());
    
    value = query.value(ClauseRecordColumns::attributeName);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_attributeName = value.toString().toStdString();
  }

  std::string ModelObjectFilterAttributeRecord_Impl::attributeName() const
  {
    return m_attributeName;
  }

  void ModelObjectFilterAttributeRecord_Impl::bindValues(QSqlQuery& query) const
  {
    ModelObjectFilterClauseRecord_Impl::bindValues(query);

    query.bindValue(ClauseRecordColumns::modelObjectFilterAttributeRecordType, m_modelObjectFilterAttributeRecordType.value());
    query.bindValue(ClauseRecordColumns::attributeName, toQString(m_attributeName));
  }

  void ModelObjectFilterAttributeRecord_Impl::setLastValues(const QSqlQuery& query, ProjectDatabase& projectDatabase)
  {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    ModelObjectFilterClauseRecord_Impl::setLastValues(query, projectDatabase);

    QVariant value;
    value = query.value(ClauseRecordColumns::modelObjectFilterAttributeRecordType);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_lastModelObjectFilterAttributeRecordType = ModelObjectFilterAttributeRecordType(value.toInt());

    value = query.value(ClauseRecordColumns::attributeName);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_lastAttributeName = value.toString().toStdString();
  }

  bool ModelObjectFilterAttributeRecord_Impl::compareValues(const QSqlQuery& query) const
  {
    bool result = true;

    result = result && ModelObjectFilterClauseRecord_Impl::compareValues(query);

    QVariant value;
    value = query.value(ClauseRecordColumns::modelObjectFilterAttributeRecordType);
    OS_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_modelObjectFilterAttributeRecordType == ModelObjectFilterAttributeRecordType(value.toInt()));
    
    value = query.value(ClauseRecordColumns::attributeName);
    OS_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_attributeName == value.toString().toStdString());

    return result;
  }

  void ModelObjectFilterAttributeRecord_Impl::saveLastValues()
  {
    ModelObjectFilterClauseRecord_Impl::saveLastValues();

    m_lastModelObjectFilterAttributeRecordType = m_modelObjectFilterAttributeRecordType; 
    m_lastAttributeName = m_attributeName; 
  }

  void ModelObjectFilterAttributeRecord_Impl::revertToLastValues()
  {
    ModelObjectFilterClauseRecord_Impl::revertToLastValues();

    m_modelObjectFilterAttributeRecordType = m_lastModelObjectFilterAttributeRecordType;
    m_attributeName = m_lastAttributeName;
  }

} // detail

ModelObjectFilterAttributeRecord::ModelObjectFilterAttributeRecord(boost::shared_ptr<detail::ModelObjectFilterAttributeRecord_Impl> impl, ProjectDatabase projectDatabase)
  : ModelObjectFilterClauseRecord(impl, projectDatabase)
{
  OS_ASSERT(getImpl<detail::ModelObjectFilterAttributeRecord_Impl>());
}

ModelObjectFilterAttributeRecord::ModelObjectFilterAttributeRecord(boost::shared_ptr<detail::ModelObjectFilterAttributeRecord_Impl> impl)
  : ModelObjectFilterClauseRecord(impl)
{
  OS_ASSERT(getImpl<detail::ModelObjectFilterAttributeRecord_Impl>());
}

std::vector<ModelObjectFilterAttributeRecord> ModelObjectFilterAttributeRecord::getModelObjectFilterAttributeRecords(ProjectDatabase& database) {
  std::vector<ModelObjectFilterAttributeRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  // TODO: Check class used to determine databaseTableName().
  // TODO: Check (or add) the WHERE portion of the query. See getAttributeRecords for a non-type WHERE statement.
  query.prepare(toQString("SELECT * FROM " + ClauseRecord::databaseTableName() + 
      " WHERE clauseRecordType=:clauseRecordType" +
      " AND filterClauseRecordType=:filterClauseRecordType" +
      " AND modelObjectFilterClauseRecordType=:modelObjectFilterClauseRecordType"));
  query.bindValue(":clauseRecordType",ClauseRecordType::FilterClauseRecord);
  query.bindValue(":filterClauseRecordType",FilterClauseRecordType::ModelObjectFilterClauseRecord);
  query.bindValue(":modelObjectFilterClauseRecordType", ModelObjectFilterClauseRecordType::ModelObjectFilterAttributeRecord);
  assertExec(query);
  while (query.next()) {
    OptionalModelObjectFilterAttributeRecord modelObjectFilterAttributeRecord = ModelObjectFilterAttributeRecord::factoryFromQuery(query, database);
    if (modelObjectFilterAttributeRecord) {
      result.push_back(*modelObjectFilterAttributeRecord);
    }
  }

  return result;
}

boost::optional<ModelObjectFilterAttributeRecord> ModelObjectFilterAttributeRecord::getModelObjectFilterAttributeRecord(int id, ProjectDatabase& database) {
  boost::optional<ModelObjectFilterAttributeRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + ClauseRecord::databaseTableName() + 
      " WHERE clauseRecordType=:clauseRecordType" +
      " AND filterClauseRecordType=:filterClauseRecordType" +
      " AND modelObjectFilterClauseRecordType=:modelObjectFilterClauseRecordType AND id=:id"));
  query.bindValue(":clauseRecordType",ClauseRecordType::FilterClauseRecord);
  query.bindValue(":filterClauseRecordType",FilterClauseRecordType::ModelObjectFilterClauseRecord);
  query.bindValue(":modelObjectFilterClauseRecordType",ModelObjectFilterClauseRecordType::ModelObjectFilterAttributeRecord);
  query.bindValue(":id",id);
  assertExec(query);
  if (query.first()) {
    result = ModelObjectFilterAttributeRecord::factoryFromQuery(query, database);
  }

  return result;
}

boost::optional<ModelObjectFilterAttributeRecord> ModelObjectFilterAttributeRecord::factoryFromQuery(const QSqlQuery& query, ProjectDatabase& database)
{
  boost::optional<ModelObjectFilterAttributeRecord> result;

  int modelObjectFilterAttributeRecordType = query.value(ClauseRecordColumns::modelObjectFilterAttributeRecordType).toInt();

  switch (modelObjectFilterAttributeRecordType){
    case ModelObjectFilterAttributeRecordType::ModelObjectFilterBooleanAttributeRecord:
      result = ModelObjectFilterBooleanAttributeRecord(query, database);
      break; 
    case ModelObjectFilterAttributeRecordType::ModelObjectFilterNumericAttributeRecord:
      result = ModelObjectFilterNumericAttributeRecord(query, database);
      break;
    case ModelObjectFilterAttributeRecordType::ModelObjectFilterStringAttributeRecord:
      result = ModelObjectFilterStringAttributeRecord(query, database);
      break;
    default:
      LOG(Error, "Unknown modelObjectFilterAttributeRecordType " << modelObjectFilterAttributeRecordType);
  }

  return result;
}

ModelObjectFilterAttributeRecord ModelObjectFilterAttributeRecord::factoryFromFilter(const openstudio::ruleset::ModelObjectFilterAttribute& filter, ProjectDatabase& projectDatabase)
{
  if (filter.optionalCast<ruleset::ModelObjectFilterBooleanAttribute>()){
    return ModelObjectFilterBooleanAttributeRecord(filter.cast<ruleset::ModelObjectFilterBooleanAttribute>(), projectDatabase);
  }else if(filter.optionalCast<ruleset::ModelObjectFilterNumericAttribute>()){
    return ModelObjectFilterNumericAttributeRecord(filter.cast<ruleset::ModelObjectFilterNumericAttribute>(), projectDatabase);
  }else if(filter.optionalCast<ruleset::ModelObjectFilterStringAttribute>()){
    return ModelObjectFilterStringAttributeRecord(filter.cast<ruleset::ModelObjectFilterStringAttribute>(), projectDatabase);
  }

  OS_ASSERT(false);
  return ModelObjectFilterAttributeRecord(boost::shared_ptr<detail::ModelObjectFilterAttributeRecord_Impl>());
}

std::string ModelObjectFilterAttributeRecord::attributeName() const
{
  return this->getImpl<detail::ModelObjectFilterAttributeRecord_Impl>()->attributeName();
}

} // project
} // openstudio
