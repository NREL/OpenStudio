/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../ZoneVentilationDesignFlowRate.hpp"
#include "../ZoneVentilationDesignFlowRate_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, ZoneVentilationDesignFlowRate) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model m;
      ZoneVentilationDesignFlowRate zv(m);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");
}

/** Tests that the designFlowRateCalculationMethod is set appropriately and the other inputs cleared
 * when using the setXXX methods for flow rates
 */
TEST_F(ModelFixture, ZoneVentilationDesignFlowRate_setFlow) {
  Model m;

  ZoneVentilationDesignFlowRate zv(m);

  // Flow/Zone
  ASSERT_TRUE(zv.setDesignFlowRate(12.05));
  EXPECT_DOUBLE_EQ(12.05, zv.designFlowRate());
  ASSERT_EQ("Flow/Zone", zv.designFlowRateCalculationMethod());
  ASSERT_FALSE(zv.flowRateperZoneFloorArea());
  ASSERT_FALSE(zv.flowRateperPerson());
  ASSERT_FALSE(zv.airChangesperHour());

  // Flow/Area
  ASSERT_TRUE(zv.setFlowRateperZoneFloorArea(10.05));
  EXPECT_DOUBLE_EQ(10.05, zv.flowRateperZoneFloorArea());
  ASSERT_EQ("Flow/Area", zv.designFlowRateCalculationMethod());
  ASSERT_FALSE(zv.designFlowRate());
  ASSERT_FALSE(zv.flowRateperPerson());
  ASSERT_FALSE(zv.airChangesperHour());

  // Flow/Person
  ASSERT_TRUE(zv.setFlowRateperPerson(15.5));
  EXPECT_DOUBLE_EQ(15.5, zv.flowRateperPerson());
  ASSERT_EQ("Flow/Person", zv.designFlowRateCalculationMethod());
  ASSERT_FALSE(zv.designFlowRate());
  ASSERT_FALSE(zv.flowRateperZoneFloorArea());
  ASSERT_FALSE(zv.airChangesperHour());

  // AirChanges/Hour
  ASSERT_TRUE(zv.setAirChangesperHour(1.05));
  EXPECT_DOUBLE_EQ(1.05, zv.airChangesperHour());
  ASSERT_EQ("AirChanges/Hour", zv.designFlowRateCalculationMethod());
  ASSERT_FALSE(zv.designFlowRate());
  ASSERT_FALSE(zv.flowRateperZoneFloorArea());
  ASSERT_FALSE(zv.flowRateperPerson());
}
