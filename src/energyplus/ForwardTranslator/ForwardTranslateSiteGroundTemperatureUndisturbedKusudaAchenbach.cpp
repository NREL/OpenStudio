/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/SiteGroundTemperatureUndisturbedKusudaAchenbach.hpp"
#include "../../model/SiteGroundTemperatureUndisturbedKusudaAchenbach_Impl.hpp"

#include <utilities/idd/Site_GroundTemperature_Undisturbed_KusudaAchenbach_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject>
    ForwardTranslator::translateSiteGroundTemperatureUndisturbedKusudaAchenbach(SiteGroundTemperatureUndisturbedKusudaAchenbach& modelObject) {
    boost::optional<double> value;

    // Name
    IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::Site_GroundTemperature_Undisturbed_KusudaAchenbach, modelObject);

    // SoilThermalConductivity
    idfObject.setDouble(Site_GroundTemperature_Undisturbed_KusudaAchenbachFields::SoilThermalConductivity, modelObject.soilThermalConductivity());

    // SoilDensity
    idfObject.setDouble(Site_GroundTemperature_Undisturbed_KusudaAchenbachFields::SoilDensity, modelObject.soilDensity());

    // SoilSpecificHeat
    idfObject.setDouble(Site_GroundTemperature_Undisturbed_KusudaAchenbachFields::SoilSpecificHeat, modelObject.soilSpecificHeat());

    // AverageSoilSurfaceTemperature
    if ((value = modelObject.averageSoilSurfaceTemperature())) {
      idfObject.setDouble(Site_GroundTemperature_Undisturbed_KusudaAchenbachFields::AverageSoilSurfaceTemperature, value.get());
    }

    // AverageAmplitudeofSurfaceTemperature
    if ((value = modelObject.averageAmplitudeofSurfaceTemperature())) {
      idfObject.setDouble(Site_GroundTemperature_Undisturbed_KusudaAchenbachFields::AverageAmplitudeofSurfaceTemperature, value.get());
    }

    // PhaseShiftofMinimumSurfaceTemperature
    if ((value = modelObject.phaseShiftofMinimumSurfaceTemperature())) {
      idfObject.setDouble(Site_GroundTemperature_Undisturbed_KusudaAchenbachFields::PhaseShiftofMinimumSurfaceTemperature, value.get());
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
