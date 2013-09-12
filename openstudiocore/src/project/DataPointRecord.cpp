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

#include <project/DataPointRecord.hpp>
#include <project/DataPointRecord_Impl.hpp>
#include <project/DataPoint_Measure_JoinRecord.hpp>
#include <project/DataPoint_Measure_JoinRecord_Impl.hpp>
#include <project/DataPointValueRecord.hpp>

#include <project/AnalysisRecord.hpp>
#include <project/AttributeRecord.hpp>
#include <project/ProblemRecord.hpp>
#include <project/ContinuousVariableRecord.hpp>
#include <project/ContinuousVariableRecord_Impl.hpp>
#include <project/FunctionRecord.hpp>
#include <project/FileReferenceRecord.hpp>
#include <project/MeasureGroupRecord.hpp>
#include <project/MeasureGroupRecord_Impl.hpp>
#include <project/MeasureRecord.hpp>
#include <project/OptimizationProblemRecord.hpp>
#include <project/OptimizationProblemRecord_Impl.hpp>
#include <project/OptimizationDataPointRecord.hpp>

#include <project/TagRecord.hpp>

#include <analysis/DataPoint.hpp>
#include <analysis/OptimizationDataPoint.hpp>
#include <analysis/OptimizationDataPoint_Impl.hpp>

#include <utilities/data/Attribute.hpp>

#include <utilities/data/Tag.hpp>

#include <utilities/core/Assert.hpp>
#include <utilities/core/FileReference.hpp>
#include <utilities/core/Containers.hpp>
#include <utilities/core/Compare.hpp>
#include <utilities/core/PathHelpers.hpp>

#include <boost/bind.hpp>

using namespace openstudio::analysis;

// DLM: I believe this will work cross-platform, I don't think ';' is allowed in a path on any system?
const char pathSep = ';';

namespace openstudio {
namespace project {

namespace detail {

  DataPointRecord_Impl::DataPointRecord_Impl(const analysis::DataPoint& dataPoint,
                                             const DataPointRecordType& dataPointRecordType,
                                             AnalysisRecord& analysisRecord,
                                             const ProblemRecord& problemRecord)
    : ObjectRecord_Impl(analysisRecord.projectDatabase(),
                        dataPoint.uuid(),
                        dataPoint.name(),
                        dataPoint.displayName(),
                        dataPoint.description(),
                        dataPoint.versionUUID()),
      m_analysisRecordId(analysisRecord.id()),
      m_problemRecordId(problemRecord.id()),
      m_dataPointRecordType(dataPointRecordType),
      m_complete(dataPoint.isComplete()),
      m_failed(dataPoint.failed()),
      m_selected(dataPoint.selected()),
      m_runType(dataPoint.runType()),
      m_directory(dataPoint.directory()),
      m_dakotaParametersFiles(dataPoint.dakotaParametersFiles())
  {
    boost::optional<runmanager::Job> topLevelJob = dataPoint.topLevelJob();
    if (topLevelJob){
      m_topLevelJobUUID = topLevelJob->uuid();
    }
  }

  DataPointRecord_Impl::DataPointRecord_Impl(const QSqlQuery& query, ProjectDatabase& database)
    : ObjectRecord_Impl(database, query)
  {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    QVariant value;

    value = query.value(DataPointRecordColumns::analysisRecordId);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_analysisRecordId = value.toInt();

    value = query.value(DataPointRecordColumns::problemRecordId);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_problemRecordId = value.toInt();

    value = query.value(DataPointRecordColumns::dataPointRecordType);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_dataPointRecordType = DataPointRecordType(value.toInt());

    value = query.value(DataPointRecordColumns::complete);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_complete = value.toBool();

    value = query.value(DataPointRecordColumns::failed);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_failed = value.toBool();

    value = query.value(DataPointRecordColumns::selected);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_selected = value.toBool();

    value = query.value(DataPointRecordColumns::runType);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_runType = DataPointRunType(value.toInt());

    value = query.value(DataPointRecordColumns::directory);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_directory = toPath(value.toString());

    value = query.value(DataPointRecordColumns::inputDataRecordId);
    if (value.isValid() && !value.isNull()) {
       m_osmInputDataRecordId = value.toInt();
    }

    value = query.value(DataPointRecordColumns::idfInputDataRecordId);
    if (value.isValid() && !value.isNull()) {
       m_idfInputDataRecordId = value.toInt();
    }

    value = query.value(DataPointRecordColumns::sqlOutputDataRecordId);
    if (value.isValid() && !value.isNull()) {
       m_sqlOutputDataRecordId = value.toInt();
    }

    value = query.value(DataPointRecordColumns::topLevelJobUUID);
    if (value.isValid() && !value.isNull() && !value.toString().isEmpty()) {
      m_topLevelJobUUID = openstudio::UUID(value.toString());
    }

    value = query.value(DataPointRecordColumns::dakotaParametersFiles);
    if (value.isValid() && !value.isNull()) {
      Q_FOREACH(const QString& path, value.toString().split(QString(pathSep), QString::SkipEmptyParts)){
        m_dakotaParametersFiles.push_back(toPath(path));
      }
    }

  }

  std::string DataPointRecord_Impl::databaseTableName() const {
    return DataPointRecord::databaseTableName();
  }

  boost::optional<ObjectRecord> DataPointRecord_Impl::parent() const {
    return analysisRecord().cast<ObjectRecord>();
  }

  std::vector<ObjectRecord> DataPointRecord_Impl::children() const {
    ObjectRecordVector result;
    DataPointValueRecordVector cvvrs = continuousVariableValueRecords();
    result.insert(result.end(),cvvrs.begin(),cvvrs.end());
    TagRecordVector trs = tagRecords();
    result.insert(result.end(),trs.begin(),trs.end());
    OptionalFileReferenceRecord ofrr = osmInputDataRecord();
    if (ofrr) {
      result.push_back(*ofrr);
    }
    ofrr = idfInputDataRecord();
    if (ofrr) {
      result.push_back(*ofrr);
    }
    ofrr = sqlOutputDataRecord();
    if (ofrr) {
      result.push_back(*ofrr);
    }
    FileReferenceRecordVector frrs = xmlOutputDataRecords();
    result.insert(result.end(),frrs.begin(),frrs.end());
    DataPointValueRecordVector rvrs = responseValueRecords();
    result.insert(result.end(),rvrs.begin(),rvrs.end());
    return result;
  }

