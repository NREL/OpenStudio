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

#include "MeasureGroupRecord.hpp"
#include "MeasureGroupRecord_Impl.hpp"
#include "MeasureRecord.hpp"
#include "ProblemRecord.hpp"
#include "FunctionRecord.hpp"
#include "JoinRecord.hpp"
#include "ProjectDatabase.hpp"

#include "../analysis/MeasureGroup.hpp"
#include "../analysis/MeasureGroup_Impl.hpp"
#include "../analysis/Measure.hpp"
#include "../analysis/UncertaintyDescription.hpp"

#include "../utilities/core/Checksum.hpp"
#include "../utilities/core/Assert.hpp"

#include <sstream>

#include <QSqlQuery>
#include <QSqlError>

using namespace openstudio::analysis;

namespace openstudio {
namespace project {

namespace detail{

  MeasureGroupRecord_Impl::MeasureGroupRecord_Impl(const analysis::MeasureGroup& measureGroup,
                                                   ProblemRecord& problemRecord,
                                                   int workflowIndex)
    : DiscreteVariableRecord_Impl(measureGroup,
                                  DiscreteVariableRecordType::MeasureGroupRecord,
                                  problemRecord,
                                  workflowIndex)
  {}

  MeasureGroupRecord_Impl::MeasureGroupRecord_Impl(const analysis::MeasureGroup& measureGroup,
                                                   FunctionRecord& functionRecord,
                                                   int variableVectorIndex,
                                                   boost::optional<double> functionCoefficient)
    : DiscreteVariableRecord_Impl(measureGroup,
                                  DiscreteVariableRecordType::MeasureGroupRecord,
                                  functionRecord,
                                  variableVectorIndex,
                                  functionCoefficient)
  {}

  MeasureGroupRecord_Impl::MeasureGroupRecord_Impl(const QSqlQuery& query, ProjectDatabase& database)
    : DiscreteVariableRecord_Impl(query, database)
  {}

  std::vector<ObjectRecord> MeasureGroupRecord_Impl::children() const {
    ObjectRecordVector result = InputVariableRecord_Impl::children();
    std::vector<MeasureRecord> measureRecords = this->measureRecords(false);
    result.insert(result.end(),measureRecords.begin(),measureRecords.end());
    return result;
  }

  std::vector<ObjectRecord> MeasureGroupRecord_Impl::resources() const {
    return std::vector<ObjectRecord>();
  }

  void MeasureGroupRecord_Impl::saveRow(const std::shared_ptr<QSqlDatabase> &database) {
    QSqlQuery query(*database);
    this->makeUpdateByIdQuery<MeasureGroupRecord>(query);
    this->bindValues(query);
    assertExec(query);
  }

  analysis::Variable MeasureGroupRecord_Impl::variable() const {
    return measureGroup().cast<analysis::Variable>();
  }

  analysis::InputVariable MeasureGroupRecord_Impl::inputVariable() const {
    return measureGroup().cast<analysis::InputVariable>();
  }

  analysis::DiscreteVariable MeasureGroupRecord_Impl::discreteVariable() const {
    return measureGroup().cast<analysis::DiscreteVariable>();
  }

  analysis::MeasureGroup MeasureGroupRecord_Impl::measureGroup() const {
    analysis::MeasureVector measures;
    for (const MeasureRecord& measureRecord : measureRecords(false)) {
      measures.push_back(measureRecord.measure());
    }
    return analysis::MeasureGroup(handle(),
                                  uuidLast(),
                                  name(),
                                  displayName(),
                                  description(),
                                  uncertaintyDescription(),
                                  measures);
  }

