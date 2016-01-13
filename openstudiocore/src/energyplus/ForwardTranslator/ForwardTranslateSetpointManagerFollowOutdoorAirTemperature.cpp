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
#include "../../model/SetpointManagerFollowOutdoorAirTemperature.hpp"
#include "../../model/Node.hpp"
#include <utilities/idd/SetpointManager_FollowOutdoorAirTemperature_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateSetpointManagerFollowOutdoorAirTemperature( 
                              SetpointManagerFollowOutdoorAirTemperature & modelObject )
{
  boost::optional<Node> node;
  boost::optional<std::string> s;
  boost::optional<double> d;

  IdfObject idfObject(IddObjectType::SetpointManager_FollowOutdoorAirTemperature);

  // Name
  s = modelObject.name();
  if( s )
  {
    idfObject.setString(SetpointManager_FollowOutdoorAirTemperatureFields::Name,s.get());
  }

  // ControlVariable
  s = modelObject.controlVariable();
  if( s )
  {
    idfObject.setString(SetpointManager_FollowOutdoorAirTemperatureFields::ControlVariable,s.get());
  }

  // SetpointNodeorNodeListName
  node = modelObject.setpointNode();
  if( node )
  {
    idfObject.setString(SetpointManager_FollowOutdoorAirTemperatureFields::SetpointNodeorNodeListName,node->name().get());
  }

  // ReferenceTemperatureType
  s = modelObject.referenceTemperatureType();
  if( s )
  {
    idfObject.setString(SetpointManager_FollowOutdoorAirTemperatureFields::ReferenceTemperatureType,s.get());
  }

  // OffsetTemperatureDifference
  d = modelObject.offsetTemperatureDifference();
  if( d )
  {
    idfObject.setDouble(SetpointManager_FollowOutdoorAirTemperatureFields::OffsetTemperatureDifference,d.get());
  }

  // MaximumSetpointTemperature
  d = modelObject.maximumSetpointTemperature();
  if( d )
  {
    idfObject.setDouble(SetpointManager_FollowOutdoorAirTemperatureFields::MaximumSetpointTemperature,d.get());
  }
  
  // MinimumSetpointTemperature
  d = modelObject.minimumSetpointTemperature();
  if( d )
  {
    idfObject.setDouble(SetpointManager_FollowOutdoorAirTemperatureFields::MinimumSetpointTemperature,d.get());
  }

  m_idfObjects.push_back(idfObject);

  return idfObject;
}

} // energyplus

} // openstudio

