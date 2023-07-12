/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../RefrigerationWalkInZoneBoundary.hpp"
#include "../RefrigerationWalkInZoneBoundary_Impl.hpp"
#include "../ThermalZone.hpp"
#include "../ThermalZone_Impl.hpp"
#include "../Schedule.hpp"
#include "../Schedule_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, RefrigerationWalkInZoneBoundary_DefaultConstructor) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model model;
      RefrigerationWalkInZoneBoundary testObject = RefrigerationWalkInZoneBoundary(model);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");
}

TEST_F(ModelFixture, RefrigerationWalkInZoneBoundary_Remove) {
  Model model;
  RefrigerationWalkInZoneBoundary testObject = RefrigerationWalkInZoneBoundary(model);

  std::vector<RefrigerationWalkInZoneBoundary> refrigerationWalkInZoneBoundaries = model.getConcreteModelObjects<RefrigerationWalkInZoneBoundary>();
  EXPECT_EQ(1, refrigerationWalkInZoneBoundaries.size());

  testObject.remove();

  refrigerationWalkInZoneBoundaries = model.getConcreteModelObjects<RefrigerationWalkInZoneBoundary>();
  EXPECT_EQ(0, refrigerationWalkInZoneBoundaries.size());
}

TEST_F(ModelFixture, RefrigerationWalkInZoneBoundary_CloneOneModelWithDefaultData) {
  Model model;
  RefrigerationWalkInZoneBoundary testObject = RefrigerationWalkInZoneBoundary(model);

  auto testObjectClone = testObject.clone(model).cast<RefrigerationWalkInZoneBoundary>();

  EXPECT_DOUBLE_EQ(43.4, testObjectClone.totalInsulatedSurfaceAreaFacingZone());
  EXPECT_DOUBLE_EQ(0.235, testObjectClone.insulatedSurfaceUValueFacingZone());
  EXPECT_EQ("StripCurtain", testObjectClone.stockingDoorOpeningProtectionTypeFacingZone());
  EXPECT_DOUBLE_EQ(2.0, testObjectClone.areaofStockingDoorsFacingZone());
  EXPECT_DOUBLE_EQ(2.0, testObjectClone.heightofStockingDoorsFacingZone());
}

TEST_F(ModelFixture, RefrigerationWalkInZoneBoundary_CloneOneModelWithCustomData) {
  Model model;
  ThermalZone thermalZone(model);
  Schedule discreteSchedule = model.alwaysOnDiscreteSchedule();
  RefrigerationWalkInZoneBoundary testObject = RefrigerationWalkInZoneBoundary(model);
  testObject.setTotalInsulatedSurfaceAreaFacingZone(999.0);
  testObject.setStockingDoorOpeningProtectionTypeFacingZone("AirCurtain");
  testObject.setInsulatedSurfaceUValueFacingZone(999.0);
  testObject.setAreaofStockingDoorsFacingZone(999.0);
  testObject.setHeightofStockingDoorsFacingZone(999.0);
  testObject.setStockingDoorUValueFacingZone(999.0);
  testObject.setThermalZone(thermalZone);
  testObject.setGlassReachInDoorOpeningScheduleFacingZone(discreteSchedule);
  testObject.setStockingDoorOpeningScheduleFacingZone(discreteSchedule);

  auto testObjectClone = testObject.clone(model).cast<RefrigerationWalkInZoneBoundary>();
  EXPECT_DOUBLE_EQ(999.0, testObjectClone.totalInsulatedSurfaceAreaFacingZone());
  EXPECT_DOUBLE_EQ(999.0, testObjectClone.insulatedSurfaceUValueFacingZone());
  EXPECT_EQ("AirCurtain", testObjectClone.stockingDoorOpeningProtectionTypeFacingZone());
  EXPECT_DOUBLE_EQ(999.0, testObjectClone.areaofStockingDoorsFacingZone());
  EXPECT_DOUBLE_EQ(999.0, testObjectClone.heightofStockingDoorsFacingZone());
  EXPECT_DOUBLE_EQ(999.0, testObjectClone.stockingDoorUValueFacingZone());
  EXPECT_FALSE(testObjectClone.thermalZone());
  EXPECT_EQ(testObjectClone.glassReachInDoorOpeningScheduleFacingZone().get().handle(),
            testObject.glassReachInDoorOpeningScheduleFacingZone().get().handle());
  EXPECT_EQ(testObjectClone.stockingDoorOpeningScheduleFacingZone().get().handle(),
            testObject.stockingDoorOpeningScheduleFacingZone().get().handle());
}

TEST_F(ModelFixture, RefrigerationWalkInZoneBoundary_CloneTwoModelsWithDefaultData) {
  Model model;
  RefrigerationWalkInZoneBoundary testObject = RefrigerationWalkInZoneBoundary(model);

  auto testObjectClone = testObject.clone(model).cast<RefrigerationWalkInZoneBoundary>();

  Model model2;

  auto testObjectClone2 = testObject.clone(model2).cast<RefrigerationWalkInZoneBoundary>();

  EXPECT_DOUBLE_EQ(43.4, testObjectClone2.totalInsulatedSurfaceAreaFacingZone());
  EXPECT_DOUBLE_EQ(0.235, testObjectClone2.insulatedSurfaceUValueFacingZone());
  EXPECT_EQ("StripCurtain", testObjectClone2.stockingDoorOpeningProtectionTypeFacingZone());
  EXPECT_DOUBLE_EQ(2.0, testObjectClone2.areaofStockingDoorsFacingZone());
  EXPECT_DOUBLE_EQ(2.0, testObjectClone2.heightofStockingDoorsFacingZone());
  EXPECT_NE(testObjectClone2, testObjectClone);
  EXPECT_NE(testObjectClone2.handle(), testObjectClone.handle());
}
