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

#include "ContinuousVariableRecord.hpp"
#include "ContinuousVariableRecord_Impl.hpp"

#include "JoinRecord.hpp"
#include "ProblemRecord.hpp"
#include "FunctionRecord.hpp"
#include "RubyContinuousVariableRecord.hpp"

#include "../analysis/RubyContinuousVariable.hpp"
#include "../analysis/RubyContinuousVariable_Impl.hpp"

#include "../utilities/math/FloatCompare.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace project {

namespace detail {

  ContinuousVariableRecord_Impl::ContinuousVariableRecord_Impl(
      const analysis::ContinuousVariable& continuousVariable,
      const ContinuousVariableRecordType& continuousVariableRecordType,
      ProblemRecord& problemRecord,
      int workflowIndex)
    : InputVariableRecord_Impl(continuousVariable,
                               InputVariableRecordType::ContinuousVariableRecord,
                               problemRecord,
                               workflowIndex),
      m_continuousVariableRecordType(continuousVariableRecordType),
      m_minimum(continuousVariable.minimum()),
      m_maximum(continuousVariable.maximum()),
      m_increment(continuousVariable.increment()),
      m_nSteps(continuousVariable.nSteps())
  {}

  ContinuousVariableRecord_Impl::ContinuousVariableRecord_Impl(
      const analysis::ContinuousVariable& continuousVariable,
      const ContinuousVariableRecordType& continuousVariableRecordType,
      FunctionRecord& functionRecord,
      int variableVectorIndex,
      boost::optional<double> functionCoefficient)
    : InputVariableRecord_Impl(continuousVariable,
                               InputVariableRecordType::ContinuousVariableRecord,
                               functionRecord,
                               variableVectorIndex,
                               functionCoefficient),
      m_continuousVariableRecordType(continuousVariableRecordType),
      m_minimum(continuousVariable.minimum()),
      m_maximum(continuousVariable.maximum()),
      m_increment(continuousVariable.increment()),
      m_nSteps(continuousVariable.nSteps())
  {}

  ContinuousVariableRecord_Impl::ContinuousVariableRecord_Impl(const QSqlQuery& query, ProjectDatabase& database)
    : InputVariableRecord_Impl(query, database)
  {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    QVariant value;

    value = query.value(ContinuousVariableRecord::ColumnsType::continuousVariableRecordType);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_continuousVariableRecordType = ContinuousVariableRecordType(value.toInt());

    value = query.value(ContinuousVariableRecord::ColumnsType::minimum);
    if (value.isValid() && !value.isNull()) {
      m_minimum = value.toDouble();
    }

    value = query.value(ContinuousVariableRecord::ColumnsType::maximum);
    if (value.isValid() && !value.isNull()) {
      m_maximum = value.toDouble();
    }

    value = query.value(ContinuousVariableRecord::ColumnsType::increment);
    if (value.isValid() && !value.isNull()) {
      m_increment = value.toDouble();
    }

    value = query.value(ContinuousVariableRecord::ColumnsType::nSteps);
    if (value.isValid() && !value.isNull()) {
      m_nSteps = value.toInt();
    }

  }

  void ContinuousVariableRecord_Impl::saveRow(const std::shared_ptr<QSqlDatabase> &database)
  {
    QSqlQuery query(*database);
    this->makeUpdateByIdQuery<ContinuousVariableRecord>(query);
    this->bindValues(query);
    assertExec(query);
  }

  boost::optional<double> ContinuousVariableRecord_Impl::minimum() const {
    return m_minimum;
  }

  boost::optional<double> ContinuousVariableRecord_Impl::maximum() const {
    return m_maximum;
  }

  boost::optional<double> ContinuousVariableRecord_Impl::increment() const {
    return m_increment;
  }

  boost::optional<int> ContinuousVariableRecord_Impl::nSteps() const {
    return m_nSteps;
  }

  void ContinuousVariableRecord_Impl::bindValues(QSqlQuery& query) const {
    InputVariableRecord_Impl::bindValues(query);

    query.bindValue(ContinuousVariableRecord::ColumnsType::continuousVariableRecordType,
                    m_continuousVariableRecordType.value());
    if (m_minimum) {
      query.bindValue(ContinuousVariableRecord::ColumnsType::minimum,*m_minimum);
    }
    else {
      query.bindValue(ContinuousVariableRecord::ColumnsType::minimum,QVariant(QVariant::Double));
    }
    if (m_maximum) {
      query.bindValue(ContinuousVariableRecord::ColumnsType::maximum,*m_maximum);
    }
    else {
      query.bindValue(ContinuousVariableRecord::ColumnsType::maximum,QVariant(QVariant::Double));
    }
    if (m_increment) {
      query.bindValue(ContinuousVariableRecord::ColumnsType::increment,*m_increment);
    }
    else {
      query.bindValue(ContinuousVariableRecord::ColumnsType::increment,QVariant(QVariant::Double));
    }
    if (m_nSteps) {
      query.bindValue(ContinuousVariableRecord::ColumnsType::nSteps,*m_nSteps);
    }
    else {
      query.bindValue(ContinuousVariableRecord::ColumnsType::nSteps,QVariant(QVariant::Int));
    }
  }

