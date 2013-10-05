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

#ifndef ANALYSIS_ANALYSISDATA_IMPL_HPP
#define ANALYSIS_ANALYSISDATA_IMPL_HPP

#include <analysis/AnalysisAPI.hpp>
#include <analysis/AnalysisObject_Impl.hpp>

#include <analysis/Problem.hpp>
#include <analysis/Algorithm.hpp>
#include <analysis/DataPoint.hpp>

#include <utilities/core/FileReference.hpp>

#include <vector>

namespace openstudio {

class Table;

namespace runmanager {
  class Job;
}

namespace analysis {

struct AnalysisSerializationOptions;

namespace detail {

  /** Analysis_Impl is a AnalysisObject_Impl that is the implementation class for Analysis.*/
  class ANALYSIS_API Analysis_Impl : public AnalysisObject_Impl {
    Q_OBJECT;
   public:
    /** @name Constructors and Destructors */
    //@{

    /** Constructor for an OSM or IDF analysis with seed model not yet specified. If the seed model
     *  is never specified before the analysis is run, the AnalysisDriver will create a blank energy
     *  model of seedType (must be OSM or IDF) to serve as a default starting point. */
    Analysis_Impl(const std::string& name,
                  const Problem& problem,
                  const FileReferenceType& seedType);

    /** Constructor for OSM analysis (weather file sepecified in Model). As no algorithm is
     *  specified, the user is expected to add incomplete \link DataPoint DataPoints \endlink
     *  by hand. The AnalysisDriver will then run these custom points. */
    Analysis_Impl(const std::string& name,
                  const Problem& problem,
                  const FileReference& seed);

    /** Constructor for IDF analysis (specify weather file directly). As no algorithm is
     *  specified, the user is expected to add incomplete \link DataPoint DataPoints \endlink
     *  by hand. The AnalysisDriver will then run these custom points. */
    Analysis_Impl(const std::string& name,
                  const Problem& problem,
                  const FileReference& seed,
                  const FileReference& weatherFile);

    /** Constructor for OSM analysis (weather file sepecified in Model). Algorithm is used to
     *  populate the Analysis with incomplete \link DataPoint DataPoints \endlink. */
    Analysis_Impl(const std::string& name,
                  const Problem& problem,
                  const Algorithm& algorithm,
                  const FileReference& seed);

    /** Constructor for IDF analysis (specify weather file directly). Algorithm is used to
     *  populate the Analysis with incomplete \link DataPoint DataPoints \endlink. */
    Analysis_Impl(const std::string& name,
                  const Problem& problem,
                  const Algorithm& algorithm,
                  const FileReference& seed,
                  const FileReference& weatherFile);

    /** Constructor provided for deserialization; not for general use. */
    Analysis_Impl(const UUID& uuid,
                  const UUID& versionUUID,
                  const std::string& name,
                  const std::string& displayName,
                  const std::string& description,
                  const Problem& problem,
                  const boost::optional<Algorithm>& algorithm,
                  const FileReference& seed,
                  const boost::optional<FileReference>& weatherFile,
                  const std::vector<DataPoint>& dataPoints,
                  bool resultsAreInvalid,
                  bool dataPointsAreInvalid);

    Analysis_Impl(const Analysis_Impl& other);

    virtual ~Analysis_Impl() {}

    virtual AnalysisObject clone() const;

    //@}
    /** @name Getters and Queries */
    //@{

    Problem problem() const;

    boost::optional<Algorithm> algorithm() const;

    FileReference seed() const;

    boost::optional<FileReference> weatherFile() const;

    std::vector<DataPoint> dataPoints() const;

    std::vector<DataPoint> dataPointsToQueue() const;

    std::vector<DataPoint> completeDataPoints() const;

    std::vector<DataPoint> successfulDataPoints() const;

    std::vector<DataPoint> failedDataPoints() const;

    /** Return all complete \link DataPoint DataPoints\endlink with runType() ==
     *  DataPointRunType::CloudDetailed and an empty directory(). */
    std::vector<DataPoint> dataPointsNeedingDetails() const;

    /** Get the DataPoints with matching variableValues. VariableValues may contain Null QVariants of
     *  the correct type, which means that any value at that position should be returned. */
    std::vector<DataPoint> getDataPoints(const std::vector<QVariant>& variableValues) const;

    /** Get the DataPoints defined by measures. Perturbations must be translatable into a valid set
     *  of variableValues for the problem(). */
    std::vector<DataPoint> getDataPoints(
        const std::vector< boost::optional<Measure> >& measures) const;

    std::vector<DataPoint> getDataPoints(const std::string& tag) const;

    /** Get the DataPoint defined by measures, if it exists. Perturbations must be a valid set
     *  of variable values for the problem(). */
    boost::optional<DataPoint> getDataPoint(const std::vector<Measure>& measures) const;

    boost::optional<DataPoint> getDataPointByUUID(const UUID& uuid) const;

    boost::optional<DataPoint> getDataPointByUUID(const DataPoint& dataPoint) const;

    /** Flag flips to true if underlying structure of analysis changes and there are any
     *  completeDataPoints(). */
    bool resultsAreInvalid() const;

    /** Flag flips to true if structure of problem's variables changes and there are dataPoints().
     *  New data points cannot be added when analysis is in this state (dataPointsAreInvalid() ==
     *  true). */
    bool dataPointsAreInvalid() const;

