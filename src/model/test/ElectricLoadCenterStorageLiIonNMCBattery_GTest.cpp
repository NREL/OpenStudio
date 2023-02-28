/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2022, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "../ElectricLoadCenterStorageLiIonNMCBattery.hpp"
#include "../ElectricLoadCenterStorageLiIonNMCBattery_Impl.hpp"
#include "../Schedule.hpp"
#include "../Schedule_Impl.hpp"
#include "../ScheduleConstant.hpp"
#include "../ScheduleConstant_Impl.hpp"
#include "../ThermalZone.hpp"
#include "../ThermalZone_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, ElectricLoadCenterStorageLiIonNMCBattery_ElectricLoadCenterStorageLiIonNMCBattery) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  const int nSeries = 139;
  const int nParallel = 25;
  const double mass = 342.0;
  const double surfaceArea = 4.26;

  ASSERT_EXIT(
    {
      Model m;
      ElectricLoadCenterStorageLiIonNMCBattery battery(m, nSeries, nParallel, mass, surfaceArea);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");

  Model m;
  ElectricLoadCenterStorageLiIonNMCBattery battery(m, nSeries, nParallel, mass, surfaceArea);

  Schedule schedule = battery.availabilitySchedule();
  boost::optional<ScheduleConstant> scheduleConstant = schedule.optionalCast<ScheduleConstant>();
  ASSERT_TRUE(scheduleConstant);
  EXPECT_EQ((*scheduleConstant).value(), 1.0);
  EXPECT_FALSE(battery.thermalZone());
  EXPECT_EQ(0, battery.radiativeFraction());
  EXPECT_EQ("KandlerSmith", battery.lifetimeModel());
  EXPECT_EQ(nSeries, battery.numberofCellsinSeries());
  EXPECT_EQ(nParallel, battery.numberofStringsinParallel());
  EXPECT_EQ(0.5, battery.initialFractionalStateofCharge());
  EXPECT_EQ(0.95, battery.dctoDCChargingEfficiency());
  EXPECT_EQ(mass, battery.batteryMass());
  EXPECT_EQ(surfaceArea, battery.batterySurfaceArea());
  EXPECT_EQ(1500, battery.batterySpecificHeatCapacity());
  EXPECT_EQ(7.5, battery.heatTransferCoefficientBetweenBatteryandAmbient());
  EXPECT_EQ(4.2, battery.fullyChargedCellVoltage());
  EXPECT_EQ(3.53, battery.cellVoltageatEndofExponentialZone());
  EXPECT_EQ(3.342, battery.cellVoltageatEndofNominalZone());
  EXPECT_EQ(3.342, battery.defaultNominalCellVoltage());
  EXPECT_EQ(3.2, battery.fullyChargedCellCapacity());
  EXPECT_EQ(0.8075, battery.fractionofCellCapacityRemovedattheEndofExponentialZone());
  EXPECT_EQ(0.976875, battery.fractionofCellCapacityRemovedattheEndofNominalZone());
  EXPECT_EQ(1, battery.chargeRateatWhichVoltagevsCapacityCurveWasGenerated());
  EXPECT_EQ(0.09, battery.batteryCellInternalElectricalResistance());
}

TEST_F(ModelFixture, ElectricLoadCenterStorageLiIonNMCBattery_SetGetFields) {
  Model m;
  ElectricLoadCenterStorageLiIonNMCBattery battery(m, 139, 25, 342, 4.26);

  ScheduleConstant sched(m);
  sched.setValue(0.5);
  ThermalZone zone(m);

  EXPECT_TRUE(battery.setAvailabilitySchedule(sched));
  EXPECT_TRUE(battery.setThermalZone(zone));
  EXPECT_TRUE(battery.setRadiativeFraction(0.5));
  EXPECT_TRUE(battery.setLifetimeModel("None"));
  EXPECT_TRUE(battery.setNumberofCellsinSeries(5));
  EXPECT_TRUE(battery.setNumberofStringsinParallel(100));
  EXPECT_TRUE(battery.setInitialFractionalStateofCharge(0.51));
  EXPECT_TRUE(battery.setDCtoDCChargingEfficiency(0.52));
  EXPECT_TRUE(battery.setBatteryMass(1000));
  EXPECT_TRUE(battery.setBatterySurfaceArea(2000));
  EXPECT_TRUE(battery.setBatterySpecificHeatCapacity(3000));
  EXPECT_TRUE(battery.setHeatTransferCoefficientBetweenBatteryandAmbient(6.5));
  EXPECT_TRUE(battery.setFullyChargedCellVoltage(3.1));
  EXPECT_TRUE(battery.setCellVoltageatEndofExponentialZone(3.25));
  EXPECT_TRUE(battery.setCellVoltageatEndofNominalZone(3.35));
  EXPECT_TRUE(battery.setDefaultNominalCellVoltage(3.15));
  EXPECT_TRUE(battery.setFullyChargedCellCapacity(3.12));
  EXPECT_TRUE(battery.setFractionofCellCapacityRemovedattheEndofExponentialZone(0.73));
  EXPECT_TRUE(battery.setFractionofCellCapacityRemovedattheEndofNominalZone(0.74));
  EXPECT_TRUE(battery.setChargeRateatWhichVoltagevsCapacityCurveWasGenerated(4000));
  EXPECT_TRUE(battery.setBatteryCellInternalElectricalResistance(6000));

  Schedule schedule = battery.availabilitySchedule();
  boost::optional<ScheduleConstant> scheduleConstant = schedule.optionalCast<ScheduleConstant>();
  ASSERT_TRUE(scheduleConstant);
  EXPECT_EQ((*scheduleConstant).value(), 0.5);
  EXPECT_TRUE(battery.thermalZone());
  EXPECT_EQ(0.5, battery.radiativeFraction());
  EXPECT_EQ("None", battery.lifetimeModel());
  EXPECT_EQ(5, battery.numberofCellsinSeries());
  EXPECT_EQ(100, battery.numberofStringsinParallel());
  EXPECT_EQ(0.51, battery.initialFractionalStateofCharge());
  EXPECT_EQ(0.52, battery.dctoDCChargingEfficiency());
  EXPECT_EQ(1000, battery.batteryMass());
  EXPECT_EQ(2000, battery.batterySurfaceArea());
  EXPECT_EQ(3000, battery.batterySpecificHeatCapacity());
  EXPECT_EQ(6.5, battery.heatTransferCoefficientBetweenBatteryandAmbient());
  EXPECT_EQ(3.1, battery.fullyChargedCellVoltage());
  EXPECT_EQ(3.25, battery.cellVoltageatEndofExponentialZone());
  EXPECT_EQ(3.35, battery.cellVoltageatEndofNominalZone());
  EXPECT_EQ(3.15, battery.defaultNominalCellVoltage());
  EXPECT_EQ(3.12, battery.fullyChargedCellCapacity());
  EXPECT_EQ(0.73, battery.fractionofCellCapacityRemovedattheEndofExponentialZone());
  EXPECT_EQ(0.74, battery.fractionofCellCapacityRemovedattheEndofNominalZone());
  EXPECT_EQ(4000, battery.chargeRateatWhichVoltagevsCapacityCurveWasGenerated());
  EXPECT_EQ(6000, battery.batteryCellInternalElectricalResistance());

  battery.resetThermalZone();

  EXPECT_FALSE(battery.thermalZone());
}

TEST_F(ModelFixture, ElectricLoadCenterStorageLiIonNMCBattery_Clone) {
  Model m;
  ThermalZone z(m);
  ElectricLoadCenterStorageLiIonNMCBattery battery(m, 139, 25, 342, 4.26);
  EXPECT_TRUE(battery.setThermalZone(z));

  battery.setRadiativeFraction(0.6);

  {
    auto batteryClone = battery.clone(m).cast<ElectricLoadCenterStorageLiIonNMCBattery>();
    EXPECT_EQ(0.6, batteryClone.radiativeFraction());
    // should the clone retain the thermalZone or clear it? StorageConverter for eg would keep it, so leaving it as well
    EXPECT_TRUE(batteryClone.thermalZone());
    EXPECT_EQ(z, batteryClone.thermalZone().get());
  }

  {
    Model m2;
    auto batteryClone2 = battery.clone(m2).cast<ElectricLoadCenterStorageLiIonNMCBattery>();
    EXPECT_EQ(0.6, batteryClone2.radiativeFraction());
    EXPECT_FALSE(batteryClone2.thermalZone());
  }
}

TEST_F(ModelFixture, ElectricLoadCenterStorageLiIonNMCBattery_Remove) {
  Model m;
  ThermalZone z(m);
  ElectricLoadCenterStorageLiIonNMCBattery battery(m, 139, 25, 342, 4.26);
  EXPECT_TRUE(battery.setThermalZone(z));
  auto size = m.modelObjects().size();
  EXPECT_FALSE(battery.remove().empty());
  EXPECT_EQ(size - 1, m.modelObjects().size());
  EXPECT_EQ(0u, m.getConcreteModelObjects<ElectricLoadCenterStorageLiIonNMCBattery>().size());
  EXPECT_EQ(1u, m.getConcreteModelObjects<ThermalZone>().size());
}
