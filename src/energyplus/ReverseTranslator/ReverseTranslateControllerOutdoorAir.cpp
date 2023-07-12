/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ReverseTranslator.hpp"
#include "../../model/ControllerOutdoorAir.hpp"
#include "../../model/ControllerOutdoorAir_Impl.hpp"
#include <utilities/idd/Controller_OutdoorAir_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  OptionalModelObject ReverseTranslator::translateControllerOutdoorAir(const WorkspaceObject& workspaceObject) {
    if (workspaceObject.iddObject().type() != IddObjectType::Controller_OutdoorAir) {
      LOG(Error, "WorkspaceObject is not IddObjectType: Controller_OutdoorAir");
      return boost::none;
    }

    ControllerOutdoorAir mo(m_model);

    boost::optional<std::string> s = workspaceObject.getString(Controller_OutdoorAirFields::Name);
    if (s) {
      mo.setName(s.get());
    }

    boost::optional<double> value = workspaceObject.getDouble(Controller_OutdoorAirFields::MinimumOutdoorAirFlowRate);
    if (value) {
      mo.setMinimumOutdoorAirFlowRate(value.get());
    }

    value = workspaceObject.getDouble(Controller_OutdoorAirFields::MaximumOutdoorAirFlowRate);
    if (value) {
      mo.setMaximumOutdoorAirFlowRate(value.get());
    }

    s = workspaceObject.getString(Controller_OutdoorAirFields::EconomizerControlType);
    if (s) {
      mo.setEconomizerControlType(s.get());
    }

    s = workspaceObject.getString(Controller_OutdoorAirFields::EconomizerControlActionType);
    if (s) {
      mo.setEconomizerControlActionType(s.get());
    }

    value = workspaceObject.getDouble(Controller_OutdoorAirFields::EconomizerMinimumLimitDryBulbTemperature);
    if (value) {
      mo.setEconomizerMaximumLimitDryBulbTemperature(value.get());
    }

    value = workspaceObject.getDouble(Controller_OutdoorAirFields::EconomizerMaximumLimitEnthalpy);
    if (value) {
      mo.setEconomizerMaximumLimitEnthalpy(value.get());
    }

    value = workspaceObject.getDouble(Controller_OutdoorAirFields::EconomizerMaximumLimitDewpointTemperature);
    if (value) {
      mo.setEconomizerMaximumLimitDewpointTemperature(value.get());
    }

    s = workspaceObject.getString(Controller_OutdoorAirFields::ElectronicEnthalpyLimitCurveName);
    if (s) {
      LOG(Error, "ControllerOutdoorAir " << workspaceObject.briefDescription() << " references a curve that is not supported");
    }

    value = workspaceObject.getDouble(Controller_OutdoorAirFields::EconomizerMinimumLimitDryBulbTemperature);
    if (value) {
      mo.setEconomizerMinimumLimitDryBulbTemperature(value.get());
    }

    s = workspaceObject.getString(Controller_OutdoorAirFields::LockoutType);
    if (s) {
      mo.setLockoutType(s.get());
    }

    s = workspaceObject.getString(Controller_OutdoorAirFields::MinimumLimitType);
    if (s) {
      mo.setMinimumLimitType(s.get());
    }

    s = workspaceObject.getString(Controller_OutdoorAirFields::MinimumOutdoorAirScheduleName);
    if (s) {
      LOG(Warn,
          "ControllerOutdoorAir " << workspaceObject.briefDescription() << " references a minimum outdoor air schedule, which is not supported");
    }

    s = workspaceObject.getString(Controller_OutdoorAirFields::MinimumFractionofOutdoorAirScheduleName);
    if (s) {
      LOG(Warn, "ControllerOutdoorAir " << workspaceObject.briefDescription()
                                        << " references a minimum fraction of outdoor air schedule, which is not supported");
    }

    s = workspaceObject.getString(Controller_OutdoorAirFields::MaximumFractionofOutdoorAirScheduleName);
    if (s) {
      LOG(Warn, "ControllerOutdoorAir " << workspaceObject.briefDescription()
                                        << " references a maximum fraction of outdoor air schedule, which is not supported");
    }

    s = workspaceObject.getString(Controller_OutdoorAirFields::MechanicalVentilationControllerName);
    if (s) {
      LOG(Warn,
          "ControllerOutdoorAir " << workspaceObject.briefDescription() << " references a mechanical ventilation controller, which is not supported");
    }

    s = workspaceObject.getString(Controller_OutdoorAirFields::TimeofDayEconomizerControlScheduleName);
    if (s) {
      LOG(Warn, "ControllerOutdoorAir " << workspaceObject.briefDescription()
                                        << " references a time of day economizer control schedule, which is not supported");
    }

    s = workspaceObject.getString(Controller_OutdoorAirFields::HighHumidityControl);
    if (s) {
      if (istringEqual("Yes", s.get())) {
        mo.setHighHumidityControl(true);
      } else {
        mo.setHighHumidityControl(false);
      }
    }

    s = workspaceObject.getString(Controller_OutdoorAirFields::HumidistatControlZoneName);
    if (s) {
      LOG(Warn, "ControllerOutdoorAir " << workspaceObject.briefDescription() << " references a high humidity control zone, which is not supported");
    }

    value = workspaceObject.getDouble(Controller_OutdoorAirFields::HighHumidityOutdoorAirFlowRatio);
    if (value) {
      mo.setHighHumidityOutdoorAirFlowRatio(value.get());
    }

    s = workspaceObject.getString(Controller_OutdoorAirFields::ControlHighIndoorHumidityBasedonOutdoorHumidityRatio);
    if (s) {
      if (istringEqual("Yes", s.get())) {
        mo.setControlHighIndoorHumidityBasedOnOutdoorHumidityRatio(true);
      } else {
        mo.setControlHighIndoorHumidityBasedOnOutdoorHumidityRatio(false);
      }
    }

    s = workspaceObject.getString(Controller_OutdoorAirFields::HeatRecoveryBypassControlType);
    if (s) {
      mo.setHeatRecoveryBypassControlType(s.get());
    }

    return mo;
  }

}  // namespace energyplus

}  // namespace openstudio
