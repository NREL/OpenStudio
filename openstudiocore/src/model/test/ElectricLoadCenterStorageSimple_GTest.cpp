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

#include <model/test/ModelFixture.hpp>

#include "../ElectricLoadCenterStorageSimple.hpp"
#include "../ElectricLoadCenterStorageSimple_Impl.hpp"
#include "../Schedule.hpp"
#include "../ScheduleCompact.hpp"


using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, ElectricLoadCenterStorageSimple_Instantiate) {
  
  Model model;
  ThermalZone thermalZone(model);

  ElectricLoadCenterStorageSimple elcStorSimple(model);
  
  // Availability Schedule, defaults to model.alwaysOnDiscrete
  EXPECT_EQ(elcStorSimple.availabilitySchedule(), model.alwaysOnDiscreteSchedule());
  ScheduleCompact scheduleCompact(model);
  EXPECT_TRUE(elcStorSimple.setAvailabilitySchedule(scheduleCompact));
  EXPECT_EQ(elcStorSimple.availabilitySchedule(), scheduleCompact);
  elcStorSimple.resetAvailabilitySchedule();
  EXPECT_EQ(elcStorSimple.availabilitySchedule(), model.alwaysOnDiscreteSchedule());
    
  // ZoneName
  EXPECT_FALSE(elcStorSimple.thermalZone());
  EXPECT_TRUE(elcStorSimple.setThermalZone(thermalZone));
  ASSERT_TRUE(elcStorSimple.thermalZone());

  // Radiative Fraction, defaults
  EXPECT_TRUE(elcStorSimple.isRadiativeFractionforZoneHeatGainsDefaulted());
  EXPECT_TRUE(elcStorSimple.setRadiativeFractionforZoneHeatGains(0.3));
  EXPECT_FALSE(elcStorSimple.isRadiativeFractionforZoneHeatGainsDefaulted());
  EXPECT_EQ(elcStorSimple.radiativeFractionforZoneHeatGains(), 0.3);
  elcStorSimple.resetRadiativeFractionforZoneHeatGains();
  EXPECT_TRUE(elcStorSimple.isRadiativeFractionforZoneHeatGainsDefaulted());
  
  // nominalEnergeticEfficiencyforCharging, defaults
  EXPECT_FALSE(elcStorSimple.isNominalEnergeticEfficiencyforChargingDefaulted());
  EXPECT_TRUE(elcStorSimple.setNominalEnergeticEfficiencyforCharging(0.875));
  EXPECT_FALSE(elcStorSimple.isNominalEnergeticEfficiencyforChargingDefaulted());
  EXPECT_EQ(elcStorSimple.nominalEnergeticEfficiencyforCharging(), 0.875);
  elcStorSimple.resetNominalEnergeticEfficiencyforCharging();
  EXPECT_TRUE(elcStorSimple.isNominalEnergeticEfficiencyforChargingDefaulted());

  // nominalEnergeticEfficiencyforDischarging, defaults
  EXPECT_FALSE(elcStorSimple.isNominalDischargingEnergeticEfficiencyDefaulted());
  EXPECT_TRUE(elcStorSimple.setNominalDischargingEnergeticEfficiency(0.855));
  EXPECT_FALSE(elcStorSimple.isNominalDischargingEnergeticEfficiencyDefaulted());
  EXPECT_EQ(elcStorSimple.nominalDischargingEnergeticEfficiency(), 0.855);
  elcStorSimple.resetNominalDischargingEnergeticEfficiency();
  EXPECT_TRUE(elcStorSimple.isNominalDischargingEnergeticEfficiencyDefaulted());

  // maximumStorageCapacity, required, assigned in ctor
  EXPECT_TRUE(elcStorSimple.maximumStorageCapacity());
  EXPECT_EQ(1.0E13, elcStorSimple.maximumStorageCapacity());
  EXPECT_TRUE(elcStorSimple.setMaximumStorageCapacity(1.0E12));
  EXPECT_EQ(1.0E12, elcStorSimple.maximumStorageCapacity());

  // maximumPowerforDischarging, required, assigned in ctor
  EXPECT_TRUE(elcStorSimple.maximumPowerforDischarging());
  EXPECT_EQ(1.0E6, elcStorSimple.maximumPowerforDischarging());
  EXPECT_TRUE(elcStorSimple.setMaximumPowerforDischarging(1.8E6));
  EXPECT_EQ(1.8E6, elcStorSimple.maximumPowerforDischarging());

  // maximumPowerforCharging, required, assigned in ctor
  EXPECT_TRUE(elcStorSimple.maximumPowerforCharging());
  EXPECT_EQ(1.0E6, elcStorSimple.maximumPowerforCharging());
  EXPECT_TRUE(elcStorSimple.setMaximumPowerforCharging(1.5E6));
  EXPECT_EQ(1.5E6, elcStorSimple.maximumPowerforCharging());

  // Initial State of Charge
  EXPECT_TRUE(elcStorSimple.isInitialStateofChargeDefaulted());
  EXPECT_EQ(elcStorSimple.initialStateofCharge(), elcStorSimple.maximumStorageCapacity() / 2.0);
  EXPECT_TRUE(elcStorSimple.setInitialStateofCharge(1E5));
  EXPECT_FALSE(elcStorSimple.isInitialStateofChargeDefaulted());
  EXPECT_EQ(elcStorSimple.initialStateofCharge(), 1E5);
  elcStorSimple.resetInitialStateofCharge();
  EXPECT_TRUE(elcStorSimple.isInitialStateofChargeDefaulted());
 
}