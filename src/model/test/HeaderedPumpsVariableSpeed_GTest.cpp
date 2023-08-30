/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../HeaderedPumpsVariableSpeed.hpp"
#include "../HeaderedPumpsVariableSpeed_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, HeaderedPumpsVariableSpeed) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model m;
      HeaderedPumpsVariableSpeed pump(m);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");
}

TEST_F(ModelFixture, HeaderedPumpsVariableSpeed_DesignFields) {

  Model m;
  HeaderedPumpsVariableSpeed p(m);

  // Check defaults
  EXPECT_EQ("PowerPerFlowPerPressure", p.designPowerSizingMethod());
  EXPECT_EQ(348701.1, p.designElectricPowerPerUnitFlowRate());
  EXPECT_EQ(1.282051282, p.designShaftPowerPerUnitFlowRatePerUnitHead());
  EXPECT_EQ("General", p.endUseSubcategory());

  EXPECT_TRUE(p.setDesignPowerSizingMethod("PowerPerFlow"));
  EXPECT_EQ("PowerPerFlow", p.designPowerSizingMethod());
  EXPECT_FALSE(p.setDesignPowerSizingMethod("ABADVALUE"));
  EXPECT_EQ("PowerPerFlow", p.designPowerSizingMethod());

  EXPECT_TRUE(p.setDesignElectricPowerPerUnitFlowRate(350000.0));
  EXPECT_EQ(350000.0, p.designElectricPowerPerUnitFlowRate());

  EXPECT_TRUE(p.setDesignShaftPowerPerUnitFlowRatePerUnitHead(1.1));
  EXPECT_EQ(1.1, p.designShaftPowerPerUnitFlowRatePerUnitHead());

  EXPECT_TRUE(p.setEndUseSubcategory("Pumps"));
  EXPECT_EQ("Pumps", p.endUseSubcategory());
}
