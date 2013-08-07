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

#include <project/ModelRulesetPerturbationRecord.hpp>
#include <project/ModelRulesetPerturbationRecord_Impl.hpp>
#include <project/DiscreteVariableRecord.hpp>
#include <project/JoinRecord.hpp>
#include <project/ProjectDatabase.hpp>
#include <project/ModelRulesetRecord.hpp>

#include <ruleset/ModelRuleset.hpp>

#include <analysis/DiscretePerturbation.hpp>
#include <analysis/DiscretePerturbation_Impl.hpp>
#include <analysis/ModelRulesetPerturbation.hpp>

#include <utilities/core/Assert.hpp>
#include <utilities/core/Checksum.hpp>

#include <boost/foreach.hpp>

#include <sstream>

#include <QSqlQuery>
#include <QSqlError>

namespace openstudio {
namespace project {

namespace detail {

  ModelRulesetPerturbationRecord_Impl::ModelRulesetPerturbationRecord_Impl(const analysis::ModelRulesetPerturbation& modelRulesetPerturbation,
                                                                           DiscreteVariableRecord& discreteVariableRecord,
                                                                           int perturbationVectorIndex)
    : DiscretePerturbationRecord_Impl(modelRulesetPerturbation,
                                      DiscretePerturbationRecordType::ModelRulesetPerturbationRecord,
                                      discreteVariableRecord,
                                      perturbationVectorIndex)
  {
    ProjectDatabase database = discreteVariableRecord.projectDatabase();
    ModelRulesetRecord modelRulesetRecord(modelRulesetPerturbation.ruleset(), database);
    m_modelRulesetRecordId = modelRulesetRecord.id();
  }

  ModelRulesetPerturbationRecord_Impl::ModelRulesetPerturbationRecord_Impl(const QSqlQuery& query, ProjectDatabase& database)
    : DiscretePerturbationRecord_Impl(query, database)
  {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    QVariant value;
    value = query.value(DiscretePerturbationRecordColumns::modelRulesetRecordId);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_modelRulesetRecordId = value.toInt();
  }

  std::vector<ObjectRecord> ModelRulesetPerturbationRecord_Impl::children() const
  {
    std::vector<ObjectRecord> result;
    result.push_back(this->modelRulesetRecord());
    return result;
  }

  std::vector<ObjectRecord> ModelRulesetPerturbationRecord_Impl::resources() const
  {
    return std::vector<ObjectRecord>();
  }

  void ModelRulesetPerturbationRecord_Impl::saveRow(const boost::shared_ptr<QSqlDatabase> &database)
  {
    QSqlQuery query(*database);
    this->makeUpdateByIdQuery<ModelRulesetPerturbationRecord>(query);
    this->bindValues(query);
    assertExec(query);
  }

  ModelRulesetRecord ModelRulesetPerturbationRecord_Impl::modelRulesetRecord() const
  {
    ProjectDatabase projectDatabase = this->projectDatabase();
    boost::optional<ModelRulesetRecord> result = ModelRulesetRecord::getModelRulesetRecord(m_modelRulesetRecordId, projectDatabase);
    OS_ASSERT(result);
    return *result;
  }

  analysis::DiscretePerturbation ModelRulesetPerturbationRecord_Impl::discretePerturbation() const {
    return modelRulesetPerturbation().cast<analysis::DiscretePerturbation>();
  }

  analysis::ModelRulesetPerturbation ModelRulesetPerturbationRecord_Impl::modelRulesetPerturbation() const
  {
    return analysis::ModelRulesetPerturbation(handle(),
                                              uuidLast(),
                                              name(),
                                              displayName(),
                                              description(),
                                              isSelected(),
                                              modelRulesetRecord().modelRuleset());
  }

  void ModelRulesetPerturbationRecord_Impl::bindValues(QSqlQuery& query) const
  {
    DiscretePerturbationRecord_Impl::bindValues(query);

    query.bindValue(DiscretePerturbationRecordColumns::modelRulesetRecordId, m_modelRulesetRecordId);
  }

  void ModelRulesetPerturbationRecord_Impl::setLastValues(const QSqlQuery& query, ProjectDatabase& projectDatabase)
  {
    DiscretePerturbationRecord_Impl::setLastValues(query, projectDatabase);

    QVariant value;
    value = query.value(DiscretePerturbationRecordColumns::modelRulesetRecordId);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_lastModelRulesetRecordId = value.toInt();
  }

  bool ModelRulesetPerturbationRecord_Impl::compareValues(const QSqlQuery& query) const
  {
    bool result = true;

    result = result && DiscretePerturbationRecord_Impl::compareValues(query);

    QVariant value;
    value = query.value(DiscretePerturbationRecordColumns::modelRulesetRecordId);
    OS_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_modelRulesetRecordId ==value.toInt());

