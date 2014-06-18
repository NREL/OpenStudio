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
#include "FileSystemSearch.hpp"
#include "../lib/JobFactory.hpp"
#include "../lib/Workflow.hpp"

#include "../../utilities/core/Application.hpp"
#include "../../utilities/plot/ProgressBar.hpp"

#include <QDesktopServices>
#include <QDirModel>
#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>
#include <QToolButton>
#include <QUrl>

namespace openstudio {
namespace runmanager {

  QVariant CompleterModel::data(const QModelIndex &index, int role) const
  {
    if (role == Qt::DisplayRole && index.column() == 0) {
      QString path  = QDir::toNativeSeparators(filePath(index));
      if (path.endsWith(QDir::separator()))
        path.chop(1);
      return path;
    }

    return QFileSystemModel::data(index, role);
  }


  MainWindow::MainWindow(QWidget *parent, Qt::WindowFlags flags, bool newdb)
    : QMainWindow(parent, flags), m_runmanager(openstudio::runmanager::RunManager::defaultDBLocation(), newdb, true),
      m_fss(openstudio::path(), false, "", QRegExp())
  {
    ui.setupUi(this);

#ifdef Q_OS_LINUX
  QIcon icon = QIcon(":/images/rm_16.png");
  icon.addPixmap(QPixmap(":/images/rm_32.png"));
  icon.addPixmap(QPixmap(":/images/rm_48.png"));
  icon.addPixmap(QPixmap(":/images/rm_64.png"));
  icon.addPixmap(QPixmap(":/images/rm_128.png"));
  icon.addPixmap(QPixmap(":/images/rm_256.png"));
  setWindowIcon(icon);
#endif

    m_selectedMessage = std::shared_ptr<StatusBarMessage>(new StatusBarMessage("0 files selected.", statusBar()));
    m_regexMessage = std::shared_ptr<StatusBarMessage>(new StatusBarMessage("", statusBar()));

    connect(ui.tabWidget, SIGNAL(currentChanged(int)), this, SLOT(updateToolbar(int)));


    connect(ui.actionEPWBrowse, SIGNAL(triggered()), this, SLOT(epwBrowse()));
    connect(ui.actionRunIDFJob, SIGNAL(triggered()), this, SLOT(runSimulation()));
    connect(ui.actionAbout, SIGNAL(triggered()), this, SLOT(helpAbout()));
    connect(ui.actionConfig, SIGNAL(triggered()), this, SLOT(setConfigOptions()));


    connect(ui.actionRestart_Out_Of_Date, SIGNAL(triggered()), this, SLOT(restartOutOfDate()));
    connect(ui.actionRMStatistics, SIGNAL(triggered()), this, SLOT(showStatistics()));
    connect(ui.actionDocumentationOnline, SIGNAL(triggered()), this, SLOT(helpDocumentationOnline()));

    connect(ui.actionFilesCheckAll, SIGNAL(triggered()), this, SLOT(filesCheckAll()));
    connect(ui.actionFilesCheckNone, SIGNAL(triggered()), this, SLOT(filesCheckNone()));
    connect(ui.actionFilesCheckSelected, SIGNAL(triggered()), this, SLOT(filesCheckSelected()));
    connect(ui.actionFilesInvertChecked, SIGNAL(triggered()), this, SLOT(filesInvertChecked()));

    connect(ui.actionFilesAddToQueue, SIGNAL(triggered()), this, SLOT(filesAddToQueue()));
    connect(ui.actionFilesAddCheckedToQueue, SIGNAL(triggered()), this, SLOT(filesAddCheckedToQueue()));
    connect(ui.actionFilesOpenFolder, SIGNAL(triggered()), this, SLOT(filesOpenFolder()));

    connect(ui.actionSettings_File, SIGNAL(triggered()), this, SLOT(settingsFileInfo()));

    connect(ui.actionFilesSelectAll, SIGNAL(triggered()), ui.listFiles, SLOT(selectAll()));
    connect(ui.actionFilesSelectNone, SIGNAL(triggered()), ui.listFiles, SLOT(clearSelection()));

    connect(ui.treeFileSystem, SIGNAL(activated(const QModelIndex &)), this, SLOT(dirActivated(const QModelIndex &)));
    connect(ui.cbRegex, SIGNAL(stateChanged(int)), this, SLOT(regexStateChanged(int)));
    connect(ui.cbRecursive, SIGNAL(stateChanged(int)), this, SLOT(recursiveStateChanged(int)));
    connect(ui.txtSearchPattern, SIGNAL(textChanged(const QString &)),
        this, SLOT(searchPatternChanged(const QString &)));
    connect(ui.txtRootPath, SIGNAL(textChanged(const QString &)),
        this, SLOT(txtRootPathChanged(const QString &)));

    connect(ui.rbIDF, SIGNAL(toggled(bool)), this, SLOT(fileTypeChanged(bool)));
    connect(ui.rbOSM, SIGNAL(toggled(bool)), this, SLOT(fileTypeChanged(bool)));
    connect(ui.rbXML, SIGNAL(toggled(bool)), this, SLOT(fileTypeChanged(bool)));

    connect(ui.actionOpenFolder, SIGNAL(triggered()), this, SLOT(openFolder()));
    connect(ui.actionSearchFolder, SIGNAL(triggered()), this, SLOT(searchFolder()));

    connect(&m_fss, SIGNAL(searchComplete()), this, SLOT(fileSearchComplete()));
    connect(&m_fss, SIGNAL(searchStarted()), this, SLOT(fileSearchStarted()));
    connect(&m_fss, SIGNAL(errorInRegex(const QString &)), this, SLOT(errorInRegex(const QString &)));
    connect(&m_fss, SIGNAL(rootPathChanged(const openstudio::path &)), this, SLOT(rootPathChanged(const openstudio::path &)));

    connect(ui.treeFileSystem, SIGNAL(expanded(const QModelIndex &)), this, SLOT(dirExpanded(const QModelIndex &)));
    ui.treeFileSystem->addAction(ui.actionOpenFolder);
    ui.treeFileSystem->addAction(ui.actionSearchFolder);

    m_fsmodel.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
    m_fsmodel.setRootPath("");

    m_completer.setModel(new QDirModel(QStringList(), QDir::AllDirs, QDir::Name, &m_completer));
    ui.txtRootPath->setCompleter(&m_completer);


    ui.treeFileSystem->setModel(&m_fsmodel);

    for( int i = 1; i < 10; ++i)
    {
      ui.treeFileSystem->setColumnHidden(i, true);
    }

    ui.listFiles->setModel(m_fss.getQItemModel());

    connect(ui.listFiles->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
        this, SLOT(filesSelectionChanged()));

    ui.comboBoxEPWFiles->insertItem(0, "");

    try {
      m_fss.setRootPath(m_runmanager.getConfigOptions().getDefaultIDFLocation());
    } catch (const std::runtime_error &) {
      LOG(Error, "Unable to read default IDF Location: " << toString(m_runmanager.getConfigOptions().getDefaultIDFLocation()));
    }


    m_statusWidget = new openstudio::runmanager::RunManagerStatusWidget(ui.tab_2);
    m_statusWidget->setRunManager(m_runmanager);
    m_statusWidget->setGoToOutputEnabled(true);
    ui.verticalLayout_5->addWidget(m_statusWidget);

    connect(m_statusWidget, SIGNAL(goToOutput()), this, SLOT(goToOutput()));

    m_jobStatusWidget = new openstudio::runmanager::JobStatusWidget(m_runmanager, ui.tab_3);
    ui.verticalLayout_3->addWidget(m_jobStatusWidget);

    connect(m_statusWidget, SIGNAL(jobSelectionCleared()), m_jobStatusWidget, SLOT(clearWorkflow()));
    connect(m_statusWidget, SIGNAL(jobSelectionChanged(const openstudio::runmanager::Job &)),
        m_jobStatusWidget, SLOT(setWorkflow(const openstudio::runmanager::Job &)));

    fileTypeChanged(true);

    updateToolbar(0);

    readSettings();

    std::vector<runmanager::ToolInfo> tools = m_runmanager.getConfigOptions().getTools().tools();

    std::set<std::string> badtools;
    for (std::vector<runmanager::ToolInfo>::const_iterator itr = tools.begin();
         itr != tools.end();
         ++itr)
    {
      if (!boost::filesystem::exists(itr->localBinPath))
      {
        badtools.insert(toString(itr->localBinPath.native()));
      }
    }

    if (!badtools.empty())
    {
      std::string paths;
      for (const auto & badtool : badtools)
      {
        paths += "\n\t" + badtool;
      }

      QMessageBox::warning(this,
        "Missing Tools",
        toQString("The following configured tools could not be found, please check your configuration, or install the missing tools.\n" + paths));
    }

    int size1 = tools.size();
    int size2 = badtools.size();

    if (size1 - size2 <= 0)
    {
      if (QMessageBox::question(this, 
        "No Configured Tools",
        "There are no valid tools configured, would you like to automatically search the local file system for tools?",
        QMessageBox::Ok | QMessageBox::Cancel)
          == QMessageBox::Ok)
      {
        ConfigOptions co = m_runmanager.getConfigOptions();
        co.findTools(true, false, false, false);
        m_runmanager.setConfigOptions(co);

        QMessageBox::information(this, 
            "Tools Found",
            QString::number(co.getToolLocations().size()) + " tool" + (co.getToolLocations().size()!=1?"s":"") + " found");

        m_runmanager.showConfigGui(this);
      }

    }

  }


