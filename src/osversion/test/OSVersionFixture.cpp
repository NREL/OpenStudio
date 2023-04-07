/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

void OSVersionFixture::SetUpTestSuite() {
  // set up logging
  logFile = FileLogSink(toPath("./OSVersionFixture.log"));
  logFile->setLogLevel(Debug);
  Logger::instance().standardOutLogger().disable();

  // create and popluate resource folder
  openstudio::path thisVersionPath = versionResourcesPath(openstudio::VersionString(openStudioVersion()));
  if (!openstudio::filesystem::exists(thisVersionPath)) {
    openstudio::filesystem::create_directories(thisVersionPath);
  }

  // ETH@20120514 Eventually this cpp file should be configured by CMake so OpenStudio.idd can be
  // directly copied over, and so all these items can be copied back into source.
  model::Model model = openstudio::model::exampleModel();
  model.save(thisVersionPath / toPath("example.osm"), true);
  model::Component component = model.getConcreteModelObjects<model::Construction>()[0].createComponent();
  component.save(thisVersionPath / toPath("example.osc"), true);
  IddFile iddFile = IddFactory::instance().getIddFile(IddFileType::OpenStudio);
  iddFile.save(thisVersionPath / toPath("OpenStudio.idd"), true);
}

void OSVersionFixture::TearDownTestSuite() {
  logFile->disable();
}

openstudio::path OSVersionFixture::versionResourcesPath(const openstudio::VersionString& version) {
  return resourcesPath() / toPath("osversion") / toPath(boost::regex_replace(version.str(), boost::regex("\\."), "_"));
}

openstudio::path OSVersionFixture::exampleModelPath(const openstudio::VersionString& version) {
  return versionResourcesPath(version) / toPath("example.osm");
}

openstudio::path OSVersionFixture::exampleComponentPath(const openstudio::VersionString& version) {
  return versionResourcesPath(version) / toPath("example.osc");
}

openstudio::path OSVersionFixture::iddPath(const openstudio::VersionString& version) {
  return versionResourcesPath(version) / toPath("OpenStudio.idd");
}

openstudio::IddFile OSVersionFixture::getOpenStudioIddFileForVersion(const VersionString& version) {
  OptionalIddFile iddFile = IddFactory::instance().getIddFile(IddFileType::OpenStudio, version);
  if (!iddFile) {
    LOG_AND_THROW("Unable to retrieve OpenStudio Version " << version.str() << " IDD from the IddFactory.");
  }
  return iddFile.get();
}

// static variables
boost::optional<openstudio::FileLogSink> OSVersionFixture::logFile;
