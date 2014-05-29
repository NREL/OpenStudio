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
#include "../ScheduleTypeRegistry.hpp"
#include "../Model.hpp"
#include "../ScheduleConstant.hpp"
#include "../ScheduleTypeLimits.hpp"
#include "../LightsDefinition.hpp"
#include "../Lights.hpp"

#include "../../utilities/core/Containers.hpp"

#include <sstream>

using namespace openstudio::model;
using namespace openstudio;

TEST_F(ModelFixture, ScheduleTypeRegistry_ClassNames) {
  StringVector classesWithSchedules = ScheduleTypeRegistry::instance().classNames();
  EXPECT_FALSE(classesWithSchedules.empty());
  std::stringstream ss;
  for (const std::string& className : classesWithSchedules) {
    EXPECT_FALSE(ScheduleTypeRegistry::instance().getScheduleTypesByClassName(className).empty());
    ss << "  " << className << std::endl;
  }
  LOG(Trace,"The following ModelObject classes reference at least one schedule:"
      << std::endl << ss.str());

}

TEST_F(ModelFixture, ScheduleTypeRegistry_UseInSetSchedule) {
  Model model;

  // create a schedule
  ScheduleConstant schedule(model);

  // set a minimal schedule type limits
  ScheduleTypeLimits anyNumber(model);
  anyNumber.setName("Any Number");
  bool ok = schedule.setScheduleTypeLimits(anyNumber);
  EXPECT_TRUE(ok);

  // try to set Lights schedule--should fail b/c needs between 0 and 1
  LightsDefinition definition(model);
  definition.setLightingLevel(100.0);
  Lights light(definition);
  ok = light.setSchedule(schedule);
  EXPECT_FALSE(ok);

  // set Availablity schedule type limits
  ScheduleType scheduleType = ScheduleTypeRegistry::instance().getScheduleType("CoilCoolingDXSingleSpeed","Availability");
  ScheduleTypeLimits availabilityLimits = ScheduleTypeRegistry::instance().getOrCreateScheduleTypeLimits(scheduleType,model);
  ok = schedule.setScheduleTypeLimits(availabilityLimits);
  EXPECT_TRUE(ok);

  // try to set Lights schedule--used to fail b/c lights schedule is Dimensionless, not Availability, now passes
  ok = light.setSchedule(schedule);
  EXPECT_TRUE(ok);

  // clear conflicting information
  availabilityLimits.resetNumericType();
  availabilityLimits.resetUnitType();

  // set Lights schedule--should succeed b/c not in conflict
  ok = light.setSchedule(schedule);
  EXPECT_TRUE(ok);

  // try to clear schedule type limits--should fail b/c in use
  ok = schedule.resetScheduleTypeLimits();
  EXPECT_FALSE(ok);

  // reset Lights schedule
  light.resetSchedule();

  // try to clear schedule type limits--should succeed
  ok = schedule.resetScheduleTypeLimits();
  EXPECT_TRUE(ok);
  EXPECT_FALSE(schedule.scheduleTypeLimits());

  // set Lights schedule--should succeed and assign an appropriate ScheduleTypeLimits
  ok = light.setSchedule(schedule);
  EXPECT_TRUE(ok);
  ASSERT_TRUE(schedule.scheduleTypeLimits());
  ScheduleTypeLimits lightsLimits = schedule.scheduleTypeLimits().get();
  EXPECT_EQ("Fractional",lightsLimits.name().get());
  ASSERT_TRUE(lightsLimits.numericType());
  EXPECT_EQ("Continuous",lightsLimits.numericType().get());
  EXPECT_EQ("Dimensionless",lightsLimits.unitType());
  EXPECT_TRUE(lightsLimits.isUnitTypeDefaulted());
  ASSERT_TRUE(lightsLimits.lowerLimitValue());
  EXPECT_DOUBLE_EQ(0.0,lightsLimits.lowerLimitValue().get());
  ASSERT_TRUE(lightsLimits.upperLimitValue());
  EXPECT_DOUBLE_EQ(1.0,lightsLimits.upperLimitValue().get());
}

