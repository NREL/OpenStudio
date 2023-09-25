/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../ElectricLoadCenterInverterSimple.hpp"
#include "../Schedule.hpp"
#include "../ScheduleCompact.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, ElectricLoadCenterInverterSimple_Instantiate) {
  auto result = false;
  auto value = 0.1;

  Model model;
  ThermalZone thermalZone(model);

  ElectricLoadCenterInverterSimple electricLoadCenterInverterSimple(model);

  EXPECT_FALSE(electricLoadCenterInverterSimple.thermalZone());
  EXPECT_TRUE(electricLoadCenterInverterSimple.setThermalZone(thermalZone));
  ASSERT_TRUE(electricLoadCenterInverterSimple.thermalZone());

  EXPECT_FALSE(electricLoadCenterInverterSimple.availabilitySchedule());
  ScheduleCompact scheduleCompact(model);
  result = electricLoadCenterInverterSimple.setAvailabilitySchedule(scheduleCompact);
  EXPECT_TRUE(result);
  EXPECT_TRUE(electricLoadCenterInverterSimple.availabilitySchedule());
  electricLoadCenterInverterSimple.resetAvailabilitySchedule();
  EXPECT_FALSE(electricLoadCenterInverterSimple.availabilitySchedule());

  EXPECT_FALSE(electricLoadCenterInverterSimple.radiativeFraction());
  result = electricLoadCenterInverterSimple.setRadiativeFraction(value);
  EXPECT_TRUE(result);
  EXPECT_TRUE(electricLoadCenterInverterSimple.radiativeFraction());
  EXPECT_EQ(electricLoadCenterInverterSimple.radiativeFraction().get(), value);
  electricLoadCenterInverterSimple.resetRadiativeFraction();
  EXPECT_FALSE(electricLoadCenterInverterSimple.radiativeFraction());

  EXPECT_FALSE(electricLoadCenterInverterSimple.inverterEfficiency());
  result = electricLoadCenterInverterSimple.setInverterEfficiency(value);
  EXPECT_TRUE(result);
  EXPECT_TRUE(electricLoadCenterInverterSimple.inverterEfficiency());
  EXPECT_EQ(electricLoadCenterInverterSimple.inverterEfficiency().get(), value);
  electricLoadCenterInverterSimple.resetInverterEfficiency();
  EXPECT_FALSE(electricLoadCenterInverterSimple.inverterEfficiency());
}
