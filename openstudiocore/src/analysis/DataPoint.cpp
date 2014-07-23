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

#include "DataPoint.hpp"
#include "DataPoint_Impl.hpp"

#include "Analysis.hpp"
#include "Analysis_Impl.hpp"
#include "InputVariable.hpp"
#include "OptimizationDataPoint.hpp"
#include "OptimizationDataPoint_Impl.hpp"

#include "../runmanager/lib/Job.hpp"
#include "../runmanager/lib/JSON.hpp"
#include "../runmanager/lib/RunManager.hpp"

#include "../ruleset/OSResult.hpp"

#include "../utilities/math/FloatCompare.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/core/Containers.hpp"
#include "../utilities/core/FileReference.hpp"
#include "../utilities/core/Finder.hpp"
#include "../utilities/core/Json.hpp"
#include "../utilities/core/UnzipFile.hpp"

namespace openstudio {
namespace analysis {

namespace detail {

  DataPoint_Impl::DataPoint_Impl(const Problem& problem,
                                 const std::vector<QVariant>& variableValues)
    : AnalysisObject_Impl(std::string()),
      m_problem(problem),
      m_problemUUID(problem.uuid()),
      m_complete(false),
      m_failed(false),
      m_selected(true),
      m_runType(DataPointRunType::Local),
      m_variableValues(variableValues)
  {}

  DataPoint_Impl::DataPoint_Impl(const UUID& uuid,
                                 const UUID& versionUUID,
                                 const std::string& name,
                                 const std::string& displayName,
                                 const std::string& description,
                                 const Problem& problem,
                                 bool complete,
                                 bool failed,
                                 bool selected,
                                 DataPointRunType runType,
                                 const std::vector<QVariant>& variableValues,
                                 const std::vector<double>& responseValues,
                                 const openstudio::path& directory,
                                 const boost::optional<FileReference>& osmInputData,
                                 const boost::optional<FileReference>& idfInputData,
                                 const boost::optional<FileReference>& sqlOutputData,
                                 const boost::optional<runmanager::Job>& topLevelJob,
                                 const std::vector<openstudio::path>& dakotaParametersFiles,
                                 const std::vector<Tag>& tags,
                                 const std::vector<Attribute>& outputAttributes)
    : AnalysisObject_Impl(uuid,versionUUID,name,displayName,description),
      m_problem(problem),
      m_problemUUID(problem.uuid()),
      m_complete(complete),
      m_failed(failed),
      m_selected(selected),
      m_runType(runType),
      m_variableValues(variableValues),
      m_responseValues(responseValues),
      m_directory(directory),
      m_osmInputData(osmInputData),
      m_idfInputData(idfInputData),
      m_sqlOutputData(sqlOutputData),
      m_topLevelJob(topLevelJob),
      m_dakotaParametersFiles(dakotaParametersFiles),
      m_tags(tags),
      m_outputAttributes(outputAttributes)
  {}

  DataPoint_Impl::DataPoint_Impl(const UUID& uuid,
                                 const UUID& versionUUID,
                                 const std::string& name,
                                 const std::string& displayName,
                                 const std::string& description,
                                 const UUID& problemUUID,
                                 const boost::optional<UUID>& analysisUUID,
                                 bool complete,
                                 bool failed,
                                 bool selected,
                                 DataPointRunType runType,
                                 const std::vector<QVariant>& variableValues,
                                 const std::vector<double>& responseValues,
                                 const openstudio::path& directory,
                                 const boost::optional<FileReference>& osmInputData,
                                 const boost::optional<FileReference>& idfInputData,
                                 const boost::optional<FileReference>& sqlOutputData,
                                 const boost::optional<runmanager::Job>& topLevelJob,
                                 const std::vector<openstudio::path>& dakotaParametersFiles,
                                 const std::vector<Tag>& tags,
                                 const std::vector<Attribute>& outputAttributes)
    : AnalysisObject_Impl(uuid,versionUUID,name,displayName,description),
      m_problemUUID(problemUUID),
      m_analysisUUID(analysisUUID),
      m_complete(complete),
      m_failed(failed),
      m_selected(selected),
      m_runType(runType),
      m_variableValues(variableValues),
      m_responseValues(responseValues),
      m_directory(directory),
      m_osmInputData(osmInputData),
      m_idfInputData(idfInputData),
      m_sqlOutputData(sqlOutputData),
      m_topLevelJob(topLevelJob),
      m_dakotaParametersFiles(dakotaParametersFiles),
      m_tags(tags),
      m_outputAttributes(outputAttributes)
  {}

  DataPoint_Impl::DataPoint_Impl(const DataPoint_Impl &other)
    : AnalysisObject_Impl(other),
      m_problem(other.m_problem),
      m_problemUUID(other.problemUUID()),
      m_analysisUUID(other.analysisUUID()),
      m_complete(other.isComplete()),
      m_failed(other.failed()),
      m_selected(other.selected()),
      m_runType(other.runType()),
      m_variableValues(other.variableValues()),
      m_responseValues(other.responseValues()),
      m_directory(other.directory()),
      m_topLevelJob(other.topLevelJob())
  {
    if (other.osmInputData()) {
      m_osmInputData = other.osmInputData().get().clone();
    }
    if (other.idfInputData()) {
      m_idfInputData = other.idfInputData().get().clone();
    }
    if (other.sqlOutputData()) {
      m_sqlOutputData = other.sqlOutputData().get().clone();
    }
    for (const FileReference& fref : other.xmlOutputData()) {
      m_xmlOutputData.push_back(fref.clone());
    }
    for (const Tag& tag : other.tags()) {
      m_tags.push_back(tag.clone());
    }
    for (const Attribute& attribute : other.outputAttributes()) {
      m_outputAttributes.push_back(attribute.clone());
    }
    // DLM: TODO should we clone topLevelJob? for now do nothing.
    // ETH: Need to clone topLevelJob for save as-ing SimpleProject.
    //      Am trying just pulling the same runmanager::Job over. When it 
    //      gets to osp, should just keep uuid.
    // DLM: TODO should we clone dakotaParametersFiles? for now do nothing.
  }

