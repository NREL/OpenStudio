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

#include "OSVersionFixture.hpp"

#include "../../model/Model.hpp"
#include "../../model/Component.hpp"
#include "../../model/Construction.hpp"
#include "../../model/Construction_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>
#include "../../utilities/core/Compare.hpp"

#include <resources.hxx>
#include <OpenStudio.hxx>

using namespace openstudio;

void OSVersionFixture::SetUp() {}

void OSVersionFixture::TearDown() {}

void OSVersionFixture::SetUpTestCase() {
  // set up logging
  logFile = FileLogSink(toPath("./OSVersionFixture.log"));
  logFile->setLogLevel(Debug);
  Logger::instance().standardOutLogger().disable();

  // create and popluate resource folder
  openstudio::path thisVersionPath = versionResourcesPath(openstudio::VersionString(openStudioVersion()));
  if (!boost::filesystem::exists(thisVersionPath)) {
    boost::filesystem::create_directory(thisVersionPath);
  }

  // ETH@20120514 Eventually this cpp file should be configured by CMake so OpenStudio.idd can be 
  // directly copied over, and so all these items can be copied back into source.
  model::Model model = openstudio::model::exampleModel();
  model.save(thisVersionPath / toPath("example.osm"),true);
  model::Component component = model.getConcreteModelObjects<model::Construction>()[0].createComponent();
  component.save(thisVersionPath / toPath("example.osc"),true);
  IddFile iddFile = IddFactory::instance().getIddFile(IddFileType::OpenStudio);
  iddFile.save(thisVersionPath / toPath("OpenStudio.idd"),true);
}

void OSVersionFixture::TearDownTestCase() {
  logFile->disable();
}

openstudio::path OSVersionFixture::versionResourcesPath(const openstudio::VersionString& version)
{
  return resourcesPath() / toPath("osversion") / 
         toPath(boost::regex_replace(version.str(),boost::regex("\\."),"_"));
}

openstudio::path OSVersionFixture::exampleModelPath(const openstudio::VersionString& version)
{
  return versionResourcesPath(version) / toPath("example.osm");
}

openstudio::path OSVersionFixture::exampleComponentPath(const openstudio::VersionString& version)
{
  return versionResourcesPath(version) / toPath("example.osc");
}

openstudio::path OSVersionFixture::iddPath(const openstudio::VersionString& version) {
  return versionResourcesPath(version) / toPath("OpenStudio.idd");
}

// static variables
boost::optional<openstudio::FileLogSink> OSVersionFixture::logFile;

