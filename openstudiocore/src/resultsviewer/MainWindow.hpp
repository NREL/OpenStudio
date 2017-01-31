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

#ifndef RESULTSVIEWER_MAINWINDOW_HPP
#define RESULTSVIEWER_MAINWINDOW_HPP

#include "TreeView.hpp"
#include "TableView.hpp"
#include "PlotView.hpp"
#include "ResultsViewerData.hpp"
#include "BrowserView.hpp"
#include "TabDropDock.hpp"
#include "FloodPlot.hpp"
#include "LinePlot.hpp"

#include "../utilities/core/TemporaryDirectory.hpp"
#include "../utilities/sql/SqlFile.hpp"
#include "../utilities/data/TimeSeries.hpp"

#include <QMainWindow>
#include <QTabWidget>
#include <QLineEdit>
#include <QTreeWidgetItem>
#include <QList>
#include <QSettings>
#include <QMessageBox>
#include <QAction>
#include <QMenu>
#include <QDockWidget>
#include <string>
#include <ui_MainWindow.h>

// forward declarations to minimize header files
class QToolBar;
class QComboBox;
class QTemporaryFile;

namespace resultsviewer{


/**
MainWindow is the ui container for the ResultsViewer application.
*/
class MainWindow : public QMainWindow
{
  Q_OBJECT

static const int MaxRecentFiles = 10;

public:
  MainWindow(QWidget *parent = nullptr, Qt::WindowFlags flags = nullptr);
  virtual ~MainWindow();

private:
  REGISTER_LOGGER("resultsviewer.MainWindow");

  // list of files to be auto cleaned up when the process exits
  std::list<std::shared_ptr<openstudio::TemporaryDirectory> > m_temporaryDirectories;

  Ui::MainWindowClass ui;

  QAction *m_separatorAction;


  // plotting
  QList<resultsviewer::PlotView *> m_plotViewList;

  int m_plotTitleNumber;
  PlotViewData plotViewDataFromResultsViewerPlotData(const resultsviewer::ResultsViewerPlotData &rvplotData);
  PlotViewData plotViewDataDifference(const resultsviewer::PlotViewData &plotViewData1, const resultsviewer::PlotViewData &plotViewData2);

  // recent file list
  QStringList m_recentFiles;
  QStringList m_recentAliases;
  void updateRecentFileActions();
  QAction *m_recentFileActions[MaxRecentFiles];

  // last path opened
  QString m_lastPathOpened;
  // image file path trac #254
  QString m_lastImageSavedPath;

  void readSettings();
  void writeSettings();

  // main widgets
  TableView *m_tableView;
  TreeView *m_treeView;
  QTabWidget *m_mainTabView;
  TabDropDock *m_mainTabDock;
  QToolBar *m_plotToolBar;
  QComboBox *m_plotComboBox;
  QToolBar *m_fileToolBar;
  QComboBox *m_fileComboBox;
  QLineEdit *m_viewFilterText;

  // data
  ResultsViewerData *m_data;

  // close open file
  void closeFile(const QString& filename);
  QAction *m_fileCloseAction;
  QAction *m_fileCloseAllAction;

  // uses system association to open a folder
  void openContainingFolder(const QString& filename);

  // toolbar for tracking open plot windows
  void createPlotToolBar();

  // toolbar for tracking open files
  void createFileToolBar();

  // create table view for displaying report variables and metrics
  void createTableView();

  // create tree view for displaying report variables and metrics
  void createTreeView();

  // alias from recent files map
  const QString recentFilesAlias(const QString& filename);

  // ABUPS
  void addEPlusHTML(const QString& filename);
  QList<resultsviewer::BrowserView *> m_browserList;
  int currentEPlusHTML(const QString& filename);
  void showEPlusHTML(const QString& filename);

  // close plot at index (combo box index)
  void closeSelectedPlot(int index);

  // float plot at index (combo box index)
  void floatSelectedPlot(int index);

  // create welcome page
  void createWelcomePage();

