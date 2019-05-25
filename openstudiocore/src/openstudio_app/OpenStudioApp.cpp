/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#include "OpenStudioApp.hpp"
#include <openstudio_app/AboutBox.hpp>
#include "StartupMenu.hpp"
#include "StartupView.hpp"
#include "LibraryDialog.hpp"
#include "../openstudio_lib/MainWindow.hpp"
#include "../openstudio_lib/OSDocument.hpp"

#include "../shared_gui_components/WaitDialog.hpp"
#include "../shared_gui_components/MeasureManager.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/core/Compare.hpp"
#include "../utilities/core/ApplicationPathHelpers.hpp"
#include "../utilities/core/Filesystem.hpp"

#include "../utilities/idf/IdfFile.hpp"
#include "../utilities/idf/IdfObject.hpp"
#include "../utilities/idf/ValidityReport.hpp"
#include "../utilities/idf/Workspace.hpp"

#include "../model/EvaporativeFluidCoolerSingleSpeed.hpp"
#include "../model/AirLoopHVACOutdoorAirSystem.hpp"
#include "../model/AirLoopHVACUnitaryHeatPumpAirToAir.hpp"
#include "../model/AirLoopHVACUnitarySystem.hpp"
#include "../model/AirTerminalSingleDuctConstantVolumeCooledBeam.hpp"
#include "../model/AirTerminalSingleDuctConstantVolumeReheat.hpp"
#include "../model/AirTerminalSingleDuctParallelPIUReheat.hpp"
#include "../model/AirTerminalSingleDuctConstantVolumeNoReheat.hpp"
#include "../model/AirTerminalSingleDuctVAVReheat.hpp"
#include "../model/AirTerminalSingleDuctVAVNoReheat.hpp"
#include "../model/BuildingStory.hpp"
#include "../model/CoilCoolingCooledBeam.hpp"
#include "../model/CoilCoolingDXSingleSpeed.hpp"
#include "../model/CoilCoolingDXTwoSpeed.hpp"
#include "../model/CoilCoolingWaterToAirHeatPumpEquationFit.hpp"
#include "../model/CoilHeatingDXSingleSpeed.hpp"
#include "../model/CoilHeatingElectric.hpp"
#include "../model/CoilHeatingGas.hpp"
#include "../model/CoilHeatingGas_Impl.hpp"
#include "../model/CoilHeatingWaterBaseboard.hpp"
#include "../model/CoilHeatingWaterToAirHeatPumpEquationFit.hpp"
#include "../model/ControllerOutdoorAir.hpp"
#include "../model/CurveBiquadratic.hpp"
#include "../model/CurveQuadratic.hpp"
#include "../model/DistrictCooling.hpp"
#include "../model/DistrictHeating.hpp"
#include "../model/EvaporativeCoolerDirectResearchSpecial.hpp"
#include "../model/FanConstantVolume.hpp"
#include "../model/FanOnOff.hpp"
#include "../model/FanVariableVolume.hpp"
#include "../model/FanZoneExhaust.hpp"
#include "../model/GeneratorFuelCellExhaustGasToWaterHeatExchanger.hpp"
// TODO: Not sure if I need to include GeneratorMicroTurbine.hpp, GeneratorMicroTurbineHeatRecovery.hpp or both
#include "../model/GeneratorMicroTurbineHeatRecovery.hpp"
#include "../model/Model.hpp"
#include "../model/ScheduleCompact.hpp"
#include "../model/SetpointManagerMixedAir.hpp"
#include "../model/SetpointManagerScheduled.hpp"
#include "../model/SetpointManagerSingleZoneReheat.hpp"
#include "../model/PlantComponentUserDefined.hpp"
#include "../model/ZoneHVACBaseboardConvectiveWater.hpp"
#include "../model/ZoneHVACFourPipeFanCoil.hpp"
#include "../model/ZoneHVACLowTempRadiantConstFlow.hpp"
#include "../model/ZoneHVACLowTemperatureRadiantElectric.hpp"
#include "../model/ZoneHVACLowTempRadiantVarFlow.hpp"
#include "../model/ZoneHVACPackagedTerminalHeatPump.hpp"
#include "../model/ZoneHVACPackagedTerminalAirConditioner.hpp"
#include "../model/ZoneHVACUnitHeater.hpp"
#include "../model/ZoneHVACWaterToAirHeatPump.hpp"

#include "../osversion/VersionTranslator.hpp"

#include "../energyplus/ForwardTranslator.hpp"
#include "../energyplus/ReverseTranslator.hpp"

#include "../gbxml/ReverseTranslator.hpp"
#include "../sdd/ReverseTranslator.hpp"
#include "../bimserver/BIMserverConnection.hpp"
#include "../bimserver/ProjectImporter.hpp"

#include <QAbstractButton>
#include <QDesktopServices>
#include <QDialog>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QFileOpenEvent>
#include <QMessageBox>
#include <QStringList>
#include <QThread>
#include <QTimer>
#include <QWidget>
#include <QProcess>
#include <QTcpServer>
#include <QtConcurrent>
#include <QtGlobal>

#include <OpenStudio.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <sstream>
#include <cstdlib>

using namespace openstudio::model;

