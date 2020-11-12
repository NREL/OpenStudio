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
#include "../../model/SizingSystem.hpp"
#include "../../model/SizingSystem_Impl.hpp"
#include "../../model/AirLoopHVAC.hpp"
#include "../../model/AirLoopHVAC_Impl.hpp"
#include <utilities/idd/Sizing_System_FieldEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  OptionalModelObject ReverseTranslator::translateSizingSystem(const WorkspaceObject& workspaceObject) {
    boost::optional<WorkspaceObject> _airLoopHVAC = workspaceObject.getTarget(Sizing_SystemFields::AirLoopName);

    boost::optional<AirLoopHVAC> airLoopHVAC;

    if (_airLoopHVAC) {
      boost::optional<ModelObject> mo = translateAndMapWorkspaceObject(_airLoopHVAC.get());

      if (mo) {
        airLoopHVAC = mo->optionalCast<AirLoopHVAC>();
      }
    }

    if (!airLoopHVAC) {
      LOG(Error, "Error importing object: " << workspaceObject.briefDescription() << " Can't find associated AirLoopHVAC.");

      return boost::none;
    }

    openstudio::model::SizingSystem sizingSystem = airLoopHVAC->sizingSystem();

    boost::optional<std::string> s;
    boost::optional<double> value;

    // TypeofLoadtoSizeOn

    s = workspaceObject.getString(Sizing_SystemFields::TypeofLoadtoSizeOn);
    if (s) {
      sizingSystem.setTypeofLoadtoSizeOn(s.get());
    }

    // DesignOutdoorAirFlowRate

    s = workspaceObject.getString(Sizing_SystemFields::DesignOutdoorAirFlowRate);
    value = workspaceObject.getDouble(Sizing_SystemFields::DesignOutdoorAirFlowRate);
    if (value) {
      sizingSystem.setDesignOutdoorAirFlowRate(value.get());
    } else if (s && istringEqual(s.get(), "Autosize")) {
      sizingSystem.autosizeDesignOutdoorAirFlowRate();
    }

    // CentralHeatingMaximumSystemAirFlowRatio

    value = workspaceObject.getDouble(Sizing_SystemFields::CentralHeatingMaximumSystemAirFlowRatio);
    if (value) {
      sizingSystem.setCentralHeatingMaximumSystemAirFlowRatio(value.get());
    }

    // PreheatDesignTemperature

    value = workspaceObject.getDouble(Sizing_SystemFields::PreheatDesignTemperature);
    if (value) {
      sizingSystem.setPreheatDesignTemperature(value.get());
    }

    // PreheatDesignHumidityRatio

    value = workspaceObject.getDouble(Sizing_SystemFields::PreheatDesignHumidityRatio);
    if (value) {
      sizingSystem.setPreheatDesignHumidityRatio(value.get());
    }

    // PrecoolDesignTemperature

    value = workspaceObject.getDouble(Sizing_SystemFields::PrecoolDesignTemperature);
    if (value) {
      sizingSystem.setPrecoolDesignTemperature(value.get());
    }

    // PrecoolDesignHumidityRatio

    value = workspaceObject.getDouble(Sizing_SystemFields::PrecoolDesignHumidityRatio);
    if (value) {
      sizingSystem.setPrecoolDesignHumidityRatio(value.get());
    }

    // CentralCoolingDesignSupplyAirTemperature

    value = workspaceObject.getDouble(Sizing_SystemFields::CentralCoolingDesignSupplyAirTemperature);
    if (value) {
      sizingSystem.setCentralCoolingDesignSupplyAirTemperature(value.get());
    }

    // CentralHeatingDesignSupplyAirTemperature

    value = workspaceObject.getDouble(Sizing_SystemFields::CentralHeatingDesignSupplyAirTemperature);
    if (value) {
      sizingSystem.setCentralHeatingDesignSupplyAirTemperature(value.get());
    }

    // SizingOption

    s = workspaceObject.getString(Sizing_SystemFields::TypeofZoneSumtoUse);
    if (s) {
      sizingSystem.setSizingOption(s.get());
    }

    // AllOutdoorAirinCooling

    s = workspaceObject.getString(Sizing_SystemFields::AllOutdoorAirinCooling);
    if (s && istringEqual(s.get(), "Yes")) {
      sizingSystem.setAllOutdoorAirinCooling(true);
    } else if (s && istringEqual(s.get(), "No")) {
      sizingSystem.setAllOutdoorAirinCooling(false);
    }

    // AllOutdoorAirinHeating

    s = workspaceObject.getString(Sizing_SystemFields::AllOutdoorAirinHeating);
    if (s && istringEqual(s.get(), "Yes")) {
      sizingSystem.setAllOutdoorAirinHeating(true);
    } else if (s && istringEqual(s.get(), "No")) {
      sizingSystem.setAllOutdoorAirinHeating(false);
    }

    // CentralCoolingDesignSupplyAirHumidityRatio

    value = workspaceObject.getDouble(Sizing_SystemFields::CentralCoolingDesignSupplyAirHumidityRatio);
    if (value) {
      sizingSystem.setCentralCoolingDesignSupplyAirHumidityRatio(value.get());
    }

    // CentralHeatingDesignSupplyAirHumidityRatio

    value = workspaceObject.getDouble(Sizing_SystemFields::CentralHeatingDesignSupplyAirHumidityRatio);
    if (value) {
      sizingSystem.setCentralHeatingDesignSupplyAirHumidityRatio(value.get());
    }

    // CoolingDesignAirFlowMethod

    s = workspaceObject.getString(Sizing_SystemFields::CoolingSupplyAirFlowRateMethod);
    if (s) {
      sizingSystem.setCoolingDesignAirFlowMethod(s.get());
    }

    // CoolingDesignAirFlowRate

    value = workspaceObject.getDouble(Sizing_SystemFields::CoolingSupplyAirFlowRate);
    if (value) {
      sizingSystem.setCoolingDesignAirFlowRate(value.get());
    }

    // HeatingDesignAirFlowMethod

    s = workspaceObject.getString(Sizing_SystemFields::HeatingSupplyAirFlowRateMethod);
    if (s) {
      sizingSystem.setHeatingDesignAirFlowMethod(s.get());
    }

    // HeatingDesignAirFlowRate

    value = workspaceObject.getDouble(Sizing_SystemFields::HeatingSupplyAirFlowRate);
    if (value) {
      sizingSystem.setHeatingDesignAirFlowRate(value.get());
    }

    // SystemOutdoorAirMethod

    s = workspaceObject.getString(Sizing_SystemFields::SystemOutdoorAirMethod);
    if (s) {
      sizingSystem.setSystemOutdoorAirMethod(s.get());
    }

    return sizingSystem;
  }

}  // namespace energyplus

}  // namespace openstudio
