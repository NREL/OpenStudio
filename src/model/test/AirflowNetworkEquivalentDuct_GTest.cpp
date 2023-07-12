/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../Node.hpp"
#include "../Node_Impl.hpp"
#include "../CoilCoolingDXSingleSpeed.hpp"
#include "../CoilCoolingDXSingleSpeed_Impl.hpp"
#include "../AirflowNetworkDistributionNode.hpp"
#include "../AirflowNetworkDistributionNode_Impl.hpp"
#include "../AirflowNetworkDistributionLinkage.hpp"
#include "../AirflowNetworkDistributionLinkage_Impl.hpp"
#include "../AirflowNetworkEquivalentDuct.hpp"
#include "../AirflowNetworkEquivalentDuct_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, AirflowNetwork_EquivalentDuct) {
  Model model;

  Node node0(model);
  Node node1(model);

  AirflowNetworkDistributionNode afnnode0 = node0.getAirflowNetworkDistributionNode();
  AirflowNetworkDistributionNode afnnode1 = node1.getAirflowNetworkDistributionNode();

  CoilCoolingDXSingleSpeed coilCooling(model);

  AirflowNetworkEquivalentDuct coolingComponent = coilCooling.getAirflowNetworkEquivalentDuct(0.1, 1.0);

  boost::optional<AirflowNetworkEquivalentDuct> opteqd = coilCooling.airflowNetworkEquivalentDuct();

  ASSERT_TRUE(opteqd);
  EXPECT_EQ(coolingComponent, opteqd.get());

  AirflowNetworkDistributionLinkage link(model, afnnode0, afnnode1, coolingComponent);

  // Test component here?

  EXPECT_TRUE(link.setComponent(coolingComponent));
  EXPECT_TRUE(link.component().componentModelObject());
}
