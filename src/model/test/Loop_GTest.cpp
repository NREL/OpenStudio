/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "ModelFixture.hpp"

#include "../AirLoopHVAC.hpp"
#include "../AirLoopHVAC_Impl.hpp"
#include "../ControllerOutdoorAir.hpp"
#include "../AirLoopHVACOutdoorAirSystem.hpp"
#include "../AirLoopHVACSupplyPlenum.hpp"
#include "../AirLoopHVACReturnPlenum.hpp"
#include "../HVACTemplates.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"

#include "../AirLoopHVACUnitarySystem.hpp"

#include "../AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass.hpp"
#include "../Schedule.hpp"
#include "../FanConstantVolume.hpp"
#include "../CoilHeatingElectric.hpp"
#include "../CoilCoolingDXSingleSpeed.hpp"

using namespace openstudio::model;

// Test for https://github.com/NREL/OpenStudio/issues/3024
TEST_F(ModelFixture, Loop_CrashDeleteUnitaryAirLoopHVAC) {
  Model model = Model();

  AirLoopHVAC airLoopHVAC(model);

  Node supplyInletNode = airLoopHVAC.supplyInletNode();
  Node supplyOutletNode = airLoopHVAC.supplyOutletNode();

  // Add an OASys
  ControllerOutdoorAir controllerOutdoorAir(model);
  AirLoopHVACOutdoorAirSystem outdoorAirSystem(model, controllerOutdoorAir);
  outdoorAirSystem.addToNode(supplyInletNode);
  std::vector<ModelObject> inletComponents = airLoopHVAC.supplyComponents(supplyInletNode, supplyOutletNode);
  EXPECT_EQ(3, inletComponents.size());

  // Add a unitary to the mixed air node
  AirLoopHVACUnitarySystem unitary(model);
  Node mixedAirNode = airLoopHVAC.mixedAirNode().get();
  unitary.addToNode(mixedAirNode);
  inletComponents = airLoopHVAC.supplyComponents(supplyInletNode, supplyOutletNode);
  // This added the unitary + the mixed air node, we expect 5:
  // SupplyInletNode, OA Sys, MixedNode, Unitary, SupplyOutletNode
  EXPECT_EQ(5, inletComponents.size());

  // Remove the unitary, we should end up in the same point as before
  unitary.remove();
  inletComponents = airLoopHVAC.supplyComponents(supplyInletNode, supplyOutletNode);
  EXPECT_EQ(3, inletComponents.size());
}

TEST_F(ModelFixture, Loop_DeleteUnitaryVAVChangeoverBypassAirLoopHVAC) {
  // Note: this already passes, but this inherits StraightComponent and not ZoneHVAComponent
  Model model = Model();

  AirLoopHVAC airLoopHVAC(model);

  Node supplyInletNode = airLoopHVAC.supplyInletNode();
  Node supplyOutletNode = airLoopHVAC.supplyOutletNode();

  // Add an OASys
  ControllerOutdoorAir controllerOutdoorAir(model);
  AirLoopHVACOutdoorAirSystem outdoorAirSystem(model, controllerOutdoorAir);
  outdoorAirSystem.addToNode(supplyInletNode);
  std::vector<ModelObject> inletComponents = airLoopHVAC.supplyComponents(supplyInletNode, supplyOutletNode);
  EXPECT_EQ(3, inletComponents.size());

  // Add a unitary to the mixed air node
  Schedule s = model.alwaysOnDiscreteSchedule();
  FanConstantVolume fan(model, s);
  CoilHeatingElectric heatingCoil(model, s);
  CoilCoolingDXSingleSpeed coolingCoil(model);
  AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass unitary(model, fan, coolingCoil, heatingCoil);

  Node mixedAirNode = airLoopHVAC.mixedAirNode().get();
  unitary.addToNode(mixedAirNode);
  inletComponents = airLoopHVAC.supplyComponents(supplyInletNode, supplyOutletNode);
  // This added the unitary + the mixed air node, we expect 5:
  // SupplyInletNode, OA Sys, MixedNode, Unitary, SupplyOutletNode
  EXPECT_EQ(5, inletComponents.size());

  // Remove the unitary, we should end up in the same point as before
  unitary.remove();
  inletComponents = airLoopHVAC.supplyComponents(supplyInletNode, supplyOutletNode);
  EXPECT_EQ(3, inletComponents.size());
}
