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

#include <project/DiscreteVariableRecord.hpp>
#include <project/DiscreteVariableRecord_Impl.hpp>
#include <project/DiscretePerturbationRecord.hpp>
#include <project/ProblemRecord.hpp>
#include <project/FunctionRecord.hpp>
#include <project/JoinRecord.hpp>
#include <project/ProjectDatabase.hpp>

#include <analysis/DiscreteVariable.hpp>
#include <analysis/DiscreteVariable_Impl.hpp>
#include <analysis/DiscretePerturbation.hpp>
#include <analysis/UncertaintyDescription.hpp>

#include <utilities/core/Checksum.hpp>
#include <utilities/core/Assert.hpp>

#include <boost/foreach.hpp>

#include <sstream>

#include <QSqlQuery>
#include <QSqlError>

using namespace openstudio::analysis;

namespace openstudio {
namespace project {

namespace detail{

  DiscreteVariableRecord_Impl::DiscreteVariableRecord_Impl(
      const analysis::DiscreteVariable& discreteVariable,
      ProblemRecord& problemRecord,
      int workflowIndex)
    : InputVariableRecord_Impl(discreteVariable,
                               InputVariableRecordType::DiscreteVariableRecord,
                               problemRecord,
                               workflowIndex)
  {}

  DiscreteVariableRecord_Impl::DiscreteVariableRecord_Impl(
      const analysis::DiscreteVariable& discreteVariable,
      FunctionRecord& functionRecord,
      int variableVectorIndex,
      boost::optional<double> functionCoefficient)
    : InputVariableRecord_Impl(discreteVariable,
                               InputVariableRecordType::DiscreteVariableRecord,
                               functionRecord,
                               variableVectorIndex,
                               functionCoefficient)
  {}

  DiscreteVariableRecord_Impl::DiscreteVariableRecord_Impl(const QSqlQuery& query, ProjectDatabase& database)
    : InputVariableRecord_Impl(query, database)
  {}

  std::vector<ObjectRecord> DiscreteVariableRecord_Impl::children() const
  {
    ObjectRecordVector result = InputVariableRecord_Impl::children();
    std::vector<DiscretePerturbationRecord> discretePerturbationRecords = this->discretePerturbationRecords(false);
    result.insert(result.end(),discretePerturbationRecords.begin(),discretePerturbationRecords.end());
    return result;
  }

  std::vector<ObjectRecord> DiscreteVariableRecord_Impl::resources() const
  {
    return std::vector<ObjectRecord>();
  }

  void DiscreteVariableRecord_Impl::saveRow(const boost::shared_ptr<QSqlDatabase> &database)
  {
    QSqlQuery query(*database);
    this->makeUpdateByIdQuery<DiscreteVariableRecord>(query);
    this->bindValues(query);
    assertExec(query);
  }

  analysis::Variable DiscreteVariableRecord_Impl::variable() const {
    return discreteVariable().cast<analysis::Variable>();
  }

  analysis::InputVariable DiscreteVariableRecord_Impl::inputVariable() const {
    return discreteVariable().cast<analysis::InputVariable>();
  }

  analysis::DiscreteVariable DiscreteVariableRecord_Impl::discreteVariable() const {
    analysis::DiscretePerturbationVector perturbations;
    BOOST_FOREACH(const DiscretePerturbationRecord& discretePerturbationRecord,discretePerturbationRecords(false)) {
      perturbations.push_back(discretePerturbationRecord.discretePerturbation());
    }
    return analysis::DiscreteVariable(handle(),
                                      uuidLast(),
                                      name(),
                                      displayName(),
                                      description(),
                                      uncertaintyDescription(),
                                      perturbations);
  }

  unsigned DiscreteVariableRecord_Impl::numPerturbations(bool selectedPerturbationsOnly) const
  {
    unsigned result = 0;

    ProjectDatabase database = this->projectDatabase();

    QSqlQuery query(*(database.qSqlDatabase()));

    if (selectedPerturbationsOnly){
      query.prepare(toQString("SELECT COUNT(*) FROM " + DiscretePerturbationRecord::databaseTableName() +
          " WHERE variableRecordId=:id AND isSelected=:isSelected"));
      query.bindValue(":id", this->id());
      query.bindValue(":isSelected", true);
    }else{
      query.prepare(toQString("SELECT COUNT(*) FROM " + DiscretePerturbationRecord::databaseTableName() +
          " WHERE variableRecordId=:id"));
      query.bindValue(":id", this->id());
    }

    assertExec(query);
    if (query.first()){
      result = query.value(0).toUInt();
    }

    return result;
  }

