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
#include "../../model/SetpointManagerScheduled.hpp"
#include "../../model/Node.hpp"
#include "../../model/Schedule.hpp"
#include <utilities/idd/SetpointManager_Scheduled_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateSetpointManagerScheduled( SetpointManagerScheduled & modelObject )
{
  boost::optional<Node> node;
  std::string s;

  IdfObject idfObject(IddObjectType::SetpointManager_Scheduled);

  // Name
  s = modelObject.name().get();
  idfObject.setString(SetpointManager_ScheduledFields::Name,s);

  // ControlVariable
  s = modelObject.controlVariable();
  idfObject.setString(SetpointManager_ScheduledFields::ControlVariable,s);

  // Schedule
  if (modelObject.hasSchedule()){
    Schedule schedule = modelObject.schedule();
    boost::optional<IdfObject> _schedule = translateAndMapModelObject(schedule);
    if( _schedule && _schedule->name() ){
      idfObject.setString(SetpointManager_ScheduledFields::ScheduleName,_schedule->name().get());
    }
  }else{
    LOG(Error, "SetpointManager:Scheduled '" << modelObject.name().get() << "' is missing required schedule");
  }

  // SetpointNodeorNodeListName
  node = modelObject.setpointNode();
  if( node )
  {
    idfObject.setString(SetpointManager_ScheduledFields::SetpointNodeorNodeListName,node->name().get());
  }

  m_idfObjects.push_back(idfObject);

  return idfObject;
}

} // energyplus

} // openstudio

