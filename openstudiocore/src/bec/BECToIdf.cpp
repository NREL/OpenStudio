/**********************************************************************
 *  Copyright (c) 2008-2015, Alliance for Sustainable Energy.
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

#include "ReverseTranslator.hpp"
#include "ForwardTranslator.hpp"

#include "../openstudio_lib/FileOperations.hpp"
#include "../model/Model.hpp"
#include "../energyplus/ForwardTranslator.hpp"
#include "../osversion/VersionTranslator.hpp"
#include "../utilities/idf/Workspace.hpp"
#include "../utilities/idf/IdfFile.hpp"
#include "../utilities/core/CommandLine.hpp"
#include "../utilities/core/Path.hpp"

#include <string>
#include <iostream>
#include <QApplication>

int BECToIdf(int argc, char *argv[]){
    std::string inputPathString;

    boost::program_options::options_description desc("Allowed options");
    desc.add_options()
        ("help", "print help message")
        ("inputPath", boost::program_options::value<std::string>(&inputPathString), "path to BEC file")
    ;

    boost::program_options::variables_map vm;
    boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
    boost::program_options::notify(vm);

    if (vm.count("help")) {
      std::cout << "Usage: BECToIdf --inputPath=./path/to/BEC.xml" << std::endl;
      std::cout << desc << std::endl;
      return 1;
    }

    if (vm.count("inputPath")) {

      openstudio::path inputPath = openstudio::toPath(inputPathString);

      openstudio::bec::ReverseTranslator reverseTranslator;
      boost::optional<openstudio::model::Model> model = reverseTranslator.loadModel(inputPath);
      if(model){
        openstudio::path osmPath = inputPath.replace_extension(openstudio::toPath("osm").string());
        model->save(osmPath, true);

        openstudio::path idfPath = inputPath.replace_extension(openstudio::toPath("idf").string());
        openstudio::energyplus::ForwardTranslator forwardTranslator;
        openstudio::Workspace workspace =  forwardTranslator.translateModel(*model);
        workspace.toIdfFile().save(idfPath, true);

        // resave as BEC, not part of production code
        openstudio::path becPath = inputPath.replace_extension(openstudio::toPath(".out.xml").string());
        openstudio::bec::ForwardTranslator becForwardTranslator;
        becForwardTranslator.modelTobec(*model, becPath);
        return 0;

      }else{
        std::cout << "Could not convert file at '" << inputPathString << "' to an OpenStudio Model" << std::endl;
        return 1;
      }

    } else {
      std::cout << "No input path given." << desc << std::endl;
      std::cout << "Usage: BECToIdf --inputPath=./path/to/BEC.xml" << std::endl;
      return 1;
    }
}

int OSMToBEC(int argc, char *argv[]){
    std::string inputPathString;

    boost::program_options::options_description desc("Allowed options");
    desc.add_options()
            ("help", "print help message")
            ("inputPath", boost::program_options::value<std::string>(&inputPathString), "path to OSM file")
            ;

    boost::program_options::variables_map vm;
    boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
    boost::program_options::notify(vm);

    if (vm.count("help")) {
        std::cout << "Usage: BECToIdf --inputPath=./path/to/project.osm" << std::endl;
        std::cout << desc << std::endl;
        return 1;
    }

    if (vm.count("inputPath")) {

        openstudio::path inputPath = openstudio::toPath(inputPathString);

        openstudio::osversion::VersionTranslator versionTranslator;
        boost::optional<openstudio::model::Model> model = openstudio::modelFromOSM(inputPath, versionTranslator);
        if( model ){
            openstudio::path becPath = inputPath.replace_extension(openstudio::toPath(".100.bec.xml").string());
            openstudio::bec::ForwardTranslator becForwardTranslator;
            becForwardTranslator.modelTobec(*model, becPath);
			std::cout << "OUTPUT:" << becPath.string() << std::endl;
            return 0;
        }else{
			std::cout << "Failed to load model" << std::endl;
            return 1;
        }
    }
	else{
		std::cout << "NO INPUT PATH." << std::endl;
	}
    return 1;
}

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
    return OSMToBEC(argc, argv);
}