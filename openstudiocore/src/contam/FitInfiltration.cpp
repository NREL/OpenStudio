/**********************************************************************
 *  Copyright (c) 2008-2010, Alliance for Sustainable Energy.
 *  Copyright (c) 2013, The Pennsylvania State University.
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

#include <contam/ForwardTranslator.hpp>
#include <contam/SimTxt.hpp>
#include <model/Model.hpp>
#include <osversion/VersionTranslator.hpp>
#include <utilities/core/CommandLine.hpp>
#include <utilities/core/Path.hpp>

#include <QProcess>

#include <string>
#include <iostream>

void usage(boost::program_options::options_description desc)
{
  std::cout << "Usage: FitInfiltration --inputPath=./path/to/input.osm" << std::endl;
  std::cout << "   or: FitInfiltration input.osm" << std::endl;
  std::cout << desc << std::endl;
}

int main(int argc, char *argv[])
{
  openstudio::path contamExePath = openstudio::toPath("C:\\Program Files (x86)\\NIST\\CONTAM 3.1\\ContamX3.exe");
  openstudio::path simreadExePath = openstudio::toPath("C:\\Users\\jwd131\\Software\\CONTAM\\simread31.exe");

  std::string inputPathString;
  boost::program_options::options_description desc("Allowed options");
  desc.add_options()
    ("help,h", "print help message")
    ("inputPath,i", boost::program_options::value<std::string>(&inputPathString), "path to OSM file")
    ("quiet,q", "suppress progress output");

  boost::program_options::positional_options_description pos;
  pos.add("inputPath", -1);
    
  boost::program_options::variables_map vm;
  // The following try/catch block is necessary to avoid uncaught
  // exceptions when the program is executed with more than one
  // "positional" argument - there's got to be a better way.
  try
  {
    boost::program_options::store(boost::program_options::command_line_parser(argc,
      argv).options(desc).positional(pos).run(), vm);
    boost::program_options::notify(vm);
  }

  catch(std::exception&)
  {
    std::cout << "Execution failed: check arguments and retry."<< std::endl << std::endl;
    usage(desc);
    return EXIT_FAILURE;
  }
  
  if (vm.count("help"))
  {
    usage(desc);
    return EXIT_SUCCESS;
  }

  bool verbose=true;
  if(vm.count("quiet"))
    verbose=false;

  if (!vm.count("inputPath"))
  {
    std::cout << "No input path given." << std::endl << std::endl;
    usage(desc);
    return EXIT_FAILURE;
  }

  openstudio::path inputPath = openstudio::toPath(inputPathString);
  openstudio::osversion::VersionTranslator vt;
  boost::optional<openstudio::model::Model> model = vt.loadModel(inputPath);

  if(!model)
  {
    std::cout << "Unable to load file '"<< inputPathString << "' as an OpenStudio model." << std::endl;
    return EXIT_FAILURE;
  }

  openstudio::path prjPath = inputPath.replace_extension(openstudio::toPath("prj").string());
  openstudio::contam::ForwardTranslator translator;
  
  QFile file(openstudio::toQString(prjPath));
  if(!file.open(QFile::WriteOnly))
  {
    std::cout << "Failed to open file '"<< openstudio::toString(prjPath) << "'." << std::endl;
    std::cout << "Check that this file location is accessible and may be written." << std::endl;
    return EXIT_FAILURE;
  }

  QTextStream textStream(&file);
  boost::optional<QString> output = translator.translateToPrj(*model,false);
  if(!output)
  {
    std::cout << "Translation failed, check errors and warnings for more information." << std::endl;
    return EXIT_FAILURE;
  }

  // This is a little clunky, but the prj::Data structure is too simple to do anything else right now
  translator.data.rc.ssWeather.windspd = QString("4.4704");
  std::cout << translator.data.rc.wind_H.toStdString() << std::endl;
  output = translator.data.print();
  textStream << *output;
  file.close();

  // Run simulation
  if(verbose)
    std::cout << "Running CONTAM simulation..." << std::endl;
  QProcess contamProcess;
  contamProcess.start(openstudio::toQString(contamExePath), QStringList() << openstudio::toQString(prjPath));
  if(!contamProcess.waitForStarted(-1))
  {
    std::cout << "Failed to start CONTAM process." << std::endl;
    return EXIT_FAILURE;
  }
  if(!contamProcess.waitForFinished(-1))
  {
    std::cout << "Failed to complete CONTAM process." << std::endl;
    return EXIT_FAILURE;
  }
  // Run simread
  if(verbose)
    std::cout << "Running SimRead on SIM file..." << std::endl;
  QProcess simreadProcess;
  simreadProcess.start(openstudio::toQString(simreadExePath), QStringList() << openstudio::toQString(prjPath));
  if(!simreadProcess.waitForStarted(-1))
  {
    std::cout << "Failed to start SimRead process." << std::endl;
    return EXIT_FAILURE;
  }
  simreadProcess.write("y\n\ny\n\n");
  if(!simreadProcess.waitForFinished(-1))
  {
    std::cout << "Failed to complete SimRead process." << std::endl;
    return EXIT_FAILURE;
  }

  // Collect results
  if(verbose)
    std::cout << "Reading results..." << std::endl;
  openstudio::contam::sim::LinkFlow lfr;
  openstudio::path lfrPath = inputPath.replace_extension(openstudio::toPath("lfr").string());
  if(!lfr.read(lfrPath))
  {
    std::cout << "Failed to read link flow results." << std::endl;
    return EXIT_FAILURE;
  }

  // Process results
  for(unsigned i=0;i<lfr.nr().size();i++)
  {
    std::cout << lfr.nr()[i] << std::endl;
  }

  openstudio::TimeSeries dpts = lfr.deltaP(1);
  openstudio::Vector dp = dpts.values();
  std::cout<< dp.size() <<std::endl;

  for(unsigned i=0;i<lfr.nr().size();i++)
  {
    openstudio::TimeSeries dpts = lfr.deltaP(i);
    openstudio::Vector dp = dpts.values();
    std::cout<< lfr.nr()[i] << " " << dp.size();
    if(dp.size())
      std::cout << " " << dp[0];
    std::cout << std::endl;
  }

  // Generate infiltration objects and attach to spaces

  // Write out new OSM
  openstudio::path outPath = openstudio::toPath("infiltrated.osm"); // = inputPath.replace_extension(openstudio::toPath("osm").string());
  if(!model->save(outPath,true))
  {
    std::cout << "Failed to write OSM file." << std::endl;
    return EXIT_FAILURE;
  }
  // The details on what we should do with these maps are still unclear
  // openstudio::path mapPath = inputPath.replace_extension(openstudio::toPath("map").string());
  // translator.writeMaps(mapPath);
    
  return EXIT_SUCCESS;
}
