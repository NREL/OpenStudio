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

#include <project/ModelObjectFilterStringAttributeRecord.hpp>
#include <project/ModelObjectFilterStringAttributeRecord_Impl.hpp>
#include <project/JoinRecord.hpp>
#include <project/ProjectDatabase.hpp>
#include <project/Rule_Clause_JoinRecord.hpp>
#include <project/Rule_Clause_JoinRecord_Impl.hpp>

#include <ruleset/ModelObjectFilterStringAttribute.hpp>

#include <utilities/core/Assert.hpp>

#include <boost/optional/optional.hpp>
#include <boost/foreach.hpp>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

namespace openstudio {
namespace project {

namespace detail {

  ModelObjectFilterStringAttributeRecord_Impl::ModelObjectFilterStringAttributeRecord_Impl(const openstudio::ruleset::ModelObjectFilterStringAttribute& modelObjectFilterStringAttribute, const ProjectDatabase& projectDatabase)
    : ModelObjectFilterAttributeRecord_Impl(modelObjectFilterStringAttribute, ModelObjectFilterAttributeRecordType::ModelObjectFilterStringAttributeRecord, projectDatabase), 
      m_attributeValue(modelObjectFilterStringAttribute.testValue()), m_predicate(modelObjectFilterStringAttribute.predicate())
  {
  }

  ModelObjectFilterStringAttributeRecord_Impl::ModelObjectFilterStringAttributeRecord_Impl(const QSqlQuery& query, const ProjectDatabase& projectDatabase)
    : ModelObjectFilterAttributeRecord_Impl(query, projectDatabase)
  {
    BOOST_ASSERT(query.isValid());
    BOOST_ASSERT(query.isActive());
    BOOST_ASSERT(query.isSelect());

    QVariant value;
    value = query.value(ClauseRecordColumns::attributeValue);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    m_attributeValue = value.toString().toStdString();

    value = query.value(ClauseRecordColumns::predicate);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    m_predicate = ruleset::RulesetStringPredicate(value.toInt());
  }

  openstudio::ruleset::ModelObjectFilterStringAttribute ModelObjectFilterStringAttributeRecord_Impl::modelObjectFilterStringAttribute() const
  {
    openstudio::ruleset::ModelObjectFilterStringAttribute result(this->attributeName(), m_predicate, m_attributeValue, this->handle(), this->uuidLast());
    return result;
  }

  openstudio::ruleset::FilterClause ModelObjectFilterStringAttributeRecord_Impl::filterClause() const
  {
    return this->modelObjectFilterStringAttribute();
  }

  boost::optional<ObjectRecord> ModelObjectFilterStringAttributeRecord_Impl::parent() const
  {
    return boost::none;
  }

  std::vector<ObjectRecord> ModelObjectFilterStringAttributeRecord_Impl::children() const
  {
    return std::vector<ObjectRecord>();
  }

  std::vector<ObjectRecord> ModelObjectFilterStringAttributeRecord_Impl::resources() const
  {
    return std::vector<ObjectRecord>();
  }

  std::vector<JoinRecord> ModelObjectFilterStringAttributeRecord_Impl::joinRecords() const
  {
    std::vector<Rule_Clause_JoinRecord> result = JoinRecord::getJoinRecordsForRightId<Rule_Clause_JoinRecord>(this->id(), this->projectDatabase());
    return std::vector<JoinRecord>(result.begin(), result.end());
  }

  void ModelObjectFilterStringAttributeRecord_Impl::saveRow(const boost::shared_ptr<QSqlDatabase> &database)
  {
    QSqlQuery query(*database);
    this->makeUpdateByIdQuery<ModelObjectFilterStringAttributeRecord>(query);
    this->bindValues(query);
    assertExec(query);
  }

  void ModelObjectFilterStringAttributeRecord_Impl::bindValues(QSqlQuery& query) const
  {
    ModelObjectFilterAttributeRecord_Impl::bindValues(query);

    query.bindValue(ClauseRecordColumns::attributeValue, toQString(m_attributeValue));
    query.bindValue(ClauseRecordColumns::predicate, m_predicate.value());
  }

  void ModelObjectFilterStringAttributeRecord_Impl::setLastValues(const QSqlQuery& query, ProjectDatabase& projectDatabase)
  {
    ModelObjectFilterAttributeRecord_Impl::setLastValues(query, projectDatabase);

    QVariant value;
    value = query.value(ClauseRecordColumns::attributeValue);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    m_lastAttributeValue = value.toString().toStdString();

    value = query.value(ClauseRecordColumns::predicate);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    m_lastPredicate = ruleset::RulesetStringPredicate(value.toInt());
  }

  bool ModelObjectFilterStringAttributeRecord_Impl::compareValues(const QSqlQuery& query) const
  {
    bool result = true;

    result = result && ModelObjectFilterAttributeRecord_Impl::compareValues(query);

    QVariant value;
    value = query.value(ClauseRecordColumns::attributeValue);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_attributeValue == value.toString().toStdString());

    value = query.value(ClauseRecordColumns::predicate);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_predicate == ruleset::RulesetStringPredicate(value.toInt()));

    return result;
  }

  void ModelObjectFilterStringAttributeRecord_Impl::saveLastValues()
  {
    ModelObjectFilterAttributeRecord_Impl::saveLastValues();

    m_lastAttributeValue = m_attributeValue;
    m_lastPredicate = m_predicate;
  }

  void ModelObjectFilterStringAttributeRecord_Impl::revertToLastValues()
  {
    ModelObjectFilterAttributeRecord_Impl::revertToLastValues();

    m_attributeValue = m_lastAttributeValue;
    m_predicate = m_lastPredicate;
  }

} // detail

