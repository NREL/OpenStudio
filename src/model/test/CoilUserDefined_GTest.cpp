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
#include "ModelFixture.hpp"

#include "../AirLoopHVAC.hpp"
#include "../CoilUserDefined.hpp"
#include "../CoilUserDefined_Impl.hpp"
#include "../EnergyManagementSystemProgramCallingManager.hpp"
#include "../EnergyManagementSystemProgram.hpp"
#include "../EnergyManagementSystemActuator.hpp"
#include "../ThermalZone.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"

#include "../../utilities/core/Path.hpp"
#include "../../utilities/idf/IdfFile.hpp"
#include "../../utilities/idf/IdfObject.hpp"

//using namespace openstudio::energyplus;
using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, CoilUserDefined_CoilUserDefined) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model m;
      CoilUserDefined coil_ud(m);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");
}

TEST_F(ModelFixture, CoilUserDefined_constructor) {
  Model m;

  CoilUserDefined b1(m);

  EXPECT_TRUE(b1.airOutletTemperatureActuator());
  EXPECT_TRUE(b1.airOutletHumidityRatioActuator());
  EXPECT_TRUE(b1.airMassFlowRateActuator());
  EXPECT_TRUE(b1.plantMinimumMassFlowRateActuator());
  EXPECT_TRUE(b1.plantMaximumMassFlowRateActuator());
  EXPECT_TRUE(b1.plantDesignVolumeFlowRateActuator());
  EXPECT_TRUE(b1.plantMassFlowRateActuator());
  EXPECT_TRUE(b1.plantOutletTemperatureActuator());

  EXPECT_TRUE(b1.airOutletTemperatureActuator().get().actuatedComponent());
  EXPECT_EQ(b1.handle(), b1.airOutletTemperatureActuator().get().actuatedComponent().get().handle());
}

TEST_F(ModelFixture, CoilUserDefined_programs) {
  Model m;

  //PlantLoop plant(m);
  CoilUserDefined b1(m);
  EnergyManagementSystemProgramCallingManager mainPCM(m);
  EnergyManagementSystemProgramCallingManager initPCM(m);
  EXPECT_TRUE(b1.setOverallModelSimulationProgramCallingManager(mainPCM));
  EXPECT_EQ(mainPCM, b1.overallModelSimulationProgramCallingManager().get());
  EXPECT_TRUE(b1.setModelSetupandSizingProgramCallingManager(initPCM));
  EXPECT_EQ(initPCM, b1.modelSetupandSizingProgramCallingManager().get());
  b1.resetOverallModelSimulationProgramCallingManager();
  b1.resetModelSetupandSizingProgramCallingManager();
  EXPECT_FALSE(b1.overallModelSimulationProgramCallingManager());
  EXPECT_FALSE(b1.modelSetupandSizingProgramCallingManager());
}

TEST_F(ModelFixture, CoilUserDefined_addToNode) {
  Model m;

  CoilUserDefined coil(m);

  model::AirLoopHVAC airLoop(m);

  model::Node supplyOutletNode = airLoop.supplyOutletNode();

  coil.addToNode(supplyOutletNode);

  ASSERT_EQ((unsigned)3, airLoop.supplyComponents().size());
}

// Add CoilUserDefined to AirLoopHVAC and PlantLoop
// Test CoilUserDefined::remove and make sure that the loops are intact
TEST_F(ModelFixture, CoilUserDefined_remove) {
  Model m;
  CoilUserDefined coil(m);

  AirLoopHVAC airLoop(m);
  Node supplyOutletNode = airLoop.supplyOutletNode();

  coil.addToNode(supplyOutletNode);
  coil.remove();
  ASSERT_EQ((unsigned)2, airLoop.supplyComponents().size());

  ASSERT_TRUE(m.getConcreteModelObjects<CoilUserDefined>().empty());

  CoilUserDefined coil2(m);
  coil2.addToNode(supplyOutletNode);

  PlantLoop plant(m);
  plant.addDemandBranchForComponent(coil2);

  ASSERT_EQ((unsigned)1, m.getConcreteModelObjects<CoilUserDefined>().size());
  ASSERT_EQ((unsigned)1, plant.demandComponents(CoilUserDefined::iddObjectType()).size());

  coil2.remove();
  ASSERT_TRUE(m.getConcreteModelObjects<CoilUserDefined>().empty());
}

// Add ONLY to PlantLoop
// Test CoilUserDefined::remove and make sure that the plant is intact
TEST_F(ModelFixture, CoilUserDefined_remove2) {
  Model m;
  CoilUserDefined coil(m);

  PlantLoop plant(m);
  plant.addDemandBranchForComponent(coil);
  ASSERT_EQ((unsigned)1, m.getConcreteModelObjects<CoilUserDefined>().size());
  ASSERT_EQ((unsigned)1, plant.demandComponents(CoilUserDefined::iddObjectType()).size());

  coil.remove();
  ASSERT_TRUE(m.getConcreteModelObjects<CoilUserDefined>().empty());
}

