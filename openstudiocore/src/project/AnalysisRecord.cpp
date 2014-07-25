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

#include "AnalysisRecord.hpp"
#include "AnalysisRecord_Impl.hpp"

#include "JoinRecord.hpp"
#include "ProblemRecord.hpp"
#include "ProblemRecord_Impl.hpp"
#include "AlgorithmRecord.hpp"
#include "DataPointRecord.hpp"
#include "DataPointRecord_Impl.hpp"
#include "FileReferenceRecord.hpp"
#include "InputVariableRecord.hpp"
#include "MeasureRecord.hpp"
#include "DataPoint_Measure_JoinRecord.hpp"
#include "TagRecord.hpp"

#include "../analysis/Analysis.hpp"
#include "../analysis/Problem.hpp"
#include "../analysis/Algorithm.hpp"
#include "../analysis/DataPoint.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/core/FileReference.hpp"
#include "../utilities/core/Containers.hpp"

using namespace openstudio::analysis;

namespace openstudio {
namespace project {

namespace detail {

  AnalysisRecord_Impl::AnalysisRecord_Impl(const analysis::Analysis& analysis,
                                           ProjectDatabase& database)
    : ObjectRecord_Impl(database,
                        analysis.uuid(),
                        analysis.name(),
                        analysis.displayName(),
                        analysis.description(),
                        analysis.versionUUID()),
      m_resultsAreInvalid(analysis.resultsAreInvalid()),
      m_dataPointsAreInvalid(analysis.dataPointsAreInvalid())
  {}

  AnalysisRecord_Impl::AnalysisRecord_Impl(const QSqlQuery& query, ProjectDatabase& database)
    : ObjectRecord_Impl(database, query)
  {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    QVariant value;

    value = query.value(AnalysisRecordColumns::problemRecordId);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_problemRecordId = value.toInt();

    value = query.value(AnalysisRecordColumns::algorithmRecordId);
    if (value.isValid() && !value.isNull()) {
      m_algorithmRecordId = value.toInt();
    }

    value = query.value(AnalysisRecordColumns::seedFileReferenceRecordId);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_seedFileReferenceRecordId = value.toInt();

    value = query.value(AnalysisRecordColumns::weatherFileReferenceRecordId);
    if (value.isValid() && !value.isNull()) {
       m_weatherFileReferenceRecordId = value.toInt();
    }

    value = query.value(AnalysisRecordColumns::resultsAreInvalid);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_resultsAreInvalid = value.toBool();

    value = query.value(AnalysisRecordColumns::dataPointsAreInvalid);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_dataPointsAreInvalid = value.toBool();
  }

  std::string AnalysisRecord_Impl::databaseTableName() const {
    return AnalysisRecord::databaseTableName();
  }

  boost::optional<ObjectRecord> AnalysisRecord_Impl::parent() const {
    return boost::none;
  }

  std::vector<ObjectRecord> AnalysisRecord_Impl::children() const {
    ObjectRecordVector result;
    DataPointRecordVector dataPointRecords = this->dataPointRecords();
    result.insert(result.end(),dataPointRecords.begin(),dataPointRecords.end());
    result.push_back(problemRecord());
    if (m_algorithmRecordId) {
      result.push_back(algorithmRecord().get());
    }
    result.push_back(seedFileReferenceRecord());
    if (m_weatherFileReferenceRecordId) {
      result.push_back(weatherFileReferenceRecord().get());
    }
    return result;
  }

  std::vector<ObjectRecord> AnalysisRecord_Impl::resources() const {
    ObjectRecordVector result;
    return result;
  }

  std::vector<JoinRecord> AnalysisRecord_Impl::joinRecords() const {
    JoinRecordVector result;
    return result;
  }

  void AnalysisRecord_Impl::saveRow(const std::shared_ptr<QSqlDatabase> &database)
  {
    QSqlQuery query(*database);
    this->makeUpdateByIdQuery<AnalysisRecord>(query);
    this->bindValues(query);
    assertExec(query);
  }

  ProblemRecord AnalysisRecord_Impl::problemRecord() const {
    ProjectDatabase database = projectDatabase();
    OptionalProblemRecord result = ProblemRecord::getProblemRecord(m_problemRecordId,database);
    if (!result) {
      LOG_AND_THROW("Unable to retrieve problem record using id " << m_problemRecordId << ".");
    }
    return result.get();
  }

