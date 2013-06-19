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

#include <project/ModelObjectFilterTypeRecord.hpp>
#include <project/ModelObjectFilterTypeRecord_Impl.hpp>
#include <project/JoinRecord.hpp>
#include <project/ProjectDatabase.hpp>
#include <project/Rule_Clause_JoinRecord.hpp>
#include <project/Rule_Clause_JoinRecord_Impl.hpp>

#include <ruleset/ModelObjectFilterType.hpp>

#include <utilities/core/Assert.hpp>

#include <boost/optional/optional.hpp>
#include <boost/foreach.hpp>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

namespace openstudio {
namespace project {

namespace detail {

  ModelObjectFilterTypeRecord_Impl::ModelObjectFilterTypeRecord_Impl(const openstudio::ruleset::ModelObjectFilterType& modelObjectFilterType, const ProjectDatabase& projectDatabase)
    : ModelObjectFilterClauseRecord_Impl(modelObjectFilterType, ModelObjectFilterClauseRecordType::ModelObjectFilterTypeRecord, projectDatabase), m_iddObjectType(modelObjectFilterType.iddObjectType())
  {
  }

  ModelObjectFilterTypeRecord_Impl::ModelObjectFilterTypeRecord_Impl(const QSqlQuery& query, const ProjectDatabase& projectDatabase)
    : ModelObjectFilterClauseRecord_Impl(query, projectDatabase)
  {
    BOOST_ASSERT(query.isValid());
    BOOST_ASSERT(query.isActive());
    BOOST_ASSERT(query.isSelect());

    QVariant value;
    value = query.value(ClauseRecordColumns::iddObjectType);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    m_iddObjectType = IddObjectType(value.toString().toStdString());
  }

  openstudio::ruleset::ModelObjectFilterType ModelObjectFilterTypeRecord_Impl::modelObjectFilterType() const
  {
    openstudio::ruleset::ModelObjectFilterType result(m_iddObjectType, this->handle(), this->uuidLast());
    return result;
  }

  openstudio::ruleset::FilterClause ModelObjectFilterTypeRecord_Impl::filterClause() const
  {
    return this->modelObjectFilterType();
  }

  boost::optional<ObjectRecord> ModelObjectFilterTypeRecord_Impl::parent() const
  {
    return boost::none;
  }

  std::vector<ObjectRecord> ModelObjectFilterTypeRecord_Impl::children() const
  {
    return std::vector<ObjectRecord>();
  }

  std::vector<ObjectRecord> ModelObjectFilterTypeRecord_Impl::resources() const
  {
    return std::vector<ObjectRecord>();
  }

  std::vector<JoinRecord> ModelObjectFilterTypeRecord_Impl::joinRecords() const
  {
    std::vector<Rule_Clause_JoinRecord> result = JoinRecord::getJoinRecordsForRightId<Rule_Clause_JoinRecord>(this->id(), this->projectDatabase());
    return std::vector<JoinRecord>(result.begin(), result.end());
  }

  void ModelObjectFilterTypeRecord_Impl::saveRow(const boost::shared_ptr<QSqlDatabase> &database)
  {
    QSqlQuery query(*database);
    this->makeUpdateByIdQuery<ModelObjectFilterTypeRecord>(query);
    this->bindValues(query);
    assertExec(query);
  }

  void ModelObjectFilterTypeRecord_Impl::bindValues(QSqlQuery& query) const
  {
    ModelObjectFilterClauseRecord_Impl::bindValues(query);

    query.bindValue(ClauseRecordColumns::iddObjectType, toQString(m_iddObjectType.valueName()));
  }

  void ModelObjectFilterTypeRecord_Impl::setLastValues(const QSqlQuery& query, ProjectDatabase& projectDatabase)
  {
    ModelObjectFilterClauseRecord_Impl::setLastValues(query, projectDatabase);

    QVariant value;
    value = query.value(ClauseRecordColumns::iddObjectType);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    m_lastIddObjectType = IddObjectType(value.toString().toStdString());
  }

  bool ModelObjectFilterTypeRecord_Impl::compareValues(const QSqlQuery& query) const
  {
    bool result = true;

    result = result && ModelObjectFilterClauseRecord_Impl::compareValues(query);

    QVariant value;
    value = query.value(ClauseRecordColumns::iddObjectType);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_iddObjectType == IddObjectType(value.toString().toStdString()));

    return result;
  }

  void ModelObjectFilterTypeRecord_Impl::saveLastValues()
  {
    ModelObjectFilterClauseRecord_Impl::saveLastValues();

    m_lastIddObjectType = m_iddObjectType;  
  }

