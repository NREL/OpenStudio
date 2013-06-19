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

#include <project/ModelObjectActionSetAttributeRecord.hpp>
#include <project/ModelObjectActionSetAttributeRecord_Impl.hpp>
#include <project/JoinRecord.hpp>
#include <project/ProjectDatabase.hpp>
#include <project/Rule_Clause_JoinRecord.hpp>
#include <project/Rule_Clause_JoinRecord_Impl.hpp>

#include <ruleset/ModelObjectActionSetAttribute.hpp>

#include <utilities/core/Assert.hpp>

#include <boost/optional/optional.hpp>
#include <boost/foreach.hpp>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

namespace openstudio {
namespace project {
namespace detail {

  ModelObjectActionSetAttributeRecord_Impl::ModelObjectActionSetAttributeRecord_Impl(const openstudio::ruleset::ModelObjectActionSetAttribute& modelObjectActionSetAttribute, const ProjectDatabase& projectDatabase)
    : ModelObjectActionClauseRecord_Impl(modelObjectActionSetAttribute, ModelObjectActionClauseRecordType::ModelObjectActionSetAttributeRecord, projectDatabase),
      m_attributeName(modelObjectActionSetAttribute.attributeName()),
      m_attributeValueType(modelObjectActionSetAttribute.attributeValueType()),
      m_attributeValue(modelObjectActionSetAttribute.attributeValue())
  {

  }

  ModelObjectActionSetAttributeRecord_Impl::ModelObjectActionSetAttributeRecord_Impl(const QSqlQuery& query, const ProjectDatabase& projectDatabase)
    : ModelObjectActionClauseRecord_Impl(query, projectDatabase)
  {
    BOOST_ASSERT(query.isValid());
    BOOST_ASSERT(query.isActive());
    BOOST_ASSERT(query.isSelect());

    QVariant value;
    value = query.value(ClauseRecordColumns::attributeName);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    m_attributeName = value.toString().toStdString();

    value = query.value(ClauseRecordColumns::attributeValueType);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    m_attributeValueType = AttributeValueType(value.toInt());

    value = query.value(ClauseRecordColumns::attributeValue);
    switch(m_attributeValueType.value()){
      case AttributeValueType::Boolean:
        BOOST_ASSERT(value.isValid() && !value.isNull());
        m_attributeValue = value.toBool();
        break;
      case AttributeValueType::Integer:
        BOOST_ASSERT(value.isValid() && !value.isNull());
        m_attributeValue = value.toInt();
        break;
      case AttributeValueType::Unsigned:
        BOOST_ASSERT(value.isValid() && !value.isNull());
        m_attributeValue = value.toUInt();
        break;
      case AttributeValueType::Double:
        BOOST_ASSERT(value.isValid() && !value.isNull());
        m_attributeValue = value.toDouble();
        break;
      case AttributeValueType::String:
        BOOST_ASSERT(value.isValid() && !value.isNull());
        m_attributeValue.setValue(value.toString().toStdString());
        break;
      case AttributeValueType::AttributeVector:
        // empty value
        m_attributeValue.clear();
        break;
      default:
        BOOST_ASSERT(false);
        break;
    }

  }

  openstudio::ruleset::ModelObjectActionSetAttribute ModelObjectActionSetAttributeRecord_Impl::modelObjectActionSetAttribute() const
  {
    openstudio::ruleset::ModelObjectActionSetAttribute result(m_attributeName, m_attributeValueType, m_attributeValue, this->handle(), this->uuidLast());
    return result;
  }

  openstudio::ruleset::ActionClause ModelObjectActionSetAttributeRecord_Impl::actionClause() const
  {
    return this->modelObjectActionSetAttribute();
  }

  boost::optional<ObjectRecord> ModelObjectActionSetAttributeRecord_Impl::parent() const
  {
    return boost::none;
  }

  std::vector<ObjectRecord> ModelObjectActionSetAttributeRecord_Impl::children() const
  {
    return std::vector<ObjectRecord>();
  }

  std::vector<ObjectRecord> ModelObjectActionSetAttributeRecord_Impl::resources() const
  {
    return std::vector<ObjectRecord>();
  }

  std::vector<JoinRecord> ModelObjectActionSetAttributeRecord_Impl::joinRecords() const
  {
    std::vector<Rule_Clause_JoinRecord> result = JoinRecord::getJoinRecordsForRightId<Rule_Clause_JoinRecord>(this->id(), this->projectDatabase());
    return std::vector<JoinRecord>(result.begin(), result.end());
  }

  void ModelObjectActionSetAttributeRecord_Impl::saveRow(const boost::shared_ptr<QSqlDatabase> &database)
  {
    QSqlQuery query(*database);
    this->makeUpdateByIdQuery<ModelObjectActionSetAttributeRecord>(query);
    this->bindValues(query);
    assertExec(query);
  }

