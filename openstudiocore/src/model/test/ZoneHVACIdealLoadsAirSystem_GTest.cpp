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

#include "../ZoneHVACIdealLoadsAirSystem.hpp"
#include "../ZoneHVACIdealLoadsAirSystem_Impl.hpp"

#include "../../utilities/units/Quantity.hpp"
#include "../../utilities/units/Unit.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture,ZoneHVACIdealLoadsAirSystem_MaximumHeatingSupplyAirTemperature_Quantity) {
  Model model;
  // TODO: Check constructor.
  ZoneHVACIdealLoadsAirSystem zoneHVACIdealLoadsAirSystem(model);

  Unit units = zoneHVACIdealLoadsAirSystem.getMaximumHeatingSupplyAirTemperature(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(zoneHVACIdealLoadsAirSystem.setMaximumHeatingSupplyAirTemperature(testQ));
  Quantity q = zoneHVACIdealLoadsAirSystem.getMaximumHeatingSupplyAirTemperature(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,ZoneHVACIdealLoadsAirSystem_MinimumCoolingSupplyAirTemperature_Quantity) {
  Model model;
  // TODO: Check constructor.
  ZoneHVACIdealLoadsAirSystem zoneHVACIdealLoadsAirSystem(model);

  Unit units = zoneHVACIdealLoadsAirSystem.getMinimumCoolingSupplyAirTemperature(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(zoneHVACIdealLoadsAirSystem.setMinimumCoolingSupplyAirTemperature(testQ));
  Quantity q = zoneHVACIdealLoadsAirSystem.getMinimumCoolingSupplyAirTemperature(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,ZoneHVACIdealLoadsAirSystem_MaximumHeatingSupplyAirHumidityRatio_Quantity) {
  Model model;
  // TODO: Check constructor.
  ZoneHVACIdealLoadsAirSystem zoneHVACIdealLoadsAirSystem(model);

  Unit units = zoneHVACIdealLoadsAirSystem.getMaximumHeatingSupplyAirHumidityRatio(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(zoneHVACIdealLoadsAirSystem.setMaximumHeatingSupplyAirHumidityRatio(testQ));
  Quantity q = zoneHVACIdealLoadsAirSystem.getMaximumHeatingSupplyAirHumidityRatio(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,ZoneHVACIdealLoadsAirSystem_MinimumCoolingSupplyAirHumidityRatio_Quantity) {
  Model model;
  // TODO: Check constructor.
  ZoneHVACIdealLoadsAirSystem zoneHVACIdealLoadsAirSystem(model);

  Unit units = zoneHVACIdealLoadsAirSystem.getMinimumCoolingSupplyAirHumidityRatio(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(zoneHVACIdealLoadsAirSystem.setMinimumCoolingSupplyAirHumidityRatio(testQ));
  Quantity q = zoneHVACIdealLoadsAirSystem.getMinimumCoolingSupplyAirHumidityRatio(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,ZoneHVACIdealLoadsAirSystem_MaximumHeatingAirFlowRate_Quantity) {
  Model model;
  // TODO: Check constructor.
  ZoneHVACIdealLoadsAirSystem zoneHVACIdealLoadsAirSystem(model);

  Unit units = zoneHVACIdealLoadsAirSystem.getMaximumHeatingAirFlowRate(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(zoneHVACIdealLoadsAirSystem.setMaximumHeatingAirFlowRate(testQ));
  OSOptionalQuantity q = zoneHVACIdealLoadsAirSystem.getMaximumHeatingAirFlowRate(true);
  ASSERT_TRUE(q.isSet());
  EXPECT_NEAR(value,q.get().value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,ZoneHVACIdealLoadsAirSystem_MaximumSensibleHeatingCapacity_Quantity) {
  Model model;
  // TODO: Check constructor.
  ZoneHVACIdealLoadsAirSystem zoneHVACIdealLoadsAirSystem(model);

  Unit units = zoneHVACIdealLoadsAirSystem.getMaximumSensibleHeatingCapacity(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(zoneHVACIdealLoadsAirSystem.setMaximumSensibleHeatingCapacity(testQ));
  OSOptionalQuantity q = zoneHVACIdealLoadsAirSystem.getMaximumSensibleHeatingCapacity(true);
  ASSERT_TRUE(q.isSet());
  EXPECT_NEAR(value,q.get().value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,ZoneHVACIdealLoadsAirSystem_MaximumCoolingAirFlowRate_Quantity) {
  Model model;
  // TODO: Check constructor.
  ZoneHVACIdealLoadsAirSystem zoneHVACIdealLoadsAirSystem(model);

  Unit units = zoneHVACIdealLoadsAirSystem.getMaximumCoolingAirFlowRate(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(zoneHVACIdealLoadsAirSystem.setMaximumCoolingAirFlowRate(testQ));
  OSOptionalQuantity q = zoneHVACIdealLoadsAirSystem.getMaximumCoolingAirFlowRate(true);
  ASSERT_TRUE(q.isSet());
  EXPECT_NEAR(value,q.get().value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,ZoneHVACIdealLoadsAirSystem_MaximumTotalCoolingCapacity_Quantity) {
  Model model;
  // TODO: Check constructor.
  ZoneHVACIdealLoadsAirSystem zoneHVACIdealLoadsAirSystem(model);

  Unit units = zoneHVACIdealLoadsAirSystem.getMaximumTotalCoolingCapacity(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(zoneHVACIdealLoadsAirSystem.setMaximumTotalCoolingCapacity(testQ));
  OSOptionalQuantity q = zoneHVACIdealLoadsAirSystem.getMaximumTotalCoolingCapacity(true);
  ASSERT_TRUE(q.isSet());
  EXPECT_NEAR(value,q.get().value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,ZoneHVACIdealLoadsAirSystem_CoolingSensibleHeatRatio_Quantity) {
  Model model;
  // TODO: Check constructor.
  ZoneHVACIdealLoadsAirSystem zoneHVACIdealLoadsAirSystem(model);

  Unit units = zoneHVACIdealLoadsAirSystem.getCoolingSensibleHeatRatio(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(zoneHVACIdealLoadsAirSystem.setCoolingSensibleHeatRatio(testQ));
  Quantity q = zoneHVACIdealLoadsAirSystem.getCoolingSensibleHeatRatio(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,ZoneHVACIdealLoadsAirSystem_SensibleHeatRecoveryEffectiveness_Quantity) {
  Model model;
  // TODO: Check constructor.
  ZoneHVACIdealLoadsAirSystem zoneHVACIdealLoadsAirSystem(model);

  Unit units = zoneHVACIdealLoadsAirSystem.getSensibleHeatRecoveryEffectiveness(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(zoneHVACIdealLoadsAirSystem.setSensibleHeatRecoveryEffectiveness(testQ));
  Quantity q = zoneHVACIdealLoadsAirSystem.getSensibleHeatRecoveryEffectiveness(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,ZoneHVACIdealLoadsAirSystem_LatentHeatRecoveryEffectiveness_Quantity) {
  Model model;
  // TODO: Check constructor.
  ZoneHVACIdealLoadsAirSystem zoneHVACIdealLoadsAirSystem(model);

  Unit units = zoneHVACIdealLoadsAirSystem.getLatentHeatRecoveryEffectiveness(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(zoneHVACIdealLoadsAirSystem.setLatentHeatRecoveryEffectiveness(testQ));
  Quantity q = zoneHVACIdealLoadsAirSystem.getLatentHeatRecoveryEffectiveness(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

