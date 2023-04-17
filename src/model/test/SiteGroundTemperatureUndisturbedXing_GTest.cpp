/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
