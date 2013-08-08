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
#include <model/Surface.hpp>
#include <model/Surface_Impl.hpp>
#include <model/Space.hpp>
#include <model/Space_Impl.hpp>
#include <osversion/VersionTranslator.hpp>
#include <utilities/core/CommandLine.hpp>
#include <utilities/core/Path.hpp>
#include <utilities/idf/Handle.hpp>

#include <QProcess>

#include <string>
#include <iostream>

void usage(boost::program_options::options_description desc)
{
  std::cout << "Usage: FitInfiltration --inputPath=./path/to/input.osm" << std::endl;
  std::cout << "   or: FitInfiltration input.osm" << std::endl;
  std::cout << desc << std::endl;
}

QVector<double> runCase(openstudio::contam::prj::Data *data, double windSpeed,
                        double windDirection, openstudio::path contamExePath, 
                        openstudio::path simreadExePath, bool verbose=false)
{
  QVector<double> results(data->zones.size(),0.0);
  // This is a little clunky, but the prj::Data structure is too simple to do anything else right now
  data->rc.ssWeather.windspd = QString().sprintf("%g",windSpeed);
  data->rc.ssWeather.winddir = QString().sprintf("%g",windDirection);
  openstudio::path inputPath = openstudio::toPath("temporary.prj");
  QString fileName = openstudio::toQString(inputPath);
  
  QFile file(fileName);
  if(!file.open(QFile::WriteOnly))
    {
      std::cout << "Failed to open file '"<< fileName.toStdString() << "'." << std::endl;
      std::cout << "Check that this file location is accessible and may be written." << std::endl;
      return QVector<double>();
    }
  QTextStream textStream(&file);
  if(verbose)
    std::cout << "Writing file " << fileName.toStdString() << std::endl;
  boost::optional<QString> output = data->print();
  textStream << *output;
  file.close();

  // Run simulation
  if(verbose)
    std::cout << "Running CONTAM simulation (" << windSpeed << "," << windDirection 
              << ")..." << std::endl;
  QProcess contamProcess;
  contamProcess.start(openstudio::toQString(contamExePath), QStringList() << fileName);
  if(!contamProcess.waitForStarted(-1))
  {
    std::cout << "Failed to start CONTAM process." << std::endl;
    return QVector<double>();
  }
  if(!contamProcess.waitForFinished(-1))
  {
    std::cout << "Failed to complete CONTAM process." << std::endl;
    return QVector<double>();
  }
  // Run simread
  if(verbose)
    std::cout << "Running SimRead on SIM file..." << std::endl;
  QProcess simreadProcess;
  simreadProcess.start(openstudio::toQString(simreadExePath), QStringList() << fileName);
  if(!simreadProcess.waitForStarted(-1))
  {
    std::cout << "Failed to start SimRead process." << std::endl;
    return QVector<double>();
  }
  simreadProcess.write("y\n\ny\n\n"); // This should work for no contaminants
  if(!simreadProcess.waitForFinished(-1))
  {
    std::cout << "Failed to complete SimRead process." << std::endl;
    return QVector<double>();
  }

  // Collect results
  if(verbose)
    std::cout << "Reading results..." << std::endl;
  openstudio::contam::sim::LinkFlow lfr;
  openstudio::path lfrPath = inputPath.replace_extension(openstudio::toPath("lfr").string());
  if(!lfr.read(lfrPath))
  {
    std::cout << "Failed to read link flow results." << std::endl;
    return QVector<double>();
  }

  // Process results
  std::vector<std::vector<double> > flow0 = lfr.F0();
  if(flow0.size() != data->paths.size())
  {
    std::cout << "Mismatch between lfr data and model path count." << std::endl;
    return QVector<double>();
  }
  for(unsigned i=0;i<flow0.size();i++)
  {
    if(flow0[i].size() != 1)
    {
      std::cout << "Missing or additional data for path " << i+1 << "." << std::endl;
      return QVector<double>();
    }
  }

  // Everything should be good to go if we made it to this point
  for(unsigned i=0;i<flow0.size();i++)
  {
    if(data->paths[i].pzm == -1)
      // Since there is no HVAC, this zone must connect the outside to a regular zone
      if(flow0[i][0] < 0.0)
        // Only sum up the negative flows (flows in, not flows out)
        results[data->paths[i].pzn-1] += flow0[i][0];
  }
  if(verbose)
    std::cout << "Processing complete." << std::endl;
  return results;
}

