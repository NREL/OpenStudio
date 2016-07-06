/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
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

#include "OpenStudioApp.hpp"
#include <openstudio_app/AboutBox.hpp>
#include "StartupMenu.hpp"
#include "StartupView.hpp"
#include "../openstudio_lib/MainWindow.hpp"
#include "../openstudio_lib/OSDocument.hpp"
#include "../openstudio_lib/FileOperations.hpp"

#include "../shared_gui_components/WaitDialog.hpp"
#include "../shared_gui_components/MeasureManager.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/core/ApplicationPathHelpers.hpp"
#include "../utilities/core/Compare.hpp"

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
#include "../model/AirTerminalSingleDuctUncontrolled.hpp"
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
// TODO: Not sure if I need to include GeneratorMicroTurbine.hpp, GeneratorMicroTurbineHeatRecovery.hpp or both
#include "../model/GeneratorMicroTurbineHeatRecovery.hpp"
#include "../model/Model.hpp"
#include "../model/ScheduleCompact.hpp"
#include "../model/SetpointManagerMixedAir.hpp"
#include "../model/SetpointManagerScheduled.hpp"
#include "../model/SetpointManagerSingleZoneReheat.hpp"
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

#include <OpenStudio.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

OpenStudioApp::OpenStudioApp( int & argc, char ** argv, const QSharedPointer<ruleset::RubyUserScriptInfoGetter> &t_infoGetter)
  : OSAppBase(argc, argv, QSharedPointer<MeasureManager>(new MeasureManager(t_infoGetter, this))),
    m_infoGetter(t_infoGetter)
{
  setOrganizationName("NREL");
  QCoreApplication::setOrganizationDomain("nrel.gov");
  setApplicationName("OpenStudio");

  readSettings();

  QFile f(":/library/OpenStudioPolicy.xml");

  openstudio::model::AccessPolicyStore::Instance().loadFile(f);

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
    setQuitOnLastWindowClosed( false );

    m_startupMenu = std::shared_ptr<StartupMenu>(new StartupMenu());

    connect(m_startupMenu.get(), &StartupMenu::exitClicked, this, &OpenStudioApp::quit);
    connect(m_startupMenu.get(), &StartupMenu::importClicked, this, &OpenStudioApp::importIdf);
    connect(m_startupMenu.get(), &StartupMenu::importgbXMLClicked, this, &OpenStudioApp::importgbXML);
    connect(m_startupMenu.get(), &StartupMenu::importSDDClicked, this, &OpenStudioApp::importSDD);
    connect(m_startupMenu.get(), &StartupMenu::importIFCClicked, this, &OpenStudioApp::importIFC);
    connect(m_startupMenu.get(), &StartupMenu::loadFileClicked, this, &OpenStudioApp::open);
    //connect(m_startupMenu.get(), &StartupMenu::loadLibraryClicked, this, &OpenStudioApp::loadLibrary);
    connect(m_startupMenu.get(), &StartupMenu::newClicked, this, &OpenStudioApp::newModel);
    connect(m_startupMenu.get(), &StartupMenu::helpClicked, this, &OpenStudioApp::showHelp);
    connect(m_startupMenu.get(), &StartupMenu::aboutClicked, this, &OpenStudioApp::showAbout);
  #endif

  this->buildCompLibraries();

  m_startupView = std::shared_ptr<openstudio::StartupView>(new openstudio::StartupView());

  connect(m_startupView.get(), &StartupView::newFromTemplate, this, &OpenStudioApp::newFromTemplateSlot);
  connect(m_startupView.get(), &StartupView::openClicked, this, &OpenStudioApp::open);
  connect(m_startupView.get(), &StartupView::importClicked, this, &OpenStudioApp::importIdf);
  //connect(m_startupView.get(), &StartupView::importgbXMLClicked, this, &OpenStudioApp::importgbXML);
  connect(m_startupView.get(), &StartupView::importSDDClicked, this, &OpenStudioApp::importSDD);

  bool openedCommandLine = false;

  QStringList args = QApplication::arguments();
  args.removeFirst(); // application name

  if (args.size() == 1 || args.size() == 2){

    // look for file path in args 0
    QFileInfo info(args.at(0)); // handles windows links and "\"
    QString fileName = info.absoluteFilePath();

    osversion::VersionTranslator versionTranslator;
    versionTranslator.setAllowNewerVersions(false);

    boost::optional<openstudio::model::Model> model = modelFromOSM(toPath(fileName), versionTranslator);
    if( model ){

      m_osDocument = std::shared_ptr<OSDocument>( new OSDocument(componentLibrary(), 
                                                                   hvacComponentLibrary(), 
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
          LOG_FREE(Warn, "OpenStudio", "Incorrect second argument '" << args.at(1) << "'");
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

  //*************************************************************************************
  //
  ///! TODO StartView has been deprecated until the template wizard functions
  //
  //if (!openedCommandLine){
  //  m_startupView->show();
  //}
  //
  if (!openedCommandLine){
    newFromTemplateSlot( NEWFROMTEMPLATE_EMPTY ); // remove when above code uncommented
  }
  //
  //*************************************************************************************

  //
  //*************************************************************************************
}

bool OpenStudioApp::openFile(const QString& fileName, bool restoreTabs)
{
  if(fileName.length() > 0)
  { 
    osversion::VersionTranslator versionTranslator;
    versionTranslator.setAllowNewerVersions(false);

    boost::optional<openstudio::model::Model> temp = modelFromOSM(toPath(fileName), versionTranslator);

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

      waitDialog()->setVisible(true);
      processEvents();

      m_osDocument = std::shared_ptr<OSDocument>( new OSDocument(componentLibrary(),
                                                                   hvacComponentLibrary(), 
                                                                   resourcesPath(), 
                                                                   model, 
                                                                   fileName, 
                                                                   false, 
                                                                   startTabIndex, 
                                                                   startSubTabIndex) );

      connectOSDocumentSignals();

      waitDialog()->setVisible(false);

      m_startupView->hide();

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

void OpenStudioApp::buildCompLibraries()
{
  osversion::VersionTranslator versionTranslator;
  versionTranslator.setAllowNewerVersions(false);

  path p = resourcesPath() / toPath("MinimalTemplate.osm");
  OS_ASSERT(exists(p));
  boost::optional<Model> temp = versionTranslator.loadModel(p);
  if (!temp){
    LOG_FREE(Error, "OpenStudioApp", "Failed to load MinimalTemplate");
    for (const auto& error : versionTranslator.errors()){
      LOG_FREE(Error, "OpenStudioApp", error.logMessage());
    }
  }
  OS_ASSERT(temp);
  m_compLibrary = temp.get();

  p = resourcesPath() / toPath("hvaclibrary/hvac_library.osm");
  OS_ASSERT(exists(p));
  temp = versionTranslator.loadModel(p);
  if (!temp){
    LOG_FREE(Error, "OpenStudioApp", "Failed to load hvaclibrary");
    for (const auto& error : versionTranslator.errors()){
      LOG_FREE(Error, "OpenStudioApp", error.logMessage());
    }
  }
  OS_ASSERT(temp);
  m_hvacCompLibrary = temp.get();
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

void OpenStudioApp::newFromTemplateSlot( NewFromTemplateEnum newFromTemplateEnum )
{
  m_osDocument = std::shared_ptr<OSDocument>( new OSDocument( componentLibrary(), hvacComponentLibrary(), resourcesPath() ) );

  connectOSDocumentSignals();

  m_startupView->hide();
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
        messageBox.setText("File is not valid to draft strictness.");
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
                                                                     hvacComponentLibrary(), 
                                                                     resourcesPath(), 
                                                                     model) );
        m_osDocument->markAsModified();
        // ETH: parent should change now ...
        //parent = m_osDocument->mainWindow();

        connectOSDocumentSignals();

        m_startupView->hide();
        
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
      hvacComponentLibrary(),
      resourcesPath(),
      *model));

    m_osDocument->markAsModified();

    connectOSDocumentSignals();

    m_startupView->hide();

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
                                                                   hvacComponentLibrary(), 
                                                                   resourcesPath(), 
                                                                   *model) );
      m_osDocument->markAsModified();
      // ETH: parent should change now ...
      //parent = m_osDocument->mainWindow();

      connectOSDocumentSignals();

      m_startupView->hide();

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
        //m_startupView->show();
        m_osDocument->mainWindow()->hide();
        m_osDocument = std::shared_ptr<OSDocument>();
        return true;

      case QMessageBox::Discard:

        // Don't Save was clicked
        //m_startupView->show();
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
    //m_startupView->show();

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
}

void OpenStudioApp::loadLibrary()
{
  if( this->currentDocument() )
  {
    QWidget * parent = this->currentDocument()->mainWindow();
  

    QString fileName = QFileDialog::getOpenFileName( parent,
                                                    tr("Select Library"),
                                                    toQString(resourcesPath()),
                                                    tr("(*.osm)") );

    if( ! (fileName == "") )
    {
      osversion::VersionTranslator versionTranslator;
      versionTranslator.setAllowNewerVersions(false);

      boost::optional<openstudio::model::Model> model = modelFromOSM(toPath(fileName), versionTranslator);
      if( model ) {
        this->currentDocument()->setComponentLibrary(*model);
        versionUpdateMessageBox(versionTranslator, true, fileName, openstudio::path());
      }else{
        LOG_FREE(Warn, "OpenStudio", "Could not open file at " << toString(fileName));
        versionUpdateMessageBox(versionTranslator, false, fileName, openstudio::path());
      }
    }
  }
}

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
  QMessageBox about(parent);
  about.setText(OPENSTUDIO_ABOUTBOX);
  about.setStyleSheet("qproperty-alignment: AlignCenter;");
  about.setWindowTitle("About " + applicationName());
  about.exec();
}

