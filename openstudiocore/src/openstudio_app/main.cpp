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

#define COMPILING_FROM_OSAPP
#include <openstudio_lib/OpenStudioAPI.hpp>
#include <openstudio_app/OpenStudioApp.hpp>

#include <openstudio_lib/MainWindow.hpp>
#include <utilities/core/Application.hpp>
#include <utilities/core/ApplicationPathHelpers.hpp>
#include <utilities/core/FileLogSink.hpp>
#include <utilities/bcl/BCLMeasure.hpp>
#include <ruleset/OSArgument.hpp>
#include <utilities/core/Logger.hpp>
#include <utilities/idf/Workspace_Impl.hpp>

#include <QAbstractButton>
#include <QApplication>
#include <QMessageBox>

#ifdef _WIN32
#include <Windows.h>
static const char *logfilepath = "./openstudio.log";
#else
static const char *logfilepath = "/var/log/openstudio.log";
#endif

#define WSAAPI
#include <utilities/core/Path.hpp>
#include <utilities/core/RubyInterpreter.hpp>
#include <ruleset/RubyUserScriptArgumentGetter_Impl.hpp>

#ifdef HAVE_RUBY_VERSION_H
#include <ruby/version.h>
#endif

int main(int argc, char *argv[])
{

#if RUBY_API_VERSION_MAJOR && RUBY_API_VERSION_MAJOR==2
  ruby_sysinit(&argc, &argv);
  {
    RUBY_INIT_STACK;
    ruby_init();
  }
#endif

#if _DEBUG || (__GNUC__ && !NDEBUG)
  openstudio::Logger::instance().standardOutLogger().setLogLevel(Debug);
  openstudio::FileLogSink fileLog(openstudio::toPath(logfilepath));
  fileLog.setLogLevel(Debug);
#else
  openstudio::Logger::instance().standardOutLogger().setLogLevel(Warn);
#endif

  bool cont = true;
  while(cont) {
    cont = false;



    std::vector<std::string> modules;
    modules.push_back("openstudioutilitiescore");
    modules.push_back("openstudioutilitiesbcl");
    modules.push_back("openstudioutilitiesidd");
    modules.push_back("openstudioutilitiesidf");
    modules.push_back("openstudioutilities");
    modules.push_back("openstudiomodel");
    modules.push_back("openstudiomodelcore");
    modules.push_back("openstudiomodelsimulation");
    modules.push_back("openstudiomodelresources");
    modules.push_back("openstudiomodelgeometry");
    modules.push_back("openstudiomodelhvac");
    modules.push_back("openstudioenergyplus");
    modules.push_back("openstudioruleset");

    //try {
    // Initialize the embedded Ruby interpreter
    boost::shared_ptr<openstudio::detail::RubyInterpreter> rubyInterpreter(
        new openstudio::detail::RubyInterpreter(openstudio::getOpenStudioRubyPath(),
          openstudio::getOpenStudioRubyScriptsPath(),
          modules));

    // Initialize the argument getter
    QSharedPointer<openstudio::ruleset::RubyUserScriptArgumentGetter> argumentGetter(
        new openstudio::ruleset::detail::RubyUserScriptArgumentGetter_Impl<openstudio::detail::RubyInterpreter>(rubyInterpreter));



    openstudio::OpenStudioApp app(argc, argv, argumentGetter);
    openstudio::Application::instance().setApplication(&app);

    try {
      return app.exec();
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
  }
}
