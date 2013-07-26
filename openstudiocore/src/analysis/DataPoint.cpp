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

#include <analysis/DataPoint.hpp>
#include <analysis/DataPoint_Impl.hpp>

#include <analysis/Analysis.hpp>
#include <analysis/Analysis_Impl.hpp>
#include <analysis/OptimizationDataPoint.hpp>
#include <analysis/OptimizationDataPoint_Impl.hpp>

#include <runmanager/lib/Job.hpp>
#include <runmanager/lib/JSON.hpp>

#include <utilities/data/Attribute.hpp>

#include <utilities/math/FloatCompare.hpp>

#include <utilities/core/FileReference.hpp>
#include <utilities/core/Assert.hpp>
#include <utilities/core/Finder.hpp>
#include <utilities/core/Json.hpp>

#include <boost/foreach.hpp>

namespace openstudio {
namespace analysis {

namespace detail {

  DataPoint_Impl::DataPoint_Impl(const Problem& problem,
                                 const std::vector<QVariant>& variableValues)
    : AnalysisObject_Impl(std::string()),
      m_problem(problem), m_problemUUID(problem.uuid()),
      m_complete(false), m_failed(false), m_variableValues(variableValues)
  {}

  DataPoint_Impl::DataPoint_Impl(const UUID& uuid,
                                 const UUID& versionUUID,
                                 const std::string& name,
                                 const std::string& displayName,
                                 const std::string& description,
                                 const Problem& problem,
                                 bool complete,
                                 bool failed,
                                 const std::vector<QVariant>& variableValues,
                                 const std::vector<double>& responseValues,
                                 const openstudio::path& directory,
                                 const boost::optional<FileReference>& osmInputData,
                                 const boost::optional<FileReference>& idfInputData,
                                 const boost::optional<FileReference>& sqlOutputData,
                                 const boost::optional<FileReference>& xmlOutputData,
                                 const boost::optional<runmanager::Job>& topLevelJob,
                                 const std::vector<openstudio::path>& dakotaParametersFiles,
                                 const std::vector<Tag>& tags,
                                 const std::vector<Attribute>& outputAttributes)
    : AnalysisObject_Impl(uuid,versionUUID,name,displayName,description),
      m_problem(problem),
      m_problemUUID(problem.uuid()),
      m_complete(complete),
      m_failed(failed),
      m_variableValues(variableValues),
      m_responseValues(responseValues),
      m_directory(directory),
      m_osmInputData(osmInputData),
      m_idfInputData(idfInputData),
      m_sqlOutputData(sqlOutputData),
      m_xmlOutputData(xmlOutputData),
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
                                 const std::vector<QVariant>& variableValues,
                                 const std::vector<double>& responseValues,
                                 const openstudio::path& directory,
                                 const boost::optional<FileReference>& osmInputData,
                                 const boost::optional<FileReference>& idfInputData,
                                 const boost::optional<FileReference>& sqlOutputData,
                                 const boost::optional<FileReference>& xmlOutputData,
                                 const boost::optional<runmanager::Job>& topLevelJob,
                                 const std::vector<openstudio::path>& dakotaParametersFiles,
                                 const std::vector<Tag>& tags,
                                 const std::vector<Attribute>& outputAttributes)
    : AnalysisObject_Impl(uuid,versionUUID,name,displayName,description),
      m_problemUUID(problemUUID),
      m_analysisUUID(analysisUUID),
      m_complete(complete),
      m_failed(failed),
      m_variableValues(variableValues),
      m_responseValues(responseValues),
      m_directory(directory),
      m_osmInputData(osmInputData),
      m_idfInputData(idfInputData),
      m_sqlOutputData(sqlOutputData),
      m_xmlOutputData(xmlOutputData),
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
    if (other.xmlOutputData()) {
      m_xmlOutputData = other.xmlOutputData().get().clone();
    }
    Q_FOREACH(const Tag& tag, other.tags()) {
      m_tags.push_back(tag.clone());
    }
    Q_FOREACH(const Attribute& attribute, other.outputAttributes()) {
      m_outputAttributes.push_back(attribute.clone());
    }
    // DLM: TODO should we clone topLevelJob? for now do nothing.
    // ETH: Need to clone topLevelJob for save as-ing SimpleProject.
    //      Am trying just pulling the same runmanager::Job over. When it 
    //      gets to osp, should just keep uuid.
    // DLM: TODO should we clone dakotaParametersFiles? for now do nothing.
  }

