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
#include "../../model/SetpointManagerFollowGroundTemperature.hpp"
#include "../../model/Node.hpp"
#include <utilities/idd/SetpointManager_FollowGroundTemperature_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateSetpointManagerFollowGroundTemperature( 
                              SetpointManagerFollowGroundTemperature & modelObject )
{
  boost::optional<Node> node;
  boost::optional<std::string> s;
  boost::optional<double> d;

  // Name
  IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::SetpointManager_FollowGroundTemperature, modelObject);

  // ControlVariable
  s = modelObject.controlVariable();
  if( s )
  {
    idfObject.setString(SetpointManager_FollowGroundTemperatureFields::ControlVariable,s.get());
  }

  // ReferenceGroundTemperatureObjectType
  idfObject.setString(SetpointManager_FollowGroundTemperatureFields::ReferenceGroundTemperatureObjectType,"Site:GroundTemperature:BuildingSurface");

  // OffsetTemperatureDifference
  d = modelObject.offsetTemperatureDifference();
  if( d )
  {
    idfObject.setDouble(SetpointManager_FollowGroundTemperatureFields::OffsetTemperatureDifference,d.get());
  }

  // MaximumSetpointTemperature
  d = modelObject.maximumSetpointTemperature();
  if( d )
  {
    idfObject.setDouble(SetpointManager_FollowGroundTemperatureFields::MaximumSetpointTemperature,d.get());
  }
  
  // MinimumSetpointTemperature
  d = modelObject.minimumSetpointTemperature();
  if( d )
  {
    idfObject.setDouble(SetpointManager_FollowGroundTemperatureFields::MinimumSetpointTemperature,d.get());
  }

  // SetpointNodeorNodeListName
  node = modelObject.setpointNode();
  if( node )
  {
    idfObject.setString(SetpointManager_FollowGroundTemperatureFields::SetpointNodeorNodeListName,node->name().get());
  }

  return idfObject;
}

} // energyplus

} // openstudio

