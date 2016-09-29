/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2016, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#include "OutputAttributeVariableRecord.hpp"
#include "OutputAttributeVariableRecord_Impl.hpp"

#include "JoinRecord.hpp"
#include "FunctionRecord.hpp"

#include "../analysis/OutputAttributeVariable.hpp"
#include "../analysis/OutputAttributeVariable_Impl.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace project {

namespace detail {

  OutputAttributeVariableRecord_Impl::OutputAttributeVariableRecord_Impl(
      const analysis::OutputAttributeVariable& outputAttributeVariable,
      FunctionRecord& functionRecord,
      int variableVectorIndex,
      boost::optional<double> functionCoefficient)
    : OutputVariableRecord_Impl(outputAttributeVariable,
                                OutputVariableRecordType::OutputAttributeVariableRecord,
                                functionRecord,
                                variableVectorIndex,
                                functionCoefficient),
      m_attributeName(outputAttributeVariable.attributeName())
  {}

  OutputAttributeVariableRecord_Impl::OutputAttributeVariableRecord_Impl(
      const QSqlQuery& query, ProjectDatabase& database)
    : OutputVariableRecord_Impl(query, database)
  {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    QVariant value;

    value = query.value(OutputAttributeVariableRecord::ColumnsType::attributeName);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_attributeName = value.toString().toStdString();
  }

  std::vector<ObjectRecord> OutputAttributeVariableRecord_Impl::children() const {
    ObjectRecordVector result;
    return result;
  }

  std::vector<ObjectRecord> OutputAttributeVariableRecord_Impl::resources() const {
    ObjectRecordVector result;
    return result;
  }

  void OutputAttributeVariableRecord_Impl::saveRow(const std::shared_ptr<QSqlDatabase> &database)
  {
    QSqlQuery query(*database);
    this->makeUpdateByIdQuery<OutputAttributeVariableRecord>(query);
    this->bindValues(query);
    assertExec(query);
  }

  analysis::Variable OutputAttributeVariableRecord_Impl::variable() const {
    return outputAttributeVariable().cast<analysis::Variable>();
  }

  analysis::OutputVariable OutputAttributeVariableRecord_Impl::outputVariable() const
  {
    return outputAttributeVariable().cast<analysis::OutputVariable>();
  }

  analysis::OutputAttributeVariable
  OutputAttributeVariableRecord_Impl::outputAttributeVariable() const
  {
    return analysis::OutputAttributeVariable(handle(),
                                             uuidLast(),
                                             name(),
                                             displayName(),
                                             description(),
                                             m_attributeName);
  }

  void OutputAttributeVariableRecord_Impl::bindValues(QSqlQuery& query) const {
    OutputVariableRecord_Impl::bindValues(query);

    query.bindValue(OutputAttributeVariableRecord::ColumnsType::attributeName,
                    toQString(m_attributeName));
  }

  void OutputAttributeVariableRecord_Impl::setLastValues(const QSqlQuery& query, ProjectDatabase& projectDatabase) {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    OutputVariableRecord_Impl::setLastValues(query,projectDatabase);

    QVariant value;

    value = query.value(OutputAttributeVariableRecord::ColumnsType::attributeName);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_lastAttributeName = value.toString().toStdString();
  }

  bool OutputAttributeVariableRecord_Impl::compareValues(const QSqlQuery& query) const {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    bool result = OutputVariableRecord_Impl::compareValues(query);

    QVariant value;

    value = query.value(OutputAttributeVariableRecord::ColumnsType::attributeName);
    OS_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_attributeName == value.toString().toStdString());

    return result;
  }

  void OutputAttributeVariableRecord_Impl::saveLastValues() {
    OutputVariableRecord_Impl::saveLastValues();

    m_lastAttributeName = m_attributeName;
  }

  void OutputAttributeVariableRecord_Impl::revertToLastValues() {
    OutputVariableRecord_Impl::revertToLastValues();

    m_attributeName = m_lastAttributeName;
  }

} // detail