  unsigned MeasureGroupRecord_Impl::numMeasures(bool selectedMeasuresOnly) const
  {
    unsigned result = 0;

    ProjectDatabase database = this->projectDatabase();

    QSqlQuery query(*(database.qSqlDatabase()));

    if (selectedMeasuresOnly){
      query.prepare(toQString("SELECT COUNT(*) FROM " + MeasureRecord::databaseTableName() +
          " WHERE variableRecordId=:id AND isSelected=:isSelected"));
      query.bindValue(":id", this->id());
      query.bindValue(":isSelected", true);
    }else{
      query.prepare(toQString("SELECT COUNT(*) FROM " + MeasureRecord::databaseTableName() +
          " WHERE variableRecordId=:id"));
      query.bindValue(":id", this->id());
    }

    assertExec(query);
    if (query.first()){
      result = query.value(0).toUInt();
    }

    return result;
  }

  std::vector<int> MeasureGroupRecord_Impl::measureRecordIds(bool selectedMeasuresOnly) const
  {
    std::vector<int> result;

    MeasureRecordVector dprs = measureRecords(selectedMeasuresOnly);
    for (const MeasureRecord& dpr : dprs) {
      result.push_back(dpr.id());
    }

    return result;
  }

  std::vector<MeasureRecord> MeasureGroupRecord_Impl::measureRecords(bool selectedMeasuresOnly) const
  {
    std::vector<MeasureRecord> result;

    ProjectDatabase database = this->projectDatabase();

    QSqlQuery query(*(database.qSqlDatabase()));

    if (selectedMeasuresOnly){
      query.prepare(toQString("SELECT * FROM " + MeasureRecord::databaseTableName() +
          " WHERE variableRecordId=:id AND isSelected=:isSelected ORDER BY measureVectorIndex"));
      query.bindValue(":id", this->id());
      query.bindValue(":isSelected", true);
    }else{
      query.prepare(toQString("SELECT * FROM " + MeasureRecord::databaseTableName() +
          " WHERE variableRecordId=:id ORDER BY measureVectorIndex"));
      query.bindValue(":id", this->id());
    }

    assertExec(query);
    OptionalInt previousIndex;
    bool resort(false);
    while (query.next()){
      boost::optional<MeasureRecord> measure = MeasureRecord::factoryFromQuery(query, database);
      OS_ASSERT(measure);
      result.push_back(*measure);
      OptionalInt index = result.back().measureVectorIndex();
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
      MeasureRecordMeasureVectorIndexLess comparator;
      std::sort(result.begin(),result.end(),comparator);
    }

    return result;
  }

  MeasureRecord MeasureGroupRecord_Impl::getMeasureRecord(int measureVectorIndex) const
  {
    ProjectDatabase database = this->projectDatabase();
    OptionalMeasureRecord candidate;

    QSqlQuery query(*(database.qSqlDatabase()));
    query.prepare(toQString("SELECT * FROM " + MeasureRecord::databaseTableName() +
        " WHERE variableRecordId=:variableRecordId AND measureVectorIndex=:measureVectorIndex"));
    query.bindValue(":variableRecordId", this->id());
    query.bindValue(":measureVectorIndex", measureVectorIndex);
    assertExec(query);
    if (query.first()) {
      candidate = MeasureRecord::factoryFromQuery(query,database).get();
    }

    // check actual vector index. can get out of date during construction.
    if ((!candidate) ||
        (!candidate->measureVectorIndex()) ||
        (candidate->measureVectorIndex().get() != measureVectorIndex))
    {
      // get all and look for index by hand
      for (const MeasureRecord& dpr : measureRecords(false)) {
        if (dpr.measureVectorIndex() && (dpr.measureVectorIndex().get() == measureVectorIndex)) {
          return dpr;
        }
      }
    }
    else {
      return *candidate;
    }
    LOG_AND_THROW("Invalid DiscretMeasure measureVectorIndex "
        << measureVectorIndex << " for MeasureGroup '" << name() << "'.");
    return MeasureRecord(std::shared_ptr<detail::MeasureRecord_Impl>(),database);
  }

  void MeasureGroupRecord_Impl::bindValues(QSqlQuery& query) const
  {
    DiscreteVariableRecord_Impl::bindValues(query);
  }

  void MeasureGroupRecord_Impl::setLastValues(const QSqlQuery& query, ProjectDatabase& projectDatabase)
  {
    DiscreteVariableRecord_Impl::setLastValues(query, projectDatabase);
  }

