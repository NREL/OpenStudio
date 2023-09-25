/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include <string>

#include "ModelFixture.hpp"

#include "../Model.hpp"
#include "../Model_Impl.hpp"

#include "../ElectricLoadCenterInverterPVWatts.hpp"
#include "../ElectricLoadCenterInverterPVWatts_Impl.hpp"
#include "../ElectricLoadCenterDistribution.hpp"
#include "../ElectricLoadCenterDistribution_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, ElectricLoadCenterInverterPVWatts_ElectricLoadCenterInverterPVWatts) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      // create a model to use
      Model model;

      // create a foundation kiva object to use
      ElectricLoadCenterInverterPVWatts inverter(model);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");

  // create a model to use
  Model model;

  // create a pvwatts inverter to use
  ElectricLoadCenterInverterPVWatts inverter(model);

  EXPECT_TRUE(inverter.isDCToACSizeRatioDefaulted());
  EXPECT_TRUE(inverter.isInverterEfficiencyDefaulted());

  ASSERT_EQ(1.1, inverter.dcToACSizeRatio());
  ASSERT_EQ(0.96, inverter.inverterEfficiency());
}

// test setting and getting
TEST_F(ModelFixture, ElectricLoadCenterInverterPVWatts_SetGetFields) {
  // create a model to use
  Model model;

  // create a pvwatts inverter object to use
  ElectricLoadCenterInverterPVWatts inverter(model);

  // set the fields
  inverter.setDCToACSizeRatio(1);
  inverter.setInverterEfficiency(1);

  // check the fields
  ASSERT_FALSE(inverter.isDCToACSizeRatioDefaulted());
  ASSERT_EQ(1, inverter.dcToACSizeRatio());
  ASSERT_FALSE(inverter.isInverterEfficiencyDefaulted());
  ASSERT_EQ(1, inverter.inverterEfficiency());

  // reset them one by one
  inverter.resetDCToACSizeRatio();
  inverter.resetInverterEfficiency();

  EXPECT_TRUE(inverter.isDCToACSizeRatioDefaulted());
  EXPECT_TRUE(inverter.isInverterEfficiencyDefaulted());
}

// test cloning it
TEST_F(ModelFixture, ElectricLoadCenterInverterPVWatts_Clone) {
  Model model;
  ElectricLoadCenterInverterPVWatts inverter(model);
  inverter.setInverterEfficiency(0.99);

  // clone it into the same model
  auto inverterClone = inverter.clone(model).cast<ElectricLoadCenterInverterPVWatts>();
  ASSERT_FALSE(inverterClone.isInverterEfficiencyDefaulted());
  ASSERT_EQ(0.99, inverterClone.inverterEfficiency());
  ASSERT_TRUE(inverterClone.isDCToACSizeRatioDefaulted());

  // clone it into a different model
  Model model2;
  auto inverterClone2 = inverter.clone(model).cast<ElectricLoadCenterInverterPVWatts>();
  ASSERT_FALSE(inverterClone2.isInverterEfficiencyDefaulted());
  ASSERT_EQ(0.99, inverterClone2.inverterEfficiency());
  ASSERT_TRUE(inverterClone2.isDCToACSizeRatioDefaulted());
}

// test that remove works
TEST_F(ModelFixture, ElectricLoadCenterInverterPVWatts_Remove) {
  Model model;
  auto size = model.modelObjects().size();
  ElectricLoadCenterInverterPVWatts inverter(model);
  EXPECT_FALSE(inverter.remove().empty());
  EXPECT_EQ(size, model.modelObjects().size());
}

// test electric load center distribution
TEST_F(ModelFixture, ElectricLoadCenterInverterPVWatts_ElectricLoadCenterDistribution) {
  Model model;
  ElectricLoadCenterDistribution elcd(model);
  EXPECT_FALSE(elcd.inverter());
  ElectricLoadCenterInverterPVWatts inverter(model);
  elcd.setInverter(inverter);
  EXPECT_TRUE(elcd.inverter());
  boost::optional<ElectricLoadCenterDistribution> optelcd = inverter.electricLoadCenterDistribution();
  EXPECT_TRUE(optelcd);
  ElectricLoadCenterDistribution elcd2 = optelcd.get();
  EXPECT_EQ(elcd, elcd2);
  elcd.resetInverter();
  EXPECT_FALSE(inverter.electricLoadCenterDistribution());
  ASSERT_FALSE(elcd.inverter());
  ASSERT_FALSE(elcd2.inverter());
  elcd.setInverter(inverter);
  elcd.remove();
  EXPECT_FALSE(inverter.electricLoadCenterDistribution());
}
