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

#include "OutputVariableRecord.hpp"
#include "OutputVariableRecord_Impl.hpp"

#include "FunctionRecord.hpp"
#include "JoinRecord.hpp"
#include "OutputAttributeVariableRecord.hpp"

#include "../analysis/OutputAttributeVariable.hpp"
#include "../analysis/OutputAttributeVariable_Impl.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace project {

namespace detail {

  OutputVariableRecord_Impl::OutputVariableRecord_Impl(const analysis::OutputVariable& outputVariable,
                                                       const OutputVariableRecordType& outputVariableRecordType,
                                                       FunctionRecord& functionRecord,
                                                       int variableVectorIndex,
                                                       boost::optional<double> functionCoefficient)
    : VariableRecord_Impl(outputVariable,
                          VariableRecordType::OutputVariableRecord,
                          functionRecord,
                          variableVectorIndex,
                          functionCoefficient),
      m_outputVariableRecordType(outputVariableRecordType)
  {}

  OutputVariableRecord_Impl::OutputVariableRecord_Impl(const QSqlQuery& query, ProjectDatabase& database)
    : VariableRecord_Impl(query, database)
  {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    QVariant value;

    value = query.value(OutputVariableRecord::ColumnsType::outputVariableRecordType);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_outputVariableRecordType = OutputVariableRecordType(value.toInt());
  }

  void OutputVariableRecord_Impl::saveRow(ProjectDatabase& projectDatabase) {
    QSqlQuery query(*(projectDatabase.qSqlDatabase()));
    this->makeUpdateByIdQuery<OutputVariableRecord>(query);
    this->bindValues(query);
    assertExec(query);
  }

  void OutputVariableRecord_Impl::bindValues(QSqlQuery& query) const {
    VariableRecord_Impl::bindValues(query);

    query.bindValue(OutputVariableRecord::ColumnsType::outputVariableRecordType,
                    m_outputVariableRecordType.value());
  }

  void OutputVariableRecord_Impl::setLastValues(const QSqlQuery& query, ProjectDatabase& projectDatabase) {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    VariableRecord_Impl::setLastValues(query,projectDatabase);

    QVariant value;

    value = query.value(OutputVariableRecord::ColumnsType::outputVariableRecordType);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_lastOutputVariableRecordType = OutputVariableRecordType(value.toInt());
  }

  bool OutputVariableRecord_Impl::compareValues(const QSqlQuery& query) const {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    bool result = VariableRecord_Impl::compareValues(query);

    QVariant value;

    value = query.value(OutputVariableRecord::ColumnsType::outputVariableRecordType);
    OS_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_outputVariableRecordType == OutputVariableRecordType(value.toInt()));

    return result;
  }

  void OutputVariableRecord_Impl::saveLastValues() {
    VariableRecord_Impl::saveLastValues();

    m_lastOutputVariableRecordType = m_outputVariableRecordType;
  }

  void OutputVariableRecord_Impl::revertToLastValues() {
    VariableRecord_Impl::revertToLastValues();

    m_outputVariableRecordType = m_lastOutputVariableRecordType;
  }

} // detail

boost::optional<OutputVariableRecord> OutputVariableRecord::factoryFromQuery(const QSqlQuery& query,
                                                                             ProjectDatabase& database)
{
  OptionalOutputVariableRecord result;

  int outputVariableRecordType = query.value(VariableRecordColumns::outputVariableRecordType).toInt();

  switch (outputVariableRecordType) {
    case OutputVariableRecordType::OutputAttributeVariableRecord :
      result = OutputAttributeVariableRecord(query, database).cast<OutputVariableRecord>();
     break;
    default :
      LOG(Error,"Unknown OutputVariableRecordType " << outputVariableRecordType);
      return boost::none;
  }

  return result;
}

OutputVariableRecord OutputVariableRecord::factoryFromOutputVariable(const analysis::OutputVariable& outputVariable,
                                                                     FunctionRecord& functionRecord,
                                                                     int variableVectorIndex,
                                                                     boost::optional<double> functionCoefficient)
{
  if (outputVariable.optionalCast<analysis::OutputAttributeVariable>()) {
    return OutputAttributeVariableRecord(outputVariable.cast<analysis::OutputAttributeVariable>(),
                                         functionRecord,
                                         variableVectorIndex,
                                         functionCoefficient);
  }

  OS_ASSERT(false);
  return OutputVariableRecord(std::shared_ptr<detail::OutputVariableRecord_Impl>());
}

std::vector<OutputVariableRecord> OutputVariableRecord::getOutputVariableRecords(ProjectDatabase& database) {
  std::vector<OutputVariableRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + VariableRecord::databaseTableName() +
                          " WHERE variableRecordType=:variableRecordType"));
  query.bindValue(":variableRecordType", VariableRecordType::OutputVariableRecord);
  assertExec(query);
  while (query.next()) {
    OptionalOutputVariableRecord outputVariableRecord = OutputVariableRecord::factoryFromQuery(query, database);
    if (outputVariableRecord) {
      result.push_back(*outputVariableRecord);
    }
  }

  return result;
}

boost::optional<OutputVariableRecord> OutputVariableRecord::getOutputVariableRecord(int id, ProjectDatabase& database) {
  boost::optional<OutputVariableRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + VariableRecord::databaseTableName() +
                          " WHERE variableRecordType=:variableRecordType AND id=:id"));
  query.bindValue(":variableRecordType", VariableRecordType::OutputVariableRecord);
  query.bindValue(":id",id);
  assertExec(query);
  if (query.first()) {
    result = OutputVariableRecord::factoryFromQuery(query, database);
  }

  return result;
}

analysis::OutputVariable OutputVariableRecord::outputVariable() const {
  return getImpl<detail::OutputVariableRecord_Impl>()->outputVariable();
}

/// @cond
OutputVariableRecord::OutputVariableRecord(std::shared_ptr<detail::OutputVariableRecord_Impl> impl)
  : VariableRecord(impl)
{}

OutputVariableRecord::OutputVariableRecord(std::shared_ptr<detail::OutputVariableRecord_Impl> impl,
                                           ProjectDatabase database)
  : VariableRecord(impl, database)
{
  OS_ASSERT(getImpl<detail::OutputVariableRecord_Impl>());
}
/// @endcond

} // project
} // openstudio

