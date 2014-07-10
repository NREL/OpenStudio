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

#include "PatApp.hpp"

#include <pat_app/AboutBox.hpp>
#include "CloudMonitor.hpp"
#include "DesignAlternativesTabController.hpp"
#include "DesignAlternativesView.hpp"
#include "ExportXML.hpp"
#include "ExportSpreadsheet.hpp"
#include "HorizontalTabWidget.hpp"
#include "MainRightColumnController.hpp"
#include "MeasuresTabController.hpp"
#include "MeasuresView.hpp"
#include "PatMainMenu.hpp"
#include "PatMainWindow.hpp"
#include "PatVerticalTabWidget.hpp"
#include "ResultsTabController.hpp"
#include "ResultsView.hpp"
#include "RunTabController.hpp"
#include "RunView.hpp"
#include "StartupView.hpp"
#include <pat_app/VagrantConfiguration.hxx>

#include "../shared_gui_components/BCLMeasureDialog.hpp"
#include "../shared_gui_components/BuildingComponentDialog.hpp"
#include "../shared_gui_components/EditController.hpp"
#include "../shared_gui_components/LocalLibraryController.hpp"
#include "../shared_gui_components/LocalLibraryView.hpp"
#include "../shared_gui_components/OSViewSwitcher.hpp"
#include "../shared_gui_components/ProcessEventsProgressBar.hpp"
#include "../shared_gui_components/WorkflowTools.hpp"

#include "../analysis/Analysis.hpp"
#include "../analysis/AnalysisObject.hpp"
#include "../analysis/AnalysisObject_Impl.hpp"
#include "../analysis/MeasureGroup.hpp"
#include "../analysis/MeasureGroup_Impl.hpp"
#include "../analysis/RubyMeasure.hpp"
#include "../analysis/RubyMeasure_Impl.hpp"
#include "../analysisdriver/CurrentAnalysis.hpp"
#include "../analysisdriver/SimpleProject_Impl.hpp"

#include "../project/ProjectDatabase.hpp"

#include "../runmanager/lib/RubyJobUtils.hpp"
#include "../runmanager/lib/RunManager.hpp"

#include "../utilities/bcl/BCLMeasure.hpp"
#include "../utilities/bcl/LocalBCL.hpp"
#include "../utilities/cloud/AWSProvider.hpp"
#include "../utilities/cloud/AWSProvider_Impl.hpp"
#include "../utilities/cloud/CloudProvider.hpp"
#include "../utilities/cloud/CloudProvider_Impl.hpp"
#include "../utilities/cloud/VagrantProvider.hpp"
#include "../utilities/cloud/VagrantProvider_Impl.hpp"
#include "../utilities/core/Application.hpp"
#include "../utilities/core/ApplicationPathHelpers.hpp"
#include "../utilities/core/Assert.hpp"
#include "../utilities/core/PathHelpers.hpp"
#include "../utilities/core/System.hpp"
#include "../utilities/core/ZipFile.hpp"
#include "../utilities/idf/IdfFile.hpp"

#include <OpenStudio.hxx>

#include <boost/filesystem.hpp>

#include <QAbstractButton>
#include <QBoxLayout>
#include <QDesktopServices>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QMouseEvent>
#include <QProgressDialog>
#include <QPushButton>
#include <QRadioButton>
#include <QRegExp>
#include <QRegExpValidator>
#include <QTimer>
#include <QUrl>

