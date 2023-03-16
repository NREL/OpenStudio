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
#include "../AirLoopHVACZoneSplitter.hpp"
#include "../AirLoopHVACUnitarySystem.hpp"

#include "../../utilities/core/Path.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, CoilUserDefined_GettersSetters) {
  Model m;

  CoilUserDefined coilUserDefined(m);
  coilUserDefined.setName("My CoilUserDefined");

  // Convenience method provided
  coilUserDefined.renameEMSSubComponents();

  // Test the convenience method + getters
  EXPECT_EQ("My_CoilUserDefined_overallModelSimulationProgramCallingManager",
            coilUserDefined.overallModelSimulationProgramCallingManager().nameString());
  EXPECT_EQ("My_CoilUserDefined_modelSetupandSizingProgramCallingManager", coilUserDefined.modelSetupandSizingProgramCallingManager().nameString());
  EXPECT_EQ("My_CoilUserDefined_overallSimulationProgram", coilUserDefined.overallSimulationProgram().nameString());
  EXPECT_EQ("My_CoilUserDefined_initializationSimulationProgram", coilUserDefined.initializationSimulationProgram().nameString());
  EXPECT_EQ("My_CoilUserDefined_airOutletTemperatureActuator", coilUserDefined.airOutletTemperatureActuator().nameString());
  EXPECT_EQ("My_CoilUserDefined_airOutletHumidityRatioActuator", coilUserDefined.airOutletHumidityRatioActuator().nameString());
  EXPECT_EQ("My_CoilUserDefined_airMassFlowRateActuator", coilUserDefined.airMassFlowRateActuator().nameString());
  EXPECT_EQ("My_CoilUserDefined_plantMinimumMassFlowRateActuator", coilUserDefined.plantMinimumMassFlowRateActuator().nameString());
  EXPECT_EQ("My_CoilUserDefined_plantMaximumMassFlowRateActuator", coilUserDefined.plantMaximumMassFlowRateActuator().nameString());
  EXPECT_EQ("My_CoilUserDefined_plantDesignVolumeFlowRateActuator", coilUserDefined.plantDesignVolumeFlowRateActuator().nameString());
  EXPECT_EQ("My_CoilUserDefined_plantMassFlowRateActuator", coilUserDefined.plantMassFlowRateActuator().nameString());
  EXPECT_EQ("My_CoilUserDefined_plantOutletTemperatureActuator", coilUserDefined.plantOutletTemperatureActuator().nameString());

  // Make sure the actuators are correct
  ASSERT_TRUE(coilUserDefined.airOutletTemperatureActuator().actuatedComponent());
  EXPECT_EQ(coilUserDefined, coilUserDefined.airOutletTemperatureActuator().actuatedComponent().get());
  ASSERT_TRUE(coilUserDefined.airOutletHumidityRatioActuator().actuatedComponent());
  EXPECT_EQ(coilUserDefined, coilUserDefined.airOutletHumidityRatioActuator().actuatedComponent().get());
  ASSERT_TRUE(coilUserDefined.airMassFlowRateActuator().actuatedComponent());
  EXPECT_EQ(coilUserDefined, coilUserDefined.airMassFlowRateActuator().actuatedComponent().get());
  ASSERT_TRUE(coilUserDefined.plantMinimumMassFlowRateActuator().actuatedComponent());
  EXPECT_EQ(coilUserDefined, coilUserDefined.plantMinimumMassFlowRateActuator().actuatedComponent().get());
  ASSERT_TRUE(coilUserDefined.plantMaximumMassFlowRateActuator().actuatedComponent());
  EXPECT_EQ(coilUserDefined, coilUserDefined.plantMaximumMassFlowRateActuator().actuatedComponent().get());
  ASSERT_TRUE(coilUserDefined.plantDesignVolumeFlowRateActuator().actuatedComponent());
  EXPECT_EQ(coilUserDefined, coilUserDefined.plantDesignVolumeFlowRateActuator().actuatedComponent().get());
  ASSERT_TRUE(coilUserDefined.plantMassFlowRateActuator().actuatedComponent());
  EXPECT_EQ(coilUserDefined, coilUserDefined.plantMassFlowRateActuator().actuatedComponent().get());
  ASSERT_TRUE(coilUserDefined.plantOutletTemperatureActuator().actuatedComponent());
  EXPECT_EQ(coilUserDefined, coilUserDefined.plantOutletTemperatureActuator().actuatedComponent().get());

  // Overall Model Simulation Program Calling Manager Name: Required Object
  EnergyManagementSystemProgramCallingManager overallModelSimulationProgramCallingManager(m);
  EXPECT_TRUE(coilUserDefined.setOverallModelSimulationProgramCallingManager(overallModelSimulationProgramCallingManager));
  EXPECT_EQ(overallModelSimulationProgramCallingManager, coilUserDefined.overallModelSimulationProgramCallingManager());

  // Model Setup and Sizing Program Calling Manager Name: Required Object
  EnergyManagementSystemProgramCallingManager modelSetupandSizingProgramCallingManager(m);
  EXPECT_TRUE(coilUserDefined.setModelSetupandSizingProgramCallingManager(modelSetupandSizingProgramCallingManager));
  EXPECT_EQ(modelSetupandSizingProgramCallingManager, coilUserDefined.modelSetupandSizingProgramCallingManager());

  // Ambient Zone Name: Optional Object
  EXPECT_FALSE(coilUserDefined.ambientZone());
  ThermalZone ambientZone(m);
  EXPECT_TRUE(coilUserDefined.setAmbientZone(ambientZone));
  ASSERT_TRUE(coilUserDefined.ambientZone());
  EXPECT_EQ(ambientZone, coilUserDefined.ambientZone().get());
  coilUserDefined.resetAmbientZone();
  EXPECT_FALSE(coilUserDefined.ambientZone());

  // Overall Simulation Program Name: Required Object
  EnergyManagementSystemProgram overallSimulationProgram(m);
  EXPECT_TRUE(coilUserDefined.setOverallSimulationProgram(overallSimulationProgram));
  EXPECT_EQ(overallSimulationProgram, coilUserDefined.overallSimulationProgram());

  // Initialization Simulation Program Name: Required Object
  EnergyManagementSystemProgram initializationSimulationProgram(m);
  EXPECT_TRUE(coilUserDefined.setInitializationSimulationProgram(initializationSimulationProgram));
  EXPECT_EQ(initializationSimulationProgram, coilUserDefined.initializationSimulationProgram());

  // Air Outlet Temperature Actuator: Required Object
  EnergyManagementSystemActuator airOutletTemperatureActuator(coilUserDefined, "Air Connection 1", "Outlet Temperature");
  EXPECT_TRUE(coilUserDefined.setAirOutletTemperatureActuator(airOutletTemperatureActuator));
  EXPECT_EQ(airOutletTemperatureActuator, coilUserDefined.airOutletTemperatureActuator());

  // Air Outlet Humidity Ratio Actuator: Required Object
  EnergyManagementSystemActuator airOutletHumidityRatioActuator(coilUserDefined, "Air Connection 1", "Outlet Humidity Ratio");
  EXPECT_TRUE(coilUserDefined.setAirOutletHumidityRatioActuator(airOutletHumidityRatioActuator));
  EXPECT_EQ(airOutletHumidityRatioActuator, coilUserDefined.airOutletHumidityRatioActuator());

  // Air Mass Flow Rate Actuator: Required Object
  EnergyManagementSystemActuator airMassFlowRateActuator(coilUserDefined, "Air Connection 1", "Mass Flow Rate");
  EXPECT_TRUE(coilUserDefined.setAirMassFlowRateActuator(airMassFlowRateActuator));
  EXPECT_EQ(airMassFlowRateActuator, coilUserDefined.airMassFlowRateActuator());

  // Plant Minimum Mass Flow Rate Actuator: Required Object
  EnergyManagementSystemActuator plantMinimumMassFlowRateActuator(coilUserDefined, "Plant Connection", "Minimum Mass Flow Rate");
  EXPECT_TRUE(coilUserDefined.setPlantMinimumMassFlowRateActuator(plantMinimumMassFlowRateActuator));
  EXPECT_EQ(plantMinimumMassFlowRateActuator, coilUserDefined.plantMinimumMassFlowRateActuator());

  // Plant Maximum Mass Flow Rate Actuator: Required Object
  EnergyManagementSystemActuator plantMaximumMassFlowRateActuator(coilUserDefined, "Plant Connection", "Maximum Mass Flow Rate");
  EXPECT_TRUE(coilUserDefined.setPlantMaximumMassFlowRateActuator(plantMaximumMassFlowRateActuator));
  EXPECT_EQ(plantMaximumMassFlowRateActuator, coilUserDefined.plantMaximumMassFlowRateActuator());

  // Plant Design Volume Flow Rate Actuator: Required Object
  EnergyManagementSystemActuator plantDesignVolumeFlowRateActuator(coilUserDefined, "Plant Connection", "Design Volume Flow Rate");
  EXPECT_TRUE(coilUserDefined.setPlantDesignVolumeFlowRateActuator(plantDesignVolumeFlowRateActuator));
  EXPECT_EQ(plantDesignVolumeFlowRateActuator, coilUserDefined.plantDesignVolumeFlowRateActuator());

  // Plant Outlet Temperature Actuator: Required Object
  EnergyManagementSystemActuator plantOutletTemperatureActuator(coilUserDefined, "Plant Connection", "Outlet Temperature");
  EXPECT_TRUE(coilUserDefined.setPlantOutletTemperatureActuator(plantOutletTemperatureActuator));
  EXPECT_EQ(plantOutletTemperatureActuator, coilUserDefined.plantOutletTemperatureActuator());

  // Plant Mass Flow Rate Actuator: Required Object
  EnergyManagementSystemActuator plantMassFlowRateActuator(coilUserDefined, "Plant Connection", "Mass Flow Rate");
  EXPECT_TRUE(coilUserDefined.setPlantMassFlowRateActuator(plantMassFlowRateActuator));
  EXPECT_EQ(plantMassFlowRateActuator, coilUserDefined.plantMassFlowRateActuator());
}