  AnalysisObject DataPoint_Impl::clone() const {
    std::shared_ptr<DataPoint_Impl> impl(new DataPoint_Impl(*this));
    return DataPoint(impl);
  }

  bool DataPoint_Impl::hasProblem() const {
    return m_problem;
  }

  Problem DataPoint_Impl::problem() const {
    if (!hasProblem()) {
      LOG_AND_THROW("This DataPoint does not have access to its Problem at this time.");
    }
    return m_problem.get();
  }

  UUID DataPoint_Impl::problemUUID() const {
    if (hasProblem()) {
      OS_ASSERT(m_problemUUID == problem().uuid());
    }
    OS_ASSERT(!m_problemUUID.isNull());
    return m_problemUUID;
  }

  boost::optional<UUID> DataPoint_Impl::analysisUUID() const {
    if (!m_analysisUUID) {
      if (OptionalAnalysisObject analysis = parent()) {
        OS_ASSERT(analysis->optionalCast<Analysis>());
        m_analysisUUID = analysis->uuid();
      }
    }
    else if (OptionalAnalysisObject analysis = parent()) {
      OS_ASSERT(analysis->optionalCast<Analysis>());
      OS_ASSERT(m_analysisUUID.get() == analysis->uuid());
    }
    return m_analysisUUID;
  }

  bool DataPoint_Impl::isComplete() const {
    return complete();
  }

  bool DataPoint_Impl::complete() const {
    return m_complete;
  }

  bool DataPoint_Impl::failed() const {
    return m_failed;
  }

  bool DataPoint_Impl::selected() const {
    return m_selected;
  }

  DataPointRunType DataPoint_Impl::runType() const {
    return m_runType;
  }

  std::vector<QVariant> DataPoint_Impl::variableValues() const {
    return m_variableValues;
  }

  std::vector<double> DataPoint_Impl::responseValues() const {
    return m_responseValues;
  }

  openstudio::path DataPoint_Impl::directory() const {
    return m_directory;
  }

  boost::optional<FileReference> DataPoint_Impl::osmInputData() const {
    return m_osmInputData;
  }

  boost::optional<FileReference> DataPoint_Impl::idfInputData() const {
    return m_idfInputData;
  }

  boost::optional<FileReference> DataPoint_Impl::sqlOutputData() const {
    return m_sqlOutputData;
  }

  std::vector<FileReference> DataPoint_Impl::xmlOutputData() const {
    // no longer serializing this. gather and cache this data as needed.
    if (m_xmlOutputData.empty() && topLevelJob()) {
      runmanager::Files allFiles = topLevelJob()->treeAllFiles();
      FileReferenceVector xmlOutputData;
      try {
        for (const runmanager::FileInfo& file : allFiles.getAllByExtension("ossr").files()) {
          xmlOutputData.push_back(FileReference(file.fullPath));
        }
      }
      catch (...) {}
      try {
        for (const runmanager::FileInfo& file : allFiles.getAllByExtension("xml").files()) {
          xmlOutputData.push_back(FileReference(file.fullPath));
        }
      }
      catch (...) {}
      m_xmlOutputData = xmlOutputData;
    }
    return m_xmlOutputData;
  }

  boost::optional<runmanager::Job> DataPoint_Impl::topLevelJob() const
  {
    return m_topLevelJob;
  }

  std::vector<openstudio::path> DataPoint_Impl::dakotaParametersFiles() const
  {
    return m_dakotaParametersFiles;
  }

  boost::optional<model::Model> DataPoint_Impl::model() const {
    if (m_model) {
      return m_model;
    }
    if (m_osmInputData) {
      m_model = model::Model::load(m_osmInputData->path());
      if (m_model) {
        OptionalSqlFile oSqlFile = sqlFile();
        if (oSqlFile) {
          m_model->setSqlFile(*oSqlFile);
        }
      }
      return m_model;
    }
    LOG(Info,"This DataPoint cannot return a Model because it has no OSM input data.");
    return boost::none;
  }

  boost::optional<Workspace> DataPoint_Impl::workspace() const {
    if (m_workspace) {
      return m_workspace;
    }
    if (m_idfInputData) {
      m_workspace = Workspace::load(m_idfInputData->path());
      return m_workspace;
    }
    LOG(Info,"This DataPoint cannot return a Workspace because it has no IDF input data.");
    return boost::none;
  }

  boost::optional<SqlFile> DataPoint_Impl::sqlFile() const {
    if (m_sqlFile) { return m_sqlFile; }
    if (m_sqlOutputData) {
      try {
        m_sqlFile = SqlFile(m_sqlOutputData->path());
      }
      catch (...) {}
      return m_sqlFile;
    }
    LOG(Info,"No SqlFile is set for this DataPoint.");
    return boost::none;
  }

  std::vector<Attribute> DataPoint_Impl::outputAttributes() const {
    if (m_outputAttributes.empty()) {
      for (const FileReference& xmlFile : xmlOutputData()) {
        AttributeVector toAdd;
        if (xmlFile.fileType() == FileReferenceType::XML) {
          OptionalAttribute wrapperAttribute = Attribute::loadFromXml(xmlFile.path());
          if (wrapperAttribute && (wrapperAttribute->valueType() == AttributeValueType::AttributeVector))
          {
            toAdd = wrapperAttribute->valueAsAttributeVector();
            m_outputAttributes.insert(m_outputAttributes.end(),toAdd.begin(),toAdd.end());
          }
          else {
            LOG(Warn,"Unable to load attribute xml from " << toString(xmlFile.path()));
          }
        }
        else if (xmlFile.fileType() == FileReferenceType::OSSR) {
          ruleset::OptionalOSResult result = ruleset::OSResult::load(xmlFile.path());
          if (result) {
            toAdd = result->attributes();
            m_outputAttributes.insert(m_outputAttributes.end(),toAdd.begin(),toAdd.end());
          }
          else {
            LOG(Warn,"Unable to load result xml from " << toString(xmlFile.path()));
          }
        }
      }
    }
    return m_outputAttributes;
  }

