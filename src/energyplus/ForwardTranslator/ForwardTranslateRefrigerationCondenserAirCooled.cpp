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
#include "../../model/RefrigerationCondenserAirCooled.hpp"
#include "../../model/CurveLinear.hpp"
#include "../../model/ThermalZone.hpp"

#include <utilities/idd/Refrigeration_Condenser_AirCooled_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateRefrigerationCondenserAirCooled(RefrigerationCondenserAirCooled& modelObject) {
    OptionalModelObject temp;
    OptionalString optS;
    boost::optional<std::string> s;
    boost::optional<double> d;

    // Name
    IdfObject object = createRegisterAndNameIdfObject(openstudio::IddObjectType::Refrigeration_Condenser_AirCooled, modelObject);

    //Rated Effective Total Heat Rejection Rate Curve Name
    boost::optional<CurveLinear> heatRejectCurve = modelObject.ratedEffectiveTotalHeatRejectionRateCurve();

    if (heatRejectCurve) {
      boost::optional<IdfObject> _heatRejectCurve = translateAndMapModelObject(heatRejectCurve.get());

      if (_heatRejectCurve && _heatRejectCurve->name()) {
        object.setString(Refrigeration_Condenser_AirCooledFields::RatedEffectiveTotalHeatRejectionRateCurveName, _heatRejectCurve->name().get());
      }
    }

    //Rated Subcooling Temperature Difference
    d = modelObject.ratedSubcoolingTemperatureDifference();
    if (d) {
      object.setDouble(Refrigeration_Condenser_AirCooledFields::RatedSubcoolingTemperatureDifference, d.get());
    }

    //Condenser Fan Speed Control Type
    s = modelObject.condenserFanSpeedControlType();
    if (s) {
      object.setString(Refrigeration_Condenser_AirCooledFields::CondenserFanSpeedControlType, s.get());
    }

    //Rated Fan Power
    d = modelObject.ratedFanPower();
    if (d) {
      object.setDouble(Refrigeration_Condenser_AirCooledFields::RatedFanPower, d.get());
    }

    //Air Inlet Node Name or Zone Name
    boost::optional<ThermalZone> airInletZone = modelObject.airInletZone();

    if (airInletZone) {
      boost::optional<IdfObject> _airInletZone = translateAndMapModelObject(airInletZone.get());

      if (_airInletZone && _airInletZone->name()) {
        object.setString(Refrigeration_Condenser_AirCooledFields::AirInletNodeNameorZoneName, _airInletZone->name().get());
      }
    }

    //End-Use Subcategory
    s = modelObject.endUseSubcategory();
    if (s) {
      object.setString(Refrigeration_Condenser_AirCooledFields::EndUseSubcategory, s.get());
    }

    //Condenser Refrigerant Operating Charge Inventory
    d = modelObject.condenserRefrigerantOperatingChargeInventory();
    if (d) {
      object.setDouble(Refrigeration_Condenser_AirCooledFields::CondenserRefrigerantOperatingChargeInventory, d.get());
    }

    //Condensate Receiver Refrigerant Inventory
    d = modelObject.condensateReceiverRefrigerantInventory();
    if (d) {
      object.setDouble(Refrigeration_Condenser_AirCooledFields::CondensateReceiverRefrigerantInventory, d.get());
    }

    //Condensate Piping Refrigerant Inventory
    d = modelObject.condensatePipingRefrigerantInventory();
    if (d) {
      object.setDouble(Refrigeration_Condenser_AirCooledFields::CondensatePipingRefrigerantInventory, d.get());
    }

    return object;
  }
}  // namespace energyplus
}  // namespace openstudio
