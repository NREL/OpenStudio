/**********************************************************************
 *  Copyright (c) 2008-2012, Alliance for Sustainable Energy.  
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

#ifndef OPENSTUDIO_PATAPP_HPP
#define OPENSTUDIO_PATAPP_HPP

#include <QApplication>
#include <QObject>
#include <QPoint>
#include <QPointer>
#include <QSharedPointer>
#include <QWidget>

// TODO #include <pat_app/CloudDialog.hpp>

#include <analysisdriver/SimpleProject.hpp>

#include <model/Model.hpp>

#include "../shared_gui_components/BaseApp.hpp"
#include "../shared_gui_components/BuildingComponentDialog.hpp"
#include "../shared_gui_components/MeasureManager.hpp"
#include "../shared_gui_components/OSDialog.hpp"

#include <utilities/bcl/BCLMeasure.hpp>
#include <utilities/core/Path.hpp>
#include <utilities/core/Url.hpp>
#include <utilities/core/UUID.hpp>
#include <utilities/plot/ProgressBar.hpp>

#include <vector>
#include <map>

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

namespace pat {

class DesignAlternativesTabController;
class InspectorController;
class LibraryTabWidget;
class MainRightColumnController;
class PatMainWindow;
class MeasuresTabController;
class OSItemId;
class ResultsTabController;
class RunTabController;
class SystemComponent;
class MainWindow;
class AddToModelView;
class StartupView;
class CloudMonitor;

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

  // Request to save the current project, will prompt user as neccessary.
  // Will return true if the path can be opened.
  bool userInteractiveOpenProject(QString path);
  
  // Request to close the current project, will prompt user as neccessary.
  // Will return true if there is no current project.  Otherwise returns true
  // if the current project was closed.
  bool userInteractiveCloseProject();

  // Request to save the current project, will prompt user as neccessary.
  // Will return false if there is no current project to save.
  bool userInteractiveSaveProject(bool allowCancel);

  // Request to save the current project to a new location, will prompt user as neccessary.
  // Will return false if there is no current project to save.
  bool userInteractiveSaveAsProject();

  virtual QWidget *mainWidget(); 

  // The main window associated with this application.
  QPointer<PatMainWindow> mainWindow;

  QSharedPointer<MainRightColumnController> mainRightColumnController() const;

  MeasureManager &measureManager();

  const MeasureManager &measureManager() const;

  boost::optional<openstudio::model::Model> currentModel()
  {
    return boost::none; // Pat doesn't edit models
  }

  virtual void updateSelectedMeasureState();

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

  void quit();

  void open();

  void create();

  void clearAllResults();

  void exportXml();

  void openBclDlg();

  void on_closeBclDlg();

  void openCloudDlg();

  void on_closeCloudDlg();

  void showHelp();

  void showAbout();

  void addMeasure();

  void duplicateSelectedMeasure();

  void updateMyMeasures();

  void updateBCLMeasures();

  void downloadUpdatedBCLMeasures();

  void disableTabsDuringRun();

 private slots:

  void showVerticalTab(int id); 

  void showStartupView();

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
  QPointer<BuildingComponentDialog> m_onlineBclDialog;
  // TODO QPointer<CloudDialog> m_cloudDialog;

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

  NewProjectDialog(QWidget * parent = 0);

  QLineEdit * m_projectNameLineEdit;

protected:

  void mouseMoveEvent(QMouseEvent *event);

  void mousePressEvent(QMouseEvent *event);

private slots:

  void enableOkButton();

private:

  QPoint dragPosition;

  bool _move;

};

}

} // openstudio

#endif // OPENSTUDIO_PATAPP_H
