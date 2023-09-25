/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "ModelFixture.hpp"

#include "../ZoneHVACCoolingPanelRadiantConvectiveWater.hpp"
#include "../ZoneHVACCoolingPanelRadiantConvectiveWater_Impl.hpp"
#include "../ThermalZone.hpp"
#include "../ThermalZone_Impl.hpp"
#include "../Schedule.hpp"
#include "../Schedule_Impl.hpp"
#include "../ScheduleConstant.hpp"
#include "../ScheduleConstant_Impl.hpp"
#include "../CoilCoolingWaterPanelRadiant.hpp"
#include "../CoilCoolingWaterPanelRadiant_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, ZoneHVACCoolingPanelRadiantConvectiveWater) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model m;
      ZoneHVACCoolingPanelRadiantConvectiveWater zonehvac(m);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");
}

TEST_F(ModelFixture, ZoneHVACCoolingPanelRadiantConvectiveWater_AddAndRemove) {
  Model m;
  ZoneHVACCoolingPanelRadiantConvectiveWater zonehvac(m);

  ThermalZone tz(m);
  ASSERT_TRUE(zonehvac.addToThermalZone(tz));
  ASSERT_TRUE(zonehvac.thermalZone());
  ASSERT_EQ(tz, zonehvac.thermalZone().get());
  ASSERT_EQ(1u, tz.equipment().size());
  zonehvac.removeFromThermalZone();
  ASSERT_EQ(0u, tz.equipment().size());

  ZoneHVACCoolingPanelRadiantConvectiveWater zonehvac2(m);
  zonehvac2.addToThermalZone(tz);
  zonehvac2.remove();
  ASSERT_EQ(0u, tz.equipment().size());
}

TEST_F(ModelFixture, ZoneHVACCoolingPanelRadiantConvectiveWater_SetGetFields) {
  Model m;
  ZoneHVACCoolingPanelRadiantConvectiveWater zonehvac(m);

  Schedule schedule = zonehvac.availabilitySchedule();
  boost::optional<ScheduleConstant> scheduleConstant = schedule.optionalCast<ScheduleConstant>();
  ASSERT_TRUE(scheduleConstant);
  EXPECT_EQ((*scheduleConstant).value(), 1.0);
  EXPECT_EQ(0.65, zonehvac.fractionRadiant());
  EXPECT_EQ(0.2, zonehvac.fractionofRadiantEnergyIncidentonPeople());
  HVACComponent coil = zonehvac.coolingCoil();
  boost::optional<CoilCoolingWaterPanelRadiant> coilCooling = coil.optionalCast<CoilCoolingWaterPanelRadiant>();
  ASSERT_TRUE(coilCooling);

  ScheduleConstant sched(m);
  sched.setValue(0.5);
  EXPECT_TRUE(zonehvac.setAvailabilitySchedule(sched));
  EXPECT_TRUE(zonehvac.setFractionRadiant(0.75));
  EXPECT_TRUE(zonehvac.setFractionofRadiantEnergyIncidentonPeople(0.3));
  CoilCoolingWaterPanelRadiant coilCooling2(m);
  EXPECT_TRUE(zonehvac.setCoolingCoil(coilCooling2));

  Schedule schedule2 = zonehvac.availabilitySchedule();
  boost::optional<ScheduleConstant> scheduleConstant2 = schedule2.optionalCast<ScheduleConstant>();
  ASSERT_TRUE(scheduleConstant2);
  EXPECT_EQ((*scheduleConstant2).value(), 0.5);
  EXPECT_EQ(0.75, zonehvac.fractionRadiant());
  EXPECT_EQ(0.3, zonehvac.fractionofRadiantEnergyIncidentonPeople());
  HVACComponent coil3 = zonehvac.coolingCoil();
  boost::optional<CoilCoolingWaterPanelRadiant> coilCooling3 = coil3.optionalCast<CoilCoolingWaterPanelRadiant>();
  ASSERT_TRUE(coilCooling3);
  EXPECT_EQ(coilCooling2.nameString(), coil3.nameString());
}
