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

#include <openstudio_lib/OSDocument.hpp>
#include "OSAppBase.hpp"
#include "../shared_gui_components/BuildingComponentDialog.hpp"
#include "../shared_gui_components/LocalLibraryController.hpp"
#include "../shared_gui_components/MeasureManager.hpp"

#include <openstudio_lib/BuildingStoriesTabController.hpp>
#include <openstudio_lib/ConstructionsTabController.hpp>
#include <openstudio_lib/FacilityTabController.hpp>
#include <openstudio_lib/FacilityView.hpp>
#include <openstudio_lib/FileOperations.hpp>
#include <openstudio_lib/HVACSystemsTabController.hpp>
#include <openstudio_lib/HorizontalTabWidget.hpp>
#include <openstudio_lib/InspectorController.hpp>
#include <openstudio_lib/InspectorView.hpp>
#include <openstudio_lib/LibraryTabWidget.hpp>
#include <openstudio_lib/LoadsTabController.hpp>
#include <openstudio_lib/LocationTabController.hpp>
#include <openstudio_lib/LocationTabView.hpp>
#include <openstudio_lib/MainRightColumnController.hpp>
#include <openstudio_lib/MainWindow.hpp>
#include <openstudio_lib/ModelObjectItem.hpp>
#include <openstudio_lib/ModelObjectTypeListView.hpp>
#include <openstudio_lib/ResultsTabController.hpp>
#include <openstudio_lib/ResultsTabView.hpp>
#include <openstudio_lib/RunTabController.hpp>
#include <openstudio_lib/RunTabView.hpp>
#include <openstudio_lib/SchedulesTabController.hpp>
#include <openstudio_lib/ScriptFolderListView.hpp>
#include <openstudio_lib/ScriptsTabController.hpp>
#include <openstudio_lib/ScriptsTabView.hpp>
#include <openstudio_lib/SimSettingsTabController.hpp>
#include <openstudio_lib/SimSettingsTabView.hpp>
#include <openstudio_lib/SpaceTypesTabController.hpp>
#include <openstudio_lib/SpaceTypesView.hpp>
#include <openstudio_lib/SummaryTabController.hpp>
#include <openstudio_lib/SummaryTabView.hpp>
#include <openstudio_lib/ThermalZonesTabController.hpp>
#include <openstudio_lib/VariablesTabController.hpp>
#include <openstudio_lib/YearSettingsWidget.hpp>

#include <analysis/Analysis.hpp>

#include <model/Component.hpp>
#include <model/Model_Impl.hpp>
#include <model/WeatherFile.hpp>
#include <model/WeatherFile_Impl.hpp>

#include <utilities/bcl/BCLComponent.hpp>
#include <utilities/bcl/LocalBCL.hpp>
#include <utilities/bcl/RemoteBCL.hpp>
#include <utilities/core/Application.hpp>
#include <utilities/core/Assert.hpp>
#include <utilities/core/PathHelpers.hpp>
#include <utilities/data/Attribute.hpp>
#include <utilities/idf/IdfFile.hpp>
#include <utilities/idf/ValidityReport.hpp>
#include <utilities/idf/Workspace.hpp>

#include <osversion/VersionTranslator.hpp>

#include <analysis/DataPoint.hpp>
#include <analysis/Problem.hpp>
#include <analysis/MeasureGroup.hpp>
#include <analysis/NullMeasure.hpp>

#include <runmanager/lib/WorkItem.hpp>

#include <OpenStudio.hxx>

#include <energyplus/ForwardTranslator.hpp>

#include <boost/filesystem/fstream.hpp>
#include <boost/filesystem.hpp>

#include <QDir>
#include <QFileDialog>
#include <QFileInfo>
#include <QFileInfoList>
#include <QMessageBox>
#include <QString>
#include <QTemporaryFile>
#include <QTimer>
#include <QMenuBar>
#include <QWidget>

#ifdef _WINDOWS
  #include <windows.h>
#endif

