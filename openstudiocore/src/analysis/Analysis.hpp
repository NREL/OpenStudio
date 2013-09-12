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

#ifndef ANALYSIS_ANALYSISDATA_HPP
#define ANALYSIS_ANALYSISDATA_HPP

#include <analysis/AnalysisAPI.hpp>
#include <analysis/AnalysisObject.hpp>

#include <utilities/core/Enum.hpp>
#include <utilities/core/Path.hpp>

#include <QVariant>

namespace openstudio {

class FileReference;
class FileReferenceType;
class Table;

namespace runmanager {
  class Job;
}

namespace analysis {

class Problem;
class Algorithm;
class DakotaAlgorithm;
class DataPoint;
class Measure;
class OpenStudioAlgorithm;

namespace detail {

  class Analysis_Impl;

} // detail

/** \class AnalysisSerializationScope
 *  \brief Enum to indicate how much of the analysis should be written out to JSON.
 *  \details See the OPENSTUDIO_ENUM documentation in utilities/core/Enum.hpp. The actual
 *  macro call is:
 *  \code
OPENSTUDIO_ENUM(AnalysisSerializationScope,
    ((ProblemFormulation))
    ((Full))
);
 *  \endcode
 *  ProblemFormulation indicates that DataPoints should not be included in the serialization;
 *  Full indicates that they should. */
OPENSTUDIO_ENUM(AnalysisSerializationScope,
    ((ProblemFormulation))
    ((Full))
);

struct ANALYSIS_API AnalysisSerializationOptions {
  openstudio::path projectDir;
  AnalysisSerializationScope scope;
  bool osServerView;

  AnalysisSerializationOptions(const openstudio::path& t_projectDir=openstudio::path(),
                               const AnalysisSerializationScope& t_scope=AnalysisSerializationScope::ProblemFormulation,
                               bool t_osServerView=true);
};

/** Analysis is a AnalysisObject that contains an entire analysis. It is constructed from a
 *  Problem, an optional Algorithm, an optional seed FileReference (an OSM or IDF file), and
 *  an optional weather FileReference (needed for IDF seeds). Analysis::addDataPoint can be
 *  used to add custom (not dictated by an Algorithm) \link DataPoint DataPoints\endlink to
 *  the Analysis. (Note that new DataPoints should be created using Problem::createDataPoint.)
 *  An analysisdriver::AnalysisDriver can be used to run the Analysis. */
class ANALYSIS_API Analysis : public AnalysisObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  /** Constructor for an OSM or IDF analysis with seed model not yet specified. If the seed model
   *  is never specified before the analysis is run, the AnalysisDriver will create a blank energy
   *  model of seedType (must be OSM or IDF) to serve as a default starting point. */
  Analysis(const std::string& name,
           const Problem& problem,
           const FileReferenceType& seedType);

  /** Constructor for OSM analysis (weather file sepecified in seed model::Model). As no
   *  algorithm is specified, the user must add incomplete \link DataPoint DataPoints \endlink
   *  by hand. A call to analysisdriver::AnalysisDriver::run will then run these custom points. */
  Analysis(const std::string& name,
           const Problem& problem,
           const FileReference& seed);

  /** Constructor for IDF analysis (specify weather file directly). As no algorithm is
   *  specified, the user must add incomplete \link DataPoint DataPoints \endlink by hand. A call
   *  to analysisdriver::AnalysisDriver::run will then run these custom points. */
  Analysis(const std::string& name,
           const Problem& problem,
           const FileReference& seed,
           const FileReference& weatherFile);

  /** Constructor for OSM analysis (weather file sepecified in seed model::Model). Algorithm
   *  is used to populate the Analysis with incomplete \link DataPoint DataPoints \endlink,
   *  either automatically using analysisdriver::AnalysisDriver, or with a user call to
   *  OpenStudioAlgorithm::createNextIteration. */
  Analysis(const std::string& name,
           const Problem& problem,
           const Algorithm& algorithm,
           const FileReference& seed);

  /** Constructor for IDF analysis (specify weather file directly). Algorithm is used to populate
   *  the Analysis with incomplete \link DataPoint DataPoints \endlink, either automatically
   *  using analysisdriver::AnalysisDriver, or with a user call to
   *  Algorithm::createNextIteration. */
  Analysis(const std::string& name,
           const Problem& problem,
           const Algorithm& algorithm,
           const FileReference& seed,
           const FileReference& weatherFile);

