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

#ifndef ANALYSIS_DATAPOINT_HPP
#define ANALYSIS_DATAPOINT_HPP

#include <analysis/AnalysisAPI.hpp>
#include <analysis/AnalysisObject.hpp>

#include <utilities/core/Enum.hpp>
#include <utilities/core/Path.hpp>

#include <QVariant>
#include <QDomDocument>

namespace openstudio {

class FileReference;
class Workspace;
class SqlFile;
class Attribute;
class Tag;

namespace runmanager {
  class Job;
  class RunManager;
}

namespace model {
  class Model;
}

namespace analysis {

class Analysis;
class Problem;

namespace detail {
  class DataPoint_Impl;
  class Problem_Impl;
  class Analysis_Impl;
} // detail

struct ANALYSIS_API DataPointSerializationOptions {
  openstudio::path projectDir;
  bool osServerView;

  DataPointSerializationOptions(const openstudio::path& t_projectDir = openstudio::path(),
                                bool t_osServerView=true);
};

/** \class DataPointRunType
 *  \brief List of DataPoint run types.
 *  \details See the OPENSTUDIO_ENUM documentation in utilities/core/Enum.hpp. The actual
 *  macro call is:
 *  \code
OPENSTUDIO_ENUM(DataPointRunType,
    ((Local))
    ((CloudSlim))
    ((CloudDetailed))
);
 *  \endcode */
OPENSTUDIO_ENUM(DataPointRunType,
    ((Local))
    ((CloudSlim))
    ((CloudDetailed))
);

/** DataPoint is an AnalysisObject that describes a single simulation run/to be run for a given
 *  Analysis. New \link DataPoint DataPoints \endlink are constructed using
 *  Problem::createDataPoint, since the DataPoint variableValues only make sense in the context of
 *  a Problem. Similarly, results only make sense in the context of an Analysis, where there is
 *  a specific Problem and a specific seed file defined. Therefore, once a DataPoint is created,
 *  it should be added to an Analysis (Analysis::addDataPoint), and then run using an
 *  analysisdriver::AnalysisDriver. In many cases, the createDataPoint and addDataPoint steps are
 *  hidden from the user through the use of an Algorithm. */
class ANALYSIS_API DataPoint : public AnalysisObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  /** Constructor provided for deserialization; not for general use. */
  DataPoint(const UUID& uuid,
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
            const std::vector<FileReference>& xmlOutputData,
            const boost::optional<runmanager::Job>& topLevelJob,
            const std::vector<openstudio::path>& dakotaParametersFiles,
            const std::vector<Tag>& tags,
            const std::vector<Attribute>& outputAttributes);

  /** Constructor provided for deserialization; not for general use. */
  DataPoint(const UUID& uuid,
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
            const std::vector<FileReference>& xmlOutputData,
            const boost::optional<runmanager::Job>& topLevelJob,
            const std::vector<openstudio::path>& dakotaParametersFiles,
            const std::vector<Tag>& tags,
            const std::vector<Attribute>& outputAttributes);

  virtual ~DataPoint() {}

  //@}
  /** @name Getters and Queries */
  //@{

  /** Returns true if DataPoint has access to the Problem that created it. Should be true
   *  unless DataPoint was deserialized from JSON and has not yet been assimilated back
   *  into its parent Analysis. */
  bool hasProblem() const;

  /** Returns the Problem used to create/associated with this DataPoint. */
  Problem problem() const;

  /** Returns the UUID of the Problem that created this DataPoint. */
  UUID problemUUID() const;

  /** Returns the UUID of the Analysis that parents this DataPoint. */
  boost::optional<UUID> analysisUUID() const;

  /** Returns true if the DataPoint has been simulated. \deprecated */
  bool isComplete() const;

  /** Returns true if the DataPoint has been simulated. */
  bool complete() const;

  /** Returns true if the DataPoint was simulated, but the simulation failed, or output results
   *  could not be retrieved for some other reason. */
  bool failed() const;

  /** Returns true if the DataPoint is selected (to be simulated in the next batch). */
  bool selected() const;

  DataPointRunType runType() const;

  /** Returns the variableValues to be applied in simulating this DataPoint. (That is, inputData
   *  will be the result of applying variableValues to the Analysis seed file.) */
  std::vector<QVariant> variableValues() const;

  /** Returns the value of the response functions for this DataPoint. Only non-empty if isComplete()
   *  and not failed() (and problem().numResponses() > 0). */
  std::vector<double> responseValues() const;

  /** Run directory for this DataPoint. Set by analysisdriver::AnalysisDriver. */
  openstudio::path directory() const;

  /** Returns the final OSM file in the workflow, if the DataPoint isComplete() but not
   *  failed(), and said file was located by problem(). */
  boost::optional<FileReference> osmInputData() const;

