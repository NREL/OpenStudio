/**********************************************************************
*  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
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

#include "EnergyPlusFixture.hpp"

#include <boost/filesystem.hpp>

#include <resources.hxx>

using openstudio::FileLogSink;
using openstudio::toPath;

void EnergyPlusFixture::SetUp() {}

void EnergyPlusFixture::TearDown() {}

void EnergyPlusFixture::SetUpTestCase() {
  // set up logging
  logFile = FileLogSink(toPath("./EnergyPlusFixture.log"));
  logFile->setLogLevel(Trace);
  openstudio::Logger::instance().standardOutLogger().disable();

  // initialize component paths
  openstudio::path basePath = resourcesPath()/openstudio::toPath("energyplus/Components/");
  // idfComponents consists of .first = path to directory, .second = component type
  idfComponents.push_back(std::pair<openstudio::path,std::string>(basePath/openstudio::toPath("idf_roof_1"),"roof"));
  idfComponents.push_back(std::pair<openstudio::path,std::string>(basePath/openstudio::toPath("idf_roof_2"),"roof"));
  idfComponents.push_back(std::pair<openstudio::path,std::string>(basePath/openstudio::toPath("idf_roof_3"),"roof"));
  idfComponents.push_back(std::pair<openstudio::path,std::string>(basePath/openstudio::toPath("idf_roof_4"),"roof"));
  idfComponents.push_back(std::pair<openstudio::path,std::string>(basePath/openstudio::toPath("idf_roof_5"),"roof"));
  idfComponents.push_back(std::pair<openstudio::path,std::string>(basePath/openstudio::toPath("idf_designday_1"),"designday"));
  idfComponents.push_back(std::pair<openstudio::path,std::string>(basePath/openstudio::toPath("idf_designday_2"),"designday"));
  idfComponents.push_back(std::pair<openstudio::path,std::string>(basePath/openstudio::toPath("idf_designday_3"),"designday"));
  idfComponents.push_back(std::pair<openstudio::path,std::string>(basePath/openstudio::toPath("idf_designday_4"),"designday"));
  idfComponents.push_back(std::pair<openstudio::path,std::string>(basePath/openstudio::toPath("idf_designday_5"),"designday"));

  // delete translated components
  for (const ComponentDirectoryAndType& idfComponent : idfComponents) {
    // delete any *.osc and oscomponent.xml files in the directory
    for (boost::filesystem::directory_iterator it(idfComponent.first), itEnd; it != itEnd; ++it) {
      if (boost::filesystem::is_regular_file(it->status())) {
        std::string ext = openstudio::toString(boost::filesystem::extension(*it));
        if (ext == ".osc") { boost::filesystem::remove(it->path()); }
        if ((ext == ".xml") && (openstudio::toString(it->path().filename()) == "oscomponent")) { 
          boost::filesystem::remove(it->path()); 
        }
      }
    } // for iterator over directory
  } // foreach component

}

void EnergyPlusFixture::TearDownTestCase() {
  logFile->disable();
}

// static variables
boost::optional<openstudio::FileLogSink> EnergyPlusFixture::logFile;
std::vector< std::pair< openstudio::path, std::string> > EnergyPlusFixture::idfComponents;

