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

#include <project/ModelObjectFilterNumericAttributeRecord.hpp>
#include <project/ModelObjectFilterNumericAttributeRecord_Impl.hpp>
#include <project/JoinRecord.hpp>
#include <project/ProjectDatabase.hpp>
#include <project/Rule_Clause_JoinRecord.hpp>
#include <project/Rule_Clause_JoinRecord_Impl.hpp>

#include <ruleset/ModelObjectFilterNumericAttribute.hpp>

#include <utilities/core/Assert.hpp>

#include <boost/optional/optional.hpp>
#include <boost/foreach.hpp>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

namespace openstudio {
namespace project {

namespace detail {

  ModelObjectFilterNumericAttributeRecord_Impl::ModelObjectFilterNumericAttributeRecord_Impl(const openstudio::ruleset::ModelObjectFilterNumericAttribute& modelObjectFilterNumericAttribute, const ProjectDatabase& projectDatabase)
    : ModelObjectFilterAttributeRecord_Impl(modelObjectFilterNumericAttribute, ModelObjectFilterAttributeRecordType::ModelObjectFilterNumericAttributeRecord, projectDatabase), 
      m_attributeValueType(modelObjectFilterNumericAttribute.attributeValueType()),   
      m_attributeValue(modelObjectFilterNumericAttribute.testValue()), 
      m_predicate(modelObjectFilterNumericAttribute.predicate())
  {
  }

  ModelObjectFilterNumericAttributeRecord_Impl::ModelObjectFilterNumericAttributeRecord_Impl(const QSqlQuery& query, const ProjectDatabase& projectDatabase)
    : ModelObjectFilterAttributeRecord_Impl(query, projectDatabase)
  {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    QVariant value;
    value = query.value(ClauseRecordColumns::attributeValueType);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_attributeValueType = AttributeValueType(value.toInt());

    value = query.value(ClauseRecordColumns::attributeValue);
    OS_ASSERT(value.isValid() && !value.isNull());
    switch(m_attributeValueType.value()){
      case AttributeValueType::Unsigned:
        m_attributeValue = value.toUInt();
        break;
      case AttributeValueType::Integer:
        m_attributeValue = value.toInt();
        break;
      case AttributeValueType::Double:
        m_attributeValue = value.toDouble();
        break;
      default:
        OS_ASSERT(false);
    }

    value = query.value(ClauseRecordColumns::predicate);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_predicate = ruleset::RulesetNumericalPredicate(value.toInt());
  }

  openstudio::ruleset::ModelObjectFilterNumericAttribute ModelObjectFilterNumericAttributeRecord_Impl::modelObjectFilterNumericAttribute() const
  {
    if (m_attributeValueType.value() == AttributeValueType::Unsigned){
      return openstudio::ruleset::ModelObjectFilterNumericAttribute(this->attributeName(), m_predicate, m_attributeValue.toUInt(), this->handle(), this->uuidLast());
    }else if(m_attributeValueType.value() == AttributeValueType::Integer){
      return openstudio::ruleset::ModelObjectFilterNumericAttribute(this->attributeName(), m_predicate, m_attributeValue.toInt(), this->handle(), this->uuidLast());
    }
    return openstudio::ruleset::ModelObjectFilterNumericAttribute(this->attributeName(), m_predicate, m_attributeValue.toDouble(), this->handle(), this->uuidLast());
  }

  openstudio::ruleset::FilterClause ModelObjectFilterNumericAttributeRecord_Impl::filterClause() const
  {
    return this->modelObjectFilterNumericAttribute();
  }

  boost::optional<ObjectRecord> ModelObjectFilterNumericAttributeRecord_Impl::parent() const
  {
    return boost::none;
  }

  std::vector<ObjectRecord> ModelObjectFilterNumericAttributeRecord_Impl::children() const
  {
    return std::vector<ObjectRecord>();
  }

  std::vector<ObjectRecord> ModelObjectFilterNumericAttributeRecord_Impl::resources() const
  {
    return std::vector<ObjectRecord>();
  }

  std::vector<JoinRecord> ModelObjectFilterNumericAttributeRecord_Impl::joinRecords() const
  {
    std::vector<Rule_Clause_JoinRecord> result = JoinRecord::getJoinRecordsForRightId<Rule_Clause_JoinRecord>(this->id(), this->projectDatabase());
    return std::vector<JoinRecord>(result.begin(), result.end());
  }

  void ModelObjectFilterNumericAttributeRecord_Impl::saveRow(const boost::shared_ptr<QSqlDatabase> &database)
  {
    QSqlQuery query(*database);
    this->makeUpdateByIdQuery<ModelObjectFilterNumericAttributeRecord>(query);
    this->bindValues(query);
    assertExec(query);
  }

