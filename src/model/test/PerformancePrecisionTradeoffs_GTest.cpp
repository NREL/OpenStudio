/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include <string>

#include "ModelFixture.hpp"

#include "../Model.hpp"
#include "../Model_Impl.hpp"

#include "../PerformancePrecisionTradeoffs.hpp"
#include "../PerformancePrecisionTradeoffs_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, PerformancePrecisionTradeoffs_PerformancePrecisionTradeoffs) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      // create a model to use
      Model model;

      // create a performance precision tradeoffs object to use
      PerformancePrecisionTradeoffs performancePrecisionTradeoffs = model.getUniqueModelObject<PerformancePrecisionTradeoffs>();

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");

  // create a model to use
  Model model;

  // create a performance precision tradeoffs object to use
  PerformancePrecisionTradeoffs performancePrecisionTradeoffs = model.getUniqueModelObject<PerformancePrecisionTradeoffs>();

  EXPECT_TRUE(performancePrecisionTradeoffs.isUseCoilDirectSolutionsDefaulted());
  EXPECT_FALSE(performancePrecisionTradeoffs.useCoilDirectSolutions());

  EXPECT_TRUE(performancePrecisionTradeoffs.isZoneRadiantExchangeAlgorithmDefaulted());
  EXPECT_EQ("ScriptF", performancePrecisionTradeoffs.zoneRadiantExchangeAlgorithm());

  EXPECT_TRUE(performancePrecisionTradeoffs.isOverrideModeDefaulted());
  EXPECT_EQ("Normal", performancePrecisionTradeoffs.overrideMode());

  EXPECT_TRUE(performancePrecisionTradeoffs.isMaxZoneTempDiffDefaulted());
  EXPECT_EQ(0.3, performancePrecisionTradeoffs.maxZoneTempDiff());

  EXPECT_TRUE(performancePrecisionTradeoffs.isMaxAllowedDelTempDefaulted());
  EXPECT_EQ(0.002, performancePrecisionTradeoffs.maxAllowedDelTemp());

  EXPECT_TRUE(performancePrecisionTradeoffs.isUseRepresentativeSurfacesforCalculationsDefaulted());
  EXPECT_FALSE(performancePrecisionTradeoffs.useRepresentativeSurfacesforCalculations());
}

