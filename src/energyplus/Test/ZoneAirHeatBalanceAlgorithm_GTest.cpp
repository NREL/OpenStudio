/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"
#include "../ReverseTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/ZoneAirHeatBalanceAlgorithm.hpp"
#include "../../model/ZoneAirHeatBalanceAlgorithm_Impl.hpp"

#include "../../utilities/idf/IdfObject.hpp"
#include "../../utilities/idf/WorkspaceObject.hpp"

#include <utilities/idd/ZoneAirHeatBalanceAlgorithm_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include <resources.hxx>

#include <sstream>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_ZoneAirHeatBalanceAlgorithm) {
  Model model;
  ZoneAirHeatBalanceAlgorithm zoneAHBA = model.getUniqueModelObject<ZoneAirHeatBalanceAlgorithm>();

  EXPECT_TRUE(zoneAHBA.setAlgorithm("AnalyticalSolution"));
  EXPECT_TRUE(zoneAHBA.setDoSpaceHeatBalanceforSizing(false));
  EXPECT_TRUE(zoneAHBA.setDoSpaceHeatBalanceforSimulation(true));

  ForwardTranslator ft;
  Workspace workspace = ft.translateModel(model);

  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::ZoneAirHeatBalanceAlgorithm).size());

  IdfObject idf_zoneAHBA = workspace.getObjectsByType(IddObjectType::ZoneAirHeatBalanceAlgorithm)[0];

  EXPECT_EQ("AnalyticalSolution", idf_zoneAHBA.getString(ZoneAirHeatBalanceAlgorithmFields::Algorithm, false).get());
  EXPECT_EQ("No", idf_zoneAHBA.getString(ZoneAirHeatBalanceAlgorithmFields::DoSpaceHeatBalanceforSizing, false).get());
  EXPECT_EQ("Yes", idf_zoneAHBA.getString(ZoneAirHeatBalanceAlgorithmFields::DoSpaceHeatBalanceforSimulation, false).get());
}

TEST_F(EnergyPlusFixture, ReverseTranslator_ZoneAirHeatBalanceAlgorithm) {
  ReverseTranslator rt;

  Workspace w(StrictnessLevel::Minimal, IddFileType::EnergyPlus);

  Model model;
  model = rt.translateWorkspace(w);
  EXPECT_FALSE(model.getOptionalUniqueModelObject<ZoneAirHeatBalanceAlgorithm>());

  OptionalWorkspaceObject idf_zoneAHBA = w.addObject(IdfObject(IddObjectType::ZoneAirHeatBalanceAlgorithm));
  ASSERT_TRUE(idf_zoneAHBA);

  EXPECT_TRUE(idf_zoneAHBA->setString(ZoneAirHeatBalanceAlgorithmFields::Algorithm, "EulerMethod"));
  EXPECT_TRUE(idf_zoneAHBA->setString(ZoneAirHeatBalanceAlgorithmFields::DoSpaceHeatBalanceforSizing, "Yes"));
  EXPECT_TRUE(idf_zoneAHBA->setString(ZoneAirHeatBalanceAlgorithmFields::DoSpaceHeatBalanceforSimulation, "No"));

  model = rt.translateWorkspace(w);
  ZoneAirHeatBalanceAlgorithm zoneAHBA = model.getUniqueModelObject<ZoneAirHeatBalanceAlgorithm>();

  EXPECT_EQ("EulerMethod", zoneAHBA.algorithm());
  EXPECT_TRUE(zoneAHBA.doSpaceHeatBalanceforSizing());
  EXPECT_FALSE(zoneAHBA.doSpaceHeatBalanceforSimulation());
}
