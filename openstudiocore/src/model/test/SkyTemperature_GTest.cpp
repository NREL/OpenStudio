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
#include "../SkyTemperature.hpp"
#include "../SkyTemperature_Impl.hpp"

#include "../Model.hpp"
#include "../Model_Impl.hpp"
#include "../Site.hpp"
#include "../Site_Impl.hpp"
#include "../RunPeriod.hpp"
#include "../RunPeriod_Impl.hpp"
#include "../DesignDay.hpp"
#include "../DesignDay_Impl.hpp"
#include "../WeatherFileDays.hpp"
#include "../WeatherFileDays_Impl.hpp"
#include "../WeatherFileConditionType.hpp"
#include "../WeatherFileConditionType_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, SkyTemperature_Hierarchy)
{
  Model model;

  SkyTemperature skyTemperature(model);
  // default parent is Site, but not made by parent()
  OptionalParentObject opo = skyTemperature.parent();
  EXPECT_FALSE(opo);
  // construct Site
  Site site = model.getUniqueModelObject<Site>();
  opo = skyTemperature.parent();
  ASSERT_TRUE(opo);
  ParentObject skyTempParent = *opo;
  EXPECT_TRUE(skyTempParent == site);

  // Make skyTemperature apply to a RunPeriod
  RunPeriod runPeriod = model.getUniqueModelObject<RunPeriod>();
  EXPECT_TRUE(skyTemperature.setParent(runPeriod));
  opo = skyTemperature.parent();
  ASSERT_TRUE(opo);
  skyTempParent = *opo;
  EXPECT_TRUE(skyTempParent == runPeriod);
  EXPECT_EQ(static_cast<unsigned>(0),site.children().size());

  SkyTemperature globalSkyTemperature(model);
  opo = globalSkyTemperature.parent();
  ASSERT_TRUE(opo);
  ParentObject globalSkyTempParent = *opo;
  EXPECT_TRUE(globalSkyTempParent == site);
  EXPECT_EQ(static_cast<unsigned>(1),site.children().size());
  EXPECT_EQ(static_cast<unsigned>(1),runPeriod.children().size());
  EXPECT_EQ(static_cast<unsigned>(4),model.numObjects());

  // skyTemperature can also be parented by SizingPeriod objects
  DesignDay designDay(model);
  EXPECT_TRUE(skyTemperature.setParent(designDay));
  WeatherFileDays weatherFileDays(model);
  EXPECT_TRUE(skyTemperature.setParent(weatherFileDays));
  WeatherFileConditionType weatherFileConditionType(model);
  EXPECT_TRUE(skyTemperature.setParent(weatherFileConditionType));
}