// test setting and getting
TEST_F(ModelFixture, PerformancePrecisionTradeoffs_SetGetFields) {
  // create a model to use
  Model model;

  // create a performance precision tradeoffs object to use
  PerformancePrecisionTradeoffs performancePrecisionTradeoffs = model.getUniqueModelObject<PerformancePrecisionTradeoffs>();

  // set & check the fields
  EXPECT_TRUE(performancePrecisionTradeoffs.setUseCoilDirectSolutions(true));
  EXPECT_FALSE(performancePrecisionTradeoffs.isUseCoilDirectSolutionsDefaulted());
  EXPECT_TRUE(performancePrecisionTradeoffs.useCoilDirectSolutions());
  performancePrecisionTradeoffs.resetUseCoilDirectSolutions();
  EXPECT_TRUE(performancePrecisionTradeoffs.isUseCoilDirectSolutionsDefaulted());

  EXPECT_TRUE(performancePrecisionTradeoffs.setZoneRadiantExchangeAlgorithm("CarrollMRT"));
  EXPECT_FALSE(performancePrecisionTradeoffs.isZoneRadiantExchangeAlgorithmDefaulted());
  EXPECT_EQ("CarrollMRT", performancePrecisionTradeoffs.zoneRadiantExchangeAlgorithm());
  EXPECT_FALSE(performancePrecisionTradeoffs.setZoneRadiantExchangeAlgorithm("BADENUM"));
  EXPECT_FALSE(performancePrecisionTradeoffs.isZoneRadiantExchangeAlgorithmDefaulted());
  EXPECT_EQ("CarrollMRT", performancePrecisionTradeoffs.zoneRadiantExchangeAlgorithm());
  performancePrecisionTradeoffs.resetZoneRadiantExchangeAlgorithm();
  EXPECT_TRUE(performancePrecisionTradeoffs.isZoneRadiantExchangeAlgorithmDefaulted());
  EXPECT_EQ("ScriptF", performancePrecisionTradeoffs.zoneRadiantExchangeAlgorithm());

  EXPECT_TRUE(performancePrecisionTradeoffs.setOverrideMode("Advanced"));
  EXPECT_FALSE(performancePrecisionTradeoffs.isOverrideModeDefaulted());
  EXPECT_EQ("Advanced", performancePrecisionTradeoffs.overrideMode());
  EXPECT_FALSE(performancePrecisionTradeoffs.setOverrideMode("BADENUM"));
  EXPECT_FALSE(performancePrecisionTradeoffs.isOverrideModeDefaulted());
  EXPECT_EQ("Advanced", performancePrecisionTradeoffs.overrideMode());
  performancePrecisionTradeoffs.resetOverrideMode();
  EXPECT_TRUE(performancePrecisionTradeoffs.isOverrideModeDefaulted());
  EXPECT_EQ("Normal", performancePrecisionTradeoffs.overrideMode());

  EXPECT_TRUE(performancePrecisionTradeoffs.setMaxZoneTempDiff(0.65));
  EXPECT_FALSE(performancePrecisionTradeoffs.isMaxZoneTempDiffDefaulted());
  EXPECT_EQ(0.65, performancePrecisionTradeoffs.maxZoneTempDiff());
  // max 3
  EXPECT_FALSE(performancePrecisionTradeoffs.setMaxZoneTempDiff(30.0));
  EXPECT_FALSE(performancePrecisionTradeoffs.isMaxZoneTempDiffDefaulted());
  EXPECT_EQ(0.65, performancePrecisionTradeoffs.maxZoneTempDiff());
  performancePrecisionTradeoffs.resetMaxZoneTempDiff();
  EXPECT_TRUE(performancePrecisionTradeoffs.isMaxZoneTempDiffDefaulted());
  EXPECT_EQ(0.3, performancePrecisionTradeoffs.maxZoneTempDiff());

  EXPECT_TRUE(performancePrecisionTradeoffs.setMaxAllowedDelTemp(0.05));
  EXPECT_FALSE(performancePrecisionTradeoffs.isMaxAllowedDelTempDefaulted());
  EXPECT_EQ(0.05, performancePrecisionTradeoffs.maxAllowedDelTemp());
  // max 0.1
  EXPECT_FALSE(performancePrecisionTradeoffs.setMaxAllowedDelTemp(1));
  EXPECT_FALSE(performancePrecisionTradeoffs.isMaxAllowedDelTempDefaulted());
  EXPECT_EQ(0.05, performancePrecisionTradeoffs.maxAllowedDelTemp());
  performancePrecisionTradeoffs.resetMaxAllowedDelTemp();
  EXPECT_TRUE(performancePrecisionTradeoffs.isMaxAllowedDelTempDefaulted());
  EXPECT_EQ(0.002, performancePrecisionTradeoffs.maxAllowedDelTemp());

  EXPECT_TRUE(performancePrecisionTradeoffs.setUseRepresentativeSurfacesforCalculations(true));
  EXPECT_FALSE(performancePrecisionTradeoffs.isUseRepresentativeSurfacesforCalculationsDefaulted());
  EXPECT_TRUE(performancePrecisionTradeoffs.useRepresentativeSurfacesforCalculations());
  performancePrecisionTradeoffs.resetUseRepresentativeSurfacesforCalculations();
  EXPECT_TRUE(performancePrecisionTradeoffs.isUseRepresentativeSurfacesforCalculationsDefaulted());
}

