/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#include "GenerateIddFactory.hpp"

#include "GeneratorApplicationPathHelpers.hpp"

#include <boost/program_options.hpp>

#include <iostream>
#include <exception>

int main(int argc, char *argv[])
{
  try {
    // set up program options
    std::string outdir;
    boost::program_options::options_description opts("Options",100);
    opts.add_options()
      ("help,h","prints help message")
      ("outdir,o",
       boost::program_options::value<std::string>(&outdir)->default_value(std::string(".")),
       "specify the output directory")
      ("idd,i",
       boost::program_options::value< std::vector<std::string> >(),
       "name,path of an IDD file; ex. EnergyPlus,C:\\EnergyPlus\\Energy+.idd; if no name provided, will use file name")
    ;
    boost::program_options::positional_options_description posOpts;
    posOpts.add("idd",-1);

    // parse command line
    boost::program_options::variables_map vm;
    boost::program_options::store(boost::program_options::command_line_parser(argc,argv).
        options(opts).positional(posOpts).run(),vm);
    boost::program_options::notify(vm);

    // provide help message
    if (vm.count("help")) {
      std::cout << opts;
      std::cout << "In addition to an outdir, at least one name,path IDD pair is required." << std::endl;
      return 0;
    }

    // get valid output directory, if possible (otherwise throws)
    openstudio::filesystem::path outPath = openstudio::getApplicationOutputDirectory(outdir);

    // get the idd file information, if possible (otherwise throws)
    openstudio::IddFileFactoryDataVector iddFiles = 
        openstudio::constructIddFileObjects(vm["idd"].as< std::vector<std::string> >());

    // data common to the start of each file
    std::stringstream fileHeader;
    fileHeader 
      << "/***********************************************************************************************************************" << std::endl
      << " *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved." << std::endl
      << " *" << std::endl
      << " *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the" << std::endl
      << " *  following conditions are met:" << std::endl
      << " *" << std::endl
      << " *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following" << std::endl
      << " *  disclaimer." << std::endl
      << " *" << std::endl
      << " *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the" << std::endl
      << " *  following disclaimer in the documentation and/or other materials provided with the distribution." << std::endl
      << " *" << std::endl
      << " *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote" << std::endl
      << " *  products derived from this software without specific prior written permission from the respective party." << std::endl
      << " *" << std::endl
      << " *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative" << std::endl
      << " *  works may not use the \"OpenStudio\" trademark, \"OS\", \"os\", or any other confusingly similar designation without" << std::endl
      << " *  specific prior written permission from Alliance for Sustainable Energy, LLC." << std::endl
      << " *" << std::endl
      << " *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS \"AS IS\" AND ANY EXPRESS OR IMPLIED WARRANTIES," << std::endl
      << " *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE" << std::endl
      << " *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR" << std::endl
      << " *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO," << std::endl
      << " *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED" << std::endl
      << " *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)" << std::endl
      << " *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE." << std::endl
      << " **********************************************************************************************************************/" << std::endl;

    // create and initialize output files
    openstudio::GenerateIddFactoryOutFiles outFiles(outPath,fileHeader.str(),iddFiles);
    openstudio::initializeOutFiles(outFiles,iddFiles);

    // parse input files and write pieces of output files that require object text
    int i(0);
    for (openstudio::IddFileFactoryData& idd : iddFiles) {
      idd.parseFile(outPath,fileHeader.str(),outFiles,i);
      ++i;
    }

    // complete writing and close output files
    openstudio::completeOutFiles(iddFiles,outFiles);

  }
  catch (std::runtime_error& e) {
    std::cout << e.what() << std::endl;
    return 1;
  }
  return 0;
}