QVector<double> runCase2(std::map <openstudio::Handle,int> spaceMap,
                         openstudio::contam::ForwardTranslator &translator,
                         std::vector<openstudio::model::Surface> extSurfaces,
                         double windSpeed, double windDirection, openstudio::path contamExePath, 
                         openstudio::path simreadExePath, bool verbose=false)
{
  QVector<double> results(spaceMap.size(),0.0);
  std::map<openstudio::Handle,int> surfaceMap = translator.surfaceMap();
  translator.setSteadyWeather(windSpeed,windDirection);
  openstudio::path inputPath = openstudio::toPath("temporary.prj"); // This should do something else
  QString fileName = openstudio::toQString(inputPath);
  
  QFile file(fileName);
  if(!file.open(QFile::WriteOnly))
    {
      std::cout << "Failed to open file '"<< fileName.toStdString() << "'." << std::endl;
      std::cout << "Check that this file location is accessible and may be written." << std::endl;
      return QVector<double>();
    }
  QTextStream textStream(&file);
  if(verbose)
    std::cout << "Writing file " << fileName.toStdString() << std::endl;
  boost::optional<std::string> output = translator.toString();
  textStream << openstudio::toQString(*output);
  file.close();

  // Run simulation
  if(verbose)
    std::cout << "Running CONTAM simulation (" << windSpeed << "," << windDirection 
              << ")..." << std::endl;
  QProcess contamProcess;
  contamProcess.start(openstudio::toQString(contamExePath), QStringList() << fileName);
  if(!contamProcess.waitForStarted(-1))
  {
    std::cout << "Failed to start CONTAM process." << std::endl;
    return QVector<double>();
  }
  if(!contamProcess.waitForFinished(-1))
  {
    std::cout << "Failed to complete CONTAM process." << std::endl;
    return QVector<double>();
  }
  // Run simread
  if(verbose)
    std::cout << "Running SimRead on SIM file..." << std::endl;
  QProcess simreadProcess;
  simreadProcess.start(openstudio::toQString(simreadExePath), QStringList() << fileName);
  if(!simreadProcess.waitForStarted(-1))
  {
    std::cout << "Failed to start SimRead process." << std::endl;
    return QVector<double>();
  }
  simreadProcess.write("y\n\ny\n\n"); // This should work for no contaminants
  if(!simreadProcess.waitForFinished(-1))
  {
    std::cout << "Failed to complete SimRead process." << std::endl;
    return QVector<double>();
  }

  // Collect results
  if(verbose)
    std::cout << "Reading results..." << std::endl;
  openstudio::contam::sim::LinkFlow lfr;
  openstudio::path lfrPath = inputPath.replace_extension(openstudio::toPath("lfr").string());
  if(!lfr.read(lfrPath))
  {
    std::cout << "Failed to read link flow results." << std::endl;
    return QVector<double>();
  }

  // Process results
  std::vector<std::vector<double> > flow0 = lfr.F0();
  //if(flow0.size() != data->paths.size())
  //{
  //  std::cout << "Mismatch between lfr data and model path count." << std::endl;
  //  return QVector<double>();
  //}
  for(unsigned i=0;i<flow0.size();i++)
  {
    if(flow0[i].size() != 1)
    {
      std::cout << "Missing or additional data for path " << i+1 << "." << std::endl;
      return QVector<double>();
    }
  }

  // Everything should be good to go if we made it to this point
  // Loop over the exterior surfaces and see what has happened
  BOOST_FOREACH(openstudio::model::Surface surface, extSurfaces)
  {
    std::map<openstudio::Handle,int>::iterator indexIter;
    indexIter = surfaceMap.find(surface.handle());
    if(indexIter == surfaceMap.end())
    {
      std::cout << "Missing surface " << openstudio::toString(surface.handle()) << " in PRJ." << std::endl;
      return QVector<double>();
    }
    int index = indexIter->second - 1;
    if(flow0[index][0] < 0.0)
    {
      boost::optional<openstudio::model::Space> space = surface.space();
      if(!space)
      {
        std::cout << "Surface " << openstudio::toString(surface.handle()) << " is not attached to a space." << std::endl;
        return QVector<double>();
      }
      indexIter = spaceMap.find(space->handle());
      if(indexIter == spaceMap.end())
      { 
        std::cout << "Missing space " << openstudio::toString(surface.handle()) << " in space map." << std::endl;
        return QVector<double>();
      }
      // Only sum up the negative flows (flows in, not flows out)
      results[indexIter->second] += flow0[index][0];
    }
  }
  return results;
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
  boost::optional<std::string> output = translator.translateToPrj(*model,false);
  if(!output)
  {
    std::cout << "Translation failed, check errors and warnings for more information." << std::endl;
    return EXIT_FAILURE;
  }

  // Get all the surfaces
  std::vector<openstudio::model::Surface> surfaces = model->getConcreteModelObjects<openstudio::model::Surface>();
  // Cut that down to the external surfaces
  std::vector<openstudio::model::Surface> extSurfaces;
  BOOST_FOREACH(openstudio::model::Surface surface, surfaces)
  {
    std::string bc = surface.outsideBoundaryCondition();
    if(bc == "Outdoors")
      extSurfaces.push_back(surface);
  }

  // Get all the spaces
  std::vector<openstudio::model::Space> spaces = model->getConcreteModelObjects<openstudio::model::Space>();
  // Build a map to the index
  std::map<openstudio::Handle,int> spaceMap;
  int index=0;
  BOOST_FOREACH(openstudio::model::Space space, spaces)
  {
    spaceMap[space.handle()] = index++;
  }

  if(verbose)
    std::cout << "Found " << extSurfaces.size() << " exterior surfaces." << std::endl;

  // This is a stand-in for a more complete procedure (such as Ng et al.)

  QVector<double> Q10(translator.data.zones.size(),0.0);
  QVector<double> Q20(translator.data.zones.size(),0.0);
  /*

  // This is a little clunky, but the prj::Data structure is too simple to do anything else right now
  translator.data.rc.ssWeather.windspd = QString("4.4704");
  translator.data.rc.ssWeather.winddir = QString("0.0");
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
  */

  for(double angle=0.0;angle<360.0;angle+=90.0)
  {
    QVector<double> results = runCase(&(translator.data), 4.4704, angle, contamExePath,
      simreadExePath, true);
    for(int i=0;i<results.size();i++)
      Q10[i] += results[i];
    results = runCase(&(translator.data), 8.9408, angle, contamExePath,
      simreadExePath, true);
    for(int i=0;i<results.size();i++)
      Q20[i] += results[i];
  }

  for(int i=0;i<Q10.size();i++)
  {
    Q10[i] *= -0.25;
    Q20[i] *= -0.25;
    //std::cout<<i<<" "<<Q10[i]<<" "<<Q20[i]<<std::endl;
  }

  QVector<double> C;
  QVector<double> D;
  for(int i=0;i<Q10.size();i++)
  {
    C << (400.0-100*Q20[i]/Q10[i])/2000.0;
    D << ( 10*Q20[i]/Q10[i]- 20.0)/2000.0;
  }
  
  for(int i=0;i<Q10.size();i++)
  {
    std::cout<<i<<" "<<C[i]<<" "<<D[i]<<std::endl;
  }

  QVector<double> results = runCase(&(translator.data), 4.4704, 0.0, contamExePath,
      simreadExePath, true);
  for(int i=0;i<results.size();i++)
  {
    std::cout<<i<<" "<<results[i]<<std::endl;
  }
  results = runCase2(spaceMap,translator,extSurfaces, 4.4704, 0.0, contamExePath, simreadExePath, true);
  for(int i=0;i<results.size();i++)
  {
    std::cout<<i<<" "<<results[i]<<std::endl;
  }


  Q10.fill(0.0);
  Q20.fill(0.0);
  for(double angle=0.0;angle<360.0;angle+=90.0)
  {
    QVector<double> results = runCase2(spaceMap, translator, extSurfaces, 4.4704, angle, contamExePath,
      simreadExePath, true);
    for(int i=0;i<results.size();i++)
      Q10[i] += results[i];
    results = runCase2(spaceMap, translator, extSurfaces, 8.9408, angle, contamExePath,
      simreadExePath, true);
    for(int i=0;i<results.size();i++)
      Q20[i] += results[i];
  }

  for(int i=0;i<Q10.size();i++)
  {
    Q10[i] *= -0.25;
    Q20[i] *= -0.25;
    //std::cout<<i<<" "<<Q10[i]<<" "<<Q20[i]<<std::endl;
  }

  C.clear();
  D.clear();
  for(int i=0;i<Q10.size();i++)
  {
    C << (400.0-100*Q20[i]/Q10[i])/2000.0;
    D << ( 10*Q20[i]/Q10[i]- 20.0)/2000.0;
  }
  
  std::cout<<std::endl;
  for(int i=0;i<Q10.size();i++)
  {
    std::cout<<i<<" "<<C[i]<<" "<<D[i]<<std::endl;
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
