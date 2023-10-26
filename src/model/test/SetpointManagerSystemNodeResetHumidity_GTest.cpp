/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../SetpointManagerSystemNodeResetHumidity.hpp"
#include "../SetpointManagerSystemNodeResetHumidity_Impl.hpp"

#include "../Model.hpp"
#include "../Node.hpp"
#include "../AirLoopHVAC.hpp"
#include "../PlantLoop.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, SetpointManagerSystemNodeResetHumidity_GettersSetters) {
  Model m;
  SetpointManagerSystemNodeResetHumidity spm(m);

  spm.setName("My SetpointManagerSystemNodeResetHumidity");

  // Control Variable: Required String
  EXPECT_TRUE(spm.setControlVariable("HumidityRatio"));
  EXPECT_EQ("HumidityRatio", spm.controlVariable());
  // Bad Value
  EXPECT_FALSE(spm.setControlVariable("BADENUM"));
  EXPECT_EQ("HumidityRatio", spm.controlVariable());

  // Setpoint at Low Reference Humidity Ratio: Required Double
  EXPECT_TRUE(spm.setSetpointatLowReferenceHumidityRatio(0.4));
  EXPECT_EQ(0.4, spm.setpointatLowReferenceHumidityRatio());

  // Setpoint at High Reference Humidity Ratio: Required Double
  EXPECT_TRUE(spm.setSetpointatHighReferenceHumidityRatio(0.5));
  EXPECT_EQ(0.5, spm.setpointatHighReferenceHumidityRatio());

  // Low Reference Humidity Ratio: Required Double
  EXPECT_TRUE(spm.setLowReferenceHumidityRatio(0.6));
  EXPECT_EQ(0.6, spm.lowReferenceHumidityRatio());

  // High Reference Humidity Ratio: Required Double
  EXPECT_TRUE(spm.setHighReferenceHumidityRatio(0.7));
  EXPECT_EQ(0.7, spm.highReferenceHumidityRatio());
}

TEST_F(ModelFixture, SetpointManagerSystemNodeResetHumidity_addToNode) {

  Model m;

  // AirLoopHVAC
  {
    SetpointManagerSystemNodeResetHumidity spm(m);
    AirLoopHVAC a(m);
    Node supplyOutlet = a.supplyOutletNode();
    Node supplyInlet = a.supplyInletNode();
    Node demandInlet = a.demandInletNode();

    EXPECT_FALSE(spm.setpointNode());
    EXPECT_FALSE(spm.referenceNode());

    EXPECT_TRUE(spm.addToNode(supplyOutlet));
    ASSERT_TRUE(spm.setpointNode());
    EXPECT_EQ(supplyOutlet, spm.setpointNode().get());
    EXPECT_FALSE(spm.referenceNode());

    EXPECT_TRUE(spm.setReferenceNode(supplyInlet));
    ASSERT_TRUE(spm.setpointNode());
    EXPECT_EQ(supplyOutlet, spm.setpointNode().get());
    ASSERT_TRUE(spm.referenceNode());
    EXPECT_EQ(supplyInlet, spm.referenceNode().get());

    EXPECT_FALSE(spm.addToNode(demandInlet));
    ASSERT_TRUE(spm.setpointNode());
    EXPECT_EQ(supplyOutlet, spm.setpointNode().get());
    ASSERT_TRUE(spm.referenceNode());
    EXPECT_EQ(supplyInlet, spm.referenceNode().get());

    EXPECT_TRUE(spm.setReferenceNode(demandInlet));
    ASSERT_TRUE(spm.setpointNode());
    EXPECT_EQ(supplyOutlet, spm.setpointNode().get());
    ASSERT_TRUE(spm.referenceNode());
    EXPECT_EQ(demandInlet, spm.referenceNode().get());

    EXPECT_TRUE(spm.setControlVariable("MinimumHumidityRatio"));
    ASSERT_EQ(1, supplyOutlet.setpointManagers().size());
    EXPECT_EQ(spm, supplyOutlet.setpointManagers().front());

    // Add a SPM with the same control variable => the first one is removed
    SetpointManagerSystemNodeResetHumidity spm2(m);
    EXPECT_TRUE(spm2.setControlVariable("MinimumHumidityRatio"));
    EXPECT_TRUE(spm2.addToNode(supplyOutlet));
    ASSERT_TRUE(spm2.setpointNode());
    EXPECT_EQ(supplyOutlet, spm2.setpointNode().get());
    ASSERT_EQ(1, supplyOutlet.setpointManagers().size());
    EXPECT_EQ(spm2, supplyOutlet.setpointManagers().front());

    SetpointManagerSystemNodeResetHumidity spm3(m);
    EXPECT_TRUE(spm3.setControlVariable("MaximumHumidityRatio"));
    EXPECT_TRUE(spm3.addToNode(supplyOutlet));
    ASSERT_TRUE(spm3.setpointNode());
    EXPECT_EQ(supplyOutlet, spm3.setpointNode().get());
    ASSERT_EQ(2, supplyOutlet.setpointManagers().size());
  }

  // PlantLoop: **NOT** allowed
  {
    SetpointManagerSystemNodeResetHumidity spm(m);
    PlantLoop p(m);
    Node supplyOutlet = p.supplyOutletNode();
    Node supplyInlet = p.supplyInletNode();
    Node demandInlet = p.demandInletNode();
    std::vector<Node> testNodes{supplyOutlet, supplyInlet, demandInlet};

    EXPECT_FALSE(spm.setpointNode());
    EXPECT_FALSE(spm.referenceNode());

    for (auto& node : testNodes) {
      EXPECT_FALSE(spm.addToNode(node));
      EXPECT_FALSE(spm.setReferenceNode(node));

      EXPECT_FALSE(spm.setpointNode());
      EXPECT_FALSE(spm.referenceNode());
    }
  }
}

