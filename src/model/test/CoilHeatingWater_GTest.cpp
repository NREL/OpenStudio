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
#include "../CoilHeatingWater.hpp"
#include "../CoilHeatingWater_Impl.hpp"
#include "../ControllerWaterCoil.hpp"
#include "../ControllerWaterCoil_Impl.hpp"
#include "../ScheduleCompact.hpp"
#include "../AirLoopHVAC.hpp"
#include "../PlantLoop.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"

using namespace openstudio::model;

TEST_F(ModelFixture, CoilHeatingWater_CoilHeatingWater) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model m;
      ScheduleCompact s(m);
      CoilHeatingWater coil(m, s);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");
}

TEST_F(ModelFixture, CoilHeatingWater_addToNode) {
  Model m;
  ScheduleCompact s(m);
  CoilHeatingWater coil(m, s);

  AirLoopHVAC airLoop(m);
  Node supplyOutletNode = airLoop.supplyOutletNode();

  coil.addToNode(supplyOutletNode);
  ASSERT_EQ((unsigned)3, airLoop.supplyComponents().size());
}

// Add CoilHeatingWater to AirLoopHVAC and PlantLoop
// Test CoilHeatingWater::remove and make sure that the loops are intact
TEST_F(ModelFixture, CoilHeatingWater_remove) {
  Model m;
  ScheduleCompact s(m);
  CoilHeatingWater coil(m, s);

  AirLoopHVAC airLoop(m);
  Node supplyOutletNode = airLoop.supplyOutletNode();

  coil.addToNode(supplyOutletNode);
  coil.remove();
  ASSERT_EQ((unsigned)2, airLoop.supplyComponents().size());

  ASSERT_TRUE(m.getModelObjects<CoilHeatingWater>().empty());

  CoilHeatingWater coil2(m, s);
  coil2.addToNode(supplyOutletNode);

  PlantLoop plant(m);
  plant.addDemandBranchForComponent(coil2);

  ASSERT_EQ((unsigned)1, m.getModelObjects<CoilHeatingWater>().size());
  ASSERT_EQ((unsigned)1, plant.demandComponents(CoilHeatingWater::iddObjectType()).size());

  coil2.remove();
  ASSERT_TRUE(m.getModelObjects<CoilHeatingWater>().empty());
}

// Add ONLY to PlantLoop
// Test CoilHeatingWater::remove and make sure that the plant is intact
TEST_F(ModelFixture, CoilHeatingWater_remove2) {
  Model m;
  ScheduleCompact s(m);
  CoilHeatingWater coil(m, s);

  PlantLoop plant(m);
  plant.addDemandBranchForComponent(coil);
  ASSERT_EQ((unsigned)1, m.getModelObjects<CoilHeatingWater>().size());
  ASSERT_EQ((unsigned)1, plant.demandComponents(CoilHeatingWater::iddObjectType()).size());

  coil.remove();
  ASSERT_TRUE(m.getModelObjects<CoilHeatingWater>().empty());
}

// Add ONLY to PlantLoop
// This time use removeDemandBranchWithComponent
TEST_F(ModelFixture, CoilHeatingWater_remove3) {
  Model m;
  ScheduleCompact s(m);
  CoilHeatingWater coil(m, s);

  PlantLoop plant(m);
  plant.addDemandBranchForComponent(coil);

  ASSERT_EQ((unsigned)1, m.getModelObjects<CoilHeatingWater>().size());
  ASSERT_EQ((unsigned)1, plant.demandComponents(CoilHeatingWater::iddObjectType()).size());

  plant.removeDemandBranchWithComponent(coil);
  ASSERT_TRUE(plant.demandComponents(CoilHeatingWater::iddObjectType()).empty());
  ASSERT_TRUE(m.getModelObjects<CoilHeatingWater>().empty());
}

// Add CoilHeatingWater to AirLoopHVAC and PlantLoop
// Test PlantLoop::removeDemandBranchWithComponent and make sure that the loops are intact
// Test that the coil is still in the model and hooked up to AirLoopHVAC
TEST_F(ModelFixture, CoilHeatingWater_remove4) {
  Model m;
  ScheduleCompact s(m);
  CoilHeatingWater coil(m, s);

  AirLoopHVAC airLoop(m);
  Node supplyOutletNode = airLoop.supplyOutletNode();

  coil.addToNode(supplyOutletNode);

  PlantLoop plant(m);
  EXPECT_TRUE(plant.addDemandBranchForComponent(coil));

  ASSERT_EQ((unsigned)3, airLoop.supplyComponents().size());
  ASSERT_EQ((unsigned)1, m.getModelObjects<CoilHeatingWater>().size());
  ASSERT_EQ((unsigned)1, plant.demandComponents(CoilHeatingWater::iddObjectType()).size());

  plant.removeDemandBranchWithComponent(coil);
  ASSERT_TRUE(plant.demandComponents(CoilHeatingWater::iddObjectType()).empty());
  ASSERT_EQ((unsigned)3, airLoop.supplyComponents().size());
  ASSERT_EQ((unsigned)1, m.getModelObjects<CoilHeatingWater>().size());
}

TEST_F(ModelFixture, CoilHeatingWater_controller) {
  Model m;
  ScheduleCompact s(m);
  CoilHeatingWater coil(m, s);

  ControllerWaterCoil controller(m);
  controller.getImpl<detail::ControllerWaterCoil_Impl>()->setWaterCoil(coil);

  EXPECT_TRUE(coil.controllerWaterCoil());
}
