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

#include "../ChillerElectricEIR.hpp"
#include "../CentralHeatPumpSystemModule.hpp"
#include "../CentralHeatPumpSystemModule_Impl.hpp"

#include "../Schedule.hpp"

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
     Model m;
     CentralHeatPumpSystem central_hp(m);

     exit(0);
  } ,
    ::testing::ExitedWithCode(0), "" );
}

// Test the various setters and getters
TEST(ChillerHeaterPerformanceElectricEIR,ChillerHeaterPerformanceElectricEIR_SettersGetters)
{
  Model m;

  CentralHeatPumpSystem central_hp(m);

  // std::vector<CentralHeatPumpSystemModule> modules()
  ASSERT_EQ( (unsigned)1, central_hp.modules().size() );
  model::CentralHeatPumpSystemModule central_hp_module = central_hp.modules()[0];

  // Return type: bool
  // Only "SmartMixing" is valid
  EXPECT_EQ("SmartMixing", central_hp.controlMethod());
  ASSERT_TRUE(central_hp.setControlMethod("SmartMixing"));
  EXPECT_TRUE(central_hp.controlMethod());
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
  ASSERT_EQ(schedule, central_hp.ancillaryOperationSchedule().get());
  // Return type: void
  central_hp.resetAncillaryOperationSchedule();
  ASSERT_FALSE(central_hp.ancillaryOperationSchedule());



  // Test the CentralHeatPumpSystemModule
  // ChillerHeaterPerformanceElectricEIR chillerHeaterModulesPerformanceComponent() const;
  // Schedule chillerHeaterModulesControlSchedule() const;
  // int numberofChillerHeaterModules() const;

  // Init a new one
  CentralHeatPumpSystemModule central_hp_module_2(m);
  // numberofChillerHeaterModules
  EXPECT_EQ(1, central_hp_module_2.numberofChillerHeaterModules);
  // Return type: bool
  ASSERT_TRUE(central_hp_module2.setNumberofChillerHeaterModules(2));
  EXPECT_TRUE(central_hp_module2.numberofChillerHeaterModules());
  ASSERT_EQ(2, central_hp_module.numberofChillerHeaterModules());


  // ChillerHeaterModulesPerformanceComponent
  ChillerHeaterPerformanceElectricEIR ch_heater(m);
  // Return type: bool
  ASSERT_TRUE(central_hp_module.setChillerHeaterModulesPerformanceComponent(ch_heater));
  EXPECT_TRUE(central_hp_module.chillerHeaterModulesPerformanceComponent());
  ASSERT_EQ(ch_heater, central_hp_module.chillerHeaterModulesPerformanceComponent());

  // chillerHeaterModulesControlSchedule
  EXPECT_EQ(m.alwaysOnDiscreteSchedule(), central_hp_module2.chillerHeaterModulesControlSchedule());
  Schedule schedule2(m);
  ASSERT_TRUE(central_hp_module2.setChillerHeaterModulesControlSchedule(schedule2));
  EXPECT_TRUE(central_hp_module2.chillerHeaterModulesControlSchedule());
  ASSERT_EQ(schedule2, central_hp_module2.chillerHeaterModulesControlSchedule());


  // Test adding/removing modules
  // Return type: bool
  ASSERT(central_hp.addModule(central_hp_module_2);)
  ASSERT_EQ( (unsigned)2, central_hp.modules().size() );
  // Return type: void
  central_hp.removeModule(central_hp_module);
  ASSERT_EQ( (unsigned)1, central_hp.modules().size() );
  ASSERT_EQ( central_hp_module_2, central_hp.modules()[0]);
  // Return type: void
  central_hp.removeAllModules();
  ASSERT_EQ( (unsigned)0, central_hp.modules().size() );

}

TEST(ChillerHeaterPerformanceElectricEIR,ChillerHeaterPerformanceElectricEIR_PlantLoopConnections)
{
    Model m;
    CentralHeatPumpSystem central_hp(m);

    // CoolingLoop: on the supply side
    {
      PlantLoop coolingPlant(m);
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
    }

    // SourceLoop: on the demand side
    {
      PlantLoop sourcePlant(m);
      auto node = sourcePlant.demandInletNode();
      EXPECT_TRUE(central_hp.addToNode(node));
      auto plant = central_hp.secondaryPlantLoop();
      EXPECT_TRUE(plant);
      if( plant ) {
        EXPECT_EQ(sourcePlant.handle(),plant->handle());
      }
      // TODO: Change that number?
      EXPECT_EQ(7u,sourcePlant.demandComponents().size());
    }

    // HeatingLoop: on the supply side
    {
      PlantLoop heatingPlant(m);
      auto node = heatingPlant.supplyOutletNode();
      EXPECT_TRUE(central_hp.addToTertiaryNode(node));
      auto plant = central_hp.tertiaryPlantLoop();
      EXPECT_TRUE(plant);
      if( plant ) {
        EXPECT_EQ(heatingPlant.handle(),plant->handle());
      }
      // TODO: Change that number?
      EXPECT_EQ(7u,heatingPlant.supplyComponents().size());

      EXPECT_TRUE( central_hp.removeFromTertiaryPlantLoop() );
      plant = central_hp.tertiaryPlantLoop();
      EXPECT_FALSE(plant);
      EXPECT_EQ(5u,heatingPlant.demandComponents().size());
    }
  }

