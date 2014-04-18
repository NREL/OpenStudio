/**********************************************************************
 *  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
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


#include <pat_app/ExportSpreadsheet.hpp>

#include <analysisdriver/SimpleProject.hpp>

#include <utilities/core/Application.hpp>
#include <utilities/core/ApplicationPathHelpers.hpp>
#include <utilities/core/System.hpp>

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
  openstudio::Application::instance().application();

  //get the project's directory
  openstudio::path projectPath = project.projectDir();

  LOG_FREE(Debug, "ExportSpreadsheet", "Starting export of projectPath:" << toString(projectPath));

  openstudio::path rubyPath;
#if defined(Q_OS_WIN32)
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

  QProcess *p = new QProcess();
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
