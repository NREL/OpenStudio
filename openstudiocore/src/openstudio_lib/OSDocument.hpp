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

#ifndef OPENSTUDIO_OSDOCUMENT_H
#define OPENSTUDIO_OSDOCUMENT_H

#include <openstudio_lib/OpenStudioAPI.hpp>
#include <shared_gui_components/OSQObjectController.hpp>

#include <model/Model.hpp>
#include <model/ModelObject.hpp>
#include <ruleset/RubyUserScriptArgumentGetter.hpp>
#include <analysisdriver/SimpleProject.hpp>

#include <QObject>
#include <QString>

#include <boost/smart_ptr.hpp>

class QDir;

namespace openstudio {

namespace runmanager {
  class RunManager;
}

class MainRightColumnController;

class HVACSystemsTabController;

class ThermalZonesTabController;

class SchedulesTabController;

class InspectorController;

class MainWindow;

class LibraryTabWidget;

class LocationTabController;

class ConstructionsTabController;

class LoadsTabController;

class SpaceTypesTabController;

class FacilityTabController;

class SummaryTabController;

class VariablesTabController;

class SimSettingsTabController;

class ScriptsTabController;

class ScriptFolderListView;

class RunTabController;

class ResultsTabController;

class BuildingStoriesTabController;

class OSItemId;

class BuildingComponentDialog;

class ApplyMeasureNowDialog;

class OPENSTUDIO_API OSDocument : public OSQObjectController {
  Q_OBJECT

  public:

  OSDocument( openstudio::model::Model library,
              openstudio::model::Model hvacLibrary,
              const openstudio::path &resourcesPath,
              openstudio::model::OptionalModel model = boost::none,
              QString filePath = QString(), bool isPlugin = false);

  virtual ~OSDocument();

  // Returns the main window associated with this document.
  MainWindow * mainWindow();

  // Returns the model associated with this document.
  model::Model model();

  // Sets the model associated with this document.
  // This will close all current windows, make sure to call app->setQuitOnLastWindowClosed(false) before calling this
  void setModel(const model::Model& model, bool modified);

  // Returns the RunManager for this document.
  runmanager::RunManager runManager();

  // Returns true if the document has unsaved changes.
  bool modified() const;

  // Returns the string path to the location where the document is saved.
  // If the document is unsaved an empty string will be returned.
  QString savePath() const;

  // Returns the path to the directory where model resources are stored
  QString modelTempDir() const;

  // Returns the component library associated with this document.
  openstudio::model::Model componentLibrary() const;

  // Sets the component library associated with this document.
  void setComponentLibrary(const openstudio::model::Model& model);

  // Returns the hvac component library associated with this document.
  openstudio::model::Model hvacComponentLibrary() const;

  // Sets the hvac component library associated with this document.
  void setHVACComponentLibrary(const openstudio::model::Model& model);

  // Returns a compined component library with hvac components
  openstudio::model::Model combinedComponentLibrary() const;

  // Returns true if OSItemId's source is the model
  bool fromModel(const OSItemId& itemId) const;

  // Returns true if OSItemId's source is the componentLibrary
  bool fromComponentLibrary(const OSItemId& itemId) const;

  // Returns true if OSItemId's source is the BCL
  bool fromBCL(const OSItemId& itemId) const;

  // Returns IddObjectType from either model, componentLibrary, or BCL
  boost::optional<IddObjectType> getIddObjectType(const OSItemId& itemId) const;

  // Returns the model object from either model or componentLibrary if possible
  // does not return model object from BCL
  boost::optional<model::ModelObject> getModelObject(const OSItemId& itemId) const;

  // Retrieves the Component identified by itemId from the local bcl library, 
  // updates it to the current version and returns it.
  boost::optional<model::Component> getComponent(const OSItemId& itemId) const;

  boost::optional<analysisdriver::SimpleProject> project() const;

  // Returns the ScriptFolderListView, from which folder display names and
  // ruleset::UserScriptInfo is available.
//  const ScriptFolderListView* scriptFolderListView() const;

//  ScriptFolderListView* scriptFolderListView();
  void openMeasuresBclDlg();

  int verticalTabIndex();

  enum VerticalTabID
  {
    SITE,
    SCHEDULES,
    CONSTRUCTIONS,
    LOADS,
    SPACE_TYPES,
    BUILDING_STORIES,
    FACILITY,
    THERMAL_ZONES,
    HVAC_SYSTEMS,
    BUILDING_SUMMARY,
    OUTPUT_VARIABLES,
    SIMULATION_SETTINGS,
    RUBY_SCRIPTS,
    RUN_SIMULATION,
    RESULTS_SUMMARY 
  };

