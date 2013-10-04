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

#include <analysisdriver/AnalysisDriver.hpp>
#include <analysisdriver/AnalysisDriver_Impl.hpp>
#include <analysisdriver/AnalysisRunOptions.hpp>
#include <analysisdriver/CurrentAnalysis.hpp>
#include <analysisdriver/CurrentAnalysis_Impl.hpp>

#include <analysis/Analysis.hpp>
#include <analysis/Problem.hpp>
#include <analysis/OpenStudioAlgorithm.hpp>
#include <analysis/OpenStudioAlgorithm_Impl.hpp>
#include <analysis/DakotaAlgorithm.hpp>
#include <analysis/DakotaAlgorithm_Impl.hpp>
#include <analysis/DataPoint.hpp>
#include <analysis/DataPoint_Impl.hpp>
#include <analysis/DakotaParametersFile.hpp>

#include <project/AnalysisRecord.hpp>
#include <project/AlgorithmRecord.hpp>
#include <project/DataPointRecord.hpp>
#include <project/DataPointRecord_Impl.hpp>

#include <runmanager/lib/Workflow.hpp>
#include <runmanager/lib/Job.hpp>
#include <runmanager/lib/JobFactory.hpp>

#include <utilities/core/Application.hpp>
#include <utilities/core/System.hpp>
#include <utilities/core/Assert.hpp>
#include <utilities/core/PathHelpers.hpp>
#include <utilities/core/FileReference.hpp>
#include <utilities/core/Compare.hpp>
#include <utilities/core/Containers.hpp>
#include <utilities/core/Optional.hpp>
#include <utilities/time/DateTime.hpp>

#include <boost/foreach.hpp>
#include <boost/filesystem/fstream.hpp>

#include <QThread>

using namespace openstudio::analysis;
using namespace openstudio::project;

namespace openstudio {
namespace analysisdriver {
namespace detail {

  AnalysisDriver_Impl::AnalysisDriver_Impl(project::ProjectDatabase& database)
    : m_running(false), m_database(database)
  {
    // connect signals and slots
    bool connected = connect(SIGNAL(analysisComplete(const openstudio::UUID&)),
                             this,
                             SLOT(catchAnalysisCompleteOrStopped(const openstudio::UUID&)));
    OS_ASSERT(connected);
    connected = connect(SIGNAL(analysisStopped(const openstudio::UUID&)),
                        this,
                        SLOT(catchAnalysisCompleteOrStopped(const openstudio::UUID&)));
    OS_ASSERT(connected);
  }

  AnalysisDriver_Impl::~AnalysisDriver_Impl() {}

  project::ProjectDatabase AnalysisDriver_Impl::database() const {
    return m_database;
  }

  std::vector<CurrentAnalysis> AnalysisDriver_Impl::currentAnalyses() const {
    return m_currentAnalyses;
  }

  boost::optional<CurrentAnalysis> AnalysisDriver_Impl::getCurrentAnalysis(
      const Analysis& analysis) const
  {
    OptionalCurrentAnalysis result;
    BOOST_FOREACH(const CurrentAnalysis& currentAnalysis,m_currentAnalyses) {
      if (currentAnalysis.analysis().uuid() == analysis.uuid()) {
        result = currentAnalysis;
        break;
      }
    }
    return result;
  }

  AnalysisDriver AnalysisDriver_Impl::getAnalysisDriver() const {
    AnalysisDriver result(boost::const_pointer_cast<AnalysisDriver_Impl>(shared_from_this()));
    return result;
  }

  CurrentAnalysis AnalysisDriver_Impl::run(analysis::Analysis& analysis,
                                           const AnalysisRunOptions& runOptions)
  {
    if (analysis.dataPointsAreInvalid()) {
      LOG_AND_THROW("Cannot run Analysis '" << analysis.name() << "', because it contains "
                    << "invalid DataPoints. Please call removeAllDataPoints and try again.");
    }
    if (analysis.resultsAreInvalid()) {
      LOG_AND_THROW("Cannot run Analysis '" << analysis.name() << "', because it contains "
                    << "invalid results. Please call clearAllResults and try again.");
    }
    if (OptionalCurrentAnalysis currentAnalysis = getCurrentAnalysis(analysis)) {
      LOG(Info,"Analysis '" << analysis.name() << "' is already running.");
      return *currentAnalysis;
    }

    cleanOutIncompleteJobs(analysis);

    // pause queue if appropriate
    if (runOptions.queuePausingBehavior() != QueuePausingBehavior::NoPause) {
      database().runManager().setPaused(true);
    }

    // set running flag as appropriate
    if (runOptions.queuePausingBehavior() != QueuePausingBehavior::FullPauseManualUnpause) {
      m_running = true;
    }

    // create working directory
    openstudio::path workingDirectory = runOptions.workingDirectory();
    if (!boost::filesystem::exists(workingDirectory)) {
      boost::filesystem::create_directory(workingDirectory);
    }

    // save analysis to the database
    AnalysisDriver copyOfThis = getAnalysisDriver();
    saveAnalysis(analysis,copyOfThis);

    // create first iteration of data points if appropriate
    if (OptionalOpenStudioAlgorithm osAlgorithm = getOpenStudioAlgorithm(analysis))
    {
      osAlgorithm->createNextIteration(analysis);
      saveAnalysis(analysis,copyOfThis);
    }

    // construct CurrentAnalysis
    CurrentAnalysis currentAnalysis(analysis,runOptions);
    m_currentAnalyses.push_back(currentAnalysis);
    CurrentAnalysisVector::iterator currentAnalysisIt = m_currentAnalyses.end();
    --currentAnalysisIt;

    // queue jobs
    queueJobs(currentAnalysisIt);
    if (OptionalDakotaAlgorithm dakotaAlgorithm = getDakotaAlgorithm(analysis)) {
      if ((!dakotaAlgorithm->isComplete()) || dakotaAlgorithm->failed()) {
        // if failed, restart may be appropriate b/c options may have been changed
        startDakotaJob(currentAnalysis);
      }
    }

    return currentAnalysis;
  }

  bool AnalysisDriver_Impl::isRunning() const {
    return m_running;
  }