  std::vector<ObjectRecord> DataPointRecord_Impl::resources() const {
    ObjectRecordVector result;
    result.push_back(this->problemRecord());
    return result;
  }

  std::vector<JoinRecord> DataPointRecord_Impl::joinRecords() const {
    JoinRecordVector result;

    DataPoint_Measure_JoinRecordVector discretePerturbationJoins =
      JoinRecord::getJoinRecordsForLeftId<DataPoint_Measure_JoinRecord>(id(),projectDatabase());
    result.insert(result.end(),discretePerturbationJoins.begin(),discretePerturbationJoins.end());

    return result;
  }

  void DataPointRecord_Impl::saveRow(const boost::shared_ptr<QSqlDatabase> &database)
  {
    QSqlQuery query(*database);
    this->makeUpdateByIdQuery<DataPointRecord>(query);
    this->bindValues(query);
    assertExec(query);
  }

  void DataPointRecord_Impl::updatePathData(const openstudio::path& originalBase,
                                            const openstudio::path& newBase)
  {
    openstudio::path temp = relocatePath(directory(),originalBase,newBase);
    if (!temp.empty()) {
      setDirectory(temp);
    }
  }

  AnalysisRecord DataPointRecord_Impl::analysisRecord() const {
    ProjectDatabase database = projectDatabase();
    return AnalysisRecord::getAnalysisRecord(m_analysisRecordId,database).get();
  }

  ProblemRecord DataPointRecord_Impl::problemRecord() const {
    ProjectDatabase database = projectDatabase();
    return ProblemRecord::getProblemRecord(m_problemRecordId, database).get();
  }

  bool DataPointRecord_Impl::isComplete() const {
    return complete();
  }

  bool DataPointRecord_Impl::complete() const {
    return m_complete;
  }

  bool DataPointRecord_Impl::failed() const {
    return m_failed;
  }

  bool DataPointRecord_Impl::selected() const {
    return m_selected;
  }

  analysis::DataPointRunType DataPointRecord_Impl::runType() const {
    return m_runType;
  }

  openstudio::path DataPointRecord_Impl::directory() const {
    return m_directory;
  }

  std::vector<DataPointValueRecord> DataPointRecord_Impl::continuousVariableValueRecords() const {
    DataPointValueRecordVector result;

    // get variable records in order
    InputVariableRecordVector ivrs = problemRecord().inputVariableRecords();
    // cast to continuous variable records--order will be preserved
    ContinuousVariableRecordVector cvrs = subsetCastVector<ContinuousVariableRecord>(ivrs);

    ProjectDatabase database = projectDatabase();
    QSqlQuery query(*(database.qSqlDatabase()));
    OptionalInt lastVectorIndex;
    BOOST_FOREACH(const ContinuousVariableRecord& cvr,cvrs) {
      // double-check ordering
      int vectorIndex = cvr.variableVectorIndex();
      if (lastVectorIndex) {
        OS_ASSERT(lastVectorIndex.get() < vectorIndex);
      }
      lastVectorIndex = vectorIndex;
      query.prepare(toQString("SELECT * FROM " + DataPointValueRecord::databaseTableName() +
                              " WHERE dataPointRecordId=:dataPointRecordId AND " +
                              "continuousVariableRecordId=:continuousVariableRecordId"));
      query.bindValue(":dataPointRecordId",id());
      query.bindValue(":continuousVariableRecordId",cvr.id());
      assertExec(query);
      if (query.first()) {
        DataPointValueRecord continuousVariableValueRecord(query, database);
        result.push_back(continuousVariableValueRecord);
      }
      query.clear();
    }

    return result;
  }

  std::vector<DataPointValueRecord> DataPointRecord_Impl::responseValueRecords() const {
    DataPointValueRecordVector result;
    ProjectDatabase database = projectDatabase();
    FunctionRecordVector responses = problemRecord().responseRecords();
    BOOST_FOREACH(const FunctionRecord& response,responses) {
      QSqlQuery query(*(database.qSqlDatabase()));
      query.prepare(toQString("SELECT * FROM " + DataPointValueRecord::databaseTableName() +
          " WHERE dataPointRecordId=:dataPointRecordId AND functionRecordId=:functionRecordId "));
      query.bindValue(":dataPointRecordId",id());
      query.bindValue(":functionRecordId",response.id());
      assertExec(query);
      if (query.first()) {
        result.push_back(DataPointValueRecord(query,database));
      }
    }
    return result;
  }

  std::vector<double> DataPointRecord_Impl::responseValues() const {
    DoubleVector result;
    DataPointValueRecordVector valueRecords = responseValueRecords();
    BOOST_FOREACH(const DataPointValueRecord& valueRecord,valueRecords) {
      result.push_back(valueRecord.dataPointValue());
    }
    return result;
  }

  boost::optional<FileReferenceRecord> DataPointRecord_Impl::osmInputDataRecord() const {
    OptionalFileReferenceRecord result;
    if (m_osmInputDataRecordId) {
      ProjectDatabase database = projectDatabase();
      result = FileReferenceRecord::getFileReferenceRecord(*m_osmInputDataRecordId,database);
    }
    return result;
  }

  boost::optional<FileReferenceRecord> DataPointRecord_Impl::idfInputDataRecord() const {
    OptionalFileReferenceRecord result;
    if (m_idfInputDataRecordId) {
      ProjectDatabase database = projectDatabase();
      result = FileReferenceRecord::getFileReferenceRecord(*m_idfInputDataRecordId,database);
    }
    return result;
  }

  boost::optional<FileReferenceRecord> DataPointRecord_Impl::sqlOutputDataRecord() const {
    OptionalFileReferenceRecord result;
    if (m_sqlOutputDataRecordId) {
      ProjectDatabase database = projectDatabase();
      result = FileReferenceRecord::getFileReferenceRecord(*m_sqlOutputDataRecordId,database);
    }
    return result;
  }

