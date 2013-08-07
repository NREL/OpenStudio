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

#include <project/ModelRulesetContinuousVariableRecord.hpp>
#include <project/ModelRulesetContinuousVariableRecord_Impl.hpp>

#include <project/JoinRecord.hpp>
#include <project/ModelRulesetRecord.hpp>
#include <project/ProblemRecord.hpp>
#include <project/FunctionRecord.hpp>

#include <analysis/Variable.hpp>
#include <analysis/Variable_Impl.hpp>
#include <analysis/ContinuousVariable.hpp>
#include <analysis/ContinuousVariable_Impl.hpp>
#include <analysis/ModelRulesetContinuousVariable.hpp>

#include <ruleset/ModelRuleset.hpp>
#include <ruleset/ModelRule.hpp>
#include <ruleset/ModelObjectFilterClause.hpp>
#include <ruleset/ModelObjectFilterClause_Impl.hpp>
#include <ruleset/ModelObjectActionClause.hpp>

#include <utilities/core/Assert.hpp>
#include <utilities/core/Containers.hpp>

namespace openstudio {
namespace project {

namespace detail {

  ModelRulesetContinuousVariableRecord_Impl::ModelRulesetContinuousVariableRecord_Impl(
      const analysis::ModelRulesetContinuousVariable& modelRulesetContinuousVariable,
      ProblemRecord& problemRecord,
      int variableVectorIndex)
    : ContinuousVariableRecord_Impl(modelRulesetContinuousVariable,
                                    ContinuousVariableRecordType::ModelRulesetContinuousVariableRecord,
                                    problemRecord,
                                    variableVectorIndex),
      m_attributeName(modelRulesetContinuousVariable.attributeName())
  {
    LOG(Trace,"Constructing ModelRulesetContinuousVariableRecord_Impl for "
        << modelRulesetContinuousVariable.name() << ".");
    // serialize ruleset
    ruleset::ModelRuleset ruleset = modelRulesetContinuousVariable.getModelRuleset(boost::none);
    ProjectDatabase database = projectDatabase();
    LOG(Trace,"Successfully retrieved ModelRuleset. Now creating ModelRulesetRecord.");
    ModelRulesetRecord rulesetRecord = ModelRulesetRecord(ruleset,database);
    m_modelRulesetRecordId = rulesetRecord.id();
  }

  ModelRulesetContinuousVariableRecord_Impl::ModelRulesetContinuousVariableRecord_Impl(
      const analysis::ModelRulesetContinuousVariable& modelRulesetContinuousVariable,
      FunctionRecord& functionRecord,
      int variableVectorIndex,
      boost::optional<double> functionCoefficient)
    : ContinuousVariableRecord_Impl(modelRulesetContinuousVariable,
                                    ContinuousVariableRecordType::ModelRulesetContinuousVariableRecord,
                                    functionRecord,
                                    variableVectorIndex,
                                    functionCoefficient),
      m_attributeName(modelRulesetContinuousVariable.attributeName())
  {
    // serialize ruleset
    ruleset::ModelRuleset ruleset = modelRulesetContinuousVariable.getModelRuleset(boost::none);
    ProjectDatabase database = projectDatabase();
    ModelRulesetRecord rulesetRecord = ModelRulesetRecord(ruleset,database);
    m_modelRulesetRecordId = rulesetRecord.id();
  }

  ModelRulesetContinuousVariableRecord_Impl::ModelRulesetContinuousVariableRecord_Impl(
      const QSqlQuery& query, ProjectDatabase& database)
    : ContinuousVariableRecord_Impl(query, database)
  {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    QVariant value;

    value = query.value(ModelRulesetContinuousVariableRecord::ColumnsType::attributeName);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_attributeName = value.toString().toStdString();

    value = query.value(ModelRulesetContinuousVariableRecord::ColumnsType::rulesetRecordId);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_modelRulesetRecordId = value.toInt();
  }

  std::vector<ObjectRecord> ModelRulesetContinuousVariableRecord_Impl::children() const {
    ObjectRecordVector result = InputVariableRecord_Impl::children();
    result.push_back(modelRulesetRecord());
    return result;
  }

  std::vector<ObjectRecord> ModelRulesetContinuousVariableRecord_Impl::resources() const {
    ObjectRecordVector result;
    return result;
  }

  void ModelRulesetContinuousVariableRecord_Impl::saveRow(const boost::shared_ptr<QSqlDatabase> &database)
  {
    QSqlQuery query(*database);
    this->makeUpdateByIdQuery<ModelRulesetContinuousVariableRecord>(query);
    this->bindValues(query);
    assertExec(query);
  }

  ModelRulesetRecord ModelRulesetContinuousVariableRecord_Impl::modelRulesetRecord() const {
    ProjectDatabase database = projectDatabase();
    return ModelRulesetRecord::getModelRulesetRecord(m_modelRulesetRecordId,database).get();
  }