namespace openstudio {

OSDocument::OSDocument( openstudio::model::Model library,
                        openstudio::model::Model hvacLibrary,
                        const openstudio::path &resourcesPath,
                        openstudio::model::OptionalModel model,
                        QString filePath, bool isPlugin )
  : OSQObjectController(),
    m_compLibrary(library),
    m_hvacCompLibrary(hvacLibrary),
    m_resourcesPath(resourcesPath),
    m_onlineMeasuresBclDialog(NULL),
    m_onlineBclDialog(NULL),
    m_localLibraryDialog(NULL),
    m_savePath(filePath),
    m_isPlugin(isPlugin)
{


  bool modifiedOnLoad = false;
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

  if( model )
  {
    m_model = model.get();
  }
  else
  {
    m_model = openstudio::model::Model();
  }

  m_mainWindow = new MainWindow(m_isPlugin);
  isConnected = connect(m_mainWindow, SIGNAL(openBclDlgClicked()), this, SLOT(openBclDlg()));
  OS_ASSERT(isConnected);
  isConnected = connect(m_mainWindow, SIGNAL(openLibDlgClicked()), this, SLOT(openLibDlg()));
  OS_ASSERT(isConnected);

  addQObject(m_mainWindow);

  bool isIP = m_mainWindow->displayIP();

  openstudio::path modelTempDir = createModelTempDir();
  m_modelTempDir = toQString(modelTempDir);

  if( !m_savePath.isEmpty() )
  {
    initializeModelTempDir(toPath(m_savePath), modelTempDir);
  }

  modifiedOnLoad |= updateModelTempDir(m_model, modelTempDir);


  openstudio::analysisdriver::SimpleProjectOptions options;
  options.setPauseRunManagerQueue(true); // do not start running when opening
  options.setLogLevel(Debug);

  // initialize project object
  if (boost::filesystem::exists(openstudio::toPath(m_modelTempDir) / openstudio::toPath("resources/project.osp")))
  {
    LOG(Debug, "project existed, opening");
    m_simpleProject = openstudio::analysisdriver::SimpleProject::open(openstudio::toPath(m_modelTempDir) / openstudio::toPath("resources"), options);

    if (m_simpleProject)
    {
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
    
    }
  } else {
    LOG(Debug, "Creating new project");
    m_simpleProject = openstudio::analysisdriver::SimpleProject::create(openstudio::toPath(m_modelTempDir) / openstudio::toPath("resources"), 
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
  
  // make sure project has baseline stuff setup
  analysis::DataPoint baselineDataPoint = m_simpleProject->baselineDataPoint();
  openstudio::analysis::Analysis analysis = m_simpleProject->analysis();

  openstudio::runmanager::ConfigOptions co(true);
  m_simpleProject->runManager().setConfigOptions(co);

  isConnected = analysis.connect(SIGNAL(changed(ChangeType)), this, SLOT(markAsModified()));
  OS_ASSERT(isConnected);


  // Main Right Column

  m_mainRightColumnController = 
    boost::shared_ptr<MainRightColumnController>(new MainRightColumnController(m_model, m_resourcesPath));
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)),
                        m_mainRightColumnController.get(), SIGNAL(toggleUnitsClicked(bool)));
  OS_ASSERT(isConnected);

  m_mainWindow->setMainRightColumnView(m_mainRightColumnController->mainRightColumnView());

  // Main Vertical Tabs

  // Location

  m_locationTabController = boost::shared_ptr<LocationTabController>( new LocationTabController(m_model, m_modelTempDir) );
  m_mainWindow->addVerticalTab( m_locationTabController->mainContentWidget(),
                                SITE,
                                "Site",
                                ":images/on_location_tab.png",
                                ":images/off_location_tab.png" );
  connect(m_locationTabController->mainContentWidget(),SIGNAL(tabSelected(int)),
          m_mainRightColumnController.get(),SLOT(configureForSiteSubTab(int)));

  // Schedules

  m_schedulesTabController = boost::shared_ptr<SchedulesTabController>( new SchedulesTabController(isIP, m_model) );
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

