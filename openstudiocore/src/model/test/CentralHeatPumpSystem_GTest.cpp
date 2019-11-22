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

#include "../CentralHeatPumpSystem.hpp"
#include "../CentralHeatPumpSystem_Impl.hpp"

#include "../CentralHeatPumpSystemModule.hpp"
#include "../CentralHeatPumpSystemModule_Impl.hpp"
#include "../ChillerHeaterPerformanceElectricEIR.hpp"
#include "../ChillerHeaterPerformanceElectricEIR_Impl.hpp"

#include "../Schedule.hpp"
#include "../ScheduleCompact.hpp"

// Tests for the connections to the PlantLoops
#include "../PlantLoop.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"

#include "../Model.hpp"
#include "../Model_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, CentralHeatPumpSystem_CentralHeatPumpSystem)
{
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT (
  {
    Model model;
    CentralHeatPumpSystem central_hp(model);

    exit(0);
  } ,
    ::testing::ExitedWithCode(0), "" );
}

// Test the various setters and getters
TEST_F(ModelFixture, CentralHeatPumpSystem_SettersGetters)
{
  Model model;

  CentralHeatPumpSystem central_hp(model);

  // std::vector<CentralHeatPumpSystemModule> modules()
  ASSERT_EQ( (unsigned)0, central_hp.modules().size() );
  CentralHeatPumpSystemModule central_hp_module(model);
  central_hp.addModule(central_hp_module);
  ASSERT_EQ( (unsigned)1, central_hp.modules().size() );
  // Test the parent method
  ASSERT_TRUE(central_hp_module.centralHeatPumpSystem() );
  ASSERT_EQ( central_hp.handle(), central_hp_module.centralHeatPumpSystem()->handle() );

  // Return type: bool
  // Only "SmartMixing" is valid
  EXPECT_EQ("SmartMixing", central_hp.controlMethod());
  ASSERT_TRUE(central_hp.setControlMethod("SmartMixing"));
  ASSERT_EQ("SmartMixing", central_hp.controlMethod());

  // Ancillary power
  // Return type: bool
  ASSERT_TRUE(central_hp.setAncillaryPower(0.7));
  EXPECT_TRUE(central_hp.ancillaryPower());
  ASSERT_EQ(0.7, central_hp.ancillaryPower());

  // Test the ancillary Operation Schedule
  // boost::optional<Schedule> ancillaryOperationSchedule() const;
  EXPECT_FALSE(central_hp.ancillaryOperationSchedule());
  Schedule schedule = model.alwaysOnDiscreteSchedule();
  // Return type: bool
  ASSERT_TRUE(central_hp.setAncillaryOperationSchedule(schedule));
  EXPECT_TRUE(central_hp.ancillaryOperationSchedule());
  ASSERT_EQ(schedule.handle(), central_hp.ancillaryOperationSchedule()->handle());
  // Return type: void
  central_hp.resetAncillaryOperationSchedule();
  ASSERT_FALSE(central_hp.ancillaryOperationSchedule());



  // Test the CentralHeatPumpSystemModule
  // ChillerHeaterPerformanceElectricEIR chillerHeaterModulesPerformanceComponent() const;
  // Schedule chillerHeaterModulesControlSchedule() const;
  // int numberofChillerHeaterModules() const;

  // Init a new one
  // CentralHeatPumpSystemModule central_hp_module_2 = CentralHeatPumpSystemModule(model);
  CentralHeatPumpSystemModule central_hp_module_2(model);
  // numberofChillerHeaterModules
  EXPECT_EQ(1, central_hp_module_2.numberofChillerHeaterModules());
  // Return type: bool
  ASSERT_TRUE(central_hp_module_2.setNumberofChillerHeaterModules(2));
  ASSERT_EQ(2, central_hp_module_2.numberofChillerHeaterModules());


  // ChillerHeaterModulesPerformanceComponent
  ChillerHeaterPerformanceElectricEIR ch_heater(model);
  // Return type: bool
  ASSERT_TRUE(central_hp_module.setChillerHeaterModulesPerformanceComponent(ch_heater));
  ASSERT_EQ(ch_heater, central_hp_module.chillerHeaterModulesPerformanceComponent());

  // chillerHeaterModulesControlSchedule
  EXPECT_EQ(model.alwaysOnDiscreteSchedule().handle(), central_hp_module_2.chillerHeaterModulesControlSchedule().handle());
  ScheduleCompact schedule2(model);
  ASSERT_TRUE(central_hp_module_2.setChillerHeaterModulesControlSchedule(schedule2));
  ASSERT_EQ(schedule2, central_hp_module_2.chillerHeaterModulesControlSchedule());


  // Test adding/removing modules
  // Return type: bool
  ASSERT_TRUE(central_hp.addModule(central_hp_module_2));
  ASSERT_EQ( (unsigned)2, central_hp.modules().size() );
  // Return type: void
  central_hp.removeModule(central_hp_module);
  ASSERT_EQ( (unsigned)1, central_hp.modules().size() );
  ASSERT_EQ( central_hp_module_2, central_hp.modules()[0]);
  // Return type: void
  central_hp.removeAllModules();
  ASSERT_EQ( (unsigned)0, central_hp.modules().size() );

}

