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

#include <openstudio_lib/RunTabView.hpp>

#include <openstudio_lib/FileOperations.hpp>
#include <openstudio_lib/OSAppBase.hpp>
#include <openstudio_lib/OSDocument.hpp>
#include <openstudio_lib/ScriptFolderListView.hpp>

#include <model/DaylightingControl.hpp>
#include <model/DaylightingControl_Impl.hpp>
#include <model/GlareSensor.hpp>
#include <model/GlareSensor_Impl.hpp>
#include <model/IlluminanceMap.hpp>
#include <model/IlluminanceMap_Impl.hpp>
#include <model/Model_Impl.hpp>
#include <model/Space.hpp>
#include <model/Space_Impl.hpp>
#include <model/ThermalZone.hpp>
#include <model/ThermalZone_Impl.hpp>

#include <runmanager/lib/JobStatusWidget.hpp>
#include <runmanager/lib/RubyJobUtils.hpp>
#include <runmanager/lib/RunManager.hpp>

#include <utilities/core/Application.hpp>
#include <utilities/core/ApplicationPathHelpers.hpp>
#include <utilities/sql/SqlFile.hpp>
#include <utilities/core/Assert.hpp>

#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>

#include <energyplus/ForwardTranslator.hpp>

#include <QButtonGroup>
#include <QDir>
#include <QGroupBox>
#include <QLabel>
#include <QMessageBox>
#include <QPainter>
#include <QPlainTextEdit>
#include <QProcess>
#include <QProgressBar>
#include <QPushButton>
#include <QRadioButton>
#include <QScrollArea>
#include <QStackedWidget>
#include <QStyleOption>
#include <QSysInfo>
#include <QToolButton>
#include <QVBoxLayout>

#define ENERGYPLUS_TEXT "EnergyPlus"
#define STD_RADIANCE_TEXT "Radiance (increases simulation time, but provides more accurate results)"
#define WARNING_RADIANCE_TEXT "Radiance (with warnings)"
#define ERROR_RADIANCE_TEXT "Radiance (has errors, unable to use)"

namespace openstudio {

RunTabView::RunTabView(const model::Model & model,
                       QWidget * parent)
  : MainTabView("Run Simulation",true,parent)
    //m_runView(new RunView(model)),
    //m_status(new openstudio::runmanager::JobStatusWidget(m_runView->runManager()))
{
  //addSubTab("Output", m_runView);
  //addSubTab("Tree", m_status);

  //connect(m_runView, SIGNAL(resultsGenerated(const openstudio::path &)),
  //    this, SIGNAL(resultsGenerated(const openstudio::path &)));
}

RunView::RunView(const model::Model & model,
                 const openstudio::path &t_modelPath,
                 const openstudio::path &t_tempFolder,
                 openstudio::runmanager::RunManager t_runManager)
  : m_model(model),
    m_modelPath(t_modelPath),
    m_tempFolder(t_tempFolder),
    m_canceling(false)

{
  bool isConnected = t_runManager.connect(SIGNAL(statsChanged()), this, SLOT(runManagerStatsChanged()));

  QGridLayout *mainLayout = new QGridLayout();
  mainLayout->setContentsMargins(5,5,5,5);
  mainLayout->setSpacing(5);
  setLayout(mainLayout);

  // Run / Play button area

  m_playButton = new QToolButton();
  m_playButton->setText("     Run");
  m_playButton->setCheckable(true);
  m_playButton->setChecked(false);
  QIcon playbuttonicon(QPixmap(":/images/run_simulation_button.png"));
  playbuttonicon.addPixmap(QPixmap(":/images/run_simulation_button.png"), QIcon::Normal, QIcon::Off);
  playbuttonicon.addPixmap(QPixmap(":/images/cancel_simulation_button.png"), QIcon::Normal, QIcon::On);
  m_playButton->setStyleSheet("QToolButton { background:transparent; font: bold; }");
  m_playButton->setIconSize(QSize(35,35));
  m_playButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  m_playButton->setIcon(playbuttonicon);
  m_playButton->setLayoutDirection(Qt::RightToLeft);
//  m_playButton->setStyleSheet("QAbstractButton:!hover { border: none; }");

  mainLayout->addWidget(m_playButton, 0, 0);
  isConnected = connect(m_playButton, SIGNAL(clicked(bool)), this, SLOT(playButtonClicked(bool)));
  OS_ASSERT(isConnected);

  // Progress bar area
  m_progressBar = new QProgressBar();
 
  QVBoxLayout *progressbarlayout = new QVBoxLayout();
  progressbarlayout->addWidget(m_progressBar);
  m_statusLabel = new QLabel("Ready");
  progressbarlayout->addWidget(m_statusLabel);
  mainLayout->addLayout(progressbarlayout, 0, 1);

  m_radianceGroup = new QButtonGroup(this);

  isConnected = connect(m_radianceGroup, SIGNAL(buttonClicked(int)),
    this, SLOT(on_radianceGroupClicked(int)));
  OS_ASSERT(isConnected);

  int buttonCount = 0;

  m_energyPlusButton = new QRadioButton(ENERGYPLUS_TEXT);
  m_radianceGroup->addButton(m_energyPlusButton,buttonCount++);

  m_radianceButton = new QRadioButton(STD_RADIANCE_TEXT);
  m_radianceGroup->addButton(m_radianceButton,buttonCount++);

  m_radianceWarningsAndErrorsButton = new QPushButton();
  m_radianceWarningsAndErrorsButton->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Preferred);
  m_radianceWarningsAndErrorsButton->hide();