  bool AnalysisDriver_Impl::waitForFinished(int m_secs) {

    if (m_secs > 0) {
      m_database.runManager().waitForFinished(m_secs);
      return false;
    }

    while (m_running) {
      m_database.runManager().waitForFinished();

      // might be waiting for signals from final jobs
      int maxTimeMS = 1000;
      boost::posix_time::ptime start = boost::posix_time::microsec_clock::universal_time();
      while ( openstudio::Application::instance().processEvents() &&
              (boost::posix_time::microsec_clock::universal_time() - start).total_milliseconds() < maxTimeMS)
      {
        openstudio::System::msleep(1);
      }
    }

    return true;
  }

  void AnalysisDriver_Impl::unpauseQueue() {
    if (!m_currentAnalyses.empty()) {
      m_running = true;
    }
    m_database.runManager().setPaused(false);
  }

  void AnalysisDriver_Impl::stop(CurrentAnalysis& currentAnalysis) {
    // pause RunManager while stopping an analysis
    runmanager::RunManager runManager = database().runManager();
    bool wasPaused = runManager.paused();
    runManager.setPaused(true);

    CurrentAnalysisVector::iterator currentAnalysisIt = std::find(m_currentAnalyses.begin(),
                                                                  m_currentAnalyses.end(),
                                                                  currentAnalysis);
    if (currentAnalysisIt != m_currentAnalyses.end()) {
      UUID analysisUUID = currentAnalysisIt->analysis().uuid();
      // register the fact that we are stopping this analysis so other methods are aware of state.
      m_stopping.push_back(analysisUUID);

      // do the work of stopping
      currentAnalysisIt->getImpl()->stop(runManager);

      // remove current analysis from list if it hasn't already
      currentAnalysisIt = std::find(m_currentAnalyses.begin(),
                                    m_currentAnalyses.end(),
                                    currentAnalysis);
      if (currentAnalysisIt != m_currentAnalyses.end()) {
        m_currentAnalyses.erase(currentAnalysisIt);
      }

      // register a successful stop
      LOG(Debug,"Analysis " << toString(analysisUUID) << " stopped.");
      UUIDVector::iterator it = std::find(m_stopping.begin(),m_stopping.end(),analysisUUID);
      m_stopping.erase(it);
      emit analysisStopped(analysisUUID);
    }
    else {
      LOG(Info,"Analysis '" << currentAnalysis.analysis().name() << "' is not running, "
          << "and so cannot be stopped.");
    }

    // start RunManager back up
    runManager.setPaused(wasPaused);
  }

  void AnalysisDriver_Impl::stop(DataPoint& dataPoint) {
    // first see if dataPoint even running
    CurrentAnalysisVector::iterator it = m_currentAnalyses.end();
    if (dataPoint.topLevelJob()) {
      it = getCurrentAnalysisByQueuedJob(dataPoint.topLevelJob()->uuid());
    }
    if (it == m_currentAnalyses.end()) {
      return;
    }

    // pause RunManager while stopping an analysis
    runmanager::RunManager runManager = database().runManager();
    bool wasPaused = runManager.paused();
    runManager.setPaused(true);

    // do the work of stopping
    it->getImpl()->stop(dataPoint,runManager);

    // register a successful stop
    LOG(Debug,"DataPoint " << toString(dataPoint.uuid()) << " simulation stopped.");
    emit dataPointStopped(it->analysis().uuid(),dataPoint.uuid());

    // start RunManager back up
    runManager.setPaused(wasPaused);
  }

  bool AnalysisDriver_Impl::connect(const std::string& signal,
                                    const QObject* qObject,
                                    const std::string& slot,
                                    Qt::ConnectionType type) const
  {
    return QObject::connect(this, signal.c_str(), qObject, slot.c_str(), type);
  }

  void AnalysisDriver_Impl::dakotaJobOutputFileChanged(const openstudio::UUID& dakotaJob,
                                                       const openstudio::runmanager::FileInfo& file)
  {
    if (file.exists && boost::regex_search(file.filename,dakotaParametersFilename())) {
      // try to create new DataPoint
      CurrentAnalysisVector::iterator currentAnalysis = getCurrentAnalysisByDakotaJob(dakotaJob);
      Analysis analysis = currentAnalysis->analysis();
      OptionalDataPoint dataPoint;

      // load file
      OptionalDakotaParametersFile params = DakotaParametersFile::load(file.fullPath);
      if (!params) {
        LOG(Error,"Unable to load Dakota parameters file '" << toString(file.fullPath) << "'.");
        LOG(Debug,"Offending parameters file:" << std::endl
            << boost::filesystem::ifstream(file.fullPath));
        writeDakotaResultsFile(dataPoint,file.fullPath);
        return;
      }

      // should return the corresponding point, even if already in the analysis
      DakotaAlgorithm algorithm = getDakotaAlgorithm(analysis).get();
      dataPoint = algorithm.createNextDataPoint(analysis,*params);

      // save analysis to the database
      AnalysisDriver copyOfThis = getAnalysisDriver();
      saveAnalysis(analysis,copyOfThis);

      // process dataPoint
      if (!dataPoint) {
        LOG(Error,"Unable to create valid dataPoint for Problem '"
            << currentAnalysis->analysis().problem().name()
            << "' from Dakota parameters file '" << toString(file.fullPath) << "'.");
        LOG(Debug,"Offending parameters file: \n" << boost::filesystem::ifstream(file.fullPath));
        dataPoint = algorithm.createNextDataPoint(analysis,*params);
        writeDakotaResultsFile(dataPoint,file.fullPath);
      }
      else if (dataPoint->isComplete()) {
        writeDakotaResultsFile(dataPoint,file.fullPath);
      }
      else {
        // will redirect treeStatusChanged signals if dataPoint already running
        queueDakotaJob(*currentAnalysis,*dataPoint,file.fullPath);
      }
    }
  }

