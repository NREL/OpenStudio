/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../ControllerWaterCoil.hpp"
#include "../ControllerWaterCoil_Impl.hpp"
#include "../Model.hpp"
#include "../AirLoopHVAC.hpp"
#include "../PlantLoop.hpp"
#include "../CoilCoolingWater.hpp"
#include "../CoilHeatingWater.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, ControllerWaterCoil_ControllerWaterCoil) {

  Model m;

  CoilCoolingWater c(m);
  EXPECT_FALSE(c.controllerWaterCoil());

  PlantLoop p(m);
  EXPECT_TRUE(p.addDemandBranchForComponent(c));
  AirLoopHVAC a(m);
  Node supplyOutletNode = a.supplyOutletNode();
  EXPECT_TRUE(c.addToNode(supplyOutletNode));

  ASSERT_TRUE(c.controllerWaterCoil());
  ControllerWaterCoil controllerWaterCoil = c.controllerWaterCoil().get();
  EXPECT_TRUE(controllerWaterCoil.waterCoil());
  EXPECT_EQ(c, controllerWaterCoil.waterCoil().get());

  // Control Variable: Optional String
  EXPECT_TRUE(controllerWaterCoil.isControlVariableDefaulted());
  EXPECT_TRUE(controllerWaterCoil.controlVariable());  // This shouldn't return an optional...
  EXPECT_EQ("Temperature", controllerWaterCoil.controlVariable().get());
  EXPECT_TRUE(controllerWaterCoil.setControlVariable("TemperatureAndHumidityRatio"));
  EXPECT_EQ("TemperatureAndHumidityRatio", controllerWaterCoil.controlVariable().get());
  EXPECT_FALSE(controllerWaterCoil.isControlVariableDefaulted());
  // Bad Value
  EXPECT_FALSE(controllerWaterCoil.setControlVariable("BADENUM"));
  EXPECT_EQ("TemperatureAndHumidityRatio", controllerWaterCoil.controlVariable().get());
  controllerWaterCoil.resetControlVariable();
  EXPECT_EQ("Temperature", controllerWaterCoil.controlVariable().get());
  EXPECT_TRUE(controllerWaterCoil.isControlVariableDefaulted());

  // Action: Optional String
  // This is from a coil cooling water, so Reverse
  ASSERT_TRUE(controllerWaterCoil.action());
  EXPECT_EQ("Reverse", controllerWaterCoil.action().get());
  EXPECT_TRUE(controllerWaterCoil.setAction("Normal"));
  ASSERT_TRUE(controllerWaterCoil.action());
  EXPECT_EQ("Normal", controllerWaterCoil.action().get());
  // Bad Value
  EXPECT_FALSE(controllerWaterCoil.setAction("BADENUM"));
  ASSERT_TRUE(controllerWaterCoil.action());
  EXPECT_EQ("Normal", controllerWaterCoil.action().get());

  // Actuator Variable: Optional String
  EXPECT_TRUE(controllerWaterCoil.isActuatorVariableDefaulted());
  EXPECT_TRUE(controllerWaterCoil.actuatorVariable());  // This shouldn't return an optional...
  EXPECT_EQ("Flow", controllerWaterCoil.actuatorVariable().get());
  EXPECT_TRUE(controllerWaterCoil.setActuatorVariable("Flow"));  // There's a single choice...
  EXPECT_EQ("Flow", controllerWaterCoil.actuatorVariable().get());
  EXPECT_FALSE(controllerWaterCoil.isActuatorVariableDefaulted());
  // Bad Value
  EXPECT_FALSE(controllerWaterCoil.setActuatorVariable("BADENUM"));
  EXPECT_EQ("Flow", controllerWaterCoil.actuatorVariable().get());
  controllerWaterCoil.resetActuatorVariable();
  EXPECT_EQ("Flow", controllerWaterCoil.actuatorVariable().get());
  EXPECT_TRUE(controllerWaterCoil.isActuatorVariableDefaulted());

  // Sensor Node Name: Optional Object
  EXPECT_FALSE(controllerWaterCoil.sensorNode());
  // Typically this would be airOutletNode
  auto airOutletNode = c.airOutletModelObject()->cast<Node>();
  // TODO: This should maybe catch whether the node makes sense or not (is it on a loop that's the coil is connected to?)
  EXPECT_TRUE(controllerWaterCoil.setSensorNode(airOutletNode));
  ASSERT_TRUE(controllerWaterCoil.sensorNode());
  EXPECT_EQ(airOutletNode, controllerWaterCoil.sensorNode().get());

  // Actuator Node Name: Optional Object
  EXPECT_FALSE(controllerWaterCoil.actuatorNode());
  // Typically the WaterInletNode
  auto waterInletNode = c.waterInletModelObject()->cast<Node>();
  EXPECT_TRUE(controllerWaterCoil.setActuatorNode(waterInletNode));
  ASSERT_TRUE(controllerWaterCoil.actuatorNode());
  EXPECT_EQ(waterInletNode, controllerWaterCoil.actuatorNode().get());

  // Controller Convergence Tolerance: Optional Double, autosizable
  EXPECT_TRUE(controllerWaterCoil.isControllerConvergenceToleranceDefaulted());
  EXPECT_TRUE(controllerWaterCoil.isControllerConvergenceToleranceAutosized());
  EXPECT_TRUE(controllerWaterCoil.setControllerConvergenceTolerance(3.0));
  ASSERT_TRUE(controllerWaterCoil.controllerConvergenceTolerance());
  EXPECT_EQ(3.0, controllerWaterCoil.controllerConvergenceTolerance().get());
  EXPECT_FALSE(controllerWaterCoil.isControllerConvergenceToleranceAutosized());
  EXPECT_FALSE(controllerWaterCoil.isControllerConvergenceToleranceDefaulted());
  controllerWaterCoil.autosizeControllerConvergenceTolerance();
  EXPECT_TRUE(controllerWaterCoil.isControllerConvergenceToleranceAutosized());
  EXPECT_FALSE(controllerWaterCoil.isControllerConvergenceToleranceDefaulted());
  controllerWaterCoil.resetControllerConvergenceTolerance();
  EXPECT_TRUE(controllerWaterCoil.isControllerConvergenceToleranceAutosized());
  EXPECT_TRUE(controllerWaterCoil.isControllerConvergenceToleranceDefaulted());

  // Maximum Actuated Flow: Optional Double, autosizable
  EXPECT_TRUE(controllerWaterCoil.isMaximumActuatedFlowAutosized());
  EXPECT_TRUE(controllerWaterCoil.setMaximumActuatedFlow(0.1));
  ASSERT_TRUE(controllerWaterCoil.maximumActuatedFlow());
  EXPECT_EQ(0.1, controllerWaterCoil.maximumActuatedFlow().get());
  // Bad Value
  EXPECT_FALSE(controllerWaterCoil.setMaximumActuatedFlow(-10.0));
  ASSERT_TRUE(controllerWaterCoil.maximumActuatedFlow());
  EXPECT_EQ(0.1, controllerWaterCoil.maximumActuatedFlow().get());
  EXPECT_FALSE(controllerWaterCoil.isMaximumActuatedFlowAutosized());
  controllerWaterCoil.resetMaximumActuatedFlow();
  EXPECT_TRUE(controllerWaterCoil.isMaximumActuatedFlowAutosized());

  // Minimum Actuated Flow: Optional Double
  EXPECT_TRUE(controllerWaterCoil.isMinimumActuatedFlowDefaulted());
  EXPECT_EQ(0.0, controllerWaterCoil.minimumActuatedFlow());
  EXPECT_TRUE(controllerWaterCoil.setMinimumActuatedFlow(0.1));
  EXPECT_EQ(0.1, controllerWaterCoil.minimumActuatedFlow());
  // Bad Value
  EXPECT_FALSE(controllerWaterCoil.setMinimumActuatedFlow(-10.0));
  EXPECT_EQ(0.1, controllerWaterCoil.minimumActuatedFlow());
}