  std::vector<FileReferenceRecord> DataPointRecord_Impl::xmlOutputDataRecords() const {
    FileReferenceRecordVector result;

    ProjectDatabase database = projectDatabase();
    QSqlQuery query(*(database.qSqlDatabase()));
    query.prepare(toQString("SELECT * FROM " + FileReferenceRecord::databaseTableName() +
                            " WHERE parentDatabaseTableName=:parentDatabaseTableName AND " +
                            "parentRecordId=:parentRecordId AND " +
                            "fileReferenceType=:fileReferenceType"));
    query.bindValue(":parentDatabaseTableName",toQString(databaseTableName()));
    query.bindValue(":parentRecordId",id());
    query.bindValue(":fileReferenceType",int(FileReferenceType::XML));
    assertExec(query);
    while (query.next()) {
      result.push_back(FileReferenceRecord(query, database));
    }
    return result;
  }

  boost::optional<openstudio::UUID> DataPointRecord_Impl::topLevelJobUUID() const {
    return m_topLevelJobUUID;
  }

  std::vector<TagRecord> DataPointRecord_Impl::tagRecords() const {
    std::vector<TagRecord> result;

    ProjectDatabase database = projectDatabase();
    QSqlQuery query(*(database.qSqlDatabase()));
    query.prepare(toQString("SELECT * FROM " + TagRecord::databaseTableName() +
        " WHERE dataPointRecordId=:dataPointRecordId"));
    query.bindValue(":dataPointRecordId",id());
    assertExec(query);
    while (query.next()) {
      result.push_back(TagRecord(query, database));
    }

    return result;
  }

  std::vector<AttributeRecord> DataPointRecord_Impl::attributeRecords() const {
    AttributeRecordVector result;
    Q_FOREACH(const FileReferenceRecord& fr,xmlOutputDataRecords()) {
      AttributeRecordVector additions = fr.attributeRecords();
      result.insert(result.end(),additions.begin(),additions.end());
    }
    return result;
  }

  analysis::DataPoint DataPointRecord_Impl::dataPoint() const {
    ProblemRecord problemRecord = this->problemRecord();

    // get variable values
    std::vector<QVariant> variableValues;
    ProjectDatabase database = projectDatabase();
    // returned in order
    InputVariableRecordVector inputVariableRecords = problemRecord.inputVariableRecords();
    BOOST_FOREACH(const InputVariableRecord& inputVariableRecord,inputVariableRecords) {
      if (inputVariableRecord.optionalCast<DiscreteVariableRecord>()) {
        QSqlQuery query(*(database.qSqlDatabase()));
        query.prepare(toQString("SELECT * FROM " + MeasureRecord::databaseTableName() + " o , " +
            DataPoint_Measure_JoinRecord::databaseTableName() + " j " +
            " WHERE o.variableRecordId=:variableRecordId AND j.leftId=:leftId AND o.id=j.rightId "));
        query.bindValue(":variableRecordId",inputVariableRecord.id());
        query.bindValue(":leftId",id());
        assertExec(query);
        if (query.first()) {
          // by asking for whole record, measureVectorIndex() will be correct even if
          // record is dirty
          OptionalMeasureRecord dpr = MeasureRecord::factoryFromQuery(query,database);
          if (dpr && dpr->measureVectorIndex()) {
            variableValues.push_back(dpr->measureVectorIndex().get());
          }
        }
      }
      else {
        OS_ASSERT(inputVariableRecord.optionalCast<ContinuousVariableRecord>());
        QSqlQuery query(*(database.qSqlDatabase()));
        query.prepare(toQString("SELECT dataPointValue FROM " + DataPointValueRecord::databaseTableName() +
            " WHERE dataPointRecordId=:dataPointRecordId AND continuousVariableRecordId=:continuousVariableRecordId"));
        query.bindValue(":dataPointRecordId",id());
        query.bindValue(":continuousVariableRecordId",inputVariableRecord.id());
        assertExec(query);
        if (query.first()) {
          QVariant value = query.value(0);
          OS_ASSERT(value.isValid() && !value.isNull());
          variableValues.push_back(value.toDouble());
        }
      }
    }
    OS_ASSERT(variableValues.size() == inputVariableRecords.size());

    OptionalFileReferenceRecord ofrr;
    OptionalFileReference oOsmInputData;
    OptionalFileReference oIdfInputData;
    OptionalFileReference oSqlOutputData;
    FileReferenceVector xmlOutputData;
    AttributeVector attributes;
    ofrr = osmInputDataRecord();
    if (ofrr) {
      oOsmInputData = ofrr->fileReference();
    }
    ofrr = idfInputDataRecord();
    if (ofrr) {
      oIdfInputData = ofrr->fileReference();
    }
    ofrr = sqlOutputDataRecord();
    if (ofrr) {
      oSqlOutputData = ofrr->fileReference();
    }
    FileReferenceRecordVector frrs = xmlOutputDataRecords();
    Q_FOREACH(const FileReferenceRecord& frr,frrs) {
      xmlOutputData.push_back(frr.fileReference());
    }
    AttributeRecordVector ars = attributeRecords();
    Q_FOREACH(const AttributeRecord& ar,ars) {
      attributes.push_back(ar.attribute());
    }

    TagRecordVector tagRecords = this->tagRecords();
    TagVector tags;
    BOOST_FOREACH(const TagRecord& tagRecord,tagRecords) {
      tags.push_back(tagRecord.tag());
    }

    boost::optional<runmanager::Job> topLevelJob;
    if (m_topLevelJobUUID) {
      try {
        topLevelJob = this->projectDatabase().runManager().getJob(*m_topLevelJobUUID);
      }
      catch (const std::exception& e) {
        LOG(Error, "Job " << toString(*m_topLevelJobUUID) << " not found in RunManager. "
            << e.what());
      }
    }

    return analysis::DataPoint(handle(),
                               uuidLast(),
                               name(),
                               displayName(),
                               description(),
                               problemRecord.problem(),
                               m_complete,
                               m_failed,
                               m_selected,
                               m_runType,
                               variableValues,
                               responseValues(),
                               m_directory,
                               oOsmInputData,
                               oIdfInputData,
                               oSqlOutputData,
                               xmlOutputData,
                               topLevelJob,
                               m_dakotaParametersFiles,
                               tags,
                               attributes);
  }

  void DataPointRecord_Impl::setDirectory(const openstudio::path& directory) {
    m_directory = openstudio::completeAndNormalize(directory);
    onChange();
  }

