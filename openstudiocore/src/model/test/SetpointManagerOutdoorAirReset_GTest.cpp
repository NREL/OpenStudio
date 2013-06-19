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

#include <model/SetpointManagerOutdoorAirReset.hpp>
#include <model/SetpointManagerOutdoorAirReset_Impl.hpp>

#include <utilities/units/Quantity.hpp>
#include <utilities/units/Unit.hpp>

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture,SetpointManagerOutdoorAirReset_SetpointatOutdoorLowTemperature_Quantity) {
  Model model;
  // TODO: Check constructor.
  SetpointManagerOutdoorAirReset setpointManagerOutdoorAirReset(model);

  Unit units = setpointManagerOutdoorAirReset.getSetpointatOutdoorLowTemperature(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(setpointManagerOutdoorAirReset.setSetpointatOutdoorLowTemperature(testQ));
  Quantity q = setpointManagerOutdoorAirReset.getSetpointatOutdoorLowTemperature(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,SetpointManagerOutdoorAirReset_OutdoorLowTemperature_Quantity) {
  Model model;
  // TODO: Check constructor.
  SetpointManagerOutdoorAirReset setpointManagerOutdoorAirReset(model);

  Unit units = setpointManagerOutdoorAirReset.getOutdoorLowTemperature(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(setpointManagerOutdoorAirReset.setOutdoorLowTemperature(testQ));
  Quantity q = setpointManagerOutdoorAirReset.getOutdoorLowTemperature(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,SetpointManagerOutdoorAirReset_SetpointatOutdoorHighTemperature_Quantity) {
  Model model;
  // TODO: Check constructor.
  SetpointManagerOutdoorAirReset setpointManagerOutdoorAirReset(model);

  Unit units = setpointManagerOutdoorAirReset.getSetpointatOutdoorHighTemperature(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(setpointManagerOutdoorAirReset.setSetpointatOutdoorHighTemperature(testQ));
  Quantity q = setpointManagerOutdoorAirReset.getSetpointatOutdoorHighTemperature(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,SetpointManagerOutdoorAirReset_OutdoorHighTemperature_Quantity) {
  Model model;
  // TODO: Check constructor.
  SetpointManagerOutdoorAirReset setpointManagerOutdoorAirReset(model);

  Unit units = setpointManagerOutdoorAirReset.getOutdoorHighTemperature(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(setpointManagerOutdoorAirReset.setOutdoorHighTemperature(testQ));
  Quantity q = setpointManagerOutdoorAirReset.getOutdoorHighTemperature(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,SetpointManagerOutdoorAirReset_SetpointatOutdoorLowTemperature2_Quantity) {
  Model model;
  // TODO: Check constructor.
  SetpointManagerOutdoorAirReset setpointManagerOutdoorAirReset(model);

  Unit units = setpointManagerOutdoorAirReset.getSetpointatOutdoorLowTemperature2(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(setpointManagerOutdoorAirReset.setSetpointatOutdoorLowTemperature2(testQ));
  OSOptionalQuantity q = setpointManagerOutdoorAirReset.getSetpointatOutdoorLowTemperature2(true);
  ASSERT_TRUE(q.isSet());
  EXPECT_NEAR(value,q.get().value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,SetpointManagerOutdoorAirReset_OutdoorLowTemperature2_Quantity) {
  Model model;
  // TODO: Check constructor.
  SetpointManagerOutdoorAirReset setpointManagerOutdoorAirReset(model);

  Unit units = setpointManagerOutdoorAirReset.getOutdoorLowTemperature2(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(setpointManagerOutdoorAirReset.setOutdoorLowTemperature2(testQ));
  OSOptionalQuantity q = setpointManagerOutdoorAirReset.getOutdoorLowTemperature2(true);
  ASSERT_TRUE(q.isSet());
  EXPECT_NEAR(value,q.get().value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,SetpointManagerOutdoorAirReset_SetpointatOutdoorHighTemperature2_Quantity) {
  Model model;
  // TODO: Check constructor.
  SetpointManagerOutdoorAirReset setpointManagerOutdoorAirReset(model);

  Unit units = setpointManagerOutdoorAirReset.getSetpointatOutdoorHighTemperature2(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(setpointManagerOutdoorAirReset.setSetpointatOutdoorHighTemperature2(testQ));
  OSOptionalQuantity q = setpointManagerOutdoorAirReset.getSetpointatOutdoorHighTemperature2(true);
  ASSERT_TRUE(q.isSet());
  EXPECT_NEAR(value,q.get().value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,SetpointManagerOutdoorAirReset_OutdoorHighTemperature2_Quantity) {
  Model model;
  // TODO: Check constructor.
  SetpointManagerOutdoorAirReset setpointManagerOutdoorAirReset(model);

  Unit units = setpointManagerOutdoorAirReset.getOutdoorHighTemperature2(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(setpointManagerOutdoorAirReset.setOutdoorHighTemperature2(testQ));
  OSOptionalQuantity q = setpointManagerOutdoorAirReset.getOutdoorHighTemperature2(true);
  ASSERT_TRUE(q.isSet());
  EXPECT_NEAR(value,q.get().value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

