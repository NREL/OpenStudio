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
#include <model/Model.hpp>
#include <model/WeatherFile.hpp>
#include <osversion/VersionTranslator.hpp>
#include <utilities/core/CommandLine.hpp>
#include <utilities/core/Path.hpp>
#include <utilities/sql/SqlFile.hpp>

#include <string>
#include <iostream>

void usage( boost::program_options::options_description desc)
{
  std::cout << "Usage: OsmToPrj --input-path=./path/to/input.osm" << std::endl;
  std::cout << "   or: OsmToPrj input.osm" << std::endl;
  std::cout << desc << std::endl;
}

static boost::optional<openstudio::path> findFile(openstudio::path base, std::string filename)
{
  //boost::filesystem::recursive_directory_iterator(base);
  //boost::filesystem::recursive_directory_iterator();
  //boost::filesystem2::basic_recursive_directory_iterator(base);
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
    translator = new openstudio::contam::ForwardTranslator(model.get(),leakageDescriptorString,returnSupplyRatio);
  }
  else
  {
    translator = new openstudio::contam::ForwardTranslator(model.get(),returnSupplyRatio,flow,0.65,75.0);
  }
  if(!translator->valid())
  {
     std::cout << "Translation failed, check errors and warnings for more information." << std::endl;
     delete translator;
     return EXIT_FAILURE;
  }
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
          if(translator->translateEpw(*epwPath,wthPath))
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
  
  return EXIT_SUCCESS;
}
