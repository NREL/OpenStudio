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

#include "../../utilities/core/Application.hpp"
#include "../../utilities/core/FileLogSink.hpp"
#include "../../utilities/core/ApplicationPathHelpers.hpp"
#include "../../utilities/core/System.hpp"

#include <QApplication>
#include <QMessageBox>

#include <boost/program_options.hpp>

void showMessage(const std::string &s)
{
#ifdef Q_OS_WIN32
  int argc = 0;
  char **argv = 0;
  QApplication qApplication(argc, argv);
  openstudio::Application::instance().setApplication(&qApplication);
  QString qs = openstudio::toQString("<tt><pre>" + s + "</pre></tt>");
  QMessageBox::information(0, "Command options information", qs, QMessageBox::Ok, QMessageBox::Ok);
#else
  std::cout << s << std::endl;
#endif
  exit(EXIT_FAILURE);
}

int doWorkflow(const openstudio::path &t_input, const openstudio::path &t_weather, 
    const std::string &t_workflowstr, 
    const openstudio::path &t_scripts,
    const openstudio::path &t_energyplus, const openstudio::path &t_xmlpreproc, 
    const openstudio::path &t_radiance, const openstudio::path &t_ruby, 
    bool t_showui, 
    const openstudio::path &t_outpath,     
    const std::string &t_slurmhost,
    const std::string &t_slurmuser,
    const openstudio::path &t_remote_energyplus,
    const openstudio::path &t_remote_xmlpreproc,
    const openstudio::path &t_remote_radiance,
    bool pause)
{
  using namespace openstudio;

  if (t_input.empty())
  {
    showMessage("No input specified");
  }

  if (!boost::filesystem::exists(t_input))
  {
    showMessage("Input file does not exist");
  }

  if (!t_weather.empty() && !boost::filesystem::exists(t_weather))
  {
    showMessage("Provided weather location does not exist");
  }

  if (t_workflowstr.empty())
  {
//    showMessage("No workflow provided, implementing standard workflow");
  }

  if (!t_energyplus.empty() && !boost::filesystem::is_directory(t_energyplus))
  {
    showMessage("Provided energyplus location is not a directory");
  }

  if (!t_xmlpreproc.empty() && !boost::filesystem::is_directory(t_xmlpreproc))
  {
    showMessage("Provided xml preprocessor location is not a directory");
  }

  if (!t_radiance.empty() && !boost::filesystem::is_directory(t_radiance))
  {
    showMessage("Provided radiance location is not a directory");
  }

  if (!t_ruby.empty() && !boost::filesystem::is_directory(t_ruby))
  {
    showMessage("Provided ruby location is not a directory");
  }

  if (!t_remote_energyplus.empty() && !boost::filesystem::exists(t_remote_energyplus))
  {
    showMessage("Provided remote energyplus file does not exist");
  }

  if (!t_remote_xmlpreproc.empty() && !boost::filesystem::exists(t_remote_xmlpreproc))
  {
    showMessage("Provided remote xml preprocessor file does not exist");
  }

  if (!t_remote_radiance.empty() && !boost::filesystem::exists(t_remote_radiance))
  {
    showMessage("Provided remote radiance file does not exist");
  }

  if (t_outpath.empty())
  {
    showMessage("No outpath specified");
  }

  openstudio::runmanager::Workflow workflow(t_workflowstr);

  openstudio::runmanager::Tools tools 
    = openstudio::runmanager::ConfigOptions::makeTools(
        t_energyplus, 
        t_xmlpreproc, 
        t_radiance, 
        t_ruby,
        openstudio::path(),
        t_remote_energyplus, 
        t_remote_xmlpreproc, 
        t_remote_radiance, 
        openstudio::path(),
        openstudio::path());




  int result;

  {
    openstudio::runmanager::RunManager runmanager(true);

    if (t_workflowstr.empty())
    {
      workflow.addStandardWorkflow(t_scripts, !t_ruby.empty(), openstudio::getOpenStudioRubyPath(), false);
    }


    workflow.add(tools);
    workflow.setInputFiles(t_input, t_weather);


    if (t_showui)
    {
      runmanager.showStatusDialog();
    }

    if (!t_slurmuser.empty() && !t_slurmhost.empty())
    {
      openstudio::runmanager::ConfigOptions co = runmanager.getConfigOptions();
      co.setSLURMUserName(t_slurmuser);
      co.setSLURMHost(t_slurmhost);
      co.setMaxSLURMJobs(99);
      runmanager.setConfigOptions(co);
    }

    openstudio::runmanager::Job j = workflow.create(t_outpath);
    runmanager.enqueue(j, true);
   
    if (pause)
    { 
      openstudio::System::msleep(10000);
    }
    runmanager.setPaused(false);

    runmanager.waitForFinished();

    if (j.treeStatus() == openstudio::runmanager::TreeStatusEnum::Finished)
    {
      result = EXIT_SUCCESS;
    } else {
      result = EXIT_FAILURE;
    }

    if (pause)
    { 
      openstudio::System::msleep(10000);
    }

    if (t_showui)
    {
      runmanager.hideStatusDialog();
    }
  }

  return result;
}

