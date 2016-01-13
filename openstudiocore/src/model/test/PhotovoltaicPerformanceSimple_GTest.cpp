/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
 *  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **********************************************************************/

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
  EXPECT_EQ(photovoltaicPerformanceSimple.fractionOfSurfaceAreaWithActiveSolarCells(),0.89);
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

