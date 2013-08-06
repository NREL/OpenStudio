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

#include <plugin/OSPlugin.hpp>

#include <openstudio_app/StartupView.hpp>

#include <openstudio_lib/OSDocument.hpp>
#include <openstudio_lib/MainWindow.hpp>

#include <model/Model.hpp>
#include <model/Model_Impl.hpp>

#include <energyplus/ReverseTranslator.hpp>

#include <utilities/idf/IdfFile.hpp>
#include <utilities/idf/Workspace.hpp>
#include <utilities/core/Application.hpp>
#include <utilities/core/Assert.hpp>

#include <iostream>

#include <QFile>
#include <QThread>
#include <QFileDialog>
#include <QDir>
#include <QMessageBox>
#include <QDesktopServices>

#if _WIN32 || _MSC_VER
  #include <QWinWidget>
  #include <Windows.h>
  #include <utilities/core/String.hpp>
  #include <boost/regex.hpp>
#endif

namespace openstudio {
namespace plugin {

// handle Qt messages here during plugin shutdown
void logQtMessage(QtMsgType type, const char *msg)
{
  switch (type) {
    case QtDebugMsg:
      std::cout << "[Qt] <Debug> " << msg << std::endl;
      break;
    case QtWarningMsg:
      std::cout << "[Qt] <Warn> " << msg << std::endl;
      break;
    case QtCriticalMsg:
      std::cout << "[Qt] <Error> " << msg << std::endl;
      break;
    case QtFatalMsg:
      std::cout << "[Qt] <Fatal> " << msg << std::endl;
      break;
  }
}

OSPlugin::OSPlugin( int & argc, char ** argv )
  : QApplication(argc, argv, true), m_sketchUpWidget(NULL)
{
  //LOG(Trace, "OSPlugin");
  //QThread* ct = QThread::currentThread();
  //LOG(Trace, "OSPlugin, currentThread = " << ct);
  //QThread* thread = this->thread();
  //LOG(Trace, "OSPlugin, thread = " << thread);

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

  m_startupView = boost::shared_ptr<openstudio::StartupView>(new openstudio::StartupView());

  bool test;
  test = connect( m_startupView.get(), SIGNAL( newFromTemplate( NewFromTemplateEnum ) ), this, SLOT( newForLastKey( NewFromTemplateEnum ) ) ) ;
  OS_ASSERT(test);
  test = connect( m_startupView.get(), SIGNAL( openClicked() ), this, SLOT( openForLastKey() ) ) ;
  OS_ASSERT(test);
  test = connect( m_startupView.get(), SIGNAL( importClicked() ), this, SLOT( importIdfForLastKey() ) ) ;
  OS_ASSERT(test);

}

OSPlugin::~OSPlugin()
{
  LOG(Trace, "~OSPlugin");
}

OSPlugin* OSPlugin::instance()
{
  LOG(Trace, "instance");

  QCoreApplication* application = QApplication::instance();
  if (!application){

    QCoreApplication::setAttribute(Qt::AA_DontUseNativeMenuBar, true);
    QCoreApplication::setAttribute(Qt::AA_MacPluginApplication, true);
    static char *argv[] = {NULL};
    static int argc = sizeof(argv) / sizeof(char*) - 1;
    application = new OSPlugin(argc, argv);
    dynamic_cast<QApplication*>(application)->setQuitOnLastWindowClosed(false);

    // check if we are in a SketchUp process
    /* SKETCHUP IS ON ANOTHER THREAD, WHAT TO DO HERE????
    #if _WIN32 || _MSC_VER

      DWORD pId = GetCurrentProcessId();
      //HMODULE hModule = GetModuleHandle(NULL); // same as hInstance
      LPTSTR className = new TCHAR[255];
      LPTSTR typeName = new TCHAR[255];
      HWND h = GetTopWindow(0);
      while ( h )
      {
        DWORD pId2;
        GetWindowThreadProcessId(h, &pId2);
        
        if ( pId == pId2 ){

          GetClassName(h, className, 255);
          GetWindowText(h, typeName, 255);

          if (boost::regex_match(toString(typeName), boost::regex(".*- SketchUp.*"))){
            dynamic_cast<OSPlugin*>(application)->m_sketchUpWidget = new QWinWidget(h);
            break;
          }
        }
         
        h = GetNextWindow(h , GW_HWNDNEXT);
      }

      delete[] className; 
      delete[] typeName; 

    #endif
    */
  }

  //QThread* ct = QThread::currentThread();
  //LOG(Trace, "instance, currentThread = " << ct);
  //QThread* thread = application->thread();
  //LOG(Trace, "instance, application->thread() = " << thread);

  return qobject_cast<OSPlugin*>(application);
}

void OSPlugin::quit()
{
  LOG(Trace, "quit");

  // close all documents

  // install new qt log message handler
  qInstallMsgHandler(logQtMessage);

  QApplication::quit();
}

void OSPlugin::showHelp()
{
  QDesktopServices::openUrl(QUrl("https://openstudio.nrel.gov/openstudio-application-getting-started"));
}

void OSPlugin::startupPrompt(const std::string& key)
{
  LOG(Trace, "startupPrompt, key = " << key);

  // this is a hack
  m_lastKey = key;

  m_startupView->show();
  m_startupView->activateWindow();
}

void OSPlugin::importIdfForLastKey()
{
  LOG(Trace, "importIdfForLastKey");

  QString fileName = QFileDialog::getOpenFileName( m_sketchUpWidget,
                                                   tr("Import Idf"),
                                                   QDir::homePath(),
                                                   tr("(*.idf)") );

  if( ! (fileName == "") ){

    boost::optional<IdfFile> idfFile;

    idfFile = openstudio::IdfFile::load(toPath(fileName),IddFileType::EnergyPlus);

    if( idfFile ){

      if( !closeDocument(m_lastKey) ) { 
        return; 
      }

      IdfFile _idfFile = idfFile.get();

      Workspace workspace(_idfFile);

      energyplus::ReverseTranslator trans;
      model::Model model = trans.translateWorkspace(workspace);

      m_startupView->hide();

      processEvents();

      boost::shared_ptr<OSDocument> document( new OSDocument(model, model, openstudio::path(), 
        boost::optional<openstudio::model::Model>(), "", true) );

      connectDocumentSignals(document.get());

      document->markAsModified();

      QMessageBox messageBox; 
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

      m_osDocuments.insert(std::make_pair<std::string, boost::shared_ptr<OSDocument> >(m_lastKey, document));

      emit newModel(m_lastKey, document->model().getImpl<openstudio::model::detail::Model_Impl>().get());

      emit documentShown(m_lastKey);

      m_lastKey = "";
    }
  }
}

void OSPlugin::openForLastKey()
{
  LOG(Trace, "openForLastKey");

  QString fileName = QFileDialog::getOpenFileName( m_sketchUpWidget,
                                                   tr("Open"),
                                                   QDir::homePath(),
                                                   tr("(*.osm)") );

  if( !(fileName == "") ){

    boost::optional<IdfFile> idfFile;

    idfFile = openstudio::IdfFile::load(toPath(fileName),IddFileType::OpenStudio);

    if( idfFile ){

      if( !closeDocument(m_lastKey) ) { 
        return; 
      }

      IdfFile _idfFile = idfFile.get();

      model::Model model(_idfFile);

      m_startupView->hide();

      processEvents();

      boost::shared_ptr<OSDocument> document( new OSDocument(model, model, openstudio::path(), 
       boost::optional<openstudio::model::Model>(), fileName, true) );

      connectDocumentSignals(document.get());

      m_osDocuments.insert(std::make_pair<std::string, boost::shared_ptr<OSDocument> >(m_lastKey, document));

      emit newModel(m_lastKey, document->model().getImpl<openstudio::model::detail::Model_Impl>().get());

      emit documentShown(m_lastKey);

      m_lastKey = "";
    }
  }
}

void OSPlugin::newForLastKey( openstudio::NewFromTemplateEnum newFromTemplateEnum )
{
  LOG(Trace, "newForLastKey");

  if( !closeDocument(m_lastKey) ) { 
    return; 
  }

  openstudio::model::Model model = openstudio::model::exampleModel();
  boost::shared_ptr<OSDocument> document( new OSDocument(model, model, openstudio::path(), 
       boost::optional<openstudio::model::Model>(), "", true) );

  connectDocumentSignals(document.get());

  m_startupView->hide();

  m_osDocuments.insert(std::make_pair<std::string, boost::shared_ptr<OSDocument> >(m_lastKey, document));

  emit newModel(m_lastKey, document->model().getImpl<openstudio::model::detail::Model_Impl>().get());

  emit documentShown(m_lastKey);

  m_lastKey = "";
}

void OSPlugin::hideSender()
{
  LOG(Trace, "hideSender");

  QObject* sender = this->sender();
  OSDocument* osDocument = qobject_cast<OSDocument*>(sender);
  OS_ASSERT(osDocument);

  osDocument->mainWindow()->hide();

  boost::optional<std::string> key = getKeyForDocument(osDocument);
  OS_ASSERT(key);

  emit documentHidden(*key);
}

void OSPlugin::importIdfForSender()
{
  LOG(Trace, "importIdfForSender");

  QObject* sender = this->sender();
  OSDocument* osDocument = qobject_cast<OSDocument*>(sender);
  OS_ASSERT(osDocument);

  boost::optional<std::string> key = getKeyForDocument(osDocument);
  OS_ASSERT(key);
  
  QString fileName = QFileDialog::getOpenFileName( m_sketchUpWidget,
                                                   tr("Import Idf"),
                                                   QDir::homePath(),
                                                   tr("(*.idf)") );

  if( ! (fileName == "") ){

    boost::optional<IdfFile> idfFile;

    idfFile = openstudio::IdfFile::load(toPath(fileName),IddFileType::EnergyPlus);

    if( idfFile ){

      if( !closeDocument(*key) ) { 
        return; 
      }

      IdfFile _idfFile = idfFile.get();

      Workspace workspace(_idfFile);

      energyplus::ReverseTranslator trans;
      model::Model model = trans.translateWorkspace(workspace);

      m_startupView->hide();

      processEvents();

      boost::shared_ptr<OSDocument> document( new OSDocument(model, model, openstudio::path(), 
       boost::optional<openstudio::model::Model>(), "", true) );

      connectDocumentSignals(document.get());

      document->markAsModified();

      QMessageBox messageBox; 
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

      m_osDocuments.insert(std::make_pair<std::string, boost::shared_ptr<OSDocument> >(*key, document));

      emit newModel(*key, document->model().getImpl<openstudio::model::detail::Model_Impl>().get());

      emit documentShown(*key);
    }
  }
}

void OSPlugin::openForSender()
{
  LOG(Trace, "openForLastKey");

  QObject* sender = this->sender();
  OSDocument* osDocument = qobject_cast<OSDocument*>(sender);
  OS_ASSERT(osDocument);

  boost::optional<std::string> key = getKeyForDocument(osDocument);
  OS_ASSERT(key);

  QString fileName = QFileDialog::getOpenFileName( m_sketchUpWidget,
                                                   tr("Open"),
                                                   QDir::homePath(),
                                                   tr("(*.osm)") );

  if( !(fileName == "") ){

    boost::optional<IdfFile> idfFile;

    idfFile = openstudio::IdfFile::load(toPath(fileName),IddFileType::OpenStudio);

    if( idfFile ){

      if( !closeDocument(*key) ) { 
        return; 
      }

      IdfFile _idfFile = idfFile.get();

      model::Model model(_idfFile);

      m_startupView->hide();

      processEvents();

      boost::shared_ptr<OSDocument> document( new OSDocument(model, model, openstudio::path(), 
        boost::optional<openstudio::model::Model>(), fileName, true) );

      connectDocumentSignals(document.get());

      m_osDocuments.insert(std::make_pair<std::string, boost::shared_ptr<OSDocument> >(*key, document));

      emit newModel(*key, document->model().getImpl<openstudio::model::detail::Model_Impl>().get());

      emit documentShown(*key);
    }
  }
}

void OSPlugin::newForSender()
{
  LOG(Trace, "newForSender");

  QObject* sender = this->sender();
  OSDocument* osDocument = qobject_cast<OSDocument*>(sender);
  OS_ASSERT(osDocument);

  boost::optional<std::string> key = getKeyForDocument(osDocument);
  OS_ASSERT(key);

  if( !closeDocument(*key) ) { 
    return; 
  }

  openstudio::model::Model model = openstudio::model::exampleModel();
  boost::shared_ptr<OSDocument> document( new OSDocument(model, model, openstudio::path(), 
        boost::optional<openstudio::model::Model>(), "", true) );

  connectDocumentSignals(document.get());

  m_startupView->hide();

  m_osDocuments.insert(std::make_pair<std::string, boost::shared_ptr<OSDocument> >(*key, document));

  emit newModel(*key, document->model().getImpl<openstudio::model::detail::Model_Impl>().get());

  emit documentShown(*key);
}

void OSPlugin::showDocument(const std::string& key)
{
  LOG(Trace, "showDocument, key = " << key);

  boost::shared_ptr<OSDocument> document;
  std::map<std::string, boost::shared_ptr<OSDocument> >::iterator it = m_osDocuments.find(key);
  if (it != m_osDocuments.end()){
    document = it->second;
  }

  if (!document){
    return;
  }

  document->mainWindow()->activateWindow();
  document->mainWindow()->show();

  emit documentShown(key);
}

void OSPlugin::hideDocument(const std::string& key)
{
  LOG(Trace, "hideDocument, key = " << key);

  boost::shared_ptr<OSDocument> document;
  std::map<std::string, boost::shared_ptr<OSDocument> >::iterator it = m_osDocuments.find(key);
  if (it != m_osDocuments.end()){
    document = it->second;
  }

  if (!document){
    return;
  }

  document->mainWindow()->hide();

  emit documentHidden(key);
}

bool OSPlugin::closeDocument(const std::string& key)
{
  LOG(Trace, "closeDocument, key = " << key);

  // find existing document
  boost::shared_ptr<OSDocument> document;
  std::map<std::string, boost::shared_ptr<OSDocument> >::iterator it = m_osDocuments.find(key);
  if (it != m_osDocuments.end()){
    document = it->second;
  }

  if (!document){
    return true;
  }

  if(document->modified()){

    QMessageBox* messageBox = new QMessageBox(m_sketchUpWidget);
    messageBox->setText("The document has been modified.");
    messageBox->setInformativeText("Do you want to save your changes?");
    messageBox->setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    messageBox->setDefaultButton(QMessageBox::Save);

    int ret = messageBox->exec();

    delete messageBox;

    switch (ret){

      case QMessageBox::Save:

        // Save was clicked
        document->save();
        document->mainWindow()->hide();

        // remove document from list
        emit modelClosed(key);
        m_osDocuments.erase(it);

        return true;

      case QMessageBox::Discard:

        // Don't Save was clicked
        document->mainWindow()->hide();

        // remove document from list
        emit modelClosed(key);
        m_osDocuments.erase(it);

        return true;

      case QMessageBox::Cancel:

        // Cancel was clicked
        document->mainWindow()->activateWindow();
        return false;

      default:

        // should never be reached
        return false;
    }

  }

  // document was not modified
  document->mainWindow()->hide();

  // remove document from list
  emit modelClosed(key);
  m_osDocuments.erase(it);

  return true;
}

void OSPlugin::connectDocumentSignals(OSDocument* osDocument)
{
  bool test;
  test = connect( osDocument, SIGNAL(closeClicked()), this, SLOT(hideSender()) );
  OS_ASSERT(test);
  //test = connect( osDocument, SIGNAL(exitClicked()), this, SLOT(quit()) );
  //OS_ASSERT(test);
  test = connect( osDocument, SIGNAL(importClicked()), this, SLOT(importIdfForSender()) );
  OS_ASSERT(test);
  test = connect( osDocument, SIGNAL(loadFileClicked()), this, SLOT(openForSender()) );
  OS_ASSERT(test);
  test = connect( osDocument, SIGNAL(newClicked()), this, SLOT(newForSender()) );
  OS_ASSERT(test);
  test = connect( osDocument, SIGNAL(helpClicked()), this, SLOT(showHelp()) );
  OS_ASSERT(test);
}

boost::optional<std::string> OSPlugin::getKeyForDocument(OSDocument* osDocument) const
{
  if (!osDocument){
    return boost::none;
  }

  std::map<std::string, boost::shared_ptr<openstudio::OSDocument> >::const_iterator it = m_osDocuments.begin();
  std::map<std::string, boost::shared_ptr<openstudio::OSDocument> >::const_iterator itend = m_osDocuments.end();
  for (; it != itend; ++it){
    if (it->second.get() == osDocument){
      return it->first;
    }
  }

  return boost::none;
}

} // plugin
} // openstudio