  isConnected = QObject::connect(m_schedulesTabController.get(), SIGNAL(openBclDlgClicked()),
                                 this, SIGNAL(openBclDlgClicked()));
  OS_ASSERT(isConnected);

  isConnected = QObject::connect(m_schedulesTabController.get(), SIGNAL(openLibDlgClicked()),
                                 this, SIGNAL(openLibDlgClicked()));
  OS_ASSERT(isConnected);

  // Constructions

  m_constructionsTabController = boost::shared_ptr<ConstructionsTabController>( new ConstructionsTabController(isIP, m_model) );
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

  isConnected = QObject::connect(m_constructionsTabController.get(), SIGNAL(openBclDlgClicked()),
                                 this, SIGNAL(openBclDlgClicked()));
  OS_ASSERT(isConnected);

  isConnected = QObject::connect(m_constructionsTabController.get(), SIGNAL(openLibDlgClicked()),
                                 this, SIGNAL(openLibDlgClicked()));
  OS_ASSERT(isConnected);

  // Loads

  m_loadsTabController = boost::shared_ptr<LoadsTabController>( new LoadsTabController(isIP, m_model) );
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

  isConnected = QObject::connect(m_loadsTabController.get(), SIGNAL(openBclDlgClicked()),
                                 this, SIGNAL(openBclDlgClicked()));
  OS_ASSERT(isConnected);

  isConnected = QObject::connect(m_loadsTabController.get(), SIGNAL(openLibDlgClicked()),
                                 this, SIGNAL(openLibDlgClicked()));
  OS_ASSERT(isConnected);

  // Space Types

  m_spaceTypesTabController = boost::shared_ptr<SpaceTypesTabController>( new SpaceTypesTabController(m_model) );
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

  isConnected = QObject::connect(m_spaceTypesTabController.get(), SIGNAL(openBclDlgClicked()),
                                 this, SIGNAL(openBclDlgClicked()));
  OS_ASSERT(isConnected);

  isConnected = QObject::connect(m_spaceTypesTabController.get(), SIGNAL(openLibDlgClicked()),
                                 this, SIGNAL(openLibDlgClicked()));
  OS_ASSERT(isConnected);

  // Building Stories

  m_buildingStoriesTabController = boost::shared_ptr<BuildingStoriesTabController>( new BuildingStoriesTabController(m_model) );
  m_mainWindow->addVerticalTab( m_buildingStoriesTabController->mainContentWidget(),
                                BUILDING_STORIES,
                                "Building Stories",
                                ":images/on_building_stories_tab.png",
                                ":images/off_building_stories_tab.png" );
  connect(m_buildingStoriesTabController->mainContentWidget(),SIGNAL(tabSelected(int)),
          m_mainRightColumnController.get(),SLOT(configureForBuildingSummarySubTab(int)));
  
  isConnected = QObject::connect(m_buildingStoriesTabController.get(), SIGNAL(openBclDlgClicked()),
                                 this, SIGNAL(openBclDlgClicked()));
  OS_ASSERT(isConnected);

  isConnected = QObject::connect(m_buildingStoriesTabController.get(), SIGNAL(openLibDlgClicked()),
                                 this, SIGNAL(openLibDlgClicked()));
  OS_ASSERT(isConnected);

  // Facility

  m_facilityTabController = boost::shared_ptr<FacilityTabController>( new FacilityTabController(isIP, m_model) );
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

  isConnected = QObject::connect(m_facilityTabController.get(), SIGNAL(openBclDlgClicked()),
                                 this, SIGNAL(openBclDlgClicked()));
  OS_ASSERT(isConnected);

  isConnected = QObject::connect(m_facilityTabController.get(), SIGNAL(openLibDlgClicked()),
                                 this, SIGNAL(openLibDlgClicked()));
  OS_ASSERT(isConnected);

  // Thermal Zones 

  m_thermalZonesTabController = boost::shared_ptr<ThermalZonesTabController>( new ThermalZonesTabController(isIP, m_model) );
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

