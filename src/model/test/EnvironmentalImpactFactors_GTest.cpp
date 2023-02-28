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

#include "ModelFixture.hpp"

#include "../EnvironmentalImpactFactors.hpp"
#include "../EnvironmentalImpactFactors_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, EnvironmentalImpactFactors_GettersSetters) {
  Model m;

  // This is a unique ModelObject
  auto environmentalImpactFactors = m.getUniqueModelObject<EnvironmentalImpactFactors>();

  // District Heating Efficiency: Required Double
  EXPECT_TRUE(environmentalImpactFactors.setDistrictHeatingEfficiency(0.1));
  EXPECT_EQ(0.1, environmentalImpactFactors.districtHeatingEfficiency());
  // Bad Value
  EXPECT_FALSE(environmentalImpactFactors.setDistrictHeatingEfficiency(-10.0));
  EXPECT_EQ(0.1, environmentalImpactFactors.districtHeatingEfficiency());

  // District Cooling COP: Required Double
  EXPECT_TRUE(environmentalImpactFactors.setDistrictCoolingCOP(0.1));
  EXPECT_EQ(0.1, environmentalImpactFactors.districtCoolingCOP());
  // Bad Value
  EXPECT_FALSE(environmentalImpactFactors.setDistrictCoolingCOP(-10.0));
  EXPECT_EQ(0.1, environmentalImpactFactors.districtCoolingCOP());

  // Steam Conversion Efficiency: Required Double
  EXPECT_TRUE(environmentalImpactFactors.setSteamConversionEfficiency(0.1));
  EXPECT_EQ(0.1, environmentalImpactFactors.steamConversionEfficiency());
  // Bad Value
  EXPECT_FALSE(environmentalImpactFactors.setSteamConversionEfficiency(-10.0));
  EXPECT_EQ(0.1, environmentalImpactFactors.steamConversionEfficiency());

  // Total Carbon Equivalent Emission Factor From N2O: Required Double
  EXPECT_TRUE(environmentalImpactFactors.setTotalCarbonEquivalentEmissionFactorFromN2O(3));
  EXPECT_EQ(3, environmentalImpactFactors.totalCarbonEquivalentEmissionFactorFromN2O());

  // Total Carbon Equivalent Emission Factor From CH4: Required Double
  EXPECT_TRUE(environmentalImpactFactors.setTotalCarbonEquivalentEmissionFactorFromCH4(3));
  EXPECT_EQ(3, environmentalImpactFactors.totalCarbonEquivalentEmissionFactorFromCH4());

  // Total Carbon Equivalent Emission Factor From CO2: Required Double
  EXPECT_TRUE(environmentalImpactFactors.setTotalCarbonEquivalentEmissionFactorFromCO2(3));
  EXPECT_EQ(3, environmentalImpactFactors.totalCarbonEquivalentEmissionFactorFromCO2());
}
