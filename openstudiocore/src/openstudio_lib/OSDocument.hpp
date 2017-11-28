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

#ifndef OPENSTUDIO_OSDOCUMENT_HPP
#define OPENSTUDIO_OSDOCUMENT_HPP

#include "OpenStudioAPI.hpp"

#include "../shared_gui_components/OSQObjectController.hpp"

#include "../model/Model.hpp"
#include "../model/ModelObject.hpp"

#include <QObject>
#include <QString>

#include <boost/smart_ptr.hpp>

class QDir;

namespace openstudio {

namespace runmanager {
  class RunManager;
}
class MainTabController;

class MainRightColumnController;

class InspectorController;

class MainWindow;

class LibraryTabWidget;

class OSItemId;

class BuildingComponentDialog;

class ApplyMeasureNowDialog;

class Workspace;

class OPENSTUDIO_API OSDocument : public OSQObjectController {
  Q_OBJECT

  public:

  OSDocument( openstudio::model::Model library,
              openstudio::model::Model hvacLibrary,
              const openstudio::path &resourcesPath,
              openstudio::model::OptionalModel model = boost::none,
              QString filePath = QString(), 
              bool isPlugin = false, 
              int startTabIndex = 0, 
              int startSubTabIndex = 0);

  virtual ~OSDocument();

  // Returns the main window associated with this document.
  MainWindow * mainWindow();

  // Returns the model associated with this document.
  model::Model model();

  // Sets the model associated with this document.
  // This will close all current windows, make sure to call app->setQuitOnLastWindowClosed(false) before calling this
  void setModel(const model::Model& model, bool modified, bool saveCurrentTabs);

  // Returns the Workspace associated with this document's model
  //boost::optional<Workspace> workspace();

  // Set the Workspace associated with this document's model.
  // Workspace is created by idf translator when the scripts tab is shown.
  // This is used to populate idf measure arguments.
  //void setWorkspace(const boost::optional<Workspace>& workspace);

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

  // Returns a compiled component library with hvac components
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

  // Returns the index of the current tab.
  int verticalTabIndex();

  // Returns the index of the current sub tab.
  // Returns -1 if there are no sub tabs.
  int subTabIndex();

  enum VerticalTabID
  {
    SITE,
    SCHEDULES,
    CONSTRUCTIONS,
    LOADS,
    SPACE_TYPES,
    GEOMETRY,
    FACILITY,
    SPACES,
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

  std::shared_ptr<MainRightColumnController> mainRightColumnController() const;

  // DLM: would like for this to not be a member variable since it is only used as a modal dialog with a well defined lifetime
  boost::shared_ptr<ApplyMeasureNowDialog> m_applyMeasureNowDialog;

 signals:

  void closeClicked();

  void importClicked();

  void importgbXMLClicked();

  void importSDDClicked();

  void importIFCClicked();

  void loadFileClicked();

  void osmDropped(QString path);

  void loadLibraryClicked();

  void newClicked();

  void exitClicked();

  void helpClicked();

  void aboutClicked();

  // called before actual save (copy from temp to user location) occurs
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

  void exportIdf();

  void exportgbXML();

  void exportSDD();

  // returns if a file was saved
  bool save();

  // returns if a file was saved
  bool saveAs();

  void showRunManagerPreferences();

  void scanForTools();

  void closeSidebar();

  void openSidebar();

  void openBclDlg();

  void openMeasuresBclDlg();

  void openMeasuresDlg();

  void openChangeMeasuresDirDlg();

 private slots:

  void onVerticalTabSelected(int id); 

  void inspectModelObject(model::OptionalModelObject &, bool readOnly);

  void showStartTabAndStartSubTab();

  void initializeModel();

  void toggleUnits(bool displayIP);

  void openLibDlg();
 
  void on_closeBclDlg();

  void on_closeMeasuresBclDlg();

  void changeBclLogin();

  void updateWindowFilePath();

  void updateSubTabSelected(int id);

  void addStandardMeasures();

  public slots:

  void enable();

  void disable();

  void enableTabsAfterRun();

  void disableTabsDuringRun();

  void weatherFileReset();

 private:

  enum fileType{
    SDD,
    GBXML
  };

  void exportFile(fileType type);

  boost::optional<BCLMeasure> standardReportMeasure();

  friend class OpenStudioApp;

  REGISTER_LOGGER("openstudio.OSDocument");

  // Sets the file path and updates the window title to match.
  // Used by save and saveAs.
  void setSavePath(const QString & savePath);

  // When opening an OSM, check the model for a weather file, if a weather file is listed
  // copy it into the temp directory.  If the listed weather file cannot be found, remove the 
  // weather file object.  Returns false if the user's weather file is reset, returns true otherwise.
  bool fixWeatherFileInTemp(bool opening);

  void createTab(int verticalId);

  void createTabButtons();

  openstudio::model::Model m_model;

  boost::optional<Workspace> m_workspace;

  openstudio::model::Model m_compLibrary;

  openstudio::model::Model m_hvacCompLibrary;

  openstudio::model::Model m_combinedCompLibrary;

  openstudio::path m_resourcesPath;

  LibraryTabWidget * m_libraryTabWidget;

  MainWindow * m_mainWindow;

  BuildingComponentDialog * m_onlineMeasuresBclDialog;

  BuildingComponentDialog * m_onlineBclDialog;

  BuildingComponentDialog * m_localLibraryDialog;

  //std::shared_ptr<OSConsoleWidget> m_consoleWidget;

  std::shared_ptr<MainTabController> m_mainTabController;

  std::shared_ptr<InspectorController> m_inspectorController;

  std::shared_ptr<MainRightColumnController> m_mainRightColumnController;

  QString m_savePath = QString();
  QString m_modelTempDir = QString();

  int m_mainTabId = 0;
  int m_subTabId = 0;
  bool m_isPlugin;

  int m_startTabIndex;
  int m_startSubTabIndex;

  int m_verticalId;
  std::vector<int> m_subTabIds;

  bool m_enableTabsAfterRun = true;

  bool m_tabButtonsCreated = false;

};

} // openstudio

#endif // OPENSTUDIO_OSDOCUMENT_HPP