  boost::optional<Attribute> DataPoint_Impl::getOutputAttribute(
      const std::string& attributeName) const
  {
    AttributeVector myAttributes = outputAttributes();
    for (const Attribute& attribute : myAttributes) {
      if (istringEqual(attribute.name(),attributeName)) {
        return attribute;
      }
    }
    return boost::none;
  }

  std::vector<Tag> DataPoint_Impl::tags() const {
    return m_tags;
  }

  bool DataPoint_Impl::isTag(const std::string& tagName) const {
    NameFinder<Tag> finder(tagName);
    auto it = std::find_if(m_tags.begin(),m_tags.end(),finder);
    return (it != m_tags.end());
  }

  bool DataPoint_Impl::matches(const std::vector<QVariant>& testVariableValues) const {
    std::vector<QVariant> myValues = variableValues();
    unsigned n = myValues.size();
    unsigned m = testVariableValues.size();
    if (m > n) {
      return false;
    }
    for (unsigned i = 0; i < n && i < m; ++i) {
      if (testVariableValues[i].isNull()) {
        continue;
      }
      QVariant mine = myValues[i];
      QVariant theirs = testVariableValues[i];
      if ((mine.type() == QVariant::Int) || (mine.type() == QVariant::UInt)) {
        // discrete variable
        if (!((theirs.type() == QVariant::Int) || (theirs.type() == QVariant::UInt))) {
          return false;
        }
        if (mine.toInt() != theirs.toInt()) {
          return false;
        }
      }
      else {
        // continuous variable
        OS_ASSERT(mine.type() == QVariant::Double);
        if (theirs.type() != QVariant::Double) {
          return false;
        }
        if (!equal(mine.toDouble(),theirs.toDouble())) {
          return false;
        }
      }
    }
    return true;
  }

  void DataPoint_Impl::setSelected(bool selected) {
    m_selected = selected;
    onChange(AnalysisObject_Impl::Benign);
  }

  void DataPoint_Impl::setRunType(const DataPointRunType& runType) {
    m_runType = runType;
    onChange(AnalysisObject_Impl::Benign);
  }

  void DataPoint_Impl::setDirectory(const openstudio::path& directory) {
    m_directory = directory;
    onChange(AnalysisObject_Impl::Benign);
  }

  void DataPoint_Impl::setTopLevelJob(const runmanager::Job& topLevelJob)
  {
    m_topLevelJob = topLevelJob;
    onChange(AnalysisObject_Impl::Benign);
  }

  void DataPoint_Impl::setDakotaParametersFiles(const std::vector<openstudio::path>& dakotaParametersFiles)
  {
    m_dakotaParametersFiles = dakotaParametersFiles;
    onChange(AnalysisObject_Impl::Benign);
  }

  void DataPoint_Impl::addTag(const std::string& tagName) {
    m_tags.push_back(Tag(tagName));
    onChange(AnalysisObject_Impl::Benign);
  }

  void DataPoint_Impl::deleteTag(const std::string& tagName) {
    NameFinder<Tag> finder(tagName);
    auto it = std::find_if(m_tags.begin(),m_tags.end(),finder);
    if (it != m_tags.end()) {
      m_tags.erase(it);
      onChange(AnalysisObject_Impl::Benign);
    }
  }

  bool DataPoint_Impl::updateFromJSON(const std::string& json, boost::optional<runmanager::RunManager>& runManager) {

    if (complete() || !directory().empty()) {
      LOG(Info,"Cannot update this DataPoint from JSON because it appears to have results already. "
          << "Clear the old results before importing new ones.");
      return false;
    }

    return this->updateFromJSON(loadJSON(json),runManager);
  }

  bool DataPoint_Impl::updateFromJSON(const AnalysisJSONLoadResult& loadResult, 
                                      boost::optional<runmanager::RunManager>& runManager) 
  {
    if (loadResult.analysisObject) {
      if (OptionalDataPoint loaded = loadResult.analysisObject->optionalCast<DataPoint>()) {
        if (loaded->uuid() == uuid()) {
          // require the variableValues to be the same
          if ((variableValues() != loaded->variableValues())) {
            LOG(Warn,"Cannot update DataPoint with a JSON version that has different variable values.");
            return false;
          }
          m_complete = loaded->complete();
          m_failed = loaded->failed();
          m_responseValues = loaded->responseValues();

          // do not pull file references over since they are not generally available for loading
          // do pull job data over because it contains errors and warnings

          boost::optional<runmanager::Job> loadedTopLevelJob = loaded->topLevelJob();
          OS_ASSERT(loadedTopLevelJob);
          if (runManager) {
            if (m_topLevelJob){
              runManager->updateJob(m_topLevelJob->uuid(), *loadedTopLevelJob);
            }else{
              runManager->updateJob(*loadedTopLevelJob);
              m_topLevelJob = runManager->getJob(loadedTopLevelJob->uuid());
            }
            LOG(Info, "Datapoint '" << toString(this->uuid()) << "' topLevelJob = '" << toString(m_topLevelJob->uuid()) << "'");
          }

          m_tags = loaded->tags();
          m_outputAttributes = loaded->outputAttributes();
          onChange(AnalysisObject_Impl::Benign);
          return true;
        }
        else {
          LOG(Warn,"Cannot update DataPoint because the DataPoint loaded from JSON has a different UUID.");
        }
      }
      else {
        LOG(Warn,"Cannot update DataPoint because the AnalysisObject loaded from JSON is not a DataPoint.");
      }
    }
    else {
      LOG(Warn,"Cannot update DataPoint from JSON because the JSON string could not be loaded.");
    }

    return false;
  }

