/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