TEST_F(ModelFixture, CentralHeatPumpSystem_PlantLoopConnections)
{
  Model model;
  CentralHeatPumpSystem central_hp(model);

  // CoolingLoop: on the supply side
  {
    PlantLoop coolingPlant(model);
    auto node = coolingPlant.supplyOutletNode();
    EXPECT_TRUE(central_hp.addToNode(node));
    auto plant = central_hp.plantLoop();
    EXPECT_TRUE(plant);
    if( plant ) {
      EXPECT_EQ(coolingPlant.handle(),plant->handle());
    }
    // PlantLoop has 5 components on the supply side by default (3 Nodes, One splitter, One mixer)
    EXPECT_EQ(7u,coolingPlant.supplyComponents().size());

    // test the convenience function
    auto plant_bis = central_hp.coolingPlantLoop();
    EXPECT_TRUE(plant_bis);
    if( plant ) {
      EXPECT_EQ(coolingPlant.handle(),plant_bis->handle());
    }

  }

  // SourceLoop: on the demand side
  {
    PlantLoop sourcePlant(model);
    auto node = sourcePlant.demandInletNode();
    EXPECT_TRUE(central_hp.addToNode(node));
    auto plant = central_hp.secondaryPlantLoop();
    EXPECT_TRUE(plant);
    if( plant ) {
      EXPECT_EQ(sourcePlant.handle(),plant->handle());
    }

    EXPECT_EQ(7u,sourcePlant.demandComponents().size());

    // test the convenience function
    auto plant_bis = central_hp.sourcePlantLoop();
    EXPECT_TRUE(plant_bis);
    if( plant ) {
      EXPECT_EQ(sourcePlant.handle(),plant_bis->handle());
    }
  }

  // HeatingLoop: on the supply side
  {
    PlantLoop heatingPlant(model);
    auto node = heatingPlant.supplyOutletNode();
    EXPECT_TRUE(central_hp.addToTertiaryNode(node));
    auto plant = central_hp.tertiaryPlantLoop();
    EXPECT_TRUE(plant);
    if( plant ) {
      EXPECT_EQ(heatingPlant.handle(),plant->handle());
    }
    // test the convenience function
    auto plant_bis = central_hp.heatingPlantLoop();
    EXPECT_TRUE(plant_bis);
    if( plant ) {
      EXPECT_EQ(heatingPlant.handle(),plant_bis->handle());
    }

    EXPECT_EQ(7u,heatingPlant.supplyComponents().size());

    EXPECT_TRUE( central_hp.removeFromTertiaryPlantLoop() );
    plant = central_hp.tertiaryPlantLoop();
    EXPECT_FALSE(plant);
    EXPECT_EQ(5u, heatingPlant.demandComponents().size());
  }
}


/* I overriden the base class WaterToWaterComponent addToNode() and addToTertiaryNode()
 * addToNode will call addToTertiaryNode behind the scenes when needed
 * that is if you try to add it to the supply side of a plant loop when it already has a coolingLoop
 * it should add it to the tertiary(=heating) loop
 * This should work with addSupplyBranchForComponent too
 * AddToTertiaryNode is overriden to not work when trying to add to a demand side node */