  std::vector<int> DiscreteVariableRecord_Impl::discretePerturbationRecordIds(bool selectedPerturbationsOnly) const
  {
    std::vector<int> result;

    DiscretePerturbationRecordVector dprs = discretePerturbationRecords(selectedPerturbationsOnly);
    BOOST_FOREACH(const DiscretePerturbationRecord& dpr,dprs) {
      result.push_back(dpr.id());
    }

    return result;
  }

  std::vector<DiscretePerturbationRecord> DiscreteVariableRecord_Impl::discretePerturbationRecords(
      bool selectedPerturbationsOnly) const
  {
    std::vector<DiscretePerturbationRecord> result;

    ProjectDatabase database = this->projectDatabase();

    QSqlQuery query(*(database.qSqlDatabase()));

    if (selectedPerturbationsOnly){
      query.prepare(toQString("SELECT * FROM " + DiscretePerturbationRecord::databaseTableName() +
          " WHERE variableRecordId=:id AND isSelected=:isSelected ORDER BY perturbationVectorIndex"));
      query.bindValue(":id", this->id());
      query.bindValue(":isSelected", true);
    }else{
      query.prepare(toQString("SELECT * FROM " + DiscretePerturbationRecord::databaseTableName() +
          " WHERE variableRecordId=:id ORDER BY perturbationVectorIndex"));
      query.bindValue(":id", this->id());
    }

    assertExec(query);
    OptionalInt previousIndex;
    bool resort(false);
    while (query.next()){
      boost::optional<DiscretePerturbationRecord> perturbation = DiscretePerturbationRecord::factoryFromQuery(query, database);
      OS_ASSERT(perturbation);
      result.push_back(*perturbation);
      OptionalInt index = result.back().perturbationVectorIndex();
      if (previousIndex && index) {
        if (index.get() <= previousIndex.get()) {
          resort = true;
        }
      }
      if (index) {
        previousIndex = index;
      }
    }

    if (resort) {
      DiscretePerturbationRecordPerturbationVectorIndexLess comparator;
      std::sort(result.begin(),result.end(),comparator);
    }

    return result;
  }

  DiscretePerturbationRecord DiscreteVariableRecord_Impl::getDiscretePerturbationRecord(
      int perturbationVectorIndex) const
  {
    ProjectDatabase database = this->projectDatabase();
    OptionalDiscretePerturbationRecord candidate;

    QSqlQuery query(*(database.qSqlDatabase()));
    query.prepare(toQString("SELECT * FROM " + DiscretePerturbationRecord::databaseTableName() +
        " WHERE variableRecordId=:variableRecordId AND perturbationVectorIndex=:perturbationVectorIndex"));
    query.bindValue(":variableRecordId", this->id());
    query.bindValue(":perturbationVectorIndex", perturbationVectorIndex);
    assertExec(query);
    if (query.first()) {
      candidate = DiscretePerturbationRecord::factoryFromQuery(query,database).get();
    }

    // check actual vector index. can get out of date during construction.
    if ((!candidate) ||
        (!candidate->perturbationVectorIndex()) ||
        (candidate->perturbationVectorIndex().get() != perturbationVectorIndex))
    {
      // get all and look for index by hand
      BOOST_FOREACH(const DiscretePerturbationRecord& dpr,discretePerturbationRecords(false)) {
        if (dpr.perturbationVectorIndex() && (dpr.perturbationVectorIndex().get() == perturbationVectorIndex)) {
          return dpr;
        }
      }
    }
    else {
      return *candidate;
    }
    LOG_AND_THROW("Invalid DiscretPerturbation perturbationVectorIndex "
        << perturbationVectorIndex << " for DiscreteVariable '" << name() << "'.");
    return DiscretePerturbationRecord(boost::shared_ptr<detail::DiscretePerturbationRecord_Impl>(),database);
  }