  void ModelObjectActionSetAttributeRecord_Impl::bindValues(QSqlQuery& query) const
  {
    ModelObjectActionClauseRecord_Impl::bindValues(query);

    query.bindValue(ClauseRecordColumns::attributeName, toQString(m_attributeName));

    query.bindValue(ClauseRecordColumns::attributeValueType, m_attributeValueType.value());

    switch(m_attributeValueType.value()){
      case AttributeValueType::Boolean:
        if (m_attributeValue.toBool()){
          query.bindValue(ClauseRecordColumns::attributeValue, "true");
        }else{
          query.bindValue(ClauseRecordColumns::attributeValue, "false");
        }
        break;
      case AttributeValueType::Integer:
        query.bindValue(ClauseRecordColumns::attributeValue, m_attributeValue.toInt());
        break;
      case AttributeValueType::Unsigned:
        query.bindValue(ClauseRecordColumns::attributeValue, m_attributeValue.toUInt());
        break;
      case AttributeValueType::Double:
        query.bindValue(ClauseRecordColumns::attributeValue, m_attributeValue.toDouble());
        break;
      case AttributeValueType::String:
        query.bindValue(ClauseRecordColumns::attributeValue, toQString(m_attributeValue.value<std::string>()));
        break;
      case AttributeValueType::AttributeVector:
        query.bindValue(ClauseRecordColumns::attributeValue, QVariant());
        break;
      default:
        BOOST_ASSERT(false);
        break;
    }
  }

  void ModelObjectActionSetAttributeRecord_Impl::setLastValues(const QSqlQuery& query, ProjectDatabase& projectDatabase)
  {
    ModelObjectActionClauseRecord_Impl::setLastValues(query, projectDatabase);

    QVariant value;
    value = query.value(ClauseRecordColumns::attributeName);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    m_lastAttributeName = value.toString().toStdString();

    value = query.value(ClauseRecordColumns::attributeValueType);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    m_lastAttributeValueType = AttributeValueType(value.toInt());

    value = query.value(ClauseRecordColumns::attributeValue);
    
    switch(m_attributeValueType.value()){
      case AttributeValueType::Boolean:
        BOOST_ASSERT(value.isValid() && !value.isNull());
        m_lastAttributeValue = value.toBool();
        break;
      case AttributeValueType::Integer:
        BOOST_ASSERT(value.isValid() && !value.isNull());
        m_lastAttributeValue = value.toInt();
        break;
      case AttributeValueType::Unsigned:
        BOOST_ASSERT(value.isValid() && !value.isNull());
        m_lastAttributeValue = value.toUInt();
        break;
      case AttributeValueType::Double:
        BOOST_ASSERT(value.isValid() && !value.isNull());
        m_lastAttributeValue = value.toDouble();
        break;
      case AttributeValueType::String:
        BOOST_ASSERT(value.isValid() && !value.isNull());
        m_lastAttributeValue.setValue(value.toString().toStdString());
        break;
      case AttributeValueType::AttributeVector:
        m_lastAttributeValue.clear();
        break;
      default:
        BOOST_ASSERT(false);
        break;
    }
  }

  bool ModelObjectActionSetAttributeRecord_Impl::compareValues(const QSqlQuery& query) const
  {
    bool result = true;

    result = result && ModelObjectActionClauseRecord_Impl::compareValues(query);

    QVariant value;
    value = query.value(ClauseRecordColumns::attributeName);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_attributeName == value.toString().toStdString());

    value = query.value(ClauseRecordColumns::attributeValueType);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_attributeValueType == AttributeValueType(value.toInt()));

    value = query.value(ClauseRecordColumns::attributeValue);
    switch(m_attributeValueType.value()){
      case AttributeValueType::Boolean:
        BOOST_ASSERT(value.isValid() && !value.isNull());
        result = result && (m_attributeValue.toBool() == value.toBool());
        break;
      case AttributeValueType::Integer:
        BOOST_ASSERT(value.isValid() && !value.isNull());
        result = result && (m_attributeValue.toInt() == value.toInt());
        break;
      case AttributeValueType::Unsigned:
        BOOST_ASSERT(value.isValid() && !value.isNull());
        result = result && (m_attributeValue.toUInt() == value.toUInt());
        break;
      case AttributeValueType::Double:
        BOOST_ASSERT(value.isValid() && !value.isNull());
        result = result && (m_attributeValue.toDouble() == value.toDouble());
        break;
      case AttributeValueType::String:
        BOOST_ASSERT(value.isValid() && !value.isNull());
        result = result && (m_attributeValue.toString().toStdString() == value.toString().toStdString());
        break;
      case AttributeValueType::AttributeVector:
        break;
      default:
        BOOST_ASSERT(false);
        break;
    }

    return result;
  }

  void ModelObjectActionSetAttributeRecord_Impl::saveLastValues()
  {
    ModelObjectActionClauseRecord_Impl::saveLastValues();

    m_lastAttributeName = m_attributeName;
    m_lastAttributeValueType = m_attributeValueType;
    m_lastAttributeValue = m_attributeValue;
  }

