/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2022, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
#include "../../model/AirLoopHVACDedicatedOutdoorAirSystem.hpp"
#include "../../model/AirLoopHVACDedicatedOutdoorAirSystem_Impl.hpp"
#include "../../model/AirLoopHVACOutdoorAirSystem.hpp"
#include "../../model/AirLoopHVACOutdoorAirSystem_Impl.hpp"
#include "../../model/ControllerOutdoorAir.hpp"
#include "../../model/ControllerOutdoorAir_Impl.hpp"
#include "../../model/AirLoopHVAC.hpp"
#include "../../model/AirLoopHVAC_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/AirLoopHVACZoneMixer.hpp"
#include "../../model/AirLoopHVACZoneMixer_Impl.hpp"
#include "../../model/AirLoopHVACZoneSplitter.hpp"
#include "../../model/AirLoopHVACZoneSplitter_Impl.hpp"
#include "../../model/PlantLoop.hpp"
#include "../../model/PlantLoop_Impl.hpp"
#include "../../model/CoilCoolingWater.hpp"
#include "../../model/CoilCoolingWater_Impl.hpp"
#include "../../model/CoilCoolingDXTwoSpeed.hpp"
#include "../../model/CoilCoolingDXTwoSpeed_Impl.hpp"
#include "../../model/CoilHeatingElectric.hpp"
#include "../../model/CoilHeatingElectric_Impl.hpp"
#include "../../model/HeatExchangerAirToAirSensibleAndLatent.hpp"
#include "../../model/HeatExchangerAirToAirSensibleAndLatent_Impl.hpp"
#include "../../model/SetpointManagerOutdoorAirReset.hpp"
#include "../../model/SetpointManagerOutdoorAirReset_Impl.hpp"

#include "../../utilities/idf/IdfObject.hpp"
#include "../../utilities/idf/WorkspaceObject.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include "../../utilities/idf/WorkspaceExtensibleGroup.hpp"

