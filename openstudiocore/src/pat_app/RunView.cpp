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

#include <pat_app/RunView.hpp>

#include <pat_app/PatApp.hpp>
#include <pat_app/CloudMonitor.hpp>

#include "../shared_gui_components/Buttons.hpp"
#include "../shared_gui_components/OSListView.hpp"
#include "../shared_gui_components/OSCollapsibleView.hpp"
#include "../shared_gui_components/OSViewSwitcher.hpp"

#include <analysis/Analysis.hpp>
#include <analysis/DataPoint.hpp>
#include <analysis/DataPoint_Impl.hpp>

#include <runmanager/lib/Job.hpp>
#include <runmanager/lib/RunManager.hpp>
#include <runmanager/lib/Workflow.hpp>

#include <utilities/cloud/AWSProvider.hpp>
#include <utilities/cloud/AWSProvider_Impl.hpp>
#include <utilities/cloud/CloudProvider.hpp>
#include <utilities/cloud/VagrantProvider.hpp>
#include <utilities/core/Assert.hpp>
#include <utilities/time/DateTime.hpp>

#include <QLabel>
#include <QPainter>
#include <QPaintEvent>
#include <QPushButton>
#include <QScrollArea>
#include <QStyleOption>
#include <QTextEdit>
#include <QTimer>
#include <QVBoxLayout>
#include <QMessageBox>

#include <fstream>

