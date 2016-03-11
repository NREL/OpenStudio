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
#include "../../model/SetpointManagerFollowSystemNodeTemperature.hpp"
#include "../../model/Node.hpp"
#include <utilities/idd/SetpointManager_FollowSystemNodeTemperature_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateSetpointManagerFollowSystemNodeTemperature( 
                              SetpointManagerFollowSystemNodeTemperature & modelObject )
{
  boost::optional<Node> node;
  boost::optional<std::string> s;
  boost::optional<double> d;

  // Name
  IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::SetpointManager_FollowSystemNodeTemperature, modelObject);

  // ControlVariable
  s = modelObject.controlVariable();
  if( s )
  {
    idfObject.setString(SetpointManager_FollowSystemNodeTemperatureFields::ControlVariable,s.get());
  }

  // ReferenceNodeName
  node = modelObject.referenceNode();
  if( node )
  {
    idfObject.setString(SetpointManager_FollowSystemNodeTemperatureFields::ReferenceNodeName,node->name().get());
  }

  // ReferenceTemperatureType
  s = modelObject.referenceTemperatureType();
  if( s )
  {
    idfObject.setString(SetpointManager_FollowSystemNodeTemperatureFields::ReferenceTemperatureType,s.get());
  }

  // OffsetTemperatureDifference
  d = modelObject.offsetTemperatureDifference();
  if( d )
  {
    idfObject.setDouble(SetpointManager_FollowSystemNodeTemperatureFields::OffsetTemperatureDifference,d.get());
  }

  // MaximumLimitSetpointTemperature
  d = modelObject.maximumLimitSetpointTemperature();
  if( d )
  {
    idfObject.setDouble(SetpointManager_FollowSystemNodeTemperatureFields::MaximumLimitSetpointTemperature,d.get());
  }
  
  // MinimumLimitSetpointTemperature
  d = modelObject.minimumLimitSetpointTemperature();
  if( d )
  {
    idfObject.setDouble(SetpointManager_FollowSystemNodeTemperatureFields::MinimumLimitSetpointTemperature,d.get());
  }

  // SetpointNodeorNodeListName
  node = modelObject.setpointNode();
  if( node )
  {
    idfObject.setString(SetpointManager_FollowSystemNodeTemperatureFields::SetpointNodeorNodeListName,node->name().get());
  }

  return idfObject;
}

} // energyplus

} // openstudio