#include <utilities/idd/AirLoopHVAC_DedicatedOutdoorAirSystem_FieldEnums.hxx>
#include <utilities/idd/AirLoopHVAC_ControllerList_FieldEnums.hxx>
#include <utilities/idd/AirLoopHVAC_OutdoorAirSystem_FieldEnums.hxx>
#include <utilities/idd/Controller_OutdoorAir_FieldEnums.hxx>
#include <utilities/idd/AirLoopHVAC_Mixer_FieldEnums.hxx>
#include <utilities/idd/AirLoopHVAC_Splitter_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_AirLoopHVACDedicatedOutdoorAirSystem) {
  Model m;

  AirLoopHVAC airLoop(m);
  airLoop.setName("Air Loop 1");
  ControllerOutdoorAir controller1(m);
  AirLoopHVACOutdoorAirSystem oaSystem1(m, controller1);
  oaSystem1.setName("Outdoor Air System 1");
  Node supplyOutlet = airLoop.supplyOutletNode();
  oaSystem1.addToNode(supplyOutlet);

  ControllerOutdoorAir controller2(m);
  AirLoopHVACOutdoorAirSystem oaSystem2(m, controller2);
  oaSystem2.setName("Outdoor Air System 2");
  CoilCoolingWater coil(m);
  AirLoopHVACDedicatedOutdoorAirSystem doaSystem(oaSystem2);
  doaSystem.setName("Dedicated Outdoor Air System 1");
  doaSystem.addAirLoop(airLoop);
  EXPECT_EQ(2u, oaSystem2.components().size());
  EXPECT_TRUE(coil.addToNode(oaSystem2.outboardOANode().get()));
  EXPECT_EQ(4u, oaSystem2.components().size());
  PlantLoop pl(m);
  EXPECT_TRUE(pl.addDemandBranchForComponent(coil));

  ForwardTranslator ft;
  Workspace w = ft.translateModel(m);

  WorkspaceObjectVector idfContrlLists(w.getObjectsByType(IddObjectType::AirLoopHVAC_ControllerList));
  ASSERT_EQ(2u, idfContrlLists.size());

  WorkspaceObjectVector idfContrlOAs(w.getObjectsByType(IddObjectType::Controller_OutdoorAir));
  ASSERT_EQ(1u, idfContrlOAs.size());
  WorkspaceObject idfContrlOA(idfContrlOAs[0]);

  WorkspaceObjectVector idfCoils(w.getObjectsByType(IddObjectType::Coil_Cooling_Water));
  ASSERT_EQ(1u, idfCoils.size());

  WorkspaceObjectVector idfContrlWCs(w.getObjectsByType(IddObjectType::Controller_WaterCoil));
  ASSERT_EQ(1u, idfContrlWCs.size());

  WorkspaceObjectVector idfOASs(w.getObjectsByType(IddObjectType::AirLoopHVAC_OutdoorAirSystem));
  ASSERT_EQ(2u, idfOASs.size());

  WorkspaceObjectVector idfOAMixers(w.getObjectsByType(IddObjectType::OutdoorAir_Mixer));
  ASSERT_EQ(1u, idfOAMixers.size());

  WorkspaceObjectVector idfAirLoops(w.getObjectsByType(IddObjectType::AirLoopHVAC));
  ASSERT_EQ(1u, idfAirLoops.size());

  WorkspaceObjectVector idfDOASs(w.getObjectsByType(IddObjectType::AirLoopHVAC_DedicatedOutdoorAirSystem));
  ASSERT_EQ(1u, idfDOASs.size());
  WorkspaceObject idfDOAS(idfDOASs[0]);

  EXPECT_EQ("Outdoor Air System 2", idfDOAS.getString(AirLoopHVAC_DedicatedOutdoorAirSystemFields::AirLoopHVAC_OutdoorAirSystemName, false).get());
  EXPECT_EQ("Always On Discrete", idfDOAS.getString(AirLoopHVAC_DedicatedOutdoorAirSystemFields::AvailabilityScheduleName, false).get());
  EXPECT_EQ("Dedicated Outdoor Air System 1 Mixer",
            idfDOAS.getString(AirLoopHVAC_DedicatedOutdoorAirSystemFields::AirLoopHVAC_MixerName, false).get());
  EXPECT_EQ("Dedicated Outdoor Air System 1 Splitter",
            idfDOAS.getString(AirLoopHVAC_DedicatedOutdoorAirSystemFields::AirLoopHVAC_SplitterName, false).get());
  EXPECT_EQ(4.5, idfDOAS.getDouble(AirLoopHVAC_DedicatedOutdoorAirSystemFields::PreheatDesignTemperature, false).get());
  EXPECT_EQ(0.004, idfDOAS.getDouble(AirLoopHVAC_DedicatedOutdoorAirSystemFields::PreheatDesignHumidityRatio, false).get());
  EXPECT_EQ(17.5, idfDOAS.getDouble(AirLoopHVAC_DedicatedOutdoorAirSystemFields::PrecoolDesignTemperature, false).get());
  EXPECT_EQ(0.012, idfDOAS.getDouble(AirLoopHVAC_DedicatedOutdoorAirSystemFields::PrecoolDesignHumidityRatio, false).get());
  EXPECT_EQ(1, idfDOAS.getInt(AirLoopHVAC_DedicatedOutdoorAirSystemFields::NumberofAirLoopHVAC, false).get());
  EXPECT_EQ(1u, idfDOAS.numExtensibleGroups());
  auto w_eg = idfDOAS.extensibleGroups()[0].cast<WorkspaceExtensibleGroup>();
  EXPECT_EQ("Air Loop 1", w_eg.getString(AirLoopHVAC_DedicatedOutdoorAirSystemExtensibleFields::AirLoopHVACName, false).get());

  WorkspaceObjectVector idfMixers(w.getObjectsByType(IddObjectType::AirLoopHVAC_Mixer));
  ASSERT_EQ(1u, idfMixers.size());
  WorkspaceObject idfMixer(idfMixers[0]);

  EXPECT_EQ("Dedicated Outdoor Air System 1 Mixer", idfMixer.getString(AirLoopHVAC_MixerFields::Name, false).get());
  EXPECT_EQ("Dedicated Outdoor Air System 1 Mixer Outlet", idfMixer.getString(AirLoopHVAC_MixerFields::OutletNodeName, false).get());
  EXPECT_EQ(1u, idfMixer.numExtensibleGroups());
  auto w_egMixer = idfMixer.extensibleGroups()[0].cast<WorkspaceExtensibleGroup>();
  EXPECT_EQ(idfContrlOA.getString(Controller_OutdoorAirFields::ReliefAirOutletNodeName, false).get(),
            w_egMixer.getString(AirLoopHVAC_MixerExtensibleFields::InletNodeName, false).get());

  WorkspaceObjectVector idfSplitters(w.getObjectsByType(IddObjectType::AirLoopHVAC_Splitter));
  ASSERT_EQ(1u, idfSplitters.size());
  WorkspaceObject idfSplitter(idfSplitters[0]);

  EXPECT_EQ("Dedicated Outdoor Air System 1 Splitter", idfSplitter.getString(AirLoopHVAC_SplitterFields::Name, false).get());
  EXPECT_EQ(coil.airOutletModelObject().get().nameString(), idfSplitter.getString(AirLoopHVAC_SplitterFields::InletNodeName, false).get());
  EXPECT_EQ(1u, idfSplitter.numExtensibleGroups());
  auto w_egSplitter = idfSplitter.extensibleGroups()[0].cast<WorkspaceExtensibleGroup>();
  EXPECT_EQ(idfContrlOA.getString(Controller_OutdoorAirFields::ActuatorNodeName, false).get(),
            w_egSplitter.getString(AirLoopHVAC_SplitterExtensibleFields::OutletNodeName, false).get());
}

