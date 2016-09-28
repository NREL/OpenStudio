/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2016, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#ifndef PATAPP_PATAPP_HPP
#define PATAPP_PATAPP_HPP

#include <QApplication>
#include <QObject>
#include <QPoint>
#include <QPointer>
#include <QSharedPointer>
#include <QWidget>

#include "../analysisdriver/SimpleProject.hpp"
#include "../analysisdriver/AnalysisDriverEnums.hpp"

#include "../model/Model.hpp"
#include "../utilities/idf/Workspace.hpp"

#include "../shared_gui_components/BaseApp.hpp"
#include "../shared_gui_components/BuildingComponentDialog.hpp"
#include "../shared_gui_components/MeasureManager.hpp"
#include "../shared_gui_components/OSDialog.hpp"
#include "PatConstants.hpp"
#include "CloudDialog.hpp"
#include "MonitorUseDialog.hpp"

#include "../utilities/bcl/BCLMeasure.hpp"
#include "../utilities/core/Path.hpp"
#include "../utilities/core/Url.hpp"
#include "../utilities/core/UUID.hpp"
#include "../utilities/plot/ProgressBar.hpp"

#include <map>
#include <vector>

class QEvent;

namespace openstudio {

class BCLMeasure;
class LocalLibraryController;
class CloudProvider;

namespace osversion {

  class VersionTranslator;

}

namespace ruleset {
  class RubyUserScriptInfoGetter;
}

namespace analysisdriver {
  class AnalysisStatus;
}

namespace pat {

class AddToModelView;
class CloudMonitor;
class DesignAlternativesTabController;
class InspectorController;
class LibraryTabWidget;
class MainRightColumnController;
class MainWindow;
class MeasuresTabController;
class OSItemId;
class PatMainWindow;
class ResultsTabController;
class RunTabController;
class StartupView;
class LoadingProjectView;
class SystemComponent;

class PatApp : public QApplication, public BaseApp
{

  Q_OBJECT

 public:

  PatApp( int & argc, char ** argv, const QSharedPointer<ruleset::RubyUserScriptInfoGetter> &t_infoGetter );

  virtual ~PatApp();

  static PatApp * instance();

  openstudio::path resourcesPath() const; 

  virtual bool notify(QObject* receiver, QEvent* event) override;

  boost::optional<analysisdriver::SimpleProject> project() override {return m_project;}

  QSharedPointer<CloudMonitor> cloudMonitor() const;

  // ensure that we set the seed on this project not a copy, will also cached the seed models
  bool setSeed(const FileReference& currentSeedLocation);

  // Request to save the current project, will prompt user as necessary.
  // Will return true if the path can be opened.
  bool userInteractiveOpenProject(QString path);
  
  // Request to close the current project, will prompt user as necessary.
  // Will return true if there is no current project.  Otherwise returns true
  // if the current project was closed.
  bool userInteractiveCloseProject();

  // Request to save the current project, will prompt user as necessary.
  // Will return false if there is no current project to save.
  bool userInteractiveSaveProject(bool allowCancel);

  // Request to save the current project to a new location, will prompt user as necessary.
  // Will return false if there is no current project to save.
  bool userInteractiveSaveAsProject();

  virtual QWidget *mainWidget() override; 

  // The main window associated with this application.
  QPointer<PatMainWindow> mainWindow;

  QSharedPointer<MainRightColumnController> mainRightColumnController() const;

  QSharedPointer<DesignAlternativesTabController> designAlternativesTabController() const;

  QSharedPointer<MeasuresTabController> measuresTabController() const;

  QSharedPointer<ResultsTabController> resultsTabController() const;

  QSharedPointer<RunTabController> runTabController() const;

  MeasureManager &measureManager() override;

  const MeasureManager &measureManager() const;

  boost::optional<openstudio::model::Model> currentModel() override
  {
    return boost::none; // Pat doesn't edit models
  }

  boost::optional<openstudio::Workspace> currentWorkspace() override
  {
    return boost::none; // Pat doesn't edit models
  }

  virtual void updateSelectedMeasureState() override;

  // The settings associated with the current user
  // independent of a particular project.
  // These settings will be used for new cloud instances
  static CloudSettings cloudSettings();

  enum VerticalTabID
  {
    MEASURES,
    DESIGN_ALTERNATIVES,
    RUN,
    RESULTS 
  };

  enum HorizontalTabID
  {
    MY_MODEL,
    LIBRARY,
    EDIT 
  };

  virtual void chooseHorizontalEditTab() override;

  virtual QSharedPointer<EditController> editController() override;

 signals:

  void userMeasuresDirChanged();

 public slots:

  void save();

  void saveAs();

  void quit(bool fromCloseEvent = false);

  void open();

  void create();

  void clearAllResults();

  void exportXml();

  void exportSpreadsheet();

  void openBclDlg() override;

  void on_closeBclDlg();

  void openCloudDlg();

  void on_closeCloudDlg();

  void openMonitorUseDlg();

  void on_closeMonitorUseDlg();

  void showHelp();

  void showAbout();

  void addMeasure() override;

  void duplicateSelectedMeasure() override;

  void updateMyMeasures() override;

  void updateBCLMeasures() override;

  void downloadUpdatedBCLMeasures() override;

  // Consider removing this in favor of setAppState()
  void disableTabsDuringRun();

 private slots:

  void setAppState(const CloudStatus & cloudStatus, const analysisdriver::AnalysisStatus & analysisStatus);

  // Calls setAppState with current cloud and analysis status
  void updateAppState();

  void onCloudStatusChanged(const CloudStatus & newStatus);

  void onAnalysisStatusChanged(analysisdriver::AnalysisStatus newAnalysisStatus);

  void showVerticalTab(int id); 

  void showStartupView();

  void showLoadingProjectView();

  void scanForTools();

  void showTools();

  void changeUserMeasuresDir();

  void markAsModified();

  void markAsUnmodified();

  void queueAnalysisChanged();

  void analysisChanged();

  void analysisSeedChanged();

 private:

  REGISTER_LOGGER("openstudio.pat.PatApp");

  bool openFile(const QString& fileName);

  void attachProject(boost::optional<analysisdriver::SimpleProject> project);

  QPointer<StartupView> m_startupView;
  QPointer<LoadingProjectView> m_loadingProjectView;
  QPointer<BuildingComponentDialog> m_onlineBclDialog;
  QPointer<CloudDialog> m_cloudDialog;
  QPointer<MonitorUseDialog> m_monitorUseDialog;

  QSharedPointer<DesignAlternativesTabController> m_designAlternativesTabController;
  QSharedPointer<MainRightColumnController> m_mainRightColumnController;
  QSharedPointer<MeasuresTabController> m_measuresTabController;
  QSharedPointer<ResultsTabController> m_resultsTabController;
  QSharedPointer<RunTabController> m_runTabController;

  boost::optional<analysisdriver::SimpleProject> m_project;

  QSharedPointer<CloudMonitor> m_cloudMonitor;

  MeasureManager m_measureManager;

  int m_mainTabId;
};

class NewProjectDialog : public OSDialog
{
  Q_OBJECT

public:

  NewProjectDialog(QWidget * parent = nullptr);

  QLineEdit * m_projectNameLineEdit;

private slots:

  void enableOkButton();

};

}

} // openstudio

#endif // PATAPP_PATAPP_HPP
