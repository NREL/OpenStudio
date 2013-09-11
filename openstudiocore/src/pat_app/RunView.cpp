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
#include "../shared_gui_components/Buttons.hpp"
#include <pat_app/PatApp.hpp>
#include "../shared_gui_components/OSListView.hpp"
#include "../shared_gui_components/OSCollapsibleView.hpp"
#include "../shared_gui_components/OSViewSwitcher.hpp"

#include <analysis/Analysis.hpp>
#include <analysis/DataPoint.hpp>
#include <analysis/DataPoint_Impl.hpp>

#include <runmanager/lib/RunManager.hpp>
#include <runmanager/lib/Workflow.hpp>
#include <runmanager/lib/Job.hpp>

#include <utilities/time/DateTime.hpp>

#include <utilities/core/Assert.hpp>

#include <QVBoxLayout>
#include <QTextEdit>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QPaintEvent>
#include <QPainter>
#include <QStyleOption>

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
  setFixedHeight(60);
  setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);
  setStyleSheet("openstudio--pat--RunStatusView { background: #D5D5D5; border-bottom: 1px solid #8C8C8C; }");

  QHBoxLayout * mainHLayout = new QHBoxLayout(); 
  mainHLayout->setContentsMargins(5,5,5,5);
  mainHLayout->setSpacing(5);

  this->setLayout(mainHLayout);

  //QWidget* space = new QWidget();
  //space->setFixedWidth(200);
  //mainHLayout->addWidget(space);

  mainHLayout->addStretch();

  // Run / Play button area

  m_playButton = new PlayButton(this);
  m_playButton->setFixedWidth(120);
  m_playButton->setText("Run");
  m_playButton->setCheckable(true);
  m_playButton->setChecked(false);
  mainHLayout->addWidget(m_playButton);
  bool isConnected = connect(m_playButton, SIGNAL(clicked(bool)), this, SIGNAL(playButtonClicked(bool)));
  OS_ASSERT(isConnected);

  // Progress bar area
  m_progressBar = new PatProgressBar();
  m_progressBar->setFixedWidth(600);
  m_progressBar->setTextVisible(false);
 
  mainHLayout->addWidget(m_progressBar);

  // percent complete/failed
  m_percentComplete = new QLabel();
  m_percentComplete->setStyleSheet("QLabel {font-size: 13px; font: bold; color: green;}");
  m_percentFailed = new QLabel();
  m_percentFailed->setStyleSheet("QLabel {font-size: 13px; font: bold; color: red;}");
  
  QVBoxLayout *percentCompleteLayout = new QVBoxLayout();
  percentCompleteLayout->setContentsMargins(0,0,0,0);
  percentCompleteLayout->addWidget(m_percentComplete);
  percentCompleteLayout->addWidget(m_percentFailed);

  mainHLayout->addLayout(percentCompleteLayout);

  mainHLayout->addStretch();

  // Start Cloud

  toggleCloudButton = new ToggleCloudButton();
  mainHLayout->addWidget(toggleCloudButton);

  mainHLayout->addStretch();
}

