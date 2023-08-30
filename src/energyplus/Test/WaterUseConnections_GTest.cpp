/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"
#include "../ReverseTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/WaterUseConnections.hpp"
#include "../../model/WaterUseConnections_Impl.hpp"
#include "../../model/WaterUseEquipmentDefinition.hpp"
#include "../../model/WaterUseEquipmentDefinition_Impl.hpp"
#include "../../model/WaterUseEquipment.hpp"
#include "../../model/WaterUseEquipment_Impl.hpp"
#include "../../model/PlantLoop.hpp"
#include "../../model/PlantLoop_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"

#include "../../utilities/idf/IdfFile.hpp"
#include "../../utilities/idf/Workspace.hpp"
#include "../../utilities/idf/IdfObject.hpp"
#include "../../utilities/idf/WorkspaceObject.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"

#include <utilities/idd/WaterUse_Connections_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_WaterUseConnections) {
  Model m;

  WaterUseConnections waterUseConnections(m);
  waterUseConnections.setDrainWaterHeatExchangerType("CrossFlow");
  waterUseConnections.setDrainWaterHeatExchangerDestination("Equipment");
  waterUseConnections.setDrainWaterHeatExchangerUFactorTimesArea(0.75);
  WaterUseEquipmentDefinition waterUseEquipmentDefinition(m);
  WaterUseEquipment waterUseEquipment1(waterUseEquipmentDefinition);
  WaterUseEquipment waterUseEquipment2(waterUseEquipmentDefinition);
  waterUseConnections.addWaterUseEquipment(waterUseEquipment1);
  waterUseConnections.addWaterUseEquipment(waterUseEquipment2);

  PlantLoop plantLoop(m);
  Node demandOutletNode = plantLoop.demandOutletNode();
  EXPECT_TRUE(waterUseConnections.addToNode(demandOutletNode));

  ForwardTranslator ft;
  Workspace w = ft.translateModel(m);

  ASSERT_EQ(2u, w.getObjectsByType(IddObjectType::WaterUse_Equipment).size());

  std::vector<WorkspaceObject> woWaterUseConnections(w.getObjectsByType(IddObjectType::WaterUse_Connections));
  ASSERT_EQ(1u, woWaterUseConnections.size());
  WorkspaceObject wo(woWaterUseConnections.at(0));

  EXPECT_NE("", wo.getString(WaterUse_ConnectionsFields::InletNodeName, false).get());
  EXPECT_NE("", wo.getString(WaterUse_ConnectionsFields::OutletNodeName, false).get());
  EXPECT_EQ("", wo.getString(WaterUse_ConnectionsFields::HotWaterSupplyTemperatureScheduleName, false).get());
  EXPECT_EQ("", wo.getString(WaterUse_ConnectionsFields::ColdWaterSupplyTemperatureScheduleName, false).get());
  EXPECT_EQ("CrossFlow", wo.getString(WaterUse_ConnectionsFields::DrainWaterHeatExchangerType, false).get());
  EXPECT_EQ("Equipment", wo.getString(WaterUse_ConnectionsFields::DrainWaterHeatExchangerDestination, false).get());
  EXPECT_EQ(0.75, wo.getDouble(WaterUse_ConnectionsFields::DrainWaterHeatExchangerUFactorTimesArea, false).get());

  std::vector<IdfExtensibleGroup> extensibleGroups = wo.extensibleGroups();
  ASSERT_EQ(2u, extensibleGroups.size());
}