  MainWindow::~MainWindow()
  {
  }

  void MainWindow::updateToolbar(int index)
  {
    bool files = index == 0;
    bool runmanager = index == 1;
    bool jobstatus = index == 2;

    std::vector<std::pair<bool, QAction *> > runmanagerstatusactions = m_statusWidget->publicActions();

    for (size_t i = 0; i< runmanagerstatusactions.size(); ++i)
    {
      if (runmanagerstatusactions[i].first || runmanager)
      {
        ui.toolBar->addAction(runmanagerstatusactions.at(i).second);
      } else {
        ui.toolBar->removeAction(runmanagerstatusactions.at(i).second);
      }

      if (runmanagerstatusactions[i].first)
      {
        ui.menuSimulation->addAction(runmanagerstatusactions[i].second);
      }
    }

    std::vector<std::pair<bool, QAction *> > jobstatusactions = m_jobStatusWidget->publicActions();


    for (size_t i = 0; i< jobstatusactions.size(); ++i)
    {
      if (jobstatusactions[i].first || jobstatus)
      {
        ui.toolBar->addAction(jobstatusactions.at(i).second);
      } else {
        ui.toolBar->removeAction(jobstatusactions.at(i).second);
      }

      if (jobstatusactions[i].first)
      {
        ui.menuSimulation->addAction(jobstatusactions[i].second);
      }
    }

    if (files)
    {
      ui.toolBar->addAction(ui.actionFilesSelectAll);
      ui.toolBar->addAction(ui.actionFilesSelectNone);
      ui.toolBar->addAction(ui.actionFilesCheckAll);
      ui.toolBar->addAction(ui.actionFilesCheckNone);
      ui.toolBar->addAction(ui.actionFilesCheckSelected);
      ui.toolBar->addAction(ui.actionFilesInvertChecked);
      ui.toolBar->addAction(ui.actionFilesAddToQueue);
      ui.toolBar->addAction(ui.actionFilesAddCheckedToQueue);
      ui.toolBar->addAction(ui.actionFilesOpenFolder);
    } else {
      ui.toolBar->removeAction(ui.actionFilesSelectAll);
      ui.toolBar->removeAction(ui.actionFilesSelectNone);
      ui.toolBar->removeAction(ui.actionFilesCheckAll);
      ui.toolBar->removeAction(ui.actionFilesCheckNone);
      ui.toolBar->removeAction(ui.actionFilesCheckSelected);
      ui.toolBar->removeAction(ui.actionFilesInvertChecked);
      ui.toolBar->removeAction(ui.actionFilesAddToQueue);
      ui.toolBar->removeAction(ui.actionFilesAddCheckedToQueue);
      ui.toolBar->removeAction(ui.actionFilesOpenFolder);
    }

  }