OutputAttributeVariableRecord::OutputAttributeVariableRecord(
    const analysis::OutputAttributeVariable& outputAttributeVariable,
    FunctionRecord& functionRecord,
    int variableVectorIndex,
    boost::optional<double> functionCoefficient)
  : OutputVariableRecord(std::shared_ptr<detail::OutputAttributeVariableRecord_Impl>(
        new detail::OutputAttributeVariableRecord_Impl(outputAttributeVariable,
                                                       functionRecord,
                                                       variableVectorIndex,
                                                       functionCoefficient)),
        functionRecord.projectDatabase())
{
  OS_ASSERT(getImpl<detail::OutputAttributeVariableRecord_Impl>());
}

OutputAttributeVariableRecord::OutputAttributeVariableRecord(
    const QSqlQuery& query, ProjectDatabase& database)
  : OutputVariableRecord(std::shared_ptr<detail::OutputAttributeVariableRecord_Impl>(
        new detail::OutputAttributeVariableRecord_Impl(query, database)),
        database)
{
  OS_ASSERT(getImpl<detail::OutputAttributeVariableRecord_Impl>());
}

OutputAttributeVariableRecord::OutputAttributeVariableRecord(
    std::shared_ptr<detail::OutputAttributeVariableRecord_Impl> impl,
    ProjectDatabase database)
  : OutputVariableRecord(impl, database)
{
  OS_ASSERT(getImpl<detail::OutputAttributeVariableRecord_Impl>());
}

boost::optional<OutputAttributeVariableRecord> OutputAttributeVariableRecord::factoryFromQuery(
    const QSqlQuery& query, ProjectDatabase& database)
{
  OptionalOutputAttributeVariableRecord result;
  try {
    result = OutputAttributeVariableRecord(query,database);
  }
  catch (const std::exception& e) {
    LOG(Error,"Unable to construct OutputAttributeVariableRecord from query, because '"
        << e.what() << "'.");
  }
  return result;
}

std::vector<OutputAttributeVariableRecord>
OutputAttributeVariableRecord::getOutputAttributeVariableRecords(
    ProjectDatabase& database)
{
  std::vector<OutputAttributeVariableRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + VariableRecord::databaseTableName() +
      " WHERE variableRecordType=:variableRecordType AND " +
      "outputVariableRecordType=:outputVariableRecordType"));
  query.bindValue(":variableRecordType", VariableRecordType::OutputVariableRecord);
  query.bindValue(":outputVariableRecordType",OutputVariableRecordType::OutputAttributeVariableRecord);
  assertExec(query);
  while (query.next()) {
    result.push_back(OutputAttributeVariableRecord(query, database));
  }

  return result;
}

boost::optional<OutputAttributeVariableRecord>
OutputAttributeVariableRecord::getOutputAttributeVariableRecord(
    int id, ProjectDatabase& database)
{
  boost::optional<OutputAttributeVariableRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + VariableRecord::databaseTableName() +
      " WHERE variableRecordType=:variableRecordType AND " +
      "outputVariableRecordType=:outputVariableRecordType AND id=:id"));
  query.bindValue(":variableRecordType", VariableRecordType::OutputVariableRecord);
  query.bindValue(":outputVariableRecordType",OutputVariableRecordType::OutputAttributeVariableRecord);
  query.bindValue(":id",id);
  assertExec(query);
  if (query.first()) {
    result = OutputAttributeVariableRecord(query, database);
  }

  return result;
}

analysis::OutputAttributeVariable
OutputAttributeVariableRecord::outputAttributeVariable() const
{
  return getImpl<detail::OutputAttributeVariableRecord_Impl>()->outputAttributeVariable();
}

/// @cond
OutputAttributeVariableRecord::OutputAttributeVariableRecord(std::shared_ptr<detail::OutputAttributeVariableRecord_Impl> impl)
  : OutputVariableRecord(impl)
{}
/// @endcond

} // project
} // openstudio

