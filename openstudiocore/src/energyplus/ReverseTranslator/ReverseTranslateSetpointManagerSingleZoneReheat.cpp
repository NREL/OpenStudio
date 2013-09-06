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

#include <energyplus/ReverseTranslator.hpp>
#include <model/SetpointManagerSingleZoneReheat.hpp>
#include <model/SetpointManagerSingleZoneReheat_Impl.hpp>
#include <model/Node.hpp>
#include <model/Node_Impl.hpp>
#include <model/Space.hpp>
#include <model/Space_Impl.hpp>
#include <model/ThermalZone.hpp>
#include <model/ThermalZone_Impl.hpp>
#include <utilities/idd/SetpointManager_SingleZone_Reheat_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

OptionalModelObject ReverseTranslator::translateSetpointManagerSingleZoneReheat( const WorkspaceObject & workspaceObject )
{
  if( workspaceObject.iddObject().type() != IddObjectType::SetpointManager_SingleZone_Reheat )
  {
     LOG(Error, "WorkspaceObject is not IddObjectType: SetpointManager_SingleZone_Reheat");
     return boost::none;
  }

  bool nodeFound = false;

  if( boost::optional<std::string> setpointNodeName = workspaceObject.getString(SetpointManager_SingleZone_ReheatFields::SetpointNodeorNodeListName) )
  {
    boost::optional<Node> setpointNode = m_model.getModelObjectByName<Node>(setpointNodeName.get());

    if( setpointNode ) { nodeFound = true; }
  }

  if( ! nodeFound )
  {
    LOG(Error, workspaceObject.briefDescription() << " is not attached to a node in the model");

    return boost::none;
  }

  SetpointManagerSingleZoneReheat mo(m_model);

  boost::optional<std::string> s = workspaceObject.getString(SetpointManager_SingleZone_ReheatFields::Name);
  if( s )
  {
    mo.setName(s.get());
  }

  boost::optional<double> value = workspaceObject.getDouble(SetpointManager_SingleZone_ReheatFields::MinimumSupplyAirTemperature);
  if( value )
  {
    mo.setMinimumSupplyAirTemperature(value.get());
  }

  value = workspaceObject.getDouble(SetpointManager_SingleZone_ReheatFields::MaximumSupplyAirTemperature);
  if( value )
  {
    mo.setMaximumSupplyAirTemperature(value.get());
  }

  s = workspaceObject.getString(SetpointManager_SingleZone_ReheatFields::ControlZoneName);
  if( s )
  {
    boost::optional<ModelObject> modelObject;
    boost::optional<Space> space;

    if( boost::optional<WorkspaceObject> _zone = 
          workspaceObject.workspace().getObjectByTypeAndName(IddObjectType::Zone,s.get()) )
    {
      modelObject = translateAndMapWorkspaceObject(_zone.get());
    }

    if( modelObject )
    {
      if( (space = modelObject->optionalCast<Space>()) )
      {
        if( boost::optional<ThermalZone> thermalZone = space->thermalZone() )
        {
          mo.setControlZone(thermalZone.get());
        }
      }
    }
  }

  s = workspaceObject.getString(SetpointManager_SingleZone_ReheatFields::SetpointNodeorNodeListName);
  if( s )
  {
    if( boost::optional<Node> node = m_model.getModelObjectByName<Node>(s.get()) )
    {
      mo.setSetpointNode(node.get());
    }
  }

  if( mo.setpointNode() )
  {
    return mo;
  }
  else
  {
    return boost::none;
  }
}

} // energyplus

} // openstudio

