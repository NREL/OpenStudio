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

void usage(boost::program_options::options_description desc)
{
  std::cout << "Usage: FitInfiltration --inputPath=./path/to/input.osm" << std::endl;
  std::cout << "   or: FitInfiltration input.osm" << std::endl;
  std::cout << desc << std::endl;
}

int main(int argc, char *argv[])
{
  std::string inputPathString;
  boost::program_options::options_description desc("Allowed options");
  desc.add_options()
    ("help", "print help message")
    ("inputPath", boost::program_options::value<std::string>(&inputPathString), "path to OSM file");

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

  if (vm.count("inputPath"))
  {
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
    if(file.open(QFile::WriteOnly))
    {
      QTextStream textStream(&file);
      boost::optional<QString> output = translator.translateToPrj(*model);
      if(output)
      {
        textStream << *output;
      }
      else
      {
        std::cout << "Translation failed, check errors and warnings for more information." << std::endl;
        return EXIT_FAILURE;
      }
    }
    else
    {
      std::cout << "Failed to open file '"<< openstudio::toString(prjPath) << "'." << std::endl;
      std::cout << "Check that this file location is accessible and may be written." << std::endl;
      return EXIT_FAILURE;
    }
    file.close();
    // Run simulation

    // Collect results

    // Process results

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
  }
  else
  {
    std::cout << "No input path given." << std::endl << std::endl;
    usage(desc);
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
