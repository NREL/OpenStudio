/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
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

