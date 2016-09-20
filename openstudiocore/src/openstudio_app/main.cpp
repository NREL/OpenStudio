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
#include <QtGlobal>

#ifdef _WIN32
#include <Windows.h>
#endif

#define WSAAPI
#include "../utilities/core/Path.hpp"
//#include "../utilities/core/RubyInterpreter.hpp"
//#include "../measure/EmbeddedRubyMeasureInfoGetter.hpp"


#ifdef Q_OS_WIN
#pragma warning(disable : 4930 )
#pragma warning(disable : 4101 )
#endif

#ifdef QT_STATIC
  //Q_IMPORT_PLUGIN(QSQLiteDriverPlugin);
#endif

#if defined(Q_OS_OSX) && defined(QT_STATIC)
  //Q_IMPORT_PLUGIN(QCocoaIntegrationPlugin);
#elif defined(Q_OS_WIN) && defined(QT_STATIC)
  //Q_IMPORT_PLUGIN(QWindowsIntegrationPlugin);
#endif

int main(int argc, char *argv[])
{
  /*
  ruby_sysinit(&argc, &argv);
  {
    RUBY_INIT_STACK;
    ruby_init();
  }
  */

#ifdef QT_STATIC
  Q_INIT_RESOURCE(openstudio);
#endif

#if _DEBUG || (__GNUC__ && !NDEBUG)
#ifdef _WIN32
  const char *logfilepath = "./openstudio.log";
#else
  const char *logfilepath = "/var/log/openstudio.log";
#endif
  openstudio::Logger::instance().standardOutLogger().setLogLevel(Debug);
  openstudio::FileLogSink fileLog(openstudio::toPath(logfilepath));
  fileLog.setLogLevel(Debug);
#else
  openstudio::Logger::instance().standardOutLogger().setLogLevel(Warn);
#endif

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
    // Make the run path the default plugin search location
    QCoreApplication::addLibraryPath(QCoreApplication::applicationDirPath());

    openstudio::OpenStudioApp app(argc, argv);
    openstudio::Application::instance().setApplication(&app);

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
