/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "ModelFixture.hpp"

#include "../ZoneHVACEnergyRecoveryVentilator.hpp"
#include "../ZoneHVACEnergyRecoveryVentilator_Impl.hpp"
#include "../ThermalZone.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, ZoneHVACEnergyRecoveryVentilator) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model m;
      ZoneHVACEnergyRecoveryVentilator zonehvac(m);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");
}

TEST_F(ModelFixture, ZoneHVACEnergyRecoveryVentilator_AddAndRemove) {
  Model m;
  ZoneHVACEnergyRecoveryVentilator zonehvac(m);

  ThermalZone tz(m);
  ASSERT_TRUE(zonehvac.addToThermalZone(tz));
  ASSERT_TRUE(zonehvac.thermalZone());
  ASSERT_EQ(tz, zonehvac.thermalZone().get());
  ASSERT_EQ(1u, tz.equipment().size());
  zonehvac.removeFromThermalZone();
  ASSERT_EQ(0u, tz.equipment().size());

  ZoneHVACEnergyRecoveryVentilator zonehvac2(m);
  zonehvac2.addToThermalZone(tz);
  zonehvac2.remove();
  ASSERT_EQ(0u, tz.equipment().size());
}
