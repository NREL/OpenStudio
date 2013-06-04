/**********************************************************************
 *  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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

#include <model/test/ModelFixture.hpp>

#include <model/AirTerminalSingleDuctConstantVolumeReheat.hpp>
#include <model/AirTerminalSingleDuctConstantVolumeReheat_Impl.hpp>

#include <utilities/units/Quantity.hpp>
#include <utilities/units/Unit.hpp>

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture,AirTerminalSingleDuctConstantVolumeReheat_MaximumAirFlowRate_Quantity) {
  Model model;
  // TODO: Check constructor.
  AirTerminalSingleDuctConstantVolumeReheat airTerminalSingleDuctConstantVolumeReheat(model);

  Unit units = airTerminalSingleDuctConstantVolumeReheat.getMaximumAirFlowRate(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(airTerminalSingleDuctConstantVolumeReheat.setMaximumAirFlowRate(testQ));
  OSOptionalQuantity q = airTerminalSingleDuctConstantVolumeReheat.getMaximumAirFlowRate(true);
  ASSERT_TRUE(q.isSet());
  EXPECT_NEAR(value,q.get().value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,AirTerminalSingleDuctConstantVolumeReheat_MaximumHotWaterorSteamFlowRate_Quantity) {
  Model model;
  // TODO: Check constructor.
  AirTerminalSingleDuctConstantVolumeReheat airTerminalSingleDuctConstantVolumeReheat(model);

  Unit units = airTerminalSingleDuctConstantVolumeReheat.getMaximumHotWaterorSteamFlowRate(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(airTerminalSingleDuctConstantVolumeReheat.setMaximumHotWaterorSteamFlowRate(testQ));
  OSOptionalQuantity q = airTerminalSingleDuctConstantVolumeReheat.getMaximumHotWaterorSteamFlowRate(true);
  ASSERT_TRUE(q.isSet());
  EXPECT_NEAR(value,q.get().value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,AirTerminalSingleDuctConstantVolumeReheat_MinimumHotWaterorSteamFlowRate_Quantity) {
  Model model;
  // TODO: Check constructor.
  AirTerminalSingleDuctConstantVolumeReheat airTerminalSingleDuctConstantVolumeReheat(model);

  Unit units = airTerminalSingleDuctConstantVolumeReheat.getMinimumHotWaterorSteamFlowRate(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(airTerminalSingleDuctConstantVolumeReheat.setMinimumHotWaterorSteamFlowRate(testQ));
  Quantity q = airTerminalSingleDuctConstantVolumeReheat.getMinimumHotWaterorSteamFlowRate(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,AirTerminalSingleDuctConstantVolumeReheat_ConvergenceTolerance_Quantity) {
  Model model;
  // TODO: Check constructor.
  AirTerminalSingleDuctConstantVolumeReheat airTerminalSingleDuctConstantVolumeReheat(model);

  Unit units = airTerminalSingleDuctConstantVolumeReheat.getConvergenceTolerance(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(airTerminalSingleDuctConstantVolumeReheat.setConvergenceTolerance(testQ));
  Quantity q = airTerminalSingleDuctConstantVolumeReheat.getConvergenceTolerance(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,AirTerminalSingleDuctConstantVolumeReheat_MaximumReheatAirTemperature_Quantity) {
  Model model;
  // TODO: Check constructor.
  AirTerminalSingleDuctConstantVolumeReheat airTerminalSingleDuctConstantVolumeReheat(model);

  Unit units = airTerminalSingleDuctConstantVolumeReheat.getMaximumReheatAirTemperature(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(airTerminalSingleDuctConstantVolumeReheat.setMaximumReheatAirTemperature(testQ));
  Quantity q = airTerminalSingleDuctConstantVolumeReheat.getMaximumReheatAirTemperature(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

