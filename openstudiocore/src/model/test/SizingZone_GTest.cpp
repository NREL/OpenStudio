/**********************************************************************
 *  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
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

#include "../SizingZone.hpp"
#include "../SizingZone_Impl.hpp"
#include "../ThermalZone.hpp"

#include "../../utilities/units/Quantity.hpp"
#include "../../utilities/units/Unit.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture,SizingZone_ZoneCoolingDesignSupplyAirTemperature_Quantity) {
  Model model;
  ThermalZone zone(model);
  SizingZone sizingZone(model,zone);

  Unit units = sizingZone.getZoneCoolingDesignSupplyAirTemperature(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(sizingZone.setZoneCoolingDesignSupplyAirTemperature(testQ));
  Quantity q = sizingZone.getZoneCoolingDesignSupplyAirTemperature(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,SizingZone_ZoneHeatingDesignSupplyAirTemperature_Quantity) {
  Model model;
  ThermalZone zone(model);
  SizingZone sizingZone(model,zone);

  Unit units = sizingZone.getZoneHeatingDesignSupplyAirTemperature(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(sizingZone.setZoneHeatingDesignSupplyAirTemperature(testQ));
  Quantity q = sizingZone.getZoneHeatingDesignSupplyAirTemperature(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,SizingZone_ZoneCoolingDesignSupplyAirHumidityRatio_Quantity) {
  Model model;
  ThermalZone zone(model);
  SizingZone sizingZone(model,zone);

  Unit units = sizingZone.getZoneCoolingDesignSupplyAirHumidityRatio(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(sizingZone.setZoneCoolingDesignSupplyAirHumidityRatio(testQ));
  Quantity q = sizingZone.getZoneCoolingDesignSupplyAirHumidityRatio(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,SizingZone_ZoneHeatingDesignSupplyAirHumidityRatio_Quantity) {
  Model model;
  ThermalZone zone(model);
  SizingZone sizingZone(model,zone);

  Unit units = sizingZone.getZoneHeatingDesignSupplyAirHumidityRatio(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(sizingZone.setZoneHeatingDesignSupplyAirHumidityRatio(testQ));
  Quantity q = sizingZone.getZoneHeatingDesignSupplyAirHumidityRatio(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,SizingZone_ZoneHeatingSizingFactor_Quantity) {
  Model model;
  ThermalZone zone(model);
  SizingZone sizingZone(model,zone);

  Unit units = sizingZone.getZoneHeatingSizingFactor(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(sizingZone.setZoneHeatingSizingFactor(testQ));
  OSOptionalQuantity q = sizingZone.getZoneHeatingSizingFactor(true);
  ASSERT_TRUE(q.isSet());
  EXPECT_NEAR(value,q.get().value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,SizingZone_ZoneCoolingSizingFactor_Quantity) {
  Model model;
  ThermalZone zone(model);
  SizingZone sizingZone(model,zone);

  Unit units = sizingZone.getZoneCoolingSizingFactor(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(sizingZone.setZoneCoolingSizingFactor(testQ));
  OSOptionalQuantity q = sizingZone.getZoneCoolingSizingFactor(true);
  ASSERT_TRUE(q.isSet());
  EXPECT_NEAR(value,q.get().value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,SizingZone_CoolingDesignAirFlowRate_Quantity) {
  Model model;
  ThermalZone zone(model);
  SizingZone sizingZone(model,zone);

  Unit units = sizingZone.getCoolingDesignAirFlowRate(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(sizingZone.setCoolingDesignAirFlowRate(testQ));
  Quantity q = sizingZone.getCoolingDesignAirFlowRate(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,SizingZone_CoolingMinimumAirFlowperZoneFloorArea_Quantity) {
  Model model;
  ThermalZone zone(model);
  SizingZone sizingZone(model,zone);

  Unit units = sizingZone.getCoolingMinimumAirFlowperZoneFloorArea(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(sizingZone.setCoolingMinimumAirFlowperZoneFloorArea(testQ));
  Quantity q = sizingZone.getCoolingMinimumAirFlowperZoneFloorArea(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,SizingZone_CoolingMinimumAirFlow_Quantity) {
  Model model;
  ThermalZone zone(model);
  SizingZone sizingZone(model,zone);

  Unit units = sizingZone.getCoolingMinimumAirFlow(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(sizingZone.setCoolingMinimumAirFlow(testQ));
  Quantity q = sizingZone.getCoolingMinimumAirFlow(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,SizingZone_CoolingMinimumAirFlowFraction_Quantity) {
  Model model;
  ThermalZone zone(model);
  SizingZone sizingZone(model,zone);

  Unit units = sizingZone.getCoolingMinimumAirFlowFraction(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(sizingZone.setCoolingMinimumAirFlowFraction(testQ));
  Quantity q = sizingZone.getCoolingMinimumAirFlowFraction(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,SizingZone_HeatingDesignAirFlowRate_Quantity) {
  Model model;
  ThermalZone zone(model);
  SizingZone sizingZone(model,zone);

  Unit units = sizingZone.getHeatingDesignAirFlowRate(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(sizingZone.setHeatingDesignAirFlowRate(testQ));
  Quantity q = sizingZone.getHeatingDesignAirFlowRate(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,SizingZone_HeatingMaximumAirFlowperZoneFloorArea_Quantity) {
  Model model;
  ThermalZone zone(model);
  SizingZone sizingZone(model,zone);

  Unit units = sizingZone.getHeatingMaximumAirFlowperZoneFloorArea(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(sizingZone.setHeatingMaximumAirFlowperZoneFloorArea(testQ));
  Quantity q = sizingZone.getHeatingMaximumAirFlowperZoneFloorArea(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,SizingZone_HeatingMaximumAirFlow_Quantity) {
  Model model;
  ThermalZone zone(model);
  SizingZone sizingZone(model,zone);

  Unit units = sizingZone.getHeatingMaximumAirFlow(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(sizingZone.setHeatingMaximumAirFlow(testQ));
  Quantity q = sizingZone.getHeatingMaximumAirFlow(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,SizingZone_HeatingMaximumAirFlowFraction_Quantity) {
  Model model;
  ThermalZone zone(model);
  SizingZone sizingZone(model,zone);

  Unit units = sizingZone.getHeatingMaximumAirFlowFraction(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(sizingZone.setHeatingMaximumAirFlowFraction(testQ));
  Quantity q = sizingZone.getHeatingMaximumAirFlowFraction(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,SizingZone_DesignZoneAirDistributionEffectivenessinCoolingMode_Quantity) {
  Model model;
  ThermalZone zone(model);
  SizingZone sizingZone(model,zone);

  Unit units = sizingZone.getDesignZoneAirDistributionEffectivenessinCoolingMode(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(sizingZone.setDesignZoneAirDistributionEffectivenessinCoolingMode(testQ));
  Quantity q = sizingZone.getDesignZoneAirDistributionEffectivenessinCoolingMode(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,SizingZone_DesignZoneAirDistributionEffectivenessinHeatingMode_Quantity) {
  Model model;
  ThermalZone zone(model);
  SizingZone sizingZone(model,zone);

  Unit units = sizingZone.getDesignZoneAirDistributionEffectivenessinHeatingMode(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(sizingZone.setDesignZoneAirDistributionEffectivenessinHeatingMode(testQ));
  Quantity q = sizingZone.getDesignZoneAirDistributionEffectivenessinHeatingMode(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

