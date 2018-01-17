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

#include "ExportSpreadsheet.hpp"

#include "../analysisdriver/SimpleProject.hpp"

#include "../utilities/core/Application.hpp"
#include "../utilities/core/ApplicationPathHelpers.hpp"
#include "../utilities/core/System.hpp"

#include <QProcess>

namespace openstudio {
namespace pat {

ExportSpreadsheet::ExportSpreadsheet()
{
}

ExportSpreadsheet::~ExportSpreadsheet()
{
}

bool ExportSpreadsheet::exportSpreadsheet(const analysisdriver::SimpleProject& project)
{
  //Make sure a QApplication exists
  openstudio::Application::instance().application(false);

  //get the project's directory
  openstudio::path projectPath = project.projectDir();

  LOG_FREE(Debug, "ExportSpreadsheet", "Starting export of projectPath:" << toString(projectPath));

  openstudio::path rubyPath;
#if defined(Q_OS_WIN)
  rubyPath = getOpenStudioEmbeddedRubyPath() / toPath("bin/ruby.exe");
#else
  rubyPath = "ruby";
#endif

  openstudio::path rubyIncludePath = getOpenStudioRubyIncludePath();
  if (!boost::filesystem::exists(rubyIncludePath)) {
    LOG_FREE(Error, "ExportSpreadsheet", "Ruby include path '" << rubyIncludePath << "' cannot be found.");
    return false;
  }

  openstudio::path scriptPath = getOpenStudioRubyScriptsPath() / openstudio::toPath("openstudio/analysisdriver/ExportToSpreadsheet.rb");
  if (!boost::filesystem::exists(scriptPath)) {
    LOG_FREE(Error, "ExportSpreadsheet", "ExportToSpreadsheet script '" << scriptPath << "' cannot be found.");
    return false;
  }

  QStringList args;
  args << "-I";
  args << openstudio::toQString(rubyIncludePath);
  args << openstudio::toQString(scriptPath);
  args << openstudio::toQString(projectPath);

  auto p = new QProcess();
  p->start(toQString(rubyPath), args);

  if (!p->waitForStarted(30000)){
    p->terminate();
    p->deleteLater();
    LOG_FREE(Error, "ExportSpreadsheet", "Process failed to start");
    return false;
  }

  if (!p->waitForFinished(300000)){
    p->terminate();
    p->deleteLater();
    LOG_FREE(Error, "ExportSpreadsheet", "Process failed to complete");
    return false;
  }

  QByteArray error = p->readAllStandardError();
  std::string errorString = toString(QString(error));
  LOG_FREE(Debug, "ExportSpreadsheet", "StandardError:" << std::endl << errorString);
  QByteArray output = p->readAllStandardOutput();
  std::string outputString = toString(QString(output));
  LOG_FREE(Debug, "ExportSpreadsheet", "StandardOutput:" << std::endl << outputString);

  // remove previous export
  openstudio::path exportPath = projectPath / toPath("analysis_spreadsheet_export");
  openstudio::path modelMeasuresCSV = exportPath / toPath("spreadsheet_model_measures_export.csv");
  openstudio::path energyplusMeasuresCSV = exportPath / toPath("spreadsheet_energyplus_measures_export.csv");
  openstudio::path reportingMeasuresCSV = exportPath / toPath("spreadsheet_reporting_measures_export.csv");
  openstudio::path outputsCSV = exportPath / toPath("spreadsheet_outputs_export.csv");

  bool result = ((p->exitStatus() == QProcess::NormalExit) &&
                  boost::filesystem::exists(modelMeasuresCSV) &&
                  boost::filesystem::exists(energyplusMeasuresCSV) &&
                  boost::filesystem::exists(reportingMeasuresCSV) && 
                  boost::filesystem::exists(outputsCSV));

  p->deleteLater();
  return result;
}

} // pat
} // openstudio