  void DiscreteVariableRecord_Impl::bindValues(QSqlQuery& query) const
  {
    InputVariableRecord_Impl::bindValues(query);
  }

  void DiscreteVariableRecord_Impl::setLastValues(const QSqlQuery& query, ProjectDatabase& projectDatabase)
  {
    InputVariableRecord_Impl::setLastValues(query, projectDatabase);
  }

  bool DiscreteVariableRecord_Impl::compareValues(const QSqlQuery& query) const
  {
    return InputVariableRecord_Impl::compareValues(query);
  }

  void DiscreteVariableRecord_Impl::saveLastValues()
  {
    InputVariableRecord_Impl::saveLastValues();
  }

  void DiscreteVariableRecord_Impl::revertToLastValues()
  {
    InputVariableRecord_Impl::revertToLastValues();
  }

} // detail

DiscreteVariableRecord::DiscreteVariableRecord(const analysis::DiscreteVariable& discreteVariable,
                                               ProblemRecord& problemRecord,
                                               int variableVectorIndex)
  : InputVariableRecord(boost::shared_ptr<detail::DiscreteVariableRecord_Impl>(
        new detail::DiscreteVariableRecord_Impl(discreteVariable,
                                                problemRecord,
                                                variableVectorIndex)),
        problemRecord.projectDatabase(),
        discreteVariable)
{
  OS_ASSERT(getImpl<detail::DiscreteVariableRecord_Impl>());

  constructDiscretePerturbationRecords(discreteVariable);
}

DiscreteVariableRecord::DiscreteVariableRecord(const analysis::DiscreteVariable& discreteVariable,
                                               FunctionRecord& functionRecord,
                                               int variableVectorIndex,
                                               boost::optional<double> functionCoefficient)
  : InputVariableRecord(boost::shared_ptr<detail::DiscreteVariableRecord_Impl>(
        new detail::DiscreteVariableRecord_Impl(discreteVariable,
                                                functionRecord,
                                                variableVectorIndex,
                                                functionCoefficient)),
        functionRecord.projectDatabase(),
        discreteVariable)
{
  OS_ASSERT(getImpl<detail::DiscreteVariableRecord_Impl>());

  constructDiscretePerturbationRecords(discreteVariable);
}

DiscreteVariableRecord::DiscreteVariableRecord(const QSqlQuery& query, ProjectDatabase& database)
  : InputVariableRecord(boost::shared_ptr<detail::DiscreteVariableRecord_Impl>(
        new detail::DiscreteVariableRecord_Impl(query, database)),
        database,
        analysis::OptionalInputVariable())
{
  OS_ASSERT(getImpl<detail::DiscreteVariableRecord_Impl>());
}

DiscreteVariableRecord::DiscreteVariableRecord(boost::shared_ptr<detail::DiscreteVariableRecord_Impl> impl,
                                               ProjectDatabase database)
  : InputVariableRecord(impl, database, analysis::OptionalInputVariable())
{
  OS_ASSERT(getImpl<detail::DiscreteVariableRecord_Impl>());
}

/// @cond
DiscreteVariableRecord::DiscreteVariableRecord(boost::shared_ptr<detail::DiscreteVariableRecord_Impl> impl)
  : InputVariableRecord(impl)
{
  OS_ASSERT(getImpl<detail::DiscreteVariableRecord_Impl>());
}
/// @endcond

boost::optional<DiscreteVariableRecord> DiscreteVariableRecord::factoryFromQuery(
    const QSqlQuery& query, ProjectDatabase& database)
{
  OptionalDiscreteVariableRecord result;
  try {
    result = DiscreteVariableRecord(query,database);
  }
  catch (const std::exception& e) {
    LOG(Error,"Unable to construct DiscreteVariableRecord from query, because '"
        << e.what() << "'.");
  }
  return result;
}

std::vector<DiscreteVariableRecord> DiscreteVariableRecord::getDiscreteVariableRecords(ProjectDatabase& database)
{
  std::vector<DiscreteVariableRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + VariableRecord::databaseTableName() +
                          " WHERE variableRecordType=:variableRecordType AND " +
                          "inputVariableRecordType=:inputVariableRecordType"));
  query.bindValue(":variableRecordType", VariableRecordType::InputVariableRecord);
  query.bindValue(":inputVariableRecordType", InputVariableRecordType::DiscreteVariableRecord);
  assertExec(query);
  while (query.next()) {
    result.push_back(DiscreteVariableRecord(query, database));
  }

