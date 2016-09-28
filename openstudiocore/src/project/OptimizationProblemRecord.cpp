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

#include "OptimizationProblemRecord.hpp"
#include "OptimizationProblemRecord_Impl.hpp"

#include "JoinRecord.hpp"
#include "FunctionRecord.hpp"

#include "../analysis/OptimizationProblem.hpp"
#include "../analysis/OptimizationProblem_Impl.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/core/Optional.hpp"

namespace openstudio {
namespace project {

namespace detail {

  OptimizationProblemRecord_Impl::OptimizationProblemRecord_Impl(
      const analysis::OptimizationProblem& optimizationProblem,
      ProjectDatabase& database)
    : ProblemRecord_Impl(optimizationProblem,
                         ProblemRecordType::OptimizationProblemRecord,
                         database)
  {}

  OptimizationProblemRecord_Impl::OptimizationProblemRecord_Impl(const QSqlQuery& query,
                                                                 ProjectDatabase& database)
    : ProblemRecord_Impl(query, database)
  {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());
  }

  std::vector<ObjectRecord> OptimizationProblemRecord_Impl::children() const {
    ObjectRecordVector result = ProblemRecord_Impl::children();

    FunctionRecordVector objectiveRecords = this->objectiveRecords();
    result.insert(result.end(),objectiveRecords.begin(),objectiveRecords.end());

    return result;
  }

  void OptimizationProblemRecord_Impl::saveRow(const std::shared_ptr<QSqlDatabase> &database)
  {
    QSqlQuery query(*database);
    this->makeUpdateByIdQuery<OptimizationProblemRecord>(query);
    this->bindValues(query);
    assertExec(query);
  }

  std::vector<FunctionRecord> OptimizationProblemRecord_Impl::objectiveRecords() const {
    FunctionRecordVector result;

    ProjectDatabase database = projectDatabase();
    QSqlQuery query(*(database.qSqlDatabase()));
    query.prepare(toQString("SELECT * FROM " + FunctionRecord::databaseTableName() +
                  " WHERE problemRecordId=:problemRecordId AND functionType=:functionType " +
                  "ORDER BY functionVectorIndex"));
    query.bindValue(":problemRecordId", id());
    query.bindValue(":functionType",FunctionType::Objective);
    assertExec(query);
    OptionalInt previousIndex;
    bool resort(false);
    while (query.next()) {
      result.push_back(FunctionRecord::factoryFromQuery(query, database).get());
      int index = result.back().functionVectorIndex();
      if (previousIndex) {
        if (index <= previousIndex.get()) {
          resort = true; // if functions are moved around, order in database itself can
                         // get out of date (as compared to database + dirty records).
        }
      }
      previousIndex = index;
    }

    if (resort) {
      FunctionRecordFunctionVectorIndexLess comparator;
      std::sort(result.begin(),result.end(),comparator);
    }

    return result;
  }

  analysis::Problem OptimizationProblemRecord_Impl::problem() const {
    return optimizationProblem().cast<analysis::Problem>();
  }

  analysis::OptimizationProblem OptimizationProblemRecord_Impl::optimizationProblem() const {
    analysis::Problem prelim = ProblemRecord_Impl::problem();
    analysis::FunctionVector objectives;
    for (const FunctionRecord& functionRecord : objectiveRecords()) {
      objectives.push_back(functionRecord.function());
    }
    return analysis::OptimizationProblem(prelim.uuid(),
                                         prelim.versionUUID(),
                                         prelim.name(),
                                         prelim.displayName(),
                                         prelim.description(),
                                         objectives,
                                         prelim.workflow(),
                                         prelim.responses());
  }

  void OptimizationProblemRecord_Impl::bindValues(QSqlQuery& query) const {
    ProblemRecord_Impl::bindValues(query);
  }

  void OptimizationProblemRecord_Impl::setLastValues(const QSqlQuery& query, ProjectDatabase& projectDatabase) {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    ProblemRecord_Impl::setLastValues(query,projectDatabase);
  }

  bool OptimizationProblemRecord_Impl::compareValues(const QSqlQuery& query) const {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    bool result = ProblemRecord_Impl::compareValues(query);

    return result;
  }

  void OptimizationProblemRecord_Impl::saveLastValues() {
    ProblemRecord_Impl::saveLastValues();
  }