  MainWindow::StatusBarMessage::StatusBarMessage(const QString &value, QStatusBar *bar)
    : m_bar(bar), m_line(new QFrame()), m_label(new QLabel(value))
  {
    assert(bar);
    m_line->setFrameStyle(QFrame::VLine | QFrame::Plain);
    m_bar->addPermanentWidget(m_line);
    m_bar->addPermanentWidget(m_label);
  }

  void MainWindow::StatusBarMessage::update(const QString &lbl)
  {
    m_label->setText(lbl);
    if (lbl.isEmpty())
    {
      m_line->hide();
      m_label->hide();
    } else {
      m_line->show();
      m_label->show();
    }
  }

  MainWindow::StatusBarMessage::~StatusBarMessage()
  {
    m_bar->removeWidget(m_line);
    m_bar->removeWidget(m_label);

    delete m_line;
    delete m_label;
  }

  MainWindow::StatusBarProgress::StatusBarProgress(const QString &name, QStatusBar *bar)
    : m_bar(bar), m_line(new QFrame()), m_label(new QLabel(name)), m_progress(new QProgressBar())
  {
    m_line->setFrameStyle(QFrame::VLine | QFrame::Plain);
    m_bar->addPermanentWidget(m_line);
    m_bar->addPermanentWidget(m_label);
    m_bar->addPermanentWidget(m_progress);
  }

