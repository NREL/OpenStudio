/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2022, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

TEST_F(ModelFixture, SiteGroundTemperatureUndisturbedKusudaAchenbach_clone) {
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
