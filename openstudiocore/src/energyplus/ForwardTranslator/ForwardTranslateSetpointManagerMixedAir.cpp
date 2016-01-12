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
#include "../../model/SetpointManagerMixedAir.hpp"
#include "../../model/Node.hpp"
#include <utilities/idd/SetpointManager_MixedAir_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateSetpointManagerMixedAir( SetpointManagerMixedAir & modelObject )
{
  boost::optional<Node> node;
  std::string s;

  IdfObject idfObject(IddObjectType::SetpointManager_MixedAir);

  // Name
  s = modelObject.name().get();
  idfObject.setString(SetpointManager_MixedAirFields::Name,s);

  // ControlVariable
  idfObject.setString(SetpointManager_MixedAirFields::ControlVariable,"Temperature");

  // ReferenceSetpointNodeName
  node = modelObject.referenceSetpointNode();
  if( node )
  {
    idfObject.setString(SetpointManager_MixedAirFields::ReferenceSetpointNodeName,node->name().get());
  }

  // FanInletNodeName
  node = modelObject.fanInletNode();
  if( node )
  {
    idfObject.setString(SetpointManager_MixedAirFields::FanInletNodeName,node->name().get());
  }

  // FanOutletNodeName
  node = modelObject.fanOutletNode();
  if( node )
  {
    idfObject.setString(SetpointManager_MixedAirFields::FanOutletNodeName,node->name().get());
  }

  // SetpointNodeorNodeListName
  node = modelObject.setpointNode();
  if( node )
  {
    idfObject.setString(SetpointManager_MixedAirFields::SetpointNodeorNodeListName,node->name().get());
  }

  m_idfObjects.push_back(idfObject);

  return idfObject;
}

} // energyplus

} // openstudio

