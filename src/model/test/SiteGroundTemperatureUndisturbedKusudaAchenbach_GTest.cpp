/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "ModelFixture.hpp"
#include "../SiteGroundTemperatureUndisturbedKusudaAchenbach.hpp"
#include "../SiteGroundTemperatureUndisturbedKusudaAchenbach_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, SiteGroundTemperatureUndisturbedKusudaAchenbach_SiteGroundTemperatureUndisturbedKusudaAchenbach) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model m;
      SiteGroundTemperatureUndisturbedKusudaAchenbach uka(m);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");

  Model m;
  SiteGroundTemperatureUndisturbedKusudaAchenbach uka(m);

  EXPECT_EQ(1.08, uka.soilThermalConductivity());
  EXPECT_EQ(962, uka.soilDensity());
  EXPECT_EQ(2576, uka.soilSpecificHeat());
  ASSERT_TRUE(uka.averageSoilSurfaceTemperature());
  EXPECT_EQ(15.5, uka.averageSoilSurfaceTemperature().get());
  ASSERT_TRUE(uka.averageAmplitudeofSurfaceTemperature());
  EXPECT_EQ(12.8, uka.averageAmplitudeofSurfaceTemperature().get());
  ASSERT_TRUE(uka.phaseShiftofMinimumSurfaceTemperature());
  EXPECT_EQ(17.3, uka.phaseShiftofMinimumSurfaceTemperature().get());
}

TEST_F(ModelFixture, SiteGroundTemperatureUndisturbedKusudaAchenbach_SetGetFields) {
  Model m;
  SiteGroundTemperatureUndisturbedKusudaAchenbach uka(m);

  EXPECT_TRUE(uka.setSoilThermalConductivity(1.2));
  EXPECT_TRUE(uka.setSoilDensity(970));
  EXPECT_TRUE(uka.setSoilSpecificHeat(2700));
  EXPECT_TRUE(uka.setAverageSoilSurfaceTemperature(17.0));
  EXPECT_TRUE(uka.setAverageAmplitudeofSurfaceTemperature(14.0));
  EXPECT_TRUE(uka.setPhaseShiftofMinimumSurfaceTemperature(19.0));

  EXPECT_EQ(1.2, uka.soilThermalConductivity());
  EXPECT_EQ(970, uka.soilDensity());
  EXPECT_EQ(2700, uka.soilSpecificHeat());
  ASSERT_TRUE(uka.averageSoilSurfaceTemperature());
  EXPECT_EQ(17.0, uka.averageSoilSurfaceTemperature().get());
  ASSERT_TRUE(uka.averageAmplitudeofSurfaceTemperature());
  EXPECT_EQ(14.0, uka.averageAmplitudeofSurfaceTemperature().get());
  ASSERT_TRUE(uka.phaseShiftofMinimumSurfaceTemperature());
  EXPECT_EQ(19.0, uka.phaseShiftofMinimumSurfaceTemperature().get());

  uka.resetAverageSoilSurfaceTemperature();
  uka.resetAverageAmplitudeofSurfaceTemperature();
  uka.resetPhaseShiftofMinimumSurfaceTemperature();

  EXPECT_FALSE(uka.averageSoilSurfaceTemperature());
  EXPECT_FALSE(uka.averageAmplitudeofSurfaceTemperature());
  EXPECT_FALSE(uka.phaseShiftofMinimumSurfaceTemperature());
}

TEST_F(ModelFixture, SiteGroundTemperatureUndisturbedKusudaAchenbach_Clone) {
  Model m;
  SiteGroundTemperatureUndisturbedKusudaAchenbach uka(m);

  EXPECT_EQ(1u, m.getConcreteModelObjects<SiteGroundTemperatureUndisturbedKusudaAchenbach>().size());

  auto ukaClone = uka.clone(m).cast<SiteGroundTemperatureUndisturbedKusudaAchenbach>();
  EXPECT_EQ(2u, m.getConcreteModelObjects<SiteGroundTemperatureUndisturbedKusudaAchenbach>().size());

  uka.remove();
  EXPECT_EQ(1u, m.getConcreteModelObjects<SiteGroundTemperatureUndisturbedKusudaAchenbach>().size());

  ukaClone.remove();
  EXPECT_EQ(0u, m.getConcreteModelObjects<SiteGroundTemperatureUndisturbedKusudaAchenbach>().size());
}