// Add ONLY to PlantLoop
// This time use removeDemandBranchWithComponent
TEST_F(ModelFixture, CoilUserDefined_remove3) {
  Model m;
  CoilUserDefined coil(m);

  PlantLoop plant(m);
  plant.addDemandBranchForComponent(coil);

  ASSERT_EQ((unsigned)1, m.getConcreteModelObjects<CoilUserDefined>().size());
  ASSERT_EQ((unsigned)1, plant.demandComponents(CoilUserDefined::iddObjectType()).size());

  plant.removeDemandBranchWithComponent(coil);
  ASSERT_TRUE(plant.demandComponents(CoilUserDefined::iddObjectType()).empty());
  ASSERT_TRUE(m.getConcreteModelObjects<CoilUserDefined>().empty());
}

// Add CoilUserDefined to AirLoopHVAC and PlantLoop
// Test PlantLoop::removeDemandBranchWithComponent and make sure that the loops are intact
// Test that the coil is still in the model and hooked up to AirLoopHVAC
TEST_F(ModelFixture, CoilUserDefined_remove4) {
  Model m;
  CoilUserDefined coil(m);

  AirLoopHVAC airLoop(m);
  Node supplyOutletNode = airLoop.supplyOutletNode();

  coil.addToNode(supplyOutletNode);

  PlantLoop plant(m);
  EXPECT_TRUE(plant.addDemandBranchForComponent(coil));

  ASSERT_EQ((unsigned)3, airLoop.supplyComponents().size());
  ASSERT_EQ((unsigned)1, m.getConcreteModelObjects<CoilUserDefined>().size());
  ASSERT_EQ((unsigned)1, plant.demandComponents(CoilUserDefined::iddObjectType()).size());

  plant.removeDemandBranchWithComponent(coil);
  ASSERT_TRUE(plant.demandComponents(CoilUserDefined::iddObjectType()).empty());
  ASSERT_EQ((unsigned)3, airLoop.supplyComponents().size());
  ASSERT_EQ((unsigned)1, m.getConcreteModelObjects<CoilUserDefined>().size());
}

TEST_F(ModelFixture, CoilUserDefined_otherAPIs) {
  Model m;
  CoilUserDefined coil(m);

  AirLoopHVAC airLoop(m);
  Node supplyOutletNode = airLoop.supplyOutletNode();

  coil.addToNode(supplyOutletNode);
  ASSERT_EQ((unsigned)3, airLoop.supplyComponents().size());

  PlantLoop plant(m);
  plant.addDemandBranchForComponent(coil);

  ASSERT_EQ((unsigned)1, m.getConcreteModelObjects<CoilUserDefined>().size());
  ASSERT_EQ((unsigned)1, plant.demandComponents(CoilUserDefined::iddObjectType()).size());

  EXPECT_FALSE(coil.ambientZone());
  ThermalZone tz(m);
  coil.setAmbientZone(tz);
  EXPECT_EQ(tz.handle(), coil.ambientZone()->handle());
  coil.resetAmbientZone();
  EXPECT_FALSE(coil.ambientZone());
}

TEST_F(ModelFixture, CoilUserDefined_clone) {
  Model m;
  Model m2;

  CoilUserDefined coil(m);
  EXPECT_EQ(13u, m.numObjects());
  EXPECT_EQ(0u, m2.numObjects());
  auto coil2 = coil.clone(m2).cast<CoilUserDefined>();
  EXPECT_EQ(13u, m2.numObjects());
  coil.remove();
  EXPECT_EQ(0u, m.numObjects());
  EXPECT_EQ(13u, m2.numObjects());
  auto cloneCoil = coil2.clone(m2).cast<CoilUserDefined>();
  EXPECT_EQ(26u, m2.numObjects());
  EXPECT_NE(coil2.handle(), cloneCoil.handle());
  EXPECT_NE(coil2.overallModelSimulationProgramCallingManager().get().handle(),
            cloneCoil.overallModelSimulationProgramCallingManager().get().handle());
  EXPECT_NE(coil2.modelSetupandSizingProgramCallingManager().get().handle(), cloneCoil.modelSetupandSizingProgramCallingManager().get().handle());
  EXPECT_NE(coil2.overallSimulationProgram().get().handle(), cloneCoil.overallSimulationProgram().get().handle());
  EXPECT_NE(coil2.initializationSimulationProgram().get().handle(), cloneCoil.initializationSimulationProgram().get().handle());
  EXPECT_NE(coil2.airOutletTemperatureActuator().get().handle(), cloneCoil.airOutletTemperatureActuator().get().handle());

  EXPECT_EQ(cloneCoil.handle(), cloneCoil.airOutletTemperatureActuator().get().actuatedComponent().get().handle());
  EXPECT_EQ(coil2.handle(), coil2.airOutletTemperatureActuator().get().actuatedComponent().get().handle());
}