  void DataPointRecord_Impl::clearResults() {
    m_complete = false;
    m_failed = false;
    m_directory = openstudio::path();
    ProjectDatabase database = projectDatabase();
    OptionalFileReferenceRecord ofrr = osmInputDataRecord();
    if (ofrr) {
      database.removeRecord(*ofrr);
    }
    ofrr = idfInputDataRecord();
    if (ofrr) {
      database.removeRecord(*ofrr);
    }
    ofrr = sqlOutputDataRecord();
    if (ofrr) {
      database.removeRecord(*ofrr);
    }
    FileReferenceRecordVector frrs = xmlOutputDataRecords();
    BOOST_FOREACH(FileReferenceRecord& frr,frrs) {
      database.removeRecord(frr);
    }
    DataPointValueRecordVector rvrs = responseValueRecords();
    BOOST_FOREACH(DataPointValueRecord& rvr,rvrs) {
      database.removeRecord(rvr);
    }
    m_topLevelJobUUID.reset();
    m_dakotaParametersFiles.clear();
    onChange();
  }

  void DataPointRecord_Impl::revertToLastRecordIds() {
    // just ones that are explicitly set in the public-object constructor
    m_osmInputDataRecordId = m_lastOsmInputDataRecordId;
    m_idfInputDataRecordId = m_lastIdfInputDataRecordId;
    m_sqlOutputDataRecordId = m_lastSqlOutputDataRecordId;
  }

  void DataPointRecord_Impl::setOsmInputDataRecordId(int id) {
    m_osmInputDataRecordId = id;
    this->onChange(false);
  }

  void DataPointRecord_Impl::clearOsmInputDataRecordId() {
    if (m_osmInputDataRecordId) {
      m_osmInputDataRecordId.reset();
      this->onChange(false);
    }
  }

  void DataPointRecord_Impl::setIdfInputDataRecordId(int id) {
    m_idfInputDataRecordId = id;
    this->onChange(false);
  }

  void DataPointRecord_Impl::clearIdfInputDataRecordId() {
    if (m_idfInputDataRecordId) {
      m_idfInputDataRecordId.reset();
      this->onChange(false);
    }
  }

  void DataPointRecord_Impl::setSqlOutputDataRecordId(int id) {
    m_sqlOutputDataRecordId = id;
    this->onChange(false);
  }

  void DataPointRecord_Impl::clearSqlOutputDataRecordId() {
    if (m_sqlOutputDataRecordId) {
      m_sqlOutputDataRecordId.reset();
      this->onChange(false);
    }
  }

  void DataPointRecord_Impl::bindValues(QSqlQuery& query) const {
    ObjectRecord_Impl::bindValues(query);

    query.bindValue(DataPointRecordColumns::analysisRecordId, m_analysisRecordId);
    query.bindValue(DataPointRecordColumns::problemRecordId, m_problemRecordId);
    query.bindValue(DataPointRecordColumns::dataPointRecordType, m_dataPointRecordType.value());
    query.bindValue(DataPointRecordColumns::complete, m_complete);
    query.bindValue(DataPointRecordColumns::failed, m_failed);
    query.bindValue(DataPointRecordColumns::selected, m_selected);
    query.bindValue(DataPointRecordColumns::runType, m_runType.value());
    query.bindValue(DataPointRecordColumns::directory, toQString(m_directory));
    if (m_osmInputDataRecordId) {
      query.bindValue(DataPointRecordColumns::inputDataRecordId, *m_osmInputDataRecordId);
    }
    else {
      query.bindValue(DataPointRecordColumns::inputDataRecordId, QVariant(QVariant::Int));
    }
    if (m_idfInputDataRecordId) {
      query.bindValue(DataPointRecordColumns::idfInputDataRecordId, *m_idfInputDataRecordId);
    }
    else {
      query.bindValue(DataPointRecordColumns::idfInputDataRecordId, QVariant(QVariant::Int));
    }
    if (m_sqlOutputDataRecordId) {
      query.bindValue(DataPointRecordColumns::sqlOutputDataRecordId, *m_sqlOutputDataRecordId);
    }
    else {
      query.bindValue(DataPointRecordColumns::sqlOutputDataRecordId, QVariant(QVariant::Int));
    }
    if (m_topLevelJobUUID) {
      query.bindValue(DataPointRecordColumns::topLevelJobUUID, m_topLevelJobUUID->toString());
    }
    else {
      query.bindValue(DataPointRecordColumns::topLevelJobUUID, QVariant(QVariant::String));
    }
    if (!m_dakotaParametersFiles.empty()){
      QStringList dakotaParametersFiles;
      BOOST_FOREACH(const openstudio::path& path, m_dakotaParametersFiles) {
        dakotaParametersFiles << toQString(path);
      }
      query.bindValue(DataPointRecordColumns::dakotaParametersFiles, dakotaParametersFiles.join(QString(pathSep)));
    }else{
      query.bindValue(DataPointRecordColumns::dakotaParametersFiles, QVariant(QVariant::String));
    }
  }

  void DataPointRecord_Impl::setLastValues(const QSqlQuery& query, ProjectDatabase& projectDatabase) {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    ObjectRecord_Impl::setLastValues(query, projectDatabase);

    QVariant value;

    value = query.value(DataPointRecordColumns::analysisRecordId);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_lastAnalysisRecordId = value.toInt();

    value = query.value(DataPointRecordColumns::problemRecordId);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_lastProblemRecordId = value.toInt();

    value = query.value(DataPointRecordColumns::dataPointRecordType);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_lastDataPointRecordType = DataPointRecordType(value.toInt());

    value = query.value(DataPointRecordColumns::complete);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_lastComplete = value.toBool();

    value = query.value(DataPointRecordColumns::failed);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_lastFailed = value.toBool();

    value = query.value(DataPointRecordColumns::selected);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_lastSelected = value.toBool();

    value = query.value(DataPointRecordColumns::runType);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_lastRunType = DataPointRunType(value.toInt());

    value = query.value(DataPointRecordColumns::directory);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_lastDirectory = toPath(value.toString());

    value = query.value(DataPointRecordColumns::inputDataRecordId);
    if (value.isValid() && !value.isNull()) {
      m_lastOsmInputDataRecordId = value.toInt();
    }
    else {
      m_lastOsmInputDataRecordId.reset();
    }

    value = query.value(DataPointRecordColumns::idfInputDataRecordId);
    if (value.isValid() && !value.isNull()) {
      m_lastIdfInputDataRecordId = value.toInt();
    }
    else {
      m_lastIdfInputDataRecordId.reset();
    }

    value = query.value(DataPointRecordColumns::sqlOutputDataRecordId);
    if (value.isValid() && !value.isNull()) {
      m_lastSqlOutputDataRecordId = value.toInt();
    }
    else {
      m_lastSqlOutputDataRecordId.reset();
    }

    value = query.value(DataPointRecordColumns::topLevelJobUUID);
    if (value.isValid() && !value.isNull() && !value.toString().isEmpty()) {
      m_lastTopLevelJobUUID = openstudio::UUID(value.toString());
    }else{
      m_lastTopLevelJobUUID.reset();
    }

    value = query.value(DataPointRecordColumns::dakotaParametersFiles);
    if (value.isValid() && !value.isNull()) {
      Q_FOREACH(const QString& path, value.toString().split(QString(pathSep), QString::SkipEmptyParts)){
        m_lastDakotaParametersFiles.push_back(toPath(path));
      }
    }else{
      m_lastDakotaParametersFiles.clear();
    }
  }

