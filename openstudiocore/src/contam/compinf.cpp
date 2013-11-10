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
#include <contam/SimFile.hpp>

#include <model/Model.hpp>
#include <model/Space.hpp>
#include <model/Space_Impl.hpp>
#include <model/ThermalZone.hpp>
#include <model/WeatherFile.hpp>
#include <model/ScheduleFixedInterval.hpp>
#include <osversion/VersionTranslator.hpp>
#include <utilities/core/CommandLine.hpp>
#include <utilities/core/Path.hpp>
#include <utilities/sql/SqlFile.hpp>

#include <model/SpaceInfiltrationDesignFlowRate.hpp>
#include <model/SpaceInfiltrationDesignFlowRate_Impl.hpp>
#include <model/SpaceInfiltrationEffectiveLeakageArea.hpp>
#include <model/SpaceInfiltrationEffectiveLeakageArea_Impl.hpp>

#include <QProcess>

#include <string>
#include <iostream>

void usage( boost::program_options::options_description desc)
{
  std::cout << "Usage: compinf --input-path=./path/to/input.osm" << std::endl;
  std::cout << "   or: compinf input.osm" << std::endl;
  std::cout << desc << std::endl;
}

static boost::optional<openstudio::path> findFile(openstudio::path base, std::string filename)
{
  if(boost::filesystem::is_directory(base))
  {
    openstudio::path filepath = base / openstudio::toPath(filename);
    std::cout<<"Looking for "<<openstudio::toString(filepath)<<std::endl;
    if(boost::filesystem::exists(filepath))
    {
      return boost::optional<openstudio::path>(filepath);
    }
    // WHY!?!?!
    boost::filesystem2::path basepath(openstudio::toString(base));
    boost::filesystem::directory_iterator iter(basepath);
    boost::filesystem::directory_iterator end;
    for(;iter!=end;++iter)
    {
      boost::optional<openstudio::path> optional = findFile(openstudio::toPath(iter->path().string()),filename);
      if(optional)
      {
        return optional;
      }
    }
  }
  return false;
}