  isConnected = connect(m_radianceWarningsAndErrorsButton, SIGNAL(clicked(bool)),
                        this, SLOT(on_radianceWarningsAndErrorsClicked(bool)));
  OS_ASSERT(isConnected);

  QHBoxLayout * radianceHLayout = new QHBoxLayout();
  radianceHLayout->addWidget(m_radianceButton);
  radianceHLayout->addWidget(m_radianceWarningsAndErrorsButton);
  radianceHLayout->addStretch();

  QVBoxLayout * radianceVLayout = new QVBoxLayout();
  radianceVLayout->addWidget(m_energyPlusButton);
  radianceVLayout->addLayout(radianceHLayout);

  QGroupBox * groupBox = new QGroupBox("For Daylighting Calculation use");
  groupBox->setLayout(radianceVLayout);

  mainLayout->addWidget(groupBox, 1, 1);

  if (usesRadianceForDaylightCalculations(t_runManager))
  {
    m_radianceButton->setChecked(true);
  } else {
    m_energyPlusButton->setChecked(true);
  }

  m_toolWarningLabel = new QLabel(openstudio::toQString("<b>Notice:</b> EnergyPlus " + getRequiredEnergyPlusVersion().toString() + " is required and not yet located.  Run File->Scan For Tools to locate."));
  m_toolWarningLabel->hide();

  mainLayout->addWidget(m_toolWarningLabel, 2, 1);

  locateEnergyPlus();

  m_warningsLabel = new QLabel("<b>Warnings:</b> 0");
  m_errorsLabel = new QLabel("<b>Errors:</b> 0");
  mainLayout->addWidget(m_warningsLabel, 3, 1);
  mainLayout->addWidget(m_errorsLabel, 4, 1);
  mainLayout->addWidget(new QLabel("Output"), 5, 1);
  m_outputWindow = new QPlainTextEdit();
  m_outputWindow->setReadOnly(true);
  mainLayout->addWidget(m_outputWindow, 6, 1);

  // Open RunManager externally button area
  QPushButton *openrunmanagerbutton = new QPushButton("Open RunManager\nfor Multiple Runs");
  mainLayout->addWidget(openrunmanagerbutton, 0, 2);
  isConnected = connect(openrunmanagerbutton, SIGNAL(clicked(bool)), this, SLOT(openRunManagerClicked()));
  OS_ASSERT(isConnected);