TEST_F(ModelFixture, CentralHeatPumpSystem_PlantLoopConnections_addToNodeOverride)
{
  Model model;
  CentralHeatPumpSystem central_hp(model);

  PlantLoop coolingPlant(model);

  PlantLoop coolingPlant2(model);
  auto c_supply_node2 = coolingPlant2.supplyOutletNode();

  PlantLoop sourcePlant(model);

  PlantLoop heatingPlant(model);
  auto h_supply_node = heatingPlant.supplyOutletNode();
  auto h_demand_node = heatingPlant.demandInletNode();


  // Connect to the cooling loop
  EXPECT_TRUE(coolingPlant.addSupplyBranchForComponent(central_hp));
  ASSERT_TRUE(central_hp.coolingPlantLoop());
  EXPECT_EQ(coolingPlant, central_hp.coolingPlantLoop().get());

  EXPECT_FALSE(central_hp.sourcePlantLoop());
  EXPECT_FALSE(central_hp.heatingPlantLoop());


  // Connect to the source loop
  EXPECT_TRUE(sourcePlant.addDemandBranchForComponent(central_hp));

  ASSERT_TRUE(central_hp.coolingPlantLoop());
  EXPECT_EQ(coolingPlant, central_hp.coolingPlantLoop().get());

  ASSERT_TRUE(central_hp.sourcePlantLoop());
  EXPECT_EQ(sourcePlant, central_hp.sourcePlantLoop().get());

  EXPECT_FALSE(central_hp.heatingPlantLoop());


  // Have a cooling loop and no heating loop: should connect the heating loop
  EXPECT_TRUE(heatingPlant.addSupplyBranchForComponent(central_hp));
  ASSERT_TRUE(central_hp.coolingPlantLoop());
  EXPECT_EQ(coolingPlant, central_hp.coolingPlantLoop().get());
  ASSERT_TRUE(central_hp.sourcePlantLoop());
  EXPECT_EQ(sourcePlant, central_hp.sourcePlantLoop().get());
  ASSERT_TRUE(central_hp.heatingPlantLoop());
  EXPECT_EQ(heatingPlant, central_hp.heatingPlantLoop().get());


  // Have a cooling loop and a heating loop: should reconnect the cooling loop
  // Try with addToNode instead
  EXPECT_TRUE(central_hp.addToNode(c_supply_node2));

  ASSERT_TRUE(central_hp.coolingPlantLoop());
  EXPECT_EQ(coolingPlant2, central_hp.coolingPlantLoop().get());

  ASSERT_TRUE(central_hp.sourcePlantLoop());
  EXPECT_EQ(sourcePlant, central_hp.sourcePlantLoop().get());

  ASSERT_TRUE(central_hp.heatingPlantLoop());
  EXPECT_EQ(heatingPlant, central_hp.heatingPlantLoop().get());


  // Disconnect the tertiary (heating) loop
  EXPECT_TRUE(central_hp.removeFromTertiaryPlantLoop());
  EXPECT_FALSE(central_hp.heatingPlantLoop());

  // Shouldn't accept tertiary connection to the demand side
  EXPECT_FALSE(central_hp.addToTertiaryNode(h_demand_node));

  // Try addToNode to the heating plant loop, should work to connect the heating loop
  EXPECT_TRUE(central_hp.addToNode(h_supply_node));

  ASSERT_TRUE(central_hp.coolingPlantLoop());
  EXPECT_EQ(coolingPlant2, central_hp.coolingPlantLoop().get());

  ASSERT_TRUE(central_hp.sourcePlantLoop());
  EXPECT_EQ(sourcePlant, central_hp.sourcePlantLoop().get());

  ASSERT_TRUE(central_hp.heatingPlantLoop());
  EXPECT_EQ(heatingPlant, central_hp.heatingPlantLoop().get());


}




TEST_F(ModelFixture, CentralHeatPumpSystem_Clone_PlantLoops)
{
  Model model;
  CentralHeatPumpSystem central_hp(model);

  // Connect it to some plant loops
  PlantLoop coolingPlant(model);
  auto node1 = coolingPlant.supplyOutletNode();
  ASSERT_TRUE(central_hp.addToNode(node1));
  PlantLoop sourcePlant(model);
  auto node2 = sourcePlant.demandInletNode();
  ASSERT_TRUE(central_hp.addToNode(node2));
  PlantLoop heatingPlant(model);
  auto node3 = heatingPlant.supplyOutletNode();
  ASSERT_TRUE(central_hp.addToTertiaryNode(node3));

  //Clone into the same model
  CentralHeatPumpSystem  central_hpClone = central_hp.clone(model).cast<CentralHeatPumpSystem>();
  // Make sure it isn't connected to the same plant loop
  ASSERT_FALSE(central_hpClone.coolingPlantLoop());
  ASSERT_FALSE(central_hpClone.sourcePlantLoop());
  ASSERT_FALSE(central_hpClone.heatingPlantLoop());


  //Clone into another model
  Model model2;
  CentralHeatPumpSystem  central_hpClone2 = central_hp.clone(model2).cast<CentralHeatPumpSystem>();
  // Make sure it isn't connected to any plant loops
  ASSERT_FALSE(central_hpClone2.coolingPlantLoop());
  ASSERT_FALSE(central_hpClone2.sourcePlantLoop());
  ASSERT_FALSE(central_hpClone2.heatingPlantLoop());

}