  bool DataPointRecord_Impl::compareValues(const QSqlQuery& query) const {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    bool result = ObjectRecord_Impl::compareValues(query);

    QVariant value;

    value = query.value(DataPointRecordColumns::analysisRecordId);
    OS_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_analysisRecordId == value.toInt());

    value = query.value(DataPointRecordColumns::problemRecordId);
    OS_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_problemRecordId == value.toInt());

    value = query.value(DataPointRecordColumns::dataPointRecordType);
    OS_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_dataPointRecordType == DataPointRecordType(value.toInt()));

    value = query.value(DataPointRecordColumns::complete);
    OS_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_complete == value.toBool());

    value = query.value(DataPointRecordColumns::failed);
    OS_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_failed == value.toBool());

    value = query.value(DataPointRecordColumns::selected);
    OS_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_selected == value.toBool());

    value = query.value(DataPointRecordColumns::runType);
    OS_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_runType == DataPointRunType(value.toInt()));

    value = query.value(DataPointRecordColumns::directory);
    OS_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_directory == toPath(value.toString()));

    value = query.value(DataPointRecordColumns::inputDataRecordId);
    if (value.isValid() && !value.isNull()) {
      result = result && m_osmInputDataRecordId && (*m_osmInputDataRecordId == value.toInt());
    }else{
      result = result && !m_osmInputDataRecordId;
    }

    value = query.value(DataPointRecordColumns::idfInputDataRecordId);
    if (value.isValid() && !value.isNull()) {
      result = result && m_idfInputDataRecordId && (*m_idfInputDataRecordId == value.toInt());
    }else{
      result = result && !m_idfInputDataRecordId;
    }

    value = query.value(DataPointRecordColumns::sqlOutputDataRecordId);
    if (value.isValid() && !value.isNull()) {
      result = result && m_sqlOutputDataRecordId && (*m_sqlOutputDataRecordId == value.toInt());
    }else{
      result = result && !m_sqlOutputDataRecordId;
    }

    value = query.value(DataPointRecordColumns::topLevelJobUUID);
    if (value.isValid() && !value.isNull() && !value.toString().isEmpty()) {
      openstudio::UUID testUUID(value.toString());
      result = result && m_lastTopLevelJobUUID && (m_lastTopLevelJobUUID.get() == testUUID);
    }else{
      result = result && !m_lastTopLevelJobUUID;
    }

    value = query.value(DataPointRecordColumns::dakotaParametersFiles);
    if (value.isValid() && !value.isNull()) {
      QStringList dakotaParametersFiles;
      BOOST_FOREACH(const openstudio::path& path, m_dakotaParametersFiles) {
        dakotaParametersFiles << toQString(path);
      }
      result = result && (value.toString() == dakotaParametersFiles.join(QString(pathSep)));
    }else{
      result = result && m_dakotaParametersFiles.empty();
    }

    return result;
  }

  void DataPointRecord_Impl::saveLastValues() {
    ObjectRecord_Impl::saveLastValues();

    m_lastAnalysisRecordId = m_analysisRecordId;
    m_lastProblemRecordId = m_problemRecordId;
    m_lastDataPointRecordType = m_dataPointRecordType;
    m_lastComplete = m_complete;
    m_lastFailed = m_failed;
    m_lastSelected = m_selected;
    m_lastRunType = m_runType;
    m_lastDirectory = m_directory;
    m_lastOsmInputDataRecordId = m_osmInputDataRecordId;
    m_lastIdfInputDataRecordId = m_idfInputDataRecordId;
    m_lastSqlOutputDataRecordId = m_sqlOutputDataRecordId;
    m_lastTopLevelJobUUID = m_topLevelJobUUID;
    m_lastDakotaParametersFiles = m_dakotaParametersFiles;
  }

  void DataPointRecord_Impl::revertToLastValues() {
    ObjectRecord_Impl::revertToLastValues();

    m_analysisRecordId = m_lastAnalysisRecordId;
    m_problemRecordId = m_lastProblemRecordId;
    m_dataPointRecordType = m_lastDataPointRecordType;
    m_complete = m_lastComplete;
    m_failed = m_lastFailed;
    m_selected = m_lastSelected;
    m_runType = m_lastRunType;
    m_directory = m_lastDirectory;
    m_osmInputDataRecordId = m_lastOsmInputDataRecordId;
    m_idfInputDataRecordId = m_lastIdfInputDataRecordId;
    m_sqlOutputDataRecordId = m_lastSqlOutputDataRecordId;
    m_topLevelJobUUID = m_lastTopLevelJobUUID;
    m_dakotaParametersFiles = m_lastDakotaParametersFiles;
  }

} // detail

DataPointRecord::DataPointRecord(const analysis::DataPoint& dataPoint,
                                 AnalysisRecord& analysisRecord,
                                 const ProblemRecord& problemRecord)
  : ObjectRecord(boost::shared_ptr<detail::DataPointRecord_Impl>(
        new detail::DataPointRecord_Impl(dataPoint,
                                         DataPointRecordType::DataPointRecord,
                                         analysisRecord,
                                         problemRecord)),
        analysisRecord.projectDatabase())
{
  OS_ASSERT(getImpl<detail::DataPointRecord_Impl>());

  constructRelatedRecords(dataPoint,analysisRecord,problemRecord);
}

