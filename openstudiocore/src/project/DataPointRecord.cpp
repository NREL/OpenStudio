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
#include <project/DataPoint_DiscretePerturbation_JoinRecord.hpp>
#include <project/DataPoint_DiscretePerturbation_JoinRecord_Impl.hpp>
#include <project/DataPointValueRecord.hpp>

#include <project/AnalysisRecord.hpp>
#include <project/ProblemRecord.hpp>
#include <project/OptimizationProblemRecord.hpp>
#include <project/OptimizationProblemRecord_Impl.hpp>
#include <project/OptimizationDataPointRecord.hpp>
#include <project/ContinuousVariableRecord.hpp>
#include <project/ContinuousVariableRecord_Impl.hpp>
#include <project/DiscreteVariableRecord.hpp>
#include <project/DiscreteVariableRecord_Impl.hpp>
#include <project/DiscretePerturbationRecord.hpp>
#include <project/FunctionRecord.hpp>
#include <project/FileReferenceRecord.hpp>
#include <project/AttributeRecord.hpp>
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
    BOOST_ASSERT(query.isValid());
    BOOST_ASSERT(query.isActive());
    BOOST_ASSERT(query.isSelect());

    QVariant value;

    value = query.value(DataPointRecordColumns::analysisRecordId);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    m_analysisRecordId = value.toInt();

    value = query.value(DataPointRecordColumns::problemRecordId);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    m_problemRecordId = value.toInt();

    value = query.value(DataPointRecordColumns::dataPointRecordType);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    m_dataPointRecordType = DataPointRecordType(value.toInt());

    value = query.value(DataPointRecordColumns::complete);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    m_complete = value.toBool();

    value = query.value(DataPointRecordColumns::failed);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    m_failed = value.toBool();

    value = query.value(DataPointRecordColumns::directory);
    BOOST_ASSERT(value.isValid() && !value.isNull());
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

    value = query.value(DataPointRecordColumns::xmlOutputDataRecordId);
    if (value.isValid() && !value.isNull()) {
       m_xmlOutputDataRecordId = value.toInt();
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
    ofrr = xmlOutputDataRecord();
    if (ofrr) {
      result.push_back(*ofrr);
    }
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

    DataPoint_DiscretePerturbation_JoinRecordVector discretePerturbationJoins =
      JoinRecord::getJoinRecordsForLeftId<DataPoint_DiscretePerturbation_JoinRecord>(id(),projectDatabase());
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
    return m_complete;
  }

  bool DataPointRecord_Impl::failed() const {
    return m_failed;
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
        BOOST_ASSERT(lastVectorIndex.get() < vectorIndex);
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

  boost::optional<FileReferenceRecord> DataPointRecord_Impl::xmlOutputDataRecord() const {
    OptionalFileReferenceRecord result;
    if (m_xmlOutputDataRecordId) {
      ProjectDatabase database = projectDatabase();
      result = FileReferenceRecord::getFileReferenceRecord(*m_xmlOutputDataRecordId,database);
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
        query.prepare(toQString("SELECT * FROM " + DiscretePerturbationRecord::databaseTableName() + " o , " +
            DataPoint_DiscretePerturbation_JoinRecord::databaseTableName() + " j " +
            " WHERE o.variableRecordId=:variableRecordId AND j.leftId=:leftId AND o.id=j.rightId "));
        query.bindValue(":variableRecordId",inputVariableRecord.id());
        query.bindValue(":leftId",id());
        assertExec(query);
        if (query.first()) {
          // by asking for whole record, perturbationVectorIndex() will be correct even if
          // record is dirty
          OptionalDiscretePerturbationRecord dpr = DiscretePerturbationRecord::factoryFromQuery(query,database);
          if (dpr && dpr->perturbationVectorIndex()) {
            variableValues.push_back(dpr->perturbationVectorIndex().get());
          }
        }
      }
      else {
        BOOST_ASSERT(inputVariableRecord.optionalCast<ContinuousVariableRecord>());
        QSqlQuery query(*(database.qSqlDatabase()));
        query.prepare(toQString("SELECT dataPointValue FROM " + DataPointValueRecord::databaseTableName() +
            " WHERE dataPointRecordId=:dataPointRecordId AND continuousVariableRecordId=:continuousVariableRecordId"));
        query.bindValue(":dataPointRecordId",id());
        query.bindValue(":continuousVariableRecordId",inputVariableRecord.id());
        assertExec(query);
        if (query.first()) {
          QVariant value = query.value(0);
          BOOST_ASSERT(value.isValid() && !value.isNull());
          variableValues.push_back(value.toDouble());
        }
      }
    }
    BOOST_ASSERT(variableValues.size() == inputVariableRecords.size());

    OptionalFileReferenceRecord ofrr;
    OptionalFileReference oOsmInputData;
    OptionalFileReference oIdfInputData;
    OptionalFileReference oSqlOutputData;
    OptionalFileReference oXmlOutputData;
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
    ofrr = xmlOutputDataRecord();
    if (ofrr) {
      oXmlOutputData = ofrr->fileReference();
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
                               m_complete,
                               m_failed,
                               problemRecord.problem(),
                               variableValues,
                               responseValues(),
                               m_directory,
                               oOsmInputData,
                               oIdfInputData,
                               oSqlOutputData,
                               oXmlOutputData,
                               tags,
                               topLevelJob,
                               m_dakotaParametersFiles);
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
    ofrr = xmlOutputDataRecord();
    if (ofrr) {
      database.removeRecord(*ofrr);
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
    m_xmlOutputDataRecordId = m_lastXmlOutputDataRecordId;
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

  void DataPointRecord_Impl::setXmlOutputDataRecordId(int id) {
    m_xmlOutputDataRecordId = id;
    this->onChange(false);
  }

  void DataPointRecord_Impl::clearXmlOutputDataRecordId() {
    if (m_xmlOutputDataRecordId) {
      m_xmlOutputDataRecordId.reset();
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
    if (m_xmlOutputDataRecordId) {
      query.bindValue(DataPointRecordColumns::xmlOutputDataRecordId, *m_xmlOutputDataRecordId);
    }
    else {
      query.bindValue(DataPointRecordColumns::xmlOutputDataRecordId, QVariant(QVariant::Int));
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
    BOOST_ASSERT(query.isValid());
    BOOST_ASSERT(query.isActive());
    BOOST_ASSERT(query.isSelect());

    ObjectRecord_Impl::setLastValues(query, projectDatabase);

    QVariant value;

    value = query.value(DataPointRecordColumns::analysisRecordId);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    m_lastAnalysisRecordId = value.toInt();

    value = query.value(DataPointRecordColumns::problemRecordId);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    m_lastProblemRecordId = value.toInt();

    value = query.value(DataPointRecordColumns::dataPointRecordType);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    m_lastDataPointRecordType = DataPointRecordType(value.toInt());

    value = query.value(DataPointRecordColumns::complete);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    m_lastComplete = value.toBool();

    value = query.value(DataPointRecordColumns::failed);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    m_lastFailed = value.toBool();

    value = query.value(DataPointRecordColumns::directory);
    BOOST_ASSERT(value.isValid() && !value.isNull());
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

    value = query.value(DataPointRecordColumns::xmlOutputDataRecordId);
    if (value.isValid() && !value.isNull()) {
      m_lastXmlOutputDataRecordId = value.toInt();
    }
    else {
      m_lastXmlOutputDataRecordId.reset();
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
    BOOST_ASSERT(query.isValid());
    BOOST_ASSERT(query.isActive());
    BOOST_ASSERT(query.isSelect());

    bool result = ObjectRecord_Impl::compareValues(query);

    QVariant value;

    value = query.value(DataPointRecordColumns::analysisRecordId);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_analysisRecordId == value.toInt());

    value = query.value(DataPointRecordColumns::problemRecordId);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_problemRecordId == value.toInt());

    value = query.value(DataPointRecordColumns::dataPointRecordType);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_dataPointRecordType == DataPointRecordType(value.toInt()));

    value = query.value(DataPointRecordColumns::complete);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_complete == value.toBool());

    value = query.value(DataPointRecordColumns::failed);
    BOOST_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_failed == value.toBool());

    value = query.value(DataPointRecordColumns::directory);
    BOOST_ASSERT(value.isValid() && !value.isNull());
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

    value = query.value(DataPointRecordColumns::xmlOutputDataRecordId);
    if (value.isValid() && !value.isNull()) {
      result = result && m_xmlOutputDataRecordId && (*m_xmlOutputDataRecordId == value.toInt());
    }else{
      result = result && !m_xmlOutputDataRecordId;
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
    m_lastDirectory = m_directory;
    m_lastOsmInputDataRecordId = m_osmInputDataRecordId;
    m_lastIdfInputDataRecordId = m_idfInputDataRecordId;
    m_lastSqlOutputDataRecordId = m_sqlOutputDataRecordId;
    m_lastXmlOutputDataRecordId = m_xmlOutputDataRecordId;
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
    m_directory = m_lastDirectory;
    m_osmInputDataRecordId = m_lastOsmInputDataRecordId;
    m_idfInputDataRecordId = m_lastIdfInputDataRecordId;
    m_sqlOutputDataRecordId = m_lastSqlOutputDataRecordId;
    m_xmlOutputDataRecordId = m_lastXmlOutputDataRecordId;
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
  BOOST_ASSERT(getImpl<detail::DataPointRecord_Impl>());

  constructRelatedRecords(dataPoint,analysisRecord,problemRecord);
}

DataPointRecord::DataPointRecord(const QSqlQuery& query, ProjectDatabase& database)
  : ObjectRecord(boost::shared_ptr<detail::DataPointRecord_Impl>(
        new detail::DataPointRecord_Impl(query, database)),
        database)
{
  BOOST_ASSERT(getImpl<detail::DataPointRecord_Impl>());
}

DataPointRecord::DataPointRecord(boost::shared_ptr<detail::DataPointRecord_Impl> impl,
                                 ProjectDatabase database)
  : ObjectRecord(impl, database)
{
  BOOST_ASSERT(getImpl<detail::DataPointRecord_Impl>());
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
      BOOST_ASSERT(*it == expectedValue);
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
    BOOST_ASSERT(test);
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
    BOOST_ASSERT(problemRecord.optionalCast<OptimizationProblemRecord>());
    return OptimizationDataPointRecord(dataPoint.cast<analysis::OptimizationDataPoint>(),
                                       analysisRecord,
                                       problemRecord.cast<OptimizationProblemRecord>());
  }
  else {
    return DataPointRecord(dataPoint,analysisRecord,problemRecord);
  }

  BOOST_ASSERT(false);
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

bool DataPointRecord::failed() const {
  return getImpl<detail::DataPointRecord_Impl>()->failed();
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

boost::optional<FileReferenceRecord> DataPointRecord::xmlOutputDataRecord() const {
  return getImpl<detail::DataPointRecord_Impl>()->xmlOutputDataRecord();
}

boost::optional<openstudio::UUID> DataPointRecord::topLevelJobUUID() const {
  return getImpl<detail::DataPointRecord_Impl>()->topLevelJobUUID();
}

std::vector<TagRecord> DataPointRecord::tagRecords() const {
  return getImpl<detail::DataPointRecord_Impl>()->tagRecords();
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
    BOOST_ASSERT(inputVariableRecords.size() == variableValues.size());
    for (int i = 0, n = variableValues.size(); i < n; ++i) {
      if (OptionalDiscreteVariableRecord odvr = inputVariableRecords[i].optionalCast<DiscreteVariableRecord>())
      {
        DiscretePerturbationRecord dpr = odvr->getDiscretePerturbationRecord(variableValues[i].toInt());
        DataPoint_DiscretePerturbation_JoinRecord discreteVariableValueRecord(copyOfThis,dpr);
      }
      else {
        OptionalContinuousVariableRecord ocvr = inputVariableRecords[i].optionalCast<ContinuousVariableRecord>();
        BOOST_ASSERT(ocvr);
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
      BOOST_ASSERT(inputVariableRecords.size() == variableValues.size());
      for (int i = 0, n = variableValues.size(); i < n; ++i) {
        if (OptionalDiscreteVariableRecord odvr = inputVariableRecords[i].optionalCast<DiscreteVariableRecord>())
        {
          LOG(Debug,"Variable " << i << ": " << odvr->name() << ", Value: " << variableValues[i].toInt());
          DiscretePerturbationRecord dpr = odvr->getDiscretePerturbationRecord(variableValues[i].toInt());
          DataPoint_DiscretePerturbation_JoinRecord discreteVariableValueRecord(copyOfThis,dpr);
        }
        else {
          OptionalContinuousVariableRecord ocvr = inputVariableRecords[i].optionalCast<ContinuousVariableRecord>();
          BOOST_ASSERT(ocvr);
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
    BOOST_ASSERT(!newOsmInputDataRecord);
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
    BOOST_ASSERT(!newIdfInputDataRecord);
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
    BOOST_ASSERT(!newSqlOutputDataRecord);
    getImpl<detail::DataPointRecord_Impl>()->clearSqlOutputDataRecordId();
  }
  OptionalFileReference xmlOutputData = dataPoint.xmlOutputData();
  OptionalFileReferenceRecord newXmlOutputDataRecord =
      saveChildFileReference(xmlOutputData,
                             xmlOutputDataRecord(),
                             copyOfThis,
                             database,
                             isNew);
  if (newXmlOutputDataRecord) {
    getImpl<detail::DataPointRecord_Impl>()->setXmlOutputDataRecordId(newXmlOutputDataRecord->id());
    // save output attributes to database for quick access
    // (old attributes will have been deleted by call to remove xmlOutputDataRecord())
    AttributeVector attributes = dataPoint.outputAttributes();
    BOOST_FOREACH(const Attribute& attribute,attributes) {
      AttributeRecord attributeRecord(attribute,*newXmlOutputDataRecord);
    }
  }
  if (!xmlOutputData) {
    BOOST_ASSERT(!newXmlOutputDataRecord);
    getImpl<detail::DataPointRecord_Impl>()->clearXmlOutputDataRecordId();
  }

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
    BOOST_ASSERT(values.size() == static_cast<size_t>(n));
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

} // project
} // openstudio