  bool MeasureGroupRecord_Impl::compareValues(const QSqlQuery& query) const
  {
    return DiscreteVariableRecord_Impl::compareValues(query);
  }

  void MeasureGroupRecord_Impl::saveLastValues()
  {
    DiscreteVariableRecord_Impl::saveLastValues();
  }

  void MeasureGroupRecord_Impl::revertToLastValues()
  {
    DiscreteVariableRecord_Impl::revertToLastValues();
  }

} // detail

MeasureGroupRecord::MeasureGroupRecord(const analysis::MeasureGroup& measureGroup,
                                       ProblemRecord& problemRecord,
                                       int variableVectorIndex)
  : DiscreteVariableRecord(std::shared_ptr<detail::MeasureGroupRecord_Impl>(
        new detail::MeasureGroupRecord_Impl(measureGroup,
                                            problemRecord,
                                            variableVectorIndex)),
        problemRecord.projectDatabase(),
        measureGroup)
{
  OS_ASSERT(getImpl<detail::MeasureGroupRecord_Impl>());

  constructMeasureRecords(measureGroup);
}

MeasureGroupRecord::MeasureGroupRecord(const analysis::MeasureGroup& measureGroup,
                                       FunctionRecord& functionRecord,
                                       int variableVectorIndex,
                                       boost::optional<double> functionCoefficient)
  : DiscreteVariableRecord(std::shared_ptr<detail::MeasureGroupRecord_Impl>(
        new detail::MeasureGroupRecord_Impl(measureGroup,
                                            functionRecord,
                                            variableVectorIndex,
                                            functionCoefficient)),
        functionRecord.projectDatabase(),
        measureGroup)
{
  OS_ASSERT(getImpl<detail::MeasureGroupRecord_Impl>());

  constructMeasureRecords(measureGroup);
}

MeasureGroupRecord::MeasureGroupRecord(const QSqlQuery& query, ProjectDatabase& database)
  : DiscreteVariableRecord(std::shared_ptr<detail::MeasureGroupRecord_Impl>(
        new detail::MeasureGroupRecord_Impl(query, database)),
        database,
        analysis::OptionalDiscreteVariable())
{
  OS_ASSERT(getImpl<detail::MeasureGroupRecord_Impl>());
}

MeasureGroupRecord::MeasureGroupRecord(std::shared_ptr<detail::MeasureGroupRecord_Impl> impl,
                                       ProjectDatabase database)
  : DiscreteVariableRecord(impl, database, analysis::OptionalDiscreteVariable())
{
  OS_ASSERT(getImpl<detail::MeasureGroupRecord_Impl>());
}

/// @cond
MeasureGroupRecord::MeasureGroupRecord(std::shared_ptr<detail::MeasureGroupRecord_Impl> impl)
  : DiscreteVariableRecord(impl)
{
  OS_ASSERT(getImpl<detail::MeasureGroupRecord_Impl>());
}
/// @endcond

boost::optional<MeasureGroupRecord> MeasureGroupRecord::factoryFromQuery(const QSqlQuery& query,
                                                                         ProjectDatabase& database)
{
  OptionalMeasureGroupRecord result;
  try {
    result = MeasureGroupRecord(query,database);
  }
  catch (const std::exception& e) {
    LOG(Error,"Unable to construct MeasureGroupRecord from query, because '"
        << e.what() << "'.");
  }
  return result;
}

std::vector<MeasureGroupRecord> MeasureGroupRecord::getMeasureGroupRecords(ProjectDatabase& database)
{
  std::vector<MeasureGroupRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + VariableRecord::databaseTableName() +
                          " WHERE variableRecordType=:variableRecordType AND " +
                          "inputVariableRecordType=:inputVariableRecordType AND " +
                          "discreteVariableRecordType=:discreteVariableRecordType"));
  query.bindValue(":variableRecordType", VariableRecordType::InputVariableRecord);
  query.bindValue(":inputVariableRecordType", InputVariableRecordType::DiscreteVariableRecord);
  query.bindValue(":discreteVariableRecordType", DiscreteVariableRecordType::MeasureGroupRecord);
  assertExec(query);
  while (query.next()) {
    result.push_back(MeasureGroupRecord(query, database));
  }

