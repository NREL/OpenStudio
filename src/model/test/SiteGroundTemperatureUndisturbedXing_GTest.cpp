/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "ModelFixture.hpp"
#include "../SiteGroundTemperatureUndisturbedXing.hpp"
#include "../SiteGroundTemperatureUndisturbedXing_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, SiteGroundTemperatureUndisturbedXing_SiteGroundTemperatureUndisturbedXing) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model m;
      SiteGroundTemperatureUndisturbedXing ux(m);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");

  Model m;
  SiteGroundTemperatureUndisturbedXing ux(m);

  EXPECT_EQ(1.08, ux.soilThermalConductivity());
  EXPECT_EQ(962, ux.soilDensity());
  EXPECT_EQ(2576, ux.soilSpecificHeat());
  EXPECT_EQ(11.1, ux.averageSoilSurfaceTemperature());
  EXPECT_EQ(13.4, ux.soilSurfaceTemperatureAmplitude1());
  EXPECT_EQ(0.7, ux.soilSurfaceTemperatureAmplitude2());
  EXPECT_EQ(25, ux.phaseShiftofTemperatureAmplitude1());
  EXPECT_EQ(30, ux.phaseShiftofTemperatureAmplitude2());
}

TEST_F(ModelFixture, SiteGroundTemperatureUndisturbedXing_SetGetFields) {
  Model m;
  SiteGroundTemperatureUndisturbedXing ux(m);

  EXPECT_TRUE(ux.setSoilThermalConductivity(1.3));
  EXPECT_TRUE(ux.setSoilDensity(971));
  EXPECT_TRUE(ux.setSoilSpecificHeat(2701));
  EXPECT_TRUE(ux.setAverageSoilSurfaceTemperature(11.2));
  EXPECT_TRUE(ux.setSoilSurfaceTemperatureAmplitude1(13.5));
  EXPECT_TRUE(ux.setSoilSurfaceTemperatureAmplitude2(0.8));
  EXPECT_TRUE(ux.setPhaseShiftofTemperatureAmplitude1(26));
  EXPECT_TRUE(ux.setPhaseShiftofTemperatureAmplitude2(31));

  EXPECT_EQ(1.3, ux.soilThermalConductivity());
  EXPECT_EQ(971, ux.soilDensity());
  EXPECT_EQ(2701, ux.soilSpecificHeat());
  EXPECT_EQ(11.2, ux.averageSoilSurfaceTemperature());
  EXPECT_EQ(13.5, ux.soilSurfaceTemperatureAmplitude1());
  EXPECT_EQ(0.8, ux.soilSurfaceTemperatureAmplitude2());
  EXPECT_EQ(26, ux.phaseShiftofTemperatureAmplitude1());
  EXPECT_EQ(31, ux.phaseShiftofTemperatureAmplitude2());
}

TEST_F(ModelFixture, SiteGroundTemperatureUndisturbedXing_Clone) {
  Model m;
  SiteGroundTemperatureUndisturbedXing ux(m);

  EXPECT_EQ(1u, m.getConcreteModelObjects<SiteGroundTemperatureUndisturbedXing>().size());

  auto uxClone = ux.clone(m).cast<SiteGroundTemperatureUndisturbedXing>();
  EXPECT_EQ(2u, m.getConcreteModelObjects<SiteGroundTemperatureUndisturbedXing>().size());

  ux.remove();
  EXPECT_EQ(1u, m.getConcreteModelObjects<SiteGroundTemperatureUndisturbedXing>().size());

  uxClone.remove();
  EXPECT_EQ(0u, m.getConcreteModelObjects<SiteGroundTemperatureUndisturbedXing>().size());
}
