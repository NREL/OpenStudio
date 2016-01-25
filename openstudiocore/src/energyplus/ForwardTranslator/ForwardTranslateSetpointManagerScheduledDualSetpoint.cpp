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
#include "../../model/SetpointManagerScheduledDualSetpoint.hpp"
#include "../../model/Node.hpp"
#include "../../model/Schedule.hpp"
#include <utilities/idd/SetpointManager_Scheduled_DualSetpoint_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateSetpointManagerScheduledDualSetpoint( SetpointManagerScheduledDualSetpoint & modelObject )
{
  // Name
  IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::SetpointManager_Scheduled_DualSetpoint, modelObject);

  // ControlVariable
  std::string s = modelObject.controlVariable();
  idfObject.setString(SetpointManager_Scheduled_DualSetpointFields::ControlVariable,s);

  // High Setpoint Schedule Name
  if( boost::optional<Schedule> highSetpointSchedule = modelObject.highSetpointSchedule() )
  {
    boost::optional<IdfObject> _highSetpointSchedule = translateAndMapModelObject(highSetpointSchedule.get());
    if( _highSetpointSchedule && _highSetpointSchedule->name() )
    {
      idfObject.setString(SetpointManager_Scheduled_DualSetpointFields::HighSetpointScheduleName,_highSetpointSchedule->name().get());
    }
  } else {
    LOG(Error, "SetpointManager:Scheduled:DualSetpoint '" << modelObject.name().get() << "' is missing required high setpoint schedule");
  }

  // Low Setpoint Schedule Name
  if( boost::optional<Schedule> lowSetpointSchedule = modelObject.lowSetpointSchedule() )
  {
    boost::optional<IdfObject> _lowSetpointSchedule = translateAndMapModelObject(lowSetpointSchedule.get());
    if( _lowSetpointSchedule && _lowSetpointSchedule->name() )
    {
      idfObject.setString(SetpointManager_Scheduled_DualSetpointFields::LowSetpointScheduleName,_lowSetpointSchedule->name().get());
    }
  } else {
    LOG(Error, "SetpointManager:Scheduled:DualSetpoint '" << modelObject.name().get() << "' is missing required low setpoint schedule");
  }

  // SetpointNodeorNodeListName
  if( boost::optional<Node> node = modelObject.setpointNode() )
  {
    idfObject.setString(SetpointManager_Scheduled_DualSetpointFields::SetpointNodeorNodeListName,node->name().get());
  }

  return idfObject;
}

} // energyplus

} // openstudio

