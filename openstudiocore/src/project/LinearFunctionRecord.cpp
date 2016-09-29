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

#include "LinearFunctionRecord.hpp"
#include "LinearFunctionRecord_Impl.hpp"

#include "JoinRecord.hpp"
#include "VariableRecord.hpp"
#include "ProblemRecord.hpp"

#include "../analysis/Function.hpp"
#include "../analysis/Function_Impl.hpp"
#include "../analysis/LinearFunction.hpp"
#include "../analysis/Variable.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/core/Containers.hpp"
#include "../utilities/core/Optional.hpp"

using namespace openstudio::analysis;

namespace openstudio {
namespace project {

namespace detail {

  LinearFunctionRecord_Impl::LinearFunctionRecord_Impl(const analysis::LinearFunction& linearFunction,
                                                       ProblemRecord& problemRecord,
                                                       const FunctionType& functionType,
                                                       int functionVectorIndex)
    : FunctionRecord_Impl(linearFunction,
                          FunctionRecordType::LinearFunctionRecord,
                          problemRecord,
                          functionType,
                          functionVectorIndex)
  {}

  LinearFunctionRecord_Impl::LinearFunctionRecord_Impl(const QSqlQuery& query, ProjectDatabase& database)
    : FunctionRecord_Impl(query, database)
  {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());
  }

  void LinearFunctionRecord_Impl::saveRow(const std::shared_ptr<QSqlDatabase> &database)
  {
    QSqlQuery query(*database);
    this->makeUpdateByIdQuery<LinearFunctionRecord>(query);
    this->bindValues(query);
    assertExec(query);
  }

  analysis::Function LinearFunctionRecord_Impl::function() const {
    return linearFunction().cast<analysis::Function>();
  }

  analysis::LinearFunction LinearFunctionRecord_Impl::linearFunction() const {
    VariableRecordVector variableRecords = this->variableRecords();
    analysis::VariableVector variables;
    DoubleVector coefficients;
    for (const VariableRecord& variableRecord : variableRecords) {
      variables.push_back(variableRecord.variable());
      OptionalDouble coefficient = variableRecord.functionCoefficient();
      if (coefficient) {
        coefficients.push_back(*coefficient);
      }
    }
    OS_ASSERT(coefficients.empty() || (coefficients.size() == variables.size()));
    return analysis::LinearFunction(handle(),
                                    uuidLast(),
                                    name(),
                                    displayName(),
                                    description(),
                                    variables,
                                    coefficients);
  }

  void LinearFunctionRecord_Impl::bindValues(QSqlQuery& query) const {
    FunctionRecord_Impl::bindValues(query);
  }

  void LinearFunctionRecord_Impl::setLastValues(const QSqlQuery& query, ProjectDatabase& projectDatabase) {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    FunctionRecord_Impl::setLastValues(query,projectDatabase);
  }

  bool LinearFunctionRecord_Impl::compareValues(const QSqlQuery& query) const {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    bool result = FunctionRecord_Impl::compareValues(query);

    return result;
  }

  void LinearFunctionRecord_Impl::saveLastValues() {
    FunctionRecord_Impl::saveLastValues();
  }