TEST_F(EnergyPlusFixture, ForwardTranslator_AirLoopHVACDedicatedOutdoorAirSystem_ControllerList) {

  // Test for #4265 - Do not translate empty AirLoopHVAC:ControllerList for AirLoopHVAC:DedicatedOutdoorAirSystem to avoid E+ Severe errors
  ForwardTranslator ft;

  Model m;

  ControllerOutdoorAir controllerOutdoorAir(m);
  AirLoopHVACOutdoorAirSystem oaSystem(m, controllerOutdoorAir);
  oaSystem.setName("Outdoor Air System 1");
  AirLoopHVACDedicatedOutdoorAirSystem doas(oaSystem);

  // There's only the AirLoopHVACOutdoorAirSystem, but no components on it -> do not translate the ControllerList
  {
    Workspace w = ft.translateModel(m);
    EXPECT_EQ(0, w.getObjectsByType(IddObjectType::AirLoopHVAC_ControllerList).size());

    WorkspaceObjectVector idfDOASs(w.getObjectsByType(IddObjectType::AirLoopHVAC_DedicatedOutdoorAirSystem));
    ASSERT_EQ(1, idfDOASs.size());
    WorkspaceObject idfDOAS(idfDOASs[0]);

    auto _idfOASystem = idfDOAS.getTarget(AirLoopHVAC_DedicatedOutdoorAirSystemFields::AirLoopHVAC_OutdoorAirSystemName);
    ASSERT_TRUE(_idfOASystem);
    EXPECT_FALSE(_idfOASystem->getTarget(AirLoopHVAC_OutdoorAirSystemFields::ControllerListName));
  }

  CoilCoolingWater coil(m);
  EXPECT_EQ(2, oaSystem.components().size());
  EXPECT_TRUE(coil.addToNode(oaSystem.outboardOANode().get()));
  EXPECT_EQ(4, oaSystem.components().size());
  PlantLoop p(m);
  EXPECT_TRUE(p.addDemandBranchForComponent(coil));

  // There's the Controller:WaterCoil, so do translate it
  {
    Workspace w = ft.translateModel(m);
    EXPECT_EQ(1, w.getObjectsByType(IddObjectType::AirLoopHVAC_ControllerList).size());

    WorkspaceObjectVector idfDOASs(w.getObjectsByType(IddObjectType::AirLoopHVAC_DedicatedOutdoorAirSystem));
    ASSERT_EQ(1, idfDOASs.size());
    WorkspaceObject idfDOAS(idfDOASs[0]);

    auto _idfOASystem = idfDOAS.getTarget(AirLoopHVAC_DedicatedOutdoorAirSystemFields::AirLoopHVAC_OutdoorAirSystemName);
    ASSERT_TRUE(_idfOASystem);
    auto _idfControllerList = _idfOASystem->getTarget(AirLoopHVAC_OutdoorAirSystemFields::ControllerListName);
    ASSERT_TRUE(_idfControllerList);

    ASSERT_EQ(1, _idfControllerList->numExtensibleGroups());
    auto w_eg = _idfControllerList->extensibleGroups()[0].cast<WorkspaceExtensibleGroup>();
    EXPECT_EQ("Controller:WaterCoil", w_eg.getString(AirLoopHVAC_ControllerListExtensibleFields::ControllerObjectType).get());
    EXPECT_TRUE(w_eg.getString(AirLoopHVAC_ControllerListExtensibleFields::ControllerName));
  }
}

