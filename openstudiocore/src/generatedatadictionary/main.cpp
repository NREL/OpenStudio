/**********************************************************************
*  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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

#include <generatedatadictionary/GenerateDataDictionary.hpp>

#include <utilities/core/GeneratorApplicationPathHelpers.hpp>

#include <boost/program_options.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/foreach.hpp>

#include <iostream>
#include <sstream>
#include <vector>

int main(int argc, char *argv[])
{
  try {
    // set up program options
    std::string outdir;
    boost::program_options::options_description opts("Options",100); // 100 is "line_length"
    opts.add_options()
      ("help,h","prints help message")
      ("outdir,o",
       boost::program_options::value<std::string>(&outdir)->default_value(std::string(".")),
       "specify the output directory")
      ("infile,i",
       boost::program_options::value< std::vector<std::string> >(),
       "path to a csv file containing Data Dictionary information")
    ;
    boost::program_options::positional_options_description posOpts;
    posOpts.add("infile",-1);

    // parse command line
    boost::program_options::variables_map vm;
    boost::program_options::store(boost::program_options::command_line_parser(argc,argv).
        options(opts).positional(posOpts).run(),vm);
    boost::program_options::notify(vm);

    // provide help message
    if (vm.count("help")) {
      std::cout << opts;
      std::cout << "In addition to an outdir, at least one path to a file containing Data " 
                << "Dictionary information is required." << std::endl;
      return 0;
    }

    // get valid output directory, if possible (otherwise throws)
    openstudio::path outPath = openstudio::getApplicationOutputDirectory(outdir);

    // data common to the start of each file
    std::stringstream fileHeader;
    fileHeader 
      << "/**********************************************************************" << std::endl
      << "*  Copyright (c) 2008-2013, Alliance for Sustainable Energy." << std::endl
      << "*  All rights reserved." << std::endl
      << "*" << std::endl
      << "*  This library is free software; you can redistribute it and/or" << std::endl
      << "*  modify it under the terms of the GNU Lesser General Public" << std::endl
      << "*  License as published by the Free Software Foundation; either" << std::endl
      << "*  version 2.1 of the License, or (at your option) any later version." << std::endl
      << "*" << std::endl
      << "*  This library is distributed in the hope that it will be useful," << std::endl
      << "*  but WITHOUT ANY WARRANTY; without even the implied warranty of" << std::endl
      << "*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU" << std::endl
      << "*  Lesser General Public License for more details." << std::endl
      << "*" << std::endl
      << "*  You should have received a copy of the GNU Lesser General Public" << std::endl
      << "*  License along with this library; if not, write to the Free Software" << std::endl
      << "*  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA" << std::endl
      << "**********************************************************************/" << std::endl;

    // create and initialize output files
    openstudio::standardsinterface::GenerateDataDictionaryOutputFiles outputFiles;
    openstudio::standardsinterface::initializeOutputFiles(outPath,fileHeader.str(),outputFiles);

    // parse input files
    std::vector<std::string> inputFilePaths = vm["infile"].as< std::vector<std::string> >();
    openstudio::standardsinterface::GenerateDataDictionaryHistory history;
    BOOST_FOREACH(const std::string inputFilePath,inputFilePaths) {
      openstudio::standardsinterface::parseInputFile(inputFilePath,outputFiles,history);
    }

    // conclude and close output files
    openstudio::standardsinterface::completeOutputFiles(outputFiles,history);

  }
  catch (std::runtime_error& e) {
    std::cout << e.what() << std::endl;
    return 1;
  }
  return 0;
}