namespace openstudio {

OpenStudioApp::OpenStudioApp( int & argc, char ** argv)
  : OSAppBase(argc, argv, QSharedPointer<MeasureManager>(new MeasureManager(this))),
    m_measureManagerProcess(nullptr)
{
  setOrganizationName("NREL");
  QCoreApplication::setOrganizationDomain("nrel.gov");
  setApplicationName("OpenStudioApp");

  readSettings();

  QFile f(":/library/OpenStudioPolicy.xml");
  if(f.open(QFile::ReadOnly)) {
    openstudio::model::AccessPolicyStore::Instance().loadFile(f.readAll());
  }

  QFile data(":/openstudiolib.qss");
  QString style;
  if(data.open(QFile::ReadOnly))
  {
    QTextStream styleIn(&data);
    style = styleIn.readAll();
    data.close();
    setStyleSheet(style);
  }

  #ifdef Q_OS_MAC
    std::stringstream webenginePath;
    webenginePath << QCoreApplication::applicationDirPath().toStdString();
    webenginePath << "/../Frameworks/QtWebEngineCore.framework/Versions/5/Helpers/QtWebEngineProcess.app/Contents/MacOS/QtWebEngineProcess";
    if( filesystem::exists(filesystem::path(webenginePath.str())) ) {
      setenv("QTWEBENGINEPROCESS_PATH",webenginePath.str().c_str(),true);
    }

    setQuitOnLastWindowClosed( false );

    m_startupMenu = std::shared_ptr<StartupMenu>(new StartupMenu());
    connect(m_startupMenu.get(), &StartupMenu::exitClicked, this, &OpenStudioApp::quit);
    connect(m_startupMenu.get(), &StartupMenu::importClicked, this, &OpenStudioApp::importIdf);
    connect(m_startupMenu.get(), &StartupMenu::importgbXMLClicked, this, &OpenStudioApp::importgbXML);
    connect(m_startupMenu.get(), &StartupMenu::importSDDClicked, this, &OpenStudioApp::importSDD);
    connect(m_startupMenu.get(), &StartupMenu::importIFCClicked, this, &OpenStudioApp::importIFC);
    connect(m_startupMenu.get(), &StartupMenu::loadFileClicked, this, &OpenStudioApp::open);
    connect(m_startupMenu.get(), &StartupMenu::newClicked, this, &OpenStudioApp::newModel);
    connect(m_startupMenu.get(), &StartupMenu::helpClicked, this, &OpenStudioApp::showHelp);
    connect(m_startupMenu.get(), &StartupMenu::aboutClicked, this, &OpenStudioApp::showAbout);
  #endif

  waitDialog()->show();

  // We are using the wait dialog to lock out the app so
  // use processEvents to make sure the dialog is up before we
  // proceed to startMeasureManagerProcess
  processEvents();

  // Non blocking
  startMeasureManagerProcess();

  auto waitForMeasureManagerFuture = QtConcurrent::run(&measureManager(),&MeasureManager::waitForStarted,10000);
  m_waitForMeasureManagerWatcher.setFuture(waitForMeasureManagerFuture);
  connect(&m_waitForMeasureManagerWatcher, &QFutureWatcher<void>::finished, this, &OpenStudioApp::onMeasureManagerAndLibraryReady);

  auto buildCompLibrariesFuture = QtConcurrent::run(this,&OpenStudioApp::buildCompLibraries);
  m_buildCompLibWatcher.setFuture(buildCompLibrariesFuture);
  connect(&m_buildCompLibWatcher, &QFutureWatcher<std::vector<std::string> >::finished, this, &OpenStudioApp::onMeasureManagerAndLibraryReady);
}

OpenStudioApp::~OpenStudioApp()
{
  if (m_measureManagerProcess){
    m_measureManagerProcess->disconnect();
    delete m_measureManagerProcess;
    m_measureManagerProcess = nullptr;
  }
}

void OpenStudioApp::onMeasureManagerAndLibraryReady() {
  if( m_buildCompLibWatcher.isFinished() && m_waitForMeasureManagerWatcher.isFinished() ) {
    auto failed = m_buildCompLibWatcher.result();
    showFailedLibraryDialog(failed);

    bool openedCommandLine = false;

    QStringList args = QApplication::arguments();
    args.removeFirst(); // application name

    if (args.size() == 1 || args.size() == 2){

      // look for file path in args 0
      QFileInfo info(args.at(0)); // handles windows links and "\"
      QString fileName = info.absoluteFilePath();

      osversion::VersionTranslator versionTranslator;
      versionTranslator.setAllowNewerVersions(false);

      boost::optional<openstudio::model::Model> model = versionTranslator.loadModel(toPath(fileName));
      if( model ){

        m_osDocument = std::shared_ptr<OSDocument>( new OSDocument(componentLibrary(),
                                                                   resourcesPath(),
                                                                   model,
                                                                   fileName) );

        connectOSDocumentSignals();

        if(args.size() == 2){
          // check for 'noSavePath'
          if (args.at(1) == QString("noSavePath")){
            m_osDocument->setSavePath("");
            QTimer::singleShot(0, m_osDocument.get(), SLOT(markAsModified()));
          }else{
            LOG_FREE(Warn, "OpenStudio", "Incorrect second argument '" << toString(args.at(1)) << "'");
          }
        }

        openedCommandLine = true;

        versionUpdateMessageBox(versionTranslator, true, fileName, openstudio::toPath(m_osDocument->modelTempDir()));

      }else{
        LOG_FREE(Warn, "OpenStudio", "Could not open file at " << toString(fileName));

        versionUpdateMessageBox(versionTranslator, false, fileName, openstudio::path());
      }

    }else if(args.size() > 2){
      LOG_FREE(Warn, "OpenStudio", "Incorrect number of arguments " << args.size());
    }

    if (!openedCommandLine){
      newFromEmptyTemplateSlot();
    }
  }
}

bool OpenStudioApp::openFile(const QString& fileName, bool restoreTabs)
{
  // Note: already checked for in open() before calling this
  if(fileName.length() > 0)
  {
    osversion::VersionTranslator versionTranslator;
    versionTranslator.setAllowNewerVersions(false);

    boost::optional<openstudio::model::Model> temp = versionTranslator.loadModel(toPath(fileName));
    // If VT worked
    if (temp) {
      model::Model model = temp.get();

      bool wasQuitOnLastWindowClosed = this->quitOnLastWindowClosed();
      this->setQuitOnLastWindowClosed(false);

      int startTabIndex = 0;
      int startSubTabIndex = 0;
      if( m_osDocument ){

        if (restoreTabs){
          startTabIndex = m_osDocument->verticalTabIndex();
          startSubTabIndex = m_osDocument->subTabIndex();
        }

        if( !closeDocument() ) {
          this->setQuitOnLastWindowClosed(wasQuitOnLastWindowClosed);
          return false;
        }
        processEvents();
      }

      // TODO: waitDialog isn't showed until VT has actually happened and worked?
      // I tried to show it visible in the begining of the method, but it isn't displayed correctly:
      // transparent + hidden by Filedialog which isn't closed yet.
      waitDialog()->setVisible(true);
      processEvents();

      m_osDocument = std::shared_ptr<OSDocument>( new OSDocument(componentLibrary(),
                                                                 resourcesPath(),
                                                                 model,
                                                                 fileName,
                                                                 false,
                                                                 startTabIndex,
                                                                 startSubTabIndex) );

      connectOSDocumentSignals();

      waitDialog()->setVisible(false);

      versionUpdateMessageBox(versionTranslator, true, fileName, openstudio::toPath(m_osDocument->modelTempDir()));

      this->setQuitOnLastWindowClosed(wasQuitOnLastWindowClosed);

      return true;
    }else{
      LOG_FREE(Warn, "OpenStudio", "Could not open file at " << toString(fileName));

      versionUpdateMessageBox(versionTranslator, false, fileName, openstudio::path());
    }
  }
  return false;
}

std::vector<std::string> OpenStudioApp::buildCompLibraries()
{
  std::vector<std::string> failed;

  // This is unused
  //QWidget * parent = nullptr;
  //if( this->currentDocument() ){
    //parent = this->currentDocument()->mainWindow();
  //}

  // Get the first Qlabel waitDialog (0 = stretch, 1 = "Loading model", 2 = "This may take a minute...", 3=hidden lable,   = stretch)
  waitDialog()->m_firstLine->setText("Loading Library Files");
  waitDialog()->m_secondLine->setText("(Manage library files in Preferences->Change default libraries)");

  // DLM: this was causing a crash because waitDialog is created on the main thread but this is called on the wait thread.
  // Because this is just the wait dialog let's just keep the line always visible.
  // Make it visible
  //waitDialog()->m_thirdLine->setVisible(true);
  //waitDialog()->m_fourthLine->setVisible(true);

  m_compLibrary = model::Model();

  std::string thisVersion = openStudioVersion();

  for( auto path : libraryPaths() ) {
    try {
      if ( exists(path) ) {
        boost::optional<VersionString> version = openstudio::IdfFile::loadVersionOnly(path);
        if (version) {
          waitDialog()->m_thirdLine->setText(QString::fromStdString("Translation From version " + version->str()
                                          + " to " + thisVersion + ": "));
        } else {
          waitDialog()->m_thirdLine->setText("Unknown starting version");
        }

        waitDialog()->m_fourthLine->setText(toQString(path));

        osversion::VersionTranslator versionTranslator;
        versionTranslator.setAllowNewerVersions(false);
        boost::optional<Model> temp = versionTranslator.loadModel(path);
        if (temp) {
          m_compLibrary.insertObjects(temp->objects());
        } else {
          LOG_FREE(Error, "OpenStudioApp", "Failed to load library");
          failed.push_back(path.string());
        }
      } else {
        failed.push_back(path.string());
      }
    } catch(...) {
      failed.push_back(path.string());
    }
  }

  // Reset all labels
  waitDialog()->resetLabels();

  return failed;
}

OpenStudioApp * OpenStudioApp::instance()
{
  return qobject_cast<OpenStudioApp *>(QApplication::instance());
}

openstudio::model::Model OpenStudioApp::componentLibrary() const
{
  return m_compLibrary;
}

openstudio::model::Model OpenStudioApp::hvacComponentLibrary() const
{
  return m_hvacCompLibrary;
}

void OpenStudioApp::quit()
{
  if( m_osDocument )
  {
    if( closeDocument() )
    {
      QApplication::quit();
    }
  }
  else
  {
    QApplication::quit();
  }
}

void OpenStudioApp::newFromEmptyTemplateSlot()
{
  newFromTemplateSlot(NEWFROMTEMPLATE_EMPTY);
}

void OpenStudioApp::newFromTemplateSlot( NewFromTemplateEnum newFromTemplateEnum )
{
  m_osDocument = std::shared_ptr<OSDocument>( new OSDocument( componentLibrary(), resourcesPath() ) );

  connectOSDocumentSignals();

  waitDialog()->hide();
}

std::shared_ptr<OSDocument> OpenStudioApp::currentDocument() const
{
  return m_osDocument;
}

void OpenStudioApp::importIdf()
{
  QWidget * parent = nullptr;

  if( this->currentDocument() )
  {
    parent = this->currentDocument()->mainWindow();
  }

  QString fileName = QFileDialog::getOpenFileName( parent,
                                                   tr("Import Idf"),
                                                   lastPath(),
                                                   tr("(*.idf)") );

  if( ! (fileName == "") )
  {
    setLastPath(QFileInfo(fileName).path());

    boost::optional<IdfFile> idfFile;

    idfFile = openstudio::IdfFile::load(toPath(fileName),IddFileType::EnergyPlus);

    if( idfFile )
    {

      IdfFile _idfFile = idfFile.get();

      if (!_idfFile.isValid(StrictnessLevel::Draft)){

        QMessageBox messageBox(parent);
        messageBox.setText("File is not valid to draft strictness.  Check that IDF is of correct version and that all fields are valid against Energy+.idd.");
        std::stringstream ss;
        ss << _idfFile.validityReport(StrictnessLevel::Draft);
        messageBox.setDetailedText(toQString(ss.str()));
        messageBox.exec();

      }else{

        Workspace workspace(_idfFile);

        energyplus::ReverseTranslator trans;
        model::Model model = trans.translateWorkspace(workspace);

        bool wasQuitOnLastWindowClosed = this->quitOnLastWindowClosed();
        this->setQuitOnLastWindowClosed(false);

        if( m_osDocument )
        {
          if( !closeDocument() ) {
            this->setQuitOnLastWindowClosed(wasQuitOnLastWindowClosed);
            return;
          }
          processEvents();
        }

        m_osDocument = std::shared_ptr<OSDocument>( new OSDocument(componentLibrary(),
                                                                   resourcesPath(),
                                                                   model) );
        m_osDocument->markAsModified();
        // ETH: parent should change now ...
        //parent = m_osDocument->mainWindow();

        connectOSDocumentSignals();

        QMessageBox messageBox; // (parent); ETH: ... but is hidden, so don't actually use
        messageBox.setText("Some portions of the idf file were not imported.");
        messageBox.setInformativeText("Only geometry, constructions, loads, thermal zones, and schedules are supported by the OpenStudio IDF import feature.");

        QString log;

        for( const auto & message : trans.errors() ) {
          log.append("\n");
          log.append("\n");
          log.append(QString::fromStdString(message.logMessage()));
          log.append("\n");
          log.append("\n");
        }

        for( const auto & message : trans.warnings() ) {
          log.append(QString::fromStdString(message.logMessage()));
          log.append("\n");
          log.append("\n");
        }

        log.append("The following idf objects were not imported.");
        log.append("\n");
        log.append("\n");

        for( const auto & idfObject : trans.untranslatedIdfObjects() ) {
          std::string message;
          if( auto name = idfObject.name() ) {
            message = idfObject.iddObject().name() + " named " + name.get();
          } else {
            message = "Unammed " + idfObject.iddObject().name();
          }
          log.append(QString::fromStdString(message));
          log.append("\n");
          log.append("\n");
        }

        messageBox.setDetailedText(log);

        messageBox.exec();

        this->setQuitOnLastWindowClosed(wasQuitOnLastWindowClosed);
      }
    }
  }
}

void OpenStudioApp::importgbXML()
{
  import(GBXML);
}

void OpenStudioApp::importSDD()
{
  import(SDD);
}

void OpenStudioApp::importIFC()
{
  QWidget * parent = nullptr;
  if( this->currentDocument() )
  {
    parent = this->currentDocument()->mainWindow();
  }

  auto projectImportation = new bimserver::ProjectImporter(parent);
  boost::optional<model::Model> model = projectImportation->run();
  projectImportation->close();

  if (model) {
    bool wasQuitOnLastWindowClosed = this->quitOnLastWindowClosed();
    this->setQuitOnLastWindowClosed(false);

    if (m_osDocument) {
      if (!closeDocument()) {
        this->setQuitOnLastWindowClosed(wasQuitOnLastWindowClosed);
        return;
      }
      processEvents();
    }

    m_osDocument = std::shared_ptr<OSDocument>(new OSDocument(componentLibrary(),
      resourcesPath(),
      *model));

    m_osDocument->markAsModified();

    connectOSDocumentSignals();

    this->setQuitOnLastWindowClosed(wasQuitOnLastWindowClosed);

  }

}

void OpenStudioApp::import(OpenStudioApp::fileType type)
{
  QWidget * parent = nullptr;

  std::vector<LogMessage> translatorErrors, translatorWarnings;

  QString fileExtension;
  if(type == SDD){
    fileExtension = "SDD";
  } else if(type == GBXML) {
    fileExtension = "gbXML";
  } else {
    // should never get here
    OS_ASSERT(false);
  }
  QString text("Import ");
  text.append(fileExtension);

  if( this->currentDocument() )
  {
    parent = this->currentDocument()->mainWindow();
  }

  QString fileName = QFileDialog::getOpenFileName( parent,
                                                   tr(text.toStdString().c_str()),
                                                   lastPath(),
                                                   tr("(*.xml)") );

  if( ! (fileName == "") )
  {
    setLastPath(QFileInfo(fileName).path());

    boost::optional<model::Model> model;

    if(type == SDD){
      sdd::ReverseTranslator trans;
      model = trans.loadModel(toPath(fileName));
      translatorErrors = trans.errors();
      translatorWarnings = trans.warnings();
    } else if(type == GBXML) {
      gbxml::ReverseTranslator trans;
      model = trans.loadModel(toPath(fileName));
      translatorErrors = trans.errors();
      translatorWarnings = trans.warnings();
    }

    if( model )
    {
      bool wasQuitOnLastWindowClosed = this->quitOnLastWindowClosed();
      this->setQuitOnLastWindowClosed(false);

      if( m_osDocument )
      {
        if( !closeDocument() ) {
          this->setQuitOnLastWindowClosed(wasQuitOnLastWindowClosed);
          return;
        }
        processEvents();
      }

      m_osDocument = std::shared_ptr<OSDocument>( new OSDocument(componentLibrary(),
                                                                 resourcesPath(),
                                                                 *model) );
      m_osDocument->markAsModified();
      // ETH: parent should change now ...
      //parent = m_osDocument->mainWindow();

      connectOSDocumentSignals();

      bool errorsOrWarnings = false;

      QString log;

      for( const auto & error : translatorErrors )
      {
        errorsOrWarnings = true;

        log.append(QString::fromStdString(error.logMessage()));
        log.append("\n");
        log.append("\n");
      }

      for( const auto & warning : translatorWarnings )
      {
        errorsOrWarnings = true;

        log.append(QString::fromStdString(warning.logMessage()));
        log.append("\n");
        log.append("\n");
      }

      if (errorsOrWarnings){
        QMessageBox messageBox; // (parent); ETH: ... but is hidden, so don't actually use
        messageBox.setText("Errors or warnings occurred on " + fileExtension + " import.");
        messageBox.setDetailedText(log);
        messageBox.exec();
      }

      this->setQuitOnLastWindowClosed(wasQuitOnLastWindowClosed);

    }else{

      QMessageBox messageBox; // (parent); ETH: ... but is hidden, so don't actually use
      messageBox.setText("Could not import SDD file.");
      messageBox.setDetailedText(QString("Could not import " + fileExtension + " file at ") + fileName);
      messageBox.exec();
    }
  }
}

bool OpenStudioApp::openFromDrag(QString path)
{
  return openFile(path);
}

bool OpenStudioApp::closeDocument()
{
  if(m_osDocument->modified())
  {
    QWidget * parent = m_osDocument->mainWindow();

    auto messageBox = new QMessageBox(parent);

    messageBox->setWindowTitle("Save Changes?");
    messageBox->setText("The document has been modified.");
    messageBox->setInformativeText("Do you want to save your changes?");

    messageBox->setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    messageBox->setDefaultButton(QMessageBox::Save);

    messageBox->button(QMessageBox::Save)->setShortcut(QKeySequence(Qt::Key_S));
    messageBox->button(QMessageBox::Discard)->setShortcut(QKeySequence(Qt::Key_D));

    messageBox->setIcon(QMessageBox::Question);

    int ret = messageBox->exec();

    delete messageBox;

    switch (ret)
    {
      case QMessageBox::Save:

        // Save was clicked
        m_osDocument->save();
        m_osDocument->mainWindow()->hide();
        m_osDocument = std::shared_ptr<OSDocument>();
        return true;

      case QMessageBox::Discard:

        // Don't Save was clicked
        m_osDocument->mainWindow()->hide();
        m_osDocument = std::shared_ptr<OSDocument>();
        return true;

      case QMessageBox::Cancel:

        // Cancel was clicked
        m_osDocument->mainWindow()->activateWindow();
        return false;

      default:

        // should never be reached
        return false;
    }

  }
  else
  {
    m_osDocument->mainWindow()->hide();
    m_osDocument = std::shared_ptr<OSDocument>();

    return true;
  }

}

void OpenStudioApp::onCloseClicked()
{
  if( closeDocument() )
  {
    #ifndef Q_OS_MAC
      QApplication::quit();
    #endif
  }
}

void OpenStudioApp::open()
{

  QWidget * parent = nullptr;

  if( this->currentDocument() )
  {
    parent = this->currentDocument()->mainWindow();
  }

  QString fileName = QFileDialog::getOpenFileName( parent,
                                                   tr("Open"),
                                                   lastPath(),
                                                   tr("(*.osm)") );

  if (!fileName.length()) return;

  setLastPath(QFileInfo(fileName).path());

  openFile(fileName);

  // Reset the labels
  waitDialog()->resetLabels();
}

//void OpenStudioApp::loadLibrary()
//{
//  if( this->currentDocument() )
//  {
//    QWidget * parent = this->currentDocument()->mainWindow();
//
//
//    QString fileName = QFileDialog::getOpenFileName( parent,
//                                                    tr("Select Library"),
//                                                    toQString(resourcesPath()),
//                                                    tr("(*.osm)") );
//
//    if( ! (fileName == "") )
//    {
//      osversion::VersionTranslator versionTranslator;
//      versionTranslator.setAllowNewerVersions(false);
//
//      boost::optional<openstudio::model::Model> model = versionTranslator.loadModel(toPath(fileName));
//      if( model ) {
//        this->currentDocument()->setComponentLibrary(*model);
//        versionUpdateMessageBox(versionTranslator, true, fileName, openstudio::path());
//      }else{
//        LOG_FREE(Warn, "OpenStudio", "Could not open file at " << toString(fileName));
//        versionUpdateMessageBox(versionTranslator, false, fileName, openstudio::path());
//      }
//    }
//  }
//}

void OpenStudioApp::newModel()
{
  bool wasQuitOnLastWindowClosed = this->quitOnLastWindowClosed();
  this->setQuitOnLastWindowClosed(false);

  if( m_osDocument )
  {
    if( !closeDocument() ) {
      this->setQuitOnLastWindowClosed(wasQuitOnLastWindowClosed);
      return;
    }
    processEvents();
  }

  //*************************************************************************************
  //
  ///! TODO StartView has been deprecated until the template wizard functions
  //
  //if (!openedCommandLine){
  //  m_startupView->show();
  //}
  //
  newFromTemplateSlot( NEWFROMTEMPLATE_EMPTY ); // remove when above code uncommented
  //
  //*************************************************************************************

  this->setQuitOnLastWindowClosed(wasQuitOnLastWindowClosed);
}

void  OpenStudioApp::showHelp()
{
  QDesktopServices::openUrl(QUrl("http://nrel.github.io/OpenStudio-user-documentation/reference/openstudio_application_interface/"));
}

void  OpenStudioApp::showAbout()
{
  QWidget * parent = nullptr;

  if (currentDocument()) {
    parent = currentDocument()->mainWindow();
  }
  QString details = "Measure Manager Server: " + measureManager().url().toString() + "\n";
  details += "Chrome Debugger: http://localhost:" + qgetenv("QTWEBENGINE_REMOTE_DEBUGGING") + "\n";
  details += "Temp Directory: " + currentDocument()->modelTempDir();
  QMessageBox about(parent);
  about.setText(OPENSTUDIO_ABOUTBOX);
  about.setDetailedText(details);
  about.setStyleSheet("qproperty-alignment: AlignCenter;");
  about.setWindowTitle("About " + applicationName());
  about.exec();
}

void OpenStudioApp::reloadFile(const QString& osmPath, bool modified, bool saveCurrentTabs)
{
  OS_ASSERT(m_osDocument);

  QFileInfo info(osmPath); // handles windows links and "\"
  QString fileName = info.absoluteFilePath();
  osversion::VersionTranslator versionTranslator;
  openstudio::path path = toPath(fileName);
  boost::optional<openstudio::model::Model> model = versionTranslator.loadModel(path);
  if (model){

    bool wasQuitOnLastWindowClosed = this->quitOnLastWindowClosed();
    this->setQuitOnLastWindowClosed(false);

    // DLM: load OSW from the existing temp dir
    openstudio::path workflowPath = openstudio::toPath(m_osDocument->modelTempDir()) / toPath("resources") / toPath("workflow.osw");
    boost::optional<WorkflowJSON> workflowJSON = WorkflowJSON::load(workflowPath);
    if (workflowJSON){
      model->setWorkflowJSON(*workflowJSON);
    }

    m_osDocument->setModel(*model, modified, saveCurrentTabs);

    versionUpdateMessageBox(versionTranslator, true, fileName, openstudio::toPath(m_osDocument->modelTempDir()));

    this->setQuitOnLastWindowClosed(wasQuitOnLastWindowClosed);

  }else{
    QMessageBox::warning (m_osDocument->mainWindow(), QString("Failed to load model"), QString("Failed to load model"));
  }

  processEvents();
}

openstudio::path OpenStudioApp::resourcesPath() const
{
  openstudio::path p;
  if (applicationIsRunningFromBuildDirectory())
  {
    p = boost::filesystem::canonical(openstudio::toPath("src/openstudio_app/Resources"), getApplicationSourceDirectory());
  }
  else
  {
    p = boost::filesystem::canonical(openstudio::toPath("../Resources"), getApplicationDirectory());
  }

  return p;
}

openstudio::path OpenStudioApp::openstudioCLIPath() const
{
  auto dir = applicationDirPath();
  QString ext;
  #ifdef _WIN32
    ext = QFileInfo(applicationFilePath()).suffix();
  #endif
  if (ext.isEmpty())
  {
    return openstudio::toPath(dir + "/openstudio");
  }
  return openstudio::toPath(dir + "/openstudio." + ext);
}

bool OpenStudioApp::notify(QObject* receiver, QEvent* event)
{
  if (event->type() == QEvent::FileOpen) {
    return openFile(static_cast<QFileOpenEvent *>(event)->file());
  }
  // Note: the original call below bypasses OSAppBase, OpenStudioApp's base class
  //return QApplication::notify(receiver, event);
  return OSAppBase::notify(receiver, event);
}

bool OpenStudioApp::event(QEvent * e)
{
  return OSAppBase::event(e);
}

void OpenStudioApp::childEvent(QChildEvent * e)
{
  OSAppBase::childEvent(e);
}

void OpenStudioApp::versionUpdateMessageBox(const osversion::VersionTranslator& translator,
                                            bool successful,
                                            const QString& fileName,
                                            const openstudio::path &tempModelDir)
{
  QMessageBox messageBox;

  QString log;

  std::vector<LogMessage> messages = translator.errors();

  for( const auto & message : messages )
  {
    log.append(QString::fromStdString(message.logMessage()));
    log.append("\n");
    log.append("\n");
  }

  messages = translator.warnings();

  for( const auto & message : messages )
  {
    log.append(QString::fromStdString(message.logMessage()));
    log.append("\n");
    log.append("\n");
  }

  if (log.length() > 0) {
    messageBox.setDetailedText(log);
  }

  if (successful){

    bool removedScriptDirs = false;
    if (!tempModelDir.empty())
    {
      std::vector<openstudio::path> scriptfolders;

      scriptfolders.push_back(tempModelDir / openstudio::toPath("resources/scripts/post_energyplus"));
      scriptfolders.push_back(tempModelDir / openstudio::toPath("resources/scripts/idf_resources"));
      scriptfolders.push_back(tempModelDir / openstudio::toPath("resources/scripts/idf_scripts"));
      scriptfolders.push_back(tempModelDir / openstudio::toPath("resources/scripts/model_resources"));
      scriptfolders.push_back(tempModelDir / openstudio::toPath("resources/scripts/model_scripts"));

      for (std::vector<openstudio::path>::const_iterator itr = scriptfolders.begin();
          itr != scriptfolders.end();
          ++itr)
      {
        if (openstudio::filesystem::exists(*itr))
        {
          removedScriptDirs = true;
          openstudio::filesystem::remove_all(*itr);
        }
      }
    }

    VersionString originalVersion(translator.originalVersion()), currentVersion(openStudioVersion());
    bool versionChanged = originalVersion != currentVersion;

    if (versionChanged || removedScriptDirs)
    {
      QTimer::singleShot(0, m_osDocument.get(), SLOT(markAsModified()));

      QString message;
      if (versionChanged)
      {
        if (originalVersion > currentVersion) {
          message = toQString("Opening future version " + originalVersion.str() + " using " +
                           currentVersion.str() + ".");
        } else {
          message = toQString("Model updated from " + originalVersion.str() + " to " +
                           currentVersion.str() + ".");
        }
      }

      if (removedScriptDirs)
      {
        if (!message.isEmpty())
        {
          message += "\n\n";
        }

        message += "Existing Ruby scripts have been removed.\nRuby scripts are no longer supported and have been replaced by measures.";
      }

      messageBox.setText(message);
      messageBox.exec();
    }
  }else{
    messageBox.setText(QString("Failed to open file at ") + fileName + QString("."));
    messageBox.exec();
  }
}

void OpenStudioApp::readSettings()
{
  QString organizationName = QCoreApplication::organizationName();
  QString applicationName = QCoreApplication::applicationName();
  QSettings settings(organizationName, applicationName);
  setLastPath(settings.value("lastPath", QDir::homePath()).toString());
}

void OpenStudioApp::writeSettings()
{
  QString organizationName = QCoreApplication::organizationName();
  QString applicationName = QCoreApplication::applicationName();
  QSettings settings(organizationName, applicationName);
  settings.setValue("lastPath", lastPath());
}

QString OpenStudioApp::lastPath() const
{
  return QDir().exists(m_lastPath) ? m_lastPath : QDir::homePath();
}

void OpenStudioApp::setLastPath(const QString& t_lastPath)
{
  m_lastPath = t_lastPath;
  writeSettings();
}

void OpenStudioApp::revertToSaved()
{
  QString fileName = this->currentDocument()->mainWindow()->windowFilePath();

  QFile testFile(fileName);
  if( !testFile.exists() ) {
    // Tell the user the file has never been saved, and ask them if they want to create a new file
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(mainWidget(), QString("Revert to Saved"), QString("This model has never been saved.\nDo you want to create a new model?"), QMessageBox::Yes|QMessageBox::No, QMessageBox::No);
    if (reply == QMessageBox::Yes)
    {
      // JM: copied DLM's hack below so we do not trigger prompt to save in call to closeDocument during newModel()
      // this->currentDocument()->markAsUnmodified();

      newModel();
    }

  } else {
    // Ask for confirmation
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(mainWidget(), QString("Revert to Saved"), QString("Are you sure you want to revert to the last saved version?"), QMessageBox::Yes|QMessageBox::No, QMessageBox::No);
    if (reply == QMessageBox::Yes)
    {
      // DLM: quick hack so we do not trigger prompt to save in call to closeDocument during openFile
      this->currentDocument()->markAsUnmodified();

      openFile(fileName, true);
    }
  }

}

void OpenStudioApp::connectOSDocumentSignals()
{
  OS_ASSERT(m_osDocument);
  connect(m_osDocument.get(), &OSDocument::closeClicked, this, &OpenStudioApp::onCloseClicked);
  connect(m_osDocument.get(), &OSDocument::exitClicked, this, &OpenStudioApp::quit);
  connect(m_osDocument.get(), &OSDocument::importClicked, this, &OpenStudioApp::importIdf);
  connect(m_osDocument.get(), &OSDocument::importgbXMLClicked, this, &OpenStudioApp::importgbXML);
  connect(m_osDocument.get(), &OSDocument::importSDDClicked, this, &OpenStudioApp::importSDD);
  connect(m_osDocument.get(), &OSDocument::importIFCClicked, this, &OpenStudioApp::importIFC);
  connect(m_osDocument.get(), &OSDocument::loadFileClicked, this, &OpenStudioApp::open);
  connect(m_osDocument.get(), &OSDocument::osmDropped, this, &OpenStudioApp::openFromDrag);
  connect(m_osDocument.get(), &OSDocument::changeDefaultLibrariesClicked, this, &OpenStudioApp::changeDefaultLibraries);
  connect(m_osDocument.get(), &OSDocument::loadLibraryClicked, this, &OpenStudioApp::loadLibrary);
  connect(m_osDocument.get(), &OSDocument::newClicked, this, &OpenStudioApp::newModel);
  connect(m_osDocument.get(), &OSDocument::helpClicked, this, &OpenStudioApp::showHelp);
  connect(m_osDocument.get(), &OSDocument::aboutClicked, this, &OpenStudioApp::showAbout);
}

void OpenStudioApp::measureManagerProcessStateChanged(QProcess::ProcessState newState)
{
}

void OpenStudioApp::measureManagerProcessFinished()
{
  // any exit of the cli is an error
  // DLM: I can't get this to fire when I terminate the process in taskmanager
  OS_ASSERT(m_measureManagerProcess);

  // the cli crashed
  QByteArray stdErr = m_measureManagerProcess->readAllStandardError();
  QByteArray stdOut = m_measureManagerProcess->readAllStandardOutput();

  QString message = "Measure Manager has crashed, attempting to restart\n\n";
  message += stdErr;
  message += stdOut;

  QMessageBox::warning(nullptr, QString("Measure Manager has crashed"), message);

  startMeasureManagerProcess();
}

void OpenStudioApp::startMeasureManagerProcess(){
  if (m_measureManagerProcess){
    // will terminate the existing process, blocking call
    delete m_measureManagerProcess;
  }

  // find available port
  QTcpServer tcpServer;
  tcpServer.listen(QHostAddress::LocalHost);
  quint16 port = tcpServer.serverPort();
  tcpServer.close();

  QString portString = QString::number(port);
  QString urlString = "http://127.0.0.1:" + portString;
  QUrl url(urlString);
  measureManager().setUrl(url);


  m_measureManagerProcess = new QProcess(this);

  bool test;
  // finished is an overloaded signal so have to be clear about which version to use
  test = connect(m_measureManagerProcess, static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished), this, &OpenStudioApp::measureManagerProcessFinished);
  OS_ASSERT(test);
  test = connect(m_measureManagerProcess, &QProcess::errorOccurred, this, &OpenStudioApp::measureManagerProcessFinished);
  OS_ASSERT(test);
  test = connect(m_measureManagerProcess, &QProcess::stateChanged, this, &OpenStudioApp::measureManagerProcessStateChanged);
  OS_ASSERT(test);

