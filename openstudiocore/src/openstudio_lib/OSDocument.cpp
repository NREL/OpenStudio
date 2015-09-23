/**********************************************************************
*  Copyright (c) 2008-2015, Alliance for Sustainable Energy.
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

#include "OSDocument.hpp"

#include "ApplyMeasureNowDialog.hpp"
#include "ConstructionsTabController.hpp"
#include "FacilityTabController.hpp"
#include "FileOperations.hpp"
#include "HorizontalTabWidget.hpp"
#include "HVACSystemsTabController.hpp"
#include "InspectorController.hpp"
#include "InspectorView.hpp"
#include "LibraryTabWidget.hpp"
#include "LoadsTabController.hpp"
#include "LocationTabController.hpp"
#include "LocationTabView.hpp"
#include "MainRightColumnController.hpp"
#include "MainWindow.hpp"
#include "ModelObjectItem.hpp"
#include "ModelObjectTypeListView.hpp"
#include "OSAppBase.hpp"
#include "ResultsTabController.hpp"
#include "ResultsTabView.hpp"
#include "RunTabController.hpp"
#include "RunTabView.hpp"
#include "SchedulesTabController.hpp"
#include "ScriptFolderListView.hpp"
#include "ScriptsTabController.hpp"
#include "ScriptsTabView.hpp"
#include "SimSettingsTabController.hpp"
#include "SimSettingsTabView.hpp"
#include "SpacesTabController.hpp"
#include "SpaceTypesTabController.hpp"
#include "SpaceTypesView.hpp"
#include "SummaryTabController.hpp"
#include "SummaryTabView.hpp"
#include "ThermalZonesTabController.hpp"
#include "VariablesTabController.hpp"
#include "VerticalTabWidget.hpp"
#include "YearSettingsWidget.hpp"

#include "../shared_gui_components/BuildingComponentDialog.hpp"
#include "../shared_gui_components/LocalLibraryController.hpp"
#include "../shared_gui_components/MeasureManager.hpp"
#include "../shared_gui_components/WaitDialog.hpp"

#include "../analysis/Analysis.hpp"

#include "../model/Building.hpp"
#include "../model/Building_Impl.hpp"
#include "../model/Component.hpp"
#include "../model/Facility.hpp"
#include "../model/Facility_Impl.hpp"
#include "../model/LifeCycleCostParameters.hpp"
#include "../model/LifeCycleCostParameters_Impl.hpp"
#include "../model/Model_Impl.hpp"
#include "../model/WeatherFile.hpp"
#include "../model/WeatherFile_Impl.hpp"

#include "../utilities/bcl/BCLComponent.hpp"
#include "../utilities/bcl/LocalBCL.hpp"
#include "../utilities/bcl/RemoteBCL.hpp"
#include "../utilities/core/Application.hpp"
#include "../utilities/core/Assert.hpp"
#include "../utilities/core/PathHelpers.hpp"
#include "../utilities/data/Attribute.hpp"
#include "../utilities/idf/IdfFile.hpp"
#include "../utilities/idf/ValidityReport.hpp"
#include "../utilities/idf/Workspace.hpp"

#include "../osversion/VersionTranslator.hpp"

#include "../analysis/DataPoint.hpp"
#include "../analysis/MeasureGroup.hpp"
#include "../analysis/MeasureGroup_Impl.hpp"
#include "../analysis/NullMeasure.hpp"
#include "../analysis/Problem.hpp"
#include "../analysis/RubyMeasure.hpp"
#include "../analysis/RubyMeasure_Impl.hpp"

#include "../runmanager/lib/WorkItem.hpp"

#include <OpenStudio.hxx>

#include "../energyplus/ForwardTranslator.hpp"
#include "../gbxml/ForwardTranslator.hpp"
#include "../sdd/ForwardTranslator.hpp"

#include <boost/filesystem/fstream.hpp>
#include <boost/filesystem.hpp>

#include <QDir>
#include <QFileDialog>
#include <QFileInfo>
#include <QFileInfoList>
#include <QMenuBar>
#include <QMessageBox>
#include <QString>
#include <QTemporaryFile>
#include <QTimer>
#include <QWidget>

#ifdef _WINDOWS
#include <windows.h>
#endif

namespace openstudio {

  OSDocument::OSDocument(openstudio::model::Model library,
    openstudio::model::Model hvacLibrary,
    const openstudio::path &resourcesPath,
    openstudio::model::OptionalModel model,
    QString filePath, bool isPlugin,
    int startTabIndex, int startSubTabIndex)
    : OSQObjectController(),
    m_compLibrary(library),
    m_hvacCompLibrary(hvacLibrary),
    m_resourcesPath(resourcesPath),
    m_onlineMeasuresBclDialog(nullptr),
    m_onlineBclDialog(nullptr),
    m_localLibraryDialog(nullptr),
    m_savePath(filePath),
    m_isPlugin(isPlugin),
    m_startTabIndex(startTabIndex),
    m_startSubTabIndex(startSubTabIndex)
  {
    m_combinedCompLibrary = model::Model(m_compLibrary.clone());
    m_combinedCompLibrary.insertObjects(m_hvacCompLibrary.objects());

    QFile data(":openstudiolib.qss");

    static QString style;
    if (style.isEmpty()){
      if (data.open(QFile::ReadOnly))
      {
        QTextStream styleIn(&data);
        style = styleIn.readAll();
        data.close();
        qobject_cast<QApplication *>(QApplication::instance())->setStyleSheet(style);
      }
    }

    m_mainWindow = new MainWindow(m_isPlugin);
    addQObject(m_mainWindow);

    if (!model){
      model = openstudio::model::Model();
    }

    openstudio::path modelTempDir = createModelTempDir();
    m_modelTempDir = toQString(modelTempDir);

    if (!m_savePath.isEmpty()){
      initializeModelTempDir(toPath(m_savePath), modelTempDir);
    }

    bool modifiedOnLoad = updateModelTempDir(*model, modelTempDir);

    if (m_savePath.isEmpty()){
      modifiedOnLoad = false;
    }

    openstudio::analysisdriver::SimpleProjectOptions options;
    options.setPauseRunManagerQueue(true); // do not start running when opening
    options.setInitializeRunManagerUI(true);
    options.setLogLevel(Debug);

    // initialize project object
    if (boost::filesystem::exists(openstudio::toPath(m_modelTempDir) / openstudio::toPath("resources/project.osp")))
    {
      LOG(Debug, "project existed, opening");
      m_simpleProject = openstudio::analysisdriver::SimpleProject::open(openstudio::toPath(m_modelTempDir) / openstudio::toPath("resources"), options);

      if (m_simpleProject)
      {
        // DLM: this does not seem very robust?
        // fix up workflow as needed
        bool save = false;
        openstudio::analysis::Problem problem = m_simpleProject->analysis().problem();
        OptionalInt index = problem.getWorkflowStepIndexByJobType(runmanager::JobType::ModelToIdf);
        if (!index) {
          problem.push(runmanager::WorkItem(runmanager::JobType::ModelToIdf));
          save = true;
        }
        index = problem.getWorkflowStepIndexByJobType(runmanager::JobType::ExpandObjects);
        if (!index) {
          problem.push(runmanager::WorkItem(runmanager::JobType::ExpandObjects));
          save = true;
        }
        index = problem.getWorkflowStepIndexByJobType(runmanager::JobType::EnergyPlusPreProcess);
        if (!index) {
          problem.push(runmanager::WorkItem(runmanager::JobType::EnergyPlusPreProcess));
          save = true;
        }
        index = problem.getWorkflowStepIndexByJobType(runmanager::JobType::EnergyPlus);
        if (!index) {
          problem.push(runmanager::WorkItem(runmanager::JobType::EnergyPlus));
          save = true;
        }
        index = problem.getWorkflowStepIndexByJobType(runmanager::JobType::OpenStudioPostProcess);
        if (!index) {
          problem.push(runmanager::WorkItem(runmanager::JobType::OpenStudioPostProcess));
          save = true;
        }

        if (save)
        {
          m_simpleProject->save();
        }

        // check that all Ruby scripts exist, duplicates code in PatApp::openFile
        std::stringstream ss;
        for (const analysis::InputVariable& inputVariable : problem.variables()){
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
          LOG(Warn, ss.str());
          // DLM: which dialog should be parent?
          QMessageBox::warning(0,
            QString("Error opening measure and run data."),
            toQString(ss.str()),
            QMessageBox::Ok);
        }

      }
      else {
        // save copy of databases about to be overwritten
        openstudio::path projectDir = openstudio::toPath(m_modelTempDir) / openstudio::toPath("resources");
        boost::filesystem::copy_file(projectDir / toPath("run.db"),
          projectDir / toPath("bad-run.db"),
          boost::filesystem::copy_option::overwrite_if_exists);
        boost::filesystem::copy_file(projectDir / toPath("project.osp"),
          projectDir / toPath("bad-project.osp"),
          boost::filesystem::copy_option::overwrite_if_exists);
        // throw up warning message
        std::stringstream ss;
        ss << "The project.osp and run.db associated with this model could not be opened. ";
        ss << "Copies have been saved as bad-run.db and bad-project.osp. New, blank databases ";
        ss << "will be created. Compared to the original, the model will no longer contain any ";
        ss << "measures or run data. If that data was present and is critical, it can be mined ";
        ss << "from the 'bad-' database copies, which are in SQLite format. If you would like ";
        ss << "to help us diagnose and fix the underlying cause of this problem, please save ";
        ss << "your model and send a zipped-up copy of the .osm file and its companion folder ";
        ss << "to OpenStudio@NREL.gov, along with a description of this model's history. Thank ";
        ss << "you, and sorry for the inconvenience.";
        LOG(Warn, ss.str());
        // DLM: which dialog should be parent?
        QMessageBox::warning(nullptr, 
                             QString("Error opening measure and run data."),
                             toQString(ss.str()),
                             QMessageBox::Ok);
      }
    }

    if (!m_simpleProject) {
      LOG(Debug, "Creating new project");
      m_simpleProject = openstudio::analysisdriver::SimpleProject::create(
        openstudio::toPath(m_modelTempDir) / openstudio::toPath("resources"),
        options,
        true);

      openstudio::analysis::Problem problem = m_simpleProject->analysis().problem();

      // add swap variable
      openstudio::analysis::MeasureGroup dvar("Alternative Model", openstudio::analysis::MeasureVector(1u, openstudio::analysis::NullMeasure()));
      problem.push(dvar);

      // set up simulation workflow
      problem.push(runmanager::WorkItem(runmanager::JobType::ModelToIdf));
      problem.push(runmanager::WorkItem(runmanager::JobType::ExpandObjects));
      problem.push(runmanager::WorkItem(runmanager::JobType::EnergyPlusPreProcess));
      problem.push(runmanager::WorkItem(runmanager::JobType::EnergyPlus));
      problem.push(runmanager::WorkItem(runmanager::JobType::OpenStudioPostProcess));
    }

  OS_ASSERT(m_simpleProject);
  
  // make sure project has baseline stuff setup
  analysis::DataPoint baselineDataPoint = m_simpleProject->baselineDataPoint();
  openstudio::analysis::Analysis analysis = m_simpleProject->analysis();

  // DLM: do we need to set seed model

  openstudio::runmanager::ConfigOptions co(true);
  m_simpleProject->runManager().setConfigOptions(co);

  bool isConnected = analysis.connect(SIGNAL(changed(ChangeType)), this, SLOT(markAsModified()));
  OS_ASSERT(isConnected);

  m_verticalId = 0;
  m_subTabIds = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
  // Make sure that the vector is the same size as the number of tabs
  OS_ASSERT(m_subTabIds.size() == static_cast<unsigned>(RESULTS_SUMMARY + 1));

  // set the model, this will create widgets
  setModel(*model, modifiedOnLoad, false);

  // connect signals to main window
  connect(m_mainWindow, &MainWindow::downloadComponentsClicked, this, &OSDocument::openBclDlg);
  connect(m_mainWindow, &MainWindow::openLibDlgClicked, this, &OSDocument::openLibDlg);
  connect(m_mainWindow, &MainWindow::closeClicked, this, &OSDocument::closeClicked);
  connect(m_mainWindow, &MainWindow::verticalTabSelected, this, &OSDocument::onVerticalTabSelected);
  connect(m_mainWindow, &MainWindow::importClicked, this, &OSDocument::importClicked);
  connect(m_mainWindow, &MainWindow::importgbXMLClicked, this, &OSDocument::importgbXMLClicked);
  connect(m_mainWindow, &MainWindow::importSDDClicked, this, &OSDocument::importSDDClicked);
  connect(m_mainWindow, &MainWindow::importIFCClicked, this, &OSDocument::importIFCClicked);
  connect(m_mainWindow, &MainWindow::loadFileClicked, this, &OSDocument::loadFileClicked);
  connect(m_mainWindow, &MainWindow::loadLibraryClicked, this, &OSDocument::loadLibraryClicked);
  connect(m_mainWindow, &MainWindow::newClicked, this, &OSDocument::newClicked);
  connect(m_mainWindow, &MainWindow::exitClicked, this, &OSDocument::exitClicked);
  connect(m_mainWindow, &MainWindow::helpClicked, this, &OSDocument::helpClicked);
  connect(m_mainWindow, &MainWindow::aboutClicked, this, &OSDocument::aboutClicked);
  connect(m_mainWindow, &MainWindow::osmDropped, this, &OSDocument::osmDropped);
  connect(m_mainWindow, &MainWindow::exportClicked, this, &OSDocument::exportIdf);
  connect(m_mainWindow, &MainWindow::exportgbXMLClicked, this, &OSDocument::exportgbXML);
  connect(m_mainWindow, &MainWindow::exportSDDClicked, this, &OSDocument::exportSDD);
  connect(m_mainWindow, &MainWindow::saveAsFileClicked, this, &OSDocument::saveAs);
  connect(m_mainWindow, &MainWindow::saveFileClicked, this, &OSDocument::save);
  // Using old-style connect here to avoid including OpenStudioApp files
  isConnected = connect(m_mainWindow, SIGNAL(revertFileClicked()), OSAppBase::instance(), SLOT(revertToSaved()));
  OS_ASSERT(isConnected);
  connect(m_mainWindow, &MainWindow::scanForToolsClicked, this, &OSDocument::scanForTools);
  connect(m_mainWindow, &MainWindow::showRunManagerPreferencesClicked, this, &OSDocument::showRunManagerPreferences);
  connect(m_mainWindow, &MainWindow::toggleUnitsClicked, this, &OSDocument::toggleUnitsClicked);
  connect(m_mainWindow, &MainWindow::applyMeasureClicked, this, &OSDocument::openMeasuresDlg);
  connect(m_mainWindow, &MainWindow::downloadMeasuresClicked, this, &OSDocument::openMeasuresBclDlg);
  connect(m_mainWindow, &MainWindow::changeMyMeasuresDir, this, &OSDocument::openChangeMeasuresDirDlg);
  connect(m_mainWindow, &MainWindow::changeBclLogin, this, &OSDocument::changeBclLogin);
  connect(this, &OSDocument::downloadComponentsClicked, this, &OSDocument::openBclDlg);
  connect(this, &OSDocument::openLibDlgClicked, this, &OSDocument::openLibDlg);

  // update window path after the dialog is shown
  QTimer::singleShot(0, this, SLOT(updateWindowFilePath())); 
  }

  //void OSDocument::showRubyConsole()
  //{
  //  if (!m_consoleWidget)
  //  {
  //    m_consoleWidget = boost::shared_ptr<OSConsoleWidget>(new OSConsoleWidget(m_model));
  //  }
  //
  //  m_consoleWidget->show();
  //}

  OSDocument::~OSDocument()
  {
    m_model.getImpl<openstudio::model::detail::Model_Impl>()->blockSignals(true);

    // release the file watchers so can remove model temp dir
    m_mainTabController.reset();

    removeDir(m_modelTempDir);
  }

  void OSDocument::showStartTabAndStartSubTab()
  {
    m_mainWindow->show();
  }

  int OSDocument::subTabIndex()
  {
    MainTabView * mainTabView = m_mainWindow->view();
    if (mainTabView){
      return mainTabView->subTabIndex();
    }
    else {
      return -1;
    }
  }

  void OSDocument::initializeModel()
  {
    // These objects used to be added to the model as you clicked through the App's tabs,
    // resulting in a uncertain set of model changes.  With these changes, every model will
    // always have the following objects.
    openstudio::model::Building building = m_model.getUniqueModelObject<openstudio::model::Building>();
    openstudio::model::Facility facility = m_model.getUniqueModelObject<openstudio::model::Facility>();
    openstudio::model::LifeCycleCostParameters lifeCycleCostParameters = m_model.getUniqueModelObject<openstudio::model::LifeCycleCostParameters>();
  }

  void OSDocument::inspectModelObject(model::OptionalModelObject & modelObject, bool readOnly)
  {
    //m_inspectorController->layoutModelObject(modelObject);

    //if( modelObject )
    //{
    //  //m_mainWindow->selectHorizontalTab(EDIT);
    //}
  }

  MainWindow * OSDocument::mainWindow()
  {
    return m_mainWindow;
  }

  boost::optional<Workspace> OSDocument::workspace()
  {
    return m_workspace;
  }

  void OSDocument::setWorkspace(const boost::optional<Workspace>& workspace)
  {
    m_workspace = workspace;
  }

  model::Model OSDocument::model()
  {
    return m_model;
  }

  void OSDocument::setModel(const model::Model& model, bool modified, bool saveCurrentTabs)
  {
    bool wasVisible = m_mainWindow->isVisible();
    m_mainWindow->setVisible(false);
    openstudio::OSAppBase * app = OSAppBase::instance();
    app->waitDialog()->setVisible(true);

    for (int i = 5; Application::instance().processEvents() && i != 0; --i) {}

    m_model = model;

    connect(m_model.getImpl<model::detail::Model_Impl>().get(), &model::detail::Model_Impl::onChange, this, &OSDocument::markAsModified);

    std::shared_ptr<OSDocument> currentDocument = app->currentDocument();
    if (currentDocument && saveCurrentTabs){
      m_startTabIndex = app->currentDocument()->verticalTabIndex();
      m_startSubTabIndex = app->currentDocument()->subTabIndex();
    }

    // Main Right Column

    m_mainRightColumnController = std::shared_ptr<MainRightColumnController>(new MainRightColumnController(m_model, m_resourcesPath));
    connect(this, &OSDocument::toggleUnitsClicked, m_mainRightColumnController.get(), &MainRightColumnController::toggleUnitsClicked);

    m_mainWindow->setMainRightColumnView(m_mainRightColumnController->mainRightColumnView());

    // Main Vertical Tabs

    if (modified){
      QTimer::singleShot(0, this, SLOT(markAsModified()));
    }
    else {
      QTimer::singleShot(0, this, SLOT(markAsUnmodified()));
    }

    // DLM: this might work to reload weather file if changed?
    this->setFullWeatherFilePath();

    if (!m_tabButtonsCreated) {
      m_tabButtonsCreated = true;
      createTabButtons();
    }
    createTab(m_verticalId);

    QTimer::singleShot(0, this, SLOT(initializeModel()));

    app->waitDialog()->setVisible(false);
    m_mainWindow->setVisible(wasVisible);

    QTimer::singleShot(0, this, SLOT(showStartTabAndStartSubTab()));
  }

  void OSDocument::createTabButtons()
  {
    // Location
    m_mainWindow->addVerticalTabButton(SITE,
      "Site",
      ":images/on_location_tab.png",
      ":images/off_location_tab.png",
      ":images/disabled_location_tab.png");

    // Schedules
    m_mainWindow->addVerticalTabButton(SCHEDULES,
      "Schedules",
      ":images/on_schedules_tab.png",
      ":images/off_schedules_tab.png",
      ":images/disabled_schedules_tab.png");

    // Constructions
    m_mainWindow->addVerticalTabButton(CONSTRUCTIONS,
      "Constructions",
      ":images/on_constructions_tab.png",
      ":images/off_constructions_tab.png",
      ":images/disabled_constructions_tab.png");

    // Loads
    m_mainWindow->addVerticalTabButton(LOADS,
      "Loads",
      ":images/on_loads_tab.png",
      ":images/off_loads_tab.png",
      ":images/disabled_loads_tab.png");

    // Space Types
    m_mainWindow->addVerticalTabButton(SPACE_TYPES,
      "Space Types",
      ":images/on_space_types_tab.png",
      ":images/off_space_types_tab.png",
      ":images/disabled_space_types_tab.png");

    // Facility
    m_mainWindow->addVerticalTabButton(FACILITY,
      "Facility",
      ":images/on_building_tab.png",
      ":images/off_building_tab.png",
      ":images/disabled_building_tab.png");

    // Spaces
    m_mainWindow->addVerticalTabButton(SPACES,
      "Spaces",
      ":images/on_spaces_tab.png",
      ":images/off_spaces_tab.png",
      ":images/disabled_spaces_tab.png");

    // Thermal Zones 
    m_mainWindow->addVerticalTabButton(THERMAL_ZONES,
      "Thermal Zones",
      ":images/on_thermal_zone_tab.png",
      ":images/off_thermal_zone_tab.png",
      ":images/disabled_thermal_zone_tab.png");

    // HVAC Systems
    m_mainWindow->addVerticalTabButton(HVAC_SYSTEMS,
      "HVAC Systems",
      ":images/on_hvac_tab.png",
      ":images/off_hvac_tab.png",
      ":images/disabled_hvac_tab.png");

    //******************************************************************************************************
    //
    //// Summary
    //
    //m_mainWindow->addVerticalTabButton( BUILDING_SUMMARY,
    //                              "Building Summary",
    //                              ":images/on_summary_tab.png",
    //                              ":images/off_summary_tab.png" );
    //
    //******************************************************************************************************

    // Variables
    m_mainWindow->addVerticalTabButton(OUTPUT_VARIABLES,
      "Output Variables",
      ":images/on_var_tab.png",
      ":images/off_var_tab.png",
      ":images/disabled_var_tab.png");

    // Sim Settings
    m_mainWindow->addVerticalTabButton(SIMULATION_SETTINGS,
      "Simulation Settings",
      ":images/on_sim_settings_tab.png",
      ":images/off_sim_settings_tab.png",
      ":images/disabled_sim_settings_tab.png");

    // Scripts
    m_mainWindow->addVerticalTabButton(RUBY_SCRIPTS,
      "Measures",
      ":images/on_scripts_tab.png",
      ":images/off_scripts_tab.png",
      ":images/disabled_scripts_tab.png");

    // Run
    m_mainWindow->addVerticalTabButton(RUN_SIMULATION,
      "Run Simulation",
      ":images/on_run_tab.png",
      ":images/off_run_tab.png",
      ":images/disabled_run_tab.png");

    // Results
    m_mainWindow->addVerticalTabButton(RESULTS_SUMMARY,
      "Results Summary",
      ":images/on_results_tab.png",
      ":images/off_results_tab.png",
      ":images/disabled_results_tab.png");
  }

  void OSDocument::createTab(int verticalId)
  {
    m_mainTabController.reset();

    m_verticalId = verticalId;

    bool isIP = m_mainWindow->displayIP();

    switch (verticalId)
    {
    case SITE:
      // Location

      m_mainTabController = std::shared_ptr<MainTabController>(new LocationTabController(isIP, m_model, m_modelTempDir));
      m_mainWindow->setView(m_mainTabController->mainContentWidget(), SITE);

      connect(m_mainTabController->mainContentWidget(), &MainTabView::tabSelected, m_mainRightColumnController.get(), &MainRightColumnController::configureForSiteSubTab);

      connect(m_mainTabController->mainContentWidget(), &MainTabView::tabSelected, this, &OSDocument::updateSubTabSelected);

      connect(this, &OSDocument::toggleUnitsClicked, m_mainTabController.get(), &LocationTabController::toggleUnitsClicked);

      break;

    case SCHEDULES:
      // Schedules

      m_mainTabController = std::shared_ptr<MainTabController>(new SchedulesTabController(isIP, m_model));
      m_mainWindow->setView(m_mainTabController->mainContentWidget(), SCHEDULES);

      connect(this, &OSDocument::toggleUnitsClicked, m_mainTabController.get(), &SchedulesTabController::toggleUnitsClicked);

      connect(m_mainTabController.get(), &SpaceTypesTabController::modelObjectSelected, m_mainRightColumnController.get(), &MainRightColumnController::inspectModelObject);

      connect(m_mainTabController.get(), &SchedulesTabController::downloadComponentsClicked, this, &OSDocument::downloadComponentsClicked);

      connect(m_mainTabController.get(), &SchedulesTabController::openLibDlgClicked, this, &OSDocument::openLibDlgClicked);

      connect(m_mainTabController->mainContentWidget(), &MainTabView::tabSelected, m_mainRightColumnController.get(), &MainRightColumnController::configureForSchedulesSubTab);

      connect(m_mainTabController->mainContentWidget(), &MainTabView::tabSelected, this, &OSDocument::updateSubTabSelected);

      break;

    case CONSTRUCTIONS:
      // Constructions

      m_mainTabController = std::shared_ptr<MainTabController>(new ConstructionsTabController(isIP, m_model));
      m_mainWindow->setView(m_mainTabController->mainContentWidget(), CONSTRUCTIONS);

      connect(this, &OSDocument::toggleUnitsClicked, m_mainTabController.get(), &ConstructionsTabController::toggleUnitsClicked);

      connect(m_mainTabController.get(), &ConstructionsTabController::downloadComponentsClicked, this, &OSDocument::downloadComponentsClicked);

      connect(m_mainTabController.get(), &ConstructionsTabController::openLibDlgClicked, this, &OSDocument::openLibDlgClicked);

      connect(m_mainTabController->mainContentWidget(), &MainTabView::tabSelected, m_mainRightColumnController.get(), &MainRightColumnController::configureForConstructionsSubTab);

      connect(m_mainTabController->mainContentWidget(), &MainTabView::tabSelected, this, &OSDocument::updateSubTabSelected);

      break;

    case LOADS:
      // Loads

      m_mainTabController = std::shared_ptr<MainTabController>(new LoadsTabController(isIP, m_model));
      m_mainWindow->setView(m_mainTabController->mainContentWidget(), LOADS);

      connect(this, &OSDocument::toggleUnitsClicked, m_mainTabController.get(), &LoadsTabController::toggleUnitsClicked);

      connect(m_mainTabController.get(), &LoadsTabController::downloadComponentsClicked, this, &OSDocument::downloadComponentsClicked);

      connect(m_mainTabController.get(), &LoadsTabController::openLibDlgClicked, this, &OSDocument::openLibDlgClicked);

      connect(m_mainTabController->mainContentWidget(), &MainTabView::tabSelected, m_mainRightColumnController.get(), &MainRightColumnController::configureForLoadsSubTab);

      connect(m_mainTabController->mainContentWidget(), &MainTabView::tabSelected, this, &OSDocument::updateSubTabSelected);

      break;

    case SPACE_TYPES:
      // Space Types

      m_mainTabController = std::shared_ptr<MainTabController>(new SpaceTypesTabController(isIP, m_model));
      m_mainWindow->setView(m_mainTabController->mainContentWidget(), SPACE_TYPES);

      connect(m_mainTabController.get(), &SpaceTypesTabController::modelObjectSelected, m_mainRightColumnController.get(), &MainRightColumnController::inspectModelObject);

      connect(m_mainTabController.get(), &SpaceTypesTabController::dropZoneItemSelected, m_mainRightColumnController.get(), &MainRightColumnController::inspectModelObjectByItem);

      connect(m_mainTabController.get(), &SpaceTypesTabController::downloadComponentsClicked, this, &OSDocument::downloadComponentsClicked);

      connect(m_mainTabController.get(), &SpaceTypesTabController::openLibDlgClicked, this, &OSDocument::openLibDlgClicked);

      connect(m_mainRightColumnController.get(), &MainRightColumnController::itemRemoveClicked, m_mainTabController.get(), &SpaceTypesTabController::itemRemoveClicked);

      connect(m_mainTabController->mainContentWidget(), &MainTabView::tabSelected, m_mainRightColumnController.get(), &MainRightColumnController::configureForSpaceTypesSubTab);

      connect(m_mainTabController->mainContentWidget(), &MainTabView::tabSelected, this, &OSDocument::updateSubTabSelected);

      break;

    case FACILITY:
      // Facility

      m_mainTabController = std::shared_ptr<MainTabController>(new FacilityTabController(isIP, m_model));
      m_mainWindow->setView(m_mainTabController->mainContentWidget(), FACILITY);

      connect(this, &OSDocument::toggleUnitsClicked, m_mainTabController.get(), &FacilityTabController::toggleUnitsClicked);

      connect(m_mainTabController.get(), &FacilityTabController::modelObjectSelected, m_mainRightColumnController.get(), &MainRightColumnController::inspectModelObject);

      connect(m_mainTabController.get(), &FacilityTabController::dropZoneItemSelected, m_mainRightColumnController.get(), &MainRightColumnController::inspectModelObjectByItem);

      connect(m_mainTabController.get(), &FacilityTabController::downloadComponentsClicked, this, &OSDocument::downloadComponentsClicked);

      connect(m_mainTabController.get(), &FacilityTabController::openLibDlgClicked, this, &OSDocument::openLibDlgClicked);

      connect(m_mainRightColumnController.get(), &MainRightColumnController::itemRemoveClicked, m_mainTabController.get(), &FacilityTabController::itemRemoveClicked);

      connect(m_mainTabController->mainContentWidget(), &MainTabView::tabSelected, m_mainRightColumnController.get(), &MainRightColumnController::configureForFacilitySubTab);

      connect(m_mainTabController->mainContentWidget(), &MainTabView::tabSelected, this, &OSDocument::updateSubTabSelected);

      break;

    case SPACES:
      // Spaces

      m_mainTabController = std::shared_ptr<MainTabController>(new SpacesTabController(isIP, m_model));
      m_mainWindow->setView(m_mainTabController->mainContentWidget(), SPACES);

      connect(this, &OSDocument::toggleUnitsClicked, m_mainTabController.get(), &SpacesTabController::toggleUnitsClicked);

      connect(m_mainTabController.get(), &SpacesTabController::modelObjectSelected, m_mainRightColumnController.get(), &MainRightColumnController::inspectModelObject);

      connect(m_mainTabController.get(), &SpacesTabController::dropZoneItemSelected, m_mainRightColumnController.get(), &MainRightColumnController::inspectModelObjectByItem);

      connect(m_mainTabController.get(), &SpacesTabController::downloadComponentsClicked, this, &OSDocument::downloadComponentsClicked);

      connect(m_mainTabController.get(), &SpacesTabController::openLibDlgClicked, this, &OSDocument::openLibDlgClicked);

      connect(m_mainRightColumnController.get(), &MainRightColumnController::itemRemoveClicked, m_mainTabController.get(), &SpacesTabController::itemRemoveClicked);

      connect(m_mainTabController->mainContentWidget(), &MainTabView::tabSelected, m_mainRightColumnController.get(), &MainRightColumnController::configureForSpacesSubTab);

      connect(m_mainTabController->mainContentWidget(), &MainTabView::tabSelected, this, &OSDocument::updateSubTabSelected);

      break;

    case THERMAL_ZONES:
      // Thermal Zones 

      m_mainTabController = std::shared_ptr<MainTabController>(new ThermalZonesTabController(isIP, m_model));
      m_mainWindow->setView(m_mainTabController->mainContentWidget(), THERMAL_ZONES);

      connect(m_mainTabController.get(), &ThermalZonesTabController::modelObjectSelected, m_mainRightColumnController.get(), &MainRightColumnController::inspectModelObject);

      connect(m_mainTabController.get(), &ThermalZonesTabController::dropZoneItemSelected, m_mainRightColumnController.get(), &MainRightColumnController::inspectModelObjectByItem);

      connect(this, &OSDocument::toggleUnitsClicked, qobject_cast<ThermalZonesTabController *>(m_mainTabController.get()), &ThermalZonesTabController::toggleUnitsClicked);

      connect(m_mainTabController->mainContentWidget(), &MainTabView::tabSelected, m_mainRightColumnController.get(), &MainRightColumnController::configureForThermalZonesSubTab);

      connect(m_mainTabController->mainContentWidget(), &MainTabView::tabSelected, this, &OSDocument::updateSubTabSelected);

      break;

    case HVAC_SYSTEMS:
      // HVAC Systems

      m_mainTabController = std::shared_ptr<MainTabController>(new HVACSystemsTabController(isIP, m_model));
      m_mainWindow->setView(m_mainTabController->mainContentWidget(), HVAC_SYSTEMS);

      connect(this, &OSDocument::toggleUnitsClicked, qobject_cast<HVACSystemsTabController *>(m_mainTabController.get()), &HVACSystemsTabController::toggleUnitsClicked);

      connect(m_mainTabController.get(), &HVACSystemsTabController::modelObjectSelected, this, &OSDocument::inspectModelObject);

      connect(m_mainTabController->mainContentWidget(), &MainTabView::tabSelected, m_mainRightColumnController.get(), &MainRightColumnController::configureForHVACSystemsSubTab);

      connect(m_mainTabController->mainContentWidget(), &MainTabView::tabSelected, this, &OSDocument::updateSubTabSelected);

      break;

    case BUILDING_SUMMARY:
      //******************************************************************************************************
      //
      //// Summary
      //
      //m_summaryTabController = std::shared_ptr<SummaryTabController>( new SummaryTabController(m_model) );
      //m_mainWindow->setView( m_summaryTabController->mainContentWidget(),BUILDING_SUMMARY );
      ////connect(m_summaryTabController->mainContentWidget(), &MainTabView::tabSelected,
      ////        m_mainRightColumnController.get(), &MainRightColumnController::configureForBuildingSummarySubTab);
      //
      //******************************************************************************************************

      break;

    case OUTPUT_VARIABLES:
      // Variables

      m_mainTabController = std::shared_ptr<MainTabController>(new VariablesTabController(m_model));
      m_mainWindow->setView(m_mainTabController->mainContentWidget(), OUTPUT_VARIABLES);

      connect(m_mainTabController->mainContentWidget(), &MainTabView::tabSelected, m_mainRightColumnController.get(), &MainRightColumnController::configureForOutputVariablesSubTab);

      connect(m_mainTabController->mainContentWidget(), &MainTabView::tabSelected, this, &OSDocument::updateSubTabSelected);

      break;

    case SIMULATION_SETTINGS:
      // Sim Settings

      m_mainTabController = std::shared_ptr<MainTabController>(new SimSettingsTabController(isIP, m_model));
      m_mainWindow->setView(m_mainTabController->mainContentWidget(), SIMULATION_SETTINGS);

      connect(this, &OSDocument::toggleUnitsClicked, m_mainTabController.get(), &SimSettingsTabController::toggleUnitsClicked);

      connect(m_mainTabController->mainContentWidget(), &MainTabView::tabSelected, m_mainRightColumnController.get(), &MainRightColumnController::configureForSimulationSettingsSubTab);

      connect(m_mainTabController->mainContentWidget(), &MainTabView::tabSelected, this, &OSDocument::updateSubTabSelected);

      break;

    case RUBY_SCRIPTS:
      // Scripts

      m_mainTabController = std::shared_ptr<MainTabController>(new ScriptsTabController());
      m_mainWindow->setView(m_mainTabController->mainContentWidget(), RUBY_SCRIPTS);

      connect(m_mainTabController->mainContentWidget(), &MainTabView::tabSelected, m_mainRightColumnController.get(), &MainRightColumnController::configureForScriptsSubTab);

      connect(m_mainTabController->mainContentWidget(), &MainTabView::tabSelected, this, &OSDocument::updateSubTabSelected);

      //connect(m_scriptsTabController->scriptFolderListView(), SIGNAL(scriptListChanged()), this, SLOT(markAsModified()));

      //isConnected = QObject::connect(m_scriptsTabController.get(), SIGNAL(downloadComponentsClicked()), this, SIGNAL(downloadComponentsClicked()));
      //OS_ASSERT(isConnected);

      //isConnected = QObject::connect(m_scriptsTabController.get(), SIGNAL(openLibDlgClicked()), this, SIGNAL(openLibDlgClicked()));
      //OS_ASSERT(isConnected);

      break;

    case RUN_SIMULATION:
      // Run

      m_mainTabController = std::shared_ptr<MainTabController>(new RunTabController(m_model, openstudio::toPath(m_savePath), openstudio::toPath(m_modelTempDir), m_simpleProject->runManager()));
      m_mainWindow->setView(m_mainTabController->mainContentWidget(), RUN_SIMULATION);

      //connect(qobject_cast<RunTabController *>(m_mainTabController.get()), &RunTabController::useRadianceStateChanged, this, &OSDocument::markAsModified);

      connect(qobject_cast<RunTabController *>(m_mainTabController.get()), &RunTabController::resultsGenerated, this, &OSDocument::runComplete);

      connect(qobject_cast<RunTabController *>(m_mainTabController.get()), &RunTabController::toolsUpdated, this, &OSDocument::markAsModified);

      connect(this, &OSDocument::toolsUpdated, this, &OSDocument::markAsModified);

      connect(this, &OSDocument::toolsUpdated, qobject_cast<RunTabController *>(m_mainTabController.get()), &RunTabController::updateToolsWarnings);

      connect(qobject_cast<RunTabController *>(m_mainTabController.get()), &RunTabController::toolsUpdated, qobject_cast<RunTabController *>(m_mainTabController.get()), &RunTabController::updateToolsWarnings);

      connect(m_mainTabController->mainContentWidget(), &MainTabView::tabSelected, m_mainRightColumnController.get(), &MainRightColumnController::configureForRunSimulationSubTab);

      connect(m_mainTabController->mainContentWidget(), &MainTabView::tabSelected, this, &OSDocument::updateSubTabSelected);

      break;

    case RESULTS_SUMMARY:
      // Results

      m_mainTabController = std::shared_ptr<MainTabController>(new ResultsTabController());
      m_mainWindow->setView(m_mainTabController->mainContentWidget(), RESULTS_SUMMARY);

      qobject_cast<ResultsTabController *>(m_mainTabController.get())->searchForExistingResults(openstudio::toPath(m_modelTempDir) / openstudio::toPath("resources") / openstudio::toPath("run"));

      connect(this, &OSDocument::toggleUnitsClicked, qobject_cast<ResultsTabController *>(m_mainTabController.get()), &ResultsTabController::onUnitSystemChange);

      connect(this, &OSDocument::treeChanged, static_cast<ResultsTabView *>(m_mainTabController->mainContentWidget()), &ResultsTabView::treeChanged);

      connect(m_mainTabController->mainContentWidget(), &MainTabView::tabSelected, m_mainRightColumnController.get(), &MainRightColumnController::configureForResultsSummarySubTab);

      connect(m_mainTabController->mainContentWidget(), &MainTabView::tabSelected, this, &OSDocument::updateSubTabSelected);

      break;

    default:
      // Should never get here
      OS_ASSERT(false);
      break;
    }
  }

  void OSDocument::markAsModified()
  {
    m_mainWindow->setWindowModified(true);

    QString fileName = this->mainWindow()->windowFilePath();

    QFile testFile(fileName);
    if (!testFile.exists()) return;

    m_mainWindow->enableRevertToSavedAction(true);
  }

  void OSDocument::markAsUnmodified()
  {
    m_mainWindow->setWindowModified(false);

    m_mainWindow->enableRevertToSavedAction(false);
  }

  void OSDocument::disableTabsDuringRun()
  {
    if (m_enableTabsAfterRun == false) {
      return; // Already in correct state, no work to be done
    }
    else {
      m_enableTabsAfterRun = false;
    }

    m_mainWindow->verticalTabWidget()->enableTabButton(SITE, m_enableTabsAfterRun);
    m_mainWindow->verticalTabWidget()->enableTabButton(SCHEDULES, m_enableTabsAfterRun);
    m_mainWindow->verticalTabWidget()->enableTabButton(CONSTRUCTIONS, m_enableTabsAfterRun);
    m_mainWindow->verticalTabWidget()->enableTabButton(LOADS, m_enableTabsAfterRun);
    m_mainWindow->verticalTabWidget()->enableTabButton(SPACE_TYPES, m_enableTabsAfterRun);
    m_mainWindow->verticalTabWidget()->enableTabButton(FACILITY, m_enableTabsAfterRun);
    m_mainWindow->verticalTabWidget()->enableTabButton(SPACES, m_enableTabsAfterRun);
    m_mainWindow->verticalTabWidget()->enableTabButton(THERMAL_ZONES, m_enableTabsAfterRun);
    m_mainWindow->verticalTabWidget()->enableTabButton(HVAC_SYSTEMS, m_enableTabsAfterRun);
    //m_mainWindow->verticalTabWidget()->enableTabButton(BUILDING_SUMMARY, m_enableTabsAfterRun); No Summary tab available
    m_mainWindow->verticalTabWidget()->enableTabButton(OUTPUT_VARIABLES, m_enableTabsAfterRun);
    m_mainWindow->verticalTabWidget()->enableTabButton(SIMULATION_SETTINGS, m_enableTabsAfterRun);
    m_mainWindow->verticalTabWidget()->enableTabButton(RUBY_SCRIPTS, m_enableTabsAfterRun);
    //m_mainWindow->verticalTabWidget()->enableTabButton(RUN_SIMULATION, m_enableTabsAfterRun); Run tab aways active
    m_mainWindow->verticalTabWidget()->enableTabButton(RESULTS_SUMMARY, m_enableTabsAfterRun);

    m_mainWindow->verticalTabWidget()->refreshTabButtons();
  }

  void OSDocument::enableTabsAfterRun()
  {

    if (m_enableTabsAfterRun == true) {
      return; // Already in correct state, no work to be done
    }
    else {
      m_enableTabsAfterRun = true;
    }

    m_mainWindow->verticalTabWidget()->enableTabButton(SITE, m_enableTabsAfterRun);
    m_mainWindow->verticalTabWidget()->enableTabButton(SCHEDULES, m_enableTabsAfterRun);
    m_mainWindow->verticalTabWidget()->enableTabButton(CONSTRUCTIONS, m_enableTabsAfterRun);
    m_mainWindow->verticalTabWidget()->enableTabButton(LOADS, m_enableTabsAfterRun);
    m_mainWindow->verticalTabWidget()->enableTabButton(SPACE_TYPES, m_enableTabsAfterRun);
    m_mainWindow->verticalTabWidget()->enableTabButton(FACILITY, m_enableTabsAfterRun);
    m_mainWindow->verticalTabWidget()->enableTabButton(SPACES, m_enableTabsAfterRun);
    m_mainWindow->verticalTabWidget()->enableTabButton(THERMAL_ZONES, m_enableTabsAfterRun);
    m_mainWindow->verticalTabWidget()->enableTabButton(HVAC_SYSTEMS, m_enableTabsAfterRun);
    //m_mainWindow->verticalTabWidget()->enableTabButton(BUILDING_SUMMARY, m_enableTabsAfterRun); No Summary tab available
    m_mainWindow->verticalTabWidget()->enableTabButton(OUTPUT_VARIABLES, m_enableTabsAfterRun);
    m_mainWindow->verticalTabWidget()->enableTabButton(SIMULATION_SETTINGS, m_enableTabsAfterRun);
    m_mainWindow->verticalTabWidget()->enableTabButton(RUBY_SCRIPTS, m_enableTabsAfterRun);
    //m_mainWindow->verticalTabWidget()->enableTabButton(RUN_SIMULATION, m_enableTabsAfterRun); Run tab aways active
    m_mainWindow->verticalTabWidget()->enableTabButton(RESULTS_SUMMARY, m_enableTabsAfterRun);

    m_mainWindow->verticalTabWidget()->refreshTabButtons();
  }

  void OSDocument::runComplete()
  {
    if (!m_savePath.isEmpty()){
      // copy all the simulation output to the save location
      // do not want to save the database or osm here
      saveModelTempDir(toPath(m_modelTempDir), toPath(m_savePath));

      // search for E+ and Radiance results in the save directory
      openstudio::path searchPath = toPath(m_savePath).parent_path() / toPath(m_savePath).stem() / openstudio::toPath("run");
      if (boost::filesystem::exists(searchPath)) {
        if (qobject_cast<ResultsTabController *>(m_mainTabController.get())){
          qobject_cast<ResultsTabController *>(m_mainTabController.get())->searchForExistingResults(searchPath);
        }
      }
    }
  }

  bool OSDocument::modified() const
  {
    return m_mainWindow->isWindowModified();
  }

  QString OSDocument::savePath() const
  {
    return m_savePath;
  }

  boost::optional<analysisdriver::SimpleProject> OSDocument::project() const
  {
    return m_simpleProject;
  }


  QString OSDocument::modelTempDir() const
  {
    return m_modelTempDir;
  }

  void OSDocument::setSavePath(const QString & filePath)
  {
    m_savePath = filePath;
    updateWindowFilePath();
  }

  bool OSDocument::setFullWeatherFilePath()
  {
    bool result = false;

    boost::optional<model::WeatherFile> weatherFile = m_model.getOptionalUniqueModelObject<model::WeatherFile>();
    if (weatherFile){
      boost::optional<openstudio::path> weatherFilePath = weatherFile->path();
      if (weatherFilePath){
        if (!weatherFilePath->is_complete()){
          if (!m_savePath.isEmpty()){
            openstudio::path osmPath = toPath(m_savePath);
            openstudio::path searchDir = osmPath.parent_path() / osmPath.stem();
            result = weatherFile->makeUrlAbsolute(searchDir);
          }
        }
      }
    }

    return result;
  }

  int OSDocument::verticalTabIndex()
  {
    return m_mainWindow->verticalTabIndex();
  }

  void OSDocument::onVerticalTabSelected(int verticalId)
  {
    openstudio::OSAppBase * app = OSAppBase::instance();
    std::shared_ptr<OSDocument> currentDocument = app->currentDocument();
    OS_ASSERT(currentDocument);

    m_mainTabId = verticalId;

    if (m_mainTabId != RUBY_SCRIPTS && m_mainRightColumnController->isMyModelTabHidden()){
      m_mainRightColumnController->hideMyModelTab(false);
    }

    createTab(m_mainTabId);

    m_subTabId = m_subTabIds.at(m_mainTabId);

    m_mainWindow->view()->selectSubTabByIndex(m_subTabId);

    switch (m_mainTabId)
    {
    case SITE:
      m_mainRightColumnController->configureForSiteSubTab(m_subTabId);
      break;
    case SCHEDULES:
      m_mainRightColumnController->configureForSchedulesSubTab(m_subTabId);
      break;
    case CONSTRUCTIONS:
      m_mainRightColumnController->configureForConstructionsSubTab(m_subTabId);
      break;
    case LOADS:
      m_mainRightColumnController->configureForLoadsSubTab(m_subTabId);
      break;
    case SPACE_TYPES:
      m_mainRightColumnController->configureForSpaceTypesSubTab(m_subTabId);
      break;
    case FACILITY:
      m_mainRightColumnController->configureForFacilitySubTab(m_subTabId);
      break;
    case SPACES:
      m_mainRightColumnController->configureForSpacesSubTab(m_subTabId);
      break;
    case THERMAL_ZONES:
      m_mainRightColumnController->configureForThermalZonesSubTab(m_subTabId);
      break;
    case HVAC_SYSTEMS:
      m_mainRightColumnController->configureForHVACSystemsSubTab(m_subTabId);
      if (qobject_cast<HVACSystemsTabController *>(m_mainTabController.get())){
        qobject_cast<HVACSystemsTabController *>(m_mainTabController.get())->clearSceneSelection();
      }
      break;
    case BUILDING_SUMMARY:
      m_mainRightColumnController->configureForBuildingSummarySubTab(m_subTabId);
      break;
    case OUTPUT_VARIABLES:
      m_mainRightColumnController->configureForOutputVariablesSubTab(m_subTabId);
      break;
    case SIMULATION_SETTINGS:
      m_mainRightColumnController->configureForSimulationSettingsSubTab(m_subTabId);
      break;
    case RUBY_SCRIPTS:
      // Do special stuff here to hide "My Model" tab
      m_mainRightColumnController->hideMyModelTab(true);
      m_mainRightColumnController->configureForScriptsSubTab(m_subTabId);
      break;
    case RUN_SIMULATION:
      m_mainRightColumnController->configureForRunSimulationSubTab(m_subTabId);
      break;
    case RESULTS_SUMMARY:
      m_mainRightColumnController->configureForResultsSummarySubTab(m_subTabId);
      break;
    default:
      break;
    }

    //m_mainWindow->selectHorizontalTab(LIBRARY);

  //boost::optional<model::ModelObject> mo;

    //m_inspectorController->layoutModelObject(mo);
  }

  void OSDocument::closeSidebar()
  {
    m_mainWindow->closeSidebar();
  }

  void OSDocument::openSidebar()
  {
    m_mainWindow->openSidebar();
  }

  void OSDocument::exportIdf()
  {

    QString defaultDir = savePath().isEmpty() ? mainWindow()->lastPath() : QFileInfo(savePath()).path();

    QString fileName = QFileDialog::getSaveFileName(this->mainWindow(),
      tr("Export Idf"),
      defaultDir,
      tr("(*.idf)"));

    if (!fileName.isEmpty())
    {
      model::Model m = this->model();
      energyplus::ForwardTranslator trans;
      Workspace workspace = trans.translateModel(m);
      openstudio::path outDir = toPath(fileName);
      boost::filesystem::ofstream ofs(outDir);
      workspace.toIdfFile().print(ofs);
      ofs.close();
    }
  }

  void OSDocument::exportgbXML()
  {
    exportFile(GBXML);
  }

  void OSDocument::exportSDD()
  {
    exportFile(SDD);
  }

  void OSDocument::exportFile(fileType type)
  {

    std::vector<LogMessage> translatorErrors, translatorWarnings;

    QString text("Export ");
    if (type == SDD){
      text.append("SDD");
    }
    else if (type == GBXML) {
      text.append("gbXML");
    }
    else {
      // should never get here
      OS_ASSERT(false);
    }

    QString defaultDir = savePath().isEmpty() ? mainWindow()->lastPath() : QFileInfo(savePath()).path();

  
  QString fileName = QFileDialog::getSaveFileName( this->mainWindow(),
                                                  tr(text.toStdString().c_str()),
                                                  defaultDir,
                                                  tr("(*.xml)") );

  if( ! fileName.isEmpty() )
    {
      model::Model m = this->model();
      openstudio::path outDir = toPath(fileName);

      if (type == SDD){
        sdd::ForwardTranslator trans;
        trans.modelToSDD(m, outDir);
        translatorErrors = trans.errors();
        translatorWarnings = trans.warnings();
      }
      else if (type == GBXML) {
        gbxml::ForwardTranslator trans;
        trans.modelToGbXML(m, outDir);
        translatorErrors = trans.errors();
        translatorWarnings = trans.warnings();
      }

      bool errorsOrWarnings = false;
      QString log;
      for (std::vector<LogMessage>::iterator it = translatorErrors.begin();
        it < translatorErrors.end();
        ++it)
      {
        errorsOrWarnings = true;

        log.append(QString::fromStdString(it->logMessage()));
        log.append("\n");
        log.append("\n");
      }

      for (std::vector<LogMessage>::iterator it = translatorWarnings.begin();
        it < translatorWarnings.end();
        ++it)
      {
        errorsOrWarnings = true;

        log.append(QString::fromStdString(it->logMessage()));
        log.append("\n");
        log.append("\n");
      }

      if (errorsOrWarnings){
        QMessageBox messageBox;
        messageBox.setText("Errors or warnings occurred on export.");
        messageBox.setDetailedText(log);
        messageBox.exec();
      }
    }
  }

  bool OSDocument::save()
  {
    bool fileSaved = false;

    // save the project file
    analysis::Analysis analysis = m_simpleProject->analysis();

    if (analysis.dataPointsAreInvalid()){
      // DLM: Elaine, is there any way to just remove datapoints that are invalid
      // or do we have to remove them all?
      // ETH@20130319 - Currently, you have to remove them all. Same with results. The ability to do
      // this more judiciously would be nice.
      bool completeRemoval = m_simpleProject->removeAllDataPoints();
      if (!completeRemoval) {
        QMessageBox::critical(mainWindow(), "Incomplete File Removal", QString("Removed all design alternatives from this project, but could not remove all of the associated files. Close all files and clear results to clean up your file system."));
      }
    }
    else if (analysis.resultsAreInvalid()){
      bool completeRemoval = m_simpleProject->clearAllResults();
      if (!completeRemoval) {
        QMessageBox::critical(mainWindow(), "Incomplete File Removal", QString("Removed all results from this project, but could not remove all of the result files. Close all files and clear results again to clean up your file system."));
      }
    }

    m_simpleProject->save();

    if (!m_savePath.isEmpty())
    {
      // saves the model to modelTempDir / in.osm
      openstudio::path modelPath = saveModel(this->model(), toPath(m_savePath), toPath(m_modelTempDir));

      this->setSavePath(toQString(modelPath));

      // saves the run database, do before saveModelTempDir
      emit modelSaving(modelPath);

      // copies modelTempDir/in.osm to modelPath, copies all resources including run database
      saveModelTempDir(toPath(m_modelTempDir), modelPath);

      this->markAsUnmodified();

      fileSaved = true;
    }
    else{
      fileSaved = saveAs();
    }

    return fileSaved;
  }

  void OSDocument::scanForTools()
  {
    openstudio::runmanager::RunManager rm;
    openstudio::runmanager::ConfigOptions co = rm.getConfigOptions();

    co.findTools(true, false, true, true);
    rm.setConfigOptions(co);
    rm.showConfigGui();

    rm.getConfigOptions().saveQSettings();

    emit toolsUpdated();
  }

  void OSDocument::showRunManagerPreferences()
  {
    openstudio::runmanager::ConfigOptions co(true);

    openstudio::runmanager::RunManager rm;
    rm.setConfigOptions(co);
    rm.showConfigGui();

    rm.getConfigOptions().saveQSettings();

    emit toolsUpdated();
  }

  bool OSDocument::saveAs()
  {
    bool fileSaved = false;

    QString defaultDir = savePath().isEmpty() ? mainWindow()->lastPath() : QFileInfo(savePath()).path();

    QString filePath = QFileDialog::getSaveFileName(this->mainWindow(),
      tr("Save"),
      defaultDir,
      tr("(*.osm)"));

    if (!filePath.isEmpty())
    {
      //scriptFolderListView()->saveOSArguments();

      // saves the model to modelTempDir / in.osm
      openstudio::path modelPath = saveModel(this->model(), toPath(filePath), toPath(m_modelTempDir));

      this->setSavePath(toQString(modelPath));

      // saves the run database, do before saveModelTempDir
      emit modelSaving(modelPath);

      // copies modelTempDir/in.osm to modelPath, copies all resources including run database
      saveModelTempDir(toPath(m_modelTempDir), modelPath);

      this->markAsUnmodified();

      fileSaved = true;
    }

    return fileSaved;

  }

  openstudio::model::Model OSDocument::componentLibrary() const
  {
    return m_compLibrary;
  }

  void OSDocument::setComponentLibrary(const openstudio::model::Model& model)
  {
    m_compLibrary = model;

    m_combinedCompLibrary = model::Model(m_compLibrary.clone());
    m_combinedCompLibrary.insertObjects(m_hvacCompLibrary.objects());

    onVerticalTabSelected(m_mainTabId);
  }

  openstudio::model::Model OSDocument::hvacComponentLibrary() const
  {
    return m_hvacCompLibrary;
  }

  void OSDocument::setHVACComponentLibrary(const openstudio::model::Model& model)
  {
    m_hvacCompLibrary = model;

    m_combinedCompLibrary = model::Model(m_compLibrary.clone());
    m_combinedCompLibrary.insertObjects(m_hvacCompLibrary.objects());

    onVerticalTabSelected(m_mainTabId);
  }

  openstudio::model::Model OSDocument::combinedComponentLibrary() const
  {
    return m_combinedCompLibrary;
  }

  bool OSDocument::fromModel(const OSItemId& itemId) const
  {
    return (itemId.sourceId() == modelToSourceId(m_model));
  }

  bool OSDocument::fromComponentLibrary(const OSItemId& itemId) const
  {
    bool fromCompLibrary = (itemId.sourceId() == modelToSourceId(m_compLibrary));
    bool fromHVACCompLibrary = (itemId.sourceId() == modelToSourceId(m_hvacCompLibrary));
    bool fromCombinedCompLibrary = (itemId.sourceId() == modelToSourceId(m_combinedCompLibrary));
    bool result = (fromCompLibrary || fromHVACCompLibrary || fromCombinedCompLibrary);
    return result;
  }

  bool OSDocument::fromBCL(const OSItemId& itemId) const
  {
    return (itemId.sourceId() == OSItemId::BCL_SOURCE_ID);
  }

  boost::optional<IddObjectType> OSDocument::getIddObjectType(const OSItemId& itemId) const
  {
    if (fromBCL(itemId)){
      boost::optional<BCLComponent> component = LocalBCL::instance().getComponent(itemId.itemId().toStdString());
      if (component){
        for (const Attribute& attribute : component->attributes()){
          if (istringEqual("OpenStudio Type", attribute.name())){
            try{
              IddObjectType iddObjectType(attribute.valueAsString());
              return iddObjectType;
            }
            catch (...){
            }
          }
        }
      }
    }

    boost::optional<model::ModelObject> modelObject = getModelObject(itemId);
    if (modelObject){
      return modelObject->iddObjectType();
    }

    return boost::none;
  }

  boost::optional<model::ModelObject> OSDocument::getModelObject(const OSItemId& itemId) const
  {
    if (fromModel(itemId)){
      Handle handle(itemId.itemId());
      return m_model.getModelObject<model::ModelObject>(handle);
    }
    else if (fromComponentLibrary(itemId)){
      if (itemId.sourceId() == modelToSourceId(m_compLibrary)){
        Handle handle(itemId.itemId());
        return m_compLibrary.getModelObject<model::ModelObject>(handle);
      }
      else if (itemId.sourceId() == modelToSourceId(m_hvacCompLibrary)){
        Handle handle(itemId.itemId());
        return m_hvacCompLibrary.getModelObject<model::ModelObject>(handle);
      }
      else if (itemId.sourceId() == modelToSourceId(m_combinedCompLibrary)){
        Handle handle(itemId.itemId());
        return m_combinedCompLibrary.getModelObject<model::ModelObject>(handle);
      }
    }

    return boost::none;
  }

  boost::optional<model::Component> OSDocument::getComponent(const OSItemId& itemId) const
  {
    boost::optional<model::Component> modelComponent;

    if (itemId.sourceId() == OSItemId::BCL_SOURCE_ID)
    {
      boost::optional<BCLComponent> component;

      component = LocalBCL::instance().getComponent(itemId.itemId().toStdString());

      if (component){

        std::vector<std::string> oscFiles = component->files("osc");

        if (oscFiles.size() == 1u){

          openstudio::path oscPath = toPath(oscFiles[0]);

#ifdef _WINDOWS

          if (oscPath.string().size() > MAX_PATH){
            if (oscPath.is_complete()){
              oscPath = toPath("\\\\?\\") / oscPath;
            }
          }

#endif

          //OS_ASSERT(boost::filesystem::exists(oscPath));

          osversion::VersionTranslator translator;
          //translator.setAllowNewerVersions(false); // DLM: allow to open newer versions?

          modelComponent = translator.loadComponent(oscPath);
        }
      }
    }

    return modelComponent;
  }

  /*
  const ScriptFolderListView* OSDocument::scriptFolderListView() const {
  return m_scriptsTabController->scriptFolderListView();
  }

  ScriptFolderListView* OSDocument::scriptFolderListView() {
  return m_scriptsTabController->scriptFolderListView();
  }
  */

  void OSDocument::toggleUnits(bool displayIP)
  {
  }

  void OSDocument::openMeasuresDlg()
  {
    // save model if dirty
    if (this->modified()){

    auto messageBox = new QMessageBox(this->mainWindow());
    messageBox->setText("You must save your model before applying a measure.");
    messageBox->setInformativeText("Do you want to save your model now?");
    messageBox->setStandardButtons(QMessageBox::Save | QMessageBox::Cancel);
    messageBox->setDefaultButton(QMessageBox::Save);
    messageBox->button(QMessageBox::Save)->setShortcut(QKeySequence(Qt::Key_S));
    messageBox->setIcon(QMessageBox::Question);

      int ret = messageBox->exec();

      delete messageBox;

      switch (ret)
      {
      case QMessageBox::Save:
        // Save was clicked
        if (this->save() != true) return;
        break;
      case QMessageBox::Cancel:
        // Cancel was clicked
        return;
      default:
        // should never be reached
        OS_ASSERT(false);
      }
    }

    // open modal dialog
    m_applyMeasureNowDialog = boost::shared_ptr<ApplyMeasureNowDialog>(new ApplyMeasureNowDialog());

    // connect signal before exec dialog
    connect(m_applyMeasureNowDialog.get(), &ApplyMeasureNowDialog::toolsUpdated, this, &OSDocument::toolsUpdated);

    m_applyMeasureNowDialog->exec();

    // DLM: kill the dialog here as there is logic in the destructor that resets application state
    // DLM: this seems overly complicated
    m_applyMeasureNowDialog.reset();
  }

  void OSDocument::openChangeMeasuresDirDlg()
  {
    openstudio::path userMeasuresDir = BCLMeasure::userMeasuresDir();

    QString dirName("");

    if (isNetworkPath(userMeasuresDir) && !isNetworkPathAvailable(userMeasuresDir)) {
      dirName = QFileDialog::getExistingDirectory(this->mainWindow(),
        tr("Select My Measures Directory"));
    }
    else {
      dirName = QFileDialog::getExistingDirectory(this->mainWindow(),
        tr("Select My Measures Directory"),
        toQString(userMeasuresDir));
    }

    userMeasuresDir = toPath(dirName);

    if (isNetworkPath(userMeasuresDir) && !isNetworkPathAvailable(userMeasuresDir)) {
      QMessageBox::information(this->mainWindow(), "Invalid Directory", "The My Measures Directory you chose appears to be on a network drive that is not currently available.\nYou can change your specified My Measures Directory using 'Preferences->Change My Measures Directory'.", QMessageBox::Ok);
      return;
    }

    if (!userMeasuresDir.empty()){
      if (BCLMeasure::setUserMeasuresDir(userMeasuresDir)){
        OSAppBase::instance()->measureManager().updateMeasuresLists();
      }
    }
  }

  void OSDocument::changeBclLogin()
  {
    QMessageBox::information(this->mainWindow(), QString("Change BCL Login Information"), QString("Not yet available.\nMiddleware testing required."));
  }

  void OSDocument::updateWindowFilePath()
  {
    if (m_savePath.isEmpty()){
      m_mainWindow->setWindowFilePath("Untitled");
    }
    else{
      m_mainWindow->setWindowFilePath(m_savePath);
    }
  }

  void OSDocument::updateSubTabSelected(int id)
  {
    m_subTabIds.at(m_verticalId) = id;
  }

  void OSDocument::openBclDlg()
  {
    std::string authKey = LocalBCL::instance().prodAuthKey();
    if (!LocalBCL::instance().setProdAuthKey(authKey)){
      LocalBCL::instance().prodAuthKeyUserPrompt(m_mainWindow);
      authKey = LocalBCL::instance().prodAuthKey();

      while (!LocalBCL::instance().setProdAuthKey(authKey)){
        QMessageBox dlg(m_mainWindow);
        dlg.setWindowTitle(tr("Online BCL"));
        dlg.setText("The BCL auth key is invalid, and Online BCL data will not be available.  To learn how to obtain a valid BCL auth key, click <a href='http://nrel.github.io/OpenStudio-user-documentation/getting_started/getting_started/'>here</a>.");
        dlg.setTextFormat(Qt::RichText);
        dlg.addButton(QMessageBox::Cancel);
        dlg.addButton(QMessageBox::Retry);
        dlg.setDefaultButton(QMessageBox::Retry);
        dlg.setIcon(QMessageBox::Warning);
        dlg.setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);

        int ret = dlg.exec();

        if (ret == QMessageBox::Cancel){
          return;
        }
        else if (ret == QMessageBox::Retry){
          LocalBCL::instance().prodAuthKeyUserPrompt(m_mainWindow);
          authKey = LocalBCL::instance().prodAuthKey();
        }
        else{
          // should never get here
          OS_ASSERT(false);
        }
      }
    }

    if (!m_onlineBclDialog){
      std::string filterType = "components";
      m_onlineBclDialog = new BuildingComponentDialog(filterType, true, m_mainWindow);

      connect(m_onlineBclDialog, &BuildingComponentDialog::rejected, this, &OSDocument::on_closeBclDlg);
    }
    if (m_onlineBclDialog && !m_onlineBclDialog->isVisible()){
      m_onlineBclDialog->setGeometry(m_mainWindow->geometry());
      m_onlineBclDialog->show();
    }
  }

  void OSDocument::on_closeBclDlg()
  {
    if (m_onlineBclDialog->showNewComponents()){
      onVerticalTabSelected(m_mainTabId);
      m_onlineBclDialog->setShowNewComponents(false);
    }
  }

  void OSDocument::openLibDlg()
  {
    if (!m_localLibraryDialog){
      std::string filterType = "components";
      m_localLibraryDialog = new BuildingComponentDialog(filterType, false, m_mainWindow);
    }
    if (m_localLibraryDialog && !m_localLibraryDialog->isVisible()){
      m_localLibraryDialog->setGeometry(m_mainWindow->geometry());
      m_localLibraryDialog->show();
    }
  }

  std::shared_ptr<MainRightColumnController> OSDocument::mainRightColumnController() const
  {
    return m_mainRightColumnController;
  }

  void OSDocument::openMeasuresBclDlg()
  {
    std::string authKey = LocalBCL::instance().prodAuthKey();
    if (!LocalBCL::instance().setProdAuthKey(authKey)){
      LocalBCL::instance().prodAuthKeyUserPrompt(m_mainWindow);
      authKey = LocalBCL::instance().prodAuthKey();

      while (!LocalBCL::instance().setProdAuthKey(authKey)){
        QMessageBox dlg(m_mainWindow);
        dlg.setWindowTitle(tr("Online BCL"));
        dlg.setText("The BCL auth key is invalid, and Online BCL data will not be available.  To learn how to obtain a valid BCL auth key, click <a href='http://nrel.github.io/OpenStudio-user-documentation/getting_started/getting_started/'>here</a>.");
        dlg.setTextFormat(Qt::RichText);
        dlg.addButton(QMessageBox::Cancel);
        dlg.addButton(QMessageBox::Retry);
        dlg.setDefaultButton(QMessageBox::Retry);
        dlg.setIcon(QMessageBox::Warning);
        dlg.setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);

        int ret = dlg.exec();

        if (ret == QMessageBox::Cancel){
          return;
        }
        else if (ret == QMessageBox::Retry){
          LocalBCL::instance().prodAuthKeyUserPrompt(m_mainWindow);
          authKey = LocalBCL::instance().prodAuthKey();
        }
        else{
          // should never get here
          OS_ASSERT(false);
        }
      }
    }

    if (!m_onlineMeasuresBclDialog){
      std::string filterType = "measures";
      m_onlineMeasuresBclDialog = new BuildingComponentDialog(filterType, true, m_mainWindow);

      connect(m_onlineMeasuresBclDialog, &BuildingComponentDialog::rejected, this, &OSDocument::on_closeMeasuresBclDlg);
    }
    if (m_onlineMeasuresBclDialog && !m_onlineMeasuresBclDialog->isVisible()){
      m_onlineMeasuresBclDialog->setGeometry(m_mainWindow->geometry());
      m_onlineMeasuresBclDialog->show();
    }

  }

  void OSDocument::on_closeMeasuresBclDlg()
  {
    if (m_onlineMeasuresBclDialog->showNewComponents()){
      OSAppBase::instance()->measureManager().updateMeasuresLists();
      m_onlineMeasuresBclDialog->setShowNewComponents(false);
    }
  }

} // openstudio
