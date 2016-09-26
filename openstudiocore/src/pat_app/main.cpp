/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2016, Alliance for Sustainable Energy, LLC. All rights reserved.
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
