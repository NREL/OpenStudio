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

#include "../Model.hpp"
#include "../Model_Impl.hpp"
#include "../DefaultScheduleSet.hpp"
#include "../DefaultScheduleSet_Impl.hpp"
#include "../ScheduleConstant.hpp"
#include "../ScheduleConstant_Impl.hpp"
#include "../ScheduleTypeLimits.hpp"
#include "../ScheduleCompact.hpp"

#include <utilities/idd/OS_DefaultScheduleSet_FieldEnums.hxx>

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, DefaultScheduleSet)
{
  Model model;

  DefaultScheduleSet defaultScheduleSet(model);
  ScheduleConstant schedule(model);

  EXPECT_FALSE(defaultScheduleSet.hoursofOperationSchedule());
  EXPECT_FALSE(defaultScheduleSet.getDefaultSchedule(DefaultScheduleType::HoursofOperationSchedule));
  EXPECT_TRUE(defaultScheduleSet.setHoursofOperationSchedule(schedule));
  ASSERT_TRUE(defaultScheduleSet.hoursofOperationSchedule());
  EXPECT_EQ(schedule.handle(), defaultScheduleSet.hoursofOperationSchedule()->handle());
  ASSERT_TRUE(defaultScheduleSet.getDefaultSchedule(DefaultScheduleType::HoursofOperationSchedule));
  EXPECT_EQ(schedule.handle(), defaultScheduleSet.getDefaultSchedule(DefaultScheduleType::HoursofOperationSchedule)->handle());
  defaultScheduleSet.resetHoursofOperationSchedule();
  EXPECT_FALSE(defaultScheduleSet.hoursofOperationSchedule());
  EXPECT_FALSE(defaultScheduleSet.getDefaultSchedule(DefaultScheduleType::HoursofOperationSchedule));

  // need different ScheduleTypeLimits
  schedule = ScheduleConstant(model);

  EXPECT_FALSE(defaultScheduleSet.numberofPeopleSchedule());
  EXPECT_FALSE(defaultScheduleSet.getDefaultSchedule(DefaultScheduleType::NumberofPeopleSchedule));
  EXPECT_TRUE(defaultScheduleSet.setNumberofPeopleSchedule(schedule));
  ASSERT_TRUE(defaultScheduleSet.numberofPeopleSchedule());
  EXPECT_EQ(schedule.handle(), defaultScheduleSet.numberofPeopleSchedule()->handle());
  ASSERT_TRUE(defaultScheduleSet.getDefaultSchedule(DefaultScheduleType::NumberofPeopleSchedule));
  EXPECT_EQ(schedule.handle(), defaultScheduleSet.getDefaultSchedule(DefaultScheduleType::NumberofPeopleSchedule)->handle());
  defaultScheduleSet.resetNumberofPeopleSchedule();
  EXPECT_FALSE(defaultScheduleSet.numberofPeopleSchedule());
  EXPECT_FALSE(defaultScheduleSet.getDefaultSchedule(DefaultScheduleType::NumberofPeopleSchedule));

  // need different ScheduleTypeLimits
  schedule = ScheduleConstant(model);

  EXPECT_FALSE(defaultScheduleSet.peopleActivityLevelSchedule());
  EXPECT_FALSE(defaultScheduleSet.getDefaultSchedule(DefaultScheduleType::PeopleActivityLevelSchedule));
  EXPECT_TRUE(defaultScheduleSet.setPeopleActivityLevelSchedule(schedule));
  ASSERT_TRUE(defaultScheduleSet.peopleActivityLevelSchedule());
  EXPECT_EQ(schedule.handle(), defaultScheduleSet.peopleActivityLevelSchedule()->handle());
  ASSERT_TRUE(defaultScheduleSet.getDefaultSchedule(DefaultScheduleType::PeopleActivityLevelSchedule));
  EXPECT_EQ(schedule.handle(), defaultScheduleSet.getDefaultSchedule(DefaultScheduleType::PeopleActivityLevelSchedule)->handle());
  defaultScheduleSet.resetPeopleActivityLevelSchedule();
  EXPECT_FALSE(defaultScheduleSet.peopleActivityLevelSchedule());
  EXPECT_FALSE(defaultScheduleSet.getDefaultSchedule(DefaultScheduleType::PeopleActivityLevelSchedule));

  // need different ScheduleTypeLimits
  schedule = ScheduleConstant(model);

  EXPECT_FALSE(defaultScheduleSet.lightingSchedule());
  EXPECT_FALSE(defaultScheduleSet.getDefaultSchedule(DefaultScheduleType::LightingSchedule));
  EXPECT_TRUE(defaultScheduleSet.setLightingSchedule(schedule));
  ASSERT_TRUE(defaultScheduleSet.lightingSchedule());
  EXPECT_EQ(schedule.handle(), defaultScheduleSet.lightingSchedule()->handle());
  ASSERT_TRUE(defaultScheduleSet.getDefaultSchedule(DefaultScheduleType::LightingSchedule));
  EXPECT_EQ(schedule.handle(), defaultScheduleSet.getDefaultSchedule(DefaultScheduleType::LightingSchedule)->handle());
  defaultScheduleSet.resetLightingSchedule();
  EXPECT_FALSE(defaultScheduleSet.lightingSchedule());
  EXPECT_FALSE(defaultScheduleSet.getDefaultSchedule(DefaultScheduleType::LightingSchedule));

  EXPECT_EQ("Fractional",schedule.scheduleTypeLimits().get().name().get());

  EXPECT_FALSE(defaultScheduleSet.electricEquipmentSchedule());
  EXPECT_FALSE(defaultScheduleSet.getDefaultSchedule(DefaultScheduleType::ElectricEquipmentSchedule));
  EXPECT_TRUE(defaultScheduleSet.setElectricEquipmentSchedule(schedule));
  ASSERT_TRUE(defaultScheduleSet.electricEquipmentSchedule());
  EXPECT_EQ(schedule.handle(), defaultScheduleSet.electricEquipmentSchedule()->handle());
  ASSERT_TRUE(defaultScheduleSet.getDefaultSchedule(DefaultScheduleType::ElectricEquipmentSchedule));
  EXPECT_EQ(schedule.handle(), defaultScheduleSet.getDefaultSchedule(DefaultScheduleType::ElectricEquipmentSchedule)->handle());
  defaultScheduleSet.resetElectricEquipmentSchedule();
  EXPECT_FALSE(defaultScheduleSet.electricEquipmentSchedule());
  EXPECT_FALSE(defaultScheduleSet.getDefaultSchedule(DefaultScheduleType::ElectricEquipmentSchedule));

  EXPECT_FALSE(defaultScheduleSet.gasEquipmentSchedule());
  EXPECT_FALSE(defaultScheduleSet.getDefaultSchedule(DefaultScheduleType::GasEquipmentSchedule));
  EXPECT_TRUE(defaultScheduleSet.setGasEquipmentSchedule(schedule));
  ASSERT_TRUE(defaultScheduleSet.gasEquipmentSchedule());
  EXPECT_EQ(schedule.handle(), defaultScheduleSet.gasEquipmentSchedule()->handle());
  ASSERT_TRUE(defaultScheduleSet.getDefaultSchedule(DefaultScheduleType::GasEquipmentSchedule));
  EXPECT_EQ(schedule.handle(), defaultScheduleSet.getDefaultSchedule(DefaultScheduleType::GasEquipmentSchedule)->handle());
  defaultScheduleSet.resetGasEquipmentSchedule();
  EXPECT_FALSE(defaultScheduleSet.gasEquipmentSchedule());
  EXPECT_FALSE(defaultScheduleSet.getDefaultSchedule(DefaultScheduleType::GasEquipmentSchedule));

  EXPECT_FALSE(defaultScheduleSet.hotWaterEquipmentSchedule());
  EXPECT_FALSE(defaultScheduleSet.getDefaultSchedule(DefaultScheduleType::HotWaterEquipmentSchedule));
  EXPECT_TRUE(defaultScheduleSet.setHotWaterEquipmentSchedule(schedule));
  ASSERT_TRUE(defaultScheduleSet.hotWaterEquipmentSchedule());
  EXPECT_EQ(schedule.handle(), defaultScheduleSet.hotWaterEquipmentSchedule()->handle());
  ASSERT_TRUE(defaultScheduleSet.getDefaultSchedule(DefaultScheduleType::HotWaterEquipmentSchedule));
  EXPECT_EQ(schedule.handle(), defaultScheduleSet.getDefaultSchedule(DefaultScheduleType::HotWaterEquipmentSchedule)->handle());
  defaultScheduleSet.resetHotWaterEquipmentSchedule();
  EXPECT_FALSE(defaultScheduleSet.hotWaterEquipmentSchedule());
  EXPECT_FALSE(defaultScheduleSet.getDefaultSchedule(DefaultScheduleType::HotWaterEquipmentSchedule));

  EXPECT_FALSE(defaultScheduleSet.infiltrationSchedule());
  EXPECT_FALSE(defaultScheduleSet.getDefaultSchedule(DefaultScheduleType::InfiltrationSchedule));
  EXPECT_TRUE(defaultScheduleSet.setInfiltrationSchedule(schedule));
  ASSERT_TRUE(defaultScheduleSet.infiltrationSchedule());
  EXPECT_EQ(schedule.handle(), defaultScheduleSet.infiltrationSchedule()->handle());
  ASSERT_TRUE(defaultScheduleSet.getDefaultSchedule(DefaultScheduleType::InfiltrationSchedule));
  EXPECT_EQ(schedule.handle(), defaultScheduleSet.getDefaultSchedule(DefaultScheduleType::InfiltrationSchedule)->handle());
  defaultScheduleSet.resetInfiltrationSchedule();
  EXPECT_FALSE(defaultScheduleSet.infiltrationSchedule());
  EXPECT_FALSE(defaultScheduleSet.getDefaultSchedule(DefaultScheduleType::InfiltrationSchedule));
}