  AnalysisObject DataPoint_Impl::clone() const {
    boost::shared_ptr<DataPoint_Impl> impl(new DataPoint_Impl(*this));
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
      Q_ASSERT(m_problemUUID == problem().uuid());
    }
    Q_ASSERT(!m_problemUUID.isNull());
    return m_problemUUID;
  }

  boost::optional<UUID> DataPoint_Impl::analysisUUID() const {
    if (!m_analysisUUID) {
      if (OptionalAnalysisObject analysis = parent()) {
        Q_ASSERT(analysis->optionalCast<Analysis>());
        m_analysisUUID = analysis->uuid();
      }
    }
    else if (OptionalAnalysisObject analysis = parent()) {
      Q_ASSERT(analysis->optionalCast<Analysis>());
      Q_ASSERT(m_analysisUUID.get() == analysis->uuid());
    }
    return m_analysisUUID;
  }

  bool DataPoint_Impl::isComplete() const {
    return m_complete;
  }

  bool DataPoint_Impl::failed() const {
    return m_failed;
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

  boost::optional<FileReference> DataPoint_Impl::xmlOutputData() const {
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
    if (!m_outputAttributes.empty()) {
      return m_outputAttributes;
    }
    if (m_xmlOutputData) {
      OptionalAttribute wrapperAttribute = Attribute::loadFromXml(m_xmlOutputData->path());
      if (wrapperAttribute) {
        m_outputAttributes = wrapperAttribute->valueAsAttributeVector();
      }
      else {
        LOG(Warn,"Unable to load attribute xml from " << toString(m_xmlOutputData->path()));
      }
      return m_outputAttributes;
    }
    LOG(Info,"No Xml output file is set for this DataPoint.");
    return m_outputAttributes;
  }

  boost::optional<Attribute> DataPoint_Impl::getOutputAttribute(
      const std::string& attributeName) const
  {
    AttributeVector myAttributes = outputAttributes();
    BOOST_FOREACH(const Attribute& attribute,myAttributes) {
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
    TagVector::const_iterator it = std::find_if(m_tags.begin(),m_tags.end(),finder);
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
        BOOST_ASSERT(mine.type() == QVariant::Double);
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
    TagVector::iterator it = std::find_if(m_tags.begin(),m_tags.end(),finder);
    if (it != m_tags.end()) {
      m_tags.erase(it);
      onChange(AnalysisObject_Impl::Benign);
    }
  }

  void DataPoint_Impl::clearFileDataFromCache() const {
    m_model = boost::none;
    m_workspace = boost::none;
    m_sqlFile = boost::none;
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
    m_xmlOutputData.reset();
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

  std::ostream& DataPoint_Impl::toJSON(std::ostream& os) const {
    os << toJSON();
    return os;
  }

  std::string DataPoint_Impl::toJSON() const {
    QVariant json = toTopLevelVariant();
    return openstudio::toJSON(json);
  }

  void DataPoint_Impl::setOsmInputData(const FileReference& file) {
    BOOST_ASSERT(file.fileType() == FileReferenceType::OSM);
    m_osmInputData = file;
    m_model = boost::none;
    onChange(AnalysisObject_Impl::Benign);
  }

  void DataPoint_Impl::setIdfInputData(const FileReference& file) {
    BOOST_ASSERT(file.fileType() == FileReferenceType::IDF);
    m_idfInputData = file;
    m_workspace = boost::none;
    onChange(AnalysisObject_Impl::Benign);
  }

  void DataPoint_Impl::setSqlOutputData(const FileReference& file) {
    BOOST_ASSERT(file.fileType() == FileReferenceType::SQL);
    m_sqlOutputData = file;
    m_model = boost::none;
    m_sqlFile = boost::none;
    onChange(AnalysisObject_Impl::Benign);
  }

  void DataPoint_Impl::setXmlOutputData(const FileReference& file) {
    BOOST_ASSERT(file.fileType() == FileReferenceType::XML);
    m_xmlOutputData = file;
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
  }

  QVariant DataPoint_Impl::toVariant() const {
    QVariantMap dataPointData = AnalysisObject_Impl::toVariant().toMap();

    dataPointData["data_point_type"] = QString("DataPoint");
    dataPointData["problem_uuid"] = problemUUID().toString();
    if (analysisUUID()) {
      dataPointData["analysis_uuid"] = analysisUUID().get().toString();
    }

    dataPointData["complete"] = isComplete();
    dataPointData["failed"] = failed();

    QVariantList variableValuesList;
    int index(0);
    Q_FOREACH(const QVariant& value, variableValues()) {
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
      Q_FOREACH(double value,responseValues()) {
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
    if (xmlOutputData()) {
      dataPointData["xml_output_data"] = openstudio::detail::toVariant(xmlOutputData().get());
    }

    if (topLevelJob()) {
      dataPointData["top_level_job"] = runmanager::detail::JSON::toVariant(*topLevelJob());
    }

    if (!dakotaParametersFiles().empty()) {
      QVariantList dakotaParametersFilesList;
      Q_FOREACH(const openstudio::path& p,dakotaParametersFiles()) {
        dakotaParametersFilesList.push_back(toQString(p));
      }
      dataPointData["dakota_parameters_files"] = QVariant(dakotaParametersFilesList);
    }

    if (!tags().empty()) {
      QVariantList tagsList;
      Q_FOREACH(const Tag& tag,tags()) {
        tagsList.push_back(openstudio::detail::toVariant(tag));
      }
      dataPointData["tags"] = QVariant(tagsList);
    }

    if (!outputAttributes().empty()) {
      QVariantList outputAttributesList;
      Q_FOREACH(const Attribute& attribute,outputAttributes()) {
        outputAttributesList.push_back(openstudio::detail::toVariant(attribute));
      }
      dataPointData["output_attributes"] = QVariant(outputAttributesList);
    }

    return QVariant(dataPointData);
  }

  QVariant DataPoint_Impl::toTopLevelVariant() const {
    QVariant dataPointData = this->toVariant();

    // create top-level of final file
    QVariantMap result;
    result["metadata"] = jsonMetadata();
    result["data_point"] = dataPointData;

    return result;
  }

  DataPoint DataPoint_Impl::factoryFromVariant(const QVariant& variant,
                                               const VersionString& version)
  {
    QVariantMap map = variant.toMap();

    if (!map.contains("data_point_type")) {
      LOG_AND_THROW("Unable to find DataPoint in expected location.");
    }

    std::string dataPointType = map["data_point_type"].toString().toStdString();
    if (dataPointType == "DataPoint") {
      return DataPoint_Impl::fromVariant(variant,version);
    }
    if (dataPointType == "OptimizationDataPoint") {
      return OptimizationDataPoint_Impl::fromVariant(variant,version);
    }

    LOG_AND_THROW("Unexpected data_point_type " << dataPointType << ".");
  }

  DataPoint DataPoint_Impl::fromVariant(const QVariant& variant, const VersionString& version) {
    QVariantMap map = variant.toMap();

    // variable values
    QVariantList variableValuesList = map["variable_values"].toList();
    unsigned n = variableValuesList.size();
    std::vector<QVariant> variableValues(n,QVariant());
    Q_FOREACH(const QVariant& variableValue,variableValuesList) {
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
      int index = vvMap["variable_value_index"];
      if (index >= n) {
        LOG_AND_THROW("Unexpected variable_value_index " << index << " in variable_values "
                      << "list of length " << n << ".");
      }
      variableValues[index] = val;
    }

    // response values
    DoubleVector responseValues;
    if (map.contains("response_values")) {
      responseValues = deserializeOrderedVector<double>(map["response_values"].toList(),
                                                        "value",
                                                        "response_value_index",
                                                        boost::bind(&QVariant::toDouble,_1));
    }

    // tags
    TagVector tags;
    if (map.contains("tags")) {
      tags = deserializeUnorderedVector<Tag>(map["tags"].toList(),
                                             boost::bind(openstudio::detail::toTag,_1));
    }

    // output attributes
    AttributeVector outputAttributes;
    if (map.contains("output_attributes")) {
      outputAttributes = deserializeUnorderedVector(map["output_attributes"].toList(),
                                                    boost::bind(openstudio::detail::toAttribute,_1));
    }

    // dakota parameters files
    std::vector<openstudio::path> dakotaParametersFiles;
    if (map.contains("dakota_parameters_files")) {
      dakotaParametersFiles = deserializeUnorderedVector(map["dakota_parameters_files"].toList(),
                                                         boost::bind(openstudio::toPath,boost::bind(&QString::toString,_1)));
    }

    return DataPoint(map["uuid"].toUUID(),
                     map["version_uuid"].toUUID(),
                     map.contains("name") ? map["name"].toString().toStdString() : std::string(),
                     map.contains("display_name") ? map["display_name"].toString().toStdString() : std::string(),
                     map.contains("description") ? map["description"].toString().toStdString() : std::string(),
                     map["problem_uuid"].toUUID(),
                     map.contains("analysis_uuid") ? map["analysis_uuid"].toUUID() : boost::none,
                     map["complete"].toBool(),
                     map["failed"].toBool(),
                     variableValues,
                     responseValues,
                     map.contains["directory"] ? toPath(map["directory"].toString()) : openstudio::path(),
                     map.contains["osm_input_data"] ? openstudio::detail::toFileReference(map["osm_input_data"],version) : OptionalFileReference(),
                     map.contains["idf_input_data"] ? openstudio::detail::toFileReference(map["idf_input_data"],version) : OptionalFileReference(),
                     map.contains["sql_output_data"] ? openstudio::detail::toFileReference(map["sql_output_data"],version) : OptionalFileReference(),
                     map.contains["xml_output_data"] ? openstudio::detail::toFileReference(map["xml_output_data"],version) : OptionalFileReference(),
                     map.contains["top_level_job"] ? runmanager::JSON::toJob(map["top_level_job"]) : boost::optional<runmanager::Job>(),
                     dakotaParametersFiles,
                     tags,
                     outputAttributes);
  }

} // detail

DataPoint::DataPoint(const Problem& problem,
                     const std::vector<QVariant>& variableValues)
  : AnalysisObject(boost::shared_ptr<detail::DataPoint_Impl>(
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
                     const std::vector<QVariant>& variableValues,
                     const std::vector<double>& responseValues,
                     const openstudio::path& directory,
                     const boost::optional<FileReference>& osmInputData,
                     const boost::optional<FileReference>& idfInputData,
                     const boost::optional<FileReference>& sqlOutputData,
                     const boost::optional<FileReference>& xmlOutputData,
                     const boost::optional<runmanager::Job>& topLevelJob,
                     const std::vector<openstudio::path>& dakotaParametersFiles,
                     const std::vector<Tag>& tags,
                     const std::vector<Attribute>& outputAttributes)
  : AnalysisObject(boost::shared_ptr<detail::DataPoint_Impl>(
        new detail::DataPoint_Impl(uuid,
                                   versionUUID,
                                   name,
                                   displayName,
                                   description,
                                   problem,
                                   complete,
                                   failed,
                                   variableValues,
                                   responseValues,
                                   directory,
                                   osmInputData,
                                   idfInputData,
                                   sqlOutputData,
                                   xmlOutputData,
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
                     const std::vector<QVariant>& variableValues,
                     const std::vector<double>& responseValues,
                     const openstudio::path& directory,
                     const boost::optional<FileReference>& osmInputData,
                     const boost::optional<FileReference>& idfInputData,
                     const boost::optional<FileReference>& sqlOutputData,
                     const boost::optional<FileReference>& xmlOutputData,
                     const boost::optional<runmanager::Job>& topLevelJob,
                     const std::vector<openstudio::path>& dakotaParametersFiles,
                     const std::vector<Tag>& tags,
                     const std::vector<Attribute>& outputAttributes)
  : AnalysisObject(boost::shared_ptr<detail::DataPoint_Impl>(
        new detail::DataPoint_Impl(uuid,
                                   versionUUID,
                                   name,
                                   displayName,
                                   description,
                                   problemUUID,
                                   analysisUUID,
                                   complete,
                                   failed,
                                   variableValues,
                                   responseValues,
                                   directory,
                                   osmInputData,
                                   idfInputData,
                                   sqlOutputData,
                                   xmlOutputData,
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

bool DataPoint::failed() const {
  return getImpl<detail::DataPoint_Impl>()->failed();
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

boost::optional<FileReference> DataPoint::xmlOutputData() const {
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

void DataPoint::setDirectory(const openstudio::path& directory) {
  getImpl<detail::DataPoint_Impl>()->setDirectory(directory);
}

void DataPoint::addTag(const std::string& tagName) {
  getImpl<detail::DataPoint_Impl>()->addTag(tagName);
}

void DataPoint::deleteTag(const std::string& tagName) {
  getImpl<detail::DataPoint_Impl>()->deleteTag(tagName);
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

bool DataPoint::saveJSON(const openstudio::path& p,bool overwrite) const {
  return getImpl<detail::DataPoint_Impl>()->saveJSON(p,overwrite);
}

std::ostream& DataPoint::toJSON(std::ostream& os) const {
  return getImpl<detail::DataPoint_Impl>()->toJSON(os);
}

std::string DataPoint::toJSON() const {
  return getImpl<detail::DataPoint_Impl>()->toJSON();
}

/// @cond
DataPoint::DataPoint(boost::shared_ptr<detail::DataPoint_Impl> impl)
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

void DataPoint::setXmlOutputData(const FileReference& file) {
  getImpl<detail::DataPoint_Impl>()->setXmlOutputData(file);
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

} // analysis
} // openstudio

