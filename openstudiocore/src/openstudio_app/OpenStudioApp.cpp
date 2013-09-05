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

#include <openstudio_app/OpenStudioApp.hpp>
#include <openstudio_app/AboutBox.hpp>
#include <openstudio_app/StartupMenu.hpp>
#include <openstudio_app/StartupView.hpp>
#include <openstudio_lib/MainWindow.hpp>
#include <openstudio_lib/OSDocument.hpp>
#include <openstudio_lib/FileOperations.hpp>

#include "../shared_gui_components/MeasureManager.hpp"

#include <utilities/idf/IdfObject.hpp>
#include <utilities/idf/Workspace.hpp>
#include <utilities/idf/ValidityReport.hpp>
#include <utilities/idf/IdfFile.hpp>

#include <utilities/core/Assert.hpp>
#include <utilities/core/ApplicationPathHelpers.hpp>
#include <utilities/core/Compare.hpp>
#include <utilities/idf/IdfFile.hpp>
#include <utilities/idf/IdfObject.hpp>
#include <utilities/idf/ValidityReport.hpp>
#include <utilities/idf/Workspace.hpp>

#include <model/EvaporativeFluidCoolerSingleSpeed.hpp>
#include <model/AirLoopHVACOutdoorAirSystem.hpp>
#include <model/AirLoopHVACUnitaryHeatPumpAirToAir.hpp>
#include <model/AirTerminalSingleDuctConstantVolumeCooledBeam.hpp>
#include <model/AirTerminalSingleDuctParallelPIUReheat.hpp>
#include <model/AirTerminalSingleDuctUncontrolled.hpp>
#include <model/AirTerminalSingleDuctVAVReheat.hpp>
#include <model/AvailabilityManagerScheduled.hpp>
#include <model/BuildingStory.hpp>
#include <model/CoilCoolingCooledBeam.hpp>
#include <model/CoilCoolingDXSingleSpeed.hpp>
#include <model/CoilCoolingDXTwoSpeed.hpp>
#include <model/CoilCoolingWaterToAirHeatPumpEquationFit.hpp>
#include <model/CoilHeatingDXSingleSpeed.hpp>
#include <model/CoilHeatingElectric.hpp>
#include <model/CoilHeatingGas.hpp>
#include <model/CoilHeatingGas_Impl.hpp>
#include <model/CoilHeatingWaterBaseboard.hpp>
#include <model/CoilHeatingWaterToAirHeatPumpEquationFit.hpp>
#include <model/ControllerOutdoorAir.hpp>
#include <model/CurveBiquadratic.hpp>
#include <model/CurveQuadratic.hpp>
#include <model/DistrictCooling.hpp>
#include <model/DistrictHeating.hpp>
#include <model/EvaporativeCoolerDirectResearchSpecial.hpp>
#include <model/FanConstantVolume.hpp>
#include <model/FanOnOff.hpp>
#include <model/FanVariableVolume.hpp>
#include <model/Model.hpp>
#include <model/ScheduleCompact.hpp>
#include <model/SetpointManagerMixedAir.hpp>
#include <model/SetpointManagerScheduled.hpp>
#include <model/SetpointManagerSingleZoneReheat.hpp>
#include <model/ZoneHVACBaseboardConvectiveWater.hpp>
#include <model/ZoneHVACFourPipeFanCoil.hpp>
#include <model/ZoneHVACLowTempRadiantConstFlow.hpp>
#include <model/ZoneHVACLowTemperatureRadiantElectric.hpp>
#include <model/ZoneHVACLowTempRadiantVarFlow.hpp>
#include <model/ZoneHVACPackagedTerminalHeatPump.hpp>
#include <model/ZoneHVACPackagedTerminalAirConditioner.hpp>
#include <model/ZoneHVACUnitHeater.hpp>
#include <model/ZoneHVACWaterToAirHeatPump.hpp>

#include <osversion/VersionTranslator.hpp>

#include <energyplus/ForwardTranslator.hpp>
#include <energyplus/ReverseTranslator.hpp>

#include <sdd/ReverseTranslator.hpp>

#include <QAbstractButton>
#include <QDesktopServices>
#include <QDialog>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QFileOpenEvent>
#include <QMessageBox>
#include <QStringList>
#include <QTimer>
#include <QWidget>

#include <OpenStudio.hxx>