TEST_F(ModelFixture, CoilUserDefined_addToNode) {

  Model m;

  CoilUserDefined coilUserDefined(m);
  EXPECT_FALSE(coilUserDefined.airLoopHVAC());
  EXPECT_FALSE(coilUserDefined.airInletModelObject());
  EXPECT_FALSE(coilUserDefined.airOutletModelObject());
  EXPECT_FALSE(coilUserDefined.plantLoop());
  EXPECT_FALSE(coilUserDefined.waterInletModelObject());
  EXPECT_FALSE(coilUserDefined.waterOutletModelObject());

  {
    AirLoopHVAC airLoop(m);

    EXPECT_EQ(0, coilUserDefined.numberofAirConnections());

    // Accepted on supply
    Node supplyOutletNode = airLoop.supplyOutletNode();
    EXPECT_FALSE(coilUserDefined.airLoopHVAC());
    EXPECT_EQ(2, airLoop.supplyComponents().size());
    EXPECT_TRUE(coilUserDefined.addToNode(supplyOutletNode));
    EXPECT_EQ(3, airLoop.supplyComponents().size());
    ASSERT_TRUE(coilUserDefined.airLoopHVAC());
    EXPECT_EQ(1, coilUserDefined.numberofAirConnections());
    EXPECT_EQ(airLoop, coilUserDefined.airLoopHVAC().get());
    ASSERT_TRUE(coilUserDefined.airInletModelObject());
    ASSERT_TRUE(coilUserDefined.airOutletModelObject());
    EXPECT_EQ(supplyOutletNode, coilUserDefined.airOutletModelObject().get());
    EXPECT_FALSE(coilUserDefined.plantLoop());
    EXPECT_FALSE(coilUserDefined.waterInletModelObject());
    EXPECT_FALSE(coilUserDefined.waterOutletModelObject());

    // Rejected on demand
    Node demandNode = airLoop.zoneSplitter().lastOutletModelObject()->cast<Node>();
    EXPECT_FALSE(coilUserDefined.addToNode(demandNode));
    // 5u: inlet splitter node mixer outlet.
    EXPECT_EQ(5, airLoop.demandComponents().size());
    EXPECT_EQ(3, airLoop.supplyComponents().size());
    ASSERT_TRUE(coilUserDefined.airLoopHVAC());
    EXPECT_EQ(airLoop, coilUserDefined.airLoopHVAC().get());
    ASSERT_TRUE(coilUserDefined.airInletModelObject());
    ASSERT_TRUE(coilUserDefined.airOutletModelObject());
    EXPECT_EQ(supplyOutletNode, coilUserDefined.airOutletModelObject().get());
    EXPECT_FALSE(coilUserDefined.plantLoop());
    EXPECT_FALSE(coilUserDefined.waterInletModelObject());
    EXPECT_FALSE(coilUserDefined.waterOutletModelObject());

    // Remove
    EXPECT_TRUE(coilUserDefined.removeFromAirLoopHVAC());
    EXPECT_EQ(0, coilUserDefined.numberofAirConnections());
    EXPECT_EQ(2, airLoop.supplyComponents().size());
    EXPECT_FALSE(coilUserDefined.airLoopHVAC());
    EXPECT_FALSE(coilUserDefined.airInletModelObject());
    EXPECT_FALSE(coilUserDefined.airOutletModelObject());
    EXPECT_FALSE(coilUserDefined.plantLoop());
    EXPECT_FALSE(coilUserDefined.waterInletModelObject());
    EXPECT_FALSE(coilUserDefined.waterOutletModelObject());
  }

  {
    PlantLoop plantLoop(m);

    // Rejected on supply
    auto supplyOutletNode = plantLoop.supplyOutletNode();
    EXPECT_EQ(5, plantLoop.supplyComponents().size());
    EXPECT_EQ(5, plantLoop.demandComponents().size());
    EXPECT_FALSE(coilUserDefined.addToNode(supplyOutletNode));
    EXPECT_EQ(5, plantLoop.supplyComponents().size());
    EXPECT_EQ(5, plantLoop.demandComponents().size());
    EXPECT_FALSE(coilUserDefined.airLoopHVAC());
    EXPECT_FALSE(coilUserDefined.airInletModelObject());
    EXPECT_FALSE(coilUserDefined.airOutletModelObject());
    EXPECT_FALSE(coilUserDefined.plantLoop());
    EXPECT_FALSE(coilUserDefined.waterInletModelObject());
    EXPECT_FALSE(coilUserDefined.waterOutletModelObject());

    // Accepted on demand
    auto demandOutletNode = plantLoop.demandOutletNode();
    EXPECT_TRUE(coilUserDefined.addToNode(demandOutletNode));
    EXPECT_EQ(5, plantLoop.supplyComponents().size());
    EXPECT_EQ(7, plantLoop.demandComponents().size());
    EXPECT_FALSE(coilUserDefined.airLoopHVAC());
    EXPECT_FALSE(coilUserDefined.airInletModelObject());
    EXPECT_FALSE(coilUserDefined.airOutletModelObject());
    ASSERT_TRUE(coilUserDefined.plantLoop());
    EXPECT_EQ(plantLoop, coilUserDefined.plantLoop().get());
    ASSERT_TRUE(coilUserDefined.waterInletModelObject());
    ASSERT_TRUE(coilUserDefined.waterOutletModelObject());
    EXPECT_EQ(demandOutletNode, coilUserDefined.waterOutletModelObject().get());

    // Remove
    EXPECT_TRUE(coilUserDefined.removeFromPlantLoop());
    EXPECT_EQ(5, plantLoop.supplyComponents().size());
    EXPECT_EQ(5, plantLoop.demandComponents().size());
    EXPECT_FALSE(coilUserDefined.airLoopHVAC());
    EXPECT_FALSE(coilUserDefined.airInletModelObject());
    EXPECT_FALSE(coilUserDefined.airOutletModelObject());
    EXPECT_FALSE(coilUserDefined.plantLoop());
    EXPECT_FALSE(coilUserDefined.waterInletModelObject());
    EXPECT_FALSE(coilUserDefined.waterOutletModelObject());
  }
}