  bool DataPoint_Impl::updateDetails(boost::optional<runmanager::RunManager>& runManager) {
    if (directory().empty()) {
      LOG(Info,"No directory set for this DataPoint.");
      return false;
    }

    openstudio::path zipPath = directory() / toPath("dataPoint.zip");
    if (!boost::filesystem::exists(zipPath)) {
      LOG(Info,"No dataPoint.zip file in directory '" << toString(directory()) << "'.");
      return false;
    }

    try{
      // unzip 
      UnzipFile unzip(zipPath);
      unzip.extractAllFiles(directory());
    }catch(const std::exception&){
      LOG(Info,"Could not unzip dataPoint.zip file in directory '" << toString(directory()) << "'.");
      return false;
    }

    boost::filesystem::remove(zipPath);

    // fix up topLevelJob
    OS_ASSERT(m_topLevelJob);
    if (runManager) {
      // files are now in directory(), need to update paths
      runManager->updateJob(*m_topLevelJob, directory());
    }

    // get file references for
    //   m_osmInputData
    //   m_idfInputData
    //   m_sqlOutputData
    //   m_xmlOutputData (vector)
    // if had json now, could transform those, but use topLevelJob instead,
    // since not preserving the json file references makes it clear that 
    // there is no model, idf, sql without downloading details.
    runmanager::Files allFiles = topLevelJob()->treeAllFiles();
    try {
      openstudio::path osmInputDataPath = allFiles.getLastByExtension("osm").fullPath;
      if (!boost::filesystem::exists(osmInputDataPath)) {
        // check to see if this was the seed model
        if (toString(osmInputDataPath.parent_path().stem()) == "seed") {
          LOG(Debug,"Last OSM is the seed model. Point this local DataPoint to the local "
              << "seed model.");
          OS_ASSERT(parent());
          osmInputDataPath = parent()->cast<Analysis>().seed().path();
        }
        else {
          LOG(Debug,"After unzipping the DataPoint's details and updating its topLevelJob "
              << "with the directory information, RunManager is reporting '" 
              << toString(osmInputDataPath) << "' as the last OSM file path, even though "
              << "that location does not exist.");
        }
      }
      setOsmInputData(FileReference(osmInputDataPath));
    }
    catch (...) {}
    try {
      openstudio::path idfInputDataPath = allFiles.getLastByExtension("idf").fullPath;
      if (!boost::filesystem::exists(idfInputDataPath)) {
                // check to see if this was the seed model
        if (toString(idfInputDataPath.parent_path().stem()) == "seed") {
          LOG(Debug,"Last IDF is the seed model. Point this local DataPoint to the local "
              << "seed model.");
          OS_ASSERT(parent());
          idfInputDataPath = parent()->cast<Analysis>().seed().path();
        }
        else {
          LOG(Debug,"After unzipping the DataPoint's details and updating its topLevelJob "
              << "with the directory information, RunManager is reporting '" 
              << toString(idfInputDataPath) << "' as the last IDF file path, even though "
              << "that location does not exist.");
        }
      }
      setIdfInputData(FileReference(idfInputDataPath));
    }
    catch (...) {}
    try {
      openstudio::path sqlOutputDataPath = allFiles.getLastByExtension("sql").fullPath;
      if (!boost::filesystem::exists(sqlOutputDataPath)) {
        LOG(Debug,"After unzipping the DataPoint's details and updating its topLevelJob "
            << "with the directory information, RunManager is reporting '" 
            << toString(sqlOutputDataPath) << "' as the last SQL file path, even though "
            << "that location does not exist.");
      }
      setSqlOutputData(FileReference(sqlOutputDataPath));
    }
    catch (...) {}
    // lazy load xml file references

    onChange(AnalysisObject_Impl::Benign);

    return true;
  }

  void DataPoint_Impl::clearFileDataFromCache() const {
    m_model = boost::none;
    m_workspace = boost::none;
    m_sqlFile = boost::none;
    m_xmlOutputData.clear();
  }

  void DataPoint_Impl::clearAllDataFromCache() const {
    clearFileDataFromCache();
    m_outputAttributes.clear();
  }

  void DataPoint_Impl::clearResults() {
    m_complete = false;
    m_failed = false;
    m_responseValues.clear();
    m_directory.clear();
    m_osmInputData.reset();
    m_idfInputData.reset();
    m_sqlOutputData.reset();
    m_topLevelJob.reset(); // this should happen here because if results are cleared want to run with a new job
    m_dakotaParametersFiles.clear(); // DLM: should this really happen here?
    clearAllDataFromCache();
    onChange(AnalysisObject_Impl::Benign);
  }

  bool DataPoint_Impl::saveJSON(const openstudio::path& p,
                                bool overwrite) const
  {
    QVariant json = toTopLevelVariant();
    return openstudio::saveJSON(json,p,overwrite);
  }

  std::ostream& DataPoint_Impl::toJSON(std::ostream& os) const
  {
    os << toJSON();
    return os;
  }

  std::string DataPoint_Impl::toJSON() const {
    QVariant json = toTopLevelVariant();
    return openstudio::toJSON(json);
  }

  void DataPoint_Impl::setOsmInputData(const FileReference& file) {
    OS_ASSERT(file.fileType() == FileReferenceType::OSM);
    m_osmInputData = file;
    m_model = boost::none;
    onChange(AnalysisObject_Impl::Benign);
  }

  void DataPoint_Impl::setIdfInputData(const FileReference& file) {
    OS_ASSERT(file.fileType() == FileReferenceType::IDF);
    m_idfInputData = file;
    m_workspace = boost::none;
    onChange(AnalysisObject_Impl::Benign);
  }

  void DataPoint_Impl::setSqlOutputData(const FileReference& file) {
    OS_ASSERT(file.fileType() == FileReferenceType::SQL);
    m_sqlOutputData = file;
    m_model = boost::none;
    m_sqlFile = boost::none;
    onChange(AnalysisObject_Impl::Benign);
  }

  void DataPoint_Impl::setXmlOutputData(const std::vector<FileReference>& files) {
    m_xmlOutputData = files;
    m_outputAttributes.clear();
    onChange(AnalysisObject_Impl::Benign);
  }

  void DataPoint_Impl::markComplete() {
    m_complete = true;
    onChange(AnalysisObject_Impl::Benign);
  }

  void DataPoint_Impl::markFailed() {
    m_failed = true;
    onChange(AnalysisObject_Impl::Benign);
  }

