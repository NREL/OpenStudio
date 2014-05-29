/**********************************************************************
 *  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
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

#include "InputVariableRecord.hpp"
#include "InputVariableRecord_Impl.hpp"

#include "AttributeRecord.hpp"
#include "AttributeRecord_Impl.hpp"
#include "ContinuousVariableRecord.hpp"
#include "DiscreteVariableRecord.hpp"
#include "JoinRecord.hpp"
#include "ProblemRecord.hpp"

#include "../analysis/ContinuousVariable.hpp"
#include "../analysis/ContinuousVariable_Impl.hpp"
#include "../analysis/DiscreteVariable.hpp"
#include "../analysis/DiscreteVariable_Impl.hpp"
#include "../analysis/GenericUncertaintyDescription.hpp"
#include "../analysis/UncertaintyDescription.hpp"
#include "../analysis/UncertaintyDescription_Impl.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace project {

namespace detail {

  InputVariableRecord_Impl::InputVariableRecord_Impl(const analysis::InputVariable& inputVariable,
                                                     const InputVariableRecordType& inputVariableRecordType,
                                                     ProblemRecord& problemRecord,
                                                     int workflowIndex)
    : VariableRecord_Impl(inputVariable,
                          VariableRecordType::InputVariableRecord,
                          problemRecord,
                          workflowIndex),
      m_problemRecordId(problemRecord.id()),
      m_inputVariableRecordType(inputVariableRecordType)
  {
    if (analysis::OptionalUncertaintyDescription oUDesc = inputVariable.uncertaintyDescription()) {
      m_uncertaintyDescriptionType = oUDesc->type();
    }
  }

  InputVariableRecord_Impl::InputVariableRecord_Impl(const analysis::InputVariable& inputVariable,
                                                     const InputVariableRecordType& inputVariableRecordType,
                                                     FunctionRecord& functionRecord,
                                                     int variableVectorIndex,
                                                     boost::optional<double> functionCoefficient)
    : VariableRecord_Impl(inputVariable,
                          VariableRecordType::InputVariableRecord,
                          functionRecord,
                          variableVectorIndex,
                          functionCoefficient),
      m_inputVariableRecordType(inputVariableRecordType)
  {
    if (analysis::OptionalUncertaintyDescription oUDesc = inputVariable.uncertaintyDescription()) {
      m_uncertaintyDescriptionType = oUDesc->type();
    }
  }

  InputVariableRecord_Impl::InputVariableRecord_Impl(const QSqlQuery& query, ProjectDatabase& database)
    : VariableRecord_Impl(query, database)
  {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    QVariant value;

    value = query.value(VariableRecordColumns::problemRecordId);
    if (value.isValid() && !value.isNull()) {
      m_problemRecordId = value.toInt();
    }

    value = query.value(VariableRecordColumns::inputVariableRecordType);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_inputVariableRecordType = InputVariableRecordType(value.toInt());

    value = query.value(VariableRecordColumns::uncertaintyDescriptionType);
    if (value.isValid() && !value.isNull()) {
      m_uncertaintyDescriptionType = analysis::UncertaintyDescriptionType(value.toInt());
    }
  }

  boost::optional<ObjectRecord> InputVariableRecord_Impl::parent() const {
    OptionalObjectRecord result = VariableRecord_Impl::parent();
    if (m_problemRecordId) {
      OS_ASSERT(!result);
      result = problemRecord().get();
    }
    return result;
  }

  std::vector<ObjectRecord> InputVariableRecord_Impl::children() const {
    AttributeRecordVector attributes = attributeRecords();
    return ObjectRecordVector(attributes.begin(),attributes.end());
  }

  void InputVariableRecord_Impl::saveRow(ProjectDatabase& projectDatabase) {
    QSqlQuery query(*(projectDatabase.qSqlDatabase()));
    this->makeUpdateByIdQuery<InputVariableRecord>(query);
    this->bindValues(query);
    assertExec(query);
  }

  boost::optional<ProblemRecord> InputVariableRecord_Impl::problemRecord() const {
    OptionalProblemRecord result;
    if (m_problemRecordId) {
      ProjectDatabase database = projectDatabase();
      result = ProblemRecord::getProblemRecord(*m_problemRecordId,database);
    }
    return result;
  }

  std::vector<AttributeRecord> InputVariableRecord_Impl::attributeRecords() const {
    AttributeRecordVector result;

    ProjectDatabase database = projectDatabase();
    QSqlQuery query(*(database.qSqlDatabase()));
    query.prepare(toQString("SELECT * FROM " + AttributeRecord::databaseTableName() + " WHERE variableRecordId=:variableRecordId"));
    query.bindValue(":variableRecordId",id());
    assertExec(query);
    while (query.next()) {
      result.push_back(database.getFromQuery<AttributeRecord>(query));
    }

    return result;
  }

  boost::optional<analysis::UncertaintyDescription> InputVariableRecord_Impl::uncertaintyDescription() const {
    analysis::OptionalUncertaintyDescription result;
    if (m_uncertaintyDescriptionType) {
      AttributeVector attributes;
      for (const AttributeRecord& attributeRecord : attributeRecords()) {
        attributes.push_back(attributeRecord.attribute());
      }
      result = analysis::GenericUncertaintyDescription(*m_uncertaintyDescriptionType,attributes);
    }
    return result;
  }

  void InputVariableRecord_Impl::bindValues(QSqlQuery& query) const {
    VariableRecord_Impl::bindValues(query);

    if (m_problemRecordId) {
      query.bindValue(VariableRecordColumns::problemRecordId, *m_problemRecordId);
    }
    else {
      query.bindValue(VariableRecordColumns::problemRecordId, QVariant(QVariant::Int));
    }

    if (m_uncertaintyDescriptionType) {
      query.bindValue(VariableRecordColumns::uncertaintyDescriptionType, m_uncertaintyDescriptionType->value());
    }
    else {
      query.bindValue(VariableRecordColumns::uncertaintyDescriptionType, QVariant(QVariant::Int));
    }

    query.bindValue(VariableRecordColumns::inputVariableRecordType, m_inputVariableRecordType.value());
  }

  void InputVariableRecord_Impl::setLastValues(const QSqlQuery& query, ProjectDatabase& projectDatabase) {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    VariableRecord_Impl::setLastValues(query,projectDatabase);

    QVariant value;

    value = query.value(VariableRecordColumns::problemRecordId);
    if (value.isValid() && !value.isNull()) {
      m_lastProblemRecordId = value.toInt();
    }
    else {
      m_lastProblemRecordId.reset();
    }

    value = query.value(VariableRecordColumns::uncertaintyDescriptionType);
    if (value.isValid() && !value.isNull()) {
      m_lastUncertaintyDescriptionType = analysis::UncertaintyDescriptionType(value.toInt());
    }
    else {
      m_lastUncertaintyDescriptionType.reset();
    }

    value = query.value(VariableRecordColumns::inputVariableRecordType);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_lastInputVariableRecordType = InputVariableRecordType(value.toInt());
  }

  bool InputVariableRecord_Impl::compareValues(const QSqlQuery& query) const {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    bool result = VariableRecord_Impl::compareValues(query);

    QVariant value;

    value = query.value(VariableRecordColumns::problemRecordId);
    if (value.isValid() && !value.isNull()) {
      result = result && m_problemRecordId && (*m_problemRecordId == value.toInt());
    }
    else {
      result = result && !m_problemRecordId;
    }

    value = query.value(VariableRecordColumns::uncertaintyDescriptionType);
    if (value.isValid() && !value.isNull()) {
      result = result && m_uncertaintyDescriptionType &&
               (*m_uncertaintyDescriptionType == analysis::UncertaintyDescriptionType(value.toInt()));
    }
    else {
      result = result && !m_uncertaintyDescriptionType;
    }

    value = query.value(VariableRecordColumns::inputVariableRecordType);
    OS_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_inputVariableRecordType == InputVariableRecordType(value.toInt()));

    return result;
  }

  void InputVariableRecord_Impl::saveLastValues() {
    VariableRecord_Impl::saveLastValues();

    // m_lastDATAMEMBERNAME = m_DATAMEMBERNAME;
  }

  void InputVariableRecord_Impl::revertToLastValues() {
    VariableRecord_Impl::revertToLastValues();

    m_lastProblemRecordId = m_problemRecordId;
    m_lastUncertaintyDescriptionType = m_uncertaintyDescriptionType;
    m_lastInputVariableRecordType = m_inputVariableRecordType;
  }

} // detail

boost::optional<InputVariableRecord> InputVariableRecord::factoryFromQuery(
    const QSqlQuery& query,
    ProjectDatabase& database)
{
  OptionalInputVariableRecord result;

  int inputVariableRecordType = query.value(VariableRecordColumns::inputVariableRecordType).toInt();

  switch (inputVariableRecordType) {
    case InputVariableRecordType::DiscreteVariableRecord :
      result = DiscreteVariableRecord::factoryFromQuery(query, database);
      break;
    case InputVariableRecordType::ContinuousVariableRecord :
      result = ContinuousVariableRecord::factoryFromQuery(query,database);
      break;
    default :
      LOG(Error,"Unknown InputVariableRecordType " << inputVariableRecordType);
      return boost::none;
  }

  return result;
}

InputVariableRecord InputVariableRecord::factoryFromInputVariable(
    const analysis::InputVariable& inputVariable,
    ProblemRecord& problemRecord,
    int workflowIndex)
{
  if (analysis::OptionalDiscreteVariable discreteVariable = inputVariable.optionalCast<analysis::DiscreteVariable>())
  {
    return DiscreteVariableRecord::factoryFromDiscreteVariable(*discreteVariable,
                                                               problemRecord,
                                                               workflowIndex);
  }
  if (analysis::OptionalContinuousVariable continuousVariable = inputVariable.optionalCast<analysis::ContinuousVariable>())
  {
    return ContinuousVariableRecord::factoryFromContinuousVariable(*continuousVariable,
                                                                   problemRecord,
                                                                   workflowIndex);
  }

  OS_ASSERT(false);
  return InputVariableRecord(std::shared_ptr<detail::InputVariableRecord_Impl>());
}

InputVariableRecord InputVariableRecord::factoryFromInputVariable(
    const analysis::InputVariable& inputVariable,
    FunctionRecord& functionRecord,
    int variableVectorIndex,
    boost::optional<double> functionCoefficient)
{
  if (analysis::OptionalDiscreteVariable discreteVariable = inputVariable.optionalCast<analysis::DiscreteVariable>())
  {
    return DiscreteVariableRecord::factoryFromDiscreteVariable(*discreteVariable,
                                                               functionRecord,
                                                               variableVectorIndex,
                                                               functionCoefficient);
  }
  if (analysis::OptionalContinuousVariable continuousVariable = inputVariable.optionalCast<analysis::ContinuousVariable>())
  {
    return ContinuousVariableRecord::factoryFromContinuousVariable(*continuousVariable,
                                                                   functionRecord,
                                                                   variableVectorIndex,
                                                                   functionCoefficient);
  }

  OS_ASSERT(false);
  return InputVariableRecord(std::shared_ptr<detail::InputVariableRecord_Impl>());
}

std::vector<InputVariableRecord> InputVariableRecord::getInputVariableRecords(ProjectDatabase& database) {
  std::vector<InputVariableRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + VariableRecord::databaseTableName() +
                          " WHERE variableRecordType=:variableRecordType"));
  query.bindValue(":variableRecordType", VariableRecordType::InputVariableRecord);
  assertExec(query);
  while (query.next()) {
    OptionalInputVariableRecord inputVariableRecord = InputVariableRecord::factoryFromQuery(query, database);
    if (inputVariableRecord) {
      result.push_back(*inputVariableRecord);
    }
  }

  return result;
}

boost::optional<InputVariableRecord> InputVariableRecord::getInputVariableRecord(int id, ProjectDatabase& database) {
  boost::optional<InputVariableRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + VariableRecord::databaseTableName() +
                          " WHERE variableRecordType=:variableRecordType AND id=:id"));
  query.bindValue(":variableRecordType", VariableRecordType::InputVariableRecord);
  query.bindValue(":id",id);
  assertExec(query);
  if (query.first()) {
    result = InputVariableRecord::factoryFromQuery(query, database);
  }

  return result;
}

boost::optional<ProblemRecord> InputVariableRecord::problemRecord() const {
  return getImpl<detail::InputVariableRecord_Impl>()->problemRecord();
}

std::vector<AttributeRecord> InputVariableRecord::attributeRecords() const {
  return getImpl<detail::InputVariableRecord_Impl>()->attributeRecords();
}

boost::optional<analysis::UncertaintyDescription> InputVariableRecord::uncertaintyDescription() const {
  return getImpl<detail::InputVariableRecord_Impl>()->uncertaintyDescription();
}

analysis::InputVariable InputVariableRecord::inputVariable() const {
  return getImpl<detail::InputVariableRecord_Impl>()->inputVariable();
}

/// @cond
InputVariableRecord::InputVariableRecord(std::shared_ptr<detail::InputVariableRecord_Impl> impl)
  : VariableRecord(impl)
{}

InputVariableRecord::InputVariableRecord(std::shared_ptr<detail::InputVariableRecord_Impl> impl,
                                         ProjectDatabase database,
                                         const boost::optional<analysis::InputVariable>& inputVariable)
  : VariableRecord(impl, database)
{
  OS_ASSERT(getImpl<detail::InputVariableRecord_Impl>());
  if (inputVariable) {
    constructRelatedRecords(*inputVariable);
  }
  LOG(Trace,"Constructed InputVariableRecord '" << name() << "' with variableVectorIndex = " << variableVectorIndex());
}

void InputVariableRecord::constructRelatedRecords(const analysis::InputVariable& inputVariable) {
  AttributeVector attributes, dbAttributes;
  InputVariableRecord copyOfThis(getImpl<detail::InputVariableRecord_Impl>());

  if (analysis::OptionalUncertaintyDescription dbUDesc = uncertaintyDescription()) {
    dbAttributes = dbUDesc->cast<analysis::GenericUncertaintyDescription>().attributes();
  }
  if (analysis::OptionalUncertaintyDescription udesc = inputVariable.uncertaintyDescription()) {
    attributes = udesc->cast<analysis::GenericUncertaintyDescription>().attributes();
  }

  for (const Attribute& attribute : attributes) {
    // find in dbOptions
    auto dbIt = std::find_if(dbAttributes.begin(),dbAttributes.end(),
                             std::bind(uuidsEqual<Attribute,Attribute>,std::placeholders::_1,attribute));
    // if not there, or if different versionUUID, save it
    if ((dbIt == dbAttributes.end()) || (attribute.versionUUID() != dbIt->versionUUID())) {
      AttributeRecord udescAttributeRecord(attribute,copyOfThis);
    }

    // if there, erase from dbOptions
    if (dbIt != dbAttributes.end()) {
      dbAttributes.erase(dbIt);
    }
  }

  // any attributes left in dbAttributes should be removed from the database
  ProjectDatabase database = projectDatabase();
  for (const Attribute& toRemove : dbAttributes) {
    AttributeRecord dbUDescAttributeRecord = database.getObjectRecordByHandle<AttributeRecord>(toRemove.uuid()).get();
    database.removeRecord(dbUDescAttributeRecord);
  }
}
/// @endcond

} // project
} // openstudio

