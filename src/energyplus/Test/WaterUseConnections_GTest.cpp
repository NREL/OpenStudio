/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
