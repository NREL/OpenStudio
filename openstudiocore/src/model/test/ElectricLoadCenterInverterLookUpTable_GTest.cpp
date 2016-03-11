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

#include "../ElectricLoadCenterInverterLookUpTable.hpp"
#include "../Schedule.hpp"
#include "../ScheduleCompact.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, ElectricLoadCenterInverterLookUpTable_Instantiate) {
  auto result = false;
  auto value = 0.1;

  Model model;
  ThermalZone thermalZone(model);

  ElectricLoadCenterInverterLookUpTable electricLoadCenterInverterLookUpTable(model);

  EXPECT_FALSE(electricLoadCenterInverterLookUpTable.thermalZone());
  EXPECT_TRUE(electricLoadCenterInverterLookUpTable.setThermalZone(thermalZone));
  ASSERT_TRUE(electricLoadCenterInverterLookUpTable.thermalZone());

  EXPECT_FALSE(electricLoadCenterInverterLookUpTable.availabilitySchedule());
  ScheduleCompact scheduleCompact(model);
  result = electricLoadCenterInverterLookUpTable.setAvailabilitySchedule(scheduleCompact);
  EXPECT_TRUE(result);
  EXPECT_TRUE(electricLoadCenterInverterLookUpTable.availabilitySchedule());
  electricLoadCenterInverterLookUpTable.resetAvailabilitySchedule();
  EXPECT_FALSE(electricLoadCenterInverterLookUpTable.availabilitySchedule());

  EXPECT_FALSE(electricLoadCenterInverterLookUpTable.radiativeFraction());
  result = electricLoadCenterInverterLookUpTable.setRadiativeFraction(value);
  EXPECT_TRUE(result);
  EXPECT_TRUE(electricLoadCenterInverterLookUpTable.radiativeFraction());
  EXPECT_EQ(electricLoadCenterInverterLookUpTable.radiativeFraction().get(), value);
  electricLoadCenterInverterLookUpTable.resetRadiativeFraction();
  EXPECT_FALSE(electricLoadCenterInverterLookUpTable.radiativeFraction());

  EXPECT_FALSE(electricLoadCenterInverterLookUpTable.ratedMaximumContinuousOutputPower());
  electricLoadCenterInverterLookUpTable.setRatedMaximumContinuousOutputPower(value);
  EXPECT_TRUE(electricLoadCenterInverterLookUpTable.ratedMaximumContinuousOutputPower());
  EXPECT_EQ(electricLoadCenterInverterLookUpTable.ratedMaximumContinuousOutputPower().get(), value);
  electricLoadCenterInverterLookUpTable.resetRatedMaximumContinuousOutputPower();
  EXPECT_FALSE(electricLoadCenterInverterLookUpTable.ratedMaximumContinuousOutputPower());

  EXPECT_FALSE(electricLoadCenterInverterLookUpTable.nightTareLossPower());
  electricLoadCenterInverterLookUpTable.setNightTareLossPower(value);
  EXPECT_TRUE(electricLoadCenterInverterLookUpTable.nightTareLossPower());
  EXPECT_EQ(electricLoadCenterInverterLookUpTable.nightTareLossPower().get(), value);
  electricLoadCenterInverterLookUpTable.resetNightTareLossPower();
  EXPECT_FALSE(electricLoadCenterInverterLookUpTable.nightTareLossPower());

  EXPECT_FALSE(electricLoadCenterInverterLookUpTable.nominalVoltageInput());
  electricLoadCenterInverterLookUpTable.setNominalVoltageInput(value);
  EXPECT_TRUE(electricLoadCenterInverterLookUpTable.nominalVoltageInput());
  EXPECT_EQ(electricLoadCenterInverterLookUpTable.nominalVoltageInput().get(), value);
  electricLoadCenterInverterLookUpTable.resetNominalVoltageInput();
  EXPECT_FALSE(electricLoadCenterInverterLookUpTable.nominalVoltageInput());

  EXPECT_FALSE(electricLoadCenterInverterLookUpTable.efficiencyAt10PowerAndNominalVoltage());
  result = electricLoadCenterInverterLookUpTable.setEfficiencyAt10PowerAndNominalVoltage(value);
  EXPECT_TRUE(result);
  EXPECT_TRUE(electricLoadCenterInverterLookUpTable.efficiencyAt10PowerAndNominalVoltage());
  EXPECT_EQ(electricLoadCenterInverterLookUpTable.efficiencyAt10PowerAndNominalVoltage().get(), value);
  electricLoadCenterInverterLookUpTable.resetEfficiencyAt10PowerAndNominalVoltage();
  EXPECT_FALSE(electricLoadCenterInverterLookUpTable.efficiencyAt10PowerAndNominalVoltage());

  EXPECT_FALSE(electricLoadCenterInverterLookUpTable.efficiencyAt20PowerAndNominalVoltage());
  result = electricLoadCenterInverterLookUpTable.setEfficiencyAt20PowerAndNominalVoltage(value);
  EXPECT_TRUE(result);
  EXPECT_TRUE(electricLoadCenterInverterLookUpTable.efficiencyAt20PowerAndNominalVoltage());
  EXPECT_EQ(electricLoadCenterInverterLookUpTable.efficiencyAt20PowerAndNominalVoltage().get(), value);
  electricLoadCenterInverterLookUpTable.resetEfficiencyAt20PowerAndNominalVoltage();
  EXPECT_FALSE(electricLoadCenterInverterLookUpTable.efficiencyAt20PowerAndNominalVoltage());

  EXPECT_FALSE(electricLoadCenterInverterLookUpTable.efficiencyAt30PowerAndNominalVoltage());
  result = electricLoadCenterInverterLookUpTable.setEfficiencyAt30PowerAndNominalVoltage(value);
  EXPECT_TRUE(result);
  EXPECT_TRUE(electricLoadCenterInverterLookUpTable.efficiencyAt30PowerAndNominalVoltage());
  EXPECT_EQ(electricLoadCenterInverterLookUpTable.efficiencyAt30PowerAndNominalVoltage().get(), value);
  electricLoadCenterInverterLookUpTable.resetEfficiencyAt30PowerAndNominalVoltage();
  EXPECT_FALSE(electricLoadCenterInverterLookUpTable.efficiencyAt30PowerAndNominalVoltage());

  EXPECT_FALSE(electricLoadCenterInverterLookUpTable.efficiencyAt50PowerAndNominalVoltage());
  result = electricLoadCenterInverterLookUpTable.setEfficiencyAt50PowerAndNominalVoltage(value);
  EXPECT_TRUE(result);
  EXPECT_TRUE(electricLoadCenterInverterLookUpTable.efficiencyAt50PowerAndNominalVoltage());
  EXPECT_EQ(electricLoadCenterInverterLookUpTable.efficiencyAt50PowerAndNominalVoltage().get(), value);
  electricLoadCenterInverterLookUpTable.resetEfficiencyAt50PowerAndNominalVoltage();
  EXPECT_FALSE(electricLoadCenterInverterLookUpTable.efficiencyAt50PowerAndNominalVoltage());

  EXPECT_FALSE(electricLoadCenterInverterLookUpTable.efficiencyAt75PowerAndNominalVoltage());
  result = electricLoadCenterInverterLookUpTable.setEfficiencyAt75PowerAndNominalVoltage(value);
  EXPECT_TRUE(result);
  EXPECT_TRUE(electricLoadCenterInverterLookUpTable.efficiencyAt75PowerAndNominalVoltage());
  EXPECT_EQ(electricLoadCenterInverterLookUpTable.efficiencyAt75PowerAndNominalVoltage().get(), value);
  electricLoadCenterInverterLookUpTable.resetEfficiencyAt75PowerAndNominalVoltage();
  EXPECT_FALSE(electricLoadCenterInverterLookUpTable.efficiencyAt75PowerAndNominalVoltage());
 
  EXPECT_FALSE(electricLoadCenterInverterLookUpTable.efficiencyAt100PowerAndNominalVoltage());
  result = electricLoadCenterInverterLookUpTable.setEfficiencyAt100PowerAndNominalVoltage(value);
  EXPECT_TRUE(result);
  EXPECT_TRUE(electricLoadCenterInverterLookUpTable.efficiencyAt100PowerAndNominalVoltage());
  EXPECT_EQ(electricLoadCenterInverterLookUpTable.efficiencyAt100PowerAndNominalVoltage().get(), value);
  electricLoadCenterInverterLookUpTable.resetEfficiencyAt100PowerAndNominalVoltage();
  EXPECT_FALSE(electricLoadCenterInverterLookUpTable.efficiencyAt100PowerAndNominalVoltage());

}

