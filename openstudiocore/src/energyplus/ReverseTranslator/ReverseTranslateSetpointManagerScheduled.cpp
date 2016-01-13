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
#include "../../model/SetpointManagerScheduled.hpp"
#include "../../model/SetpointManagerScheduled_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include <utilities/idd/SetpointManager_Scheduled_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

OptionalModelObject ReverseTranslator::translateSetpointManagerScheduled( const WorkspaceObject & workspaceObject )
{
  if( workspaceObject.iddObject().type() != IddObjectType::SetpointManager_Scheduled )
  {
     LOG(Error, "WorkspaceObject is not IddObjectType: SetpointManager_Scheduled");
     return boost::none;
  }

  boost::optional<WorkspaceObject> wo = workspaceObject.getTarget(SetpointManager_ScheduledFields::ScheduleName);
  boost::optional<Schedule> schedule;

  if( wo )
  {
    boost::optional<ModelObject> mo = translateAndMapWorkspaceObject(wo.get());
    if( mo )
    {
      if( ! (schedule = mo->optionalCast<Schedule>()) )
      {
        LOG(Error, workspaceObject.briefDescription() << " does not have an associated schedule");

        return boost::none;
      }
    }
  }

  bool nodeFound = false;

  if( boost::optional<std::string> setpointNodeName = workspaceObject.getString(SetpointManager_ScheduledFields::SetpointNodeorNodeListName) )
  {
    boost::optional<Node> setpointNode = m_model.getModelObjectByName<Node>(setpointNodeName.get());

    if( setpointNode ) { nodeFound = true; }
  }

  if( ! nodeFound )
  {
    LOG(Error, workspaceObject.briefDescription() << " is not attached to a node in the model");

    return boost::none;
  }

  if( schedule )
  {
    SetpointManagerScheduled mo(m_model,schedule.get());

    // Name
    boost::optional<std::string> s = workspaceObject.getString(SetpointManager_ScheduledFields::Name);
    if( s )
    {
      mo.setName(s.get());
    }

    // Setpoint Node
    s = workspaceObject.getString(SetpointManager_ScheduledFields::SetpointNodeorNodeListName);
    if( s )
    {
      boost::optional<Node> node = m_model.getModelObjectByName<Node>(s.get());
    
      if( node )
      {
        mo.addToNode(node.get());
      }
    }

    // Control Variable
    s = workspaceObject.getString(SetpointManager_ScheduledFields::ControlVariable);
    if( s )
    {
      mo.setControlVariable(s.get());
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
  else
  {
    return boost::none;
  }
}

} // energyplus

} // openstudio

