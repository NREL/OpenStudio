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
  class RubyUserScriptArgumentGetter;
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

  PatApp( int & argc, char ** argv, const QSharedPointer<ruleset::RubyUserScriptArgumentGetter> &t_argumentGetter );

  virtual ~PatApp();

  static PatApp * instance();

  openstudio::path resourcesPath() const; 

  virtual bool notify(QObject* receiver, QEvent* event);

  boost::optional<analysisdriver::SimpleProject> project() {return m_project;}

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

  virtual QWidget *mainWidget(); 

  // The main window associated with this application.
  QPointer<PatMainWindow> mainWindow;

  QSharedPointer<MainRightColumnController> mainRightColumnController() const;

  QSharedPointer<DesignAlternativesTabController> designAlternativesTabController() const;

  QSharedPointer<MeasuresTabController> measuresTabController() const;

  QSharedPointer<ResultsTabController> resultsTabController() const;

  QSharedPointer<RunTabController> runTabController() const;

  MeasureManager &measureManager();

  const MeasureManager &measureManager() const;

  boost::optional<openstudio::model::Model> currentModel()
  {
    return boost::none; // Pat doesn't edit models
  }

  virtual void updateSelectedMeasureState();

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

  virtual void chooseHorizontalEditTab();

  virtual QSharedPointer<EditController> editController();

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

  void openBclDlg();

  void on_closeBclDlg();

  void openCloudDlg();

  void on_closeCloudDlg();

  void openMonitorUseDlg();

  void on_closeMonitorUseDlg();

  void showHelp();

  void showAbout();

  void addMeasure();

  void duplicateSelectedMeasure();

  void updateMyMeasures();

  void updateBCLMeasures();

  void downloadUpdatedBCLMeasures();

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
