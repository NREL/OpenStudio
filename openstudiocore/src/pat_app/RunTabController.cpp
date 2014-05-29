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

#include "CloudMonitor.hpp"
#include "PatApp.hpp"
#include "RunTabController.hpp"
#include "RunView.hpp"
#include "PatMainWindow.hpp"
#include "PatVerticalTabWidget.hpp"

#include "../analysis/DataPoint_Impl.hpp"
#include "../analysis/Measure.hpp"
#include "../analysis/NullMeasure.hpp"
#include "../analysis/NullMeasure_Impl.hpp"

#include "../analysisdriver/AnalysisDriver.hpp"
#include "../analysisdriver/AnalysisRunOptions.hpp"
#include "../analysisdriver/CurrentAnalysis.hpp"
#include "../analysisdriver/CloudAnalysisDriver.hpp"
#include "../analysisdriver/SimpleProject_Impl.hpp"
#include "../analysisdriver/SimpleProject.hpp"

#include "../runmanager/lib/RunManager.hpp"
#include "../runmanager/lib/Job.hpp"

#include "../utilities/core/ApplicationPathHelpers.hpp"
#include "../utilities/core/Assert.hpp"
#include "../utilities/core/Path.hpp"

#include "../shared_gui_components/Buttons.hpp"
#include "../shared_gui_components/WorkflowTools.hpp"

#include <OpenStudio.hxx>

#include <QMessageBox>
#include <QTimer>