// Add CoilUserDefined to AirLoopHVAC and PlantLoop
// Test CoilUserDefined::remove and make sure that the loops are intact
TEST_F(ModelFixture, CoilUserDefined_remove) {
  Model m;
  CoilUserDefined coil(m);

  AirLoopHVAC airLoop(m);
  PlantLoop plantLoop(m);
  EXPECT_EQ(2, airLoop.supplyComponents().size());
  EXPECT_EQ(5, airLoop.demandComponents().size());
  EXPECT_EQ(5, plantLoop.supplyComponents().size());
  EXPECT_EQ(5, plantLoop.demandComponents().size());

  Node airSupplyOutletNode = airLoop.supplyOutletNode();
  coil.addToNode(airSupplyOutletNode);
  Node waterDemandOutletNode = plantLoop.demandOutletNode();
  coil.addToNode(waterDemandOutletNode);
  EXPECT_EQ(3, airLoop.supplyComponents().size());
  EXPECT_EQ(5, airLoop.demandComponents().size());
  EXPECT_EQ(5, plantLoop.supplyComponents().size());
  EXPECT_EQ(7, plantLoop.demandComponents().size());

  EXPECT_EQ(1, airLoop.supplyComponents(CoilUserDefined::iddObjectType()).size());
  EXPECT_EQ(1, plantLoop.demandComponents(CoilUserDefined::iddObjectType()).size());

  coil.remove();
  EXPECT_EQ(2, airLoop.supplyComponents().size());
  EXPECT_EQ(5, airLoop.demandComponents().size());
  EXPECT_EQ(5, plantLoop.supplyComponents().size());
  EXPECT_EQ(5, plantLoop.demandComponents().size());

  EXPECT_TRUE(m.getConcreteModelObjects<CoilUserDefined>().empty());
  EXPECT_EQ(0, airLoop.supplyComponents(CoilUserDefined::iddObjectType()).size());
  EXPECT_EQ(0, plantLoop.demandComponents(CoilUserDefined::iddObjectType()).size());
}

