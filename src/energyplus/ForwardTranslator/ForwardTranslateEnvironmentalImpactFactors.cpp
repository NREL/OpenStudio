/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"

#include "../../model/EnvironmentalImpactFactors.hpp"

#include <utilities/idd/EnvironmentalImpactFactors_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateEnvironmentalImpactFactors(model::EnvironmentalImpactFactors& modelObject) {

    // It doesn't have a name
    IdfObject idfObject(openstudio::IddObjectType::EnvironmentalImpactFactors);
    m_idfObjects.push_back(idfObject);

    // District Heating Efficiency
    idfObject.setDouble(EnvironmentalImpactFactorsFields::DistrictHeatingEfficiency, modelObject.districtHeatingEfficiency());

    // District Cooling COP
    idfObject.setDouble(EnvironmentalImpactFactorsFields::DistrictCoolingCOP, modelObject.districtCoolingCOP());

    // Steam Conversion Efficiency
    idfObject.setDouble(EnvironmentalImpactFactorsFields::SteamConversionEfficiency, modelObject.steamConversionEfficiency());

    // Total Carbon Equivalent Emission Factor From N2O
    idfObject.setDouble(EnvironmentalImpactFactorsFields::TotalCarbonEquivalentEmissionFactorFromN2O,
                        modelObject.totalCarbonEquivalentEmissionFactorFromN2O());

    // Total Carbon Equivalent Emission Factor From CH4
    idfObject.setDouble(EnvironmentalImpactFactorsFields::TotalCarbonEquivalentEmissionFactorFromCH4,
                        modelObject.totalCarbonEquivalentEmissionFactorFromCH4());

    // Total Carbon Equivalent Emission Factor From CO2
    idfObject.setDouble(EnvironmentalImpactFactorsFields::TotalCarbonEquivalentEmissionFactorFromCO2,
                        modelObject.totalCarbonEquivalentEmissionFactorFromCO2());

    return idfObject;
  }  // End of translate function

}  // end namespace energyplus
}  // end namespace openstudio