int doUI(int argc, char*argv[])
{
  bool cont = true;
  while(cont) {
    cont = false;

    QApplication qApplication(argc,argv);
    openstudio::Application::instance().setApplication(&qApplication);

    try {
      bool retry = true;
      bool newdb = false;

      while (retry)
      {
        openstudio::runmanager::MainWindow w(nullptr,nullptr,newdb);
        newdb = false;
        w.show();
    
        int retval = qApplication.exec();
        if (retval == -1)
        {
          newdb = true;
        } else {
          retry = false;
          return retval;
        }
      }
    } catch (const std::exception &e) {
      LOG_FREE(Fatal, "RunManager", "An unhandled exception has occurred: " << e.what());
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
      LOG_FREE(Fatal, "RunManager", "An unknown exception has occurred.");
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

  return EXIT_FAILURE;
}

int main(int argc, char *argv[])
{
  using namespace openstudio;

  openstudio::path logPath = openstudio::tempDir() / toPath("OpenStudio") / toPath("RunManager") / toPath("RunManager.log");
  boost::filesystem::create_directories(logPath.parent_path());

  openstudio::FileLogSink logFile(logPath);
  logFile.setLogLevel(Debug);

  namespace po = boost::program_options;

  // Declare the supported options.
  po::options_description desc("Options for command line execution");
  desc.add_options()
    ("help", "produce help message")
    ("showstatus", "Show status UI of job processing to user")
    ("input", po::value<std::string>(), "Set file to process")
    ("weather", po::value<std::string>(), "Weather file or directory to search for weather file")
    ("workflow", po::value<std::string>(), "The workflow to process, example: \"modeltoidf->expandobjects->energyplus\"")
    ("energyplus", po::value<std::string>(), "Path to directory containing energyplus binary")
    ("xmlpreproc", po::value<std::string>(), "Path to directory containing the xml preprocessor binary")
    ("radiance", po::value<std::string>(), "Path to directory containing the radiance binaries")
    ("ruby", po::value<std::string>(), "Path to directory containing the ruby binaries")
    ("remote_energyplus", po::value<std::string>(), "Path to package for remote execution of energyplus")
    ("remote_xmlpreproc", po::value<std::string>(), "Path to package for remote execution of xml preprocessor")
    ("remote_radiance", po::value<std::string>(), "Path to package for remote execution of radiance")
    ("outpath", po::value<std::string>(), "Path to send processing output to")
    ("scripts", po::value<std::string>(), "Path to search for scripts to inject into standard workflow")
    ("slurmusername", po::value<std::string>(), "User name to use when making remote SLURM connections")
    ("slurmhostname", po::value<std::string>(), "Host name to use when making remote SLURM connections")
    ("pause", "Pause after jobs are completed")
    ;

  po::variables_map vm;
  po::store(po::command_line_parser(argc, argv).options(desc).allow_unregistered().run(), vm);
  po::notify(vm);    

  if (vm.count("help")) {
    std::stringstream ss;
    ss << desc;
    showMessage(ss.str());
  }

  if (vm.count("input")) {
    openstudio::path input;
    openstudio::path energyplus;
    openstudio::path weather;
    openstudio::path scripts;
    openstudio::path xmlpreproc;
    openstudio::path radiance;
    openstudio::path ruby;
    openstudio::path remote_energyplus;
    openstudio::path remote_xmlpreproc;
    openstudio::path remote_radiance;
    openstudio::path outpath;

    std::string workflow;
    std::string slurmusername;
    std::string slurmhostname;

    bool showui = false;

    if (vm.count("showstatus"))
    {
      showui = true;
    }

    if (vm.count("input"))
    {
      input = toPath(vm["input"].as<std::string>());
    }

    if (vm.count("energyplus"))
    {
      energyplus = toPath(vm["energyplus"].as<std::string>());
    }

    if (vm.count("scripts"))
    {
      scripts = toPath(vm["scripts"].as<std::string>());
    }

    if (vm.count("weather"))
    {
      weather = toPath(vm["weather"].as<std::string>());
    }

    if (vm.count("xmlpreproc"))
    {
      xmlpreproc = toPath(vm["xmlpreproc"].as<std::string>());
    }

    if (vm.count("radiance"))
    {
      radiance = toPath(vm["radiance"].as<std::string>());
    }

    if (vm.count("ruby"))
    {
      ruby = toPath(vm["ruby"].as<std::string>());
    }

    if (vm.count("remote_energyplus"))
    {
      remote_energyplus = toPath(vm["remote_energyplus"].as<std::string>());
    }

    if (vm.count("remote_xmlpreproc"))
    {
      remote_xmlpreproc = toPath(vm["remote_xmlpreproc"].as<std::string>());
    }

    if (vm.count("remote_radiance"))
    {
      remote_radiance = toPath(vm["remote_radiance"].as<std::string>());
    }

    if (vm.count("workflow"))
    {
      workflow = vm["workflow"].as<std::string>();
    }

    if (vm.count("slurmusername"))
    {
      slurmusername = vm["slurmusername"].as<std::string>();
    }

    if (vm.count("slurmhostname"))
    {
      slurmhostname = vm["slurmhostname"].as<std::string>();
    }

    if (vm.count("outpath"))
    {
      outpath = toPath(vm["outpath"].as<std::string>());
    }

    doWorkflow(input, weather, workflow, scripts,
        energyplus, xmlpreproc, radiance, ruby,
        showui, outpath,
        slurmhostname, slurmusername,
        remote_energyplus, remote_xmlpreproc, remote_radiance, vm.count("pause"));

  } else {
    return doUI(argc, argv);
  }

}