// test cloning it
TEST_F(ModelFixture, PerformancePrecisionTradeoffs_Clone) {
  // create a model to use
  Model model;

  // create a performance precision tradeoffs object to use
  PerformancePrecisionTradeoffs performancePrecisionTradeoffs = model.getUniqueModelObject<PerformancePrecisionTradeoffs>();

  // change some of the fields
  EXPECT_TRUE(performancePrecisionTradeoffs.setUseCoilDirectSolutions(true));
  EXPECT_TRUE(performancePrecisionTradeoffs.setZoneRadiantExchangeAlgorithm("CarrollMRT"));
  EXPECT_TRUE(performancePrecisionTradeoffs.setOverrideMode("Advanced"));
  EXPECT_TRUE(performancePrecisionTradeoffs.setMaxZoneTempDiff(0.65));
  EXPECT_TRUE(performancePrecisionTradeoffs.setMaxAllowedDelTemp(0.05));
  EXPECT_TRUE(performancePrecisionTradeoffs.setUseRepresentativeSurfacesforCalculations(true));

  // clone it into the same model
  auto performancePrecisionTradeoffsClone = performancePrecisionTradeoffs.clone(model).cast<PerformancePrecisionTradeoffs>();
  EXPECT_FALSE(performancePrecisionTradeoffsClone.isUseCoilDirectSolutionsDefaulted());
  EXPECT_TRUE(performancePrecisionTradeoffsClone.useCoilDirectSolutions());
  EXPECT_FALSE(performancePrecisionTradeoffsClone.isZoneRadiantExchangeAlgorithmDefaulted());
  EXPECT_EQ("CarrollMRT", performancePrecisionTradeoffsClone.zoneRadiantExchangeAlgorithm());
  EXPECT_FALSE(performancePrecisionTradeoffsClone.isOverrideModeDefaulted());
  EXPECT_EQ("Advanced", performancePrecisionTradeoffsClone.overrideMode());
  EXPECT_FALSE(performancePrecisionTradeoffsClone.isMaxZoneTempDiffDefaulted());
  EXPECT_EQ(0.65, performancePrecisionTradeoffsClone.maxZoneTempDiff());
  EXPECT_FALSE(performancePrecisionTradeoffs.isMaxAllowedDelTempDefaulted());
  EXPECT_EQ(0.05, performancePrecisionTradeoffs.maxAllowedDelTemp());
  EXPECT_FALSE(performancePrecisionTradeoffsClone.isUseRepresentativeSurfacesforCalculationsDefaulted());
  EXPECT_TRUE(performancePrecisionTradeoffsClone.useRepresentativeSurfacesforCalculations());

  // clone it into a different model
  Model model2;
  auto performancePrecisionTradeoffsClone2 = performancePrecisionTradeoffs.clone(model2).cast<PerformancePrecisionTradeoffs>();
  EXPECT_FALSE(performancePrecisionTradeoffsClone2.isUseCoilDirectSolutionsDefaulted());
  EXPECT_TRUE(performancePrecisionTradeoffsClone2.useCoilDirectSolutions());
  EXPECT_FALSE(performancePrecisionTradeoffsClone2.isZoneRadiantExchangeAlgorithmDefaulted());
  EXPECT_EQ("CarrollMRT", performancePrecisionTradeoffsClone2.zoneRadiantExchangeAlgorithm());
  EXPECT_FALSE(performancePrecisionTradeoffsClone2.isOverrideModeDefaulted());
  EXPECT_EQ("Advanced", performancePrecisionTradeoffsClone2.overrideMode());
  EXPECT_FALSE(performancePrecisionTradeoffsClone2.isMaxZoneTempDiffDefaulted());
  EXPECT_EQ(0.65, performancePrecisionTradeoffsClone2.maxZoneTempDiff());
  EXPECT_FALSE(performancePrecisionTradeoffsClone2.isMaxAllowedDelTempDefaulted());
  EXPECT_EQ(0.05, performancePrecisionTradeoffsClone2.maxAllowedDelTemp());
  EXPECT_FALSE(performancePrecisionTradeoffsClone2.isUseRepresentativeSurfacesforCalculationsDefaulted());
  EXPECT_TRUE(performancePrecisionTradeoffsClone2.useRepresentativeSurfacesforCalculations());
}

// check that remove works
TEST_F(ModelFixture, PerformancePrecisionTradeoffs_Remove) {
  Model model;
  auto size = model.modelObjects().size();
  PerformancePrecisionTradeoffs performancePrecisionTradeoffs = model.getUniqueModelObject<PerformancePrecisionTradeoffs>();
  EXPECT_FALSE(performancePrecisionTradeoffs.remove().empty());
  EXPECT_EQ(size, model.modelObjects().size());
}