  return result;
}

boost::optional<MeasureGroupRecord> MeasureGroupRecord::getMeasureGroupRecord(int MeasureGroupRecordId,
                                                                              ProjectDatabase& database)
{
  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + VariableRecord::databaseTableName() + " WHERE id=:id AND " +
                          "inputVariableRecordType=:inputVariableRecordType AND " +
                          "discreteVariableRecordType=:discreteVariableRecordType"));
  query.bindValue(":id", MeasureGroupRecordId);
  query.bindValue(":variableRecordType", VariableRecordType::InputVariableRecord);
  query.bindValue(":inputVariableRecordType", InputVariableRecordType::DiscreteVariableRecord);
  query.bindValue(":discreteVariableRecordType", DiscreteVariableRecordType::MeasureGroupRecord);
  assertExec(query);
  if (query.first()) {
    return MeasureGroupRecord(query, database);
  }

  return boost::none;
}

analysis::MeasureGroup MeasureGroupRecord::measureGroup() const {
  return getImpl<detail::MeasureGroupRecord_Impl>()->measureGroup();
}

unsigned MeasureGroupRecord::numMeasures(bool selectedMeasuresOnly) const {
  return getImpl<detail::MeasureGroupRecord_Impl>()->numMeasures(selectedMeasuresOnly);
}

std::vector<int> MeasureGroupRecord::measureRecordIds(bool selectedMeasuresOnly) const {
  return getImpl<detail::MeasureGroupRecord_Impl>()->measureRecordIds(selectedMeasuresOnly);
}

std::vector<MeasureRecord> MeasureGroupRecord::measureRecords(
    bool selectedMeasuresOnly) const
{
  return getImpl<detail::MeasureGroupRecord_Impl>()->measureRecords(selectedMeasuresOnly);
}

MeasureRecord MeasureGroupRecord::getMeasureRecord(
    int measureVectorIndex) const
{
  return getImpl<detail::MeasureGroupRecord_Impl>()->getMeasureRecord(measureVectorIndex);
}

void MeasureGroupRecord::constructMeasureRecords(const analysis::MeasureGroup& measureGroup) {
  MeasureGroupRecord copyOfThis(getImpl<detail::MeasureGroupRecord_Impl>());
  ProjectDatabase database = copyOfThis.projectDatabase();
  bool isNew = database.isNewRecord(copyOfThis);

  int i = 0;
  std::vector<UUID> measureUUIDs;
  for (const Measure& measure : measureGroup.measures(false)) {
    measureUUIDs.push_back(measure.uuid());
    if (measure.isDirty() || isNew) {
      MeasureRecord newMeasureRecord =
          MeasureRecord::factoryFromMeasure(measure,copyOfThis,i);
    }
    ++i;
  }
  if (!isNew) {
    removeMeasureRecords(measureUUIDs,database);
  }
}

void MeasureGroupRecord::removeMeasureRecords(const std::vector<UUID>& uuidsToKeep,
                                              ProjectDatabase& database)
{
  QSqlQuery query(*(database.qSqlDatabase()));
  std::stringstream ss;
  ss << "SELECT * FROM " + MeasureRecord::databaseTableName() +
        " WHERE (variableRecordId=:variableRecordId) AND (handle NOT IN (";
  std::string sep("");
  for (const UUID& handle : uuidsToKeep) {
    ss << sep << "'" << toString(handle) << "'";
    sep = std::string(", ");
  }
  ss << "))";
  query.prepare(toQString(ss.str()));
  query.bindValue(":variableRecordId",id());
  assertExec(query);
  while (query.next()) {
    OptionalMeasureRecord measureRecord = MeasureRecord::factoryFromQuery(query, database);
    if (measureRecord) {
      database.removeRecord(*measureRecord);
    }
  }
}

} // project
} // openstudio

