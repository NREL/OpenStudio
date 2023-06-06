/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../Screen.hpp"
#include "../Screen_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, Screen_DefaultConstructor) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model m;
      Screen screen(m);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");
}

TEST_F(ModelFixture, Screen_GettersSetters) {
  Model m;
  Screen screen(m);

  // Ctor defaults
  double diffuseSolarReflectance = 0.08;
  double diffuseVisibleReflectance = 0.08;
  double screenMaterialSpacing = 0.00157;
  double screenMaterialDiameter = 0.000381;

  // Reflected Beam Transmittance Accounting Method:  String
  // Check Idd default: "ModelAsDiffuse"
  EXPECT_TRUE(screen.isReflectedBeamTransmittanceAccountingMethodDefaulted());
  EXPECT_EQ("ModelAsDiffuse", screen.reflectedBeamTransmittanceAccountingMethod());
  // Test a valid choice
  EXPECT_TRUE(screen.setReflectedBeamTransmittanceAccountingMethod("DoNotModel"));
  EXPECT_FALSE(screen.isReflectedBeamTransmittanceAccountingMethodDefaulted());
  EXPECT_EQ("DoNotModel", screen.reflectedBeamTransmittanceAccountingMethod());
  // Test an invalid choice
  EXPECT_FALSE(screen.setReflectedBeamTransmittanceAccountingMethod("BadChoice"));
  EXPECT_EQ("DoNotModel", screen.reflectedBeamTransmittanceAccountingMethod());

  // Diffuse Solar Reflectance:  Double
  // Check Idd default: 0.08, overriden by ctor arg with same value
  EXPECT_EQ(diffuseSolarReflectance, screen.diffuseSolarReflectance());
  EXPECT_TRUE(screen.setDiffuseSolarReflectance(0.04));
  EXPECT_EQ(0.04, screen.diffuseSolarReflectance());

  // Diffuse Visible Reflectance:  Double
  // Check Idd default: 0.08, overriden by ctor arg with same value
  EXPECT_EQ(diffuseVisibleReflectance, screen.diffuseVisibleReflectance());
  EXPECT_TRUE(screen.setDiffuseVisibleReflectance(0.04));
  EXPECT_EQ(0.04, screen.diffuseVisibleReflectance());

  // Thermal Hemispherical Emissivity:  Double
  // Check Idd default: 0.9
  EXPECT_TRUE(screen.isThermalHemisphericalEmissivityDefaulted());
  EXPECT_EQ(0.9, screen.thermalHemisphericalEmissivity());
  EXPECT_TRUE(screen.setThermalHemisphericalEmissivity(0.45));
  EXPECT_FALSE(screen.isThermalHemisphericalEmissivityDefaulted());
  EXPECT_EQ(0.45, screen.thermalHemisphericalEmissivity());
  screen.resetThermalHemisphericalEmissivity();
  EXPECT_EQ(0.9, screen.thermalHemisphericalEmissivity());

  // Conductivity:  Double
  // Check Idd default: 221.0
  EXPECT_TRUE(screen.isConductivityDefaulted());
  EXPECT_EQ(221.0, screen.conductivity());
  EXPECT_TRUE(screen.setConductivity(110.5));
  EXPECT_FALSE(screen.isConductivityDefaulted());
  EXPECT_EQ(110.5, screen.conductivity());
  screen.resetConductivity();
  EXPECT_EQ(221.0, screen.conductivity());

  // Screen Material Spacing:  Double
  // Check Idd default: 0.00157, overriden by ctor arg with same value
  EXPECT_EQ(screenMaterialSpacing, screen.screenMaterialSpacing());
  EXPECT_TRUE(screen.setScreenMaterialSpacing(0.1));
  EXPECT_EQ(0.1, screen.screenMaterialSpacing());

  // Screen Material Diameter:  Double
  // Check Idd default: 0.000381, overriden by ctor arg with same value
  EXPECT_EQ(screenMaterialDiameter, screen.screenMaterialDiameter());
  EXPECT_TRUE(screen.setScreenMaterialDiameter(0.01));
  EXPECT_EQ(0.01, screen.screenMaterialDiameter());

  // Screen to Glass Distance:  Double
  // Check Idd default: 0.025
  EXPECT_TRUE(screen.isScreentoGlassDistanceDefaulted());
  EXPECT_EQ(0.025, screen.screentoGlassDistance());
  EXPECT_TRUE(screen.setScreentoGlassDistance(0.01));
  EXPECT_FALSE(screen.isScreentoGlassDistanceDefaulted());
  EXPECT_EQ(0.01, screen.screentoGlassDistance());
  screen.resetScreentoGlassDistance();
  EXPECT_EQ(0.025, screen.screentoGlassDistance());

  // Top Opening Multiplier:  Double
  // Check Idd default: 0.0
  EXPECT_TRUE(screen.isTopOpeningMultiplierDefaulted());
  EXPECT_EQ(0.0, screen.topOpeningMultiplier());
  EXPECT_TRUE(screen.setTopOpeningMultiplier(0.0));
  EXPECT_FALSE(screen.isTopOpeningMultiplierDefaulted());
  EXPECT_EQ(0.0, screen.topOpeningMultiplier());
  screen.resetTopOpeningMultiplier();
  EXPECT_EQ(0.0, screen.topOpeningMultiplier());

  // Bottom Opening Multiplier:  Double
  // Check Idd default: 0.0
  EXPECT_TRUE(screen.isBottomOpeningMultiplierDefaulted());
  EXPECT_EQ(0.0, screen.bottomOpeningMultiplier());
  EXPECT_TRUE(screen.setBottomOpeningMultiplier(0.0));
  EXPECT_FALSE(screen.isBottomOpeningMultiplierDefaulted());
  EXPECT_EQ(0.0, screen.bottomOpeningMultiplier());
  screen.resetBottomOpeningMultiplier();
  EXPECT_EQ(0.0, screen.bottomOpeningMultiplier());

  // Left Side Opening Multiplier:  Double
  // Check Idd default: 0.0
  EXPECT_TRUE(screen.isLeftSideOpeningMultiplierDefaulted());
  EXPECT_EQ(0.0, screen.leftSideOpeningMultiplier());
  EXPECT_TRUE(screen.setLeftSideOpeningMultiplier(0.0));
  EXPECT_FALSE(screen.isLeftSideOpeningMultiplierDefaulted());
  EXPECT_EQ(0.0, screen.leftSideOpeningMultiplier());
  screen.resetLeftSideOpeningMultiplier();
  EXPECT_EQ(0.0, screen.leftSideOpeningMultiplier());

  // Right Side Opening Multiplier:  Double
  // Check Idd default: 0.0
  EXPECT_TRUE(screen.isRightSideOpeningMultiplierDefaulted());
  EXPECT_EQ(0.0, screen.rightSideOpeningMultiplier());
  EXPECT_TRUE(screen.setRightSideOpeningMultiplier(0.0));
  EXPECT_FALSE(screen.isRightSideOpeningMultiplierDefaulted());
  EXPECT_EQ(0.0, screen.rightSideOpeningMultiplier());
  screen.resetRightSideOpeningMultiplier();
  EXPECT_EQ(0.0, screen.rightSideOpeningMultiplier());

  // Angle of Resolution for Screen Transmittance Output Map:  String
  // Check Idd default: "0"
  EXPECT_TRUE(screen.isAngleofResolutionforScreenTransmittanceOutputMapDefaulted());
  EXPECT_EQ("0", screen.angleofResolutionforScreenTransmittanceOutputMap());
  // Test a valid choice
  EXPECT_TRUE(screen.setAngleofResolutionforScreenTransmittanceOutputMap("1"));
  EXPECT_FALSE(screen.isAngleofResolutionforScreenTransmittanceOutputMapDefaulted());
  EXPECT_EQ("1", screen.angleofResolutionforScreenTransmittanceOutputMap());
  // Test an invalid choice
  EXPECT_FALSE(screen.setAngleofResolutionforScreenTransmittanceOutputMap("BadChoice"));
  EXPECT_EQ("1", screen.angleofResolutionforScreenTransmittanceOutputMap());
}
