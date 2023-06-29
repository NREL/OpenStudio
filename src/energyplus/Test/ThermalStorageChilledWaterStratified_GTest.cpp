/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"

#include "../../model/ThermalStorageChilledWaterStratified.hpp"
#include "../../model/Model.hpp"
#include "../../model/PlantLoop.hpp"

#include <utilities/idd/ThermalStorage_ChilledWater_Stratified_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

/* Gtest for https://github.com/NREL/OpenStudio/issues/2373 */
TEST_F(EnergyPlusFixture, ForwardTranslator_ThermalStorageChilledWaterStratified_NumNodesAndTankTime) {
  Model model;

  ThermalStorageChilledWaterStratified t(model);

  ASSERT_TRUE(t.setTankRecoveryTime(4.25));
  ASSERT_TRUE(t.setNumberofNodes(7));

  PlantLoop p(model);
  ASSERT_TRUE(p.addSupplyBranchForComponent(t));

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);

  WorkspaceObjectVector idfObjs(workspace.getObjectsByType(IddObjectType::ThermalStorage_ChilledWater_Stratified));
  EXPECT_EQ(1u, idfObjs.size());
  WorkspaceObject idf_t(idfObjs[0]);

  EXPECT_DOUBLE_EQ(4.25, *idf_t.getDouble(ThermalStorage_ChilledWater_StratifiedFields::TankRecoveryTime));
  EXPECT_EQ(7, *idf_t.getInt(ThermalStorage_ChilledWater_StratifiedFields::NumberofNodes));
}
