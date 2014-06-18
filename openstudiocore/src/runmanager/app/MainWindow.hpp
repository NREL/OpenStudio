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

#ifndef RUNMANAGER_APP_MAINWINDOW_HPP
#define RUNMANAGER_APP_MAINWINDOW_HPP

#include "AboutBox.hpp"
#include "FileSystemSearch.hpp"
#include <runmanager/app/ui_MainWindow.h>
#include "../lib/Job.hpp"
#include "../lib/JobStatusWidget.hpp"
#include "../lib/RunManager.hpp"
#include "../lib/RunManagerStatusWidget.hpp"
#include "../lib/Workflow.hpp"

#include <QCompleter>
#include <QFileSystemModel>
#include <QProgressBar>
#include <QTimer>

#include <boost/optional.hpp>

#include <string>


namespace openstudio {
namespace runmanager {

  class CompleterModel : public QFileSystemModel
  {
    public:
      virtual ~CompleterModel() {}
      QVariant data(const QModelIndex &index, int role) const;
  };


  class MainWindow : public QMainWindow
  {
    Q_OBJECT

  public:
    MainWindow(QWidget *parent = nullptr, Qt::WindowFlags flags = nullptr, bool newdb = false);
    virtual ~MainWindow();

  protected:
    virtual void closeEvent(QCloseEvent *event);

  private:
    REGISTER_LOGGER("openstudio.runmanager.MainWindow");


    class StatusBarProgress
    {
      public:
        StatusBarProgress(const QString &name, QStatusBar *bar);
        ~StatusBarProgress();
        void update(int min, int max, int current);
        void update(int value);

      private:
        QStatusBar *m_bar;
        QFrame *m_line;
        QLabel *m_label;
        QProgressBar *m_progress;
    };

    class StatusBarMessage
    {
      public:
        StatusBarMessage(const QString &value, QStatusBar *bar);
        ~StatusBarMessage();
        void update(const QString &value);

      private:
        QStatusBar *m_bar;
        QFrame *m_line;
        QLabel *m_label;
    };

    openstudio::runmanager::RunManager m_runmanager;
    
    Ui::MainWindow ui;
    openstudio::runmanager::RunManagerStatusWidget *m_statusWidget;
    openstudio::runmanager::JobStatusWidget *m_jobStatusWidget;
    QFileSystemModel m_fsmodel;
    FileSystemSearch m_fss;
    
    std::shared_ptr<StatusBarMessage> m_selectedMessage;
    std::shared_ptr<StatusBarMessage> m_regexMessage;

    QCompleter m_completer;

    void updateFileSelection(const QString &t_file, QComboBox &t_qcb);
    void openFile(const QString &p);

  private slots:
    void filesSelectionChanged();
    void epwBrowse();
    void runSimulation();
    void helpAbout();
    void on_actionHtmlResults_triggered();
    void goToOutput();
    void helpDocumentationOnline();
    void restartOutOfDate();
    void showStatistics();
    void dirActivated(const QModelIndex &index);
    void regexStateChanged(int state);
    void recursiveStateChanged(int state);
    void searchPatternChanged(const QString &);
    void errorInRegex(const QString &);
    void dirExpanded(const QModelIndex &);
    void txtRootPathChanged(const QString &txt);
    void rootPathChanged(const openstudio::path &);
    void fileSearchComplete();
    void fileSearchStarted();
    void fileTypeChanged(bool);
    void setConfigOptions();
    void readSettings();
    void settingsFileInfo();

    void openFolder();
    void searchFolder();

    void filesCheckAll();
    void filesCheckNone();
    void filesCheckSelected();
    void filesInvertChecked();

    void filesAddToQueue();
    void filesOpenFolder();
    void filesAddCheckedToQueue();

    void queueSimulation(const openstudio::path &t_input, const openstudio::path &t_epw);
    void queueSimulations(const std::vector<openstudio::path> &t_files);

    void updateToolbar(int index);

    //void queueFiles(const std::vector<openstudio::path> &t_files);

    // create an intelligently named output directory based on the input file path 
    openstudio::path getOutDir(const openstudio::path &t_file, const openstudio::runmanager::Workflow &t_wf, const std::string &t_jobname,
        bool t_simplename);


    /// enable / disable the weather UI components
    void enableWeather(bool enabled);
 
  };

} // runmanager namespace
} // openstudio

#endif // RUNMANAGER_APP_MAINWINDOW_HPP