  updateRunManagerStats(t_runManager);
}

void RunView::getRadiancePreRunWarningsAndErrors(std::vector<std::string> & warnings,
                                                 std::vector<std::string> & errors)
{
  warnings.clear();
  errors.clear();

  openstudio::runmanager::ConfigOptions co(true);
  runManager().setConfigOptions(co);
  bool ruby_not_installed = co.getTools().getAllByName("ruby").tools().size() == 0;
  bool radiance_not_installed = co.getTools().getAllByName("rtrace").tools().size() == 0;
  
  if(radiance_not_installed){
    errors.push_back("Radiance is required, but not installed.");
  }

  if(ruby_not_installed){
    errors.push_back("Ruby is required, but not installed.");
  }
  
  // TODO remove when fixed
  #if defined(Q_OS_WIN)
  if(QSysInfo::windowsVersion() == QSysInfo::WV_XP){
    errors.push_back("OpenStudio is currently unable to run Radiance on XP operating systems.");
  }
  #endif

  // ThermalZone
  std::vector<model::ThermalZone> thermalZones = m_model.getModelObjects<model::ThermalZone>();
  if(thermalZones.size() > 0){
    //BOOST_FOREACH(model::ThermalZone thermalZone, thermalZones){
    //  std::vector<model::Space> spaces = thermalZone.spaces();
    //  if(spaces.size() > 0){
    //  }
    //  boost::optional<model::IlluminanceMap> illuminanceMap = thermalZone.illuminanceMap();
    //  if(illuminanceMap){
    //  }
    //  boost::optional<model::DaylightingControl> primaryDaylightingControl = thermalZone.primaryDaylightingControl();
    //  if(primaryDaylightingControl){
    //  }
    //}
  }
  else{
    errors.push_back("The OpenStudio model has no ThermalZone objects.");
  }

  // Space
  std::vector<model::Space> spaces = m_model.getModelObjects<model::Space>();
  if(spaces.size() == 0){
    errors.push_back("The OpenStudio model has no Space objects.");
  }

  // IlluminanceMap
  std::vector<model::IlluminanceMap> illuminanceMaps = m_model.getModelObjects<model::IlluminanceMap>();
  if(illuminanceMaps.size() > 0){
    BOOST_FOREACH(model::IlluminanceMap illuminanceMap, illuminanceMaps){
      boost::optional<model::Space> space = illuminanceMap.space();
      if(!space){
        errors.push_back("An OpenStudio model IlluminanceMap object is not assigned to a Space object.");
        break;
      }
      else{
        //boost::optional<model::ThermalZone> thermalZone = space->thermalZone();
        //std::vector<model::DaylightingControl> daylightingControls = space->daylightingControls();
        //std::vector<model::IlluminanceMap> illuminanceMaps = space->illuminanceMaps();
        //std::vector<model::GlareSensor> glareSensors = space->glareSensors();
      }
    }
  }
  else{
    errors.push_back("The OpenStudio model has no IlluminanceMap objects.");
  }

  // DaylightingControl
  std::vector<model::DaylightingControl> daylightingControls = m_model.getModelObjects<model::DaylightingControl>();
  if(daylightingControls.size() > 0){
    BOOST_FOREACH(model::DaylightingControl daylightingControl, daylightingControls){
      boost::optional<model::Space> space = daylightingControl.space();
      if(!space){
        errors.push_back("An OpenStudio model DaylightingControl object is not assigned to a Space object.");
        break;
      }
    }
  }
  else{
    errors.push_back("The OpenStudio model has no DaylightingControl objects.");
  }

  // GlareSensor
  std::vector<model::GlareSensor> glareSensors = m_model.getModelObjects<model::GlareSensor>();
  if(glareSensors.size() == 0){
    warnings.push_back("The OpenStudio model has no GlareSensor objects.");
  }

}