TEST_F(EnergyPlusFixture, ForwardTranslator_AirLoopHVACDedicatedOutdoorAirSystem_DeleteOAS) {
  // Test for #4539 - Delete OutdoorAirSystem when deleting AirLoopHVAC:DedicatedOutdoorAirSystem

  Model m;

  AirLoopHVAC airLoop(m);
  airLoop.setName("Air Loop 1");
  ControllerOutdoorAir controller1(m);
  AirLoopHVACOutdoorAirSystem oaSystem1(m, controller1);
  oaSystem1.setName("Outdoor Air System 1");
  CoilHeatingElectric coil1(m);
  coil1.addToNode(oaSystem1.outboardOANode().get());

  ControllerOutdoorAir controller2(m);
  AirLoopHVACOutdoorAirSystem oaSystem2(m, controller2);
  oaSystem2.setName("Outdoor Air System 2");
  CoilHeatingElectric coil2(m);
  coil2.addToNode(oaSystem2.outboardOANode().get());

  AirLoopHVACDedicatedOutdoorAirSystem doaSystem(oaSystem1);
  doaSystem.setName("Dedicated Outdoor Air System 1");
  doaSystem.addAirLoop(airLoop);

  SetpointManagerOutdoorAirReset spm1(m);
  SetpointManagerOutdoorAirReset spm2(m);
  spm1.addToNode(coil1.outletModelObject()->optionalCast<Node>().get());
  spm1.addToNode(coil2.outletModelObject()->optionalCast<Node>().get());

  // delete doaSystem and make sure oaSystem2 is deleted (oaSystem1 remains)
  doaSystem.remove();

  ForwardTranslator ft;
  Workspace w = ft.translateModel(m);

  // assert there is only one setpoint manager left in the model
  WorkspaceObjectVector idfSTPTs(w.getObjectsByType(IddObjectType::SetpointManager_OutdoorAirReset));
  ASSERT_EQ(1u, idfSTPTs.size());

  // assert there are no DOAS left
  WorkspaceObjectVector idfDOASs(w.getObjectsByType(IddObjectType::AirLoopHVAC_DedicatedOutdoorAirSystem));
  ASSERT_EQ(0u, idfDOASs.size());
}