namespace openstudio {

namespace pat {

PatApp::PatApp( int & argc, char ** argv, const QSharedPointer<ruleset::RubyUserScriptArgumentGetter> &t_argumentGetter )
  : QApplication(argc, argv),
    m_onlineBclDialog(nullptr),
    m_cloudDialog(nullptr),
    m_measureManager(t_argumentGetter, this)
{
  bool isConnected = connect(this,SIGNAL(userMeasuresDirChanged()),&m_measureManager,SLOT(updateMeasuresLists()));
  OS_ASSERT(isConnected);

  setOrganizationName("NREL");
  setOrganizationDomain("nrel.gov");
  setApplicationName("ParametricAnalysisTool");

  QFile data(":/pat.qss");
  QString style;
  if(data.open(QFile::ReadOnly))
  {
    QTextStream styleIn(&data);
    style = styleIn.readAll();
    data.close();
    setStyleSheet(style);
  }

  #ifdef Q_OS_MAC
    setQuitOnLastWindowClosed( true );
  #endif

  QStringList args = QApplication::arguments();
  args.removeFirst(); // application name

  QString fileName;
  if (args.size() == 1){
    QFileInfo info(args.at(0)); // handles windows links and "\"
    fileName = info.absoluteFilePath();
  }else if(args.size() > 1){
    LOG_FREE(Warn, "PatApp", "Incorrect number of arguments " << args.size());
  }

  m_cloudMonitor = QSharedPointer<CloudMonitor>(new CloudMonitor());

  isConnected = connect(m_cloudMonitor.data(),SIGNAL(cloudStatusChanged(const CloudStatus &)),
                        this,SLOT(onCloudStatusChanged(const CloudStatus &)));
  OS_ASSERT(isConnected);

  mainWindow = new PatMainWindow();

  m_startupView = new StartupView();

  m_loadingProjectView = new LoadingProjectView();

  // Main Right Column
  m_mainRightColumnController = QSharedPointer<MainRightColumnController>(new MainRightColumnController()),
  mainWindow->setMainRightColumnView(m_mainRightColumnController->horizontalTabWidget);
  m_measureManager.setLibraryController(m_mainRightColumnController->localLibraryController());

  // Main Vertical Tabs

  // Measures

  mainWindow->verticalTabWidget->addTab( MEASURES,
                                         "Measures",
                                         ":images/tab_measures_on.png",
                                         ":images/tab_measures_off.png",
                                         ":images/tab_measures_disabled.png",
                                         ":images/tab_measures_done.png",
                                         false );

  // DesignAlternatives

  mainWindow->verticalTabWidget->addTab( DESIGN_ALTERNATIVES,
                                         "Design Alternatives",
                                         ":images/tab_design_alts_on.png",
                                         ":images/tab_design_alts_off.png",
                                         ":images/tab_design_alts_disabled.png",
                                         ":images/tab_design_alts_done.png",
                                         false );

  // Run

  mainWindow->verticalTabWidget->addTab( RUN,
                                         "Run",
                                         ":images/tab_run_on.png",
                                         ":images/tab_run_off.png",
                                         ":images/tab_run_disabled.png",
                                         ":images/tab_run_done.png",
                                         false );

  // Results

  mainWindow->verticalTabWidget->addTab( RESULTS,
                                         "Results",
                                         ":images/tab_results_on.png",
                                         ":images/tab_results_off.png",
                                         ":images/tab_results_disabled.png",
                                         ":images/tab_results_done.png",
                                         false );

  isConnected = connect(mainWindow->verticalTabWidget,SIGNAL(tabSelected(int)),this,SLOT(showVerticalTab(int)));
  OS_ASSERT(isConnected);

  isConnected = connect(mainWindow, SIGNAL(loadFileClicked()), this, SLOT(open()));
  OS_ASSERT(isConnected);

  isConnected = connect(mainWindow, SIGNAL(newClicked()), this, SLOT(create()));
  OS_ASSERT(isConnected);

  isConnected = connect(mainWindow, SIGNAL(exitClicked()),this,SLOT(quit()));
  OS_ASSERT(isConnected);

  isConnected = connect(mainWindow, SIGNAL(openBclDlgClicked()),this,SLOT(openBclDlg()));
  OS_ASSERT(isConnected);

  isConnected = connect(mainWindow, SIGNAL(openCloudDlgClicked()),this,SLOT(openCloudDlg()));
  OS_ASSERT(isConnected);

  isConnected = connect(mainWindow, SIGNAL(openMonitorUseDlgClicked()),this,SLOT(openMonitorUseDlg()));
  OS_ASSERT(isConnected);

  isConnected = connect(mainWindow, SIGNAL(helpClicked()),this,SLOT(showHelp()));
  OS_ASSERT(isConnected);

  isConnected = connect(mainWindow, SIGNAL(aboutClicked()),this,SLOT(showAbout()));
  OS_ASSERT(isConnected);

  isConnected = connect(mainWindow, SIGNAL(saveAsFileClicked()), this, SLOT(saveAs()));
  OS_ASSERT(isConnected);

  isConnected = connect(mainWindow, SIGNAL(saveFileClicked()), this, SLOT(save()));
  OS_ASSERT(isConnected);

  isConnected = connect(mainWindow, SIGNAL(clearAllResultsClicked()), this, SLOT(clearAllResults()));
  OS_ASSERT(isConnected);

  isConnected = connect(mainWindow, SIGNAL(exportXmlClicked()), this, SLOT(exportXml()));
  OS_ASSERT(isConnected);

  isConnected = connect(mainWindow, SIGNAL(exportSpreadsheetClicked()), this, SLOT(exportSpreadsheet()));
  OS_ASSERT(isConnected);

  isConnected = connect(mainWindow, SIGNAL(scanForToolsClicked()), this, SLOT(scanForTools()));
  OS_ASSERT(isConnected);

  isConnected = connect(mainWindow, SIGNAL(showToolsClicked()), this, SLOT(showTools()));
  OS_ASSERT(isConnected);

  isConnected = connect(mainWindow, SIGNAL(changeMeasuresClicked()), this, SLOT(changeUserMeasuresDir()));
  OS_ASSERT(isConnected);

  mainWindow->show();

  if (!QDir().exists(toQString(BCLMeasure::userMeasuresDir()))){
    BCLMeasure::setUserMeasuresDir(BCLMeasure::userMeasuresDir());
  }

  m_measureManager.updateMeasuresLists();

  if (!fileName.isEmpty()){
    if (openFile(fileName)){
      return;
    }
  }


  attachProject(boost::none);
}

PatApp::~PatApp()
{
  delete mainWindow;

  delete m_startupView;

  delete m_loadingProjectView;
}

PatApp * PatApp::instance()
{
  return qobject_cast<PatApp *>(QApplication::instance());
}

void PatApp::save()
{
  if (m_project){
    // DLM: do we have to worry about if we are running or not?

    runmanager::RunManager runManager = m_project->runManager();
    bool wasPaused = runManager.paused();
    runManager.setPaused(true);

    m_project->save();

    runManager.setPaused(wasPaused);
  }
}

void PatApp::saveAs()
{
  CloudStatus status = cloudMonitor()->status();

  if (status == CLOUD_STARTING || status == CLOUD_STOPPING || status == CLOUD_RUNNING) {
    QMessageBox::warning(mainWindow, 
      "Cannot Save Copy", 
      "PAT cannot save a copy of the current project while the cloud is starting, running, or stopping.  Once the cloud connection is terminated this operation will become available.", 
      QMessageBox::Ok);

    return;

  } else if (status == CLOUD_ERROR) {
    QMessageBox::warning(mainWindow, 
      "Cannot Save Copy", 
      "PAT cannot save a copy of the current project while the cloud is in an error state.  Once the cloud connection is established or the session is terminated this operation will become available.", 
      QMessageBox::Ok);

    return;
  }

  userInteractiveSaveAsProject();
}

void PatApp::chooseHorizontalEditTab()
{
  mainRightColumnController()->horizontalTabWidget->setCurrentId(MainRightColumnController::EDIT);
}

QSharedPointer<EditController> PatApp::editController()
{
  return mainRightColumnController()->editController();
}


void PatApp::quit(bool fromCloseEvent)
{
  if (!fromCloseEvent) {

    // This duplicates the PatMainWindow closeEvent.  This is only used when File->Exit is called which bypasses that closeEvent
    CloudStatus status = cloudMonitor()->status();

    if (status == CLOUD_STARTING || status == CLOUD_STOPPING) {
      QMessageBox::warning(mainWindow, 
        "Cannot Exit PAT", 
        "PAT cannot be closed while the cloud is starting or stopping.  The current cloud operation should be completed shortly.", 
        QMessageBox::Ok);

      return;

    } else if (status == CLOUD_RUNNING) {

      // if project is running we can quit, user might want to leave cloud on
      // if project is idle we can quit, 99% sure we should turn cloud off
      // if project is starting can't quit
      // if project is stopping we can quit, 90% sure we should turn cloud off
      // if project is error we can quit, 90% sure we should turn cloud off
      boost::optional<analysisdriver::SimpleProject> project = PatApp::instance()->project();
      if (project && (project->status() == analysisdriver::AnalysisStatus::Starting)){
        QMessageBox::warning(mainWindow, 
          "Cannot Exit PAT", 
          "PAT cannot be closed while the remote analysis is starting.  The current cloud operation should be completed shortly.", 
          QMessageBox::Ok);
        return;
      }else{
        int result = QMessageBox::warning(mainWindow, 
                       "Close PAT?", 
                       "The cloud is currently running and charges are accruing.  Are you sure you want to exit PAT?", 
                       QMessageBox::Ok, 
                       QMessageBox::Cancel);

        if(result == QMessageBox::Cancel) return;
      }

    } else if (status == CLOUD_STOPPED) {
    // DLM: check if running locally?

    } else if (status == CLOUD_ERROR) {
      int result = QMessageBox::warning(mainWindow, 
                     "Close PAT?", 
                     "You are disconnected from the cloud, but it may currently be running and accruing charges.  Are you sure you want to exit PAT?", 
                     QMessageBox::Ok, 
                     QMessageBox::Cancel);

      if(result == QMessageBox::Cancel) return;
    }
  }

  // no more clicking around we are quitting
  mainWindow->setEnabled(false);
  this->processEvents();

  if (m_project){
    runmanager::RunManager rm = m_project->runManager();
    rm.disconnect();
    rm.setPaused(true);

    analysis::Analysis analysis = m_project->analysis();
    analysis.disconnect();

    // ask if user wants to save or not, they cannot cancel
    userInteractiveSaveProject(false);

    m_project->stop();
    this->processEvents();

    m_project.reset();
  }

  QApplication::quit();
}

void PatApp::open()
{
  CloudStatus status = cloudMonitor()->status();

  if (status == CLOUD_STARTING || status == CLOUD_STOPPING) {
    QMessageBox::warning(mainWindow, 
      "Cannot Open Project", 
      "Projects cannot be opened while the cloud is starting or stopping.  The current cloud operation should be completed shortly.", 
      QMessageBox::Ok);

    return;

  } else if (status == CLOUD_RUNNING) {

    // if project is running we can open, user might want to leave cloud on
    // if project is idle we can quit, 99% sure we should turn cloud off
    // if project is starting can't open
    // if project is stopping we can open, 90% sure we should turn cloud off
    // if project is error we can open, 90% sure we should turn cloud off
    boost::optional<analysisdriver::SimpleProject> project = PatApp::instance()->project();
    if (project && (project->status() == analysisdriver::AnalysisStatus::Starting)){
      QMessageBox::warning(mainWindow, 
        "Cannot Open Project", 
        "Projects cannot be opened while the remote analysis is starting.  The current cloud operation should be completed shortly.", 
        QMessageBox::Ok);
      return;
    }else{
      int result = QMessageBox::warning(mainWindow, 
                      "Open Project?", 
                      "The cloud is currently running and charges are accruing.  Are you sure you want to open a different project?", 
                      QMessageBox::Ok, 
                      QMessageBox::Cancel);

      if(result == QMessageBox::Cancel) return;
    }

  } else if (status == CLOUD_STOPPED) {

  // DLM: check if running locally?

  } else if (status == CLOUD_ERROR) {
    int result = QMessageBox::warning(mainWindow, 
                    "Open Project?", 
                    "You are disconnected from the cloud, but it may currently be running and accruing charges.  Are you sure you want to open a different project?", 
                    QMessageBox::Ok, 
                    QMessageBox::Cancel);

    if(result == QMessageBox::Cancel) return;
  }

  QString fileName = QFileDialog::getOpenFileName( mainWindow,
                                                   tr("Open Project"),
                                                   mainWindow->lastPath(),
                                                   tr("(project.osp)") );
  if (!fileName.length()) return;

  mainWindow->setLastPath(QFileInfo(fileName).path());
  userInteractiveOpenProject(fileName);
}

void PatApp::create()
{
  CloudStatus status = cloudMonitor()->status();

  if (status == CLOUD_STARTING || status == CLOUD_STOPPING) {
    QMessageBox::warning(mainWindow, 
      "Cannot Create New Project", 
      "Projects cannot be created while the cloud is starting or stopping.  The current cloud operation should be completed shortly.", 
      QMessageBox::Ok);

    return;

  } else if (status == CLOUD_RUNNING) {

    // if project is running we can create, user might want to leave cloud on
    // if project is idle we can quit, 99% sure we should turn cloud off
    // if project is starting can't create
    // if project is stopping we can create, 90% sure we should turn cloud off
    // if project is error we can create, 90% sure we should turn cloud off
    boost::optional<analysisdriver::SimpleProject> project = PatApp::instance()->project();
    if (project && (project->status() == analysisdriver::AnalysisStatus::Starting)){
      QMessageBox::warning(mainWindow, 
        "Cannot Create New Project", 
        "Projects cannot be created while the remote analysis is starting.  The current cloud operation should be completed shortly.", 
        QMessageBox::Ok);
      return;
    }else{
      int result = QMessageBox::warning(mainWindow, 
                      "Create Project?", 
                      "The cloud is currently running and charges are accruing.  Are you sure you want to create and open a new project?", 
                      QMessageBox::Ok, 
                      QMessageBox::Cancel);

      if(result == QMessageBox::Cancel) return;
    }

  } else if (status == CLOUD_STOPPED) {

  // DLM: check if running locally?

  } else if (status == CLOUD_ERROR) {
    int result = QMessageBox::warning(mainWindow, 
                    "Create Project?", 
                    "You are disconnected from the cloud, but it may currently be running and accruing charges.  Are you sure you want to create and open a new project?", 
                    QMessageBox::Ok, 
                    QMessageBox::Cancel);

    if(result == QMessageBox::Cancel) return;
  }

  auto dialog = new NewProjectDialog();

  QString projectName;
  if(dialog->exec()){
    projectName = dialog->m_projectNameLineEdit->text().simplified();
  }

  delete dialog;

  if(!projectName.length()) return;

  QString dir = QFileDialog::getExistingDirectory( mainWindow,
                                                   tr("Choose New Project Folder"),
                                                   QDir::homePath());

  if(dir.length() > 0){
    // check that project does not already exist
    QDir desiredDir(dir);
    if(desiredDir.exists(projectName)){
      QMessageBox::critical(mainWindow,
        "Error Creating Project",
        QString("Project '") + projectName + QString("' already exists.\nChoose a new project name, or a new project folder."));
      return;
    }

    // check if already have a project open
    if (m_project){
      // see if user wants to close the current project
      if (!userInteractiveCloseProject()){
        return;
      }
    }

    QString fileName(dir);
    fileName += "/";
    fileName += projectName;
    openstudio::path projectDir = openstudio::toPath(fileName);
    openstudio::analysisdriver::SimpleProjectOptions options;
    options.setPauseRunManagerQueue(true); // do not start running when creating
    options.setInitializeRunManagerUI(false);
    options.setLogLevel(Debug);
    boost::optional<openstudio::analysisdriver::SimpleProject> project = analysisdriver::createPATProject(projectDir, options);
    if(project.is_initialized()){
      OS_ASSERT(project->isPATProject());

      attachProject(project);

      mainWindow->setWindowTitle("");
      mainWindow->setWindowFilePath(toQString(projectDir));
      //project analysis is dirty
      QTimer::singleShot(0, this, SLOT(markAsModified()));
    } else {
      QMessageBox::warning(mainWindow,
        "Error Creating Project",
        QString("Could not create project in '") + fileName + QString("'."));
    }
  }
}

void PatApp::openBclDlg()
{
  std::string authKey = LocalBCL::instance().prodAuthKey();
  if (!LocalBCL::instance().setProdAuthKey(authKey)){
    LocalBCL::instance().prodAuthKeyUserPrompt(mainWindow);
    authKey = LocalBCL::instance().prodAuthKey();

    while(!LocalBCL::instance().setProdAuthKey(authKey)){
      QMessageBox dlg(mainWindow);
      dlg.setWindowTitle(tr("Online BCL"));
      dlg.setText("The BCL auth key is invalid, and Online BCL data will not be available.  To learn how to obtain a valid BCL auth key, click <a href='http://openstudio.nrel.gov/using-building-component-library-bcl-key-openstudio'>here</a>.");
      dlg.setTextFormat(Qt::RichText);
      dlg.addButton(QMessageBox::Cancel);
      dlg.addButton(QMessageBox::Retry);
      dlg.setDefaultButton(QMessageBox::Retry);
      dlg.setIcon(QMessageBox::Warning);
      dlg.setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);

      int ret = dlg.exec();

      if(ret == QMessageBox::Cancel){
        return;
      }
      else if(ret == QMessageBox::Retry){
        LocalBCL::instance().prodAuthKeyUserPrompt(mainWindow);
        authKey = LocalBCL::instance().prodAuthKey();
      }
      else{
        // should never get here
        OS_ASSERT(false);
      }
    }
  }

