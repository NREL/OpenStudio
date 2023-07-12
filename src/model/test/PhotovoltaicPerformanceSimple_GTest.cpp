/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../PhotovoltaicPerformanceSimple.hpp"
#include "../Schedule.hpp"
#include "../ScheduleCompact.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, PhotovoltaicPerformanceSimple_Instantiate) {
  auto result = false;
  auto value = 0.1;

  Model model;

  PhotovoltaicPerformanceSimple photovoltaicPerformanceSimple(model);

  EXPECT_EQ("Fixed", photovoltaicPerformanceSimple.conversionEfficiencyInputMode());
  EXPECT_TRUE(photovoltaicPerformanceSimple.fixedEfficiency());
  EXPECT_EQ(0.12, photovoltaicPerformanceSimple.fixedEfficiency().get());
  EXPECT_EQ(photovoltaicPerformanceSimple.fractionOfSurfaceAreaWithActiveSolarCells(), 0.89);
  EXPECT_TRUE(photovoltaicPerformanceSimple.isfractionOfSurfaceAreaWithActiveSolarCellsDefaulted());
  result = photovoltaicPerformanceSimple.setFractionOfSurfaceAreaWithActiveSolarCells(value);
  EXPECT_TRUE(result);
  EXPECT_TRUE(photovoltaicPerformanceSimple.fractionOfSurfaceAreaWithActiveSolarCells());
  EXPECT_EQ(photovoltaicPerformanceSimple.fractionOfSurfaceAreaWithActiveSolarCells(), value);
  photovoltaicPerformanceSimple.resetFractionOfSurfaceAreaWithActiveSolarCells();
  EXPECT_EQ(photovoltaicPerformanceSimple.fractionOfSurfaceAreaWithActiveSolarCells(), 0.89);

  result = photovoltaicPerformanceSimple.setFixedEfficiency(value);
  EXPECT_TRUE(result);
  EXPECT_EQ("Fixed", photovoltaicPerformanceSimple.conversionEfficiencyInputMode());
  EXPECT_TRUE(photovoltaicPerformanceSimple.fixedEfficiency());
  EXPECT_EQ(photovoltaicPerformanceSimple.fixedEfficiency().get(), value);
  photovoltaicPerformanceSimple.resetFixedEfficiency();
  EXPECT_EQ("Fixed", photovoltaicPerformanceSimple.conversionEfficiencyInputMode());
  EXPECT_TRUE(photovoltaicPerformanceSimple.fixedEfficiency());
  EXPECT_EQ(0.12, photovoltaicPerformanceSimple.fixedEfficiency().get());

  EXPECT_FALSE(photovoltaicPerformanceSimple.efficiencySchedule());
  ScheduleCompact scheduleCompact(model);
  result = photovoltaicPerformanceSimple.setEfficiencySchedule(scheduleCompact);
  EXPECT_TRUE(result);
  EXPECT_EQ("Scheduled", photovoltaicPerformanceSimple.conversionEfficiencyInputMode());
  EXPECT_TRUE(photovoltaicPerformanceSimple.efficiencySchedule());
  photovoltaicPerformanceSimple.resetEfficiencySchedule();
  EXPECT_FALSE(photovoltaicPerformanceSimple.efficiencySchedule());
  EXPECT_EQ("Fixed", photovoltaicPerformanceSimple.conversionEfficiencyInputMode());
  EXPECT_TRUE(photovoltaicPerformanceSimple.fixedEfficiency());
  EXPECT_EQ(0.12, photovoltaicPerformanceSimple.fixedEfficiency().get());
}
