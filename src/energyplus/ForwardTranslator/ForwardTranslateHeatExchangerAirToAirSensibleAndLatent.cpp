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

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"
#include "../../model/HeatExchangerAirToAirSensibleAndLatent.hpp"
#include "../../model/HeatExchangerAirToAirSensibleAndLatent_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../utilities/idf/Workspace.hpp"
#include "../../utilities/core/Logger.hpp"
#include <utilities/idd/HeatExchanger_AirToAir_SensibleAndLatent_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateHeatExchangerAirToAirSensibleAndLatent(HeatExchangerAirToAirSensibleAndLatent& modelObject) {
    OptionalString s;
    OptionalDouble d;
    OptionalModelObject temp;

    IdfObject idfObject(IddObjectType::HeatExchanger_AirToAir_SensibleAndLatent);

    m_idfObjects.push_back(idfObject);

    // Name
    s = modelObject.name();
    if (s) {
      idfObject.setName(*s);
    }

    // AvailabilityScheduleName
    Schedule sched = modelObject.availabilitySchedule();
    translateAndMapModelObject(sched);
    idfObject.setString(HeatExchanger_AirToAir_SensibleAndLatentFields::AvailabilityScheduleName, sched.name().get());

    // NominalSupplyAirFlowRate
    if (modelObject.isNominalSupplyAirFlowRateAutosized()) {
      idfObject.setString(HeatExchanger_AirToAir_SensibleAndLatentFields::NominalSupplyAirFlowRate, "Autosize");
    } else {
      d = modelObject.nominalSupplyAirFlowRate();
      if (d) {
        idfObject.setDouble(HeatExchanger_AirToAir_SensibleAndLatentFields::NominalSupplyAirFlowRate, d.get());
      }
    }

    // SensibleEffectivenessat100_HeatingAirFlow
    d = modelObject.sensibleEffectivenessat100HeatingAirFlow();
    if (d) {
      idfObject.setDouble(HeatExchanger_AirToAir_SensibleAndLatentFields::SensibleEffectivenessat100_HeatingAirFlow, d.get());
    }

    // LatentEffectivenessat100_HeatingAirFlow
    d = modelObject.latentEffectivenessat100HeatingAirFlow();
    if (d) {
      idfObject.setDouble(HeatExchanger_AirToAir_SensibleAndLatentFields::LatentEffectivenessat100_HeatingAirFlow, d.get());
    }

    // SensibleEffectivenessat75_HeatingAirFlow
    d = modelObject.sensibleEffectivenessat75HeatingAirFlow();
    if (d) {
      idfObject.setDouble(HeatExchanger_AirToAir_SensibleAndLatentFields::SensibleEffectivenessat75_HeatingAirFlow, d.get());
    }

    // LatentEffectivenessat75_HeatingAirFlow
    d = modelObject.latentEffectivenessat75HeatingAirFlow();
    if (d) {
      idfObject.setDouble(HeatExchanger_AirToAir_SensibleAndLatentFields::LatentEffectivenessat75_HeatingAirFlow, d.get());
    }

    // SensibleEffectivenessat100_CoolingAirFlow
    d = modelObject.sensibleEffectivenessat100CoolingAirFlow();
    if (d) {
      idfObject.setDouble(HeatExchanger_AirToAir_SensibleAndLatentFields::SensibleEffectivenessat100_CoolingAirFlow, d.get());
    }

    // LatentEffectivenessat100_CoolingAirFlow
    d = modelObject.latentEffectivenessat100CoolingAirFlow();
    if (d) {
      idfObject.setDouble(HeatExchanger_AirToAir_SensibleAndLatentFields::LatentEffectivenessat100_CoolingAirFlow, d.get());
    }

    // SensibleEffectivenessat75_CoolingAirFlow
    d = modelObject.sensibleEffectivenessat75CoolingAirFlow();
    if (d) {
      idfObject.setDouble(HeatExchanger_AirToAir_SensibleAndLatentFields::SensibleEffectivenessat75_CoolingAirFlow, d.get());
    }

    // LatentEffectivenessat75_CoolingAirFlow
    d = modelObject.latentEffectivenessat75CoolingAirFlow();
    if (d) {
      idfObject.setDouble(HeatExchanger_AirToAir_SensibleAndLatentFields::LatentEffectivenessat75_CoolingAirFlow, d.get());
    }

    // SupplyAirInletNodeName
    temp = modelObject.primaryAirInletModelObject();
    if (temp) {
      s = temp->name();
      if (s) {
        idfObject.setString(HeatExchanger_AirToAir_SensibleAndLatentFields::SupplyAirInletNodeName, *s);
      }
    }

    // SupplyAirOutletNodeName
    temp = modelObject.primaryAirOutletModelObject();
    if (temp) {
      s = temp->name();
      if (s) {
        idfObject.setString(HeatExchanger_AirToAir_SensibleAndLatentFields::SupplyAirOutletNodeName, *s);
      }
    }

    // ExhaustAirInletNodeName
    temp = modelObject.secondaryAirInletModelObject();
    if (temp) {
      s = temp->name();
      if (s) {
        idfObject.setString(HeatExchanger_AirToAir_SensibleAndLatentFields::ExhaustAirInletNodeName, *s);
      }
    }

    // ExhaustAirOutletNodeName
    temp = modelObject.secondaryAirOutletModelObject();
    if (temp) {
      s = temp->name();
      if (s) {
        idfObject.setString(HeatExchanger_AirToAir_SensibleAndLatentFields::ExhaustAirOutletNodeName, *s);
      }
    }

    // NominalElectricPower
    d = modelObject.nominalElectricPower();
    if (d) {
      idfObject.setDouble(HeatExchanger_AirToAir_SensibleAndLatentFields::NominalElectricPower, *d);
    }

    // SupplyAirOutletTemperatureControl
    if (modelObject.supplyAirOutletTemperatureControl()) {
      idfObject.setString(HeatExchanger_AirToAir_SensibleAndLatentFields::SupplyAirOutletTemperatureControl, "Yes");
    } else {
      idfObject.setString(HeatExchanger_AirToAir_SensibleAndLatentFields::SupplyAirOutletTemperatureControl, "No");
    }

    // HeatExchangerType
    s = modelObject.heatExchangerType();
    if (s) {
      idfObject.setString(HeatExchanger_AirToAir_SensibleAndLatentFields::HeatExchangerType, *s);
    }

    // FrostControlType
    s = modelObject.frostControlType();
    if (s) {
      idfObject.setString(HeatExchanger_AirToAir_SensibleAndLatentFields::FrostControlType, *s);
    }

    // ThresholdTemperature
    d = modelObject.thresholdTemperature();
    if (d) {
      idfObject.setDouble(HeatExchanger_AirToAir_SensibleAndLatentFields::ThresholdTemperature, *d);
    }

    // InitialDefrostTimeFraction
    d = modelObject.initialDefrostTimeFraction();
    if (d) {
      idfObject.setDouble(HeatExchanger_AirToAir_SensibleAndLatentFields::InitialDefrostTimeFraction, *d);
    }

    // RateofDefrostTimeFractionIncrease
    d = modelObject.rateofDefrostTimeFractionIncrease();
    if (d) {
      idfObject.setDouble(HeatExchanger_AirToAir_SensibleAndLatentFields::RateofDefrostTimeFractionIncrease, *d);
    }

    // EconomizerLockout
    if (modelObject.economizerLockout()) {
      idfObject.setString(HeatExchanger_AirToAir_SensibleAndLatentFields::EconomizerLockout, "Yes");
    } else {
      idfObject.setString(HeatExchanger_AirToAir_SensibleAndLatentFields::EconomizerLockout, "No");
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
