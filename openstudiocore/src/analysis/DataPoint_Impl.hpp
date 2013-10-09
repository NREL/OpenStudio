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

#ifndef ANALYSIS_DATAPOINT_IMPL_HPP
#define ANALYSIS_DATAPOINT_IMPL_HPP

#include <analysis/AnalysisAPI.hpp>
#include <analysis/AnalysisObject_Impl.hpp>

#include <analysis/DataPoint.hpp>
#include <analysis/Problem.hpp>

#include <model/Model.hpp>

#include <runmanager/lib/Job.hpp>

#include <utilities/idf/Workspace.hpp>
#include <utilities/sql/SqlFile.hpp>
#include <utilities/data/Attribute.hpp>
#include <utilities/data/Tag.hpp>

#include <utilities/core/FileReference.hpp>

#include <QVariant>

namespace openstudio {

namespace runmanager {
  class Job;
}

namespace analysis {

namespace detail {

  /** DataPoint_Impl is a AnalysisObject_Impl that is the implementation class for DataPoint.*/
  class ANALYSIS_API DataPoint_Impl : public AnalysisObject_Impl {
    Q_OBJECT;
   public:
    /** @name Constructors and Destructors */
    //@{

    /** Constructor from variableValues. Called by Algorithm, may also be called directly by a
     *  user. After construction isComplete() == false. */
    DataPoint_Impl(const Problem& problem,
                   const std::vector<QVariant>& variableValues);

    /** Constructor provided for deserialization; not for general use. */
    DataPoint_Impl(const UUID& uuid,
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
    DataPoint_Impl(const UUID& uuid,
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

    DataPoint_Impl(const DataPoint_Impl& other);

    virtual ~DataPoint_Impl() {}

    virtual AnalysisObject clone() const;

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

    openstudio::path directory() const;

    boost::optional<FileReference> osmInputData() const;

    boost::optional<FileReference> idfInputData() const;

    boost::optional<FileReference> sqlOutputData() const;

    /** Returns the openstudio::Attribute XML files created by any reporting measures, if
     *  complete() and not failed(), and problem() located such files during the update process.
     *  Otherwise, the return value is .empty(). */
    std::vector<FileReference> xmlOutputData() const;

    boost::optional<runmanager::Job> topLevelJob() const;

    std::vector<openstudio::path> dakotaParametersFiles() const;

    boost::optional<model::Model> model() const; // for analyses on osm

    boost::optional<Workspace> workspace() const; // for analyses on idf

    boost::optional<SqlFile> sqlFile() const; // also available through model().sqlFile() if model available

    std::vector<Attribute> outputAttributes() const;

    boost::optional<Attribute> getOutputAttribute(const std::string& attributeName) const;

    std::vector<Tag> tags() const;

    bool isTag(const std::string& tagName) const;

    /** Returns true if testVariableValues is correct size, and any non-null values match.
     *  Otherwise, returns false. */
    bool matches(const std::vector<QVariant>& testVariableValues) const;

    //@}
    /** @name Setters */
    //@{

    void setSelected(bool selected);

    void setRunType(const DataPointRunType& runType);

    void setDirectory(const openstudio::path& directory);

    void setTopLevelJob(const runmanager::Job& topLevelJob);

    void setDakotaParametersFiles(const std::vector<openstudio::path>& dakotaParametersFiles);

    void addTag(const std::string& tagName);

    void deleteTag(const std::string& tagName);

    //@}
    /** @name Actions */
    //@{

    /** Update high level results from json. */
    bool updateFromJSON(const std::string& json, boost::optional<runmanager::RunManager>& runManager);

    virtual bool updateFromJSON(const AnalysisJSONLoadResult& loadResult, boost::optional<runmanager::RunManager>& runManager);

    /** Whoever downloaded the zip file should have setDirectory(), and had the file placed in
     *  directory() / toPath("dataPoint.zip"). */
    bool updateDetails(boost::optional<runmanager::RunManager>& runManager);

    /** Clear model, workspace, and sqlFile from cache. */
    void clearFileDataFromCache() const;

    /** Clear model, workspace, sqlFile, and outputAttributes from cache. */
    void clearAllDataFromCache() const;

    virtual void clearResults();

    //@}
    /** @name Serialization */
    //@{

    bool saveJSON(const openstudio::path& p,
                  const DataPointSerializationOptions& options,
                  bool overwrite=false) const;

    std::ostream& toJSON(std::ostream& os,const DataPointSerializationOptions& options) const;

    std::string toJSON(const DataPointSerializationOptions& options) const;

    //@}
    /** @name Protected in or Absent from Public Class */
    //@{

    void setOsmInputData(const FileReference& file);

    void setIdfInputData(const FileReference& file);

    void setSqlOutputData(const FileReference& file);

    void setXmlOutputData(const std::vector<FileReference>& files);

    void markComplete();

    void markFailed();

    void setResponseValues(const std::vector<double> values);

    void setProblem(const Problem& problem);

    virtual QVariant toVariant() const;

    /** Contents of toVariant finalized for direct serialization (adds jsonMetadata and
     *  data_point moniker. */
    QVariant toTopLevelVariant(const DataPointSerializationOptions& options) const;

    static DataPoint factoryFromVariant(const QVariant& variant,
                                        const VersionString& version,
                                        const boost::optional<Problem>& problem);

    static DataPoint fromVariant(const QVariant& variant, const VersionString& version);

    virtual QVariant toServerDataPointsVariant() const;

    //@}
   protected:
    // relationship state
    // most of the time, m_problem should exist and m_parent should point to an Analysis
    boost::optional<Problem> m_problem;     // Problem to which this DataPoint refers
    // immediately after deserialization from json, may just have problem and analysis uuids
    UUID m_problemUUID;
    mutable boost::optional<UUID> m_analysisUUID;

    // pure data
    bool m_complete;                        // false after construction
                                            // set to true by Problem update
    bool m_failed;                          // false after construction
                                            // set to true by Problem update if point is unusable
    bool m_selected;                        // true after construction
                                            // used by Analysis to determine which points to run
    DataPointRunType m_runType;
    std::vector<QVariant> m_variableValues; // variable values for this run
    std::vector<double> m_responseValues;   // response function values for this run
    openstudio::path m_directory;           // directory containing results
    boost::optional<FileReference> m_osmInputData;  // an osm file
    boost::optional<FileReference> m_idfInputData;  // an idf file
    boost::optional<FileReference> m_sqlOutputData; // a sql file
    std::vector<FileReference> m_xmlOutputData; // attribute xml
    boost::optional<runmanager::Job> m_topLevelJob;
    std::vector<openstudio::path> m_dakotaParametersFiles;
    std::vector<Tag> m_tags;                // meta-data for query and display

    // cache variables
    mutable std::vector<Attribute> m_outputAttributes; // serialized and deserialized
    mutable boost::optional<model::Model> m_model;
    mutable boost::optional<Workspace> m_workspace;
    mutable boost::optional<SqlFile> m_sqlFile;    
   private:
    REGISTER_LOGGER("openstudio.analysis.DataPoint");   
  };

} // detail

} // model
} // openstudio

#endif // ANALYSIS_DATAPOINT_IMPL_HPP