TEST_F(EnergyPlusFixture, ForwardTranslator_AirLoopHVACDedicatedOutdoorAirSystem_CoilCoolingDXTwoSpeed) {

  // Test for #4290 - Add a StraightComponent to an AirLoopHVAC:OutdoorAirSystem attached to AirLoopHVAC:DedicatedOutdoorAirSystem
  Model m;

  AirLoopHVAC airLoop(m);
  airLoop.setName("Air Loop 1");

  ControllerOutdoorAir controller1(m);
  AirLoopHVACOutdoorAirSystem oaSystem1(m, controller1);
  oaSystem1.setName("Outdoor Air System 1");
  Node supplyOutlet = airLoop.supplyOutletNode();
  oaSystem1.addToNode(supplyOutlet);

  ControllerOutdoorAir controller2(m);
  AirLoopHVACOutdoorAirSystem oaSystem2(m, controller2);
  oaSystem2.setName("Outdoor Air System 2");
  CoilCoolingDXTwoSpeed coil(m);
  AirLoopHVACDedicatedOutdoorAirSystem doaSystem(oaSystem2);
  doaSystem.setName("Dedicated Outdoor Air System 1");
  doaSystem.addAirLoop(airLoop);
  EXPECT_EQ(2u, oaSystem2.components().size());
  EXPECT_EQ(1u, oaSystem2.oaComponents().size());
  EXPECT_EQ(1u, oaSystem2.reliefComponents().size());
  EXPECT_TRUE(coil.addToNode(oaSystem2.outboardOANode().get()));
  EXPECT_EQ(4u, oaSystem2.components().size());
  EXPECT_EQ(3u, oaSystem2.oaComponents().size());
  EXPECT_EQ(1u, oaSystem2.reliefComponents().size());

  ForwardTranslator ft;
  Workspace w = ft.translateModel(m);

  WorkspaceObjectVector idfContrlLists(w.getObjectsByType(IddObjectType::AirLoopHVAC_ControllerList));
  ASSERT_EQ(1u, idfContrlLists.size());  // FIXME: is 1 correct?

  WorkspaceObjectVector idfContrlOAs(w.getObjectsByType(IddObjectType::Controller_OutdoorAir));
  ASSERT_EQ(1u, idfContrlOAs.size());
  WorkspaceObject idfContrlOA(idfContrlOAs[0]);

  WorkspaceObjectVector idfCoils(w.getObjectsByType(IddObjectType::Coil_Cooling_DX_TwoSpeed));
  ASSERT_EQ(1u, idfCoils.size());

  WorkspaceObjectVector idfContrlWCs(w.getObjectsByType(IddObjectType::Controller_WaterCoil));
  ASSERT_EQ(0u, idfContrlWCs.size());

  WorkspaceObjectVector idfOASs(w.getObjectsByType(IddObjectType::AirLoopHVAC_OutdoorAirSystem));
  ASSERT_EQ(2u, idfOASs.size());

  WorkspaceObjectVector idfOAMixers(w.getObjectsByType(IddObjectType::OutdoorAir_Mixer));
  ASSERT_EQ(1u, idfOAMixers.size());

  WorkspaceObjectVector idfAirLoops(w.getObjectsByType(IddObjectType::AirLoopHVAC));
  ASSERT_EQ(1u, idfAirLoops.size());

  WorkspaceObjectVector idfDOASs(w.getObjectsByType(IddObjectType::AirLoopHVAC_DedicatedOutdoorAirSystem));
  ASSERT_EQ(1u, idfDOASs.size());
  WorkspaceObject idfDOAS(idfDOASs[0]);

  EXPECT_EQ("Outdoor Air System 2", idfDOAS.getString(AirLoopHVAC_DedicatedOutdoorAirSystemFields::AirLoopHVAC_OutdoorAirSystemName, false).get());
  EXPECT_EQ("Always On Discrete", idfDOAS.getString(AirLoopHVAC_DedicatedOutdoorAirSystemFields::AvailabilityScheduleName, false).get());
  EXPECT_EQ("Dedicated Outdoor Air System 1 Mixer",
            idfDOAS.getString(AirLoopHVAC_DedicatedOutdoorAirSystemFields::AirLoopHVAC_MixerName, false).get());
  EXPECT_EQ("Dedicated Outdoor Air System 1 Splitter",
            idfDOAS.getString(AirLoopHVAC_DedicatedOutdoorAirSystemFields::AirLoopHVAC_SplitterName, false).get());
  EXPECT_EQ(4.5, idfDOAS.getDouble(AirLoopHVAC_DedicatedOutdoorAirSystemFields::PreheatDesignTemperature, false).get());
  EXPECT_EQ(0.004, idfDOAS.getDouble(AirLoopHVAC_DedicatedOutdoorAirSystemFields::PreheatDesignHumidityRatio, false).get());
  EXPECT_EQ(17.5, idfDOAS.getDouble(AirLoopHVAC_DedicatedOutdoorAirSystemFields::PrecoolDesignTemperature, false).get());
  EXPECT_EQ(0.012, idfDOAS.getDouble(AirLoopHVAC_DedicatedOutdoorAirSystemFields::PrecoolDesignHumidityRatio, false).get());
  EXPECT_EQ(1, idfDOAS.getInt(AirLoopHVAC_DedicatedOutdoorAirSystemFields::NumberofAirLoopHVAC, false).get());
  EXPECT_EQ(1u, idfDOAS.numExtensibleGroups());
  auto w_eg = idfDOAS.extensibleGroups()[0].cast<WorkspaceExtensibleGroup>();
  EXPECT_EQ("Air Loop 1", w_eg.getString(AirLoopHVAC_DedicatedOutdoorAirSystemExtensibleFields::AirLoopHVACName, false).get());

  WorkspaceObjectVector idfMixers(w.getObjectsByType(IddObjectType::AirLoopHVAC_Mixer));
  ASSERT_EQ(1u, idfMixers.size());
  WorkspaceObject idfMixer(idfMixers[0]);

  EXPECT_EQ("Dedicated Outdoor Air System 1 Mixer", idfMixer.getString(AirLoopHVAC_MixerFields::Name, false).get());
  EXPECT_EQ("Dedicated Outdoor Air System 1 Mixer Outlet", idfMixer.getString(AirLoopHVAC_MixerFields::OutletNodeName, false).get());
  EXPECT_EQ(1u, idfMixer.numExtensibleGroups());
  auto w_egMixer = idfMixer.extensibleGroups()[0].cast<WorkspaceExtensibleGroup>();
  EXPECT_EQ(idfContrlOA.getString(Controller_OutdoorAirFields::ReliefAirOutletNodeName, false).get(),
            w_egMixer.getString(AirLoopHVAC_MixerExtensibleFields::InletNodeName, false).get());

  WorkspaceObjectVector idfSplitters(w.getObjectsByType(IddObjectType::AirLoopHVAC_Splitter));
  ASSERT_EQ(1u, idfSplitters.size());
  WorkspaceObject idfSplitter(idfSplitters[0]);

  EXPECT_EQ("Dedicated Outdoor Air System 1 Splitter", idfSplitter.getString(AirLoopHVAC_SplitterFields::Name, false).get());
  EXPECT_EQ(coil.outletModelObject().get().nameString(), idfSplitter.getString(AirLoopHVAC_SplitterFields::InletNodeName, false).get());
  EXPECT_EQ(1u, idfSplitter.numExtensibleGroups());
  auto w_egSplitter = idfSplitter.extensibleGroups()[0].cast<WorkspaceExtensibleGroup>();
  EXPECT_EQ(idfContrlOA.getString(Controller_OutdoorAirFields::ActuatorNodeName, false).get(),
            w_egSplitter.getString(AirLoopHVAC_SplitterExtensibleFields::OutletNodeName, false).get());
}

