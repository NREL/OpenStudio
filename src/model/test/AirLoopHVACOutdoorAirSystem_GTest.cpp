/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "ModelFixture.hpp"

#include "../AirLoopHVACOutdoorAirSystem.hpp"
#include "../AirLoopHVACOutdoorAirSystem_Impl.hpp"

#include "../AirLoopHVAC.hpp"
#include "../CoilHeatingElectric.hpp"
#include "../CoilHeatingElectric_Impl.hpp"
#include "../CoilCoolingDXVariableSpeed.hpp"
#include "../ControllerOutdoorAir.hpp"
#include "../ControllerOutdoorAir_Impl.hpp"
#include "../HeatExchangerAirToAirSensibleAndLatent.hpp"
#include "../HeatExchangerAirToAirSensibleAndLatent_Impl.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"

#include <utilities/idd/IddEnums.hxx>

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, AirLoopHVACOutdoorAirSystem_GettersSetters) {
  Model m;

  ControllerOutdoorAir controller(m);
  AirLoopHVACOutdoorAirSystem oaSystem(m, controller);
  oaSystem.setName("My AirLoopHVACOutdoorAirSystem");
  EXPECT_EQ(controller, oaSystem.getControllerOutdoorAir());

  ControllerOutdoorAir controller2(m);
  EXPECT_TRUE(oaSystem.setControllerOutdoorAir(controller2));
  EXPECT_EQ(controller2, oaSystem.getControllerOutdoorAir());

  EXPECT_EQ(2, oaSystem.components().size());
  EXPECT_EQ(1, oaSystem.reliefComponents().size());
  ASSERT_TRUE(oaSystem.outboardReliefNode());
  auto outboardReliefNode = oaSystem.outboardReliefNode().get();
  outboardReliefNode.setName("Outboard Relief Node");

  ASSERT_TRUE(oaSystem.reliefAirModelObject());
  EXPECT_EQ(oaSystem.outboardReliefNode().get(), oaSystem.reliefAirModelObject().get());

  EXPECT_EQ(1, oaSystem.oaComponents().size());
  ASSERT_TRUE(oaSystem.outboardOANode());
  auto outboardOANode = oaSystem.outboardOANode().get();
  outboardOANode.setName("Outboard OA Node");

  ASSERT_TRUE(oaSystem.outdoorAirModelObject());
  EXPECT_EQ(oaSystem.outboardOANode().get(), oaSystem.outdoorAirModelObject().get());

  EXPECT_FALSE(oaSystem.returnAirModelObject());
  EXPECT_FALSE(oaSystem.mixedAirModelObject());

  EXPECT_FALSE(oaSystem.airLoopHVAC());

  AirLoopHVAC a(m);
  EXPECT_FALSE(a.mixedAirNode());
  EXPECT_FALSE(a.returnAirNode());
  EXPECT_FALSE(a.outdoorAirNode());
  EXPECT_FALSE(a.reliefAirNode());
  EXPECT_EQ(2, a.supplyComponents().size());
  EXPECT_EQ(0, a.supplyComponents(openstudio::IddObjectType::OS_AirLoopHVAC_OutdoorAirSystem).size());

  auto demandInletNode = a.demandInletNode();
  EXPECT_FALSE(oaSystem.addToNode(demandInletNode));
  EXPECT_FALSE(a.addBranchForHVACComponent(oaSystem));

  auto supplyInletNode = a.supplyInletNode();
  EXPECT_TRUE(oaSystem.addToNode(supplyInletNode));
  ASSERT_TRUE(oaSystem.airLoopHVAC());
  EXPECT_EQ(a, oaSystem.airLoopHVAC().get());

  EXPECT_EQ(3, a.supplyComponents().size());
  EXPECT_EQ(1, a.supplyComponents(openstudio::IddObjectType::OS_AirLoopHVAC_OutdoorAirSystem).size());
  ASSERT_TRUE(a.supplyComponent(oaSystem.handle()));

  ASSERT_TRUE(a.reliefAirNode());
  EXPECT_EQ(2, oaSystem.components().size());
  EXPECT_EQ(1, oaSystem.reliefComponents().size());
  ASSERT_TRUE(oaSystem.outboardReliefNode());
  ASSERT_TRUE(oaSystem.reliefAirModelObject());
  EXPECT_EQ(oaSystem.outboardReliefNode().get(), oaSystem.reliefAirModelObject().get());
  EXPECT_EQ(a.reliefAirNode().get(), oaSystem.reliefAirModelObject().get());

  ASSERT_TRUE(a.outdoorAirNode());
  EXPECT_EQ(1, oaSystem.oaComponents().size());
  ASSERT_TRUE(oaSystem.outboardOANode());
  ASSERT_TRUE(oaSystem.outdoorAirModelObject());
  EXPECT_EQ(oaSystem.outboardOANode().get(), oaSystem.outdoorAirModelObject().get());
  EXPECT_EQ(a.outdoorAirNode().get(), oaSystem.outdoorAirModelObject().get());

  ASSERT_TRUE(a.mixedAirNode());
  a.mixedAirNode().get().setName("Mixed Air Node");
  ASSERT_TRUE(oaSystem.mixedAirModelObject());
  EXPECT_EQ(a.mixedAirNode().get(), oaSystem.mixedAirModelObject().get());

  ASSERT_TRUE(a.returnAirNode());
  a.returnAirNode().get().setName("Return Air Node");
  ASSERT_TRUE(oaSystem.returnAirModelObject());
  EXPECT_EQ(a.returnAirNode().get(), oaSystem.returnAirModelObject().get());

  CoilHeatingElectric oaCoil(m);
  EXPECT_TRUE(oaCoil.addToNode(outboardOANode));

  EXPECT_EQ(4, oaSystem.components().size());
  EXPECT_EQ(1, oaSystem.reliefComponents().size());                    // (Relief)
  EXPECT_EQ(outboardReliefNode, oaSystem.outboardReliefNode().get());  //
  EXPECT_EQ(3, oaSystem.oaComponents().size());                        // (OA)-----Coil------o
  EXPECT_EQ(outboardOANode, oaSystem.outboardOANode().get());

  HeatExchangerAirToAirSensibleAndLatent erv(m);
  EXPECT_TRUE(erv.addToNode(outboardOANode));

  EXPECT_EQ(8, oaSystem.components().size());
  EXPECT_EQ(outboardReliefNode, oaSystem.outboardReliefNode().get());
  EXPECT_EQ(outboardOANode, oaSystem.outboardOANode().get());
  EXPECT_EQ(3, oaSystem.reliefComponents().size());  //     o-----ERV-----(Relief)
  EXPECT_EQ(5, oaSystem.oaComponents().size());      //  (OA)-----ERV-----o-----Coil1-----o
                                                     //  <=>
                                                     //  (RE)---- ERV---------------------o
                                                     //            X
                                                     //  (OA)-----ERV-----o-----Coil1-----o

  CoilHeatingElectric reliefCoil(m);
  EXPECT_TRUE(reliefCoil.addToNode(outboardReliefNode));
  EXPECT_EQ(10, oaSystem.components().size());
  EXPECT_EQ(outboardReliefNode, oaSystem.outboardReliefNode().get());
  EXPECT_EQ(outboardOANode, oaSystem.outboardOANode().get());
  EXPECT_EQ(5, oaSystem.reliefComponents().size());  //     o-----ERV-----o-----Coil2----(Relief)
  EXPECT_EQ(5, oaSystem.oaComponents().size());      //  (OA)-----ERV-----o-----Coil1-----o
                                                     //  <=>
                                                     //  (RE)---- Coil2-----o----ERV---------------------o
                                                     //                           X
                                                     //  (OA)--------------------ERV-----o-----Coil1-----o
}