// Test cloning the CentralHeatPump system, make sure the modules are cloned too
// Will test cloning the CentralHeatPumpSystemModule too, see if the ChillerHeater is cloned too
TEST_F(ModelFixture, CentralHeatPumpSystem_Clone)
{
  Model model;
  CentralHeatPumpSystem central_hp(model);

  ASSERT_EQ(1u, model.getModelObjects<CentralHeatPumpSystem>().size());
  ASSERT_EQ(0u, model.getModelObjects<CentralHeatPumpSystemModule>().size());

  //Clone into the same model
  CentralHeatPumpSystem  central_hpClone = central_hp.clone(model).cast<CentralHeatPumpSystem>();
  ASSERT_EQ(2u, model.getModelObjects<CentralHeatPumpSystem>().size());
  ASSERT_EQ(0u, model.getModelObjects<CentralHeatPumpSystemModule>().size());

  // Add a CentralHeatPumpSystemModule and clone again
  // Clear the modules first, just in case
  central_hp.removeAllModules();

  CentralHeatPumpSystemModule central_hp_module(model);
  // This should have create one module, and ChillerHeater
  ASSERT_EQ(1u, model.getModelObjects<CentralHeatPumpSystemModule>().size());
  ASSERT_EQ(1u, model.getModelObjects<ChillerHeaterPerformanceElectricEIR>().size());

  ASSERT_TRUE(central_hp.addModule(central_hp_module));
  ASSERT_EQ(1u, central_hp.modules().size());
  // Make sure it didn't touch the previous clone
  // (modules should be a new instance of ModelObjectList, not the same)
  ASSERT_EQ(0u, central_hpClone.modules().size());


  // Clone in same model and verify that the CentralHeatPumpSystemModule is also cloned
  CentralHeatPumpSystem  central_hpClone1 = central_hp.clone(model).cast<CentralHeatPumpSystem>();
  // There should now be 3 CentralHeatPumpSystems
  ASSERT_EQ(3u, model.getModelObjects<CentralHeatPumpSystem>().size());
  // The CentralHeatPumpModule should have been cloned
  ASSERT_EQ(2u, model.getModelObjects<CentralHeatPumpSystemModule>().size());
  // The chillerHeater shouldn't have, it should point to the same
  ASSERT_EQ(1u, model.getModelObjects<ChillerHeaterPerformanceElectricEIR>().size());

  ASSERT_EQ(1u, central_hpClone1.modules().size());
  // Make sure it's not just pointing to the same one
  CentralHeatPumpSystemModule central_hp_moduleClone1 = central_hpClone1.modules()[0];
  EXPECT_NE(central_hp_module.handle(), central_hp_moduleClone1.handle());

  // Check that they still use the same ChillerHeaterPerformanceElectricEIR object
  ChillerHeaterPerformanceElectricEIR ch_heater = central_hp_module.chillerHeaterModulesPerformanceComponent();
  ChillerHeaterPerformanceElectricEIR ch_heaterClone1 = central_hp_moduleClone1.chillerHeaterModulesPerformanceComponent();
  ASSERT_EQ(ch_heater.handle(), ch_heaterClone1.handle());


  //Clone into another model
  Model model2;
  CentralHeatPumpSystem  central_hpClone2 = central_hp.clone(model2).cast<CentralHeatPumpSystem>();
  // Check that the Module and ChillerHeater are carried with
  ASSERT_EQ(1u, model2.getModelObjects<CentralHeatPumpSystem>().size());
  ASSERT_EQ(1u, model2.getModelObjects<CentralHeatPumpSystemModule>().size());
  ASSERT_EQ(1u, model2.getModelObjects<ChillerHeaterPerformanceElectricEIR>().size());
  ASSERT_EQ(1u, central_hpClone2.modules().size());

  CentralHeatPumpSystemModule central_hp_moduleClone2 = central_hpClone2.modules()[0];
  EXPECT_NE(central_hp_module.handle(), central_hp_moduleClone2.handle());

}

TEST_F(ModelFixture, CentralHeatPumpSystemModule_ReverseLookup)
{
  Model model;
  CentralHeatPumpSystem central_hp(model);

  CentralHeatPumpSystemModule central_hp_module(model);
  // Should be empty for now
  ASSERT_FALSE(central_hp_module.centralHeatPumpSystem());

  ASSERT_TRUE(central_hp.addModule(central_hp_module));

  // Test that you can get the CentralHeatPumpSystem from the Module
  ASSERT_TRUE(central_hp_module.centralHeatPumpSystem());
  ASSERT_EQ(central_hp.handle(), central_hp_module.centralHeatPumpSystem()->handle());

}
