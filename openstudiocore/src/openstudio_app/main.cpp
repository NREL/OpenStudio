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

//#include <vld.h>

#define COMPILING_FROM_OSAPP
#include "../openstudio_lib/OpenStudioAPI.hpp"
#include "OpenStudioApp.hpp"

#include "../measure/OSArgument.hpp"

#include "../openstudio_lib/MainWindow.hpp"
#include "../utilities/core/Application.hpp"
#include "../utilities/core/FileLogSink.hpp"
#include "../utilities/bcl/BCLMeasure.hpp"
#include "../utilities/core/Logger.hpp"
#include "../utilities/core/String.hpp"
#include "../utilities/idf/Workspace_Impl.hpp"

#include <QAbstractButton>
#include <QApplication>
#include <QMessageBox>
#include <QtPlugin>
#include <QDir>
#include <QTcpServer>
#include <QtGlobal>

#ifdef _WIN32
#include <Windows.h>
#endif

#define WSAAPI
#include "../utilities/core/Path.hpp"

// OS App is linked to the dynamic plugins
//Q_IMPORT_PLUGIN(QSQLiteDriverPlugin);
//#if defined(Q_OS_OSX)
//  Q_IMPORT_PLUGIN(QCocoaIntegrationPlugin);
//#elif defined(Q_OS_WIN)
//  Q_IMPORT_PLUGIN(QWindowsIntegrationPlugin);
//#elif defined(Q_OS_LINUX)
//  Q_IMPORT_PLUGIN(QXcbIntegrationPlugin);  
//#endif

void qDebugMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QByteArray localMsg = msg.toLocal8Bit();
    switch (type) {
    case QtDebugMsg:
        fprintf(stderr, "Debug: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtInfoMsg:
        fprintf(stderr, "Info: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtWarningMsg:
        fprintf(stderr, "Warning: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtCriticalMsg:
        fprintf(stderr, "Critical: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtFatalMsg:
        fprintf(stderr, "Fatal: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        abort();
    }
}

void qMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QByteArray localMsg = msg.toLocal8Bit();
    switch (type) {
    case QtDebugMsg:
        //fprintf(stderr, "Debug: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtInfoMsg:
        //fprintf(stderr, "Info: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtWarningMsg:
        //fprintf(stderr, "Warning: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtCriticalMsg:
        fprintf(stderr, "Critical: %s\n", localMsg.constData());
        break;
    case QtFatalMsg:
        fprintf(stderr, "Fatal: %s\n", localMsg.constData());
        abort();
    }
}

int main(int argc, char *argv[])
{
  /*
  ruby_sysinit(&argc, &argv);
  {
    RUBY_INIT_STACK;
    ruby_init();
  }
  */

#ifndef SHARED_OS_LIBS
  Q_INIT_RESOURCE(openstudio);
#endif // SHARED_OS_LIBS

// DLM: on Windows run with 'OpenStudioApp.exe  > out.log 2>&1' to capture all debug output
// DLM: set env var 'QT_FATAL_WARNINGS' to error on qt warnings for debugging
#if _DEBUG || (__GNUC__ && !NDEBUG)
  bool debugging = true;
#else
  bool debugging = (qEnvironmentVariableIsSet("OPENSTUDIO_APPLICATION_DEBUG") && !qEnvironmentVariableIsEmpty("OPENSTUDIO_APPLICATION_DEBUG"));
#endif

  if (debugging){
    qInstallMessageHandler(qDebugMessageHandler);
    openstudio::Logger::instance().standardOutLogger().setLogLevel(Debug);
  }else{
    qInstallMessageHandler(qMessageHandler);
    openstudio::Logger::instance().standardOutLogger().setLogLevel(Warn);
  }

  bool cont = true;
  while(cont) {
    cont = false;
    /*
    std::vector<std::string> modules;
    for (const auto& path : openstudio::getOpenStudioBareRubyPaths()){
      modules.push_back(openstudio::toString(path));
    }
    
    //try {
    // Initialize the embedded Ruby interpreter
    std::shared_ptr<openstudio::detail::RubyInterpreter> rubyInterpreter(
        new openstudio::detail::RubyInterpreter(openstudio::getOpenStudioRubyPath(),
          openstudio::getOpenStudioRubyScriptsPath(),
          modules));
         
    // Initialize the measure info getter
    QSharedPointer<openstudio::measure::OSMeasureInfoGetter> infoGetter(
      new openstudio::measure::EmbeddedRubyMeasureInfoGetter<openstudio::detail::RubyInterpreter>(rubyInterpreter));
       */

    // find available port for debugging, have to do this before creating app
    QString debugPort(qgetenv("QTWEBENGINE_REMOTE_DEBUGGING"));
    if (debugPort.isEmpty()){
      QTcpServer tcpServer;
      tcpServer.listen(QHostAddress::LocalHost);
      quint16 port = tcpServer.serverPort();
      tcpServer.close();
      debugPort = QString::number(port);
      qputenv("QTWEBENGINE_REMOTE_DEBUGGING", debugPort.toStdString().c_str());
    }

    openstudio::OpenStudioApp app(argc, argv);
    openstudio::Application::instance().setApplication(&app);

    // Make the run path the default plugin search location
    QCoreApplication::addLibraryPath(QCoreApplication::applicationDirPath());

#ifdef Q_OS_DARWIN
  // Gross but perhaps the simplest way to find the webengine process
  // Improvements are welcome.
  auto qtwebengineprocess_path = QCoreApplication::applicationDirPath() + "/QtWebEngineProcess";
  if( QFile::exists(qtwebengineprocess_path) ) {
    setenv("QTWEBENGINEPROCESS_PATH", qtwebengineprocess_path.toStdString().c_str(),0);
  }
#endif

#if !(_DEBUG || (__GNUC__ && !NDEBUG))
    try {
#endif
      return app.exec();
#if !(_DEBUG || (__GNUC__ && !NDEBUG))
    } catch (const std::exception &e) {
      LOG_FREE(Fatal, "OpenStudio", "An unhandled exception has occurred: " << e.what());
      cont = true;
      QMessageBox msgBox;
      msgBox.setWindowTitle("Unhandled Exception");
      msgBox.setIcon(QMessageBox::Critical);
      msgBox.setText("An unhandled exception has occurred.");
      msgBox.setInformativeText(e.what());
      msgBox.setStandardButtons(QMessageBox::Retry | QMessageBox::Close);
      msgBox.button(QMessageBox::Retry)->setText("Relaunch");
      if (msgBox.exec() == QMessageBox::Close) {
        cont = false;
      }
    } catch (...) {
      LOG_FREE(Fatal, "OpenStudio", "An unknown exception has occurred.");
      cont = true;
      QMessageBox msgBox;
      msgBox.setWindowTitle("Unknown Exception");
      msgBox.setIcon(QMessageBox::Critical);
      msgBox.setText("An unknown exception has occurred.");
      msgBox.setStandardButtons(QMessageBox::Retry | QMessageBox::Close);
      msgBox.button(QMessageBox::Retry)->setText("Relaunch");
      if (msgBox.exec() == QMessageBox::Close) {
        cont = false;
      }
    }
#endif
  }
}