int main(int argc, char *argv[])
{
  std::string inputPathString;
  std::string outputPathString = "scheduled-infiltration.osm";
  std::string leakageDescriptorString="Average";
  double flow=27.1;
  double returnSupplyRatio=1.0;
  bool setLevel = true;
  boost::program_options::options_description desc("Allowed options");

  desc.add_options()
    ("flow,f", boost::program_options::value<double>(&flow), "leakage flow rate per envelope area [m^3/h/m^2]")
    ("help,h", "print help message")
    ("input-path,i", boost::program_options::value<std::string>(&inputPathString), "path to input OSM file")
    ("level,l", boost::program_options::value<std::string>(&leakageDescriptorString), "airtightness: Leaky|Average|Tight (default: Average)")
    ("quiet,q", "suppress progress output");

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
  
  if(vm.count("help"))
  {
    usage(desc);
    return EXIT_SUCCESS;
  }

  if(!vm.count("input-path"))
  {
    std::cout << "No input path given." << std::endl << std::endl;
    usage(desc);
    return EXIT_FAILURE;
  }

  if(vm.count("flow"))
  {
    // Probably should do a sanity check of input - but maybe later
    setLevel = false;
  }
  
  // Open the model
  openstudio::path inputPath = openstudio::toPath(inputPathString);
  openstudio::osversion::VersionTranslator vt;
  boost::optional<openstudio::model::Model> model = vt.loadModel(inputPath);

  if(!model)
  {
    std::cout << "Unable to load file '"<< inputPathString << "' as an OpenStudio model." << std::endl;
    return EXIT_FAILURE;
  }

  // Try to find and connect a results file - this really should be done using the RunManager database,
  // but I don't know how to do that and it can be done right at a later date by someone who knows how
  openstudio::path dir = inputPath.parent_path() / inputPath.stem();
  boost::optional<openstudio::path> sqlpath = findFile(dir,"eplusout.sql");
  if(sqlpath)
  {
    std::cout<<"Found results file, attaching it to the model."<<std::endl;
    model->setSqlFile(openstudio::SqlFile(*sqlpath));
  }

  openstudio::path prjPath = inputPath.replace_extension(openstudio::toPath("prj").string());
  openstudio::path cvfPath = inputPath.replace_extension(openstudio::toPath("cvf").string());
  openstudio::path wthPath = inputPath.replace_extension(openstudio::toPath("wth").string());
  openstudio::path simPath = inputPath.replace_extension(openstudio::toPath("sim").string());

  bool needWth = true;
  if(boost::filesystem::exists(wthPath))
  {
    needWth = false;
  }

  openstudio::contam::ForwardTranslator *translator=0;
  if(setLevel)
  {
    QVector<std::string> known;
    known << "Tight" << "Average" << "Leaky";
    if(!known.contains(leakageDescriptorString))
    {
      std::cout << "Unknown airtightness level '" << leakageDescriptorString << "'" << std::endl;
      return EXIT_FAILURE;
    }
    translator = new openstudio::contam::ForwardTranslator(model.get(),leakageDescriptorString,false);
  }
  else
  {
    translator = new openstudio::contam::ForwardTranslator(model.get(),flow,0.65,75.0,false);
  }
  if(!translator->valid())
  {
     std::cout << "Translation failed, check errors and warnings for more information." << std::endl;
     delete translator;
     return EXIT_FAILURE;
  }
  // Since we really need this to be a transient case, bail out now if it is not
  if(!translator->startDateTime() || !translator->endDateTime())
  {
    std::cout << "The translated model is a steady-state model, bailing out" << std::endl;
    return EXIT_FAILURE;
  }
  boost::optional<openstudio::EpwFile> epwFile;
  QFile file(openstudio::toQString(prjPath));
  if(file.open(QFile::WriteOnly))
  {
    QTextStream textStream(&file);
    // Attempt to translate weather
    boost::optional<openstudio::model::WeatherFile> weatherFile = model->weatherFile();
    if(weatherFile)
    {
      boost::optional<openstudio::path> path = weatherFile->path();
      if(path)
      {
        boost::optional<openstudio::path> epwPath = findFile(dir,openstudio::toString(path->string()));
        if(epwPath)
        {
          if(!needWth)
          {
            try
            {
              epwFile = boost::optional<openstudio::EpwFile>(openstudio::EpwFile(epwPath.get(),true));
            }
            catch(...)
            {
              std::cout << "Failed to correctly load EPW file, weather will be steady state" << std::endl;
            }
            translator->rc().setWTHpath(openstudio::toString(wthPath));
          }
          else if(epwFile = translator->translateEpw(*epwPath,wthPath))
          {
            translator->rc().setWTHpath(openstudio::toString(wthPath));
          }
          else
          {
            std::cout << "EPW translation to WTH failed, WTH file will not be used in simulation" << std::endl;
          }
        }
        else
        {
          std::cout << "Failed to find EPW file, WTH file will not be written" << std::endl;
        }
      }
      else
      {
        std::cout << "No path to EPW file, WTH file will not be written" << std::endl;
      }
    }
    else
    {
      std::cout << "No weather file object to process, WTH file will not be written" << std::endl;
    }

    // Write out a CVF if needed
    if(translator->writeCvFile(cvfPath))
    {
      // Need to set the CVF file in the PRJ, this path may need to be made relative. Not too sure
      translator->rc().setCVFpath(openstudio::toString(cvfPath));
    }
    textStream << openstudio::toQString(translator->toString());
  }
  else
  {
    std::cout << "Failed to open file '"<< openstudio::toString(prjPath) << "'." << std::endl;
    std::cout << "Check that this file location is accessible and may be written." << std::endl;
    return EXIT_FAILURE;
  }
  file.close();

  // Now we should have a CONTAM model file. There will need to be some steps taken if parts of the 
  // process have not been successful (e.g. the creation of a WTH file), but for now just assume that
  // everything worked.
  //
  // Run CONTAM on the PRJ file
  //
  std::cout << "Running CONTAM simulation" << std::endl;
  // Ugly hard code
  openstudio::path contamExe = openstudio::toPath("C:\\Program Files (x86)\\NIST\\CONTAM 3.1\\ContamX3.exe");
  openstudio::path simreadxExe = openstudio::toPath("C:\\Users\\jwd131\\Software\\CONTAM\\simreadx.exe");
  //
  // Run CONTAM
  //
  QProcess contamProcess;
  contamProcess.start(openstudio::toQString(contamExe), QStringList() << openstudio::toQString(prjPath));
  if(!contamProcess.waitForStarted(-1))
  {
    std::cout << "Failed to start ContamX process." << std::endl;
    return EXIT_FAILURE;
  }
  if(!contamProcess.waitForFinished(-1))
  {
    std::cout << "Failed to complete ContamX process." << std::endl;
    return EXIT_FAILURE;
  }
  std::cout << "Successfully ran ContamX" << std::endl;
  //
  // Run SimRead - this will hopefully go away at some point
  //
  QProcess simreadProcess;
  simreadProcess.start(openstudio::toQString(simreadxExe), QStringList() << "-a" << openstudio::toQString(prjPath));
  if(!simreadProcess.waitForStarted(-1))
  {
    std::cout << "Failed to start SimReadX process." << std::endl;
    return EXIT_FAILURE;
  }
  if(!simreadProcess.waitForFinished(-1))
  {
    std::cout << "Failed to complete SimReadX process." << std::endl;
    return EXIT_FAILURE;
  }
  std::cout << "Successfully ran SimReadX" << std::endl;
  //
  // Read in the results
  //
  openstudio::contam::SimFile sim(simPath);
  /*
  std::cout << sim.F0().size() << std::endl;
  std::cout << sim.F1().size() << std::endl;
  std::vector<std::vector<int> > extPaths = translator->zoneExteriorFlowPaths();
  for(unsigned int i=0;i<extPaths.size();i++)
  {
    std::cout << i+1 << " ";
    for(unsigned int j=0;j<extPaths[i].size();j++)
    {
      std::cout << extPaths[i][j] << " ";
    }
    std::cout << std::endl;
  }
  */
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
  // Set the default here in case the EpwFile route fails
  openstudio::Time diff = translator->endDateTime().get()-translator->startDateTime().get();
  //std::cout << diff.days()*24 << std::endl;
  double ssP = QString().fromStdString(translator->rc().ssWeather().Tambt()).toDouble(); // There's a better way to do this
  double ssT = QString().fromStdString(translator->rc().ssWeather().barpres()).toDouble(); // There's a better way to do this
  //std::cout << ssP << " " << ssT << std::endl;
  //std::vector<double> values(diff.days()*24,287.058*T/P);
  //openstudio::Vector oneOverDensity = openstudio::createVector(std::vector<double>(diff.days()*24,287.058*T/P));
  // Try to get the outdoor conditions
  openstudio::TimeSeries seriesP;
  openstudio::TimeSeries seriesT;
  bool variableWeather = false;
  if(epwFile)
  {
    boost::optional<openstudio::TimeSeries> optP = epwFile->timeSeries("Atmospheric Station Pressure");
    boost::optional<openstudio::TimeSeries> optT = epwFile->timeSeries("Dry Bulb Temperature");
    if(optP && optT)
    {
      variableWeather = true;
      seriesP = optP.get();
      seriesT = optT.get();
    }
    //std::vector<double>
    /*
    if(optP && optT)
    {
      // For maximum safety, use interpolation to paper over any missing data
      openstudio::Time delta(0,1); // Do an hourly schedule
      std::vector<double> values;
      for(openstudio::DateTime current=translator->startDateTime().get()+delta; current <= translator->endDateTime().get(); current += delta)
      {
        P = optP.get().value(current);
        T = optT.get().value(current) + 273.15;
        values.push_back(287.058*T/P);
      }
      oneOverDensity = openstudio::createVector(values);
    }
    */
  }
  std::vector<openstudio::TimeSeries> infiltration = translator->zoneInfiltration(&sim); // These are in kg/s
  // Create a schedule for each zone
  std::map<openstudio::Handle,int> map = translator->zoneMap();
  // This approach implicitly requires that each space is a zone. There should be a way to distibute the infiltration so that when it 
  // is all put together it adds up to the right thing. That would allow for more than one sapce in a zone - which needs to happen at
  // some point.
  std::vector<openstudio::model::Space> spaces = model->getConcreteModelObjects<openstudio::model::Space>();
  BOOST_FOREACH(openstudio::model::Space space, spaces)
  {
    boost::optional<openstudio::model::ThermalZone> zone = space.thermalZone();
    if(!zone)
    {
      std::cout << "Space '" << openstudio::toString(space.handle()) << "' has no attached thermal zone." << std::endl;
      continue;
    }
    if(map.count(zone->handle()) == 0)
    {
      std::cout << "Zone '" << openstudio::toString(zone->handle()) << "' has no associated CONTAM zone." << std::endl;
      continue;
    }
    openstudio::TimeSeries zoneInfiltration = infiltration[map[zone->handle()]-1];
    /*
    std::cout << zoneInfiltration.value(0) << std::endl;
    openstudio::DateTime dt(openstudio::Date(1,1),openstudio::Time(0,2));
    std::cout << dt.toString() << " " << zoneInfiltration.value(dt) << std::endl;
    std::cout << zoneInfiltration.firstReportDateTime().toString() << std::endl;
    for(unsigned i=0;i<zoneInfiltration.values().size();i++)
    {
      std::cout << zoneInfiltration.values()[i] << std::endl;
      //if(i==20)exit(0);
    }
    */
    openstudio::Time delta(0,1); // Do an hourly schedule
    std::vector<double> values;
    for(openstudio::DateTime current=translator->startDateTime().get()+delta; current <= translator->endDateTime().get(); current += delta)
    {
      double inf = zoneInfiltration.value(current); // This will be in kg/s
      double P = ssP;
      double T = ssT;
      if(variableWeather)
      {
        P = seriesP.value(current);
        T = seriesT.value(current) + 273.15;
        //std::cout << current.toString() << " " << inf << " " << P << " " << T << std::endl;
      }
      values.push_back(inf*287.058*T/P); // Compute m^3/s
    }
    // Make the time series
    openstudio::TimeSeries infiltrationTimeSeries(translator->startDateTime()->date(),delta,openstudio::createVector(values),"");
    //std::cout << infiltrationTimeSeries.firstReportDateTime().toString() << std::endl;
    // Make the schedule
    openstudio::model::ScheduleFixedInterval schedule(*model);
    schedule.setTimeSeries(infiltrationTimeSeries);

    openstudio::model::SpaceInfiltrationDesignFlowRate infObj(*model);
    infObj.setDesignFlowRate(1.0);
    infObj.setConstantTermCoefficient(1.0);
    infObj.setSpace(space);
    infObj.setSchedule(schedule);
  }

  //std::cout << seriesP.firstReportDateTime().toString() << std::endl;
  //std::cout << seriesT.firstReportDateTime().toString() << std::endl;
  //std::cout << translator->startDateTime().get().toString() << std::endl;

  //openstudio::model::ScheduleFixedInterval schedule(*model);
  //schedule.setTimeSeries(infiltration[0]);
  openstudio::path outPath = openstudio::toPath(outputPathString);
  if(!model->save(outPath,true))
  {
    std::cout << "Failed to write OSM file." << std::endl;
    return EXIT_FAILURE;
  }
  /*
  std::cout << infiltration.size() << std::endl;
  openstudio::Vector values = infiltration[0].values();
  openstudio::Vector daysFrom = infiltration[0].daysFromFirstReport();
  for(unsigned int i=0;i<values.size();i++)
  {
    std::cout << i << '\t' << daysFrom[i] << '\t' << values[i] << std::endl;
  }
  */
  // The details on what we should do with these maps are still unclear
  // openstudio::path mapPath = inputPath.replace_extension(openstudio::toPath("map").string());
  // translator.writeMaps(mapPath);
  return EXIT_SUCCESS;
}
