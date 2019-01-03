/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
#include "../../model/RefrigerationGasCoolerAirCooled.hpp"
#include "../../model/CurveLinear.hpp"
// #include <model/ThermalZone.hpp>

#include <utilities/idd/Refrigeration_GasCooler_AirCooled_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateRefrigerationGasCoolerAirCooled(RefrigerationGasCoolerAirCooled & modelObject)
{
  OptionalModelObject temp;
  OptionalString optS;
  boost::optional<std::string> s;
  boost::optional<double> d;

// Name
  IdfObject object = createRegisterAndNameIdfObject(openstudio::IddObjectType::Refrigeration_GasCooler_AirCooled, modelObject);

// Rated Total Heat Rejection Rate Curve Name
  boost::optional<CurveLinear> heatRejectCurve = modelObject.ratedTotalHeatRejectionRateCurve();

  if( heatRejectCurve )
  {
    boost::optional<IdfObject> _heatRejectCurve = translateAndMapModelObject(heatRejectCurve.get());

    if( _heatRejectCurve && _heatRejectCurve->name() )
    {
      object.setString(Refrigeration_GasCooler_AirCooledFields::RatedTotalHeatRejectionRateCurveName,_heatRejectCurve->name().get());
    }
  }

// Gas Cooler Fan Speed Control Type
  s = modelObject.gasCoolerFanSpeedControlType();
  if (s) {
    object.setString(Refrigeration_GasCooler_AirCooledFields::GasCoolerFanSpeedControlType,s.get());
  }

// Rated Fan Power
  d = modelObject.ratedFanPower();
  if (d) {
    object.setDouble(Refrigeration_GasCooler_AirCooledFields::RatedFanPower,d.get());
  }

// Minimum Fan Air Flow Ratio
  d = modelObject.minimumFanAirFlowRatio();
  if (d) {
    object.setDouble(Refrigeration_GasCooler_AirCooledFields::MinimumFanAirFlowRatio,d.get());
  }

// Transition Temperature
  d = modelObject.transitionTemperature();
  if (d) {
    object.setDouble(Refrigeration_GasCooler_AirCooledFields::TransitionTemperature,d.get());
  }

// Transcritical Approach Temperature
  d = modelObject.transcriticalApproachTemperature();
  if (d) {
    object.setDouble(Refrigeration_GasCooler_AirCooledFields::TranscriticalApproachTemperature,d.get());
  }

// Subcritical Temperature Difference
  d = modelObject.subcriticalTemperatureDifference();
  if (d) {
    object.setDouble(Refrigeration_GasCooler_AirCooledFields::SubcriticalTemperatureDifference,d.get());
  }

// Minimum Condensing Temperature
  d = modelObject.minimumCondensingTemperature();
  if (d) {
    object.setDouble(Refrigeration_GasCooler_AirCooledFields::MinimumCondensingTemperature,d.get());
  }

// Air Inlet Node Name
  // boost::optional<ThermalZone> airInletNode = modelObject.airInletNode();

  // if( airInletNode )
  // {
  //   boost::optional<IdfObject> _airInletNode = translateAndMapModelObject(airInletNode.get());

  //   if( _airInletNode && _airInletNode->name() )
  //   {
  //     object.setString(Refrigeration_GasCooler_AirCooledFields::AirInletNodeName,_airInletNode->name().get());
  //   }
  // }
  object.setString(Refrigeration_GasCooler_AirCooledFields::AirInletNodeName,"");

// End-Use Subcategory
  s = modelObject.endUseSubcategory();
  if (s) {
    object.setString(Refrigeration_GasCooler_AirCooledFields::EndUseSubcategory,s.get());
  }

// Gas Cooler Refrigerant Operating Charge Inventory
  d = modelObject.gasCoolerRefrigerantOperatingChargeInventory();
  if (d) {
    object.setDouble(Refrigeration_GasCooler_AirCooledFields::GasCoolerRefrigerantOperatingChargeInventory,d.get());
  }

// Gas Cooler Receiver Refrigerant Inventory
  d = modelObject.gasCoolerReceiverRefrigerantInventory();
  if (d) {
    object.setDouble(Refrigeration_GasCooler_AirCooledFields::GasCoolerReceiverRefrigerantInventory,d.get());
  }

// Gas Cooler Outlet Piping Refrigerant Inventory
  d = modelObject.gasCoolerOutletPipingRefrigerantInventory();
  if (d) {
    object.setDouble(Refrigeration_GasCooler_AirCooledFields::GasCoolerOutletPipingRefrigerantInventory,d.get());
  }

  return object;

}
}
}