DataPointRecord::DataPointRecord(const QSqlQuery& query, ProjectDatabase& database)
  : ObjectRecord(boost::shared_ptr<detail::DataPointRecord_Impl>(
        new detail::DataPointRecord_Impl(query, database)),
        database)
{
  OS_ASSERT(getImpl<detail::DataPointRecord_Impl>());
}

DataPointRecord::DataPointRecord(boost::shared_ptr<detail::DataPointRecord_Impl> impl,
                                 ProjectDatabase database)
  : ObjectRecord(impl, database)
{
  OS_ASSERT(getImpl<detail::DataPointRecord_Impl>());
}

std::string DataPointRecord::databaseTableName() {
  return "DataPointRecords";
}

UpdateByIdQueryData DataPointRecord::updateByIdQueryData() {
  static UpdateByIdQueryData result;
  if (result.queryString.empty()) {
    // numeric column identifiers
    result.columnValues = ColumnsType::getValues();

    // query string
    std::stringstream ss;
    ss << "UPDATE " << databaseTableName() << " SET ";
    int expectedValue = 0;
    for (std::set<int>::const_iterator it = result.columnValues.begin(),
         itend = result.columnValues.end(); it != itend; ++it)
    {
      // require 0 based columns, don't skip any
      OS_ASSERT(*it == expectedValue);
      // column name is name, type is description
      ss << ColumnsType::valueName(*it) << "=:" << ColumnsType::valueName(*it);
      // is this the last column?
      std::set<int>::const_iterator nextIt = it;
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
    for (std::set<int>::const_iterator it = result.columnValues.begin(),
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

void DataPointRecord::createIndices(QSqlDatabase& qSqlDatabase, const std::string& databaseTableName)
{
  ObjectRecord::createIndices(qSqlDatabase,databaseTableName);

  QSqlQuery query(qSqlDatabase);
  query.prepare(toQString("CREATE INDEX " + databaseTableName + "analysisRecordIdIndex ON " + databaseTableName + " (analysisRecordId)"));
  assertExec(query);

  query.prepare(toQString("CREATE INDEX " + databaseTableName + "problemRecordIdIndex ON " + databaseTableName + " (problemRecordId)"));
  assertExec(query);
}

void DataPointRecord::updatePathData(ProjectDatabase database,
                                     const openstudio::path& originalBase,
                                     const openstudio::path& newBase)
{
  bool didStartTransaction = database.startTransaction();

  DataPointRecordVector records = getDataPointRecords(database);
  BOOST_FOREACH(DataPointRecord& record,records) {
    record.getImpl<detail::DataPointRecord_Impl>()->updatePathData(originalBase,newBase);
  }

  if (didStartTransaction) {
    bool test = database.commitTransaction();
    OS_ASSERT(test);
  }
}

boost::optional<DataPointRecord> DataPointRecord::factoryFromQuery(const QSqlQuery& query,
                                                                   ProjectDatabase& database)
{
  OptionalDataPointRecord result;

  int dataPointRecordType = query.value(DataPointRecordColumns::dataPointRecordType).toInt();

  switch (dataPointRecordType) {
    case DataPointRecordType::DataPointRecord :
      result = DataPointRecord(query, database);
     break;
    case DataPointRecordType::OptimizationDataPointRecord :
      result = OptimizationDataPointRecord(query, database);
     break;
    default :
      LOG(Error,"Unknown DataPointRecordType " << dataPointRecordType);
  }

  return result;
}

DataPointRecord DataPointRecord::factoryFromDataPoint(const analysis::DataPoint& dataPoint,
                                                      AnalysisRecord& analysisRecord,
                                                      const ProblemRecord& problemRecord)
{
  if (dataPoint.optionalCast<analysis::OptimizationDataPoint>()) {
    OS_ASSERT(problemRecord.optionalCast<OptimizationProblemRecord>());
    return OptimizationDataPointRecord(dataPoint.cast<analysis::OptimizationDataPoint>(),
                                       analysisRecord,
                                       problemRecord.cast<OptimizationProblemRecord>());
  }
  else {
    return DataPointRecord(dataPoint,analysisRecord,problemRecord);
  }

  OS_ASSERT(false);
  return DataPointRecord(boost::shared_ptr<detail::DataPointRecord_Impl>());
}

std::vector<DataPointRecord> DataPointRecord::getDataPointRecords(ProjectDatabase& database) {
  std::vector<DataPointRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + DataPointRecord::databaseTableName()));
  assertExec(query);
  while (query.next()) {
    OptionalDataPointRecord dataPointRecord = DataPointRecord::factoryFromQuery(query, database);
    if (dataPointRecord) {
      result.push_back(*dataPointRecord);
    }
  }

  return result;
}

boost::optional<DataPointRecord> DataPointRecord::getDataPointRecord(
    int id, ProjectDatabase& database)
{
  boost::optional<DataPointRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + DataPointRecord::databaseTableName() + " WHERE id=:id"));
  query.bindValue(":id",id);
  assertExec(query);
  if (query.first()) {
    result = DataPointRecord::factoryFromQuery(query, database);
  }

  return result;
}

AnalysisRecord DataPointRecord::analysisRecord() const {
  return getImpl<detail::DataPointRecord_Impl>()->analysisRecord();
}

ProblemRecord DataPointRecord::problemRecord() const {
  return getImpl<detail::DataPointRecord_Impl>()->problemRecord();
}

bool DataPointRecord::isComplete() const {
  return getImpl<detail::DataPointRecord_Impl>()->isComplete();
}

bool DataPointRecord::complete() const {
  return getImpl<detail::DataPointRecord_Impl>()->complete();
}

bool DataPointRecord::failed() const {
  return getImpl<detail::DataPointRecord_Impl>()->failed();
}

bool DataPointRecord::selected() const {
  return getImpl<detail::DataPointRecord_Impl>()->selected();
}

analysis::DataPointRunType DataPointRecord::runType() const {
  return getImpl<detail::DataPointRecord_Impl>()->runType();
}

openstudio::path DataPointRecord::directory() const {
  return getImpl<detail::DataPointRecord_Impl>()->directory();
}

std::vector<DataPointValueRecord> DataPointRecord::continuousVariableValueRecords() const {
  return getImpl<detail::DataPointRecord_Impl>()->continuousVariableValueRecords();
}