  void DataPoint_Impl::setResponseValues(const std::vector<double> values) {
    m_responseValues = values;
    onChange(AnalysisObject_Impl::Benign);
  }

  void DataPoint_Impl::setProblem(const Problem& problem) {
    m_problem = problem;
    m_problemUUID = problem.uuid();
  }

  QVariant DataPoint_Impl::toVariant() const {
    QVariantMap dataPointData = AnalysisObject_Impl::toVariant().toMap();

    dataPointData["data_point_type"] = QString("DataPoint");
    dataPointData["problem_uuid"] = toQString(removeBraces(problemUUID()));
    if (analysisUUID()) {
      dataPointData["analysis_uuid"] = toQString(removeBraces(analysisUUID().get()));
    }

    dataPointData["complete"] = isComplete();
    dataPointData["failed"] = failed();
    dataPointData["selected"] = selected();
    dataPointData["run_type"] = toQString(runType().valueName());

    QVariantList variableValuesList;
    int index(0);
    for (const QVariant& value : variableValues()) {
      QVariantMap valueMap;
      valueMap["variable_value_index"] = QVariant(index);
      valueMap["value_type"] = value.typeName();
      valueMap["value"] = value;
      variableValuesList.push_back(QVariant(valueMap));
      ++index;
    }
    dataPointData["variable_values"] = variableValuesList;

    if (!responseValues().empty()) {
      QVariantList responseValuesList;
      index = 0;
      for (double value : responseValues()) {
        QVariantMap responseMap;
        responseMap["response_value_index"] = QVariant(index);
        responseMap["value"] = QVariant(value);
        responseValuesList.push_back(responseMap);
        ++index;
      }
      dataPointData["response_values"] = responseValuesList;
    }

    if (!directory().empty()) {
      dataPointData["directory"] = toQString(directory());
    }

    if (osmInputData()) {
      dataPointData["osm_input_data"] = openstudio::detail::toVariant(osmInputData().get());
    }
    if (idfInputData()) {
      dataPointData["idf_input_data"] = openstudio::detail::toVariant(idfInputData().get());
    }
    if (sqlOutputData()) {
      dataPointData["sql_output_data"] = openstudio::detail::toVariant(sqlOutputData().get());
    }

    if (topLevelJob()) {
      dataPointData["top_level_job"] = runmanager::detail::JSON::toVariant(*topLevelJob());
    }

    if (!dakotaParametersFiles().empty()) {
      QVariantList dakotaParametersFilesList;
      for (const openstudio::path& p : dakotaParametersFiles()) {
        dakotaParametersFilesList.push_back(toQString(p));
      }
      dataPointData["dakota_parameters_files"] = QVariant(dakotaParametersFilesList);
    }

    if (!tags().empty()) {
      QVariantList tagsList;
      for (const Tag& tag : tags()) {
        tagsList.push_back(openstudio::detail::toVariant(tag));
      }
      dataPointData["tags"] = QVariant(tagsList);
    }

    if (!outputAttributes().empty()) {
      QVariantList outputAttributesList;
      for (const Attribute& attribute : outputAttributes()) {
        outputAttributesList.push_back(openstudio::detail::toVariant(attribute));
      }
      dataPointData["output_attributes"] = QVariant(outputAttributesList);
    }

    return QVariant(dataPointData);
  }

  QVariant DataPoint_Impl::toTopLevelVariant() const {
    QVariantMap dataPointData = this->toVariant().toMap();

    // create top-level of final file
    QVariantMap result = jsonMetadata().toMap(); // openstudio_version
    result["data_point"] = dataPointData;

    return result;
  }

  DataPoint DataPoint_Impl::factoryFromVariant(const QVariant& variant,
                                               const VersionString& version,
                                               const boost::optional<Problem>& problem)
  {
    QVariantMap map = variant.toMap();

    if (!map.contains("data_point_type")) {
      LOG_AND_THROW("Unable to find DataPoint in expected location.");
    }

    OptionalDataPoint result;
    std::string dataPointType = map["data_point_type"].toString().toStdString();
    if (dataPointType == "DataPoint") {
      result = DataPoint_Impl::fromVariant(variant,version);
    }
    else if (dataPointType == "OptimizationDataPoint") {
      result = OptimizationDataPoint_Impl::fromVariant(variant,version);
    }
    else {
      LOG_AND_THROW("Unexpected data_point_type " << dataPointType << ".");
    }

    OS_ASSERT(result);
    if (problem) {
      if (result->problemUUID() != problem->uuid()) {
        LOG_AND_THROW("Problem UUID stored for this DataPoint, " << toString(result->problemUUID())
                      << " does not match the UUID of Problem " << problem->name() << ", which is "
                      << toString(problem->uuid()) << ".");
      }
      result->setProblem(*problem);
    }

    return result.get();
  }

