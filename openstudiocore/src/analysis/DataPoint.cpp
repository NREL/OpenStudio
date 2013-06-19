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

#include <runmanager/lib/Job.hpp>

#include <utilities/data/Attribute.hpp>

#include <utilities/math/FloatCompare.hpp>

#include <utilities/core/FileReference.hpp>
#include <utilities/core/Assert.hpp>
#include <utilities/core/Finder.hpp>

#include <boost/foreach.hpp>

namespace openstudio {
namespace analysis {

namespace detail {

  DataPoint_Impl::DataPoint_Impl(const Problem& problem,
                                 const std::vector<QVariant>& variableValues)
    : AnalysisObject_Impl(std::string()),
      m_complete(false), m_failed(false), m_problem(problem), m_variableValues(variableValues)
  {}

  DataPoint_Impl::DataPoint_Impl(const UUID& uuid,
                                 const UUID& versionUUID,
                                 const std::string& name,
                                 const std::string& displayName,
                                 const std::string& description,
                                 bool complete,
                                 bool failed,
                                 const Problem& problem,
                                 const std::vector<QVariant>& variableValues,
                                 const std::vector<double>& responseValues,
                                 const openstudio::path& directory,
                                 const boost::optional<FileReference>& osmInputData,
                                 const boost::optional<FileReference>& idfInputData,
                                 const boost::optional<FileReference>& sqlOutputData,
                                 const boost::optional<FileReference>& xmlOutputData,
                                 const std::vector<Tag>& tags,
                                 const boost::optional<runmanager::Job>& topLevelJob,
                                 const std::vector<openstudio::path>& dakotaParametersFiles)
    : AnalysisObject_Impl(uuid,versionUUID,name,displayName,description),
      m_complete(complete),
      m_failed(failed),
      m_problem(problem),
      m_variableValues(variableValues),
      m_responseValues(responseValues),
      m_directory(directory),
      m_osmInputData(osmInputData),
      m_idfInputData(idfInputData),
      m_sqlOutputData(sqlOutputData),
      m_xmlOutputData(xmlOutputData),
      m_topLevelJob(topLevelJob),
      m_dakotaParametersFiles(dakotaParametersFiles),
      m_tags(tags)
  {}

  DataPoint_Impl::DataPoint_Impl(const DataPoint_Impl &other)
    : AnalysisObject_Impl(other),
      m_complete(other.isComplete()),
      m_failed(other.failed()),
      m_problem(other.problem()),
      m_variableValues(other.variableValues()),
      m_responseValues(other.responseValues()),
      m_directory(other.directory())
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
    BOOST_FOREACH(const Tag& tag, other.tags()) {
      m_tags.push_back(tag.clone());
    }
    // DLM: TODO should we clone topLevelJob? for now do nothing.
    // DLM: TODO should we clone dakotaParametersFiles? for now do nothing.
  }

  AnalysisObject DataPoint_Impl::clone() const {
    boost::shared_ptr<DataPoint_Impl> impl(new DataPoint_Impl(*this));
    return DataPoint(impl);
  }

  bool DataPoint_Impl::isComplete() const {
    return m_complete;
  }

  bool DataPoint_Impl::failed() const {
    return m_failed;
  }

  Problem DataPoint_Impl::problem() const {
    return m_problem;
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
                     bool complete,
                     bool failed,
                     const Problem& problem,
                     const std::vector<QVariant>& variableValues,
                     const std::vector<double>& responseValues,
                     const openstudio::path& directory,
                     const boost::optional<FileReference>& osmInputData,
                     const boost::optional<FileReference>& idfInputData,
                     const boost::optional<FileReference>& sqlOutputData,
                     const boost::optional<FileReference>& xmlOutputData,
                     const std::vector<Tag>& tags,
                     const boost::optional<runmanager::Job>& topLevelJob,
                     const std::vector<openstudio::path>& dakotaParametersFiles)
  : AnalysisObject(boost::shared_ptr<detail::DataPoint_Impl>(
        new detail::DataPoint_Impl(uuid,
                                   versionUUID,
                                   name,
                                   displayName,
                                   description,
                                   complete,
                                   failed,
                                   problem,
                                   variableValues,
                                   responseValues,
                                   directory,
                                   osmInputData,
                                   idfInputData,
                                   sqlOutputData,
                                   xmlOutputData,
                                   tags,
                                   topLevelJob,
                                   dakotaParametersFiles)))
{}

bool DataPoint::isComplete() const {
  return getImpl<detail::DataPoint_Impl>()->isComplete();
}

bool DataPoint::failed() const {
  return getImpl<detail::DataPoint_Impl>()->failed();
}

Problem DataPoint::problem() const {
  return getImpl<detail::DataPoint_Impl>()->problem();
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

