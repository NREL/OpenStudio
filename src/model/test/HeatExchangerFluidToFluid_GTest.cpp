/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../HeatExchangerFluidToFluid.hpp"
#include "../HeatExchangerFluidToFluid_Impl.hpp"
#include "../PlantLoop.hpp"
#include "../PlantLoop_Impl.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, HeatExchangerFluidToFluid) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model m;
      HeatExchangerFluidToFluid hx(m);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");

  {
    Model m;
    PlantLoop plant1(m);
    PlantLoop plant2(m);

    HeatExchangerFluidToFluid hx(m);
    Node node = plant1.supplyOutletNode();
    ASSERT_TRUE(hx.addToNode(node));
    ASSERT_TRUE(plant2.addDemandBranchForComponent(hx));
  }

  {
    Model m;
    PlantLoop plant1(m);
    PlantLoop plant2(m);

    HeatExchangerFluidToFluid hx(m);
    Node node = plant1.supplyOutletNode();
    ASSERT_TRUE(hx.addToNode(node));
    node = plant2.supplyOutletNode();
    ASSERT_TRUE(hx.addToNode(node));

    ASSERT_EQ(5u, plant1.supplyComponents().size());
    ASSERT_EQ(7u, plant2.supplyComponents().size());

    hx.remove();
    ASSERT_EQ(5u, plant2.supplyComponents().size());
  }

  {
    Model m;
    AirLoopHVAC airLoopHVAC(m);

    HeatExchangerFluidToFluid hx(m);
    Node node = airLoopHVAC.supplyOutletNode();
    ASSERT_FALSE(hx.addToNode(node));
  }
}