  boost::optional<AlgorithmRecord> AnalysisRecord_Impl::algorithmRecord() const {
    OptionalAlgorithmRecord result;
    if (m_algorithmRecordId) {
      ProjectDatabase database = projectDatabase();
      result = AlgorithmRecord::getAlgorithmRecord(*m_algorithmRecordId,database).get();
    }
    return result;
  }

  FileReferenceRecord AnalysisRecord_Impl::seedFileReferenceRecord() const {
    ProjectDatabase database = projectDatabase();
    return FileReferenceRecord::getFileReferenceRecord(m_seedFileReferenceRecordId,database).get();
  }

  boost::optional<FileReferenceRecord> AnalysisRecord_Impl::weatherFileReferenceRecord() const {
    OptionalFileReferenceRecord result;
    if (m_weatherFileReferenceRecordId) {
      ProjectDatabase database = projectDatabase();
      result = FileReferenceRecord::getFileReferenceRecord(*m_weatherFileReferenceRecordId,database);
    }
    return result;
  }

  std::vector<DataPointRecord> AnalysisRecord_Impl::dataPointRecords() const {
    DataPointRecordVector result;

    ProjectDatabase database = projectDatabase();
    QSqlQuery query(*(database.qSqlDatabase()));
    query.prepare(toQString("SELECT * FROM " + DataPointRecord::databaseTableName() +
        " WHERE analysisRecordId=:analysisRecordId" ));
    query.bindValue(":analysisRecordId",id());
    assertExec(query);
    while (query.next()) {
      result.push_back(DataPointRecord::factoryFromQuery(query, database).get());
    }

    return result;
  }

  std::vector<DataPointRecord> AnalysisRecord_Impl::incompleteDataPointRecords() const {
    DataPointRecordVector result;

    ProjectDatabase database = projectDatabase();
    QSqlQuery query(*(database.qSqlDatabase()));
    query.prepare(toQString("SELECT * FROM " + DataPointRecord::databaseTableName() +
        " WHERE analysisRecordId=:analysisRecordId AND complete=:complete" ));
    query.bindValue(":analysisRecordId",id());
    query.bindValue(":complete",false);
    assertExec(query);
    while (query.next()) {
      result.push_back(DataPointRecord::factoryFromQuery(query, database).get());
    }

    return result;
  }

  std::vector<DataPointRecord> AnalysisRecord_Impl::completeDataPointRecords() const {
    DataPointRecordVector result;

    ProjectDatabase database = projectDatabase();
    QSqlQuery query(*(database.qSqlDatabase()));
    query.prepare(toQString("SELECT * FROM " + DataPointRecord::databaseTableName() +
        " WHERE analysisRecordId=:analysisRecordId AND complete=:complete" ));
    query.bindValue(":analysisRecordId",id());
    query.bindValue(":complete",true);
    assertExec(query);
    while (query.next()) {
      result.push_back(DataPointRecord::factoryFromQuery(query, database).get());
    }

    return result;
  }

  std::vector<DataPointRecord> AnalysisRecord_Impl::successfulDataPointRecords() const {
    DataPointRecordVector result;

    ProjectDatabase database = projectDatabase();
    QSqlQuery query(*(database.qSqlDatabase()));
    query.prepare(toQString("SELECT * FROM " + DataPointRecord::databaseTableName() +
        " WHERE analysisRecordId=:analysisRecordId AND complete=:complete AND failed=:failed" ));
    query.bindValue(":analysisRecordId",id());
    query.bindValue(":complete",true);
    query.bindValue(":failed",false);
    assertExec(query);
    while (query.next()) {
      result.push_back(DataPointRecord::factoryFromQuery(query, database).get());
    }

    return result;
  }

  std::vector<DataPointRecord> AnalysisRecord_Impl::failedDataPointRecords() const {
    DataPointRecordVector result;

    ProjectDatabase database = projectDatabase();
    QSqlQuery query(*(database.qSqlDatabase()));
    query.prepare(toQString("SELECT * FROM " + DataPointRecord::databaseTableName() +
        " WHERE analysisRecordId=:analysisRecordId AND complete=:complete AND failed=:failed" ));
    query.bindValue(":analysisRecordId",id());
    query.bindValue(":complete",true);
    query.bindValue(":failed",true);
    assertExec(query);
    while (query.next()) {
      result.push_back(DataPointRecord::factoryFromQuery(query, database).get());
    }

    return result;
  }

