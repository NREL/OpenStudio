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
#include "../../model/RefrigerationCondenserWaterCooled.hpp"
#include "../../model/Schedule.hpp"

#include <utilities/idd/Refrigeration_Condenser_WaterCooled_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateRefrigerationCondenserWaterCooled(RefrigerationCondenserWaterCooled& modelObject) {
    OptionalModelObject temp;
    OptionalString optS;
    boost::optional<std::string> s;
    boost::optional<double> d;

    // Name
    IdfObject waterCooled = createRegisterAndNameIdfObject(openstudio::IddObjectType::Refrigeration_Condenser_WaterCooled, modelObject);

    // Rated Effective Total Heat Rejection Rate
    d = modelObject.ratedEffectiveTotalHeatRejectionRate();
    if (d) {
      waterCooled.setDouble(Refrigeration_Condenser_WaterCooledFields::RatedEffectiveTotalHeatRejectionRate, d.get());
    }

    // Rated Condensing Temperature
    d = modelObject.ratedCondensingTemperature();
    if (d) {
      waterCooled.setDouble(Refrigeration_Condenser_WaterCooledFields::RatedCondensingTemperature, d.get());
    }

    // Rated Subcooling Temperature Difference
    d = modelObject.ratedSubcoolingTemperatureDifference();
    if (d) {
      waterCooled.setDouble(Refrigeration_Condenser_WaterCooledFields::RatedSubcoolingTemperatureDifference, d.get());
    }

    // Rated Water Inlet Temperature
    d = modelObject.ratedWaterInletTemperature();
    if (d) {
      waterCooled.setDouble(Refrigeration_Condenser_WaterCooledFields::RatedWaterInletTemperature, d.get());
    }

    // Water Inlet Node Name
    if ((temp = modelObject.inletModelObject())) {
      if (temp->name()) {
        waterCooled.setString(Refrigeration_Condenser_WaterCooledFields::WaterInletNodeName, temp->name().get());
      }
    }

    // Water Outlet Node Name
    if ((temp = modelObject.outletModelObject())) {
      if (temp->name()) {
        waterCooled.setString(Refrigeration_Condenser_WaterCooledFields::WaterOutletNodeName, temp->name().get());
      }
    }

    // Water-Cooled Loop Flow Type
    s = modelObject.waterCooledLoopFlowType();
    if (s) {
      waterCooled.setString(Refrigeration_Condenser_WaterCooledFields::WaterCooledLoopFlowType, s.get());
    }

    // Water Outlet Temperature Schedule Name
    boost::optional<Schedule> waterOutletTemperatureSchedule = modelObject.waterOutletTemperatureSchedule();

    if (waterOutletTemperatureSchedule) {
      boost::optional<IdfObject> _waterOutletTemperatureSchedule = translateAndMapModelObject(waterOutletTemperatureSchedule.get());

      if (_waterOutletTemperatureSchedule && _waterOutletTemperatureSchedule->name()) {
        waterCooled.setString(Refrigeration_Condenser_WaterCooledFields::WaterOutletTemperatureScheduleName,
                              _waterOutletTemperatureSchedule->name().get());
      }
    }

    // Water Design Flow Rate
    d = modelObject.waterDesignFlowRate();
    if (d) {
      waterCooled.setDouble(Refrigeration_Condenser_WaterCooledFields::WaterDesignFlowRate, d.get());
    }

    // Water Maximum Flow Rate
    d = modelObject.waterMaximumFlowRate();
    if (d) {
      waterCooled.setDouble(Refrigeration_Condenser_WaterCooledFields::WaterMaximumFlowRate, d.get());
    }

    // Water Maximum Water Outlet Temperature
    d = modelObject.waterMaximumWaterOutletTemperature();
    if (d) {
      waterCooled.setDouble(Refrigeration_Condenser_WaterCooledFields::WaterMaximumWaterOutletTemperature, d.get());
    }

    // Water Minimum Water Inlet Temperature
    d = modelObject.waterMinimumWaterInletTemperature();
    if (d) {
      waterCooled.setDouble(Refrigeration_Condenser_WaterCooledFields::WaterMinimumWaterInletTemperature, d.get());
    }

    // End-Use Subcategory
    s = modelObject.endUseSubcategory();
    if (s) {
      waterCooled.setString(Refrigeration_Condenser_WaterCooledFields::EndUseSubcategory, s.get());
    }

    // Condenser Refrigerant Operating Charge Inventory
    d = modelObject.condenserRefrigerantOperatingChargeInventory();
    if (d) {
      waterCooled.setDouble(Refrigeration_Condenser_WaterCooledFields::CondenserRefrigerantOperatingChargeInventory, d.get());
    }

    // Condensate Receiver Refrigerant Inventory
    d = modelObject.condensateReceiverRefrigerantInventory();
    if (d) {
      waterCooled.setDouble(Refrigeration_Condenser_WaterCooledFields::CondensateReceiverRefrigerantInventory, d.get());
    }

    //Condensate Piping Refrigerant Inventory
    d = modelObject.condensatePipingRefrigerantInventory();
    if (d) {
      waterCooled.setDouble(Refrigeration_Condenser_WaterCooledFields::CondensatePipingRefrigerantInventory, d.get());
    }

    return waterCooled;
  }
}  // namespace energyplus
}  // namespace openstudio
