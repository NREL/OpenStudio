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

#include "ApplyMeasureNowDialog.hpp"

#include <shared_gui_components/BusyWidget.hpp>
#include <shared_gui_components/EditController.hpp>
#include <shared_gui_components/EditView.hpp>
#include <shared_gui_components/LocalLibraryController.hpp>
#include <shared_gui_components/LocalLibraryView.hpp>
#include <shared_gui_components/MeasureManager.hpp>
#include <shared_gui_components/OSViewSwitcher.hpp>
#include <shared_gui_components/VariableList.hpp>

#include <openstudio_lib/MainRightColumnController.hpp>
#include <openstudio_lib/OSAppBase.hpp>
#include <openstudio_lib/OSDocument.hpp>
#include <openstudio_lib/OSItem.hpp>

#include <model/Model.hpp>
#include <model/Model_Impl.hpp>

#include <utilities/core/ApplicationPathHelpers.hpp>
#include <utilities/core/PathHelpers.hpp>
#include <utilities/core/RubyException.hpp>

#include <runmanager/lib/AdvancedStatus.hpp>
#include <runmanager/lib/Job.hpp>
#include <runmanager/lib/RunManager.hpp>
#include <runmanager/lib/RubyJobUtils.hpp>
#include <runmanager/lib/Workflow.hpp>

#include <QBoxLayout>
#include <QCloseEvent>
#include <QLabel>
#include <QMessageBox>
#include <QPainter>
#include <QPointer>
#include <QPushButton>
#include <QScrollArea>
#include <QSharedPointer>
#include <QStackedWidget>
#include <QTextEdit>
#include <QTimer>

#include <fstream>

#define FAILED_ARG_TEXT "<FONT COLOR = RED>Failed to Show Arguments<FONT COLOR = BLACK> <br> <br>Reason(s): <br> <br>"

#define ACCEPT_CHANGES "Accept Changes"
#define APPLY_MEASURE "Apply Measure"