  QString program = toQString(openstudioCLIPath());
  QStringList arguments;
  arguments << "measure";
  arguments << "-s";
  arguments << portString;

  LOG(Debug, "Starting measure manager server at " << url.toString().toStdString());
  LOG(Debug, "Command: " << toString(openstudioCLIPath()) << " measure -s " << toString(portString));

  m_measureManagerProcess->start(program, arguments);
}



void OpenStudioApp::writeLibraryPaths(std::vector<openstudio::path> paths) {

    auto defaultPaths = defaultLibraryPaths();

    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());

    if ( paths == defaultPaths ) {
      settings.remove("library");
    } else {
      // Write the paths
      settings.remove("library");
      settings.beginWriteArray("library");
      int i = 0;
      auto resPath = resourcesPath();
      std::string s_resPath = toString(resPath);

      for( const auto path : paths ) {
        settings.setArrayIndex(i);

        // If this is one of the defaultPaths
        // if (std::find(defaultPaths.begin(),defaultPaths.end(),path) == defaultPaths.end())
        // If path is located in the 'Resources' folder

        openstudio::path::const_iterator begin1 = resPath.begin();
        openstudio::path::const_iterator end1 = resPath.end();

        openstudio::path::const_iterator begin2 = path.begin();
        openstudio::path::const_iterator end2 = path.end();

        bool is_resource = false;

        if (std::distance(begin1, end1) > std::distance(begin2, end2))
        {
          is_resource = false; // the run dir has fewer elements than the build dir - cannot be running from builddir
        } else {
          // if the rundir begins with the builddir, we know it's running from the builddir
          is_resource = std::equal(begin1, end1, begin2);
        }

        if (is_resource) {
          // Only in boost < 1.6... : boost::filesystem::relative
          std::string s_path = toString(path);

          // Every path here is an absolute canonical path, so we can just slice the path up to the
          // 'Resources' part, +1 to strip also the delimiter
          // TODO: there is probably a more reliable way to do this, it works on linux but unsure on Windows
          s_path = s_path.substr(s_resPath.length() + 1);
          openstudio::path rel_path = openstudio::path(s_path);
          // std::cout << "For '" << path << "', computed relative: " << rel_path << "\n";
          settings.setValue("path",QString::fromStdString(rel_path.string()));

        } else {
          settings.setValue("path",QString::fromStdString(path.string()));

        }

        settings.setValue("is_resource", is_resource);


        ++i;
      }
      settings.endArray();
    }

}

