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

#include "ModelFixture.hpp"
#include "../Model.hpp"

#include "../ZoneHVACTerminalUnitVariableRefrigerantFlow.hpp"
#include "../CoilCoolingDXVariableRefrigerantFlow.hpp"
#include "../CoilHeatingDXVariableRefrigerantFlow.hpp"
#include "../CoilHeatingElectric.hpp"

#include "../Node.hpp"
#include "../Node_Impl.hpp"
#include "../AirLoopHVAC.hpp"
#include "../PlantLoop.hpp"
#include "../AirLoopHVACOutdoorAirSystem.hpp"
#include "../ControllerOutdoorAir.hpp"
#include "../FanOnOff.hpp"
#include "../HVACComponent.hpp"
#include "../HVACComponent_Impl.hpp"
#include "../AirLoopHVACZoneSplitter.hpp"
#include "../AirLoopHVACZoneSplitter_Impl.hpp"
#include "../Splitter.hpp"
#include "../Splitter_Impl.hpp"
#include "../AirTerminalSingleDuctConstantVolumeNoReheat.hpp"

#include "../ScheduleConstant.hpp"
#include "../Schedule.hpp"
#include "../ThermalZone.hpp"

#include "../../utilities/core/Compare.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, ZoneHVACTerminalUnitVariableRefrigerantFlow_Default_Ctor) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model model;

      ZoneHVACTerminalUnitVariableRefrigerantFlow vrfTerminal(model);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");
}

TEST_F(ModelFixture, ZoneHVACTerminalUnitVariableRefrigerantFlow_Explicit_Ctor) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model model;

      FanOnOff fan(model);
      CoilCoolingDXVariableRefrigerantFlow cc(model);
      CoilHeatingDXVariableRefrigerantFlow hc(model);

      ZoneHVACTerminalUnitVariableRefrigerantFlow vrfTerminal(model, cc, hc, fan);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");
}

TEST_F(ModelFixture, ZoneHVACTerminalUnitVariableRefrigerantFlow_SupplementalHeating) {

  Model m;
  ZoneHVACTerminalUnitVariableRefrigerantFlow vrf(m);

  // test defaults
  EXPECT_FALSE(vrf.supplementalHeatingCoil());
  EXPECT_TRUE(vrf.isMaximumSupplyAirTemperaturefromSupplementalHeaterAutosized());
  EXPECT_FALSE(vrf.maximumSupplyAirTemperaturefromSupplementalHeater());
  EXPECT_EQ(21.0, vrf.maximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation());

  // Supplemental Coil
  CoilHeatingElectric supplemental_hc(m);
  EXPECT_TRUE(vrf.setSupplementalHeatingCoil(supplemental_hc));
  ASSERT_TRUE(vrf.supplementalHeatingCoil());
  EXPECT_EQ(supplemental_hc, vrf.supplementalHeatingCoil().get());

  CoilHeatingDXVariableRefrigerantFlow hc_wrong_type(m);
  EXPECT_FALSE(vrf.setSupplementalHeatingCoil(hc_wrong_type));
  ASSERT_TRUE(vrf.supplementalHeatingCoil());
  EXPECT_EQ(supplemental_hc, vrf.supplementalHeatingCoil().get());

  vrf.resetSupplementalHeatingCoil();
  EXPECT_FALSE(vrf.supplementalHeatingCoil());

  // Max SAT
  vrf.setMaximumSupplyAirTemperaturefromSupplementalHeater(35.0);
  EXPECT_FALSE(vrf.isMaximumSupplyAirTemperaturefromSupplementalHeaterAutosized());
  ASSERT_TRUE(vrf.maximumSupplyAirTemperaturefromSupplementalHeater());
  EXPECT_EQ(35.0, vrf.maximumSupplyAirTemperaturefromSupplementalHeater().get());

  vrf.autosizeMaximumSupplyAirTemperaturefromSupplementalHeater();
  EXPECT_TRUE(vrf.isMaximumSupplyAirTemperaturefromSupplementalHeaterAutosized());
  EXPECT_FALSE(vrf.maximumSupplyAirTemperaturefromSupplementalHeater());

  // Max OATdb for supplemental heater
  EXPECT_TRUE(vrf.setMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation(19.0));
  EXPECT_EQ(19.0, vrf.maximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation());
  EXPECT_FALSE(vrf.setMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation(45.0));  // > max
  EXPECT_EQ(19.0, vrf.maximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation());
}

TEST_F(ModelFixture, ZoneHVACTerminalUnitVariableRefrigerantFlow_addToNode_MainBranch) {
  Model m;
  ZoneHVACTerminalUnitVariableRefrigerantFlow testObject(m);

  AirLoopHVAC airLoop(m);

  Node supplyOutletNode = airLoop.supplyOutletNode();

  EXPECT_TRUE(testObject.addToNode(supplyOutletNode));

  EXPECT_EQ((unsigned)3, airLoop.supplyComponents().size());

  EXPECT_TRUE(testObject.inletPort());
  EXPECT_TRUE(testObject.outletPort());
}

