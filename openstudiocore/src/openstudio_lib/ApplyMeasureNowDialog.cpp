/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#include "ApplyMeasureNowDialog.hpp"

#include "../shared_gui_components/BusyWidget.hpp"
#include "../shared_gui_components/EditController.hpp"
#include "../shared_gui_components/EditView.hpp"
#include "../shared_gui_components/LocalLibraryController.hpp"
#include "../shared_gui_components/LocalLibraryView.hpp"
#include "../shared_gui_components/MeasureManager.hpp"
#include "../shared_gui_components/OSViewSwitcher.hpp"
#include "../shared_gui_components/TextEditDialog.hpp"
#include "../shared_gui_components/WorkflowController.hpp"

#include "MainRightColumnController.hpp"
#include "OSAppBase.hpp"
#include "OSDocument.hpp"
#include "OSItem.hpp"

#include "../model/Model.hpp"
#include "../model/Model_Impl.hpp"

#include "../openstudio_app/OpenStudioApp.hpp"

#include "../utilities/core/PathHelpers.hpp"
#include "../utilities/core/RubyException.hpp"
#include "../utilities/filetypes/WorkflowJSON.hpp"
#include "../utilities/filetypes/WorkflowStep.hpp"
#include "../utilities/filetypes/WorkflowStepResult.hpp"
#include "../utilities/time/DateTime.hpp"

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
#include <QStandardPaths>

#include <fstream>

#define FAILED_ARG_TEXT "<FONT COLOR = RED>Failed to Show Arguments<FONT COLOR = BLACK> <br> <br>Reason(s): <br> <br>"

#define ACCEPT_CHANGES "Accept Changes"
#define APPLY_MEASURE "Apply Measure"

