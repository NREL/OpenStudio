/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../Model.hpp"

#include "../CentralHeatPumpSystem.hpp"
#include "../CentralHeatPumpSystem_Impl.hpp"

#include "../CentralHeatPumpSystemModule.hpp"
#include "../CentralHeatPumpSystemModule_Impl.hpp"
#include "../ChillerHeaterPerformanceElectricEIR.hpp"

#include "../Schedule.hpp"
#include "../ScheduleCompact.hpp"

// TODO: Write the tests for the connections to the PlantLoops
#include "../PlantLoop.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST(CentralHeatPumpSystem,CentralHeatPumpSystem_CentralHeatPumpSystem)
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
TEST(CentralHeatPumpSystem,CentralHeatPumpSystem_SettersGetters)
{
  Model model;

  CentralHeatPumpSystem central_hp(model);

  // std::vector<CentralHeatPumpSystemModule> modules()
  ASSERT_EQ( (unsigned)1, central_hp.modules().size() );
  model::CentralHeatPumpSystemModule central_hp_module = central_hp.modules()[0];

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
  EXPECT_TRUE(central_hp_module_2.numberofChillerHeaterModules());
  ASSERT_EQ(2, central_hp_module.numberofChillerHeaterModules());


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

TEST(CentralHeatPumpSystem,CentralHeatPumpSystem_PlantLoopConnections)
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
      // TODO: Change that number?
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
      // TODO: Change that number?
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

      // TODO: Change that number?
      EXPECT_EQ(7u,heatingPlant.supplyComponents().size());

      EXPECT_TRUE( central_hp.removeFromTertiaryPlantLoop() );
      plant = central_hp.tertiaryPlantLoop();
      EXPECT_FALSE(plant);
      EXPECT_EQ(5u,heatingPlant.demandComponents().size());
    }
  }

