/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include <string>

#include "ModelFixture.hpp"

#include "../Model.hpp"
#include "../Model_Impl.hpp"

#include "../SiteWaterMainsTemperature.hpp"
#include "../SiteWaterMainsTemperature_Impl.hpp"
#include "../ScheduleConstant.hpp"
#include "../ScheduleConstant_Impl.hpp"

using namespace openstudio::model;

TEST_F(ModelFixture, SiteWaterMainsTemperature_SiteWaterMainsTemperature) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      // create a model to use
      Model model;

      // create a site water mains temperature object to use
      auto siteWater = model.getUniqueModelObject<SiteWaterMainsTemperature>();

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");

  // create a model to use
  Model model;

  // create a site water mains temperature object to use
  auto siteWater = model.getUniqueModelObject<SiteWaterMainsTemperature>();

  EXPECT_EQ("CorrelationFromWeatherFile", siteWater.calculationMethod());
  EXPECT_FALSE(siteWater.temperatureSchedule());
  EXPECT_FALSE(siteWater.annualAverageOutdoorAirTemperature());
  EXPECT_FALSE(siteWater.maximumDifferenceInMonthlyAverageOutdoorAirTemperatures());
}

// test setting and getting
TEST_F(ModelFixture, SiteWaterMainsTemperature_SetGetFields) {
  // create a model to use
  Model model;

  // create a site water mains temperature object to use
  auto siteWater = model.getUniqueModelObject<SiteWaterMainsTemperature>();

  // set the fields
  siteWater.setCalculationMethod("Correlation");
  siteWater.setAnnualAverageOutdoorAirTemperature(50);
  siteWater.setMaximumDifferenceInMonthlyAverageOutdoorAirTemperatures(60);

  // check the fields
  EXPECT_EQ("Correlation", siteWater.calculationMethod());
  ASSERT_TRUE(siteWater.annualAverageOutdoorAirTemperature());
  EXPECT_EQ(50, siteWater.annualAverageOutdoorAirTemperature().get());
  ASSERT_TRUE(siteWater.maximumDifferenceInMonthlyAverageOutdoorAirTemperatures());
  EXPECT_EQ(60, siteWater.maximumDifferenceInMonthlyAverageOutdoorAirTemperatures().get());

  // reset them one by one
  siteWater.resetAnnualAverageOutdoorAirTemperature();
  siteWater.resetMaximumDifferenceInMonthlyAverageOutdoorAirTemperatures();

  EXPECT_FALSE(siteWater.annualAverageOutdoorAirTemperature());
  EXPECT_FALSE(siteWater.maximumDifferenceInMonthlyAverageOutdoorAirTemperatures());

  // check more
  ScheduleConstant schedule(model);
  schedule.setValue(21.111);
  siteWater.setTemperatureSchedule(schedule);
  EXPECT_EQ("Schedule", siteWater.calculationMethod());
  ASSERT_TRUE(siteWater.temperatureSchedule());
  EXPECT_EQ(schedule.name().get(), siteWater.temperatureSchedule().get().name().get());
  siteWater.setCalculationMethod("Correlation");
  EXPECT_TRUE(siteWater.temperatureSchedule());
  EXPECT_FALSE(siteWater.annualAverageOutdoorAirTemperature());
  EXPECT_FALSE(siteWater.maximumDifferenceInMonthlyAverageOutdoorAirTemperatures());
  siteWater.setCalculationMethod("CorrelationFromWeatherFile");
  EXPECT_TRUE(siteWater.temperatureSchedule());
  EXPECT_FALSE(siteWater.annualAverageOutdoorAirTemperature());
  EXPECT_FALSE(siteWater.maximumDifferenceInMonthlyAverageOutdoorAirTemperatures());
}

// test cloning it
TEST_F(ModelFixture, SiteWaterMainsTemperature_Clone) {
  // create a model to use
  Model model;

  // create a site water mains temperature object to use
  auto siteWater = model.getUniqueModelObject<SiteWaterMainsTemperature>();

  // change some of the fields
  siteWater.setAnnualAverageOutdoorAirTemperature(45);

  // clone it into the same model
  auto siteWaterClone = siteWater.clone(model).cast<SiteWaterMainsTemperature>();
  // UniqueModelObject: should be the same as the original
  EXPECT_EQ(siteWater, siteWaterClone);
  ASSERT_TRUE(siteWaterClone.annualAverageOutdoorAirTemperature());
  EXPECT_EQ(45, siteWaterClone.annualAverageOutdoorAirTemperature().get());

  // clone it into a different model
  Model model2;
  auto siteWaterClone2 = siteWaterClone.clone(model2).cast<SiteWaterMainsTemperature>();
  ASSERT_TRUE(siteWaterClone2.annualAverageOutdoorAirTemperature());
  EXPECT_EQ(45, siteWaterClone2.annualAverageOutdoorAirTemperature().get());
}

// check that remove works
TEST_F(ModelFixture, SiteWaterMainsTemperature_Remove) {
  Model model;
  auto size = model.modelObjects().size();
  auto siteWater = model.getUniqueModelObject<SiteWaterMainsTemperature>();
  EXPECT_FALSE(siteWater.remove().empty());
  EXPECT_EQ(size, model.modelObjects().size());
}
