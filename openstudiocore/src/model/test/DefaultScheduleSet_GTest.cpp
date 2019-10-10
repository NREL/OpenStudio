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