namespace openstudio {

namespace pat {

RunView::RunView()
  : PatMainTabView()
{
  setTitle("Run Simulations");

  // Main Content
  mainContent = new QWidget();

  QVBoxLayout * mainContentVLayout = new QVBoxLayout();
  mainContentVLayout->setContentsMargins(0,0,0,0);
  mainContentVLayout->setSpacing(0);
  mainContentVLayout->setAlignment(Qt::AlignTop);

  mainContent->setLayout(mainContentVLayout);

  // Run Header

  runStatusView = new RunStatusView();
  mainContentVLayout->addWidget(runStatusView);

  // Data Point Run List
  dataPointRunListView = new OSListView();
  dataPointRunListView->setContentsMargins(5,5,5,5);
  dataPointRunListView->setSpacing(5);

  QScrollArea* scrollArea = new QScrollArea();
  scrollArea->setWidgetResizable(true);
  scrollArea->setWidget(dataPointRunListView);
  
  mainContentVLayout->addWidget(scrollArea);

  viewSwitcher->setView(mainContent);
}

RunStatusView::RunStatusView()
  : QWidget()
{
  setStyleSheet("openstudio--pat--RunStatusView { background: #D5D5D5; border-bottom: 1px solid #8C8C8C; }");

  QVBoxLayout * mainVLayout = new QVBoxLayout(); 
  mainVLayout->setContentsMargins(5,5,5,5);
  mainVLayout->setSpacing(5);
  this->setLayout(mainVLayout);

  QHBoxLayout * mainHLayout = new QHBoxLayout(); 
  mainHLayout->setContentsMargins(5,5,5,5);
  mainHLayout->setSpacing(5);
  mainVLayout->addLayout(mainHLayout);

  QHBoxLayout * buttonHLayout = new QHBoxLayout(); 
  buttonHLayout->setContentsMargins(5,5,5,5);
  buttonHLayout->setSpacing(0);
  mainVLayout->addLayout(buttonHLayout);

  // Run / Play button area

  m_playLabel = new QLabel(this);
  m_playLabel->setMinimumWidth(120);
  m_playLabel->setObjectName("H2");
  m_playLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
  mainHLayout->addWidget(m_playLabel);

  playButton = new PlayButton(this);
  mainHLayout->addWidget(playButton);

  // Progress bar area
  m_progressBar = new PatProgressBar();
  m_progressBar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
  m_progressBar->setTextVisible(false);
 
  mainHLayout->addWidget(m_progressBar);

  // percent complete/failed
  QWidget *percentContainer = new QWidget();
  percentContainer->setMinimumWidth(105);
  m_percentComplete = new QLabel();
  m_percentComplete->setStyleSheet("QLabel {font-size: 13px; font: bold; color: green;}");
  m_percentFailed = new QLabel();
  m_percentFailed->setStyleSheet("QLabel {font-size: 13px; font: bold; color: red;}");
  
  QVBoxLayout *percentCompleteLayout = new QVBoxLayout();
  percentCompleteLayout->setContentsMargins(0,0,0,0);
  percentCompleteLayout->addWidget(m_percentComplete);
  percentCompleteLayout->addWidget(m_percentFailed);

  percentContainer->setLayout(percentCompleteLayout);
  mainHLayout->addWidget(percentContainer);

  // Start Cloud
  
  cloudOnButton = new CloudOnButton();
  mainHLayout->addWidget(cloudOnButton);
  
  cloudStartingButton = new CloudStartingButton();
  mainHLayout->addWidget(cloudStartingButton);
  cloudStartingButton->setEnabled(false);
  cloudStartingButton->hide();

  cloudOffButton = new CloudOffButton();
  mainHLayout->addWidget(cloudOffButton);
  cloudOffButton->hide();

  cloudStoppingButton = new CloudStoppingButton();
  mainHLayout->addWidget(cloudStoppingButton);
  cloudStoppingButton->setEnabled(false);
  cloudStoppingButton->hide();

  cloudLostConnectionButton = new CloudLostConnectionButton();
  mainHLayout->addWidget(cloudLostConnectionButton);
  cloudLostConnectionButton->setEnabled(true);
  cloudLostConnectionButton->hide();


  // Cloud Status
  QWidget *statusContainer = new QWidget();
  statusContainer->setMinimumWidth(105);
  m_cloudTime = new QLabel(this);
  m_cloudInstances = new QLabel(this);
  QVBoxLayout * vLayout = new QVBoxLayout();
  vLayout->setContentsMargins(0,0,0,0);
  vLayout->addWidget(m_cloudTime);
  vLayout->addWidget(m_cloudInstances);
  statusContainer->setLayout(vLayout);
  m_timer = new QTimer(this);
  bool isConnected = connect(m_timer, SIGNAL(timeout()),
                        this, SLOT(updateCloudData()));
  OS_ASSERT(isConnected);
  mainHLayout->addWidget(statusContainer);

  // "Select All" Button Layout

  QLabel * label = 0;

  QSpacerItem * horizontalSpacer = 0;

  QString style;

  m_selectAllDataPoints = new QPushButton(this);
  m_selectAllDataPoints->setFlat(true);
  m_selectAllDataPoints->setFixedSize(QSize(87,16));
  style = "QPushButton {"
          "  background-image:url(':/images/select_all_button.png');"
          "  border:none;"
          "}";
  m_selectAllDataPoints->setStyleSheet(style);
  isConnected = connect(m_selectAllDataPoints, SIGNAL(clicked(bool)),
                        this, SLOT(on_selectAllDataPoints(bool)));
  OS_ASSERT(isConnected);
  buttonHLayout->addWidget(m_selectAllDataPoints);

  horizontalSpacer = new QSpacerItem(5, 0, QSizePolicy::Fixed, QSizePolicy::Fixed);
  buttonHLayout->addSpacerItem(horizontalSpacer);

  m_clearSelectionDataPoints = new QPushButton(this);
  m_clearSelectionDataPoints->setFlat(true);
  m_clearSelectionDataPoints->setFixedSize(QSize(87,16));
  style = "QPushButton {"
          "  background-image:url(':/images/clear_all_button.png');"
          "  border:none;"
          "}";
  m_clearSelectionDataPoints->setStyleSheet(style);
  isConnected = connect(m_clearSelectionDataPoints, SIGNAL(clicked(bool)),
                        this, SLOT(on_clearSelectionDataPoints(bool)));
  OS_ASSERT(isConnected);
  buttonHLayout->addWidget(m_clearSelectionDataPoints);

  buttonHLayout->addStretch();

  m_selectAllDownloads = new QPushButton(this);
  m_selectAllDownloads->setFlat(true);
  m_selectAllDownloads->setFixedSize(QSize(18,18));
  isConnected = connect(m_selectAllDownloads, SIGNAL(clicked(bool)),
                        this, SLOT(on_selectAllDownloads(bool)));
  OS_ASSERT(isConnected);
  buttonHLayout->addWidget(m_selectAllDownloads);

  m_selectAllDownloadsLabel = new QLabel();
  buttonHLayout->addWidget(m_selectAllDownloadsLabel);

  horizontalSpacer = new QSpacerItem(14, 0, QSizePolicy::Fixed, QSizePolicy::Fixed);
  buttonHLayout->addSpacerItem(horizontalSpacer);
  
  m_selectAllClears = new QPushButton(this);
  m_selectAllClears->setFlat(true);
  m_selectAllClears->setFixedSize(QSize(18,18));
  style = "QPushButton {"
                        "background-image:url(':/images/clear_results_enabled.png');"
                        "  border:none;"
                        "}";
  m_selectAllClears->setStyleSheet(style);
  isConnected = connect(m_selectAllClears, SIGNAL(clicked(bool)),
                        this, SLOT(on_selectAllClears(bool)));
  OS_ASSERT(isConnected);
  buttonHLayout->addWidget(m_selectAllClears);

  label = new QLabel("All");
  buttonHLayout->addWidget(label); 

  horizontalSpacer = new QSpacerItem(10, 0, QSizePolicy::Fixed, QSizePolicy::Fixed);
  buttonHLayout->addSpacerItem(horizontalSpacer); 

  QSharedPointer<CloudMonitor> cloudMonitor = PatApp::instance()->cloudMonitor();
  CloudStatus cloudStatus = PatApp::instance()->cloudMonitor()->status();

  boost::optional<analysisdriver::SimpleProject> project = PatApp::instance()->project();
  OS_ASSERT(project);
  analysisdriver::AnalysisStatus analysisStatus = project->status();

  setStatus(cloudStatus, analysisStatus);
}

void RunStatusView::setStatus(const CloudStatus & cloudStatus, analysisdriver::AnalysisStatus analysisStatus)
{
  // update run text
  switch (analysisStatus.value())
  {
    case analysisdriver::AnalysisStatus::Idle:
      if(cloudStatus == CLOUD_STOPPED){
        m_runText = "Run Locally";
      }else{
        m_runText = "Run on Cloud";
      }
      break;
    case analysisdriver::AnalysisStatus::Starting:
      m_runText = "Starting";
      break;
    case analysisdriver::AnalysisStatus::Running:
      if(cloudStatus == CLOUD_STOPPED){
        m_runText = "Running Locally";
      }else if(cloudStatus == CLOUD_RUNNING){
        m_runText = "Running on Cloud";
      }else{
        // should not be possible, what to do here?
        m_runText = "";
      }
      break;
    case analysisdriver::AnalysisStatus::Stopping:
      m_runText = "Stopping";
      break;
    case analysisdriver::AnalysisStatus::Error:
      m_runText = "Restart Run";
      break;
  }
  m_playLabel->setText(m_runText);

  // update play button
  switch (analysisStatus.value())
  {
    case analysisdriver::AnalysisStatus::Idle:
      if((cloudStatus == CLOUD_STARTING) || 
         (cloudStatus == CLOUD_STOPPING)){
        playButton->setStatus(PlayButton::IDLEDISABLED);
      }else{
        playButton->setStatus(PlayButton::IDLE);
      }
      break;
    case analysisdriver::AnalysisStatus::Starting:
      playButton->setStatus(PlayButton::STARTING);
      break;
    case analysisdriver::AnalysisStatus::Running:
      playButton->setStatus(PlayButton::RUNNING);
      break;
    case analysisdriver::AnalysisStatus::Stopping:
      playButton->setStatus(PlayButton::STOPPING);
      break;
    case analysisdriver::AnalysisStatus::Error:
      playButton->setStatus(PlayButton::ERROR);
      break;
  }

  // update progress bar
  if (analysisStatus == analysisdriver::AnalysisStatus::Running){
    // m_progressBar will be driven by onIterationProgress in RunTabController
  }else{
    m_percentFailed->setText("");
    m_percentComplete->setText("");
    m_progressBar->setRange(0,1);
    m_progressBar->setValue(0);
  }

  // update cloud time and instances 
  // CLOUD_STARTING, CLOUD_RUNNING, CLOUD_STOPPING, CLOUD_STOPPED, CLOUD_ERROR 
  if(cloudStatus == CLOUD_RUNNING){
    m_cloudTime->show();
    m_cloudInstances->show();
    m_timer->start(30000);
  } else {
    m_cloudTime->hide();
    m_cloudInstances->hide();
    m_timer->stop();
  }
  updateCloudData();
  
  // update download button
  if(cloudStatus == CLOUD_RUNNING){
    QString style;
    style = "QPushButton {"
            "  background-image:url(':/images/results_yes_download.png');"
            "  border:none;"
            "}";
    m_selectAllDownloads->setStyleSheet(style);
    m_selectAllDownloadsLabel->setText("All");
  } else {
    QString style;
    style = "QPushButton {"
            "  background-image:url(':/images/results_space_download.png');"
            "  border:none;"
            "}";
    m_selectAllDownloads->setStyleSheet(style);
    m_selectAllDownloadsLabel->setText("   ");
  }

  // update cloud button
  cloudOnButton->hide();
  cloudStartingButton->hide();
  cloudOffButton->hide();
  cloudStoppingButton->hide();
  cloudLostConnectionButton->hide();
  if( cloudStatus == CLOUD_STOPPED ){
    cloudOnButton->show();
    // don't allow turn on cloud while running, starting, or stopping analysis
    if ((analysisStatus == analysisdriver::AnalysisStatus::Running) || 
        (analysisStatus == analysisdriver::AnalysisStatus::Starting) ||
        (analysisStatus == analysisdriver::AnalysisStatus::Stopping)){ 
      cloudOnButton->setEnabled(false);
    }else{
      cloudOnButton->setEnabled(true);
    }
  }else if( cloudStatus == CLOUD_STARTING ){
    cloudStartingButton->show();
  }else if( cloudStatus == CLOUD_RUNNING ){
    cloudOffButton->show();
    // don't allow turn off cloud while running, starting, or stopping analysis
    if ((analysisStatus == analysisdriver::AnalysisStatus::Running) || 
        (analysisStatus == analysisdriver::AnalysisStatus::Starting) ||
        (analysisStatus == analysisdriver::AnalysisStatus::Stopping)){ 
      cloudOffButton->setEnabled(false);
    }else{
      cloudOffButton->setEnabled(true);
    }
  }else if( cloudStatus == CLOUD_STOPPING ){
    cloudStoppingButton->show();
  }else if( cloudStatus == CLOUD_ERROR ){
    cloudLostConnectionButton->show();
  }

}

void RunStatusView::paintEvent(QPaintEvent * e)
{
  QStyleOption opt;
  opt.init(this);
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void RunStatusView::setProgress(int numCompletedJobs, int numFailedJobs, int numJobsInIteration)
{
  m_progressBar->setRange(0, std::max(numJobsInIteration, 1));
  m_progressBar->setValue(numCompletedJobs);

  if (numJobsInIteration > 0){
    double percentFailed = 100.0 * numFailedJobs / (double)numJobsInIteration;
    double percentComplete = 100.0 * numCompletedJobs / (double)numJobsInIteration;

    if (numFailedJobs > 0){
      m_percentFailed->setText(QString::number(percentFailed, 'f', 0) + QString("% Failed"));
    }else{
      m_percentFailed->setText("");
    }

    m_percentComplete->setText(QString::number(percentComplete, 'f', 0) + QString("% Complete"));
  }else{
    m_percentFailed->setText("");
    m_percentComplete->setText("");
  }
}

void RunStatusView::on_selectAllDataPoints(bool checked)
{
  boost::optional<analysisdriver::SimpleProject> project = PatApp::instance()->project();
  if (project){

    // Can only select points if project is idle
    bool isProjectIdle = (project->status() == analysisdriver::AnalysisStatus::Idle);

    if (!isProjectIdle){
      return;
    }

    std::vector<analysis::DataPoint> dataPoints = project->analysis().dataPoints();
    Q_FOREACH(analysis::DataPoint dataPoint, dataPoints){

      // Ignore signal if dataPoint has data
      bool isComplete = dataPoint.complete();

      if (!isComplete){
        dataPoint.setSelected(true);
      }
    }
  }
}

void RunStatusView::on_clearSelectionDataPoints(bool checked)
{
  boost::optional<analysisdriver::SimpleProject> project = PatApp::instance()->project();
  if (project){

    // Can only select points if project is idle
    bool isProjectIdle = (project->status() == analysisdriver::AnalysisStatus::Idle);

    if (!isProjectIdle){
      return;
    }

    std::vector<analysis::DataPoint> dataPoints = project->analysis().dataPoints();
    Q_FOREACH(analysis::DataPoint dataPoint, dataPoints){

      // Ignore signal if dataPoint has data
      bool isComplete = dataPoint.complete();

      if (!isComplete){
        dataPoint.setSelected(false);
      }
    }
  }
}

void RunStatusView::on_selectAllDownloads(bool checked)
{
  boost::optional<analysisdriver::SimpleProject> project = PatApp::instance()->project();
  if (project){
    std::vector<analysis::DataPoint> dataPoints = project->analysis().dataPoints();
    Q_FOREACH(analysis::DataPoint dataPoint, dataPoints){
      if(checked){
        dataPoint.setRunType(analysis::DataPointRunType::CloudSlim);
      } else {
        dataPoint.setRunType(analysis::DataPointRunType::CloudDetailed);
      }
    }
  }
}

void RunStatusView::on_selectAllClears(bool checked)
{
  boost::optional<analysisdriver::SimpleProject> project = PatApp::instance()->project();
  if (project){

    // Can only clear points if project is idle
    bool isProjectIdle = (project->status() == analysisdriver::AnalysisStatus::Idle);

    if (isProjectIdle){
      QMessageBox::StandardButton test = QMessageBox::question(this, "Clear Results", "Do you want to clear all results?", QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
      if (test == QMessageBox::Yes){
        analysis::Analysis analysis = project->analysis();
        analysisdriver::AnalysisDriver driver = project->analysisDriver();
        std::vector<analysis::DataPoint> dataPoints = analysis.dataPoints();
        Q_FOREACH(analysis::DataPoint dataPoint, dataPoints) {
          analysisdriver::clearResults(analysis,dataPoint,driver);
        }
      }
    }
  }
}

void RunStatusView::updateCloudData()
{
  QString temp;

  boost::optional<CloudSession> session = CloudMonitor::currentProjectSession();
  if (session) {
    boost::optional<AWSSession> awsSession = session->optionalCast<AWSSession>();
    if (awsSession) {
      temp = temp.setNum(awsSession->totalSessionUptime());
      temp.prepend("Time: <b>");
      temp += " minutes</b>"; 
      m_cloudTime->setText(temp);

      temp = temp.setNum(awsSession->totalSessionInstances());
      temp.prepend("Instances: <b>");
      temp += "</b>";
      m_cloudInstances->setText(temp);

      return;
    }
  }

  m_cloudTime->setText("Time: <b>0 minutes</b>");

  m_cloudInstances->setText("Instances: <b>0</b>");
}

DataPointRunHeaderView::DataPointRunHeaderView(const openstudio::analysis::DataPoint& dataPoint)
  : OSHeader(new HeaderToggleButton()),
  m_dataPoint(dataPoint),
  m_name(0),
  m_lastRunTime(0),
  m_status(0),
  m_nas(0),
  m_warnings(0),
  m_errors(0),
  m_download(0),
  m_clear(0),
  m_updateRequested(false)
{
  setFixedHeight(30);

  QHBoxLayout * mainHLayout = new QHBoxLayout();
  mainHLayout->setContentsMargins(5,5,5,5);
  mainHLayout->setSpacing(5);
  mainHLayout->setAlignment(Qt::AlignLeft);
  setLayout(mainHLayout);

  mainHLayout->addWidget(toggleButton);

  m_name = new QLabel();
  mainHLayout->addWidget(m_name);

  mainHLayout->addStretch();

  m_lastRunTime = new QLabel();
  m_lastRunTime->setFixedWidth(150);
  mainHLayout->addWidget(m_lastRunTime);

  m_status = new QLabel();
  m_status->setFixedWidth(75);
  mainHLayout->addWidget(m_status);

  m_nas = new QLabel();
  m_nas->setFixedWidth(50);
  mainHLayout->addWidget(m_nas);

  m_warnings = new QLabel();
  m_warnings->setFixedWidth(75);
  mainHLayout->addWidget(m_warnings);

  m_errors = new QLabel();
  m_errors->setFixedWidth(75);
  mainHLayout->addWidget(m_errors);

  m_download = new QPushButton();
  m_download->setCheckable(true);
  m_download->setFlat(true);
  m_download->setFixedSize(QSize(18,18));
  mainHLayout->addWidget(m_download);

  bool isConnected;
  isConnected = connect(m_download,SIGNAL(clicked(bool)),
                        this,SLOT(on_downloadClicked(bool)));
  OS_ASSERT(isConnected);

  QSpacerItem* horizontalSpacer = new QSpacerItem(20, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
  mainHLayout->addSpacerItem(horizontalSpacer);

  m_clear = new QPushButton();
  m_clear->setCheckable(false);
  m_clear->setFlat(true);
  m_clear->setFixedSize(QSize(18,18));
  mainHLayout->addWidget(m_clear);

  isConnected = connect(m_clear,SIGNAL(clicked(bool)),
                this,SLOT(on_clearClicked(bool)));
  OS_ASSERT(isConnected);

  horizontalSpacer = new QSpacerItem(20, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
  mainHLayout->addSpacerItem(horizontalSpacer);

  this->setCheckable(true);

  isConnected = connect(this,SIGNAL(clicked(bool)),
                this,SLOT(on_clicked(bool)));
  OS_ASSERT(isConnected);

  update();
}

void DataPointRunHeaderView::requestUpdate()
{
  if (!m_updateRequested){
    m_updateRequested = true;
    QTimer::singleShot(0, this, SLOT(update()));
  }
}

void DataPointRunHeaderView::update()
{
  m_updateRequested = false;

  boost::optional<openstudio::runmanager::Job> topLevelJob = m_dataPoint.topLevelJob();

  boost::optional<openstudio::DateTime> lastRunTime;
  unsigned numNAs(0);
  unsigned numWarnings(0);
  unsigned numErrors(0);
  if (topLevelJob){
    lastRunTime = topLevelJob->startTime();

    // if running remotely we may not know startTime, use end time
    if (topLevelJob->externallyManaged() && !lastRunTime){
      lastRunTime = topLevelJob->lastRun();
    }

    openstudio::runmanager::JobErrors treeErrors = topLevelJob->treeErrors();
    numNAs = treeErrors.numNAs;
    numErrors = treeErrors.errors().size();
    numWarnings = treeErrors.warnings().size();
  }

  QString nasStyle;
  if (numNAs > 0){
    nasStyle = "QLabel { color : #C47B06; }";
  }
  m_nas->setText(QString::number(numNAs) + QString(numNAs == 1 ? " NA" : " NAs"));
  m_nas->setStyleSheet(nasStyle);

  QString warningsStyle;
  if (numWarnings > 0){
    warningsStyle = "QLabel { color : #C47B06; }";
  }
  m_warnings->setText(QString::number(numWarnings) + QString(numWarnings == 1 ? " Warning" : " Warnings"));
  m_warnings->setStyleSheet(warningsStyle);

  QString errorsStyle;
  if (numErrors > 0){
    errorsStyle = "QLabel { color : red; }";
  }
  m_errors->setText(QString::number(numErrors) + QString(numErrors == 1 ? " Error" : " Errors"));
  m_errors->setStyleSheet(errorsStyle);

  QString lastRunTimeString;
  if(lastRunTime){
    lastRunTimeString = toQString(lastRunTime->toString());
  }else{
    lastRunTimeString = "Not Started";
  }

  QString status;
  QString statusStyle;
  if (m_dataPoint.isComplete()){
    if (m_dataPoint.failed()){
      status = "Failed";
      statusStyle = "QLabel { color : red; }";
    }else{
      status = "Finished";
      statusStyle = "QLabel { color : green; }";
    }
  }else{
    if (topLevelJob){
      
      runmanager::TreeStatusEnum treeStatus = topLevelJob->treeStatus();
      status = toQString(topLevelJob->treeStatus().valueDescription());
      statusStyle = "";

      if (topLevelJob->externallyManaged() && (treeStatus == runmanager::TreeStatusEnum::Waiting)){
        runmanager::AdvancedStatus advancedStatus = topLevelJob->status();
        if (advancedStatus.value() == runmanager::AdvancedStatusEnum::Processing){
          status = "Running";
        }else if (advancedStatus.value() == runmanager::AdvancedStatusEnum::CopyingResultFiles){
          status = "Downloading";
        }else if (advancedStatus.value() == runmanager::AdvancedStatusEnum::Queuing){
          status = "Waiting";
        }
        if(!lastRunTime){
          lastRunTimeString = "Running Remotely";
        }
      }
    }else{
      status = "Not Started";
      statusStyle = "";
    }
  }

  m_name->setText(toQString(m_dataPoint.name()));
  m_status->setText(status);
  m_status->setStyleSheet(statusStyle);
  m_lastRunTime->setText(lastRunTimeString);

  QString style;

  ///// DataPointRunHeaderView

  this->blockSignals(true);
  this->setChecked(m_dataPoint.selected());
  this->blockSignals(false);

  if (m_dataPoint.complete()){
    style = "openstudio--pat--DataPointRunHeaderView  { "
            "background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, "
            "stop: 0 #808080, stop: 0.5 #b3b3b3, stop: 1.0 #cccccc); "
            "border: 1px solid #8C8C8C; "
            "} ";
  }else if(this->isChecked()){
    style = "openstudio--pat--DataPointRunHeaderView  { "
            "background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, "
            "stop: 0 #ffd573, stop: 0.5 #ffeec7, stop: 1.0 #ffd573); "
            "border: 1px solid #8C8C8C; "
            "} ";
  }else{
    style = "openstudio--pat--DataPointRunHeaderView  { "
            "background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, "
            "stop: 0 #cccccc, stop: 0.5 #f2f2f2, stop: 1.0 #cccccc); "
            "border: 1px solid #8C8C8C; "
            "} ";
  }
  setStyleSheet(style);

  ///// Clear button

  bool hasDataToClear = (m_dataPoint.complete());
  if(hasDataToClear){
    style = "QPushButton {"
                          "background-image:url(':/images/clear_results_enabled.png');"
                          "  border:none;"
                          "}";
  } else {
    style = "QPushButton {"
                          "background-image:url(':/images/clear_results_disabled.png');"
                          "  border:none;"
                          "}";
  }
  m_clear->setStyleSheet(style);
  m_clear->setEnabled(hasDataToClear);

  ///// Download button

  bool isCloudRunning = (PatApp::instance()->cloudMonitor()->status() == CLOUD_RUNNING);
  if (!isCloudRunning){
    m_download->setEnabled(false);
    m_download->setChecked(false);
    style = "QPushButton {"
                                "background-image:url(':/images/results_space_download.png');"
                                "  border:none;"
                                "}";
    m_download->setStyleSheet(style);

  }else{

    // Determine if datapoint already has detailed data
    if(this->m_dataPoint.complete() && !this->m_dataPoint.directory().empty()){
      m_download->setEnabled(false);
      m_download->setChecked(false);
      style = "QPushButton {"
                            "background-image:url(':/images/results_downloaded.png');"
                            "  border:none;"
                            "}";

    // If dataPoint not selected can't request download
    } else if(!m_dataPoint.selected()){
      m_download->setEnabled(false);
      m_download->setChecked(false);
      style = "QPushButton {"
                            "background-image:url(':/images/results_space_download.png');"
                            "  border:none;"
                            "}";

    // If dataPoint is not complete and is selected
    } else {
      
      // can't request to download on completion if already complete
      if (this->m_dataPoint.complete()){
        m_download->setEnabled(false);
      }else{
        m_download->setEnabled(true);
      }

      if(m_dataPoint.runType() == analysis::DataPointRunType::CloudDetailed){
        m_download->setChecked(true);
        style = "QPushButton {"
                              "background-image:url(':/images/results_yes_download.png');"
                              "  border:none;"
                              "}";
      // Note: there are more remaining types just analysis::DataPointRunType::CloudSlim
      } else {
        m_download->setChecked(false);
        style = "QPushButton {"
                              "background-image:url(':/images/results_no_download.png');"
                              "  border:none;"
                              "}";
      }
    }
    m_download->setStyleSheet(style);
  }
  
}

void DataPointRunHeaderView::on_clicked(bool checked)
{
  // Can only select points if project is idle
  boost::optional<analysisdriver::SimpleProject> project = PatApp::instance()->project();
  OS_ASSERT(project);
  bool isProjectIdle = (project->status() == analysisdriver::AnalysisStatus::Idle);

  // Ignore signal if dataPoint has data
  bool isComplete = this->m_dataPoint.complete();
  
  if(!isProjectIdle || isComplete){
    this->blockSignals(true);
    this->setChecked(!checked);
    this->blockSignals(false);
    return;
  }

  if(checked){
    m_dataPoint.setSelected(true);
  } else {
    m_dataPoint.setSelected(false);
  }
}

void DataPointRunHeaderView::on_downloadClicked(bool checked)
{
  if(checked){
    m_dataPoint.setRunType(analysis::DataPointRunType::CloudDetailed);
  } else {
    m_dataPoint.setRunType(analysis::DataPointRunType::CloudSlim);
  }
}

void DataPointRunHeaderView::on_clearClicked(bool checked)
{
  // Can only clear points if project is idle
  boost::optional<analysisdriver::SimpleProject> project = PatApp::instance()->project();
  OS_ASSERT(project);
  bool isProjectIdle = (project->status() == analysisdriver::AnalysisStatus::Idle);

  if (isProjectIdle){
    analysis::Analysis analysis = project->analysis();
    analysisdriver::AnalysisDriver driver = project->analysisDriver();
    analysisdriver::clearResults(analysis,m_dataPoint,driver);
  }
}

DataPointRunContentView::DataPointRunContentView()
  : OSListView()
{
  setContentsMargins(0,5,0,5);
}

DataPointRunItemView::DataPointRunItemView(const openstudio::analysis::DataPoint& dataPoint)
  : OSCollapsibleView(), m_dataPoint(dataPoint)
{
  dataPointRunHeaderView = new DataPointRunHeaderView(dataPoint);
  setHeader(dataPointRunHeaderView);
 
  dataPointRunContentView = new DataPointRunContentView(); 
  setContent(dataPointRunContentView);

  checkForUpdate();
}

void DataPointRunItemView::checkForUpdate()
{
  UUID topLevelJobUUID;
  boost::optional<runmanager::Job> topLevelJob = m_dataPoint.topLevelJob();
  if (topLevelJob){
    topLevelJobUUID = topLevelJob->uuid();
  }

  if (topLevelJobUUID != m_topLevelJobUUID){
    m_topLevelJobUUID = topLevelJobUUID;
    update();
  }
}

DataPointJobHeaderView::DataPointJobHeaderView()
  : OSHeader(new HeaderToggleButton())
{
  QHBoxLayout * mainHLayout = new QHBoxLayout();
  mainHLayout->setContentsMargins(15,5,5,5);
  mainHLayout->setSpacing(5);
  mainHLayout->setAlignment(Qt::AlignLeft);
  setLayout(mainHLayout);

  mainHLayout->addWidget(toggleButton);

  m_name = new QLabel();
  
  mainHLayout->addWidget(m_name);
  mainHLayout->addStretch();

  m_lastRunTime = new QLabel();
  m_lastRunTime->setFixedWidth(150);
  mainHLayout->addWidget(m_lastRunTime);

  m_status = new QLabel();
  m_status->setFixedWidth(75);
  mainHLayout->addWidget(m_status);

  m_na = new QLabel();
  m_na->setFixedWidth(50);
  mainHLayout->addWidget(m_na);

  m_warnings = new QLabel();
  m_warnings->setFixedWidth(75);
  mainHLayout->addWidget(m_warnings);

  m_errors = new QLabel();
  m_errors->setFixedWidth(75);
  mainHLayout->addWidget(m_errors);
}

void DataPointJobHeaderView::setName(const std::string& name)
{
  m_name->setText(toQString(name));
}

void DataPointJobHeaderView::setLastRunTime(const boost::optional<openstudio::DateTime>& lastRunTime)
{
  if (lastRunTime){
    std::string s = lastRunTime->toString();
    m_lastRunTime->setText(toQString(s));
  }else{
    m_lastRunTime->setText("Not Started");
  }
}

void DataPointJobHeaderView::setStatus(const openstudio::runmanager::AdvancedStatus& status, bool isCanceled)
{
  if (!isCanceled)
  {
    std::string s = status.toString();
    m_status->setText(toQString(s));
  } else {
    m_status->setText("Canceled");
  }
}

void DataPointJobHeaderView::setNA(bool na)
{
  QString text;
  QString naStyle;
  if (na){
    text = "   NA";
    naStyle = "QLabel { color : #C47B06; }";
  }
  m_na->setText(text);
  m_na->setStyleSheet(naStyle);
}

void DataPointJobHeaderView::setNumWarnings(unsigned numWarnings)
{
  QString warningsStyle;
  if (numWarnings > 0){
    warningsStyle = "QLabel { color : #C47B06; }";
  }
  m_warnings->setText(QString::number(numWarnings) + QString(numWarnings == 1 ? " Warning" : " Warnings"));
  m_warnings->setStyleSheet(warningsStyle);
}

void DataPointJobHeaderView::setNumErrors(unsigned numErrors)
{
  QString errorsStyle;
  if (numErrors > 0){
    errorsStyle = "QLabel { color : red; }";
  }
  m_errors->setText(QString::number(numErrors) + QString(numErrors == 1 ? " Error" : " Errors"));
  m_errors->setStyleSheet(errorsStyle);
}

DataPointJobContentView::DataPointJobContentView()
  : QWidget()
{
  QHBoxLayout* mainHLayout = new QHBoxLayout();
  mainHLayout->setContentsMargins(15,5,5,5);
  mainHLayout->setSpacing(0);
  mainHLayout->setAlignment(Qt::AlignLeft);
  setLayout(mainHLayout);

  m_textEdit = new QLabel();
  m_textEdit->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
  m_textEdit->setWordWrap(true);

  mainHLayout->addWidget(m_textEdit);
}

void DataPointJobContentView::clear()
{
  m_textEdit->setText("");
}

void DataPointJobContentView::addInitialConditionMessage(const std::string& message)
{
  QString html = m_textEdit->text();
  html += QString("<b style=\"color:blue\">Initial Condition</b>: ") + QString::fromStdString(message) + QString("<br></br>");
  m_textEdit->setText(html);
}

void DataPointJobContentView::addFinalConditionMessage(const std::string& message)
{
  QString html = m_textEdit->text();
  html += QString("<b style=\"color:blue\">Final Condition</b>: ") + QString::fromStdString(message) + QString("<br></br>");
  m_textEdit->setText(html);
}

void DataPointJobContentView::addInfoMessage(const std::string& message)
{
  QString html = m_textEdit->text();
  html += QString("<b style=\"color:green\">Info</b>: ") + QString::fromStdString(message) + QString("<br></br>");
  m_textEdit->setText(html);
}

void DataPointJobContentView::addWarningMessage(const std::string& message)
{
  QString html = m_textEdit->text();
  html += QString("<b style=\"color:#C47B06\">Warning</b>: ") + QString::fromStdString(message) + QString("<br></br>");
  m_textEdit->setText(html);
}

void DataPointJobContentView::addErrorMessage(const std::string& message)
{
  QString html = m_textEdit->text();
  html += QString("<b style=\"color:red\">Error</b>: ") + QString::fromStdString(message) + QString("<br></br>");
  m_textEdit->setText(html);
}

void DataPointJobContentView::addStdErrorMessage(const std::string& message)
{
  QString html = m_textEdit->text();
  html += QString::fromStdString(message) + QString("<br></br>");
  m_textEdit->setText(html);
}

DataPointJobItemView::DataPointJobItemView(const analysis::WorkflowStepJob& workflowStepJob)
  : OSCollapsibleView(),
    m_workflowStepJob(workflowStepJob),
    m_updateRequested(false)
{
  setStyleSheet("openstudio--pat--DataPointJobItemView { background: #C3C3C3; margin-left:10px; }");

  dataPointJobHeaderView = new DataPointJobHeaderView();
  setHeader(dataPointJobHeaderView);
 
  dataPointJobContentView = new DataPointJobContentView(); 
  setContent(dataPointJobContentView);

  update();
}

void DataPointJobItemView::paintEvent(QPaintEvent * e)
{
  QStyleOption opt;
  opt.init(this);
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void DataPointJobItemView::requestUpdate()
{
  if (!m_updateRequested){
    m_updateRequested = true;
    QTimer::singleShot(0, this, SLOT(update()));
  }
}

void DataPointJobItemView::update()
{
  m_updateRequested = false;

  if (m_workflowStepJob.measure) {
    dataPointJobHeaderView->setName(m_workflowStepJob.measure->name());
  }
  else {
    OS_ASSERT(m_workflowStepJob.step.isWorkItem());
    dataPointJobHeaderView->setName(m_workflowStepJob.step.workItemType().valueName());
  }

  OS_ASSERT(m_workflowStepJob.job);

  dataPointJobHeaderView->setLastRunTime(m_workflowStepJob.job->lastRun());
  dataPointJobHeaderView->setStatus(m_workflowStepJob.job->status(), m_workflowStepJob.job->canceled());

  openstudio::runmanager::JobErrors jobErrors = m_workflowStepJob.job->errors();

  // DLM: would we rather put these in order of when they were logged?

  dataPointJobContentView->clear();

  std::vector<std::string> initialConditions = jobErrors.initialConditions();
  Q_FOREACH(const std::string& initialCondition, initialConditions){
    dataPointJobContentView->addInitialConditionMessage(initialCondition);
  }

  std::vector<std::string> finalConditions = jobErrors.finalConditions();
  Q_FOREACH(const std::string& finalCondition, finalConditions){
    dataPointJobContentView->addFinalConditionMessage(finalCondition);
  }

  std::vector<std::string> errors = jobErrors.errors();
  dataPointJobHeaderView->setNumErrors(errors.size());
  Q_FOREACH(const std::string& errorMessage, errors){
    dataPointJobContentView->addErrorMessage(errorMessage);
  }

  // also display std err if job failed and it exists and is not empty
  if (m_workflowStepJob.job->lastRun() && !m_workflowStepJob.job->running() && !jobErrors.succeeded()){
    try{
      runmanager::Files files(m_workflowStepJob.job->outputFiles());
      openstudio::path stdErrPath = files.getLastByFilename("stderr").fullPath;
      std::ifstream ifs(toString(stdErrPath).c_str());
      std::string stdErrorMessage((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
      ifs.close();
      if (!stdErrorMessage.empty()){
        dataPointJobContentView->addStdErrorMessage(stdErrorMessage);
      }
    }catch(std::exception&){

    }
  }

  std::vector<std::string> warnings = jobErrors.warnings();
  dataPointJobHeaderView->setNumWarnings(warnings.size());
  Q_FOREACH(const std::string& warningMessage, warnings){
    dataPointJobContentView->addWarningMessage(warningMessage);
  }

  std::vector<std::string> infos = jobErrors.infos();
  Q_FOREACH(const std::string& infoMessage, infos){
    dataPointJobContentView->addInfoMessage(infoMessage);
  }

  if (jobErrors.result == ruleset::OSResultValue::NA){
    dataPointJobHeaderView->setNA(true);
  }else{
    dataPointJobHeaderView->setNA(false);
  }
}

PatProgressBar::PatProgressBar()
  : QProgressBar()
{
  setValue(0);
}

void PatProgressBar::setValue(int value) 
{
  QProgressBar::setValue(value);

  QString style;
  
  style.append("QProgressBar {");
  style.append("  margin-left: 9px;");
  style.append("  border: 1px solid black;");
  style.append("  border-radius: 10px;");
  style.append("  height: 20px;");
  style.append("  background-color: qlineargradient(x1:0,y1:0,");
  style.append("                                    x2:0,y2:1,");
  style.append("                                    stop: 0 #363639, ");
  style.append("                                    stop: 1 #7D7E80);");
  style.append("} ");
  
  style.append("QProgressBar::chunk {");
  style.append("  background-color: qlineargradient(x1:0,y1:0,");
  style.append("                                    x2:0,y2:1,");
  style.append("                                    stop: 0 #8DC63F, ");
  style.append("                                    stop: 1 #5D9732);");
  style.append("} ");

  if( value == maximum() )
  { 
    style.append("QProgressBar::chunk {");
    style.append(" border-bottom-right-radius: 10px;");
    style.append(" border-bottom-left-radius: 10px;");
    style.append(" border-top-right-radius: 10px;");
    style.append(" border-top-left-radius: 10px;");
    style.append("} ");
  }
  else
  {
    style.append("QProgressBar::chunk {");
    style.append(" border-bottom-right-radius: 0px;");
    style.append(" border-bottom-left-radius: 10px;");
    style.append(" border-top-right-radius: 0px;");
    style.append(" border-top-left-radius: 10px;");
    style.append("} ");
  }

  setStyleSheet(style);
}

}

} // openstudio