using namespace openstudio::model;

namespace openstudio {

OpenStudioApp::OpenStudioApp( int & argc, char ** argv, const QSharedPointer<ruleset::RubyUserScriptArgumentGetter> &t_argumentGetter)
  : OSAppBase(argc, argv, QSharedPointer<MeasureManager>(new MeasureManager(t_argumentGetter, this))),
    m_argumentGetter(t_argumentGetter)
{
  setOrganizationName("NREL");
  QCoreApplication::setOrganizationDomain("nrel.gov");
  setApplicationName("OpenStudio");

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

    m_startupMenu = boost::shared_ptr<StartupMenu>(new StartupMenu());

    connect( m_startupMenu.get(), SIGNAL(exitClicked()), this,SLOT(quit()) );
    connect( m_startupMenu.get(), SIGNAL(importClicked()), this,SLOT(importIdf()) );
    connect( m_startupMenu.get(), SIGNAL(importSDDClicked()), this,SLOT(importSDD()) );
    connect( m_startupMenu.get(), SIGNAL(loadFileClicked()), this,SLOT(open()) );
    //connect( m_startupMenu.get(), SIGNAL(loadLibraryClicked()), this,SLOT(loadLibrary()) );
    connect( m_startupMenu.get(), SIGNAL(newClicked()), this,SLOT(newModel()) );
    connect( m_startupMenu.get(), SIGNAL(helpClicked()), this,SLOT(showHelp()) );
    connect( m_startupMenu.get(), SIGNAL(aboutClicked()), this,SLOT(showAbout()) );
  #endif

  this->buildCompLibraries();

  m_startupView = boost::shared_ptr<openstudio::StartupView>(new openstudio::StartupView());

  connect( m_startupView.get(), SIGNAL( newFromTemplate( NewFromTemplateEnum ) ), this, SLOT( newFromTemplateSlot( NewFromTemplateEnum ) ) ) ;
  connect( m_startupView.get(), SIGNAL( openClicked() ), this, SLOT( open() ) ) ;
  connect( m_startupView.get(), SIGNAL( importClicked() ), this, SLOT( importIdf() ) ) ;
  connect( m_startupView.get(), SIGNAL( importSDDClicked() ), this, SLOT( importSDD() ) ) ;

  bool openedCommandLine = false;

  QStringList args = QApplication::arguments();
  args.removeFirst(); // application name

  if (args.size() == 1 || args.size() == 2){

    // look for file path in args 0
    QFileInfo info(args.at(0)); // handles windows links and "\"
    QString fileName = info.absoluteFilePath();
    osversion::VersionTranslator versionTranslator;
    boost::optional<openstudio::model::Model> model = modelFromOSM(toPath(fileName), versionTranslator);
    if( model ){

      m_osDocument = boost::shared_ptr<OSDocument>( new OSDocument(componentLibrary(), 
                                                                   hvacComponentLibrary(), 
                                                                   resourcesPath(), 
                                                                   model,
                                                                   fileName) );

      connect( m_osDocument.get(), SIGNAL(closeClicked()), this, SLOT(onCloseClicked()) );
      connect( m_osDocument.get(), SIGNAL(exitClicked()), this,SLOT(quit()) );
      connect( m_osDocument.get(), SIGNAL(importClicked()), this,SLOT(importIdf()) );
      connect( m_osDocument.get(), SIGNAL(importSDDClicked()), this,SLOT(importSDD()) );
      connect( m_osDocument.get(), SIGNAL(loadFileClicked()), this,SLOT(open()) );
      connect( m_osDocument.get(), SIGNAL(osmDropped(QString)), this,SLOT(openFromDrag(QString)) );
      connect( m_osDocument.get(), SIGNAL(loadLibraryClicked()), this,SLOT(loadLibrary()) );
      connect( m_osDocument.get(), SIGNAL(newClicked()), this,SLOT(newModel()) );
      connect( m_osDocument.get(), SIGNAL(helpClicked()), this,SLOT(showHelp()) );
      connect( m_osDocument.get(), SIGNAL(aboutClicked()), this,SLOT(showAbout()) );

      if(args.size() == 2){
        // check for 'noSavePath'
        if (args.at(1) == QString("noSavePath")){
          // so new model can get access to weather file, file will be made relative again on save
          m_osDocument->setFullWeatherFilePath(); 
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

bool OpenStudioApp::openFile(const QString& fileName)
{
  if(fileName.length() > 0)
  {
    osversion::VersionTranslator versionTranslator;
    boost::optional<openstudio::model::Model> temp = modelFromOSM(toPath(fileName), versionTranslator);
    if (temp) {
      model::Model model = temp.get();

      bool wasQuitOnLastWindowClosed = this->quitOnLastWindowClosed();
      this->setQuitOnLastWindowClosed(false);

      if( m_osDocument ){
        if( !closeDocument() ) { 
          this->setQuitOnLastWindowClosed(wasQuitOnLastWindowClosed);
          return false;
        }
        processEvents();
      }

      m_osDocument = boost::shared_ptr<OSDocument>( new OSDocument(componentLibrary(), 
                                                                   hvacComponentLibrary(), 
                                                                   resourcesPath(), 
                                                                   model, 
                                                                   fileName) );

      connect( m_osDocument.get(), SIGNAL(closeClicked()), this, SLOT(onCloseClicked()) );
      connect( m_osDocument.get(), SIGNAL(exitClicked()), this,SLOT(quit()) );
      connect( m_osDocument.get(), SIGNAL(importClicked()), this,SLOT(importIdf()) );
      connect( m_osDocument.get(), SIGNAL(importSDDClicked()), this,SLOT(importSDD()) );
      connect( m_osDocument.get(), SIGNAL(loadFileClicked()), this,SLOT(open()) );
      connect( m_osDocument.get(), SIGNAL(osmDropped(QString)), this,SLOT(openFromDrag(QString)) );
      connect( m_osDocument.get(), SIGNAL(loadLibraryClicked()), this,SLOT(loadLibrary()) );
      connect( m_osDocument.get(), SIGNAL(newClicked()), this,SLOT(newModel()) );
      connect( m_osDocument.get(), SIGNAL(helpClicked()), this,SLOT(showHelp()) );
      connect( m_osDocument.get(), SIGNAL(aboutClicked()), this,SLOT(showAbout()) );

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

  path p = resourcesPath() / toPath("MinimalTemplate.osm");
  OS_ASSERT(exists(p));
  boost::optional<Model> temp = versionTranslator.loadModel(p);
  OS_ASSERT(temp);
  m_compLibrary = temp.get();

  p = resourcesPath() / toPath("hvaclibrary/hvac_library.osm");
  OS_ASSERT(exists(p));
  temp = versionTranslator.loadModel(p);
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
  m_osDocument = boost::shared_ptr<OSDocument>( new OSDocument( componentLibrary(), hvacComponentLibrary(), resourcesPath() ) );

  connect( m_osDocument.get(), SIGNAL(closeClicked()), this, SLOT(onCloseClicked()) );
  connect( m_osDocument.get(), SIGNAL(exitClicked()), this,SLOT(quit()) );
  connect( m_osDocument.get(), SIGNAL(importClicked()), this,SLOT(importIdf()) );
  connect( m_osDocument.get(), SIGNAL(importSDDClicked()), this,SLOT(importSDD()) );
  connect( m_osDocument.get(), SIGNAL(loadFileClicked()), this,SLOT(open()) );
  connect( m_osDocument.get(), SIGNAL(osmDropped(QString)), this,SLOT(openFromDrag(QString)) );
  connect( m_osDocument.get(), SIGNAL(loadLibraryClicked()), this,SLOT(loadLibrary()) );
  connect( m_osDocument.get(), SIGNAL(newClicked()), this,SLOT(newModel()) );
  connect( m_osDocument.get(), SIGNAL(helpClicked()), this,SLOT(showHelp()) );
  connect( m_osDocument.get(), SIGNAL(aboutClicked()), this,SLOT(showAbout()) );
 
  m_startupView->hide();
}

boost::shared_ptr<OSDocument> OpenStudioApp::currentDocument() const
{
  return m_osDocument;
}

void OpenStudioApp::importIdf()
{
  QWidget * parent = NULL;

  if( this->currentDocument() )
  {
    parent = this->currentDocument()->mainWindow();
  }

  QString fileName = QFileDialog::getOpenFileName( parent,
                                                   tr("Import Idf"),
                                                   QDir::homePath(),
                                                   tr("(*.idf)") );

  if( ! (fileName == "") )
  {
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

        m_osDocument = boost::shared_ptr<OSDocument>( new OSDocument(componentLibrary(), 
                                                                     hvacComponentLibrary(), 
                                                                     resourcesPath(), 
                                                                     model) );
        m_osDocument->markAsModified();
        // ETH: parent should change now ...
        parent = m_osDocument->mainWindow();

        connect( m_osDocument.get(), SIGNAL(closeClicked()), this, SLOT(onCloseClicked()) );
        connect( m_osDocument.get(), SIGNAL(exitClicked()), this,SLOT(quit()) );
        connect( m_osDocument.get(), SIGNAL(importClicked()), this,SLOT(importIdf()) );
        connect( m_osDocument.get(), SIGNAL(importSDDClicked()), this,SLOT(importSDD()) );
        connect( m_osDocument.get(), SIGNAL(loadFileClicked()), this,SLOT(open()) );
        connect( m_osDocument.get(), SIGNAL(osmDropped(QString)), this,SLOT(openFromDrag(QString)) );
        connect( m_osDocument.get(), SIGNAL(loadLibraryClicked()), this,SLOT(loadLibrary()) );
        connect( m_osDocument.get(), SIGNAL(newClicked()), this,SLOT(newModel()) );
        connect( m_osDocument.get(), SIGNAL(helpClicked()), this,SLOT(showHelp()) );
        connect( m_osDocument.get(), SIGNAL(aboutClicked()), this,SLOT(showAbout()) );

        m_startupView->hide();
        
        QMessageBox messageBox; // (parent); ETH: ... but is hidden, so don't actually use
        messageBox.setText("Some portions of the idf file were not imported.");

        QString log;

        std::vector<LogMessage> messages = trans.errors();

        for( std::vector<LogMessage>::iterator it = messages.begin();
             it < messages.end();
             it++ )
        {
          log.append(QString::fromStdString(it->logMessage()));
          log.append("\n");
          log.append("\n");
        }

        messages = trans.warnings();

        for( std::vector<LogMessage>::iterator it = messages.begin();
             it < messages.end();
             it++ )
        {
          log.append(QString::fromStdString(it->logMessage()));
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

void OpenStudioApp::importSDD()
{
  QWidget * parent = NULL;

  if( this->currentDocument() )
  {
    parent = this->currentDocument()->mainWindow();
  }

  QString fileName = QFileDialog::getOpenFileName( parent,
                                                   tr("Import SDD"),
                                                   QDir::homePath(),
                                                   tr("(*.xml)") );

  if( ! (fileName == "") )
  {
    boost::optional<model::Model> model;

    sdd::ReverseTranslator trans;
    model = trans.loadModel(toPath(fileName));

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

      m_osDocument = boost::shared_ptr<OSDocument>( new OSDocument(componentLibrary(), 
                                                                   hvacComponentLibrary(), 
                                                                   resourcesPath(), 
                                                                   *model) );
      m_osDocument->markAsModified();
      // ETH: parent should change now ...
      parent = m_osDocument->mainWindow();

      connect( m_osDocument.get(), SIGNAL(closeClicked()), this, SLOT(onCloseClicked()) );
      connect( m_osDocument.get(), SIGNAL(exitClicked()), this,SLOT(quit()) );
      connect( m_osDocument.get(), SIGNAL(importClicked()), this,SLOT(importIdf()) );
      connect( m_osDocument.get(), SIGNAL(importSDDClicked()), this,SLOT(importSDD()) );
      connect( m_osDocument.get(), SIGNAL(loadFileClicked()), this,SLOT(open()) );
      connect( m_osDocument.get(), SIGNAL(osmDropped(QString)), this,SLOT(openFromDrag(QString)) );
      connect( m_osDocument.get(), SIGNAL(loadLibraryClicked()), this,SLOT(loadLibrary()) );
      connect( m_osDocument.get(), SIGNAL(newClicked()), this,SLOT(newModel()) );
      connect( m_osDocument.get(), SIGNAL(helpClicked()), this,SLOT(showHelp()) );
      connect( m_osDocument.get(), SIGNAL(aboutClicked()), this,SLOT(showAbout()) );

      m_startupView->hide();

      bool errorsOrWarnings = false;

      QString log;

      std::vector<LogMessage> messages = trans.errors();

      for( std::vector<LogMessage>::iterator it = messages.begin();
           it < messages.end();
           it++ )
      {
        errorsOrWarnings = true;

        log.append(QString::fromStdString(it->logMessage()));
        log.append("\n");
        log.append("\n");
      }

      messages = trans.warnings();

      for( std::vector<LogMessage>::iterator it = messages.begin();
           it < messages.end();
           it++ )
      {
        errorsOrWarnings = true;

        log.append(QString::fromStdString(it->logMessage()));
        log.append("\n");
        log.append("\n");
      }

      if (errorsOrWarnings){
        QMessageBox messageBox; // (parent); ETH: ... but is hidden, so don't actually use
        messageBox.setText("Errors or warnings occurred on SDD import.");
        messageBox.setDetailedText(log);
        messageBox.exec();
      }

      this->setQuitOnLastWindowClosed(wasQuitOnLastWindowClosed);

    }else{

      QMessageBox messageBox; // (parent); ETH: ... but is hidden, so don't actually use
      messageBox.setText("Could not import SDD file.");
      messageBox.setDetailedText(QString("Could not import SDD file at ") + fileName);
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

    QMessageBox * messageBox = new QMessageBox(parent);

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
        m_osDocument = boost::shared_ptr<OSDocument>();
        return true;

      case QMessageBox::Discard:

        // Don't Save was clicked
        //m_startupView->show();
        m_osDocument->mainWindow()->hide();
        m_osDocument = boost::shared_ptr<OSDocument>();
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
    m_osDocument = boost::shared_ptr<OSDocument>();
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
  QWidget * parent = NULL;

  if( this->currentDocument() )
  {
    parent = this->currentDocument()->mainWindow();
  }

  QString fileName = QFileDialog::getOpenFileName( parent,
                                                   tr("Open"),
                                                   QDir::homePath(),
                                                   tr("(*.osm)") );

  if (!fileName.length()) return;
  
  openFile(fileName);
}

void OpenStudioApp::loadLibrary()
{
  QWidget * parent = NULL;

  if( this->currentDocument() )
  {
    parent = this->currentDocument()->mainWindow();
  

    QString fileName = QFileDialog::getOpenFileName( parent,
                                                    tr("Select Library"),
                                                    toQString(resourcesPath()),
                                                    tr("(*.osm)") );

    if( ! (fileName == "") )
    {
      osversion::VersionTranslator versionTranslator;
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
  QDesktopServices::openUrl(QUrl("https://openstudio.nrel.gov/openstudio-application-getting-started"));
}

void  OpenStudioApp::showAbout()
{
  QMessageBox about(currentDocument()->mainWindow());
  about.setText(OPENSTUDIO_ABOUTBOX);
  about.setStyleSheet("qproperty-alignment: AlignCenter;");
  about.setWindowTitle("About " + applicationName());
  about.exec();
}

openstudio::path OpenStudioApp::resourcesPath() const
{
  if (applicationIsRunningFromBuildDirectory())
  {
    return getApplicationSourceDirectory() / openstudio::toPath("src/openstudio_app/Resources");
  } 
  else 
  {
    return getApplicationRunDirectory() / openstudio::toPath("../share/openstudio/OSApp");
  }
}

bool OpenStudioApp::notify(QObject* receiver, QEvent* event)
{
  if (event->type() == QEvent::FileOpen) {
    return openFile(static_cast<QFileOpenEvent *>(event)->file());
  }
  return QApplication::notify(receiver, event);
}

void OpenStudioApp::versionUpdateMessageBox(const osversion::VersionTranslator& translator, bool successful, const QString& fileName, 
    const openstudio::path &tempModelDir) {
  
  QMessageBox messageBox; 

  QString log;

  std::vector<LogMessage> messages = translator.errors();

  for( std::vector<LogMessage>::iterator it = messages.begin();
       it < messages.end();
       it++ )
  {
    log.append(QString::fromStdString(it->logMessage()));
    log.append("\n");
    log.append("\n");
  }

  messages = translator.warnings();

  for( std::vector<LogMessage>::iterator it = messages.begin();
       it < messages.end();
       it++ )
  {
    log.append(QString::fromStdString(it->logMessage()));
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
      m_osDocument->markAsModified();

      QString message;
      if (versionChanged)
      {
        message = toQString("Model updated from " + originalVersion.str() + " to " + 
                       currentVersion.str() + ".");
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

} // openstudio