  void MainWindow::StatusBarProgress::update(int min, int max, int current)
  {
    m_progress->setRange(min, max);
    m_progress->setValue(current);
  }

  void MainWindow::StatusBarProgress::update(int value)
  {
    m_progress->setValue(value);
  }

  MainWindow::StatusBarProgress::~StatusBarProgress()
  {
    m_bar->removeWidget(m_line);
    m_bar->removeWidget(m_label);
    m_bar->removeWidget(m_progress);

    delete m_line;
    delete m_label;
    delete m_progress;
  }


  void MainWindow::errorInRegex(const QString &msg)
  {
    if (!msg.isEmpty())
    {
      m_regexMessage->update("Filter Error: " + msg);
    } else {
      m_regexMessage->update("");
    }
  }

  void MainWindow::filesSelectionChanged()
  {
    const int selected = ui.listFiles->selectionModel()->selectedRows().size();

    QString message = QString::number(selected) + " file";

    if (selected == 0 || selected > 1)
    {
      message += "s";
    }

    message += " selected.";

    LOG(Debug, "Updating selected message " << toString(message));

    m_selectedMessage->update(message);
  }

  void MainWindow::settingsFileInfo()
  {
    if (QMessageBox::information(this, "Settings File", QString("The settings and job queue are currently stored in the file: ") 
          + toQString(m_runmanager.dbPath().native()),
          QMessageBox::Ok | QMessageBox::RestoreDefaults)
          == QMessageBox::RestoreDefaults)
    {
      if (QMessageBox::question(this, "Restore Defaults", "Are you sure you would like to restore defaults? The application will close and restart with an empty job queue and default settings.", QMessageBox::Ok | QMessageBox::Cancel)
          == QMessageBox::Ok)
      {
        Application::instance().application()->exit(-1);
      }

    }
  }

  void MainWindow::openFolder()
  {
    QModelIndex i = ui.treeFileSystem->currentIndex();
    if (i.isValid())
    {
      openFile(m_fsmodel.filePath(i));
    }
  }

  void MainWindow::openFile(const QString &p)
  {
    QUrl qurl1 = QUrl::fromLocalFile(p);

    LOG(Debug, "Opening folder: " << openstudio::toString(qurl1.toString()));

    if (!QDesktopServices::openUrl(qurl1)) {
      QMessageBox::critical(this, "Unable to launch viewer", "No operating system registered viewer for: " + p);
    }
  }

  void MainWindow::txtRootPathChanged(const QString &txt)
  {
    m_fss.setRootPath(toPath(txt));
  }

  void MainWindow::rootPathChanged(const openstudio::path &p)
  {
    if (toPath(ui.txtRootPath->text()) != p)
    {
      ui.txtRootPath->setText(toQString(p.native()));
    }
  }

  void MainWindow::searchFolder()
  {
    QModelIndex i = ui.treeFileSystem->currentIndex();
    if (i.isValid())
    {
      m_fss.setRootPath(openstudio::toPath(m_fsmodel.filePath(i)));
    }
  }

  void MainWindow::goToOutput()
  {
    ui.tabWidget->setCurrentIndex(2);
  }

  void MainWindow::filesAddToQueue()
  {
    QModelIndex i = ui.listFiles->selectionModel()->currentIndex();
    if (i.isValid())
    {
      std::vector<openstudio::path> files;
      files.push_back(m_fss.getFile(i));

      queueSimulations(files);
    }
  }