  // open a list of files from dialog or command line
  // \param[in] t_makeTempCopies if true, copy the files into a new temp
  //            location before opening and delete them when the process exits.
  void openFileList(const QStringList& fileList, bool t_makeTempCopies);

  // timeseries specific context menus
  void timeseriesTableViewMenu( QMenu& menu);
  void timeseriesTreeViewMenu( QMenu& menu);

  // illuminanceMap specific context menus
  void illuminanceMapTableViewMenu( QMenu& menu);
  void illuminanceMapTreeViewMenu( QMenu& menu);

  // trac #1182 - new flood plot and new line plot
  bool treeViewSelectedItemsCanBePlotted(int &timeseriesItemCount, int &illuminanceMapItemCount);
  bool tableViewSelectedItemsCanBePlotted(int &timeseriesItemCount, int &illuminanceMapItemCount);


signals:
  // to repopulate tree when file closed
  void tableWidgetItemClicked(QTableWidgetItem *item);
  // add plot
  void signalAddPlot(resultsviewer::PlotView *plot);

private slots:
  void fileOpen();
  bool openRecentFile();
  void closeEvent(QCloseEvent *evt) override;
  void showTreeViewContextMenu(const QPoint &pos);
  void showTableViewContextMenu(const QPoint &pos);
  void loadFile(const QString& alias, const QString& filename);
  // limit items that can be selected
  void onTreeViewSelectionChanged();
  // file toolbar
  void closeSelectedFiles();
  void slotCloseAllFiles();
  void openContainingFolder();
  void selectNextFile();
  void selectPreviousFile();
  // file combo box
  void slotSelectedFileChanged(int index);
  // plot toolbar - based on m_plotComboBox drop down list
  void showPreviousPlot();
  void showNextPlot();
  void closePlot();
  void closeAllPlots();
  // plot combo box
  void onSelectedPlotChanged(int index);
  // add plot
  void slotAddPlot(resultsviewer::PlotView *plot);
  // help slots
  void slotHelpAbout();
  // command line arguments
  void slotParseCommandLine();
  // clear RecentFiles
  void slotClearRecentFiles();
  // clear all settings
  void slotClearSettings();
  // set to default layout
  void slotDefaultLayout();
  // plotting with timeseries caching
  void slotAddFloodPlot(const std::vector<resultsviewer::ResultsViewerPlotData> &fpVec);
  void slotAddLinePlot(const std::vector<resultsviewer::ResultsViewerPlotData> &lpVec);
  void slotAddIlluminancePlot(const std::vector<resultsviewer::ResultsViewerPlotData> &ipVec);
  void slotAddFloodPlotComparison(const std::vector<resultsviewer::ResultsViewerPlotData> &fpVec);
  void slotAddLinePlotComparison(const std::vector<resultsviewer::ResultsViewerPlotData> &lpVec);
  void slotAddIlluminancePlotComparison(const std::vector<resultsviewer::ResultsViewerPlotData> &ipVec);
  void slotDragPlotViewData(const std::vector<resultsviewer::ResultsViewerPlotData> &rvplotData);
  // track closed plots
  void slotClosePlot(QString& title);
  // track closed plots
  void slotCloseBrowser(resultsviewer::BrowserView *browser);
  // float and dock
  void slotFloatPlot();
  void slotFloatOrDockPlot(resultsviewer::PlotView *plotView);
  // close tabs in main tabbed window
  void slotCloseTab(int index);
  // float or dock browser window
  void floatOrDockBrowser(resultsviewer::BrowserView *browser);
  // main filtering - apply or clear
  void slotClearFilter();
  void slotApplyFilter(const QString& text);
  // alias updating
  void slotUpdateAlias();
  // image file saving path trac #254
  void slotUpdateLastImageSavedPath(QString &path);
  // double clicking
  void slotTableViewDoubleClick(QTableWidgetItem *item);
  void slotTreeViewDoubleClick(QTreeWidgetItem *item, int col);

  // trac #1182 - new flood plot and new line plot
  void slotNewLinePlot();
  void slotNewFloodPlot();
};


}; // resultsviewer namespace

#endif // RESULTSVIEWER_MAINWINDOW_HPP