std::vector<DataPointValueRecord> DataPointRecord::responseValueRecords() const {
  return getImpl<detail::DataPointRecord_Impl>()->responseValueRecords();
}

std::vector<double> DataPointRecord::responseValues() const {
  return getImpl<detail::DataPointRecord_Impl>()->responseValues();
}

boost::optional<FileReferenceRecord> DataPointRecord::osmInputDataRecord() const {
  return getImpl<detail::DataPointRecord_Impl>()->osmInputDataRecord();
}

boost::optional<FileReferenceRecord> DataPointRecord::idfInputDataRecord() const {
  return getImpl<detail::DataPointRecord_Impl>()->idfInputDataRecord();
}

boost::optional<FileReferenceRecord> DataPointRecord::sqlOutputDataRecord() const {
  return getImpl<detail::DataPointRecord_Impl>()->sqlOutputDataRecord();
}

std::vector<FileReferenceRecord> DataPointRecord::xmlOutputDataRecords() const {
  return getImpl<detail::DataPointRecord_Impl>()->xmlOutputDataRecords();
}

boost::optional<openstudio::UUID> DataPointRecord::topLevelJobUUID() const {
  return getImpl<detail::DataPointRecord_Impl>()->topLevelJobUUID();
}

std::vector<TagRecord> DataPointRecord::tagRecords() const {
  return getImpl<detail::DataPointRecord_Impl>()->tagRecords();
}

std::vector<AttributeRecord> DataPointRecord::attributeRecords() const {
  return getImpl<detail::DataPointRecord_Impl>()->attributeRecords();
}

analysis::DataPoint DataPointRecord::dataPoint() const {
  return getImpl<detail::DataPointRecord_Impl>()->dataPoint();
}

void DataPointRecord::setDirectory(const openstudio::path& directory) {
  getImpl<detail::DataPointRecord_Impl>()->setDirectory(directory);
}

void DataPointRecord::clearResults() {
  getImpl<detail::DataPointRecord_Impl>()->clearResults();
}

/// @cond
DataPointRecord::DataPointRecord(boost::shared_ptr<detail::DataPointRecord_Impl> impl)
  : ObjectRecord(impl)
{}
/// @endcond

void DataPointRecord::constructRelatedRecords(const analysis::DataPoint& dataPoint,
                                              AnalysisRecord& analysisRecord,
                                              const ProblemRecord& problemRecord)
{
  DataPointRecord copyOfThis(getImpl<detail::DataPointRecord_Impl>());
  ProjectDatabase database = copyOfThis.projectDatabase();
  bool isNew = database.isNewRecord(copyOfThis);
  if (!isNew) {
    getImpl<detail::DataPointRecord_Impl>()->revertToLastRecordIds();
  }

  // DataPoints are defined by their variable values. Variable values cannot be edited.
  // Thus, only need to save variable values on initial construction.

  if (isNew) {
    std::vector<QVariant> variableValues = dataPoint.variableValues();
    InputVariableRecordVector inputVariableRecords = problemRecord.inputVariableRecords();
    OS_ASSERT(inputVariableRecords.size() == variableValues.size());
    for (int i = 0, n = variableValues.size(); i < n; ++i) {
      if (OptionalMeasureGroupRecord omgr = inputVariableRecords[i].optionalCast<MeasureGroupRecord>())
      {
        MeasureRecord mr = omgr->getMeasureRecord(variableValues[i].toInt());
        DataPoint_Measure_JoinRecord discreteVariableValueRecord(copyOfThis,mr);
      }
      else {
        OptionalContinuousVariableRecord ocvr = inputVariableRecords[i].optionalCast<ContinuousVariableRecord>();
        OS_ASSERT(ocvr);
        DataPointValueRecord continuousVariableValueRecord(variableValues[i].toDouble(),
                                                           copyOfThis,
                                                           *ocvr);
      }
    }
  }
  else {
    // DataPoint variable values are fixed in the non-serialization public interface, but there
    // are select places where variables may be added behind the scenes.
    JoinRecordVector jrs = joinRecords();
    DataPointValueRecordVector cvvrs = continuousVariableValueRecords();
    unsigned n = jrs.size() + cvvrs.size();
    std::vector<QVariant> variableValues = dataPoint.variableValues();
    if (n < variableValues.size()) {
      // variables have been added
      // remove all old records
      BOOST_FOREACH(JoinRecord& jr,jrs) {
        database.removeRecord(jr);
      }
      BOOST_FOREACH(DataPointValueRecord& cvvr,cvvrs) {
        database.removeRecord(cvvr);
      }
      // add new records
      InputVariableRecordVector inputVariableRecords = problemRecord.inputVariableRecords();
      OS_ASSERT(inputVariableRecords.size() == variableValues.size());
      for (int i = 0, n = variableValues.size(); i < n; ++i) {
        if (OptionalMeasureGroupRecord omgr = inputVariableRecords[i].optionalCast<MeasureGroupRecord>())
        {
          LOG(Debug,"Variable " << i << ": " << omgr->name() << ", Value: " << variableValues[i].toInt());
          MeasureRecord mr = omgr->getMeasureRecord(variableValues[i].toInt());
          DataPoint_Measure_JoinRecord discreteVariableValueRecord(copyOfThis,mr);
        }
        else {
          OptionalContinuousVariableRecord ocvr = inputVariableRecords[i].optionalCast<ContinuousVariableRecord>();
          OS_ASSERT(ocvr);
          DataPointValueRecord continuousVariableValueRecord(variableValues[i].toDouble(),
                                                             copyOfThis,
                                                             *ocvr);
        }
      }
    }
  }

  // Save child FileReferences
  OptionalFileReference osmInputData = dataPoint.osmInputData();
  OptionalFileReferenceRecord newOsmInputDataRecord =
      saveChildFileReference(osmInputData,
                             osmInputDataRecord(),
                             copyOfThis,
                             database,
                             isNew);
  if (newOsmInputDataRecord) {
    getImpl<detail::DataPointRecord_Impl>()->setOsmInputDataRecordId(newOsmInputDataRecord->id());
  }
  if (!osmInputData) {
    OS_ASSERT(!newOsmInputDataRecord);
    getImpl<detail::DataPointRecord_Impl>()->clearOsmInputDataRecordId();
  }
  OptionalFileReference idfInputData = dataPoint.idfInputData();
  OptionalFileReferenceRecord newIdfInputDataRecord =
      saveChildFileReference(idfInputData,
                             idfInputDataRecord(),
                             copyOfThis,
                             database,
                             isNew);
  if (newIdfInputDataRecord) {
    getImpl<detail::DataPointRecord_Impl>()->setIdfInputDataRecordId(newIdfInputDataRecord->id());
  }
  if (!idfInputData) {
    OS_ASSERT(!newIdfInputDataRecord);
    getImpl<detail::DataPointRecord_Impl>()->clearIdfInputDataRecordId();
  }
  OptionalFileReference sqlOutputData = dataPoint.sqlOutputData();
  OptionalFileReferenceRecord newSqlOutputDataRecord =
      saveChildFileReference(sqlOutputData,
                             sqlOutputDataRecord(),
                             copyOfThis,
                             database,
                             isNew);
  if (newSqlOutputDataRecord) {
    getImpl<detail::DataPointRecord_Impl>()->setSqlOutputDataRecordId(newSqlOutputDataRecord->id());
  }
  if (!sqlOutputData) {
    OS_ASSERT(!newSqlOutputDataRecord);
    getImpl<detail::DataPointRecord_Impl>()->clearSqlOutputDataRecordId();
  }
  FileReferenceVector xmlOutputData = dataPoint.xmlOutputData();
  saveChildFileReferences(xmlOutputData,
                          xmlOutputDataRecords(),
                          copyOfThis,
                          database,
                          isNew);

  // Remove old response function values
  if (!isNew) {
    DataPointValueRecordVector oldResponseValueRecords = responseValueRecords();
    BOOST_FOREACH(DataPointValueRecord& oldRecord,oldResponseValueRecords) {
      database.removeRecord(oldRecord);
    }
  }

  // Save current response function values
  FunctionRecordVector responses = problemRecord.responseRecords();
  DoubleVector values = dataPoint.responseValues();
  if (!values.empty()) {
    int n = responses.size();
    OS_ASSERT(values.size() == static_cast<size_t>(n));
    for (int i = 0; i < n; ++i) {
      DataPointValueRecord newValue(values[i],copyOfThis,responses[i]);
    }
  }

  // Create tag records
  TagVector tags = dataPoint.tags();
  BOOST_FOREACH(const Tag& tag,tags) {
    TagRecord newTagRecord(tag,copyOfThis);
  }

  // Delete obsolete tag records
  BOOST_FOREACH(const TagRecord& tagRecord,tagRecords()) {
    if (std::find_if(tags.begin(),tags.end(),boost::bind(uuidEquals<Tag,UUID>,_1,tagRecord.handle())) == tags.end()) {
      Record record = tagRecord.cast<Record>();
      analysisRecord.projectDatabase().removeRecord(record);
    }
  }
}