TEST_F(ModelFixture, DefaultScheduleSet_ScheduleTypes) {
  Model model;
  ScheduleCompact alwaysOn(model,1.0);
  alwaysOn.setName("Always On");
  EXPECT_FALSE(alwaysOn.scheduleTypeLimits());

  DefaultScheduleSet scheduleSet(model);
  EXPECT_TRUE(scheduleSet.setLightingSchedule(alwaysOn));
  ASSERT_TRUE(alwaysOn.scheduleTypeLimits());
  EXPECT_EQ("Fractional",alwaysOn.scheduleTypeLimits()->name().get());
  ASSERT_TRUE(alwaysOn.scheduleTypeLimits()->numericType());
  EXPECT_EQ("Continuous",alwaysOn.scheduleTypeLimits()->numericType().get());

  // always on is now a continuous schedule
  // it can now be used as an Hours of Operation Schedule through the interface ...
  EXPECT_TRUE(scheduleSet.setHoursofOperationSchedule(alwaysOn));
  EXPECT_TRUE(scheduleSet.hoursofOperationSchedule());
  // ... or through the low-level data methods
  EXPECT_TRUE(scheduleSet.setPointer(OS_DefaultScheduleSetFields::HoursofOperationScheduleName,alwaysOn.handle()));
  EXPECT_TRUE(scheduleSet.hoursofOperationSchedule());
}
