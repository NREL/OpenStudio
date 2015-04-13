/**********************************************************************
*  Copyright (c) 2008-2015, Alliance for Sustainable Energy.  
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

#include "PatApp.hpp"

#include "../ruleset/OSArgument.hpp"

#include "../utilities/core/Application.hpp"
#include "../utilities/core/ApplicationPathHelpers.hpp"
#include "../utilities/core/FileLogSink.hpp"
#include "../utilities/core/Logger.hpp"
#include "../utilities/core/String.hpp"
#include "../utilities/idf/Workspace_Impl.hpp"

#include <QApplication>
#include <QMessageBox>

#ifdef _WIN32
#include <windows.h>
#endif

#define WSAAPI
#include "../utilities/core/Path.hpp"
#include "../utilities/core/RubyInterpreter.hpp"
#include "../ruleset/EmbeddedRubyUserScriptInfoGetter.hpp"

int main(int argc, char *argv[])
{

  ruby_sysinit(&argc, &argv);
  {
    RUBY_INIT_STACK;
    ruby_init();
  }

#if _DEBUG || (__GNUC__ && !NDEBUG)
#ifdef _WIN32
  const char *logfilepath = "./openstudio_pat.log";
#else
  const char *logfilepath = "/var/log/openstudio_pat.log";
#endif
  openstudio::Logger::instance().standardOutLogger().setLogLevel(Debug);
  openstudio::FileLogSink fileLog(openstudio::toPath(logfilepath));
  fileLog.setLogLevel(Debug);
#else
  openstudio::Logger::instance().standardOutLogger().setLogLevel(Warn);
#endif

  // list of Ruby modules we want to load into the interpreter
  std::vector<std::string> modules;
  for (const auto& path : openstudio::getOpenStudioBareRubyPaths()){
    modules.push_back(openstudio::toString(path));
  }

  bool cont = true;
  while(cont) {
    cont = false;

    // Initialize the embedded Ruby interpreter
    std::shared_ptr<openstudio::detail::RubyInterpreter> rubyInterpreter(
        new openstudio::detail::RubyInterpreter(openstudio::getOpenStudioRubyPath(),
          openstudio::getOpenStudioRubyScriptsPath(),
          modules));

    // Initialize the argument getter
    QSharedPointer<openstudio::ruleset::RubyUserScriptInfoGetter> infoGetter(
        new openstudio::ruleset::EmbeddedRubyUserScriptInfoGetter<openstudio::detail::RubyInterpreter>(rubyInterpreter));


    // Make the run path the default plugin search location
    QCoreApplication::addLibraryPath(openstudio::toQString(openstudio::getApplicationRunDirectory()));

    openstudio::pat::PatApp app(argc, argv, infoGetter);
    openstudio::Application::instance().setApplication(&app);

    try {
      return app.exec();
    } catch (const std::exception &e) {
      LOG_FREE(Fatal, "PatApp", "An unhandled exception has occurred: " << e.what());
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
      LOG_FREE(Fatal, "PatApp", "An unknown exception has occurred.");
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
  }
}
