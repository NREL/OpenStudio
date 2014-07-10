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

#include "OSDocument.hpp"
#include "ApplyMeasureNowDialog.hpp"
#include "BuildingStoriesTabController.hpp"
#include "ConstructionsTabController.hpp"
#include "FacilityTabController.hpp"
#include "FacilityView.hpp"
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
#include "SpaceTypesTabController.hpp"
#include "SpaceTypesView.hpp"
#include "SummaryTabController.hpp"
#include "SummaryTabView.hpp"
#include "ThermalZonesTabController.hpp"
#include "VariablesTabController.hpp"
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

OSDocument::OSDocument( openstudio::model::Model library,
                        openstudio::model::Model hvacLibrary,
                        const openstudio::path &resourcesPath,
                        openstudio::model::OptionalModel model,
                        QString filePath, bool isPlugin, 
                        int startTabIndex, int startSubTabIndex)
  : OSQObjectController(),
    m_compLibrary(library),
    m_hvacCompLibrary(hvacLibrary),
    m_resourcesPath(resourcesPath),
    m_onlineMeasuresBclDialog(NULL),
    m_onlineBclDialog(NULL),
    m_localLibraryDialog(NULL),
    m_savePath(filePath),
    m_isPlugin(isPlugin),
    m_startTabIndex(startTabIndex),
    m_startSubTabIndex(startSubTabIndex)
{
  bool isConnected = false;

  m_combinedCompLibrary = model::Model(m_compLibrary.clone());
  m_combinedCompLibrary.insertObjects(m_hvacCompLibrary.objects());

  QFile data(":openstudiolib.qss");

  static QString style;
  if (style.isEmpty()){
    if(data.open(QFile::ReadOnly))
    {
      QTextStream styleIn(&data);
      style = styleIn.readAll();
      data.close();
      qobject_cast<QApplication *>(QApplication::instance())->setStyleSheet(style);
    }
  }

  m_mainWindow = new MainWindow(m_isPlugin);
  addQObject(m_mainWindow);

  if( !model ){
    model = openstudio::model::Model();
  }

  openstudio::path modelTempDir = createModelTempDir();
  m_modelTempDir = toQString(modelTempDir);

  if( !m_savePath.isEmpty() ){
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
        LOG(Warn,ss.str());
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
      LOG(Warn,ss.str());
      // DLM: which dialog should be parent?
      QMessageBox::warning(0, 
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
    openstudio::analysis::MeasureGroup dvar("Alternative Model",openstudio::analysis::MeasureVector(1u,openstudio::analysis::NullMeasure()));
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

  isConnected = analysis.connect(SIGNAL(changed(ChangeType)), this, SLOT(markAsModified()));
  OS_ASSERT(isConnected);

  // set the model, this will create widgets
  setModel(*model, modifiedOnLoad, false);

  // connect signals to main window
  isConnected = connect(m_mainWindow, SIGNAL(downloadComponentsClicked()), this, SLOT(openBclDlg()));
  OS_ASSERT(isConnected);
  isConnected = connect(m_mainWindow, SIGNAL(openLibDlgClicked()), this, SLOT(openLibDlg()));
  OS_ASSERT(isConnected);

  isConnected = connect( m_mainWindow, SIGNAL(closeClicked()), this, SIGNAL(closeClicked()) );
  OS_ASSERT(isConnected);

  isConnected = connect(m_mainWindow,SIGNAL(verticalTabSelected(int)),this,SLOT(onVerticalTabSelected(int)));
  OS_ASSERT(isConnected);

  isConnected = connect(m_mainWindow, SIGNAL(importClicked()), this, SIGNAL(importClicked()));
  OS_ASSERT(isConnected);
  isConnected = connect(m_mainWindow, SIGNAL(importgbXMLClicked()), this, SIGNAL(importgbXMLClicked()));
  OS_ASSERT(isConnected);
  isConnected = connect(m_mainWindow, SIGNAL(importSDDClicked()), this, SIGNAL(importSDDClicked()));
  OS_ASSERT(isConnected);
  isConnected = connect(m_mainWindow, SIGNAL(loadFileClicked()), this, SIGNAL(loadFileClicked()));
  OS_ASSERT(isConnected);
  isConnected = connect(m_mainWindow, SIGNAL(loadLibraryClicked()), this, SIGNAL(loadLibraryClicked()));
  OS_ASSERT(isConnected);
  isConnected = connect(m_mainWindow, SIGNAL(newClicked()), this, SIGNAL(newClicked()));
  OS_ASSERT(isConnected);
  isConnected = connect(m_mainWindow, SIGNAL(exitClicked()),this,SIGNAL(exitClicked()));
  OS_ASSERT(isConnected);
  isConnected = connect(m_mainWindow, SIGNAL(helpClicked()),this,SIGNAL(helpClicked()));
  OS_ASSERT(isConnected);
  isConnected = connect(m_mainWindow, SIGNAL(aboutClicked()),this,SIGNAL(aboutClicked()));
  OS_ASSERT(isConnected);
  isConnected = connect(m_mainWindow, SIGNAL(osmDropped(QString)),this,SIGNAL(osmDropped(QString)));
  OS_ASSERT(isConnected);
  isConnected = connect(m_mainWindow, SIGNAL(exportClicked()), this, SLOT(exportIdf()));
  OS_ASSERT(isConnected);
  isConnected = connect(m_mainWindow, SIGNAL(exportgbXMLClicked()), this, SLOT(exportgbXML()));
  OS_ASSERT(isConnected);
  isConnected = connect(m_mainWindow, SIGNAL(exportSDDClicked()), this, SLOT(exportSDD()));
  OS_ASSERT(isConnected);
  isConnected = connect(m_mainWindow, SIGNAL(saveAsFileClicked()), this, SLOT(saveAs()));
  OS_ASSERT(isConnected);
  isConnected = connect(m_mainWindow, SIGNAL(saveFileClicked()), this, SLOT(save()));
  OS_ASSERT(isConnected);
  isConnected = connect(m_mainWindow, SIGNAL(revertFileClicked()), OSAppBase::instance(), SLOT(revertToSaved()));
  OS_ASSERT(isConnected);
  isConnected = connect(m_mainWindow, SIGNAL(scanForToolsClicked()), this, SLOT(scanForTools()));
  OS_ASSERT(isConnected);
  isConnected = connect(m_mainWindow, SIGNAL(showRunManagerPreferencesClicked()), this, SLOT(showRunManagerPreferences()));
  OS_ASSERT(isConnected);
  isConnected = connect(m_mainWindow, SIGNAL(toggleUnitsClicked(bool)), this, SIGNAL(toggleUnitsClicked(bool)));
  OS_ASSERT(isConnected);
  isConnected = connect(m_mainWindow, SIGNAL(applyMeasureClicked()), this, SLOT(openMeasuresDlg()));
  OS_ASSERT(isConnected);
  isConnected = connect(m_mainWindow, SIGNAL(downloadMeasuresClicked()), this, SLOT(openMeasuresBclDlg()));
  OS_ASSERT(isConnected);
  isConnected = connect(m_mainWindow, SIGNAL(changeMyMeasuresDir()), this, SLOT(openChangeMeasuresDirDlg()));
  OS_ASSERT(isConnected);
  isConnected = connect(m_mainWindow, SIGNAL(changeBclLogin()), this, SLOT(changeBclLogin()));
  OS_ASSERT(isConnected);
  isConnected = QObject::connect(this, SIGNAL(downloadComponentsClicked()), this, SLOT(openBclDlg()));
  OS_ASSERT(isConnected);
  isConnected = QObject::connect(this, SIGNAL(openLibDlgClicked()), this, SLOT(openLibDlg()));
  OS_ASSERT(isConnected);

  // call to setModel will register this
  //QTimer::singleShot(0, this, SLOT(showStartTabAndStartSubTab())); 

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
  m_scriptsTabController.reset();

  removeDir(m_modelTempDir);
}
  
//void OSDocument::showFirstTab()
//{
//  m_mainWindow->selectVerticalTab(SITE);
//
//  m_mainWindow->show();
//}

void OSDocument::showStartTabAndStartSubTab()
{
  m_mainWindow->selectVerticalTabByIndex(m_startTabIndex);

  MainTabView* mainTabView = m_mainWindow->verticalTabByIndex(m_startTabIndex);
  if (mainTabView){
    mainTabView->selectSubTabByIndex(m_startSubTabIndex);
  }

  m_mainWindow->show();
}

//void OSDocument::showTab(int tabIndex)
//{
//  m_mainWindow->selectVerticalTabByIndex(tabIndex);
//}

int OSDocument::subTabIndex()
{
  int tabIndex = verticalTabIndex();
  MainTabView* mainTabView = m_mainWindow->verticalTabByIndex(tabIndex);
  if(mainTabView){
    return mainTabView->subTabIndex();
  } else {
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

model::Model OSDocument::model()
{
  return m_model;
}

void OSDocument::setModel(const model::Model& model, bool modified, bool saveCurrentTabs)
{
  bool isConnected = false;

  bool wasVisible = m_mainWindow->isVisible();
  m_mainWindow->setVisible(false);
  openstudio::OSAppBase * app = OSAppBase::instance();
  app->waitDialog()->setVisible(true);
  app->processEvents();

  bool isIP = m_mainWindow->displayIP();

  m_model = model;

  std::shared_ptr<OSDocument> currentDocument = app->currentDocument();
  if(currentDocument && saveCurrentTabs){
    m_startTabIndex = app->currentDocument()->verticalTabIndex();
    m_startSubTabIndex = app->currentDocument()->subTabIndex();
  }

  // Main Right Column

  m_mainRightColumnController = std::shared_ptr<MainRightColumnController>(new MainRightColumnController(m_model, m_resourcesPath));
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)),
                        m_mainRightColumnController.get(), SIGNAL(toggleUnitsClicked(bool)));
  OS_ASSERT(isConnected);

  m_mainWindow->setMainRightColumnView(m_mainRightColumnController->mainRightColumnView());

  // Main Vertical Tabs

  m_mainWindow->deleteAllVerticalTabs();

  // Location

  m_locationTabController = std::shared_ptr<LocationTabController>( new LocationTabController(m_model, m_modelTempDir) );
  m_mainWindow->addVerticalTab( m_locationTabController->mainContentWidget(),
                                SITE,
                                "Site",
                                ":images/on_location_tab.png",
                                ":images/off_location_tab.png" );
  connect(m_locationTabController->mainContentWidget(),SIGNAL(tabSelected(int)),
          m_mainRightColumnController.get(),SLOT(configureForSiteSubTab(int)));

  // Schedules

  m_schedulesTabController = std::shared_ptr<SchedulesTabController>( new SchedulesTabController(isIP, m_model) );
  m_mainWindow->addVerticalTab( m_schedulesTabController->mainContentWidget(),
                                SCHEDULES,
                                "Schedules",
                                ":images/on_schedules_tab.png",
                                ":images/off_schedules_tab.png" );
  connect(m_schedulesTabController->mainContentWidget(),SIGNAL(tabSelected(int)),
          m_mainRightColumnController.get(),SLOT(configureForSchedulesSubTab(int)));

  isConnected = connect(this,SIGNAL(toggleUnitsClicked(bool)),
                        m_schedulesTabController.get(),SIGNAL(toggleUnitsClicked(bool)));
  OS_ASSERT(isConnected);

  isConnected = QObject::connect(m_schedulesTabController.get(), SIGNAL(downloadComponentsClicked()),
                                 this, SIGNAL(downloadComponentsClicked()));
  OS_ASSERT(isConnected);

  isConnected = QObject::connect(m_schedulesTabController.get(), SIGNAL(openLibDlgClicked()),
                                 this, SIGNAL(openLibDlgClicked()));
  OS_ASSERT(isConnected);

  // Constructions

  m_constructionsTabController = std::shared_ptr<ConstructionsTabController>( new ConstructionsTabController(isIP, m_model) );
  m_mainWindow->addVerticalTab( m_constructionsTabController->mainContentWidget(),
                                CONSTRUCTIONS,
                                "Constructions",
                                ":images/on_constructions_tab.png",
                                ":images/off_constructions_tab.png" );
  connect(m_constructionsTabController->mainContentWidget(),SIGNAL(tabSelected(int)),
          m_mainRightColumnController.get(),SLOT(configureForConstructionsSubTab(int)));

  isConnected = connect(this,SIGNAL(toggleUnitsClicked(bool)),
                        m_constructionsTabController.get(),SIGNAL(toggleUnitsClicked(bool)));
  OS_ASSERT(isConnected);

  isConnected = QObject::connect(m_constructionsTabController.get(), SIGNAL(downloadComponentsClicked()),
                                 this, SIGNAL(downloadComponentsClicked()));
  OS_ASSERT(isConnected);

  isConnected = QObject::connect(m_constructionsTabController.get(), SIGNAL(openLibDlgClicked()),
                                 this, SIGNAL(openLibDlgClicked()));
  OS_ASSERT(isConnected);

  // Loads

  m_loadsTabController = std::shared_ptr<LoadsTabController>( new LoadsTabController(isIP, m_model) );
  m_mainWindow->addVerticalTab( m_loadsTabController->mainContentWidget(),
                                LOADS,
                                "Loads",
                                ":images/on_loads_tab.png",
                                ":images/off_loads_tab.png" );
  connect(m_loadsTabController->mainContentWidget(),SIGNAL(tabSelected(int)),
          m_mainRightColumnController.get(),SLOT(configureForLoadsSubTab(int)));

  isConnected = connect(this,SIGNAL(toggleUnitsClicked(bool)),
                        m_loadsTabController.get(),SIGNAL(toggleUnitsClicked(bool)));
  OS_ASSERT(isConnected);

  isConnected = QObject::connect(m_loadsTabController.get(), SIGNAL(downloadComponentsClicked()),
                                 this, SIGNAL(downloadComponentsClicked()));
  OS_ASSERT(isConnected);

  isConnected = QObject::connect(m_loadsTabController.get(), SIGNAL(openLibDlgClicked()),
                                 this, SIGNAL(openLibDlgClicked()));
  OS_ASSERT(isConnected);

  // Space Types

  m_spaceTypesTabController = std::shared_ptr<SpaceTypesTabController>( new SpaceTypesTabController(m_model) );
  m_mainWindow->addVerticalTab( m_spaceTypesTabController->mainContentWidget(),
                                SPACE_TYPES,
                                "Space Types",
                                ":images/on_space_types_tab.png",
                                ":images/off_space_types_tab.png" );
  connect(m_spaceTypesTabController->mainContentWidget(),SIGNAL(tabSelected(int)),
          m_mainRightColumnController.get(),SLOT(configureForSpaceTypesSubTab(int)));

  connect( m_spaceTypesTabController.get(),
           SIGNAL(modelObjectSelected(model::OptionalModelObject &, bool )),
           m_mainRightColumnController.get(),
           SLOT(inspectModelObject( model::OptionalModelObject &, bool )) );

  isConnected = QObject::connect(m_spaceTypesTabController.get(), SIGNAL(downloadComponentsClicked()),
                                 this, SIGNAL(downloadComponentsClicked()));
  OS_ASSERT(isConnected);

  isConnected = QObject::connect(m_spaceTypesTabController.get(), SIGNAL(openLibDlgClicked()),
                                 this, SIGNAL(openLibDlgClicked()));
  OS_ASSERT(isConnected);

  // Building Stories

  m_buildingStoriesTabController = std::shared_ptr<BuildingStoriesTabController>( new BuildingStoriesTabController(m_model) );
  m_mainWindow->addVerticalTab( m_buildingStoriesTabController->mainContentWidget(),
                                BUILDING_STORIES,
                                "Building Stories",
                                ":images/on_building_stories_tab.png",
                                ":images/off_building_stories_tab.png" );
  connect(m_buildingStoriesTabController->mainContentWidget(),SIGNAL(tabSelected(int)),
          m_mainRightColumnController.get(),SLOT(configureForBuildingSummarySubTab(int)));
  
  isConnected = QObject::connect(m_buildingStoriesTabController.get(), SIGNAL(downloadComponentsClicked()),
                                 this, SIGNAL(downloadComponentsClicked()));
  OS_ASSERT(isConnected);

  isConnected = QObject::connect(m_buildingStoriesTabController.get(), SIGNAL(openLibDlgClicked()),
                                 this, SIGNAL(openLibDlgClicked()));
  OS_ASSERT(isConnected);

  // Facility

  m_facilityTabController = std::shared_ptr<FacilityTabController>( new FacilityTabController(isIP, m_model) );
  m_mainWindow->addVerticalTab( m_facilityTabController->mainContentWidget(),
                                FACILITY,
                                "Facility",
                                ":images/on_building_tab.png",
                                ":images/off_building_tab.png" );
  connect(m_facilityTabController->mainContentWidget(),SIGNAL(tabSelected(int)),
          m_mainRightColumnController.get(),SLOT(configureForFacilitySubTab(int)));

  isConnected = connect(this,SIGNAL(toggleUnitsClicked(bool)),
                        m_facilityTabController.get(),SIGNAL(toggleUnitsClicked(bool)));
  OS_ASSERT(isConnected);

  connect( m_facilityTabController.get(),
           SIGNAL(modelObjectSelected(model::OptionalModelObject &, bool )),
           m_mainRightColumnController.get(),
           SLOT(inspectModelObject( model::OptionalModelObject &, bool )) );

  isConnected = QObject::connect(m_facilityTabController.get(), SIGNAL(downloadComponentsClicked()),
                                 this, SIGNAL(downloadComponentsClicked()));
  OS_ASSERT(isConnected);

  isConnected = QObject::connect(m_facilityTabController.get(), SIGNAL(openLibDlgClicked()),
                                 this, SIGNAL(openLibDlgClicked()));
  OS_ASSERT(isConnected);

  // Thermal Zones 

  m_thermalZonesTabController = std::shared_ptr<ThermalZonesTabController>( new ThermalZonesTabController(isIP, m_model) );
  m_mainWindow->addVerticalTab( m_thermalZonesTabController->mainContentWidget(),
                                THERMAL_ZONES,
                                "Thermal Zones",
                                ":images/on_thermal_zone_tab.png",
                                ":images/off_thermal_zone_tab.png" );
  connect(m_thermalZonesTabController->mainContentWidget(),SIGNAL(tabSelected(int)),
          m_mainRightColumnController.get(),SLOT(configureForThermalZonesSubTab(int)));

  connect( m_thermalZonesTabController.get(),
           SIGNAL(modelObjectSelected(model::OptionalModelObject &, bool )),
           m_mainRightColumnController.get(),
           SLOT(inspectModelObject( model::OptionalModelObject &, bool )) );

  isConnected = connect(this,SIGNAL(toggleUnitsClicked(bool)),
                        m_thermalZonesTabController.get(),SIGNAL(toggleUnitsClicked(bool)));
  OS_ASSERT(isConnected);

  // HVAC Systems

  m_hvacSystemsTabController = std::shared_ptr<HVACSystemsTabController>( new HVACSystemsTabController(isIP, m_model) );
  m_mainWindow->addVerticalTab( m_hvacSystemsTabController->mainContentWidget(),
                                HVAC_SYSTEMS,
                                "HVAC Systems",
                                ":images/on_hvac_tab.png",
                                ":images/off_hvac_tab.png" );
  connect(m_hvacSystemsTabController->mainContentWidget(),SIGNAL(tabSelected(int)),
          m_mainRightColumnController.get(),SLOT(configureForHVACSystemsSubTab(int)));

  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)),
                        m_hvacSystemsTabController.get(), SIGNAL(toggleUnitsClicked(bool)));
  OS_ASSERT(isConnected);

  //******************************************************************************************************
  //
  ///! TODO This tab has been deprecated until building summary information is available
  //
  //// Summary
  //
  //m_summaryTabController = std::shared_ptr<SummaryTabController>( new SummaryTabController(m_model) );
  //m_mainWindow->addVerticalTab( m_summaryTabController->mainContentWidget(),
  //                              BUILDING_SUMMARY,
  //                              "Building Summary",
  //                              ":images/on_summary_tab.png",
  //                              ":images/off_summary_tab.png" );
  //connect(m_summaryTabController->mainContentWidget(),SIGNAL(tabSelected(int)),
  //        m_mainRightColumnController.get(),SLOT(configureForBuildingSummarySubTab(int)));
  //
  //******************************************************************************************************

  // Variables

  m_variablesTabController = std::shared_ptr<VariablesTabController>( new VariablesTabController(m_model) );
  m_mainWindow->addVerticalTab( m_variablesTabController->mainContentWidget(),
                                OUTPUT_VARIABLES,
                                "Output Variables",
                                ":images/on_var_tab.png",
                                ":images/off_var_tab.png" );
  connect(m_variablesTabController->mainContentWidget(),SIGNAL(tabSelected(int)),
          m_mainRightColumnController.get(),SLOT(configureForOutputVariablesSubTab(int)));

  // Sim Settings

  m_simSettingsTabController = std::shared_ptr<SimSettingsTabController>( new SimSettingsTabController(isIP, m_model) );
  m_mainWindow->addVerticalTab( m_simSettingsTabController->mainContentWidget(),
                                SIMULATION_SETTINGS,
                                "Simulation Settings",
                                ":images/on_sim_settings_tab.png",
                                ":images/off_sim_settings_tab.png" );
  connect(m_simSettingsTabController->mainContentWidget(),SIGNAL(tabSelected(int)),
          m_mainRightColumnController.get(),SLOT(configureForSimulationSettingsSubTab(int)));

  isConnected = connect(this,SIGNAL(toggleUnitsClicked(bool)),
                        m_simSettingsTabController.get(),SIGNAL(toggleUnitsClicked(bool)));
  OS_ASSERT(isConnected);

  // Scripts

  m_scriptsTabController = std::shared_ptr<ScriptsTabController>( new ScriptsTabController() );
  m_mainWindow->addVerticalTab( m_scriptsTabController->mainContentWidget(),
                                RUBY_SCRIPTS,
                                "Measures",
                                ":images/on_scripts_tab.png",
                                ":images/off_scripts_tab.png" );
  connect(m_scriptsTabController->mainContentWidget(),SIGNAL(tabSelected(int)),
          m_mainRightColumnController.get(),SLOT(configureForScriptsSubTab(int)));

  //connect(m_scriptsTabController->scriptFolderListView(), SIGNAL(scriptListChanged()),
  //    this, SLOT(markAsModified()));

  //isConnected = QObject::connect(m_scriptsTabController.get(), SIGNAL(downloadComponentsClicked()),
  //                               this, SIGNAL(downloadComponentsClicked()));
  //OS_ASSERT(isConnected);

  //isConnected = QObject::connect(m_scriptsTabController.get(), SIGNAL(openLibDlgClicked()),
  //                               this, SIGNAL(openLibDlgClicked()));
  //OS_ASSERT(isConnected);

  // Run

  m_runTabController = std::shared_ptr<RunTabController>( new RunTabController(m_model, 
        openstudio::toPath(m_savePath), openstudio::toPath(m_modelTempDir), m_simpleProject->runManager()));
        
  m_mainWindow->addVerticalTab( m_runTabController->mainContentWidget(),
                                RUN_SIMULATION,
                                "Run Simulation",
                                ":images/on_run_tab.png",
                                ":images/off_run_tab.png" );
  connect(m_runTabController->mainContentWidget(),SIGNAL(tabSelected(int)),
          m_mainRightColumnController.get(),SLOT(configureForRunSimulationSubTab(int)));
  // DLM: this signal is not connected, is it needed?
  connect(this, SIGNAL(modelSaving(const openstudio::path &)), 
          m_runTabController.get(), SLOT(saveDatabase(const openstudio::path &)));
  connect(m_runTabController.get(), SIGNAL(useRadianceStateChanged(bool)),
          this, SLOT(markAsModified()));

  connect(m_runTabController.get(), SIGNAL(resultsGenerated(const openstudio::path &, const openstudio::path &)), 
      m_resultsTabController.get(), SLOT(resultsGenerated(const openstudio::path &, const openstudio::path &)));

  connect(m_runTabController.get(), SIGNAL(resultsGenerated(const openstudio::path &, const openstudio::path &)),
      this, SLOT(runComplete())); 
  
  // Results

  m_resultsTabController = std::shared_ptr<ResultsTabController>( new ResultsTabController() );
  m_mainWindow->addVerticalTab( m_resultsTabController->mainContentWidget(),
                                RESULTS_SUMMARY,
                                "Results Summary",
                                ":images/on_results_tab.png",
                                ":images/off_results_tab.png" );
  connect(m_resultsTabController->mainContentWidget(),SIGNAL(tabSelected(int)),
          m_mainRightColumnController.get(),SLOT(configureForResultsSummarySubTab(int)));

  isConnected = connect(this,SIGNAL(toggleUnitsClicked(bool)),
                        m_resultsTabController.get(), SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);

  isConnected = connect(this, SIGNAL(treeChanged(const openstudio::UUID &)),
    m_resultsTabController->mainContentWidget(), SIGNAL(treeChanged(const openstudio::UUID &)));
  OS_ASSERT(isConnected);

  m_resultsTabController->searchForExistingResults(openstudio::toPath(m_modelTempDir) / openstudio::toPath("resources") / openstudio::toPath("run"));

  // DLM: this signal is not connected, is it needed?
  connect(m_runTabController.get(), SIGNAL(toolsUpdated()),
      m_scriptsTabController.get(), SIGNAL(updateRubyInterpreterWarning()));

  // DLM: this signal is not connected, is it needed?
  connect(m_scriptsTabController.get(), SIGNAL(toolsUpdated()),
      this, SLOT(markAsModified()));

  connect(m_runTabController.get(), SIGNAL(toolsUpdated()),
      this, SLOT(markAsModified()));

  connect(this, SIGNAL(toolsUpdated()),
          this, SLOT(markAsModified()));

  // DLM: this signal is not connected, is it needed?
  connect(this, SIGNAL(toolsUpdated()),
      m_scriptsTabController.get(), SIGNAL(updateRubyInterpreterWarning()));

  connect(this, SIGNAL(toolsUpdated()),
      m_runTabController.get(), SLOT(updateToolsWarnings()));
  connect(m_runTabController.get(), SIGNAL(toolsUpdated()),
      m_runTabController.get(), SLOT(updateToolsWarnings()));
  isConnected = connect(m_model.getImpl<openstudio::model::detail::Model_Impl>().get(), SIGNAL(onChange()), this, SLOT(markAsModified()) );
  OS_ASSERT(isConnected);

  isConnected = connect( m_hvacSystemsTabController.get(),
           SIGNAL(modelObjectSelected(model::OptionalModelObject &, bool )),
           this,
           SLOT(inspectModelObject( model::OptionalModelObject &, bool )) );
  OS_ASSERT(isConnected);

  if (modified){
    QTimer::singleShot(0, this, SLOT(markAsModified()));
  } else {
    QTimer::singleShot(0, this, SLOT(markAsUnmodified()));
  }

  // DLM: this might work to reload weather file if changed?
  this->setFullWeatherFilePath(); 

  QTimer::singleShot(0, this, SLOT(initializeModel())); 

  app->waitDialog()->setVisible(false);
  m_mainWindow->setVisible(wasVisible);

  QTimer::singleShot(0, this, SLOT(showStartTabAndStartSubTab())); 
}

