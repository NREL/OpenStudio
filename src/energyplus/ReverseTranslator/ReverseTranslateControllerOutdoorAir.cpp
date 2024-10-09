/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ReverseTranslator.hpp"
#include "../../model/ControllerOutdoorAir.hpp"
#include "../../model/ControllerOutdoorAir_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"
#include "../../model/Curve.hpp"
#include "../../model/Curve_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
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

    boost::optional<WorkspaceObject> _wo;
    boost::optional<ModelObject> _mo;

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

    if ((_wo = workspaceObject.getTarget(Controller_OutdoorAirFields::ElectronicEnthalpyLimitCurveName))) {
      if ((_mo = translateAndMapWorkspaceObject(_wo.get()))) {
        if (boost::optional<Curve> _curve = _mo->optionalCast<Curve>()) {
          mo.setElectronicEnthalpyLimitCurve(_curve.get());
        } else {
          LOG(Warn, workspaceObject.briefDescription() << " has a wrong type for 'Electronic Enthalpy Limit Curve Name'");
        }
      }
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

    if ((_wo = workspaceObject.getTarget(Controller_OutdoorAirFields::MinimumOutdoorAirScheduleName))) {
      if ((_mo = translateAndMapWorkspaceObject(_wo.get()))) {
        if (boost::optional<Schedule> _schedule = _mo->optionalCast<Schedule>()) {
          mo.setMinimumOutdoorAirSchedule(_schedule.get());
        } else {
          LOG(Warn, workspaceObject.briefDescription() << " has a wrong type for 'Minimum Outdoor Air Schedule Name'");
        }
      }
    }

    if ((_wo = workspaceObject.getTarget(Controller_OutdoorAirFields::MinimumFractionofOutdoorAirScheduleName))) {
      if ((_mo = translateAndMapWorkspaceObject(_wo.get()))) {
        if (boost::optional<Schedule> _schedule = _mo->optionalCast<Schedule>()) {
          mo.setMinimumFractionofOutdoorAirSchedule(_schedule.get());
        } else {
          LOG(Warn, workspaceObject.briefDescription() << " has a wrong type for 'Minimum Fraction of Outdoor Air Schedule Name'");
        }
      }
    }

    if ((_wo = workspaceObject.getTarget(Controller_OutdoorAirFields::MaximumFractionofOutdoorAirScheduleName))) {
      if ((_mo = translateAndMapWorkspaceObject(_wo.get()))) {
        if (boost::optional<Schedule> _schedule = _mo->optionalCast<Schedule>()) {
          mo.setMaximumFractionofOutdoorAirSchedule(_schedule.get());
        } else {
          LOG(Warn, workspaceObject.briefDescription() << " has a wrong type for 'Maximum Fraction of Outdoor Air Schedule Name'");
        }
      }
    }

    s = workspaceObject.getString(Controller_OutdoorAirFields::MechanicalVentilationControllerName);
    if (s) {
      LOG(Warn,
          "ControllerOutdoorAir " << workspaceObject.briefDescription() << " references a mechanical ventilation controller, which is not supported");
    }

    if ((_wo = workspaceObject.getTarget(Controller_OutdoorAirFields::TimeofDayEconomizerControlScheduleName))) {
      if ((_mo = translateAndMapWorkspaceObject(_wo.get()))) {
        if (boost::optional<Schedule> _schedule = _mo->optionalCast<Schedule>()) {
          mo.setTimeofDayEconomizerControlSchedule(_schedule.get());
        } else {
          LOG(Warn, workspaceObject.briefDescription() << " has a wrong type for 'Time of Day Economizer Control Schedule Name'");
        }
      }
    }

    if ((_wo = workspaceObject.getTarget(Controller_OutdoorAirFields::HumidistatControlZoneName))) {
      if ((_mo = translateAndMapWorkspaceObject(_wo.get()))) {
        // Zone is translated, and a Space is returned instead
        if (boost::optional<Space> space_ = _mo->optionalCast<Space>()) {
          if (auto z_ = space_->thermalZone()) {
            mo.setHumidistatControlZone(z_.get());
          }
        } else {
          LOG(Warn, workspaceObject.briefDescription() << " has a wrong type for 'Humidistat Control Zone Name'");
        }
      }
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

    s = workspaceObject.getString(Controller_OutdoorAirFields::EconomizerOperationStaging);
    if (s) {
      mo.setEconomizerOperationStaging(s.get());
    }

    return mo;
  }

}  // namespace energyplus

}  // namespace openstudio
