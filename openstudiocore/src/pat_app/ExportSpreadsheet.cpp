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

  // remove previous export
  openstudio::path modelMeasuresCSV = projectPath / toPath("spreadsheet_model_measures_export.csv");
  openstudio::path energyplusMeasuresCSV = projectPath / toPath("spreadsheet_energyplus_measures_export.csv");
  openstudio::path reportingMeasuresCSV = projectPath / toPath("spreadsheet_reporting_measures_export.csv");

  if (boost::filesystem::exists(modelMeasuresCSV)) {
    boost::filesystem::remove(modelMeasuresCSV);
  }
  if (boost::filesystem::exists(energyplusMeasuresCSV)) {
    boost::filesystem::remove(energyplusMeasuresCSV);
  }
  if (boost::filesystem::exists(reportingMeasuresCSV)) {
    boost::filesystem::remove(reportingMeasuresCSV);
  }

  openstudio::path rubyPath = getOpenStudioEmbeddedRubyPath();
#if defined(Q_OS_WIN32)
  rubyPath = getOpenStudioEmbeddedRubyPath() / toPath("bin/ruby.exe");
#else
  rubyPath("ruby");
#endif

  openstudio::path rubyIncludePath = getOpenStudioRubyIncludePath();
  if (!boost::filesystem::exists(rubyIncludePath)) {
    LOG_FREE(Error, "ExportSpreadsheet", "Ruby include paths cannot be found.");
    return false;
  }

  openstudio::path scriptPath = getOpenStudioRubyScriptsPath() / openstudio::toPath("openstudio/analysisdriver/ExportToSpreadsheet.rb");
  if (!boost::filesystem::exists(scriptPath)) {
    LOG_FREE(Error, "ExportSpreadsheet", "ExportToSpreadsheet script cannot be found.");
    return false;
  }

  QStringList args;
  args << toQString("-I");
  args << toQString(rubyIncludePath);
  args << toQString(scriptPath);
  args << toQString(projectPath);

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

  bool result = ((p->exitStatus() == QProcess::NormalExit) &&
                  boost::filesystem::exists(modelMeasuresCSV) &&
                  boost::filesystem::exists(energyplusMeasuresCSV) &&
                  boost::filesystem::exists(reportingMeasuresCSV));

  p->deleteLater();
  return result;
}

} // pat
} // openstudio
