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
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"
#include "../../model/ThermostatSetpointDualSetpoint.hpp"
#include "../../model/ThermostatSetpointDualSetpoint_Impl.hpp"
#include <utilities/idd/Zone_FieldEnums.hxx>
#include <utilities/idd/ZoneControl_Thermostat_FieldEnums.hxx>
#include <utilities/idd/ZoneList_FieldEnums.hxx>
#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include <utilities/idd/ZoneHVAC_EquipmentConnections_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

OptionalModelObject ReverseTranslator::translateZone( const WorkspaceObject & workspaceObject )
{
 if( workspaceObject.iddObject().type() != IddObjectType::Zone ){
    LOG(Error, "WorkspaceObject is not IddObjectType: Zone");
    return boost::none;
  }

  // this function creates a space and a thermal zone, it returns the space.  If you want the 
  // thermal zone you can reliably dereference the result of space.thermalZone().

  openstudio::model::ThermalZone thermalZone( m_model );

  openstudio::model::Space space( m_model );
  space.setThermalZone(thermalZone);

  boost::optional<std::string> idfZoneName;

  OptionalString s = workspaceObject.name();
  if(s){
    space.setName(*s);
    thermalZone.setName(*s + " Thermal Zone");
    idfZoneName = *s;
  }

  OptionalDouble d = workspaceObject.getDouble(ZoneFields::DirectionofRelativeNorth);
  if(d){
    space.setDirectionofRelativeNorth(*d);
  }

  d=workspaceObject.getDouble(ZoneFields::XOrigin);
  if(d){
    space.setXOrigin(*d);
  }

  d=workspaceObject.getDouble(ZoneFields::YOrigin);
  if(d){
    space.setYOrigin(*d);
  }

  d=workspaceObject.getDouble(ZoneFields::ZOrigin);
  if(d){
    space.setZOrigin(*d);
  }

  OptionalInt i = workspaceObject.getInt(ZoneFields::Type);
  if(i){
    // no-op
  }

  i = workspaceObject.getInt(ZoneFields::Multiplier);
  if(i){
    thermalZone.setMultiplier(*i);
  }

  d = workspaceObject.getDouble(ZoneFields::CeilingHeight);
  if(d){
    thermalZone.setCeilingHeight(*d);
  }

  d=workspaceObject.getDouble(ZoneFields::Volume);
  if(d){
    thermalZone.setVolume(*d);
  }

  s = workspaceObject.getString(ZoneFields::ZoneInsideConvectionAlgorithm);
  if(s){
    thermalZone.setZoneInsideConvectionAlgorithm(*s);
  }

  s = workspaceObject.getString(ZoneFields::ZoneOutsideConvectionAlgorithm);
  if(s){
    thermalZone.setZoneOutsideConvectionAlgorithm(*s);
  }

  s = workspaceObject.getString(ZoneFields::PartofTotalFloorArea);
  if(s){
    if(istringEqual("Yes",*s))
    {
      space.setPartofTotalFloorArea(true);
    }
    else
    {
      space.setPartofTotalFloorArea(false);
    }
  }

  // Thermostat

  // If the zone in the idf file does not have a name, there is no use in even trying to find a thermostat
  if( idfZoneName )
  {
    Workspace workspace = workspaceObject.workspace();
    
    std::vector<WorkspaceObject> _zoneControlThermostats;
    _zoneControlThermostats = workspace.getObjectsByType(IddObjectType::ZoneControl_Thermostat);

    for( const auto & _zoneControlThermostat : _zoneControlThermostats )
    {
      if( boost::optional<std::string> zoneName = _zoneControlThermostat.getString( ZoneControl_ThermostatFields::ZoneorZoneListName ) )
      {
        bool zoneControlThermostatfound = false;

        if( zoneName.get() == idfZoneName ) 
        { 
          zoneControlThermostatfound = true; 
        }
        else if( boost::optional<WorkspaceObject> _zoneList = workspace.getObjectByTypeAndName(IddObjectType::ZoneList,zoneName.get()) )
        {
          std::vector<IdfExtensibleGroup> zoneListGroup = _zoneList->extensibleGroups();

          for( const auto & zoneListElem : zoneListGroup )
          {
            boost::optional<std::string> zoneListZoneName = zoneListElem.getString(ZoneListExtensibleFields::ZoneName);
            if( zoneListZoneName )
            {
              if( zoneListZoneName.get() == idfZoneName ) { zoneControlThermostatfound = true; }
              break;
            }
          }
        }
        if( zoneControlThermostatfound )
        {
          std::vector<IdfExtensibleGroup> extensibleGroups = _zoneControlThermostat.extensibleGroups();
          for( const auto & extensibleGroup : extensibleGroups )
          {
            boost::optional<std::string> thermostatType = extensibleGroup.getString(ZoneControl_ThermostatExtensibleFields::ControlObjectType);
            boost::optional<std::string> thermostatName = extensibleGroup.getString(ZoneControl_ThermostatExtensibleFields::ControlName);

            if( thermostatName && thermostatType )
            {
              boost::optional<WorkspaceObject> _thermostat
               = workspace.getObjectByTypeAndName(IddObjectType(thermostatType.get()),thermostatName.get());
              
              if( _thermostat )
              {
                boost::optional<ModelObject> thermostat = translateAndMapWorkspaceObject(_thermostat.get());
                if( thermostat )
                {
                  if( boost::optional<ThermostatSetpointDualSetpoint> thermostatSetpointDualSetpoint
                      = thermostat->optionalCast<ThermostatSetpointDualSetpoint>() )
                  {
                    thermalZone.setThermostatSetpointDualSetpoint(thermostatSetpointDualSetpoint.get());
                  }
                }
              }
            }
          }
          break;
        }
      }
    }
  }

  // Zone Equipment
/*
  if( idfZoneName )
  {
    std::vector<WorkspaceObject> zoneHVACEquipmentConnections;
    zoneHVACEquipmentConnections = workspaceObject.workspace().getObjectsByType(IddObjectType::ZoneHVAC_EquipmentConnections);

    for( std::vector<WorkspaceObject>::iterator it = zoneHVACEquipmentConnections.begin();
         it != zoneHVACEquipmentConnections.end();
         it++ )
    {
      s = it->getString(ZoneHVAC_EquipmentConnectionsFields::ZoneName);

      if( s && istringEqual(s.get(),idfZoneName.get()) )
      {
        boost::optional<WorkspaceObject> _zoneEquipmentList = it->getTarget(ZoneHVAC_EquipmentConnectionsFields::ZoneConditioningEquipmentListName);

        if( _zoneEquipmentList )
        {
          translateAndMapWorkspaceObject(_zoneEquipmentList.get());
        }

        break;
      }
    }
  }
*/

  return space;
}

} // energyplus

} // openstudio

