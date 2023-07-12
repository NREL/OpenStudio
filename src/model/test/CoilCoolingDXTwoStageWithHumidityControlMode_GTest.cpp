/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "ModelFixture.hpp"
#include "../CoilCoolingDXTwoStageWithHumidityControlMode.hpp"
#include "../CoilCoolingDXTwoStageWithHumidityControlMode_Impl.hpp"
#include "../CoilPerformanceDXCooling.hpp"
#include "../CoilPerformanceDXCooling_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, CoilCoolingDXTwoStageWithHumidityControlMode) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model m;
      CoilCoolingDXTwoStageWithHumidityControlMode coil(m);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");
}

/* Verifies that the CoilPerformanceDXCooling objects are indeed cloned too
 * Address https://github.com/NREL/OpenStudio/issues/2253
 */
TEST_F(ModelFixture, CoilCoolingDXTwoStageWithHumidityControlMode_CloneCoilPerf) {
  Model m;
  Model m2;

  CoilCoolingDXTwoStageWithHumidityControlMode coil(m);

  // The Ctor should have assigned the CoilPerformanceDXCooling Objects
  ASSERT_TRUE(coil.normalModeStage1CoilPerformance());
  ASSERT_TRUE(coil.normalModeStage1Plus2CoilPerformance());
  ASSERT_TRUE(coil.dehumidificationMode1Stage1CoilPerformance());
  ASSERT_TRUE(coil.dehumidificationMode1Stage1Plus2CoilPerformance());
  EXPECT_EQ(4u, m.getConcreteModelObjects<CoilPerformanceDXCooling>().size());

  // Clone in same model
  auto coilClone = coil.clone(m).cast<CoilCoolingDXTwoStageWithHumidityControlMode>();
  ASSERT_TRUE(coilClone.normalModeStage1CoilPerformance());
  ASSERT_TRUE(coilClone.normalModeStage1Plus2CoilPerformance());
  ASSERT_TRUE(coilClone.dehumidificationMode1Stage1CoilPerformance());
  ASSERT_TRUE(coilClone.dehumidificationMode1Stage1Plus2CoilPerformance());
  // These are "resources", so they should point to the same as the ori, not cloned
  EXPECT_EQ(4u, m.getConcreteModelObjects<CoilPerformanceDXCooling>().size());

  ASSERT_EQ(coil.normalModeStage1CoilPerformance().get().handle(), coilClone.normalModeStage1CoilPerformance().get().handle());
  ASSERT_EQ(coil.normalModeStage1Plus2CoilPerformance().get().handle(), coilClone.normalModeStage1Plus2CoilPerformance().get().handle());
  ASSERT_EQ(coil.dehumidificationMode1Stage1CoilPerformance().get().handle(), coilClone.dehumidificationMode1Stage1CoilPerformance().get().handle());
  ASSERT_EQ(coil.dehumidificationMode1Stage1Plus2CoilPerformance().get().handle(),
            coilClone.dehumidificationMode1Stage1Plus2CoilPerformance().get().handle());

  // Clone in other model
  //
  // This line triggers an OS_ASSERT exception ModelObject_Impl::clone (868)
  auto coilClone2 = coil.clone(m2).cast<CoilCoolingDXTwoStageWithHumidityControlMode>();
  ASSERT_TRUE(coilClone2.normalModeStage1CoilPerformance());
  ASSERT_TRUE(coilClone2.normalModeStage1Plus2CoilPerformance());
  ASSERT_TRUE(coilClone2.dehumidificationMode1Stage1CoilPerformance());
  ASSERT_TRUE(coilClone2.dehumidificationMode1Stage1Plus2CoilPerformance());
  // They should have been cloned to the new model too
  EXPECT_EQ(4u, m2.getConcreteModelObjects<CoilPerformanceDXCooling>().size());

  // Name comparison (can't do handle, not the same model, and actual clone)
  ASSERT_EQ(coil.normalModeStage1CoilPerformance().get().nameString(), coilClone2.normalModeStage1CoilPerformance().get().nameString());
  ASSERT_EQ(coil.normalModeStage1Plus2CoilPerformance().get().nameString(), coilClone2.normalModeStage1Plus2CoilPerformance().get().nameString());
  ASSERT_EQ(coil.dehumidificationMode1Stage1CoilPerformance().get().nameString(),
            coilClone2.dehumidificationMode1Stage1CoilPerformance().get().nameString());
  ASSERT_EQ(coil.dehumidificationMode1Stage1Plus2CoilPerformance().get().nameString(),
            coilClone2.dehumidificationMode1Stage1Plus2CoilPerformance().get().nameString());
}

TEST_F(ModelFixture, CoilCoolingDXTwoStageWithHumidityControlMode_MinOATCompressor) {
  Model m;

  CoilCoolingDXTwoStageWithHumidityControlMode coil(m);

  // #3976 - Minimum Outdoor Dry-Bulb Temperature for Compressor Operation
  // IDD Default
  EXPECT_EQ(-25.0, coil.minimumOutdoorDryBulbTemperatureforCompressorOperation());
  // There are no IDD limits, so everything should work
  EXPECT_TRUE(coil.setMinimumOutdoorDryBulbTemperatureforCompressorOperation(-5));
  EXPECT_EQ(-5, coil.minimumOutdoorDryBulbTemperatureforCompressorOperation());
}
