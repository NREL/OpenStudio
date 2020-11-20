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