  void ModelObjectFilterTypeRecord_Impl::revertToLastValues()
  {
    ModelObjectFilterClauseRecord_Impl::revertToLastValues();

    m_iddObjectType = m_lastIddObjectType; 
  }

} // detail

ModelObjectFilterTypeRecord::ModelObjectFilterTypeRecord(const openstudio::ruleset::ModelObjectFilterType& modelObjectFilterType, ProjectDatabase& projectDatabase)
  : ModelObjectFilterClauseRecord(boost::shared_ptr<detail::ModelObjectFilterTypeRecord_Impl>(new detail::ModelObjectFilterTypeRecord_Impl(modelObjectFilterType, projectDatabase)), projectDatabase)
{
  BOOST_ASSERT(getImpl<detail::ModelObjectFilterTypeRecord_Impl>());
}

ModelObjectFilterTypeRecord::ModelObjectFilterTypeRecord(const QSqlQuery& query, ProjectDatabase& projectDatabase)
  : ModelObjectFilterClauseRecord(boost::shared_ptr<detail::ModelObjectFilterTypeRecord_Impl>(new detail::ModelObjectFilterTypeRecord_Impl(query, projectDatabase)), projectDatabase)
{
  BOOST_ASSERT(getImpl<detail::ModelObjectFilterTypeRecord_Impl>());
}

ModelObjectFilterTypeRecord::ModelObjectFilterTypeRecord(boost::shared_ptr<detail::ModelObjectFilterTypeRecord_Impl> impl, ProjectDatabase projectDatabase)
  : ModelObjectFilterClauseRecord(impl, projectDatabase)
{
  BOOST_ASSERT(getImpl<detail::ModelObjectFilterTypeRecord_Impl>());
}

ModelObjectFilterTypeRecord::ModelObjectFilterTypeRecord(boost::shared_ptr<detail::ModelObjectFilterTypeRecord_Impl> impl)
  : ModelObjectFilterClauseRecord(impl)
{
  BOOST_ASSERT(getImpl<detail::ModelObjectFilterTypeRecord_Impl>());
}

boost::optional<ModelObjectFilterTypeRecord> ModelObjectFilterTypeRecord::factoryFromQuery(
    const QSqlQuery& query, ProjectDatabase& database)
{
  OptionalModelObjectFilterTypeRecord result;
  try {
    result = ModelObjectFilterTypeRecord(query,database);
  }
  catch (const std::exception& e) {
    LOG(Error,"Unable to construct ModelObjectFilterTypeRecord from query, because '" 
        << e.what() << "'.");
  }
  return result;
}

std::vector<ModelObjectFilterTypeRecord> ModelObjectFilterTypeRecord::getModelObjectFilterTypeRecords(ProjectDatabase& database) {
  std::vector<ModelObjectFilterTypeRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + ClauseRecord::databaseTableName() + 
      " WHERE clauseRecordType=:clauseRecordType" +
      " AND filterClauseRecordType=:filterClauseRecordType" +
      " AND modelObjectFilterClauseRecordType=:modelObjectFilterClauseRecordType"));
  query.bindValue(":clauseRecordType",ClauseRecordType::FilterClauseRecord);
  query.bindValue(":filterClauseRecordType",FilterClauseRecordType::ModelObjectFilterClauseRecord);
  query.bindValue(":modelObjectFilterClauseRecordType", ModelObjectFilterClauseRecordType::ModelObjectFilterTypeRecord);
  assertExec(query);
  while (query.next()) {
    result.push_back(ModelObjectFilterTypeRecord(query, database));
  }

  return result;
}

boost::optional<ModelObjectFilterTypeRecord> ModelObjectFilterTypeRecord::getModelObjectFilterTypeRecord(int id, ProjectDatabase& database) {
  boost::optional<ModelObjectFilterTypeRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + ClauseRecord::databaseTableName() + 
      " WHERE clauseRecordType=:clauseRecordType" +
      " AND filterClauseRecordType=:filterClauseRecordType" +
      " AND modelObjectFilterClauseRecordType=:modelObjectFilterClauseRecordType AND id=:id"));
  query.bindValue(":clauseRecordType",ClauseRecordType::FilterClauseRecord);
  query.bindValue(":filterClauseRecordType",FilterClauseRecordType::ModelObjectFilterClauseRecord);
  query.bindValue(":modelObjectFilterClauseRecordType", ModelObjectFilterClauseRecordType::ModelObjectFilterTypeRecord);
  query.bindValue(":id",id);
  assertExec(query);
  if (query.first()) {
    result = ModelObjectFilterTypeRecord(query, database);
  }

  return result;
}

openstudio::ruleset::ModelObjectFilterType ModelObjectFilterTypeRecord::modelObjectFilterType() const
{
  return this->getImpl<detail::ModelObjectFilterTypeRecord_Impl>()->modelObjectFilterType();
}

} // project
} // openstudio
