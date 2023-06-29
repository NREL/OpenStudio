/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "ModelFixture.hpp"

#include "../PipeIndoor.hpp"
#include "../PipeIndoor_Impl.hpp"

#include "../AirLoopHVACZoneSplitter.hpp"
#include "../Construction.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"
#include "../PlantLoop.hpp"
#include "../StandardOpaqueMaterial.hpp"
#include "../Schedule.hpp"
#include "../ScheduleConstant.hpp"
#include "../ThermalZone.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, PipeIndoor_GettersSetters) {
  Model m;

  PipeIndoor pipeIndoor(m);
  pipeIndoor.setName("My PipeIndoor");

  // Construction: Optional Object
  StandardOpaqueMaterial mat(m, "Smooth", 3.00E-03, 45.31, 7833.0, 500.0);
  EXPECT_TRUE(mat.setThermalAbsorptance(0.9));
  EXPECT_TRUE(mat.setSolarAbsorptance(0.5));
  EXPECT_TRUE(mat.setVisibleAbsorptance(0.5));
  Construction c(m);
  EXPECT_TRUE(c.insertLayer(0, mat));
  EXPECT_TRUE(pipeIndoor.setConstruction(c));
  ASSERT_TRUE(pipeIndoor.construction());
  EXPECT_EQ(c, pipeIndoor.construction().get());

  // Environment Type: Required String
  EXPECT_TRUE(pipeIndoor.setEnvironmentType("Zone"));
  EXPECT_EQ("Zone", pipeIndoor.environmentType());
  // Bad Value
  EXPECT_FALSE(pipeIndoor.setEnvironmentType("BADENUM"));
  EXPECT_EQ("Zone", pipeIndoor.environmentType());

  // Ambient Temperature Zone: Optional Object
  ThermalZone ambientTemperatureZone(m);
  EXPECT_TRUE(pipeIndoor.setAmbientTemperatureZone(ambientTemperatureZone));
  ASSERT_TRUE(pipeIndoor.ambientTemperatureZone());
  EXPECT_EQ(ambientTemperatureZone, pipeIndoor.ambientTemperatureZone().get());

  // Ambient Temperature Schedule: Optional Object
  ScheduleConstant ambientTemperatureSchedule(m);
  EXPECT_TRUE(pipeIndoor.setAmbientTemperatureSchedule(ambientTemperatureSchedule));
  ASSERT_TRUE(pipeIndoor.ambientTemperatureSchedule());
  EXPECT_EQ(ambientTemperatureSchedule, pipeIndoor.ambientTemperatureSchedule().get());

  // Ambient Air Velocity Schedule: Optional Object
  ScheduleConstant ambientAirVelocitySchedule(m);
  EXPECT_TRUE(pipeIndoor.setAmbientAirVelocitySchedule(ambientAirVelocitySchedule));
  ASSERT_TRUE(pipeIndoor.ambientAirVelocitySchedule());
  EXPECT_EQ(ambientAirVelocitySchedule, pipeIndoor.ambientAirVelocitySchedule().get());

  // Pipe Inside Diameter: Required Double
  EXPECT_TRUE(pipeIndoor.setPipeInsideDiameter(1.0));
  EXPECT_EQ(1.0, pipeIndoor.pipeInsideDiameter());
  // Bad Value
  EXPECT_FALSE(pipeIndoor.setPipeInsideDiameter(-10.0));
  EXPECT_EQ(1.0, pipeIndoor.pipeInsideDiameter());

  // Pipe Length: Required Double
  EXPECT_TRUE(pipeIndoor.setPipeLength(50.0));
  EXPECT_EQ(50.0, pipeIndoor.pipeLength());
  // Bad Value
  EXPECT_FALSE(pipeIndoor.setPipeLength(-10.0));
  EXPECT_EQ(50.0, pipeIndoor.pipeLength());
}

TEST_F(ModelFixture, PipeIndoor_addToNode) {
  Model m;
  PipeIndoor pipeIndoor(m);

  AirLoopHVAC airLoop(m);

  Node supplyOutletNode = airLoop.supplyOutletNode();

  EXPECT_FALSE(pipeIndoor.addToNode(supplyOutletNode));
  EXPECT_EQ((unsigned)2, airLoop.supplyComponents().size());

  Node inletNode = airLoop.zoneSplitter().lastOutletModelObject()->cast<Node>();

  EXPECT_FALSE(pipeIndoor.addToNode(inletNode));
  EXPECT_EQ((unsigned)5, airLoop.demandComponents().size());

  PlantLoop plantLoop(m);
  supplyOutletNode = plantLoop.supplyOutletNode();
  EXPECT_TRUE(pipeIndoor.addToNode(supplyOutletNode));
  EXPECT_EQ((unsigned)7, plantLoop.supplyComponents().size());

  Node demandOutletNode = plantLoop.demandOutletNode();
  EXPECT_TRUE(pipeIndoor.addToNode(demandOutletNode));
  EXPECT_EQ((unsigned)7, plantLoop.demandComponents().size());

  PipeIndoor pipeIndoor2(m);

  EXPECT_TRUE(pipeIndoor2.addToNode(demandOutletNode));
  EXPECT_EQ((unsigned)9, plantLoop.demandComponents().size());

  PlantLoop plantLoop2(m);
  demandOutletNode = plantLoop2.demandOutletNode();
  EXPECT_TRUE(pipeIndoor.addToNode(demandOutletNode));
  EXPECT_EQ((unsigned)7, plantLoop2.demandComponents().size());

  auto pipeIndoorClone = pipeIndoor.clone(m).cast<PipeIndoor>();
  supplyOutletNode = plantLoop.supplyOutletNode();

  EXPECT_TRUE(pipeIndoorClone.addToNode(supplyOutletNode));
  EXPECT_EQ((unsigned)7, plantLoop.supplyComponents().size());
}

TEST_F(ModelFixture, PipeIndoor_HeatCoolFuelTypes) {
  Model m;

  PipeIndoor pipeIndoor(m);

  EXPECT_EQ(ComponentType(ComponentType::None), pipeIndoor.componentType());
  testFuelTypeEquality({}, pipeIndoor.coolingFuelTypes());
  testFuelTypeEquality({}, pipeIndoor.heatingFuelTypes());
  testAppGFuelTypeEquality({}, pipeIndoor.appGHeatingFuelTypes());
}
