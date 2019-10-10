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