void RunStatusView::paintEvent(QPaintEvent * e)
{
  QStyleOption opt;
  opt.init(this);
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void RunStatusView::setRunning(bool isRunning)
{
  if (isRunning){
    m_playButton->setChecked(true);
  }else{
    m_playButton->setChecked(false);
  }
}

void RunStatusView::setProgress(int numCompletedJobs, int numFailedJobs, int numJobsInIteration, bool isRunning)
{
  // there are four states:
  // not started - not running, numCompletedJobs = 0
  // restarted - not running, numCompletedJobs > 0
  // running - running
  // complete - not running, numCompletedJobs = numJobsInIteration

  //bool showPercentComplete = false;
  if (isRunning){
    // running
    //m_playButton->setText("Pause");
    m_playButton->setText("Stop");
    m_playButton->setChecked(true);
  }else{
    if (numCompletedJobs == 0){
      m_playButton->setText("Run");
      m_playButton->setChecked(false);
    }else if (numCompletedJobs == numJobsInIteration){
      m_playButton->setText("Complete");
      m_playButton->setChecked(false); // third style to show complete?
    }else {
      m_playButton->setText("Resume");
      m_playButton->setChecked(false);
    }
  }

  m_progressBar->setRange(0, std::max(numJobsInIteration, 1));
  m_progressBar->setValue(numCompletedJobs);

  if (numJobsInIteration > 0){
    double percentFailed = 100.0 * numFailedJobs / (double)numJobsInIteration;
    double percentComplete = 100.0 * numCompletedJobs / (double)numJobsInIteration;

    if (numFailedJobs > 0){
      m_percentFailed->setHidden(false);
    }else{
      m_percentFailed->setHidden(true);
    }
    m_percentFailed->setText(QString::number(percentFailed, 'f', 0) + QString("% Failed"));
    m_percentComplete->setText(QString::number(percentComplete, 'f', 0) + QString("% Complete"));
  }else{
    m_percentFailed->setText("");
    m_percentComplete->setText("");
  }
}

ToggleCloudButton::ToggleCloudButton()
  : GrayButton(),
    m_starting(false),
    m_stopping(false)
{
  m_turnOnText  = "Turn On Cloud";
  m_turnOffText = "Turn Off Cloud";

  setCheckable(true);

  QFontMetrics fm(font());
  int onWidth = fm.width(m_turnOnText);
  int offWidth = fm.width(m_turnOffText);

  if( onWidth > offWidth )
  {
    setFixedWidth(onWidth + 20);
  }
  else
  {
    setFixedWidth(offWidth + 20);
  }

  updateText();
}

void ToggleCloudButton::updateText()
{
  if( ! isChecked() && ! m_starting && ! m_stopping )
  {
    setText(m_turnOnText);
  }
  else if( m_starting )
  {
    setText("Starting");
  }
  else if( isChecked() )
  {
    setText(m_turnOffText);
  }
  else if( m_stopping )
  {
    setText("Stopping");
  }
}

bool ToggleCloudButton::isStarting() const
{
  return m_starting;
}

void ToggleCloudButton::setStarting(bool isStarting)
{
  m_starting = isStarting;

  updateText();
}

bool ToggleCloudButton::isStopping() const
{
  return m_stopping;
}

void ToggleCloudButton::setStopping(bool isStopping)
{
  m_stopping = isStopping;

  updateText();
}

void ToggleCloudButton::nextCheckState()
{
  if( ! isChecked() && ! m_starting && ! m_stopping ) // Not Running
  {
    m_starting = true; 
    setChecked(true);
  }
  else if( m_starting )
  {
    m_starting = false;
    m_stopping = false;
    setChecked(true); 
  }
  else if( isChecked() ) // Running
  {
    m_starting = false;
    m_stopping = true;
    setChecked(false);  
  }
  else if( m_stopping )
  {
    m_stopping = false;
    m_starting = false;
    setChecked(false);
  }

  updateText();
}

DataPointRunHeaderView::DataPointRunHeaderView(const openstudio::analysis::DataPoint& dataPoint)
  : OSHeader(new HeaderToggleButton()), m_dataPoint(dataPoint)
{
  setFixedHeight(30);
  QString style;
  style.append("openstudio--pat--DataPointRunHeaderView { ");
  style.append("background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, ");
  style.append("stop: 0 #cccccc, stop: 0.5 #f2f2f2, stop: 1.0 #cccccc); ");
  style.append("border: 1px solid #8C8C8C; ");
  style.append("} ");
  setStyleSheet(style);

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

  update();
}

void DataPointRunHeaderView::update()
{
  boost::optional<openstudio::runmanager::Job> topLevelJob = m_dataPoint.topLevelJob();

  boost::optional<openstudio::DateTime> lastRunTime;
  unsigned numNAs(0);
  unsigned numWarnings(0);
  unsigned numErrors(0);
  if (topLevelJob){
    lastRunTime = topLevelJob->startTime();
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
      status = toQString(topLevelJob->treeStatus().valueDescription());
      statusStyle = "";
    }else{
      status = "Not Started";
      statusStyle = "";
    }
  }

  m_name->setText(toQString(m_dataPoint.name()));
  if(lastRunTime){
    m_lastRunTime->setText(toQString(lastRunTime->toString()));
  }else{
    m_lastRunTime->setText(QString("Not Started"));
  }
  m_status->setText(status);
  m_status->setStyleSheet(statusStyle);

}

DataPointRunContentView::DataPointRunContentView()
  : OSListView()
{
  setContentsMargins(0,5,0,5);
}

DataPointRunItemView::DataPointRunItemView(const openstudio::analysis::DataPoint& dataPoint)
  : OSCollapsibleView()
{
  dataPointRunHeaderView = new DataPointRunHeaderView(dataPoint);
  setHeader(dataPointRunHeaderView);
 
  dataPointRunContentView = new DataPointRunContentView(); 
  setContent(dataPointRunContentView);
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
    m_lastRunTime->setText(toQString(lastRunTime->toString()));
  }else{
    m_lastRunTime->setText("Not Started");
  }
}

void DataPointJobHeaderView::setStatus(const openstudio::runmanager::AdvancedStatus& status)
{
  m_status->setText(toQString(status.toString()));
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
    m_workflowStepJob(workflowStepJob)
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

void DataPointJobItemView::update()
{
  if (m_workflowStepJob.measure) {
    dataPointJobHeaderView->setName(m_workflowStepJob.measure->name());
  }
  else {
    OS_ASSERT(m_workflowStepJob.step.isWorkItem());
    dataPointJobHeaderView->setName(m_workflowStepJob.step.workItemType().valueName());
  }

  OS_ASSERT(m_workflowStepJob.job);

  dataPointJobHeaderView->setLastRunTime(m_workflowStepJob.job->lastRun());
  dataPointJobHeaderView->setStatus(m_workflowStepJob.job->status());

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
