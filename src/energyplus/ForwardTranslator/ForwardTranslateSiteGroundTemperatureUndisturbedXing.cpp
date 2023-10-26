/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/SiteGroundTemperatureUndisturbedXing.hpp"
#include "../../model/SiteGroundTemperatureUndisturbedXing_Impl.hpp"

#include <utilities/idd/Site_GroundTemperature_Undisturbed_Xing_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateSiteGroundTemperatureUndisturbedXing(SiteGroundTemperatureUndisturbedXing& modelObject) {
    boost::optional<double> value;

    // Name
    IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::Site_GroundTemperature_Undisturbed_Xing, modelObject);

    // SoilThermalConductivity
    idfObject.setDouble(Site_GroundTemperature_Undisturbed_XingFields::SoilThermalConductivity, modelObject.soilThermalConductivity());

    // SoilDensity
    idfObject.setDouble(Site_GroundTemperature_Undisturbed_XingFields::SoilDensity, modelObject.soilDensity());

    // SoilSpecificHeat
    idfObject.setDouble(Site_GroundTemperature_Undisturbed_XingFields::SoilSpecificHeat, modelObject.soilSpecificHeat());

    // AverageSoilSurfaceTemperature
    if ((value = modelObject.averageSoilSurfaceTemperature())) {
      idfObject.setDouble(Site_GroundTemperature_Undisturbed_XingFields::AverageSoilSurfaceTemperature, value.get());
    }

    // SoilSurfaceTemperatureAmplitude1
    if ((value = modelObject.soilSurfaceTemperatureAmplitude1())) {
      idfObject.setDouble(Site_GroundTemperature_Undisturbed_XingFields::SoilSurfaceTemperatureAmplitude1, value.get());
    }

    // SoilSurfaceTemperatureAmplitude2
    if ((value = modelObject.soilSurfaceTemperatureAmplitude2())) {
      idfObject.setDouble(Site_GroundTemperature_Undisturbed_XingFields::SoilSurfaceTemperatureAmplitude2, value.get());
    }

    // PhaseShiftofTemperatureAmplitude1
    if ((value = modelObject.phaseShiftofTemperatureAmplitude1())) {
      idfObject.setDouble(Site_GroundTemperature_Undisturbed_XingFields::PhaseShiftofTemperatureAmplitude1, value.get());
    }

    // PhaseShiftofTemperatureAmplitude2
    if ((value = modelObject.phaseShiftofTemperatureAmplitude2())) {
      idfObject.setDouble(Site_GroundTemperature_Undisturbed_XingFields::PhaseShiftofTemperatureAmplitude2, value.get());
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