namespace openstudio {

ApplyMeasureNowDialog::ApplyMeasureNowDialog(QWidget* parent)
  : OSDialog(false, parent),
  m_editController(0),
  m_mainPaneStackedWidget(0),
  m_rightPaneStackedWidget(0),
  m_argumentsFailedTextEdit(0),
  m_jobItemView(0),
  m_timer(0),
  m_showAdvancedOutput(0),
  m_advancedOutput(QString()),
  m_workingDir(openstudio::path())
{
  setWindowTitle("Apply Measure Now");
  setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  createWidgets();

  openstudio::OSAppBase * app = OSAppBase::instance();
  bool isConnected = connect(this, SIGNAL(reloadFile(const QString&, bool, bool)), app, SLOT(reloadFile(const QString&, bool, bool)), Qt::QueuedConnection);
  OS_ASSERT(isConnected);
}

ApplyMeasureNowDialog::~ApplyMeasureNowDialog()
{
  // DLM: would be nice if this was not needed..
  // restore app state, the app's library controller was swapped out in createWidgets
  openstudio::OSAppBase * app = OSAppBase::instance();
  if (app){
    app->measureManager().setLibraryController(app->currentDocument()->mainRightColumnController()->measureLibraryController()); 
  }
}

QSize ApplyMeasureNowDialog::sizeHint() const
{
  return QSize(770,560);
}

void ApplyMeasureNowDialog::createWidgets()
{
  QWidget * widget = 0;
  QBoxLayout * layout = 0;
  QLabel * label = 0;
  bool isConnected = false;

  openstudio::OSAppBase * app = OSAppBase::instance();

  // PAGE STACKED WIDGET

  m_mainPaneStackedWidget = new  QStackedWidget();
  upperLayout()->addWidget(m_mainPaneStackedWidget);

  // INPUT

  m_argumentsFailedTextEdit = new QTextEdit(FAILED_ARG_TEXT);
  m_argumentsFailedTextEdit->setReadOnly(true);
  
  m_editController = QSharedPointer<EditController>( new EditController(true) );
  bool onlyShowModelMeasures = true;
  m_localLibraryController = QSharedPointer<LocalLibraryController>( new LocalLibraryController(app,onlyShowModelMeasures) );
  m_localLibraryController->localLibraryView->setStyleSheet("QStackedWidget { border-top: 0px; }");

  // DLM: this is changing application state, needs to be undone in the destructor
  app->measureManager().setLibraryController(m_localLibraryController); 
  app->measureManager().updateMeasuresLists();

  m_rightPaneStackedWidget = new  QStackedWidget();
  m_argumentsFailedPageIdx = m_rightPaneStackedWidget->addWidget(m_argumentsFailedTextEdit);

  OSViewSwitcher * viewSwitcher = new OSViewSwitcher();
  viewSwitcher->setView(m_editController->editView);
  m_argumentsOkPageIdx = m_rightPaneStackedWidget->addWidget(viewSwitcher);

  layout = new QHBoxLayout();
  layout->addWidget(m_localLibraryController->localLibraryView);
  layout->addWidget(m_rightPaneStackedWidget);

  widget = new QWidget();
  widget->setLayout(layout);
  m_inputPageIdx = m_mainPaneStackedWidget->addWidget(widget);

  // RUNNING

  label = new QLabel("Running Measure");
  label->setObjectName("H2");

  BusyWidget * busyWidget = new BusyWidget();

  m_timer = new QTimer(this);
  connect(m_timer,SIGNAL(timeout()),busyWidget,SLOT(rotate()));

  layout = new QVBoxLayout();
  layout->addStretch();
  layout->addWidget(label,0,Qt::AlignCenter);
  layout->addWidget(busyWidget,0,Qt::AlignCenter);
  layout->addStretch();
  
  widget = new QWidget();
  widget->setLayout(layout);
  m_runningPageIdx = m_mainPaneStackedWidget->addWidget(widget);

  // OUTPUT

  label = new QLabel("Measure Output");
  label->setObjectName("H1");

  m_jobItemView = new DataPointJobItemView();

  m_jobPath = new QLabel();
  m_jobPath->setTextInteractionFlags(Qt::TextSelectableByMouse);
  #if _NDEBUG
    m_jobPath->hide();
  #endif

  layout = new QVBoxLayout();
  layout->addWidget(label);
  layout->addWidget(m_jobPath);
  layout->addWidget(m_jobItemView,0,Qt::AlignTop);

  layout->addStretch(); 

  m_showAdvancedOutput = new QPushButton("Advanced Output"); 
  isConnected = connect(m_showAdvancedOutput,SIGNAL(clicked(bool)),this,SLOT(showAdvancedOutput()));
  OS_ASSERT(isConnected);

  //layout->addStretch();

  QHBoxLayout * hLayout = new QHBoxLayout();
  hLayout->addWidget(m_showAdvancedOutput);
  hLayout->addStretch();
  layout->addLayout(hLayout);

  widget = new QWidget();
  widget->setLayout(layout);

  QScrollArea * scrollArea = new QScrollArea();
  scrollArea->setWidgetResizable(true);
  scrollArea->setWidget(widget);

  m_outputPageIdx = m_mainPaneStackedWidget->addWidget(scrollArea);

  // SET CURRENT INDEXES

  m_rightPaneStackedWidget->setCurrentIndex(m_argumentsOkPageIdx);

  m_mainPaneStackedWidget->setCurrentIndex(m_inputPageIdx);

  // BUTTONS

  this->okButton()->setText(APPLY_MEASURE);
  this->okButton()->setEnabled(false);

  this->backButton()->show();
  this->backButton()->setEnabled(false);

  // OS SETTINGS

  #ifdef Q_OS_MAC
    setWindowFlags(Qt::FramelessWindowHint);
  #elif defined(Q_OS_WIN32)
    setWindowFlags(Qt::WindowCloseButtonHint | Qt::MSWindowsFixedSizeDialogHint);
  #endif
}

void ApplyMeasureNowDialog::displayMeasure()
{
  this->okButton()->setText(APPLY_MEASURE);
  this->okButton()->show();
  this->okButton()->setEnabled(false);

  m_rightPaneStackedWidget->setCurrentIndex(m_argumentsOkPageIdx);

  m_bclMeasure.reset();
  m_currentMeasureItem.clear();
  m_job.reset();
  m_model.reset();
  m_reloadPath.reset();

  openstudio::OSAppBase * app = OSAppBase::instance();

  QPointer<LibraryItem> selectedItem = m_localLibraryController->selectedItem();

  if (!selectedItem){
    return;
  }

  UUID id = selectedItem->uuid();

  try {
    // Get the selected measure
    m_bclMeasure = app->measureManager().getMeasure(id);
    OS_ASSERT(m_bclMeasure);
    OS_ASSERT(m_bclMeasure->measureType() == MeasureType::ModelMeasure);
   
    // measure
    analysis::RubyMeasure rubyMeasure(*m_bclMeasure);
    try{
      // DLM: we don't want to use this, if we need to reload cached arguments for measure from member map we can
      // we could use this t_project.hasStoredArguments(*projectMeasure)
      
      boost::optional<model::Model> currentModel = app->currentModel();
      OS_ASSERT(currentModel);

      // clone the current model in case arguments getting changes model
      m_model = currentModel->clone().cast<model::Model>();

      // pass in an empty workspace for the idf since you know it is a model measure
      Workspace dummyIdf;
      std::vector<ruleset::OSArgument> args = app->measureManager().argumentGetter()->getArguments(*m_bclMeasure, m_model, dummyIdf);
      rubyMeasure.setArguments(args);

      // DLM: don't save the arguments to the project, if we need to preserve user inputs save to member variable map or something
      //t_project.registerArguments(t_measure, args);

    } catch (const RubyException & e) {
      QString errorMessage("Failed to compute arguments for measure: \n\n");
      errorMessage += QString::fromStdString(e.what());
      errorMessage.prepend(FAILED_ARG_TEXT);
      m_argumentsFailedTextEdit->setText(errorMessage);
      m_rightPaneStackedWidget->setCurrentIndex(m_argumentsFailedPageIdx);
      return;
    }

    m_currentMeasureItem = QSharedPointer<measuretab::MeasureItem>(new measuretab::MeasureItem(rubyMeasure, app));

    bool isConnected = false;
    isConnected = connect(m_currentMeasureItem.data(),SIGNAL(argumentsChanged(bool)),this,SLOT(disableOkButton(bool)));
    OS_ASSERT(isConnected);

    bool hasIncompleteArguments = m_currentMeasureItem->hasIncompleteArguments();
    disableOkButton(hasIncompleteArguments);

    m_currentMeasureItem->setName(m_bclMeasure->name().c_str());
    m_currentMeasureItem->setDescription(m_bclMeasure->description().c_str());

    // DLM: this is ok, call with overload to ignore isItOKToClearResults
    m_editController->setMeasureItem(m_currentMeasureItem.data(), app);

  } catch (const std::exception & e) {
    QString errorMessage("Failed to display measure: \n\n");
    errorMessage += QString::fromStdString(e.what());
    errorMessage.prepend(FAILED_ARG_TEXT);
    m_argumentsFailedTextEdit->setText(errorMessage);
    m_rightPaneStackedWidget->setCurrentIndex(m_argumentsFailedPageIdx);
    return;
  }
}

void ApplyMeasureNowDialog::runMeasure()
{
  runmanager::ConfigOptions co(true);

  if (co.getTools().getAllByName("ruby").tools().size() == 0) 
  {
    QMessageBox::information(this,
      "Missing Ruby",
      "Ruby could not be located.\nOpenStudio will scan for tools.",
      QMessageBox::Ok);

    co.findTools(true);
    openstudio::runmanager::RunManager rm;
    rm.setConfigOptions(co);
    rm.showConfigGui();

    rm.getConfigOptions().saveQSettings();

    emit toolsUpdated();

    if (co.getTools().getAllByName("ruby").tools().size() == 0)
    {
      QMessageBox::information(this,
        "Missing Ruby",
        "Ruby was not located by tool search.\nPlease ensure Ruby correctly installed.\nSimulation aborted.",
        QMessageBox::Ok);

      m_mainPaneStackedWidget->setCurrentIndex(m_inputPageIdx);
      m_timer->stop();
      this->okButton()->hide();
      this->backButton()->hide();

      return;
    }
  }

  m_mainPaneStackedWidget->setCurrentIndex(m_runningPageIdx);
  m_timer->start(50);
  this->okButton()->hide();
  this->backButton()->hide();
  OS_ASSERT(m_model);

  openstudio::OSAppBase * app = OSAppBase::instance();
  m_workingDir = openstudio::toPath(app->currentDocument()->modelTempDir()) / openstudio::toPath("ApplyMeasureNow");
  openstudio::path modelPath = m_workingDir / openstudio::toPath("modelClone.osm");
  openstudio::path epwPath; // DLM: todo look at how this is done in the run tab

  removeWorkingDir();
  
  // save cloned model to temp directory
  m_model->save(modelPath,true); 

  // remove? this is shown only in debug (EW)
  QString path("Measure Output Location: ");
  path.append(toQString(m_workingDir));
  m_jobPath->setText(path);

  analysis::RubyMeasure rubyMeasure = m_currentMeasureItem->measure();

  // DLM: should be able to assert this
  bool hasIncompleteArguments = m_currentMeasureItem->hasIncompleteArguments();
  OS_ASSERT(!hasIncompleteArguments);

  runmanager::RubyJobBuilder rjb(*m_bclMeasure, rubyMeasure.arguments());

  openstudio::path p = getOpenStudioRubyIncludePath();
  QString arg = "-I";
  arg.append(toQString(p));
  rjb.addToolArgument(arg.toStdString());

  openstudio::runmanager::Workflow wf;
  rjb.addToWorkflow(wf);
  wf.add(co.getTools());
  wf.setInputFiles(modelPath, openstudio::path());

  m_job = wf.create(m_workingDir, modelPath);

  // DLM: you could make rm a class member then you would not have to call waitForFinished here
  runmanager::RunManager rm;
  bool queued = rm.enqueue(*m_job, true);
  OS_ASSERT(queued);
  std::vector<runmanager::Job> jobs = rm.getJobs();
  OS_ASSERT(jobs.size() == 1);
  rm.waitForFinished ();

  QTimer::singleShot(0, this, SLOT(displayResults()));
}

void ApplyMeasureNowDialog::displayResults()
{
  analysis::RubyMeasure rubyMeasure = m_currentMeasureItem->measure();

  try{
    m_reloadPath = m_job->allFiles().getLastByFilename("out.osm").fullPath;
  }catch(...){
  }

  m_mainPaneStackedWidget->setCurrentIndex(m_outputPageIdx);
  m_timer->stop();

  this->okButton()->setText(ACCEPT_CHANGES);
  this->okButton()->show();
  if (m_reloadPath){
    this->okButton()->setEnabled(true);
  }else{
    this->okButton()->setEnabled(false);
  }
  this->backButton()->show();
  this->backButton()->setEnabled(true);
  this->cancelButton()->setEnabled(true);

  runmanager::JobErrors jobErrors = m_job->errors();
  OS_ASSERT(m_jobItemView);
  m_jobItemView->update(rubyMeasure, *m_bclMeasure, jobErrors, *m_job);
  m_jobItemView->setExpanded(true);

  if(!jobErrors.errors().empty()){
    this->okButton()->setDisabled(true);
  }

  m_advancedOutput.clear();
  // DLM: always show these files if they exist?
  //if(!jobErrors.succeeded()){
    try{
      runmanager::Files files(m_job->outputFiles());
      openstudio::path stdErrPath = files.getLastByFilename("stderr").fullPath;
      std::ifstream ifs(toString(stdErrPath).c_str());
      std::string stdMessage((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
      ifs.close();
      m_advancedOutput = toQString(stdMessage);
      m_advancedOutput += QString("\n");
    }catch(std::exception&){
    }
  //}

  // DLM: always show these files if they exist?
  //if(!jobErrors.succeeded()){
    try{
      runmanager::Files files(m_job->outputFiles());
      openstudio::path stdOutPath = files.getLastByFilename("stdout").fullPath;
      std::ifstream ifs(toString(stdOutPath).c_str());
      std::string stdMessage((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
      ifs.close();
      m_advancedOutput += toQString(stdMessage);
    }catch(std::exception&){
    }
  //}
 
}

void ApplyMeasureNowDialog::removeWorkingDir()
{
  removeDirectory(m_workingDir);
}

DataPointJobHeaderView::DataPointJobHeaderView()
  : OSHeader(new HeaderToggleButton()),
  m_name(0),
  m_lastRunTime(0),
  m_status(0),
  m_na(0),
  m_warnings(0),
  m_errors(0)
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
  : QWidget(),
  m_textEdit(0)
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

QString DataPointJobContentView::formatMessageForHTML(const std::string &t_message)
{
  QString str = QString::fromStdString(t_message);
  str.replace("\n", "<br>");
  return str;
}

void DataPointJobContentView::addInitialConditionMessage(const std::string& message)
{
  QString html = m_textEdit->text();
  html += QString("<b style=\"color:blue\">Initial Condition</b>: ") + formatMessageForHTML(message) + QString("<br></br>");
  m_textEdit->setText(html);
}

void DataPointJobContentView::addFinalConditionMessage(const std::string& message)
{
  QString html = m_textEdit->text();
  html += QString("<b style=\"color:blue\">Final Condition</b>: ") + formatMessageForHTML(message) + QString("<br></br>");
  m_textEdit->setText(html);
}

void DataPointJobContentView::addInfoMessage(const std::string& message)
{
  QString html = m_textEdit->text();
  html += QString("<b style=\"color:green\">Info</b>: ") + formatMessageForHTML(message) + QString("<br></br>");
  m_textEdit->setText(html);
}

void DataPointJobContentView::addWarningMessage(const std::string& message)
{
  QString html = m_textEdit->text();
  html += QString("<b style=\"color:#C47B06\">Warning</b>: ") + formatMessageForHTML(message) + QString("<br></br>");
  m_textEdit->setText(html);
}

void DataPointJobContentView::addErrorMessage(const std::string& message)
{
  QString html = m_textEdit->text();
  html += QString("<b style=\"color:red\">Error</b>: ") + formatMessageForHTML(message) + QString("<br></br>");
  m_textEdit->setText(html);
}

void DataPointJobContentView::addStdErrorMessage(const std::string& message)
{
  QString html = m_textEdit->text();
  html += formatMessageForHTML(message) + QString("<br></br>");
  m_textEdit->setText(html);
}

DataPointJobItemView::DataPointJobItemView()
  : OSCollapsibleView(true),
  m_dataPointJobHeaderView(0),
  m_dataPointJobContentView(0)
{
  setStyleSheet("openstudio--pat--DataPointJobItemView { background: #C3C3C3; margin-left:10px; }");

  m_dataPointJobHeaderView = new DataPointJobHeaderView();
  setHeader(m_dataPointJobHeaderView);
 
  m_dataPointJobContentView = new DataPointJobContentView(); 
  setContent(m_dataPointJobContentView);
}

void DataPointJobItemView::paintEvent(QPaintEvent * e)
{
  QStyleOption opt;
  opt.init(this);
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void DataPointJobItemView::update(analysis::RubyMeasure & rubyMeasure, BCLMeasure & bclMeasure, openstudio::runmanager::JobErrors jobErrors, openstudio::runmanager::Job job)
{
  OS_ASSERT(m_dataPointJobHeaderView);

  m_dataPointJobHeaderView->setName(rubyMeasure.name());
  m_dataPointJobHeaderView->setLastRunTime(job.lastRun());
  m_dataPointJobHeaderView->setStatus(job.status(), job.canceled());

  m_dataPointJobHeaderView->m_na->setText("");
  m_dataPointJobHeaderView->m_warnings->setText("");
  m_dataPointJobHeaderView->m_errors->setText("");

  OS_ASSERT(m_dataPointJobContentView);
  m_dataPointJobContentView->clear();

  std::vector<std::string> initialConditions = jobErrors.initialConditions();
  for (const std::string& initialCondition : initialConditions){
    m_dataPointJobContentView->addInitialConditionMessage(initialCondition);
  }

  std::vector<std::string> finalConditions = jobErrors.finalConditions();
  for (const std::string& finalCondition : finalConditions){
    m_dataPointJobContentView->addFinalConditionMessage(finalCondition);
  }

  std::vector<std::string> errors = jobErrors.errors();
  m_dataPointJobHeaderView->setNumErrors(errors.size());
  for (const std::string& errorMessage : errors){
    m_dataPointJobContentView->addErrorMessage(errorMessage);
  }

  // also display std err if job failed and it exists and is not empty
  if (job.lastRun() && !job.running() && !jobErrors.succeeded()){
    try{
      runmanager::Files files(job.outputFiles());
      openstudio::path stdErrPath = files.getLastByFilename("stderr").fullPath;
      std::string stdErrPathStr = toString(stdErrPath);
      std::ifstream ifs(stdErrPathStr.c_str());
      std::string stdErrorMessage((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
      ifs.close();
      if (!stdErrorMessage.empty()){
        m_dataPointJobContentView->addStdErrorMessage(stdErrorMessage);
      }
    }catch(std::exception&){

    }
  }

  std::vector<std::string> warnings = jobErrors.warnings();
  m_dataPointJobHeaderView->setNumWarnings(warnings.size());
  for (const std::string& warningMessage : warnings){
    m_dataPointJobContentView->addWarningMessage(warningMessage);
  }

  std::vector<std::string> infos = jobErrors.infos();
  for (const std::string& infoMessage : infos){
    m_dataPointJobContentView->addInfoMessage(infoMessage);
  }

  if (jobErrors.result == ruleset::OSResultValue::NA){
    m_dataPointJobHeaderView->setNA(true);
  }else{
    m_dataPointJobHeaderView->setNA(false);
  }
}

//***** SLOTS *****

void ApplyMeasureNowDialog::on_cancelButton(bool checked)
{
  if(m_mainPaneStackedWidget->currentIndex() == m_inputPageIdx){
    // Nothing specific here
  } else if(m_mainPaneStackedWidget->currentIndex() == m_runningPageIdx) {
    if(m_job){
      m_job->requestStop();
      this->cancelButton()->setDisabled(true);
      this->okButton()->setDisabled(true);
      return;
    }
    m_mainPaneStackedWidget->setCurrentIndex(m_inputPageIdx);
    m_timer->stop();
    this->okButton()->show();
    this->backButton()->show();
    return;
  } else if(m_mainPaneStackedWidget->currentIndex() == m_outputPageIdx) {
    m_mainPaneStackedWidget->setCurrentIndex(m_inputPageIdx);
  }

  // DLM: m_job->requestStop() might still be working, don't try to delete this here
  //removeWorkingDir();

  OSDialog::on_cancelButton(checked);
}

void ApplyMeasureNowDialog::on_backButton(bool checked)
{
  if(m_mainPaneStackedWidget->currentIndex() == m_inputPageIdx){
    // Nothing specific here
  } else if(m_mainPaneStackedWidget->currentIndex() == m_runningPageIdx) {
    // Nothing specific here
  } else if(m_mainPaneStackedWidget->currentIndex() == m_outputPageIdx) {
    this->okButton()->setEnabled(true);
    this->okButton()->setText(APPLY_MEASURE);
    this->backButton()->setEnabled(false);
    m_mainPaneStackedWidget->setCurrentIndex(m_inputPageIdx);
  }
}

void ApplyMeasureNowDialog::on_okButton(bool checked)
{
  if(m_mainPaneStackedWidget->currentIndex() == m_inputPageIdx){
    runMeasure();
  } else if(m_mainPaneStackedWidget->currentIndex() == m_runningPageIdx) {
    // N/A
    OS_ASSERT(false);
  } else if(m_mainPaneStackedWidget->currentIndex() == m_outputPageIdx) {
    // reload the model
    requestReload();
  }
}

void ApplyMeasureNowDialog::requestReload()
{
  // todo: do this in memory without reloading from disk
  OS_ASSERT(m_reloadPath);
  QString fileToLoad = toQString(*m_reloadPath);
  emit reloadFile(fileToLoad, true, true);

  // close the dialog
  close();
}

void ApplyMeasureNowDialog::closeEvent(QCloseEvent *e)
{
  //DLM: don't do this here in case we are going to load the model
  //removeWorkingDir();

  // DLM: do not allow closing window while running
  if(m_mainPaneStackedWidget->currentIndex() == m_runningPageIdx){
    e->ignore();
    return;
  } 

  e->accept();
}

void ApplyMeasureNowDialog::disableOkButton(bool disable)
{
  this->okButton()->setDisabled(disable);
}

void ApplyMeasureNowDialog::showAdvancedOutput()
{
  if(m_advancedOutput.isEmpty()){
    QMessageBox::information(this, QString("Advanced Output"), QString("No advanced output."));
  }else{
    QMessageBox::information(this, QString("Advanced Output"), QString("Advanced output:\n") + m_advancedOutput);
  }
}

} // openstudio