ModelObjectFilterStringAttributeRecord::ModelObjectFilterStringAttributeRecord(const openstudio::ruleset::ModelObjectFilterStringAttribute& modelObjectFilterStringAttribute, ProjectDatabase& projectDatabase)
  : ModelObjectFilterAttributeRecord(boost::shared_ptr<detail::ModelObjectFilterStringAttributeRecord_Impl>(new detail::ModelObjectFilterStringAttributeRecord_Impl(modelObjectFilterStringAttribute, projectDatabase)), projectDatabase)
{
  BOOST_ASSERT(getImpl<detail::ModelObjectFilterStringAttributeRecord_Impl>());
}

ModelObjectFilterStringAttributeRecord::ModelObjectFilterStringAttributeRecord(const QSqlQuery& query, ProjectDatabase& projectDatabase)
  : ModelObjectFilterAttributeRecord(boost::shared_ptr<detail::ModelObjectFilterStringAttributeRecord_Impl>(new detail::ModelObjectFilterStringAttributeRecord_Impl(query, projectDatabase)), projectDatabase)
{
  BOOST_ASSERT(getImpl<detail::ModelObjectFilterStringAttributeRecord_Impl>());
}

ModelObjectFilterStringAttributeRecord::ModelObjectFilterStringAttributeRecord(boost::shared_ptr<detail::ModelObjectFilterStringAttributeRecord_Impl> impl, ProjectDatabase projectDatabase)
  : ModelObjectFilterAttributeRecord(impl, projectDatabase)
{
  BOOST_ASSERT(getImpl<detail::ModelObjectFilterStringAttributeRecord_Impl>());
}

ModelObjectFilterStringAttributeRecord::ModelObjectFilterStringAttributeRecord(boost::shared_ptr<detail::ModelObjectFilterStringAttributeRecord_Impl> impl)
  : ModelObjectFilterAttributeRecord(impl)
{
  BOOST_ASSERT(getImpl<detail::ModelObjectFilterStringAttributeRecord_Impl>());
}

boost::optional<ModelObjectFilterStringAttributeRecord> ModelObjectFilterStringAttributeRecord::factoryFromQuery(
    const QSqlQuery& query, ProjectDatabase& database)
{
  OptionalModelObjectFilterStringAttributeRecord result;
  try {
    result = ModelObjectFilterStringAttributeRecord(query,database);
  }
  catch (const std::exception& e) {
    LOG(Error,"Unable to construct ModelObjectFilterStringAttributeRecord from query, because '" 
        << e.what() << "'.");
  }
  return result;
}

std::vector<ModelObjectFilterStringAttributeRecord> ModelObjectFilterStringAttributeRecord::getModelObjectFilterStringAttributeRecords(ProjectDatabase& database) {
  std::vector<ModelObjectFilterStringAttributeRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + ClauseRecord::databaseTableName() + 
      " WHERE clauseRecordType=:clauseRecordType" +
      " AND filterClauseRecordType=:filterClauseRecordType" +
      " AND modelObjectFilterClauseRecordType=:modelObjectFilterClauseRecordType" +
      " AND modelObjectFilterAttributeRecordType=:modelObjectFilterAttributeRecordType"));
  query.bindValue(":clauseRecordType",ClauseRecordType::FilterClauseRecord);
  query.bindValue(":filterClauseRecordType",FilterClauseRecordType::ModelObjectFilterClauseRecord);
  query.bindValue(":modelObjectFilterClauseRecordType", ModelObjectFilterClauseRecordType::ModelObjectFilterAttributeRecord);
  query.bindValue(":modelObjectFilterAttributeRecordType", ModelObjectFilterAttributeRecordType::ModelObjectFilterStringAttributeRecord);
  assertExec(query);
  while (query.next()) {
    result.push_back(ModelObjectFilterStringAttributeRecord(query, database));
  }

  return result;
}

boost::optional<ModelObjectFilterStringAttributeRecord> ModelObjectFilterStringAttributeRecord::getModelObjectFilterStringAttributeRecord(int id, ProjectDatabase& database) {
  boost::optional<ModelObjectFilterStringAttributeRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + ClauseRecord::databaseTableName() + 
      " WHERE clauseRecordType=:clauseRecordType" +
      " AND filterClauseRecordType=:filterClauseRecordType" +
      " AND modelObjectFilterClauseRecordType=:modelObjectFilterClauseRecordType" +
      " AND modelObjectFilterAttributeRecordType=:modelObjectFilterAttributeRecordType AND id=:id"));
  query.bindValue(":clauseRecordType",ClauseRecordType::FilterClauseRecord);
  query.bindValue(":filterClauseRecordType",FilterClauseRecordType::ModelObjectFilterClauseRecord);
  query.bindValue(":modelObjectFilterClauseRecordType", ModelObjectFilterClauseRecordType::ModelObjectFilterAttributeRecord);
  query.bindValue(":modelObjectFilterAttributeRecordType", ModelObjectFilterAttributeRecordType::ModelObjectFilterStringAttributeRecord);
  query.bindValue(":id",id);
  assertExec(query);
  if (query.first()) {
    result = ModelObjectFilterStringAttributeRecord(query, database);
  }

  return result;
}

openstudio::ruleset::ModelObjectFilterStringAttribute ModelObjectFilterStringAttributeRecord::modelObjectFilterStringAttribute() const
{
  return this->getImpl<detail::ModelObjectFilterStringAttributeRecord_Impl>()->modelObjectFilterStringAttribute();
}

} // project
} // openstudio
