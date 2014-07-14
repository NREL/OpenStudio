/**********************************************************************
 *  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
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

#include "../ReverseTranslator.hpp"
#include "../../model/SizingSystem.hpp"
#include "../../model/SizingSystem_Impl.hpp"
#include "../../model/AirLoopHVAC.hpp"
#include "../../model/AirLoopHVAC_Impl.hpp"
#include <utilities/idd/Sizing_System_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

OptionalModelObject ReverseTranslator::translateSizingSystem( const WorkspaceObject & workspaceObject )
{
  boost::optional<WorkspaceObject> _airLoopHVAC = workspaceObject.getTarget(Sizing_SystemFields::AirLoopName);

  boost::optional<AirLoopHVAC> airLoopHVAC;

  if( _airLoopHVAC )
  {
    boost::optional<ModelObject> mo = translateAndMapWorkspaceObject(_airLoopHVAC.get());

    if( mo )
    {
      airLoopHVAC = mo->optionalCast<AirLoopHVAC>();
    }
  }

  if( ! airLoopHVAC )
  {
    LOG(Error, "Error importing object: "
             << workspaceObject.briefDescription()
             << " Can't find associated AirLoopHVAC.");

    return boost::none;
  }

  openstudio::model::SizingSystem sizingSystem = airLoopHVAC->sizingSystem(); 

  boost::optional<std::string> s;
  boost::optional<double> value;

  // TypeofLoadtoSizeOn

  s = workspaceObject.getString(Sizing_SystemFields::TypeofLoadtoSizeOn);
  if( s )
  {
    sizingSystem.setTypeofLoadtoSizeOn(s.get());
  }

  // DesignOutdoorAirFlowRate

  s = workspaceObject.getString(Sizing_SystemFields::DesignOutdoorAirFlowRate);
  value = workspaceObject.getDouble(Sizing_SystemFields::DesignOutdoorAirFlowRate);
  if( value )
  {
    sizingSystem.setDesignOutdoorAirFlowRate(value.get());
  }
  else if( s && istringEqual(s.get(),"Autosize") )
  {
    sizingSystem.autosizeDesignOutdoorAirFlowRate();
  }

  // MinimumSystemAirFlowRatio

  value = workspaceObject.getDouble(Sizing_SystemFields::MinimumSystemAirFlowRatio);
  if( value )
  {
    sizingSystem.setMinimumSystemAirFlowRatio(value.get());
  }

  // PreheatDesignTemperature

  value = workspaceObject.getDouble(Sizing_SystemFields::PreheatDesignTemperature);
  if( value )
  {
    sizingSystem.setPreheatDesignTemperature(value.get());
  }

  // PreheatDesignHumidityRatio

  value = workspaceObject.getDouble(Sizing_SystemFields::PreheatDesignHumidityRatio);
  if( value )
  {
    sizingSystem.setPreheatDesignHumidityRatio(value.get());
  }

  // PrecoolDesignTemperature

  value = workspaceObject.getDouble(Sizing_SystemFields::PrecoolDesignTemperature);
  if( value )
  {
    sizingSystem.setPrecoolDesignTemperature(value.get());
  }

  // PrecoolDesignHumidityRatio

  value = workspaceObject.getDouble(Sizing_SystemFields::PrecoolDesignHumidityRatio);
  if( value )
  {
    sizingSystem.setPrecoolDesignHumidityRatio(value.get());
  }

  // CentralCoolingDesignSupplyAirTemperature

  value = workspaceObject.getDouble(Sizing_SystemFields::CentralCoolingDesignSupplyAirTemperature);
  if( value )
  {
    sizingSystem.setCentralCoolingDesignSupplyAirTemperature(value.get());
  }

  // CentralHeatingDesignSupplyAirTemperature

  value = workspaceObject.getDouble(Sizing_SystemFields::CentralHeatingDesignSupplyAirTemperature);
  if( value )
  {
    sizingSystem.setCentralHeatingDesignSupplyAirTemperature(value.get());
  }

  // SizingOption

  s = workspaceObject.getString(Sizing_SystemFields::SizingOption);
  if( s )
  {
    sizingSystem.setSizingOption(s.get());
  }

  // AllOutdoorAirinCooling

  s = workspaceObject.getString(Sizing_SystemFields::AllOutdoorAirinCooling);
  if( s && istringEqual(s.get(),"Yes") )
  {
    sizingSystem.setAllOutdoorAirinCooling(true);
  }
  else if( s && istringEqual(s.get(),"No") )
  {
    sizingSystem.setAllOutdoorAirinCooling(false);
  }

  // AllOutdoorAirinHeating

  s = workspaceObject.getString(Sizing_SystemFields::AllOutdoorAirinHeating);
  if( s && istringEqual(s.get(),"Yes")  )
  {
    sizingSystem.setAllOutdoorAirinHeating(true);
  }
  else if( s && istringEqual(s.get(),"No") )
  {
    sizingSystem.setAllOutdoorAirinHeating(false);
  }

  // CentralCoolingDesignSupplyAirHumidityRatio

  value = workspaceObject.getDouble(Sizing_SystemFields::CentralCoolingDesignSupplyAirHumidityRatio);
  if( value )
  {
    sizingSystem.setCentralCoolingDesignSupplyAirHumidityRatio(value.get());
  }

  // CentralHeatingDesignSupplyAirHumidityRatio

  value = workspaceObject.getDouble(Sizing_SystemFields::CentralHeatingDesignSupplyAirHumidityRatio);
  if( value )
  {
    sizingSystem.setCentralHeatingDesignSupplyAirHumidityRatio(value.get());
  }

  // CoolingDesignAirFlowMethod

  s = workspaceObject.getString(Sizing_SystemFields::CoolingDesignAirFlowMethod);
  if( s )
  {
    sizingSystem.setCoolingDesignAirFlowMethod(s.get());
  }

  // CoolingDesignAirFlowRate

  value = workspaceObject.getDouble(Sizing_SystemFields::CoolingDesignAirFlowRate);
  if( value )
  {
    sizingSystem.setCoolingDesignAirFlowRate(value.get());
  }

  // HeatingDesignAirFlowMethod

  s = workspaceObject.getString(Sizing_SystemFields::HeatingDesignAirFlowMethod);
  if( s )
  {
    sizingSystem.setHeatingDesignAirFlowMethod(s.get());
  }

  // HeatingDesignAirFlowRate

  value = workspaceObject.getDouble(Sizing_SystemFields::HeatingDesignAirFlowRate);
  if( value )
  {
    sizingSystem.setHeatingDesignAirFlowRate(value.get());
  }

  // SystemOutdoorAirMethod

  s = workspaceObject.getString(Sizing_SystemFields::SystemOutdoorAirMethod);
  if( s )
  {
    sizingSystem.setSystemOutdoorAirMethod(s.get());
  }

  return sizingSystem;
}

} // energyplus

} // openstudio

