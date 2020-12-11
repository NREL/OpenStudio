/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../StandardGlazing.hpp"
#include "../StandardGlazing_Impl.hpp"
#include "../MaterialPropertyGlazingSpectralData.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, StandardGlazing_DefaultConstructor) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model m;
      StandardGlazing standardGlazing(m);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");
}

TEST_F(ModelFixture, StandardGlazing_GettersSetters) {
  Model m;
  StandardGlazing standardGlazing(m);

  // Ctor arg defaults
  std::string opticalDataType = "SpectralAverage";
  double thickness = 0.1;

  // Optical Data Type:  String
  // Default in Ctor arg
  EXPECT_EQ("SpectralAverage", standardGlazing.opticalDataType());
  // TODO: JM 2018-12-13 Setter needs to be removed!
  // Test another valid choice
  EXPECT_TRUE(standardGlazing.setOpticalDataType("Spectral"));
  EXPECT_EQ("Spectral", standardGlazing.opticalDataType());
  // Test an invalid choice
  EXPECT_FALSE(standardGlazing.setOpticalDataType("BadChoice"));
  EXPECT_EQ("Spectral", standardGlazing.opticalDataType());
  EXPECT_TRUE(standardGlazing.setOpticalDataType("SpectralAverage"));
  EXPECT_EQ("SpectralAverage", standardGlazing.opticalDataType());

  // Window Glass Spectral Data Set Name: Optional Object
  // No Default
  EXPECT_FALSE(standardGlazing.windowGlassSpectralDataSet());
  MaterialPropertyGlazingSpectralData spec(m);
  // Setter/Resetter also change the opticalDataType
  EXPECT_TRUE(standardGlazing.setWindowGlassSpectralDataSet(spec));
  EXPECT_EQ("Spectral", standardGlazing.opticalDataType());
  ASSERT_TRUE(standardGlazing.windowGlassSpectralDataSet());
  EXPECT_EQ(spec, standardGlazing.windowGlassSpectralDataSet().get());
  standardGlazing.resetWindowGlassSpectralDataSet();
  EXPECT_EQ("SpectralAverage", standardGlazing.opticalDataType());
  EXPECT_FALSE(standardGlazing.windowGlassSpectralDataSet());

  // Thickness:  Double
  // Default in Ctor arg
  EXPECT_EQ(thickness, standardGlazing.thickness());
  EXPECT_TRUE(standardGlazing.setThickness(1.0));
  EXPECT_EQ(1.0, standardGlazing.thickness());

  // Solar Transmittance at Normal Incidence: Optional Double
  // No Default
  EXPECT_TRUE(standardGlazing.setSolarTransmittanceatNormalIncidence(0.5));
  ASSERT_TRUE(standardGlazing.solarTransmittanceatNormalIncidence());
  EXPECT_EQ(0.5, standardGlazing.solarTransmittanceatNormalIncidence().get());
  // Test reset
  standardGlazing.resetSolarTransmittanceatNormalIncidence();
  EXPECT_FALSE(standardGlazing.solarTransmittanceatNormalIncidence());

  // Front Side Solar Reflectance at Normal Incidence: Optional Double
  // No Default
  EXPECT_TRUE(standardGlazing.setFrontSideSolarReflectanceatNormalIncidence(0.5));
  ASSERT_TRUE(standardGlazing.frontSideSolarReflectanceatNormalIncidence());
  EXPECT_EQ(0.5, standardGlazing.frontSideSolarReflectanceatNormalIncidence().get());
  // Test reset
  standardGlazing.resetFrontSideSolarReflectanceatNormalIncidence();
  EXPECT_FALSE(standardGlazing.frontSideSolarReflectanceatNormalIncidence());

  // Back Side Solar Reflectance at Normal Incidence: Optional Double
  // No Default
  EXPECT_TRUE(standardGlazing.setBackSideSolarReflectanceatNormalIncidence(0.5));
  ASSERT_TRUE(standardGlazing.backSideSolarReflectanceatNormalIncidence());
  EXPECT_EQ(0.5, standardGlazing.backSideSolarReflectanceatNormalIncidence().get());
  // Test reset
  standardGlazing.resetBackSideSolarReflectanceatNormalIncidence();
  EXPECT_FALSE(standardGlazing.backSideSolarReflectanceatNormalIncidence());

  // Visible Transmittance at Normal Incidence: Optional Double
  // No Default
  EXPECT_TRUE(standardGlazing.setVisibleTransmittanceatNormalIncidence(0.5));
  ASSERT_TRUE(standardGlazing.visibleTransmittanceatNormalIncidence());
  EXPECT_EQ(0.5, standardGlazing.visibleTransmittanceatNormalIncidence().get());
  // Test reset
  standardGlazing.resetVisibleTransmittanceatNormalIncidence();
  EXPECT_FALSE(standardGlazing.visibleTransmittanceatNormalIncidence());

  // Front Side Visible Reflectance at Normal Incidence: Optional Double
  // No Default
  EXPECT_TRUE(standardGlazing.setFrontSideVisibleReflectanceatNormalIncidence(0.5));
  ASSERT_TRUE(standardGlazing.frontSideVisibleReflectanceatNormalIncidence());
  EXPECT_EQ(0.5, standardGlazing.frontSideVisibleReflectanceatNormalIncidence().get());
  // Test reset
  standardGlazing.resetFrontSideVisibleReflectanceatNormalIncidence();
  EXPECT_FALSE(standardGlazing.frontSideVisibleReflectanceatNormalIncidence());

  // Back Side Visible Reflectance at Normal Incidence: Optional Double
  // No Default
  EXPECT_TRUE(standardGlazing.setBackSideVisibleReflectanceatNormalIncidence(0.5));
  ASSERT_TRUE(standardGlazing.backSideVisibleReflectanceatNormalIncidence());
  EXPECT_EQ(0.5, standardGlazing.backSideVisibleReflectanceatNormalIncidence().get());
  // Test reset
  standardGlazing.resetBackSideVisibleReflectanceatNormalIncidence();
  EXPECT_FALSE(standardGlazing.backSideVisibleReflectanceatNormalIncidence());

  // Infrared Transmittance at Normal Incidence:  Double
  // Check Idd default: 0.0
  EXPECT_TRUE(standardGlazing.isInfraredTransmittanceatNormalIncidenceDefaulted());
  EXPECT_EQ(0.0, standardGlazing.infraredTransmittanceatNormalIncidence());
  EXPECT_TRUE(standardGlazing.setInfraredTransmittanceatNormalIncidence(0.005));
  EXPECT_FALSE(standardGlazing.isInfraredTransmittanceatNormalIncidenceDefaulted());
  EXPECT_EQ(0.005, standardGlazing.infraredTransmittanceatNormalIncidence());
  // Test reset
  standardGlazing.resetInfraredTransmittanceatNormalIncidence();
  EXPECT_EQ(0.0, standardGlazing.infraredTransmittanceatNormalIncidence());
  EXPECT_TRUE(standardGlazing.isInfraredTransmittanceatNormalIncidenceDefaulted());

  // Front Side Infrared Hemispherical Emissivity:  Double
  // Check Idd default: 0.84
  EXPECT_TRUE(standardGlazing.isFrontSideInfraredHemisphericalEmissivityDefaulted());
  EXPECT_EQ(0.84, standardGlazing.frontSideInfraredHemisphericalEmissivity());
  EXPECT_TRUE(standardGlazing.setFrontSideInfraredHemisphericalEmissivity(0.24));
  EXPECT_FALSE(standardGlazing.isFrontSideInfraredHemisphericalEmissivityDefaulted());
  EXPECT_EQ(0.24, standardGlazing.frontSideInfraredHemisphericalEmissivity());
  // Test reset
  standardGlazing.resetFrontSideInfraredHemisphericalEmissivity();
  EXPECT_EQ(0.84, standardGlazing.frontSideInfraredHemisphericalEmissivity());
  EXPECT_TRUE(standardGlazing.isFrontSideInfraredHemisphericalEmissivityDefaulted());

  // Back Side Infrared Hemispherical Emissivity:  Double
  // Check Idd default: 0.84
  EXPECT_TRUE(standardGlazing.isBackSideInfraredHemisphericalEmissivityDefaulted());
  EXPECT_EQ(0.84, standardGlazing.backSideInfraredHemisphericalEmissivity());
  EXPECT_TRUE(standardGlazing.setBackSideInfraredHemisphericalEmissivity(0.42));
  EXPECT_FALSE(standardGlazing.isBackSideInfraredHemisphericalEmissivityDefaulted());
  EXPECT_EQ(0.42, standardGlazing.backSideInfraredHemisphericalEmissivity());
  // Test reset
  standardGlazing.resetBackSideInfraredHemisphericalEmissivity();
  EXPECT_EQ(0.84, standardGlazing.backSideInfraredHemisphericalEmissivity());
  EXPECT_TRUE(standardGlazing.isBackSideInfraredHemisphericalEmissivityDefaulted());

  // Conductivity:  Double
  // Check Idd default: 0.9
  EXPECT_TRUE(standardGlazing.isConductivityDefaulted());
  EXPECT_EQ(0.9, standardGlazing.conductivity());
  EXPECT_TRUE(standardGlazing.setConductivity(0.45));
  EXPECT_FALSE(standardGlazing.isConductivityDefaulted());
  EXPECT_EQ(0.45, standardGlazing.conductivity());
  // Test reset
  standardGlazing.resetConductivity();
  EXPECT_EQ(0.9, standardGlazing.conductivity());
  EXPECT_TRUE(standardGlazing.isConductivityDefaulted());

  // Dirt Correction Factor for Solar and Visible Transmittance:  Double
  // Check Idd default: 1.0
  EXPECT_TRUE(standardGlazing.isDirtCorrectionFactorforSolarandVisibleTransmittanceDefaulted());
  EXPECT_EQ(1.0, standardGlazing.dirtCorrectionFactorforSolarandVisibleTransmittance());
  EXPECT_TRUE(standardGlazing.setDirtCorrectionFactorforSolarandVisibleTransmittance(0.5));
  EXPECT_FALSE(standardGlazing.isDirtCorrectionFactorforSolarandVisibleTransmittanceDefaulted());
  EXPECT_EQ(0.5, standardGlazing.dirtCorrectionFactorforSolarandVisibleTransmittance());
  // Test reset
  standardGlazing.resetDirtCorrectionFactorforSolarandVisibleTransmittance();
  EXPECT_EQ(1.0, standardGlazing.dirtCorrectionFactorforSolarandVisibleTransmittance());
  EXPECT_TRUE(standardGlazing.isDirtCorrectionFactorforSolarandVisibleTransmittanceDefaulted());

  // Solar Diffusing:  Boolean
  // Check Idd default: false
  EXPECT_TRUE(standardGlazing.isSolarDiffusingDefaulted());
  EXPECT_FALSE(standardGlazing.solarDiffusing());
  // Test true
  EXPECT_TRUE(standardGlazing.setSolarDiffusing(true));
  EXPECT_TRUE(standardGlazing.solarDiffusing());
  EXPECT_FALSE(standardGlazing.isSolarDiffusingDefaulted());
  // Test false
  EXPECT_TRUE(standardGlazing.setSolarDiffusing(false));
  EXPECT_FALSE(standardGlazing.solarDiffusing());
  // Test reset
  standardGlazing.resetSolarDiffusing();
  EXPECT_TRUE(standardGlazing.isSolarDiffusingDefaulted());
  EXPECT_FALSE(standardGlazing.solarDiffusing());
}