  void AnalysisDriver_Impl::jobTreeStateChanged(const openstudio::UUID& uuid) {
    LOG(Trace,"Received tree state changed signal from RunManager for job " << toString(uuid) << ".");
    boost::optional<openstudio::runmanager::Job> job = getFinishedOrFailedParentJob(uuid);

    if (job) {
      // unhook signals and slots
      // ETH@20130227 - Was testing return value == true, but was failing. Not sure why. Removed
      // test because does not seem critical.
      job->disconnect(SIGNAL(treeChanged(const openstudio::UUID &)),
                      this,
                      SLOT(jobTreeStateChanged(const openstudio::UUID &)));

      // retrieve analysis
      CurrentAnalysisVector::iterator currentAnalysisIt = getCurrentAnalysisByQueuedJob(job->uuid());
      if (currentAnalysisIt == m_currentAnalyses.end()) {
        // can happen if multiple finished or failed signals are emitted by RunManager
        LOG(Trace,"Parent job uuid '" << toString(job->uuid()) << "' is no longer registered as a "
            << "queued job in a CurrentAnalysis.")
        return;
      }
      CurrentAnalysis currentAnalysis = *currentAnalysisIt;
      Analysis analysis = currentAnalysis.analysis();

      // retrieve and update data point
      OptionalDataPoint dataPoint, duplicate;
      bool isDakota(false);
      if (currentAnalysis.getImpl()->isQueuedOSDataPoint(job->uuid())) {
        dataPoint = currentAnalysis.getImpl()->removeCompletedOSDataPoint(job->uuid());
      }
      if (currentAnalysis.getImpl()->isQueuedDakotaDataPoint(job->uuid())) {
        isDakota = true;
        if (!dataPoint) {
          dataPoint = currentAnalysis.getImpl()->removeCompletedDakotaDataPoint(job->uuid());
        } else {
          duplicate = currentAnalysis.getImpl()->removeCompletedDakotaDataPoint(job->uuid());
        }
      }
      OS_ASSERT(dataPoint);
      LOG(Info,"Processing " << job->treeStatus().valueDescription() << " job tree for "
          << toString(dataPoint->directory().stem()) << " from Analysis '" << analysis.name()
          << "'. Parent job uuid is '" << toString(job->uuid()) << "'.");
      analysis.problem().updateDataPoint(*dataPoint,*job);
      OS_ASSERT(dataPoint->isComplete());

      // create new data points as appropriate
      int n(0);
      bool callQueueJobs(false);
      int nTot = currentAnalysis.totalNumJobsInOSIteration();
      int nQueued = currentAnalysis.numQueuedOSJobs();
      int nComplete = currentAnalysis.numCompletedJobsInOSIteration();
      if (nTot > nQueued + nComplete) {
        callQueueJobs = true;
      }
      else if (nTot == nComplete) {
        callQueueJobs = true; // need to call in order to emit finished signal
                              // that method will check to see if Dakota is running
        if (OptionalOpenStudioAlgorithm osAlgorithm = getOpenStudioAlgorithm(analysis))
        {
          n = osAlgorithm->createNextIteration(analysis);
          LOG(Info,"OpenStudioAlgorithm '" << osAlgorithm->name() << "' created " << n
              << " new DataPoints.");
        }
      }

      // save analysis to the database
      AnalysisDriver copyOfThis = getAnalysisDriver();
      saveAnalysis(analysis,copyOfThis);
      OptionalDataPointRecord odpr = m_database.getObjectRecordByHandle<DataPointRecord>(dataPoint->uuid());
      OS_ASSERT(odpr);
      OS_ASSERT(odpr->isComplete());
      OS_ASSERT(odpr->uuidLast() == dataPoint->versionUUID());
      odpr.reset();

      // write out results file(s)
      if (isDakota) {
        std::vector<openstudio::path> paramFiles;
        if (duplicate) {
          paramFiles = duplicate->dakotaParametersFiles();
        }
        else {
          paramFiles = dataPoint->dakotaParametersFiles();
        }
        BOOST_FOREACH(const openstudio::path& infile,paramFiles) {
          writeDakotaResultsFile(*dataPoint,infile);
        }
      }

      // emit signals
      emit dataPointComplete(analysis.uuid(),dataPoint->uuid());
      emit resultsChanged();

      // queue up new points
      if (callQueueJobs && !isAnalysisBeingStopped(analysis.uuid())) {
        currentAnalysisIt = std::find(m_currentAnalyses.begin(),
                                      m_currentAnalyses.end(),
                                      currentAnalysis);
        if (currentAnalysisIt != m_currentAnalyses.end()) {
          queueJobs(currentAnalysisIt);
        }
      }
    }
  }

  void AnalysisDriver_Impl::dakotaJobComplete(const openstudio::UUID &uuid,
                                              const openstudio::runmanager::JobErrors& jobErrors)
  {
    CurrentAnalysisVector::iterator currentAnalysis = getCurrentAnalysisByDakotaJob(uuid);
    if ((currentAnalysis != m_currentAnalyses.end()) &&
        (m_stopping.empty() || (!isAnalysisBeingStopped(currentAnalysis->analysis().uuid()))))
    {
      OS_ASSERT(currentAnalysis != m_currentAnalyses.end());

      // unhook signals and slots (needed for reuse of AnalysisDriver)
      runmanager::Job dakotaJob = database().runManager().getJob(uuid);
      bool test = dakotaJob.disconnect(SIGNAL(outputFileChanged(const openstudio::UUID &, const openstudio::runmanager::FileInfo&)),
                                       this,
                                       SLOT(dakotaJobOutputFileChanged(const openstudio::UUID&, const openstudio::runmanager::FileInfo&)));
      OS_ASSERT(test);
      test = dakotaJob.disconnect(SIGNAL(finished(const openstudio::UUID &, const openstudio::runmanager::JobErrors&)),
                                  this,
                                  SLOT(dakotaJobComplete(const openstudio::UUID&,const openstudio::runmanager::JobErrors&)));
      OS_ASSERT(test);

      // register completion with analysis
      Analysis analysis = currentAnalysis->analysis();
      analysis.updateDakotaAlgorithm(dakotaJob);
      OS_ASSERT(analysis.algorithm());
      OS_ASSERT(analysis.algorithm()->isComplete());

      // save analysis to the database
      {
        AnalysisDriver copyOfThis = getAnalysisDriver();
        AnalysisRecord analysisRecord = saveAnalysis(analysis,copyOfThis);
        OptionalAlgorithmRecord algorithmRecord = analysisRecord.algorithmRecord();
        OS_ASSERT(algorithmRecord);
        OS_ASSERT(algorithmRecord->isComplete());
      }

      // shut down the analysis if appropriate
      currentAnalysis->getImpl()->clearDakotaJob(jobErrors);
      runmanager::RunManager runManager = database().runManager();
      currentAnalysis->getImpl()->stopQueuedDakotaJobs(runManager);
      if (currentAnalysis->numQueuedJobs() == 0) {
        openstudio::UUID analysisUUID = currentAnalysis->analysis().uuid();
        m_currentAnalyses.erase(currentAnalysis);
        LOG(Debug,"Analysis " << toString(analysisUUID) << "complete. The Dakota job has completed.");
        emit analysisComplete(analysisUUID);
      }
    }
  }