void OpenStudioApp::reloadFile(const QString& fileToLoad, bool modified, bool saveCurrentTabs)
{
  OS_ASSERT(m_osDocument);

  QFileInfo info(fileToLoad); // handles windows links and "\"
  QString fileName = info.absoluteFilePath();
  osversion::VersionTranslator versionTranslator;
  boost::optional<openstudio::model::Model> model = modelFromOSM(toPath(fileName), versionTranslator);
  if( model ){ 
    
    bool wasQuitOnLastWindowClosed = this->quitOnLastWindowClosed();
    this->setQuitOnLastWindowClosed(false);
    
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
  if (applicationIsRunningFromBuildDirectory())
  {
    return getApplicationSourceDirectory() / openstudio::toPath("src/openstudio_app/Resources");
  } 
  else 
  {
    return getApplicationRunDirectory() / openstudio::toPath("../share/openstudio-" + openStudioVersion() + "/OSApp");
  }
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
        if (boost::filesystem::exists(*itr))
        {
          removedScriptDirs = true;
          boost::filesystem::remove_all(*itr);
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
  if(!testFile.exists()) return;

  QMessageBox::StandardButton reply;
  reply = QMessageBox::question(mainWidget(), QString("Revert to Saved"), QString("Are you sure you want to revert to the last saved version?"), QMessageBox::Yes|QMessageBox::No, QMessageBox::No);
  if (reply == QMessageBox::Yes) 
  {
    // DLM: quick hack so we do not trigger prompt to save in call to closeDocument during openFile
    this->currentDocument()->markAsUnmodified();

    openFile(fileName, true);
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
  connect(m_osDocument.get(), &OSDocument::loadLibraryClicked, this, &OpenStudioApp::loadLibrary);
  connect(m_osDocument.get(), &OSDocument::newClicked, this, &OpenStudioApp::newModel);
  connect(m_osDocument.get(), &OSDocument::helpClicked, this, &OpenStudioApp::showHelp);
  connect(m_osDocument.get(), &OSDocument::aboutClicked, this, &OpenStudioApp::showAbout);
}

} // openstudio