  void LinearFunctionRecord_Impl::revertToLastValues() {
    FunctionRecord_Impl::revertToLastValues();
  }

} // detail

LinearFunctionRecord::LinearFunctionRecord(const analysis::LinearFunction& linearFunction,
                                           ProblemRecord& problemRecord,
                                           const FunctionType& functionType,
                                           int functionVectorIndex)
  : FunctionRecord(std::shared_ptr<detail::LinearFunctionRecord_Impl>(
        new detail::LinearFunctionRecord_Impl(linearFunction,
                                              problemRecord,
                                              functionType,
                                              functionVectorIndex)),
        problemRecord.projectDatabase())
{
  OS_ASSERT(getImpl<detail::LinearFunctionRecord_Impl>());

  LinearFunctionRecord copyOfThis(getImpl<detail::LinearFunctionRecord_Impl>());
  ProjectDatabase database = copyOfThis.projectDatabase();
  bool isNew = database.isNewRecord(copyOfThis);

  // Save child Variables
  std::vector<UUID> variableUUIDs;
  VariableVector variables = linearFunction.variables();
  DoubleVector coefficients = linearFunction.coefficients();
  int n = variables.size();
  // empty coefficients implies vector of 1's.
  OS_ASSERT(coefficients.empty() || (coefficients.size() == static_cast<size_t>(n)));
  for (int i = 0; i < n; ++i) {
    variableUUIDs.push_back(variables[i].uuid());
    if (variables[i].isDirty() || isNew) {
      OptionalDouble coefficient;
      if (!coefficients.empty()) {
        coefficient = coefficients[i];
      }
      VariableRecord newVariableRecord = VariableRecord::factoryFromVariable(variables[i],
                                                                             copyOfThis,
                                                                             i,
                                                                             coefficient);
    }
  }
  if (!isNew) {
    removeVariableRecords(variableUUIDs,database);
  }
}

LinearFunctionRecord::LinearFunctionRecord(const QSqlQuery& query, ProjectDatabase& database)
  : FunctionRecord(std::shared_ptr<detail::LinearFunctionRecord_Impl>(
        new detail::LinearFunctionRecord_Impl(query, database)),
        database)
{
  OS_ASSERT(getImpl<detail::LinearFunctionRecord_Impl>());
}

LinearFunctionRecord::LinearFunctionRecord(std::shared_ptr<detail::LinearFunctionRecord_Impl> impl,
                                           ProjectDatabase database)
  : FunctionRecord(impl, database)
{
  OS_ASSERT(getImpl<detail::LinearFunctionRecord_Impl>());
}

boost::optional<LinearFunctionRecord> LinearFunctionRecord::factoryFromQuery(
    const QSqlQuery& query, ProjectDatabase& database)
{
  OptionalLinearFunctionRecord result;
  try {
    result = LinearFunctionRecord(query,database);
  }
  catch (const std::exception& e) {
    LOG(Error,"Unable to construct LinearFunctionRecord from query, because '"
        << e.what() << "'.");
  }
  return result;
}

std::vector<LinearFunctionRecord> LinearFunctionRecord::getLinearFunctionRecords(ProjectDatabase& database) {
  std::vector<LinearFunctionRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + FunctionRecord::databaseTableName() +
      " WHERE functionRecordType=:functionRecordType"));
  query.bindValue(":functionRecordType", FunctionRecordType::LinearFunctionRecord);
  assertExec(query);
  while (query.next()) {
    result.push_back(LinearFunctionRecord(query, database));
  }

  return result;
}

boost::optional<LinearFunctionRecord> LinearFunctionRecord::getLinearFunctionRecord(int id, ProjectDatabase& database) {
  boost::optional<LinearFunctionRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + FunctionRecord::databaseTableName() +
      " WHERE functionRecordType=:functionRecordType AND id=:id"));
  query.bindValue(":functionRecordType", FunctionRecordType::LinearFunctionRecord);
  query.bindValue(":id",id);
  assertExec(query);
  if (query.first()) {
    result = LinearFunctionRecord(query, database);
  }

  return result;
}

analysis::LinearFunction LinearFunctionRecord::linearFunction() const {
  return getImpl<detail::LinearFunctionRecord_Impl>()->linearFunction();
}

/// @cond
LinearFunctionRecord::LinearFunctionRecord(std::shared_ptr<detail::LinearFunctionRecord_Impl> impl)
  : FunctionRecord(impl)
{}
/// @endcond

void LinearFunctionRecord::removeVariableRecords(const std::vector<UUID>& uuidsToKeep,
                                                 ProjectDatabase& database)
{
  QSqlQuery query(*(database.qSqlDatabase()));
  std::stringstream ss;
  ss << "SELECT * FROM " + VariableRecord::databaseTableName() +
        " WHERE (functionRecordId=:functionRecordId) AND (handle NOT IN (";
  std::string sep("");
  for (const UUID& handle : uuidsToKeep) {
    ss << sep << "'" << toString(handle) << "'";
    sep = std::string(", ");
  }
  ss << "))";
  query.prepare(toQString(ss.str()));
  query.bindValue(":functionRecordId",id());
  assertExec(query);
  while (query.next()) {
    OptionalVariableRecord variableRecord = VariableRecord::factoryFromQuery(query, database);
    if (variableRecord) {
      database.removeRecord(*variableRecord);
    }
  }
}

} // project
} // openstudio