  void MainWindow::filesOpenFolder()
  {
    openFile(toQString(m_fss.rootPath()));
  }


  void MainWindow::filesAddCheckedToQueue()
  {
    runSimulation();
  }

  void MainWindow::filesCheckAll()
  {
    m_fss.selectAll();
  }

  void MainWindow::filesCheckNone()
  {
    m_fss.selectNone();
  }

  void MainWindow::filesInvertChecked()
  {
    m_fss.invertSelection();
  }

  void MainWindow::filesCheckSelected()
  {
    m_fss.select(ui.listFiles->selectionModel()->selectedRows());
  }

  void MainWindow::enableWeather(bool enabled)
  {
    ui.btnEPWBrowse->setEnabled(enabled);
    ui.comboBoxEPWFiles->setEnabled(enabled);
  }

  void MainWindow::fileTypeChanged(bool state)
  {
    if (state)
    {
      QString extension;

      // We are in the callback for the one that was turned on
      // let's update the file types here
      if (ui.rbIDF->isChecked())
      {
        enableWeather(true);

        extension = "idf";

        ui.cbWorkflow->clear();
        ui.cbWorkflow->addItem("energyplus");
        ui.cbWorkflow->addItem("expandobjects->energyplus");
      } else if (ui.rbOSM->isChecked()) {
        enableWeather(false);

        extension = "osm";

        ui.cbWorkflow->clear();
        ui.cbWorkflow->addItem("modeltoidf");
        ui.cbWorkflow->addItem("modeltoidf->energyplus");
        ui.cbWorkflow->addItem("modeltoidf->expandobjects->energyplus");
        ui.cbWorkflow->addItem("<Custom>");
      } else if (ui.rbXML->isChecked()) {
        enableWeather(true);

        extension = "xml";

        ui.cbWorkflow->clear();
        ui.cbWorkflow->addItem("xmlpreprocessor");
        ui.cbWorkflow->addItem("xmlpreprocessor->energyplus");
      }


      if (ui.cbRegex->checkState() == Qt::Checked)
      {
        ui.txtSearchPattern->setText(".*\\." + extension + "$");
      } else {
        ui.txtSearchPattern->setText("*." + extension);
      }

    }
  }

  void MainWindow::dirExpanded(const QModelIndex &)
  {
    ui.treeFileSystem->resizeColumnToContents(0);
  }

  void MainWindow::fileSearchComplete()
  {
    ui.listFiles->resizeColumnToContents(0);
    statusBar()->showMessage("File search complete.");
  }

  void MainWindow::fileSearchStarted()
  {
    statusBar()->showMessage("Searching for matching files...");
  }

  void MainWindow::restartOutOfDate()
  {
    m_runmanager.setOutOfDateRunnable();
  }

  void MainWindow::showStatistics()
  {
    std::map<std::string, double> stats = m_runmanager.statistics();

    QString strstats;

//    strstats = "\\rtf1\\ansi\\deff0\n\\trowd\n\\cellx2000\n\\cellx2000 \n";
    strstats = "<table>";

    for (std::map<std::string, double>::const_iterator itr = stats.begin();
         itr != stats.end();
         ++itr)
    {
      strstats += "<tr>";
      strstats += "<td>" + toQString(itr->first) + "</td>";
      strstats += "<td>" + QString::number(itr->second) + "</td>";
      strstats += "</tr>";
    }

    strstats += "</table>";

    QMessageBox::information(this, "RunManager Job Statistics",
      strstats);
  }

  void MainWindow::searchPatternChanged(const QString &pattern)
  {
    QRegExp regex;
    if (!pattern.isEmpty())
    {

      if (ui.cbRegex->checkState() != Qt::Checked)
      {
#ifdef Q_OS_WIN
        QString cleanedpattern = pattern;
        cleanedpattern.replace("\\", "[\\\\]");
        QString appendedpattern = "*" + cleanedpattern + "*";
        regex = QRegExp(appendedpattern, Qt::CaseInsensitive, QRegExp::Wildcard);
#else
        QString appendedpattern = "*" + pattern + "*";
        regex = QRegExp( appendedpattern, Qt::CaseInsensitive, QRegExp::WildcardUnix);
#endif
      } else {
        QString appendedpattern = ".*" + pattern + ".*";
        regex = QRegExp(appendedpattern, Qt::CaseInsensitive, QRegExp::RegExp2);
      }
    } else {
      regex = QRegExp("*", Qt::CaseInsensitive, QRegExp::Wildcard);
    }

    regex.setMinimal(true);
    m_fss.setRegex(regex);
  }