  if(!m_onlineBclDialog){
    std::string filterType = "measures";
    m_onlineBclDialog = new BuildingComponentDialog(filterType, true, mainWindow);

    bool isConnected = connect(m_onlineBclDialog, SIGNAL(rejected()),
                          this, SLOT(on_closeBclDlg()));
    OS_ASSERT(isConnected);
  }
  if(m_onlineBclDialog && !m_onlineBclDialog->isVisible()){
    m_onlineBclDialog->setGeometry(mainWindow->geometry());
    m_onlineBclDialog->show();
  }

}

void PatApp::on_closeBclDlg()
{
  if(m_onlineBclDialog->showNewComponents()){
    m_measureManager.updateMeasuresLists();
    m_onlineBclDialog->setShowNewComponents(false);
  }
}

void PatApp::openCloudDlg()
{
  if( ! (m_cloudMonitor->status() == CLOUD_STOPPED) )
  {
    QMessageBox::warning(mainWindow, 
      "Cannot Access Cloud Settings", 
      "The cloud settings are not available while the cloud is running.  Please turn off the cloud to change settings.", 
      QMessageBox::Ok);

    return;
  }

  if(!m_cloudDialog){
    m_cloudDialog = new CloudDialog();

    bool isConnected = connect(m_cloudDialog, SIGNAL(rejected()),
                               this, SLOT(on_closeBclDlg()));
    OS_ASSERT(isConnected);
  }
  if(m_cloudDialog && !m_cloudDialog->isVisible()){
    m_cloudDialog->show();
  }
}

void PatApp::on_closeCloudDlg()
{
// TODO m_cloudDialog
}

void PatApp::openMonitorUseDlg()
{
  if(!m_monitorUseDialog){
    m_monitorUseDialog = new MonitorUseDialog();

    bool isConnected = connect(m_monitorUseDialog, SIGNAL(rejected()),
                               this, SLOT(on_closeMonitorUseDlg()));
    OS_ASSERT(isConnected);
  }
  if(m_monitorUseDialog && !m_monitorUseDialog->isVisible()){
    m_monitorUseDialog->show();
  }
}

void PatApp::on_closeMonitorUseDlg()
{
// TODO m_monitorUseDialog
}

void PatApp::showHelp()
{
  QDesktopServices::openUrl(QUrl("http://openstudio.nrel.gov/parametric-analysis-tool-getting-started"));
}

void PatApp::showAbout()
{
  QMessageBox about(mainWindow);
  about.setText(PAT_ABOUTBOX);
  about.setStyleSheet("qproperty-alignment: AlignCenter;");
  about.setWindowTitle("About " + applicationName());
  about.exec();
}

openstudio::path PatApp::resourcesPath() const
{
  if (applicationIsRunningFromBuildDirectory())
  {
    return getApplicationSourceDirectory() / openstudio::toPath("src/pat_app/Resources");
  }
  else
  {
    return getApplicationRunDirectory() / openstudio::toPath("../share/pat/OSApp");
  }
}

bool PatApp::notify(QObject* receiver, QEvent* event)
{
  if (event->type() == QEvent::FileOpen) {
    return userInteractiveOpenProject(static_cast<QFileOpenEvent *>(event)->file());
  }
  return QApplication::notify(receiver, event);
}

bool PatApp::setSeed(const FileReference& currentSeedLocation) {

  std::pair<bool,std::vector<BCLMeasure> > result(false,std::vector<BCLMeasure>());

  if (m_project){

    // setting the seed model may take a long time due to version and energyplus translation
    // this progress bar will ensure process events is called to avoid locking up the app
    // we will disable the app until the processing is complete
    auto processEventsProgressBar = new ProcessEventsProgressBar();
    mainWindow->setEnabled(false);

    // get original number of variables
    int nvars = m_project->analysis().problem().numVariables();

    // check that the version is compatible
    boost::optional<VersionString> candidate = IdfFile::loadVersionOnly(currentSeedLocation.path());
    VersionString osVersion(openStudioVersion());
    if (!candidate){
      LOG(Error, "Cannot determine OpenStudio version for file at '" << toString(currentSeedLocation.path()) << "'");
      mainWindow->setEnabled(true);
      return false;
    }else if (*candidate > osVersion){
      LOG(Error, "OpenStudio version for file at '" << toString(currentSeedLocation.path()) << "' is '" << *candidate 
        << "' which is newer than current Openstudio version '" << osVersion << "'");
      mainWindow->setEnabled(true);
      return false;
    }

    // set seed model
    result = m_project->setSeed(currentSeedLocation, processEventsProgressBar);
    if (result.first){
      
      // load seed models
      m_project->seedModel(processEventsProgressBar);
      m_project->seedIdf(processEventsProgressBar);
      
      // update any duplicate measures of different versions
      for (const BCLMeasure& measure : result.second) {
        measureManager().updateMeasure(*m_project,measure);
      }

      // refresh the measures tab
      m_measuresTabController->refreshAllViews();

      // add standard report if not there (is not added on opening old projects)
      if (!m_project->getStandardReportWorkflowStep()) {
        m_project->insertStandardReportWorkflowStep();
      }

      // update whether workflow contains calibration report
      if (m_project->shouldIncludeCalibrationReports()) {
        if (!m_project->getCalibrationReportWorkflowStep()) {
          m_project->insertCalibrationReportWorkflowStep();
        }
      } else {
        if (m_project->getCalibrationReportWorkflowStep()) {
          m_project->clearCalibrationReportWorkflowStep();
        }
      }

      // DLM: TODO check imported model to see what this should do
      if (projectHasRadiance(*m_project)){
        removeRadianceFromProject(*m_project);
      }
      
      // get new number of variables and report out how many fixed measures were added
      int nvarsAdded = m_project->analysis().problem().numVariables() - nvars;
      if (nvarsAdded > 0) {
        QMessageBox::information(mainWidget(), 
                                 QString("Measures Imported"), 
                                 QString("Measures from baseline model were imported as fixed measures."));
      }

    }

    delete processEventsProgressBar;

    mainWindow->setEnabled(true);
    mainWindow->setFocus();
  }

  return result.first;
}

bool PatApp::userInteractiveOpenProject(QString path)
{
  // check if already have a project open
  if (m_project){
    // see if user wants to close the current project
    if (!userInteractiveCloseProject()){
      return false;
    }
  }
  return openFile(path);
}

bool PatApp::userInteractiveCloseProject()
{
  // check if we have a project
  if (!m_project){
    return true;
  }

  // check if we are running
  if (m_project->isRunning()){
    QMessageBox::StandardButton test = QMessageBox::question(mainWindow, "Stop Running Analysis", "Do you want to stop the currently running analysis?", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    if (test == QMessageBox::No){
      return false;
    }
    m_project->stop();
  }

  // if there are changes to be saved
  if (m_project->analysis().isDirty()){
    // see if the user wants to save
    if (!this->userInteractiveSaveProject(true)){
      return false;
    }
  }

  // blow away the current project
  mainWindow->setWindowTitle(applicationName());
  attachProject(boost::none);

  // show start screen so we are not in indeterminate state if next action fails
  showStartupView();

  return true;
}

bool PatApp::userInteractiveSaveProject(bool allowCancel)
{
  // check if we have a project
  if (!m_project){
    return false;
  }

  // if there are no changes to be saved
  if (!m_project->analysis().isDirty()){
    return true;
  }

  // check if we are running
  if (allowCancel && m_project->isRunning()){
    QMessageBox::StandardButton test = QMessageBox::question(mainWindow, "Stop Running Analysis", "Do you want to stop the currently running analysis?", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    if (test == QMessageBox::No){
      // equivalent to a cancel
      return false;
    }
  }
  m_project->stop();

  // ask if user wants to save or not
  QMessageBox::StandardButtons buttons = QMessageBox::Yes | QMessageBox::No;
  QMessageBox::StandardButton defaultButton = QMessageBox::Yes;
  if (allowCancel){
    buttons = QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel;
    defaultButton = QMessageBox::Cancel;
  }
  QMessageBox::StandardButton test = QMessageBox::question(mainWindow, "Save Project", "Do you want to save the current project?", buttons, defaultButton);

  if (test == QMessageBox::Cancel){
    return false;
  }

  // save the project
  if (test == QMessageBox::Yes){
    m_project->save(); // DLM: can this ever fail?
  }

  return true;
}

bool PatApp::userInteractiveSaveAsProject()
{
  // check if we have a project
  if (!m_project){
    return false;
  }

  // check if we are running
  if (m_project->isRunning()){
    QMessageBox::StandardButton test = QMessageBox::question(mainWindow, "Stop Running Analysis", "Do you want to stop the currently running analysis?", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    if (test == QMessageBox::No){
      return false;
    }
    m_project->stop();
  }

  auto dialog = new NewProjectDialog();

  QString projectName;
  if(dialog->exec()){
    projectName = dialog->m_projectNameLineEdit->text().simplified();
  }

  delete dialog;

  if(!projectName.length()) return false;

  QString dir = QFileDialog::getExistingDirectory( mainWindow,
                                                   tr("Choose Project Folder"),
                                                   toQString(m_project->projectDir().parent_path()));

  if(dir.length() > 0){
    // TODO backend appears unable to create project over one which already exists.  Is this desired behavior?
    // check that project does not already exist
    QDir desiredDir(dir);
    if(desiredDir.exists(projectName)){
      QMessageBox::critical(mainWindow,
        "Error Creating Project",
        QString("Project '") + projectName + QString("' already exists.\nChoose a new project name, or a new project folder."));
      return false;
    }

    QString fileName(dir);
    fileName += "/";
    fileName += projectName;
    openstudio::path projectDir = openstudio::toPath(fileName);

    if(analysisdriver::saveAs(*m_project,projectDir)){
      openFile(fileName + "/project.osp");
      return true;
    }
    else{
      QMessageBox::warning(mainWindow,
      "Error Creating Project",
      QString("Could not create project in '") + fileName + QString("'."));

      return false;
    }
  }

  return false;
}

void PatApp::showVerticalTab(int verticalId)
{
  m_mainTabId = verticalId;

  // get rid of the view before destroying all the widgets
  mainWindow->verticalTabWidget->mainViewSwitcher->clear();

  m_designAlternativesTabController.clear();
  m_runTabController.clear();
  m_measuresTabController.clear();
  m_resultsTabController.clear();

  switch( m_mainTabId )
  {
    case MEASURES:
      m_measuresTabController = QSharedPointer<MeasuresTabController>( new MeasuresTabController() );
      mainWindow->verticalTabWidget->mainViewSwitcher->setView(m_measuresTabController->measuresTabView);
      mainWindow->showRightColumn(); // TODO should the user's pref be held in a member var?

      break;
    case DESIGN_ALTERNATIVES:
      m_designAlternativesTabController = QSharedPointer<DesignAlternativesTabController>(new DesignAlternativesTabController() );
      mainWindow->verticalTabWidget->mainViewSwitcher->setView(m_designAlternativesTabController->designAlternativesTabView);
      mainWindow->hideRightColumn();
      m_mainRightColumnController->editController()->reset();

      break;
    case RUN:
      m_runTabController = QSharedPointer<RunTabController>(new RunTabController() );
      mainWindow->verticalTabWidget->mainViewSwitcher->setView(m_runTabController->runView);
      mainWindow->hideRightColumn();
      m_mainRightColumnController->editController()->reset();

      break;
    case RESULTS:
      m_resultsTabController = QSharedPointer<ResultsTabController>(new ResultsTabController() );
      mainWindow->verticalTabWidget->mainViewSwitcher->setView(m_resultsTabController->resultsView);
      mainWindow->hideRightColumn();
      m_mainRightColumnController->editController()->reset();

      break;
    default:
      break;
  }

  mainWindow->verticalTabWidget->blockSignals(true);
  mainWindow->verticalTabWidget->selectTab(m_mainTabId);
  mainWindow->verticalTabWidget->blockSignals(false);

  mainWindow->setEnabled(true);
  mainWindow->setFocus();

  updateAppState();
}

void PatApp::showStartupView()
{
  mainWindow->hideRightColumn();
  mainWindow->verticalTabWidget->selectNone();
  mainWindow->verticalTabWidget->mainViewSwitcher->setView(m_startupView);
}

void PatApp::showLoadingProjectView()
{
  mainWindow->hideRightColumn();
  mainWindow->verticalTabWidget->selectNone();
  mainWindow->verticalTabWidget->mainViewSwitcher->setView(m_loadingProjectView);
}

void PatApp::clearAllResults()
{
  if (m_project){
    runmanager::RunManager runManager = m_project->runManager();
    analysis::Analysis analysis = m_project->analysis();

    // do if no data points (performs a clean-up of any orphaned files),
    // or if there are data points and user agrees.
    bool doOperation(true);

    if (analysis.dataPoints().size() > 0) {

      int result = QMessageBox::question(mainWindow, "Clear all results?", "Do you really want to clear all results and save your project?", QMessageBox::Ok, QMessageBox::Cancel);

      if (result != QMessageBox::Ok) {
        doOperation = false;
      }

    }
    if (doOperation) {

      // stop the analysis and clears all results
      bool completeRemoval = m_project->clearAllResults();
      if (!completeRemoval) {
        QMessageBox::critical( mainWindow, 
            "Incomplete File Removal", 
            QString("Removed all results from this project, but could not remove all of the result files. Close all files and clear results again to clean up your file system.") );
      }
      m_project->save();

      // if current tab is run tab or results tab need to update
      switch( m_mainTabId )
      {
          case MEASURES:
            break;
          case DESIGN_ALTERNATIVES:
            break;
          case RUN:
            showVerticalTab(m_mainTabId);
            break;
          case RESULTS:
            showVerticalTab(m_mainTabId);
            break;
          default:
            break;
      }

    }
  }
}

void PatApp::exportXml()
{

  //make sure the project exists
  if (!m_project){
    // log
    return;
  }

  //get the project's directory
  openstudio::path projectPath = m_project->projectDir();

  //user picks the name and location of the .zip file where the report will be saved
  QString zipFilePathQS = QFileDialog::getSaveFileName( mainWindow,
                                                    "Create XML Report",
                                                    QDir::homePath(),
                                                    "(*.zip)" );

  //make sure they picked a path
  if (zipFilePathQS.isEmpty()){
    // log
    return;
  }

  openstudio::path outzip = toPath(zipFilePathQS);
  try {
    if (boost::filesystem::exists(outzip)){
      boost::filesystem::remove(outzip);
    }
  }catch(std::exception&){
    // file to overwrite exists and cannot be removed
    QMessageBox::warning(mainWindow, "Export Failed", QString("Cannot open '") + toQString(outzip) + QString("' for writing"));
    return;
  }

  //QMessageBox::information(mainWindow, "Exporting XML Report", "This may take several minutes, you will get a message when the export is complete.");
  mainWindow->setEnabled(false);
  this->processEvents();

  //make results.xml inside the project directory
  openstudio::path resultsXmlPath = projectPath / toPath("results.xml");
  openstudio::analysis::exportxml::ExportXML newXMLdoc;
  if (!newXMLdoc.exportXML(*m_project, toQString(resultsXmlPath))) {
    // user canceled, stop the export process
    mainWindow->setEnabled(true);
    return;
  }

  openstudio::path outdir = projectPath / toPath("QAQC");

  // make zip file
  try{
    boost::filesystem::create_directories(outzip.parent_path());
    openstudio::ZipFile zf(outzip, false);

    if (boost::filesystem::exists(resultsXmlPath)){
      zf.addFile(resultsXmlPath, openstudio::toPath("results.xml"));
    }else{
      // log
    }

    boost::filesystem::remove(resultsXmlPath);
    boost::filesystem::remove_all(outdir);

  }catch(std::exception&){
    // log
  }

  mainWindow->setEnabled(true);
  if (boost::filesystem::exists(outzip)){
    QMessageBox::information(mainWindow, "Export Complete", QString("Your export is available at '") + toQString(outzip) + QString("'"));
  }else{
    QMessageBox::warning(mainWindow, "Export Failed", QString("Export to '") + toQString(outzip) + QString("' failed"));
  }

}

void PatApp::exportSpreadsheet()
{
  //make sure the project exists
  if (!m_project){
    // log
    return;
  }

  if (m_project->analysis().successfulDataPoints().empty()){
    QMessageBox::warning(mainWindow, "Export Failed", QString("You must have run at least one datapoint before exporting Analysis Spreadsheet."));
    return;
  }

  //QMessageBox::information(mainWindow, "Exporting Analysis Spreadsheet", "This may take several minutes, you will get a message when the export is complete.");
  mainWindow->setEnabled(false);
  this->processEvents();

  ExportSpreadsheet exporter;
  bool result = exporter.exportSpreadsheet(*m_project);

  mainWindow->setEnabled(true);
  if (result){
    QMessageBox::information(mainWindow, "Export Complete", QString("Your export is available under '") + toQString(m_project->projectDir()) + QString("'"));
  }else{
    QMessageBox::warning(mainWindow, "Export Failed", QString("Export to '") + toQString(m_project->projectDir()) + QString("' failed"));
  }
}

void PatApp::scanForTools()
{
  openstudio::runmanager::RunManager rm;
  openstudio::runmanager::ConfigOptions co = rm.getConfigOptions();

  co.findTools(true, false, true, true);
  rm.setConfigOptions(co);
  rm.showConfigGui();

  rm.getConfigOptions().saveQSettings();
}

void PatApp::showTools()
{
  openstudio::runmanager::ConfigOptions co(true);

  openstudio::runmanager::RunManager rm;
  rm.setConfigOptions(co);
  rm.showConfigGui();

  rm.getConfigOptions().saveQSettings();
}

void PatApp::changeUserMeasuresDir()
{
  openstudio::path userMeasuresDir = BCLMeasure::userMeasuresDir();

  QString dirName = QFileDialog::getExistingDirectory( mainWindow,
                                                       tr("Select My Measures Directory"),
                                                       toQString(userMeasuresDir));

  if(dirName.length() > 0){
    userMeasuresDir = toPath(dirName);
    if (BCLMeasure::setUserMeasuresDir(userMeasuresDir)){
      emit userMeasuresDirChanged();
    }
  }
}

void PatApp::markAsModified()
{
  mainWindow->setWindowModified(true);
}

void PatApp::markAsUnmodified()
{
  mainWindow->setWindowModified(false);
}

void PatApp::queueAnalysisChanged()
{
  QTimer::singleShot(0, this, SLOT(analysisChanged()));
}

void PatApp::analysisChanged()
{
  if (m_project){
    analysis::Analysis analysis = m_project->analysis();

    if (analysis.dataPointsAreInvalid()){
      // DLM: Elaine, is there any way to just remove datapoints that are invalid
      // or do we have to remove them all?
      // ETH@20130319 - Currently, you have to remove them all. Same with results. The ability to do
      // this more judiciously would be nice.
      bool completeRemoval = m_project->removeAllDataPoints();
      if (!completeRemoval) {
        QMessageBox::critical( mainWindow, "Incomplete File Removal", QString("Removed all design alternatives from this project, but could not remove all of the associated files. Close all files and clear results to clean up your file system.") );
      }
      m_project->save();
    }else if (analysis.resultsAreInvalid()){
      bool completeRemoval = m_project->clearAllResults();
      if (!completeRemoval) {
        QMessageBox::critical( mainWindow, "Incomplete File Removal", QString("Removed all results from this project, but could not remove all of the result files. Close all files and clear results again to clean up your file system.") );
      }
      m_project->save();
    }
  }
}

void PatApp::analysisSeedChanged()
{
  // this signal can only be generated when the user is on tab 1
  if (m_project){
    
    // Changing the seed can make script arguments invalid
    m_measureManager.updateMeasures(*m_project, m_project->measures(), false);

    // DLM: this should never happen if the run tab is active
    //if (m_runTabController)
    //{
    //  m_runTabController->seedChanged();
    //
    //}
    // DLM: Elaine changing the seed doesn't also make data points invalid right?

    // ETH@20130319 - Deleting explicit call to clearAllResults. analysisChanged() slot will take
    // care of this as appropriate.

    LOG(Debug, "analysisSeedChanged() ");   
  }
}

bool PatApp::openFile(const QString& fileName)
{
  if(fileName.length() > 0)
  {
    showLoadingProjectView();
    processEvents();
    QFileInfo fileInfo(fileName);
    QDir dir = fileInfo.dir();
    QString dirAbsolutePath = dir.absolutePath();
    openstudio::path projectDir = openstudio::toPath(dirAbsolutePath);
    openstudio::analysisdriver::SimpleProjectOptions options;
    options.setPauseRunManagerQueue(true); // do not start running when opening
    options.setInitializeRunManagerUI(false);
    options.setLogLevel(Debug);
    boost::optional<openstudio::analysisdriver::SimpleProject> project = analysisdriver::openPATProject(projectDir, options);
    if(project.is_initialized()){
      OS_ASSERT(project->isPATProject());

      project::ProjectDatabase projectDatabase = project->projectDatabase();
      openstudio::VersionString projectVersion(projectDatabase.version());
      openstudio::VersionString currentOSVersion(openStudioVersion());
      if (projectVersion > currentOSVersion){
        std::stringstream ss;
        ss << "Unable to open project at '" << toString(projectDir) << "'." << std::endl;
        ss << "Project has version of '" << projectVersion << "' which is newer than current OpenStudio version '" << currentOSVersion << "'.";
        QMessageBox::warning(mainWindow,
                             "Error Opening Project",
                             QString(ss.str().c_str()));
        showStartupView();
        return false;
      }

      attachProject(project);
      mainWindow->setWindowTitle("");
      mainWindow->setWindowFilePath(dir.absolutePath());
      if (m_project->analysis().isDirty())
      {
        QTimer::singleShot(0, this, SLOT(markAsModified()));
      } else {
        QTimer::singleShot(0, this, SLOT(markAsUnmodified()));
      }

      openstudio::path osmForThisOsp = project->projectDir().parent_path() / project->projectDir().stem();
      osmForThisOsp = setFileExtension(osmForThisOsp,"osm");
      if (boost::filesystem::exists(osmForThisOsp)) {
        QMessageBox::warning(mainWindow,
                             "PAT Project Associated with an OSM",
                             QString("This project appears to be associated with the OpenStudio Application file '") + 
                             toQString(osmForThisOsp) + 
                             QString("'. For best results, 'Save as ...' this project elsewhere before continuing your work."));
      }

      // check that all Ruby scripts exist, duplicates code in OSDocument::OSDocument
      std::stringstream ss;
      for (const analysis::InputVariable& inputVariable : project->analysis().problem().variables()){
        boost::optional<analysis::MeasureGroup> measureGroup = inputVariable.optionalCast<analysis::MeasureGroup>();
        if (measureGroup){
          for (const analysis::Measure& measure : measureGroup->measures(false)){
            boost::optional<analysis::RubyMeasure> rubyMeasure = measure.optionalCast<analysis::RubyMeasure>();
            if (rubyMeasure){
              boost::optional<BCLMeasure> bclMeasure = rubyMeasure->bclMeasure();
              if (!bclMeasure){
                ss << "Cannot find measure '" << rubyMeasure->name() << "' in scripts directory." << std::endl;
              }
            }
          }
        }
      }
      if (ss.str().size() > 0){
        ss << std::endl << "Ensure that all measures are correctly located in the scripts directory.";
        LOG(Warn,ss.str());
        // DLM: which dialog should be parent?
        QMessageBox::warning(0, 
                             QString("Error opening measure and run data."),
                             toQString(ss.str()),
                             QMessageBox::Ok);
      }

      return true;
    } else {
      //if (analysisdriver::OptionalSimpleProject plainProject = analysisdriver::SimpleProject::open(projectDir,options)) {
      //  QMessageBox::warning(mainWindow,
      //                       "Error Opening Project",
      //                       QString("Project at '") + fileName + QString("' is not a PAT project."));
      //}
      //else {
        QMessageBox::warning(mainWindow,
                             "Error Opening Project",
                             QString("Unable to open project at '") + dirAbsolutePath + QString("'."));
      //}
      showStartupView();
    }
  }

  return false;
}

void PatApp::attachProject(boost::optional<analysisdriver::SimpleProject> project)
{
  m_mainRightColumnController->editController()->reset();
  m_mainRightColumnController->setCurrentId(MainRightColumnController::MEASURE_LIBRARY);

  // if we had a project
  if (m_project){
    openstudio::analysis::Analysis analysis = m_project->analysis();

    // detach all signals from analysis
    analysis.disconnect();

    // detach all signals from project
    disconnect(m_project->getImpl().get(), nullptr, this, nullptr);
  }

  // set this project as current project
  // DLM: this should be the ONLY place that m_project is set or reset
  m_project = project;

  // if we are getting a new project
  if (m_project){

    // TODO: Do not create baseline point here. Add SimpleProject::getOptionalBaselineDataPoint
    // and use that. Call this original version when the run button is hit.

    // update built in measures that may have changed if we upgraded versions
    m_measureManager.updatePatApplicationMeasures(*m_project);

    // cache the seed models here
    m_project->seedModel();
    m_project->seedIdf();

    // ensure that baseline exists before attaching signals
    analysis::DataPoint baselineDataPoint = m_project->baselineDataPoint();

    openstudio::analysis::Analysis analysis = m_project->analysis();

    // connect signals from the the analysis to this
    bool isConnected = analysis.connect(SIGNAL(changed(ChangeType)), this, SLOT(markAsModified()));
    OS_ASSERT(isConnected);

    isConnected = analysis.connect(SIGNAL(clean()), this, SLOT(markAsUnmodified()));
    OS_ASSERT(isConnected);

    // DLM: would like to make this a queued connection but ChangeType is not a QObject, burying ChangeType in impl class is kind of annoying too
    isConnected = analysis.connect(SIGNAL(changed(ChangeType)), this, SLOT(queueAnalysisChanged()));
    OS_ASSERT(isConnected);

    isConnected = analysis.connect(SIGNAL(seedChanged()), this, SLOT(analysisSeedChanged()));
    OS_ASSERT(isConnected);

    isConnected = connect(m_project->getImpl().get(),SIGNAL(analysisStatusChanged(analysisdriver::AnalysisStatus)),
                          this,SLOT(onAnalysisStatusChanged(analysisdriver::AnalysisStatus)));
    OS_ASSERT(isConnected);

    m_cloudMonitor->reconnectCloud();

    mainWindow->verticalTabWidget->enableTabs(true);

    // enable main menu functionality
    mainWindow->mainMenu->configure(true);

    // show the right tab
    if( m_cloudMonitor->status() != CLOUD_STOPPED )
    {
      showVerticalTab(PatApp::RUN);
    }
    else
    {
      showVerticalTab(PatApp::MEASURES);
    }

    updateAppState();

  }else{

    mainWindow->verticalTabWidget->enableTabs(false);

    // disable main menu functionality
     mainWindow->mainMenu->configure(false);

    // show the right tab
    showStartupView();
  }
}

MeasureManager &PatApp::measureManager()
{
  return m_measureManager;
}

const MeasureManager &PatApp::measureManager() const
{
  return m_measureManager;
}

QSharedPointer<MainRightColumnController> PatApp::mainRightColumnController() const
{
  return m_mainRightColumnController;
}

QSharedPointer<DesignAlternativesTabController> PatApp::designAlternativesTabController() const
{
  return m_designAlternativesTabController;
}

QSharedPointer<MeasuresTabController> PatApp::measuresTabController() const
{
  return m_measuresTabController;
}

QSharedPointer<ResultsTabController> PatApp::resultsTabController() const
{
  return m_resultsTabController;
}

QSharedPointer<RunTabController> PatApp::runTabController() const
{
  return m_runTabController;
}

void PatApp::addMeasure()
{
  m_measureManager.addMeasure();
}

void PatApp::duplicateSelectedMeasure()
{
  m_measureManager.duplicateSelectedMeasure();
}

void PatApp::updateMyMeasures()
{
  if (m_project)
  {
    // this can take a while
    mainWindow->setEnabled(false);
    bool doUpdate(true);
    if (!m_project->analysis().completeDataPoints().empty()) {
      int result = QMessageBox::warning(
                       mainWindow, 
                       "Update measures?", 
                       "Updating your measures may clear all results and save your project. Do you want to proceed?", 
                       QMessageBox::Ok, 
                       QMessageBox::Cancel);
      if (result != QMessageBox::Ok) {
        doUpdate = false;
      }
    }
    if (doUpdate) {
      m_measureManager.updateMyMeasures(*m_project);
    }
    mainWindow->setEnabled(true);
  } else {
    LOG(Error, "Unable to update measures, there is no project set...");
  }
}

void PatApp::updateBCLMeasures()
{
  if (m_project)
  {
    // this can take a while
    mainWindow->setEnabled(false);
    m_measureManager.updateBCLMeasures(*m_project);
    mainWindow->setEnabled(true);
  } else {
    LOG(Error, "Unable to update bcl measures, there is no project set...");
  }
}

void PatApp::downloadUpdatedBCLMeasures()
{
  m_measureManager.downloadBCLMeasures();
}

void PatApp::disableTabsDuringRun()
{
  //bool isRunning = project()->isRunning();

  //// user cannot navigate to tabs 1 or 2 if we are running
  //if(isRunning){
  //  mainWindow->verticalTabWidget->enableTab(false, PatApp::MEASURES);
  //  mainWindow->verticalTabWidget->enableTab(false, PatApp::DESIGN_ALTERNATIVES);
  //}else{
  //  mainWindow->verticalTabWidget->enableTab(true, PatApp::MEASURES);
  //  mainWindow->verticalTabWidget->enableTab(true, PatApp::DESIGN_ALTERNATIVES);
  //}
}

void PatApp::updateSelectedMeasureState()
{
  if (PatApp::instance()->mainRightColumnController()){
    if (PatApp::instance()->measureManager().isMeasureSelected()){
      PatApp::instance()->mainRightColumnController()->localLibraryController()->localLibraryView->duplicateMeasureButton->setEnabled(true);
    }else{
      PatApp::instance()->mainRightColumnController()->localLibraryController()->localLibraryView->duplicateMeasureButton->setEnabled(false);
    }
  }
}

QWidget *PatApp::mainWidget() { 
  PatMainWindow *w = mainWindow;
  return w; 
}

QSharedPointer<CloudMonitor> PatApp::cloudMonitor() const
{
  return m_cloudMonitor;
}

void PatApp::onCloudStatusChanged(const CloudStatus & newCloudStatus)
{
  if( m_project )
  {
    analysisdriver::AnalysisStatus analysisStatus = m_project->status();

    setAppState(newCloudStatus, analysisStatus);
  }
  else
  {
    setAppState(newCloudStatus,analysisdriver::AnalysisStatus::Idle);
  }
}

void PatApp::onAnalysisStatusChanged(analysisdriver::AnalysisStatus newAnalysisStatus)
{
  CloudStatus cloudStatus = m_cloudMonitor->status();

  setAppState(cloudStatus, newAnalysisStatus);
}

void PatApp::updateAppState()
{
  CloudStatus cloudStatus = m_cloudMonitor->status();

  if( m_project )
  {
    analysisdriver::AnalysisStatus analysisStatus = m_project->status();

    setAppState(cloudStatus, analysisStatus);
  }
  else
  {
    setAppState(cloudStatus,analysisdriver::AnalysisStatus::Idle);
  }
}

// Control tabs, play button, cloud button, download button, ... add to list as required
void PatApp::setAppState(const CloudStatus & cloudStatus, const analysisdriver::AnalysisStatus & analysisStatus)
{
  switch (cloudStatus)
  {
    case CLOUD_STARTING:
      switch (analysisStatus.value())
      {
        case analysisdriver::AnalysisStatus::Idle:
          break;
        case analysisdriver::AnalysisStatus::Starting:
          break;
        case analysisdriver::AnalysisStatus::Running:
          break;
        case analysisdriver::AnalysisStatus::Stopping:
          break;
        case analysisdriver::AnalysisStatus::Error:
          break;
      }
      mainWindow->verticalTabWidget->enableTab(false, PatApp::MEASURES);
      mainWindow->verticalTabWidget->enableTab(false, PatApp::DESIGN_ALTERNATIVES);
      mainWindow->verticalTabWidget->enableTab(true, PatApp::RUN);
      mainWindow->verticalTabWidget->enableTab(false, PatApp::RESULTS);
      break;
    case CLOUD_RUNNING:
      switch (analysisStatus.value())
      {
        case analysisdriver::AnalysisStatus::Idle:
          mainWindow->verticalTabWidget->enableTabs(true);
          break;
        case analysisdriver::AnalysisStatus::Starting:
          mainWindow->verticalTabWidget->enableTab(false, PatApp::MEASURES);
          mainWindow->verticalTabWidget->enableTab(false, PatApp::DESIGN_ALTERNATIVES);
          mainWindow->verticalTabWidget->enableTab(true, PatApp::RUN);
          mainWindow->verticalTabWidget->enableTab(true, PatApp::RESULTS);
          break;
        case analysisdriver::AnalysisStatus::Running:
          mainWindow->verticalTabWidget->enableTab(false, PatApp::MEASURES);
          mainWindow->verticalTabWidget->enableTab(false, PatApp::DESIGN_ALTERNATIVES);
          mainWindow->verticalTabWidget->enableTab(true, PatApp::RUN);
          mainWindow->verticalTabWidget->enableTab(true, PatApp::RESULTS);
          break;
        case analysisdriver::AnalysisStatus::Stopping:
          mainWindow->verticalTabWidget->enableTab(false, PatApp::MEASURES);
          mainWindow->verticalTabWidget->enableTab(false, PatApp::DESIGN_ALTERNATIVES);
          mainWindow->verticalTabWidget->enableTab(true, PatApp::RUN);
          mainWindow->verticalTabWidget->enableTab(true, PatApp::RESULTS);
          break;
        case analysisdriver::AnalysisStatus::Error:
          mainWindow->verticalTabWidget->enableTabs(true);
          break;
      }
      break;
    case CLOUD_STOPPING:
      switch (analysisStatus.value())
      {
        case analysisdriver::AnalysisStatus::Idle:
          break;
        case analysisdriver::AnalysisStatus::Starting:
          break;
        case analysisdriver::AnalysisStatus::Running:
          break;
        case analysisdriver::AnalysisStatus::Stopping:
          break;
        case analysisdriver::AnalysisStatus::Error:
          break;
      }
      mainWindow->verticalTabWidget->enableTab(false, PatApp::MEASURES);
      mainWindow->verticalTabWidget->enableTab(false, PatApp::DESIGN_ALTERNATIVES);
      mainWindow->verticalTabWidget->enableTab(true, PatApp::RUN);
      mainWindow->verticalTabWidget->enableTab(false, PatApp::RESULTS);
      break;
    case CLOUD_STOPPED:
      switch (analysisStatus.value())
      {
        case analysisdriver::AnalysisStatus::Idle:
          mainWindow->verticalTabWidget->enableTabs(true);
          break;
        case analysisdriver::AnalysisStatus::Starting:
          mainWindow->verticalTabWidget->enableTab(false, PatApp::MEASURES);
          mainWindow->verticalTabWidget->enableTab(false, PatApp::DESIGN_ALTERNATIVES);
          mainWindow->verticalTabWidget->enableTab(true, PatApp::RUN);
          mainWindow->verticalTabWidget->enableTab(true, PatApp::RESULTS);
          break;
        case analysisdriver::AnalysisStatus::Running:
          mainWindow->verticalTabWidget->enableTab(false, PatApp::MEASURES);
          mainWindow->verticalTabWidget->enableTab(false, PatApp::DESIGN_ALTERNATIVES);
          mainWindow->verticalTabWidget->enableTab(true, PatApp::RUN);
          mainWindow->verticalTabWidget->enableTab(true, PatApp::RESULTS);
          break;
        case analysisdriver::AnalysisStatus::Stopping:
          mainWindow->verticalTabWidget->enableTab(false, PatApp::MEASURES);
          mainWindow->verticalTabWidget->enableTab(false, PatApp::DESIGN_ALTERNATIVES);
          mainWindow->verticalTabWidget->enableTab(true, PatApp::RUN);
          mainWindow->verticalTabWidget->enableTab(true, PatApp::RESULTS);
          break;
        case analysisdriver::AnalysisStatus::Error:
          mainWindow->verticalTabWidget->enableTabs(true);
          break;
      }
      break;
    case CLOUD_ERROR:
      switch (analysisStatus.value())
      {
        case analysisdriver::AnalysisStatus::Idle:
          break;
        case analysisdriver::AnalysisStatus::Starting:
          break;
        case analysisdriver::AnalysisStatus::Running:
          break;
        case analysisdriver::AnalysisStatus::Stopping:
          break;
        case analysisdriver::AnalysisStatus::Error:
          break;
      }
      mainWindow->verticalTabWidget->selectTab(PatApp::RUN);
      mainWindow->verticalTabWidget->enableTab(false, PatApp::MEASURES);
      mainWindow->verticalTabWidget->enableTab(false, PatApp::DESIGN_ALTERNATIVES);
      mainWindow->verticalTabWidget->enableTab(true, PatApp::RUN);
      mainWindow->verticalTabWidget->enableTab(false, PatApp::RESULTS);
      break;
  }

  if( m_runTabController )
  {
    m_runTabController->runView->runStatusView->setStatus(cloudStatus, analysisStatus);

    // DLM: this should not be called every time cloud or analysis status changes
    //m_runTabController->seedChanged();
  }
}

CloudSettings PatApp::cloudSettings()
{
  // AWSSettings and VagrantSettings persist to different QSettings
  // so it is ambiguous which to load.  This is a shortcut.
  // If more cloud providers are introduced something better needs to be done.

  if( showVagrant() )
  {
    VagrantSettings settings;

    settings.loadSettings(true);

    return settings;
  }
  else
  {
    AWSSettings settings;

    settings.loadSettings(true);

    return settings;
  }
}

NewProjectDialog::NewProjectDialog(QWidget * parent)
  : OSDialog(parent)
{
  //setSizeHint(QSize(440,240));

  okButton()->setText("Continue");

  setOkButtonAsDefault(true);

  QLabel * label = nullptr;

  label = new QLabel("Enter New Project Name",this);
  label->setObjectName("H1");
  upperLayout()->addWidget(label);

  auto vertLayout = new QVBoxLayout();
  vertLayout->setContentsMargins(20,10,10,10);
  vertLayout->setSpacing(20);
  upperLayout()->addLayout(vertLayout);

  m_projectNameLineEdit = new QLineEdit();
  m_okButton->setEnabled(false);
  bool bingo = connect(m_projectNameLineEdit,SIGNAL(textChanged(const QString &)),this,SLOT(enableOkButton()));
  OS_ASSERT(bingo);

  QRegExp regExp("^([a-zA-Z0-9 ]+[-]?[_]?)*$");
  m_projectNameLineEdit->setValidator( new QRegExpValidator(regExp, this) );

  vertLayout->addWidget(m_projectNameLineEdit);

  label = new QLabel("Enter the name for your new project, this will be the name of the directory containing your project.\nIn the next step you will choose the location for your new project folder.",this);
  label->setObjectName("H3");
  vertLayout->addWidget(label);

  m_projectNameLineEdit->setFocus();

  vertLayout->addStretch();
}

void NewProjectDialog::enableOkButton()
{
  if(m_projectNameLineEdit->text().length() > 0){
    m_okButton->setEnabled(true);
  }
  else{
    m_okButton->setEnabled(false);
  }
}

//QSharedPointer<CloudProvider> CloudMonitor::cloudProvider() const
//{
//  return m_cloudProvider;
//}
//
//void CloudMonitor::setCloudProvider(QSharedPointer<CloudProvider> cloudProvider)
//{
//  m_cloudProvider = cloudProvider;
//
//  bool bingo;
//
//  bingo = connect(m_cloudProvider->getImpl<detail::CloudProvider_Impl>().get(),SIGNAL(serverStarting()),this,SIGNAL(serverStarting()));
//  OS_ASSERT(bingo);
//
//  bingo = connect(m_cloudProvider->getImpl<detail::CloudProvider_Impl>().get(),SIGNAL(serverStarted(const Url&)),
//                  this,SIGNAL(serverStarted(const Url&)));
//  OS_ASSERT(bingo);
//
//  bingo = connect(m_cloudProvider->getImpl<detail::CloudProvider_Impl>().get(),SIGNAL(workerStarting()),
//                  this,SIGNAL(workerStarting()));
//  OS_ASSERT(bingo);
//
//  bingo = connect(m_cloudProvider->getImpl<detail::CloudProvider_Impl>().get(),SIGNAL(workerStarted(const Url&)),
//                  this,SIGNAL(workerStarted(const Url&)));
//  OS_ASSERT(bingo);
//
//  bingo = connect(m_cloudProvider->getImpl<detail::CloudProvider_Impl>().get(),SIGNAL(allWorkersStarted()),
//                  this,SIGNAL(allWorkersStarted()));
//  OS_ASSERT(bingo);
//
//  bingo = connect(m_cloudProvider->getImpl<detail::CloudProvider_Impl>().get(),SIGNAL(terminating()),
//                  this,SIGNAL(terminating()));
//  OS_ASSERT(bingo);
//
//  bingo = connect(m_cloudProvider->getImpl<detail::CloudProvider_Impl>().get(),SIGNAL(terminateComplete()),
//                  this,SIGNAL(terminateComplete()));
//  OS_ASSERT(bingo);
//}

} // pat

} // openstudio
