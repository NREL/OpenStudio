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

#include <project/ModelObjectFilterRelationshipRecord.hpp>
#include <project/ModelObjectFilterRelationshipRecord_Impl.hpp>
#include <project/JoinRecord.hpp>
#include <project/ProjectDatabase.hpp>
#include <project/Rule_Clause_JoinRecord.hpp>
#include <project/Rule_Clause_JoinRecord_Impl.hpp>

#include <ruleset/ModelObjectFilterRelationship.hpp>
#include <ruleset/ModelObjectFilterRelationship_Impl.hpp>

#include <utilities/core/Assert.hpp>

namespace openstudio {
namespace project {

namespace detail {

  ModelObjectFilterRelationshipRecord_Impl::ModelObjectFilterRelationshipRecord_Impl(
      const ruleset::ModelObjectFilterRelationship& modelObjectFilterRelationship, 
      ProjectDatabase& database)
    : ModelObjectFilterClauseRecord_Impl(modelObjectFilterRelationship, 
                                         ModelObjectFilterClauseRecordType(ModelObjectFilterClauseRecordType::ModelObjectFilterRelationshipRecord),
                                         database),
      m_relationshipName(modelObjectFilterRelationship.relationshipName())
  {}

  ModelObjectFilterRelationshipRecord_Impl::ModelObjectFilterRelationshipRecord_Impl(const QSqlQuery& query, ProjectDatabase& database)
    : ModelObjectFilterClauseRecord_Impl(query, database)
  {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    QVariant value;

    value = query.value(ModelObjectFilterRelationshipRecord::ColumnsType::relationshipName);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_relationshipName = value.toString().toStdString();
  }

  boost::optional<ObjectRecord> ModelObjectFilterRelationshipRecord_Impl::parent() const {
    return boost::none;
  }

  std::vector<ObjectRecord> ModelObjectFilterRelationshipRecord_Impl::children() const {
    ObjectRecordVector result;
    return result;
  }

  std::vector<ObjectRecord> ModelObjectFilterRelationshipRecord_Impl::resources() const {
    ObjectRecordVector result;
    return result;
  }

  std::vector<JoinRecord> ModelObjectFilterRelationshipRecord_Impl::joinRecords() const {
    std::vector<Rule_Clause_JoinRecord> result = JoinRecord::getJoinRecordsForRightId<Rule_Clause_JoinRecord>(this->id(), this->projectDatabase());
    return std::vector<JoinRecord>(result.begin(), result.end());
  }

  void ModelObjectFilterRelationshipRecord_Impl::saveRow(const boost::shared_ptr<QSqlDatabase> &database)
  {
    QSqlQuery query(*database);
    this->makeUpdateByIdQuery<ModelObjectFilterRelationshipRecord>(query);
    this->bindValues(query);
    assertExec(query);
  }

  openstudio::ruleset::FilterClause ModelObjectFilterRelationshipRecord_Impl::filterClause() const {
    return modelObjectFilterRelationship().cast<ruleset::FilterClause>();
  }

  std::string ModelObjectFilterRelationshipRecord_Impl::relationshipName() const {
    return m_relationshipName;
  }

  ruleset::ModelObjectFilterRelationship ModelObjectFilterRelationshipRecord_Impl::modelObjectFilterRelationship() const 
  {
    return ruleset::ModelObjectFilterRelationship(m_relationshipName,handle(),uuidLast());
  }

  void ModelObjectFilterRelationshipRecord_Impl::bindValues(QSqlQuery& query) const {
    ModelObjectFilterClauseRecord_Impl::bindValues(query);

    query.bindValue(ModelObjectFilterRelationshipRecord::ColumnsType::relationshipName,toQString(m_relationshipName));
  }

  void ModelObjectFilterRelationshipRecord_Impl::setLastValues(const QSqlQuery& query, ProjectDatabase& projectDatabase) {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    ModelObjectFilterClauseRecord_Impl::setLastValues(query,projectDatabase);

    QVariant value;

    value = query.value(ModelObjectFilterRelationshipRecord::ColumnsType::relationshipName);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_lastRelationshipName = value.toString().toStdString();
  }

  bool ModelObjectFilterRelationshipRecord_Impl::compareValues(const QSqlQuery& query) const {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    bool result = ModelObjectFilterClauseRecord_Impl::compareValues(query);

    QVariant value;

    value = query.value(ModelObjectFilterRelationshipRecord::ColumnsType::relationshipName);
    OS_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_relationshipName == value.toString().toStdString());

    return result;
  }

  void ModelObjectFilterRelationshipRecord_Impl::saveLastValues() {
    ModelObjectFilterClauseRecord_Impl::saveLastValues();

    m_lastRelationshipName = m_relationshipName;
  }

