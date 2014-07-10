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

#ifndef ANALYSISDRIVER_ANALYSISDRIVER_IMPL_HPP
#define ANALYSISDRIVER_ANALYSISDRIVER_IMPL_HPP

#include "AnalysisDriverAPI.hpp"
#include "CurrentAnalysis.hpp"
#include "AnalysisDriverEnums.hpp"

#include "../project/ProjectDatabase.hpp"

#include "../runmanager/lib/FileInfo.hpp"
#include "../runmanager/lib/JobErrors.hpp"

#include <QObject>

#include <boost/regex.hpp>

namespace openstudio {

namespace analysis {
  class Analysis;
  class DataPoint;
}

namespace analysisdriver {

class AnalysisDriver;
class AnalysisRunOptions;

namespace detail {

  /** AnalysisDriver_Impl is the implementation class for AnalysisDriver.*/
  class ANALYSISDRIVER_API AnalysisDriver_Impl
      : public QObject,
        public std::enable_shared_from_this<AnalysisDriver_Impl>
  {
    Q_OBJECT;
   public:

    /** @name Constructors and Destructors */
    //@{

    AnalysisDriver_Impl(project::ProjectDatabase& database);

    virtual ~AnalysisDriver_Impl();

    //@}
    /** @name Getters */
    //@{

    project::ProjectDatabase database() const;

    std::vector<CurrentAnalysis> currentAnalyses() const;

    /** Tries to return the CurrentAnalysis by matching Analysis UUIDs. */
    boost::optional<CurrentAnalysis> getCurrentAnalysis(const analysis::Analysis& analysis) const;

    /** Get a public object that wraps this impl.*/
    AnalysisDriver getAnalysisDriver() const;

    //@}
    /** @name Run Management */
    //@{

    /** Runs analysis and saves results to database(). DataPoints to be simulated are determined in
     *  one of three ways:
     *
     *  <ol>
     *  <li> analysis may alreay contain dataPointsToBeQueued</li>
     *  <li> If analysis.algorithm().optionalCast<OpenStudioAlgorithm>(), then AnalysisDriver will
     *       call analysis.algorithm().cast<OpenStudioAlgorithm>().createNextIteration() as soon as
     *       all the dataPointsToBeQueued are complete.</li>
     *  <li> If analysis.algorithm().optionalCast<DakotaAlgorithm>(), then AnalysisDriver will
     *       kick off a runmanager::DakotaJob and create a new DataPoint from each paramter file
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

    bool isRunning() const;

    AnalysisStatus status() const;

    /** Call this method between .run calls and code that should execute once the analysis is
     *  finished. */
    bool waitForFinished(int m_secs);

    /** Unpauses the RunManager queue and notes whether this AnalysisDriver is actively running. */
    void unpauseQueue();

    /** Stop a running analysis. */
    void stop(CurrentAnalysis& currentAnalysis);

    /** Stop a running data point. */
    void stop(analysis::DataPoint& dataPoint);

    /// Connect signal from this AnalysisDriver implementation object to slot on qObject.
    bool connect(const std::string& signal,
                 const QObject* qObject,
                 const std::string& slot,
                 Qt::ConnectionType type = Qt::AutoConnection) const;

    //@}
   public slots:

    void dakotaJobOutputFileChanged(const openstudio::UUID& dakotaJob,
                                    const openstudio::runmanager::FileInfo& file);

    void jobTreeStateChanged(const openstudio::UUID& uuid);

    void dakotaJobComplete(const openstudio::UUID &uuid,
                           const openstudio::runmanager::JobErrors& jobErrors);

    void catchAnalysisCompleteOrStopped(const openstudio::UUID& analysis);

   signals:

    void resultsChanged();

    void dataPointQueued(const openstudio::UUID& analysis, const openstudio::UUID& dataPoint);

    void dataPointComplete(const openstudio::UUID& analysis, const openstudio::UUID& dataPoint);

    void dataPointStopped(const openstudio::UUID& analysis, const openstudio::UUID& dataPoint);

    void analysisComplete(const openstudio::UUID& analysis);

    void analysisStopped(const openstudio::UUID& analysis);

    void analysisStatusChanged(analysisdriver::AnalysisStatus newStatus);

   private:
    REGISTER_LOGGER("openstudio.analysisdriver.AnalysisDriver");

    bool m_running;
    std::vector<UUID> m_stopping;
    AnalysisStatus m_status;
    project::ProjectDatabase m_database;
    std::vector<CurrentAnalysis> m_currentAnalyses;
    
    void setStatus(AnalysisStatus status);

    void cleanOutIncompleteJobs(analysis::Analysis& analysis);

    void queueJobs(std::vector<CurrentAnalysis>::iterator& currentAnalysis);

    void startDakotaJob(CurrentAnalysis& currentAnalysis);

    void queueDakotaJob(CurrentAnalysis& currentAnalysis,
                        analysis::DataPoint& dataPoint,
                        const openstudio::path& dakotaParametersFile);

    std::vector<CurrentAnalysis>::iterator getCurrentAnalysisByQueuedJob(const openstudio::UUID& queuedJob);

    std::vector<CurrentAnalysis>::iterator getCurrentAnalysisByDakotaJob(const openstudio::UUID& dakotaJob);

    boost::optional<openstudio::runmanager::Job> getFinishedOrFailedParentJob(
        const openstudio::UUID& uuid) const;

    boost::regex dakotaParametersFilename() const;

    /** If dataPoint is empty, writes empty file to trigger Dakota error and stop dummy driver. */
    void writeDakotaResultsFile(const boost::optional<analysis::DataPoint>& dataPoint,
                                const openstudio::path& dakotaParametersFile);

    bool isAnalysisBeingStopped(const UUID& analysis) const;
  };

} // detail

} // analysisdriver
} // openstudio

#endif // ANALYSISDRIVER_ANALYSISDRIVER_IMPL_HPP