  void ModelObjectActionSetAttributeRecord_Impl::revertToLastValues()
  {
    ModelObjectActionClauseRecord_Impl::revertToLastValues();

    m_attributeName = m_lastAttributeName;
    m_attributeValueType = m_lastAttributeValueType;
    m_attributeValue = m_lastAttributeValue;
  }

} // detail

ModelObjectActionSetAttributeRecord::ModelObjectActionSetAttributeRecord(const openstudio::ruleset::ModelObjectActionSetAttribute& modelObjectActionSetAttribute, ProjectDatabase& projectDatabase)
  : ModelObjectActionClauseRecord(boost::shared_ptr<detail::ModelObjectActionSetAttributeRecord_Impl>(new detail::ModelObjectActionSetAttributeRecord_Impl(modelObjectActionSetAttribute, projectDatabase)), projectDatabase)
{
  BOOST_ASSERT(getImpl<detail::ModelObjectActionSetAttributeRecord_Impl>());
}

ModelObjectActionSetAttributeRecord::ModelObjectActionSetAttributeRecord(const QSqlQuery& query, ProjectDatabase& projectDatabase)
  : ModelObjectActionClauseRecord(boost::shared_ptr<detail::ModelObjectActionSetAttributeRecord_Impl>(new detail::ModelObjectActionSetAttributeRecord_Impl(query, projectDatabase)), projectDatabase)
{
  BOOST_ASSERT(getImpl<detail::ModelObjectActionSetAttributeRecord_Impl>());
}

ModelObjectActionSetAttributeRecord::ModelObjectActionSetAttributeRecord(boost::shared_ptr<detail::ModelObjectActionSetAttributeRecord_Impl> impl, ProjectDatabase projectDatabase)
  : ModelObjectActionClauseRecord(impl, projectDatabase)
{
  BOOST_ASSERT(getImpl<detail::ModelObjectActionSetAttributeRecord_Impl>());
}

ModelObjectActionSetAttributeRecord::ModelObjectActionSetAttributeRecord(boost::shared_ptr<detail::ModelObjectActionSetAttributeRecord_Impl> impl)
  : ModelObjectActionClauseRecord(impl)
{
  BOOST_ASSERT(getImpl<detail::ModelObjectActionSetAttributeRecord_Impl>());
}

boost::optional<ModelObjectActionSetAttributeRecord> ModelObjectActionSetAttributeRecord::factoryFromQuery(
    const QSqlQuery& query, ProjectDatabase& database)
{
  OptionalModelObjectActionSetAttributeRecord result;
  try {
    result = ModelObjectActionSetAttributeRecord(query,database);
  }
  catch (const std::exception& e) {
    LOG(Error,"Unable to construct ModelObjectActionSetAttributeRecord from query, because '" 
        << e.what() << "'.");
  }
  return result;
}

std::vector<ModelObjectActionSetAttributeRecord> ModelObjectActionSetAttributeRecord::getModelObjectActionSetAttributeRecords(ProjectDatabase& database) {
  std::vector<ModelObjectActionSetAttributeRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + ClauseRecord::databaseTableName() + 
      " WHERE clauseRecordType=:clauseRecordType AND" +
      " actionClauseRecordType=:actionClauseRecordType AND" +
      " modelObjectActionClauseRecordType=:modelObjectActionClauseRecordType"));
  query.bindValue(":clauseRecordType", ClauseRecordType::ActionClauseRecord);
  query.bindValue(":actionClauseRecordType", ActionClauseRecordType::ModelObjectActionClauseRecord);
  query.bindValue(":modelObjectActionClauseRecordType", ModelObjectActionClauseRecordType::ModelObjectActionSetAttributeRecord);
  assertExec(query);
  while (query.next()) {
    result.push_back(ModelObjectActionSetAttributeRecord(query, database));
  }

  return result;
}

boost::optional<ModelObjectActionSetAttributeRecord> ModelObjectActionSetAttributeRecord::getModelObjectActionSetAttributeRecord(int id, ProjectDatabase& database) {
  boost::optional<ModelObjectActionSetAttributeRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + ClauseRecord::databaseTableName() + 
      " WHERE clauseRecordType=:clauseRecordType AND" +
      " actionClauseRecordType=:actionClauseRecordType AND" +
      " modelObjectActionClauseRecordType=:modelObjectActionClauseRecordType AND id=:id"));
  query.bindValue(":clauseRecordType", ClauseRecordType::ActionClauseRecord);
  query.bindValue(":actionClauseRecordType", ActionClauseRecordType::ModelObjectActionClauseRecord);  
  query.bindValue(":modelObjectActionClauseRecordType", ModelObjectActionClauseRecordType::ModelObjectActionSetAttributeRecord);
  query.bindValue(":id",id);
  assertExec(query);
  if (query.first()) {
    result = ModelObjectActionSetAttributeRecord(query, database);
  }

  return result;
}

openstudio::ruleset::ModelObjectActionSetAttribute ModelObjectActionSetAttributeRecord::modelObjectActionSetAttribute() const
{
  return this->getImpl<detail::ModelObjectActionSetAttributeRecord_Impl>()->modelObjectActionSetAttribute();
}

} // project
} // openstudio