  analysis::Variable ModelRulesetContinuousVariableRecord_Impl::variable() const {
    return modelRulesetContinuousVariable().cast<analysis::Variable>();
  }

  analysis::InputVariable ModelRulesetContinuousVariableRecord_Impl::inputVariable() const {
    return modelRulesetContinuousVariable().cast<analysis::InputVariable>();
  }

  analysis::ContinuousVariable ModelRulesetContinuousVariableRecord_Impl::continuousVariable() const {
    return modelRulesetContinuousVariable().cast<analysis::ContinuousVariable>();
  }

  analysis::ModelRulesetContinuousVariable
  ModelRulesetContinuousVariableRecord_Impl::modelRulesetContinuousVariable() const
  {
    ruleset::ModelRuleset ruleset = modelRulesetRecord().modelRuleset();
    ruleset::ModelRuleVector rules = ruleset.rules();
    OS_ASSERT(rules.size() == 1u);
    ruleset::ModelObjectFilterClauseVector modelObjectFilters =
        castVector<ruleset::ModelObjectFilterClause>(rules[0].filters());
    OS_ASSERT(rules[0].actions().empty());
    return analysis::ModelRulesetContinuousVariable(handle(),
                                                    uuidLast(),
                                                    name(),
                                                    displayName(),
                                                    description(),
                                                    uncertaintyDescription(),
                                                    modelObjectFilters,
                                                    m_attributeName,
                                                    minimum(),
                                                    maximum(),
                                                    increment(),
                                                    nSteps());
  }

  void ModelRulesetContinuousVariableRecord_Impl::bindValues(QSqlQuery& query) const {
    ContinuousVariableRecord_Impl::bindValues(query);

    query.bindValue(ModelRulesetContinuousVariableRecord::ColumnsType::attributeName,
                    toQString(m_attributeName));
    query.bindValue(ModelRulesetContinuousVariableRecord::ColumnsType::rulesetRecordId,
                    m_modelRulesetRecordId);
  }

  void ModelRulesetContinuousVariableRecord_Impl::setLastValues(const QSqlQuery& query,
                                                                ProjectDatabase& projectDatabase)
  {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    ContinuousVariableRecord_Impl::setLastValues(query,projectDatabase);

    QVariant value;

    value = query.value(ModelRulesetContinuousVariableRecord::ColumnsType::attributeName);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_lastAttributeName = value.toString().toStdString();

    value = query.value(ModelRulesetContinuousVariableRecord::ColumnsType::rulesetRecordId);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_lastModelRulesetRecordId = value.toInt();
  }

  bool ModelRulesetContinuousVariableRecord_Impl::compareValues(const QSqlQuery& query) const {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    bool result = ContinuousVariableRecord_Impl::compareValues(query);

    QVariant value;

    value = query.value(ModelRulesetContinuousVariableRecord::ColumnsType::attributeName);
    OS_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_attributeName == value.toString().toStdString());

    value = query.value(ModelRulesetContinuousVariableRecord::ColumnsType::rulesetRecordId);
    OS_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_modelRulesetRecordId == value.toInt());

    return result;
  }

  void ModelRulesetContinuousVariableRecord_Impl::saveLastValues() {
    ContinuousVariableRecord_Impl::saveLastValues();

    m_lastAttributeName = m_attributeName;
    m_lastModelRulesetRecordId = m_modelRulesetRecordId;
  }

  void ModelRulesetContinuousVariableRecord_Impl::revertToLastValues() {
    ContinuousVariableRecord_Impl::revertToLastValues();

    m_attributeName = m_lastAttributeName;
    m_modelRulesetRecordId = m_lastModelRulesetRecordId;
  }

} // detail

ModelRulesetContinuousVariableRecord::ModelRulesetContinuousVariableRecord(
    const analysis::ModelRulesetContinuousVariable& modelRulesetContinuousVariable,
    ProblemRecord& problemRecord,
    int variableVectorIndex)
  : ContinuousVariableRecord(boost::shared_ptr<detail::ModelRulesetContinuousVariableRecord_Impl>(
        new detail::ModelRulesetContinuousVariableRecord_Impl(modelRulesetContinuousVariable,
                                                              problemRecord,
                                                              variableVectorIndex)),
        problemRecord.projectDatabase(),
        modelRulesetContinuousVariable)
{
  OS_ASSERT(getImpl<detail::ModelRulesetContinuousVariableRecord_Impl>());
}