// Add ONLY to PlantLoop
// Test CoilUserDefined::remove and make sure that the plant is intact
TEST_F(ModelFixture, CoilUserDefined_remove2) {
  Model m;
  CoilUserDefined coil(m);

  PlantLoop plant(m);
  plant.addDemandBranchForComponent(coil);
  EXPECT_EQ(1, m.getConcreteModelObjects<CoilUserDefined>().size());
  EXPECT_EQ(1, plant.demandComponents(CoilUserDefined::iddObjectType()).size());

  coil.remove();
  EXPECT_TRUE(plant.demandComponents(CoilUserDefined::iddObjectType()).empty());
  EXPECT_TRUE(m.getConcreteModelObjects<CoilUserDefined>().empty());
}

// Add ONLY to PlantLoop
// This time use removeDemandBranchWithComponent
TEST_F(ModelFixture, CoilUserDefined_remove3) {
  Model m;
  CoilUserDefined coil(m);

  PlantLoop plant(m);
  plant.addDemandBranchForComponent(coil);

  EXPECT_EQ(1, m.getConcreteModelObjects<CoilUserDefined>().size());
  EXPECT_EQ(1, plant.demandComponents(CoilUserDefined::iddObjectType()).size());

  plant.removeDemandBranchWithComponent(coil);
  EXPECT_TRUE(plant.demandComponents(CoilUserDefined::iddObjectType()).empty());
  EXPECT_TRUE(m.getConcreteModelObjects<CoilUserDefined>().empty());
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

  EXPECT_EQ(3, airLoop.supplyComponents().size());
  EXPECT_EQ(1, m.getConcreteModelObjects<CoilUserDefined>().size());
  EXPECT_EQ(1, plant.demandComponents(CoilUserDefined::iddObjectType()).size());

  plant.removeDemandBranchWithComponent(coil);
  EXPECT_TRUE(plant.demandComponents(CoilUserDefined::iddObjectType()).empty());
  EXPECT_EQ(3, airLoop.supplyComponents().size());
  EXPECT_EQ(1, m.getConcreteModelObjects<CoilUserDefined>().size());
}