  void MainWindow::regexStateChanged(int state)
  {
    searchPatternChanged(ui.txtSearchPattern->text());
  }


  void MainWindow::recursiveStateChanged(int state)
  {
    m_fss.setRecursive(state == Qt::Checked);
  }

  void MainWindow::dirActivated(const QModelIndex &index)
  {
    openstudio::path p = openstudio::toPath(m_fsmodel.filePath(index));

    m_fss.setRootPath(p);
  }

  void MainWindow::helpDocumentationOnline()
  {
    QDesktopServices::openUrl(QUrl("https://openstudio.nrel.gov/runmanager-getting-started"));
  }

  void MainWindow::closeEvent(QCloseEvent *event)
  {
    QSettings settings("OpenStudio", "RunManager");
    settings.setValue("geometry", saveGeometry());
    settings.setValue("windowState", saveState());
    QMainWindow::closeEvent(event);
  }

  void MainWindow::readSettings()
  {
    QSettings settings("OpenStudio", "RunManager");
    restoreGeometry(settings.value("geometry").toByteArray());
    restoreState(settings.value("windowState").toByteArray());
  }


  void MainWindow::setConfigOptions()
  {
    m_runmanager.showConfigGui(this);
  }

  openstudio::path MainWindow::getOutDir(const openstudio::path &t_file, const openstudio::runmanager::Workflow &t_wf, const std::string &t_jobstring, bool t_simpleName)
  {
    openstudio::path outpath = m_runmanager.getConfigOptions().getOutputLocation();
    openstudio::path parentdir = t_file.parent_path();
    openstudio::path jobname;
    openstudio::path tailpathname = toPath("workflow-" + (t_simpleName?t_jobstring:t_wf.key()));

    QString jobstring = toQString(t_jobstring);
    jobstring.remove(">");

    if (openstudio::toString(t_file.stem()) == "in")
    {
      // The file is called "in", let's use the parent path's name as our job name
      jobname = parentdir.filename();
    } else {
      // Let's use the stem of the file name to create a working dir, since it's a meaningful name
      jobname = t_file.stem();     
    }

    //std::string workflowkey = "workflow-" + t_wf.key();

    if (outpath.empty())
    {
      if (openstudio::toString(t_file.stem()) == "in")
      {
        return parentdir / tailpathname; // if we are running in the parent dir, and our name is "in" no reason to make a subdir
      } else {
        return parentdir / jobname / tailpathname; // we are either reconstructing the path it was already in, or making a new one based on the file name
      }

    } else {
      return outpath / jobname / tailpathname; // use the outpath
    }

  }


  
  void MainWindow::queueSimulation(const openstudio::path &t_input, const openstudio::path &t_epw)
  {
    try {
      // Build list of jobs to create
      std::string jobsstring = toString(ui.cbWorkflow->currentText());

      if (jobsstring == "<Custom>")
      {
        openstudio::path expecteddb = t_input.parent_path() / t_input.stem() / openstudio::toPath("run.db");

        if (!boost::filesystem::exists(expecteddb))
        {
          QMessageBox::critical(this, "Unable to launch job", "Db containing custom workflow for OpenStudio UI created OSM not found at the expected location: " + openstudio::toQString(expecteddb));
        } else {
          try {
            m_runmanager.loadJobs(expecteddb);
            statusBar()->showMessage("Job Queued - " + openstudio::toQString(t_input));
          } catch (const std::exception &e) {
            QMessageBox::critical(this, "Unable to launch job", e.what());
          }
        }
      } else {
        // parse the string list with the workflow constructor
        openstudio::runmanager::Workflow workflow(jobsstring);

        // Build list of tools
        openstudio::runmanager::Tools tools;
        ConfigOptions co = m_runmanager.getConfigOptions();
        tools.append(co.getTools());
        workflow.add(tools);

        workflow.setInputFiles(t_input, t_epw);
        openstudio::runmanager::Job job = workflow.create(getOutDir(t_input, workflow, jobsstring, co.getSimpleName()));

        m_runmanager.enqueue(job, false);
        statusBar()->showMessage("Job Queued - " + openstudio::toQString(job.description()));
      }
    } catch (const std::exception &e) {
      QMessageBox::critical(this, "Unable to launch job", e.what());
    }
  }