runmanager::RunManager OSDocument::runManager() {
  return m_runTabController->runManager();
}

void OSDocument::markAsModified()
{
  m_mainWindow->setWindowModified(true);

  QString fileName = this->mainWindow()->windowFilePath();

  QFile testFile(fileName);
  if(!testFile.exists()) return;

  m_mainWindow->enableRevertToSavedAction(true);
}

void OSDocument::markAsUnmodified()
{
  m_mainWindow->setWindowModified(false);

  m_mainWindow->enableRevertToSavedAction(false);
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
      m_resultsTabController->searchForExistingResults(searchPath);
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
  m_mainTabId = verticalId;

  if(m_mainTabId != RUBY_SCRIPTS && m_mainRightColumnController->isMyModelTabHidden()){
     m_mainRightColumnController->hideMyModelTab(false);
  }
  switch( m_mainTabId )
  {
    case SITE:
      m_subTabId = m_locationTabController->mainContentWidget()->subTabId();
      m_mainRightColumnController->configureForSiteSubTab(m_subTabId);
      break;
    case SCHEDULES:
      m_subTabId = m_schedulesTabController->mainContentWidget()->subTabId();
      m_mainRightColumnController->configureForSchedulesSubTab(m_subTabId);
      break;
    case CONSTRUCTIONS:
      m_subTabId = m_constructionsTabController->mainContentWidget()->subTabId();
      m_mainRightColumnController->configureForConstructionsSubTab(m_subTabId);
      break;
    case LOADS:
      m_subTabId = m_loadsTabController->mainContentWidget()->subTabId();
      m_mainRightColumnController->configureForLoadsSubTab(m_subTabId);
      break;
    case SPACE_TYPES:
      m_subTabId = m_spaceTypesTabController->mainContentWidget()->subTabId();
      m_mainRightColumnController->configureForSpaceTypesSubTab(m_subTabId);
      break;
    case BUILDING_STORIES:
      m_subTabId = m_buildingStoriesTabController->mainContentWidget()->subTabId();
      m_mainRightColumnController->configureForBuildingStoriesSubTab(m_subTabId);
      break;
    case FACILITY:
      m_subTabId = m_facilityTabController->mainContentWidget()->subTabId();
      m_mainRightColumnController->configureForFacilitySubTab(m_subTabId);
      break;
    case THERMAL_ZONES:
      m_subTabId = m_thermalZonesTabController->mainContentWidget()->subTabId();
      m_mainRightColumnController->configureForThermalZonesSubTab(m_subTabId);
      break;
    case HVAC_SYSTEMS:
      m_subTabId = m_hvacSystemsTabController->mainContentWidget()->subTabId();
      m_mainRightColumnController->configureForHVACSystemsSubTab(m_subTabId);
      m_hvacSystemsTabController->clearSceneSelection();
      break;
    case BUILDING_SUMMARY:
      m_subTabId = m_summaryTabController->mainContentWidget()->subTabId();
      m_mainRightColumnController->configureForBuildingSummarySubTab(m_subTabId);
      break;
    case OUTPUT_VARIABLES:
      m_subTabId = m_variablesTabController->mainContentWidget()->subTabId();
      m_mainRightColumnController->configureForOutputVariablesSubTab(m_subTabId);
      break;
    case SIMULATION_SETTINGS:
      m_subTabId = m_simSettingsTabController->mainContentWidget()->subTabId();
      m_mainRightColumnController->configureForSimulationSettingsSubTab(m_subTabId);
      break;
    case RUBY_SCRIPTS:
      // Do special stuff here to hide "My Model" tab
      m_mainRightColumnController->hideMyModelTab(true);
      m_subTabId = m_scriptsTabController->mainContentWidget()->subTabId();
      m_mainRightColumnController->configureForScriptsSubTab(m_subTabId);
      break;
    case RUN_SIMULATION:
      m_subTabId = m_runTabController->mainContentWidget()->subTabId();
      m_mainRightColumnController->configureForRunSimulationSubTab(m_subTabId);
      break;
    case RESULTS_SUMMARY:
      m_subTabId = m_resultsTabController->mainContentWidget()->subTabId();
      m_mainRightColumnController->configureForResultsSummarySubTab(m_subTabId);
      break;
    default:
      break;
  }

  //m_mainWindow->selectHorizontalTab(LIBRARY);

  boost::optional<model::ModelObject> mo;

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
  
  QString fileName = QFileDialog::getSaveFileName( this->mainWindow(),
                                                  tr("Export Idf"),
                                                  defaultDir,
                                                  tr("(*.idf)") );

  if( ! fileName.isEmpty() )
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
  if(type == SDD){
    text.append("SDD");
  } else if(type == GBXML) {
    text.append("gbXML");
  } else {
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

    if(type == SDD){
      sdd::ForwardTranslator trans;
      trans.modelToSDD(m, outDir);
      translatorErrors = trans.errors();
      translatorWarnings = trans.warnings();
    } else if(type == GBXML) {
      gbxml::ForwardTranslator trans;
      trans.modelToGbXML(m, outDir);
      translatorErrors = trans.errors();
      translatorWarnings = trans.warnings();
    } 

    bool errorsOrWarnings = false;
    QString log;
    for( std::vector<LogMessage>::iterator it = translatorErrors.begin();
          it < translatorErrors.end();
          ++it )
    {
      errorsOrWarnings = true;

      log.append(QString::fromStdString(it->logMessage()));
      log.append("\n");
      log.append("\n");
    }  

    for( std::vector<LogMessage>::iterator it = translatorWarnings.begin();
          it < translatorWarnings.end();
          ++it )
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
      QMessageBox::critical( mainWindow(), "Incomplete File Removal", QString("Removed all design alternatives from this project, but could not remove all of the associated files. Close all files and clear results to clean up your file system.") );
    }
  }else if (analysis.resultsAreInvalid()){
    bool completeRemoval = m_simpleProject->clearAllResults();
    if (!completeRemoval) {
      QMessageBox::critical( mainWindow(), "Incomplete File Removal", QString("Removed all results from this project, but could not remove all of the result files. Close all files and clear results again to clean up your file system.") );
    }
  } 

  m_simpleProject->save();

  if( !m_savePath.isEmpty() )
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
  }else{
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

  QString filePath = QFileDialog::getSaveFileName( this->mainWindow(),
                                                  tr("Save"),
                                                  defaultDir,
                                                  tr("(*.osm)") );

  if( ! filePath.isEmpty() )
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
          }catch(...){
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
  }else if (fromComponentLibrary(itemId)){
    if (itemId.sourceId() == modelToSourceId(m_compLibrary)){
      Handle handle(itemId.itemId());
      return m_compLibrary.getModelObject<model::ModelObject>(handle);
    }else if (itemId.sourceId() == modelToSourceId(m_hvacCompLibrary)){
      Handle handle(itemId.itemId());
      return m_hvacCompLibrary.getModelObject<model::ModelObject>(handle);
    }else if (itemId.sourceId() == modelToSourceId(m_combinedCompLibrary)){
      Handle handle(itemId.itemId());
      return m_combinedCompLibrary.getModelObject<model::ModelObject>(handle);
    }
  }

  return boost::none;
}

