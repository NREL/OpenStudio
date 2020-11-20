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
#include "../../model/RefrigerationCondenserEvaporativeCooled.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/Schedule.hpp"

#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include <utilities/idd/Refrigeration_Condenser_EvaporativeCooled_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject>
    ForwardTranslator::translateRefrigerationCondenserEvaporativeCooled(RefrigerationCondenserEvaporativeCooled& modelObject) {
    OptionalModelObject temp;
    OptionalString optS;
    boost::optional<std::string> s;
    boost::optional<double> d;

    // Name
    IdfObject object = createRegisterAndNameIdfObject(openstudio::IddObjectType::Refrigeration_Condenser_EvaporativeCooled, modelObject);

    //Rated Effective Total Heat Rejection Rate
    d = modelObject.ratedEffectiveTotalHeatRejectionRate();
    if (d) {
      object.setDouble(Refrigeration_Condenser_EvaporativeCooledFields::RatedEffectiveTotalHeatRejectionRate, d.get());
    }

    //Rated Subcooling Temperature Difference
    d = modelObject.ratedSubcoolingTemperatureDifference();
    if (d) {
      object.setDouble(Refrigeration_Condenser_EvaporativeCooledFields::RatedSubcoolingTemperatureDifference, d.get());
    }

    //Fan Speed Control Type
    s = modelObject.fanSpeedControlType();
    if (s) {
      object.setString(Refrigeration_Condenser_EvaporativeCooledFields::FanSpeedControlType, s.get());
    }

    //Rated Fan Power
    d = modelObject.ratedFanPower();
    if (d) {
      object.setDouble(Refrigeration_Condenser_EvaporativeCooledFields::RatedFanPower, d.get());
    }

    //Minimum Fan Air Flow Ratio
    d = modelObject.minimumFanAirFlowRatio();
    if (d) {
      object.setDouble(Refrigeration_Condenser_EvaporativeCooledFields::MinimumFanAirFlowRatio, d.get());
    }

    //Approach Temperature Constant Term
    d = modelObject.approachTemperatureConstantTerm();
    if (d) {
      object.setDouble(Refrigeration_Condenser_EvaporativeCooledFields::ApproachTemperatureConstantTerm, d.get());
    }

    //Approach Temperature Coefficient 2
    d = modelObject.approachTemperatureCoefficient2();
    if (d) {
      object.setDouble(Refrigeration_Condenser_EvaporativeCooledFields::ApproachTemperatureCoefficient2, d.get());
    }

    //Approach Temperature Coefficient 3
    d = modelObject.approachTemperatureCoefficient3();
    if (d) {
      object.setDouble(Refrigeration_Condenser_EvaporativeCooledFields::ApproachTemperatureCoefficient3, d.get());
    }

    //Approach Temperature Coefficient 4
    d = modelObject.approachTemperatureCoefficient4();
    if (d) {
      object.setDouble(Refrigeration_Condenser_EvaporativeCooledFields::ApproachTemperatureCoefficient4, d.get());
    }

    //Minimum Capacity Factor
    d = modelObject.minimumCapacityFactor();
    if (d) {
      object.setDouble(Refrigeration_Condenser_EvaporativeCooledFields::MinimumCapacityFactor, d.get());
    }

    //Maximum Capacity Factor
    d = modelObject.maximumCapacityFactor();
    if (d) {
      object.setDouble(Refrigeration_Condenser_EvaporativeCooledFields::MaximumCapacityFactor, d.get());
    }

    //Air Inlet Node Name
    object.setString(Refrigeration_Condenser_EvaporativeCooledFields::AirInletNodeName, "");

    //Rated Air Flow Rate
    d = modelObject.ratedAirFlowRate();
    if (d) {
      object.setDouble(Refrigeration_Condenser_EvaporativeCooledFields::RatedAirFlowRate, d.get());
    } else if (modelObject.isRatedAirFlowRateAutocalculated()) {
      object.setString(Refrigeration_Condenser_EvaporativeCooledFields::RatedAirFlowRate, "Autocalculate");
    }

    //Basin Heater Capacity
    d = modelObject.basinHeaterCapacity();
    if (d) {
      object.setDouble(Refrigeration_Condenser_EvaporativeCooledFields::BasinHeaterCapacity, d.get());
    }

    //Basin Heater Setpoint Temperature
    d = modelObject.basinHeaterSetpointTemperature();
    if (d) {
      object.setDouble(Refrigeration_Condenser_EvaporativeCooledFields::BasinHeaterSetpointTemperature, d.get());
    }

    //Rated Water Pump Power
    d = modelObject.ratedWaterPumpPower();
    if (d) {
      object.setDouble(Refrigeration_Condenser_EvaporativeCooledFields::RatedWaterPumpPower, d.get());
    } else if (modelObject.isRatedWaterPumpPowerAutocalculated()) {
      object.setString(Refrigeration_Condenser_EvaporativeCooledFields::RatedWaterPumpPower, "Autocalculate");
    }

    //Evaporative Water Supply Tank Name
    object.setString(Refrigeration_Condenser_EvaporativeCooledFields::EvaporativeWaterSupplyTankName, "");

    //Evaporative Condenser Availability Schedule Name
    boost::optional<Schedule> evaporativeCondenserAvailabilitySchedule = modelObject.evaporativeCondenserAvailabilitySchedule();

    if (evaporativeCondenserAvailabilitySchedule) {
      boost::optional<IdfObject> _evaporativeCondenserAvailabilitySchedule =
        translateAndMapModelObject(evaporativeCondenserAvailabilitySchedule.get());

      if (_evaporativeCondenserAvailabilitySchedule && _evaporativeCondenserAvailabilitySchedule->name()) {
        object.setString(Refrigeration_Condenser_EvaporativeCooledFields::EvaporativeCondenserAvailabilityScheduleName,
                         _evaporativeCondenserAvailabilitySchedule->name().get());
      }
    }

    //End-Use Subcategory
    s = modelObject.endUseSubcategory();
    if (s) {
      object.setString(Refrigeration_Condenser_EvaporativeCooledFields::EndUseSubcategory, s.get());
    }

    //Condenser Refrigerant Operating Charge Inventory
    d = modelObject.condenserRefrigerantOperatingChargeInventory();
    if (d) {
      object.setDouble(Refrigeration_Condenser_EvaporativeCooledFields::CondenserRefrigerantOperatingChargeInventory, d.get());
    }

    //Condensate Receiver Refrigerant Inventory
    d = modelObject.condensateReceiverRefrigerantInventory();
    if (d) {
      object.setDouble(Refrigeration_Condenser_EvaporativeCooledFields::CondensateReceiverRefrigerantInventory, d.get());
    }

    //Condensate Piping Refrigerant Inventory
    d = modelObject.condensatePipingRefrigerantInventory();
    if (d) {
      object.setDouble(Refrigeration_Condenser_EvaporativeCooledFields::CondensatePipingRefrigerantInventory, d.get());
    }

    return object;
  }
}  // namespace energyplus
}  // namespace openstudio
