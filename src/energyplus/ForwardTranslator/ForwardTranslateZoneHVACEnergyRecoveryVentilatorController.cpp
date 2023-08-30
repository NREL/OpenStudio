/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/ZoneHVACEnergyRecoveryVentilatorController.hpp"
#include "../../model/ZoneHVACEnergyRecoveryVentilatorController_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/Curve.hpp"
#include <utilities/idd/ZoneHVAC_EnergyRecoveryVentilator_Controller_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject>
    ForwardTranslator::translateZoneHVACEnergyRecoveryVentilatorController(ZoneHVACEnergyRecoveryVentilatorController& modelObject) {
    boost::optional<std::string> s;
    boost::optional<double> value;
    boost::optional<ModelObject> temp;

    // Name
    IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::ZoneHVAC_EnergyRecoveryVentilator_Controller, modelObject);

    // TemperatureHighLimit
    if ((value = modelObject.temperatureHighLimit())) {
      idfObject.setDouble(ZoneHVAC_EnergyRecoveryVentilator_ControllerFields::TemperatureHighLimit, value.get());
    }

    // TemperatureLowLimit
    if ((value = modelObject.temperatureLowLimit())) {
      idfObject.setDouble(ZoneHVAC_EnergyRecoveryVentilator_ControllerFields::TemperatureLowLimit, value.get());
    }

    // EnthalpyHighLimit
    if ((value = modelObject.enthalpyHighLimit())) {
      idfObject.setDouble(ZoneHVAC_EnergyRecoveryVentilator_ControllerFields::EnthalpyHighLimit, value.get());
    }

    // DewpointTemperatureLimit
    if ((value = modelObject.dewpointTemperatureLimit())) {
      idfObject.setDouble(ZoneHVAC_EnergyRecoveryVentilator_ControllerFields::DewpointTemperatureLimit, value.get());
    }

    // ElectronicEnthalpyLimitCurveName
    if ((temp = modelObject.electronicEnthalpyLimitCurve())) {
      if (auto _curve = translateAndMapModelObject(temp.get())) {
        idfObject.setString(ZoneHVAC_EnergyRecoveryVentilator_ControllerFields::ElectronicEnthalpyLimitCurveName, _curve->name().get());
      }
    }

    // ExhaustAirTemperatureLimit
    if ((s = modelObject.exhaustAirTemperatureLimit())) {
      idfObject.setString(ZoneHVAC_EnergyRecoveryVentilator_ControllerFields::ExhaustAirTemperatureLimit, s.get());
    }

    // ExhaustAirEnthalpyLimit
    if ((s = modelObject.exhaustAirEnthalpyLimit())) {
      idfObject.setString(ZoneHVAC_EnergyRecoveryVentilator_ControllerFields::ExhaustAirEnthalpyLimit, s.get());
    }

    // TimeofDayEconomizerFlowControlScheduleName
    if ((temp = modelObject.timeofDayEconomizerFlowControlSchedule())) {
      if (auto _schedule = translateAndMapModelObject(temp.get())) {
        idfObject.setString(ZoneHVAC_EnergyRecoveryVentilator_ControllerFields::TimeofDayEconomizerFlowControlScheduleName, _schedule->name().get());
      }
    }

    // HighHumidityControlFlag
    if (modelObject.highHumidityControlFlag()) {
      idfObject.setString(ZoneHVAC_EnergyRecoveryVentilator_ControllerFields::HighHumidityControlFlag, "Yes");
    } else {
      idfObject.setString(ZoneHVAC_EnergyRecoveryVentilator_ControllerFields::HighHumidityControlFlag, "No");
    }

    // HumidistatControlZoneName
    // Set in parent object

    // HighHumidityOutdoorAirFlowRatio
    if ((value = modelObject.highHumidityOutdoorAirFlowRatio())) {
      idfObject.setDouble(ZoneHVAC_EnergyRecoveryVentilator_ControllerFields::HighHumidityOutdoorAirFlowRatio, value.get());
    }
    // ControlHighIndoorHumidityBasedonOutdoorHumidityRatio
    if (modelObject.controlHighIndoorHumidityBasedonOutdoorHumidityRatio()) {
      idfObject.setString(ZoneHVAC_EnergyRecoveryVentilator_ControllerFields::ControlHighIndoorHumidityBasedonOutdoorHumidityRatio, "Yes");
    } else {
      idfObject.setString(ZoneHVAC_EnergyRecoveryVentilator_ControllerFields::ControlHighIndoorHumidityBasedonOutdoorHumidityRatio, "No");
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
