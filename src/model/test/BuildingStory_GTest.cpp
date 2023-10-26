/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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