TEST_F(ModelFixture, SetpointManagerSystemNodeResetHumidity_remove) {
  Model m;
  AirLoopHVAC a(m);
  Node supplyOutlet = a.supplyOutletNode();

  SetpointManagerSystemNodeResetHumidity spm(m);

  EXPECT_TRUE(spm.setControlVariable("MinimumHumidityRatio"));

  EXPECT_TRUE(spm.addToNode(supplyOutlet));

  EXPECT_EQ(1, supplyOutlet.setpointManagers().size());
  EXPECT_EQ(1, m.getConcreteModelObjects<SetpointManagerSystemNodeResetHumidity>().size());

  spm.remove();

  EXPECT_EQ(0, supplyOutlet.setpointManagers().size());
  EXPECT_EQ(0, m.getConcreteModelObjects<SetpointManagerSystemNodeResetHumidity>().size());
}

TEST_F(ModelFixture, SetpointManagerSystemNodeResetHumidity_clone) {
  Model m;
  AirLoopHVAC a(m);
  Node supplyOutlet = a.supplyOutletNode();
  Node supplyInlet = a.supplyInletNode();

  SetpointManagerSystemNodeResetHumidity spm(m);

  EXPECT_TRUE(spm.setControlVariable("MinimumHumidityRatio"));
  EXPECT_TRUE(spm.setSetpointatLowReferenceHumidityRatio(0.002));
  EXPECT_TRUE(spm.setSetpointatHighReferenceHumidityRatio(0.001));
  EXPECT_TRUE(spm.setLowReferenceHumidityRatio(0.004));
  EXPECT_TRUE(spm.setHighReferenceHumidityRatio(0.003));

  EXPECT_TRUE(spm.addToNode(supplyOutlet));
  EXPECT_TRUE(spm.setReferenceNode(supplyInlet));
  ASSERT_TRUE(spm.setpointNode());
  EXPECT_EQ(supplyOutlet, spm.setpointNode().get());
  ASSERT_TRUE(spm.referenceNode());
  EXPECT_EQ(supplyInlet, spm.referenceNode().get());

  auto spmClone = spm.clone(m).cast<SetpointManagerSystemNodeResetHumidity>();
  EXPECT_FALSE(spmClone.setpointNode());
  EXPECT_FALSE(spmClone.referenceNode());

  EXPECT_EQ(spm.controlVariable(), spmClone.controlVariable());
  EXPECT_EQ(spm.setpointatLowReferenceHumidityRatio(), spmClone.setpointatLowReferenceHumidityRatio());
  EXPECT_EQ(spm.setpointatHighReferenceHumidityRatio(), spmClone.setpointatHighReferenceHumidityRatio());
  EXPECT_EQ(spm.lowReferenceHumidityRatio(), spmClone.lowReferenceHumidityRatio());
  EXPECT_EQ(spm.highReferenceHumidityRatio(), spmClone.highReferenceHumidityRatio());
}