  void ModelObjectFilterNumericAttributeRecord_Impl::bindValues(QSqlQuery& query) const
  {
    ModelObjectFilterAttributeRecord_Impl::bindValues(query);

    query.bindValue(ClauseRecordColumns::attributeValueType, m_attributeValueType.value());

    switch(m_attributeValueType.value()){
      case AttributeValueType::Integer:
        query.bindValue(ClauseRecordColumns::attributeValue, m_attributeValue.toInt());
        break;
      case AttributeValueType::Unsigned:
        query.bindValue(ClauseRecordColumns::attributeValue, m_attributeValue.toUInt());
        break;
      case AttributeValueType::Double:
        query.bindValue(ClauseRecordColumns::attributeValue, m_attributeValue.toDouble());
        break;
      default:
        OS_ASSERT(false);
        break;
    }

    query.bindValue(ClauseRecordColumns::predicate, m_predicate.value());
  }

  void ModelObjectFilterNumericAttributeRecord_Impl::setLastValues(const QSqlQuery& query, ProjectDatabase& projectDatabase)
  {
    ModelObjectFilterAttributeRecord_Impl::setLastValues(query, projectDatabase);

    QVariant value;
    value = query.value(ClauseRecordColumns::attributeValueType);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_lastAttributeValueType = AttributeValueType(value.toInt());

    value = query.value(ClauseRecordColumns::attributeValue);
    OS_ASSERT(value.isValid() && !value.isNull());
    switch(m_lastAttributeValueType.value()){
      case AttributeValueType::Unsigned:
        m_lastAttributeValue = value.toUInt();
        break;
      case AttributeValueType::Integer:
        m_lastAttributeValue = value.toInt();
        break;
      case AttributeValueType::Double:
        m_lastAttributeValue = value.toDouble();
        break;
      default:
        OS_ASSERT(false);
    }

    value = query.value(ClauseRecordColumns::predicate);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_lastPredicate = ruleset::RulesetNumericalPredicate(value.toInt());
  }

  bool ModelObjectFilterNumericAttributeRecord_Impl::compareValues(const QSqlQuery& query) const
  {
    bool result = true;

    result = result && ModelObjectFilterAttributeRecord_Impl::compareValues(query);

    QVariant value;

    value = query.value(ClauseRecordColumns::attributeValueType);
    OS_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_attributeValueType == AttributeValueType(value.toInt()));

    value = query.value(ClauseRecordColumns::attributeValue);
    OS_ASSERT(value.isValid() && !value.isNull());
    switch(m_attributeValueType.value()){
      case AttributeValueType::Unsigned:
        result = result && (m_attributeValue.toUInt() == value.toUInt());
        break;
      case AttributeValueType::Integer:
       result = result && (m_attributeValue.toInt() == value.toInt());
        break;
      case AttributeValueType::Double:
        result = result && (m_attributeValue.toDouble() == value.toDouble());
        break;
      default:
        OS_ASSERT(false);
    }

    value = query.value(ClauseRecordColumns::predicate);
    OS_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_predicate == ruleset::RulesetNumericalPredicate(value.toInt()));

    return result;
  }

  void ModelObjectFilterNumericAttributeRecord_Impl::saveLastValues()
  {
    ModelObjectFilterAttributeRecord_Impl::saveLastValues();

    m_lastAttributeValueType = m_attributeValueType;
    m_lastAttributeValue = m_attributeValue;
    m_lastPredicate = m_predicate;
  }

  void ModelObjectFilterNumericAttributeRecord_Impl::revertToLastValues()
  {
    ModelObjectFilterAttributeRecord_Impl::revertToLastValues();

    m_attributeValueType = m_lastAttributeValueType;
    m_attributeValue = m_lastAttributeValue;
    m_predicate = m_lastPredicate;
  }

} // detail

ModelObjectFilterNumericAttributeRecord::ModelObjectFilterNumericAttributeRecord(const openstudio::ruleset::ModelObjectFilterNumericAttribute& modelObjectFilterNumericAttribute, ProjectDatabase& projectDatabase)
  : ModelObjectFilterAttributeRecord(boost::shared_ptr<detail::ModelObjectFilterNumericAttributeRecord_Impl>(new detail::ModelObjectFilterNumericAttributeRecord_Impl(modelObjectFilterNumericAttribute, projectDatabase)), projectDatabase)
{
  OS_ASSERT(getImpl<detail::ModelObjectFilterNumericAttributeRecord_Impl>());
}