void RunView::locateEnergyPlus()
{
  openstudio::runmanager::ConfigOptions co(true);
  boost::optional<int> major = getRequiredEnergyPlusVersion().getMajor();
  boost::optional<int> minor = getRequiredEnergyPlusVersion().getMinor();
  bool energyplus_not_installed;
  if (major && minor){
    energyplus_not_installed = co.getTools().getAllByName("energyplus").getAllByVersion(openstudio::runmanager::ToolVersion(*major,*minor)).tools().size() == 0;
  } else {
    energyplus_not_installed = co.getTools().getAllByName("energyplus").getAllByVersion(openstudio::runmanager::ToolVersion(8,0)).tools().size() == 0;
  }
  
  if (energyplus_not_installed){
    m_toolWarningLabel->show();
  } else {
    m_toolWarningLabel->hide();
  }
}

void RunView::updateToolsWarnings()
{
  LOG(Debug, "updateToolsWarnings called");
  
  getRadiancePreRunWarningsAndErrors(m_radianceWarnings,m_radianceErrors);

  QString checkBoxText;
  QString buttonText;

  if(m_radianceErrors.size() > 0){
    checkBoxText = ERROR_RADIANCE_TEXT;
    m_energyPlusButton->setChecked(true);
    buttonText = "View errors";
    m_radianceWarningsAndErrorsButton->show();
  }
  else if(m_radianceWarnings.size() > 0){
    checkBoxText = WARNING_RADIANCE_TEXT;
    buttonText = "View warnings";
    m_radianceWarningsAndErrorsButton->show();
  }
  else{
    checkBoxText = STD_RADIANCE_TEXT;
    m_radianceWarningsAndErrorsButton->hide();
  }

  m_radianceWarningsAndErrorsButton->setText(buttonText);
  m_radianceButton->setText(checkBoxText);

  locateEnergyPlus();
}

void RunView::outputDataAdded(const openstudio::UUID &, const std::string &t_data)
{
  QTextCursor cursor = m_outputWindow->textCursor();
  cursor.movePosition(QTextCursor::End);
  cursor.insertText(openstudio::toQString(t_data));
  m_outputWindow->ensureCursorVisible();
}

void RunView::updateRunManagerStats(openstudio::runmanager::RunManager t_runManager)
{
  double numberofjobs = 0;
  double completedjobs = 0;
  double totalerrors = 0;
  double totalwarnings = 0;

  std::map<std::string, double> stats = t_runManager.statistics();
  numberofjobs = stats["Number of Jobs"];
  completedjobs = stats["Completed Jobs"];
  totalerrors = stats["Total Errors"];
  totalwarnings = stats["Total Warnings"];

  if (numberofjobs == 0) numberofjobs = 1;

  m_progressBar->setRange(0, numberofjobs);
  m_progressBar->setValue(completedjobs);

  m_warningsLabel->setText(openstudio::toQString("<b>Warnings:</b> " + boost::lexical_cast<std::string>(int(totalwarnings))));
  m_errorsLabel->setText(openstudio::toQString("<b>Errors:</b> " + boost::lexical_cast<std::string>(int(totalerrors))));
}

void RunView::runManagerStatsChanged()
{
  updateRunManagerStats(runManager());
}

void RunView::runFinished(const openstudio::path &t_sqlFile, const openstudio::path &t_radianceOutputPath)
{
  if (m_canceling)
  {
    m_statusLabel->setText("Canceled");
  }

  // DLM: should we attach the sql file to the model here?
  // DLM: if model is re-opened with results they will not be added here, better to do this on results tab
  //if (exists(t_sqlFile)){
  //  SqlFile sqlFile(t_sqlFile);
  //  if (sqlFile.connectionOpen()){
  //    boost::shared_ptr<OSDocument> osdocument = OSAppBase::instance()->currentDocument();
  //    osdocument->model().setSqlFile(sqlFile);
  //  }
  //}
  
  m_canceling = false;
  LOG(Debug, "Emitting results generated for sqlfile: " << openstudio::toString(t_sqlFile) << " and radiance file " << openstudio::toString(t_radianceOutputPath));
  emit resultsGenerated(t_sqlFile, t_radianceOutputPath);

  // needed so save of osm file does not trigger out of date and start running again
  runManager().setPaused(true);

  m_playButton->setChecked(false);
}