  /** Returns the final IDF file in the workflow, if the DataPoint isComplete() but not
   *  failed(), and said file was located by problem(). */
  boost::optional<FileReference> idfInputData() const;

  /** Returns the EnergyPlus SqLite output file, if the DataPoint isComplete() but not failed(),
   *  and said file was located by problem(). */
  boost::optional<FileReference> sqlOutputData() const;

  /** Returns the openstudio::Attribute XML files created by any reporting measures, if
   *  complete() and not failed(), and problem() located such files during the update process.
   *  Otherwise, the return value is .empty(). */
  std::vector<FileReference> xmlOutputData() const;

  /** If osmInputData() exists, returns the corresponding model::Model. Also caches the Model
   *  for future use. */
  boost::optional<model::Model> model() const;

  /** If idfInputData() exists, returns the corresponding Workspace. Also caches the Workspace
   *  for future use. */
  boost::optional<Workspace> workspace() const;

  /** If sqlOutputData() exists, returns the corresponding SqlFile, which is also available through
   *  model()->sqlFile(), if model() exists. Also caches the SqlFile for future use. */
  boost::optional<SqlFile> sqlFile() const;

  /** If xmlOutputData() exists, returns the corresponding openstudio::Attributes. Caches the
   *  Attributes for future use. */
  std::vector<Attribute> outputAttributes() const;

  /** Returns the outputAttributes() element with attributeName, if it exists. */
  boost::optional<Attribute> getOutputAttribute(const std::string& attributeName) const;

  std::vector<Tag> tags() const;

  bool isTag(const std::string& tagName) const;

  /** Returns true if testVariableValues.size() <= variableValues().size(), and any non-null
   *  values match. Otherwise, returns false. */
  bool matches(const std::vector<QVariant>& testVariableValues) const;

  /** Returns the top level job associated with this data point. \sa Problem::getJobsByWorkflowStep */
  boost::optional<runmanager::Job> topLevelJob() const;

  /** Returns vector of dakotaParameterFiles associated with this data point. */
  std::vector<openstudio::path> dakotaParametersFiles() const;

  //@}
  /** @name Setters */
  //@{

  void setSelected(bool selected);

  void setRunType(const DataPointRunType& runType);

  /** Sets the run directory for this DataPoint. Generally called by
   *  analysisdriver::AnalysisDriver. */
  void setDirectory(const openstudio::path& directory);

  void addTag(const std::string& tagName);

  void deleteTag(const std::string& tagName);

  //@}
  /** @name Actions */
  //@{

  /** Update high level results from json. */
  bool updateFromJSON(const std::string& json, boost::optional<runmanager::RunManager>& runManager);

  /** Whoever downloaded the zip file should have setDirectory(), and had the file placed in
   *  directory() / toPath("dataPoint.zip"). */
  bool updateDetails(boost::optional<runmanager::RunManager>& runManager);

  /** Clear model, workspace, and sqlFile from cache. */
  void clearFileDataFromCache() const;

  /** Clear model, workspace, sqlFile, and outputAttributes from cache. */
  void clearAllDataFromCache() const;

  void clearResults();

  //@}
  /** @name Serialization */
  //@{

  bool saveJSON(const openstudio::path& p,
                const DataPointSerializationOptions& options,
                bool overwrite=false) const;

  std::ostream& toJSON(std::ostream& os,const DataPointSerializationOptions& options) const;

  std::string toJSON(const DataPointSerializationOptions& options) const;

  //@}
 protected:
  /// @cond
  typedef detail::DataPoint_Impl ImplType;

  friend class AnalysisObject;
  friend class detail::AnalysisObject_Impl;
  friend class detail::DataPoint_Impl;

  explicit DataPoint(boost::shared_ptr<detail::DataPoint_Impl> impl);
  /// @endcond

  friend class detail::Problem_Impl;

  /** Constructor from variableValues. Called by Problem::createDataPoint. After construction
   *  complete() == false and selected() == true. */
  DataPoint(const Problem& problem,
            const std::vector<QVariant>& variableValues);

  void setOsmInputData(const FileReference& file);

  void setIdfInputData(const FileReference& file);

  void setSqlOutputData(const FileReference& file);

  void setXmlOutputData(const std::vector<FileReference>& files);

  void markComplete();

  void markFailed();

  void setResponseValues(const std::vector<double> values);

  friend class Analysis; // for calling setParent
  friend class detail::Analysis_Impl;

  /** Let Analysis_Impl set the DataPoint's Problem to get a clean clone. */
  void setProblem(const Problem& problem);

 private:

  REGISTER_LOGGER("openstudio.analysis.DataPoint");
};

/** \relates DataPoint*/
typedef boost::optional<DataPoint> OptionalDataPoint;

/** \relates DataPoint*/
typedef std::vector<DataPoint> DataPointVector;

} // analysis
} // openstudio

#endif // ANALYSIS_DATAPOINT_HPP