  void ContinuousVariableRecord_Impl::setLastValues(const QSqlQuery& query,
                                                    ProjectDatabase& projectDatabase)
  {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    InputVariableRecord_Impl::setLastValues(query,projectDatabase);

    QVariant value;

    value = query.value(ContinuousVariableRecord::ColumnsType::continuousVariableRecordType);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_lastContinuousVariableRecordType = ContinuousVariableRecordType(value.toInt());

    value = query.value(ContinuousVariableRecord::ColumnsType::minimum);
    if (value.isValid() && !value.isNull()) {
      m_lastMinimum = value.toDouble();
    }
    else {
      m_lastMinimum.reset();
    }

    value = query.value(ContinuousVariableRecord::ColumnsType::maximum);
    if (value.isValid() && !value.isNull()) {
      m_lastMaximum = value.toDouble();
    }
    else {
      m_lastMaximum.reset();
    }

    value = query.value(ContinuousVariableRecord::ColumnsType::increment);
    if (value.isValid() && !value.isNull()) {
      m_lastIncrement = value.toDouble();
    }
    else {
      m_lastIncrement.reset();
    }

    value = query.value(ContinuousVariableRecord::ColumnsType::nSteps);
    if (value.isValid() && !value.isNull()) {
      m_lastNSteps = value.toInt();
    }
    else {
      m_lastNSteps.reset();
    }
  }

  bool ContinuousVariableRecord_Impl::compareValues(const QSqlQuery& query) const {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    bool result = InputVariableRecord_Impl::compareValues(query);

    QVariant value;

    value = query.value(ContinuousVariableRecord::ColumnsType::continuousVariableRecordType);
    OS_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_continuousVariableRecordType == ContinuousVariableRecordType(value.toInt()));

    value = query.value(ContinuousVariableRecord::ColumnsType::minimum);
    if (value.isValid() && !value.isNull()) {
      result = result && m_minimum && equal(*m_minimum,value.toDouble());
    }
    else {
      result = result && !m_minimum;
    }

    value = query.value(ContinuousVariableRecord::ColumnsType::maximum);
    if (value.isValid() && !value.isNull()) {
      result = result && m_maximum && equal(*m_maximum,value.toDouble());
    }
    else {
      result = result && !m_maximum;
    }

    value = query.value(ContinuousVariableRecord::ColumnsType::increment);
    if (value.isValid() && !value.isNull()) {
      result = result && m_increment && equal(*m_increment,value.toDouble());
    }
    else {
      result = result && !m_increment;
    }

    value = query.value(ContinuousVariableRecord::ColumnsType::nSteps);
    if (value.isValid() && !value.isNull()) {
      result = result && m_nSteps && (*m_nSteps == value.toInt());
    }
    else {
      result = result && !m_nSteps;
    }

    return result;
  }

  void ContinuousVariableRecord_Impl::saveLastValues() {
    InputVariableRecord_Impl::saveLastValues();

    m_lastContinuousVariableRecordType = m_continuousVariableRecordType;
    m_lastMinimum = m_minimum;
    m_lastMaximum = m_maximum;
    m_lastIncrement = m_increment;
    m_lastNSteps = m_nSteps;
  }

  void ContinuousVariableRecord_Impl::revertToLastValues() {
    InputVariableRecord_Impl::revertToLastValues();

    m_continuousVariableRecordType = m_lastContinuousVariableRecordType;
    m_minimum = m_lastMinimum;
    m_maximum = m_lastMaximum;
    m_increment = m_lastIncrement;
    m_nSteps = m_lastNSteps;
  }

} // detail

ContinuousVariableRecord::ContinuousVariableRecord(std::shared_ptr<detail::ContinuousVariableRecord_Impl> impl,
                                                   ProjectDatabase database,
                                                   const boost::optional<analysis::ContinuousVariable>& variable)
  : InputVariableRecord(impl, database, (variable) ? *variable : analysis::OptionalInputVariable())
{
  OS_ASSERT(getImpl<detail::ContinuousVariableRecord_Impl>());
}

