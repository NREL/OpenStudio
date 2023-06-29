/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../RefractionExtinctionGlazing.hpp"
#include "../RefractionExtinctionGlazing_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, RefractionExtinctionGlazing_Ctor) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model m;
      RefractionExtinctionGlazing reg(m);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");
}

TEST_F(ModelFixture, RefractionExtinctionGlazing_GettersSetters) {
  Model m;

  // Ctors defaults
  double thickness = 0.1;
  double solarIndexofRefraction = 1.1;
  double solarExtinctionCoefficient = 0.1;
  double visibleIndexofRefraction = 1.1;
  double visibleExtinctionCoefficient = 0.1;
  RefractionExtinctionGlazing reg(m, thickness, solarIndexofRefraction, solarExtinctionCoefficient, visibleIndexofRefraction,
                                  visibleExtinctionCoefficient);

  // Thickness:  Double
  EXPECT_EQ(thickness, reg.thickness());
  EXPECT_TRUE(reg.setThickness(1.0));
  EXPECT_EQ(1.0, reg.thickness());

  // Solar Index of Refraction:  Double
  EXPECT_EQ(solarIndexofRefraction, reg.solarIndexofRefraction());
  EXPECT_TRUE(reg.setSolarIndexofRefraction(2.0));
  EXPECT_EQ(2.0, reg.solarIndexofRefraction());

  // Solar Extinction Coefficient:  Double
  EXPECT_EQ(solarExtinctionCoefficient, reg.solarExtinctionCoefficient());
  EXPECT_TRUE(reg.setSolarExtinctionCoefficient(1.1));
  EXPECT_EQ(1.1, reg.solarExtinctionCoefficient());

  // Visible Index of Refraction:  Double
  EXPECT_EQ(visibleIndexofRefraction, reg.visibleIndexofRefraction());
  EXPECT_TRUE(reg.setVisibleIndexofRefraction(2.1));
  EXPECT_EQ(2.1, reg.visibleIndexofRefraction());

  // Visible Extinction Coefficient:  Double
  EXPECT_EQ(visibleExtinctionCoefficient, reg.visibleExtinctionCoefficient());
  EXPECT_TRUE(reg.setVisibleExtinctionCoefficient(1.3));
  EXPECT_EQ(1.3, reg.visibleExtinctionCoefficient());

  // Infrared Transmittance at Normal Incidence:  Double
  // Check Idd default: 0.0
  EXPECT_EQ(0.0, reg.infraredTransmittanceatNormalIncidence());
  EXPECT_TRUE(reg.setInfraredTransmittanceatNormalIncidence(0.2));
  EXPECT_EQ(0.2, reg.infraredTransmittanceatNormalIncidence());
  reg.resetInfraredTransmittanceatNormalIncidence();
  EXPECT_EQ(0.0, reg.infraredTransmittanceatNormalIncidence());

  // Infrared Hemispherical Emissivity:  Double
  // Check Idd default: 0.84
  EXPECT_EQ(0.84, reg.infraredHemisphericalEmissivity());
  EXPECT_TRUE(reg.setInfraredHemisphericalEmissivity(0.42));
  EXPECT_EQ(0.42, reg.infraredHemisphericalEmissivity());
  reg.resetInfraredHemisphericalEmissivity();
  EXPECT_EQ(0.84, reg.infraredHemisphericalEmissivity());

  // Conductivity:  Double
  // Check Idd default: 0.9
  EXPECT_EQ(0.9, reg.conductivity());
  EXPECT_TRUE(reg.setConductivity(0.45));
  EXPECT_EQ(0.45, reg.conductivity());
  reg.resetConductivity();
  EXPECT_EQ(0.9, reg.conductivity());

  // Dirt Correction Factor for Solar and Visible Transmittance:  Double
  // Check Idd default: 1.0
  EXPECT_EQ(1.0, reg.dirtCorrectionFactorforSolarandVisibleTransmittance());
  EXPECT_TRUE(reg.setDirtCorrectionFactorforSolarandVisibleTransmittance(0.5));
  EXPECT_EQ(0.5, reg.dirtCorrectionFactorforSolarandVisibleTransmittance());
  reg.resetDirtCorrectionFactorforSolarandVisibleTransmittance();
  EXPECT_EQ(1.0, reg.dirtCorrectionFactorforSolarandVisibleTransmittance());

  // Solar Diffusing:  Boolean
  // Check Idd default: "No"
  EXPECT_FALSE(reg.solarDiffusing());

  EXPECT_TRUE(reg.setSolarDiffusing(true));
  EXPECT_TRUE(reg.solarDiffusing());

  EXPECT_TRUE(reg.setSolarDiffusing(false));
  EXPECT_FALSE(reg.solarDiffusing());
}
