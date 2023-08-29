/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ReverseTranslator.hpp"

#include "../../model/EnvironmentalImpactFactors.hpp"
#include "../../model/EnvironmentalImpactFactors_Impl.hpp"

#include <utilities/idd/EnvironmentalImpactFactors_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<ModelObject> ReverseTranslator::translateEnvironmentalImpactFactors(const WorkspaceObject& workspaceObject) {

    auto modelObject = m_model.getUniqueModelObject<EnvironmentalImpactFactors>();

    // District Heating Water Efficiency: Optional Double
    if (boost::optional<double> _districtHeatingEfficiency = workspaceObject.getDouble(EnvironmentalImpactFactorsFields::DistrictHeatingWaterEfficiency)) {
      modelObject.setDistrictHeatingEfficiency(_districtHeatingEfficiency.get());
    }

    // District Cooling COP: Optional Double
    if (boost::optional<double> _districtCoolingCOP = workspaceObject.getDouble(EnvironmentalImpactFactorsFields::DistrictCoolingCOP)) {
      modelObject.setDistrictCoolingCOP(_districtCoolingCOP.get());
    }

    // District Heating Steam Conversion Efficiency: Optional Double
    if (boost::optional<double> _steamConversionEfficiency = workspaceObject.getDouble(EnvironmentalImpactFactorsFields::DistrictHeatingSteamConversionEfficiency)) {
      modelObject.setSteamConversionEfficiency(_steamConversionEfficiency.get());
    }

    // Total Carbon Equivalent Emission Factor From N2O: Optional Double
    if (boost::optional<double> _totalCarbonEquivalentEmissionFactorFromN2O =
          workspaceObject.getDouble(EnvironmentalImpactFactorsFields::TotalCarbonEquivalentEmissionFactorFromN2O)) {
      modelObject.setTotalCarbonEquivalentEmissionFactorFromN2O(_totalCarbonEquivalentEmissionFactorFromN2O.get());
    }

    // Total Carbon Equivalent Emission Factor From CH4: Optional Double
    if (boost::optional<double> _totalCarbonEquivalentEmissionFactorFromCH4 =
          workspaceObject.getDouble(EnvironmentalImpactFactorsFields::TotalCarbonEquivalentEmissionFactorFromCH4)) {
      modelObject.setTotalCarbonEquivalentEmissionFactorFromCH4(_totalCarbonEquivalentEmissionFactorFromCH4.get());
    }

    // Total Carbon Equivalent Emission Factor From CO2: Optional Double
    if (boost::optional<double> _totalCarbonEquivalentEmissionFactorFromCO2 =
          workspaceObject.getDouble(EnvironmentalImpactFactorsFields::TotalCarbonEquivalentEmissionFactorFromCO2)) {
      modelObject.setTotalCarbonEquivalentEmissionFactorFromCO2(_totalCarbonEquivalentEmissionFactorFromCO2.get());
    }

    return modelObject;
  }  // End of translate function

}  // end namespace energyplus
}  // end namespace openstudio