  std::vector<DataPointRecord> AnalysisRecord_Impl::getDataPointRecords(
      const std::vector<QVariant>& variableValues) const
  {
    DataPointRecordVector result;

    // ETH@20130314 - Not making this robust to vector indices because this should never be
    // called during AnalysisRecord construction (and because they are super-tangled here).

    // TODO: Have this work for Problems with ContinuousVariables. Perhaps need eps bound on
    // actual value of continuous variables.

    InputVariableRecordVector ivrs = problemRecord().inputVariableRecords();
    IntVector variableVectorIndices;
    for (const InputVariableRecord& ivr : ivrs) {
      variableVectorIndices.push_back(ivr.variableVectorIndex());
    }
    if (variableValues.size() > variableVectorIndices.size()) {
      return result;
    }

    ProjectDatabase database = projectDatabase();
    IntVector measureRecordIds;
    for (int index = 0, n = variableValues.size(); index < n; ++index)
    {
      QVariant value = variableValues[index];
      if (!value.isNull()) {
        QSqlQuery query(*(database.qSqlDatabase()));
        query.prepare(toQString("SELECT m.id FROM " +
            MeasureRecord::databaseTableName() + " m, " +
            VariableRecord::databaseTableName() + " v WHERE " +
            "m.measureVectorIndex=:measureVectorIndex AND " +
            "m.variableRecordId=v.id AND " +
            "v.problemRecordId=:problemRecordId AND " +
            "v.variableVectorIndex=:variableVectorIndex"));
        query.bindValue(":measureVectorIndex",value.toInt());
        query.bindValue(":problemRecordId",m_problemRecordId);
        query.bindValue(":variableVectorIndex",variableVectorIndices[index]);
        assertExec(query);
        if (query.first()) {
          measureRecordIds.push_back(query.value(0).toInt());
        }
        else {
          LOG(Debug,"Query: " << query.lastQuery().toStdString() << std::endl
              << value.toInt() << std::endl
              << m_problemRecordId << std::endl
              << variableVectorIndices[index]);
          return result;
        }
      }
    }

    return getDataPointRecords(measureRecordIds);
  }

  std::vector<DataPointRecord> AnalysisRecord_Impl::getDataPointRecords(
      const std::vector<int>& discretePerturbationRecordIds) const
  {
    DataPointRecordVector result;

    ProjectDatabase database = projectDatabase();
    QSqlQuery query(*(database.qSqlDatabase()));
    std::stringstream ss;
    ss << "SELECT id FROM " << DataPointRecord::databaseTableName()
       << " WHERE analysisRecordId=:analysisRecordId";
    for (int i = 0, n = discretePerturbationRecordIds.size(); i < n; ++i) {
      ss << " INTERSECT SELECT d.id FROM "
         << DataPointRecord::databaseTableName() << " d, "
         << DataPoint_Measure_JoinRecord::databaseTableName() << " j "
         << "WHERE ";
      ss << "d.id=j.leftId AND j.rightId=:rightId" << i;
    }
    query.prepare(toQString(ss.str()));
    ss.str("");
    query.bindValue(":analysisRecordId",id());
    for (int i = 0, n = discretePerturbationRecordIds.size(); i < n; ++i) {
      ss << ":rightId" << i;
      query.bindValue(toQString(ss.str()),discretePerturbationRecordIds[i]);
    }
    assertExec(query);
    while (query.next()) {
      OptionalDataPointRecord dataPointRecord =
          DataPointRecord::getDataPointRecord(query.value(0).toInt(),database);
      OS_ASSERT(dataPointRecord);
      result.push_back(*dataPointRecord);
    }
    return result;
  }

  std::vector<DataPointRecord> AnalysisRecord_Impl::getDataPointRecords(
      const std::string& tag) const
  {
    DataPointRecordVector result;

    ProjectDatabase database = projectDatabase();
    QSqlQuery query(*(database.qSqlDatabase()));
    query.prepare(toQString("SELECT d.id FROM " + DataPointRecord::databaseTableName() + " d, " +
        TagRecord::databaseTableName() + " t WHERE d.analysisRecordId=:analysisRecordId AND " +
        "d.id=t.dataPointRecordId AND t.name=:name"));
    query.bindValue(":analysisRecordId",id());
    query.bindValue(":name",toQString(tag));
    assertExec(query);
    while (query.next()) {
      OptionalDataPointRecord dataPointRecord =
          DataPointRecord::getDataPointRecord(query.value(0).toInt(),database);
      OS_ASSERT(dataPointRecord);
      result.push_back(*dataPointRecord);
    }

    return result;
  }

