/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"
#include "../ScheduleWeek.hpp"
#include "../ScheduleWeek_Impl.hpp"
#include "../ScheduleDay.hpp"
#include "../ScheduleDay_Impl.hpp"
#include "../ScheduleTypeLimits.hpp"
#include "../ScheduleTypeLimits_Impl.hpp"

#include "../../utilities/time/Date.hpp"
#include "../../utilities/time/Time.hpp"

using namespace openstudio::model;
using namespace openstudio;

TEST_F(ModelFixture, Schedule_Week)
{
  Model model;

  ScheduleDay schedule1(model);
  ScheduleDay schedule2(model);
  ScheduleDay schedule3(model);

  ScheduleWeek weekSchedule(model);
  EXPECT_FALSE(weekSchedule.mondaySchedule());
  EXPECT_FALSE(weekSchedule.sundaySchedule());
  EXPECT_FALSE(weekSchedule.holidaySchedule());

  EXPECT_TRUE(weekSchedule.setMondaySchedule(schedule1));
  ASSERT_TRUE(weekSchedule.mondaySchedule());
  EXPECT_EQ(schedule1.handle(), weekSchedule.mondaySchedule()->handle());
  EXPECT_FALSE(weekSchedule.sundaySchedule());
  EXPECT_FALSE(weekSchedule.holidaySchedule());

  EXPECT_TRUE(weekSchedule.setWeekendSchedule(schedule2));
  ASSERT_TRUE(weekSchedule.mondaySchedule());
  EXPECT_EQ(schedule1.handle(), weekSchedule.mondaySchedule()->handle());
  ASSERT_TRUE(weekSchedule.sundaySchedule());
  EXPECT_EQ(schedule2.handle(), weekSchedule.sundaySchedule()->handle());
  EXPECT_FALSE(weekSchedule.holidaySchedule());

  EXPECT_TRUE(weekSchedule.setAllSchedules(schedule3));
  ASSERT_TRUE(weekSchedule.mondaySchedule());
  EXPECT_EQ(schedule3.handle(), weekSchedule.mondaySchedule()->handle());
  ASSERT_TRUE(weekSchedule.sundaySchedule());
  EXPECT_EQ(schedule3.handle(), weekSchedule.sundaySchedule()->handle());
  ASSERT_TRUE(weekSchedule.holidaySchedule());
  EXPECT_EQ(schedule3.handle(), weekSchedule.holidaySchedule()->handle());
}
