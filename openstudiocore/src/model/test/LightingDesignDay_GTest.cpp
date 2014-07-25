/**********************************************************************
*  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
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

#include "../LightingDesignDay.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, LightingDesignDay) {

  Model model;
  LightingDesignDay lightingDesignDay("Clear", Date(MonthOfYear::Dec, 21), model);
  EXPECT_EQ("Clear", lightingDesignDay.cieSkyModel());
  EXPECT_EQ(Date(MonthOfYear::Dec, 21), lightingDesignDay.date());
  EXPECT_EQ(0u, lightingDesignDay.simulationTimes().size());
  EXPECT_FALSE(lightingDesignDay.addSimulationTime(Time(1,9,0)));
  EXPECT_FALSE(lightingDesignDay.addSimulationTime(Time(0,-9,0)));
  EXPECT_EQ(0u, lightingDesignDay.simulationTimes().size());
  EXPECT_TRUE(lightingDesignDay.addSimulationTime(Time(0,9,0)));
  EXPECT_TRUE(lightingDesignDay.addSimulationTime(Time(0,12,0)));
  EXPECT_TRUE(lightingDesignDay.addSimulationTime(Time(0,15,0)));
  EXPECT_EQ(3u, lightingDesignDay.simulationTimes().size());
  EXPECT_EQ(3u, lightingDesignDay.simulationDateTimes().size());
}
