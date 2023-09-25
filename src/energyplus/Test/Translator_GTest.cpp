/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ErrorFile.hpp"
#include "../ForwardTranslator.hpp"
#include "../ReverseTranslator.hpp"

#include "../../model/Model.hpp"

#include "../../utilities/core/Optional.hpp"
#include "../../utilities/core/Checksum.hpp"
#include "../../utilities/core/Logger.hpp"
#include "../../utilities/sql/SqlFile.hpp"
#include "../../utilities/idd/IddEnums.hpp"
#include "../../utilities/idd/IddFile.hpp"
#include "../../utilities/idf/IdfFile.hpp"

#include <resources.hxx>

#include <sstream>

using openstudio::model::OptionalModel;
using openstudio::energyplus::ForwardTranslator;
using openstudio::energyplus::ReverseTranslator;
using openstudio::energyplus::ErrorFile;
using openstudio::IdfFile;
using openstudio::OptionalIdfFile;
using openstudio::Workspace;
using openstudio::OptionalWorkspace;
using openstudio::SqlFile;
using openstudio::OptionalInt;
using openstudio::IddFileType;

TEST_F(EnergyPlusFixture, IddFile) {
  boost::optional<openstudio::IddFile> iddFile = openstudio::IddFile::load(resourcesPath() / openstudio::toPath("energyplus/ProposedEnergy+.idd"));
  ASSERT_TRUE(iddFile);
}

/*
TEST_F(EnergyPlusFixture,Translator_RoundTrip)
{
  // this is typical operation of translator

  openstudio::path epDir = resourcesPath() / openstudio::toPath("energyplus/5ZoneAirCooled/");
  openstudio::path idfPath = epDir / openstudio::toPath("in.idf");
  openstudio::path sqlPath = epDir / openstudio::toPath("eplusout.sql");
  openstudio::path errorPath = epDir / openstudio::toPath("eplusout.err");

  OptionalIdfFile idfFile = IdfFile::load(idfPath, IddFileType::EnergyPlus);
  ASSERT_TRUE(idfFile);
  Workspace inWorkspace(*idfFile);
  SqlFile sqlFile(sqlPath);
  ErrorFile errorFile(errorPath);

  ReverseTranslator reverseTranslator(inWorkspace);
  OptionalModel model = reverseTranslator.convert();
  ASSERT_TRUE(model);

  model->setSqlFile(sqlFile);

  ForwardTranslator forwardTranslator(*model);
  OptionalWorkspace outWorkspace = forwardTranslator.convert();
  ASSERT_TRUE(outWorkspace);
}
*/