  bool AnalysisRecord_Impl::resultsAreInvalid() const {
    return m_resultsAreInvalid;
  }

  bool AnalysisRecord_Impl::dataPointsAreInvalid() const {
    return m_dataPointsAreInvalid;
  }

  analysis::Analysis AnalysisRecord_Impl::analysis() const {

    analysis::OptionalAlgorithm oAlgorithm;
    OptionalAlgorithmRecord oAlgorithmRecord = algorithmRecord();
    if (oAlgorithmRecord) {
      oAlgorithm = oAlgorithmRecord->algorithm();
    }

    OptionalFileReference oWeatherFileReference;
    OptionalFileReferenceRecord oWeatherFileReferenceRecord = weatherFileReferenceRecord();
    if (oWeatherFileReferenceRecord) {
      oWeatherFileReference = oWeatherFileReferenceRecord->fileReference();
    }

    analysis::DataPointVector dataPoints;
    for (const DataPointRecord& dataPointRecord : dataPointRecords()) {
      dataPoints.push_back(dataPointRecord.dataPoint());
    }

    return analysis::Analysis(handle(),
                              uuidLast(),
                              name(),
                              displayName(),
                              description(),
                              problemRecord().problem(),
                              oAlgorithm,
                              seedFileReferenceRecord().fileReference(),
                              oWeatherFileReference,
                              dataPoints,
                              m_resultsAreInvalid,
                              m_dataPointsAreInvalid);
  }

  void AnalysisRecord_Impl::setResultsAreInvalid(bool value) {
    m_resultsAreInvalid = value;
    onChange();
  }

  void AnalysisRecord_Impl::setDataPointsAreInvalid(bool value) {
    m_dataPointsAreInvalid = value;
    onChange();
  }

  void AnalysisRecord_Impl::bindValues(QSqlQuery& query) const {
    ObjectRecord_Impl::bindValues(query);

    query.bindValue(AnalysisRecordColumns::problemRecordId, m_problemRecordId);
    if (m_algorithmRecordId) {
      query.bindValue(AnalysisRecordColumns::algorithmRecordId, *m_algorithmRecordId);
    }
    else {
      query.bindValue(AnalysisRecordColumns::algorithmRecordId, QVariant(QVariant::Int));
    }
    query.bindValue(AnalysisRecordColumns::seedFileReferenceRecordId, m_seedFileReferenceRecordId);
    if (m_weatherFileReferenceRecordId) {
      query.bindValue(AnalysisRecordColumns::weatherFileReferenceRecordId, *m_weatherFileReferenceRecordId);
    }
    else {
      query.bindValue(AnalysisRecordColumns::weatherFileReferenceRecordId, QVariant(QVariant::Int));
    }
    query.bindValue(AnalysisRecordColumns::resultsAreInvalid, m_resultsAreInvalid);
    query.bindValue(AnalysisRecordColumns::dataPointsAreInvalid, m_dataPointsAreInvalid);
  }

  void AnalysisRecord_Impl::setLastValues(const QSqlQuery& query, ProjectDatabase& projectDatabase) {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    ObjectRecord_Impl::setLastValues(query,projectDatabase);

    QVariant value;

    value = query.value(AnalysisRecordColumns::problemRecordId);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_lastProblemRecordId = value.toInt();

    value = query.value(AnalysisRecordColumns::algorithmRecordId);
    if (value.isValid() && !value.isNull()) {
      m_lastAlgorithmRecordId = value.toInt();
    }
    else {
      m_lastAlgorithmRecordId.reset();
    }

    value = query.value(AnalysisRecordColumns::seedFileReferenceRecordId);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_lastSeedFileReferenceRecordId = value.toInt();

    value = query.value(AnalysisRecordColumns::weatherFileReferenceRecordId);
    if (value.isValid() && !value.isNull()) {
      m_lastWeatherFileReferenceRecordId = value.toInt();
    }
    else {
      m_lastWeatherFileReferenceRecordId.reset();
    }

    value = query.value(AnalysisRecordColumns::resultsAreInvalid);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_lastResultsAreInvalid = value.toBool();

    value = query.value(AnalysisRecordColumns::dataPointsAreInvalid);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_lastDataPointsAreInvalid = value.toBool();
  }

