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
#include <osversion/VersionTranslator.hpp>
#include <utilities/core/CommandLine.hpp>
#include <utilities/core/Path.hpp>

#include <string>
#include <iostream>

void usage( boost::program_options::options_description desc)
{
  std::cout << "Usage: OsmToPrj --inputPath=./path/to/input.osm" << std::endl;
  std::cout << "   or: OsmToPrj input.osm" << std::endl;
  std::cout << desc << std::endl;
}

int main(int argc, char *argv[])
{
  std::string inputPathString;
  std::string leakageDescriptorString="Average";
  double flow=27.1;
  bool setLevel = true;
  boost::program_options::options_description desc("Allowed options");
  desc.add_options()
    ("help", "print help message")
    ("inputPath", boost::program_options::value<std::string>(&inputPathString), "path to OSM file");

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
    // Build the airflow element map 
    /*
    if(!translator.translate(*model,flow,false))
    {
      std::cout << "Translation failed, check errors and warnings for more information." << std::endl;
      return EXIT_FAILURE;
    }
    */
  }
  else
  {
    /*
    if(!translator.translate(*model,false,leakageDescriptorString))
    {
      std::cout << "Translation failed, check errors and warnings for more information." << std::endl;
      return EXIT_FAILURE;
    }
    */
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
  if(setLevel)
  {
    translator.setAirtightnessLevel(leakageDescriptorString);
  }
  QFile file(openstudio::toQString(prjPath));
  if(file.open(QFile::WriteOnly))
  {
    QTextStream textStream(&file);
    if(!translator.translate(*model))
    {
      std::cout << "Translation failed, check errors and warnings for more information." << std::endl;
      return EXIT_FAILURE;
    }
    textStream << openstudio::toQString(translator.toString());
  }
  else
  {
    std::cout << "Failed to open file '"<< openstudio::toString(prjPath) << "'." << std::endl;
    std::cout << "Check that this file location is accessible and may be written." << std::endl;
    return EXIT_FAILURE;
  }
  file.close();
    // The details on what we should do with these maps are still unclear
    // openstudio::path mapPath = inputPath.replace_extension(openstudio::toPath("map").string());
    // translator.writeMaps(mapPath);
  return EXIT_SUCCESS;
}