TEST_F(EnergyPlusFixture, ForwardTranslator_AirLoopHVACDedicatedOutdoorAirSystem_HeatExchangerAirToAirSensibleAndLatent) {

  // Test for #4293 - Heat exchanger added to AirLoopHVAC:DOAS gets a bad Exhaust Air Inlet Node
  Model m;

  AirLoopHVAC airLoop(m);
  airLoop.setName("Air Loop 1");

  ControllerOutdoorAir controller1(m);
  AirLoopHVACOutdoorAirSystem oaSystem1(m, controller1);
  oaSystem1.setName("Outdoor Air System 1");
  Node supplyOutlet = airLoop.supplyOutletNode();
  oaSystem1.addToNode(supplyOutlet);

  ControllerOutdoorAir controller2(m);
  AirLoopHVACOutdoorAirSystem oaSystem2(m, controller2);
  oaSystem2.setName("Outdoor Air System 2");
  HeatExchangerAirToAirSensibleAndLatent hx(m);
  AirLoopHVACDedicatedOutdoorAirSystem doaSystem(oaSystem2);
  doaSystem.setName("Dedicated Outdoor Air System 1");
  doaSystem.addAirLoop(airLoop);
  EXPECT_EQ(2u, oaSystem2.components().size());
  EXPECT_EQ(1u, oaSystem2.oaComponents().size());
  EXPECT_EQ(1u, oaSystem2.reliefComponents().size());
  EXPECT_TRUE(hx.addToNode(oaSystem2.outboardOANode().get()));
  EXPECT_EQ(6u, oaSystem2.components().size());  // FIXME: is 6 correct?
  EXPECT_EQ(3u, oaSystem2.oaComponents().size());
  EXPECT_EQ(3u, oaSystem2.reliefComponents().size());

  ForwardTranslator ft;
  Workspace w = ft.translateModel(m);

  WorkspaceObjectVector idfContrlLists(w.getObjectsByType(IddObjectType::AirLoopHVAC_ControllerList));
  ASSERT_EQ(1u, idfContrlLists.size());  // FIXME: is 1 correct?

  WorkspaceObjectVector idfContrlOAs(w.getObjectsByType(IddObjectType::Controller_OutdoorAir));
  ASSERT_EQ(1u, idfContrlOAs.size());
  WorkspaceObject idfContrlOA(idfContrlOAs[0]);

  WorkspaceObjectVector idfHXs(w.getObjectsByType(IddObjectType::HeatExchanger_AirToAir_SensibleAndLatent));
  ASSERT_EQ(1u, idfHXs.size());

  WorkspaceObjectVector idfContrlWCs(w.getObjectsByType(IddObjectType::Controller_WaterCoil));
  ASSERT_EQ(0u, idfContrlWCs.size());

  WorkspaceObjectVector idfOASs(w.getObjectsByType(IddObjectType::AirLoopHVAC_OutdoorAirSystem));
  ASSERT_EQ(2u, idfOASs.size());

  WorkspaceObjectVector idfOAMixers(w.getObjectsByType(IddObjectType::OutdoorAir_Mixer));
  ASSERT_EQ(1u, idfOAMixers.size());

  WorkspaceObjectVector idfAirLoops(w.getObjectsByType(IddObjectType::AirLoopHVAC));
  ASSERT_EQ(1u, idfAirLoops.size());

  WorkspaceObjectVector idfDOASs(w.getObjectsByType(IddObjectType::AirLoopHVAC_DedicatedOutdoorAirSystem));
  ASSERT_EQ(1u, idfDOASs.size());
  WorkspaceObject idfDOAS(idfDOASs[0]);

  EXPECT_EQ("Outdoor Air System 2", idfDOAS.getString(AirLoopHVAC_DedicatedOutdoorAirSystemFields::AirLoopHVAC_OutdoorAirSystemName, false).get());
  EXPECT_EQ("Always On Discrete", idfDOAS.getString(AirLoopHVAC_DedicatedOutdoorAirSystemFields::AvailabilityScheduleName, false).get());
  EXPECT_EQ("Dedicated Outdoor Air System 1 Mixer",
            idfDOAS.getString(AirLoopHVAC_DedicatedOutdoorAirSystemFields::AirLoopHVAC_MixerName, false).get());
  EXPECT_EQ("Dedicated Outdoor Air System 1 Splitter",
            idfDOAS.getString(AirLoopHVAC_DedicatedOutdoorAirSystemFields::AirLoopHVAC_SplitterName, false).get());
  EXPECT_EQ(4.5, idfDOAS.getDouble(AirLoopHVAC_DedicatedOutdoorAirSystemFields::PreheatDesignTemperature, false).get());
  EXPECT_EQ(0.004, idfDOAS.getDouble(AirLoopHVAC_DedicatedOutdoorAirSystemFields::PreheatDesignHumidityRatio, false).get());
  EXPECT_EQ(17.5, idfDOAS.getDouble(AirLoopHVAC_DedicatedOutdoorAirSystemFields::PrecoolDesignTemperature, false).get());
  EXPECT_EQ(0.012, idfDOAS.getDouble(AirLoopHVAC_DedicatedOutdoorAirSystemFields::PrecoolDesignHumidityRatio, false).get());
  EXPECT_EQ(1, idfDOAS.getInt(AirLoopHVAC_DedicatedOutdoorAirSystemFields::NumberofAirLoopHVAC, false).get());
  EXPECT_EQ(1u, idfDOAS.numExtensibleGroups());
  auto w_eg = idfDOAS.extensibleGroups()[0].cast<WorkspaceExtensibleGroup>();
  EXPECT_EQ("Air Loop 1", w_eg.getString(AirLoopHVAC_DedicatedOutdoorAirSystemExtensibleFields::AirLoopHVACName, false).get());

  WorkspaceObjectVector idfMixers(w.getObjectsByType(IddObjectType::AirLoopHVAC_Mixer));
  ASSERT_EQ(1u, idfMixers.size());
  WorkspaceObject idfMixer(idfMixers[0]);

  EXPECT_EQ("Dedicated Outdoor Air System 1 Mixer", idfMixer.getString(AirLoopHVAC_MixerFields::Name, false).get());
  EXPECT_EQ(hx.secondaryAirInletModelObject().get().nameString(), idfMixer.getString(AirLoopHVAC_MixerFields::OutletNodeName, false).get());
  EXPECT_EQ(1u, idfMixer.numExtensibleGroups());
  auto w_egMixer = idfMixer.extensibleGroups()[0].cast<WorkspaceExtensibleGroup>();
  EXPECT_EQ(idfContrlOA.getString(Controller_OutdoorAirFields::ReliefAirOutletNodeName, false).get(),
            w_egMixer.getString(AirLoopHVAC_MixerExtensibleFields::InletNodeName, false).get());

  WorkspaceObjectVector idfSplitters(w.getObjectsByType(IddObjectType::AirLoopHVAC_Splitter));
  ASSERT_EQ(1u, idfSplitters.size());
  WorkspaceObject idfSplitter(idfSplitters[0]);

  EXPECT_EQ("Dedicated Outdoor Air System 1 Splitter", idfSplitter.getString(AirLoopHVAC_SplitterFields::Name, false).get());
  EXPECT_EQ(hx.primaryAirOutletModelObject().get().nameString(), idfSplitter.getString(AirLoopHVAC_SplitterFields::InletNodeName, false).get());
  EXPECT_EQ(1u, idfSplitter.numExtensibleGroups());
  auto w_egSplitter = idfSplitter.extensibleGroups()[0].cast<WorkspaceExtensibleGroup>();
  EXPECT_EQ(idfContrlOA.getString(Controller_OutdoorAirFields::ActuatorNodeName, false).get(),
            w_egSplitter.getString(AirLoopHVAC_SplitterExtensibleFields::OutletNodeName, false).get());
}

