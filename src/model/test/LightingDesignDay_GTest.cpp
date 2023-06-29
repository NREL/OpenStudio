/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../LightingDesignDay.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, LightingDesignDay) {

  Model model;
  LightingDesignDay lightingDesignDay("Clear", Date(MonthOfYear::Dec, 21), model);
  EXPECT_EQ("Clear", lightingDesignDay.cieSkyModel());
  EXPECT_EQ(Date(MonthOfYear::Dec, 21), lightingDesignDay.date());
  EXPECT_EQ(0u, lightingDesignDay.simulationTimes().size());
  EXPECT_FALSE(lightingDesignDay.addSimulationTime(Time(1, 9, 0)));
  EXPECT_FALSE(lightingDesignDay.addSimulationTime(Time(0, -9, 0)));
  EXPECT_EQ(0u, lightingDesignDay.simulationTimes().size());
  EXPECT_TRUE(lightingDesignDay.addSimulationTime(Time(0, 9, 0)));
  EXPECT_TRUE(lightingDesignDay.addSimulationTime(Time(0, 12, 0)));
  EXPECT_TRUE(lightingDesignDay.addSimulationTime(Time(0, 15, 0)));
  EXPECT_EQ(3u, lightingDesignDay.simulationTimes().size());
  EXPECT_EQ(3u, lightingDesignDay.simulationDateTimes().size());
}
