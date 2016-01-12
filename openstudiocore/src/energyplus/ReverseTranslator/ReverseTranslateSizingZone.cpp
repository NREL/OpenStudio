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

#include "../ReverseTranslator.hpp"
#include "../../model/SizingZone.hpp"
#include "../../model/SizingZone_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"
#include "../../model/DesignSpecificationOutdoorAir.hpp"
#include "../../model/DesignSpecificationOutdoorAir_Impl.hpp"

#include "../../utilities/idf/WorkspaceExtensibleGroup.hpp"
#include <utilities/idd/Sizing_Zone_FieldEnums.hxx>
#include <utilities/idd/DesignSpecification_ZoneAirDistribution_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

#include "../../utilities/core/Assert.hpp"

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

OptionalModelObject ReverseTranslator::translateSizingZone( const WorkspaceObject & workspaceObject )
{
  boost::optional<WorkspaceObject> target = workspaceObject.getTarget(Sizing_ZoneFields::ZoneorZoneListName);

  std::vector<ThermalZone> thermalZones;

  if( target ){

    // just one thermal zone
    boost::optional<ModelObject> mo;
    if (target->iddObject().type() == IddObjectType::Zone){
      mo = translateAndMapWorkspaceObject(target.get());
      if( mo ) {
        if( boost::optional<Space> space = mo->optionalCast<Space>() ) {
          boost::optional<ThermalZone> thermalZone = space->thermalZone();
          if (thermalZone){
            thermalZones.push_back(*thermalZone);
          }
        }
      }
    }else if (target->iddObject().type() == IddObjectType::ZoneList){

      // get all thermal zones in zone list
      for (const IdfExtensibleGroup& idfGroup : target->extensibleGroups()){
        WorkspaceExtensibleGroup workspaceGroup = idfGroup.cast<WorkspaceExtensibleGroup>();
        OptionalWorkspaceObject owo = workspaceGroup.getTarget(0);
        if (owo){
          mo = translateAndMapWorkspaceObject(owo.get());
          if( mo ) {
            if( boost::optional<Space> space = mo->optionalCast<Space>() ) {
              boost::optional<ThermalZone> thermalZone = space->thermalZone();
              if (thermalZone){
                thermalZones.push_back(*thermalZone);
              }
            }
          }
        }
      }
    }
  }

  if(thermalZones.empty())
  {
    LOG(Error, "Error importing object: "
             << workspaceObject.briefDescription()
             << " Can't find associated ThermalZone(s).");

    return boost::none;
  }

  boost::optional<ModelObject> result;
  for (ThermalZone thermalZone : thermalZones){

    // sizing zone is constructed in thermal zone ctor
    openstudio::model::SizingZone sizingZone = thermalZone.sizingZone(); 

    // return first sizing zone
    if (!result){
      result = sizingZone;
    }

    boost::optional<std::string> s;
    boost::optional<double> value;

    // ZoneCoolingDesignSupplyAirTemperature

    value = workspaceObject.getDouble(Sizing_ZoneFields::ZoneCoolingDesignSupplyAirTemperature);
    if( value )
    {
      sizingZone.setZoneCoolingDesignSupplyAirTemperature(value.get());
    }

    // ZoneHeatingDesignSupplyAirTemperature

    value = workspaceObject.getDouble(Sizing_ZoneFields::ZoneHeatingDesignSupplyAirTemperature);
    if( value )
    {
      sizingZone.setZoneHeatingDesignSupplyAirTemperature(value.get());
    }

    // ZoneCoolingDesignSupplyAirHumidityRatio

    value = workspaceObject.getDouble(Sizing_ZoneFields::ZoneHeatingDesignSupplyAirHumidityRatio);
    if( value )
    {
      sizingZone.setZoneHeatingDesignSupplyAirHumidityRatio(value.get());
    }

    // ZoneHeatingDesignSupplyAirHumidityRatio

    value = workspaceObject.getDouble(Sizing_ZoneFields::ZoneHeatingDesignSupplyAirHumidityRatio);
    if( value )
    {
      sizingZone.setZoneHeatingDesignSupplyAirHumidityRatio(value.get());
    }

    // DesignSpecificationOutdoorAirObjectName

    target = workspaceObject.getTarget(Sizing_ZoneFields::DesignSpecificationOutdoorAirObjectName);
    if (target){
      OptionalModelObject mo = translateDesignSpecificationOutdoorAir(*target);
      if (mo){
        if (mo->optionalCast<DesignSpecificationOutdoorAir>()){
          std::vector<Space> spaces = thermalZone.spaces();
          OS_ASSERT(spaces.size() == 1);
          spaces[0].setDesignSpecificationOutdoorAir(mo->cast<DesignSpecificationOutdoorAir>());
        }
      }
    }

    // ZoneHeatingSizingFactor

    value = workspaceObject.getDouble(Sizing_ZoneFields::ZoneHeatingSizingFactor);
    if( value )
    {
      sizingZone.setZoneHeatingSizingFactor(value.get());
    }

    // ZoneCoolingSizingFactor

    value = workspaceObject.getDouble(Sizing_ZoneFields::ZoneCoolingSizingFactor);
    if( value )
    {
      sizingZone.setZoneCoolingSizingFactor(value.get());
    }

    // CoolingDesignAirFlowMethod
    
    s = workspaceObject.getString(Sizing_ZoneFields::CoolingDesignAirFlowMethod);
    if( s )
    {
      sizingZone.setCoolingDesignAirFlowMethod(s.get());
    }

    // CoolingDesignAirFlowRate

    value = workspaceObject.getDouble(Sizing_ZoneFields::CoolingDesignAirFlowRate);
    if( value )
    {
      sizingZone.setCoolingDesignAirFlowRate(value.get());
    }

    // CoolingMinimumAirFlowperZoneFloorArea

    value = workspaceObject.getDouble(Sizing_ZoneFields::CoolingMinimumAirFlowperZoneFloorArea);
    if( value )
    {
      sizingZone.setCoolingMinimumAirFlowperZoneFloorArea(value.get());
    }

    // CoolingMinimumAirFlow
    
    value = workspaceObject.getDouble(Sizing_ZoneFields::CoolingMinimumAirFlow);
    if( value )
    {
      sizingZone.setCoolingMinimumAirFlow(value.get());
    }

    // CoolingMinimumAirFlowFraction
    
    value = workspaceObject.getDouble(Sizing_ZoneFields::CoolingMinimumAirFlowFraction);
    if( value )
    {
      sizingZone.setCoolingMinimumAirFlowFraction(value.get());
    }

    // HeatingDesignAirFlowMethod
    
    s = workspaceObject.getString(Sizing_ZoneFields::HeatingDesignAirFlowMethod);
    if( s )
    {
      sizingZone.setHeatingDesignAirFlowMethod(s.get());
    }

    // HeatingDesignAirFlowRate

    value = workspaceObject.getDouble(Sizing_ZoneFields::HeatingDesignAirFlowRate);
    if( value )
    {
      sizingZone.setHeatingDesignAirFlowRate(value.get());
    }

    // HeatingMaximumAirFlowperZoneFloorArea

    value = workspaceObject.getDouble(Sizing_ZoneFields::HeatingMaximumAirFlowperZoneFloorArea);
    if( value )
    {
      sizingZone.setHeatingMaximumAirFlowperZoneFloorArea(value.get());
    }

    // HeatingMaximumAirFlow
    
    value = workspaceObject.getDouble(Sizing_ZoneFields::HeatingMaximumAirFlow);
    if( value )
    {
      sizingZone.setHeatingMaximumAirFlow(value.get());
    }

    // HeatingMaximumAirFlowFraction
    
    value = workspaceObject.getDouble(Sizing_ZoneFields::HeatingMaximumAirFlowFraction);
    if( value )
    {
      sizingZone.setHeatingMaximumAirFlowFraction(value.get());
    }

    //DesignSpecification_ZoneAirDistribution

    boost::optional<WorkspaceObject> _designSpecification 
        = workspaceObject.getTarget(Sizing_ZoneFields::DesignSpecificationZoneAirDistributionObjectName);

    if( _designSpecification )
    {
      // ZoneAirDistributionEffectivenessinCoolingMode

      value = _designSpecification->getDouble(
                DesignSpecification_ZoneAirDistributionFields::ZoneAirDistributionEffectivenessinCoolingMode);
      if( value )
      {
        sizingZone.setDesignZoneAirDistributionEffectivenessinCoolingMode(value.get());
      }

      // ZoneAirDistributionEffectivenessinHeatingMode

      value = _designSpecification->getDouble(
                DesignSpecification_ZoneAirDistributionFields::ZoneAirDistributionEffectivenessinHeatingMode);
      if( value )
      {
        sizingZone.setDesignZoneAirDistributionEffectivenessinHeatingMode(value.get());
      }
    }

  }

  return result;
}

} // energyplus

} // openstudio

