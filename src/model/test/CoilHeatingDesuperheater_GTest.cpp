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
#include "../CoilHeatingDesuperheater.hpp"
#include "../CoilHeatingDesuperheater_Impl.hpp"
#include "../RefrigerationCondenserAirCooled.hpp"
#include "../RefrigerationCondenserAirCooled_Impl.hpp"
#include "../ScheduleCompact.hpp"
#include "../ScheduleCompact_Impl.hpp"
#include "../AirLoopHVAC.hpp"
#include "../PlantLoop.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"
#include "../AirLoopHVACZoneSplitter.hpp"
#include "../AirLoopHVACOutdoorAirSystem.hpp"
#include "../ControllerOutdoorAir.hpp"

using namespace openstudio;
using namespace openstudio::model;

//Test construction of Coil:Heating:Desuperheater
TEST_F(ModelFixture, CoilHeatingDesuperheater_DefaultConstructor) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model model;
      CoilHeatingDesuperheater testObject = CoilHeatingDesuperheater(model);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");
}

//Test removal of Coil:Heating:Desuperheater
TEST_F(ModelFixture, CoilHeatingDesuperheater_Remove) {
  Model model;
  CoilHeatingDesuperheater testObject = CoilHeatingDesuperheater(model);
  RefrigerationCondenserAirCooled condenser = RefrigerationCondenserAirCooled(model);
  testObject.setHeatingSource(condenser);

  std::vector<CoilHeatingDesuperheater> coilHeatingDesuperheaters = model.getModelObjects<CoilHeatingDesuperheater>();
  EXPECT_EQ(1, coilHeatingDesuperheaters.size());

  std::vector<RefrigerationCondenserAirCooled> refrigerationCondensers = model.getModelObjects<RefrigerationCondenserAirCooled>();
  EXPECT_EQ(1, refrigerationCondensers.size());

  testObject.remove();

  coilHeatingDesuperheaters = model.getModelObjects<CoilHeatingDesuperheater>();
  EXPECT_EQ(0, coilHeatingDesuperheaters.size());

  refrigerationCondensers = model.getModelObjects<RefrigerationCondenserAirCooled>();
  EXPECT_EQ(1, refrigerationCondensers.size());
}

//Test the methods that set and get the fields
TEST_F(ModelFixture, CoilHeatingDesuperheater_set_get) {
  Model model;

  CoilHeatingDesuperheater desuperheater(model);

  desuperheater.setHeatReclaimRecoveryEfficiency(999.0);
  desuperheater.setParasiticElectricLoad(999.0);

  EXPECT_DOUBLE_EQ(desuperheater.heatReclaimRecoveryEfficiency(), 999.0);
  EXPECT_DOUBLE_EQ(desuperheater.parasiticElectricLoad(), 999.0);
}

TEST_F(ModelFixture, CoilHeatingDesuperheater_addToNode) {
  Model m;
  CoilHeatingDesuperheater testObject(m);

  AirLoopHVAC airLoop(m);
  ControllerOutdoorAir controllerOutdoorAir(m);
  AirLoopHVACOutdoorAirSystem outdoorAirSystem(m, controllerOutdoorAir);

  Node supplyOutletNode = airLoop.supplyOutletNode();
  outdoorAirSystem.addToNode(supplyOutletNode);

  EXPECT_TRUE(testObject.addToNode(supplyOutletNode));
  EXPECT_EQ((unsigned)5, airLoop.supplyComponents().size());

  Node inletNode = airLoop.zoneSplitter().lastOutletModelObject()->cast<Node>();

  EXPECT_FALSE(testObject.addToNode(inletNode));
  EXPECT_EQ((unsigned)5, airLoop.demandComponents().size());

  PlantLoop plantLoop(m);
  supplyOutletNode = plantLoop.supplyOutletNode();
  EXPECT_FALSE(testObject.addToNode(supplyOutletNode));
  EXPECT_EQ((unsigned)5, plantLoop.supplyComponents().size());

  Node demandOutletNode = plantLoop.demandOutletNode();
  EXPECT_FALSE(testObject.addToNode(demandOutletNode));
  EXPECT_EQ((unsigned)5, plantLoop.demandComponents().size());

  CoilHeatingDesuperheater testObject2(m);

  if (boost::optional<Node> OANode = outdoorAirSystem.outboardOANode()) {
    EXPECT_TRUE(testObject2.addToNode(*OANode));
    EXPECT_EQ((unsigned)5, airLoop.supplyComponents().size());
    EXPECT_EQ((unsigned)3, outdoorAirSystem.oaComponents().size());
  }

  CoilHeatingDesuperheater testObject3(m);

  if (boost::optional<Node> reliefNode = outdoorAirSystem.outboardReliefNode()) {
    EXPECT_TRUE(testObject3.addToNode(*reliefNode));
    EXPECT_EQ((unsigned)5, airLoop.supplyComponents().size());
    EXPECT_EQ((unsigned)3, outdoorAirSystem.reliefComponents().size());
  }

  CoilHeatingDesuperheater testObjectClone = testObject.clone(m).cast<CoilHeatingDesuperheater>();
  supplyOutletNode = airLoop.supplyOutletNode();

  EXPECT_TRUE(testObjectClone.addToNode(supplyOutletNode));
  EXPECT_EQ((unsigned)7, airLoop.supplyComponents().size());
}

