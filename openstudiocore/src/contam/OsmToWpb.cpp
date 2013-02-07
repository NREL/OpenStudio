/**********************************************************************
 *  Copyright (c) 2008-2010, Alliance for Sustainable Energy.
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

#include <utilities/core/CommandLine.hpp>
#include <utilities/core/Path.hpp>

#include <string>
#include <iostream>

void printHelp( boost::program_options::options_description desc) {
  std::cout << "Usage: OsmToWpb --inputPath=./path/to/input.osm" << std::endl;
  std::cout << "   or: OsmToWpb input.osm" << std::endl;
  std::cout << desc << std::endl;
}

int main(int argc, char *argv[]) {
  std::string inputPathString;
  
  boost::program_options::options_description desc("Allowed options");
  desc.add_options()
      ("help", "print help message")
      ("inputPath", boost::program_options::value<std::string>(&inputPathString), 
       "path to OSM file");

  boost::program_options::positional_options_description pos;
  pos.add("inputPath", -1);

  boost::program_options::variables_map vm;
  // The following try/catch block is necessary to avoid uncaught
  // exceptions when the program is executed with more than one
  // "positional" argument - there's got to be a better way.
  try {
    boost::program_options::store(boost::program_options::command_line_parser(argc, argv).
				  options(desc).positional(pos).run(), vm);
    boost::program_options::notify(vm);
  }
  catch(std::exception&) {
    std::cout << "Execution failed: check arguments and retry."<< std::endl << std::endl;
    printHelp(desc);
    return EXIT_FAILURE;
  }
  if (vm.count("help")) {
    printHelp(desc);
    return EXIT_SUCCESS;
  }
  if (vm.count("inputPath")) {
    openstudio::path inputPath = openstudio::toPath(inputPathString);
    boost::optional<openstudio::model::Model> model = openstudio::model::Model::load(inputPath);
    
    if(!model) {
      std::cout << "Unable to load file '"<< inputPathString << "' as an OpenStudio model." 
		<< std::endl;
      return EXIT_FAILURE;
    }
    openstudio::path outPath = inputPath.replace_extension(openstudio::toPath("wpb").string());
    bool success = openstudio::contam::ForwardTranslator::modelToJson(*model, outPath);
    if(!success) {
      std::cout << "Failed to write file '"<< openstudio::toString(outPath.string()) << "'." << std::endl;
      std::cout << "Check that this file location is accessible and may be written." << std::endl;
      return EXIT_FAILURE;
    }
  }
  else { 
    std::cout << "No input path given." << std::endl << std::endl;
    printHelp(desc);
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}



