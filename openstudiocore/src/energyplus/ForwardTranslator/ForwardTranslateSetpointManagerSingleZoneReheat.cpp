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
#include <model/SetpointManagerSingleZoneReheat.hpp>
#include <model/ThermalZone.hpp>
#include <model/ThermalZone_Impl.hpp>
#include <model/Node.hpp>
#include <model/Node_Impl.hpp>
#include <model/PortList.hpp>
#include <model/PortList_Impl.hpp>
#include <utilities/idd/SetpointManager_SingleZone_Reheat_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateSetpointManagerSingleZoneReheat( SetpointManagerSingleZoneReheat & modelObject )
{
  std::string s;
  double n;
  boost::optional<Node> node;

  IdfObject idfObject(IddObjectType::SetpointManager_SingleZone_Reheat);

  m_idfObjects.push_back(idfObject);

  // Name
  s = modelObject.name().get();
  idfObject.setString(SetpointManager_SingleZone_ReheatFields::Name,s);

  // ControlVariable
  idfObject.setString(SetpointManager_SingleZone_ReheatFields::ControlVariable,"Temperature");

  // MinimumSupplyAirTemperature
  n = modelObject.minimumSupplyAirTemperature();
  idfObject.setDouble(SetpointManager_SingleZone_ReheatFields::MinimumSupplyAirTemperature,n);

  // MaximumSupplyAirTemperature
  n = modelObject.maximumSupplyAirTemperature();
  idfObject.setDouble(SetpointManager_SingleZone_ReheatFields::MaximumSupplyAirTemperature,n);

  // ControlZoneName
  boost::optional<ThermalZone> thermalZone = modelObject.controlZone();
  if( thermalZone )
  {
    idfObject.setString(SetpointManager_SingleZone_ReheatFields::ControlZoneName,thermalZone->name().get());
  }

  // ZoneNodeName
  if( thermalZone )
  {
    node = thermalZone->zoneAirNode();
    idfObject.setString(SetpointManager_SingleZone_ReheatFields::ZoneNodeName,node->name().get());
  } 

  // ZoneInletNodeName
  if( thermalZone )
  {
    if( boost::optional<ModelObject> mo = thermalZone->inletPortList().airLoopHVACModelObject() )
    {
      if( (node = mo->optionalCast<Node>()) )
      {
        idfObject.setString(SetpointManager_SingleZone_ReheatFields::ZoneInletNodeName,node->name().get());
      }
    }
  }

  // SetpointNodeorNodeListName
  node = modelObject.setpointNode();
  if( node )
  {
    idfObject.setString(SetpointManager_SingleZone_ReheatFields::SetpointNodeorNodeListName,node->name().get());
  }
  
  return idfObject;
}

} // energyplus

} // openstudio