TEST_F(ModelFixture, AirLoopHVACOutdoorAirSystem_Clone_Simple) {
  Model m;

  ControllerOutdoorAir controller(m);
  AirLoopHVACOutdoorAirSystem oaSystem(m, controller);

  ASSERT_TRUE(oaSystem.outboardReliefNode());
  auto outboardReliefNode = oaSystem.outboardReliefNode().get();
  auto outboardOANode = oaSystem.outboardOANode().get();
  outboardOANode.setName("Outboard OA Node");

  CoilHeatingElectric oaCoil(m);
  EXPECT_TRUE(oaCoil.addToNode(outboardOANode));

  CoilHeatingElectric reliefCoil(m);
  EXPECT_TRUE(reliefCoil.addToNode(outboardReliefNode));

  EXPECT_EQ(6, oaSystem.components().size());
  EXPECT_EQ(3, oaSystem.reliefComponents().size());
  EXPECT_EQ(3, oaSystem.oaComponents().size());  // (OA)-----Coil------o
  EXPECT_EQ(1, m.getConcreteModelObjects<AirLoopHVACOutdoorAirSystem>().size());
  EXPECT_EQ(1, m.getConcreteModelObjects<ControllerOutdoorAir>().size());
  EXPECT_EQ(4, m.getConcreteModelObjects<Node>().size());
  EXPECT_EQ(2, m.getConcreteModelObjects<CoilHeatingElectric>().size());

  {
    // Other model first
    Model m2;
    auto oaSystem2 = oaSystem.clone(m2).cast<AirLoopHVACOutdoorAirSystem>();

    EXPECT_NE(controller, oaSystem2.getControllerOutdoorAir());

    EXPECT_EQ(6, oaSystem2.components().size());
    EXPECT_EQ(3, oaSystem2.reliefComponents().size());
    EXPECT_EQ(3, oaSystem2.oaComponents().size());  // (OA)-----Coil------o
    EXPECT_EQ(1, m2.getConcreteModelObjects<AirLoopHVACOutdoorAirSystem>().size());
    EXPECT_EQ(1, m2.getConcreteModelObjects<ControllerOutdoorAir>().size());
    EXPECT_EQ(4, m2.getConcreteModelObjects<Node>().size());
    EXPECT_EQ(2, m2.getConcreteModelObjects<CoilHeatingElectric>().size());
  }

  {
    // Same model
    auto oaSystem2 = oaSystem.clone(m).cast<AirLoopHVACOutdoorAirSystem>();

    EXPECT_NE(controller, oaSystem2.getControllerOutdoorAir());

    EXPECT_EQ(6, oaSystem2.components().size());
    EXPECT_EQ(3, oaSystem2.reliefComponents().size());
    EXPECT_EQ(3, oaSystem2.oaComponents().size());  // (OA)-----Coil------o
    EXPECT_EQ(2, m.getConcreteModelObjects<AirLoopHVACOutdoorAirSystem>().size());
    EXPECT_EQ(2, m.getConcreteModelObjects<ControllerOutdoorAir>().size());
    EXPECT_EQ(8, m.getConcreteModelObjects<Node>().size());
    EXPECT_EQ(4, m.getConcreteModelObjects<CoilHeatingElectric>().size());
  }
}

