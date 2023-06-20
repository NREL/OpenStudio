/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/HumidifierSteamGas.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Curve.hpp"
#include <utilities/idd/Humidifier_Steam_Gas_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateHumidifierSteamGas(HumidifierSteamGas& modelObject) {
    OptionalDouble d;
    OptionalString s;
    OptionalModelObject temp;

    // Name
    IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::Humidifier_Steam_Gas, modelObject);

    // Availability Schedule Name
    if ((temp = modelObject.availabilitySchedule())) {
      if (boost::optional<IdfObject> _schedule = translateAndMapModelObject(temp.get())) {
        idfObject.setString(Humidifier_Steam_GasFields::AvailabilityScheduleName, _schedule->name().get());
      }
    }

    // Rated Capacity
    if (modelObject.isRatedCapacityAutosized()) {
      idfObject.setString(Humidifier_Steam_GasFields::RatedCapacity, "Autosize");
    }
    if ((d = modelObject.ratedCapacity())) {
      idfObject.setDouble(Humidifier_Steam_GasFields::RatedCapacity, d.get());
    }

    // Rated Gas Use Rate
    if ((d = modelObject.ratedGasUseRate())) {
      idfObject.setDouble(Humidifier_Steam_GasFields::RatedGasUseRate, d.get());
    } else if (modelObject.isRatedGasUseRateAutosized()) {
      idfObject.setString(Humidifier_Steam_GasFields::RatedGasUseRate, "Autosize");
    }

    // Thermal Efficiency
    if ((d = modelObject.thermalEfficiency())) {
      idfObject.setDouble(Humidifier_Steam_GasFields::ThermalEfficiency, d.get());
    }

    // Thermal Efficiency Modifier Curve Name
    if (boost::optional<model::Curve> curve = modelObject.thermalEfficiencyModifierCurve()) {
      if (boost::optional<IdfObject> _curve = translateAndMapModelObject(curve.get())) {
        idfObject.setString(Humidifier_Steam_GasFields::ThermalEfficiencyModifierCurveName, _curve->name().get());
      }
    }

    // Rated Fan Power
    if ((d = modelObject.ratedFanPower())) {
      idfObject.setDouble(Humidifier_Steam_GasFields::RatedFanPower, d.get());
    }

    // Auxiliary Electric Power
    if ((d = modelObject.auxiliaryElectricPower())) {
      idfObject.setDouble(Humidifier_Steam_GasFields::AuxiliaryElectricPower, d.get());
    }

    // Air Inlet Node Name
    temp = modelObject.inletModelObject();
    if (temp) {
      idfObject.setString(Humidifier_Steam_GasFields::AirInletNodeName, temp->name().get());
    }

    // Air Outlet Node Name
    temp = modelObject.outletModelObject();
    if (temp) {
      idfObject.setString(Humidifier_Steam_GasFields::AirOutletNodeName, temp->name().get());
    }

    // Water Storage Tank Name
    // not currently used

    // Inlet Water Temperature Option
    if ((s = modelObject.inletWaterTemperatureOption())) {
      idfObject.setString(Humidifier_Steam_GasFields::InletWaterTemperatureOption, s.get());
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
