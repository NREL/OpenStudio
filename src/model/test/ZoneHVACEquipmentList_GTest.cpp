/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <algorithm>

#include <gtest/gtest.h>

#include "ModelFixture.hpp"
#include "../ThermalZone.hpp"
#include "../ZoneHVACBaseboardConvectiveElectric.hpp"
#include "../ScheduleConstant.hpp"
#include "../Schedule.hpp"
#include "../ScheduleTypeLimits.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, ZoneHVACEquipmentList_Logic) {
  Model m;

  ThermalZone z(m);

  EXPECT_TRUE(z.setLoadDistributionScheme("UniformPLR"));
  EXPECT_EQ(z.loadDistributionScheme(), "UniformPLR");

  ZoneHVACBaseboardConvectiveElectric b1(m);
  EXPECT_TRUE(b1.addToThermalZone(z));
  EXPECT_TRUE(z.setHeatingPriority(b1, 0));
  ZoneHVACBaseboardConvectiveElectric b2(m);
  EXPECT_TRUE(b2.addToThermalZone(z));

  // One that isn't part of the ZoneHVACEquipmentList
  ZoneHVACBaseboardConvectiveElectric bwrong(m);

  EXPECT_EQ(2u, z.equipment().size());
  ASSERT_EQ(2u, z.equipmentInCoolingOrder().size());
  EXPECT_EQ(b1, z.equipmentInCoolingOrder()[0]);
  EXPECT_EQ(b2, z.equipmentInCoolingOrder()[1]);

  // TODO: expected to fail right now due to https://github.com/NREL/EnergyPlus/issues/7193
  // ASSERT_EQ(1u, z.equipmentInHeatingOrder().size());
  // EXPECT_EQ(b2, z.equipmentInHeatingOrder()[0]);

  // Shouldn't be able to get/set anything for an equipment that isn't part of the ZoneEqList
  EXPECT_FALSE(z.setHeatingPriority(bwrong, 3));
  EXPECT_FALSE(z.setCoolingPriority(bwrong, 3));

  // Can't set/get the Sequential Fraction if:

  // 1. Equipment isn't part of the ZoneEqList
  EXPECT_TRUE(z.setLoadDistributionScheme("SequentialLoad"));
  EXPECT_FALSE(z.setSequentialCoolingFraction(bwrong, 0.5));
  EXPECT_FALSE(z.setSequentialHeatingFraction(bwrong, 0.5));
  EXPECT_FALSE(z.sequentialCoolingFraction(bwrong));
  EXPECT_FALSE(z.sequentialHeatingFraction(bwrong));

  // 2. Load Distribution isn't 'SequentialLoad'
  EXPECT_TRUE(z.setLoadDistributionScheme("UniformPLR"));
  EXPECT_FALSE(z.setSequentialCoolingFraction(b1, 0.5));
  EXPECT_FALSE(z.setSequentialHeatingFraction(b1, 0.5));
  EXPECT_FALSE(z.sequentialCoolingFraction(b1));
  EXPECT_FALSE(z.sequentialHeatingFraction(b1));

  EXPECT_TRUE(z.setLoadDistributionScheme("SequentialLoad"));

  // 3. Has a priority of zero
  // TODO: expected to fail (that is, it'll work) right now due to https://github.com/NREL/EnergyPlus/issues/7193
  // EXPECT_FALSE(z.setSequentialHeatingFraction(b1, 0.5));
  // EXPECT_FALSE(z.sequentialHeatingFraction(b1));

  // Rest should work fine
  EXPECT_TRUE(z.setSequentialCoolingFraction(b1, 0.5));
  ASSERT_TRUE(z.sequentialCoolingFraction(b1));
  EXPECT_EQ(0.5, z.sequentialCoolingFraction(b1).get());

  EXPECT_TRUE(z.setSequentialHeatingFraction(b2, 0.65));
  ASSERT_TRUE(z.sequentialHeatingFraction(b2));
  EXPECT_EQ(0.65, z.sequentialHeatingFraction(b2).get());

  EXPECT_TRUE(z.setSequentialCoolingFraction(b2, 0.7));
  ASSERT_TRUE(z.sequentialCoolingFraction(b2));
  EXPECT_EQ(0.7, z.sequentialCoolingFraction(b2).get());

  ScheduleConstant scheduleCooling(m);
  EXPECT_TRUE(scheduleCooling.setValue(0.9));
  EXPECT_TRUE(z.setSequentialCoolingFractionSchedule(b2, scheduleCooling));
  ASSERT_TRUE(z.sequentialCoolingFraction(b2));
  EXPECT_EQ(0.9, z.sequentialCoolingFraction(b2).get());
  ASSERT_TRUE(z.sequentialCoolingFractionSchedule(b2));

  ScheduleConstant scheduleHeating(m);
  EXPECT_TRUE(scheduleHeating.setValue(0.33));
  EXPECT_TRUE(z.setSequentialHeatingFractionSchedule(b2, scheduleHeating));
  ASSERT_TRUE(z.sequentialHeatingFraction(b2));
  EXPECT_EQ(0.33, z.sequentialHeatingFraction(b2).get());
  ASSERT_TRUE(z.sequentialHeatingFractionSchedule(b2));

  // Setting a priority to zero should reset the corresponding Sequential Fraction
  // TODO
  // EXPECT_TRUE(z.setCoolingPriority(b2, 0));
  EXPECT_EQ(2u, z.equipment().size());
  // TODO
  // ASSERT_EQ(1u, z.equipmentInCoolingOrder().size());
  // EXPECT_EQ(b1, z.equipmentInCoolingOrder()[0]);

  // TODO
  // EXPECT_FALSE(z.sequentialCoolingFraction(b2)); // reset
  EXPECT_TRUE(z.sequentialHeatingFraction(b2));  // untouched

  // Setting the Load Distribution Scheme to something else should reset all sequential fractions
  EXPECT_TRUE(z.setLoadDistributionScheme("UniformLoad"));
  EXPECT_FALSE(z.sequentialCoolingFraction(b1));
  EXPECT_FALSE(z.sequentialCoolingFractionSchedule(b1));
  EXPECT_FALSE(z.sequentialCoolingFraction(b2));
  EXPECT_FALSE(z.sequentialCoolingFractionSchedule(b2));
  EXPECT_FALSE(z.sequentialHeatingFraction(b1));
  EXPECT_FALSE(z.sequentialHeatingFractionSchedule(b1));
  EXPECT_FALSE(z.sequentialHeatingFraction(b2));
  EXPECT_FALSE(z.sequentialHeatingFractionSchedule(b2));
}

TEST_F(ModelFixture, ZoneHVACEquipmentList_ScheduleTypeLimits) {
  Model m;

  ThermalZone z(m);

  EXPECT_TRUE(z.setLoadDistributionScheme("SequentialLoad"));

  ZoneHVACBaseboardConvectiveElectric b1(m);
  EXPECT_TRUE(b1.addToThermalZone(z));
  EXPECT_TRUE(z.setHeatingPriority(b1, 0));
  ZoneHVACBaseboardConvectiveElectric b2(m);
  EXPECT_TRUE(b2.addToThermalZone(z));

  EXPECT_TRUE(z.setSequentialCoolingFraction(b1, 0.5));

  // This should have created a schedule, and this schedule should automatically have been assigned a schedule type lim via the ScheduleTypeRegistry
  boost::optional<Schedule> _sch = z.sequentialCoolingFractionSchedule(b1);
  ASSERT_TRUE(_sch);
  boost::optional<ScheduleTypeLimits> _sch_lim = _sch->scheduleTypeLimits();
  ASSERT_TRUE(_sch_lim);
  EXPECT_EQ("Dimensionless", _sch_lim->unitType());
}