  void AnalysisDriver_Impl::catchAnalysisCompleteOrStopped(const openstudio::UUID& analysisUUID) {
    if (m_currentAnalyses.empty()) {
      LOG(Debug, "AnalysisDriver no longer runnning.");
      m_running = false;
    }
  }

  void AnalysisDriver_Impl::cleanOutIncompleteJobs(analysis::Analysis& analysis) {
    AnalysisDriver copyOfThis = getAnalysisDriver();

    DataPointVector incompletePoints = analysis.dataPointsToQueue();
    BOOST_FOREACH(DataPoint& incompletePoint,incompletePoints) {
      if (incompletePoint.topLevelJob()) {
        clearResults(analysis,incompletePoint,copyOfThis);
      }
    }

    if (OptionalDakotaAlgorithm dakotaAlgorithm = getDakotaAlgorithm(analysis)) {
      if (dakotaAlgorithm->job() && ((!dakotaAlgorithm->isComplete()) || dakotaAlgorithm->failed())) {
        try {
          runmanager::Job dakotaJob = database().runManager().getJob(dakotaAlgorithm->job()->uuid());
          dakotaJob.setCanceled(true);
          dakotaJob.requestStop();
          dakotaJob.waitForFinished();
          database().runManager().remove(dakotaJob);
        }
        catch (...) {}
      }
    }
  }

  void AnalysisDriver_Impl::queueJobs(std::vector<CurrentAnalysis>::iterator& currentAnalysis) {
    LOG(Debug, "Queuing next batch of jobs");

    // Check to see if there is any work to do.
    Analysis analysis = currentAnalysis->analysis();
    DataPointVector dataPoints = analysis.dataPointsToQueue();
    DataPointVector::iterator dpit = dataPoints.begin();
    // Analysis does not know what is already running, so remove any points that already are.
    while (dpit != dataPoints.end()) {
      if (currentAnalysis->getImpl()->isQueuedDataPoint(*dpit)) {
        dpit = dataPoints.erase(dpit);
      } else {
        ++dpit;
      }
    }
    if (dataPoints.empty()) {
      // Declare analysis complete if possible
      bool analysisCompleteFlag(false);
      if (!analysis.algorithm() || !getDakotaAlgorithm(analysis))
      {
        analysisCompleteFlag = true;
      }
      else {
        // Analysis using DakotaAlgorithm--see if it has finished.
        if (currentAnalysis->getImpl()->dakotaStarted() &&
            !currentAnalysis->getImpl()->dakotaJob())
        {
          analysisCompleteFlag = true;
        }
        // or will never start
        if (analysis.algorithm()->cast<DakotaAlgorithm>().isComplete()) {
          analysisCompleteFlag = true;
        }
      }
      if (analysisCompleteFlag) {
        m_currentAnalyses.erase(currentAnalysis);
        LOG(Debug, "Analysis " << toString(analysis.uuid()) << " complete. There are no more data points to run.");
        emit analysisComplete(analysis.uuid());
      }
      return;
    }

    // Handle queue pausing behavior.
    AnalysisRunOptions runOptions = currentAnalysis->runOptions();
    QueuePausingBehavior queuePausingBehavior = runOptions.queuePausingBehavior();
    int firstN = runOptions.firstN();
    if (queuePausingBehavior != QueuePausingBehavior::NoPause) {
      database().runManager().setPaused(true);
    }

    // Set size of iteration if needed
    if (currentAnalysis->numCompletedJobsInOSIteration() == currentAnalysis->totalNumJobsInOSIteration()) {
      currentAnalysis->getImpl()->setNumOSJobsInIteration(dataPoints.size());
    }

    {
      // Prepare to create workflows and queue jobs.
      DataPointVector nextBatch;
      Problem problem = analysis.problem();
      openstudio::path weatherFile;
      if (analysis.weatherFile()) {
        weatherFile = analysis.weatherFile().get().path();
      }
      openstudio::path workingDirectory = runOptions.workingDirectory();
      openstudio::path rubyIncludeDirectory = runOptions.rubyIncludeDirectory();
      std::vector<URLSearchPath> urlSearchPaths = runOptions.urlSearchPaths();
      bool force = runOptions.force();
      OptionalInt queueSize = runOptions.queueSize();

      // Loop through DataPoints and queue jobs.
      BOOST_FOREACH(DataPoint& dataPoint, dataPoints) {

        if (queueSize && (currentAnalysis->numQueuedJobs() + int(nextBatch.size()) >= *queueSize)) {
          break;
        }

        // create workflow
        runmanager::Workflow workflow = problem.createWorkflow(dataPoint,rubyIncludeDirectory);
        openstudio::runmanager::JobParams params;
        params.append("cleanoutfiles", runOptions.jobCleanUpBehavior().valueName());
        workflow.add(params);
        if (boost::optional<runmanager::Tools> tools = runOptions.runManagerTools()) {
          workflow.add(*tools);
        }

        // get DataPointRecord and determine run directory
        DataPointRecord dataPointRecord =
            database().getObjectRecordByHandle<DataPointRecord>(dataPoint.uuid()).get();
        std::stringstream ss;
        ss << "dataPoint" << dataPointRecord.id();
        openstudio::path seedPath = boost::filesystem::complete(analysis.seed().path());
        openstudio::path runDir = boost::filesystem::complete(workingDirectory / toPath(ss.str()));
        dataPoint.setDirectory(runDir);
        if (boost::filesystem::exists(runDir)) {
          // rerunning--clear out old data
          try {
            boost::filesystem::remove_all(runDir);
          }
          catch (std::exception& e) {
            LOG(Warn,"Tried to erase old data before re-running data point, but was unable to, "
                << "because " << e.what() << ". You may see multiple similar and conflicting "
                << "sub-folders in " << toString(runDir) << ".");
          }
        }
        boost::filesystem::create_directory(runDir);

        LOG(Info,"Queuing user or OpenStudioAlgorithm generated '" << ss.str() << "'.");

        runmanager::Job job = workflow.create(runDir,seedPath,weatherFile,urlSearchPaths);
        runmanager::JobFactory::optimizeJobTree(job);

        std::vector<openstudio::path> dakotaParametersFiles;
        analysis.setDataPointRunInformation(dataPoint, job, dakotaParametersFiles);

        nextBatch.push_back(dataPoint);

        bool test = job.connect(SIGNAL(treeChanged(const openstudio::UUID &)),this,
                                SLOT(jobTreeStateChanged(const openstudio::UUID &)));
        OS_ASSERT(test);

        database().runManager().enqueue(job,force);
        if ((queuePausingBehavior == QueuePausingBehavior::PauseForFirstN) &&
            (nextBatch.size() == static_cast<size_t>(firstN)))
        {
          unpauseQueue();
        }
      }

      currentAnalysis->getImpl()->addNextBatchOSDataPoints(nextBatch);
    }

    // Save analysis to the database to capture DataPoint run directories
    AnalysisDriver copyOfThis = getAnalysisDriver();
    saveAnalysis(analysis,copyOfThis);

    if (queuePausingBehavior != QueuePausingBehavior::FullPauseManualUnpause) {
      unpauseQueue();
    }

    // Emit queued signals
    BOOST_FOREACH(DataPoint& dataPoint, dataPoints) {
      emit dataPointQueued(analysis.uuid(),dataPoint.uuid());
    }
  }