  enum HorizontalTabID
  {
    MY_MODEL,
    LIBRARY,
    EDIT 
  };

  boost::shared_ptr<MainRightColumnController> mainRightColumnController() const;

  boost::shared_ptr<ApplyMeasureNowDialog> m_applyMeasureNowDialog;

 signals:

  void closeClicked();

  void importClicked();

  void importgbXMLClicked();

  void importSDDClicked();

  void loadFileClicked();

  void osmDropped(QString path);

  void loadLibraryClicked();

  void newClicked();

  void exitClicked();

  void helpClicked();

  void aboutClicked();

  void modelSaving(const openstudio::path &t_path);

  void downloadComponentsClicked();

  void openLibDlgClicked();

  void toolsUpdated();

  void toggleUnitsClicked(bool displayIP);

  void treeChanged(const openstudio::UUID &t_uuid);

  void enableRevertToSaved(bool enable);

 public slots:

  void markAsModified();

  void markAsUnmodified();

  void runComplete();

  void exportIdf();

  void exportgbXML();

  void exportSDD();

  void save();

  void saveAs();

  void revert();

  void showRunManagerPreferences();

  void scanForTools();

  void closeSidebar();

  void openSidebar();

  void openBclDlg();

  void openMeasuresDlg();

  void openChangeMeasuresDirDlg();

 private slots:

  void onVerticalTabSelected(int id); 

  void inspectModelObject(model::OptionalModelObject &, bool readOnly);

  void showFirstTab();

  void showTab(int tabIndex);
 
  void initializeModel();

  void toggleUnits(bool displayIP);

  void openLibDlg();
 
  void on_closeBclDlg();

  void on_closeMeasuresBclDlg();

  void changeBclLogin();

  void updateWindowFilePath();

 private:

  enum fileType{
    SDD,
    GBXML
  };

  void exportFile(fileType type);

  friend class OpenStudioApp;

  REGISTER_LOGGER("openstudio.OSDocument");

  // Sets the file path and updates the window title to match.
  // Used by save and saveAs.
  void setSavePath(const QString & savePath);

  // Attempts to make the model's weather file have a fully qualified path
  bool setFullWeatherFilePath();

  openstudio::model::Model m_model;

  openstudio::model::Model m_compLibrary;

  openstudio::model::Model m_hvacCompLibrary;

  openstudio::model::Model m_combinedCompLibrary;

  openstudio::path m_resourcesPath;

  LibraryTabWidget * m_libraryTabWidget;

  MainWindow * m_mainWindow;

  BuildingComponentDialog * m_onlineMeasuresBclDialog;

  BuildingComponentDialog * m_onlineBclDialog;

  BuildingComponentDialog * m_localLibraryDialog;

  //boost::shared_ptr<OSConsoleWidget> m_consoleWidget;

  boost::shared_ptr<HVACSystemsTabController> m_hvacSystemsTabController;

  boost::shared_ptr<ThermalZonesTabController> m_thermalZonesTabController;

  boost::shared_ptr<SchedulesTabController> m_schedulesTabController;

  boost::shared_ptr<InspectorController> m_inspectorController;

  boost::shared_ptr<LocationTabController> m_locationTabController;

  boost::shared_ptr<ConstructionsTabController> m_constructionsTabController;

  boost::shared_ptr<LoadsTabController> m_loadsTabController;

  boost::shared_ptr<SpaceTypesTabController> m_spaceTypesTabController;

  boost::shared_ptr<FacilityTabController> m_facilityTabController;

  boost::shared_ptr<SummaryTabController> m_summaryTabController;

  boost::shared_ptr<VariablesTabController> m_variablesTabController;

  boost::shared_ptr<SimSettingsTabController> m_simSettingsTabController;

  boost::shared_ptr<ScriptsTabController> m_scriptsTabController;

  boost::shared_ptr<RunTabController> m_runTabController;

  boost::shared_ptr<ResultsTabController> m_resultsTabController;

  boost::shared_ptr<BuildingStoriesTabController> m_buildingStoriesTabController;

  boost::shared_ptr<MainRightColumnController> m_mainRightColumnController;

  boost::optional<analysisdriver::SimpleProject> m_simpleProject;

  QString m_savePath;
  QString m_modelTempDir;

  int m_mainTabId;
  int m_subTabId;
  bool m_isPlugin;
};

} // openstudio

#endif // OPENSTUDIO_OSDOCUMENT_H