boost::optional<FileReferenceRecord> DataPointRecord::saveChildFileReference(
    const boost::optional<FileReference>& childFileReference,
    boost::optional<FileReferenceRecord> oldFileReferenceRecord,
    DataPointRecord& copyOfThis,
    ProjectDatabase& database,
    bool isNew)
{
  bool save = false;
  OptionalFileReferenceRecord result;
  if (!isNew) {
    {
      if (oldFileReferenceRecord &&
          (!childFileReference ||
           (oldFileReferenceRecord->handle() != childFileReference->uuid())))
      {
        database.removeRecord(*oldFileReferenceRecord);
        save = true;
      }
      else if (childFileReference &&
               (!oldFileReferenceRecord ||
                (oldFileReferenceRecord->uuidLast() != childFileReference->versionUUID())))
      {
        save = true;
      }
    }
    database.unloadUnusedCleanRecords();
  }
  if (childFileReference && (save || isNew)) {
    result = FileReferenceRecord(*childFileReference,copyOfThis);
    LOG(Debug,"DataPoint " << id() << ", directory " << toString(directory()) << ", child FileReference "
      << toString(childFileReference->path()) << ".");
  }
  return result;
}

std::vector<FileReferenceRecord> DataPointRecord::saveChildFileReferences(
    const std::vector<FileReference>& childFileReferences,
    std::vector<FileReferenceRecord> oldFileReferenceRecords,
    DataPointRecord& copyOfThis,
    ProjectDatabase& database,
    bool isNew)
{
  FileReferenceRecordVector result;
  Q_FOREACH(const FileReference& childFileReference, childFileReferences) {
    bool save(true);
    if (!isNew) {
      // see if there is already a record
      FileReferenceRecordVector::iterator it = std::find_if(
            oldFileReferenceRecords.begin(),
            oldFileReferenceRecords.end(),
            boost::bind(handleEquals<ObjectRecord,UUID>,_1,childFileReference.uuid()));
      if (it != oldFileReferenceRecords.end()) {
        // found, see if has changed
        if (it->uuidLast() == childFileReference.versionUUID()) {
          save = false;
        }
        else {
          // will save. clear out AttributeRecords associated with the old one.
          AttributeRecordVector oldAttributes = it->attributeRecords();
          BOOST_FOREACH(AttributeRecord& oldAttribute, oldAttributes) {
            database.removeRecord(oldAttribute);
          }
        }
        oldFileReferenceRecords.erase(it); // do not remove this existing record
      }
      database.unloadUnusedCleanRecords();
    }
    if (save || isNew) {
      result.push_back(FileReferenceRecord(childFileReference,copyOfThis));
      // save attributes
      OptionalAttribute wrapperAttribute = Attribute::loadFromXml(childFileReference.path());
      if (wrapperAttribute &&
          (wrapperAttribute->valueType() == AttributeValueType::AttributeVector))
      {
        Q_FOREACH(const Attribute& attribute,wrapperAttribute->valueAsAttributeVector()) {
          AttributeRecord attributeRecord(attribute,result.back());
          Q_UNUSED(attributeRecord);
        }
      }
    }
  }

  // remove remaining oldFileReferenceRecords
  if (!isNew) {
    BOOST_FOREACH(FileReferenceRecord& oldFileReferenceRecord,oldFileReferenceRecords) {
      database.removeRecord(oldFileReferenceRecord);
    }
  }

  return result;
}

} // project
} // openstudio

