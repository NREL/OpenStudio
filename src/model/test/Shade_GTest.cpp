/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../Shade.hpp"
#include "../Shade_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, Shade_DefaultConstructor) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model m;
      Shade shade(m);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");
}

TEST_F(ModelFixture, Shade_GettersSetters) {
  Model m;
  Shade shade(m);

  // Ctor argument defaults
  double solarTransmittance = 0.4;
  double solarReflectance = 0.5;
  double visibleTransmittance = 0.4;
  double visibleReflectance = 0.5;
  double thermalHemisphericalEmissivity = 0.9;
  double thermalTransmittance = 0.0;
  double thickness = 0.005;
  double conductivity = 0.1;

  // Solar Transmittance:  Double
  // Check Idd default: 0.4, overriden by ctor arg with same value
  EXPECT_EQ(solarTransmittance, shade.solarTransmittance());
  EXPECT_TRUE(shade.setSolarTransmittance(0.2));
  EXPECT_EQ(0.2, shade.solarTransmittance());

  // Solar Reflectance:  Double
  // Check Idd default: 0.5, overriden by ctor arg with same value
  EXPECT_EQ(solarReflectance, shade.solarReflectance());
  EXPECT_TRUE(shade.setSolarReflectance(0.25));
  EXPECT_EQ(0.25, shade.solarReflectance());

  // Visible Transmittance:  Double
  // Check Idd default: 0.4, overriden by ctor arg with same value
  EXPECT_EQ(visibleTransmittance, shade.visibleTransmittance());
  EXPECT_TRUE(shade.setVisibleTransmittance(0.225));
  EXPECT_EQ(0.225, shade.visibleTransmittance());

  // Visible Reflectance:  Double
  // Check Idd default: 0.5, overriden by ctor arg with same value
  EXPECT_EQ(visibleReflectance, shade.visibleReflectance());
  EXPECT_TRUE(shade.setVisibleReflectance(0.245));
  EXPECT_EQ(0.245, shade.visibleReflectance());

  // Thermal Hemispherical Emissivity:  Double
  // Check Idd default: 0.9, overriden by ctor arg with same value
  EXPECT_EQ(thermalHemisphericalEmissivity, shade.thermalHemisphericalEmissivity());
  EXPECT_TRUE(shade.setThermalHemisphericalEmissivity(0.45));
  EXPECT_EQ(0.45, shade.thermalHemisphericalEmissivity());

  // Thermal Transmittance:  Double
  // Check Idd default: 0.0, overriden by ctor arg with same value
  EXPECT_EQ(thermalTransmittance, shade.thermalTransmittance());
  EXPECT_TRUE(shade.setThermalTransmittance(0.6));
  EXPECT_EQ(0.6, shade.thermalTransmittance());

  // Thickness:  Double
  // Check Idd default: 0.005, overriden by ctor arg with same value
  EXPECT_EQ(thickness, shade.thickness());
  EXPECT_TRUE(shade.setThickness(0.05));
  EXPECT_EQ(0.05, shade.thickness());

  // Conductivity:  Double
  // Check Idd default: 0.1, overriden by ctor arg with same value
  EXPECT_EQ(conductivity, shade.conductivity());
  EXPECT_TRUE(shade.setConductivity(0.05));
  EXPECT_EQ(0.05, shade.conductivity());

  // Shade to Glass Distance:  Double
  // Check Idd default: 0.050
  EXPECT_TRUE(shade.isShadetoGlassDistanceDefaulted());
  EXPECT_EQ(0.050, shade.shadetoGlassDistance());
  EXPECT_TRUE(shade.setShadetoGlassDistance(0.03));
  EXPECT_FALSE(shade.isShadetoGlassDistanceDefaulted());
  EXPECT_EQ(0.03, shade.shadetoGlassDistance());
  shade.resetShadetoGlassDistance();
  EXPECT_EQ(0.050, shade.shadetoGlassDistance());

  // Top Opening Multiplier:  Double
  // Check Idd default: 0.0
  EXPECT_TRUE(shade.isTopOpeningMultiplierDefaulted());
  EXPECT_EQ(0.0, shade.topOpeningMultiplier());
  EXPECT_TRUE(shade.setTopOpeningMultiplier(0.0));
  EXPECT_FALSE(shade.isTopOpeningMultiplierDefaulted());
  EXPECT_EQ(0.0, shade.topOpeningMultiplier());
  shade.resetTopOpeningMultiplier();
  EXPECT_EQ(0.0, shade.topOpeningMultiplier());

  // Bottom Opening Multiplier:  Double
  // Check Idd default: 0.0
  EXPECT_TRUE(shade.isBottomOpeningMultiplierDefaulted());
  EXPECT_EQ(0.0, shade.bottomOpeningMultiplier());
  EXPECT_TRUE(shade.setBottomOpeningMultiplier(0.0));
  EXPECT_FALSE(shade.isBottomOpeningMultiplierDefaulted());
  EXPECT_EQ(0.0, shade.bottomOpeningMultiplier());
  shade.resetBottomOpeningMultiplier();
  EXPECT_EQ(0.0, shade.bottomOpeningMultiplier());

  // Left-Side Opening Multiplier:  Double
  // Check Idd default: 0.0
  EXPECT_TRUE(shade.isLeftSideOpeningMultiplierDefaulted());
  EXPECT_EQ(0.0, shade.leftSideOpeningMultiplier());
  EXPECT_TRUE(shade.setLeftSideOpeningMultiplier(0.0));
  EXPECT_FALSE(shade.isLeftSideOpeningMultiplierDefaulted());
  EXPECT_EQ(0.0, shade.leftSideOpeningMultiplier());
  shade.resetLeftSideOpeningMultiplier();
  EXPECT_EQ(0.0, shade.leftSideOpeningMultiplier());

  // Right-Side Opening Multiplier:  Double
  // Check Idd default: 0.0
  EXPECT_TRUE(shade.isRightSideOpeningMultiplierDefaulted());
  EXPECT_EQ(0.0, shade.rightSideOpeningMultiplier());
  EXPECT_TRUE(shade.setRightSideOpeningMultiplier(0.0));
  EXPECT_FALSE(shade.isRightSideOpeningMultiplierDefaulted());
  EXPECT_EQ(0.0, shade.rightSideOpeningMultiplier());
  shade.resetRightSideOpeningMultiplier();
  EXPECT_EQ(0.0, shade.rightSideOpeningMultiplier());

  // Airflow Permeability:  Double
  // Check Idd default: 0.0
  EXPECT_TRUE(shade.isAirflowPermeabilityDefaulted());
  EXPECT_EQ(0.0, shade.airflowPermeability());
  EXPECT_TRUE(shade.setAirflowPermeability(0.0));
  EXPECT_FALSE(shade.isAirflowPermeabilityDefaulted());
  EXPECT_EQ(0.0, shade.airflowPermeability());
  shade.resetAirflowPermeability();
  EXPECT_EQ(0.0, shade.airflowPermeability());
}
