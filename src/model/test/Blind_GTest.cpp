/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../Blind.hpp"
#include "../Blind_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, Blind_GettersSetters) {

  Model m;
  Blind blind(m);

  // Slat Orientation:  String
  // Check Idd default: "Horizontal"
  EXPECT_EQ("Horizontal", blind.slatOrientation());
  // Test a valid choice
  EXPECT_TRUE(blind.setSlatOrientation("Vertical"));
  EXPECT_EQ("Vertical", blind.slatOrientation());
  // Test an invalid choice
  EXPECT_FALSE(blind.setSlatOrientation("BadChoice"));
  EXPECT_EQ("Vertical", blind.slatOrientation());

  // Slat Width:  Double
  // Check Idd default: 0.025
  EXPECT_EQ(0.025, blind.slatWidth());
  EXPECT_TRUE(blind.setSlatWidth(0.01));
  EXPECT_EQ(0.01, blind.slatWidth());

  // Slat Separation:  Double
  // Check Idd default: 0.01875
  EXPECT_EQ(0.01875, blind.slatSeparation());
  EXPECT_TRUE(blind.setSlatSeparation(0.01));
  EXPECT_EQ(0.01, blind.slatSeparation());

  // Slat Thickness:  Double
  // Check Idd default: 0.001
  EXPECT_EQ(0.001, blind.slatThickness());
  EXPECT_TRUE(blind.setSlatThickness(0.021));
  EXPECT_EQ(0.021, blind.slatThickness());
  blind.resetSlatThickness();
  EXPECT_EQ(0.001, blind.slatThickness());

  // Slat Angle:  Double
  // Check Idd default: 45
  EXPECT_EQ(45, blind.slatAngle());
  EXPECT_TRUE(blind.setSlatAngle(22.5));
  EXPECT_EQ(22.5, blind.slatAngle());
  blind.resetSlatAngle();
  EXPECT_EQ(45, blind.slatAngle());

  // Slat Conductivity:  Double
  // Check Idd default: 221.0
  EXPECT_EQ(221.0, blind.slatConductivity());
  EXPECT_TRUE(blind.setSlatConductivity(110.5));
  EXPECT_EQ(110.5, blind.slatConductivity());
  blind.resetSlatConductivity();
  EXPECT_EQ(221.0, blind.slatConductivity());

  // Slat Beam Solar Transmittance:  Double
  // Check Idd default: 0.0
  EXPECT_EQ(0.0, blind.slatBeamSolarTransmittance());
  EXPECT_TRUE(blind.setSlatBeamSolarTransmittance(0.35));
  EXPECT_EQ(0.35, blind.slatBeamSolarTransmittance());
  blind.resetSlatBeamSolarTransmittance();
  EXPECT_EQ(0.0, blind.slatBeamSolarTransmittance());

  // Front Side Slat Beam Solar Reflectance:  Double
  // Check Idd default: 0.5
  EXPECT_EQ(0.5, blind.frontSideSlatBeamSolarReflectance());
  EXPECT_TRUE(blind.setFrontSideSlatBeamSolarReflectance(0.205));
  EXPECT_EQ(0.205, blind.frontSideSlatBeamSolarReflectance());

  // Back Side Slat Beam Solar Reflectance:  Double
  // Check Idd default: 0.5
  EXPECT_EQ(0.5, blind.backSideSlatBeamSolarReflectance());
  EXPECT_TRUE(blind.setBackSideSlatBeamSolarReflectance(0.105));
  EXPECT_EQ(0.105, blind.backSideSlatBeamSolarReflectance());

  // Slat Diffuse Solar Transmittance:  Double
  // Check Idd default: 0.0
  EXPECT_EQ(0.0, blind.slatDiffuseSolarTransmittance());
  EXPECT_TRUE(blind.setSlatDiffuseSolarTransmittance(0.31));
  EXPECT_EQ(0.31, blind.slatDiffuseSolarTransmittance());

  // Front Side Slat Diffuse Solar Reflectance:  Double
  // Check Idd default: 0.5
  EXPECT_EQ(0.5, blind.frontSideSlatDiffuseSolarReflectance());
  EXPECT_TRUE(blind.setFrontSideSlatDiffuseSolarReflectance(0.225));
  EXPECT_EQ(0.225, blind.frontSideSlatDiffuseSolarReflectance());

  // Back Side Slat Diffuse Solar Reflectance:  Double
  // Check Idd default: 0.5
  EXPECT_EQ(0.5, blind.backSideSlatDiffuseSolarReflectance());
  EXPECT_TRUE(blind.setBackSideSlatDiffuseSolarReflectance(0.336));
  EXPECT_EQ(0.336, blind.backSideSlatDiffuseSolarReflectance());

  // Slat Beam Visible Transmittance:  Double
  // Check Idd default: 0.0
  EXPECT_EQ(0.0, blind.slatBeamVisibleTransmittance());
  EXPECT_TRUE(blind.setSlatBeamVisibleTransmittance(0.112));
  EXPECT_EQ(0.112, blind.slatBeamVisibleTransmittance());

  // Front Side Slat Beam Visible Reflectance:  Double
  // Check Idd default: 0.5
  EXPECT_EQ(0.5, blind.frontSideSlatBeamVisibleReflectance().get());
  EXPECT_TRUE(blind.setFrontSideSlatBeamVisibleReflectance(0.145));
  EXPECT_EQ(0.145, blind.frontSideSlatBeamVisibleReflectance().get());
  blind.resetFrontSideSlatBeamVisibleReflectance();
  EXPECT_EQ(0.5, blind.frontSideSlatBeamVisibleReflectance().get());

  // Back Side Slat Beam Visible Reflectance:  Double
  // Check Idd default: 0.5
  EXPECT_EQ(0.5, blind.backSideSlatBeamVisibleReflectance().get());
  EXPECT_TRUE(blind.setBackSideSlatBeamVisibleReflectance(0.185));
  EXPECT_EQ(0.185, blind.backSideSlatBeamVisibleReflectance().get());
  blind.resetBackSideSlatBeamVisibleReflectance();
  EXPECT_EQ(0.5, blind.backSideSlatBeamVisibleReflectance().get());

  // Slat Diffuse Visible Transmittance:  Double
  // Check Idd default: 0.0
  EXPECT_EQ(0.0, blind.slatDiffuseVisibleTransmittance());
  EXPECT_TRUE(blind.setSlatDiffuseVisibleTransmittance(0.015));
  EXPECT_EQ(0.015, blind.slatDiffuseVisibleTransmittance());
  blind.resetSlatDiffuseVisibleTransmittance();
  EXPECT_EQ(0.0, blind.slatDiffuseVisibleTransmittance());

  // Front Side Slat Diffuse Visible Reflectance:  Double
  // Check Idd default: 0.5
  EXPECT_EQ(0.5, blind.frontSideSlatDiffuseVisibleReflectance().get());
  EXPECT_TRUE(blind.setFrontSideSlatDiffuseVisibleReflectance(0.055));
  EXPECT_EQ(0.055, blind.frontSideSlatDiffuseVisibleReflectance().get());
  blind.resetFrontSideSlatDiffuseVisibleReflectance();
  EXPECT_EQ(0.5, blind.frontSideSlatDiffuseVisibleReflectance().get());

  // Back Side Slat Diffuse Visible Reflectance:  Double
  // Check Idd default: 0.5
  EXPECT_EQ(0.5, blind.backSideSlatDiffuseVisibleReflectance().get());
  EXPECT_TRUE(blind.setBackSideSlatDiffuseVisibleReflectance(0.085));
  EXPECT_EQ(0.085, blind.backSideSlatDiffuseVisibleReflectance().get());
  blind.resetBackSideSlatDiffuseVisibleReflectance();
  EXPECT_EQ(0.5, blind.backSideSlatDiffuseVisibleReflectance().get());

  // Slat Infrared Hemispherical Transmittance:  Double
  // Check Idd default: 0.0
  EXPECT_EQ(0.0, blind.slatInfraredHemisphericalTransmittance());
  EXPECT_TRUE(blind.setSlatInfraredHemisphericalTransmittance(0.095));
  EXPECT_EQ(0.095, blind.slatInfraredHemisphericalTransmittance());
  blind.resetSlatInfraredHemisphericalTransmittance();
  EXPECT_EQ(0.0, blind.slatInfraredHemisphericalTransmittance());

  // Front Side Slat Infrared Hemispherical Emissivity:  Double
  // Check Idd default: 0.9
  EXPECT_EQ(0.9, blind.frontSideSlatInfraredHemisphericalEmissivity());
  EXPECT_TRUE(blind.setFrontSideSlatInfraredHemisphericalEmissivity(0.451));
  EXPECT_EQ(0.451, blind.frontSideSlatInfraredHemisphericalEmissivity());
  blind.resetFrontSideSlatInfraredHemisphericalEmissivity();
  EXPECT_EQ(0.9, blind.frontSideSlatInfraredHemisphericalEmissivity());

  // Back Side Slat Infrared Hemispherical Emissivity:  Double
  // Check Idd default: 0.9
  EXPECT_EQ(0.9, blind.backSideSlatInfraredHemisphericalEmissivity());
  EXPECT_TRUE(blind.setBackSideSlatInfraredHemisphericalEmissivity(0.455));
  EXPECT_EQ(0.455, blind.backSideSlatInfraredHemisphericalEmissivity());
  blind.resetBackSideSlatInfraredHemisphericalEmissivity();
  EXPECT_EQ(0.9, blind.backSideSlatInfraredHemisphericalEmissivity());

  // Blind to Glass Distance:  Double
  // Check Idd default: 0.050
  EXPECT_EQ(0.050, blind.blindtoGlassDistance());
  EXPECT_TRUE(blind.setBlindtoGlassDistance(0.033));
  EXPECT_EQ(0.033, blind.blindtoGlassDistance());
  blind.resetBlindtoGlassDistance();
  EXPECT_EQ(0.050, blind.blindtoGlassDistance());

  // Blind Top Opening Multiplier:  Double
  // Check Idd default: 0.5
  EXPECT_EQ(0.5, blind.blindTopOpeningMultiplier());
  EXPECT_TRUE(blind.setBlindTopOpeningMultiplier(0.259));
  EXPECT_EQ(0.259, blind.blindTopOpeningMultiplier());
  blind.resetBlindTopOpeningMultiplier();
  EXPECT_EQ(0.5, blind.blindTopOpeningMultiplier());

  // Blind Bottom Opening Multiplier:  Double
  // Check Idd default: 0.5
  EXPECT_EQ(0.5, blind.blindBottomOpeningMultiplier());
  EXPECT_TRUE(blind.setBlindBottomOpeningMultiplier(0.278));
  EXPECT_EQ(0.278, blind.blindBottomOpeningMultiplier());
  blind.resetBlindBottomOpeningMultiplier();
  EXPECT_EQ(0.5, blind.blindBottomOpeningMultiplier());

  // Blind Left Side Opening Multiplier:  Double
  // Check Idd default: 0.5
  EXPECT_EQ(0.5, blind.blindLeftSideOpeningMultiplier());
  EXPECT_TRUE(blind.setBlindLeftSideOpeningMultiplier(0.324));
  EXPECT_EQ(0.324, blind.blindLeftSideOpeningMultiplier());
  blind.resetBlindLeftSideOpeningMultiplier();
  EXPECT_EQ(0.5, blind.blindLeftSideOpeningMultiplier());

  // Blind Right Side Opening Multiplier:  Double
  // Check Idd default: 0.5
  EXPECT_EQ(0.5, blind.blindRightSideOpeningMultiplier());
  EXPECT_TRUE(blind.setBlindRightSideOpeningMultiplier(0.165));
  EXPECT_EQ(0.165, blind.blindRightSideOpeningMultiplier());
  blind.resetBlindRightSideOpeningMultiplier();
  EXPECT_EQ(0.5, blind.blindRightSideOpeningMultiplier());

  // Minimum Slat Angle:  Double
  // Check Idd default: 0
  EXPECT_EQ(0, blind.minimumSlatAngle());
  EXPECT_TRUE(blind.setMinimumSlatAngle(13.0));
  EXPECT_EQ(13.0, blind.minimumSlatAngle());
  blind.resetMinimumSlatAngle();
  EXPECT_EQ(0, blind.minimumSlatAngle());

  // Maximum Slat Angle:  Double
  // Check Idd default: 180
  EXPECT_EQ(180, blind.maximumSlatAngle());
  EXPECT_TRUE(blind.setMaximumSlatAngle(74.0));
  EXPECT_EQ(74.0, blind.maximumSlatAngle());
  blind.resetMaximumSlatAngle();
  EXPECT_EQ(180, blind.maximumSlatAngle());
}
