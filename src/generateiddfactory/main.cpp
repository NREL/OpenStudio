/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "GenerateIddFactory.hpp"

#include "GeneratorApplicationPathHelpers.hpp"

#include <boost/program_options.hpp>

#include <iostream>
#include <exception>

int main(int argc, char* argv[]) {
  try {
    // set up program options
    std::string outdir;
    boost::program_options::options_description opts("Options", 100);
    opts.add_options()("help,h",
                       "prints help message")("outdir,o", boost::program_options::value<std::string>(&outdir)->default_value(std::string(".")),
                                              "specify the output directory")("idd,i", boost::program_options::value<std::vector<std::string>>(),
                                                                              "name,path of an IDD file; ex. EnergyPlus,C:\\EnergyPlus\\Energy+.idd; "
                                                                              "if no name provided, will use file name");
    boost::program_options::positional_options_description posOpts;
    posOpts.add("idd", -1);

    // parse command line
    boost::program_options::variables_map vm;
    boost::program_options::store(boost::program_options::command_line_parser(argc, argv).options(opts).positional(posOpts).run(), vm);
    boost::program_options::notify(vm);

    // provide help message
    if (vm.count("help") != 0u) {
      std::cout << opts;
      std::cout << "In addition to an outdir, at least one name,path IDD pair is required." << '\n';
      return 0;
    }

    // get valid output directory, if possible (otherwise throws)
    openstudio::filesystem::path outPath = openstudio::getApplicationOutputDirectory(outdir);

    // get the idd file information, if possible (otherwise throws)
    openstudio::IddFileFactoryDataVector iddFiles = openstudio::constructIddFileObjects(vm["idd"].as<std::vector<std::string>>());

    // data common to the start of each file
    std::stringstream fileHeader;
    fileHeader << "/***********************************************************************************************************************" << '\n'
               << "*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC." << '\n'
               << "*  See also https://openstudio.net/license" << '\n'
               << "***********************************************************************************************************************/" << '\n';

    // create and initialize output files
    openstudio::GenerateIddFactoryOutFiles outFiles(outPath, fileHeader.str(), iddFiles);
    openstudio::initializeOutFiles(outFiles, iddFiles);

    // parse input files and write pieces of output files that require object text
    int i(0);
    for (openstudio::IddFileFactoryData& idd : iddFiles) {
      idd.parseFile(outPath, fileHeader.str(), outFiles, i);
      ++i;
    }

    // complete writing and close output files
    openstudio::completeOutFiles(iddFiles, outFiles);

  } catch (std::runtime_error& e) {
    std::cout << e.what() << '\n';
    return 1;
  }
  return 0;
}