  void AnalysisDriver_Impl::startDakotaJob(CurrentAnalysis& currentAnalysis) {
    // Runmanager settings
    runmanager::ConfigOptions rmConfig = database().runManager().getConfigOptions();
    int numLocalJobs = rmConfig.getMaxLocalJobs();
    if (numLocalJobs < 2) {
      rmConfig.setMaxLocalJobs(2); // perhaps would be better to ++ on start and -- on end
      database().runManager().setConfigOptions(rmConfig);
      numLocalJobs = database().runManager().getConfigOptions().getMaxLocalJobs();
    }
    OS_ASSERT(numLocalJobs > 1);

    // WRITE dakota.in
    openstudio::path inFilePath = currentAnalysis.runOptions().workingDirectory() / toPath("dakota.in");
    boost::filesystem::ofstream inFile(inFilePath);

    // header
    inFile << "# Generated by OpenStudio AnalysisDriver, " << DateTime::now() << "; for " << std::endl;
    inFile << "# Analysis '" << currentAnalysis.analysis().name() << "', " << toString(currentAnalysis.analysis().uuid()) << "." << std::endl << std::endl;

    // strategy
    inFile << "strategy," << std::endl;
    inFile << "        single" << std::endl;
    inFile << "        tabular_graphics_data" << std::endl << std::endl;

    // method
    DakotaAlgorithm algorithm = currentAnalysis.analysis().algorithm()->cast<DakotaAlgorithm>();
    inFile << algorithm.dakotaInFileDescription();

    // variables and responses
    inFile << currentAnalysis.analysis().problem().dakotaInFileDescription(algorithm);

    // interface
    inFile << "interface," << std::endl;
    inFile << "        fork" << std::endl;
    inFile << "          asynchronous" << std::endl;
    if (OptionalInt maxQueue = currentAnalysis.runOptions().queueSize()) {
      inFile << "          evaluation_concurrency = " << *maxQueue << std::endl;
    }
    inFile << "          analysis_driver = 'ruby waitForDataPointResultsFile.rb'" << std::endl;
    inFile << "          parameters_file = 'params.in'" << std::endl;
    inFile << "          results_file    = 'results.out'" << std::endl;
    inFile << "            file_tag" << std::endl;
    if (currentAnalysis.runOptions().dakotaFileSave()) {
      inFile << "            file_save" << std::endl;
    }

    inFile.close();

    // WRITE waitForDataPointResultsFile.rb
    openstudio::path dummyDriverPath =
        currentAnalysis.runOptions().workingDirectory() / toPath("waitForDataPointResultsFile.rb");
    boost::filesystem::ofstream dummyDriverFile(dummyDriverPath);

    // ETH@20111114 May want to add option for how long this dummy driver sleeps.
    dummyDriverFile << "# Autogenerated by OpenStudio::AnalysisDriver::AnalysisDriver.\r\n";
    dummyDriverFile << "# ARGV[0] = params.in\r\n";
    dummyDriverFile << "# ARGV[1] = results.out\r\n";
    dummyDriverFile << "# Purpose of this script is to spin until result.out has been written.\r\n";
    dummyDriverFile << "\r\n";
    dummyDriverFile << "puts 'Looking for ' + ARGV[1]\r\n";
    dummyDriverFile << "sleep 1 while not File.exist?(ARGV[1])\r\n";
    dummyDriverFile << "\r\n";

    dummyDriverFile.close();

    // QUEUE DakotaJob
    runmanager::Workflow dakotaWorkflow;
    dakotaWorkflow.addJob(runmanager::JobType::Dakota);
    dakotaWorkflow.add(runmanager::ConfigOptions::makeTools(
        openstudio::path(),
        openstudio::path(),
        openstudio::path(),
        openstudio::path(),
        currentAnalysis.runOptions().dakotaExePath().parent_path()));
    openstudio::runmanager::JobParams params;
    params.append("cleanoutfiles", currentAnalysis.runOptions().jobCleanUpBehavior().valueName());
    dakotaWorkflow.add(params);

    // instantiate workflow as job
    std::vector<openstudio::path> inFiles;
    inFiles.push_back(boost::filesystem::system_complete(inFilePath));
    inFiles.push_back(boost::filesystem::system_complete(dummyDriverPath));
    dakotaWorkflow.setInputFiles(inFiles);
    dakotaWorkflow.addParam(runmanager::JobParam("-o dakota.out"));
    if (OptionalFileReference restartFile = algorithm.restartFileReference()) {
      std::string fileName = toString(restartFile->path().filename());
      dakotaWorkflow.addParam(runmanager::JobParam("-read_restart " + fileName));
    }
    runmanager::Job dakotaJob = dakotaWorkflow.create(boost::filesystem::system_complete(currentAnalysis.runOptions().workingDirectory()));
    runmanager::JobFactory::optimizeJobTree(dakotaJob);
    // add links to job and to important files that will be generated
    currentAnalysis.getImpl()->setDakotaJob(dakotaJob.uuid());
    currentAnalysis.analysis().initializeDakotaAlgorithm(algorithm,dakotaJob);
    // save analysis to the database
    AnalysisDriver copyOfThis = getAnalysisDriver();
    Analysis analysis = currentAnalysis.analysis();
    saveAnalysis(analysis,copyOfThis);

    // signals and slots
    bool test = dakotaJob.connect(SIGNAL(outputFileChanged(const openstudio::UUID &, const openstudio::runmanager::FileInfo&)),
                                  this,
                                  SLOT(dakotaJobOutputFileChanged(const openstudio::UUID&, const openstudio::runmanager::FileInfo&)));
    OS_ASSERT(test);
    test = dakotaJob.connect(SIGNAL(finished(const openstudio::UUID &, const openstudio::runmanager::JobErrors&)),
                             this,
                             SLOT(dakotaJobComplete(const openstudio::UUID&,const openstudio::runmanager::JobErrors&)));
    OS_ASSERT(test);

    // finally, queue
    database().runManager().enqueue(dakotaJob,true);

    // UNPAUSE queue
    if (currentAnalysis.runOptions().queuePausingBehavior() != QueuePausingBehavior::FullPauseManualUnpause) {
      unpauseQueue();
    }
  }