    return result;
  }

  void ModelRulesetPerturbationRecord_Impl::saveLastValues()
  {
    DiscretePerturbationRecord_Impl::saveLastValues();

    m_lastModelRulesetRecordId = m_modelRulesetRecordId;
  }

  void ModelRulesetPerturbationRecord_Impl::revertToLastValues()
  {
    DiscretePerturbationRecord_Impl::revertToLastValues();

    m_modelRulesetRecordId = m_lastModelRulesetRecordId;
  }

} // detail

ModelRulesetPerturbationRecord::ModelRulesetPerturbationRecord(const analysis::ModelRulesetPerturbation& modelRulesetPerturbation,
                                                               DiscreteVariableRecord& discreteVariableRecord,
                                                               int perturbationVectorIndex)
  : DiscretePerturbationRecord(boost::shared_ptr<detail::ModelRulesetPerturbationRecord_Impl>(
        new detail::ModelRulesetPerturbationRecord_Impl(modelRulesetPerturbation,
                                                        discreteVariableRecord,
                                                        perturbationVectorIndex)),
        discreteVariableRecord.projectDatabase())
{
  OS_ASSERT(getImpl<detail::ModelRulesetPerturbationRecord_Impl>());
}

ModelRulesetPerturbationRecord::ModelRulesetPerturbationRecord(const QSqlQuery& query, ProjectDatabase& database)
  : DiscretePerturbationRecord(boost::shared_ptr<detail::ModelRulesetPerturbationRecord_Impl>(
        new detail::ModelRulesetPerturbationRecord_Impl(query, database)),
        database)
{
  OS_ASSERT(getImpl<detail::ModelRulesetPerturbationRecord_Impl>());
}

ModelRulesetPerturbationRecord::ModelRulesetPerturbationRecord(boost::shared_ptr<detail::ModelRulesetPerturbationRecord_Impl> impl, ProjectDatabase database)
  : DiscretePerturbationRecord(impl, database)
{
  OS_ASSERT(getImpl<detail::ModelRulesetPerturbationRecord_Impl>());
}

ModelRulesetPerturbationRecord::ModelRulesetPerturbationRecord(boost::shared_ptr<detail::ModelRulesetPerturbationRecord_Impl> impl)
  : DiscretePerturbationRecord(impl)
{
  OS_ASSERT(getImpl<detail::ModelRulesetPerturbationRecord_Impl>());
}

boost::optional<ModelRulesetPerturbationRecord> ModelRulesetPerturbationRecord::factoryFromQuery(
    const QSqlQuery& query, ProjectDatabase& database)
{
  OptionalModelRulesetPerturbationRecord result;
  try {
    result = ModelRulesetPerturbationRecord(query,database);
  }
  catch (const std::exception& e) {
    LOG(Error,"Unable to construct ModelRulesetPerturbationRecord from query, because '"
        << e.what() << "'.");
  }
  return result;
}

std::vector<ModelRulesetPerturbationRecord> ModelRulesetPerturbationRecord::getModelRulesetPerturbationRecords(ProjectDatabase& database)
{
  std::vector<ModelRulesetPerturbationRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + DiscretePerturbationRecord::databaseTableName() + " WHERE discretePerturbationRecordType=:discretePerturbationRecordType"));
  query.bindValue(":discretePerturbationRecordType", DiscretePerturbationRecordType::ModelRulesetPerturbationRecord);
  assertExec(query);
  while (query.next()) {
    result.push_back(ModelRulesetPerturbationRecord(query,database));
  }

  return result;
}

boost::optional<ModelRulesetPerturbationRecord> ModelRulesetPerturbationRecord::getModelRulesetPerturbationRecord(int id, ProjectDatabase& database)
{
  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + ModelRulesetPerturbationRecord::databaseTableName() + " WHERE discretePerturbationRecordType=:discretePerturbationRecordType AND id=:id"));
  query.bindValue(":discretePerturbationRecordType", DiscretePerturbationRecordType::ModelRulesetPerturbationRecord);
  query.bindValue(":id", id);
  assertExec(query);
  if (query.first()) {
    return ModelRulesetPerturbationRecord(query,database);
  }

  return boost::none;
}

ModelRulesetRecord ModelRulesetPerturbationRecord::modelRulesetRecord() const
{
  return getImpl<detail::ModelRulesetPerturbationRecord_Impl>()->modelRulesetRecord();
}

analysis::ModelRulesetPerturbation ModelRulesetPerturbationRecord::modelRulesetPerturbation() const
{
  return getImpl<detail::ModelRulesetPerturbationRecord_Impl>()->modelRulesetPerturbation();
}

} // project
} // openstudio

