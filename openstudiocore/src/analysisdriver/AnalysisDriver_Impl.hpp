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

