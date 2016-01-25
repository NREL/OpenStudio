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

boost::optional<IdfObject> ForwardTranslator::translateRefrigerationCondenserAirCooled( RefrigerationCondenserAirCooled & modelObject )
{
  OptionalModelObject temp;
  OptionalString optS;
  boost::optional<std::string> s;
  boost::optional<double> d;

// Name
  IdfObject object = createRegisterAndNameIdfObject(openstudio::IddObjectType::Refrigeration_Condenser_AirCooled, modelObject);

//Rated Effective Total Heat Rejection Rate Curve Name
  boost::optional<CurveLinear> heatRejectCurve = modelObject.ratedEffectiveTotalHeatRejectionRateCurve();

  if( heatRejectCurve )
  {
    boost::optional<IdfObject> _heatRejectCurve = translateAndMapModelObject(heatRejectCurve.get());

    if( _heatRejectCurve && _heatRejectCurve->name() )
    {
      object.setString(Refrigeration_Condenser_AirCooledFields::RatedEffectiveTotalHeatRejectionRateCurveName,_heatRejectCurve->name().get());
    }
  }

//Rated Subcooling Temperature Difference
  d = modelObject.ratedSubcoolingTemperatureDifference();
  if (d) {
    object.setDouble(Refrigeration_Condenser_AirCooledFields::RatedSubcoolingTemperatureDifference,d.get());
  }

//Condenser Fan Speed Control Type
  s = modelObject.condenserFanSpeedControlType();
  if (s) {
    object.setString(Refrigeration_Condenser_AirCooledFields::CondenserFanSpeedControlType,s.get());
  }

//Rated Fan Power
  d = modelObject.ratedFanPower();
  if (d) {
    object.setDouble(Refrigeration_Condenser_AirCooledFields::RatedFanPower,d.get());
  }

//Air Inlet Node Name or Zone Name
  boost::optional<ThermalZone> airInletZone = modelObject.airInletZone();

  if( airInletZone )
  {
    boost::optional<IdfObject> _airInletZone = translateAndMapModelObject(airInletZone.get());

    if( _airInletZone && _airInletZone->name() )
    {
      object.setString(Refrigeration_Condenser_AirCooledFields::AirInletNodeNameorZoneName,_airInletZone->name().get());
    }
  }

//End-Use Subcategory
  s = modelObject.endUseSubcategory();
  if (s) {
    object.setString(Refrigeration_Condenser_AirCooledFields::EndUseSubcategory,s.get());
  }

//Condenser Refrigerant Operating Charge Inventory
  d = modelObject.condenserRefrigerantOperatingChargeInventory();
  if (d) {
    object.setDouble(Refrigeration_Condenser_AirCooledFields::CondenserRefrigerantOperatingChargeInventory,d.get());
  }

//Condensate Receiver Refrigerant Inventory
  d = modelObject.condensateReceiverRefrigerantInventory();
  if (d) {
    object.setDouble(Refrigeration_Condenser_AirCooledFields::CondensateReceiverRefrigerantInventory,d.get());
  }

//Condensate Piping Refrigerant Inventory
  d = modelObject.condensatePipingRefrigerantInventory();
  if (d) {
    object.setDouble(Refrigeration_Condenser_AirCooledFields::CondensatePipingRefrigerantInventory,d.get());
  }

  return object;

}
}
}
