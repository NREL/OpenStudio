/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/AirLoopHVACOutdoorAirSystem.hpp"
#include "../../model/ControllerOutdoorAir.hpp"
#include "../../model/ControllerMechanicalVentilation.hpp"
#include "../../model/Node.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/Curve.hpp"
#include "../../model/ZoneControlHumidistat.hpp"
#include <utilities/idd/Controller_OutdoorAir_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateControllerOutdoorAir(ControllerOutdoorAir& modelObject) {
    OptionalString s;
    OptionalDouble d;

    // Create a new IddObjectType::AirLoopHVAC_OutdoorAirSystem
    IdfObject idfObject(IddObjectType::Controller_OutdoorAir);

    m_idfObjects.push_back(idfObject);

    ///////////////////////////////////////////////////////////////////////////
    // Field: Name ////////////////////////////////////////////////////////////
    s = modelObject.name();
    if (s) {
      idfObject.setString(openstudio::Controller_OutdoorAirFields::Name, *s);
    }
    ///////////////////////////////////////////////////////////////////////////
    // Field: Relief Air Outlet Node Name /////////////////////////////////////
    s = modelObject.airLoopHVACOutdoorAirSystem()->reliefAirModelObject()->name();
    if (s) {
      idfObject.setString(openstudio::Controller_OutdoorAirFields::ReliefAirOutletNodeName, *s);
    }

    ///////////////////////////////////////////////////////////////////////////
    // Field: Return Air Outlet Node Name /////////////////////////////////////
    s = modelObject.airLoopHVACOutdoorAirSystem()->returnAirModelObject()->name();
    if (s) {
      idfObject.setString(openstudio::Controller_OutdoorAirFields::ReturnAirNodeName, *s);
    }

    ///////////////////////////////////////////////////////////////////////////
    // Field: Mixed Air Outlet Node Name //////////////////////////////////////
    s = modelObject.airLoopHVACOutdoorAirSystem()->mixedAirModelObject()->name();
    if (s) {
      idfObject.setString(openstudio::Controller_OutdoorAirFields::MixedAirNodeName, *s);
    }

    ///////////////////////////////////////////////////////////////////////////
    // Field: Actuator Node Name //////////////////////////////////////////////
    s = modelObject.airLoopHVACOutdoorAirSystem()->outboardOANode()->name();
    if (s) {
      idfObject.setString(openstudio::Controller_OutdoorAirFields::ActuatorNodeName, *s);
    }

    ///////////////////////////////////////////////////////////////////////////
    // Field: Minimum Outdoor Air Flow Rate ///////////////////////////////////
    if (modelObject.controllerMechanicalVentilation().demandControlledVentilation() && modelObject.isMinimumOutdoorAirFlowRateAutosized()) {
      idfObject.setDouble(openstudio::Controller_OutdoorAirFields::MinimumOutdoorAirFlowRate, 0.0);
    } else {
      d = modelObject.minimumOutdoorAirFlowRate();
      if (d) {
        idfObject.setDouble(openstudio::Controller_OutdoorAirFields::MinimumOutdoorAirFlowRate, *d);
      } else {
        idfObject.setString(openstudio::Controller_OutdoorAirFields::MinimumOutdoorAirFlowRate, "Autosize");
      }
    }

    ///////////////////////////////////////////////////////////////////////////
    // Field: Maximum Outdoor Air Flow Rate ///////////////////////////////////
    d = modelObject.maximumOutdoorAirFlowRate();
    if (d) {
      idfObject.setDouble(openstudio::Controller_OutdoorAirFields::MaximumOutdoorAirFlowRate, *d);
    } else {
      idfObject.setString(openstudio::Controller_OutdoorAirFields::MaximumOutdoorAirFlowRate, "Autosize");
    }

    ///////////////////////////////////////////////////////////////////////////
    // Field: Economizer Control Type /////////////////////////////////////////
    idfObject.setString(openstudio::Controller_OutdoorAirFields::EconomizerControlType, modelObject.getEconomizerControlType());

    ///////////////////////////////////////////////////////////////////////////
    // Field: Economizer Control Action Type //////////////////////////////////
    idfObject.setString(openstudio::Controller_OutdoorAirFields::EconomizerControlActionType, modelObject.getEconomizerControlActionType());

    ///////////////////////////////////////////////////////////////////////////
    // Field: Economizer Maximum Limit DryBulb Temperature ////////////////////
    d = modelObject.getEconomizerMaximumLimitDryBulbTemperature();
    if (d) {
      idfObject.setDouble(openstudio::Controller_OutdoorAirFields::EconomizerMaximumLimitDryBulbTemperature, d.get());
    } else {
      idfObject.setString(openstudio::Controller_OutdoorAirFields::EconomizerMaximumLimitDryBulbTemperature, "");
    }

    ///////////////////////////////////////////////////////////////////////////
    // Field: Economizer Maximum Limit Enthalpy ///////////////////////////////
    d = modelObject.getEconomizerMaximumLimitEnthalpy();
    if (d) {
      idfObject.setDouble(openstudio::Controller_OutdoorAirFields::EconomizerMaximumLimitEnthalpy, d.get());
    } else {
      idfObject.setString(openstudio::Controller_OutdoorAirFields::EconomizerMaximumLimitEnthalpy, "");
    }

    ///////////////////////////////////////////////////////////////////////////
    // Field: Economizer Maximum Limit Dewpoint Temperature ///////////////////
    d = modelObject.getEconomizerMaximumLimitDewpointTemperature();
    if (d) {
      idfObject.setDouble(openstudio::Controller_OutdoorAirFields::EconomizerMaximumLimitDewpointTemperature, d.get());
    } else {
      idfObject.setString(openstudio::Controller_OutdoorAirFields::EconomizerMaximumLimitDewpointTemperature, "");
    }

    ///////////////////////////////////////////////////////////////////////////
    // Field: Electronic Enthalpy Limit Curve Name ////////////////////////////
    if (boost::optional<model::Curve> curve = modelObject.electronicEnthalpyLimitCurve()) {
      if (boost::optional<IdfObject> _curve = translateAndMapModelObject(curve.get())) {
        idfObject.setString(Controller_OutdoorAirFields::ElectronicEnthalpyLimitCurveName, _curve->name().get());
      }
    }

    ///////////////////////////////////////////////////////////////////////////
    // Field: Economizer Minimum Limit DryBulb Temperature ////////////////////
    d = modelObject.getEconomizerMinimumLimitDryBulbTemperature();
    if (d) {
      idfObject.setDouble(openstudio::Controller_OutdoorAirFields::EconomizerMinimumLimitDryBulbTemperature, d.get());
    } else {
      idfObject.setString(openstudio::Controller_OutdoorAirFields::EconomizerMinimumLimitDryBulbTemperature, "");
    }

    ///////////////////////////////////////////////////////////////////////////
    // Field: Lockout Type ////////////////////////////////////////////////////
    idfObject.setString(openstudio::Controller_OutdoorAirFields::LockoutType, modelObject.getLockoutType());

    ///////////////////////////////////////////////////////////////////////////
    // Field: Minimum Limit Type //////////////////////////////////////////////
    idfObject.setString(openstudio::Controller_OutdoorAirFields::MinimumLimitType, modelObject.getMinimumLimitType());

    // HighHumidityControl
    boost::optional<bool> ob;
    ob = modelObject.getHighHumidityControl();
    if (ob) {
      if (*ob) {
        idfObject.setString(openstudio::Controller_OutdoorAirFields::HighHumidityControl, "Yes");
      } else {
        idfObject.setString(openstudio::Controller_OutdoorAirFields::HighHumidityControl, "No");
      }
    }

    // HumidistatControlZoneName
    if (auto zone_ = modelObject.humidistatControlZone()) {
      if (boost::optional<ZoneControlHumidistat> humidistat = zone_->zoneControlHumidistat()) {
        idfObject.setString(openstudio::Controller_OutdoorAirFields::HumidistatControlZoneName, zone_->nameString());
      } else {
        LOG(Warn, modelObject.briefDescription() << " has a humidistat control zone " << zone_->nameString()
                                                 << " without a zone control humidistat; humidistat control zone field will not be translated");
        idfObject.setString(openstudio::Controller_OutdoorAirFields::HighHumidityControl, "No");
      }
    }

    // HighHumidityOutdoorAirFlowRatio
    d = modelObject.getHighHumidityOutdoorAirFlowRatio();
    if (d) {
      idfObject.setDouble(openstudio::Controller_OutdoorAirFields::HighHumidityOutdoorAirFlowRatio, *d);
    }

    // ControlHighIndoorHumidityBasedonOutdoorHumidityRatio
    if (modelObject.getControlHighIndoorHumidityBasedOnOutdoorHumidityRatio()) {
      idfObject.setString(openstudio::Controller_OutdoorAirFields::ControlHighIndoorHumidityBasedonOutdoorHumidityRatio, "Yes");
    } else {
      idfObject.setString(openstudio::Controller_OutdoorAirFields::ControlHighIndoorHumidityBasedonOutdoorHumidityRatio, "No");
    }

    // HeatRecoveryBypassControlType
    s = modelObject.getHeatRecoveryBypassControlType();
    if (s) {
      idfObject.setString(openstudio::Controller_OutdoorAirFields::HeatRecoveryBypassControlType, *s);
    }

    // EconomizerOperationStaging
    idfObject.setString(openstudio::Controller_OutdoorAirFields::EconomizerOperationStaging, modelObject.economizerOperationStaging());

    // Controller Mechanical Ventilation
    model::ControllerMechanicalVentilation controllerMechanicalVentilation = modelObject.controllerMechanicalVentilation();

    boost::optional<IdfObject> controllerMechanicalVentilationIdf = translateAndMapModelObject(controllerMechanicalVentilation);
    if (controllerMechanicalVentilationIdf) {
      idfObject.setString(openstudio::Controller_OutdoorAirFields::MechanicalVentilationControllerName,
                          controllerMechanicalVentilationIdf->name().get());
    }

    // MinimumOutdoorAirSchedule

    if (boost::optional<Schedule> s = modelObject.minimumOutdoorAirSchedule()) {
      if (boost::optional<IdfObject> _s = translateAndMapModelObject(s.get())) {
        idfObject.setString(openstudio::Controller_OutdoorAirFields::MinimumOutdoorAirScheduleName, _s->name().get());
      }
    }

    // MinimumFractionofOutdoorAirSchedule

    if (boost::optional<Schedule> s = modelObject.minimumFractionofOutdoorAirSchedule()) {
      if (boost::optional<IdfObject> _s = translateAndMapModelObject(s.get())) {
        idfObject.setString(openstudio::Controller_OutdoorAirFields::MinimumFractionofOutdoorAirScheduleName, _s->name().get());
      }
    }

    // MaximumFractionofOutdoorAirSchedule

    if (boost::optional<Schedule> s = modelObject.maximumFractionofOutdoorAirSchedule()) {
      if (boost::optional<IdfObject> _s = translateAndMapModelObject(s.get())) {
        idfObject.setString(openstudio::Controller_OutdoorAirFields::MaximumFractionofOutdoorAirScheduleName, _s->name().get());
      }
    }

    // TimeofDayEconomizerControlSchedule

    if (boost::optional<Schedule> s = modelObject.timeofDayEconomizerControlSchedule()) {
      if (boost::optional<IdfObject> _s = translateAndMapModelObject(s.get())) {
        idfObject.setString(openstudio::Controller_OutdoorAirFields::TimeofDayEconomizerControlScheduleName, _s->name().get());
      }
    }

    return boost::optional<IdfObject>(idfObject);
  }

}  // namespace energyplus

}  // namespace openstudio