TEST_F(ModelFixture, ZoneHVACTerminalUnitVariableRefrigerantFlow_AddToNodeTwoSameObjects) {
  Model m;
  ZoneHVACTerminalUnitVariableRefrigerantFlow testObject(m);

  AirLoopHVAC airLoop(m);

  Node supplyOutletNode = airLoop.supplyOutletNode();
  testObject.addToNode(supplyOutletNode);
  supplyOutletNode = airLoop.supplyOutletNode();
  EXPECT_FALSE(testObject.addToNode(supplyOutletNode));

  EXPECT_TRUE(testObject.inletPort());
  EXPECT_TRUE(testObject.outletPort());
}

TEST_F(ModelFixture, ZoneHVACTerminalUnitVariableRefrigerantFlow_AddToNodeAirLoopDemandSide) {
  Model m;
  ZoneHVACTerminalUnitVariableRefrigerantFlow testObject(m);

  AirLoopHVAC airLoop(m);

  Node inletNode = airLoop.zoneSplitter().lastOutletModelObject()->cast<Node>();

  EXPECT_FALSE(testObject.addToNode(inletNode));

  EXPECT_EQ((unsigned)5, airLoop.demandComponents().size());
}

TEST_F(ModelFixture, ZoneHVACTerminalUnitVariableRefrigerantFlow_AddToNodePlantLoop) {

  Model m;
  ZoneHVACTerminalUnitVariableRefrigerantFlow testObject(m);

  PlantLoop plantLoop(m);

  EXPECT_EQ((unsigned)5, plantLoop.demandComponents().size());

  Node demandInletNode = plantLoop.demandSplitter().lastOutletModelObject()->cast<Node>();

  EXPECT_FALSE(testObject.addToNode(demandInletNode));
  EXPECT_FALSE(plantLoop.addDemandBranchForComponent(testObject));
  EXPECT_EQ((unsigned)5, plantLoop.demandComponents().size());

  Node supplyInletNode = plantLoop.supplySplitter().lastOutletModelObject()->cast<Node>();

  EXPECT_FALSE(testObject.addToNode(supplyInletNode));
  EXPECT_EQ((unsigned)5, plantLoop.supplyComponents().size());
  EXPECT_FALSE(plantLoop.addSupplyBranchForComponent(testObject));
  EXPECT_EQ((unsigned)5, plantLoop.supplyComponents().size());
  ASSERT_FALSE(testObject.plantLoop());
  // EXPECT_EQ(plantLoop, testObject.plantLoop().get());
}

TEST_F(ModelFixture, ZoneHVACTerminalUnitVariableRefrigerantFlow_AddToNodeOutdoorAirSystem) {
  Model m;

  AirLoopHVAC airLoop(m);
  Node supplyOutletNode = airLoop.supplyOutletNode();

  ControllerOutdoorAir controllerOutdoorAir(m);
  AirLoopHVACOutdoorAirSystem outdoorAirSystem(m, controllerOutdoorAir);
  outdoorAirSystem.addToNode(supplyOutletNode);

  EXPECT_EQ(3, airLoop.supplyComponents().size());
  EXPECT_EQ(1, outdoorAirSystem.oaComponents().size());
  EXPECT_EQ(1, outdoorAirSystem.reliefComponents().size());
  ZoneHVACTerminalUnitVariableRefrigerantFlow testObject1(m);

  if (boost::optional<Node> OANode = outdoorAirSystem.outboardOANode()) {
    EXPECT_TRUE(testObject1.addToNode(OANode.get()));
    EXPECT_EQ(3, airLoop.supplyComponents().size());
    EXPECT_EQ(3, outdoorAirSystem.oaComponents().size());
  }

  ZoneHVACTerminalUnitVariableRefrigerantFlow testObject2(m);

  if (boost::optional<Node> reliefNode = outdoorAirSystem.outboardReliefNode()) {
    EXPECT_TRUE(testObject2.addToNode(reliefNode.get()));
    EXPECT_EQ(3, airLoop.supplyComponents().size());
    EXPECT_EQ(3, outdoorAirSystem.reliefComponents().size());
  }
}

TEST_F(ModelFixture, ZoneHVACTerminalUnitVariableRefrigerantFlow_controllingZoneorThermostatLocation) {

  // Used only for AirloopHVAC equipment on a main branch and defines zone name where thermostat is located.
  // Not required for zone equipment. Leave blank if terminal unit is used in AirLoopHVAC:OutdoorAirSystem:EquipmentList.
  // Required when terminal unit is used on main AirloopHVAC branch and coils are not set point controlled.
  // When terminal unit is used in air loop and is load controlled, this zone's thermostat will control operation.

  Model m;
  ZoneHVACTerminalUnitVariableRefrigerantFlow testObject(m);

  AirLoopHVAC airLoop(m);
  Node supplyOutletNode = airLoop.supplyOutletNode();
  EXPECT_TRUE(testObject.addToNode(supplyOutletNode));

  ThermalZone z(m);
  ScheduleConstant sch(m);
  AirTerminalSingleDuctConstantVolumeNoReheat atu(m, sch);
  EXPECT_TRUE(airLoop.addBranchForZone(z, atu));

  EXPECT_FALSE(testObject.controllingZoneorThermostatLocation());
  EXPECT_TRUE(testObject.setControllingZoneorThermostatLocation(z));
  ASSERT_TRUE(testObject.controllingZoneorThermostatLocation());
  EXPECT_EQ(z, testObject.controllingZoneorThermostatLocation().get());
  testObject.resetControllingZoneorThermostatLocation();
  EXPECT_FALSE(testObject.controllingZoneorThermostatLocation());
}