void RunView::treeChanged(const openstudio::UUID &t_uuid)
{
  std::string statusstr = "Ready";

  try {
    openstudio::runmanager::Job j = runManager().getJob(t_uuid);
    while (j.parent())
    {
      j = j.parent().get();
    }

    openstudio::runmanager::TreeStatusEnum status = j.treeStatus();
    LOG(Debug, "Tree finished, status is: " << status.valueName());
    statusstr = status.valueDescription();

    openstudio::path sqlpath;
    openstudio::path radianceOutPath;

    if (status == openstudio::runmanager::TreeStatusEnum::Finished
        || status == openstudio::runmanager::TreeStatusEnum::Failed
        || status == openstudio::runmanager::TreeStatusEnum::Canceled)
    {
      if (status == openstudio::runmanager::TreeStatusEnum::Failed && m_canceling)
      {
        statusstr = "Canceled";
      }

      try {
        sqlpath = j.treeAllFiles().getLastByFilename("eplusout.sql").fullPath;
      } catch (const std::exception &e) {
        LOG(Debug, "Tree finished, error getting sql file: " << e.what());
      } catch (...) {
        LOG(Debug, "Tree finished, error getting sql file");
        // no sql file exists
      }

      try {
        radianceOutPath = j.treeOutputFiles().getLastByFilename("radout.sql").fullPath;
      } catch (const std::exception &e) {
        LOG(Debug, "Tree finished, error getting radout.sql file: " << e.what());
      } catch (...) {
        LOG(Debug, "Tree finished, error getting radout.sql file");
        // no sql file exists
      }

      runFinished(sqlpath, radianceOutPath);
    } else { 
      m_canceling = false;
    }
  } catch (const std::exception &e) {
    LOG(Debug, "Tree finished, error getting status: " << e.what());
    runFinished(openstudio::path(), openstudio::path());

  } catch (...) {
    LOG(Debug, "Tree finished, error getting status");
    runFinished(openstudio::path(), openstudio::path());
    // no sql file exists
  }

  m_statusLabel->setText(openstudio::toQString(statusstr));
}

openstudio::runmanager::ToolVersion RunView::getRequiredEnergyPlusVersion()
{
  return openstudio::runmanager::ToolVersion::fromString(ENERGYPLUS_VERSION);
}

