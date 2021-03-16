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
#include "../AirLoopHVACOutdoorAirSystem.hpp"
#include "../AirLoopHVACOutdoorAirSystem_Impl.hpp"
#include "../ControllerOutdoorAir.hpp"
#include "../ControllerOutdoorAir_Impl.hpp"
#include "../HVACComponent.hpp"
#include "../HVACComponent_Impl.hpp"
#include "../CoilHeatingElectric.hpp"
#include "../Schedule.hpp"
#include "../Node.hpp"
#include "../AirLoopHVAC.hpp"
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

TEST_F(ModelFixture, AirLoopHVACOutdoorAirSystem_AirLoopHVACOutdoorAirSystem) {
  Model model = Model();
  OptionalModelObject modelObject;

  ControllerOutdoorAir controller(model);
  AirLoopHVACOutdoorAirSystem oaSystem = AirLoopHVACOutdoorAirSystem(model, controller);

  ASSERT_EQ(openstudio::IddObjectType::OS_AirLoopHVAC_OutdoorAirSystem, oaSystem.iddObjectType().value());
}

TEST_F(ModelFixture, AirLoopHVACOutdoorAirSystem_controllerOutdoorAir) {
  Model model = Model();
  OptionalModelObject modelObject;

  ControllerOutdoorAir controller(model);
  AirLoopHVACOutdoorAirSystem oaSystem = AirLoopHVACOutdoorAirSystem(model, controller);

  ASSERT_EQ(openstudio::IddObjectType::OS_Controller_OutdoorAir, oaSystem.getControllerOutdoorAir().iddObjectType().value());
}

TEST_F(ModelFixture, AirLoopHVACOutdoorAirSystem_clone) {
  Model model = Model();
  Model model2 = Model();
  OptionalModelObject modelObject;

  ControllerOutdoorAir controller(model);
  AirLoopHVACOutdoorAirSystem oaSystem = AirLoopHVACOutdoorAirSystem(model, controller);

  ASSERT_EQ(openstudio::IddObjectType::OS_Controller_OutdoorAir, oaSystem.getControllerOutdoorAir().iddObjectType().value());

  AirLoopHVACOutdoorAirSystem oaSystem2 = oaSystem.clone(model2).cast<AirLoopHVACOutdoorAirSystem>();

  ASSERT_EQ(openstudio::IddObjectType::OS_Controller_OutdoorAir, oaSystem2.getControllerOutdoorAir().iddObjectType().value());
}

//TEST_F(ModelFixture,AirLoopHVACOutdoorAirSystem_edges)
//{
//  Model m;
//  Schedule s = m.alwaysOnDiscreteSchedule();
//  AirLoopHVAC airLoop(m);
//  ControllerOutdoorAir controllerOutdoorAir(m);
//  AirLoopHVACOutdoorAirSystem outdoorAirSystem(m,controllerOutdoorAir);
//  CoilHeatingElectric coil(m, s);
//
//  Node supplyOutletNode = airLoop.supplyOutletNode();
//  outdoorAirSystem.addToNode(supplyOutletNode);
//  coil.addToNode(supplyOutletNode);
//  boost::optional<ModelObject> testObject = airLoop.supplyComponent(outdoorAirSystem.handle());
//  ASSERT_TRUE(testObject);
//  EXPECT_EQ(outdoorAirSystem, *testObject);
//  std::vector<HVACComponent> edges = outdoorAirSystem.getImpl<detail::HVACComponent_Impl>()->edges(false); // should be Node
//  ASSERT_EQ(1, edges.size());
//  edges = edges[0].getImpl<detail::HVACComponent_Impl>()->edges(false); // should be CoilHeatingElectric
//  ASSERT_EQ(1, edges.size());
//  EXPECT_EQ(coil, edges[0]);
//}