TEST_F(ModelFixture, AirLoopHVACOutdoorAirSystem_Clone_Shared) {
  Model m;

  ControllerOutdoorAir controller(m);
  AirLoopHVACOutdoorAirSystem oaSystem(m, controller);

  ASSERT_TRUE(oaSystem.outboardReliefNode());
  auto outboardReliefNode = oaSystem.outboardReliefNode().get();
  auto outboardOANode = oaSystem.outboardOANode().get();
  outboardOANode.setName("Outboard OA Node");

  CoilHeatingElectric oaCoil(m);
  oaCoil.setName("OACoil");
  EXPECT_TRUE(oaCoil.addToNode(outboardOANode));

  HeatExchangerAirToAirSensibleAndLatent erv(m);
  erv.setName("ERV");
  EXPECT_TRUE(erv.addToNode(outboardOANode));

  CoilHeatingElectric reliefCoil(m);
  reliefCoil.setName("RECoil");
  EXPECT_TRUE(reliefCoil.addToNode(outboardReliefNode));

  EXPECT_EQ(10, oaSystem.components().size());       // ERV counted twice
  EXPECT_EQ(5, oaSystem.reliefComponents().size());  //     o-----ERV-----o-----RECoil----(RE)
  EXPECT_EQ(5, oaSystem.oaComponents().size());      //  (OA)-----ERV-----o-----OACoil-----o
                                                     //  <=>
                                                     //  (RE)---- RECoil-----o-----ERV----------------------o
                                                     //                             X
                                                     //  (OA)----------------------ERV-----o-----OACoil-----o
  EXPECT_EQ(1, m.getConcreteModelObjects<AirLoopHVACOutdoorAirSystem>().size());
  EXPECT_EQ(1, m.getConcreteModelObjects<ControllerOutdoorAir>().size());
  EXPECT_EQ(6, m.getConcreteModelObjects<Node>().size());
  EXPECT_EQ(2, m.getConcreteModelObjects<CoilHeatingElectric>().size());
  EXPECT_EQ(1, m.getConcreteModelObjects<HeatExchangerAirToAirSensibleAndLatent>().size());

  {
    // Other model first
    Model m2;
    auto oaSystem2 = oaSystem.clone(m2).cast<AirLoopHVACOutdoorAirSystem>();

    EXPECT_NE(controller, oaSystem2.getControllerOutdoorAir());

    EXPECT_EQ(10, oaSystem2.components().size());
    EXPECT_EQ(5, oaSystem2.reliefComponents().size());
    EXPECT_EQ(5, oaSystem2.oaComponents().size());
    EXPECT_EQ(1, m2.getConcreteModelObjects<AirLoopHVACOutdoorAirSystem>().size());
    EXPECT_EQ(1, m2.getConcreteModelObjects<ControllerOutdoorAir>().size());
    EXPECT_EQ(6, m2.getConcreteModelObjects<Node>().size());
    EXPECT_EQ(2, m2.getConcreteModelObjects<CoilHeatingElectric>().size());
    EXPECT_EQ(1, m2.getConcreteModelObjects<HeatExchangerAirToAirSensibleAndLatent>().size());
  }

  {
    // Same model
    auto oaSystem2 = oaSystem.clone(m).cast<AirLoopHVACOutdoorAirSystem>();

    EXPECT_NE(controller, oaSystem2.getControllerOutdoorAir());

    EXPECT_EQ(10, oaSystem2.components().size());
    EXPECT_EQ(5, oaSystem2.reliefComponents().size());
    EXPECT_EQ(5, oaSystem2.oaComponents().size());
    EXPECT_EQ(2, m.getConcreteModelObjects<AirLoopHVACOutdoorAirSystem>().size());
    EXPECT_EQ(2, m.getConcreteModelObjects<ControllerOutdoorAir>().size());
    EXPECT_EQ(12, m.getConcreteModelObjects<Node>().size());
    EXPECT_EQ(4, m.getConcreteModelObjects<CoilHeatingElectric>().size());
    EXPECT_EQ(2, m.getConcreteModelObjects<HeatExchangerAirToAirSensibleAndLatent>().size());
  }
}