namespace openstudio {

ApplyMeasureNowDialog::ApplyMeasureNowDialog(QWidget* parent)
  : OSDialog(false, parent),
  m_editController(nullptr),
  m_mainPaneStackedWidget(nullptr),
  m_rightPaneStackedWidget(nullptr),
  m_argumentsFailedTextEdit(nullptr),
  m_jobItemView(nullptr),
  m_timer(nullptr),
  m_showAdvancedOutput(nullptr),
  m_advancedOutput(QString()),
  m_workingDir(openstudio::path()),
  m_advancedOutputDialog(nullptr)
{
  setWindowTitle("Apply Measure Now");
  setWindowModality(Qt::ApplicationModal);
  setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  createWidgets();

  OSAppBase * app = OSAppBase::instance();
  connect(this, &ApplyMeasureNowDialog::reloadFile, static_cast<OpenStudioApp *>(app), &OpenStudioApp::reloadFile, Qt::QueuedConnection);

  m_advancedOutputDialog = new TextEditDialog("Advanced Output");

  //m_workingDir = toPath("E:/test/ApplyMeasureNow");
  m_workingDir = openstudio::toPath(app->currentDocument()->modelTempDir()) / openstudio::toPath("ApplyMeasureNow");

  // save the model's workflow JSON
  m_modelWorkflowJSON = app->currentModel()->workflowJSON();

  // set a temporary workflow JSON
  m_tempWorkflowJSON = WorkflowJSON();

  // use the temp model as the seed
  m_tempWorkflowJSON.setSeedFile(app->measureManager().tempModelPath());

  // copy the weather file reference
  boost::optional<openstudio::path> weatherFile = m_modelWorkflowJSON.weatherFile();
  if (weatherFile){
    m_tempWorkflowJSON.setWeatherFile(*weatherFile);
  }

  // add file paths from current workflow so we can find weather file
  for (const auto& absoluteFilePath : m_modelWorkflowJSON.absoluteFilePaths()){
    m_tempWorkflowJSON.addFilePath(absoluteFilePath);
  }

  m_tempWorkflowJSON.saveAs(m_workingDir / toPath("temp.osw"));

  // temporarily swap app's workflow for this one
  app->currentModel()->setWorkflowJSON(m_tempWorkflowJSON);
}

ApplyMeasureNowDialog::~ApplyMeasureNowDialog()
{
  // DLM: would be nice if this was not needed..
  // restore app state, the app's library controller was swapped out in createWidgets
  openstudio::OSAppBase * app = OSAppBase::instance();
  if (app){
    app->measureManager().setLibraryController(app->currentDocument()->mainRightColumnController()->measureLibraryController()); 

    // restore the model's workflow JSON
    // DLM: this is not sufficient if the OSM is being reloaded
    app->currentModel()->setWorkflowJSON(m_modelWorkflowJSON);
  }

  if(m_advancedOutputDialog){
    delete m_advancedOutputDialog;
  }
}

QSize ApplyMeasureNowDialog::sizeHint() const
{
  return QSize(770,560);
}

void ApplyMeasureNowDialog::createWidgets()
{
  QWidget * widget = nullptr;
  QBoxLayout * layout = nullptr;
  QLabel * label = nullptr;

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
  m_localLibraryController->localLibraryView->addBCLMeasureButton->setVisible(false);

  // DLM: this is changing application state, needs to be undone in the destructor
  app->measureManager().setLibraryController(m_localLibraryController); 
  app->currentDocument()->disable();
  app->measureManager().updateMeasuresLists();
  app->currentDocument()->enable();

  m_rightPaneStackedWidget = new  QStackedWidget();
  m_argumentsFailedPageIdx = m_rightPaneStackedWidget->addWidget(m_argumentsFailedTextEdit);

  auto viewSwitcher = new OSViewSwitcher();
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

  auto busyWidget = new BusyWidget();

  m_timer = new QTimer(this);
  connect(m_timer, &QTimer::timeout, busyWidget, &BusyWidget::rotate);

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
  #if !(_DEBUG || (__GNUC__ && !NDEBUG))
    m_jobPath->hide();
  #endif

  layout = new QVBoxLayout();
  layout->addWidget(label);
  layout->addWidget(m_jobPath);
  layout->addWidget(m_jobItemView,0,Qt::AlignTop);

  layout->addStretch(); 

  m_showAdvancedOutput = new QPushButton("Advanced Output"); 
  connect(m_showAdvancedOutput, &QPushButton::clicked, this, &ApplyMeasureNowDialog::showAdvancedOutput);

  //layout->addStretch();

  auto hLayout = new QHBoxLayout();
  hLayout->addWidget(m_showAdvancedOutput);
  hLayout->addStretch();
  layout->addLayout(hLayout);

  widget = new QWidget();
  widget->setLayout(layout);

  auto scrollArea = new QScrollArea();
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
  #elif defined(Q_OS_WIN)
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
  m_currentMeasureStepItem.clear();
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

    m_tempWorkflowJSON.resetMeasurePaths();
    m_tempWorkflowJSON.addMeasurePath(m_bclMeasure->directory().parent_path());

    MeasureStep step(toString(m_bclMeasure->directory().stem()));
    std::vector<WorkflowStep> steps;
    steps.push_back(step);
    m_tempWorkflowJSON.setWorkflowSteps(steps);
    
    m_tempWorkflowJSON.save();

    m_currentMeasureStepItem = QSharedPointer<measuretab::MeasureStepItem>(new measuretab::MeasureStepItem(MeasureType::ModelMeasure, step, app));

    connect(m_currentMeasureStepItem.data(), &measuretab::MeasureStepItem::argumentsChanged, this, &ApplyMeasureNowDialog::disableOkButton);

    m_currentMeasureStepItem->arguments();
    bool hasIncompleteArguments = m_currentMeasureStepItem->hasIncompleteArguments();
    disableOkButton(hasIncompleteArguments);

    //m_currentMeasureStepItem->setName(m_bclMeasure->name().c_str());
    m_currentMeasureStepItem->setName(m_bclMeasure->displayName().c_str());
    //m_currentMeasureStepItem->setDisplayName(m_bclMeasure->displayName().c_str());
    m_currentMeasureStepItem->setDescription(m_bclMeasure->description().c_str());

    m_editController->setMeasureStepItem(m_currentMeasureStepItem.data(), app);

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
  m_mainPaneStackedWidget->setCurrentIndex(m_runningPageIdx);
  m_timer->start(50);
  this->okButton()->hide();
  this->backButton()->hide();

  openstudio::OSAppBase * app = OSAppBase::instance();

  removeWorkingDir();

  m_tempWorkflowJSON.save();

  // DLM: should be able to assert this
  bool hasIncompleteArguments = m_currentMeasureStepItem->hasIncompleteArguments();
  OS_ASSERT(!hasIncompleteArguments);

  m_runProcess = new QProcess(this);
  connect(m_runProcess, static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished), this, &ApplyMeasureNowDialog::displayResults);
    