  bool AnalysisRecord_Impl::compareValues(const QSqlQuery& query) const {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    bool result = ObjectRecord_Impl::compareValues(query);

    QVariant value;

    value = query.value(AnalysisRecordColumns::problemRecordId);
    OS_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_problemRecordId == value.toInt());

    value = query.value(AnalysisRecordColumns::algorithmRecordId);
    if (value.isValid() && !value.isNull()) {
      result = result && m_algorithmRecordId && (*m_algorithmRecordId == value.toInt());
    }else{
      result = result && !m_algorithmRecordId;
    }

    value = query.value(AnalysisRecordColumns::seedFileReferenceRecordId);
    OS_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_seedFileReferenceRecordId == value.toInt());

    value = query.value(AnalysisRecordColumns::weatherFileReferenceRecordId);
    if (value.isValid() && !value.isNull()) {
      result = result && m_weatherFileReferenceRecordId && (*m_weatherFileReferenceRecordId == value.toInt());
    }else{
      result = result && !m_weatherFileReferenceRecordId;
    }

    value = query.value(AnalysisRecordColumns::resultsAreInvalid);
    OS_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_resultsAreInvalid == value.toBool());

    value = query.value(AnalysisRecordColumns::dataPointsAreInvalid);
    OS_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_dataPointsAreInvalid == value.toBool());

    return result;
  }

  void AnalysisRecord_Impl::saveLastValues() {
    ObjectRecord_Impl::saveLastValues();

    m_lastProblemRecordId = m_problemRecordId;
    m_lastAlgorithmRecordId = m_algorithmRecordId;
    m_lastSeedFileReferenceRecordId = m_seedFileReferenceRecordId;
    m_lastWeatherFileReferenceRecordId = m_weatherFileReferenceRecordId;
    m_lastResultsAreInvalid = m_resultsAreInvalid;
    m_lastDataPointsAreInvalid = m_dataPointsAreInvalid;
  }

  void AnalysisRecord_Impl::revertToLastValues() {
    ObjectRecord_Impl::revertToLastValues();

    m_problemRecordId = m_lastProblemRecordId;
    m_algorithmRecordId = m_lastAlgorithmRecordId;
    m_seedFileReferenceRecordId = m_lastSeedFileReferenceRecordId;
    m_weatherFileReferenceRecordId = m_lastWeatherFileReferenceRecordId;
    m_resultsAreInvalid = m_lastResultsAreInvalid;
    m_dataPointsAreInvalid = m_lastDataPointsAreInvalid;
  }

  void AnalysisRecord_Impl::revertToLastRecordIds() {
    m_problemRecordId = m_lastProblemRecordId;
    m_algorithmRecordId = m_lastAlgorithmRecordId;
    m_seedFileReferenceRecordId = m_lastSeedFileReferenceRecordId;
    m_weatherFileReferenceRecordId = m_lastWeatherFileReferenceRecordId;
  }

  void AnalysisRecord_Impl::setProblemRecordId(int id) {
    m_problemRecordId = id;
    this->onChange(false);
  }

  void AnalysisRecord_Impl::setAlgorithmRecordId(int id) {
    m_algorithmRecordId = id;
    this->onChange(false);
  }

  void AnalysisRecord_Impl::setSeedFileReferenceRecordId(int id) {
    m_seedFileReferenceRecordId = id;
    this->onChange(false);
  }

  void AnalysisRecord_Impl::setWeatherFileReferenceRecordId(int id) {
    m_weatherFileReferenceRecordId = id;
    this->onChange(false);
  }

} // detail