/* TEST_F(EnergyPlusFixture, ReverseTranslator_AirLoopHVACDedicatedOutdoorAirSystem) {
  openstudio::Workspace workspace(openstudio::StrictnessLevel::Minimal, openstudio::IddFileType::EnergyPlus);

  openstudio::IdfObject idf_controller(openstudio::IddObjectType::Controller_OutdoorAir);
  idf_controller.setString(Controller_OutdoorAirFields::Name, "Controller Outdoor Air 1");

  openstudio::WorkspaceObject epControllerOA = workspace.addObject(idf_controller).get();

  openstudio::IdfObject idf_controllerlist(openstudio::IddObjectType::AirLoopHVAC_ControllerList);
  idf_controllerlist.setString(AirLoopHVAC_ControllerListFields::Name, "Controller List 1");
  idf_controllerlist.setString(1, "Controller:OutdoorAir");
  idf_controllerlist.setString(2, "Controller Outdoor Air 1");

  openstudio::WorkspaceObject epControllerList = workspace.addObject(idf_controllerlist).get();

  openstudio::IdfObject idf_oas(openstudio::IddObjectType::AirLoopHVAC_OutdoorAirSystem);
  idf_oas.setString(AirLoopHVAC_OutdoorAirSystemFields::Name, "Outdoor Air System 1");
  idf_oas.setString(AirLoopHVAC_OutdoorAirSystemFields::ControllerListName, "Controller List 1");

  openstudio::WorkspaceObject epOAS = workspace.addObject(idf_oas).get();

  openstudio::IdfObject idf_doas(openstudio::IddObjectType::AirLoopHVAC_DedicatedOutdoorAirSystem);
  idf_doas.setString(AirLoopHVAC_DedicatedOutdoorAirSystemFields::Name, "Dedicated Outdoor Air System 1");
  idf_doas.setString(AirLoopHVAC_DedicatedOutdoorAirSystemFields::AirLoopHVAC_OutdoorAirSystemName, "Outdoor Air System 1");

  openstudio::WorkspaceObject epDOAS = workspace.addObject(idf_doas).get();

  ReverseTranslator trans;
  ASSERT_NO_THROW(trans.translateWorkspace(workspace));
  Model model = trans.translateWorkspace(workspace);

  std::vector<AirLoopHVACDedicatedOutdoorAirSystem> doass = model.getConcreteModelObjects<AirLoopHVACDedicatedOutdoorAirSystem>();
  ASSERT_EQ(1u, doass.size());
} */
