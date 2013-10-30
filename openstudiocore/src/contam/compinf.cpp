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

#include <model/WeatherFile.hpp>
#include <utilities/filetypes/EpwFile.hpp>

#include <QProcess>

#include <string>
#include <iostream>

void usage(boost::program_options::options_description desc)
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
  // Some ugly hard coded paths
  // Windows
  //openstudio::path contamExePath = openstudio::toPath("C:\\Program Files (x86)\\NIST\\CONTAM 3.1\\ContamX3.exe");
  //openstudio::path simreadExePath = openstudio::toPath("C:\\Users\\jwd131\\Software\\CONTAM\\simread31.exe");
  // Linux
  //openstudio::path contamExePath = openstudio::toPath("/usr/local/bin/contamx-3.1-linux-release-static.exe");
  //openstudio::path simreadExePath = openstudio::toPath("/usr/local/bin/simreadx-linux-static");

  double density = 1.2041;

  std::string csvPathString;
  std::string inputPathString;
  std::string outputPathString="customInfiltration.osm";
  std::string leakageDescriptorString="Average";
  double flow=27.1;
  boost::program_options::options_description desc("Allowed options");
  desc.add_options()
    ("eval,e", boost::program_options::value<std::string>(&csvPathString),"store E+ weather and infiltration results in a csv file")
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

  if (!vm.count("input-path"))
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

  if(vm.count("eval"))
  {
    openstudio::path dir = inputPath.parent_path() / inputPath.stem();
    openstudio::path epwPath;
    // First try to load the weather file
    boost::optional<openstudio::model::WeatherFile> weatherFile = model->weatherFile();
    if(weatherFile)
    {
      boost::optional<openstudio::path> path = weatherFile->path();
      if(path)
      {
        boost::optional<openstudio::path> epwOpt = findFile(dir,openstudio::toString(path->string()));
        if(epwOpt)
        {
          epwPath = epwOpt.get();
        }
        else
        {
          std::cout << "Failed to find EPW file, infiltration evaluation file will not be written" << std::endl;
          return EXIT_FAILURE;
        }
      }
      else
      {
        std::cout << "No path to EPW file, infiltration evaluation file will not be written" << std::endl;
        return EXIT_FAILURE;
      }
    }
    else
    {
      std::cout << "No weather file object to process, infiltration evaluation file will not be written" << std::endl;
      return EXIT_FAILURE;
    }
    std::cout << openstudio::toString(epwPath) << std::endl;
    openstudio::EpwFile epwFile(epwPath,true);
    boost::optional<openstudio::TimeSeries> windSpeed = epwFile.timeSeries("Wind Speed");
    if(windSpeed)
      std::cout << "Found wind speed, has " << windSpeed->values().size() << " entries." << std::endl;
    else
      std::cout << "Failed to find wind speed." << std::endl;
    boost::optional<openstudio::TimeSeries> windDir = epwFile.timeSeries("Wind Direction");
    if(windDir)
      std::cout << "Found wind direction, has " << windDir->values().size() << " entries." << std::endl;
    else
      std::cout << "Failed to find wind direction." << std::endl;

    openstudio::Time delta(0,1);
    openstudio::Time startTime;
    openstudio::Time endTime;
    openstudio::DateTime current=openstudio::DateTime(epwFile.startDate(),startTime);
    //for(openstudio::DateTime current=openstudio::DateTime(epwFile.startDate(),startTime);
    //  current<=openstudio::DateTime(epwFile.endDate(),endTime);current += delta)
    for(int i=0;i<10;i++)
    {
      current += delta;
      std::cout << QString("%1/%2\t%3\t%3\t0\t283.15").arg(openstudio::month(current.date().monthOfYear()))
        .arg(current.date().dayOfMonth()).arg(windSpeed->value(current)).toStdString() <<std::endl;
    }
    openstudio::Vector values = windSpeed->values();
    std::cout << windSpeed->values()[1] << std::endl;
    for(int i=0;i<10;i++)
    {
      std::cout << epwFile.data()[i].fieldByName("Wind Speed").get() << " " 
        << epwFile.data()[i].windSpeed().get() << " " << values[i] << std::endl;
    }
    return EXIT_SUCCESS;
  }

  openstudio::contam::InfiltrationCalculator calc(*model);

  if(vm.count("flow"))
  {
    calc.setFlowAt75Pa(flow);
  }
  else
  {
    calc.setLeakageDescriptor(leakageDescriptorString);
  }

  if(!calc.run())
  {
    std::cout << "Infiltration calculation failed, check for errors or warnings and try again." << std::endl;
    return EXIT_FAILURE;
  }

  std::map<openstudio::Handle,openstudio::contam::DesignFlowRateCoeffs> coeffs = calc.coeffs();

  // Write out the coefficients if required
  if(vm.count("write-coeffs"))
  {
    QFile txtfile("coeffs.txt");
    if(!txtfile.open(QFile::WriteOnly))
    {
      std::cout << "Failed to open file 'coeffs.txt'" << std::endl;
      std::cout << "Check that this file location is accessible and may be written." << std::endl;
    }
    else
    {
      QTextStream output(&txtfile);
      std::pair <openstudio::Handle,openstudio::contam::DesignFlowRateCoeffs> handleCoeff;
      BOOST_FOREACH(handleCoeff, coeffs)
      {
        output << openstudio::toString(handleCoeff.first) << ' ' << handleCoeff.second.Idesign() << ' '
               << handleCoeff.second.C() << ' ' << handleCoeff.second.D() << '\n';
      }
    }
  }

  std::pair <openstudio::Handle,openstudio::contam::DesignFlowRateCoeffs> handleCoeff;
  BOOST_FOREACH(handleCoeff, coeffs)
  {
    std::cout << openstudio::toString(handleCoeff.first) << ' ' << handleCoeff.second.Idesign() << ' '
              << handleCoeff.second.C() << ' ' << handleCoeff.second.D() <<std::endl;
  }

  // Write out new OSM
  if(!vm.count("no-osm"))
  {
    calc.apply();
    openstudio::path outPath = openstudio::toPath(outputPathString);
    if(!model->save(outPath,true))
    {
      std::cout << "Failed to write OSM file." << std::endl;
      return EXIT_FAILURE;
    }
  }

  return EXIT_SUCCESS;
}
