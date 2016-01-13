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
#include "../../model/RefrigerationCondenserWaterCooled.hpp"
#include "../../model/Schedule.hpp"

#include <utilities/idd/Refrigeration_Condenser_WaterCooled_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateRefrigerationCondenserWaterCooled( RefrigerationCondenserWaterCooled & modelObject )
{
  OptionalModelObject temp;
  OptionalString optS;
  boost::optional<std::string> s;
  boost::optional<double> d;

  // Name
  IdfObject waterCooled = createRegisterAndNameIdfObject(openstudio::IddObjectType::Refrigeration_Condenser_WaterCooled, modelObject);

  // Rated Effective Total Heat Rejection Rate
  d = modelObject.ratedEffectiveTotalHeatRejectionRate();
  if (d) {
    waterCooled.setDouble(Refrigeration_Condenser_WaterCooledFields::RatedEffectiveTotalHeatRejectionRate,d.get());
  }

  // Rated Condensing Temperature
  d = modelObject.ratedCondensingTemperature();
  if (d) {
    waterCooled.setDouble(Refrigeration_Condenser_WaterCooledFields::RatedCondensingTemperature,d.get());
  }
        
  // Rated Subcooling Temperature Difference
  d = modelObject.ratedSubcoolingTemperatureDifference();
  if (d) {
    waterCooled.setDouble(Refrigeration_Condenser_WaterCooledFields::RatedSubcoolingTemperatureDifference,d.get());
  }
   
  // Rated Water Inlet Temperature
  d = modelObject.ratedWaterInletTemperature();
  if (d) {
    waterCooled.setDouble(Refrigeration_Condenser_WaterCooledFields::RatedWaterInletTemperature,d.get());
  }

  // Water Inlet Node Name
  if( (temp = modelObject.inletModelObject()) )
  {
    if(temp->name())
    {
      waterCooled.setString(Refrigeration_Condenser_WaterCooledFields::WaterInletNodeName,temp->name().get());
    }
  }
  
  // Water Outlet Node Name
  if( (temp = modelObject.outletModelObject()) )
  {
    if(temp->name())
    {
      waterCooled.setString(Refrigeration_Condenser_WaterCooledFields::WaterOutletNodeName,temp->name().get());
    }
  }

  // Water-Cooled Loop Flow Type
  s = modelObject.waterCooledLoopFlowType();
  if (s) {
    waterCooled.setString(Refrigeration_Condenser_WaterCooledFields::WaterCooledLoopFlowType,s.get());
  }

  // Water Outlet Temperature Schedule Name
  boost::optional<Schedule> waterOutletTemperatureSchedule = modelObject.waterOutletTemperatureSchedule();

  if( waterOutletTemperatureSchedule )
  {
    boost::optional<IdfObject> _waterOutletTemperatureSchedule = translateAndMapModelObject(waterOutletTemperatureSchedule.get());

    if( _waterOutletTemperatureSchedule && _waterOutletTemperatureSchedule->name() )
    {
      waterCooled.setString(Refrigeration_Condenser_WaterCooledFields::WaterOutletTemperatureScheduleName,_waterOutletTemperatureSchedule->name().get());
    }
  }
  

  // Water Design Flow Rate
  d = modelObject.waterDesignFlowRate();
  if (d) {
    waterCooled.setDouble(Refrigeration_Condenser_WaterCooledFields::WaterDesignFlowRate,d.get());
  }

  // Water Maximum Flow Rate
  d = modelObject.waterMaximumFlowRate();
  if (d) {
    waterCooled.setDouble(Refrigeration_Condenser_WaterCooledFields::WaterMaximumFlowRate,d.get());
  }

  // Water Maximum Water Outlet Temperature
  d = modelObject.waterMaximumWaterOutletTemperature();
  if (d) {
    waterCooled.setDouble(Refrigeration_Condenser_WaterCooledFields::WaterMaximumWaterOutletTemperature,d.get());
  }

  // Water Minimum Water Inlet Temperature
  d = modelObject.waterMinimumWaterInletTemperature();
  if (d) {
    waterCooled.setDouble(Refrigeration_Condenser_WaterCooledFields::WaterMinimumWaterInletTemperature,d.get());
  }

  // End-Use Subcategory
  s = modelObject.endUseSubcategory();
  if (s) {
    waterCooled.setString(Refrigeration_Condenser_WaterCooledFields::EndUseSubcategory,s.get());
  }

  // Condenser Refrigerant Operating Charge Inventory
  d = modelObject.condenserRefrigerantOperatingChargeInventory();
  if (d) {
    waterCooled.setDouble(Refrigeration_Condenser_WaterCooledFields::CondenserRefrigerantOperatingChargeInventory,d.get());
  }

  // Condensate Receiver Refrigerant Inventory
  d = modelObject.condensateReceiverRefrigerantInventory();
  if (d) {
    waterCooled.setDouble(Refrigeration_Condenser_WaterCooledFields::CondensateReceiverRefrigerantInventory,d.get());
  }

  //Condensate Piping Refrigerant Inventory
  d = modelObject.condensatePipingRefrigerantInventory();
  if (d) {
    waterCooled.setDouble(Refrigeration_Condenser_WaterCooledFields::CondensatePipingRefrigerantInventory,d.get());
  }
  
  return waterCooled;

}
}
}
