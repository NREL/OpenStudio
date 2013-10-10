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

#include <pat_app/CloudMonitor.hpp>
#include <pat_app/PatApp.hpp>
#include <pat_app/RunTabController.hpp>
#include <pat_app/RunView.hpp>
#include <pat_app/PatMainWindow.hpp>
#include <pat_app/PatVerticalTabWidget.hpp>

#include <analysis/DataPoint_Impl.hpp>
#include <analysis/Measure.hpp>
#include <analysis/NullMeasure.hpp>
#include <analysis/NullMeasure_Impl.hpp>

#include <analysisdriver/AnalysisDriver.hpp>
#include <analysisdriver/AnalysisRunOptions.hpp>
#include <analysisdriver/CurrentAnalysis.hpp>
#include <analysisdriver/CloudAnalysisDriver.hpp>
#include <analysisdriver/SimpleProject_Impl.hpp>
#include <analysisdriver/SimpleProject.hpp>

#include <runmanager/lib/RunManager.hpp>
#include <runmanager/lib/Job.hpp>

#include <utilities/core/ApplicationPathHelpers.hpp>
#include <utilities/core/Assert.hpp>
#include <utilities/core/Path.hpp>

#include "../shared_gui_components/Buttons.hpp"

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

  bingo = connect(runView->runStatusView->cloudOnButton,SIGNAL(clicked()),
                  PatApp::instance()->cloudMonitor().data(),SLOT(toggleCloud()));
  OS_ASSERT(bingo);

  bingo = connect(runView->runStatusView->cloudOffButton,SIGNAL(clicked()),
                  PatApp::instance()->cloudMonitor().data(),SLOT(toggleCloud()));
  OS_ASSERT(bingo);

  QSharedPointer<CloudMonitor> cloudMonitor = PatApp::instance()->cloudMonitor();

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
      bool bingo = cloudAnalysisDriver->connect(SIGNAL(dataPointQueued(const openstudio::UUID&, const openstudio::UUID&)), this, SLOT(reqestRefresh()), Qt::QueuedConnection);
      OS_ASSERT(bingo);

      // connect cloudAnalysisDriver to update progress on this
      bool isConnected = cloudAnalysisDriver->connect(SIGNAL(dataPointComplete(const openstudio::UUID&, const openstudio::UUID&)), this, SLOT(onIterationProgress()), Qt::QueuedConnection);
      OS_ASSERT(isConnected);

      // currentAnalyses is empty for cloudAnalysisDriver
    }else{
      bool bingo = analysisDriver.connect(SIGNAL(dataPointQueued(const openstudio::UUID&, const openstudio::UUID&)), this, SLOT(reqestRefresh()), Qt::QueuedConnection);
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

    runView->dataPointRunListView->setListController(m_dataPointRunListController);
    runView->dataPointRunListView->setDelegate(m_dataPointRunItemDelegate);

    refresh();

    if ((project->status() == analysisdriver::AnalysisStatus::Running) ||
        (project->status() == analysisdriver::AnalysisStatus::Idle)){
      onIterationProgress();
    }
  }
}

RunTabController::~RunTabController()
{
  if( runView ) { delete runView; }
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

    // only need to check for E+ if running locally
    if (!cloudAnalysisDriver){
      runmanager::ConfigOptions co(true);
      co.findTools(true, true, true, true);
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

      if (numCompletedJobs == totalNumJobs){
        // does the user want to clear results?
        QMessageBox::StandardButton test = QMessageBox::question(runView, "Clear Results", "The simulations are already complete, do you want to clear all results?", QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
        if (test == QMessageBox::Yes){

          // remove all results
          bool completeRemoval = project->clearAllResults();
          if (!completeRemoval) {
            QMessageBox::critical( runView, "Incomplete File Removal", QString("Removed all results from this project, but could not remove all of the result files.") );
          }
          project->save();
        }

        reqestRefresh();
        PatApp::instance()->processEvents();
      }

      if (cloudAnalysisDriver){

        // refresh this tab when data points are queued
        bool bingo = cloudAnalysisDriver->connect(SIGNAL(dataPointQueued(const openstudio::UUID&, const openstudio::UUID&)), this, SLOT(reqestRefresh()), Qt::QueuedConnection);
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

        // start the run
        analysisDriver.unpauseQueue();

        // connect currentAnalysis to update progress on this
        bool isConnected = currentAnalysis.connect(SIGNAL(iterationProgress(int,int)), this, SLOT(onIterationProgress()), Qt::QueuedConnection);
        OS_ASSERT(isConnected);
      }

      // update progress
      onIterationProgress();

    }else{

      // just unpause the queue
      analysisDriver.unpauseQueue();
    }

  }else if(analysisStatus == analysisdriver::AnalysisStatus::Running){
    if (cloudAnalysisDriver){

      // DLM: it is a big deal to quit the cloud, should we prompt the user?
      QMessageBox::StandardButton test = QMessageBox::question(runView, "Stop Cloud", "Do you want to stop the cloud?", QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
      if (test == QMessageBox::Yes) {
        // request stop
        cloudAnalysisDriver->requestStop(false);
      }

    }else{
      // request stop
      project->runManager().setPaused(true);
      project->stop();
    }
  }
}

void RunTabController::onIterationProgress()
{
  // connect to this slot via Qt::QueuedConnection so analysis is up to date when processing here

  boost::optional<analysisdriver::SimpleProject> project = PatApp::instance()->project();
  OS_ASSERT(project);

  runmanager::RunManager runManager = project->runManager();
  analysis::Analysis analysis = project->analysis();

  boost::optional<analysisdriver::CloudAnalysisDriver> cloudAnalysisDriver = project->cloudAnalysisDriver();

  int numCompletedJobs = (int)analysis.completeDataPoints().size();
  int totalNumJobs = (int)analysis.completeDataPoints().size() + (int)analysis.dataPointsToQueue().size();
  int numFailedJobs = (int)analysis.failedDataPoints().size();

  if (numCompletedJobs == totalNumJobs){
    if (cloudAnalysisDriver){
      // no-op
    }else{
      runManager.setPaused(true);
    }
  }

  runView->runStatusView->setProgress(numCompletedJobs, numFailedJobs, totalNumJobs);
}

void RunTabController::reqestRefresh()
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
  QSharedPointer<DataPointRunListItem> dataPointRunListItem = dataSource.dynamicCast<DataPointRunListItem>();
  openstudio::analysis::DataPoint dataPoint = dataPointRunListItem->dataPoint();

  DataPointRunItemView* result = new DataPointRunItemView(dataPoint);
  bool test = connect(dataPoint.getImpl<openstudio::analysis::detail::DataPoint_Impl>().get(), SIGNAL(changed(ChangeType)),
                      result, SLOT(update()));
  OS_ASSERT(test);

  if (dataPoint.topLevelJob()){
    test = dataPoint.topLevelJob()->connect(SIGNAL(treeChanged(const openstudio::UUID&)),
                                            result->dataPointRunHeaderView, SLOT(update()));
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

  DataPointJobItemView* result = new DataPointJobItemView(workflowStepJob);

  OS_ASSERT(workflowStepJob.job);
  bool test = workflowStepJob.job->connect(SIGNAL(statusChanged(const openstudio::runmanager::AdvancedStatus&)), result, SLOT(update()));
  OS_ASSERT(test);

  return result;
}

} // pat
} // openstudio
