/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
      SiteWaterMainsTemperature siteWater = model.getUniqueModelObject<SiteWaterMainsTemperature>();

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");

  // create a model to use
  Model model;

  // create a site water mains temperature object to use
  SiteWaterMainsTemperature siteWater = model.getUniqueModelObject<SiteWaterMainsTemperature>();

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
  SiteWaterMainsTemperature siteWater = model.getUniqueModelObject<SiteWaterMainsTemperature>();

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
  EXPECT_EQ(schedule.name().get(), siteWater.temperatureSchedule().get().name());
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
  SiteWaterMainsTemperature siteWater = model.getUniqueModelObject<SiteWaterMainsTemperature>();

  // change some of the fields
  siteWater.setAnnualAverageOutdoorAirTemperature(45);

  // clone it into the same model
  SiteWaterMainsTemperature siteWaterClone = siteWater.clone(model).cast<SiteWaterMainsTemperature>();
  // UniqueModelObject: should be the same as the original
  EXPECT_EQ(siteWater, siteWaterClone);
  ASSERT_TRUE(siteWaterClone.annualAverageOutdoorAirTemperature());
  EXPECT_EQ(45, siteWaterClone.annualAverageOutdoorAirTemperature().get());

  // clone it into a different model
  Model model2;
  SiteWaterMainsTemperature siteWaterClone2 = siteWaterClone.clone(model2).cast<SiteWaterMainsTemperature>();
  ASSERT_TRUE(siteWaterClone2.annualAverageOutdoorAirTemperature());
  EXPECT_EQ(45, siteWaterClone2.annualAverageOutdoorAirTemperature().get());
}

// check that remove works
TEST_F(ModelFixture, SiteWaterMainsTemperature_Remove) {
  Model model;
  auto size = model.modelObjects().size();
  SiteWaterMainsTemperature siteWater = model.getUniqueModelObject<SiteWaterMainsTemperature>();
  EXPECT_FALSE(siteWater.remove().empty());
  EXPECT_EQ(size, model.modelObjects().size());
}