  m_hvacSystemsTabController = boost::shared_ptr<HVACSystemsTabController>( new HVACSystemsTabController(m_model) );
  m_mainWindow->addVerticalTab( m_hvacSystemsTabController->mainContentWidget(),
                                HVAC_SYSTEMS,
                                "HVAC Systems",
                                ":images/on_hvac_tab.png",
                                ":images/off_hvac_tab.png" );
  connect(m_hvacSystemsTabController->mainContentWidget(),SIGNAL(tabSelected(int)),
          m_mainRightColumnController.get(),SLOT(configureForHVACSystemsSubTab(int)));

  //******************************************************************************************************
  //
  ///! TODO This tab has been deprecated until building summary information is availalbe
  //
  //// Summary
  //
  //m_summaryTabController = boost::shared_ptr<SummaryTabController>( new SummaryTabController(m_model) );
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

  m_variablesTabController = boost::shared_ptr<VariablesTabController>( new VariablesTabController(m_model) );
  m_mainWindow->addVerticalTab( m_variablesTabController->mainContentWidget(),
                                OUTPUT_VARIABLES,
                                "Output Variables",
                                ":images/on_var_tab.png",
                                ":images/off_var_tab.png" );
  connect(m_variablesTabController->mainContentWidget(),SIGNAL(tabSelected(int)),
          m_mainRightColumnController.get(),SLOT(configureForOutputVariablesSubTab(int)));

  // Sim Settings

  m_simSettingsTabController = boost::shared_ptr<SimSettingsTabController>( new SimSettingsTabController(isIP, m_model) );
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

  m_scriptsTabController = boost::shared_ptr<ScriptsTabController>( new ScriptsTabController() );
  m_mainWindow->addVerticalTab( m_scriptsTabController->mainContentWidget(),
                                RUBY_SCRIPTS,
                                "Measures",
                                ":images/on_scripts_tab.png",
                                ":images/off_scripts_tab.png" );
  connect(m_scriptsTabController->mainContentWidget(),SIGNAL(tabSelected(int)),
          m_mainRightColumnController.get(),SLOT(configureForScriptsSubTab(int)));

  //connect(m_scriptsTabController->scriptFolderListView(), SIGNAL(scriptListChanged()),
  //    this, SLOT(markAsModified()));

  //isConnected = QObject::connect(m_scriptsTabController.get(), SIGNAL(openBclDlgClicked()),
  //                               this, SIGNAL(openBclDlgClicked()));
  //OS_ASSERT(isConnected);

  //isConnected = QObject::connect(m_scriptsTabController.get(), SIGNAL(openLibDlgClicked()),
  //                               this, SIGNAL(openLibDlgClicked()));
  //OS_ASSERT(isConnected);

  // Run