TEST_F(ModelFixture, CoilUserDefined_Clone) {
  Model m;
  Model m2;

  // Make a coil and fill out every field
  CoilUserDefined coil(m);

  EXPECT_EQ(13, m.numObjects());
  EXPECT_EQ(0, m2.numObjects());
  PlantLoop plantLoop(m);
  AirLoopHVAC airLoop(m);
  EXPECT_TRUE(plantLoop.addDemandBranchForComponent(coil));
  Node supplyOutletNode = airLoop.supplyOutletNode();
  EXPECT_TRUE(coil.addToNode(supplyOutletNode));
  ThermalZone z(m);
  EXPECT_TRUE(coil.setAmbientZone(z));

  const int numObjects = m.numObjects();
  EXPECT_EQ(0, m2.numObjects());

  EXPECT_TRUE(coil.airLoopHVAC());
  EXPECT_TRUE(coil.airInletModelObject());
  EXPECT_TRUE(coil.airOutletModelObject());
  EXPECT_TRUE(coil.plantLoop());
  EXPECT_TRUE(coil.waterInletModelObject());
  EXPECT_TRUE(coil.waterOutletModelObject());

  {
    auto clonedCoil = coil.clone(m2).cast<CoilUserDefined>();

    EXPECT_FALSE(clonedCoil.airLoopHVAC());
    EXPECT_FALSE(clonedCoil.airInletModelObject());
    EXPECT_FALSE(clonedCoil.airOutletModelObject());
    EXPECT_FALSE(clonedCoil.plantLoop());
    EXPECT_FALSE(clonedCoil.waterInletModelObject());
    EXPECT_FALSE(clonedCoil.waterOutletModelObject());

    EXPECT_EQ(13, m2.numObjects());

    EXPECT_NE(coil.handle(), clonedCoil.handle());
    EXPECT_NE(coil.overallModelSimulationProgramCallingManager(), clonedCoil.overallModelSimulationProgramCallingManager());
    EXPECT_NE(coil.modelSetupandSizingProgramCallingManager(), clonedCoil.modelSetupandSizingProgramCallingManager());
    EXPECT_NE(coil.overallSimulationProgram(), clonedCoil.overallSimulationProgram());
    EXPECT_NE(coil.initializationSimulationProgram(), clonedCoil.initializationSimulationProgram());
    EXPECT_NE(coil.airOutletTemperatureActuator(), clonedCoil.airOutletTemperatureActuator());
    EXPECT_NE(coil.airOutletHumidityRatioActuator(), clonedCoil.airOutletHumidityRatioActuator());
    EXPECT_NE(coil.airMassFlowRateActuator(), clonedCoil.airMassFlowRateActuator());
    EXPECT_NE(coil.plantMinimumMassFlowRateActuator(), clonedCoil.plantMinimumMassFlowRateActuator());
    EXPECT_NE(coil.plantMaximumMassFlowRateActuator(), clonedCoil.plantMaximumMassFlowRateActuator());
    EXPECT_NE(coil.plantDesignVolumeFlowRateActuator(), clonedCoil.plantDesignVolumeFlowRateActuator());
    EXPECT_NE(coil.plantMassFlowRateActuator(), clonedCoil.plantMassFlowRateActuator());
    EXPECT_NE(coil.plantOutletTemperatureActuator(), clonedCoil.plantOutletTemperatureActuator());
    EXPECT_FALSE(clonedCoil.ambientZone());

    ASSERT_TRUE(clonedCoil.airOutletTemperatureActuator().actuatedComponent());
    EXPECT_EQ(clonedCoil, clonedCoil.airOutletTemperatureActuator().actuatedComponent().get());
    ASSERT_TRUE(clonedCoil.airOutletHumidityRatioActuator().actuatedComponent());
    EXPECT_EQ(clonedCoil, clonedCoil.airOutletHumidityRatioActuator().actuatedComponent().get());
    ASSERT_TRUE(clonedCoil.airMassFlowRateActuator().actuatedComponent());
    EXPECT_EQ(clonedCoil, clonedCoil.airMassFlowRateActuator().actuatedComponent().get());
    ASSERT_TRUE(clonedCoil.plantMinimumMassFlowRateActuator().actuatedComponent());
    EXPECT_EQ(clonedCoil, clonedCoil.plantMinimumMassFlowRateActuator().actuatedComponent().get());
    ASSERT_TRUE(clonedCoil.plantMaximumMassFlowRateActuator().actuatedComponent());
    EXPECT_EQ(clonedCoil, clonedCoil.plantMaximumMassFlowRateActuator().actuatedComponent().get());
    ASSERT_TRUE(clonedCoil.plantDesignVolumeFlowRateActuator().actuatedComponent());
    EXPECT_EQ(clonedCoil, clonedCoil.plantDesignVolumeFlowRateActuator().actuatedComponent().get());
    ASSERT_TRUE(clonedCoil.plantMassFlowRateActuator().actuatedComponent());
    EXPECT_EQ(clonedCoil, clonedCoil.plantMassFlowRateActuator().actuatedComponent().get());
    ASSERT_TRUE(clonedCoil.plantOutletTemperatureActuator().actuatedComponent());
    EXPECT_EQ(clonedCoil, clonedCoil.plantOutletTemperatureActuator().actuatedComponent().get());
  }

  {
    // Same model
    auto clonedCoil = coil.clone(m).cast<CoilUserDefined>();

    EXPECT_FALSE(clonedCoil.airLoopHVAC());
    EXPECT_FALSE(clonedCoil.airInletModelObject());
    EXPECT_FALSE(clonedCoil.airOutletModelObject());
    EXPECT_FALSE(clonedCoil.plantLoop());
    EXPECT_FALSE(clonedCoil.waterInletModelObject());
    EXPECT_FALSE(clonedCoil.waterOutletModelObject());

    EXPECT_EQ(numObjects + 13, m.numObjects());

    EXPECT_NE(coil.handle(), clonedCoil.handle());
    EXPECT_NE(coil.overallModelSimulationProgramCallingManager(), clonedCoil.overallModelSimulationProgramCallingManager());
    EXPECT_NE(coil.modelSetupandSizingProgramCallingManager(), clonedCoil.modelSetupandSizingProgramCallingManager());
    EXPECT_NE(coil.overallSimulationProgram(), clonedCoil.overallSimulationProgram());
    EXPECT_NE(coil.initializationSimulationProgram(), clonedCoil.initializationSimulationProgram());
    EXPECT_NE(coil.airOutletTemperatureActuator(), clonedCoil.airOutletTemperatureActuator());
    EXPECT_NE(coil.airOutletHumidityRatioActuator(), clonedCoil.airOutletHumidityRatioActuator());
    EXPECT_NE(coil.airMassFlowRateActuator(), clonedCoil.airMassFlowRateActuator());
    EXPECT_NE(coil.plantMinimumMassFlowRateActuator(), clonedCoil.plantMinimumMassFlowRateActuator());
    EXPECT_NE(coil.plantMaximumMassFlowRateActuator(), clonedCoil.plantMaximumMassFlowRateActuator());
    EXPECT_NE(coil.plantDesignVolumeFlowRateActuator(), clonedCoil.plantDesignVolumeFlowRateActuator());
    EXPECT_NE(coil.plantMassFlowRateActuator(), clonedCoil.plantMassFlowRateActuator());
    EXPECT_NE(coil.plantOutletTemperatureActuator(), clonedCoil.plantOutletTemperatureActuator());
    EXPECT_FALSE(clonedCoil.ambientZone());

    ASSERT_TRUE(clonedCoil.airOutletTemperatureActuator().actuatedComponent());
    EXPECT_EQ(clonedCoil, clonedCoil.airOutletTemperatureActuator().actuatedComponent().get());
    ASSERT_TRUE(clonedCoil.airOutletHumidityRatioActuator().actuatedComponent());
    EXPECT_EQ(clonedCoil, clonedCoil.airOutletHumidityRatioActuator().actuatedComponent().get());
    ASSERT_TRUE(clonedCoil.airMassFlowRateActuator().actuatedComponent());
    EXPECT_EQ(clonedCoil, clonedCoil.airMassFlowRateActuator().actuatedComponent().get());
    ASSERT_TRUE(clonedCoil.plantMinimumMassFlowRateActuator().actuatedComponent());
    EXPECT_EQ(clonedCoil, clonedCoil.plantMinimumMassFlowRateActuator().actuatedComponent().get());
    ASSERT_TRUE(clonedCoil.plantMaximumMassFlowRateActuator().actuatedComponent());
    EXPECT_EQ(clonedCoil, clonedCoil.plantMaximumMassFlowRateActuator().actuatedComponent().get());
    ASSERT_TRUE(clonedCoil.plantDesignVolumeFlowRateActuator().actuatedComponent());
    EXPECT_EQ(clonedCoil, clonedCoil.plantDesignVolumeFlowRateActuator().actuatedComponent().get());
    ASSERT_TRUE(clonedCoil.plantMassFlowRateActuator().actuatedComponent());
    EXPECT_EQ(clonedCoil, clonedCoil.plantMassFlowRateActuator().actuatedComponent().get());
    ASSERT_TRUE(clonedCoil.plantOutletTemperatureActuator().actuatedComponent());
    EXPECT_EQ(clonedCoil, clonedCoil.plantOutletTemperatureActuator().actuatedComponent().get());
  }
}

