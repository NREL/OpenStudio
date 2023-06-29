/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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

int main(int argc, char* argv[]) {
  std::string inputPathString;

  boost::program_options::options_description desc("Allowed options");
  desc.add_options()("help", "print help message")("inputPath", boost::program_options::value<std::string>(&inputPathString), "path to gbXML file");

  boost::program_options::variables_map vm;
  boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
  boost::program_options::notify(vm);

  if (vm.count("help")) {
    std::cout << "Usage: gbXMLToIdf --inputPath=./path/to/gbXML.xml" << '\n';
    std::cout << desc << std::endl;  // endl will flush
    return 1;
  }

  if (vm.count("inputPath")) {

    openstudio::path inputPath = openstudio::toPath(inputPathString);

    openstudio::gbxml::ReverseTranslator reverseTranslator;
    boost::optional<openstudio::model::Model> model = reverseTranslator.loadModel(inputPath);
    if (model) {
      openstudio::path osmPath = inputPath.replace_extension(openstudio::toPath("osm").string());
      model->save(osmPath, true);

      openstudio::path idfPath = inputPath.replace_extension(openstudio::toPath("idf").string());
      openstudio::energyplus::ForwardTranslator forwardTranslator;
      openstudio::Workspace workspace = forwardTranslator.translateModel(*model);
      workspace.toIdfFile().save(idfPath, true);

      // resave as gbXML, not part of production code
      openstudio::path gbxmlPath = inputPath.replace_extension(openstudio::toPath(".out.xml").string());
      openstudio::gbxml::ForwardTranslator gbxmlForwardTranslator;
      gbxmlForwardTranslator.modelToGbXML(*model, gbxmlPath);
      return 0;

    } else {
      std::cout << "Could not convert file at '" << inputPathString << "' to an OpenStudio Model" << std::endl;  // endl will flush
      return 1;
    }

  } else {
    std::cout << "No input path given." << desc << '\n';
    std::cout << "Usage: gbXMLToIdf --inputPath=./path/to/gbXML.xml" << std::endl;  // endl will flush
    return 1;
  }
}