  QStringList paths;
  paths << QCoreApplication::applicationDirPath();
  auto openstudioExePath = QStandardPaths::findExecutable("openstudio", paths);

  boost::optional<openstudio::path> tempWorkflowJSONPath = m_tempWorkflowJSON.oswPath();
  OS_ASSERT(tempWorkflowJSONPath);

  QStringList arguments;
  arguments << "run" << "-m" << "-w" << toQString(*tempWorkflowJSONPath);

  m_runProcess->start(openstudioExePath, arguments);

}

void ApplyMeasureNowDialog::displayResults()
{
  QString qstdout;
  QString qstderr;
  if (m_runProcess){
    qstdout.append(m_runProcess->readAllStandardOutput());
    qstderr.append(m_runProcess->readAllStandardError());
  }

  delete m_runProcess;
  m_runProcess = nullptr;

  m_reloadPath = m_workingDir / toPath("run/in.osm");
  openstudio::path outWorkflowJSONPath = m_workingDir / toPath("out.osw");

  m_mainPaneStackedWidget->setCurrentIndex(m_outputPageIdx);
  m_timer->stop();

  this->okButton()->setText(ACCEPT_CHANGES);
  this->okButton()->show();
  if (boost::filesystem::exists(*m_reloadPath)){
    this->okButton()->setEnabled(true);
  } else{
    this->okButton()->setEnabled(false);
  }
  this->backButton()->show();
  this->backButton()->setEnabled(true);
  this->cancelButton()->setEnabled(true);

  boost::optional<WorkflowJSON> outWorkflowJSON = WorkflowJSON::load(outWorkflowJSONPath);

  m_jobItemView->update(*m_bclMeasure, outWorkflowJSON, false);
  m_jobItemView->setExpanded(true);

  if(!outWorkflowJSON || !outWorkflowJSON->completedStatus() || outWorkflowJSON->completedStatus().get() != "Success"){
    this->okButton()->setDisabled(true);
  }

  m_advancedOutput.clear();

  try{

    m_advancedOutput = "";

    m_advancedOutput += "<b>Standard Output:</b>\n";
    m_advancedOutput += qstdout;
    m_advancedOutput += QString("\n");

    m_advancedOutput += "<b>Standard Error:</b>\n";
    m_advancedOutput += qstderr;
    m_advancedOutput += QString("\n");
     
    openstudio::path logPath =  m_workingDir / toPath("run/run.log");
    m_advancedOutput += "<b>run.log:</b>\n";
    QFile file(toQString(logPath));
    if (file.open(QFile::ReadOnly))
    {
      QTextStream docIn(&file);
      m_advancedOutput += docIn.readAll();
      file.close();
    }

    m_advancedOutput += QString("\n");

    m_advancedOutput.replace("\n", "<br>");
   
  }catch(std::exception&){
  }
 
}

void ApplyMeasureNowDialog::removeWorkingDir()
{
  removeDirectory(m_workingDir);
}

