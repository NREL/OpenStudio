/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../LoadProfilePlant.hpp"
#include "../LoadProfilePlant_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, LoadProfilePlant_LoadProfilePlant) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model m;
      LoadProfilePlant lpp(m);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");

  Model m;
  LoadProfilePlant lpp(m);

  EXPECT_EQ(0.002, lpp.peakFlowRate());
  EXPECT_EQ("Water", lpp.plantLoopFluidType());
  EXPECT_EQ(5.0, lpp.degreeofSubCooling());
  EXPECT_EQ(20.0, lpp.degreeofLoopSubCooling());
}

TEST_F(ModelFixture, LoadProfilePlant_GettersSetters) {
  Model m;
  LoadProfilePlant lpp(m);

  EXPECT_TRUE(lpp.setPeakFlowRate(0.005));
  EXPECT_TRUE(lpp.setPlantLoopFluidType("Steam"));
  EXPECT_TRUE(lpp.setDegreeofSubCooling(6.0));
  EXPECT_TRUE(lpp.setDegreeofLoopSubCooling(21.0));

  EXPECT_EQ(0.005, lpp.peakFlowRate());
  EXPECT_EQ("Steam", lpp.plantLoopFluidType());
  EXPECT_EQ(6.0, lpp.degreeofSubCooling());
  EXPECT_EQ(21.0, lpp.degreeofLoopSubCooling());
}