boost::optional<ContinuousVariableRecord> ContinuousVariableRecord::factoryFromQuery(
    const QSqlQuery& query,
    ProjectDatabase& database)
{
  OptionalContinuousVariableRecord result;

  int continuousVariableRecordType =
      query.value(VariableRecordColumns::continuousVariableRecordType).toInt();

  switch (continuousVariableRecordType){
    case ContinuousVariableRecordType::RubyContinuousVariableRecord :
      result = RubyContinuousVariableRecord(query,database);
      break;
    default:
      LOG(Error, "Unknown continuousVariableRecordType " << continuousVariableRecordType);
  }

  return result;
}

ContinuousVariableRecord ContinuousVariableRecord::factoryFromContinuousVariable(
    const analysis::ContinuousVariable& variable,
    ProblemRecord& problemRecord,
    int variableVectorIndex)
{
  if (variable.optionalCast<analysis::RubyContinuousVariable>()) {
    return RubyContinuousVariableRecord(
        variable.cast<analysis::RubyContinuousVariable>(),
        problemRecord,
        variableVectorIndex);
  }

  OS_ASSERT(false);
  return ContinuousVariableRecord(std::shared_ptr<detail::ContinuousVariableRecord_Impl>());
}

ContinuousVariableRecord ContinuousVariableRecord::factoryFromContinuousVariable(
    const analysis::ContinuousVariable& variable,
    FunctionRecord& functionRecord,
    int variableVectorIndex,
    boost::optional<double> functionCoefficient)
{
  if (variable.optionalCast<analysis::RubyContinuousVariable>()) {
    return RubyContinuousVariableRecord(
        variable.cast<analysis::RubyContinuousVariable>(),
        functionRecord,
        variableVectorIndex,
        functionCoefficient);
  }

  OS_ASSERT(false);
  return ContinuousVariableRecord(std::shared_ptr<detail::ContinuousVariableRecord_Impl>());
}

std::vector<ContinuousVariableRecord> ContinuousVariableRecord::getContinuousVariableRecords(
    ProjectDatabase& database)
{
  std::vector<ContinuousVariableRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + VariableRecord::databaseTableName() +
      " WHERE variableRecordType=:variableRecordType AND " +
      "inputVariableRecordType=:inputVariableRecordType"));
  query.bindValue(":variableRecordType", VariableRecordType::InputVariableRecord);
  query.bindValue(":inputVariableRecordType", InputVariableRecordType::ContinuousVariableRecord);
  assertExec(query);
  while (query.next()) {
    OptionalContinuousVariableRecord continuousVariableRecord = ContinuousVariableRecord::factoryFromQuery(query, database);
    if (continuousVariableRecord) {
      result.push_back(*continuousVariableRecord);
    }
  }

  return result;
}

boost::optional<ContinuousVariableRecord> ContinuousVariableRecord::getContinuousVariableRecord(int id, ProjectDatabase& database) {
  boost::optional<ContinuousVariableRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + VariableRecord::databaseTableName() +
      " WHERE variableRecordType=:variableRecordType AND " +
      "inputVariableRecordType=:inputVariableRecordType AND id=:id"));
  query.bindValue(":variableRecordType", VariableRecordType::InputVariableRecord);
  query.bindValue(":inputVariableRecordType", InputVariableRecordType::ContinuousVariableRecord);
  query.bindValue(":id",id);
  assertExec(query);
  if (query.first()) {
    result = ContinuousVariableRecord::factoryFromQuery(query, database);
  }

  return result;
}

boost::optional<double> ContinuousVariableRecord::minimum() const {
  return getImpl<detail::ContinuousVariableRecord_Impl>()->minimum();
}

boost::optional<double> ContinuousVariableRecord::maximum() const {
  return getImpl<detail::ContinuousVariableRecord_Impl>()->maximum();
}

boost::optional<double> ContinuousVariableRecord::increment() const {
  return getImpl<detail::ContinuousVariableRecord_Impl>()->increment();
}

boost::optional<int> ContinuousVariableRecord::nSteps() const {
  return getImpl<detail::ContinuousVariableRecord_Impl>()->nSteps();
}

analysis::ContinuousVariable ContinuousVariableRecord::continuousVariable() const {
  return getImpl<detail::ContinuousVariableRecord_Impl>()->continuousVariable();
}

/// @cond
ContinuousVariableRecord::ContinuousVariableRecord(std::shared_ptr<detail::ContinuousVariableRecord_Impl> impl)
  : InputVariableRecord(impl)
{}
/// @endcond

} // project
} // openstudio

