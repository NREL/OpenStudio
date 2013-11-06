/**********************************************************************
 *  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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

#include <energyplus/ForwardTranslator.hpp>
#include <model/Model.hpp>
#include <model/SizingSystem.hpp>
#include <model/SizingSystem_Impl.hpp>
#include <model/AirLoopHVAC.hpp>
#include <model/AirLoopHVAC_Impl.hpp>
#include <utilities/core/Logger.hpp>
#include <utilities/core/Assert.hpp>
#include <utilities/idd/Sizing_System_FieldEnums.hxx>
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
    idfObject.setDouble(Sizing_SystemFields::MinimumSystemAirFlowRatio,value.get());
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
    idfObject.setString(Sizing_SystemFields::SizingOption,s.get());
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
    idfObject.setString(Sizing_SystemFields::CoolingDesignAirFlowMethod,s.get());
  }

  // CoolingDesignAirFlowRate

  value = modelObject.coolingDesignAirFlowRate();
  if( value )
  {
    idfObject.setDouble(Sizing_SystemFields::CoolingDesignAirFlowRate,value.get());
  }

  // HeatingDesignAirFlowMethod

  s = modelObject.heatingDesignAirFlowMethod();
  if( s )
  {
    idfObject.setString(Sizing_SystemFields::HeatingDesignAirFlowMethod,s.get());
  }

  // HeatingDesignAirFlowRate

  value = modelObject.heatingDesignAirFlowRate();
  if( value )
  {
    idfObject.setDouble(Sizing_SystemFields::HeatingDesignAirFlowRate,value.get());
  }

  // SystemOutdoorAirMethod
  
  s = modelObject.systemOutdoorAirMethod();
  if( s )
  {
    idfObject.setString(Sizing_SystemFields::SystemOutdoorAirMethod,s.get());
  }

  return idfObject;
}

} // energyplus

} // openstudio

