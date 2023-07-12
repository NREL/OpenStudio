/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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
