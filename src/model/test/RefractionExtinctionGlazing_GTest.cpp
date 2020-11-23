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
