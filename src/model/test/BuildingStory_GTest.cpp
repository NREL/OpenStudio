/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "../BuildingStory.hpp"
#include "../BuildingStory_Impl.hpp"

#include "../Building.hpp"
#include "../SpaceType.hpp"
#include "../DefaultScheduleSet.hpp"
#include "../ScheduleConstant.hpp"

#include "../../utilities/units/Quantity.hpp"
#include "../../utilities/units/Unit.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, BuildingStory_NominalZCoordinate) {
  Model model;
  BuildingStory buildingStory(model);

  double value(1.0);
  buildingStory.setNominalZCoordinate(value);
  boost::optional<double> result = buildingStory.nominalZCoordinate();
  ASSERT_TRUE(result.is_initialized());
  EXPECT_NEAR(value, result.get(), 1.0E-8);
}

TEST_F(ModelFixture, BuildingStory_NominalFloortoFloorHeight) {
  Model model;

  BuildingStory buildingStory(model);
  EXPECT_FALSE(buildingStory.nominalFloortoFloorHeight());

  double value(1.0);
  EXPECT_TRUE(buildingStory.setNominalFloortoFloorHeight(value));
  boost::optional<double> result = buildingStory.nominalFloortoFloorHeight();
  ASSERT_TRUE(result);
  EXPECT_NEAR(value, *result, 1.0E-8);
}

TEST_F(ModelFixture, BuildingStory_getDefaultSchedule) {
  Model model;

  BuildingStory buildingStory(model);
  DefaultScheduleSet bldgStoryDefaultScheduleSet(model);
  ScheduleConstant sch_bs_lighting(model);
  EXPECT_TRUE(bldgStoryDefaultScheduleSet.setLightingSchedule(sch_bs_lighting));
  EXPECT_TRUE(buildingStory.setDefaultScheduleSet(bldgStoryDefaultScheduleSet));

  Building building = model.getUniqueModelObject<Building>();
  DefaultScheduleSet bldgDefaultScheduleSet(model);
  ScheduleConstant sch_bldg_lighting(model);
  EXPECT_TRUE(bldgDefaultScheduleSet.setLightingSchedule(sch_bldg_lighting));
  ScheduleConstant sch_bldg_people(model);
  EXPECT_TRUE(bldgDefaultScheduleSet.setNumberofPeopleSchedule(sch_bldg_people));
  EXPECT_TRUE(building.setDefaultScheduleSet(bldgDefaultScheduleSet));

  SpaceType spaceType(model);
  EXPECT_TRUE(building.setSpaceType(spaceType));

  DefaultScheduleSet spDefaultScheduleSet(model);
  ScheduleConstant sch_sp_lighting(model);
  EXPECT_TRUE(spDefaultScheduleSet.setLightingSchedule(sch_sp_lighting));
  ScheduleConstant sch_sp_people(model);
  EXPECT_TRUE(spDefaultScheduleSet.setNumberofPeopleSchedule(sch_sp_people));
  ScheduleConstant sch_sp_hours(model);
  EXPECT_TRUE(spDefaultScheduleSet.setHoursofOperationSchedule(sch_sp_hours));
  EXPECT_TRUE(spaceType.setDefaultScheduleSet(spDefaultScheduleSet));

  // BuildingStory, Building and its SpaceType both have a lighting schedule. It should return the BuildingStory's one in priority
  ASSERT_TRUE(buildingStory.getDefaultSchedule(DefaultScheduleType::LightingSchedule));
  EXPECT_EQ(sch_bs_lighting.handle(), buildingStory.getDefaultSchedule(DefaultScheduleType::LightingSchedule)->handle());

  // BuldingStory doesn't have one, but Building and its SpaceType both have a people schedule. It should return the building's one in priority
  ASSERT_TRUE(buildingStory.getDefaultSchedule(DefaultScheduleType::NumberofPeopleSchedule));
  EXPECT_EQ(sch_bldg_people.handle(), buildingStory.getDefaultSchedule(DefaultScheduleType::NumberofPeopleSchedule)->handle());

  // BuldingStory and Building don't have an hours of operation schedule, but its SpaceType does so it should return the SpaceType's one
  ASSERT_TRUE(buildingStory.getDefaultSchedule(DefaultScheduleType::HoursofOperationSchedule));
  EXPECT_EQ(sch_sp_hours.handle(), buildingStory.getDefaultSchedule(DefaultScheduleType::HoursofOperationSchedule)->handle());
}
