/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2016, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#ifndef ANALYSISDRIVER_ANALYSISDRIVER_HPP
#define ANALYSISDRIVER_ANALYSISDRIVER_HPP

#include "AnalysisDriverAPI.hpp"

#include "../utilities/core/Logger.hpp"

#include <boost/optional.hpp>

#include <vector>

namespace openstudio {

namespace analysis {
  class Analysis;
  class DataPoint;
}

namespace project {
  class ProjectDatabase;
  class AnalysisRecord;
}

namespace analysisdriver {

class AnalysisRunOptions;
class CurrentAnalysis;
class AnalysisStatus;

namespace detail {

  class AnalysisDriver_Impl;
  class CurrentAnalysis_Impl;
  class SimpleProject_Impl;

} // detail

/** AnalysisDriver uses a runmanager::RunManager and a project::ProjectDatabase to run and store
 *  the results of one or more analysis::Analyses. */
class ANALYSISDRIVER_API AnalysisDriver {
 public:
  /** @name Constructors and Destructors */
  //@{

  AnalysisDriver(project::ProjectDatabase& database);

  virtual ~AnalysisDriver() {}

  //@}
  /** @name Getters */
  //@{

  project::ProjectDatabase database() const;

  /** Returns CurrentAnalysis objects corresponding to analysis::Analyses that are actively
   *  running (right now). */
  std::vector<CurrentAnalysis> currentAnalyses() const;

  /** Tries to return the CurrentAnalysis that is monitoring analysis by matching Analysis UUIDs. */
  boost::optional<CurrentAnalysis> getCurrentAnalysis(const analysis::Analysis& analysis) const;

  //@}
  /** @name Run Management */
  //@{

  /** Runs analysis and saves results to database(). DataPoints to be simulated are determined in
   *  one of three ways:
   *
   *  <ol>
   *  <li> analysis may already contain dataPointsToBeQueued</li>
   *  <li> If analysis.algorithm().optionalCast<OpenStudioAlgorithm>(), then AnalysisDriver will
   *       call analysis.algorithm().cast<OpenStudioAlgorithm>().createNextIteration() as soon as
   *       all the dataPointsToBeQueued are complete.</li>
   *  <li> If analysis.algorithm().optionalCast<DakotaAlgorithm>(), then AnalysisDriver will
   *       kick off a runmanager::DakotaJob and create a new DataPoint from each parameter file
   *       dropped by Dakota. Depending on the circumstances, this may include an explicit restart
   *       of DAKOTA.</li>
   *  </ol>
   *
   *  This method will throw if analysis contains invalid data points or results. If this analysis
   *  is already running, this method will return the corresponding CurrentAnalysis already being
   *  held by this AnalysisDriver, and will log an info message. Otherwise, this method will start
   *  by clearing out results for any incomplete (not isComplete, but has a topLevelJob) DataPoints,
   *  and removing any incomplete DakotaAlgorithm jobs. */
  CurrentAnalysis run(analysis::Analysis& analysis, const AnalysisRunOptions& runOptions);

  /** Returns true if this AnalysisDriver is running (is actively queuing and monitoring jobs). */
  bool isRunning() const;

  AnalysisStatus status() const;

  /** Call this method between .run calls and code that should execute once the analysis is
   *  finished. If no argument is specified, will run to completion and return true. If a
   *  positive m_secs is specified, will block the process for m_secs, and give a return value
   *  of false. */
  bool waitForFinished(int m_secs = -1);

  /** Unpauses the RunManager queue and flips the value of isRunning() to true if there are 
   *  any currentAnalyses(). */
  void unpauseQueue();

  /** Stop a running analysis. */
  void stop(CurrentAnalysis& currentAnalysis);

  /** Stop a running data point. */
  void stop(analysis::DataPoint& dataPoint);

  /** Connect signal from this AnalysisDriver implementation object to slot on qObject. */
  bool connect(const std::string& signal,
               const QObject* qObject,
               const std::string& slot,
               Qt::ConnectionType type = Qt::AutoConnection) const;

  void moveToThread(QThread* targetThread);

 //@}
 protected:
  std::shared_ptr<detail::AnalysisDriver_Impl> getImpl() const;

  /// @cond
  typedef detail::AnalysisDriver_Impl ImplType;

  friend class detail::AnalysisDriver_Impl;
  friend class detail::CurrentAnalysis_Impl;
  friend class detail::SimpleProject_Impl;

  explicit AnalysisDriver(std::shared_ptr<detail::AnalysisDriver_Impl> impl);

  /// @endcond
 private:

  std::shared_ptr<detail::AnalysisDriver_Impl> m_impl;

  REGISTER_LOGGER("openstudio.analysisdriver.AnalysisDriver");
};

/** \relates AnalysisDriver*/
typedef boost::optional<AnalysisDriver> OptionalAnalysisDriver;

/** \relates AnalysisDriver*/
typedef std::vector<AnalysisDriver> AnalysisDriverVector;

/** Saves analysis to analysisDriver.projectDatabase(), unless analysis.resultsAreInvalid() or
 *  analysis.dataPointsAreInvalid(), in which case it throws. */
ANALYSISDRIVER_API project::AnalysisRecord saveAnalysis(analysis::Analysis& analysis,
                                                        AnalysisDriver& analysisDriver);

/** Removes dataPoint from analysis and the filesystem. Terminates and removes any corresponding
 *  RunManager jobs. DOES NOT register this change with the ProjectDatabase. */
ANALYSISDRIVER_API bool removeDataPoint(analysis::Analysis& analysis,
                                        analysis::DataPoint& dataPoint,
                                        AnalysisDriver& analysisDriver);

/** Removes all dataPoints from analysis and the filesystem. Terminates and removes any
 *  corresponding RunManager jobs. DOES NOT register this change with the ProjectDatabase. */
ANALYSISDRIVER_API bool removeAllDataPoints(analysis::Analysis& analysis,
                                            AnalysisDriver& analysisDriver);

/** Clears results from dataPoint and filesystem. Terminates and removes any corresponding
 *  RunManager jobs. DOES NOT register this change with the ProjectDatabase. */
ANALYSISDRIVER_API bool clearResults(analysis::Analysis& analysis,
                                     analysis::DataPoint& dataPoint,
                                     AnalysisDriver& analysisDriver);

/** Clears all results from analysis and the filesystem. Terminates and removes any corresponding
 *  RunManager jobs. DOES NOT register this change with the ProjectDatabase. Returns false if the
 *  process is incomplete, that is, if not all files could be removed from the file system. */
ANALYSISDRIVER_API bool clearAllResults(analysis::Analysis& analysis,
                                        AnalysisDriver& analysisDriver);

} // analysisdriver
} // openstudio

#endif // ANALYSISDRIVER_ANALYSISDRIVER_HPP