ModelObjectFilterNumericAttributeRecord::ModelObjectFilterNumericAttributeRecord(const QSqlQuery& query, ProjectDatabase& projectDatabase)
  : ModelObjectFilterAttributeRecord(boost::shared_ptr<detail::ModelObjectFilterNumericAttributeRecord_Impl>(new detail::ModelObjectFilterNumericAttributeRecord_Impl(query, projectDatabase)), projectDatabase)
{
  OS_ASSERT(getImpl<detail::ModelObjectFilterNumericAttributeRecord_Impl>());
}

ModelObjectFilterNumericAttributeRecord::ModelObjectFilterNumericAttributeRecord(boost::shared_ptr<detail::ModelObjectFilterNumericAttributeRecord_Impl> impl, ProjectDatabase projectDatabase)
  : ModelObjectFilterAttributeRecord(impl, projectDatabase)
{
  OS_ASSERT(getImpl<detail::ModelObjectFilterNumericAttributeRecord_Impl>());
}

ModelObjectFilterNumericAttributeRecord::ModelObjectFilterNumericAttributeRecord(boost::shared_ptr<detail::ModelObjectFilterNumericAttributeRecord_Impl> impl)
  : ModelObjectFilterAttributeRecord(impl)
{
  OS_ASSERT(getImpl<detail::ModelObjectFilterNumericAttributeRecord_Impl>());
}

boost::optional<ModelObjectFilterNumericAttributeRecord> ModelObjectFilterNumericAttributeRecord::factoryFromQuery(
    const QSqlQuery& query, ProjectDatabase& database)
{
  OptionalModelObjectFilterNumericAttributeRecord result;
  try {
    result = ModelObjectFilterNumericAttributeRecord(query,database);
  }
  catch (const std::exception& e) {
    LOG(Error,"Unable to construct ModelObjectFilterNumericAttributeRecord from query, because '" 
        << e.what() << "'.");
  }
  return result;
}

std::vector<ModelObjectFilterNumericAttributeRecord> ModelObjectFilterNumericAttributeRecord::getModelObjectFilterNumericAttributeRecords(ProjectDatabase& database) {
  std::vector<ModelObjectFilterNumericAttributeRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + ClauseRecord::databaseTableName() + 
      " WHERE clauseRecordType=:clauseRecordType" +
      " AND filterClauseRecordType=:filterClauseRecordType" +
      " AND modelObjectFilterClauseRecordType=:modelObjectFilterClauseRecordType" +
      " AND modelObjectFilterAttributeRecordType=:modelObjectFilterAttributeRecordType"));
  query.bindValue(":clauseRecordType",ClauseRecordType::FilterClauseRecord);
  query.bindValue(":filterClauseRecordType",FilterClauseRecordType::ModelObjectFilterClauseRecord);
  query.bindValue(":modelObjectFilterClauseRecordType", ModelObjectFilterClauseRecordType::ModelObjectFilterAttributeRecord);
  query.bindValue(":modelObjectFilterAttributeRecordType", ModelObjectFilterAttributeRecordType::ModelObjectFilterNumericAttributeRecord);
  assertExec(query);
  while (query.next()) {
    result.push_back(ModelObjectFilterNumericAttributeRecord(query, database));
  }

  return result;
}

boost::optional<ModelObjectFilterNumericAttributeRecord> ModelObjectFilterNumericAttributeRecord::getModelObjectFilterNumericAttributeRecord(int id, ProjectDatabase& database) {
  boost::optional<ModelObjectFilterNumericAttributeRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + ClauseRecord::databaseTableName() + 
      " WHERE clauseRecordType=:clauseRecordType" +
      " AND filterClauseRecordType=:filterClauseRecordType" +
      " AND modelObjectFilterClauseRecordType=:modelObjectFilterClauseRecordType" +
      " AND modelObjectFilterAttributeRecordType=:modelObjectFilterAttributeRecordType AND id=:id"));
  query.bindValue(":clauseRecordType",ClauseRecordType::FilterClauseRecord);
  query.bindValue(":filterClauseRecordType",FilterClauseRecordType::ModelObjectFilterClauseRecord);
  query.bindValue(":modelObjectFilterClauseRecordType", ModelObjectFilterClauseRecordType::ModelObjectFilterAttributeRecord);
  query.bindValue(":modelObjectFilterAttributeRecordType", ModelObjectFilterAttributeRecordType::ModelObjectFilterNumericAttributeRecord);
  query.bindValue(":id",id);
  assertExec(query);
  if (query.first()) {
    result = ModelObjectFilterNumericAttributeRecord(query, database);
  }

  return result;
}

openstudio::ruleset::ModelObjectFilterNumericAttribute 
ModelObjectFilterNumericAttributeRecord::modelObjectFilterNumericAttribute() const
{
  return this->getImpl<detail::ModelObjectFilterNumericAttributeRecord_Impl>()->modelObjectFilterNumericAttribute();
}

} // project
} // openstudio