void OpenStudioApp::loadLibrary() {
  if ( this->currentDocument() ) {
    QWidget * parent = this->currentDocument()->mainWindow();

    QString fileName = QFileDialog::getOpenFileName( parent,
                                                    tr("Select Library"),
                                                    toQString(resourcesPath()),
                                                    tr("(*.osm)") );

    if( ! (fileName == "") ) {
      QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());

      auto paths = libraryPaths();

      auto path = toPath(fileName.toStdString());

      if( std::find(paths.begin(),paths.end(),path) == paths.end() ) {
        // If the loaded library isn't already in the list of paths, we add it and rewrite all
        paths.push_back(path);
        writeLibraryPaths(paths);

        auto future = QtConcurrent::run(this,&OpenStudioApp::buildCompLibraries);
        m_changeLibrariesWatcher.setFuture(future);
        connect(&m_changeLibrariesWatcher, &QFutureWatcher<std::vector<std::string> >::finished, this, &OpenStudioApp::onChangeDefaultLibrariesDone);
      }
    }
  }
}

void OpenStudioApp::changeDefaultLibraries() {
  auto defaultPaths = defaultLibraryPaths();
  auto paths = libraryPaths();
  auto resources = resourcesPath();
  // Starts the library dialog
  LibraryDialog dialog(paths, defaultPaths, resources);
  auto code = dialog.exec();
  auto newPaths = dialog.paths();

  // If user accepts its changes, and there are actually changes to the list of paths
  if ( (code == QDialog::Accepted) && (paths != newPaths) ) {

    // Write the library settings
    writeLibraryPaths(newPaths);

    // Trigger actual loading of the libraries
    auto future = QtConcurrent::run(this,&OpenStudioApp::buildCompLibraries);
    m_changeLibrariesWatcher.setFuture(future);
    connect(&m_changeLibrariesWatcher, &QFutureWatcher<std::vector<std::string> >::finished, this, &OpenStudioApp::onChangeDefaultLibrariesDone);
  }
}