  DataPoint DataPoint_Impl::fromVariant(const QVariant& variant, const VersionString& version) {
    QVariantMap map = variant.toMap();

    // variable values
    QVariantList variableValuesList = map["variable_values"].toList();
    unsigned n = variableValuesList.size();
    std::vector<QVariant> variableValues(n,QVariant());
    for (const QVariant& variableValue : variableValuesList) {
      QVariantMap vvMap = variableValue.toMap();
      QVariant val;
      if (vvMap["value_type"] == QVariant(double(0.0)).typeName()) {
        val = vvMap["value"].toDouble();
      }
      else if (vvMap["value_type"] == QVariant(int(0)).typeName()) {
        val = vvMap["value"].toInt();
      }
      else {
        LOG_AND_THROW("Unexpected value_type " << toString(vvMap["value_type"].toString())
                      << " in data_point's variable_values.");
      }
      int index = vvMap["variable_value_index"].toInt();
      if (index >= int(n)) {
        LOG_AND_THROW("Unexpected variable_value_index " << index << " in variable_values "
                      << "list of length " << n << ".");
      }
      variableValues[index] = val;
    }

    // response values
    DoubleVector responseValues;
    if (map.contains("response_values")) {
      bool ok(false);
      responseValues = deserializeOrderedVector<double>(
            map["response_values"].toList(),
            "value",
            "response_value_index",
            std::function<double (QVariant*)>(std::bind(&QVariant::toDouble,std::placeholders::_1,&ok)));
    }

    // tags
    TagVector tags;
    if (map.contains("tags")) {
      tags = deserializeUnorderedVector<Tag>(
            map["tags"].toList(),
            std::function<Tag (const QVariant&)>(std::bind(openstudio::detail::toTag,std::placeholders::_1,version)));
    }

    // output attributes
    AttributeVector outputAttributes;
    if (map.contains("output_attributes")) {
      outputAttributes = deserializeUnorderedVector<Attribute>(
            map["output_attributes"].toList(),
            std::function<Attribute (const QVariant&)>(std::bind(openstudio::detail::toAttribute,std::placeholders::_1,version)));
    }

    // drop xml_output_data

    // dakota parameters files
    std::vector<openstudio::path> dakotaParametersFiles;
    if (map.contains("dakota_parameters_files")) {
      openstudio::path (*fToPath)(const QString&) = openstudio::toPath;
      dakotaParametersFiles = deserializeUnorderedVector<openstudio::path>(
            map["dakota_parameters_files"].toList(),
            std::function<openstudio::path (QVariant*)>(std::bind(fToPath,std::bind(&QVariant::toString,std::placeholders::_1))));
    }

    return DataPoint(toUUID(map["uuid"].toString().toStdString()),
                     toUUID(map["version_uuid"].toString().toStdString()),
                     map.contains("name") ? map["name"].toString().toStdString() : std::string(),
                     map.contains("display_name") ? map["display_name"].toString().toStdString() : std::string(),
                     map.contains("description") ? map["description"].toString().toStdString() : std::string(),
                     toUUID(map["problem_uuid"].toString().toStdString()),
                     map.contains("analysis_uuid") ? toUUID(map["analysis_uuid"].toString().toStdString()) : boost::optional<openstudio::UUID>(),
                     map["complete"].toBool(),
                     map["failed"].toBool(),
                     version < VersionString("1.0.4") ? true : map["selected"].toBool(),
                     version < VersionString("1.0.5") ? DataPointRunType(DataPointRunType::Local) : DataPointRunType(map["run_type"].toString().toStdString()),
                     variableValues,
                     responseValues,
                     map.contains("directory") ? toPath(map["directory"].toString()) : openstudio::path(),
                     map.contains("osm_input_data") ? openstudio::detail::toFileReference(map["osm_input_data"],version) : OptionalFileReference(),
                     map.contains("idf_input_data") ? openstudio::detail::toFileReference(map["idf_input_data"],version) : OptionalFileReference(),
                     map.contains("sql_output_data") ? openstudio::detail::toFileReference(map["sql_output_data"],version) : OptionalFileReference(),
                     map.contains("top_level_job") ? runmanager::detail::JSON::toJob(map["top_level_job"],version, true) : boost::optional<runmanager::Job>(),
                     dakotaParametersFiles,
                     tags,
                     outputAttributes);
  }

} // detail

DataPoint::DataPoint(const Problem& problem,
                     const std::vector<QVariant>& variableValues)
  : AnalysisObject(std::shared_ptr<detail::DataPoint_Impl>(
        new detail::DataPoint_Impl(problem,variableValues)))
{}

DataPoint::DataPoint(const UUID& uuid,
                     const UUID& versionUUID,
                     const std::string& name,
                     const std::string& displayName,
                     const std::string& description,
                     const Problem& problem,
                     bool complete,
                     bool failed,
                     bool selected,
                     DataPointRunType runType,
                     const std::vector<QVariant>& variableValues,
                     const std::vector<double>& responseValues,
                     const openstudio::path& directory,
                     const boost::optional<FileReference>& osmInputData,
                     const boost::optional<FileReference>& idfInputData,
                     const boost::optional<FileReference>& sqlOutputData,
                     const boost::optional<runmanager::Job>& topLevelJob,
                     const std::vector<openstudio::path>& dakotaParametersFiles,
                     const std::vector<Tag>& tags,
                     const std::vector<Attribute>& outputAttributes)
  : AnalysisObject(std::shared_ptr<detail::DataPoint_Impl>(
        new detail::DataPoint_Impl(uuid,
                                   versionUUID,
                                   name,
                                   displayName,
                                   description,
                                   problem,
                                   complete,
                                   failed,
                                   selected,
                                   runType,
                                   variableValues,
                                   responseValues,
                                   directory,
                                   osmInputData,
                                   idfInputData,
                                   sqlOutputData,
                                   topLevelJob,
                                   dakotaParametersFiles,
                                   tags,
                                   outputAttributes)))
{}

DataPoint::DataPoint(const UUID& uuid,
                     const UUID& versionUUID,
                     const std::string& name,
                     const std::string& displayName,
                     const std::string& description,
                     const UUID& problemUUID,
                     const boost::optional<UUID>& analysisUUID,
                     bool complete,
                     bool failed,
                     bool selected,
                     DataPointRunType runType,
                     const std::vector<QVariant>& variableValues,
                     const std::vector<double>& responseValues,
                     const openstudio::path& directory,
                     const boost::optional<FileReference>& osmInputData,
                     const boost::optional<FileReference>& idfInputData,
                     const boost::optional<FileReference>& sqlOutputData,
                     const boost::optional<runmanager::Job>& topLevelJob,
                     const std::vector<openstudio::path>& dakotaParametersFiles,
                     const std::vector<Tag>& tags,
                     const std::vector<Attribute>& outputAttributes)
  : AnalysisObject(std::shared_ptr<detail::DataPoint_Impl>(
        new detail::DataPoint_Impl(uuid,
                                   versionUUID,
                                   name,
                                   displayName,
                                   description,
                                   problemUUID,
                                   analysisUUID,
                                   complete,
                                   failed,
                                   selected,
                                   runType,
                                   variableValues,
                                   responseValues,
                                   directory,
                                   osmInputData,
                                   idfInputData,
                                   sqlOutputData,
                                   topLevelJob,
                                   dakotaParametersFiles,
                                   tags,
                                   outputAttributes)))
{}

bool DataPoint::hasProblem() const {
  return getImpl<detail::DataPoint_Impl>()->hasProblem();
}

Problem DataPoint::problem() const {
  return getImpl<detail::DataPoint_Impl>()->problem();
}

UUID DataPoint::problemUUID() const {
  return getImpl<detail::DataPoint_Impl>()->problemUUID();
}

boost::optional<UUID> DataPoint::analysisUUID() const {
  return getImpl<detail::DataPoint_Impl>()->analysisUUID();
}

bool DataPoint::isComplete() const {
  return getImpl<detail::DataPoint_Impl>()->isComplete();
}

bool DataPoint::complete() const {
  return getImpl<detail::DataPoint_Impl>()->complete();
}

bool DataPoint::failed() const {
  return getImpl<detail::DataPoint_Impl>()->failed();
}

bool DataPoint::selected() const {
  return getImpl<detail::DataPoint_Impl>()->selected();
}

DataPointRunType DataPoint::runType() const {
  return getImpl<detail::DataPoint_Impl>()->runType();
}

std::vector<QVariant> DataPoint::variableValues() const {
  return getImpl<detail::DataPoint_Impl>()->variableValues();
}

std::vector<double> DataPoint::responseValues() const {
  return getImpl<detail::DataPoint_Impl>()->responseValues();
}

openstudio::path DataPoint::directory() const {
  return getImpl<detail::DataPoint_Impl>()->directory();
}

boost::optional<FileReference> DataPoint::osmInputData() const {
  return getImpl<detail::DataPoint_Impl>()->osmInputData();
}

boost::optional<FileReference> DataPoint::idfInputData() const {
  return getImpl<detail::DataPoint_Impl>()->idfInputData();
}

boost::optional<FileReference> DataPoint::sqlOutputData() const {
  return getImpl<detail::DataPoint_Impl>()->sqlOutputData();
}

std::vector<FileReference> DataPoint::xmlOutputData() const {
  return getImpl<detail::DataPoint_Impl>()->xmlOutputData();
}

boost::optional<runmanager::Job> DataPoint::topLevelJob() const
{
  return getImpl<detail::DataPoint_Impl>()->topLevelJob();
}

std::vector<openstudio::path> DataPoint::dakotaParametersFiles() const
{
  return getImpl<detail::DataPoint_Impl>()->dakotaParametersFiles();
}

boost::optional<model::Model> DataPoint::model() const {
  return getImpl<detail::DataPoint_Impl>()->model();
}

boost::optional<Workspace> DataPoint::workspace() const {
  return getImpl<detail::DataPoint_Impl>()->workspace();
}

boost::optional<SqlFile> DataPoint::sqlFile() const {
  return getImpl<detail::DataPoint_Impl>()->sqlFile();
}

std::vector<Attribute> DataPoint::outputAttributes() const {
  return getImpl<detail::DataPoint_Impl>()->outputAttributes();
}

boost::optional<Attribute> DataPoint::getOutputAttribute(const std::string& attributeName) const {
  return getImpl<detail::DataPoint_Impl>()->getOutputAttribute(attributeName);
}

std::vector<Tag> DataPoint::tags() const {
  return getImpl<detail::DataPoint_Impl>()->tags();
}

bool DataPoint::isTag(const std::string& tagName) const {
  return getImpl<detail::DataPoint_Impl>()->isTag(tagName);
}

bool DataPoint::matches(const std::vector<QVariant>& testVariableValues) const {
  return getImpl<detail::DataPoint_Impl>()->matches(testVariableValues);
}

void DataPoint::setSelected(bool selected) {
  getImpl<detail::DataPoint_Impl>()->setSelected(selected);
}

void DataPoint::setRunType(const DataPointRunType& runType) {
  getImpl<detail::DataPoint_Impl>()->setRunType(runType);
}

void DataPoint::setDirectory(const openstudio::path& directory) {
  getImpl<detail::DataPoint_Impl>()->setDirectory(directory);
}

void DataPoint::addTag(const std::string& tagName) {
  getImpl<detail::DataPoint_Impl>()->addTag(tagName);
}

void DataPoint::deleteTag(const std::string& tagName) {
  getImpl<detail::DataPoint_Impl>()->deleteTag(tagName);
}

bool DataPoint::updateFromJSON(const std::string& json, boost::optional<runmanager::RunManager>& runManager) {
  return getImpl<detail::DataPoint_Impl>()->updateFromJSON(json,runManager);
}

bool DataPoint::updateDetails(boost::optional<runmanager::RunManager>& runManager) {
  return getImpl<detail::DataPoint_Impl>()->updateDetails(runManager);
}

void DataPoint::clearFileDataFromCache() const {
  getImpl<detail::DataPoint_Impl>()->clearFileDataFromCache();
}

void DataPoint::clearAllDataFromCache() const {
  getImpl<detail::DataPoint_Impl>()->clearAllDataFromCache();
}

void DataPoint::clearResults() {
  getImpl<detail::DataPoint_Impl>()->clearResults();
}

bool DataPoint::saveJSON(const openstudio::path& p,
                         bool overwrite) const
{
  return getImpl<detail::DataPoint_Impl>()->saveJSON(p,overwrite);
}

std::ostream& DataPoint::toJSON(std::ostream& os) const
{
  return getImpl<detail::DataPoint_Impl>()->toJSON(os);
}

std::string DataPoint::toJSON() const {
  return getImpl<detail::DataPoint_Impl>()->toJSON();
}

boost::optional<DataPoint> DataPoint::loadJSON(const openstudio::path& p)
{
  OptionalDataPoint result;
  AnalysisJSONLoadResult loadResult = analysis::loadJSON(p);
  if (loadResult.analysisObject && loadResult.analysisObject->optionalCast<DataPoint>()) {
    result = loadResult.analysisObject->cast<DataPoint>();
  }
  return result;
}

boost::optional<DataPoint> DataPoint::loadJSON(std::istream& json)
{
  OptionalDataPoint result;
  AnalysisJSONLoadResult loadResult = analysis::loadJSON(json);
  if (loadResult.analysisObject && loadResult.analysisObject->optionalCast<DataPoint>()) {
    result = loadResult.analysisObject->cast<DataPoint>();
  }
  return result;
}

boost::optional<DataPoint> DataPoint::loadJSON(const std::string& json)
{
  OptionalDataPoint result;
  AnalysisJSONLoadResult loadResult = analysis::loadJSON(json);
  if (loadResult.analysisObject && loadResult.analysisObject->optionalCast<DataPoint>()) {
    result = loadResult.analysisObject->cast<DataPoint>();
  }
  return result;
}

/// @cond
DataPoint::DataPoint(std::shared_ptr<detail::DataPoint_Impl> impl)
  : AnalysisObject(impl)
{}
/// @endcond

void DataPoint::setOsmInputData(const FileReference& file) {
  getImpl<detail::DataPoint_Impl>()->setOsmInputData(file);
}

void DataPoint::setIdfInputData(const FileReference& file) {
  getImpl<detail::DataPoint_Impl>()->setIdfInputData(file);
}

void DataPoint::setSqlOutputData(const FileReference& file) {
  getImpl<detail::DataPoint_Impl>()->setSqlOutputData(file);
}

void DataPoint::setXmlOutputData(const std::vector<FileReference>& files) {
  getImpl<detail::DataPoint_Impl>()->setXmlOutputData(files);
}

void DataPoint::markComplete() {
  getImpl<detail::DataPoint_Impl>()->markComplete();
}

void DataPoint::markFailed() {
  getImpl<detail::DataPoint_Impl>()->markFailed();
}

void DataPoint::setResponseValues(const std::vector<double> values) {
  getImpl<detail::DataPoint_Impl>()->setResponseValues(values);
}

void DataPoint::setProblem(const Problem& problem) {
  getImpl<detail::DataPoint_Impl>()->setProblem(problem);
}

bool saveJSON(const std::vector<DataPoint>& dataPoints,
              const openstudio::path& p,
              bool overwrite)
{
  QVariant json = detail::toTopLevelVariant(dataPoints);
  return openstudio::saveJSON(json,p,overwrite);
}

std::string toJSON(const std::vector<DataPoint>& dataPoints) 
{
  QVariant json = detail::toTopLevelVariant(dataPoints);
  return openstudio::toJSON(json);
}

std::ostream& toJSON(const std::vector<DataPoint>& dataPoints,
                     std::ostream& os)
{
  os << toJSON(dataPoints);
  return os;
}

std::vector<DataPoint> toDataPointVector(const openstudio::path& jsonFilepath) {
  DataPointVector result;
  try {
    QVariant variant = openstudio::loadJSON(jsonFilepath);
    VersionString version = extractOpenStudioVersion(variant);
    QVariantMap map = variant.toMap();
    if (map.contains("data_points")) {
      result = deserializeOrderedVector<DataPoint>(
                   map["data_points"].toList(),
                   "data_point_batch_index",
                   std::function<DataPoint (const QVariant&)>(std::bind(detail::DataPoint_Impl::factoryFromVariant,std::placeholders::_1,version,boost::none)));
    }
    else {
      LOG_FREE(Error,"openstudio.analysis.DataPoint",
               "The file at " << toString(jsonFilepath) << " cannot be parsed as an OpenStudio "
               << "data point vector json file, because it does not contain a top-level "
               << "'data_points' object.");
    }
  }
  catch (std::exception& e) {
    LOG_FREE(Error,"openstudio.analysis.DataPoint",
             "The file at " << toString(jsonFilepath) << " cannot be parsed as an OpenStudio "
             << "data point vector json file, because " << e.what());
  }
  catch (...) {
    LOG_FREE(Error,"openstudio.analysis.DataPoint",
             "The file at " << toString(jsonFilepath) << " cannot be parsed as an OpenStudio "
             << "data point vector json file.");
  }

  return result;
}

std::vector<DataPoint> toDataPointVector(const std::string& json) {
  DataPointVector result;
  try {
    QVariant variant = openstudio::loadJSON(json);
    VersionString version = extractOpenStudioVersion(variant);
    QVariantMap map = variant.toMap();
    if (map.contains("data_points")) {
      result = deserializeOrderedVector<DataPoint>(
                   map["data_points"].toList(),
                   "data_point_batch_index",
                   std::function<DataPoint (const QVariant&)>(std::bind(detail::DataPoint_Impl::factoryFromVariant,std::placeholders::_1,version,boost::none)));
    }
    else {
      LOG_FREE(Error,"openstudio.analysis.DataPoint",
               "The parsed json string does not contain a top-level 'data_points' object.");
    }
  }
  catch (std::exception& e) {
    LOG_FREE(Error,"openstudio.analysis.DataPoint",
             "The json string cannot be parsed as an OpenStudio data point vector json file, because " 
             << e.what());
  }
  catch (...) {
    LOG_FREE(Error,"openstudio.analysis.DataPoint",
             "The following string cannot be parsed as an OpenStudio data point vector json file." 
             << std::endl << std::endl << json);
  }
  return result;
}

std::vector<DataPoint> toDataPointVector(std::istream& json) {
  // istream -> string code from
  // http://stackoverflow.com/questions/2602013/read-whole-ascii-file-into-c-stdstring
  std::string contents;
  json.seekg(0, std::ios::end);
  contents.resize(json.tellg());
  json.seekg(0, std::ios::beg);
  json.read(&contents[0], contents.size());
  return toDataPointVector(contents);
}

namespace detail {
  QVariant toTopLevelVariant(const std::vector<DataPoint>& dataPoints) 
  {
    QVariantList list;
    unsigned i(0);
    for (const DataPoint& dataPoint : dataPoints) {
      QVariantMap dpm = dataPoint.toVariant().toMap();
      dpm["data_point_batch_index"] = i;
      list.push_back(dpm);
      ++i;
    }
    QVariantMap json = jsonMetadata().toMap();
    json["data_points"] = QVariant(list);
    return QVariant(json);
  }
}

} // analysis
} // openstudio

