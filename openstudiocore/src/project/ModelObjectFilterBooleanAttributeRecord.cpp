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

#include <project/ModelObjectFilterBooleanAttributeRecord.hpp>
#include <project/ModelObjectFilterBooleanAttributeRecord_Impl.hpp>
#include <project/JoinRecord.hpp>
#include <project/ProjectDatabase.hpp>
#include <project/Rule_Clause_JoinRecord.hpp>
#include <project/Rule_Clause_JoinRecord_Impl.hpp>

#include <ruleset/ModelObjectFilterBooleanAttribute.hpp>

#include <utilities/core/Assert.hpp>

#include <boost/optional/optional.hpp>
#include <boost/foreach.hpp>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

namespace openstudio {
namespace project {

namespace detail {

  ModelObjectFilterBooleanAttributeRecord_Impl::ModelObjectFilterBooleanAttributeRecord_Impl(const openstudio::ruleset::ModelObjectFilterBooleanAttribute& modelObjectFilterBooleanAttribute, const ProjectDatabase& projectDatabase)
    : ModelObjectFilterAttributeRecord_Impl(modelObjectFilterBooleanAttribute, ModelObjectFilterAttributeRecordType::ModelObjectFilterBooleanAttributeRecord, projectDatabase), m_attributeValue(modelObjectFilterBooleanAttribute.testValue())
  {
  }

  ModelObjectFilterBooleanAttributeRecord_Impl::ModelObjectFilterBooleanAttributeRecord_Impl(const QSqlQuery& query, const ProjectDatabase& projectDatabase)
    : ModelObjectFilterAttributeRecord_Impl(query, projectDatabase)
  {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    QVariant value;
    value = query.value(ClauseRecordColumns::attributeValue);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_attributeValue = value.toBool();
  }

  openstudio::ruleset::ModelObjectFilterBooleanAttribute ModelObjectFilterBooleanAttributeRecord_Impl::modelObjectFilterBooleanAttribute() const
  {
    openstudio::ruleset::ModelObjectFilterBooleanAttribute result(this->attributeName(), m_attributeValue, this->handle(), this->uuidLast());
    return result;
  }

  openstudio::ruleset::FilterClause ModelObjectFilterBooleanAttributeRecord_Impl::filterClause() const
  {
    return this->modelObjectFilterBooleanAttribute();
  }

  boost::optional<ObjectRecord> ModelObjectFilterBooleanAttributeRecord_Impl::parent() const
  {
    return boost::none;
  }

  std::vector<ObjectRecord> ModelObjectFilterBooleanAttributeRecord_Impl::children() const
  {
    return std::vector<ObjectRecord>();
  }

  std::vector<ObjectRecord> ModelObjectFilterBooleanAttributeRecord_Impl::resources() const
  {
    return std::vector<ObjectRecord>();
  }

  std::vector<JoinRecord> ModelObjectFilterBooleanAttributeRecord_Impl::joinRecords() const
  {
    std::vector<Rule_Clause_JoinRecord> result = JoinRecord::getJoinRecordsForRightId<Rule_Clause_JoinRecord>(this->id(), this->projectDatabase());
    return std::vector<JoinRecord>(result.begin(), result.end());
  }

  void ModelObjectFilterBooleanAttributeRecord_Impl::saveRow(const boost::shared_ptr<QSqlDatabase> &database)
  {
    QSqlQuery query(*database);
    this->makeUpdateByIdQuery<ModelObjectFilterBooleanAttributeRecord>(query);
    this->bindValues(query);
    assertExec(query);
  }

  void ModelObjectFilterBooleanAttributeRecord_Impl::bindValues(QSqlQuery& query) const
  {
    ModelObjectFilterAttributeRecord_Impl::bindValues(query);

    query.bindValue(ClauseRecordColumns::attributeValue, m_attributeValue);
  }

  void ModelObjectFilterBooleanAttributeRecord_Impl::setLastValues(const QSqlQuery& query, ProjectDatabase& projectDatabase)
  {
    ModelObjectFilterAttributeRecord_Impl::setLastValues(query, projectDatabase);

    QVariant value;
    value = query.value(ClauseRecordColumns::attributeValue);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_lastAttributeValue = value.toBool();
  }

  bool ModelObjectFilterBooleanAttributeRecord_Impl::compareValues(const QSqlQuery& query) const
  {
    bool result = true;

    result = result && ModelObjectFilterAttributeRecord_Impl::compareValues(query);

    QVariant value;
    value = query.value(ClauseRecordColumns::attributeValue);
    OS_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_attributeValue == value.toBool());
    
    return result;
  }

  void ModelObjectFilterBooleanAttributeRecord_Impl::saveLastValues()
  {
    ModelObjectFilterAttributeRecord_Impl::saveLastValues();

    m_lastAttributeValue = m_attributeValue; 
  }