void OpenStudioApp::removeLibraryFromsSettings( const openstudio::path & path ) {
  // erase the given path from the current list of paths
  auto paths = libraryPaths();
  paths.erase(std::remove(paths.begin(), paths.end(), path), paths.end());
  // Rewrite all
  writeLibraryPaths(paths);
}

void OpenStudioApp::showFailedLibraryDialog(const std::vector<std::string> & failed) {
  if( ! failed.empty() ) {
    QString text("Failed to load the following libraries...\n\n");
    for( const auto & path : failed ) {
      text.append(QString::fromStdString(path));
      text.append("\n");
    }
    // text.append("\n\nYou will now be able to modify the library paths or restore to the default paths");
    // QMessageBox::critical(nullptr, QString("Failed to load library"), text);

    text.append("\n\nWould you like to Restore library paths to default values or Open the library settings to change them manually?");

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(mainWidget(), QString("Failed to load library"), text,
                                  QMessageBox::RestoreDefaults|QMessageBox::Open, QMessageBox::Open);
    if (reply == QMessageBox::RestoreDefaults) {
      QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
      settings.remove("library");
    } else {
      // Open the library dialog
      changeDefaultLibraries();
    }
  }
}

void OpenStudioApp::onChangeDefaultLibrariesDone() {
  auto failed = m_changeLibrariesWatcher.result();

  showFailedLibraryDialog(failed);

  auto doc = currentDocument();
  if( doc ) {
    doc->setComponentLibrary(m_compLibrary);
  }
}