ModelRulesetContinuousVariableRecord::ModelRulesetContinuousVariableRecord(
    const analysis::ModelRulesetContinuousVariable& modelRulesetContinuousVariable,
    FunctionRecord& functionRecord,
    int variableVectorIndex,
    boost::optional<double> functionCoefficient)
  : ContinuousVariableRecord(boost::shared_ptr<detail::ModelRulesetContinuousVariableRecord_Impl>(
        new detail::ModelRulesetContinuousVariableRecord_Impl(modelRulesetContinuousVariable,
                                                              functionRecord,
                                                              variableVectorIndex,
                                                              functionCoefficient)),
        functionRecord.projectDatabase(),
        modelRulesetContinuousVariable)
{
  OS_ASSERT(getImpl<detail::ModelRulesetContinuousVariableRecord_Impl>());
}

ModelRulesetContinuousVariableRecord::ModelRulesetContinuousVariableRecord(const QSqlQuery& query,
                                                                           ProjectDatabase& database)
  : ContinuousVariableRecord(boost::shared_ptr<detail::ModelRulesetContinuousVariableRecord_Impl>(
        new detail::ModelRulesetContinuousVariableRecord_Impl(query, database)),
        database,
        analysis::OptionalContinuousVariable())
{
  OS_ASSERT(getImpl<detail::ModelRulesetContinuousVariableRecord_Impl>());
}

ModelRulesetContinuousVariableRecord::ModelRulesetContinuousVariableRecord(boost::shared_ptr<detail::ModelRulesetContinuousVariableRecord_Impl> impl,
                                                                           ProjectDatabase database)
  : ContinuousVariableRecord(impl, database, analysis::OptionalContinuousVariable())
{
  OS_ASSERT(getImpl<detail::ModelRulesetContinuousVariableRecord_Impl>());
}

boost::optional<ModelRulesetContinuousVariableRecord> ModelRulesetContinuousVariableRecord::factoryFromQuery(
    const QSqlQuery& query, ProjectDatabase& database)
{
  OptionalModelRulesetContinuousVariableRecord result;
  try {
    result = ModelRulesetContinuousVariableRecord(query,database);
  }
  catch (const std::exception& e) {
    LOG(Error,"Unable to construct ModelRulesetContinuousVariableRecord from query, because '"
        << e.what() << "'.");
  }
  return result;
}

std::vector<ModelRulesetContinuousVariableRecord>
ModelRulesetContinuousVariableRecord::getModelRulesetContinuousVariableRecords(
    ProjectDatabase& database)
{
  std::vector<ModelRulesetContinuousVariableRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + VariableRecord::databaseTableName() +
      " WHERE variableRecordType=:variableRecordType AND " +
      "inputVariableRecordType=:inputVariableRecordType AND " +
      "continuousVariableRecordType=:continuousVariableRecordType"));
  query.bindValue(":variableRecordType",VariableRecordType::InputVariableRecord);
  query.bindValue(":inputVariableRecordType",InputVariableRecordType::ContinuousVariableRecord);
  query.bindValue(":continuousVariableRecordType",
                  ContinuousVariableRecordType::ModelRulesetContinuousVariableRecord);
  assertExec(query);
  while (query.next()) {
    result.push_back(ModelRulesetContinuousVariableRecord(query, database));
  }

  return result;
}

boost::optional<ModelRulesetContinuousVariableRecord> ModelRulesetContinuousVariableRecord::getModelRulesetContinuousVariableRecord(int id, ProjectDatabase& database) {
  boost::optional<ModelRulesetContinuousVariableRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + VariableRecord::databaseTableName() +
      " WHERE variableRecordType=:variableRecordType AND " +
      "inputVariableRecordType=:inputVariableRecordType AND " +
      "continuousVariableRecordType=:continuousVariableRecordType AND id=:id"));
  query.bindValue(":variableRecordType",VariableRecordType::InputVariableRecord);
  query.bindValue(":inputVariableRecordType",InputVariableRecordType::ContinuousVariableRecord);
  query.bindValue(":continuousVariableRecordType",
                  ContinuousVariableRecordType::ModelRulesetContinuousVariableRecord);
  query.bindValue(":id",id);
  assertExec(query);
  if (query.first()) {
    result = ModelRulesetContinuousVariableRecord(query, database);
  }

  return result;
}

ModelRulesetRecord ModelRulesetContinuousVariableRecord::modelRulesetRecord() const {
  return getImpl<detail::ModelRulesetContinuousVariableRecord_Impl>()->modelRulesetRecord();
}

analysis::ModelRulesetContinuousVariable
ModelRulesetContinuousVariableRecord::modelRulesetContinuousVariable() const
{
  return getImpl<detail::ModelRulesetContinuousVariableRecord_Impl>()->modelRulesetContinuousVariable();
}

/// @cond
ModelRulesetContinuousVariableRecord::ModelRulesetContinuousVariableRecord(boost::shared_ptr<detail::ModelRulesetContinuousVariableRecord_Impl> impl)
  : ContinuousVariableRecord(impl)
{}
/// @endcond

} // project
} // openstudio

