/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
 *  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **********************************************************************/

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