TEST_F(ModelFixture, ControllerWaterCoil_onLoopAdd) {

  Model m;

  CoilCoolingWater c(m);
  EXPECT_FALSE(c.controllerWaterCoil());

  PlantLoop p(m);
  EXPECT_TRUE(p.addDemandBranchForComponent(c));
  ASSERT_TRUE(c.controllerWaterCoil());
  ControllerWaterCoil controllerWaterCoil = c.controllerWaterCoil().get();
  EXPECT_TRUE(controllerWaterCoil.setControlVariable("TemperatureAndHumidityRatio"));
  EXPECT_EQ("TemperatureAndHumidityRatio", controllerWaterCoil.controlVariable().get());

  AirLoopHVAC a(m);
  Node supplyOutletNode = a.supplyOutletNode();
  EXPECT_TRUE(c.addToNode(supplyOutletNode));
  ASSERT_TRUE(c.controllerWaterCoil());
  ControllerWaterCoil controllerWaterCoil2 = c.controllerWaterCoil().get();
  EXPECT_EQ(controllerWaterCoil, controllerWaterCoil2);
  EXPECT_EQ("TemperatureAndHumidityRatio", controllerWaterCoil2.controlVariable().get());

  PlantLoop p2(m);
  EXPECT_TRUE(p.addDemandBranchForComponent(c));
  EXPECT_EQ(controllerWaterCoil, controllerWaterCoil2);
  EXPECT_EQ("TemperatureAndHumidityRatio", controllerWaterCoil2.controlVariable().get());
}