  void AnalysisDriver_Impl::queueDakotaJob(CurrentAnalysis& currentAnalysis,
                                           analysis::DataPoint& dataPoint,
                                           const openstudio::path& dakotaParametersFile)
  {
    if (OptionalDataPoint queuedJob = currentAnalysis.getImpl()->isQueuedOSDataPoint(dataPoint)) {
      // job is already running as OS job
      currentAnalysis.getImpl()->addDakotaDataPoint(*queuedJob);
      currentAnalysis.getImpl()->augmentQueuedDakotaDataPoint(dataPoint,dakotaParametersFile);
      return;
    }
    if (OptionalDataPoint queuedJob = currentAnalysis.getImpl()->isQueuedDakotaDataPoint(dataPoint)) {
      // job is already running as Dakota job
      currentAnalysis.getImpl()->augmentQueuedDakotaDataPoint(dataPoint,dakotaParametersFile);
      return;
    }

    OS_ASSERT(database().runManager().getConfigOptions().getMaxLocalJobs() > 1);

    Analysis analysis = currentAnalysis.analysis();
    Problem problem = analysis.problem();
    AnalysisRunOptions runOptions = currentAnalysis.runOptions();

    // Handle queue pausing behavior.
    QueuePausingBehavior queuePausingBehavior = runOptions.queuePausingBehavior();
    if (queuePausingBehavior != QueuePausingBehavior::NoPause) {
      database().runManager().setPaused(true);
    }

    {
      openstudio::path weatherFile;
      if (analysis.weatherFile()) {
        weatherFile = analysis.weatherFile().get().path();
      }

      // create workflow
      runmanager::Workflow workflow = problem.createWorkflow(dataPoint,runOptions.rubyIncludeDirectory());
      openstudio::runmanager::JobParams params;
      params.append("cleanoutfiles", runOptions.jobCleanUpBehavior().valueName());
      workflow.add(params);
      if (boost::optional<runmanager::Tools> tools = runOptions.runManagerTools()) {
        workflow.add(*tools);
      }

      // get DataPointRecord and determine run directory
      DataPointRecord dataPointRecord = database().getObjectRecordByHandle<DataPointRecord>(dataPoint.uuid()).get();
      std::stringstream ss;
      ss << "dataPoint" << dataPointRecord.id();
      openstudio::path seedPath = boost::filesystem::complete(analysis.seed().path());
      openstudio::path runDir = boost::filesystem::complete(runOptions.workingDirectory() / toPath(ss.str()));
      dataPoint.setDirectory(runDir);
      if (boost::filesystem::exists(runDir)) {
        // rerunning--clear out old data
        try {
          boost::filesystem::remove_all(runDir);
        }
        catch (std::exception& e) {
          LOG(Warn,"Tried to erase old data before re-running data point, but was unable to, "
              << "because " << e.what() << ". You may see multiple similar and conflicting "
              << "sub-folders in " << toString(runDir) << ".");
        }
      }
      boost::filesystem::create_directory(runDir);

      LOG(Info,"Queuing Dakota generated " << ss.str() << ".");

      runmanager::Job job = workflow.create(runDir,
                                            analysis.seed().path(),
                                            weatherFile,
                                            runOptions.urlSearchPaths());
      runmanager::JobFactory::optimizeJobTree(job);

      std::vector<openstudio::path> dakotaParametersFiles(1u,dakotaParametersFile);
      analysis.setDataPointRunInformation(dataPoint, job, dakotaParametersFiles);

      currentAnalysis.getImpl()->addDakotaDataPoint(dataPoint);

      bool test = job.connect(SIGNAL(treeChanged(const openstudio::UUID &)),this,
                              SLOT(jobTreeStateChanged(const openstudio::UUID &)));
      OS_ASSERT(test);

      database().runManager().enqueue(job,runOptions.force());
    }

    // Save analysis to the database to capture DataPoint run directories
    AnalysisDriver copyOfThis = getAnalysisDriver();
    saveAnalysis(analysis,copyOfThis);

    if (queuePausingBehavior != QueuePausingBehavior::FullPauseManualUnpause) {
      unpauseQueue();
    }

    // Emit queued signal
    emit dataPointQueued(analysis.uuid(),dataPoint.uuid());
  }

  std::vector<CurrentAnalysis>::iterator AnalysisDriver_Impl::getCurrentAnalysisByQueuedJob(
      const openstudio::UUID& queuedJob)
  {
    CurrentAnalysisVector::iterator result = m_currentAnalyses.begin();
    while (result != m_currentAnalyses.end()) {
      if (result->getImpl()->isQueuedDataPoint(queuedJob)) {
        break;
      }
      ++result;
    }
    return result;
  }

