/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#include "ReverseTranslator.hpp"
#include "ForwardTranslator.hpp"
#include "../model/Model.hpp"
#include "../energyplus/ForwardTranslator.hpp"

#include "../utilities/idf/Workspace.hpp"
#include "../utilities/idf/IdfFile.hpp"
#include "../utilities/core/CommandLine.hpp"
#include "../utilities/core/Path.hpp"

#include <string>
#include <iostream>


int main(int argc, char *argv[])
{
  std::string inputPathString;

  boost::program_options::options_description desc("Allowed options");
  desc.add_options()
      ("help", "print help message")
      ("inputPath", boost::program_options::value<std::string>(&inputPathString), "path to gbXML file")
  ;

  boost::program_options::variables_map vm;
  boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
  boost::program_options::notify(vm);

  if (vm.count("help")) {
    std::cout << "Usage: gbXMLToIdf --inputPath=./path/to/gbXML.xml" << std::endl;
    std::cout << desc << std::endl;
    return 1;
  }

  if (vm.count("inputPath")) {

    openstudio::path inputPath = openstudio::toPath(inputPathString);

    openstudio::gbxml::ReverseTranslator reverseTranslator;
    boost::optional<openstudio::model::Model> model = reverseTranslator.loadModel(inputPath);
    if(model){
      openstudio::path osmPath = inputPath.replace_extension(openstudio::toPath("osm").string());
      model->save(osmPath, true);

      openstudio::path idfPath = inputPath.replace_extension(openstudio::toPath("idf").string());
      openstudio::energyplus::ForwardTranslator forwardTranslator;
      openstudio::Workspace workspace =  forwardTranslator.translateModel(*model);
      workspace.toIdfFile().save(idfPath, true);

      // resave as gbXML, not part of production code
      openstudio::path gbxmlPath = inputPath.replace_extension(openstudio::toPath(".out.xml").string());
      openstudio::gbxml::ForwardTranslator gbxmlForwardTranslator;
      gbxmlForwardTranslator.modelToGbXML(*model, gbxmlPath);
      return 0;

    }else{
      std::cout << "Could not convert file at '" << inputPathString << "' to an OpenStudio Model" << std::endl;
      return 1;
    }

  } else {
    std::cout << "No input path given." << desc << std::endl;
    std::cout << "Usage: gbXMLToIdf --inputPath=./path/to/gbXML.xml" << std::endl;
    return 1;
  }
}