TEST_F(ModelFixture, CoilHeatingDesuperheater_AddBranchForHVACComponent_AirLoop) {
  Model model;
  CoilHeatingDesuperheater desuperheater(model);
  AirLoopHVAC airLoop = AirLoopHVAC(model);

  EXPECT_FALSE(airLoop.addBranchForHVACComponent(desuperheater));
}

TEST_F(ModelFixture, CoilHeatingDesuperheater_AddDemandBranchForComponent_PlantLoop) {
  Model model;
  CoilHeatingDesuperheater desuperheater(model);
  PlantLoop plantLoop = PlantLoop(model);

  EXPECT_FALSE(plantLoop.addDemandBranchForComponent(desuperheater));
}

TEST_F(ModelFixture, CoilHeatingDesuperheater_AddSupplyBranchForComponent_PlantLoop) {
  Model model;
  CoilHeatingDesuperheater desuperheater(model);
  PlantLoop plantLoop = PlantLoop(model);

  EXPECT_FALSE(plantLoop.addSupplyBranchForComponent(desuperheater));
}

//Test clone model with default data
TEST_F(ModelFixture, CoilHeatingDesuperheater_CloneModelWithDefaultData) {
  Model model;

  CoilHeatingDesuperheater testObject = CoilHeatingDesuperheater(model);

  CoilHeatingDesuperheater testObjectClone = testObject.clone(model).cast<CoilHeatingDesuperheater>();

  EXPECT_DOUBLE_EQ(testObjectClone.heatReclaimRecoveryEfficiency(), 0.8);
  EXPECT_DOUBLE_EQ(testObjectClone.parasiticElectricLoad(), 0.0);
}

//Test clone model with custom data
TEST_F(ModelFixture, CoilHeatingDesuperheater_CloneModelWithCustomData) {
  Model model;

  CoilHeatingDesuperheater testObject = CoilHeatingDesuperheater(model);
  RefrigerationCondenserAirCooled condenser = RefrigerationCondenserAirCooled(model);

  ScheduleCompact alwaysOn = ScheduleCompact(model);

  testObject.setHeatReclaimRecoveryEfficiency(0.5);
  testObject.setParasiticElectricLoad(1.0);
  testObject.setHeatingSource(condenser);
  testObject.setAvailabilitySchedule(alwaysOn);

  CoilHeatingDesuperheater testObjectClone = testObject.clone(model).cast<CoilHeatingDesuperheater>();

  EXPECT_DOUBLE_EQ(testObjectClone.heatReclaimRecoveryEfficiency(), 0.5);
  EXPECT_DOUBLE_EQ(testObjectClone.parasiticElectricLoad(), 1.0);
  EXPECT_TRUE(testObjectClone.availabilitySchedule());
  EXPECT_EQ(alwaysOn, testObjectClone.availabilitySchedule().get());
  EXPECT_FALSE(testObjectClone.heatingSource());
}

//Test clone two model with custom data
TEST_F(ModelFixture, CoilHeatingDesuperheater_CloneTwoModelWithCustomData) {
  Model model;

  CoilHeatingDesuperheater testObject = CoilHeatingDesuperheater(model);
  RefrigerationCondenserAirCooled condenser = RefrigerationCondenserAirCooled(model);

  ScheduleCompact alwaysOn = ScheduleCompact(model);

  testObject.setHeatReclaimRecoveryEfficiency(0.5);
  testObject.setParasiticElectricLoad(1.0);
  testObject.setHeatingSource(condenser);
  testObject.setAvailabilitySchedule(alwaysOn);

  CoilHeatingDesuperheater testObjectClone = testObject.clone(model).cast<CoilHeatingDesuperheater>();

  Model model2;
  CoilHeatingDesuperheater testObjectClone2 = testObject.clone(model2).cast<CoilHeatingDesuperheater>();

  EXPECT_DOUBLE_EQ(testObjectClone2.heatReclaimRecoveryEfficiency(), 0.5);
  EXPECT_DOUBLE_EQ(testObjectClone2.parasiticElectricLoad(), 1.0);
  EXPECT_TRUE(testObjectClone2.availabilitySchedule());
  EXPECT_NE(alwaysOn, testObjectClone2.availabilitySchedule().get());
  EXPECT_FALSE(testObjectClone2.heatingSource());
  EXPECT_NE(testObjectClone2, testObjectClone);
  EXPECT_NE(testObjectClone, testObject);
}