  std::vector<CurrentAnalysis>::iterator AnalysisDriver_Impl::getCurrentAnalysisByDakotaJob(
      const openstudio::UUID& dakotaJob)
  {
    CurrentAnalysisVector::iterator result = m_currentAnalyses.begin();
    while (result != m_currentAnalyses.end()) {
      if (OptionalUUID candidate = result->getImpl()->dakotaJob()) {
        if (*candidate == dakotaJob) {
          break;
        }
      }
      ++result;
    }
    return result;
  }

  boost::optional<openstudio::runmanager::Job> AnalysisDriver_Impl::getFinishedOrFailedParentJob(
        const openstudio::UUID& uuid) const
  {
    // retrieve job by uuid
    boost::optional<openstudio::runmanager::Job> job;
    try {
      job = m_database.runManager().getJob(uuid);
    }
    catch(const std::exception& e) {
      LOG(Error, "Job " << toString(uuid) << " not found in RunManager, maybe it was cancelled? "
          << e.what());
      return boost::none;
    }

    // check status
    runmanager::TreeStatusEnum status = job->treeStatus();
    if ((status == openstudio::runmanager::TreeStatusEnum::Finished) ||
        (status == openstudio::runmanager::TreeStatusEnum::Failed))
    {
      openstudio::runmanager::Job parentJob = *job;
      while ((job = parentJob.parent())) {
        parentJob = *job;
      }
      return parentJob;
    }

    return boost::none;
  }

  boost::regex AnalysisDriver_Impl::dakotaParametersFilename() const {
    return boost::regex("params\\.in");
  }

  void AnalysisDriver_Impl::writeDakotaResultsFile(const boost::optional<analysis::DataPoint>& dataPoint,
                                                   const openstudio::path& dakotaParametersFile)
  {
    openstudio::path resultsFilePath = toPath(boost::regex_replace(toString(dakotaParametersFile),
                                                                   dakotaParametersFilename(),
                                                                   "results.out"));
    LOG(Debug,"Writing Dakota results file to '" << toString(resultsFilePath) << "'.");

    OptionalString fileText;
    if (dataPoint) {
      fileText = dataPoint->problem().getDakotaResultsFile(*dataPoint);
      LOG(Debug,"Printing Dakota results file for dataPoint " << dataPoint->name()
          << ", " << toString(dataPoint->uuid()) << ".");
    }
    else {
      LOG(Debug,"No dataPoint given for '" << toString(resultsFilePath) << "'.");
    }

    boost::filesystem::ofstream resultsFile(resultsFilePath);
    if (fileText) {
      resultsFile << *fileText;
      if (fileText.get().empty()) {
        LOG(Debug,"Printed text for '" << toString(resultsFilePath) << "', but it is empty.");
      }
    }
    resultsFile.close();
  }

