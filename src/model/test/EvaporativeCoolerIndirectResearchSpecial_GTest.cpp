/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"
#include "../EvaporativeCoolerIndirectResearchSpecial.hpp"
#include "../EvaporativeCoolerIndirectResearchSpecial_Impl.hpp"
#include "../PlantLoop.hpp"
#include "../PlantLoop_Impl.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, EvaporativeCoolerIndirectResearchSpecial) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model m;
      EvaporativeCoolerIndirectResearchSpecial idec(m);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");

  {
    Model m;
    AirLoopHVAC airLoopHVAC(m);
    EvaporativeCoolerIndirectResearchSpecial idec(m);

    Node supplyOutletNode = airLoopHVAC.supplyOutletNode();
    EXPECT_TRUE(idec.addToNode(supplyOutletNode));

    EXPECT_EQ(3u, airLoopHVAC.supplyComponents().size());

    Node demandInletNode = airLoopHVAC.demandInletNode();
    EXPECT_FALSE(idec.addToNode(demandInletNode));

    EXPECT_EQ(3u, airLoopHVAC.supplyComponents().size());

    Node node(m);
    EXPECT_TRUE(idec.getImpl<model::detail::EvaporativeCoolerIndirectResearchSpecial_Impl>()->setReliefAirInletNode(node));

    ASSERT_TRUE(idec.getImpl<model::detail::EvaporativeCoolerIndirectResearchSpecial_Impl>()->reliefAirInletNode());
    EXPECT_EQ(node, idec.getImpl<model::detail::EvaporativeCoolerIndirectResearchSpecial_Impl>()->reliefAirInletNode().get());
  }

  {
    Model m;
    PlantLoop plant(m);

    EvaporativeCoolerIndirectResearchSpecial idec(m);
    Node node = plant.supplyOutletNode();
    EXPECT_FALSE(idec.addToNode(node));
  }
}
