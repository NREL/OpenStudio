/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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