  m_runTabController = boost::shared_ptr<RunTabController>( new RunTabController(m_model, 
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

  // Results

  m_resultsTabController = boost::shared_ptr<ResultsTabController>( new ResultsTabController(m_model) );
  m_mainWindow->addVerticalTab( m_resultsTabController->mainContentWidget(),
                                RESULTS_SUMMARY,
                                "Results Summary",
                                ":images/on_results_tab.png",
                                ":images/off_results_tab.png" );
  connect(m_resultsTabController->mainContentWidget(),SIGNAL(tabSelected(int)),
          m_mainRightColumnController.get(),SLOT(configureForResultsSummarySubTab(int)));

  isConnected = connect(this,SIGNAL(toggleUnitsClicked(bool)),
                        m_resultsTabController.get(), SLOT(onUnitSystemChange(bool)));

  connect(m_runTabController.get(), SIGNAL(resultsGenerated(const openstudio::path &, const openstudio::path &)), 
      m_resultsTabController.get(), SLOT(resultsGenerated(const openstudio::path &, const openstudio::path &)));

  connect(m_runTabController.get(), SIGNAL(resultsGenerated(const openstudio::path &, const openstudio::path &)),
      this, SLOT(runComplete()));

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


  isConnected = connect( m_mainWindow, SIGNAL(closeClicked()), this, SIGNAL(closeClicked()) );
  OS_ASSERT(isConnected);

  isConnected = connect(m_model.getImpl<openstudio::model::detail::Model_Impl>().get(), SIGNAL(onChange()), this, SLOT(markAsModified()) );
  OS_ASSERT(isConnected);

  isConnected = connect( m_hvacSystemsTabController.get(),
           SIGNAL(modelObjectSelected(model::OptionalModelObject &, bool )),
           this,
           SLOT(inspectModelObject( model::OptionalModelObject &, bool )) );
  OS_ASSERT(isConnected);

  isConnected = connect(m_mainWindow,SIGNAL(verticalTabSelected(int)),this,SLOT(onVerticalTabSelected(int)));
  OS_ASSERT(isConnected);

  isConnected = connect(m_mainWindow, SIGNAL(importClicked()), this, SIGNAL(importClicked()));
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
  isConnected = connect(m_mainWindow, SIGNAL(saveAsFileClicked()), this, SLOT(saveAs()));
  OS_ASSERT(isConnected);
  isConnected = connect(m_mainWindow, SIGNAL(saveFileClicked()), this, SLOT(save()));
  OS_ASSERT(isConnected);
  isConnected = connect(m_mainWindow, SIGNAL(scanForToolsClicked()), this, SLOT(scanForTools()));
  OS_ASSERT(isConnected);
  isConnected = connect(m_mainWindow, SIGNAL(showRunManagerPreferencesClicked()), this, SLOT(showRunManagerPreferences()));
  OS_ASSERT(isConnected);
  isConnected = connect(m_mainWindow, SIGNAL(toggleUnitsClicked(bool)), this, SIGNAL(toggleUnitsClicked(bool)));
  OS_ASSERT(isConnected);

  isConnected = QObject::connect(this, SIGNAL(openBclDlgClicked()),
                                 this, SLOT(openBclDlg()));
  OS_ASSERT(isConnected);

  isConnected = QObject::connect(this, SIGNAL(openLibDlgClicked()),
                                 this, SLOT(openLibDlg()));
  OS_ASSERT(isConnected);

  QTimer::singleShot(0, this, SLOT(showFirstTab())); 

  if( m_savePath.isEmpty() ){
    m_mainWindow->setWindowFilePath("Untitled");
    QTimer::singleShot(0, this, SLOT(markAsUnmodified()));
  }else{
    m_mainWindow->setWindowFilePath(m_savePath);
    if (modifiedOnLoad)
    {
      QTimer::singleShot(0, this, SLOT(markAsModified()));
    } else {
      QTimer::singleShot(0, this, SLOT(markAsUnmodified()));
    }
  }
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
  
void OSDocument::showFirstTab()
{
  m_mainWindow->selectVerticalTab(SITE);

  m_mainWindow->show();
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

runmanager::RunManager OSDocument::runManager() {
  return m_runTabController->runManager();
}

void OSDocument::markAsModified()
{
  m_mainWindow->setWindowModified(true);
}

void OSDocument::markAsUnmodified()
{
  m_mainWindow->setWindowModified(false);
}

void OSDocument::runComplete()
{
  if (!m_savePath.isEmpty()){
    // copy all the simulation output to the save location
    // do not want to save the database or osm here
    saveModelTempDir(toPath(m_modelTempDir), toPath(m_savePath));
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
  if ( m_savePath.isEmpty() ){
    m_mainWindow->setWindowFilePath("Untitled");
  }else{
    m_mainWindow->setWindowFilePath(m_savePath);
  }
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
          openstudio::path searchDir = osmPath.parent_path() / toPath(osmPath.stem());
          result = weatherFile->makeUrlAbsolute(searchDir);
        }
      }
    }
  }

  return result;
}

void OSDocument::onVerticalTabSelected(int verticalId)
{
  m_mainTabId = verticalId;

  switch( m_mainTabId )
  {
    case SITE:
      m_subTabId = m_locationTabController->mainContentWidget()->currentId();
      m_mainRightColumnController->configureForSiteSubTab(m_subTabId);
      break;
    case SCHEDULES:
      m_subTabId = m_schedulesTabController->mainContentWidget()->currentId();
      m_mainRightColumnController->configureForSchedulesSubTab(m_subTabId);
      break;
    case CONSTRUCTIONS:
      m_subTabId = m_constructionsTabController->mainContentWidget()->currentId();
      m_mainRightColumnController->configureForConstructionsSubTab(m_subTabId);
      break;
    case LOADS:
      m_subTabId = m_loadsTabController->mainContentWidget()->currentId();
      m_mainRightColumnController->configureForLoadsSubTab(m_subTabId);
      break;
    case SPACE_TYPES:
      m_subTabId = m_spaceTypesTabController->mainContentWidget()->currentId();
      m_mainRightColumnController->configureForSpaceTypesSubTab(m_subTabId);
      break;
    case BUILDING_STORIES:
      m_subTabId = m_buildingStoriesTabController->mainContentWidget()->currentId();
      m_mainRightColumnController->configureForBuildingStoriesSubTab(m_subTabId);
      break;
    case FACILITY:
      m_subTabId = m_facilityTabController->mainContentWidget()->currentId();
      m_mainRightColumnController->configureForFacilitySubTab(m_subTabId);
      break;
    case THERMAL_ZONES:
      m_subTabId = m_thermalZonesTabController->mainContentWidget()->currentId();
      m_mainRightColumnController->configureForThermalZonesSubTab(m_subTabId);
      break;
    case HVAC_SYSTEMS:
      m_subTabId = m_hvacSystemsTabController->mainContentWidget()->currentId();
      m_mainRightColumnController->configureForHVACSystemsSubTab(m_subTabId);
      m_hvacSystemsTabController->clearSceneSelection();
      break;
    case BUILDING_SUMMARY:
      m_subTabId = m_summaryTabController->mainContentWidget()->currentId();
      m_mainRightColumnController->configureForBuildingSummarySubTab(m_subTabId);
      break;
    case OUTPUT_VARIABLES:
      m_subTabId = m_variablesTabController->mainContentWidget()->currentId();
      m_mainRightColumnController->configureForOutputVariablesSubTab(m_subTabId);
      break;
    case SIMULATION_SETTINGS:
      m_subTabId = m_simSettingsTabController->mainContentWidget()->currentId();
      m_mainRightColumnController->configureForSimulationSettingsSubTab(m_subTabId);
      break;
    case RUBY_SCRIPTS:
      m_subTabId = m_scriptsTabController->mainContentWidget()->currentId();
      m_mainRightColumnController->configureForScriptsSubTab(m_subTabId);
      break;
    case RUN_SIMULATION:
      m_subTabId = m_runTabController->mainContentWidget()->currentId();
      m_mainRightColumnController->configureForRunSimulationSubTab(m_subTabId);
      break;
    case RESULTS_SUMMARY:
      m_subTabId = m_resultsTabController->mainContentWidget()->currentId();
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

  QString fileName = QFileDialog::getSaveFileName( this->mainWindow(),
                                                  tr("Export Idf"),
                                                  QDir::homePath(),
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

void OSDocument::save()
{
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
  }else{
    saveAs();
  }
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

void OSDocument::saveAs()
{
  QString filePath, defaultDir;

  if (!this->savePath().length()){
    defaultDir = QDir::homePath();
  }else{
    defaultDir = toQString(openstudio::toPath(this->savePath()).parent_path());
  }

  filePath = QFileDialog::getSaveFileName( this->mainWindow(),
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
  }
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
      BOOST_FOREACH(const Attribute& attribute, component->attributes()){
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

    bool isConnected = false;

    isConnected = connect(m_onlineBclDialog, SIGNAL(rejected()),
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

boost::shared_ptr<MainRightColumnController> OSDocument::mainRightColumnController() const
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

    bool isConnected = false;

    isConnected = connect(m_onlineMeasuresBclDialog, SIGNAL(rejected()),
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
