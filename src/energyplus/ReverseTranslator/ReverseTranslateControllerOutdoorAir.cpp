/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