DataPointJobHeaderView::DataPointJobHeaderView()
  : OSHeader(new HeaderToggleButton()),
  m_name(nullptr),
  m_lastRunTime(nullptr),
  m_status(nullptr),
  m_na(nullptr),
  m_warnings(nullptr),
  m_errors(nullptr)
{
  auto mainHLayout = new QHBoxLayout();
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

void DataPointJobHeaderView::setStatus(const std::string& status, bool isCanceled)
{
  if (!isCanceled)
  {
    m_status->setText(toQString(status));
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
  m_textEdit(nullptr)
{
  auto mainHLayout = new QHBoxLayout();
  mainHLayout->setContentsMargins(15,5,5,5);
  mainHLayout->setSpacing(0);
  mainHLayout->setAlignment(Qt::AlignLeft);
  setLayout(mainHLayout);

  m_textEdit = new QLabel();
  m_textEdit->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
  m_textEdit->setWordWrap(true);
  m_textEdit->setOpenExternalLinks(true);

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
  m_dataPointJobHeaderView(nullptr),
  m_dataPointJobContentView(nullptr)
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

void DataPointJobItemView::update(const BCLMeasure & bclMeasure, const boost::optional<WorkflowJSON>& workflowJSON, bool canceled)
{
  OS_ASSERT(m_dataPointJobHeaderView);

  m_dataPointJobHeaderView->setName(bclMeasure.className());

  m_dataPointJobHeaderView->m_na->setText("");
  m_dataPointJobHeaderView->m_warnings->setText("");
  m_dataPointJobHeaderView->m_errors->setText("");

  OS_ASSERT(m_dataPointJobContentView);
  m_dataPointJobContentView->clear();

  if (!workflowJSON){
    // unknown error
    return;
  }

  if(!workflowJSON->completedStatus() || workflowJSON->completedStatus().get() != "Success"){
    // error
  }

  boost::optional<DateTime> completedAt = workflowJSON->completedAt();
  if (completedAt){
    m_dataPointJobHeaderView->setLastRunTime(*completedAt);
  }

  boost::optional<std::string> completedStatus = workflowJSON->completedStatus();
  if (completedStatus){
    m_dataPointJobHeaderView->setStatus(*completedStatus, canceled);
  }else{
    m_dataPointJobHeaderView->setStatus("Unknown", canceled);
  }

  for (const auto& step : workflowJSON->workflowSteps()){

    boost::optional<WorkflowStepResult> result = step.result();
    if (!result){
      continue;
    }

    boost::optional<std::string> initialCondition = result->stepInitialCondition();
    if (initialCondition){
      m_dataPointJobContentView->addInitialConditionMessage(*initialCondition);
    }

    boost::optional<std::string> finalCondition = result->stepFinalCondition();
    if (finalCondition){
      m_dataPointJobContentView->addFinalConditionMessage(*finalCondition);
    }

    std::vector<std::string> errors = result->stepErrors();
    m_dataPointJobHeaderView->setNumErrors(errors.size());
    for (const std::string& errorMessage : errors){
      m_dataPointJobContentView->addErrorMessage(errorMessage);
    }

    std::vector<std::string> warnings = result->stepWarnings();
    m_dataPointJobHeaderView->setNumWarnings(warnings.size());
    for (const std::string& warningMessage : warnings){
      m_dataPointJobContentView->addWarningMessage(warningMessage);
    }

    std::vector<std::string> infos = result->stepInfo();
    m_dataPointJobHeaderView->setNumWarnings(infos.size());
    for (const std::string& info : infos){
      m_dataPointJobContentView->addInfoMessage(info);
    }

    // there should only be on step so this is ok
    boost::optional<StepResult> stepResult = result->stepResult();
    if (stepResult && stepResult->value() == StepResult::NA){
      m_dataPointJobHeaderView->setNA(true);
    }else{
      m_dataPointJobHeaderView->setNA(false);
    }

  }

}

/***** SLOTS *****/

void ApplyMeasureNowDialog::on_cancelButton(bool checked)
{
  if(m_mainPaneStackedWidget->currentIndex() == m_inputPageIdx){
    // Nothing specific here
  } else if(m_mainPaneStackedWidget->currentIndex() == m_runningPageIdx) {
// TODO: fix
//    if(m_job){
//      m_job->requestStop();
//      this->cancelButton()->setDisabled(true);
//      this->okButton()->setDisabled(true);
//      return;
//    }
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
    m_advancedOutputDialog->setText(m_advancedOutput);
    m_advancedOutputDialog->setSizeHint(QSize(this->geometry().width(), this->geometry().height()));
    m_advancedOutputDialog->exec();
    m_advancedOutputDialog->raise();
  }
}

} // openstudio