    //@}
    /** @name Setters */
    //@{

    bool setProblem(Problem& problem);

    bool setAlgorithm(Algorithm& algorithm);

    void clearAlgorithm();

    /** Set seed to newSeed. Used by AnalysisDriver to point to copy of seed placed within
     *  ProjectDatabase file structure. */
    bool setSeed(const FileReference& newSeed);

    /** Set seed to blank model of seedType. Fails if seedType is not OSM or IDF or is incompatible
     *  with current data. */
    bool clearSeed(const FileReferenceType& seedType);

    bool setWeatherFile(const FileReference& newWeatherFile);

    void clearWeatherFile();

    /** Adds a DataPoint to this analysis and returns true if the data point is not already in the
     *  analysis and if not dataPointsAreInvalid. Returns false otherwise. Throws an
     *  openstudio::Exception if dataPoint.variableValues() are not valid for problem(). Should be
     *  called before running a given workflow. Usually called by Algorithm, but may also be called
     *  directly by a user to run custom analyses. */
    bool addDataPoint(DataPoint& dataPoint);

    /** Adds a DataPoint to this analysis and returns true if measures are valid for problem(),
     *  the resulting DataPoint is not yet in this Analysis, and if not dataPointsAreInvalid. */
    bool addDataPoint(const std::vector<Measure>& measures);

    /** Sets run information on a DataPoint. Returns false if dataPoint is not in this analysis by
     *  UUID. */
    bool setDataPointRunInformation(DataPoint& dataPoint, const runmanager::Job& topLevelJob, const std::vector<openstudio::path>& dakotaParametersFiles);

    /** Removes dataPoint from this analysis. Returns false if dataPoint is not in this analysis by
     *  UUID. */
    bool removeDataPoint(const DataPoint& dataPoint);

    /** Removes all dataPoints from this analysis, resets dataPointsAreInvalid flag, and
     *  reinitializes algorithm(), if it exists. */
    void removeAllDataPoints();

    /** Resets dataPoint to not point to any output data and to be incomplete and not failed. */
    bool clearResults(DataPoint& dataPoint);

    /** Resets all data points to an incomplete, not failed state, resets resultsAreInvalid flag,
     *  and reinitializes algorithm(), if it exists. */
    void clearAllResults();

    //@}
    /** @name Actions */
    //@{

    /** Call this method to reset the dirty flag of this object and all its children. In general,
     *  this method should be called after this object has been saved to the ProjectDatabase
     *  (record constructed, database saved, and transaction committed). Returns false if the flag
     *  cannot be cleared for some reason. */
    virtual bool clearDirtyFlag();

    /** Updates an existing DataPoint in this analysis. Throws an openstudio::Exception if DataPoint
     *  is not in this analysis, or if completedJob is not complete, or was not spawned by algorithm()
     *  acting on problem(). Should be called after a given workflow has been run. */
    void updateDataPoint(DataPoint& dataPoint,
                         const runmanager::Job& completedJob);

    /** Register Dakota's restart file location so that the analysis can be restarted if necessary.
     *  Throws if dakotaAlgorithm != algorithm().get(). */
    void initializeDakotaAlgorithm(DakotaAlgorithm& dakotaAlgorithm,
                                   const runmanager::Job& dakotaJob);

    /** Updates the DakotaAlgorithm being used by this analysis. Thows an openstudio::Exception if
     *  DakotaAlgorithm is not being used by this analysis, or if completedJob is not complete, or
     *  does not correspond to the proper Dakota job. Should be called when the given Dakota job
     *  completes. */
    void updateDakotaAlgorithm(const runmanager::Job& completedDakotaJob);

    /** Returns a csv summary of all the data points in this analysis. */
    Table summaryTable() const;

    /// Relocate path data from originalBase to newBase.
    virtual void updateInputPathData(const openstudio::path& originalBase,
                                     const openstudio::path& newBase);

    //@}
    /** @name Serialization
     *  Methods to save to json format. See AnalysisObject.hpp, openstudio::analysis::loadJSON for
     *  the de-serialization methods. */
    //@{

    bool saveJSON(const openstudio::path& p,
                  const AnalysisSerializationOptions& options,
                  bool overwrite=false) const;

    std::ostream& toJSON(std::ostream& os,const AnalysisSerializationOptions& options) const;

    std::string toJSON(const AnalysisSerializationOptions& options) const;

    //@}
    /** @name Protected in or Absent from Public Class */
    //@{

    virtual QVariant toVariant() const;

    /** Finalizes Analysis JSON based on options. */
    QVariant toVariant(const AnalysisSerializationOptions& options) const;

    static Analysis fromVariant(const QVariant& variant,const VersionString& version);

    //@}
   signals:
    void seedChanged();
   protected:
    Problem m_problem;
    boost::optional<Algorithm> m_algorithm;
    FileReference m_seed;
    boost::optional<FileReference> m_weatherFile;
    std::vector<DataPoint> m_dataPoints;
    bool m_resultsAreInvalid;
    bool m_dataPointsAreInvalid;

    /** Calls AnalysisObject_Impl version and invalidates data points if appropriate. */
    virtual void onChange(ChangeType changeType);

   private:
    REGISTER_LOGGER("openstudio.analysis.Analysis");
  };

} // detail

} // model
} // openstudio

#endif // ANALYSIS_ANALYSISDATA_IMPL_HPP