TEST_F(ModelFixture, CoilUserDefined_containingComponent) {

  Model m;

  {
    CoilUserDefined coil(m);
    EXPECT_EQ(0, coil.numberofAirConnections());

    AirLoopHVACUnitarySystem unitary(m);
    EXPECT_TRUE(unitary.setCoolingCoil(coil));
    EXPECT_EQ(1, coil.numberofAirConnections());
    boost::optional<HVACComponent> _c = coil.containingHVACComponent();
    ASSERT_TRUE(_c);
    EXPECT_EQ(_c->handle(), unitary.handle());
  }

  {
    CoilUserDefined coil(m);
    EXPECT_EQ(0, coil.numberofAirConnections());

    AirLoopHVACUnitarySystem unitary(m);
    EXPECT_TRUE(unitary.setHeatingCoil(coil));
    EXPECT_EQ(1, coil.numberofAirConnections());
    boost::optional<HVACComponent> _c = coil.containingHVACComponent();
    ASSERT_TRUE(_c);
    EXPECT_EQ(_c->handle(), unitary.handle());
  }

  {
    CoilUserDefined coil(m);
    EXPECT_EQ(0, coil.numberofAirConnections());

    AirLoopHVACUnitarySystem unitary(m);
    EXPECT_TRUE(unitary.setSupplementalHeatingCoil(coil));
    EXPECT_EQ(1, coil.numberofAirConnections());
    boost::optional<HVACComponent> _c = coil.containingHVACComponent();
    ASSERT_TRUE(_c);
    EXPECT_EQ(_c->handle(), unitary.handle());
  }
}