AnalysisRecord::AnalysisRecord(const analysis::Analysis& analysis, ProjectDatabase& database)
  : ObjectRecord(std::shared_ptr<detail::AnalysisRecord_Impl>(
        new detail::AnalysisRecord_Impl(analysis, database)),
        database)
{
  OS_ASSERT(getImpl<detail::AnalysisRecord_Impl>());

  AnalysisRecord copyOfThis(getImpl<detail::AnalysisRecord_Impl>());
  bool isNew = database.isNewRecord(copyOfThis);
  if (!isNew) {
    getImpl<detail::AnalysisRecord_Impl>()->revertToLastRecordIds();
  }

  // Save child Problem
  Problem problem = analysis.problem();
  // See if old problem record should be removed -- do this first so can use standard
  // interface, and in case children of oldProblemRecord need to be reinstated.
  // Check even if !problem.isDirty() since new problem could have been saved to the database
  // prior to being set as analysis's problem.
  if (!isNew) {
    {
      ProblemRecord oldProblemRecord = problemRecord();
      // don't remove if have invalid data points still--they need to point to the old one
      if ((oldProblemRecord.handle() != problem.uuid()) && (!analysis.dataPointsAreInvalid()))
      {
        database.removeRecord(oldProblemRecord);
      }
    }
    database.unloadUnusedCleanRecords();
  }
  // Save problem if it is dirty, or if this analysisRecord is new. The latter condition is to
  // avoid making a bad object in the case of user error (calling clearDirtyFlag before saving to
  // the database).
  if (problem.isDirty() || isNew) {
    ProblemRecord problemRecord = ProblemRecord::factoryFromProblem(analysis.problem(),database);
    getImpl<detail::AnalysisRecord_Impl>()->setProblemRecordId(problemRecord.id());
  }

  // Save child Algorithm
  OptionalAlgorithm algorithm = analysis.algorithm();
  bool algorithmWasNull = false;
  if (!isNew) {
    {
      // see if old algorithm record should be removed
      OptionalAlgorithmRecord oldAlgorithmRecord = algorithmRecord();
      if (oldAlgorithmRecord && (!algorithm ||
                                 (oldAlgorithmRecord->handle() != algorithm->uuid())))
      {
        database.removeRecord(*oldAlgorithmRecord);
      }
      else if (!oldAlgorithmRecord) {
        algorithmWasNull = true;
      }
    }
    database.unloadUnusedCleanRecords();
  }
  if (algorithm && (algorithmWasNull ||
                    algorithm.get().isDirty() ||
                    isNew))
  {
    AlgorithmRecord algorithmRecord = AlgorithmRecord::factoryFromAlgorithm(*algorithm,copyOfThis);
    getImpl<detail::AnalysisRecord_Impl>()->setAlgorithmRecordId(algorithmRecord.id());
  }

  // Save child seed FileReference
  FileReference seedReference = analysis.seed();
  bool saveSeedReference = false;
  if (!isNew) {
    {
      // see if old seed file reference record should be removed
      FileReferenceRecord oldSeedFileReferenceRecord = seedFileReferenceRecord();
      if (oldSeedFileReferenceRecord.handle() != seedReference.uuid()) {
        database.removeRecord(oldSeedFileReferenceRecord);
        saveSeedReference = true;
      }
      else if (oldSeedFileReferenceRecord.uuidLast() != seedReference.versionUUID()) {
        saveSeedReference = true;
      }
    }
    database.unloadUnusedCleanRecords();
  }
  if (saveSeedReference || isNew) {
    FileReferenceRecord seedFileReferenceRecord(seedReference,copyOfThis);
    getImpl<detail::AnalysisRecord_Impl>()->setSeedFileReferenceRecordId(seedFileReferenceRecord.id());
  }

  // Save child weather FileReference
  OptionalFileReference weatherFileReference = analysis.weatherFile();
  bool saveWeatherFileReference = false;
  if (!isNew) {
    {
      // see if old weather file reference should be removed
      OptionalFileReferenceRecord oldWeatherFileReferenceRecord = weatherFileReferenceRecord();
      if (oldWeatherFileReferenceRecord &&
          (!weatherFileReference ||
           (oldWeatherFileReferenceRecord->handle() != weatherFileReference->uuid())))
      {
        database.removeRecord(*oldWeatherFileReferenceRecord);
        saveWeatherFileReference = true;
      }
      else if (weatherFileReference &&
               (!oldWeatherFileReferenceRecord ||
                (oldWeatherFileReferenceRecord->uuidLast() != weatherFileReference->versionUUID())))
      {
        saveWeatherFileReference = true;
      }
    }
    database.unloadUnusedCleanRecords();
  }
  if (weatherFileReference && (saveWeatherFileReference || isNew))
  {
    FileReferenceRecord weatherFileReferenceRecord(*weatherFileReference,copyOfThis);
    getImpl<detail::AnalysisRecord_Impl>()->setWeatherFileReferenceRecordId(weatherFileReferenceRecord.id());
  }

  // Save child DataPoints
  DataPointVector dataPoints = analysis.dataPoints();
  std::vector<UUID> dataPointUUIDs;
  OptionalProblemRecord problemRecord;
  for (const DataPoint& dataPoint : dataPoints) {
    dataPointUUIDs.push_back(dataPoint.uuid());
    if (dataPoint.isDirty() || isNew) {
      if (!problemRecord) {
        problemRecord = this->problemRecord();
      }
      if (dataPoint.problemUUID() == problemRecord->handle()) {
        DataPointRecord dataPointRecord =
            DataPointRecord::factoryFromDataPoint(dataPoint,copyOfThis,*problemRecord);
      }
      else {
        OptionalProblemRecord opr = database.getObjectRecordByHandle<ProblemRecord>(dataPoint.problemUUID());
        if (opr) {
          DataPointRecord dataPointRecord =
              DataPointRecord::factoryFromDataPoint(dataPoint,copyOfThis,*opr);
        }
        else {
          LOG(Debug,"Not saving DataPoint '" << dataPoint.name() << "' to OSP, because it was made with a "
              << "Problem that is no longer in the database.");
        }
      }
    }
  }
  if (!isNew) {
    removeDataPointRecords(dataPointUUIDs,database);
  }
}

