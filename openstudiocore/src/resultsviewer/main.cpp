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

#include "MainWindow.hpp"

#include "../utilities/core/Application.hpp"
#include "../utilities/core/Logger.hpp"
#include "../utilities/core/FileLogSink.hpp"

#include <QApplication>

#ifdef _WIN32
static const char *logfilepath = "./resultsviewer.log";
#else
static const char *logfilepath = "/var/log/resultsviewer.log";
#endif

int main(int argc, char *argv[])
{
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

    QApplication qApplication(argc,argv);
    openstudio::Application::instance().setApplication(&qApplication);

    try {
      resultsviewer::MainWindow w;
      w.show();

      return qApplication.exec();

    } catch (const std::exception &e) {
      LOG_FREE(Fatal, "ResultsViewer", "An unhandled exception has occurred: " << e.what());
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
      LOG_FREE(Fatal, "ResultsViewer", "An unknown exception has occurred.");
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
