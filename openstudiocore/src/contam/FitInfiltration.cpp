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

#include <contam/InfiltrationCalculator.hpp>
#include <contam/ForwardTranslator.hpp>
#include <contam/SimTxt.hpp>
#include <model/Model.hpp>
#include <model/Surface.hpp>
#include <model/Surface_Impl.hpp>
#include <model/Space.hpp>
#include <model/Space_Impl.hpp>
#include <model/SpaceInfiltrationDesignFlowRate.hpp>
#include <model/SpaceInfiltrationDesignFlowRate_Impl.hpp>
#include <model/SpaceInfiltrationEffectiveLeakageArea.hpp>
#include <model/SpaceInfiltrationEffectiveLeakageArea_Impl.hpp>
#include <osversion/VersionTranslator.hpp>
#include <utilities/core/CommandLine.hpp>
#include <utilities/core/Path.hpp>
#include <utilities/idf/Handle.hpp>

#include <QProcess>

#include <string>
#include <iostream>

//#define SIMREADX

void usage(boost::program_options::options_description desc)
{
  std::cout << "Usage: FitInfiltration --input-path=./path/to/input.osm" << std::endl;
  std::cout << "   or: FitInfiltration input.osm" << std::endl;
  std::cout << desc << std::endl;
}

QVector<double> runCase(std::map <openstudio::Handle,int> spaceMap,
                        openstudio::contam::ForwardTranslator &translator,
                        std::vector<openstudio::model::Surface> extSurfaces,
                        double windSpeed, double windDirection, openstudio::path contamExePath, 
                        openstudio::path simreadExePath, bool verbose=false)
{
  QVector<double> results(spaceMap.size(),0.0);
  std::map<openstudio::Handle,int> surfaceMap = translator.surfaceMap();
  translator.setSteadyWeather(windSpeed,windDirection);
  QString fileName = QString("temporary-%1-%2.prj").arg(windSpeed).arg(windDirection);
  openstudio::path inputPath = openstudio::toPath(fileName); // This should do something else
  //QString fileName = openstudio::toQString(inputPath);
  
  QFile file(fileName);
  if(!file.open(QFile::WriteOnly))
  {
    std::cout << "Failed to open file '"<< fileName.toStdString() << "'." << std::endl;
    std::cout << "Check that this file location is accessible and may be written." << std::endl;
    return QVector<double>();
  }
  QTextStream textStream(&file);
  if(verbose)
  {
    std::cout << "Writing file " << fileName.toStdString() << std::endl;
  }
  boost::optional<std::string> output = translator.toString();
  textStream << openstudio::toQString(*output);
  file.close();

  // Run simulation
  if(verbose)
  {
    std::cout << "Running CONTAM simulation (" << windSpeed << "," << windDirection 
              << ")..." << std::endl;
  }
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
  {
    std::cout << "Running SimRead on SIM file..." << std::endl;
  }
  QProcess simreadProcess;
#ifndef SIMREADX
  simreadProcess.start(openstudio::toQString(simreadExePath), QStringList() << fileName);
  if(!simreadProcess.waitForStarted(-1))
  {
    std::cout << "Failed to start SimRead process." << std::endl;
    return QVector<double>();
  }
  simreadProcess.write("y\n\ny\n\n"); // This should work for no contaminants
#else
  simreadProcess.start(openstudio::toQString(simreadExePath), QStringList() << "-F" << fileName);
  if(!simreadProcess.waitForStarted(-1))
  {
    std::cout << "Failed to start SimRead process." << std::endl;
    return QVector<double>();
  }
#endif
  if(!simreadProcess.waitForFinished(-1))
  {
    std::cout << "Failed to complete SimRead process." << std::endl;
    return QVector<double>();
  }

  // Collect results
  if(verbose)
  {
    std::cout << "Reading results..." << std::endl;
  }
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
  // Some ugly hard coded paths
  // Windows
  openstudio::path contamExePath = openstudio::toPath("C:\\Program Files (x86)\\NIST\\CONTAM 3.1\\ContamX3.exe");
  openstudio::path simreadExePath = openstudio::toPath("C:\\Users\\jwd131\\Software\\CONTAM\\simread31.exe");
  // Linux
  //openstudio::path contamExePath = openstudio::toPath("/usr/local/bin/contamx-3.1-linux-release-static.exe");
  //openstudio::path simreadExePath = openstudio::toPath("/usr/local/bin/simreadx-linux-static");

  double density = 1.2041;

  std::string inputPathString;
  std::string outputPathString="customInfiltration.osm";
  std::string leakageDescriptorString="Average";
  double flow=27.1;
  boost::program_options::options_description desc("Allowed options");
  desc.add_options()
    ("flow,f", boost::program_options::value<double>(&flow), "leakage flow rate per envelope area [m^3/h/m^2]")
    ("help,h", "print help message")
    ("input-path,i", boost::program_options::value<std::string>(&inputPathString), "path to input OSM file")
    ("level,l", boost::program_options::value<std::string>(&leakageDescriptorString), "airtightness: Leaky|Average|Tight (default: Average)")
    ("no-osm", "suppress output of OSM file")
    ("output-path,o", boost::program_options::value<std::string>(&outputPathString), "path to output OSM file")
    ("quiet,q", "suppress progress output")
    ("write-coeffs","write out coefficients in coeffs.txt");

  boost::program_options::positional_options_description pos;
  pos.add("input-path", -1);
    
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
  {
    verbose=false;
  }

  bool noOsm=false;
  if(vm.count("no-osm"))
  {
    noOsm=true;
  }

  if (!vm.count("input-path"))
  {
    std::cout << "No input path given." << std::endl << std::endl;
    usage(desc);
    return EXIT_FAILURE;
  }

  bool writeCoeffs=false;
  if(vm.count("write-coeffs"))
  {
    writeCoeffs=true;
  }

  openstudio::path inputPath = openstudio::toPath(inputPathString);
  openstudio::osversion::VersionTranslator vt;
  boost::optional<openstudio::model::Model> model = vt.loadModel(inputPath);

  if(!model)
  {
    std::cout << "Unable to load file '"<< inputPathString << "' as an OpenStudio model." << std::endl;
    return EXIT_FAILURE;
  }

  openstudio::contam::ForwardTranslator translator;

  if(vm.count("flow"))
  {
    std::cout << flow << std::endl;
    if(!translator.translate(*model,flow,false))
    {
      std::cout << "Translation failed, check errors and warnings for more information." << std::endl;
      return EXIT_FAILURE;
    }
  }
  else
  {
    if(!translator.translate(*model,false,leakageDescriptorString))
    {
      std::cout << "Translation failed, check errors and warnings for more information." << std::endl;
      return EXIT_FAILURE;
    }
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
  {
    std::cout << "Found " << extSurfaces.size() << " exterior surfaces." << std::endl;
  }

  int nspaces = model->getConcreteModelObjects<openstudio::model::Space>().size();

  // This is a stand-in for a more complete procedure (such as Ng et al.)

  QVector<double> Q10(nspaces,0.0);
  QVector<double> Q20(nspaces,0.0);

  for(double angle=0.0;angle<360.0;angle+=90.0)
  {
    QVector<double> results = runCase(spaceMap, translator, extSurfaces, 4.4704, angle, contamExePath,
      simreadExePath, verbose);
    for(int i=0;i<results.size();i++)
    {
      Q10[i] += results[i];
    }
    results = runCase(spaceMap, translator, extSurfaces, 8.9408, angle, contamExePath,
      simreadExePath, verbose);
    for(int i=0;i<results.size();i++)
    {
      Q20[i] += results[i];
    }
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
    //C << (400.0-100*Q20[i]/Q10[i])/2000.0;
    //D << ( 10*Q20[i]/Q10[i]- 20.0)/2000.0;
    double denom = 4.4704*8.9408*8.9408 - 8.9408*4.4704*4.4704;
    C << (8.9408*8.9408 - 4.4704*4.4704*Q20[i]/Q10[i])/denom;
    D << (4.4704*Q20[i]/Q10[i] - 8.9408)/denom;
  }
  
  //for(int i=0;i<Q10.size();i++)
  //{
  //  std::cout<<i<<" "<<C[i]<<" "<<D[i]<<std::endl;
  //}

  // Check fit accuracy
  if(verbose)
  {
    for(int i=0;i<Q10.size();i++)
    {
      double calcQ10 = Q10[i]*(C[i]*4.4704 + D[i]*4.4704*4.4704);
      double calcQ20 = Q10[i]*(C[i]*8.9408 + D[i]*8.9408*8.9408);
      std::cout<<openstudio::toString(spaces[i].handle())<<", 10 mph error: "<<Q10[i]-calcQ10<<std::endl;
      std::cout<<openstudio::toString(spaces[i].handle())<<", 20 mph error: "<<Q20[i]-calcQ20<<std::endl;
    }
  }

  // Remove previous infiltration objects
  std::vector<openstudio::model::SpaceInfiltrationDesignFlowRate> dfrInf = model->getConcreteModelObjects<openstudio::model::SpaceInfiltrationDesignFlowRate>();
  BOOST_FOREACH(openstudio::model::SpaceInfiltrationDesignFlowRate inf, dfrInf)
  {
    inf.remove();
  }
  std::vector<openstudio::model::SpaceInfiltrationEffectiveLeakageArea> elaInf = model->getConcreteModelObjects<openstudio::model::SpaceInfiltrationEffectiveLeakageArea>();
  BOOST_FOREACH(openstudio::model::SpaceInfiltrationEffectiveLeakageArea inf, elaInf)
  {
    inf.remove();
  }

  // Generate infiltration objects and attach to spaces
  std::pair <openstudio::Handle,int> handleInt;
  BOOST_FOREACH(handleInt, spaceMap)
  {
    boost::optional<openstudio::model::Space> space = model->getModelObject<openstudio::model::Space>(handleInt.first);
    if(!space)
    {
      std::cout << "Failed to find space " << openstudio::toString(handleInt.first)<< std::endl;
      return EXIT_FAILURE;
    }
    openstudio::model::SpaceInfiltrationDesignFlowRate infObj(*model);
    infObj.setDesignFlowRate(density*Q10[handleInt.second]);
    infObj.setConstantTermCoefficient(0.0);
    infObj.setTemperatureTermCoefficient(0.0);
    infObj.setVelocityTermCoefficient(C[handleInt.second]);
    infObj.setVelocitySquaredTermCoefficient(D[handleInt.second]);
    infObj.setSpace(*space);
  }

  // Write out new OSM
  // QString outstring = openstudio::toQString(inputPathString).replace(".osm",openstudio::toQString(leakageDescriptorString)+".osm");
  if(!noOsm)
  {
    openstudio::path outPath = openstudio::toPath(outputPathString);
      if(!model->save(outPath,true))
      {
        std::cout << "Failed to write OSM file." << std::endl;
        return EXIT_FAILURE;
      }
  }

  // Write out the coefficients if required
  if(writeCoeffs)
  {
    QFile txtfile("coeffs.txt");
    if(!txtfile.open(QFile::WriteOnly))
    {
      std::cout << "Failed to open file 'coeffs.txt'" << std::endl;
      std::cout << "Check that this file location is accessible and may be written." << std::endl;
    }
    else
    {
      QTextStream coeffs(&txtfile);
      BOOST_FOREACH(handleInt, spaceMap)
      {
        boost::optional<openstudio::model::Space> space = model->getModelObject<openstudio::model::Space>(handleInt.first);
        if(!space)
        {
          std::cout << "Failed to find space " << openstudio::toString(handleInt.first)<< std::endl;
          return EXIT_FAILURE;
        }
        coeffs << openstudio::toString(space.get().handle()) << ' ' 
          << density*Q10[handleInt.second] << ' '
          << C[handleInt.second] << ' '
          << D[handleInt.second] << '\n';
      }
    }
  }

  openstudio::contam::InfiltrationCalculator calc(*model);

  calc.run();

  std::map<openstudio::Handle,openstudio::contam::DesignFlowRateCoeffs> coeffs = calc.coeffs();

  std::pair <openstudio::Handle,openstudio::contam::DesignFlowRateCoeffs> handleCoeff;
  BOOST_FOREACH(handleCoeff, coeffs)
  {
    std::cout << openstudio::toString(handleCoeff.first) << ' ' << handleCoeff.second.Idesign() << ' '
              << handleCoeff.second.C() << ' ' << handleCoeff.second.D() <<std::endl;
  }
    
  return EXIT_SUCCESS;
}