AnalysisRecord::AnalysisRecord(const QSqlQuery& query, ProjectDatabase& database)
  : ObjectRecord(std::shared_ptr<detail::AnalysisRecord_Impl>(
        new detail::AnalysisRecord_Impl(query, database)),
        database)
{
  OS_ASSERT(getImpl<detail::AnalysisRecord_Impl>());
}

AnalysisRecord::AnalysisRecord(std::shared_ptr<detail::AnalysisRecord_Impl> impl,
                               ProjectDatabase database)
  : ObjectRecord(impl, database)
{
  OS_ASSERT(getImpl<detail::AnalysisRecord_Impl>());
}

std::string AnalysisRecord::databaseTableName() {
  return "AnalysisRecords";
}

UpdateByIdQueryData AnalysisRecord::updateByIdQueryData() {
  static UpdateByIdQueryData result;
  if (result.queryString.empty()) {
    // numeric column identifiers
    result.columnValues = ColumnsType::getValues();

    // query string
    std::stringstream ss;
    ss << "UPDATE " << databaseTableName() << " SET ";
    int expectedValue = 0;
    for (auto it = result.columnValues.begin(),
         itend = result.columnValues.end(); it != itend; ++it)
    {
      // require 0 based columns, don't skip any
      OS_ASSERT(*it == expectedValue);
      // column name is name, type is description
      ss << ColumnsType::valueName(*it) << "=:" << ColumnsType::valueName(*it);
      // is this the last column?
      auto nextIt = it;
      ++nextIt;
      if (nextIt == itend) {
        ss << " ";
      }
      else {
        ss << ", ";
      }
      ++expectedValue;
    }
    ss << "WHERE id=:id";
    result.queryString = ss.str();

    // null values
    for (auto it = result.columnValues.begin(),
         itend = result.columnValues.end(); it != itend; ++it)
    {
      // bind all values to avoid parameter mismatch error
      if (istringEqual(ColumnsType::valueDescription(*it), "INTEGER")) {
        result.nulls.push_back(QVariant(QVariant::Int));
      }
      else {
        result.nulls.push_back(QVariant(QVariant::String));
      }
    }
  }
  return result;
}

void AnalysisRecord::updatePathData(ProjectDatabase database,
                                    const openstudio::path& originalBase,
                                    const openstudio::path& newBase)
{
  // At this time, no path data is stored in AnalysisRecord.
}

boost::optional<AnalysisRecord> AnalysisRecord::factoryFromQuery(
    const QSqlQuery& query, ProjectDatabase& database)
{
  OptionalAnalysisRecord result;
  try {
    result = AnalysisRecord(query,database);
  }
  catch (const std::exception& e) {
    LOG(Error,"Unable to construct AnalysisRecord from query, because '"
        << e.what() << "'.");
  }
  return result;
}

std::vector<AnalysisRecord> AnalysisRecord::getAnalysisRecords(ProjectDatabase& database) {
  std::vector<AnalysisRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + AnalysisRecord::databaseTableName()));
  assertExec(query);
  while (query.next()) {
    result.push_back(AnalysisRecord(query, database));
  }

  return result;
}

boost::optional<AnalysisRecord> AnalysisRecord::getAnalysisRecord(int id, ProjectDatabase& database) {
  boost::optional<AnalysisRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + AnalysisRecord::databaseTableName() + " WHERE id=:id"));
  query.bindValue(":id",id);
  assertExec(query);
  if (query.first()) {
    result = AnalysisRecord(query, database);
  }

  return result;
}

