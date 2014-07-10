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

#include "MainWindow.hpp"
#include <resultsviewer/AboutBox.hpp>
#include "ChangeAliasDialog.hpp"

#include "../utilities/core/String.hpp"
#include "../utilities/core/TemporaryDirectory.hpp"
#include "../utilities/sql/SqlFileEnums.hpp"

#include <QComboBox>
#include <QDesktopServices>
#include <QDrag>
#include <QProcess>
#include <QProgressDialog>
#include <QSplitter>
#include <QTemporaryFile>
#include <QTimer>
#include <QToolBar>
#include <QUrl>

using openstudio::ReportingFrequency;
using openstudio::toPath;
using openstudio::toQString;

namespace resultsviewer{

  MainWindow::MainWindow(QWidget *parent, Qt::WindowFlags flags)
    : QMainWindow(parent, flags)
  {
    // plot number used when plots are created - keeps track of max number created
    m_plotTitleNumber = 0;


    // from ui_Mainwindow Qt Designer
    ui.setupUi(this);

#ifdef Q_OS_LINUX
    QIcon icon = QIcon(":/images/rv_16.png");
    icon.addPixmap(QPixmap(":/images/rv_32.png"));
    icon.addPixmap(QPixmap(":/images/rv_48.png"));
    icon.addPixmap(QPixmap(":/images/rv_64.png"));
    icon.addPixmap(QPixmap(":/images/rv_128.png"));
    icon.addPixmap(QPixmap(":/images/rv_256.png"));
    setWindowIcon(icon);
#endif

    connect(ui.actionFileOpen, SIGNAL(triggered()), this, SLOT(fileOpen()));
    connect(ui.actionReset_Layout, SIGNAL(triggered()), this, SLOT(slotDefaultLayout()));
    connect(ui.actionClear_Recent_Files, SIGNAL(triggered()), this, SLOT(slotClearRecentFiles()));
    connect(ui.actionClear_Settings, SIGNAL(triggered()), this, SLOT(slotClearSettings()));
    connect(ui.actionAbout, SIGNAL(triggered()), this, SLOT(slotHelpAbout()));

    // file close
    /*
    m_fileCloseAction = new QAction(tr("&Close"), this);
    m_fileCloseAction->setToolTip("Close selected file.");
    connect(m_fileCloseAction, SIGNAL(triggered()), this, SLOT(closeSelectedFiles()));
    ui.menuFile->addAction(m_fileCloseAction);
    */
    m_fileCloseAllAction = new QAction(tr("Close &All"), this);
    m_fileCloseAllAction->setToolTip("Close all files.");
    connect(m_fileCloseAllAction, SIGNAL(triggered()), this, SLOT(slotCloseAllFiles()));
    ui.menuFile->addAction(m_fileCloseAllAction);

    // recent files
    m_separatorAction = ui.menuFile->addSeparator(); // for showing and hiding
    for (auto & elem : m_recentFileActions) {
      elem = new QAction(this);
      elem->setVisible(false);
      ui.menuFile->addAction(elem);
      connect(elem, SIGNAL(triggered()), this, SLOT(openRecentFile()));
    }

    ui.menuFile->addSeparator();

    // application exit
    QAction *exitAction = new QAction(tr("E&xit"), this);
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));
    ui.menuFile->addAction(exitAction);

    // Data manager
    m_data = new resultsviewer::ResultsViewerData();

    // File toolbar
    createFileToolBar();

    // Plot toolbar
    createPlotToolBar();

    // Table View
    createTableView();

    // Tree View
    createTreeView();

    // main tab with table and tree views and filtering
    m_mainTabView = new QTabWidget(this);
    m_mainTabView->setObjectName("MainTabView");
    m_mainTabView->setMovable(true);
    m_tableView->setFrameStyle(QFrame::NoFrame);
    m_mainTabView->addTab(m_tableView, "Table View");
    m_treeView->setFrameStyle(QFrame::NoFrame);
    m_mainTabView->addTab(m_treeView, "Tree View");

    // main docking area for plots and eplustbl
    m_mainTabDock = new resultsviewer::TabDropDock(this);
    m_mainTabDock->setObjectName("MainTabDock");
    m_mainTabDock->setTabsClosable(true);
    // m_mainTabDock->setMovable(true);
    connect( m_mainTabDock, SIGNAL( tabCloseRequested ( int ) ), this, SLOT( slotCloseTab( int ) ) );

    // filter data for searching views
    m_viewFilterText = new QLineEdit("", this);

    // remove central layout created by Qt Designer
    delete ui.verticalLayout;
    delete ui.centralWidget;

    // default layout - dockable items
    slotDefaultLayout();

    // hide toolbars
    //m_fileToolBar->hide();
    //m_plotToolBar->hide();


    // default page to appear in mainDock - information, description, etc. - like VS Welcome
    createWelcomePage();

    // setup main views
    QLabel *filterLabel = new QLabel("Filter Criteria", this);
    //QPushButton *btnApplyFilter = new QPushButton("Apply", this);
    QPushButton *btnClearFilter = new QPushButton("Clear", this);
    //connect(btnApplyFilter, SIGNAL(clicked()),this,SLOT(slotApplyFilter()));
    connect(m_viewFilterText, SIGNAL(textChanged(const QString&)), this, SLOT(slotApplyFilter(const QString&)));
    connect(btnClearFilter, SIGNAL(clicked()),this,SLOT(slotClearFilter()));
    auto filterButtonLayout = new QHBoxLayout;
    filterButtonLayout->addWidget(m_viewFilterText);
    //filterButtonLayout->addWidget(btnApplyFilter);
    filterButtonLayout->addWidget(btnClearFilter);
    auto viewLayout = new QVBoxLayout;
    viewLayout->addWidget(filterLabel);
    viewLayout->addLayout(filterButtonLayout);
    viewLayout->addWidget(m_mainTabView);

    auto mainSplitter = new QSplitter(this);
    mainSplitter->setObjectName("MainSplitter");
    setCentralWidget(mainSplitter);
    auto mainWidget = new QWidget(this);
    mainWidget->setLayout(viewLayout);
    mainSplitter->addWidget(mainWidget);
    mainSplitter->addWidget(m_mainTabDock);

    // default path when opening new files - updated in settings
    m_lastPathOpened = QApplication::applicationDirPath();
    m_lastImageSavedPath = m_lastPathOpened;
    readSettings();

    // parse command line - timer used to keep window responsive if several files to load
    QTimer::singleShot(0, this, SLOT(slotParseCommandLine()));

    // Mac toolbar integration
    //    QMainWindow::setUnifiedTitleAndToolBarOnMac(true);
  }

  MainWindow::~MainWindow()
  {
  }


  void MainWindow::slotSelectedFileChanged(int index)
  {
    QString filename = m_fileComboBox->itemData(index,Qt::ToolTipRole).toString();
    m_treeView->goToFile(filename);
    m_tableView->goToFile(filename);
    // update eplusTbl if present
    showEPlusHTML(filename);
  }

  void MainWindow::showEPlusHTML(const QString& filename)
  {
    int index = currentEPlusHTML(filename);
    if (index > - 1)
    {
      if (m_browserList.at(index)->parent() == nullptr)
      {
        m_browserList.at(index)->show();
        m_browserList.at(index)->raise();
        m_browserList.at(index)->activateWindow();
      }
      else
      {
        m_mainTabDock->setCurrentWidget(m_browserList.at(index));
      }
    }
  }


  void MainWindow::slotUpdateAlias()
  {
    int index = m_fileComboBox->currentIndex();
    if (index > -1)
    {
      QString oldAlias = m_fileComboBox->itemText(index);
      QString filename = m_fileComboBox->itemData(index,Qt::ToolTipRole).toString();

      ChangeAliasDialog dialog(oldAlias, filename, this);

      if (dialog.exec())
      {
        QString newAlias = dialog.newAlias();
        if (!newAlias.isEmpty())
        {
          // update view widgets
          m_treeView->updateFileAlias(newAlias, filename);
          m_tableView->updateFileAlias(newAlias, filename);
          // update file list
          m_fileComboBox->setItemText(index,newAlias);
          // update recentFiles
          int i = m_recentFiles.indexOf(filename);
          m_recentAliases[i] = newAlias;
          // m_recentFiles[filename] = newAlias;
          updateRecentFileActions();
          // update resultsviewerdata
          m_data->updateAlias(newAlias, filename);
          // update plots
          for (int i=0; i<m_plotViewList.count(); i++)
            m_plotViewList[i]->updateAlias(newAlias, filename);
          // update eplustbl
          int browserIndex = currentEPlusHTML(filename);
          if (browserIndex > - 1)
          {
            m_browserList[browserIndex]->updateAlias(newAlias, filename);
            // tabs are not updating with window title
            int tabIndex = m_mainTabDock->indexOf(m_browserList[browserIndex]);
            if (tabIndex > -1) m_mainTabDock->setTabText(tabIndex, m_browserList[browserIndex]->windowTitle());
          }
        }
      }
    }
  }

  void MainWindow::fileOpen()
  {
    QStringList fileList = QFileDialog::getOpenFileNames(this ,tr("Open SQLite Database"), m_lastPathOpened,tr("SQLite files (*.sql)"));
    openFileList(fileList, false);
  }

  void MainWindow::loadFile(const QString& alias, const QString& filename)
  {
    int index;
    switch(m_data->addFile( alias, filename))
    {
    case RVD_UNSUPPORTEDFILEFORMAT:
      QMessageBox::information(this, tr("File Open"), tr("Unsupported file format."));
      break;
    case RVD_FILEALREADYOPENED:
      QMessageBox::information(this, tr("File Open"), tr("File already opened."));
      break;
    case RVD_FILEDOESNOTEXIST:
      QMessageBox::information(this, tr("File Open"), tr("File not found:\n" + filename.toUtf8()));
      break;
    case RVD_UNSUPPORTEDVERSION:
      QMessageBox::information(this, tr("File Open"), tr("Unsupported EnergyPlus version. Continuing, unknown errors may occur."));
    case RVD_SUCCESS:
      m_treeView->displayFile(alias,  m_data->sqlFile(filename), resultsviewer::TreeView::tvdtVariableName);

      index = m_fileComboBox->count();
      m_fileComboBox->addItem(alias);
      m_fileComboBox->setItemData(index,filename,Qt::ToolTipRole);
      m_fileComboBox->setCurrentIndex(index);

      m_tableView->addFile(alias, m_data->sqlFile(filename));

      m_lastPathOpened = QFileInfo(filename).absoluteFilePath();

      index = m_recentFiles.indexOf(filename);
      if (index>-1)
      {
        m_recentFiles.removeAt(index);
        m_recentAliases.removeAt(index);
      }
      m_recentFiles.prepend(filename);
      m_recentAliases.prepend(alias);
      updateRecentFileActions();

      addEPlusHTML(filename);

      break;
    default:
      QMessageBox::information(this, tr("File Open"), tr("File open failed."));
    }
  }

  void MainWindow::slotCloseTab(int index)
  {
    m_mainTabDock->widget(index)->close();
  }

  int MainWindow::currentEPlusHTML(const QString& filename)
  {
    int browser = -1;
    if (!filename.isEmpty())
    {
      for (int i=0; i<m_browserList.count(); i++)
      {
        if (m_browserList[i]->filename().toUpper() == filename.toUpper())
        {
          browser = i;
          break;
        }
      }
    }
    return browser;
  }


  void MainWindow::addEPlusHTML(const QString& filename)
  {
    QFileInfo filePath(filename);
    QString folder = QFileInfo(filename).absolutePath();
    QString abups = folder + "/eplustbl.htm";
    if (QFile::exists(abups))
    {
      // text browser for ABUPS
      auto browser = new resultsviewer::BrowserView(this);
      connect(browser,SIGNAL(signalClose(resultsviewer::BrowserView*)), this, SLOT(slotCloseBrowser(resultsviewer::BrowserView *)));
      connect(browser,SIGNAL(signalFloatOrDockMe(resultsviewer::BrowserView*)), this, SLOT(floatOrDockBrowser(resultsviewer::BrowserView *)));
      browser->setSource(QUrl::fromLocalFile(abups));
      browser->setFilename(filename);
      browser->setAlias(m_data->alias(filename));
      browser->scrollToAnchor("AnnualBuildingUtilityPerformanceSummary::EntireFacility");
      m_mainTabDock->addTab(browser, browser->windowTitle());
      m_mainTabDock->setCurrentIndex(m_mainTabDock->count()-1);
      m_browserList.push_back(browser);
    }
  }


  void MainWindow::updateRecentFileActions()
  {
    int i;
    // check that file exists
    for (i=m_recentFiles.count()-1;i>=0;i--)
    {
      if (!QFile::exists(m_recentFiles[i]))
      {
        m_recentFiles.removeAt(i);
        m_recentAliases.removeAt(i);
      }
    }
    // limit to MaxRecentFiles
    while (m_recentFiles.count() > MaxRecentFiles)
      m_recentFiles.removeLast();

    // update menu
    for (auto & recentFile : m_recentFileActions) recentFile->setVisible(false);

    for (i=0; i<m_recentFiles.count();i++)
    {
      QString fileLabel = tr("&%1 (%2) %3").arg(i).arg(m_recentAliases[i]).arg(m_recentFiles[i]);
      m_recentFileActions[i]->setText(fileLabel);
      m_recentFileActions[i]->setData(QStringList() << m_recentAliases[i] << m_recentFiles[i]);
      m_recentFileActions[i]->setVisible(true);
    }
    // set separator visibility
    m_separatorAction->setVisible(!m_recentFiles.isEmpty());
  }

  bool MainWindow::openRecentFile()
  {
    // Qt macro for casting the sender of the signal to this slot
    QAction *recentFileAction = qobject_cast<QAction *>(sender());
    if (recentFileAction)
    {
      QStringList aliasFilename = recentFileAction->data().toStringList();
      if (aliasFilename.count() != 2)
        return false;
      else
        loadFile(aliasFilename[0],aliasFilename[1]);
    }
    return true;
  }


  void MainWindow::readSettings()
  {
    QSettings settings("OpenStudio", "ResultsViewer");
    restoreGeometry(settings.value("geometry").toByteArray());
    restoreState(settings.value("state").toByteArray());
    m_recentFiles = settings.value("recentFilesSL").toStringList();
    m_recentAliases = settings.value("recentAliasesSL").toStringList();
    m_lastPathOpened = settings.value("lastPathOpened").toString();
    m_lastImageSavedPath = settings.value("lastImageSavedPath").toString();
    updateRecentFileActions();
  }

  void MainWindow::closeEvent(QCloseEvent *evt)
  {
    int i;
    evt->accept();
    writeSettings();
    delete m_data;

    for (i=m_plotViewList.size() -1; i>-1; i--)
    {
      if (m_plotViewList[i]) m_plotViewList[i]->close();
    }

    for (i=m_browserList.size() -1; i>-1; i--)
    {
      if (m_browserList[i]) m_browserList[i]->close();
    }

  }

  void MainWindow::slotClearSettings()
  {
    QSettings settings("OpenStudio", "ResultsViewer");
    settings.clear();
    readSettings();
  }

  void MainWindow::writeSettings()
  {
    QSettings settings("OpenStudio", "ResultsViewer");
    settings.setValue("geometry", saveGeometry());
    settings.setValue("state", saveState());
    settings.setValue("recentFilesSL", m_recentFiles);
    settings.setValue("recentAliasesSL", m_recentAliases);
    settings.setValue("lastPathOpened", m_lastPathOpened);
    settings.setValue("lastImageSavedPath", m_lastImageSavedPath);
  }

  void MainWindow::slotDragPlotViewData(const std::vector<resultsviewer::ResultsViewerPlotData> &rvplotData)
  {
    QApplication::setOverrideCursor(Qt::WaitCursor);
    std::vector<PlotViewData> plotViewDataVec;
    std::vector<resultsviewer::ResultsViewerPlotData>::const_iterator rvPlotDataVecIt;
    for(rvPlotDataVecIt = rvplotData.begin(); rvPlotDataVecIt != rvplotData.end(); ++rvPlotDataVecIt)
    {
      plotViewDataVec.push_back(plotViewDataFromResultsViewerPlotData(*rvPlotDataVecIt));
    }

    if (plotViewDataVec.size() > 0)
    {
      auto mimeData = new PlotViewMimeData(plotViewDataVec);
      auto drag = new QDrag(this);
      drag->setMimeData(mimeData);
      drag->exec(Qt::MoveAction);
    }
    QApplication::restoreOverrideCursor();
  }

  resultsviewer::PlotViewData MainWindow::plotViewDataFromResultsViewerPlotData(const resultsviewer::ResultsViewerPlotData &rvplotData)
  {

    resultsviewer::PlotViewData plotViewData;

    plotViewData.interval = rvplotData.reportFreq.toUpper();

    switch (rvplotData.dataType)
    {

    case RVD_ILLUMINANCEMAP:
      plotViewData.dbIdentifier = rvplotData.dbIdentifier; // illuminance mapname

      plotViewData.legendName = "(%1) " + rvplotData.variableName;
      if (!rvplotData.keyName.isEmpty()) plotViewData.legendName = "(%1) " + rvplotData.variableName + "," + rvplotData.keyName;
      plotViewData.alias.append(m_data->alias(rvplotData.filename));
      plotViewData.plotSource.append(rvplotData.filename);
      plotViewData.plotTitle = rvplotData.reportFreq + "," + rvplotData.variableName;
      plotViewData.windowTitle = rvplotData.filename + " : " + rvplotData.variableName;
      break;

    case RVD_TIMESERIES:
      plotViewData.legendName = "(%1) " + rvplotData.variableName;
      if (!rvplotData.keyName.isEmpty()) plotViewData.legendName = "(%1) " + rvplotData.variableName + "," + rvplotData.keyName;

      plotViewData.plotTitle = rvplotData.reportFreq + "," + rvplotData.variableName;
      plotViewData.windowTitle = rvplotData.filename + " : " + rvplotData.variableName;


      if (m_data->sqlFile(rvplotData.filename).connectionOpen())
      {
        plotViewData.alias.append(m_data->alias(rvplotData.filename));
        plotViewData.plotSource.append(rvplotData.filename);
        openstudio::OptionalTimeSeries ts = m_data->sqlFile(rvplotData.filename).timeSeries(openstudio::toString(rvplotData.envPeriod), openstudio::toString(rvplotData.reportFreq), openstudio::toString(rvplotData.variableName), openstudio::toString(rvplotData.keyName));
        if (ts)
        {
          if (ts->values().size() > 0) {
            plotViewData.ts = ts;
          } else {
            QMessageBox::information(this, tr("No Time Data"), "No time to plot for " + rvplotData.variableName + ".\nCheck the input file for environment period:\n" + rvplotData.envPeriod + ".");
          }
        }
        else { // ticket 174
          QMessageBox::information(this, tr("No Plot Data"), "No data to plot for " + rvplotData.variableName + ".\nThe input file likely scheduled off reporting for environment period:\n" + rvplotData.envPeriod + ".");
        }
      }
      break;

    }


    return plotViewData;
  }

  resultsviewer::PlotViewData MainWindow::plotViewDataDifference(const resultsviewer::PlotViewData &plotViewData1, const resultsviewer::PlotViewData &plotViewData2)
  {
    resultsviewer::PlotViewData plotViewData;

    QString legendName = plotViewData2.legendName;
    legendName = legendName.replace("%1","%2");
    plotViewData.legendName = plotViewData1.legendName + " - " + legendName;
    plotViewData.plotTitle = plotViewData1.plotTitle + " - " + plotViewData2.plotTitle;
    plotViewData.windowTitle = plotViewData1.windowTitle + " - " + plotViewData2.windowTitle;

    // TODO - handle different intervals
    plotViewData.interval = plotViewData1.interval;

    plotViewData.alias.append(plotViewData1.alias);
    plotViewData.alias.append(plotViewData2.alias);
    plotViewData.plotSource.append(plotViewData1.plotSource);
    plotViewData.plotSource.append(plotViewData2.plotSource);

    plotViewData.ts = *plotViewData1.ts - *plotViewData2.ts;

    // TODO - check timeseries ptr and report issues if any

    return plotViewData;
  }



  void MainWindow::slotAddFloodPlot(const std::vector<resultsviewer::ResultsViewerPlotData> &fpVec)
  {
    QApplication::setOverrideCursor(Qt::WaitCursor);
    std::vector<resultsviewer::ResultsViewerPlotData>::const_iterator fpVecIt;
    for(fpVecIt = fpVec.begin(); fpVecIt != fpVec.end(); ++fpVecIt)
    {
      resultsviewer::PlotViewData pd = plotViewDataFromResultsViewerPlotData(*fpVecIt);
      if (pd.ts) // create plot widget only if timeseries data available
      {
        auto fp = new resultsviewer::PlotView(m_lastImageSavedPath, RVPV_FLOODPLOT);
        fp->plotViewData(pd, std::function<bool ()>());
        fp->show();
        emit (signalAddPlot(fp));
      }
    }
    QApplication::restoreOverrideCursor();
  }

  void MainWindow::slotAddIlluminancePlot(const std::vector<resultsviewer::ResultsViewerPlotData> &ipVec)
  {
    QApplication::setOverrideCursor(Qt::WaitCursor);
    std::vector<resultsviewer::ResultsViewerPlotData>::const_iterator ipVecIt;
    for(ipVecIt = ipVec.begin(); ipVecIt != ipVec.end(); ++ipVecIt)
    {
      resultsviewer::PlotViewData pd = plotViewDataFromResultsViewerPlotData(*ipVecIt);
      auto ip = new resultsviewer::PlotView(m_lastImageSavedPath, RVPV_ILLUMINANCEPLOT);
      ip->plotViewData(pd, std::function<bool ()>());
      ip->show();
      emit (signalAddPlot(ip));
    }
    QApplication::restoreOverrideCursor();
  }

  void MainWindow::slotAddIlluminancePlotComparison(const std::vector<resultsviewer::ResultsViewerPlotData> &ipVec)
  {
    if (ipVec.size() != 2) return;

    QApplication::setOverrideCursor(Qt::WaitCursor);
    resultsviewer::PlotViewData pd0 = plotViewDataFromResultsViewerPlotData(ipVec[0]);
    resultsviewer::PlotViewData pd1 = plotViewDataFromResultsViewerPlotData(ipVec[1]);
    auto ip = new resultsviewer::PlotView(m_lastImageSavedPath, RVPV_ILLUMINANCEPLOT);
    ip->plotViewDataDifference(pd0,pd1);
    ip->show();
    emit (signalAddPlot(ip));
    QApplication::restoreOverrideCursor();
  }



  void MainWindow::slotAddLinePlot(const std::vector<resultsviewer::ResultsViewerPlotData> &lpVec)
  {
    QProgressDialog *progressdialog = new QProgressDialog("Generating Line Plot", "Cancel", 0, lpVec.size() * 2, this);
    progressdialog->setMinimumDuration(1000);
    progressdialog->open();
    int progress = 0;

    if (lpVec.size() < 1) return;

    std::vector<resultsviewer::PlotViewData> pdVec;
    QApplication::setOverrideCursor(Qt::WaitCursor);
    std::vector<resultsviewer::ResultsViewerPlotData>::const_iterator lpVecIt;
    for (lpVecIt = lpVec.begin(); lpVecIt != lpVec.end() && !progressdialog->wasCanceled(); ++lpVecIt)
    {
      resultsviewer::PlotViewData pd = plotViewDataFromResultsViewerPlotData(*lpVecIt);
      if (pd.ts) pdVec.push_back(pd);
      ++progress;

      progressdialog->setValue(progress);
      QApplication::processEvents();
    }

    progressdialog->setMaximum(pdVec.size() + lpVec.size());

    if (progressdialog->wasCanceled())
    {
      QApplication::restoreOverrideCursor();
      return;
    }

    if (pdVec.size() > 0)
    {
      std::function<bool ()> canceledfunc = std::bind(&QProgressDialog::wasCanceled, progressdialog);
      auto lp = new resultsviewer::PlotView(m_lastImageSavedPath, RVPV_LINEPLOT);
      std::vector<resultsviewer::PlotViewData>::iterator pdVecIt;
      for (pdVecIt = pdVec.begin(); pdVecIt != pdVec.end() && !progressdialog->wasCanceled(); ++pdVecIt)
      {
        lp->plotViewData(*pdVecIt, canceledfunc);
        if (!progressdialog->wasCanceled())
        {
          ++progress;
          progressdialog->setValue(progress);
        }
      }

      if (progressdialog->wasCanceled())
      {
        QApplication::restoreOverrideCursor();
        return;
      }

      lp->show();
      emit (signalAddPlot(lp));
    }
    QApplication::restoreOverrideCursor();
  }


  void MainWindow::slotAddFloodPlotComparison(const std::vector<resultsviewer::ResultsViewerPlotData> &fpVec)
  {
    if (fpVec.size() != 2) return;

    QApplication::setOverrideCursor(Qt::WaitCursor);

    resultsviewer::PlotViewData pd0 = plotViewDataFromResultsViewerPlotData(fpVec[0]);
    resultsviewer::PlotViewData pd1 = plotViewDataFromResultsViewerPlotData(fpVec[1]);

    if ( (pd0.ts) && (pd1.ts) ) // create plot widget only if timeseries data available
    {
      resultsviewer::PlotViewData plotViewData = plotViewDataDifference(pd0, pd1);
      auto fp0minus1 = new resultsviewer::PlotView(m_lastImageSavedPath, RVPV_FLOODPLOT);
      fp0minus1->plotViewData(plotViewData, std::function<bool ()>());
      fp0minus1->show();
      emit (signalAddPlot(fp0minus1));

    }
    QApplication::restoreOverrideCursor();
  }

  void MainWindow::slotAddLinePlotComparison(const std::vector<resultsviewer::ResultsViewerPlotData> &lpVec)
  {
    if (lpVec.size() != 2) return;

    QApplication::setOverrideCursor(Qt::WaitCursor);

    resultsviewer::PlotViewData pd0 = plotViewDataFromResultsViewerPlotData(lpVec[0]);
    resultsviewer::PlotViewData pd1 = plotViewDataFromResultsViewerPlotData(lpVec[1]);

    if ( (pd0.ts) && (pd1.ts) ) // create plot widget only if timeseries data available
    {
      resultsviewer::PlotViewData plotViewData = plotViewDataDifference(pd0, pd1);
      auto lp0minus1 = new resultsviewer::PlotView(m_lastImageSavedPath, RVPV_LINEPLOT);
      lp0minus1->plotViewData(plotViewData, std::function<bool ()>());
      lp0minus1->show();
      emit (signalAddPlot(lp0minus1));

    }
    QApplication::restoreOverrideCursor();
  }

  /*
  void MainWindow::addFloodPlot(std::vector<openstudio::Plot2DData> fpVec)
  {
  std::vector<openstudio::Plot2DData>::iterator fpVecIt;
  for(fpVecIt = fpVec.begin(); fpVecIt != fpVec.end(); ++fpVecIt)
  {
  // assign cached timeseries if possible
  openstudio::FloodPlot *fp = new openstudio::FloodPlot();
  fp->plot2DData(*fpVecIt);
  fp->show();
  emit (signalAddPlot(fp));
  }
  }

  void MainWindow::addLinePlot(std::vector<openstudio::Plot2DData> lpVec)
  {
  if (lpVec.size() < 1) return;

  openstudio::LinePlot *lp = new openstudio::LinePlot();
  std::vector<openstudio::Plot2DData>::iterator lpVecIt;
  for (lpVecIt = lpVec.begin(); lpVecIt != lpVec.end(); ++lpVecIt)
  {
  lp->plot2DData(*lpVecIt);
  }
  lp->show();
  emit (signalAddPlot(lp));
  }
  */

  // reference http://www.qtcentre.org/forum/p-qtreewidgetitem-have-contextmenu--post93472/postcount3.html
  void MainWindow::showTreeViewContextMenu(const QPoint &pos)
  {
    /*
    QList<QTreeWidgetItem *> selectedTreeItems = m_treeView->selectedItems();

    if (selectedTreeItems.count() < 1) return;

    QList<QTreeWidgetItem *>::iterator treeIter;
    int illuminanceMapItemCount = 0;
    int timeseriesItemCount = 0;
    for (treeIter = selectedTreeItems.begin(); treeIter != selectedTreeItems.end(); treeIter++)
    {
    if ((*treeIter)->parent()->data(0,Qt::UserRole) == RVD_TIMESERIES) timeseriesItemCount++;
    if ((*treeIter)->parent()->data(0,Qt::UserRole) == RVD_ILLUMINANCEMAP) illuminanceMapItemCount++;
    }

    if ((timeseriesItemCount > 0) && (illuminanceMapItemCount > 0))
    {
    QMessageBox::information(this, tr("Multiple Data Types"), tr("You have selected %1 time series and %2 illuminance maps.\nYou can only plot one type of data at a time.\nPlease check your selected items.").arg(timeseriesItemCount).arg(illuminanceMapItemCount));
    return;
    }

    */

    int illuminanceMapItemCount = 0;
    int timeseriesItemCount = 0;
    if (treeViewSelectedItemsCanBePlotted(timeseriesItemCount, illuminanceMapItemCount))
    {

      QMenu menu(tr("Tree Context Menu"), this);

      // context menu based on selected row type
      if (timeseriesItemCount > 0)
        timeseriesTreeViewMenu(menu);
      else if (illuminanceMapItemCount > 0)
        illuminanceMapTreeViewMenu(menu);

      menu.exec(m_treeView->viewport()->mapToGlobal(pos));
    }
  }

  void MainWindow::timeseriesTreeViewMenu(QMenu &menu)
  {
    QList<QTreeWidgetItem *> selectedTreeItems = m_treeView->selectedItems();
    // acts on all selected items
    QAction *floodPlotAction = new QAction(tr("Flood Plot"), this);
    connect(floodPlotAction, SIGNAL(triggered()), m_treeView, SLOT(generateFloodPlotData()));
    QAction *singleLinePlotAction = new QAction(tr("Line Plot"), this);
    connect(singleLinePlotAction, SIGNAL(triggered()), m_treeView, SLOT(generateLinePlotData()));

    menu.addAction(floodPlotAction);

    // should only be shown if two items selected
    if (selectedTreeItems.count() == 2)
    {
      resultsviewer::ResultsViewerPlotData rvpd1 = m_treeView->resultsViewerPlotDataFromTreeItem(selectedTreeItems[0]);
      resultsviewer::ResultsViewerPlotData rvpd2 = m_treeView->resultsViewerPlotDataFromTreeItem(selectedTreeItems[1]);

      QAction *floodPlot1minus2Action = new QAction(tr("Flood Plot, %1 - %2").arg(rvpd1.idName()).arg(rvpd2.idName()), this);
      connect(floodPlot1minus2Action, SIGNAL(triggered()), m_treeView, SLOT(generateFloodPlotComparisonData()));
      QAction *floodPlot2minus1Action = new QAction(tr("Flood Plot, %1 - %2").arg(rvpd2.idName()).arg(rvpd1.idName()), this);
      connect(floodPlot2minus1Action, SIGNAL(triggered()), m_treeView, SLOT(generateReverseFloodPlotComparisonData()));
      menu.addAction(floodPlot1minus2Action);
      menu.addAction(floodPlot2minus1Action);

      menu.addAction(singleLinePlotAction);

      QAction *linePlot1minus2Action = new QAction(tr("Line Plot, %1 - %2").arg(rvpd1.idName()).arg(rvpd2.idName()), this);
      connect(linePlot1minus2Action, SIGNAL(triggered()), m_treeView, SLOT(generateLinePlotComparisonData()));
      QAction *linePlot2minus1Action = new QAction(tr("Line Plot, %1 - %2").arg(rvpd2.idName()).arg(rvpd1.idName()), this);
      connect(linePlot2minus1Action, SIGNAL(triggered()), m_treeView, SLOT(generateReverseLinePlotComparisonData()));
      menu.addAction(linePlot1minus2Action);
      menu.addAction(linePlot2minus1Action);
    }
    else
      menu.addAction(singleLinePlotAction);
  }

  void MainWindow::illuminanceMapTreeViewMenu(QMenu& menu)
  {
    QList<QTreeWidgetItem *> selectedTreeItems = m_treeView->selectedItems();
    QAction *illuminancePlotAction = new QAction(tr("Illuminance Map"), this);
    connect(illuminancePlotAction, SIGNAL(triggered()), m_treeView, SLOT(generateIlluminancePlotData()));

    menu.addAction(illuminancePlotAction);

    if (selectedTreeItems.count() == 2)
    {
      resultsviewer::ResultsViewerPlotData rvpd1 = m_treeView->resultsViewerPlotDataFromTreeItem(selectedTreeItems[0]);
      resultsviewer::ResultsViewerPlotData rvpd2 = m_treeView->resultsViewerPlotDataFromTreeItem(selectedTreeItems[1]);

      QAction *illuminanceMap1minus2Action = new QAction(tr("Illuminance Map, %1 - %2").arg(rvpd1.idName()).arg(rvpd2.idName()), this);
      connect(illuminanceMap1minus2Action, SIGNAL(triggered()), m_treeView, SLOT(generateIlluminancePlotComparisonData()));
      QAction *illuminanceMap2minus1Action = new QAction(tr("Illuminance Map, %1 - %2").arg(rvpd2.idName()).arg(rvpd1.idName()), this);
      connect(illuminanceMap2minus1Action, SIGNAL(triggered()), m_treeView, SLOT(generateReverseIlluminancePlotComparisonData()));
      menu.addAction(illuminanceMap1minus2Action);
      menu.addAction(illuminanceMap2minus1Action);
    }
  }

  void MainWindow::onTreeViewSelectionChanged()
  {
    for (QTreeWidgetItem *item : m_treeView->selectedItems())
    {
      if ( item->type() != ddtKeyValue )
      {
        if ((item->type() == ddtVariableName) && (item->childCount() == 0))
        {// exclude RunPeriod
          //        if (ReportingFrequency(openstudio::toString(item->parent()->text(0))) == ReportingFrequency::RunPeriod)
          openstudio::SqlFile sqlFile = m_data->sqlFile(m_treeView->filenameFromTreeItem(item));
          if (sqlFile.reportingFrequencyFromDB(openstudio::toString(item->parent()->text(0))))
            if (*(sqlFile.reportingFrequencyFromDB(openstudio::toString(item->parent()->text(0)))) == ReportingFrequency::RunPeriod)
              item->setSelected(false);
        }
        else
          item->setSelected(false);
      }
    }
  }

  void MainWindow::timeseriesTableViewMenu(QMenu& menu)
  {
    // acts on all selected timeseries items
    QAction *floodPlotAction = new QAction(tr("Flood Plot"), this);
    connect(floodPlotAction, SIGNAL(triggered()), m_tableView, SLOT(generateFloodPlotData()));
    QAction *singleLinePlotAction = new QAction(tr("Line Plot"), this);
    connect(singleLinePlotAction, SIGNAL(triggered()), m_tableView, SLOT(generateLinePlotData()));

    menu.addAction(floodPlotAction);

    std::vector<int> selectedRows = m_tableView->selectedRows();
    // should only be shown if two items selected
    if (selectedRows.size() == 2)
    {
      resultsviewer::ResultsViewerPlotData rvpd1 = m_tableView->resultsViewerPlotDataFromTableRow(selectedRows[0]);
      resultsviewer::ResultsViewerPlotData rvpd2 = m_tableView->resultsViewerPlotDataFromTableRow(selectedRows[1]);

      QAction *floodPlot1minus2Action = new QAction(tr("Flood Plot, %1 - %2").arg(rvpd1.idName()).arg(rvpd2.idName()), this);
      connect(floodPlot1minus2Action, SIGNAL(triggered()), m_tableView, SLOT(generateFloodPlotComparisonData()));
      QAction *floodPlot2minus1Action = new QAction(tr("Flood Plot, %1 - %2").arg(rvpd2.idName()).arg(rvpd1.idName()), this);
      connect(floodPlot2minus1Action, SIGNAL(triggered()), m_tableView, SLOT(generateReverseFloodPlotComparisonData()));
      menu.addAction(floodPlot1minus2Action);
      menu.addAction(floodPlot2minus1Action);

      menu.addAction(singleLinePlotAction);

      QAction *linePlot1minus2Action = new QAction(tr("Line Plot, %1 - %2").arg(rvpd1.idName()).arg(rvpd2.idName()), this);
      connect(linePlot1minus2Action, SIGNAL(triggered()), m_tableView, SLOT(generateLinePlotComparisonData()));
      QAction *linePlot2minus1Action = new QAction(tr("Line Plot, %1 - %2").arg(rvpd2.idName()).arg(rvpd1.idName()), this);
      connect(linePlot2minus1Action, SIGNAL(triggered()), m_tableView, SLOT(generateReverseLinePlotComparisonData()));
      menu.addAction(linePlot1minus2Action);
      menu.addAction(linePlot2minus1Action);
    }
    else
      menu.addAction(singleLinePlotAction);
  }

  void MainWindow::illuminanceMapTableViewMenu(QMenu& menu)
  {
    QAction *illuminancePlotAction = new QAction(tr("Illuminance Map"), this);
    connect(illuminancePlotAction, SIGNAL(triggered()), m_tableView, SLOT(generateIlluminancePlotData()));

    menu.addAction(illuminancePlotAction);

    std::vector<int> selectedRows = m_tableView->selectedRows();
    if (selectedRows.size() == 2)
    {
      resultsviewer::ResultsViewerPlotData rvpd1 = m_tableView->resultsViewerPlotDataFromTableRow(selectedRows[0]);
      resultsviewer::ResultsViewerPlotData rvpd2 = m_tableView->resultsViewerPlotDataFromTableRow(selectedRows[1]);

      QAction *illuminanceMap1minus2Action = new QAction(tr("Illuminance Map, %1 - %2").arg(rvpd1.idName()).arg(rvpd2.idName()), this);
      connect(illuminanceMap1minus2Action, SIGNAL(triggered()), m_tableView, SLOT(generateIlluminancePlotComparisonData()));
      QAction *illuminanceMap2minus1Action = new QAction(tr("Illuminance Map, %1 - %2").arg(rvpd2.idName()).arg(rvpd1.idName()), this);
      connect(illuminanceMap2minus1Action, SIGNAL(triggered()), m_tableView, SLOT(generateReverseIlluminancePlotComparisonData()));
      menu.addAction(illuminanceMap1minus2Action);
      menu.addAction(illuminanceMap2minus1Action);
    }
  }


  void MainWindow::slotNewFloodPlot()
  {
    // determine that is active
    int illuminanceMapItemCount = 0;
    int timeseriesItemCount = 0;

    if (m_mainTabView->currentIndex() == 0) // tableView
    {
      if (tableViewSelectedItemsCanBePlotted(timeseriesItemCount, illuminanceMapItemCount))
      {
        if (illuminanceMapItemCount==0) emit(m_tableView->generateFloodPlotData());
      }
    }
    else if (m_mainTabView->currentIndex() == 1) // treeView
    {
      if (tableViewSelectedItemsCanBePlotted(timeseriesItemCount, illuminanceMapItemCount))
      {
        if (illuminanceMapItemCount==0) emit(m_treeView->generateFloodPlotData());
      }
    }
  }

  void MainWindow::slotNewLinePlot()
  {
    // determine that is active
    int illuminanceMapItemCount = 0;
    int timeseriesItemCount = 0;

    if (m_mainTabView->currentIndex() == 0) // tableView
    {
      if (tableViewSelectedItemsCanBePlotted(timeseriesItemCount, illuminanceMapItemCount))
      {
        if (illuminanceMapItemCount==0) emit(m_tableView->generateLinePlotData());
      }
    }
    else if (m_mainTabView->currentIndex() == 1) // treeView
    {
      if (tableViewSelectedItemsCanBePlotted(timeseriesItemCount, illuminanceMapItemCount))
      {
        if (illuminanceMapItemCount==0) emit(m_treeView->generateLinePlotData());
      }
    }
  }

  bool MainWindow::tableViewSelectedItemsCanBePlotted(int &timeseriesItemCount, int &illuminanceMapItemCount)
  {
    QList<QTableWidgetItem *> selectedTableItems = m_tableView->selectedItems();

    if (selectedTableItems.count() < 1) return false;

    // determine plot item types selected
    std::vector<int> selectedRows = m_tableView->selectedRows();
    std::vector<int>::iterator rowIter;
    illuminanceMapItemCount = 0;
    timeseriesItemCount = 0;
    for (rowIter = selectedRows.begin(); rowIter != selectedRows.end(); ++rowIter)
    {
      if (m_tableView->item(*rowIter, m_tableView->headerNames().indexOf("File"))->data(Qt::UserRole) == RVD_TIMESERIES) timeseriesItemCount++;
      if (m_tableView->item(*rowIter, m_tableView->headerNames().indexOf("File"))->data(Qt::UserRole) == RVD_ILLUMINANCEMAP) illuminanceMapItemCount++;
    }

    if ((timeseriesItemCount > 0) && (illuminanceMapItemCount > 0))
    {
      QMessageBox::information(this, tr("Multiple Data Types"), tr("You have selected %1 time series and %2 illuminance maps.\nYou can only plot one type of data at a time.\nPlease check your selected items.").arg(timeseriesItemCount).arg(illuminanceMapItemCount));
      return false;
    }
    else
      return true;
  }

  bool MainWindow::treeViewSelectedItemsCanBePlotted(int &timeseriesItemCount, int &illuminanceMapItemCount)
  {
    QList<QTreeWidgetItem *> selectedTreeItems = m_treeView->selectedItems();

    if (selectedTreeItems.count() < 1) return false;

    QList<QTreeWidgetItem *>::iterator treeIter;
    illuminanceMapItemCount = 0;
    timeseriesItemCount = 0;
    for (treeIter = selectedTreeItems.begin(); treeIter != selectedTreeItems.end(); ++treeIter)
    {
      if ((*treeIter)->parent()->data(0,Qt::UserRole) == RVD_TIMESERIES) timeseriesItemCount++;
      if ((*treeIter)->parent()->data(0,Qt::UserRole) == RVD_ILLUMINANCEMAP) illuminanceMapItemCount++;
    }

    if ((timeseriesItemCount > 0) && (illuminanceMapItemCount > 0))
    {
      QMessageBox::information(this, tr("Multiple Data Types"), tr("You have selected %1 time series and %2 illuminance maps.\nYou can only plot one type of data at a time.\nPlease check your selected items.").arg(timeseriesItemCount).arg(illuminanceMapItemCount));
      return false;
    }
    else
      return true;
  }


  void MainWindow::showTableViewContextMenu(const QPoint &pos)
  {
    /* QList<QTableWidgetItem *> selectedTableItems = m_tableView->selectedItems();

    if (selectedTableItems.count() < 1) return;

    // determine plot item types selected
    std::vector<int> selectedRows = m_tableView->selectedRows();
    std::vector<int>::iterator rowIter;
    int illuminanceMapItemCount = 0;
    int timeseriesItemCount = 0;
    for (rowIter = selectedRows.begin(); rowIter != selectedRows.end(); rowIter++)
    {
    if (m_tableView->item(*rowIter, m_tableView->headerNames().indexOf("File"))->data(Qt::UserRole) == RVD_TIMESERIES) timeseriesItemCount++;
    if (m_tableView->item(*rowIter, m_tableView->headerNames().indexOf("File"))->data(Qt::UserRole) == RVD_ILLUMINANCEMAP) illuminanceMapItemCount++;
    }

    if ((timeseriesItemCount > 0) && (illuminanceMapItemCount > 0))
    {
    QMessageBox::information(this, tr("Multiple Data Types"), tr("You have selected %1 time series and %2 illuminance maps.\nYou can only plot one type of data at a time.\nPlease check your selected items.").arg(timeseriesItemCount).arg(illuminanceMapItemCount));
    return;
    }
    */
    int illuminanceMapItemCount = 0;
    int timeseriesItemCount = 0;
    if (tableViewSelectedItemsCanBePlotted(timeseriesItemCount, illuminanceMapItemCount))
    {
      QMenu menu(tr("Table Context Menu"), this);

      // context menu based on selected row type
      if (timeseriesItemCount > 0)
        timeseriesTableViewMenu(menu);
      else if (illuminanceMapItemCount > 0)
        illuminanceMapTableViewMenu(menu);

      menu.exec(m_tableView->viewport()->mapToGlobal(pos));
    }
  }

  void MainWindow::closeSelectedFiles()
  {
    QString fileToClose = m_fileComboBox->itemData(m_fileComboBox->currentIndex(),Qt::ToolTipRole).toString();
    closeFile(fileToClose);
  }

  void MainWindow::slotCloseAllFiles()
  {
    for (int i=m_fileComboBox->count()-1;i>-1;i--)
    {
      m_fileComboBox->setCurrentIndex(i);
      QString fileToClose = m_fileComboBox->itemData(i,Qt::ToolTipRole).toString();
      closeFile(fileToClose);
    }
  }

  void MainWindow::closeFile(const QString& filename)
  {
    // delete from resultViewerData (delete sqlFile)
    // remove from listView, treeView and tableView widgets (leave plots for now)
    m_tableView->removeFile(filename);
    m_fileComboBox->removeItem(m_fileComboBox->currentIndex());
    m_treeView->removeFile(filename);
    m_data->removeFile(filename);
    // close ABUPS if present
    int index = currentEPlusHTML(filename);
    if (index > -1)
    {
      m_browserList[index]->close();
      m_browserList.removeAt(index);
    }
  }

  void MainWindow::openContainingFolder()
  {
    if ((m_fileComboBox->currentIndex() > -1) && (m_fileComboBox->currentIndex() < m_fileComboBox->count()))
    {
      QString file = m_fileComboBox->itemData(m_fileComboBox->currentIndex(),Qt::ToolTipRole).toString();
      openContainingFolder(file);
    }
  }

  void MainWindow::openContainingFolder(const QString& filename)
  {
    QString folder = QFileInfo(filename).absolutePath();
    QDesktopServices::openUrl(QUrl::fromLocalFile(folder));
  }

  void MainWindow::selectNextFile()
  {
    if (m_fileComboBox->count() > 0)
    {
      int nextItem = m_fileComboBox->currentIndex();
      nextItem++;
      if (nextItem >= m_fileComboBox->count()) nextItem--;
      if (nextItem < 0) nextItem = 0;
      m_fileComboBox->setCurrentIndex(nextItem);
    }
  }

  void MainWindow::selectPreviousFile()
  {
    if (m_fileComboBox->count() > 0)
    {
      int previousItem = m_fileComboBox->currentIndex();
      previousItem--;
      if (previousItem < 0) previousItem = 0;
      m_fileComboBox->setCurrentIndex(previousItem);
    }
  }

  void MainWindow::createFileToolBar()
  {
    m_fileToolBar = new QToolBar(tr("Files"));

    m_fileToolBar->addWidget(new QLabel("Select File"));


    m_fileComboBox = new QComboBox;
    m_fileComboBox->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    connect(m_fileComboBox, SIGNAL( currentIndexChanged(int) ), this, SLOT( slotSelectedFileChanged(int) ) );

    // to preserve settings
    m_fileToolBar->setObjectName("fileToolBar");
    m_fileToolBar->addWidget(m_fileComboBox);

    // previous next and close
    QAction *previousFileAction = new QAction(QIcon(":/images/previous.png"), tr("Previous File"), this);
    previousFileAction->setToolTip("Previous File");
    connect(previousFileAction, SIGNAL(triggered()), this, SLOT(selectPreviousFile()));
    m_fileToolBar->addAction(previousFileAction);

    QAction *nextFileAction = new QAction(QIcon(":/images/next.png"), tr("Next File"), this);
    nextFileAction->setToolTip("Next File");
    connect(nextFileAction, SIGNAL(triggered()), this, SLOT(selectNextFile()));
    m_fileToolBar->addAction(nextFileAction);

    QAction *fileContainingFolderAction = new QAction(QIcon(":/images/open_container_folder.png"), tr("Open Containing Folder"), this);
    fileContainingFolderAction->setToolTip("Open Containing Folder");
    connect(fileContainingFolderAction, SIGNAL(triggered()), this, SLOT(openContainingFolder()));
    m_fileToolBar->addAction(fileContainingFolderAction);

    QAction *updateAliasAction = new QAction(QIcon(":/images/update_alias.png"), tr("Update Alias"), this);
    updateAliasAction->setToolTip("Update Alias");
    connect(updateAliasAction, SIGNAL(triggered()), this, SLOT(slotUpdateAlias()));
    m_fileToolBar->addAction(updateAliasAction);

    QAction *fileCloseAction = new QAction(QIcon(":/images/close_file.png"),tr("Close Selected File"), this);
    fileCloseAction->setToolTip("Close Selected File");
    connect(fileCloseAction, SIGNAL(triggered()), this, SLOT(closeSelectedFiles()));
    m_fileToolBar->addAction(fileCloseAction);

    QAction *fileCloseAllAction = new QAction(QIcon(":/images/close_all.png"),tr("Close All Files"), this);
    fileCloseAllAction->setToolTip("Close All Files");
    connect(fileCloseAllAction, SIGNAL(triggered()), this, SLOT(slotCloseAllFiles()));
    m_fileToolBar->addAction(fileCloseAllAction);

    m_fileToolBar->setAllowedAreas(Qt::AllToolBarAreas);
    addToolBar(m_fileToolBar);

  }


  void MainWindow::createPlotToolBar()
  {
    m_plotToolBar = new QToolBar(tr("Plots"));

    // m_plotToolBar->addWidget(new QLabel(" Open Plots "));


    m_plotComboBox = new QComboBox;
    m_plotComboBox->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    connect(m_plotComboBox, SIGNAL( currentIndexChanged(int) ), this, SLOT( onSelectedPlotChanged(int) ) );

    m_plotToolBar->setObjectName("plotToolBar"); // to preserve settings
    // m_plotToolBar->addWidget(m_plotComboBox);

    // previous next and close

    /*
    QAction *previousPlot = new QAction(QIcon(":/images/left.png"),tr("Previous Plot"), this);
    previousPlot->setToolTip("Previous plot");
    connect(previousPlot, SIGNAL(triggered()), this, SLOT(showPreviousPlot()));
    m_plotToolBar->addAction(previousPlot);

    QAction *nextPlot = new QAction(QIcon(":/images/right.png"),tr("Next Plot"), this);
    nextPlot->setToolTip("Next plot");
    connect(nextPlot, SIGNAL(triggered()), this, SLOT(showNextPlot()));
    m_plotToolBar->addAction(nextPlot);
    */
    QAction *newLinePlot = new QAction(QIcon(":/images/new_line_plot.png"),tr("New Line Plot"), this);
    newLinePlot->setToolTip("New Line Plot");
    connect(newLinePlot, SIGNAL(triggered()), this, SLOT(slotNewLinePlot()));
    m_plotToolBar->addAction(newLinePlot);

    QAction *newFloodPlot = new QAction(QIcon(":/images/new_flood_plot.png"),tr("New Flood Plot"), this);
    newFloodPlot->setToolTip("New Flood Plot");
    connect(newFloodPlot, SIGNAL(triggered()), this, SLOT(slotNewFloodPlot()));
    m_plotToolBar->addAction(newFloodPlot);


    QAction *closePlot = new QAction(QIcon(":/images/close_plot.png"),tr("Close Selected Plot"), this);
    closePlot->setToolTip("Close Selected Plot");
    connect(closePlot, SIGNAL(triggered()), this, SLOT(closePlot()));
    m_plotToolBar->addAction(closePlot);

    QAction *closeAllPlot = new QAction(QIcon(":/images/close_all_plots.png"),tr("Close All Plots"), this);
    closeAllPlot->setToolTip("Close All Plots");
    connect(closeAllPlot, SIGNAL(triggered()), this, SLOT(closeAllPlots()));
    m_plotToolBar->addAction(closeAllPlot);

    /*
    QAction *floatPlot = new QAction(QIcon(":/images/floatOrDock.png"),tr("Float or Dock Plot"), this);
    floatPlot->setToolTip("Float or dock selected plot.");
    connect(floatPlot, SIGNAL(triggered()), this, SLOT(slotFloatPlot()));
    m_plotToolBar->addAction(floatPlot);
    */
    m_plotToolBar->setAllowedAreas(Qt::AllToolBarAreas);
    addToolBar(m_plotToolBar);


    connect(this, SIGNAL(signalAddPlot(resultsviewer::PlotView *) ), this, SLOT( slotAddPlot(resultsviewer::PlotView *) ) );

  }


  void MainWindow::showNextPlot()
  {
    int currentIndex = m_plotComboBox->currentIndex();
    if (currentIndex > -1)
    {
      currentIndex++;
      if (currentIndex < m_plotComboBox->count())
        m_plotComboBox->setCurrentIndex(currentIndex);
    }
  }

  void MainWindow::showPreviousPlot()
  {
    int currentIndex = m_plotComboBox->currentIndex();
    if (currentIndex > -1)
    {
      currentIndex--;
      if (currentIndex >= 0)
        m_plotComboBox->setCurrentIndex(currentIndex);
    }
  }

  void MainWindow::closeAllPlots()
  {
    for( int i = m_plotComboBox->count()-1 ; i>-1 ; i-- )
    {
      closeSelectedPlot(i);
    }
  }


  void MainWindow::closeSelectedPlot(int index)
  {
    if ((index > -1) && (index < m_plotComboBox->count()))
    {
      m_plotViewList[index]->close();
      m_plotViewList.removeAt(index);
      m_plotComboBox->removeItem(index);
    }
  }

  void MainWindow::closePlot()
  {
    int currentIndex = m_plotComboBox->currentIndex();
    closeSelectedPlot(currentIndex);
  }


  void MainWindow::floatSelectedPlot(int index)
  {
    if ((index > -1) && (index < m_plotComboBox->count()))
    {
      if (m_plotViewList.at(index)->parent() == nullptr)
      {
        m_mainTabDock->addTab(m_plotViewList.at(index),m_plotViewList.at(index)->windowTitle());
      }
      else
      {
        m_plotViewList.at(index)->setParent(nullptr);
        // default location
        m_plotViewList.at(index)->setGeometry(10,30,1024,768);
      }
      m_plotComboBox->setCurrentIndex(index);
      onSelectedPlotChanged(index);
    }
  }


  void MainWindow::slotFloatOrDockPlot(resultsviewer::PlotView *plot)
  {
    int currentIndex = m_plotViewList.indexOf(plot);
    floatSelectedPlot(currentIndex);
  }


  void MainWindow::slotFloatPlot()
  {
    int currentIndex = m_plotComboBox->currentIndex();
    floatSelectedPlot(currentIndex);
  }


  void MainWindow::onSelectedPlotChanged(int index)
  { // show plot
    if ((index > -1) && (index < m_plotViewList.count()) )
    {
      if (m_plotViewList.at(index)->parent() == nullptr)
      {
        m_plotViewList.at(index)->show();
        m_plotViewList.at(index)->raise();
        m_plotViewList.at(index)->activateWindow();
      }
      else
      {
        m_mainTabDock->setCurrentWidget(m_plotViewList.at(index));
      }
    }
  }

  void MainWindow::slotCloseBrowser(resultsviewer::BrowserView *browser)
  {
    int index = m_browserList.indexOf(browser);
    if (index > -1)
      m_browserList.removeAt(index);
  }



  void MainWindow::slotClosePlot(QString &title)
  {
    int index = m_plotComboBox->findText(title);
    if ((index > -1) && (index < m_plotComboBox->count()))
    {
      m_plotViewList.removeAt(index);
      m_plotComboBox->removeItem(index);
    }
  }

  void MainWindow::slotAddPlot(resultsviewer::PlotView *plot)
  {
    QString windowTitle = tr("Plot %1").arg(m_plotTitleNumber++);
    plot->setWindowTitle(windowTitle);
    m_mainTabDock->addTab(plot, windowTitle);
    m_mainTabDock->setCurrentWidget(plot);

    connect(plot, SIGNAL(signalClose(QString&)),this,SLOT(slotClosePlot(QString&)));
    connect(plot, SIGNAL(signalFloatOrDockMe(resultsviewer::PlotView*)),this,SLOT(slotFloatOrDockPlot(resultsviewer::PlotView*)));
    connect(plot, SIGNAL(signalLastImageSavedPath(QString&)),this,SLOT(slotUpdateLastImageSavedPath(QString&)));
    m_plotViewList.push_back(plot);
    m_plotComboBox->addItem(m_plotViewList.back()->windowTitle());
    m_plotComboBox->setCurrentIndex(m_plotComboBox->count()-1);
  }

  void MainWindow::slotHelpAbout()
  {// html format - can use separate form
    QMessageBox aboutBox(this);
    aboutBox.setText(RESULTSVIEWER_ABOUTBOX);
    aboutBox.setStyleSheet("qproperty-alignment: AlignCenter;");
    aboutBox.setWindowTitle("About ResultsViewer");
    aboutBox.exec();
  }

  void MainWindow::slotParseCommandLine()
  {
    QStringList args = QApplication::arguments();
    args.removeFirst(); // application name
    bool makeTempCopies = args.removeAll("--maketempcopies") > 0;
    openFileList(args, makeTempCopies);
  }

  void MainWindow::openFileList(const QStringList& fileList, bool t_makeTempCopies)
  {
    if (!fileList.isEmpty())
    {
      for (QString file : fileList)
      {
        if (!QFile::exists(file)) {
          QMessageBox::information(this, tr("File Open"), tr("File not found:\n" + file.toUtf8()));
          continue;
        }

        if (t_makeTempCopies)
        {
          openstudio::path basename = openstudio::toPath(QFileInfo(file).baseName());
          openstudio::path fullpath = openstudio::toPath(file);
          std::shared_ptr<openstudio::TemporaryDirectory> temporaryDirectory(new openstudio::TemporaryDirectory());
          openstudio::path newpath = temporaryDirectory->path() / basename;
          QFile::copy(openstudio::toQString(fullpath), openstudio::toQString(newpath));

          openstudio::path eplustbl = fullpath.parent_path() / openstudio::toPath("eplustbl.htm");
          if (QFile::exists(toQString(eplustbl)))
          {
            QFile::copy(openstudio::toQString(eplustbl), openstudio::toQString(temporaryDirectory->path() / openstudio::toPath("eplustbl.htm")));
          }

          file = openstudio::toQString(newpath);
          m_temporaryDirectories.push_back(temporaryDirectory);
        }

        QFileInfo info(file); // handles windows links and "\"
        QString filename = info.absoluteFilePath();
        QString alias = recentFilesAlias(filename);
        if (alias.isEmpty()) alias = m_data->defaultAlias(filename);
        emit (loadFile(alias, filename));
      }
    }
  }

  const QString MainWindow::recentFilesAlias(const QString& filename)
  {
    QString alias = "";
    // find alias or default
    int i = m_recentFiles.indexOf(filename);
    if (i > -1) alias = m_recentAliases[i];
    return alias;
  }



  void MainWindow::slotClearRecentFiles()
  {
    m_recentFiles.clear();
    updateRecentFileActions();
  }


  void MainWindow::slotDefaultLayout()
  {
    // set up initial docking layout
    addToolBar(Qt::TopToolBarArea, m_fileToolBar);
    addToolBar(Qt::TopToolBarArea, m_plotToolBar);
    m_fileToolBar->show();
    m_plotToolBar->show();
    /* dock option
    setDockNestingEnabled(true);
    m_tableViewDock->setFloating(false);
    m_treeViewDock->setFloating(false);
    setCorner(Qt::TopLeftCorner, Qt::LeftDockWidgetArea);
    setCorner(Qt::TopRightCorner, Qt::RightDockWidgetArea);
    setCorner(Qt::BottomLeftCorner, Qt::LeftDockWidgetArea);
    setCorner(Qt::BottomRightCorner, Qt::RightDockWidgetArea);
    addDockWidget(Qt::LeftDockWidgetArea, m_tableViewDock);
    tabifyDockWidget(m_tableViewDock, m_treeViewDock);
    setTabPosition(Qt::LeftDockWidgetArea, QTabWidget::North);
    */
  }

  void MainWindow::createWelcomePage()
  {
    auto browser = new resultsviewer::BrowserView(this);
    browser->setWindowTitle(tr("Welcome"));
    browser->setSearchPaths(QStringList(":/doc"));
    browser->setSource(QUrl::fromLocalFile("Welcome.htm"));
    connect(browser,SIGNAL(signalFloatOrDockMe(resultsviewer::BrowserView*)), this, SLOT(floatOrDockBrowser(resultsviewer::BrowserView *)));
    connect(browser,SIGNAL(signalClose(resultsviewer::BrowserView*)), this, SLOT(slotCloseBrowser(resultsviewer::BrowserView *)));
    m_mainTabDock->addTab(browser, browser->windowTitle());
    m_browserList.push_back(browser);
  }

  void MainWindow::floatOrDockBrowser(resultsviewer::BrowserView *browser)
  {
    if (browser)
    {
      if (browser->parent() == nullptr)
      {
        m_mainTabDock->addTab(browser,browser->windowTitle());
        m_mainTabDock->setCurrentIndex(m_mainTabDock->count()-1);
      }
      else
      {
        browser->setParent(nullptr);
        browser->setGeometry(10,30,1024,768);
        browser->show();
        browser->raise();
        browser->activateWindow();
      }
    }
  }

  void MainWindow::createTableView()
  {
    m_tableView = new resultsviewer::TableView(this);
    m_tableView->setAlternatingRowColors(true);
    connect(m_tableView, SIGNAL( customContextMenuRequested(const QPoint &) ), SLOT( showTableViewContextMenu(const QPoint &) ) );
    //    connect(m_tableView, SIGNAL( itemDoubleClicked(QTableWidgetItem * ) ), m_tableView, SLOT( generateLinePlotData() ));
    connect(m_tableView, SIGNAL( itemDoubleClicked(QTableWidgetItem * ) ), this, SLOT( slotTableViewDoubleClick(QTableWidgetItem * )));
    connect(m_tableView, SIGNAL( signalAddLinePlot(const std::vector<resultsviewer::ResultsViewerPlotData>& ) ), this, SLOT( slotAddLinePlot(const std::vector<resultsviewer::ResultsViewerPlotData>& ) ) );
    connect(m_tableView, SIGNAL( signalAddFloodPlot(const std::vector<resultsviewer::ResultsViewerPlotData>& ) ), this, SLOT( slotAddFloodPlot(const std::vector<resultsviewer::ResultsViewerPlotData>& ) ) );
    connect(m_tableView, SIGNAL( signalAddLinePlotComparison(const std::vector<resultsviewer::ResultsViewerPlotData>& ) ), this, SLOT( slotAddLinePlotComparison(const std::vector<resultsviewer::ResultsViewerPlotData>& ) ) );
    connect(m_tableView, SIGNAL( signalAddFloodPlotComparison(const std::vector<resultsviewer::ResultsViewerPlotData>& ) ), this, SLOT( slotAddFloodPlotComparison(const std::vector<resultsviewer::ResultsViewerPlotData>& ) ) );
    connect(m_tableView, SIGNAL( signalDragResultsViewerPlotData(const std::vector<resultsviewer::ResultsViewerPlotData>& ) ), this, SLOT( slotDragPlotViewData(const std::vector<resultsviewer::ResultsViewerPlotData>& ) ) );
    connect(m_tableView, SIGNAL( signalAddIlluminancePlot(const std::vector<resultsviewer::ResultsViewerPlotData>& ) ), this, SLOT( slotAddIlluminancePlot(const std::vector<resultsviewer::ResultsViewerPlotData>& ) ) );
    connect(m_tableView, SIGNAL( signalAddIlluminancePlotComparison(const std::vector<resultsviewer::ResultsViewerPlotData>& ) ), this, SLOT( slotAddIlluminancePlotComparison(const std::vector<resultsviewer::ResultsViewerPlotData>& ) ) );
  }

  void MainWindow::slotTableViewDoubleClick(QTableWidgetItem *item)
  {
    if (!item) return;

    // if (m_tableView->item(item->row(),1)->data(Qt::UserRole) == RVD_ILLUMINANCEMAP)
    if (m_tableView->item(item->row(),m_tableView->headerNames().indexOf("File"))->data(Qt::UserRole) == RVD_ILLUMINANCEMAP)
      m_tableView->generateIlluminancePlotData();
    else
      m_tableView->generateLinePlotData();
  }

  void MainWindow::slotTreeViewDoubleClick(QTreeWidgetItem *item, int col)
  {
    if (!item) return;
    if (!item->parent()) return;

    // if (item->data(col, Qt::UserRole) == RVD_ILLUMINANCEMAP)
    if (item->parent()->data(col, Qt::UserRole) == RVD_ILLUMINANCEMAP)
      m_treeView->generateIlluminancePlotData();
    else
      m_treeView->generateLinePlotData();
  }

  void MainWindow::createTreeView()
  {
    m_treeView = new resultsviewer::TreeView(this);
    m_treeView->setContextMenuPolicy(Qt::CustomContextMenu);
    m_treeView->headerItem()->setHidden(true);
    m_treeView->setColumnCount(1);
    m_treeView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    m_treeView->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_treeView->setAlternatingRowColors(true);
    connect(m_treeView, SIGNAL(customContextMenuRequested(const QPoint &)), SLOT(showTreeViewContextMenu(const QPoint &)));
    //    connect(m_treeView, SIGNAL( itemDoubleClicked(QTreeWidgetItem *, int ) ), m_treeView, SLOT( generateLinePlotData() ));
    connect(m_treeView, SIGNAL( itemDoubleClicked(QTreeWidgetItem *, int ) ), this, SLOT(slotTreeViewDoubleClick(QTreeWidgetItem *, int ) ));
    connect(m_treeView, SIGNAL( itemSelectionChanged() ), this, SLOT( onTreeViewSelectionChanged() ));
    connect(m_treeView, SIGNAL( signalAddLinePlot(const std::vector<resultsviewer::ResultsViewerPlotData>& ) ), this, SLOT( slotAddLinePlot(const std::vector<resultsviewer::ResultsViewerPlotData>& ) ) );
    connect(m_treeView, SIGNAL( signalAddFloodPlot(const std::vector<resultsviewer::ResultsViewerPlotData>& ) ), this, SLOT( slotAddFloodPlot(const std::vector<resultsviewer::ResultsViewerPlotData>& ) ) );
    connect(m_treeView, SIGNAL( signalAddLinePlotComparison(const std::vector<resultsviewer::ResultsViewerPlotData>& ) ), this, SLOT( slotAddLinePlotComparison(const std::vector<resultsviewer::ResultsViewerPlotData>& ) ) );
    connect(m_treeView, SIGNAL( signalAddFloodPlotComparison(const std::vector<resultsviewer::ResultsViewerPlotData>& ) ), this, SLOT( slotAddFloodPlotComparison(const std::vector<resultsviewer::ResultsViewerPlotData>& ) ) );
    connect(m_treeView, SIGNAL( signalDragResultsViewerPlotData(const std::vector<resultsviewer::ResultsViewerPlotData>& ) ), this, SLOT( slotDragPlotViewData(const std::vector<resultsviewer::ResultsViewerPlotData>& ) ) );
    connect(m_treeView, SIGNAL( signalAddIlluminancePlot(const std::vector<resultsviewer::ResultsViewerPlotData>& ) ), this, SLOT( slotAddIlluminancePlot(const std::vector<resultsviewer::ResultsViewerPlotData>& ) ) );
    connect(m_treeView, SIGNAL( signalAddIlluminancePlotComparison(const std::vector<resultsviewer::ResultsViewerPlotData>& ) ), this, SLOT( slotAddIlluminancePlotComparison(const std::vector<resultsviewer::ResultsViewerPlotData>& ) ) );
  }

  void MainWindow::slotApplyFilter(const QString& text)
  {
    QString filterText = text.trimmed();
    // trac 1380 exact match (case insensitive) if double quotes - wildcard front and back if not - single words/phrases only
    // no AND, OR searching and searches all fields
    if (!(filterText.startsWith("\"") && filterText.endsWith("\"")))
      filterText = "*" + filterText.replace("\"","") + "*";
    else // exact phrase
      filterText = filterText.replace("\"","");
    if (filterText.size() > 0)
    {
      m_tableView->applyFilter(filterText);
      m_treeView->applyFilter(filterText);
    }
  }

  void MainWindow::slotClearFilter()
  {
    m_viewFilterText->clear();
    m_tableView->clearFilter();
    m_treeView->clearFilter();
  }

  void MainWindow::slotUpdateLastImageSavedPath(QString &path)
  {
    if (!path.isEmpty()) m_lastImageSavedPath = path;
    // can update all plotviews here for same path
  }


};