std::vector<openstudio::path> OpenStudioApp::defaultLibraryPaths() const {
  std::vector<openstudio::path> paths;

  paths.push_back(resourcesPath() / toPath("default/hvac_library.osm"));
  paths.push_back(resourcesPath() / toPath("default/office_default.osm"));

  return paths;
}

std::vector<openstudio::path> OpenStudioApp::libraryPaths() const {
  std::vector<openstudio::path> paths;

  QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());

  openstudio::path resPath = resourcesPath();
  int size = settings.beginReadArray("library");
  for (int i = 0; i < size; ++i) {
    settings.setArrayIndex(i);
    auto path = toPath(settings.value("path").toString());

    // Read whether this path is in the resource folder, if not present, assume its absolute
    auto is_resource = settings.value("is_resource", false).toBool();
    // We stored resources path as relative, so recompute an absolute canonical path
    if (is_resource) {
      // std::cout << "i=" << i << "; Path rel=" << path << "\n";
      // std::cout << "i=" << i << "; Path abs=" << resPath / path << "\n";

      paths.push_back(resPath / path);

    } else {
      // std::cout << "i=" << i << "; Path is already abs=" << resPath / path << "\n";
      paths.push_back(path);
    }
  }
  settings.endArray();

  if( paths.empty() ) {
    return defaultLibraryPaths();
  } else {
    return paths;
  }
}

} // openstudio