ProblemRecord AnalysisRecord::problemRecord() const {
  return getImpl<detail::AnalysisRecord_Impl>()->problemRecord();
}

boost::optional<AlgorithmRecord> AnalysisRecord::algorithmRecord() const {
  return getImpl<detail::AnalysisRecord_Impl>()->algorithmRecord();
}

FileReferenceRecord AnalysisRecord::seedFileReferenceRecord() const {
  return getImpl<detail::AnalysisRecord_Impl>()->seedFileReferenceRecord();
}

boost::optional<FileReferenceRecord> AnalysisRecord::weatherFileReferenceRecord() const {
  return getImpl<detail::AnalysisRecord_Impl>()->weatherFileReferenceRecord();
}

std::vector<DataPointRecord> AnalysisRecord::dataPointRecords() const {
  return getImpl<detail::AnalysisRecord_Impl>()->dataPointRecords();
}

std::vector<DataPointRecord> AnalysisRecord::incompleteDataPointRecords() const {
  return getImpl<detail::AnalysisRecord_Impl>()->incompleteDataPointRecords();
}

std::vector<DataPointRecord> AnalysisRecord::completeDataPointRecords() const {
  return getImpl<detail::AnalysisRecord_Impl>()->completeDataPointRecords();
}

std::vector<DataPointRecord> AnalysisRecord::successfulDataPointRecords() const {
  return getImpl<detail::AnalysisRecord_Impl>()->successfulDataPointRecords();
}

std::vector<DataPointRecord> AnalysisRecord::failedDataPointRecords() const {
  return getImpl<detail::AnalysisRecord_Impl>()->failedDataPointRecords();
}

std::vector<DataPointRecord> AnalysisRecord::getDataPointRecords(
    const std::vector<QVariant>& variableValues) const
{
  return getImpl<detail::AnalysisRecord_Impl>()->getDataPointRecords(variableValues);
}

std::vector<DataPointRecord> AnalysisRecord::getDataPointRecords(
    const std::vector<int>& discretePerturbationRecordIds) const
{
  return getImpl<detail::AnalysisRecord_Impl>()->getDataPointRecords(discretePerturbationRecordIds);
}

std::vector<DataPointRecord> AnalysisRecord::getDataPointRecords(const std::string& tag) const {
  return getImpl<detail::AnalysisRecord_Impl>()->getDataPointRecords(tag);
}

bool AnalysisRecord::resultsAreInvalid() const {
  return getImpl<detail::AnalysisRecord_Impl>()->resultsAreInvalid();
}

bool AnalysisRecord::dataPointsAreInvalid() const {
  return getImpl<detail::AnalysisRecord_Impl>()->dataPointsAreInvalid();
}

analysis::Analysis AnalysisRecord::analysis() const {
  return getImpl<detail::AnalysisRecord_Impl>()->analysis();
}

void AnalysisRecord::setResultsAreInvalid(bool value) {
  getImpl<detail::AnalysisRecord_Impl>()->setResultsAreInvalid(value);
}

void AnalysisRecord::setDataPointsAreInvalid(bool value) {
  getImpl<detail::AnalysisRecord_Impl>()->setDataPointsAreInvalid(value);
}

/// @cond
AnalysisRecord::AnalysisRecord(std::shared_ptr<detail::AnalysisRecord_Impl> impl)
  : ObjectRecord(impl)
{}
/// @endcond

void AnalysisRecord::removeDataPointRecords(const std::vector<UUID>& uuidsToKeep,
                                            ProjectDatabase& database)
{
  QSqlQuery query(*(database.qSqlDatabase()));
  std::stringstream ss;
  ss << "SELECT * FROM " + DataPointRecord::databaseTableName() +
        " WHERE (analysisRecordId=:analysisRecordId) AND (handle NOT IN (";
  std::string sep("");
  for (const UUID& handle : uuidsToKeep) {
    ss << sep << "'" << toString(handle) << "'";
    sep = std::string(", ");
  }
  ss << "))";
  query.prepare(toQString(ss.str()));
  query.bindValue(":analysisRecordId",id());
  assertExec(query);
  while (query.next()) {
    OptionalDataPointRecord dataPointRecord = DataPointRecord::factoryFromQuery(query, database);
    if (dataPointRecord) {
      database.removeRecord(*dataPointRecord);
    }
  }
}

} // project
} // openstudio