TEST_F(ModelFixture, AirLoopHVACOutdoorAirSystem_HeatCoolFuelTypes) {
  Model m;

  ControllerOutdoorAir controller(m);
  AirLoopHVACOutdoorAirSystem oaSystem(m, controller);
  auto outboardReliefNode = oaSystem.outboardReliefNode().get();
  auto outboardOANode = oaSystem.outboardOANode().get();

  EXPECT_EQ(ComponentType(ComponentType::None), oaSystem.componentType());
  testFuelTypeEquality({}, oaSystem.coolingFuelTypes());
  testFuelTypeEquality({}, oaSystem.heatingFuelTypes());
  testAppGFuelTypeEquality({}, oaSystem.appGHeatingFuelTypes());

  CoilHeatingElectric oaCoil(m);
  EXPECT_TRUE(oaCoil.addToNode(outboardOANode));

  EXPECT_EQ(ComponentType(ComponentType::Heating), oaSystem.componentType());
  testFuelTypeEquality({}, oaSystem.coolingFuelTypes());
  testFuelTypeEquality({FuelType::Electricity}, oaSystem.heatingFuelTypes());
  testAppGFuelTypeEquality({AppGFuelType::Electric}, oaSystem.appGHeatingFuelTypes());

  CoilCoolingDXVariableSpeed reliefCoil(m);
  EXPECT_TRUE(reliefCoil.addToNode(outboardReliefNode));

  EXPECT_EQ(ComponentType(ComponentType::Both), oaSystem.componentType());
  testFuelTypeEquality({FuelType::Electricity}, oaSystem.coolingFuelTypes());
  testFuelTypeEquality({FuelType::Electricity}, oaSystem.heatingFuelTypes());
  testAppGFuelTypeEquality({AppGFuelType::Electric}, oaSystem.appGHeatingFuelTypes());
}
