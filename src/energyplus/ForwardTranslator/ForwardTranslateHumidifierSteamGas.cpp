/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
