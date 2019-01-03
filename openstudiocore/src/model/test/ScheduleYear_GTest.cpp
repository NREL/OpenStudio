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
#include "../ScheduleYear.hpp"
#include "../ScheduleYear_Impl.hpp"
#include "../ScheduleWeek.hpp"
#include "../ScheduleWeek_Impl.hpp"
#include "../YearDescription.hpp"
#include "../YearDescription_Impl.hpp"
#include "../ScheduleTypeLimits.hpp"
#include "../ScheduleTypeLimits_Impl.hpp"

#include "../../utilities/core/UUID.hpp"
#include "../../utilities/time/Date.hpp"
#include "../../utilities/time/Time.hpp"

using namespace openstudio::model;
using namespace openstudio;

TEST_F(ModelFixture, Schedule_Year)
{
  Model model;

  openstudio::model::YearDescription yd = model.getUniqueModelObject<openstudio::model::YearDescription>();

  ScheduleWeek weekSchedule1(model);
  ScheduleWeek weekSchedule2(model);
  ScheduleWeek weekSchedule3(model);

  // clear the schedule
  ScheduleYear yearSchedule(model);

  EXPECT_EQ(0u, yearSchedule.dates().size());
  EXPECT_EQ(0u, yearSchedule.scheduleWeeks().size());

  EXPECT_FALSE(yearSchedule.getScheduleWeek(yd.makeDate(1,1)));
  EXPECT_FALSE(yearSchedule.getScheduleWeek(yd.makeDate(6,1)));
  EXPECT_FALSE(yearSchedule.getScheduleWeek(yd.makeDate(12,31)));

  // until 3/1 weekSchedule1
  EXPECT_TRUE(yearSchedule.addScheduleWeek(yd.makeDate(3,1), weekSchedule1));

  ASSERT_EQ(1u, yearSchedule.dates().size());
  EXPECT_EQ(yd.makeDate(3,1), yearSchedule.dates()[0]);
  ASSERT_EQ(1u, yearSchedule.scheduleWeeks().size());
  EXPECT_EQ(weekSchedule1.handle(), yearSchedule.scheduleWeeks()[0].handle());

  ASSERT_TRUE(yearSchedule.getScheduleWeek(yd.makeDate(1,1)));
  EXPECT_EQ(weekSchedule1.handle(), yearSchedule.getScheduleWeek(yd.makeDate(1,1))->handle());
  EXPECT_FALSE(yearSchedule.getScheduleWeek(yd.makeDate(6,1)));
  EXPECT_FALSE(yearSchedule.getScheduleWeek(yd.makeDate(12,31)));

  // until 3/1 weekSchedule1, until 7/1 weekSchedule2
  EXPECT_TRUE(yearSchedule.addScheduleWeek(yd.makeDate(7,1), weekSchedule2));

  ASSERT_EQ(2u, yearSchedule.dates().size());
  EXPECT_EQ(yd.makeDate(3,1), yearSchedule.dates()[0]);
  EXPECT_EQ(yd.makeDate(7,1), yearSchedule.dates()[1]);
  ASSERT_EQ(2u, yearSchedule.scheduleWeeks().size());
  EXPECT_EQ(weekSchedule1.handle(), yearSchedule.scheduleWeeks()[0].handle());
  EXPECT_EQ(weekSchedule2.handle(), yearSchedule.scheduleWeeks()[1].handle());

  ASSERT_TRUE(yearSchedule.getScheduleWeek(yd.makeDate(1,1)));
  EXPECT_EQ(weekSchedule1.handle(), yearSchedule.getScheduleWeek(yd.makeDate(1,1))->handle());
  ASSERT_TRUE(yearSchedule.getScheduleWeek(yd.makeDate(6,1)));
  EXPECT_EQ(weekSchedule2.handle(), yearSchedule.getScheduleWeek(yd.makeDate(6,1))->handle());
  EXPECT_FALSE(yearSchedule.getScheduleWeek(yd.makeDate(12,31)));

  // until 3/1 weekSchedule1, until 7/1 weekSchedule2, until 12/31 weekSchedule3
  EXPECT_TRUE(yearSchedule.addScheduleWeek(yd.makeDate(12,31), weekSchedule3));

  ASSERT_EQ(3u, yearSchedule.dates().size());
  EXPECT_EQ(yd.makeDate(3,1), yearSchedule.dates()[0]);
  EXPECT_EQ(yd.makeDate(7,1), yearSchedule.dates()[1]);
  EXPECT_EQ(yd.makeDate(12,31), yearSchedule.dates()[2]);
  ASSERT_EQ(3u, yearSchedule.scheduleWeeks().size());
  EXPECT_EQ(weekSchedule1.handle(), yearSchedule.scheduleWeeks()[0].handle());
  EXPECT_EQ(weekSchedule2.handle(), yearSchedule.scheduleWeeks()[1].handle());
  EXPECT_EQ(weekSchedule3.handle(), yearSchedule.scheduleWeeks()[2].handle());

  ASSERT_TRUE(yearSchedule.getScheduleWeek(yd.makeDate(1,1)));
  EXPECT_EQ(weekSchedule1.handle(), yearSchedule.getScheduleWeek(yd.makeDate(1,1))->handle());
  ASSERT_TRUE(yearSchedule.getScheduleWeek(yd.makeDate(6,1)));
  EXPECT_EQ(weekSchedule2.handle(), yearSchedule.getScheduleWeek(yd.makeDate(6,1))->handle());
  ASSERT_TRUE(yearSchedule.getScheduleWeek(yd.makeDate(12,31)));
  EXPECT_EQ(weekSchedule3.handle(), yearSchedule.getScheduleWeek(yd.makeDate(12,31))->handle());

  // clear the schedule
  yearSchedule.clearScheduleWeeks();
  EXPECT_FALSE(yearSchedule.getScheduleWeek(yd.makeDate(1,1)));
  EXPECT_FALSE(yearSchedule.getScheduleWeek(yd.makeDate(6,1)));
  EXPECT_FALSE(yearSchedule.getScheduleWeek(yd.makeDate(12,31)));

  // until 12/31 weekSchedule3
  EXPECT_TRUE(yearSchedule.addScheduleWeek(yd.makeDate(12,31), weekSchedule3));

  ASSERT_EQ(1u, yearSchedule.dates().size());
  EXPECT_EQ(yd.makeDate(12,31), yearSchedule.dates()[0]);
  ASSERT_EQ(1u, yearSchedule.scheduleWeeks().size());
  EXPECT_EQ(weekSchedule3.handle(), yearSchedule.scheduleWeeks()[0].handle());

  ASSERT_TRUE(yearSchedule.getScheduleWeek(yd.makeDate(1,1)));
  EXPECT_EQ(weekSchedule3.handle(), yearSchedule.getScheduleWeek(yd.makeDate(1,1))->handle());
  ASSERT_TRUE(yearSchedule.getScheduleWeek(yd.makeDate(6,1)));
  EXPECT_EQ(weekSchedule3.handle(), yearSchedule.getScheduleWeek(yd.makeDate(6,1))->handle());
  ASSERT_TRUE(yearSchedule.getScheduleWeek(yd.makeDate(12,31)));
  EXPECT_EQ(weekSchedule3.handle(), yearSchedule.getScheduleWeek(yd.makeDate(12,31))->handle());

  // until 7/1 weekSchedule2, until 12/31 weekSchedule3
  EXPECT_TRUE(yearSchedule.addScheduleWeek(yd.makeDate(7,1), weekSchedule2));

  ASSERT_EQ(2u, yearSchedule.dates().size());
  EXPECT_EQ(yd.makeDate(7,1), yearSchedule.dates()[0]);
  EXPECT_EQ(yd.makeDate(12,31), yearSchedule.dates()[1]);
  ASSERT_EQ(2u, yearSchedule.scheduleWeeks().size());
  EXPECT_EQ(weekSchedule2.handle(), yearSchedule.scheduleWeeks()[0].handle());
  EXPECT_EQ(weekSchedule3.handle(), yearSchedule.scheduleWeeks()[1].handle());

  ASSERT_TRUE(yearSchedule.getScheduleWeek(yd.makeDate(1,1)));
  EXPECT_EQ(weekSchedule2.handle(), yearSchedule.getScheduleWeek(yd.makeDate(1,1))->handle());
  ASSERT_TRUE(yearSchedule.getScheduleWeek(yd.makeDate(6,1)));
  EXPECT_EQ(weekSchedule2.handle(), yearSchedule.getScheduleWeek(yd.makeDate(6,1))->handle());
  ASSERT_TRUE(yearSchedule.getScheduleWeek(yd.makeDate(12,31)));
  EXPECT_EQ(weekSchedule3.handle(), yearSchedule.getScheduleWeek(yd.makeDate(12,31))->handle());

  // until 3/1 weekSchedule1, until 7/1 weekSchedule2, until 12/31 weekSchedule3
  EXPECT_TRUE(yearSchedule.addScheduleWeek(yd.makeDate(3,1), weekSchedule1));

  ASSERT_EQ(3u, yearSchedule.dates().size());
  EXPECT_EQ(yd.makeDate(3,1), yearSchedule.dates()[0]);
  EXPECT_EQ(yd.makeDate(7,1), yearSchedule.dates()[1]);
  EXPECT_EQ(yd.makeDate(12,31), yearSchedule.dates()[2]);
  ASSERT_EQ(3u, yearSchedule.scheduleWeeks().size());
  EXPECT_EQ(weekSchedule1.handle(), yearSchedule.scheduleWeeks()[0].handle());
  EXPECT_EQ(weekSchedule2.handle(), yearSchedule.scheduleWeeks()[1].handle());
  EXPECT_EQ(weekSchedule3.handle(), yearSchedule.scheduleWeeks()[2].handle());

  ASSERT_TRUE(yearSchedule.getScheduleWeek(yd.makeDate(1,1)));
  EXPECT_EQ(weekSchedule1.handle(), yearSchedule.getScheduleWeek(yd.makeDate(1,1))->handle());
  ASSERT_TRUE(yearSchedule.getScheduleWeek(yd.makeDate(6,1)));
  EXPECT_EQ(weekSchedule2.handle(), yearSchedule.getScheduleWeek(yd.makeDate(6,1))->handle());
  ASSERT_TRUE(yearSchedule.getScheduleWeek(yd.makeDate(12,31)));
  EXPECT_EQ(weekSchedule3.handle(), yearSchedule.getScheduleWeek(yd.makeDate(12,31))->handle());
}