  return result;
}

boost::optional<DiscreteVariableRecord> DiscreteVariableRecord::getDiscreteVariableRecord(int DiscreteVariableRecordId, ProjectDatabase& database)
{
  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + VariableRecord::databaseTableName() + " WHERE id=:id AND " +
                          "variableRecordType=:variableRecordType AND " +
                          "inputVariableRecordType=:inputVariableRecordType"));
  query.bindValue(":id", DiscreteVariableRecordId);
  query.bindValue(":variableRecordType", VariableRecordType::InputVariableRecord);
  query.bindValue(":inputVariableRecordType", InputVariableRecordType::DiscreteVariableRecord);
  assertExec(query);
  if (query.first()) {
    return DiscreteVariableRecord(query, database);
  }

  return boost::none;
}

analysis::DiscreteVariable DiscreteVariableRecord::discreteVariable() const {
  return getImpl<detail::DiscreteVariableRecord_Impl>()->discreteVariable();
}

unsigned DiscreteVariableRecord::numPerturbations(bool selectedPerturbationsOnly) const {
  return getImpl<detail::DiscreteVariableRecord_Impl>()->numPerturbations(selectedPerturbationsOnly);
}

std::vector<int> DiscreteVariableRecord::discretePerturbationRecordIds(bool selectedPerturbationsOnly) const {
  return getImpl<detail::DiscreteVariableRecord_Impl>()->discretePerturbationRecordIds(selectedPerturbationsOnly);
}

std::vector<DiscretePerturbationRecord> DiscreteVariableRecord::discretePerturbationRecords(
    bool selectedPerturbationsOnly) const
{
  return getImpl<detail::DiscreteVariableRecord_Impl>()->discretePerturbationRecords(selectedPerturbationsOnly);
}

DiscretePerturbationRecord DiscreteVariableRecord::getDiscretePerturbationRecord(
    int perturbationVectorIndex) const
{
  return getImpl<detail::DiscreteVariableRecord_Impl>()->getDiscretePerturbationRecord(perturbationVectorIndex);
}

void DiscreteVariableRecord::constructDiscretePerturbationRecords(const analysis::DiscreteVariable& discreteVariable) {
  DiscreteVariableRecord copyOfThis(getImpl<detail::DiscreteVariableRecord_Impl>());
  ProjectDatabase database = copyOfThis.projectDatabase();
  bool isNew = database.isNewRecord(copyOfThis);

  int i = 0;
  std::vector<UUID> perturbationUUIDs;
  BOOST_FOREACH(const DiscretePerturbation& perturbation,discreteVariable.perturbations(false)) {
    perturbationUUIDs.push_back(perturbation.uuid());
    if (perturbation.isDirty() || isNew) {
      DiscretePerturbationRecord newDiscretePerturbationRecord =
          DiscretePerturbationRecord::factoryFromDiscretePerturbation(perturbation,copyOfThis,i);
    }
    ++i;
  }
  if (!isNew) {
    removeDiscretePerturbationRecords(perturbationUUIDs,database);
  }
}

void DiscreteVariableRecord::removeDiscretePerturbationRecords(const std::vector<UUID>& uuidsToKeep,
                                                               ProjectDatabase& database)
{
  QSqlQuery query(*(database.qSqlDatabase()));
  std::stringstream ss;
  ss << "SELECT * FROM " + DiscretePerturbationRecord::databaseTableName() +
        " WHERE (variableRecordId=:variableRecordId) AND (handle NOT IN (";
  std::string sep("");
  BOOST_FOREACH(const UUID& handle,uuidsToKeep) {
    ss << sep << "'" << toString(handle) << "'";
    sep = std::string(", ");
  }
  ss << "))";
  query.prepare(toQString(ss.str()));
  query.bindValue(":variableRecordId",id());
  assertExec(query);
  while (query.next()) {
    OptionalDiscretePerturbationRecord perturbationRecord = DiscretePerturbationRecord::factoryFromQuery(query, database);
    if (perturbationRecord) {
      database.removeRecord(*perturbationRecord);
    }
  }
}

} // project
} // openstudio