  /** Constructor provided for deserialization; not for general use. */
  Analysis(const UUID& uuid,
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

  virtual ~Analysis() {}

  //@}
  /** @name Getters and Queries */
  //@{

  Problem problem() const;

  boost::optional<Algorithm> algorithm() const;

  /** Returns the FileReference that points to this analysis's seed model. If no specific model
   *  has been specified, seed().path() will be "*.osm" or "*.idf". */
  FileReference seed() const;

  boost::optional<FileReference> weatherFile() const;

  /** Returns all \link DataPoint DataPoints\endlink in this Analysis. */
  std::vector<DataPoint> dataPoints() const;

  /** Returns all selected, incomplete \link DataPoint DataPoints\endlink. */
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
  std::vector<DataPoint> getDataPoints(const std::vector<boost::optional<Measure> >& measures) const;

  std::vector<DataPoint> getDataPoints(const std::string& tag) const;

  /** Get the DataPoint defined by measures, if it exists. Similar to getDataPoints, but
   *  without accepting boost::optionals on input (and therefore SWIG-friendlier, but more limited
   *  in use). */
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
  //{

  bool setProblem(Problem& problem);

  bool setAlgorithm(Algorithm& algorithm);

  void clearAlgorithm();

  /** Set seed to newSeed. */
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

  /** Removes dataPoint from this analysis. Returns false if dataPoint is not in this analysis by
   *  UUID. */
  bool removeDataPoint(const DataPoint& dataPoint);

  /** Removes all dataPoints from this analysis, resets dataPointsAreInvalid flag, and
   *  reinitializes algorithm(), if it exists. */
  void removeAllDataPoints();

  /** Resets dataPoint to not point to any output data and to be incomplete and not failed. */
  bool clearResults(DataPoint& dataPoint);

  /** Resets all data points to not point to any output data and to be incomplete and not failed. */
  void clearAllResults();

  //@}
  /** @name Actions */
  //@{

  /** Sets run information on a DataPoint. Returns false if dataPoint is not in this analysis by
   *  UUID. Intention is for this method to be called by AnalysisDriver as soon as the dataPoint's
   *  topLevelJob has been queued. */
  bool setDataPointRunInformation(DataPoint& dataPoint, 
                                  const runmanager::Job& topLevelJob, 
                                  const std::vector<openstudio::path>& dakotaParametersFiles);

  /** Updates an existing DataPoint in this analysis. Throws an openstudio::Exception if DataPoint
   *  is not in this analysis, or if completedJob is not complete, or was not spawned by algorithm()
   *  acting on problem(). Should be called after a given workflow has been run. */
  void updateDataPoint(DataPoint& dataPoint,
                       const runmanager::Job& completedJob);

  /** Register Dakota's restart file location so that the analysis can be restarted if necessary. */
  void initializeDakotaAlgorithm(DakotaAlgorithm& dakotaAlgorithm,
                                 const runmanager::Job& dakotaJob);

  /** Updates the DakotaAlgorithm being used by this analysis. Thows an openstudio::Exception if
   *  DakotaAlgorithm is not being used by this analysis, or if completedJob is not complete, or
   *  does not correspond to the proper Dakota job. Should be called when the given Dakota job
   *  completes. */
  void updateDakotaAlgorithm(const runmanager::Job& completedDakotaJob);

  /** Returns a csv summary of all the data points in this analysis. */
  Table summaryTable() const;

  /** Relocate input path data from originalBase to newBase. Only updates file paths used to set
   *  up an analysis; paths that point to analysis results should be fixed up by a separate import
   *  process. */
  void updateInputPathData(const openstudio::path& originalBase,
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
 protected:
  /// @cond
  typedef detail::Analysis_Impl ImplType;

  friend class AnalysisObject;
  friend class detail::AnalysisObject_Impl;
  friend class detail::Analysis_Impl;

  explicit Analysis(boost::shared_ptr<detail::Analysis_Impl> impl);

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.analysis.Analysis");
};

/** \relates Analysis*/
typedef boost::optional<Analysis> OptionalAnalysis;

/** \relates Analysis*/
typedef std::vector<Analysis> AnalysisVector;

/** \relates Analysis */
ANALYSIS_API boost::optional<OpenStudioAlgorithm> getOpenStudioAlgorithm(const Analysis& analysis);

/** \relates Analysis */
ANALYSIS_API boost::optional<DakotaAlgorithm> getDakotaAlgorithm(const Analysis& analysis);

} // analysis
} // openstudio

#endif // ANALYSIS_ANALYSISDATA_HPP

