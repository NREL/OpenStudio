/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
#include "../PlantComponentUserDefined.hpp"
#include "../PlantComponentUserDefined_Impl.hpp"
#include "../AirLoopHVAC.hpp"
#include "../PlantLoop.hpp"
#include "../ThermalZone.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"
#include "../AirLoopHVACZoneSplitter.hpp"
#include "../EnergyManagementSystemProgramCallingManager.hpp"
#include "../EnergyManagementSystemActuator.hpp"

#include "../../utilities/core/Path.hpp"
#include "../../utilities/idf/IdfFile.hpp"
#include "../../utilities/idf/IdfObject.hpp"

//using namespace openstudio::model;
using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(ModelFixture, PlantComponentUserDefined_PlantComponentUserDefined)
{
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT (
  {
     Model m;
     PlantComponentUserDefined boiler(m);

     exit(0);
  } ,
    ::testing::ExitedWithCode(0), "" );
}

TEST_F(ModelFixture, PlantComponentUserDefined_connections)
{
  Model m;
  PlantComponentUserDefined boiler(m);

  Node inletNode(m);
  Node outletNode(m);

  m.connect(inletNode,inletNode.outletPort(),boiler,boiler.inletPort());
  m.connect(boiler,boiler.outletPort(),outletNode,outletNode.inletPort());

  ASSERT_TRUE( boiler.inletModelObject() );
  ASSERT_TRUE( boiler.outletModelObject() );

  EXPECT_EQ( inletNode.handle(), boiler.inletModelObject()->handle() );
  EXPECT_EQ( outletNode.handle(), boiler.outletModelObject()->handle() );
}

TEST_F(ModelFixture, PlantComponentUserDefined_addToNode) {
  Model m;
  PlantComponentUserDefined testObject(m);

  AirLoopHVAC airLoop(m);

  Node supplyOutletNode = airLoop.supplyOutletNode();

  EXPECT_FALSE(testObject.addToNode(supplyOutletNode));
  EXPECT_EQ( (unsigned)2, airLoop.supplyComponents().size() );

  Node inletNode = airLoop.zoneSplitter().lastOutletModelObject()->cast<Node>();

  EXPECT_FALSE(testObject.addToNode(inletNode));
  EXPECT_EQ((unsigned)5, airLoop.demandComponents().size());

  PlantLoop plantLoop(m);
  supplyOutletNode = plantLoop.supplyOutletNode();
  EXPECT_TRUE(testObject.addToNode(supplyOutletNode));
  EXPECT_EQ((unsigned)7, plantLoop.supplyComponents().size());
  EXPECT_EQ((unsigned)5, plantLoop.demandComponents().size());

  Node demandOutletNode = plantLoop.demandOutletNode();
  EXPECT_TRUE(testObject.addToNode(demandOutletNode));
  EXPECT_EQ((unsigned)5, plantLoop.supplyComponents().size());
  EXPECT_EQ((unsigned)7, plantLoop.demandComponents().size());

  m.save(toPath("./PlantComponentUserDefinedTestBefore.osm"), true);
  EXPECT_EQ((unsigned)7, plantLoop.demandComponents().size());
  EnergyManagementSystemActuator actuator = testObject.designVolumeFlowRateActuator().get();

  PlantComponentUserDefined testObjectClone = testObject.clone(m).cast<PlantComponentUserDefined>();
  EXPECT_TRUE(testObject.designVolumeFlowRateActuator());
  EXPECT_TRUE(testObjectClone.designVolumeFlowRateActuator());
  EXPECT_TRUE(testObjectClone.designVolumeFlowRateActuator().get().actuatedComponent());
  EXPECT_EQ(testObjectClone.designVolumeFlowRateActuator().get().actuatedComponent().get().handle(), testObjectClone.handle());
  m.save(toPath("./PlantComponentUserDefinedTestClone.osm"), true);
}