  void MainWindow::runSimulation()
  {
    std::vector<openstudio::path> files = m_fss.selectedFiles();

    queueSimulations(files);
  }

  void MainWindow::queueSimulations(const std::vector<openstudio::path> &t_files)
  {
    if (t_files.empty())
    {
      QMessageBox::information(this, "No Files Selected", "Please select files for simulation");
      return;
    }


    openstudio::path epw;
    QString jobtype;
    bool wantsepw = false;

    if (ui.rbIDF->isChecked())
    {
      epw = openstudio::toPath(ui.comboBoxEPWFiles->currentText());
      wantsepw = true;
      jobtype = "IDF";
    } else if (ui.rbOSM->isChecked()) {
      wantsepw = false;
      jobtype = "OSM";
    } else if (ui.rbXML->isChecked()) {
      epw = openstudio::toPath(ui.comboBoxEPWFiles->currentText());
      wantsepw = true;
      jobtype = "XML";
    } else {
      assert("!Unknown job type");
    }

    if (wantsepw)
    {
      if (epw.empty())
      {
        if (QMessageBox::warning(this, "No EPW Selected", 
              "No EPW is selected, the application will try to find a matching epw from the default epw directory", 
              QMessageBox::Ok | QMessageBox::Cancel) == QMessageBox::Cancel)
        {
          return;
        }
        epw = m_runmanager.getConfigOptions().getDefaultEPWLocation();
      }
    }

    bool runjobs = true;

    if (t_files.size() > 1)
    {
      QString text = "Add " + QString::number(t_files.size());
      text += " " + jobtype;
      text += " jobs to queue?";
      if (wantsepw)
      {
        text += "\n\nEPW: ";
        if (epw.empty())
        {
          text += "None";
        } else {
          text += toQString(epw.native());
        }
      }

      text += "\nWorkflow: " + ui.cbWorkflow->currentText();
      if (QMessageBox::question(this, "Add Jobs To Queue?", text, QMessageBox::Ok | QMessageBox::Cancel) == QMessageBox::Ok)
      {
        runjobs = true;
      } else {
        runjobs = false;
      }
    }

    if (runjobs)
    {
      setEnabled(false);
      StatusBarProgress sbp("Adding jobs to queue", statusBar());
      sbp.update(0, t_files.size()-1, 0);
      int place = 0;
      for (const auto & file : t_files)
      {
        queueSimulation(file, epw);
        ++place;
        sbp.update(place);
      }

//      ui.listFiles->clearSelection();
//      m_fss.selectNone();

      setEnabled(true);
    }
  }

  void MainWindow::helpAbout()
  {
    QMessageBox aboutBox(this);
    aboutBox.setText(RUNMANAGER_ABOUTBOX);
    aboutBox.setStyleSheet("qproperty-alignment: AlignCenter;");
    aboutBox.setWindowTitle("About RunManager");
    aboutBox.exec();
  }

  void MainWindow::epwBrowse()
  {
    updateFileSelection(QFileDialog::getOpenFileName(this, "Select Weather File", 
          toQString(m_runmanager.getConfigOptions().getDefaultEPWLocation()), "EnergyPlus Weather Files (*.epw)"), *ui.comboBoxEPWFiles);
  }

  void MainWindow::on_actionHtmlResults_triggered()
  {
    statusBar()->showMessage(tr("results triggered"));
  }


  void MainWindow::updateFileSelection(const QString &t_file, QComboBox &t_qcb)
  {
    if (!t_file.isEmpty())
    {
      QString filename = toQString(toPath(t_file).native());
      int i = t_qcb.findText(filename);
      if (i == -1)
      {
        t_qcb.addItem(filename);
        t_qcb.setCurrentIndex(t_qcb.count() - 1);
      } else {
        t_qcb.setCurrentIndex(i);
      }
    }
  }


} // runmanager
} // openstudio

