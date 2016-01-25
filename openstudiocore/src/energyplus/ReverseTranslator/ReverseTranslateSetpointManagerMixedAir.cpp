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
#include "../../model/SetpointManagerMixedAir.hpp"
#include "../../model/SetpointManagerMixedAir_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include <utilities/idd/SetpointManager_MixedAir_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

OptionalModelObject ReverseTranslator::translateSetpointManagerMixedAir( const WorkspaceObject & workspaceObject )
{
  if( workspaceObject.iddObject().type() != IddObjectType::SetpointManager_MixedAir )
  {
     LOG(Error, "WorkspaceObject is not IddObjectType: SetpointManager_MixedAir");
     return boost::none;
  }

  bool nodeFound = false;

  if( boost::optional<std::string> setpointNodeName = workspaceObject.getString(SetpointManager_MixedAirFields::SetpointNodeorNodeListName) )
  {
    boost::optional<Node> setpointNode = m_model.getModelObjectByName<Node>(setpointNodeName.get());

    if( setpointNode ) { nodeFound = true; }
  }

  if( ! nodeFound )
  {
    LOG(Error, workspaceObject.briefDescription() << " is not attached to a node in the model");

    return boost::none;
  }

  SetpointManagerMixedAir mo(m_model);

  boost::optional<std::string> s = workspaceObject.getString(SetpointManager_MixedAirFields::Name);
  if( s )
  {
    mo.setName(s.get());
  }

  s = workspaceObject.getString(SetpointManager_MixedAirFields::ReferenceSetpointNodeName);
  if( s )
  {
    boost::optional<Node> node = m_model.getModelObjectByName<Node>(s.get());

    if( node )
    {
      mo.setReferenceSetpointNode(node.get());
    }
  }

  s = workspaceObject.getString(SetpointManager_MixedAirFields::FanInletNodeName);
  if( s )
  {
    boost::optional<Node> node = m_model.getModelObjectByName<Node>(s.get());

    if( node )
    {
      mo.setFanInletNode(node.get());
    }
  }

  s = workspaceObject.getString(SetpointManager_MixedAirFields::FanOutletNodeName);
  if( s )
  {
    boost::optional<Node> node = m_model.getModelObjectByName<Node>(s.get());

    if( node )
    {
      mo.setFanOutletNode(node.get());
    }
  }

  s = workspaceObject.getString(SetpointManager_MixedAirFields::SetpointNodeorNodeListName);
  if( s )
  {
    boost::optional<Node> node = m_model.getModelObjectByName<Node>(s.get());
  
    if( node )
    {
      mo.addToNode(node.get());
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