boost::optional<model::Component> OSDocument::getComponent(const OSItemId& itemId) const
{
  boost::optional<model::Component> modelComponent;

  if(itemId.sourceId() == OSItemId::BCL_SOURCE_ID) 
  {
    boost::optional<BCLComponent> component;

    component = LocalBCL::instance().getComponent(itemId.itemId().toStdString());

    if(component){

      std::vector<std::string> oscFiles = component->files("osc");

      if(oscFiles.size() == 1u){

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
  if(this->modified()){

    QMessageBox * messageBox = new QMessageBox(this->mainWindow());
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
        if(this->save() != true) return;
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
  bool isConnected = false;
  isConnected = connect(m_applyMeasureNowDialog.get(), SIGNAL(toolsUpdated()), this, SIGNAL(toolsUpdated()));
  OS_ASSERT(isConnected);

  m_applyMeasureNowDialog->exec();

  // DLM: kill the dialog here as there is logic in the destructor that resets application state
  // DLM: this seems overly complicated
  m_applyMeasureNowDialog.reset();
}

void OSDocument::openChangeMeasuresDirDlg()
{
  openstudio::path userMeasuresDir = BCLMeasure::userMeasuresDir();
 
  QString dirName = QFileDialog::getExistingDirectory( this->mainWindow(),
                                                       tr("Select My Measures Directory"),
                                                       toQString(userMeasuresDir));
 
  if(dirName.length() > 0){
    userMeasuresDir = toPath(dirName);
    if (BCLMeasure::setUserMeasuresDir(userMeasuresDir)){
      OSAppBase::instance()->measureManager().updateMeasuresLists();
    }
  }
}

void OSDocument::changeBclLogin()
{
  // TODO
  QMessageBox::information( this->mainWindow(), QString("Change BCL Login Information"), QString("Not yet available.\nMiddleware testing required."));
}

void OSDocument::updateWindowFilePath()
{
  if ( m_savePath.isEmpty() ){
    m_mainWindow->setWindowFilePath("Untitled");
  }else{
    m_mainWindow->setWindowFilePath(m_savePath);
  }
}

void OSDocument::openBclDlg()
{
  std::string authKey = LocalBCL::instance().prodAuthKey();
  if (!LocalBCL::instance().setProdAuthKey(authKey)){
    LocalBCL::instance().prodAuthKeyUserPrompt(m_mainWindow);
    authKey = LocalBCL::instance().prodAuthKey();

    while(!LocalBCL::instance().setProdAuthKey(authKey)){
      QMessageBox dlg(m_mainWindow);
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
        LocalBCL::instance().prodAuthKeyUserPrompt(m_mainWindow);
        authKey = LocalBCL::instance().prodAuthKey();
      }
      else{
        // should never get here
        OS_ASSERT(false);
      }
    }
  }

  if(!m_onlineBclDialog){
    std::string filterType = "components";
    m_onlineBclDialog = new BuildingComponentDialog(filterType, true, m_mainWindow);

    bool isConnected = connect(m_onlineBclDialog, SIGNAL(rejected()),
                          this, SLOT(on_closeBclDlg()));
    OS_ASSERT(isConnected);
  }
  if(m_onlineBclDialog && !m_onlineBclDialog->isVisible()){
    m_onlineBclDialog->setGeometry(m_mainWindow->geometry());
    m_onlineBclDialog->show();
  }
}

void OSDocument::on_closeBclDlg()
{
  if(m_onlineBclDialog->showNewComponents()){
    onVerticalTabSelected(m_mainTabId); 
    m_onlineBclDialog->setShowNewComponents(false);
  }
}

void OSDocument::openLibDlg()
{
  if(!m_localLibraryDialog){
    std::string filterType = "components";
    m_localLibraryDialog = new BuildingComponentDialog(filterType, false, m_mainWindow);
  }
  if(m_localLibraryDialog && !m_localLibraryDialog->isVisible()){
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

    while(!LocalBCL::instance().setProdAuthKey(authKey)){
      QMessageBox dlg(m_mainWindow);
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
        LocalBCL::instance().prodAuthKeyUserPrompt(m_mainWindow);
        authKey = LocalBCL::instance().prodAuthKey();
      }
      else{
        // should never get here
        OS_ASSERT(false);
      }
    }
  }

  if(!m_onlineMeasuresBclDialog){
    std::string filterType = "measures";
    m_onlineMeasuresBclDialog = new BuildingComponentDialog(filterType, true, m_mainWindow);

    bool isConnected = connect(m_onlineMeasuresBclDialog, SIGNAL(rejected()),
                          this, SLOT(on_closeMeasuresBclDlg()));
    OS_ASSERT(isConnected);
  }
  if(m_onlineMeasuresBclDialog && !m_onlineMeasuresBclDialog->isVisible()){
    m_onlineMeasuresBclDialog->setGeometry(m_mainWindow->geometry());
    m_onlineMeasuresBclDialog->show();
  }

}

void OSDocument::on_closeMeasuresBclDlg()
{
  if(m_onlineMeasuresBclDialog->showNewComponents()){
    OSAppBase::instance()->measureManager().updateMeasuresLists();
    m_onlineMeasuresBclDialog->setShowNewComponents(false);
  }
}

} // openstudio
