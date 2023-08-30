/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../Node.hpp"
#include "../Node_Impl.hpp"
#include "../AirflowNetworkDistributionNode.hpp"
#include "../AirflowNetworkDistributionNode_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, AirflowNetwork_DistributionNode) {
  Model model;

  Node node(model);

  AirflowNetworkDistributionNode afnnode(model);

  AirflowNetworkDistributionNode afnnode2 = node.getAirflowNetworkDistributionNode();

  ASSERT_TRUE(afnnode2.node());
  EXPECT_EQ(node, afnnode2.node().get());

  /*
  AirLoopHVAC airloop(model);
  Test other loop-related nodes here...
  */
}