TEST_F(ModelFixture, PlantComponentUserDefined_remove) {
  Model m;

  PlantLoop plant(m);
  PlantComponentUserDefined b1(m);
  EXPECT_EQ(8u, m.getObjectsByType(EnergyManagementSystemActuator::iddObjectType()).size());
  //EXPECT_EQ(3u, m.getObjectsByType(EnergyManagementSystemProgramCallingManager::iddObjectType()).size());
  PlantComponentUserDefined b2(m);
  EXPECT_EQ(16u, m.getObjectsByType(EnergyManagementSystemActuator::iddObjectType()).size());
  //EXPECT_EQ(6u, m.getObjectsByType(EnergyManagementSystemProgramCallingManager::iddObjectType()).size());

  EXPECT_FALSE(b1.loop());
  EXPECT_FALSE(b2.loop());

  EXPECT_EQ(plant.supplyComponents(Node::iddObjectType()).size(), 3u);

  EXPECT_TRUE(plant.addSupplyBranchForComponent(b1));
  EXPECT_GT(plant.supplyComponents(b1, plant.supplyOutletNode()).size(), 0u);
  EXPECT_TRUE(b1.loop());
  EXPECT_EQ(plant.supplyComponents(Node::iddObjectType()).size(), 4u);
  {
    auto node = b1.outletModelObject()->cast<Node>();
    EXPECT_TRUE(b2.addToNode(node));
    EXPECT_GT(plant.supplyComponents(b2, plant.supplyOutletNode()).size(), 0u);
    EXPECT_TRUE(b2.loop());
  }

  EXPECT_EQ(plant.supplyComponents(Node::iddObjectType()).size(), 5u);

  EXPECT_GT(plant.supplyComponents(b1, plant.supplyOutletNode()).size(), 0u);
  EXPECT_GT(plant.supplyComponents(b2, plant.supplyOutletNode()).size(), 0u);
  EXPECT_EQ(plant.demandComponents(b1, plant.demandOutletNode()).size(), 0u);
  EXPECT_EQ(plant.demandComponents(b2, plant.demandOutletNode()).size(), 0u);

  auto n1 = b1.outletModelObject().get();
  auto n2 = b2.outletModelObject().get();

  b1.remove();
  EXPECT_EQ(plant.supplyComponents(b1, plant.supplyOutletNode()).size(), 0u);
  EXPECT_GT(plant.supplyComponents(b2, plant.supplyOutletNode()).size(), 0u);
  EXPECT_EQ(plant.supplyComponents(Node::iddObjectType()).size(), 4u);
  EXPECT_EQ(8u, m.getObjectsByType(EnergyManagementSystemActuator::iddObjectType()).size());
  //EXPECT_EQ(3u, m.getObjectsByType(EnergyManagementSystemProgramCallingManager::iddObjectType()).size());
  b2.remove();
  EXPECT_EQ(plant.supplyComponents(b2, plant.supplyOutletNode()).size(), 0u);
  EXPECT_EQ(plant.supplyComponents(Node::iddObjectType()).size(), 3u);
  EXPECT_EQ(0u, m.getObjectsByType(EnergyManagementSystemActuator::iddObjectType()).size());
  //EXPECT_EQ(0u, m.getObjectsByType(EnergyManagementSystemProgramCallingManager::iddObjectType()).size());

  EXPECT_TRUE(n1.handle().isNull());
  EXPECT_TRUE(n2.handle().isNull());
}

TEST_F(ModelFixture, PlantComponentUserDefined_programs) {
  Model m;

  PlantLoop plant(m);
  PlantComponentUserDefined b1(m);
  EnergyManagementSystemProgramCallingManager mainPCM(m);
  EnergyManagementSystemProgramCallingManager initPCM(m);
  EnergyManagementSystemProgramCallingManager simPCM(m);
  EXPECT_TRUE(b1.setMainModelProgramCallingManager(mainPCM));
  EXPECT_EQ(mainPCM,b1.mainModelProgramCallingManager());
  EXPECT_TRUE(b1.setPlantInitializationProgramCallingManager(initPCM));
  EXPECT_EQ(initPCM, b1.plantInitializationProgramCallingManager());
  EXPECT_TRUE(b1.setPlantSimulationProgramCallingManager(simPCM));
  EXPECT_EQ(simPCM, b1.plantSimulationProgramCallingManager());
  b1.resetMainModelProgramCallingManager();
  b1.resetPlantInitializationProgramCallingManager();
  b1.resetPlantSimulationProgramCallingManager();
  EXPECT_FALSE(b1.mainModelProgramCallingManager());
  EXPECT_FALSE(b1.plantInitializationProgramCallingManager());
  EXPECT_FALSE(b1.plantSimulationProgramCallingManager());
}

TEST_F(ModelFixture, PlantComponentUserDefined_zone) {
  Model m;

  PlantLoop plant(m);
  ThermalZone tz(m);
  PlantComponentUserDefined b1(m);

  EXPECT_TRUE(b1.setAmbientZone(tz));
  EXPECT_EQ(tz, b1.ambientZone());
  b1.resetAmbientZone();
  EXPECT_FALSE(b1.ambientZone());
}

TEST_F(ModelFixture, PlantComponentUserDefined_constructor) {
  Model m;

  PlantLoop plant(m);
  PlantComponentUserDefined b1(m);

  EXPECT_EQ("MeetsLoadWithNominalCapacityHiOutLimit",b1.plantLoadingMode());
  EXPECT_EQ("NeedsFlowIfLoopOn", b1.plantLoopFlowRequestMode());
  EXPECT_FALSE(b1.setPlantLoopFlowRequestMode("bad value"));
  EXPECT_FALSE(b1.setPlantLoadingMode("bad value"));
  EXPECT_TRUE(b1.setPlantLoopFlowRequestMode("NeedsFlowAndTurnsLoopOn"));
  EXPECT_TRUE(b1.setPlantLoopFlowRequestMode("ReceivesWhateverFlowAvailable"));
  EXPECT_TRUE(b1.setPlantLoadingMode("MeetsLoadWithNominalCapacityLowOutLimit"));
  EXPECT_TRUE(b1.setPlantLoadingMode("MeetsLoadWithNominalCapacity"));
  EXPECT_TRUE(b1.setPlantLoadingMode("MeetsLoadWithPassiveCapacity"));
  EXPECT_TRUE(b1.setPlantLoadingMode("DemandsLoad"));

  EXPECT_TRUE(b1.designVolumeFlowRateActuator());
  EXPECT_TRUE(b1.minimumLoadingCapacityActuator());
  EXPECT_TRUE(b1.maximumLoadingCapacityActuator());
  EXPECT_TRUE(b1.minimumMassFlowRateActuator());
  EXPECT_TRUE(b1.maximumMassFlowRateActuator());
  EXPECT_TRUE(b1.optimalLoadingCapacityActuator());
  EXPECT_TRUE(b1.outletTemperatureActuator());
  EXPECT_TRUE(b1.massFlowRateActuator());

  EXPECT_TRUE(b1.designVolumeFlowRateActuator().get().actuatedComponent());
  EXPECT_EQ(b1.handle(),b1.designVolumeFlowRateActuator().get().actuatedComponent().get().handle());

}