  void ModelObjectFilterBooleanAttributeRecord_Impl::revertToLastValues()
  {
    ModelObjectFilterAttributeRecord_Impl::revertToLastValues();

    m_attributeValue = m_lastAttributeValue;
  }

} // detail

ModelObjectFilterBooleanAttributeRecord::ModelObjectFilterBooleanAttributeRecord(const openstudio::ruleset::ModelObjectFilterBooleanAttribute& modelObjectFilterBooleanAttribute, ProjectDatabase& projectDatabase)
  : ModelObjectFilterAttributeRecord(boost::shared_ptr<detail::ModelObjectFilterBooleanAttributeRecord_Impl>(new detail::ModelObjectFilterBooleanAttributeRecord_Impl(modelObjectFilterBooleanAttribute, projectDatabase)), projectDatabase)
{
  OS_ASSERT(getImpl<detail::ModelObjectFilterBooleanAttributeRecord_Impl>());
}

ModelObjectFilterBooleanAttributeRecord::ModelObjectFilterBooleanAttributeRecord(const QSqlQuery& query, ProjectDatabase& projectDatabase)
  : ModelObjectFilterAttributeRecord(boost::shared_ptr<detail::ModelObjectFilterBooleanAttributeRecord_Impl>(new detail::ModelObjectFilterBooleanAttributeRecord_Impl(query, projectDatabase)), projectDatabase)
{
  OS_ASSERT(getImpl<detail::ModelObjectFilterBooleanAttributeRecord_Impl>());
}

ModelObjectFilterBooleanAttributeRecord::ModelObjectFilterBooleanAttributeRecord(boost::shared_ptr<detail::ModelObjectFilterBooleanAttributeRecord_Impl> impl, ProjectDatabase projectDatabase)
  : ModelObjectFilterAttributeRecord(impl, projectDatabase)
{
  OS_ASSERT(getImpl<detail::ModelObjectFilterBooleanAttributeRecord_Impl>());
}

ModelObjectFilterBooleanAttributeRecord::ModelObjectFilterBooleanAttributeRecord(boost::shared_ptr<detail::ModelObjectFilterBooleanAttributeRecord_Impl> impl)
  : ModelObjectFilterAttributeRecord(impl)
{
  OS_ASSERT(getImpl<detail::ModelObjectFilterBooleanAttributeRecord_Impl>());
}

boost::optional<ModelObjectFilterBooleanAttributeRecord> ModelObjectFilterBooleanAttributeRecord::factoryFromQuery(
    const QSqlQuery& query, ProjectDatabase& database)
{
  OptionalModelObjectFilterBooleanAttributeRecord result;
  try {
    result = ModelObjectFilterBooleanAttributeRecord(query,database);
  }
  catch (const std::exception& e) {
    LOG(Error,"Unable to construct ModelObjectFilterBooleanAttributeRecord from query, because '" 
        << e.what() << "'.");
  }
  return result;
}

std::vector<ModelObjectFilterBooleanAttributeRecord> ModelObjectFilterBooleanAttributeRecord::getModelObjectFilterBooleanAttributeRecords(ProjectDatabase& database) {
  std::vector<ModelObjectFilterBooleanAttributeRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + ClauseRecord::databaseTableName() + 
      " WHERE clauseRecordType=:clauseRecordType" +
      " AND filterClauseRecordType=:filterClauseRecordType" +
      " AND modelObjectFilterClauseRecordType=:modelObjectFilterClauseRecordType" +
      " AND modelObjectFilterAttributeRecordType=:modelObjectFilterAttributeRecordType"));
  query.bindValue(":clauseRecordType",ClauseRecordType::FilterClauseRecord);
  query.bindValue(":filterClauseRecordType",FilterClauseRecordType::ModelObjectFilterClauseRecord);
  query.bindValue(":modelObjectFilterClauseRecordType", ModelObjectFilterClauseRecordType::ModelObjectFilterAttributeRecord);
  query.bindValue(":modelObjectFilterAttributeRecordType", ModelObjectFilterAttributeRecordType::ModelObjectFilterBooleanAttributeRecord);
  assertExec(query);
  while (query.next()) {
    result.push_back(ModelObjectFilterBooleanAttributeRecord(query, database));
  }

  return result;
}

boost::optional<ModelObjectFilterBooleanAttributeRecord> ModelObjectFilterBooleanAttributeRecord::getModelObjectFilterBooleanAttributeRecord(int id, ProjectDatabase& database) {
  boost::optional<ModelObjectFilterBooleanAttributeRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + ClauseRecord::databaseTableName() + 
      " WHERE clauseRecordType=:clauseRecordType" +
      " AND filterClauseRecordType=:filterClauseRecordType" +
      " AND modelObjectFilterClauseRecordType=:modelObjectFilterClauseRecordType" +
      " AND modelObjectFilterAttributeRecordType=:modelObjectFilterAttributeRecordType AND id=:id"));
  query.bindValue(":clauseRecordType",ClauseRecordType::FilterClauseRecord);
  query.bindValue(":filterClauseRecordType",FilterClauseRecordType::ModelObjectFilterClauseRecord);
  query.bindValue(":modelObjectFilterClauseRecordType", ModelObjectFilterClauseRecordType::ModelObjectFilterAttributeRecord);
  query.bindValue(":modelObjectFilterAttributeRecordType", ModelObjectFilterAttributeRecordType::ModelObjectFilterBooleanAttributeRecord);
  query.bindValue(":id",id);
  assertExec(query);
  if (query.first()) {
    result = ModelObjectFilterBooleanAttributeRecord(query, database);
  }

  return result;
}

openstudio::ruleset::ModelObjectFilterBooleanAttribute ModelObjectFilterBooleanAttributeRecord::modelObjectFilterBooleanAttribute() const
{
  return this->getImpl<detail::ModelObjectFilterBooleanAttributeRecord_Impl>()->modelObjectFilterBooleanAttribute();
}

} // project
} // openstudio