  void OptimizationProblemRecord_Impl::revertToLastValues() {
    ProblemRecord_Impl::revertToLastValues();
  }

} // detail

OptimizationProblemRecord::OptimizationProblemRecord(
    const analysis::OptimizationProblem& optimizationProblem,
    ProjectDatabase& database)
  : ProblemRecord(std::shared_ptr<detail::OptimizationProblemRecord_Impl>(
        new detail::OptimizationProblemRecord_Impl(optimizationProblem, database)),
        database)
{
  OS_ASSERT(getImpl<detail::OptimizationProblemRecord_Impl>());
  OptimizationProblemRecord copyOfThis(getImpl<detail::OptimizationProblemRecord_Impl>());
  bool isNew = database.isNewRecord(copyOfThis);

  constructRelatedRecords(optimizationProblem);

  // Save child objective functions
  int i = 0;
  std::vector<UUID> objectiveUUIDs;
  for (const analysis::Function& objective : optimizationProblem.objectives()) {
    objectiveUUIDs.push_back(objective.uuid());
    if (objective.isDirty() || isNew) {
      FunctionRecord newFunctionRecord = FunctionRecord::factoryFromFunction(
            objective,
            copyOfThis,
            FunctionType::Objective,
            i);
    }
    ++i;
  }
  if (!isNew) {
    removeObjectiveRecords(objectiveUUIDs,database);
  }
}

OptimizationProblemRecord::OptimizationProblemRecord(const QSqlQuery& query,
                                                     ProjectDatabase& database)
  : ProblemRecord(std::shared_ptr<detail::OptimizationProblemRecord_Impl>(
        new detail::OptimizationProblemRecord_Impl(query, database)),
        database)
{
  OS_ASSERT(getImpl<detail::OptimizationProblemRecord_Impl>());
}

OptimizationProblemRecord::OptimizationProblemRecord(
    std::shared_ptr<detail::OptimizationProblemRecord_Impl> impl,
    ProjectDatabase database)
  : ProblemRecord(impl, database)
{
  OS_ASSERT(getImpl<detail::OptimizationProblemRecord_Impl>());
}

boost::optional<OptimizationProblemRecord> OptimizationProblemRecord::factoryFromQuery(
    const QSqlQuery& query, ProjectDatabase& database)
{
  OptionalOptimizationProblemRecord result;
  try {
    result = OptimizationProblemRecord(query,database);
  }
  catch (const std::exception& e) {
    LOG(Error,"Unable to construct OptimizationProblemRecord from query, because '"
        << e.what() << "'.");
  }
  return result;
}

std::vector<OptimizationProblemRecord> OptimizationProblemRecord::getOptimizationProblemRecords(ProjectDatabase& database) {
  std::vector<OptimizationProblemRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + ProblemRecord::databaseTableName() +
      " WHERE problemRecordType=:problemRecordType"));
  query.bindValue(":problemRecordType", ProblemRecordType::OptimizationProblemRecord);
  assertExec(query);
  while (query.next()) {
    result.push_back(OptimizationProblemRecord(query, database));
  }

  return result;
}

boost::optional<OptimizationProblemRecord> OptimizationProblemRecord::getOptimizationProblemRecord(
    int id, ProjectDatabase& database)
{
  boost::optional<OptimizationProblemRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + ProblemRecord::databaseTableName() +
      " WHERE problemRecordType=:problemRecordType AND id=:id"));
  query.bindValue(":problemRecordType", ProblemRecordType::OptimizationProblemRecord);
  query.bindValue(":id",id);
  assertExec(query);
  if (query.first()) {
    result = OptimizationProblemRecord(query, database);
  }

  return result;
}

std::vector<FunctionRecord> OptimizationProblemRecord::objectiveRecords() const {
  return getImpl<detail::OptimizationProblemRecord_Impl>()->objectiveRecords();
}

analysis::OptimizationProblem OptimizationProblemRecord::optimizationProblem() const {
  return getImpl<detail::OptimizationProblemRecord_Impl>()->optimizationProblem();
}

/// @cond
OptimizationProblemRecord::OptimizationProblemRecord(std::shared_ptr<detail::OptimizationProblemRecord_Impl> impl)
  : ProblemRecord(impl)
{}
/// @endcond

void OptimizationProblemRecord::removeObjectiveRecords(const std::vector<UUID>& uuidsToKeep,
                                                       ProjectDatabase& database)
{
  QSqlQuery query(*(database.qSqlDatabase()));
  std::stringstream ss;
  ss << "SELECT * FROM " + FunctionRecord::databaseTableName() + " WHERE ";
  ss << "(problemRecordId=:problemRecordId) AND (functionType=:functionType) AND ";
  ss << "(handle NOT IN (";
  std::string sep("");
  for (const UUID& handle : uuidsToKeep) {
    ss << sep << "'" << toString(handle) << "'";
    sep = std::string(", ");
  }
  ss << "))";
  query.prepare(toQString(ss.str()));
  query.bindValue(":problemRecordId",id());
  query.bindValue(":functionType",FunctionType(FunctionType::Objective).value());
  assertExec(query);
  while (query.next()) {
    OptionalFunctionRecord functionRecord = FunctionRecord::factoryFromQuery(query, database);
    if (functionRecord) {
      database.removeRecord(*functionRecord);
    }
  }
}

} // project
} // openstudio
