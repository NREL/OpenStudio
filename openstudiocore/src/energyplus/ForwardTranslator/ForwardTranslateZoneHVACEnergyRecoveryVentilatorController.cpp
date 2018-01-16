/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

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

boost::optional<IdfObject> ForwardTranslator::translateZoneHVACEnergyRecoveryVentilatorController(
    ZoneHVACEnergyRecoveryVentilatorController & modelObject )
{
  boost::optional<std::string> s;
  boost::optional<double> value;
  boost::optional<ModelObject> temp;

  // Name
  IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::ZoneHVAC_EnergyRecoveryVentilator_Controller, modelObject);

  // TemperatureHighLimit
  if( (value = modelObject.temperatureHighLimit()) ) {
    idfObject.setDouble(ZoneHVAC_EnergyRecoveryVentilator_ControllerFields::TemperatureHighLimit,value.get());
  }

  // TemperatureLowLimit
  if( (value = modelObject.temperatureLowLimit()) ) {
    idfObject.setDouble(ZoneHVAC_EnergyRecoveryVentilator_ControllerFields::TemperatureLowLimit,value.get());
  }

  // EnthalpyHighLimit
  if( (value = modelObject.enthalpyHighLimit()) ) {
    idfObject.setDouble(ZoneHVAC_EnergyRecoveryVentilator_ControllerFields::EnthalpyHighLimit,value.get());
  }

  // DewpointTemperatureLimit
  if( (value = modelObject.dewpointTemperatureLimit()) ) {
    idfObject.setDouble(ZoneHVAC_EnergyRecoveryVentilator_ControllerFields::DewpointTemperatureLimit,value.get());
  }

  // ElectronicEnthalpyLimitCurveName
  if( (temp = modelObject.electronicEnthalpyLimitCurve()) ) {
    if( auto _curve = translateAndMapModelObject(temp.get()) ) {
      idfObject.setString(ZoneHVAC_EnergyRecoveryVentilator_ControllerFields::ElectronicEnthalpyLimitCurveName,_curve->name().get());
    }
  }

  // ExhaustAirTemperatureLimit
  if( (s = modelObject.exhaustAirTemperatureLimit()) ) {
    idfObject.setString(ZoneHVAC_EnergyRecoveryVentilator_ControllerFields::ExhaustAirTemperatureLimit,s.get());
  }

  // ExhaustAirEnthalpyLimit
  if( (s = modelObject.exhaustAirEnthalpyLimit()) ) {
    idfObject.setString(ZoneHVAC_EnergyRecoveryVentilator_ControllerFields::ExhaustAirEnthalpyLimit,s.get());
  }

  // TimeofDayEconomizerFlowControlScheduleName
  if( (temp = modelObject.timeofDayEconomizerFlowControlSchedule()) ) {
    if( auto _schedule = translateAndMapModelObject(temp.get()) ) {
      idfObject.setString(ZoneHVAC_EnergyRecoveryVentilator_ControllerFields::TimeofDayEconomizerFlowControlScheduleName,_schedule->name().get());
    }
  }

  // HighHumidityControlFlag
  if( modelObject.highHumidityControlFlag() ) {
    idfObject.setString(ZoneHVAC_EnergyRecoveryVentilator_ControllerFields::HighHumidityControlFlag,"Yes");
  } else {
    idfObject.setString(ZoneHVAC_EnergyRecoveryVentilator_ControllerFields::HighHumidityControlFlag,"No");
  }

  // HumidistatControlZoneName
  // Set in parent object

  // HighHumidityOutdoorAirFlowRatio
  if( (value = modelObject.highHumidityOutdoorAirFlowRatio()) ) {
    idfObject.setDouble(ZoneHVAC_EnergyRecoveryVentilator_ControllerFields::HighHumidityOutdoorAirFlowRatio,value.get());
  }
  // ControlHighIndoorHumidityBasedonOutdoorHumidityRatio
  if( modelObject.controlHighIndoorHumidityBasedonOutdoorHumidityRatio() ) {
    idfObject.setString(ZoneHVAC_EnergyRecoveryVentilator_ControllerFields::ControlHighIndoorHumidityBasedonOutdoorHumidityRatio,"Yes");
  } else {
    idfObject.setString(ZoneHVAC_EnergyRecoveryVentilator_ControllerFields::ControlHighIndoorHumidityBasedonOutdoorHumidityRatio,"No");
  }

  return idfObject;
}

} // energyplus

} // openstudio

