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

#include "../Model.hpp"
#include "../FanConstantVolume.hpp"
#include "../FanConstantVolume_Impl.hpp"
#include "../CoilCoolingWater.hpp"
#include "../CoilCoolingWater_Impl.hpp"
#include "../CoilHeatingWater.hpp"
#include "../CoilHeatingWater_Impl.hpp"
#include "../ScheduleConstant.hpp"
#include "../ZoneHVACFourPipeFanCoil.hpp"
#include "../ZoneHVACFourPipeFanCoil_Impl.hpp"
#include "../ThermalZone.hpp"
#include "../ThermalZone_Impl.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"

#include "../../utilities/units/Quantity.hpp"
#include "../../utilities/units/Unit.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture,ZoneHVACFourPipeFanCoil_MaximumSupplyAirFlowRate_Quantity) {
  Model model;
  EXPECT_EQ(0u,model.objects().size());
  EXPECT_TRUE(model.versionObject());
  // need schedule
  ScheduleConstant sched(model);
  EXPECT_EQ(1u,model.objects().size());
  sched.setValue(1.0); // Always on
  // need fan
  FanConstantVolume fan(model,sched);
  // need cooling and heating coils
  CoilCoolingWater coolingCoil(model,sched);
  CoilHeatingWater heatingCoil(model,sched);
  // construct object
  ZoneHVACFourPipeFanCoil zoneHVACFourPipeFanCoil(model,sched,fan,coolingCoil,heatingCoil);
  EXPECT_EQ(6u,model.objects().size());

  Unit units = zoneHVACFourPipeFanCoil.getMaximumSupplyAirFlowRate(true).units(); // Get IP units.
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(zoneHVACFourPipeFanCoil.setMaximumSupplyAirFlowRate(testQ));
  OSOptionalQuantity q = zoneHVACFourPipeFanCoil.getMaximumSupplyAirFlowRate(true);
  ASSERT_TRUE(q.isSet());
  EXPECT_NEAR(value,q.get().value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,ZoneHVACFourPipeFanCoil_LowSpeedSupplyAirFlowRatio_Quantity) {
  Model model;
  EXPECT_EQ(0u,model.objects().size());
  EXPECT_TRUE(model.versionObject());
  // need schedule
  ScheduleConstant sched(model);
  EXPECT_EQ(1u,model.objects().size());
  sched.setValue(1.0); // Always on
  // need fan
  FanConstantVolume fan(model,sched);
  // need cooling and heating coils
  CoilCoolingWater coolingCoil(model,sched);
  CoilHeatingWater heatingCoil(model,sched);
  // construct object
  ZoneHVACFourPipeFanCoil zoneHVACFourPipeFanCoil(model,sched,fan,coolingCoil,heatingCoil);
  EXPECT_EQ(6u,model.objects().size());

  Unit units = zoneHVACFourPipeFanCoil.getLowSpeedSupplyAirFlowRatio(true).units(); // Get IP units.
  double value(0.33);
  Quantity testQ(value,units);
  EXPECT_TRUE(zoneHVACFourPipeFanCoil.setLowSpeedSupplyAirFlowRatio(testQ));
  Quantity q = zoneHVACFourPipeFanCoil.getLowSpeedSupplyAirFlowRatio(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,ZoneHVACFourPipeFanCoil_MediumSpeedSupplyAirFlowRatio_Quantity) {
  Model model;
  EXPECT_EQ(0u,model.objects().size());
  EXPECT_TRUE(model.versionObject());
  // need schedule
  ScheduleConstant sched(model);
  EXPECT_EQ(1u,model.objects().size());
  sched.setValue(1.0); // Always on
  // need fan
  FanConstantVolume fan(model,sched);
  // need cooling and heating coils
  CoilCoolingWater coolingCoil(model,sched);
  CoilHeatingWater heatingCoil(model,sched);
  // construct object
  ZoneHVACFourPipeFanCoil zoneHVACFourPipeFanCoil(model,sched,fan,coolingCoil,heatingCoil);
  EXPECT_EQ(6u,model.objects().size());

  Unit units = zoneHVACFourPipeFanCoil.getMediumSpeedSupplyAirFlowRatio(true).units(); // Get IP units.
  double value(0.66);
  Quantity testQ(value,units);
  EXPECT_TRUE(zoneHVACFourPipeFanCoil.setMediumSpeedSupplyAirFlowRatio(testQ));
  Quantity q = zoneHVACFourPipeFanCoil.getMediumSpeedSupplyAirFlowRatio(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,ZoneHVACFourPipeFanCoil_MaximumOutdoorAirFlowRate_Quantity) {
  Model model;
  EXPECT_EQ(0u,model.objects().size());
  EXPECT_TRUE(model.versionObject());
  // need schedule
  ScheduleConstant sched(model);
  EXPECT_EQ(1u,model.objects().size());
  sched.setValue(1.0); // Always on
  // need fan
  FanConstantVolume fan(model,sched);
  // need cooling and heating coils
  CoilCoolingWater coolingCoil(model,sched);
  CoilHeatingWater heatingCoil(model,sched);
  // construct object
  ZoneHVACFourPipeFanCoil zoneHVACFourPipeFanCoil(model,sched,fan,coolingCoil,heatingCoil);
  EXPECT_EQ(6u,model.objects().size());

  Unit units = zoneHVACFourPipeFanCoil.getMaximumOutdoorAirFlowRate(true).units(); // Get IP units.
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(zoneHVACFourPipeFanCoil.setMaximumOutdoorAirFlowRate(testQ));
  OSOptionalQuantity q = zoneHVACFourPipeFanCoil.getMaximumOutdoorAirFlowRate(true);
  ASSERT_TRUE(q.isSet());
  EXPECT_NEAR(value,q.get().value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,ZoneHVACFourPipeFanCoil_MaximumColdWaterFlowRate_Quantity) {
  Model model;
  EXPECT_EQ(0u,model.objects().size());
  EXPECT_TRUE(model.versionObject());
  // need schedule
  ScheduleConstant sched(model);
  EXPECT_EQ(1u,model.objects().size());
  sched.setValue(1.0); // Always on
  // need fan
  FanConstantVolume fan(model,sched);
  // need cooling and heating coils
  CoilCoolingWater coolingCoil(model,sched);
  CoilHeatingWater heatingCoil(model,sched);
  // construct object
  ZoneHVACFourPipeFanCoil zoneHVACFourPipeFanCoil(model,sched,fan,coolingCoil,heatingCoil);
  EXPECT_EQ(6u,model.objects().size());

  Unit units = zoneHVACFourPipeFanCoil.getMaximumColdWaterFlowRate(true).units(); // Get IP units.
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(zoneHVACFourPipeFanCoil.setMaximumColdWaterFlowRate(testQ));
  OSOptionalQuantity q = zoneHVACFourPipeFanCoil.getMaximumColdWaterFlowRate(true);
  ASSERT_TRUE(q.isSet());
  EXPECT_NEAR(value,q.get().value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,ZoneHVACFourPipeFanCoil_MinimumColdWaterFlowRate_Quantity) {
  Model model;
  EXPECT_EQ(0u,model.objects().size());
  EXPECT_TRUE(model.versionObject());
  // need schedule
  ScheduleConstant sched(model);
  EXPECT_EQ(1u,model.objects().size());
  sched.setValue(1.0); // Always on
  // need fan
  FanConstantVolume fan(model,sched);
  // need cooling and heating coils
  CoilCoolingWater coolingCoil(model,sched);
  CoilHeatingWater heatingCoil(model,sched);
  // construct object
  ZoneHVACFourPipeFanCoil zoneHVACFourPipeFanCoil(model,sched,fan,coolingCoil,heatingCoil);
  EXPECT_EQ(6u,model.objects().size());

  Unit units = zoneHVACFourPipeFanCoil.getMinimumColdWaterFlowRate(true).units(); // Get IP units.
  double value(0.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(zoneHVACFourPipeFanCoil.setMinimumColdWaterFlowRate(testQ));
  Quantity q = zoneHVACFourPipeFanCoil.getMinimumColdWaterFlowRate(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,ZoneHVACFourPipeFanCoil_CoolingConvergenceTolerance_Quantity) {
  Model model;
  EXPECT_EQ(0u,model.objects().size());
  EXPECT_TRUE(model.versionObject());
  // need schedule
  ScheduleConstant sched(model);
  EXPECT_EQ(1u,model.objects().size());
  sched.setValue(1.0); // Always on
  // need fan
  FanConstantVolume fan(model,sched);
  // need cooling and heating coils
  CoilCoolingWater coolingCoil(model,sched);
  CoilHeatingWater heatingCoil(model,sched);
  // construct object
  ZoneHVACFourPipeFanCoil zoneHVACFourPipeFanCoil(model,sched,fan,coolingCoil,heatingCoil);
  EXPECT_EQ(6u,model.objects().size());

  Unit units = zoneHVACFourPipeFanCoil.getCoolingConvergenceTolerance(true).units(); // Get IP units.
  double value(0.001);
  Quantity testQ(value,units);
  EXPECT_TRUE(zoneHVACFourPipeFanCoil.setCoolingConvergenceTolerance(testQ));
  Quantity q = zoneHVACFourPipeFanCoil.getCoolingConvergenceTolerance(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,ZoneHVACFourPipeFanCoil_MaximumHotWaterFlowRate_Quantity) {
  Model model;
  EXPECT_EQ(0u,model.objects().size());
  EXPECT_TRUE(model.versionObject());
  // need schedule
  ScheduleConstant sched(model);
  EXPECT_EQ(1u,model.objects().size());
  sched.setValue(1.0); // Always on
  // need fan
  FanConstantVolume fan(model,sched);
  // need cooling and heating coils
  CoilCoolingWater coolingCoil(model,sched);
  CoilHeatingWater heatingCoil(model,sched);
  // construct object
  ZoneHVACFourPipeFanCoil zoneHVACFourPipeFanCoil(model,sched,fan,coolingCoil,heatingCoil);
  EXPECT_EQ(6u,model.objects().size());

  Unit units = zoneHVACFourPipeFanCoil.getMaximumHotWaterFlowRate(true).units(); // Get IP units.
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(zoneHVACFourPipeFanCoil.setMaximumHotWaterFlowRate(testQ));
  OSOptionalQuantity q = zoneHVACFourPipeFanCoil.getMaximumHotWaterFlowRate(true);
  ASSERT_TRUE(q.isSet());
  EXPECT_NEAR(value,q.get().value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,ZoneHVACFourPipeFanCoil_MinimumHotWaterFlowRate_Quantity) {
  Model model;
  EXPECT_EQ(0u,model.objects().size());
  EXPECT_TRUE(model.versionObject());
  // need schedule
  ScheduleConstant sched(model);
  EXPECT_EQ(1u,model.objects().size());
  sched.setValue(1.0); // Always on
  // need fan
  FanConstantVolume fan(model,sched);
  // need cooling and heating coils
  CoilCoolingWater coolingCoil(model,sched);
  CoilHeatingWater heatingCoil(model,sched);
  // construct object
  ZoneHVACFourPipeFanCoil zoneHVACFourPipeFanCoil(model,sched,fan,coolingCoil,heatingCoil);
  EXPECT_EQ(6u,model.objects().size());

  Unit units = zoneHVACFourPipeFanCoil.getMinimumHotWaterFlowRate(true).units(); // Get IP units.
  double value(0.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(zoneHVACFourPipeFanCoil.setMinimumHotWaterFlowRate(testQ));
  Quantity q = zoneHVACFourPipeFanCoil.getMinimumHotWaterFlowRate(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,ZoneHVACFourPipeFanCoil_HeatingConvergenceTolerance_Quantity) {
  Model model;
  EXPECT_EQ(0u,model.objects().size());
  EXPECT_TRUE(model.versionObject());
  // need schedule
  ScheduleConstant sched(model);
  EXPECT_EQ(1u,model.objects().size());
  sched.setValue(1.0); // Always on
  // need fan
  FanConstantVolume fan(model,sched);
  // need cooling and heating coils
  CoilCoolingWater coolingCoil(model,sched);
  CoilHeatingWater heatingCoil(model,sched);
  // construct object
  ZoneHVACFourPipeFanCoil zoneHVACFourPipeFanCoil(model,sched,fan,coolingCoil,heatingCoil);
  EXPECT_EQ(6u,model.objects().size());

  Unit units = zoneHVACFourPipeFanCoil.getHeatingConvergenceTolerance(true).units(); // Get IP units.
  double value(0.001);
  Quantity testQ(value,units);
  EXPECT_TRUE(zoneHVACFourPipeFanCoil.setHeatingConvergenceTolerance(testQ));
  Quantity q = zoneHVACFourPipeFanCoil.getHeatingConvergenceTolerance(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,ZoneHVACFourPipeFanCoil_addToThermalZone) {
  Model model;

  // need schedule
  ScheduleConstant sched(model);
  EXPECT_EQ(1u,model.objects().size());
  sched.setValue(1.0); // Always on

  // need fan
  FanConstantVolume fan(model,sched);

  // need cooling and heating coils
  CoilCoolingWater coolingCoil(model,sched);
  CoilHeatingWater heatingCoil(model,sched);

  // construct object
  ZoneHVACFourPipeFanCoil zoneHVACFourPipeFanCoil(model,sched,fan,coolingCoil,heatingCoil);

  // Is it hooked up to the inside components?
  EXPECT_EQ(fan.handle(),zoneHVACFourPipeFanCoil.supplyAirFan().handle());
  EXPECT_EQ(coolingCoil.handle(),zoneHVACFourPipeFanCoil.coolingCoil().handle());
  EXPECT_EQ(heatingCoil.handle(),zoneHVACFourPipeFanCoil.heatingCoil().handle());

  // construct a ThermalZone

  ThermalZone thermalZone(model);

  EXPECT_EQ(0u,thermalZone.equipment().size());

  EXPECT_TRUE(zoneHVACFourPipeFanCoil.addToThermalZone(thermalZone));

  EXPECT_TRUE(zoneHVACFourPipeFanCoil.inletNode());
  EXPECT_TRUE(zoneHVACFourPipeFanCoil.outletNode());

  EXPECT_EQ(1u,thermalZone.equipment().size());

  EXPECT_TRUE(fan.containingZoneHVACComponent());
  EXPECT_TRUE(coolingCoil.containingZoneHVACComponent());
  EXPECT_TRUE(heatingCoil.containingZoneHVACComponent());
}