TEST_F(ModelFixture, ControllerWaterCoil_onLoopAdd_HC) {

  Model m;

  CoilHeatingWater c(m);
  EXPECT_FALSE(c.controllerWaterCoil());

  PlantLoop p(m);
  EXPECT_TRUE(p.addDemandBranchForComponent(c));
  ASSERT_TRUE(c.controllerWaterCoil());
  ControllerWaterCoil controllerWaterCoil = c.controllerWaterCoil().get();

  // This is from a coil **heating** water, so Normal
  ASSERT_TRUE(controllerWaterCoil.action());
  EXPECT_EQ("Normal", controllerWaterCoil.action().get());

  EXPECT_TRUE(controllerWaterCoil.setControlVariable("TemperatureAndHumidityRatio"));
  EXPECT_EQ("TemperatureAndHumidityRatio", controllerWaterCoil.controlVariable().get());

  AirLoopHVAC a(m);
  Node supplyOutletNode = a.supplyOutletNode();
  EXPECT_TRUE(c.addToNode(supplyOutletNode));
  ASSERT_TRUE(c.controllerWaterCoil());
  ControllerWaterCoil controllerWaterCoil2 = c.controllerWaterCoil().get();
  EXPECT_EQ(controllerWaterCoil, controllerWaterCoil2);
  EXPECT_EQ("TemperatureAndHumidityRatio", controllerWaterCoil2.controlVariable().get());

  PlantLoop p2(m);
  EXPECT_TRUE(p.addDemandBranchForComponent(c));
  EXPECT_EQ(controllerWaterCoil, controllerWaterCoil2);
  EXPECT_EQ("TemperatureAndHumidityRatio", controllerWaterCoil2.controlVariable().get());
}

TEST_F(ModelFixture, ControllerWaterCoil_removeCoil) {
  Model m;

  CoilCoolingWater c(m);
  EXPECT_FALSE(c.controllerWaterCoil());

  PlantLoop p(m);
  EXPECT_TRUE(p.addDemandBranchForComponent(c));
  EXPECT_EQ(1U, m.getConcreteModelObjects<ControllerWaterCoil>().size());

  c.remove();
  EXPECT_EQ(0U, m.getConcreteModelObjects<ControllerWaterCoil>().size());
}