void RunView::playButtonClicked(bool t_checked)
{
  LOG(Debug, "playButtonClicked " << t_checked);



  boost::shared_ptr<OSDocument> osdocument = OSAppBase::instance()->currentDocument();

  if(osdocument->modified())
  {
    QMessageBox * messageBox = new QMessageBox(this);

    messageBox->setText("The document must be saved before the simulation can run.");
    messageBox->setInformativeText("Do you want to save your changes?");
    messageBox->setStandardButtons(QMessageBox::Save | QMessageBox::Cancel);
    messageBox->setDefaultButton(QMessageBox::Save);
    messageBox->button(QMessageBox::Save)->setShortcut(QKeySequence(Qt::Key_S));
    messageBox->setIcon(QMessageBox::Question);

    int ret = messageBox->exec();

    delete messageBox;

    switch (ret)
    {
      case QMessageBox::Save:
        osdocument->save();
        // save was cancelled
        if(osdocument->modified()) {
          return;
        }
        break;

      case QMessageBox::Cancel:
        return;

      default:
        // should never be reached
        break;
    };
  }

  if (!t_checked)
  {
    m_playButton->setChecked(true);

    if (!m_canceling)
    {
      // we are pausing the simulations
      m_statusLabel->setText("Canceling");
      m_canceling = true;
      openstudio::Application::instance().processEvents();
      runmanager::RunManager rm = runManager();
      pauseRunManager(rm);
   } else {
      LOG(Debug, "Already canceling, not doing it again");
    }
  } else {
    openstudio::runmanager::ToolVersion epver = getRequiredEnergyPlusVersion();
    if (openstudio::runmanager::ConfigOptions(true).getTools().getAllByName("energyplus").getAllByVersion(epver).tools().size() == 0)
    {
      QMessageBox::information(this, 
          "Missing EnergyPlus",
          openstudio::toQString("EnergyPlus " + epver.toString() + " could not be located, simulation aborted."),
          QMessageBox::Ok);
      m_playButton->setChecked(false);
      return;
    }
    // TODO call Dan's ModelToRad translator to determine if there are problems
    std::vector<std::string> warnings;
    std::vector<std::string> errors;
    if(warnings.size() || errors.size()){
      showRadianceWarningsAndErrors(warnings,errors);
      if(errors.size()){
        return;
      }
      else{
        // check messageBox return value to run with warnings
      }
    }

    m_canceling = false;
    m_outputWindow->clear();

    // reset the model's sqlFile
    osdocument->model().resetSqlFile();

    // we are starting the simulations
    openstudio::runmanager::RunManager rm = runManager();
    startRunManager(rm, m_modelPath, m_tempFolder, m_radianceButton->isChecked(), this);
  }
}

void RunView::openRunManagerClicked()
{
  LOG(Debug, "openRunManagerClicked");

#ifdef Q_OS_MAC
  openstudio::path runmanager
    = openstudio::getApplicationRunDirectory().parent_path().parent_path().parent_path() / openstudio::toPath("RunManager.app/Contents/MacOS/RunManager");
#else
  openstudio::path runmanager
    = openstudio::getApplicationRunDirectory() / openstudio::toPath("RunManager");
#endif

  LOG(Debug, "Starting RunManager in: " << openstudio::toString(runmanager));

  if (!QProcess::startDetached(openstudio::toQString(runmanager), QStringList()))
  {
    QMessageBox::critical(this, "Unable to launch RunManager", "RunManager was not found in the expected location:\n" + openstudio::toQString(runmanager));
  }
}

openstudio::runmanager::RunManager RunView::runManager()
{
  return OSAppBase::instance()->project()->runManager();
}

void RunView::showRadianceWarningsAndErrors(const std::vector<std::string> & warnings,
                                            const std::vector<std::string> & errors)
{
  QString errorsAndWarnings;
  QString text;
  
  if(warnings.size()){
    errorsAndWarnings += "WARNINGS:\n";
    BOOST_FOREACH(std::string warning, warnings){
      text = warning.c_str();
      errorsAndWarnings += text;
      errorsAndWarnings += '\n';
    }
    errorsAndWarnings += '\n';
  }

  if(errors.size()){
    errorsAndWarnings += "ERRORS:\n";
    BOOST_FOREACH(std::string error, errors){
      text = error.c_str();
      errorsAndWarnings += text;
      errorsAndWarnings += '\n';
    }
  }

  QMessageBox::critical(this, "Radiance Warnings and Errors", errorsAndWarnings);
}

void RunView::on_radianceWarningsAndErrorsClicked(bool checked)
{
  showRadianceWarningsAndErrors(m_radianceWarnings,m_radianceErrors);
}

void RunView::on_radianceGroupClicked(int idx)
{
  QAbstractButton * button = m_radianceGroup->button(idx);
  OS_ASSERT(button);
  if(button == m_radianceButton){
    emit useRadianceStateChanged(true);
    updateToolsWarnings();
    if(m_radianceErrors.size()){
      showRadianceWarningsAndErrors(m_radianceWarnings,m_radianceErrors);
    }
  }
  else{
    emit useRadianceStateChanged(false);
  }
}

} // openstudio
