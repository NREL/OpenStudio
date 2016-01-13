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
#include "../../model/SizingSystem.hpp"
#include "../../model/SizingSystem_Impl.hpp"
#include "../../model/AirLoopHVAC.hpp"
#include "../../model/AirLoopHVAC_Impl.hpp"
#include "../../utilities/core/Logger.hpp"
#include "../../utilities/core/Assert.hpp"
#include <utilities/idd/Sizing_System_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateSizingSystem( SizingSystem & modelObject )
{
  boost::optional<std::string> s;
  boost::optional<double> value;

  IdfObject idfObject(IddObjectType::Sizing_System);

  m_idfObjects.push_back(idfObject);

  // AirLoopName

  s = modelObject.airLoopHVAC().name();
  if( s )
  {
    idfObject.setString(Sizing_SystemFields::AirLoopName,s.get());
  }

  // TypeofLoadtoSizeOn

  s = modelObject.typeofLoadtoSizeOn();
  if( s )
  {
    idfObject.setString(Sizing_SystemFields::TypeofLoadtoSizeOn,s.get());
  }

  // DesignOutdoorAirFlowRate

  if( modelObject.isDesignOutdoorAirFlowRateAutosized() )
  {
    idfObject.setString(Sizing_SystemFields::DesignOutdoorAirFlowRate,"Autosize");
  }
  else if( (value = modelObject.designOutdoorAirFlowRate()) )
  {
    idfObject.setDouble(Sizing_SystemFields::DesignOutdoorAirFlowRate,value.get());
  }

  // MinimumSystemAirFlowRatio

  value = modelObject.minimumSystemAirFlowRatio();
  if( value )
  {
    idfObject.setDouble(Sizing_SystemFields::CentralHeatingMaximumSystemAirFlowRatio,value.get());
  }

  // PreheatDesignTemperature
  
  value = modelObject.preheatDesignTemperature();
  if( value )
  {
    idfObject.setDouble(Sizing_SystemFields::PreheatDesignTemperature,value.get());
  }

  // PreheatDesignHumidityRatio

  value = modelObject.preheatDesignHumidityRatio(); 
  if( value )
  {
    idfObject.setDouble(Sizing_SystemFields::PreheatDesignHumidityRatio,value.get());
  }

  // PrecoolDesignTemperature

  value = modelObject.precoolDesignTemperature();
  if( value )
  {
    idfObject.setDouble(Sizing_SystemFields::PrecoolDesignTemperature,value.get());
  }

  // PrecoolDesignHumidityRatio

  value = modelObject.precoolDesignHumidityRatio();
  if( value )
  {
    idfObject.setDouble(Sizing_SystemFields::PrecoolDesignHumidityRatio,value.get());
  }

  // CentralCoolingDesignSupplyAirTemperature

  value = modelObject.centralCoolingDesignSupplyAirTemperature();
  if( value )
  {
    idfObject.setDouble(Sizing_SystemFields::CentralCoolingDesignSupplyAirTemperature,value.get());
  }

  // CentralHeatingDesignSupplyAirTemperature

  value = modelObject.centralHeatingDesignSupplyAirTemperature();
  if( value )
  {
    idfObject.setDouble(Sizing_SystemFields::CentralHeatingDesignSupplyAirTemperature,value.get());
  }

  // SizingOption

  s = modelObject.sizingOption();
  if( s )
  {
    idfObject.setString(Sizing_SystemFields::TypeofZoneSumtoUse,s.get());
  }

  // AllOutdoorAirinCooling
  
  bool b = modelObject.allOutdoorAirinCooling(); 
  if( b )
  {
    idfObject.setString(Sizing_SystemFields::AllOutdoorAirinCooling,"Yes");
  }
  else
  {
    idfObject.setString(Sizing_SystemFields::AllOutdoorAirinCooling,"No");
  }

  // AllOutdoorAirinHeating
  
  b = modelObject.allOutdoorAirinHeating(); 
  if( b )
  {
    idfObject.setString(Sizing_SystemFields::AllOutdoorAirinHeating,"Yes");
  }
  else
  {
    idfObject.setString(Sizing_SystemFields::AllOutdoorAirinHeating,"No");
  }

  // CentralCoolingDesignSupplyAirHumidityRatio
  
  value = modelObject.centralCoolingDesignSupplyAirHumidityRatio();
  if( value )
  {
    idfObject.setDouble(Sizing_SystemFields::CentralCoolingDesignSupplyAirHumidityRatio,value.get());
  } 

  // CentralHeatingDesignSupplyAirHumidityRatio
  
  value = modelObject.centralHeatingDesignSupplyAirHumidityRatio();
  if( value )
  {
    idfObject.setDouble(Sizing_SystemFields::CentralHeatingDesignSupplyAirHumidityRatio,value.get());
  } 

  // CoolingDesignAirFlowMethod

  s = modelObject.coolingDesignAirFlowMethod();
  if( s )
  {
    idfObject.setString(Sizing_SystemFields::CoolingSupplyAirFlowRateMethod,s.get());
  }

  // CoolingDesignAirFlowRate

  value = modelObject.coolingDesignAirFlowRate();
  if( value )
  {
    idfObject.setDouble(Sizing_SystemFields::CoolingSupplyAirFlowRate,value.get());
  }

  // HeatingDesignAirFlowMethod

  s = modelObject.heatingDesignAirFlowMethod();
  if( s )
  {
    idfObject.setString(Sizing_SystemFields::HeatingSupplyAirFlowRateMethod,s.get());
  }

  // HeatingDesignAirFlowRate

  value = modelObject.heatingDesignAirFlowRate();
  if( value )
  {
    idfObject.setDouble(Sizing_SystemFields::HeatingSupplyAirFlowRate,value.get());
  }

  // SystemOutdoorAirMethod
  
  s = modelObject.systemOutdoorAirMethod();
  if( s )
  {
    idfObject.setString(Sizing_SystemFields::SystemOutdoorAirMethod,s.get());
  }

  // ZoneMaximumOutdoorAirFraction
  value = modelObject.zoneMaximumOutdoorAirFraction();
  if( value ) {
    idfObject.setDouble(Sizing_SystemFields::ZoneMaximumOutdoorAirFraction,value.get());
  }

  // CoolingSupplyAirFlowRatePerFloorArea
  value = modelObject.coolingSupplyAirFlowRatePerFloorArea();
  if( value ) {
    idfObject.setDouble(Sizing_SystemFields::CoolingSupplyAirFlowRatePerFloorArea,value.get());
  }
  
  // CoolingFractionofAutosizedCoolingSupplyAirFlowRate
  value = modelObject.coolingFractionofAutosizedCoolingSupplyAirFlowRate();
  if( value ) {
    idfObject.setDouble(Sizing_SystemFields::CoolingFractionofAutosizedCoolingSupplyAirFlowRate,value.get());
  }

  // CoolingSupplyAirFlowRatePerUnitCoolingCapacity
  value = modelObject.coolingSupplyAirFlowRatePerUnitCoolingCapacity();
  if( value ) {
    idfObject.setDouble(Sizing_SystemFields::CoolingSupplyAirFlowRatePerUnitCoolingCapacity,value.get());
  }

  // HeatingSupplyAirFlowRatePerFloorArea
  value = modelObject.heatingSupplyAirFlowRatePerFloorArea();
  if( value ) {
    idfObject.setDouble(Sizing_SystemFields::HeatingSupplyAirFlowRatePerFloorArea,value.get());
  }
  
  // HeatingFractionofAutosizedHeatingSupplyAirFlowRate
  value = modelObject.heatingFractionofAutosizedHeatingSupplyAirFlowRate();
  if( value ) {
    idfObject.setDouble(Sizing_SystemFields::HeatingFractionofAutosizedHeatingSupplyAirFlowRate,value.get());
  }
  
  // HeatingFractionofAutosizedCoolingSupplyAirFlowRate
  value = modelObject.heatingFractionofAutosizedCoolingSupplyAirFlowRate();
  if( value ) {
    idfObject.setDouble(Sizing_SystemFields::HeatingFractionofAutosizedCoolingSupplyAirFlowRate,value.get());
  }

  // HeatingSupplyAirFlowRatePerUnitHeatingCapacity
  value = modelObject.heatingSupplyAirFlowRatePerUnitHeatingCapacity();
  if( value ) {
    idfObject.setDouble(Sizing_SystemFields::HeatingSupplyAirFlowRatePerUnitHeatingCapacity,value.get());
  }

  // CoolingDesignCapacityMethod
  s = modelObject.coolingDesignCapacityMethod();
  if( s ) {
    idfObject.setString(Sizing_SystemFields::CoolingDesignCapacityMethod,s.get());
  }

  // CoolingDesignCapacity
  value = modelObject.coolingDesignCapacity();
  if( modelObject.isCoolingDesignCapacityAutosized() ) {
    idfObject.setString(Sizing_SystemFields::CoolingDesignCapacity,"Autosize");
  } else if( value ) {
    idfObject.setDouble(Sizing_SystemFields::CoolingDesignCapacity,value.get());
  }
  
  
  // CoolingDesignCapacityPerFloorArea
  value = modelObject.coolingDesignCapacityPerFloorArea();
  if( value ) {
    idfObject.setDouble(Sizing_SystemFields::CoolingDesignCapacityPerFloorArea,value.get());
  }

  // FractionofAutosizedCoolingDesignCapacity
  value = modelObject.fractionofAutosizedCoolingDesignCapacity(); 
  if( value ) {
    idfObject.setDouble(Sizing_SystemFields::FractionofAutosizedCoolingDesignCapacity,value.get());
  }

  // HeatingDesignCapacityMethod
  s = modelObject.heatingDesignCapacityMethod();
  if( s ) {
    idfObject.setString(Sizing_SystemFields::HeatingDesignCapacityMethod,s.get());
  }

  // HeatingDesignCapacity
  value = modelObject.heatingDesignCapacity();
  if( modelObject.isHeatingDesignCapacityAutosized() ) {
    idfObject.setString(Sizing_SystemFields::HeatingDesignCapacity,"Autosize");
  } else if( value ) {
    idfObject.setDouble(Sizing_SystemFields::HeatingDesignCapacity,value.get());
  }
  
  // HeatingDesignCapacityPerFloorArea
  value = modelObject.heatingDesignCapacityPerFloorArea();
  if( value ) {
    idfObject.setDouble(Sizing_SystemFields::HeatingDesignCapacityPerFloorArea,value.get());
  }

  // FractionofAutosizedHeatingDesignCapacity
  value = modelObject.fractionofAutosizedHeatingDesignCapacity(); 
  if( value ) {
    idfObject.setDouble(Sizing_SystemFields::FractionofAutosizedHeatingDesignCapacity,value.get());
  }

  // CentralCoolingCapacityControlMethod
  s = modelObject.centralCoolingCapacityControlMethod();
  if( s ) {
    idfObject.setString(Sizing_SystemFields::CentralCoolingCapacityControlMethod,s.get());
  }

  return idfObject;
}

} // energyplus

} // openstudio