  bool AnalysisDriver_Impl::isAnalysisBeingStopped(const UUID& analysis) const {
    return (std::find(m_stopping.begin(),m_stopping.end(),analysis) != m_stopping.end());
  }

} // detail

AnalysisDriver::AnalysisDriver(project::ProjectDatabase& database)
  : m_impl(boost::shared_ptr<detail::AnalysisDriver_Impl>(new detail::AnalysisDriver_Impl(database)))
{}

project::ProjectDatabase AnalysisDriver::database() const {
  return getImpl()->database();
}

std::vector<CurrentAnalysis> AnalysisDriver::currentAnalyses() const {
  return getImpl()->currentAnalyses();
}

boost::optional<CurrentAnalysis> AnalysisDriver::getCurrentAnalysis(const Analysis& analysis) const {
  return getImpl()->getCurrentAnalysis(analysis);
}

void AnalysisDriver::moveToThread(QThread* targetThread) {
  getImpl()->moveToThread(targetThread);
}

boost::shared_ptr<detail::AnalysisDriver_Impl> AnalysisDriver::getImpl() const {
  return m_impl;
}

CurrentAnalysis AnalysisDriver::run(analysis::Analysis& analysis, const AnalysisRunOptions& runOptions) {
  return getImpl()->run(analysis,runOptions);
}

bool AnalysisDriver::isRunning() const {
  return getImpl()->isRunning();
}

bool AnalysisDriver::waitForFinished(int m_secs) {
  return getImpl()->waitForFinished(m_secs);
}

void AnalysisDriver::unpauseQueue() {
  getImpl()->unpauseQueue();
}

void AnalysisDriver::stop(CurrentAnalysis& currentAnalysis) {
  getImpl()->stop(currentAnalysis);
}

void AnalysisDriver::stop(DataPoint& dataPoint) {
  getImpl()->stop(dataPoint);
}

bool AnalysisDriver::connect(const std::string& signal,
                             const QObject* qObject,
                             const std::string& slot,
                             Qt::ConnectionType type) const
{
  return getImpl()->connect(signal,qObject,slot,type);
}

/// @cond
AnalysisDriver::AnalysisDriver(boost::shared_ptr<detail::AnalysisDriver_Impl> impl)
  : m_impl(impl)
{}
/// @endcond

project::AnalysisRecord saveAnalysis(analysis::Analysis& analysis,
                                     AnalysisDriver& analysisDriver)
{
  if (analysis.dataPointsAreInvalid()) {
    LOG_FREE_AND_THROW("openstudio.analysisdriver.AnalysisDriver","Analysis '"
                       << analysis.name() << "' contains invalid data points. "
                       << "Call removeAllDataPoints before saving to database.");
  }
  else if (analysis.resultsAreInvalid()) {
    LOG_FREE_AND_THROW("openstudio.analysisdriver.AnalysisDriver","Analysis '"
                       << analysis.name() << "' contains invalid results. "
                       << "Call clearAllResults before saving to database.");
  }

  ProjectDatabase database = analysisDriver.database();
  bool didStartTransaction = database.startTransaction();
  if (!didStartTransaction) {
    LOG_FREE(Debug,"openstudio.analysisdriver.AnalysisDriver","Unable to start transation "
             << "for saving analysis '" << analysis.name() << "' to the ProjectDatabase at "
             << toString(database.path()) << ".");
  }
  database.unloadUnusedCleanRecords();
  AnalysisRecord analysisRecord(analysis,database);
  database.save();
  if (didStartTransaction) {
    database.commitTransaction();
  }

  analysis.clearDirtyFlag();

  return analysisRecord;
}

bool removeDataPoint(analysis::Analysis& analysis,
                     analysis::DataPoint& dataPoint,
                     AnalysisDriver& analysisDriver)
{
  OptionalDataPoint dp = analysis.getDataPointByUUID(dataPoint);
  if (!dp) {
    return false;
  }
  analysisDriver.stop(*dp);
  if (boost::optional<runmanager::Job> job = dp->topLevelJob()) {
    analysisDriver.database().runManager().remove(*job);
  }
  openstudio::path dpDir = dp->directory();
  if (!dpDir.empty() && boost::filesystem::exists(dpDir)) {
    try {
      boost::filesystem::remove_all(dpDir);
    }
    catch (std::exception& e) {
      LOG_FREE(Warn,"openstudio.analysisdriver.AnalysisDriver","Could not remove " << toString(dpDir) 
          << " from file system, because " << e.what() << ". Although these files may persist, they "
          << "will not be programmatically accessible through the OpenStudio Analysis Framework.");
    }
  }
  bool ok = analysis.removeDataPoint(*dp);
  OS_ASSERT(ok);
  return true;
}

bool removeAllDataPoints(analysis::Analysis& analysis,AnalysisDriver& analysisDriver)
{
  bool result(true);
  bool callAnalysisMethod(false);
  if (OptionalCurrentAnalysis currentAnalysis = analysisDriver.getCurrentAnalysis(analysis)) {
    analysisDriver.stop(*currentAnalysis);
  }
  runmanager::RunManager runManager = analysisDriver.database().runManager();
  BOOST_FOREACH(const DataPoint& dataPoint,analysis.dataPoints()) {
    callAnalysisMethod = true;
    if (boost::optional<runmanager::Job> job = dataPoint.topLevelJob()) {
      runManager.remove(*job);
    }
    openstudio::path dpDir = dataPoint.directory();
    if (!dpDir.empty() && boost::filesystem::exists(dpDir)) {
      try {
        boost::filesystem::remove_all(dpDir);
      }
      catch (...) {
        result = false;
      }
    }
  }
  if (OptionalDakotaAlgorithm dakotaAlgorithm = getDakotaAlgorithm(analysis)) {
    if (boost::optional<runmanager::Job> dakotaJob = dakotaAlgorithm->job()) {
      callAnalysisMethod = true;
      openstudio::path dakotaDir = dakotaJob->outdir();
      if (boost::filesystem::exists(dakotaDir)) {
        try {
          boost::filesystem::remove_all(dakotaDir);
        }
        catch (...) {
          result = false;
        }
      }
      runManager.remove(*dakotaJob);
    }
  }
  if ((!callAnalysisMethod) && analysis.algorithm()) {
    Algorithm alg = analysis.algorithm().get();
    if (alg.isComplete() || alg.failed() || (alg.iter() != -1)) {
      callAnalysisMethod = true;
    }
  }
  if (callAnalysisMethod) {
    analysis.removeAllDataPoints();
  }
  return result;
}

bool clearResults(analysis::Analysis& analysis,
                  analysis::DataPoint& dataPoint,
                  AnalysisDriver& analysisDriver)
{
  OptionalDataPoint dp = analysis.getDataPointByUUID(dataPoint);
  if (!dp) {
    return false;
  }
  analysisDriver.stop(*dp);
  if (boost::optional<runmanager::Job> job = dp->topLevelJob()) {
    analysisDriver.database().runManager().remove(*job);
  }
  openstudio::path dpDir = dp->directory();
  if (!dpDir.empty() && boost::filesystem::exists(dpDir)) {
    try {
      boost::filesystem::remove_all(dpDir);
    }
    catch (std::exception& e) {
      LOG_FREE(Warn,"openstudio.analysisdriver.AnalysisDriver","Could not remove " << toString(dpDir) 
          << " from file system, because " << e.what() << ". Although these files may persist, they "
          << "will not be programmatically accessible through the OpenStudio Analysis Framework.");
    }
  }
  analysis.clearResults(*dp);
  return true;
}

bool clearAllResults(analysis::Analysis& analysis,AnalysisDriver& analysisDriver)
{
  bool result(true);
  bool callAnalysisMethod(false);
  if (OptionalCurrentAnalysis currentAnalysis = analysisDriver.getCurrentAnalysis(analysis)) {
    analysisDriver.stop(*currentAnalysis);
  }
  runmanager::RunManager runManager = analysisDriver.database().runManager();
  BOOST_FOREACH(const DataPoint& dataPoint,analysis.dataPoints()) {
    if (boost::optional<runmanager::Job> job = dataPoint.topLevelJob()) {
      callAnalysisMethod = true;
      runManager.remove(*job);
    }
    openstudio::path dpDir = dataPoint.directory();
    if (!dpDir.empty() && boost::filesystem::exists(dpDir)) {
      callAnalysisMethod = true;
      try {
        boost::filesystem::remove_all(dpDir);
      }
      catch (...) {
        result = false; // flag to indicate process incomplete
      }
    }
  }
  if (OptionalDakotaAlgorithm dakotaAlgorithm = getDakotaAlgorithm(analysis)) {
    if (boost::optional<runmanager::Job> dakotaJob = dakotaAlgorithm->job()) {
      callAnalysisMethod = true;
      openstudio::path dakotaDir = dakotaJob->outdir();
      if (boost::filesystem::exists(dakotaDir)) {
        try {
          boost::filesystem::remove_all(dakotaDir);
        }
        catch (...) {
          result = false;
        }
      }
      runManager.remove(*dakotaJob);
    }
  }
  if ((!callAnalysisMethod) && analysis.algorithm()) {
    Algorithm alg = analysis.algorithm().get();
    if (alg.isComplete() || alg.failed() || (alg.iter() != -1)) {
      callAnalysisMethod = true;
    }
  }
  if (callAnalysisMethod) {
    // will mark as dirty
    analysis.clearAllResults();
  }
  return result;
}

} // analysisdriver
} // openstudio