namespace openstudio {

namespace pat {

RunTabController::RunTabController()
  : QObject(), m_refreshScheduled(false)
{
  runView = new RunView();

  bool bingo;
  bingo = connect(runView->runStatusView->playButton, SIGNAL(clicked()), this, SLOT(onPlayButtonClicked()));
  OS_ASSERT(bingo);

  bingo = connect(runView->runStatusView, SIGNAL(radianceEnabledChanged(bool)), this, SLOT(onRadianceEnabledChanged(bool)));
  OS_ASSERT(bingo);

  bingo = connect(runView->runStatusView->cloudOnButton,SIGNAL(clicked()),
                  PatApp::instance()->cloudMonitor().data(),SLOT(toggleCloud()));
  OS_ASSERT(bingo);

  bingo = connect(runView->runStatusView->cloudOffButton,SIGNAL(clicked()),
                  PatApp::instance()->cloudMonitor().data(),SLOT(toggleCloud()));
  OS_ASSERT(bingo);

  bingo = connect(runView->runStatusView->cloudLostConnectionButton,SIGNAL(clicked()),
                  PatApp::instance()->cloudMonitor().data(),SLOT(toggleCloud()));
  OS_ASSERT(bingo);

  bingo = connect(runView->runStatusView,SIGNAL(dataPointResultsCleared(const openstudio::UUID&)),
                  this,SLOT(onDataPointResultsCleared(const openstudio::UUID&)));
  OS_ASSERT(bingo);

  boost::optional<analysisdriver::SimpleProject> project = PatApp::instance()->project();
  if (project){

    // ensure that baseline exists
    analysis::DataPoint baselineDataPoint = project->baselineDataPoint();

    analysis::Analysis analysis = project->analysis();
    boost::optional<analysisdriver::CloudAnalysisDriver> cloudAnalysisDriver = project->cloudAnalysisDriver();
    analysisdriver::AnalysisDriver analysisDriver = project->analysisDriver();
    std::vector<analysisdriver::CurrentAnalysis> currentAnalyses;

    // refresh this tab when data points are queued
    if (cloudAnalysisDriver){
      bool bingo = cloudAnalysisDriver->connect(SIGNAL(dataPointQueued(const openstudio::UUID&, const openstudio::UUID&)), this, SLOT(onDataPointQueued(const openstudio::UUID&, const openstudio::UUID&)), Qt::QueuedConnection);
      OS_ASSERT(bingo);

      // connect cloudAnalysisDriver to update progress on this
      bool isConnected = cloudAnalysisDriver->connect(SIGNAL(dataPointComplete(const openstudio::UUID&, const openstudio::UUID&)), this, SLOT(onIterationProgress()), Qt::QueuedConnection);
      OS_ASSERT(isConnected);

      // currentAnalyses is empty for cloudAnalysisDriver
    }else{
      bool bingo = analysisDriver.connect(SIGNAL(dataPointQueued(const openstudio::UUID&, const openstudio::UUID&)), this, SLOT(onDataPointQueued(const openstudio::UUID&, const openstudio::UUID&)), Qt::QueuedConnection);
      OS_ASSERT(bingo);

      currentAnalyses = analysisDriver.currentAnalyses();
      if (!currentAnalyses.empty()){
        // connect currentAnalysis to update progress on this
        bingo = currentAnalyses[0].connect(SIGNAL(iterationProgress(int,int)), this, SLOT(onIterationProgress()), Qt::QueuedConnection);
        OS_ASSERT(bingo);

        // connect currentAnalysis to update progress in PatApp
        // DLM: PatApp is already connected to this signal
        //bingo = currentAnalyses[0].connect(SIGNAL(iterationProgress(int,int)), PatApp::instance(), SLOT(disableTabsDuringRun()), Qt::QueuedConnection);
        //OS_ASSERT(bingo);
      }
    }

    m_dataPointRunListController = QSharedPointer<DataPointRunListController>(new DataPointRunListController(analysis));
    m_dataPointRunItemDelegate = QSharedPointer<DataPointRunItemDelegate>(new DataPointRunItemDelegate());
    
    // ETH: Is the following the right way to connect to a signal emanating from a QSharedPointer?
    // (Suprisinging hard to google and get a good answer.)
    bingo = connect(m_dataPointRunItemDelegate.data(),SIGNAL(dataPointResultsCleared(const openstudio::UUID&)),
                    this,SLOT(onDataPointResultsCleared(const openstudio::UUID&)));
    OS_ASSERT(bingo);

    runView->dataPointRunListView->setListController(m_dataPointRunListController);
    runView->dataPointRunListView->setDelegate(m_dataPointRunItemDelegate);

    refresh();

    if ((project->status() == analysisdriver::AnalysisStatus::Running)){
      onIterationProgress();
    }

 
    bool radianceEnabled = openstudio::projectHasRadiance(*project); 
    LOG(Debug, "Project has radiance " << radianceEnabled);
    runView->runStatusView->setRadianceEnabled(radianceEnabled);
    m_radianceEnabled = radianceEnabled;
  }
}

RunTabController::~RunTabController()
{
  if( runView ) { delete runView; }
}

void RunTabController::onRadianceEnabledChanged(bool t_radianceEnabled)
{
  boost::optional<analysisdriver::SimpleProject> project = PatApp::instance()->project();
  OS_ASSERT(project);

  if (m_radianceEnabled != t_radianceEnabled)
  {
    if (!(project->analysis().completeDataPoints().empty())) {
      QMessageBox::StandardButton test = QMessageBox::question(runView, "Clear Results", "Simulation results must be cleared when changing daylight simulation engine. Continue with change of daylight simulation engine?", QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
      if (test == QMessageBox::Yes) {
        bool completeRemoval = project->clearAllResults();
        if (!completeRemoval) {
          QMessageBox::critical( runView, "Incomplete File Removal", QString("Removed all results from this project, but could not remove all of the result files.") );
        }
        project->save();
        // force refresh after selecting all or clearing results
        refresh();
        PatApp::instance()->processEvents();
      } else {
        // user canceled results clearing, so returning
        if (runView) {
          runView->runStatusView->setRadianceEnabled(m_radianceEnabled);
        }
        return;
      }
    }

    // if we got here, then the user wants to continue
    if (t_radianceEnabled)
    {
      boost::optional<model::Model> seedModel = project->seedModel();
      runmanager::RunManager runManager = project->runManager();


      if (!checkSeedForRadianceWarningsAndErrors(seedModel, runManager))
      {
        return; // cannot use radiance
      }
    }

    m_radianceEnabled = t_radianceEnabled;

    if (m_radianceEnabled)
    {
      LOG(Debug, "Adding radiance to project");
      openstudio::addRadianceToProject(*project);
    } else {
      openstudio::removeRadianceFromProject(*project);
    }
  } 

}

void RunTabController::showRadianceWarningsAndErrors(const std::vector<std::string> & warnings,
                                            const std::vector<std::string> & errors)
{
  QString errorsAndWarnings;
  QString text;
  
  if(warnings.size()){
    errorsAndWarnings += "WARNINGS:\n";
    for (std::string warning : warnings){
      text = warning.c_str();
      errorsAndWarnings += text;
      errorsAndWarnings += '\n';
    }
    errorsAndWarnings += '\n';
  }

  if(errors.size()){
    errorsAndWarnings += "ERRORS:\n";
    for (std::string error : errors){
      text = error.c_str();
      errorsAndWarnings += text;
      errorsAndWarnings += '\n';
    }
  }

  QMessageBox::critical(runView, "Radiance Warnings and Errors", errorsAndWarnings);
}


bool RunTabController::checkSeedForRadianceWarningsAndErrors(boost::optional<model::Model> &t_seedModel, runmanager::RunManager &t_runManager)
{
  if (!t_seedModel)
  {
    return true;
  }

  std::vector<std::string> warnings;
  std::vector<std::string> errors;
  openstudio::getRadiancePreRunWarningsAndErrors(warnings, errors, t_runManager, t_seedModel);

  if (!warnings.empty() || !errors.empty())
  {
    showRadianceWarningsAndErrors(warnings, errors);
  }

  if (!errors.empty())
  {
    runView->runStatusView->setRadianceEnabled(false);
    return false;
  }

  return true;
}

void RunTabController::seedChanged()
{
  if (m_radianceEnabled)
  {
    boost::optional<analysisdriver::SimpleProject> project = PatApp::instance()->project();
    OS_ASSERT(project);
    boost::optional<model::Model> seedModel = project->seedModel();
    runmanager::RunManager runManager = project->runManager();

    checkSeedForRadianceWarningsAndErrors(seedModel, runManager);
  }
}

void RunTabController::onPlayButtonClicked()
{
  boost::optional<analysisdriver::SimpleProject> project = PatApp::instance()->project();
  OS_ASSERT(project);

  runmanager::RunManager runManager = project->runManager();
  analysis::Analysis analysis = project->analysis();

  boost::optional<analysisdriver::CloudAnalysisDriver> cloudAnalysisDriver = project->cloudAnalysisDriver();
  analysisdriver::AnalysisDriver analysisDriver = project->analysisDriver();

  analysisdriver::AnalysisStatus analysisStatus = project->status();

  if (analysisStatus == analysisdriver::AnalysisStatus::Idle){

    // Make sure everything is in a kosher state for starting the simulation

    // need a seed model
    boost::optional<model::Model> seedModel = project->seedModel();
    if (!seedModel){
      // todo: check message
      QMessageBox::warning(runView, "No Baseline Model", "Cannot run analysis until a baseline model has been set on the first tab.");
      return;
    }

    // version translation and weather file
    if (project->modelsRequireUpdate()) {
      std::stringstream question;
      question << "One or more of your models (baseline or alternate) needs to be updated to OpenStudio Version ";
      question << openStudioVersion() << ", or the weather file needs to be located. Do you want to proceed?";
      if ((!project->analysis().weatherFile()) || (!boost::filesystem::exists(project->analysis().weatherFile()->path()))) {
        question << " (Results will be cleared because your weather file must be reset.)";
      }

      QMessageBox::StandardButton test = QMessageBox::question(runView, "Update Models", toQString(question.str()), QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
      if (test == QMessageBox::Yes) {
        bool ok = project->updateModels();
        if (!ok) {
          QMessageBox::critical(runView, "Incomplete Upgrade", QString("The upgrade process was only partially successful. If there are errors, you may also want to check project.log for more information."));
        }
        PatApp::instance()->processEvents(); // results may need to be cleared, which would negate next question
        if (!(project->analysis().completeDataPoints().empty())) {
          question.str("");
          question << "The input models were just updated and the project contains old results. Do you want to clear all results before proceeding? (Choosing no could result in design alternatives simulated with different versions of OpenStudio.)";
          QMessageBox::StandardButton test = QMessageBox::question(runView, "Clear Results", toQString(question.str()), QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
          if (test == QMessageBox::Yes) {
            bool completeRemoval = project->clearAllResults();
            if (!completeRemoval) {
              QMessageBox::critical( runView, "Incomplete File Removal", QString("Removed all results from this project, but could not remove all of the result files.") );
            }
          }
        }
        project->save();

      } else {
        // no update, quit
        return;
      }
    }

    // only need to check for E+ and Ruby if running locally
    if (!cloudAnalysisDriver){
      runmanager::ConfigOptions co(true);
      co.findTools(true, true, false, true);
      co.saveQSettings();

      if (co.getTools().getAllByName("energyplus").tools().size() == 0)
      {
        /// \todo check for specific version of energy plus
        QMessageBox::information(runView,
            "Missing EnergyPlus",
            "EnergyPlus could not be located, simulation aborted.",
            QMessageBox::Ok);

        return;
      }
      
      if (co.getTools().getAllByName("ruby").tools().size() == 0)
      {
        /// \todo check for specific version of ruby
        QMessageBox::information(runView,
            "Missing Ruby",
            "Ruby could not be located, simulation aborted.",
            QMessageBox::Ok);

        return;
      }
    }

    // check if we already have current analysis
    std::vector<analysisdriver::CurrentAnalysis> currentAnalyses;
    if (cloudAnalysisDriver){
      // no-op for the cloud, currentAnalyses will be empty
    }else{
      currentAnalyses = analysisDriver.currentAnalyses();
    }

    if (currentAnalyses.empty()){
      // check if we are already done
      int numCompletedJobs = (int)analysis.completeDataPoints().size();
      int totalNumJobs = (int)analysis.completeDataPoints().size() + (int)analysis.dataPointsToQueue().size();
      int totalDataPoints = (int)analysis.dataPoints().size();

      if (numCompletedJobs == totalNumJobs){
        // there is nothing to do, see if the user wants to select all or clear results

        if (totalNumJobs < totalDataPoints){
          // does the user want to select all?
          QMessageBox::StandardButton test = QMessageBox::question(runView, "Select All", "No simulations are selected to run, do you want to select all?", QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
          if (test == QMessageBox::Yes){
            for (analysis::DataPoint dataPoint : analysis.dataPoints()){
              dataPoint.setSelected(true);
            }
            project->save();
          }else{
            // nothing to do
            return;
          }
        }else{
          // does the user want to clear results?
          QMessageBox::StandardButton test = QMessageBox::question(runView, "Clear Results", "The simulations are already complete, do you want to clear all results?", QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
          if (test == QMessageBox::Yes){

            // remove all results
            bool completeRemoval = project->clearAllResults();
            if (!completeRemoval) {
              QMessageBox::critical( runView, "Incomplete File Removal", QString("Removed all results from this project, but could not remove all of the result files.") );
            }
            project->save();
          }else{
            // nothing to do
            return;
          }
        }

        // force refresh after selecting all or clearing results
        refresh();
        PatApp::instance()->processEvents();
      }

      if (cloudAnalysisDriver){

        // refresh this tab when data points are queued
        bool bingo = cloudAnalysisDriver->connect(SIGNAL(dataPointQueued(const openstudio::UUID&, const openstudio::UUID&)), this, SLOT(onDataPointQueued(const openstudio::UUID&, const openstudio::UUID&)), Qt::QueuedConnection);
        OS_ASSERT(bingo);

        // connect cloudAnalysisDriver to update progress on this
        bool isConnected = cloudAnalysisDriver->connect(SIGNAL(dataPointComplete(const openstudio::UUID&, const openstudio::UUID&)), this, SLOT(onIterationProgress()), Qt::QueuedConnection);
        OS_ASSERT(isConnected);

        // start the run
        cloudAnalysisDriver->requestRun();
      }else{

        // request new run
        analysisdriver::AnalysisRunOptions runOptions = standardRunOptions(*project);
        analysisdriver::CurrentAnalysis currentAnalysis = analysisDriver.run(analysis, runOptions);

        // connect currentAnalysis to update progress on this
        bool isConnected = currentAnalysis.connect(SIGNAL(iterationProgress(int,int)), this, SLOT(onIterationProgress()), Qt::QueuedConnection);
        OS_ASSERT(isConnected);

        // start the run
        analysisDriver.unpauseQueue();
      }

      // update progress
      onIterationProgress();

    }else{

      // just unpause the queue
      analysisDriver.unpauseQueue();
    }

  }else if(analysisStatus == analysisdriver::AnalysisStatus::Error){

    if (cloudAnalysisDriver){
      // start the run
      cloudAnalysisDriver->requestRun();
    }else{
      // DLM: should never get here
    }

  }else if(analysisStatus == analysisdriver::AnalysisStatus::Running){

    QMessageBox::StandardButton test = QMessageBox::question(runView, "Stop Analysis", "Do you want to stop the analysis?", QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
    if (test == QMessageBox::Yes) {

      if (cloudAnalysisDriver){
        // request stop
        cloudAnalysisDriver->requestStop(false);

        // it is a big deal to quit the cloud, prompt the user
        test = QMessageBox::question(runView, "Stop Cloud", "Do you also want to stop the cloud?", QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
        if (test == QMessageBox::Yes) {
          // stop cloud
          PatApp::instance()->cloudMonitor()->stopCloud();
        }

      }else{
            
        // check if we already have current analysis
        std::vector<analysisdriver::CurrentAnalysis> currentAnalyses = analysisDriver.currentAnalyses();
        if (!currentAnalyses.empty()){
          analysisDriver.stop(currentAnalyses[0]);
        }else{
          // should not get here, nothing to stop
        }
      }
    }

  }
}

void RunTabController::onIterationProgress()
{
  // connect to this slot via Qt::QueuedConnection so analysis is up to date when processing here

  boost::optional<analysisdriver::SimpleProject> project = PatApp::instance()->project();
  OS_ASSERT(project);

  int numCompletedJobs = 0;
  int totalNumJobs = 0;
  int numFailedJobs = 0;

  boost::optional<analysisdriver::CloudAnalysisDriver> cloudAnalysisDriver = project->cloudAnalysisDriver();
  if (cloudAnalysisDriver){

    numCompletedJobs = cloudAnalysisDriver->numCompleteDataPoints();
    totalNumJobs = cloudAnalysisDriver->numDataPointsInIteration();
    numFailedJobs = cloudAnalysisDriver->numFailedDataPoints();
  
  }else{
    analysisdriver::AnalysisDriver analysisDriver = project->analysisDriver();
    std::vector<analysisdriver::CurrentAnalysis> currentAnalyses = analysisDriver.currentAnalyses();
    if (!currentAnalyses.empty()){

      numCompletedJobs = currentAnalyses[0].numCompletedJobsInOSIteration();
      totalNumJobs = currentAnalyses[0].totalNumJobsInOSIteration();
      numFailedJobs = currentAnalyses[0].numFailedJobsInOSIteration();

      if (numCompletedJobs == totalNumJobs){
        // kind of a hack to do this here instead of analysis driver
        project->runManager().setPaused(true);
      }
    }
  }

  runView->runStatusView->setProgress(numCompletedJobs, numFailedJobs, totalNumJobs);
}

// ETH: Could replace this and next slot with single slot if analysis emitted
// dataPointChanged(const openstudio::UUID& dataPoint) whenever a child DataPoint
// emits changed. Not sure if that is desirable or not.
void RunTabController::onDataPointQueued(const openstudio::UUID& analysis, const openstudio::UUID& dataPoint)
{
  emitDataPointChanged(dataPoint);
}

void RunTabController::onDataPointResultsCleared(const openstudio::UUID& dataPoint) {
  emitDataPointChanged(dataPoint);
}

void RunTabController::requestRefresh()
{
  if (!m_refreshScheduled){
    m_refreshScheduled = true;
    QTimer::singleShot(0, this, SLOT(refresh()));
  }
}

void RunTabController::refresh()
{
  m_refreshScheduled = false;

  CloudStatus cloudStatus = PatApp::instance()->cloudMonitor()->status();

  boost::optional<analysisdriver::SimpleProject> project = PatApp::instance()->project();
  OS_ASSERT(project);
  analysisdriver::AnalysisStatus analysisStatus = project->status();

  runView->runStatusView->setStatus(cloudStatus, analysisStatus);

  QTimer::singleShot(0, runView->dataPointRunListView, SLOT(refreshAllViews()));
}

void RunTabController::emitDataPointChanged(const openstudio::UUID& dataPoint) {
  QSharedPointer<DataPointRunListController> listController = runView->dataPointRunListView->listController().dynamicCast<DataPointRunListController>();
  int N = listController->count();
  for (int i = 0; i < N; ++i){
    QSharedPointer<OSListItem> item = listController->itemAt(i);
    if (item){
      QSharedPointer<DataPointRunListItem> dataPointItem = item.dynamicCast<DataPointRunListItem>();
      if (dataPointItem){
        if (dataPointItem->dataPoint().uuid() == dataPoint){
          listController->emitItemChanged(i);
        }
      }
    }
  }
}

DataPointRunListController::DataPointRunListController(const openstudio::analysis::Analysis& analysis)
  : OSListController(), m_analysis(analysis)
{
}

QSharedPointer<OSListItem> DataPointRunListController::itemAt(int i)
{
  std::vector<openstudio::analysis::DataPoint> dataPoints = m_analysis.dataPoints();
  return QSharedPointer<OSListItem>(new DataPointRunListItem(dataPoints[i]));
}

int DataPointRunListController::count()
{
  std::vector<openstudio::analysis::DataPoint> dataPoints = m_analysis.dataPoints();
  return (int)dataPoints.size();
}

void DataPointRunListController::emitItemChanged(int i)
{
  emit itemChanged(i);
}

DataPointRunListItem::DataPointRunListItem(const openstudio::analysis::DataPoint& dataPoint)
  : OSListItem(), m_dataPoint(dataPoint)
{
}

openstudio::analysis::DataPoint DataPointRunListItem::dataPoint() const
{
  return m_dataPoint;
}

QWidget * DataPointRunItemDelegate::view(QSharedPointer<OSListItem> dataSource)
{
  QSharedPointer<CloudMonitor> cloudMonitor = PatApp::instance()->cloudMonitor();

  QSharedPointer<DataPointRunListItem> dataPointRunListItem = dataSource.dynamicCast<DataPointRunListItem>();
  openstudio::analysis::DataPoint dataPoint = dataPointRunListItem->dataPoint();

  // connect signals to DataPointRunItemView 
  auto result = new DataPointRunItemView(dataPoint);
  bool test = connect(dataPoint.getImpl<openstudio::analysis::detail::DataPoint_Impl>().get(), SIGNAL(changed(ChangeType)),
                      result, SLOT(checkForUpdate()));
  OS_ASSERT(test);
  test = connect(result,SIGNAL(dataPointResultsCleared(const openstudio::UUID&)),
                 this,SIGNAL(dataPointResultsCleared(const openstudio::UUID&)));
  OS_ASSERT(test);

  // connect signals to header
  test = connect(dataPoint.getImpl<openstudio::analysis::detail::DataPoint_Impl>().get(), SIGNAL(changed(ChangeType)),
                      result->dataPointRunHeaderView, SLOT(requestUpdate()));
  OS_ASSERT(test);
  
  test = connect(cloudMonitor.data(), SIGNAL(cloudStatusChanged(const CloudStatus&)), result->dataPointRunHeaderView, SLOT(requestUpdate()));
  OS_ASSERT(test);

  if (dataPoint.topLevelJob()){
    test = dataPoint.topLevelJob()->connect(SIGNAL(treeChanged(const openstudio::UUID&)),
                                            result->dataPointRunHeaderView, SLOT(requestUpdate()));
    OS_ASSERT(test);
  }

  QSharedPointer<DataPointJobController> dataPointJobController(new DataPointJobController(dataPoint));
  QSharedPointer<DataPointJobItemDelegate> dataPointJobItemDelegate(new DataPointJobItemDelegate());

  result->dataPointRunContentView->setListController(dataPointJobController);
  result->dataPointRunContentView->setDelegate(dataPointJobItemDelegate);

  return result;
}

DataPointJobController::DataPointJobController(const openstudio::analysis::DataPoint& dataPoint)
  : OSListController(), m_dataPoint(dataPoint)
{
}

QSharedPointer<OSListItem> DataPointJobController::itemAt(int i)
{
  QSharedPointer<OSListItem> result;
  std::vector<analysis::WorkflowStepJob> workflowStepJobs = getJobsByWorkflowStep(m_dataPoint);

  if (i < (int)workflowStepJobs.size()) {
    result = QSharedPointer<OSListItem>(new DataPointJobItem(workflowStepJobs[i]));
  }

  return result;
}

int DataPointJobController::count() {
  std::vector<analysis::WorkflowStepJob> jobsByWorkflowStep = getJobsByWorkflowStep(m_dataPoint);

  int result = (int)jobsByWorkflowStep.size();

  return result;
}

std::vector<analysis::WorkflowStepJob> DataPointJobController::getJobsByWorkflowStep(const analysis::DataPoint& dataPoint)
{
  analysis::Problem problem = m_dataPoint.problem();
  return problem.getJobsByWorkflowStep(dataPoint,true);
}

DataPointJobItem::DataPointJobItem(const analysis::WorkflowStepJob& workflowStepJob)
  : OSListItem(), m_workflowStepJob(workflowStepJob)
{}

analysis::WorkflowStepJob DataPointJobItem::workflowStepJob() const
{
  return m_workflowStepJob;
}

/// Widget returned will be a DataPointJobItem
QWidget * DataPointJobItemDelegate::view(QSharedPointer<OSListItem> dataSource)
{
  QSharedPointer<DataPointJobItem> dataPointJobItem = dataSource.dynamicCast<DataPointJobItem>();

  analysis::WorkflowStepJob workflowStepJob = dataPointJobItem->workflowStepJob();

  auto result = new DataPointJobItemView(workflowStepJob);

  OS_ASSERT(workflowStepJob.job);
  bool test = workflowStepJob.job->connect(SIGNAL(statusChanged(const openstudio::runmanager::AdvancedStatus&)), result, SLOT(requestUpdate()));
  OS_ASSERT(test);

  return result;
}

} // pat
} // openstudio