  void ModelObjectFilterRelationshipRecord_Impl::revertToLastValues() {
    ModelObjectFilterClauseRecord_Impl::revertToLastValues();

    m_relationshipName = m_lastRelationshipName;
  }

} // detail

ModelObjectFilterRelationshipRecord::ModelObjectFilterRelationshipRecord(
    const ruleset::ModelObjectFilterRelationship& modelObjectFilterRelationship, 
    ProjectDatabase& database)
  : ModelObjectFilterClauseRecord(boost::shared_ptr<detail::ModelObjectFilterRelationshipRecord_Impl>(
        new detail::ModelObjectFilterRelationshipRecord_Impl(modelObjectFilterRelationship, database)),
        database)
{
  OS_ASSERT(getImpl<detail::ModelObjectFilterRelationshipRecord_Impl>());
  LOG(Trace,"ModelObjectFilterRelationshipRecord_Impl has been constructed.");
}

ModelObjectFilterRelationshipRecord::ModelObjectFilterRelationshipRecord(const QSqlQuery& query, ProjectDatabase& database)
  : ModelObjectFilterClauseRecord(boost::shared_ptr<detail::ModelObjectFilterRelationshipRecord_Impl>(
        new detail::ModelObjectFilterRelationshipRecord_Impl(query, database)),
        database)
{
  OS_ASSERT(getImpl<detail::ModelObjectFilterRelationshipRecord_Impl>());
}

ModelObjectFilterRelationshipRecord::ModelObjectFilterRelationshipRecord(boost::shared_ptr<detail::ModelObjectFilterRelationshipRecord_Impl> impl,
                                                                         ProjectDatabase database)
  : ModelObjectFilterClauseRecord(impl, database)
{
  OS_ASSERT(getImpl<detail::ModelObjectFilterRelationshipRecord_Impl>());
}

boost::optional<ModelObjectFilterRelationshipRecord> ModelObjectFilterRelationshipRecord::factoryFromQuery(const QSqlQuery& query, ProjectDatabase& database)
{
  OptionalModelObjectFilterRelationshipRecord result;

  try {
    result = ModelObjectFilterRelationshipRecord(query,database);
  }
  catch (const std::exception& e) {
    LOG(Error,"Unable to construct ModelObjectFilterRelationshipRecord from query, because '"
        << e.what() << "'.");
  }

  return result;
}

std::vector<ModelObjectFilterRelationshipRecord> ModelObjectFilterRelationshipRecord::getModelObjectFilterRelationshipRecords(ProjectDatabase& database) {
  std::vector<ModelObjectFilterRelationshipRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + ClauseRecord::databaseTableName() + 
      " WHERE clauseRecordType=:clauseRecordType" +
      " AND filterClauseRecordType=:filterClauseRecordType" +
      " AND modelObjectFilterClauseRecordType=:modelObjectFilterClauseRecordType"));
  query.bindValue(":clauseRecordType",ClauseRecordType::FilterClauseRecord);
  query.bindValue(":filterClauseRecordType",FilterClauseRecordType::ModelObjectFilterClauseRecord);
  query.bindValue(":modelObjectFilterClauseRecordType", ModelObjectFilterClauseRecordType::ModelObjectFilterRelationshipRecord);
  assertExec(query);
  while (query.next()) {
    result.push_back(ModelObjectFilterRelationshipRecord(query, database));
  }

  return result;
}

boost::optional<ModelObjectFilterRelationshipRecord> ModelObjectFilterRelationshipRecord::getModelObjectFilterRelationshipRecord(int id, ProjectDatabase& database) {
  boost::optional<ModelObjectFilterRelationshipRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + ClauseRecord::databaseTableName() + 
      " WHERE clauseRecordType=:clauseRecordType" +
      " AND filterClauseRecordType=:filterClauseRecordType" +
      " AND modelObjectFilterClauseRecordType=:modelObjectFilterClauseRecordType AND id=:id"));
  query.bindValue(":clauseRecordType",ClauseRecordType::FilterClauseRecord);
  query.bindValue(":filterClauseRecordType",FilterClauseRecordType::ModelObjectFilterClauseRecord);
  query.bindValue(":modelObjectFilterClauseRecordType", ModelObjectFilterClauseRecordType::ModelObjectFilterRelationshipRecord);
  query.bindValue(":id",id);
  assertExec(query);
  if (query.first()) {
    result = ModelObjectFilterRelationshipRecord(query, database);
  }

  return result;
}

std::string ModelObjectFilterRelationshipRecord::relationshipName() const {
  return getImpl<detail::ModelObjectFilterRelationshipRecord_Impl>()->relationshipName();
}

ruleset::ModelObjectFilterRelationship ModelObjectFilterRelationshipRecord::modelObjectFilterRelationship() const {
  return getImpl<detail::ModelObjectFilterRelationshipRecord_Impl>()->modelObjectFilterRelationship();
}

/// @cond
ModelObjectFilterRelationshipRecord::ModelObjectFilterRelationshipRecord(boost::shared_ptr<detail::ModelObjectFilterRelationshipRecord_Impl> impl)
  : ModelObjectFilterClauseRecord(impl)
{}
/// @endcond

} // project
} // openstudio

